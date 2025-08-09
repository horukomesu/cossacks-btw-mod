from __future__ import annotations

from PySide6 import QtCore, QtGui, QtWidgets

from .gp_parser import parse_gp_summary
from .gp_render import render_gp_frame


class ImageView(QtWidgets.QGraphicsView):
    def __init__(self, parent: QtWidgets.QWidget | None = None):
        super().__init__(parent)
        self.setScene(QtWidgets.QGraphicsScene(self))
        self._pix_item: QtWidgets.QGraphicsPixmapItem | None = None  # type: ignore[name-defined]
        self.setDragMode(QtWidgets.QGraphicsView.ScrollHandDrag)
        self.setRenderHints(QtGui.QPainter.Antialiasing | QtGui.QPainter.SmoothPixmapTransform)
        # Checkerboard background
        pm = QtGui.QPixmap(16, 16)
        pm.fill(QtGui.QColor(220, 220, 220))
        p = QtGui.QPainter(pm)
        p.fillRect(0, 0, 8, 8, QtGui.QColor(245, 245, 245))
        p.fillRect(8, 8, 8, 8, QtGui.QColor(245, 245, 245))
        p.end()
        self.setBackgroundBrush(QtGui.QBrush(pm))
        self._zoom = 0

    def set_pixmap(self, pm: QtGui.QPixmap):
        if self._pix_item is None:
            self._pix_item = self.scene().addPixmap(pm)
        else:
            self._pix_item.setPixmap(pm)
        self.scene().setSceneRect(pm.rect())
        self.reset_transform()

    def reset_transform(self):
        self._zoom = 0
        self.setTransform(QtGui.QTransform())
        self.centerOn(0, 0)

    def fit_to_window(self):
        if not self._pix_item:
            return
        self._zoom = 0
        self.setTransform(QtGui.QTransform())
        self.fitInView(self._pix_item, QtCore.Qt.KeepAspectRatio)

    def zoom_in(self):
        self._zoom += 1
        self.scale(1.25, 1.25)

    def zoom_out(self):
        self._zoom -= 1
        self.scale(0.8, 0.8)

    def wheelEvent(self, event: QtGui.QWheelEvent):
        if QtWidgets.QApplication.keyboardModifiers() & QtCore.Qt.ControlModifier:
            if event.angleDelta().y() > 0:
                self.zoom_in()
            else:
                self.zoom_out()
            event.accept()
        else:
            super().wheelEvent(event)


class GpViewerWidget(QtWidgets.QWidget):
    def __init__(self, parent: QtWidgets.QWidget | None, blob: bytes, title: str, palette: bytes | None = None):
        super().__init__(parent)
        self._blob = blob
        self._summary = parse_gp_summary(blob)
        self._palette = palette
        self._palettes: list[tuple[str, bytes]] = []
        if palette:
            self._palettes.append(("auto", palette))

        self.view = ImageView(self)

        self.slider = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        self.slider.setRange(0, max(0, self._summary.npictures - 1))
        self.slider.valueChanged.connect(self._render)

        self.mirror = QtWidgets.QCheckBox("Mirror (SprIndex ≥ 4096)")
        self.mirror.stateChanged.connect(self._render)

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
        ctrls.addWidget(QtWidgets.QLabel("Frame:"))
        ctrls.addWidget(self.slider)
        ctrls.addWidget(self.mirror)
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
            rgba, w, h = render_gp_frame(self._blob, idx, palette=pal)
            img = QtGui.QImage(rgba, w, h, QtGui.QImage.Format_RGBA8888)
            if self.mirror.isChecked():
                img = img.mirrored(True, False)
            pm = QtGui.QPixmap.fromImage(img)
            self.view.set_pixmap(pm)
            self.info.setText(f"Frame {idx+1}/{self._summary.npictures} — {w}×{h}")
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
        # Wheel without Ctrl switches frames (convenience)
        if not (QtWidgets.QApplication.keyboardModifiers() & QtCore.Qt.ControlModifier):
            step = 1 if event.angleDelta().y() > 0 else -1
            self.slider.setValue(min(self.slider.maximum(), max(0, self.slider.value() + step)))
            event.accept()
            return
        super().wheelEvent(event)

    def _export_png(self):
        path, _ = QtWidgets.QFileDialog.getSaveFileName(self, "Export frame as PNG", "frame.png", "PNG Images (*.png)")
        if not path:
            return
        if self.view._pix_item is None:
            return
        pm: QtGui.QPixmap = self.view._pix_item.pixmap()  # type: ignore[assignment]
        pm.save(path, "PNG")


class GpViewer(QtWidgets.QDialog):
    def __init__(self, parent: QtWidgets.QWidget | None, blob: bytes, title: str, palette: bytes | None = None):
        super().__init__(parent)
        self.setWindowTitle(f"GP Viewer — {title}")
        self.resize(900, 700)
        self.widget = GpViewerWidget(self, blob, title, palette)
        lay = QtWidgets.QVBoxLayout(self)
        lay.addWidget(self.widget)


