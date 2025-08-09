from __future__ import annotations

import os
import sys
from dataclasses import dataclass
import io
from pathlib import Path
from typing import Optional

from PySide6 import QtCore, QtGui, QtWidgets
from PIL import Image

from .gsc_format import GscArchive, BuildItem, build_gsc
from .gp_parser import parse_gp_summary
from .gp_render import render_gp_frame
from .gp_viewer import GpViewer, GpViewerWidget
from .smp_view import parse_lnk, LnkScene, LnkView


class FileTreeItem(QtGui.QStandardItem):
    def __init__(self, name: str, full_name: Optional[str] = None):
        super().__init__(name)
        self.full_name = full_name  # UPPERCASE name inside archive


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("GSC Archive Manager")
        self.resize(1200, 700)

        self.archive: Optional[GscArchive] = None
        self.model = QtGui.QStandardItemModel()
        self.model.setHorizontalHeaderLabels(["Name", "Size", "Flags"])
        # New files not present in the original archive (collected from folder)
        self._new_files: dict[str, bytes] = {}

        self.tree = QtWidgets.QTreeView()
        self.tree.setModel(self.model)
        self.tree.setSelectionMode(QtWidgets.QAbstractItemView.ExtendedSelection)
        self.tree.setContextMenuPolicy(QtCore.Qt.CustomContextMenu)
        self.tree.customContextMenuRequested.connect(self._context_menu)
        self.tree.doubleClicked.connect(self._on_double_click)

        self.preview = QtWidgets.QLabel(alignment=QtCore.Qt.AlignCenter)
        self.preview.setBackgroundRole(QtGui.QPalette.Base)
        self.preview.setSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        self.preview.setText("Open an archive to preview files")

        # Non-scaled central layout: left fixed tree, right stacked viewers
        self.text_edit = QtWidgets.QPlainTextEdit()
        self.text_edit.setReadOnly(False)
        self.text_edit.hide()
        self.image_label = QtWidgets.QLabel(alignment=QtCore.Qt.AlignCenter)
        self.image_label.hide()
        self.image_label.setSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        self.image_label.setMinimumSize(512, 384)
        self.image_label.setMaximumSize(4096, 4096)

        right_stack = QtWidgets.QStackedWidget()
        right_stack.addWidget(self.preview)     # index 0
        right_stack.addWidget(self.text_edit)   # index 1
        right_stack.addWidget(self.image_label) # index 2
        self.right_stack = right_stack

        self.tree.setMinimumWidth(320)
        self.tree.setMaximumWidth(480)

        central = QtWidgets.QWidget()
        hbox = QtWidgets.QHBoxLayout(central)
        hbox.addWidget(self.tree)
        hbox.addWidget(self.right_stack, 1)
        self.setCentralWidget(central)

        self._build_menu()

        self.status = QtWidgets.QStatusBar()
        self.setStatusBar(self.status)

    def _build_menu(self):
        mb = self.menuBar()
        fm = mb.addMenu("File")
        open_act = fm.addAction("Open .gsc…")
        save_as_act = fm.addAction("Save As… (override.gsc)")
        fm.addSeparator()
        exit_act = fm.addAction("Exit")

        open_act.triggered.connect(self.action_open)
        save_as_act.triggered.connect(self.action_save_as)
        exit_act.triggered.connect(self.close)

        bm = mb.addMenu("Batch")
        export_folder = bm.addAction("Export Folder…")
        replace_from_folder = bm.addAction("Replace From Folder…")
        add_new_from_folder = bm.addAction("Add New From Folder…")
        export_folder.triggered.connect(self.action_export_folder)
        replace_from_folder.triggered.connect(self.action_replace_from_folder)
        add_new_from_folder.triggered.connect(self.action_add_new_from_folder)

        om = mb.addMenu("Options")
        self.ru_key_action = om.addAction("Use RU key (0x4EBA)")
        self.ru_key_action.setCheckable(True)
        self.ru_key_action.setChecked(False)
        self.ru_key_action.triggered.connect(self._toggle_key)

    def _toggle_key(self):
        if self.archive:
            self.archive.key_word = 0x4EBA if self.ru_key_action.isChecked() else 0x78CD
            self.archive.key_byte = (~((self.archive.key_word >> 8) & 0xFF)) & 0xFF
            self.status.showMessage(f"Key set to 0x{self.archive.key_word:04X}", 3000)

    def action_open(self):
        path, _ = QtWidgets.QFileDialog.getOpenFileName(self, "Open .gsc", str(Path.cwd()), "GSC Archives (*.gsc)")
        if not path:
            return
        self.load_archive(path)

    def load_archive(self, path: str):
        try:
            if self.archive:
                self.archive.close()
            self.archive = GscArchive(path)
            # Key selection based on checkbox before read
            if self.ru_key_action.isChecked():
                self.archive.key_word = 0x4EBA
                self.archive.key_byte = (~((self.archive.key_word >> 8) & 0xFF)) & 0xFF
            self.archive.open()
        except Exception as e:
            QtWidgets.QMessageBox.critical(self, "Open failed", str(e))
            return
        self.status.showMessage(f"Loaded {path}")
        self._populate_tree()

    def _populate_tree(self):
        self.model.removeRows(0, self.model.rowCount())
        if not self.archive:
            return
        root = {}
        for e in self.archive.list():
            parts = e.name.split("\\")
            curr = root
            for p in parts[:-1]:
                curr = curr.setdefault(p, {})
            curr[parts[-1]] = e
        self._add_items(self.model, root)
        self.tree.expandAll()

    def _add_items(self, parent: QtGui.QStandardItemModel | QtGui.QStandardItem, node):
        if isinstance(parent, QtGui.QStandardItemModel):
            parent_item = parent.invisibleRootItem()
        else:
            parent_item = parent
        for name, value in sorted(node.items(), key=lambda kv: (not isinstance(kv[1], dict), kv[0])):
            if isinstance(value, dict):
                it = FileTreeItem(name)
                size_it = QtGui.QStandardItem("")
                flag_it = QtGui.QStandardItem("")
                parent_item.appendRow([it, size_it, flag_it])
                self._add_items(it, value)
            else:
                e = value
                it = FileTreeItem(name, e.name)
                size_it = QtGui.QStandardItem(str(e.size))
                flag_it = QtGui.QStandardItem(str(e.flags))
                parent_item.appendRow([it, size_it, flag_it])

    def _context_menu(self, pos: QtCore.QPoint):
        index = self.tree.indexAt(pos)
        if not index.isValid():
            return
        menu = QtWidgets.QMenu(self)
        export_action = menu.addAction("Export…")
        replace_action = menu.addAction("Replace…")
        delete_action = menu.addAction("Delete (virtual)")
        action = menu.exec_(self.tree.viewport().mapToGlobal(pos))
        if action == export_action:
            self.action_export()
        elif action == replace_action:
            self.action_replace()
        elif action == delete_action:
            self.action_delete()

    def _on_double_click(self, index: QtCore.QModelIndex):
        if not self.archive:
            return
        it = self.model.itemFromIndex(index)
        if not isinstance(it, FileTreeItem) or it.full_name is None:
            return
        try:
            data = self.archive.read_file(it.full_name)
        except Exception as e:
            self.preview.setText(str(e))
            return
        # Try preview (images and known resource types); otherwise show hex
        name = it.full_name.upper()
        if name.endswith((".BMP", ".PNG", ".JPG", ".JPEG", ".TGA")):
            try:
                im = Image.open(io.BytesIO(data))  # type: ignore[name-defined]
            except Exception:
                self.preview.setText("Cannot decode image")
                self.right_stack.setCurrentIndex(0)
                return
            # Показ через общий ImageView: создаём GP viewer с одним кадром из изображения
            rgba_im = im.convert('RGBA')
            w, h = rgba_im.size
            rgba = rgba_im.tobytes('raw', 'RGBA')
            from .gp_viewer import ImageView  # reuse viewer
            if not hasattr(self, '_image_view'):
                self._image_view = ImageView(self.right_stack)
                self.right_stack.addWidget(self._image_view)
            qimg = QtGui.QImage(rgba, w, h, QtGui.QImage.Format_RGBA8888)
            pm = QtGui.QPixmap.fromImage(qimg)
            self._image_view.set_pixmap(pm)
            self.right_stack.setCurrentWidget(self._image_view)
        elif name.endswith((".TXT", ".LST")):
            enc = 'utf-8'
            try:
                txt = data.decode(enc)
            except UnicodeDecodeError:
                try:
                    txt = data.decode('cp1251')
                except UnicodeDecodeError:
                    txt = data.decode('latin-1', errors='replace')
            self.text_edit.setPlainText(txt)
            self.text_edit.show()
            self.right_stack.setCurrentIndex(1)
        elif name.endswith(".GP"):
            try:
                # Встраиваемый GP viewer в правую панель
                pal = self.archive.try_read_palette()
                self._gp_widget = GpViewerWidget(self.right_stack, data, it.full_name, palette=pal)
                # загрузим все доступные палитры
                pals = self.archive.list_palettes()
                if pals:
                    self._gp_widget.add_palettes_bulk(pals)
                # поместим во временную страницу стека
                self.right_stack.addWidget(self._gp_widget)
                self.right_stack.setCurrentWidget(self._gp_widget)
            except Exception as e:
                # fallback: метаданные
                try:
                    gp = parse_gp_summary(data)
                    info = [f"Sign: 0x{gp.sign:08X}", f"Frames: {gp.npictures}"]
                    self.preview.setText("\n".join(info))
                except Exception:
                    self.preview.setText(f"GP parse error: {e}")
                self.right_stack.setCurrentIndex(0)
        elif name.endswith(".SMP") or name.endswith(".LNK"):
            # Попытка форматного предпросмотра .lnk (структуры корней и точек)
            try:
                smp = parse_lnk(data)
                scene = LnkScene(smp, self)
                self._lnk_view = LnkView(scene, self.right_stack)
                self.right_stack.addWidget(self._lnk_view)
                self.right_stack.setCurrentWidget(self._lnk_view)
            except Exception:
                # fallback: текстовый редактор/hex
                try:
                    text = data.decode('utf-8')
                except UnicodeDecodeError:
                    try:
                        text = data.decode('cp1251')
                    except UnicodeDecodeError:
                        text = None
                if text is None:
                    self.preview.setText(self._as_hex(data[:4096]))
                    self.right_stack.setCurrentIndex(0)
                else:
                    self.text_edit.setPlainText(text)
                    self.right_stack.setCurrentIndex(1)
        else:
            # Любые неизвестные — показываем как текст в редакторе
            text = None
            for enc in ('utf-8', 'cp1251', 'latin-1'):
                try:
                    text = data.decode(enc)
                    break
                except UnicodeDecodeError:
                    continue
            if text is None:
                text = self._as_hex(data[:4096])
                self.preview.setText(text)
                self.right_stack.setCurrentIndex(0)
            else:
                self.text_edit.setPlainText(text)
                self.right_stack.setCurrentIndex(1)

    def _pil_to_qimage(self, im: Image.Image) -> QtGui.QImage:
        if im.mode not in ("RGB", "RGBA"):
            im = im.convert("RGBA")
        data = im.tobytes("raw", im.mode)
        if im.mode == "RGBA":
            return QtGui.QImage(data, im.width, im.height, QtGui.QImage.Format_RGBA8888)
        return QtGui.QImage(data, im.width, im.height, QtGui.QImage.Format_RGB888)

    def _as_hex(self, data: bytes) -> str:
        import binascii

        return binascii.hexlify(data, sep=b" ").decode("ascii")

    def action_export(self):
        if not self.archive:
            return
        idxs = self.tree.selectionModel().selectedIndexes()
        if not idxs:
            return
        # Export first selected file
        it = self.model.itemFromIndex(idxs[0])
        if not isinstance(it, FileTreeItem) or it.full_name is None:
            return
        data = self.archive.read_file(it.full_name)
        out, _ = QtWidgets.QFileDialog.getSaveFileName(self, "Export File", it.full_name.split("\\")[-1])
        if out:
            Path(out).write_bytes(data)
            self.status.showMessage(f"Exported to {out}", 3000)

    def action_replace(self):
        if not self.archive:
            return
        idxs = self.tree.selectionModel().selectedIndexes()
        if not idxs:
            return
        it = self.model.itemFromIndex(idxs[0])
        if not isinstance(it, FileTreeItem) or it.full_name is None:
            return
        src, _ = QtWidgets.QFileDialog.getOpenFileName(self, "Choose replacement", it.full_name.split("\\")[-1])
        if not src:
            return
        data = Path(src).read_bytes()
        # Virtual replace: build override.gsc on save
        it.setData(data, QtCore.Qt.UserRole + 1)
        it.setForeground(QtGui.QBrush(QtGui.QColor("orange")))
        self.status.showMessage(f"Marked for replacement: {it.full_name}", 3000)

    def action_delete(self):
        idxs = self.tree.selectionModel().selectedIndexes()
        for idx in idxs:
            it = self.model.itemFromIndex(idx)
            if isinstance(it, FileTreeItem) and it.full_name:
                it.setData(b"__DELETE__", QtCore.Qt.UserRole + 1)
                it.setForeground(QtGui.QBrush(QtGui.QColor("red")))

    def _collect_overrides(self):
        overrides = {}
        def walk(item: QtGui.QStandardItem):
            for r in range(item.rowCount()):
                row_item = item.child(r, 0)
                mark = row_item.data(QtCore.Qt.UserRole + 1)
                if isinstance(row_item, FileTreeItem) and row_item.full_name and mark is not None:
                    overrides[row_item.full_name] = mark
                walk(row_item)
        root = self.model.invisibleRootItem()
        walk(root)
        return overrides

    def action_save_as(self):
        if not self.archive:
            return
        out, _ = QtWidgets.QFileDialog.getSaveFileName(self, "Save override.gsc", "override.gsc", "GSC Archives (*.gsc)")
        if not out:
            return
        overrides = self._collect_overrides()
        items = []
        # Take original entries (unless deleted), then add/replace overrides
        have = set()
        for e in self.archive.list():
            if e.name in overrides and overrides[e.name] == b"__DELETE__":
                continue
            data = overrides.get(e.name)
            if isinstance(data, (bytes, bytearray)):
                items.append(BuildItem(e.name, bytes(data), encrypted=bool(e.flags)))
            else:
                items.append(BuildItem(e.name, self.archive.read_file(e.name), encrypted=bool(e.flags)))
            have.add(e.name)
        # Add new files not present originally
        for name, payload in overrides.items():
            if name not in have and isinstance(payload, (bytes, bytearray)):
                enc = name.upper().endswith((".GP", ".RLC", ".VOC"))
                items.append(BuildItem(name, bytes(payload), encrypted=enc))
        # Plus new files added from folder
        for name, payload in self._new_files.items():
            if name not in have:
                enc = name.upper().endswith((".GP", ".RLC", ".VOC"))
                items.append(BuildItem(name, payload, encrypted=enc))
        blob = build_gsc(items, key_word=(0x4EBA if self.ru_key_action.isChecked() else 0x78CD))
        try:
            Path(out).write_bytes(blob)
        except Exception as e:
            QtWidgets.QMessageBox.critical(self, "Save failed", str(e))
            return
        self.status.showMessage(f"Saved {out}", 4000)

    def action_export_folder(self):
        if not self.archive:
            return
        target = QtWidgets.QFileDialog.getExistingDirectory(self, "Export Folder")
        if not target:
            return
        # Export all files preserving folder tree
        for e in self.archive.list():
            data = self.archive.read_file(e.name)
            dst = Path(target) / e.name.replace("\\", os.sep)
            dst.parent.mkdir(parents=True, exist_ok=True)
            dst.write_bytes(data)
        self.status.showMessage(f"Exported {len(self.archive.entries)} files to {target}", 4000)

    def action_replace_from_folder(self):
        if not self.archive:
            return
        source = QtWidgets.QFileDialog.getExistingDirectory(self, "Replace From Folder")
        if not source:
            return
        base = Path(source)
        changed = 0
        # Iterate all existing entries; if file exists at source/name, mark replacement
        for e in self.archive.list():
            src = base / e.name.replace("\\", os.sep)
            if src.exists() and src.is_file():
                data = src.read_bytes()
                # find item in model to mark
                idx = self._find_item_by_fullname(e.name)
                if idx is not None:
                    idx.setData(data, QtCore.Qt.UserRole + 1)
                    idx.setForeground(QtGui.QBrush(QtGui.QColor("orange")))
                    changed += 1
        self.status.showMessage(f"Marked {changed} files for replacement", 4000)

    def action_add_new_from_folder(self):
        base = QtWidgets.QFileDialog.getExistingDirectory(self, "Add New From Folder")
        if not base:
            return
        base_path = Path(base)
        added = 0
        for p in base_path.rglob('*'):
            if p.is_file():
                rel = p.relative_to(base_path).as_posix().replace('/', '\\').upper()
                data = p.read_bytes()
                self._new_files[rel] = data
                added += 1
        self.status.showMessage(f"Staged {added} new files for adding", 4000)

    def _find_item_by_fullname(self, fullname: str) -> Optional[FileTreeItem]:
        def walk(item: QtGui.QStandardItem) -> Optional[FileTreeItem]:
            for r in range(item.rowCount()):
                child = item.child(r, 0)
                if isinstance(child, FileTreeItem) and child.full_name == fullname:
                    return child
                found = walk(child)
                if found:
                    return found
            return None
        return walk(self.model.invisibleRootItem())


def main():
    app = QtWidgets.QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(app.exec())


