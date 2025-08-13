from __future__ import annotations

from PySide6 import QtCore, QtGui, QtWidgets

from .rlc_parser import parse_rlc_summary, decode_rlc_rgba
from .gp_viewer import ImageView


class RlcViewerWidget(QtWidgets.QWidget):
    def __init__(self, parent: QtWidgets.QWidget | None, blob: bytes, title: str, palette: bytes | None = None):
        super().__init__(parent)
        self._blob = blob
        self._summary = parse_rlc_summary(blob)
        self._palette = palette
        self._palettes: list[tuple[str, bytes]] = []
        if palette:
            self._palettes.append(("auto", palette))

        self.view = ImageView(self)

        self.slider = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        self.slider.setRange(0, max(0, self._summary.nimages - 1))
        self.slider.valueChanged.connect(self._render)

        self.pal_combo = QtWidgets.QComboBox()
        self.pal_combo.addItem("Auto palette")
        self.pal_combo.currentIndexChanged.connect(self._render)

        self.fit_btn = QtWidgets.QPushButton("Fit")
        self.fit_btn.clicked.connect(self.view.fit_to_window)
        self.reset_btn = QtWidgets.QPushButton("100%")
        self.reset_btn.clicked.connect(self.view.reset_transform)
        self.save_btn = QtWidgets.QPushButton("Export PNG…")
        self.save_btn.clicked.connect(self._export_png)

        self.info = QtWidgets.QLabel()

        ctrls = QtWidgets.QHBoxLayout()
        ctrls.addWidget(QtWidgets.QLabel("Image:"))
        ctrls.addWidget(self.slider)
        ctrls.addWidget(QtWidgets.QLabel("Palette:"))
        ctrls.addWidget(self.pal_combo)
        ctrls.addWidget(self.fit_btn)
        ctrls.addWidget(self.reset_btn)
        ctrls.addWidget(self.save_btn)

        layout = QtWidgets.QVBoxLayout(self)
        layout.addWidget(self.view, 1)
        layout.addLayout(ctrls)
        layout.addWidget(self.info)

        self._render()

    def _render(self):
        idx = self.slider.value()
        try:
            pal = self._palette
            if self.pal_combo.currentIndex() > 0 and self._palettes:
                pal = self._palettes[self.pal_combo.currentIndex() - 1][1]
            rgba, w, h = decode_rlc_rgba(self._blob, idx, palette_rgb_768=pal)
            img = QtGui.QImage(rgba, w, h, QtGui.QImage.Format_RGBA8888)
            pm = QtGui.QPixmap.fromImage(img)
            self.view.set_pixmap(pm)
            self.info.setText(f"Image {idx+1}/{self._summary.nimages} — {w}×{h}")
        except Exception as e:
            self.info.setText(str(e))

    def add_palette(self, name: str, pal: bytes):
        if len(pal) >= 768:
            self._palettes.append((name, pal[:768]))
            self.pal_combo.addItem(name)

    def add_palettes_bulk(self, items: list[tuple[str, bytes]]):
        for n, p in items:
            self.add_palette(n, p)

    def keyPressEvent(self, event: QtGui.QKeyEvent) -> None:
        if event.key() in (QtCore.Qt.Key_Left, QtCore.Qt.Key_Up):
            self.slider.setValue(max(0, self.slider.value() - 1))
            event.accept()
            return
        if event.key() in (QtCore.Qt.Key_Right, QtCore.Qt.Key_Down):
            self.slider.setValue(min(self.slider.maximum(), self.slider.value() + 1))
            event.accept()
            return
        super().keyPressEvent(event)

    def wheelEvent(self, event: QtGui.QWheelEvent) -> None:
        # Wheel without Ctrl switches images
        if not (QtWidgets.QApplication.keyboardModifiers() & QtCore.Qt.ControlModifier):
            step = 1 if event.angleDelta().y() > 0 else -1
            self.slider.setValue(min(self.slider.maximum(), max(0, self.slider.value() + step)))
            event.accept()
            return
        super().wheelEvent(event)

    def _export_png(self):
        path, _ = QtWidgets.QFileDialog.getSaveFileName(self, "Export as PNG", "image.png", "PNG Images (*.png)")
        if not path:
            return
        if self.view._pix_item is None:
            return
        pm: QtGui.QPixmap = self.view._pix_item.pixmap()  # type: ignore[assignment]
        pm.save(path, "PNG")


class RlcViewer(QtWidgets.QDialog):
    def __init__(self, parent: QtWidgets.QWidget | None, blob: bytes, title: str, palette: bytes | None = None):
        super().__init__(parent)
        self.setWindowTitle(f"RLC Viewer — {title}")
        self.resize(900, 700)
        self.widget = RlcViewerWidget(self, blob, title, palette)
        lay = QtWidgets.QVBoxLayout(self)
        lay.addWidget(self.widget)


