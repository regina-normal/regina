
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
#include "triangulation/ntriangulation.h"

// UI includes:
#include "coordinates.h"
#include "nsurfacecoordinateitem.h"

#include <klocale.h>
#include <qpainter.h>
#include <qstyle.h>

NSurfaceCoordinateItem::NSurfaceCoordinateItem(QListView* parent,
        regina::NNormalSurfaceList* fromSurfaces,
        unsigned long newSurfaceIndex, QString& newName, int useCoordSystem) :
        GridListViewItem(parent),
        surface(fromSurfaces->getSurface(newSurfaceIndex)),
        name(newName),
        surfaceIndex(newSurfaceIndex),
        surfaces(fromSurfaces),
        coordSystem(useCoordSystem),
        coordCols(Coordinates::numColumns(useCoordSystem,
            fromSurfaces->getTriangulation())) {
}

unsigned NSurfaceCoordinateItem::propertyColCount(bool embeddedOnly) {
    return (embeddedOnly ? 7 : 5);
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
            case 6 : return i18n("Type");
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
            case 6: return i18n("Other interesting properties");
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
    regina::NTriBool triBool;
    if (surfaces->isEmbeddedOnly()) {
        const int nCols = 7;
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
                if (triBool.isTrue())
                    return i18n("Orbl");
                else if (triBool.isFalse())
                    return i18n("Non-orbl");
                else
                    return i18n("Unknown");
            case 3:
                if (! surface->isCompact())
                    return QString::null;

                triBool = surface->isTwoSided();
                if (triBool.isTrue())
                    return "2";
                else if (triBool.isFalse())
                    return "1";
                else
                    return i18n("Unknown");
            case 4:
                if (! surface->isCompact())
                    return i18n("Infinite");
                else if (surface->hasRealBoundary())
                    return i18n("Real Bdry");
                else
                    return i18n("Closed");
            case 5: {
                const regina::NVertex* v;
                std::pair<const regina::NEdge*, const regina::NEdge*> e;

                if ((v = surface->isVertexLink()))
                    return i18n("Vertex %1").arg(
                        surfaces->getTriangulation()->vertexIndex(v));
                else if ((e = surface->isThinEdgeLink()).first) {
                    if (e.second)
                        return i18n("Thin edges %1, %2").
                            arg(surfaces->getTriangulation()->edgeIndex(
                                e.first)).
                            arg(surfaces->getTriangulation()->edgeIndex(
                                e.second));
                    else
                        return i18n("Thin edge %1").
                            arg(surfaces->getTriangulation()->edgeIndex(
                                e.first));
                } else
                    return QString::null;
            }
            case 6:
                {
                    regina::NLargeInteger tot;
                    if (surface->isSplitting())
                        return i18n("Splitting");
                    else if ((tot = surface->isCentral()) != 0)
                        return i18n("Central (%1)").arg(tot.longValue());
                    else
                        return QString::null;
                }
            default:
                if (column >= coordCols + nCols || column < 0)
                    return QString::null;

                regina::NLargeInteger ans = Coordinates::getCoordinate(
                    coordSystem, *surface, column - nCols);
                if (ans == (long)0)
                    return QString::null;
                else
                    return ans.stringValue().c_str();
        }
    } else {
        const int nCols = 5;
        switch (column) {
            case 0:
                return name;
            case 1:
                if (! surface->isCompact())
                    return QString::null;

                return surface->getEulerCharacteristic().stringValue().c_str();
            case 2:
                if (! surface->isCompact())
                    return i18n("Infinite");
                else if (surface->hasRealBoundary())
                    return i18n("Real Bdry");
                else
                    return i18n("Closed");
            case 3: {
                const regina::NVertex* v;
                std::pair<const regina::NEdge*, const regina::NEdge*> e;

                if ((v = surface->isVertexLink()))
                    return i18n("Vertex %1").arg(
                        surfaces->getTriangulation()->vertexIndex(v));
                else if ((e = surface->isThinEdgeLink()).first) {
                    if (e.second)
                        return i18n("Thin edges %1, %2").
                            arg(surfaces->getTriangulation()->edgeIndex(
                                e.first)).
                            arg(surfaces->getTriangulation()->edgeIndex(
                                e.second));
                    else
                        return i18n("Thin edge %1").
                            arg(surfaces->getTriangulation()->edgeIndex(
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
                if (column >= coordCols + nCols || column < 0)
                    return QString::null;

                regina::NLargeInteger ans = Coordinates::getCoordinate(
                    coordSystem, *surface, column - nCols);
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
     // Paint the cell in the correct colour.
     ItemColour colour = getColour(column);
     if (colour == Plain)
         GridListViewItem::paintCell(p, cg, column, width, align);
     else {
         QColorGroup altCg(cg);
         if (colour == Green)
             altCg.setColor(QColorGroup::Text, darkGreen);
         else if (colour == Yellow)
             altCg.setColor(QColorGroup::Text, darkYellow);
         else
             altCg.setColor(QColorGroup::Text, darkRed);
         GridListViewItem::paintCell(p, altCg, column, width, align);
     }
}

NSurfaceCoordinateItem::ItemColour NSurfaceCoordinateItem::getColour(
        int column) {
    if (surfaces->isEmbeddedOnly()) {
        regina::NTriBool triBool;
        switch (column) {
            case 2:
                if (! surface->isCompact())
                    return Plain;

                triBool = surface->isOrientable();
                if (triBool.isTrue())
                    return Green;
                else if (triBool.isFalse())
                    return Red;
                else
                    return Yellow;
            case 3:
                if (! surface->isCompact())
                    return Plain;

                triBool = surface->isTwoSided();
                if (triBool.isTrue())
                    return Green;
                else if (triBool.isFalse())
                    return Red;
                else
                    return Yellow;
            case 4:
                if (! surface->isCompact())
                    return Yellow;
                else if (surface->hasRealBoundary())
                    return Red;
                else
                    return Green;
        }
    } else {
        switch (column) {
            case 2:
                if (! surface->isCompact())
                    return Yellow;
                else if (surface->hasRealBoundary())
                    return Red;
                else
                    return Green;
        }
    }
    return Plain;
}

