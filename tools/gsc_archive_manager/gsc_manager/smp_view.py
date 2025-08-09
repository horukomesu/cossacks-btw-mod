from __future__ import annotations

import struct
from dataclasses import dataclass
from typing import List
from PySide6 import QtCore, QtGui, QtWidgets


_SAMPLE_ROOT = struct.Struct('<BBhhII')  # RootType, Direction, x, y, AttachMask, Reserved


@dataclass
class SmpRoot:
    rtype: int
    dir: int
    x: int
    y: int
    mask: int


@dataclass
class SmpData:
    dx: int
    dy: int
    roots: List[SmpRoot]
    points: List[tuple[int, int]]


def parse_lnk(blob: bytes) -> SmpData:
    # Minimal parser based on game reading order in LinkSys::LoadOneLink
    # int, int (unknown), word Nsq, word Nrt, short dx, short dy, then Nsq pairs of bytes (xi, yi), then Nrt SampleRoot
    if len(blob) < 4 * 2 + 2 * 2 + 2 * 2:
        raise ValueError('LNK too small')
    off = 0
    _ = struct.unpack_from('<II', blob, off); off += 8
    nsq, nrt = struct.unpack_from('<HH', blob, off); off += 4
    dx, dy = struct.unpack_from('<hh', blob, off); off += 4
    points: List[tuple[int, int]] = []
    for _i in range(nsq):
        if off + 2 > len(blob):
            break
        xi = struct.unpack_from('<b', blob, off)[0]; off += 1
        yi = struct.unpack_from('<b', blob, off)[0]; off += 1
        points.append((xi, yi))
    roots: List[SmpRoot] = []
    for _i in range(nrt):
        if off + _SAMPLE_ROOT.size > len(blob):
            break
        rtype, di, rx, ry, mask, _res = _SAMPLE_ROOT.unpack_from(blob, off)
        off += _SAMPLE_ROOT.size
        roots.append(SmpRoot(rtype, di, rx, ry, mask))
    return SmpData(dx=dx, dy=dy, roots=roots, points=points)


class LnkScene(QtWidgets.QGraphicsScene):
    def __init__(self, smp: SmpData, parent: QtWidgets.QObject | None = None):
        super().__init__(parent)
        self.smp = smp
        self._draw()

    def _draw(self):
        # Simple orthographic drawing: points as small squares, roots as arrows
        scale = 8
        # grid
        for x in range(-16, 17):
            self.addLine(x * scale, -16 * scale, x * scale, 16 * scale, QtGui.QPen(QtGui.QColor(230, 230, 230)))
        for y in range(-16, 17):
            self.addLine(-16 * scale, y * scale, 16 * scale, y * scale, QtGui.QPen(QtGui.QColor(230, 230, 230)))
        # points
        for (x, y) in self.smp.points:
            r = QtCore.QRectF(x * scale - 2, y * scale - 2, 4, 4)
            self.addRect(r, QtGui.QPen(QtGui.QColor(0, 120, 215)), QtGui.QBrush(QtGui.QColor(0, 120, 215)))
        # roots as arrows (direction 0..7) with color by type
        colors = [QtGui.QColor(200,0,0), QtGui.QColor(0,180,0), QtGui.QColor(0,80,200), QtGui.QColor(180,140,0)]
        for r in self.smp.roots:
            c = colors[r.rtype % len(colors)]
            x0 = r.x * scale; y0 = r.y * scale
            dx = [1,1,0,-1,-1,-1,0,1][r.dir % 8]
            dy = [0,1,1,1,0,-1,-1,-1][r.dir % 8]
            x1 = x0 + dx * scale
            y1 = y0 + dy * scale
            pen = QtGui.QPen(c, 2)
            self.addLine(x0, y0, x1, y1, pen)
            # arrowhead
            self.addEllipse(QtCore.QRectF(x0-1,y0-1,2,2), pen, QtGui.QBrush(c))

class LnkView(QtWidgets.QGraphicsView):
    def __init__(self, scene: QtWidgets.QGraphicsScene, parent: QtWidgets.QWidget | None = None):
        super().__init__(scene, parent)
        self.setRenderHints(QtGui.QPainter.Antialiasing)
        self.setDragMode(QtWidgets.QGraphicsView.ScrollHandDrag)
        self.setBackgroundBrush(QtGui.QBrush(QtGui.QColor(250,250,250)))
        self.setTransformationAnchor(QtWidgets.QGraphicsView.AnchorUnderMouse)

    def wheelEvent(self, event: QtGui.QWheelEvent) -> None:
        if event.angleDelta().y() > 0:
            self.scale(1.25, 1.25)
        else:
            self.scale(0.8, 0.8)
        event.accept()


