
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

// Regina core includes:
#include "surfaces/nnormalsurface.h"
#include "surfaces/nnormalsurfacelist.h"

#define NICE_SIZE 200
#define MIN_CELL 5
#define MAX_CELL 20
#define LEFT_MARGIN 5
#define TOP_MARGIN 5
#define OUTER_MARGIN 10
#define OUTER_MARGIN_TINY 5
#define TICK_LENGTH 3

// UI includes:
#include "ncompatcanvas.h"

#include <qcolor.h>

using regina::NNormalSurface;
using regina::NNormalSurfaceList;

NCompatCanvas::NCompatCanvas(unsigned useNumSurfaces) :
        QCanvas(),
        nSurfaces(useNumSurfaces), filled(false) {
    if (MIN_CELL * nSurfaces > NICE_SIZE)
        cellSize = MIN_CELL;
    else {
        cellSize = NICE_SIZE / nSurfaces;
        if (cellSize > MAX_CELL)
            cellSize = MAX_CELL;
    }

    // Work out how much vertical and horizontal space we will need for
    // text.  Assume here that (nSurfaces-1) is the largest number we
    // will need to draw.
    QCanvasText* t = new QCanvasText(QString::number(nSurfaces - 1), this);
    unsigned textWidth = t->boundingRect().width();
    unsigned textHeight = t->boundingRect().height();
    delete t;

    gridX = LEFT_MARGIN + textWidth + 2 * TICK_LENGTH;
    gridY = TOP_MARGIN + textHeight + 2 * TICK_LENGTH;
    gridSize = nSurfaces * cellSize + 1;

    // Work out how much extra space we might need beyond the bottom of the
    // canvas.  Be careful when subtracting, since we are using unsigned ints.
    unsigned rightMargin, bottomMargin;

    if (textWidth < cellSize)
        rightMargin = OUTER_MARGIN;
    else {
        rightMargin = ((textWidth - cellSize) / 2) + OUTER_MARGIN_TINY;
        if (rightMargin < OUTER_MARGIN)
            rightMargin = OUTER_MARGIN;
    }
    if (textHeight < cellSize)
        bottomMargin = OUTER_MARGIN;
    else {
        bottomMargin = ((textHeight - cellSize) / 2) + OUTER_MARGIN_TINY;
        if (bottomMargin < OUTER_MARGIN)
            bottomMargin = OUTER_MARGIN;
    }

    resize(gridX + gridSize + rightMargin, gridY + gridSize + bottomMargin);

    // Draw a bounding box.
    QCanvasRectangle* box = new QCanvasRectangle(
        gridX, gridY, gridSize, gridSize, this);
    box->setZ(10);
    box->show();

    // Draw labels along the horizontal axis.
    unsigned labelFreq = (nSurfaces >= 160 ? 20 : nSurfaces >= 30 ? 10 : 5);
    unsigned halfCell = cellSize / 2;

    unsigned pos;
    unsigned i;

    pos = gridX + halfCell;
    QCanvasText* prev = new QCanvasText(" 0 ", this);
    prev->setTextFlags(Qt::AlignHCenter | Qt::AlignTop);
    prev->move(pos, TOP_MARGIN);
    prev->show();

    QCanvasLine* tick = new QCanvasLine(this);
    tick->setPoints(pos, TOP_MARGIN + textHeight + TICK_LENGTH,
        pos, TOP_MARGIN + textHeight + 2 * TICK_LENGTH);
    tick->show();

    pos = gridX + halfCell + cellSize * (nSurfaces - 1);
    QCanvasText* last = new QCanvasText(
        QString(" %1 ").arg(nSurfaces - 1), this);
    last->setTextFlags(Qt::AlignHCenter | Qt::AlignTop);
    last->move(pos, TOP_MARGIN);
    if (last->collidesWith(prev)) {
        delete last;
        last = 0;
    } else {
        last->show();

        tick = new QCanvasLine(this);
        tick->setPoints(pos, TOP_MARGIN + textHeight + TICK_LENGTH,
            pos, TOP_MARGIN + textHeight + 2 * TICK_LENGTH);
        tick->show();
    }

    if (last) {
        for (i = labelFreq; i < nSurfaces - 1; i += labelFreq) {
            pos = gridX + halfCell + cellSize * i;
            t = new QCanvasText(QString(" %1 ").arg(i), this);
            t->setTextFlags(Qt::AlignHCenter | Qt::AlignTop);
            t->move(pos, TOP_MARGIN);
            if (t->collidesWith(prev) || t->collidesWith(last))
                delete t;
            else {
                t->show();
                prev = t;

                tick = new QCanvasLine(this);
                tick->setPoints(pos, TOP_MARGIN + textHeight + TICK_LENGTH,
                    pos, TOP_MARGIN + textHeight + 2 * TICK_LENGTH);
                tick->show();
            }
        }
    }

    // Draw labels along the vertical axis.
    pos = gridY + halfCell;
    prev = new QCanvasText(" 0 ", this);
    prev->setTextFlags(Qt::AlignRight | Qt::AlignVCenter);
    prev->move(LEFT_MARGIN + textWidth, pos);
    prev->show();

    tick = new QCanvasLine(this);
    tick->setPoints(LEFT_MARGIN + textWidth + TICK_LENGTH, pos,
        LEFT_MARGIN + textWidth + 2 * TICK_LENGTH, pos);
    tick->show();

    pos = gridY + halfCell + cellSize * (nSurfaces - 1);
    last = new QCanvasText(QString(" %1 ").arg(nSurfaces - 1), this);
    last->setTextFlags(Qt::AlignRight | Qt::AlignVCenter);
    last->move(LEFT_MARGIN + textWidth, pos);
    if (last->collidesWith(prev)) {
        delete last;
        last = 0;
    } else {
        last->show();

        tick = new QCanvasLine(this);
        tick->setPoints(LEFT_MARGIN + textWidth + TICK_LENGTH, pos,
            LEFT_MARGIN + textWidth + 2 * TICK_LENGTH, pos);
        tick->show();
    }

    if (last) {
        for (i = labelFreq; i < nSurfaces - 1; i += labelFreq) {
            pos = gridY + halfCell + cellSize * i;
            t = new QCanvasText(QString(" %1 ").arg(i), this);
            t->setTextFlags(Qt::AlignRight | Qt::AlignVCenter);
            t->move(LEFT_MARGIN + textWidth, pos);
            if (t->collidesWith(prev) || t->collidesWith(last))
                delete t;
            else {
                t->show();
                prev = t;

                tick = new QCanvasLine(this);
                tick->setPoints(LEFT_MARGIN + textWidth + TICK_LENGTH, pos,
                    LEFT_MARGIN + textWidth + 2 * TICK_LENGTH, pos);
                tick->show();
            }
        }
    }

    // Draw internal guide lines.
    for (i = 1; i < nSurfaces; ++i) {
        QCanvasLine* l = new QCanvasLine(this);
        if (i % 5)
            l->setPen(Qt::lightGray);
        l->setPoints(
            gridX,                gridY + (i * cellSize),
            gridX + gridSize - 1, gridY + (i * cellSize));
        l->setZ(9);
        l->show();
    }
    for (i = 1; i < nSurfaces; ++i) {
        QCanvasLine* l = new QCanvasLine(this);
        if (i % 5)
            l->setPen(Qt::lightGray);
        l->setPoints(
            gridX + (i * cellSize), gridY,
            gridX + (i * cellSize), gridY + gridSize - 1);
        l->setZ(9);
        l->show();
    }

    update();
}

NCompatCanvas::~NCompatCanvas() {
}

void NCompatCanvas::fillLocal(const NNormalSurfaceList& surfaces) {
    if (filled)
        return;

    QCanvasRectangle* box;
    QPen border(QPen::NoPen);
    QBrush fill(Qt::darkCyan);

    unsigned i, j;
    const NNormalSurface *s, *t;
    for (i = 0; i < nSurfaces; ++i) {
        s = surfaces.getSurface(i);

        for (j = i; j < nSurfaces; ++j) {
            t = surfaces.getSurface(j);

            if (s->locallyCompatible(*t)) {
                box = new QCanvasRectangle(
                    gridX + i * cellSize, gridY + j * cellSize,
                    cellSize + 1, cellSize + 1, this);
                box->setPen(border);
                box->setBrush(fill);
                box->setZ(8);
                box->show();

                if (i != j) {
                    box = new QCanvasRectangle(
                        gridX + j * cellSize, gridY + i * cellSize,
                        cellSize + 1, cellSize + 1, this);
                    box->setPen(border);
                    box->setBrush(fill);
                    box->setZ(8);
                    box->show();
                }
            }
        }
    }

    filled = true;
    update();
}

void NCompatCanvas::fillGlobal(const NNormalSurfaceList& surfaces) {
    if (filled)
        return;

    // We know the surface list is non-empty.

    unsigned i, j;
    const NNormalSurface *s, *t;

    bool* usable = new bool[nSurfaces];
    for (i = 0; i < nSurfaces; ++i) {
        s = surfaces.getSurface(i);
        usable[i] = (s->isCompact() && (! s->isEmpty()) &&
            s->isConnected().isTrue());
    }

    QCanvasRectangle* box;
    QPen border(QPen::NoPen);
    QBrush fill(Qt::darkGreen);
    QBrush hash(Qt::darkRed, Qt::DiagCrossPattern);

    for (i = 0; i < nSurfaces; ++i) {
        if (! usable[i]) {
            box = new QCanvasRectangle(
                gridX + i * cellSize, gridY, cellSize + 1, gridSize, this);
            box->setPen(border);
            box->setBrush(hash);
            box->setZ(7);
            box->show();

            box = new QCanvasRectangle(
                gridX, gridY + i * cellSize, gridSize, cellSize + 1, this);
            box->setPen(border);
            box->setBrush(hash);
            box->setZ(7);
            box->show();

            continue;
        }

        s = surfaces.getSurface(i);

        for (j = i; j < nSurfaces; ++j) {
            if (! usable[j])
                continue;

            t = surfaces.getSurface(j);

            if (s->disjoint(*t)) {
                box = new QCanvasRectangle(
                    gridX + i * cellSize, gridY + j * cellSize,
                    cellSize + 1, cellSize + 1, this);
                box->setPen(border);
                box->setBrush(fill);
                box->setZ(8);
                box->show();

                if (i != j) {
                    box = new QCanvasRectangle(
                        gridX + j * cellSize, gridY + i * cellSize,
                        cellSize + 1, cellSize + 1, this);
                    box->setPen(border);
                    box->setBrush(fill);
                    box->setZ(8);
                    box->show();
                }
            }
        }
    }

    delete[] usable;

    filled = true;
    update();
}

#include "ncompatcanvas.moc"

