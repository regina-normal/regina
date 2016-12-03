
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

// Regina core includes:
#include "hypersurface/normalhypersurface.h"
#include "hypersurface/normalhypersurfaces.h"
#include "surfaces/normalsurface.h"
#include "surfaces/normalsurfaces.h"

#define NICE_SIZE 200
#define MIN_CELL 5
#define MAX_CELL 20
#define LEFT_MARGIN 5
#define TOP_MARGIN 5
#define OUTER_MARGIN 10
#define OUTER_MARGIN_TINY 5
#define TICK_LENGTH 3

// UI includes:
#include "compatcanvas.h"

#include <qcolor.h>
#include <QGraphicsSimpleTextItem>

using regina::NormalHypersurface;
using regina::NormalHypersurfaces;
using regina::NormalSurface;
using regina::NormalSurfaces;

CompatCanvas::CompatCanvas(unsigned useNumSurfaces) :
        QGraphicsScene(),
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
    QGraphicsSimpleTextItem* t = new QGraphicsSimpleTextItem(
        QString::number(nSurfaces - 1));
    unsigned textWidth = t->boundingRect().width();
    unsigned textHeight = t->boundingRect().height();
    delete t;

    gridX = LEFT_MARGIN + textWidth + 2 * TICK_LENGTH;
    gridY = TOP_MARGIN + textHeight + 2 * TICK_LENGTH;
    gridSize = nSurfaces * cellSize;

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

    setSceneRect(0, 0, gridX + gridSize + 1 + rightMargin,
        gridY + gridSize + 1 + bottomMargin);

    // Draw a bounding box.
    QGraphicsRectItem* box = new QGraphicsRectItem(
        gridX, gridY, gridSize, gridSize);
    addItem(box);
    box->setZValue(10);
    box->show();

    // Draw labels along the horizontal axis.
    unsigned labelFreq = (nSurfaces >= 160 ? 20 : nSurfaces >= 30 ? 10 : 5);
    unsigned halfCell = cellSize / 2;

    unsigned pos;
    unsigned i;

    pos = gridX + halfCell;
    QGraphicsSimpleTextItem* prev = new QGraphicsSimpleTextItem(" 0 ");
    addItem(prev);
    prev->setPos(pos - prev->boundingRect().width() / 2, TOP_MARGIN);
    prev->show();

    QGraphicsLineItem* tick = new QGraphicsLineItem();
    addItem(tick);
    tick->setLine(pos, TOP_MARGIN + textHeight + TICK_LENGTH,
        pos, TOP_MARGIN + textHeight + 2 * TICK_LENGTH);
    tick->show();

    pos = gridX + halfCell + cellSize * (nSurfaces - 1);
    QGraphicsSimpleTextItem* last = new QGraphicsSimpleTextItem(
        QString(" %1 ").arg(nSurfaces - 1));
    addItem(last);
    last->setPos(pos - last->boundingRect().width() / 2, TOP_MARGIN);
    if (last->collidesWithItem(prev)) {
        delete last;
        last = 0;
    } else {
        last->show();

        tick = new QGraphicsLineItem();
        addItem(tick);
        tick->setLine(pos, TOP_MARGIN + textHeight + TICK_LENGTH,
            pos, TOP_MARGIN + textHeight + 2 * TICK_LENGTH);
        tick->show();
    }

    if (last) {
        for (i = labelFreq; i < nSurfaces - 1; i += labelFreq) {
            pos = gridX + halfCell + cellSize * i;
            t = new QGraphicsSimpleTextItem(QString(" %1 ").arg(i));
            addItem(t);
            t->setPos(pos - t->boundingRect().width() / 2, TOP_MARGIN);
            if (t->collidesWithItem(prev) || t->collidesWithItem(last))
                delete t;
            else {
                t->show();
                prev = t;

                tick = new QGraphicsLineItem();
                addItem(tick);
                tick->setLine(pos, TOP_MARGIN + textHeight + TICK_LENGTH,
                    pos, TOP_MARGIN + textHeight + 2 * TICK_LENGTH);
                tick->show();
            }
        }
    }

    // Draw labels along the vertical axis.
    pos = gridY + halfCell;
    prev = new QGraphicsSimpleTextItem(" 0 ");
    addItem(prev);
    prev->setPos(LEFT_MARGIN + textWidth - prev->boundingRect().width(),
        pos - prev->boundingRect().height() / 2);
    prev->show();

    tick = new QGraphicsLineItem();
    addItem(tick);
    tick->setLine(LEFT_MARGIN + textWidth + TICK_LENGTH, pos,
        LEFT_MARGIN + textWidth + 2 * TICK_LENGTH, pos);
    tick->show();

    pos = gridY + halfCell + cellSize * (nSurfaces - 1);
    last = new QGraphicsSimpleTextItem(QString(" %1 ").arg(nSurfaces - 1));
    addItem(last);
    last->setPos(LEFT_MARGIN + textWidth - last->boundingRect().width(),
        pos - last->boundingRect().height() / 2);
    if (last->collidesWithItem(prev)) {
        delete last;
        last = 0;
    } else {
        last->show();

        tick = new QGraphicsLineItem();
        addItem(tick);
        tick->setLine(LEFT_MARGIN + textWidth + TICK_LENGTH, pos,
            LEFT_MARGIN + textWidth + 2 * TICK_LENGTH, pos);
        tick->show();
    }

    if (last) {
        for (i = labelFreq; i < nSurfaces - 1; i += labelFreq) {
            pos = gridY + halfCell + cellSize * i;
            t = new QGraphicsSimpleTextItem(QString(" %1 ").arg(i));
            addItem(t);
            t->setPos(LEFT_MARGIN + textWidth - t->boundingRect().width(),
                pos - t->boundingRect().height() / 2);
            if (t->collidesWithItem(prev) || t->collidesWithItem(last))
                delete t;
            else {
                t->show();
                prev = t;

                tick = new QGraphicsLineItem();
                addItem(tick);
                tick->setLine(LEFT_MARGIN + textWidth + TICK_LENGTH, pos,
                    LEFT_MARGIN + textWidth + 2 * TICK_LENGTH, pos);
                tick->show();
            }
        }
    }

    // Draw internal guide lines.
    for (i = 1; i < nSurfaces; ++i) {
        QGraphicsLineItem* l = new QGraphicsLineItem();
        addItem(l);
        if (i % 5)
            l->setPen(QPen(Qt::lightGray));
        l->setLine(
            gridX,                gridY + (i * cellSize),
            gridX + gridSize, gridY + (i * cellSize));
        l->setZValue(9);
        l->show();
    }
    for (i = 1; i < nSurfaces; ++i) {
        QGraphicsLineItem* l = new QGraphicsLineItem();
        addItem(l);
        if (i % 5)
            l->setPen(QPen(Qt::lightGray));
        l->setLine(
            gridX + (i * cellSize), gridY,
            gridX + (i * cellSize), gridY + gridSize);
        l->setZValue(9);
        l->show();
    }

    update();
}

CompatCanvas::~CompatCanvas() {
}

void CompatCanvas::fillLocal(const NormalSurfaces& surfaces) {
    if (filled)
        return;

    QGraphicsRectItem* box;
    QPen border(Qt::NoPen);
    QBrush fill(Qt::darkCyan);

    unsigned i, j;
    const NormalSurface *s, *t;
    for (i = 0; i < nSurfaces; ++i) {
        s = surfaces.surface(i);

        for (j = i; j < nSurfaces; ++j) {
            t = surfaces.surface(j);

            if (s->locallyCompatible(*t)) {
                box = new QGraphicsRectItem(
                    gridX + i * cellSize, gridY + j * cellSize,
                    cellSize, cellSize);
                addItem(box);
                box->setPen(border);
                box->setBrush(fill);
                box->setZValue(8);
                box->show();

                if (i != j) {
                    box = new QGraphicsRectItem(
                        gridX + j * cellSize, gridY + i * cellSize,
                        cellSize, cellSize);
                    addItem(box);
                    box->setPen(border);
                    box->setBrush(fill);
                    box->setZValue(8);
                    box->show();
                }
            }
        }
    }

    filled = true;
    update();
}

void CompatCanvas::fillLocal(const NormalHypersurfaces& surfaces) {
    if (filled)
        return;

    QGraphicsRectItem* box;
    QPen border(Qt::NoPen);
    QBrush fill(Qt::darkCyan);

    unsigned i, j;
    const NormalHypersurface *s, *t;
    for (i = 0; i < nSurfaces; ++i) {
        s = surfaces.hypersurface(i);

        for (j = i; j < nSurfaces; ++j) {
            t = surfaces.hypersurface(j);

            if (s->locallyCompatible(*t)) {
                box = new QGraphicsRectItem(
                    gridX + i * cellSize, gridY + j * cellSize,
                    cellSize, cellSize);
                addItem(box);
                box->setPen(border);
                box->setBrush(fill);
                box->setZValue(8);
                box->show();

                if (i != j) {
                    box = new QGraphicsRectItem(
                        gridX + j * cellSize, gridY + i * cellSize,
                        cellSize, cellSize);
                    addItem(box);
                    box->setPen(border);
                    box->setBrush(fill);
                    box->setZValue(8);
                    box->show();
                }
            }
        }
    }

    filled = true;
    update();
}

void CompatCanvas::fillGlobal(const NormalSurfaces& surfaces) {
    if (filled)
        return;

    // We know the surface list is non-empty.

    unsigned i, j;
    const NormalSurface *s, *t;

    bool* usable = new bool[nSurfaces];
    for (i = 0; i < nSurfaces; ++i) {
        s = surfaces.surface(i);
        usable[i] = (s->isCompact() && (! s->isEmpty()) && s->isConnected());
    }

    QGraphicsRectItem* box;
    QPen border(Qt::NoPen);
    QBrush fill(Qt::darkGreen);
    QBrush hash(Qt::darkRed, Qt::DiagCrossPattern);

    for (i = 0; i < nSurfaces; ++i) {
        if (! usable[i]) {
            box = new QGraphicsRectItem(
                gridX + i * cellSize, gridY, cellSize, gridSize);
            addItem(box);
            box->setPen(border);
            box->setBrush(hash);
            box->setZValue(7);
            box->show();

            box = new QGraphicsRectItem(
                gridX, gridY + i * cellSize, gridSize, cellSize);
            addItem(box);
            box->setPen(border);
            box->setBrush(hash);
            box->setZValue(7);
            box->show();

            continue;
        }

        s = surfaces.surface(i);

        for (j = i; j < nSurfaces; ++j) {
            if (! usable[j])
                continue;

            t = surfaces.surface(j);

            if (s->disjoint(*t)) {
                box = new QGraphicsRectItem(
                    gridX + i * cellSize, gridY + j * cellSize,
                    cellSize, cellSize);
                addItem(box);
                box->setPen(border);
                box->setBrush(fill);
                box->setZValue(8);
                box->show();

                if (i != j) {
                    box = new QGraphicsRectItem(
                        gridX + j * cellSize, gridY + i * cellSize,
                        cellSize, cellSize);
                    addItem(box);
                    box->setPen(border);
                    box->setBrush(fill);
                    box->setZValue(8);
                    box->show();
                }
            }
        }
    }

    delete[] usable;

    filled = true;
    update();
}


