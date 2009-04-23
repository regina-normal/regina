
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
#define LEFT_MARGIN 10
#define TOP_MARGIN 10
#define RIGHT_MARGIN 10
#define BOTTOM_MARGIN 10

// UI includes:
#include "ncompatcanvas.h"

#include <qcolor.h>

using regina::NNormalSurface;
using regina::NNormalSurfaceList;

NCompatCanvas::NCompatCanvas(unsigned useNumSurfaces) :
        QCanvas(),
        nSurfaces(useNumSurfaces), filled(false) {
    if (MIN_CELL * useNumSurfaces > NICE_SIZE)
        cellSize = MIN_CELL;
    else {
        cellSize = NICE_SIZE / useNumSurfaces;
        if (cellSize > MAX_CELL)
            cellSize = MAX_CELL;
    }

    resize(useNumSurfaces * cellSize + 1 + LEFT_MARGIN + RIGHT_MARGIN,
           useNumSurfaces * cellSize + 1 + TOP_MARGIN + BOTTOM_MARGIN);

    // Draw a bounding box.
    QCanvasRectangle* box = new QCanvasRectangle(LEFT_MARGIN, TOP_MARGIN,
        useNumSurfaces * cellSize + 1, useNumSurfaces * cellSize + 1,
        this);
    box->setZ(10);
    box->show();

    // Draw internal guide lines.
    unsigned i;
    for (i = 1; i < useNumSurfaces; ++i) {
        QCanvasLine* l = new QCanvasLine(this);
        if (i % 5)
            l->setPen(Qt::lightGray);
        l->setPoints(
            LEFT_MARGIN,
            TOP_MARGIN + (i * cellSize),
            LEFT_MARGIN + (useNumSurfaces * cellSize),
            TOP_MARGIN + (i * cellSize));
        l->setZ(9);
        l->show();
    }
    for (i = 1; i < useNumSurfaces; ++i) {
        QCanvasLine* l = new QCanvasLine(this);
        if (i % 5)
            l->setPen(Qt::lightGray);
        l->setPoints(
            LEFT_MARGIN + (i * cellSize),
            TOP_MARGIN,
            LEFT_MARGIN + (i * cellSize),
            TOP_MARGIN + (useNumSurfaces * cellSize));
        l->setZ(9);
        l->show();
    }

    // TODO: ticks+labels; verify!!

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
                    LEFT_MARGIN + i * cellSize,
                    TOP_MARGIN + j * cellSize,
                    cellSize + 1, cellSize + 1, this);
                box->setPen(border);
                box->setBrush(fill);
                box->setZ(8);
                box->show();

                if (i != j) {
                    box = new QCanvasRectangle(
                        LEFT_MARGIN + j * cellSize,
                        TOP_MARGIN + i * cellSize,
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
                LEFT_MARGIN + i * cellSize, TOP_MARGIN,
                cellSize + 1, cellSize * nSurfaces + 1, this);
            box->setPen(border);
            box->setBrush(hash);
            box->setZ(7);
            box->show();

            box = new QCanvasRectangle(
                LEFT_MARGIN, TOP_MARGIN + i * cellSize,
                cellSize * nSurfaces + 1, cellSize + 1, this);
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
                    LEFT_MARGIN + i * cellSize,
                    TOP_MARGIN + j * cellSize,
                    cellSize + 1, cellSize + 1, this);
                box->setPen(border);
                box->setBrush(fill);
                box->setZ(8);
                box->show();

                if (i != j) {
                    box = new QCanvasRectangle(
                        LEFT_MARGIN + j * cellSize,
                        TOP_MARGIN + i * cellSize,
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

