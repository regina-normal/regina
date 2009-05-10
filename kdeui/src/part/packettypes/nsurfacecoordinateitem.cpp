
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
#include "maths/nlargeinteger.h"
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

unsigned NSurfaceCoordinateItem::propertyColCount(bool embeddedOnly,
        bool almostNormal) {
    return (embeddedOnly ? 8 : 6) + (almostNormal ? 1 : 0);
}

QString NSurfaceCoordinateItem::propertyColName(int whichCol,
        bool embeddedOnly, bool almostNormal) {
    if (embeddedOnly) {
        switch (whichCol) {
            case 0 : return QString(); // Surface number
            case 1 : return i18n("Name");
            case 2 : return i18n("Euler");
            case 3 : return i18n("Orient");
            case 4 : return i18n("Sides");
            case 5 : return i18n("Bdry");
            case 6 : return i18n("Link");
            case 7 : return i18n("Type");
        }
        if (whichCol == 8 && almostNormal)
            return i18n("Octagon");
    } else {
        switch (whichCol) {
            case 0 : return QString(); // Surface number
            case 1 : return i18n("Name");
            case 2 : return i18n("Euler");
            case 3 : return i18n("Bdry");
            case 4 : return i18n("Link");
            case 5 : return i18n("Type");
        }
        if (whichCol == 6 && almostNormal)
            return i18n("Octagon");
    }

    return i18n("Unknown");
}

QString NSurfaceCoordinateItem::propertyColDesc(int whichCol,
        bool embeddedOnly, bool almostNormal) {
    if (embeddedOnly) {
        switch (whichCol) {
            case 0: return i18n("The index of this surface within the "
                "overall list (surfaces are numbered 0,1,2,...)");
            case 1: return i18n("Name (this has no special meaning and "
                "can be edited)");
            case 2: return i18n("Euler characteristic");
            case 3: return i18n("Orientability");
            case 4: return i18n("1-sided or 2-sided");
            case 5: return i18n("Does this surface have boundary?");
            case 6: return i18n("Has this surface been identified as "
                "the link of a particular subcomplex?");
            case 7: return i18n("Other interesting properties");
        }
        if (whichCol == 8 && almostNormal)
            return i18n("The coordinate position containing the octagonal "
                "disc type, and the number of discs of that type");
    } else {
        switch (whichCol) {
            case 0: return i18n("The index of this surface within the "
                "overall list (surfaces are numbered 0,1,2,...)");
            case 1: return i18n("Name (this has no special meaning and "
                "can be edited)");
            case 2: return i18n("Euler characteristic");
            case 3: return i18n("Does this surface have boundary?");
            case 4: return i18n("Has this surface been identified as "
                "the link of a particular subcomplex?");
            case 5: return i18n("Other interesting properties");
        }
        if (whichCol == 6 && almostNormal)
            return i18n("The coordinate position containing the octagonal "
                "disc type, and the number of discs of that type");
    }

    return i18n("Unknown");
}

void NSurfaceCoordinateItem::setText(int column, const QString& str) {
    if (column == 1)
        name = str;
    KListViewItem::setText(column, str);
}

QString NSurfaceCoordinateItem::text(int column) const {
    const int nCols = propertyColCount(surfaces->isEmbeddedOnly(),
        surfaces->allowsAlmostNormal());

    regina::NTriBool triBool;
    if (surfaces->isEmbeddedOnly()) {
        switch (column) {
            case 0:
                return i18n("%1.").arg(surfaceIndex);
            case 1:
                return name;
            case 2:
                if (! surface->isCompact())
                    return QString::null;

                return surface->getEulerCharacteristic().stringValue().c_str();
            case 3:
                if (! surface->isCompact())
                    return QString::null;

                triBool = surface->isOrientable();
                if (triBool.isTrue())
                    return i18n("Orbl");
                else if (triBool.isFalse())
                    return i18n("Non-orbl");
                else
                    return i18n("Unknown");
            case 4:
                if (! surface->isCompact())
                    return QString::null;

                triBool = surface->isTwoSided();
                if (triBool.isTrue())
                    return "2";
                else if (triBool.isFalse())
                    return "1";
                else
                    return i18n("Unknown");
            case 5:
                if (! surface->isCompact())
                    return i18n("Infinite");
                else if (surface->hasRealBoundary())
                    return i18n("Real Bdry");
                else
                    return i18n("Closed");
            case 6: {
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
            case 7:
                {
                    regina::NLargeInteger tot;
                    if (surface->isSplitting())
                        return i18n("Splitting");
                    else if ((tot = surface->isCentral()) != 0)
                        return i18n("Central (%1)").arg(tot.longValue());
                    else
                        return QString::null;
                }
            case 8:
                if (surfaces->allowsAlmostNormal()) {
                    regina::NDiscType oct = surface->getOctPosition();
                    if (oct == regina::NDiscType::NONE)
                        return QString::null;
                    else {
                        regina::NLargeInteger tot = surface->getOctCoord(
                            oct.tetIndex, oct.type);
                        if (tot == 1) {
                            return i18n("K%1: %2 (1 oct)").
                                arg(oct.tetIndex).
                                arg(regina::vertexSplitString[oct.type]);
                        } else {
                            return i18n("K%1: %2 (%3 octs)").
                                arg(oct.tetIndex).
                                arg(regina::vertexSplitString[oct.type]).
                                arg(tot.stringValue());
                        }
                    }
                }
                // If we don't support almost normal surfaces, fall
                // through to the default case below.
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
        switch (column) {
            case 0:
                return i18n("%1.").arg(surfaceIndex);
            case 1:
                return name;
            case 2:
                if (! surface->isCompact())
                    return QString::null;

                return surface->getEulerCharacteristic().stringValue().c_str();
            case 3:
                if (! surface->isCompact())
                    return i18n("Infinite");
                else if (surface->hasRealBoundary())
                    return i18n("Real Bdry");
                else
                    return i18n("Closed");
            case 4: {
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
            case 5:
                if (surface->isSplitting())
                    return i18n("Splitting");
                else
                    return QString::null;
            case 6:
                if (surfaces->allowsAlmostNormal()) {
                    regina::NDiscType oct = surface->getOctPosition();
                    if (oct == regina::NDiscType::NONE)
                        return QString::null;
                    else {
                        regina::NLargeInteger tot = surface->getOctCoord(
                            oct.tetIndex, oct.type);
                        if (tot == 1) {
                            return i18n("1 oct at K%1: %2").
                                arg(oct.tetIndex).
                                arg(regina::vertexSplitString[oct.type]);
                        } else {
                            return i18n("%1 octs at K%2: %3").
                                arg(tot.stringValue()).
                                arg(oct.tetIndex).
                                arg(regina::vertexSplitString[oct.type]);
                        }
                    }
                }
                // If we don't support almost normal surfaces, fall
                // through to the default case below.
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
            case 3:
                if (! surface->isCompact())
                    return Plain;

                triBool = surface->isOrientable();
                if (triBool.isTrue())
                    return Green;
                else if (triBool.isFalse())
                    return Red;
                else
                    return Yellow;
            case 4:
                if (! surface->isCompact())
                    return Plain;

                triBool = surface->isTwoSided();
                if (triBool.isTrue())
                    return Green;
                else if (triBool.isFalse())
                    return Red;
                else
                    return Yellow;
            case 5:
                if (! surface->isCompact())
                    return Yellow;
                else if (surface->hasRealBoundary())
                    return Red;
                else
                    return Green;
        }
        if (column == 8 && surfaces->allowsAlmostNormal()) {
            regina::NDiscType oct = surface->getOctPosition();
            if (oct != regina::NDiscType::NONE) {
                if (surface->getOctCoord(oct.tetIndex, oct.type) > 1)
                    return Red;
                else
                    return Green;
            }
        }
    } else {
        switch (column) {
            case 3:
                if (! surface->isCompact())
                    return Yellow;
                else if (surface->hasRealBoundary())
                    return Red;
                else
                    return Green;
        }
        if (column == 6 && surfaces->allowsAlmostNormal()) {
            regina::NDiscType oct = surface->getOctPosition();
            if (oct != regina::NDiscType::NONE) {
                if (surface->getOctCoord(oct.tetIndex, oct.type) > 1)
                    return Red;
                else
                    return Green;
            }
        }
    }
    return Plain;
}

