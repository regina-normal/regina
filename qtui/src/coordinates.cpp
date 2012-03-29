
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
#include "triangulation/nface.h"
#include "triangulation/ntriangulation.h"

#include <QObject>
#include <QString>

using regina::NFace;
using regina::NNormalSurfaceList;

namespace Coordinates {
    const char* name(int coordSystem, bool capitalise) {
        if (capitalise) {
            if (coordSystem == NNormalSurfaceList::STANDARD)
                return QT_TR_NOOP("Standard normal (tri-quad)");
            if (coordSystem == NNormalSurfaceList::AN_STANDARD)
                return QT_TR_NOOP("Standard almost normal (tri-quad-oct)");
            if (coordSystem == NNormalSurfaceList::AN_LEGACY)
                return QT_TR_NOOP("Legacy almost normal (pruned tri-quad-oct)");
            if (coordSystem == NNormalSurfaceList::QUAD)
                return QT_TR_NOOP("Quad normal");
            if (coordSystem == NNormalSurfaceList::AN_QUAD_OCT)
                return QT_TR_NOOP("Quad-oct almost normal");
            if (coordSystem == NNormalSurfaceList::EDGE_WEIGHT)
                return QT_TR_NOOP("Edge weight");
            if (coordSystem == NNormalSurfaceList::FACE_ARCS)
                return QT_TR_NOOP("Face arc");
            if (coordSystem == NNormalSurfaceList::ORIENTED)
                return QT_TR_NOOP("Transversely oriented normal");
            if (coordSystem == NNormalSurfaceList::ORIENTED_QUAD)
                return QT_TR_NOOP("Transversely oriented quad normal");
            return QT_TR_NOOP("Unknown");
        } else {
            if (coordSystem == NNormalSurfaceList::STANDARD)
                return QT_TR_NOOP("standard normal (tri-quad)");
            if (coordSystem == NNormalSurfaceList::AN_STANDARD)
                return QT_TR_NOOP("standard almost normal (tri-quad-oct)");
            if (coordSystem == NNormalSurfaceList::AN_LEGACY)
                return QT_TR_NOOP("legacy almost normal (pruned tri-quad-oct)");
            if (coordSystem == NNormalSurfaceList::QUAD)
                return QT_TR_NOOP("quad normal");
            if (coordSystem == NNormalSurfaceList::AN_QUAD_OCT)
                return QT_TR_NOOP("quad-oct almost normal");
            if (coordSystem == NNormalSurfaceList::EDGE_WEIGHT)
                return QT_TR_NOOP("edge weight");
            if (coordSystem == NNormalSurfaceList::FACE_ARCS)
                return QT_TR_NOOP("face arc");
            if (coordSystem == NNormalSurfaceList::ORIENTED)
                return QT_TR_NOOP("transversely oriented normal");
            if (coordSystem == NNormalSurfaceList::ORIENTED_QUAD)
                return QT_TR_NOOP("transversely oriented quad normal");
            return QT_TR_NOOP("unknown");
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
        else if (coordSystem == NNormalSurfaceList::ORIENTED_QUAD)
            return tri->getNumberOfTetrahedra() * 6;
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
                return QString("T%1: %2").arg(whichCoord / 10).
                    arg(whichCoord % 10);
            else if (whichCoord % 10 < 7)
                return QString("Q%1: %2").arg(whichCoord / 10).
                    arg(regina::vertexSplitString[(whichCoord % 10) - 4]);
            else
                return QString("K%1: %2").arg(whichCoord / 10).
                    arg(regina::vertexSplitString[(whichCoord % 10) - 7]);
        } else if (coordSystem == NNormalSurfaceList::QUAD) {
            return QString("%1: %2").arg(whichCoord / 3).
                arg(regina::vertexSplitString[whichCoord % 3]);
        } else if (coordSystem == NNormalSurfaceList::AN_QUAD_OCT) {
            if (whichCoord % 6 < 3)
                return QString("Q%1: %2").arg(whichCoord / 6).
                    arg(regina::vertexSplitString[whichCoord % 6]);
            else
                return QString("K%1: %2").arg(whichCoord / 6).
                    arg(regina::vertexSplitString[(whichCoord % 6) - 3]);
        } else if (coordSystem == NNormalSurfaceList::EDGE_WEIGHT) {
            if (! (tri && tri->getEdge(whichCoord)->isBoundary()))
                return QString::number(whichCoord);
            else
                return QString("%1 [B]").arg(whichCoord);
        } else if (coordSystem == NNormalSurfaceList::FACE_ARCS) {
            return QString("%1: %2").arg(whichCoord / 3).arg(whichCoord % 3);
        } else if (coordSystem == NNormalSurfaceList::ORIENTED) {
            unsigned long stdCoord = whichCoord / 2;
            if (whichCoord % 2 == 0) {
                // "true" orientation.
                // Note that vertexSplitDefn[i][0] == 0 always.
                if (stdCoord % 7 < 4)
                    return QString("%1: %2").arg(stdCoord / 7).
                        arg(stdCoord % 7);
                else
                    return QString("%1: %2%3").arg(stdCoord / 7).
                        arg(regina::vertexSplitDefn[(stdCoord % 7) - 4][0]).
                        arg(regina::vertexSplitDefn[(stdCoord % 7) - 4][1]);
            } else {
                // "false" orientation.
                if (stdCoord % 7 < 4)
                    return QString("%1: %2").arg(stdCoord / 7).
                        arg(NFace::ordering[stdCoord % 7].trunc3().c_str());
                else
                    return QString("%1: %2%3").arg(stdCoord / 7).
                        arg(regina::vertexSplitDefn[(stdCoord % 7) - 4][2]).
                        arg(regina::vertexSplitDefn[(stdCoord % 7) - 4][3]);
            }
        } else if (coordSystem == NNormalSurfaceList::ORIENTED_QUAD) {
            unsigned long quadCoord = whichCoord / 2;
            if (whichCoord % 2 == 0) {
                // "true" orientation.
                // Note that vertexSplitDefn[i][0] == 0 always.
                return QString("%1: %2%3").arg(quadCoord / 3).
                    arg(regina::vertexSplitDefn[(quadCoord % 3)][0]).
                    arg(regina::vertexSplitDefn[(quadCoord % 3)][1]);
            } else {
                // "false" orientation.
                return QString("%1: %2%3").arg(quadCoord / 3).
                    arg(regina::vertexSplitDefn[(quadCoord % 3)][2]).
                    arg(regina::vertexSplitDefn[(quadCoord % 3)][3]);
            }
        }

        return QString("Unknown");
    }

    QString columnDesc(int coordSystem, unsigned long whichCoord,
            const QObject *context, regina::NTriangulation* tri) {
        if (coordSystem == NNormalSurfaceList::STANDARD) {
            if (whichCoord % 7 < 4)
                return context->tr("Tetrahedron %1, triangle about vertex %2").
                    arg(whichCoord / 7).arg(whichCoord % 7);
            else
                return context->tr("Tetrahedron %1, quad splitting vertices %2").
                    arg(whichCoord / 7).
                    arg(regina::vertexSplitString[(whichCoord % 7) - 4]);
        } else if (coordSystem == NNormalSurfaceList::AN_STANDARD ||
                coordSystem == NNormalSurfaceList::AN_LEGACY) {
            if (whichCoord % 10 < 4)
                return context->tr("Tetrahedron %1, triangle about vertex %2").
                    arg(whichCoord / 10).arg(whichCoord % 10);
            else if (whichCoord % 10 < 7)
                return context->tr("Tetrahedron %1, quad splitting vertices %2").
                    arg(whichCoord / 10).
                    arg(regina::vertexSplitString[(whichCoord % 10) - 4]);
            else
                return context->tr("Tetrahedron %1, oct partitioning vertices %2").
                    arg(whichCoord / 10).
                    arg(regina::vertexSplitString[(whichCoord % 10) - 7]);
        } else if (coordSystem == NNormalSurfaceList::QUAD) {
            return context->tr("Tetrahedron %1, quad splitting vertices %2").
                arg(whichCoord / 3).
                arg(regina::vertexSplitString[whichCoord % 3]);
        } else if (coordSystem == NNormalSurfaceList::AN_QUAD_OCT) {
            if (whichCoord % 6 < 3)
                return context->tr("Tetrahedron %1, quad splitting vertices %2").
                    arg(whichCoord / 6).
                    arg(regina::vertexSplitString[whichCoord % 6]);
            else
                return context->tr("Tetrahedron %1, oct partitioning vertices %2").
                    arg(whichCoord / 6).
                    arg(regina::vertexSplitString[(whichCoord % 6) - 3]);
        } else if (coordSystem == NNormalSurfaceList::EDGE_WEIGHT) {
            if (tri) {
                if (tri->getEdge(whichCoord)->isBoundary())
                    return context->tr("Weight of (boundary) edge %1").
                        arg(whichCoord);
                else
                    return context->tr("Weight of (internal) edge %1").
                        arg(whichCoord);
            } else
                return context->tr("Weight of edge %1").arg(whichCoord);
        } else if (coordSystem == NNormalSurfaceList::FACE_ARCS) {
            return context->tr("Arcs on face %1 crossing face vertex %2").
                arg(whichCoord / 3).arg(whichCoord % 3);
        } else if (coordSystem == NNormalSurfaceList::ORIENTED) {
            unsigned long stdCoord = whichCoord / 2;
            if (whichCoord % 2 == 0) {
                // "true" orientation.
                // Note that vertexSplitDefn[i][0] == 0 always.
                if (stdCoord % 7 < 4)
                    return context->tr("Tetrahedron %1, "
                        "triangle oriented towards vertex %2").
                        arg(stdCoord / 7).
                        arg(stdCoord % 7);
                else
                    return context->tr("Tetrahedron %1, "
                        "quad oriented towards edge %2%3").
                        arg(stdCoord / 7).
                        arg(regina::vertexSplitDefn[(stdCoord % 7) - 4][0]).
                        arg(regina::vertexSplitDefn[(stdCoord % 7) - 4][1]);
            } else {
                // "false" orientation.
                if (stdCoord % 7 < 4)
                    return context->tr("Tetrahedron %1, "
                        "triangle oriented towards face %2").
                        arg(stdCoord / 7).
                        arg(NFace::ordering[stdCoord % 7].trunc3().c_str());
                else
                    return context->tr("Tetrahedron %1, "
                        "quad oriented towards edge %2%3").
                        arg(stdCoord / 7).
                        arg(regina::vertexSplitDefn[(stdCoord % 7) - 4][2]).
                        arg(regina::vertexSplitDefn[(stdCoord % 7) - 4][3]);
            }
        } else if (coordSystem == NNormalSurfaceList::ORIENTED_QUAD) {
            unsigned long quadCoord = whichCoord / 2;
            if (whichCoord % 2 == 0) {
                // "true" orientation.
                // Note that vertexSplitDefn[i][0] == 0 always.
                return context->tr("Tetrahedron %1, "
                    "quad oriented towards edge %2%3").
                    arg(quadCoord / 3).
                    arg(regina::vertexSplitDefn[(quadCoord % 3)][0]).
                    arg(regina::vertexSplitDefn[(quadCoord % 3)][1]);
            } else {
                // "false" orientation.
                return context->tr("Tetrahedron %1, "
                    "quad oriented towards edge %2%3").
                    arg(quadCoord / 3).
                    arg(regina::vertexSplitDefn[(quadCoord % 3)][2]).
                    arg(regina::vertexSplitDefn[(quadCoord % 3)][3]);
            }
        }

        return context->tr("This coordinate system is not known");
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
            bool orientation = (whichCoord % 2 == 0);
            whichCoord /= 2;
            if (whichCoord % 7 < 4)
                return surface.getOrientedTriangleCoord(
                    whichCoord / 7, whichCoord % 7, orientation);
            else
                return surface.getOrientedQuadCoord(
                    whichCoord / 7, (whichCoord % 7) - 4, orientation);
        } else if (coordSystem == NNormalSurfaceList::ORIENTED_QUAD) {
            bool orientation = (whichCoord % 2 == 0);
            whichCoord /= 2;
            return surface.getOrientedQuadCoord(
                whichCoord / 3, whichCoord % 3, orientation);
        }

        return (long)0;
    }
};

