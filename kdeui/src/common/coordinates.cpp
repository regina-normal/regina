
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

#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"

#include <klocale.h>

using regina::NNormalSurfaceList;

namespace Coordinates {
    QString name(int coordSystem, bool capitalise) {
        if (capitalise) {
            if (coordSystem == NNormalSurfaceList::STANDARD)
                return i18n("Standard normal (tri-quad)");
            if (coordSystem == NNormalSurfaceList::AN_STANDARD)
                return i18n("Standard almost normal (tri-quad-oct)");
            if (coordSystem == NNormalSurfaceList::AN_LEGACY)
                return i18n("Legacy almost normal (pruned tri-quad-oct)");
            if (coordSystem == NNormalSurfaceList::QUAD)
                return i18n("Quad normal");
            if (coordSystem == NNormalSurfaceList::AN_QUAD_OCT)
                return i18n("Quad-oct almost normal");
            if (coordSystem == NNormalSurfaceList::EDGE_WEIGHT)
                return i18n("Edge weight");
            if (coordSystem == NNormalSurfaceList::FACE_ARCS)
                return i18n("Face arc");
            if (coordSystem == NNormalSurfaceList::ORIENTED)
                return i18n("Transversely oriented normal (tri-quad)");
            if (coordSystem == NNormalSurfaceList::ORIENTED_QUAD)
                return i18n("Transversely oriented quad normal (quad)");
            return i18n("Unknown");
        } else {
            if (coordSystem == NNormalSurfaceList::STANDARD)
                return i18n("standard normal (tri-quad)");
            if (coordSystem == NNormalSurfaceList::AN_STANDARD)
                return i18n("standard almost normal (tri-quad-oct)");
            if (coordSystem == NNormalSurfaceList::AN_LEGACY)
                return i18n("legacy almost normal (pruned tri-quad-oct)");
            if (coordSystem == NNormalSurfaceList::QUAD)
                return i18n("quad normal");
            if (coordSystem == NNormalSurfaceList::AN_QUAD_OCT)
                return i18n("quad-oct almost normal");
            if (coordSystem == NNormalSurfaceList::EDGE_WEIGHT)
                return i18n("edge weight");
            if (coordSystem == NNormalSurfaceList::FACE_ARCS)
                return i18n("face arc");
            if (coordSystem == NNormalSurfaceList::ORIENTED)
                return i18n("transversely oriented normal (tri-quad)");
            if (coordSystem == NNormalSurfaceList::ORIENTED_QUAD)
                return i18n("transversely oriented quad normal (quad)");
            return i18n("unknown");
        }
    }

    bool generatesAlmostNormal(int coordSystem) {
        return (coordSystem == NNormalSurfaceList::AN_STANDARD ||
                coordSystem == NNormalSurfaceList::AN_LEGACY ||
                coordSystem == NNormalSurfaceList::AN_QUAD_OCT);
    }

    unsigned long numColumns(int coordSystem, regina::NTriangulation* tri) {
        if (coordSystem == NNormalSurfaceList::STANDARD)
            return tri->getNumberOfTetrahedra() * 7;
        else if (coordSystem == NNormalSurfaceList::AN_STANDARD)
            return tri->getNumberOfTetrahedra() * 10;
        else if (coordSystem == NNormalSurfaceList::AN_LEGACY)
            return tri->getNumberOfTetrahedra() * 10;
        else if (coordSystem == NNormalSurfaceList::QUAD)
            return tri->getNumberOfTetrahedra() * 3;
        else if (coordSystem == NNormalSurfaceList::AN_QUAD_OCT)
            return tri->getNumberOfTetrahedra() * 6;
        else if (coordSystem == NNormalSurfaceList::EDGE_WEIGHT)
            return tri->getNumberOfEdges();
        else if (coordSystem == NNormalSurfaceList::FACE_ARCS)
            return tri->getNumberOfFaces() * 3;
        else if (coordSystem == NNormalSurfaceList::ORIENTED)
            return tri->getNumberOfTetrahedra() * 14;
        else if (coordSystem == NNormalSurfaceList::ORIENTED)
            return tri->getNumberOfTetrahedra() * 8;
        else
            return 0;
    }

    QString columnName(int coordSystem, unsigned long whichCoord,
            regina::NTriangulation* tri) {
        if (coordSystem == NNormalSurfaceList::STANDARD) {
            if (whichCoord % 7 < 4)
                return QString("%1: %2").arg(whichCoord / 7).
                    arg(whichCoord % 7);
            else
                return QString("%1: %2").arg(whichCoord / 7).
                    arg(regina::vertexSplitString[(whichCoord % 7) - 4]);
        } else if (coordSystem == NNormalSurfaceList::AN_STANDARD ||
                coordSystem == NNormalSurfaceList::AN_LEGACY) {
            if (whichCoord % 10 < 4)
                return i18n("T%1: %2").arg(whichCoord / 10).
                    arg(whichCoord % 10);
            else if (whichCoord % 10 < 7)
                return i18n("Q%1: %2").arg(whichCoord / 10).
                    arg(regina::vertexSplitString[(whichCoord % 10) - 4]);
            else
                return i18n("K%1: %2").arg(whichCoord / 10).
                    arg(regina::vertexSplitString[(whichCoord % 10) - 7]);
        } else if (coordSystem == NNormalSurfaceList::QUAD) {
            return QString("%1: %2").arg(whichCoord / 3).
                arg(regina::vertexSplitString[whichCoord % 3]);
        } else if (coordSystem == NNormalSurfaceList::AN_QUAD_OCT) {
            if (whichCoord % 6 < 3)
                return i18n("Q%1: %2").arg(whichCoord / 6).
                    arg(regina::vertexSplitString[whichCoord % 6]);
            else
                return i18n("K%1: %2").arg(whichCoord / 6).
                    arg(regina::vertexSplitString[(whichCoord % 6) - 3]);
        } else if (coordSystem == NNormalSurfaceList::EDGE_WEIGHT) {
            if (! (tri && tri->getEdge(whichCoord)->isBoundary()))
                return QString::number(whichCoord);
            else
                return i18n("%1 [B]").arg(whichCoord);
        } else if (coordSystem == NNormalSurfaceList::FACE_ARCS) {
            return QString("%1: %2").arg(whichCoord / 3).arg(whichCoord % 3);
        } else if (coordSystem == NNormalSurfaceList::ORIENTED) {
            QString orientation = "+";
            unsigned long coord = whichCoord;
            if (whichCoord % 2 == 1) {
                coord--;
                orientation = "-";
            }
            coord = coord / 2;
            if (coord % 7 < 4)
                return QString("%1: %2 (%3)").arg(coord / 7).
                    arg(coord % 7).
                    arg(orientation);
            else
                return QString("%1: %2 (%3)").arg(coord / 7).
                    arg(regina::vertexSplitString[(coord % 7) - 4]).
                    arg(orientation);
        } else if (coordSystem == NNormalSurfaceList::ORIENTED_QUAD) {
            QString orientation = "+";
            unsigned long coord = whichCoord;
            if (whichCoord % 2 == 1) {
                coord--;
                orientation = "-";
            }
            coord = coord / 2;
            return QString("%1: %2 (%3)").arg(coord / 3).
                arg(regina::vertexSplitString[coord % 3]).
                arg(orientation);
        }

        return i18n("Unknown");
    }

    QString columnDesc(int coordSystem, unsigned long whichCoord,
            regina::NTriangulation* tri) {
        if (coordSystem == NNormalSurfaceList::STANDARD) {
            if (whichCoord % 7 < 4)
                return i18n("Tetrahedron %1, triangle about vertex %2").
                    arg(whichCoord / 7).arg(whichCoord % 7);
            else
                return i18n("Tetrahedron %1, quad splitting vertices %2").
                    arg(whichCoord / 7).
                    arg(regina::vertexSplitString[(whichCoord % 7) - 4]);
        } else if (coordSystem == NNormalSurfaceList::AN_STANDARD ||
                coordSystem == NNormalSurfaceList::AN_LEGACY) {
            if (whichCoord % 10 < 4)
                return i18n("Tetrahedron %1, triangle about vertex %2").
                    arg(whichCoord / 10).arg(whichCoord % 10);
            else if (whichCoord % 10 < 7)
                return i18n("Tetrahedron %1, quad splitting vertices %2").
                    arg(whichCoord / 10).
                    arg(regina::vertexSplitString[(whichCoord % 10) - 4]);
            else
                return i18n("Tetrahedron %1, oct partitioning vertices %2").
                    arg(whichCoord / 10).
                    arg(regina::vertexSplitString[(whichCoord % 10) - 7]);
        } else if (coordSystem == NNormalSurfaceList::QUAD) {
            return i18n("Tetrahedron %1, quad splitting vertices %2").
                arg(whichCoord / 3).
                arg(regina::vertexSplitString[whichCoord % 3]);
        } else if (coordSystem == NNormalSurfaceList::AN_QUAD_OCT) {
            if (whichCoord % 6 < 3)
                return i18n("Tetrahedron %1, quad splitting vertices %2").
                    arg(whichCoord / 6).
                    arg(regina::vertexSplitString[whichCoord % 6]);
            else
                return i18n("Tetrahedron %1, oct partitioning vertices %2").
                    arg(whichCoord / 6).
                    arg(regina::vertexSplitString[(whichCoord % 6) - 3]);
        } else if (coordSystem == NNormalSurfaceList::EDGE_WEIGHT) {
            if (tri) {
                if (tri->getEdge(whichCoord)->isBoundary())
                    return i18n("Weight of (boundary) edge %1").arg(whichCoord);
                else
                    return i18n("Weight of (internal) edge %1").arg(whichCoord);
            } else
                return i18n("Weight of edge %1").arg(whichCoord);
        } else if (coordSystem == NNormalSurfaceList::FACE_ARCS) {
            return i18n("Arcs on face %1 crossing face vertex %2").
                arg(whichCoord / 3).arg(whichCoord % 3);
        } else if (coordSystem == NNormalSurfaceList::ORIENTED) {
            QString orientation = "+";
            unsigned long coord = whichCoord;
            if (whichCoord % 2 == 1) {
                coord--;
                orientation = "-";
            }
            coord = coord / 2;
            if (coord % 7 < 4)
                return i18n("Tetrahedron %1, triangle about vertex %2, "
                    "orientation %3").
                    arg(coord / 7).arg(coord % 7).
                    arg(orientation);
            else
                return i18n("Tetrahedron %1, quad splitting vertices %2, "
                    "orientation %3").
                    arg(coord / 7).
                    arg(regina::vertexSplitString[(coord % 7) - 4]).
                    arg(orientation);
        } else if (coordSystem == NNormalSurfaceList::ORIENTED_QUAD) {
            QString orientation = "+";
            unsigned long coord = whichCoord;
            if (whichCoord % 2 == 1) {
                coord--;
                orientation = "-";
            }
            coord = coord / 2;
            return i18n("Tetrahedron %1, quad splitting vertices %2, "
                "orientation %3").
                arg(coord / 3).
                arg(regina::vertexSplitString[coord % 3]).
                arg(orientation);
        }

        return i18n("This coordinate system is not known");
    }

    regina::NLargeInteger getCoordinate(int coordSystem,
            const regina::NNormalSurface& surface, unsigned long whichCoord) {
        if (coordSystem == NNormalSurfaceList::STANDARD) {
            if (whichCoord % 7 < 4)
                return surface.getTriangleCoord(
                    whichCoord / 7, whichCoord % 7);
            else
                return surface.getQuadCoord(
                    whichCoord / 7, (whichCoord % 7) - 4);
        } else if (coordSystem == NNormalSurfaceList::AN_STANDARD ||
                coordSystem == NNormalSurfaceList::AN_LEGACY) {
            if (whichCoord % 10 < 4)
                return surface.getTriangleCoord(
                    whichCoord / 10, whichCoord % 10);
            else if (whichCoord % 10 < 7)
                return surface.getQuadCoord(
                    whichCoord / 10, (whichCoord % 10) - 4);
            else
                return surface.getOctCoord(
                    whichCoord / 10, (whichCoord % 10) - 7);
        } else if (coordSystem == NNormalSurfaceList::QUAD) {
            return surface.getQuadCoord(whichCoord / 3, whichCoord % 3);
        } else if (coordSystem == NNormalSurfaceList::AN_QUAD_OCT) {
            if (whichCoord % 6 < 3)
                return surface.getQuadCoord(whichCoord / 6, whichCoord % 6);
            else
                return surface.getOctCoord(
                    whichCoord / 6, (whichCoord % 6) - 3);
        } else if (coordSystem == NNormalSurfaceList::EDGE_WEIGHT) {
            return surface.getEdgeWeight(whichCoord);
        } else if (coordSystem == NNormalSurfaceList::FACE_ARCS) {
            return surface.getFaceArcs(whichCoord / 3, whichCoord % 3);
        } else if (coordSystem == NNormalSurfaceList::ORIENTED) {
            bool orientation = true;
            unsigned long coord = whichCoord;
            if (whichCoord % 2 == 1) {
                coord--;
                orientation = false;
            }
            coord = coord / 2;
            if (coord % 7 < 4)
                return surface.getTriangleCoord(
                    coord / 7, coord % 7,orientation);
            else
                return surface.getQuadCoord(
                    coord / 7, (coord % 7) - 4,orientation);
        } else if (coordSystem == NNormalSurfaceList::ORIENTED_QUAD) {
            bool orientation = true;
            unsigned long coord = whichCoord;
            if (whichCoord % 2 == 1) {
                coord--;
                orientation = false;
            }
            coord = coord / 2;
            return surface.getQuadCoord(whichCoord / 3, coord % 3, orientation);
        }

        return (long)0;
    }
};

