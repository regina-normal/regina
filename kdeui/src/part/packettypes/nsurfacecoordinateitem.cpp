
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

// Regina core includes:
#include "surfaces/nnormalsurface.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "coordinates.h"
#include "nsurfacecoordinateitem.h"

#include <klocale.h>
#include <qpainter.h>
#include <qstyle.h>

unsigned NSurfaceCoordinateItem::propertyColCount(bool embeddedOnly) {
    return (embeddedOnly ? 8 : 5);
}

QString NSurfaceCoordinateItem::propertyColName(int whichCol,
        bool embeddedOnly) {
    if (embeddedOnly) {
        switch (whichCol) {
            case 0 : return i18n("Name");
            case 1 : return i18n("Euler");
            case 2 : return i18n("Orient");
            case 3 : return i18n("Sides");
            case 4 : return i18n("Bdry");
            case 5 : return i18n("Link");
            case 6 : return i18n("Crush");
            case 7 : return i18n("Type");
        }
    } else {
        switch (whichCol) {
            case 0 : return i18n("Name");
            case 1 : return i18n("Euler");
            case 2 : return i18n("Bdry");
            case 3 : return i18n("Link");
            case 4 : return i18n("Type");
        }
    }

    return i18n("Unknown");
}

QString NSurfaceCoordinateItem::propertyColDesc(int whichCol,
        bool embeddedOnly) {
    if (embeddedOnly) {
        switch (whichCol) {
            case 0: return i18n("Name (this has no special meaning and "
                "can be edited)");
            case 1: return i18n("Euler characteristic");
            case 2: return i18n("Orientability");
            case 3: return i18n("1-sided or 2-sided");
            case 4: return i18n("Does this surface have boundary?");
            case 5: return i18n("Has this surface been identified as "
                "the link of a particular subcomplex?");
            case 6: return i18n("Is it safe to crush this surface to a point?");
            case 7: return i18n("Other interesting properties");
        }
    } else {
        switch (whichCol) {
            case 0: return i18n("Name (this has no special meaning and "
                "can be edited)");
            case 1: return i18n("Euler characteristic");
            case 2: return i18n("Does this surface have boundary?");
            case 3: return i18n("Has this surface been identified as "
                "the link of a particular subcomplex?");
            case 4: return i18n("Other interesting properties");
        }
    }

    return i18n("Unknown");
}

void NSurfaceCoordinateItem::setText(int column, const QString& str) {
    if (column == 0)
        name = str;
    KListViewItem::setText(column, str);
}

QString NSurfaceCoordinateItem::text(int column) const {
    int triBool;
    if (surfaces->isEmbeddedOnly()) {
        switch (column) {
            case 0:
                return name;
            case 1:
                if (! surface->isCompact())
                    return QString::null;

                return surface->getEulerCharacteristic().stringValue().c_str();
            case 2:
                if (! surface->isCompact())
                    return QString::null;

                triBool = surface->isOrientable();
                if (triBool == 1)
                    return i18n("Orbl"); // green
                else if (triBool == -1)
                    return i18n("Non-orbl"); // red
                else
                    return i18n("Unknown"); // yellow
            case 3:
                if (! surface->isCompact())
                    return QString::null;

                triBool = surface->isTwoSided();
                if (triBool == 1) // green
                    return "2";
                else if (triBool == -1) // red
                    return "1";
                else
                    return i18n("Unknown"); // yellow
            case 4:
                if (! surface->isCompact())
                    return i18n("Infinite"); // yellow
                else if (surface->hasRealBoundary())
                    return i18n("Real Bdry"); // red
                else
                    return i18n("Closed"); // green
            case 5: {
                const regina::NVertex* v;
                std::pair<const regina::NEdge*, const regina::NEdge*> e;

                if ((v = surface->isVertexLink()))
                    return i18n("Vertex %1").arg(
                        surfaces->getTriangulation()->getVertexIndex(v));
                else if ((e = surface->isThinEdgeLink()).first) {
                    if (e.second)
                        return i18n("Thin edges %1, %2").
                            arg(surfaces->getTriangulation()->getEdgeIndex(
                                e.first)).
                            arg(surfaces->getTriangulation()->getEdgeIndex(
                                e.second));
                    else
                        return i18n("Thin edge %1").
                            arg(surfaces->getTriangulation()->getEdgeIndex(
                                e.first));
                } else
                    return QString::null;
            }
            case 6:
                if (surfaces->allowsAlmostNormal() || ! surface->isCompact())
                    return i18n("N/A"); // yellow
                else if (surface->knownCanCrush())
                    return i18n("Yes"); // green
                else
                    return i18n("Unknown"); // yellow
            case 7:
                if (surface->isSplitting())
                    return i18n("Splitting");
                else
                    return QString::null;
            default:
                regina::NLargeInteger ans = Coordinates::getCoordinate(
                    coordSystem, *surface, column - 8);
                if (ans == (long)0)
                    return QString::null;
                else
                    return ans.stringValue().c_str();
        }
    } else {
        switch (column) {
            case 0:
                return name;
            case 1:
                if (! surface->isCompact())
                    return QString::null;

                return surface->getEulerCharacteristic().stringValue().c_str();
            case 2:
                if (! surface->isCompact())
                    return i18n("Infinite"); // yellow
                else if (surface->hasRealBoundary())
                    return i18n("Real Bdry"); // red
                else
                    return i18n("Closed"); // green
            case 3: {
                const regina::NVertex* v;
                std::pair<const regina::NEdge*, const regina::NEdge*> e;

                if ((v = surface->isVertexLink()))
                    return i18n("Vertex %1").arg(
                        surfaces->getTriangulation()->getVertexIndex(v));
                else if ((e = surface->isThinEdgeLink()).first) {
                    if (e.second)
                        return i18n("Thin edges %1, %2").
                            arg(surfaces->getTriangulation()->getEdgeIndex(
                                e.first)).
                            arg(surfaces->getTriangulation()->getEdgeIndex(
                                e.second));
                    else
                        return i18n("Thin edge %1").
                            arg(surfaces->getTriangulation()->getEdgeIndex(
                                e.first));
                } else
                    return QString::null;
            }
            case 4:
                if (surface->isSplitting())
                    return i18n("Splitting");
                else
                    return QString::null;
            default:
                regina::NLargeInteger ans = Coordinates::getCoordinate(
                    coordSystem, *surface, column - 5);
                if (ans == (long)0)
                    return QString::null;
                else
                    return ans.stringValue().c_str();
        }
    }
    return i18n("Unknown");
}

void NSurfaceCoordinateItem::paintCell(QPainter* p, const QColorGroup& cg,
        int column, int width, int align) {
    // Do the standard painting.
    // TODO: Change colour if appropriate.
    KListViewItem::paintCell(p, cg, column, width, align);

    // Draw a box around the cell.
    p->setPen((QRgb)listView()->style().styleHint(
        QStyle::SH_Table_GridLineColor, listView()));
    p->drawLine(0, height() - 1, width - 1, height() - 1);
    p->lineTo(width - 1, 0);
}

