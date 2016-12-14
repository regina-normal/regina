
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
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

#include "hypersurface/normalhypersurfaces.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

#include "reginaprefset.h"

#include <QObject>
#include <QString>

using regina::HyperCoords;
using regina::NormalSurfaces;
using regina::NormalCoords;
using regina::Triangle;

namespace Coordinates {
    const char* name(NormalCoords coordSystem, bool capitalise) {
        if (capitalise) {
            if (coordSystem == regina::NS_STANDARD)
                return QT_TR_NOOP("Standard normal (tri-quad)");
            if (coordSystem == regina::NS_AN_STANDARD)
                return QT_TR_NOOP("Standard almost normal (tri-quad-oct)");
            if (coordSystem == regina::NS_AN_LEGACY)
                return QT_TR_NOOP("Legacy almost normal (pruned tri-quad-oct)");
            if (coordSystem == regina::NS_QUAD)
                return QT_TR_NOOP("Quad normal");
            if (coordSystem == regina::NS_AN_QUAD_OCT)
                return QT_TR_NOOP("Quad-oct almost normal");
            if (coordSystem == regina::NS_EDGE_WEIGHT)
                return QT_TR_NOOP("Edge weight");
            if (coordSystem == regina::NS_TRIANGLE_ARCS)
                return QT_TR_NOOP("Triangle arc");
            if (coordSystem == regina::NS_ORIENTED)
                return QT_TR_NOOP("Transversely oriented normal");
            if (coordSystem == regina::NS_ORIENTED_QUAD)
                return QT_TR_NOOP("Transversely oriented quad normal");
            return QT_TR_NOOP("Unknown");
        } else {
            if (coordSystem == regina::NS_STANDARD)
                return QT_TR_NOOP("standard normal (tri-quad)");
            if (coordSystem == regina::NS_AN_STANDARD)
                return QT_TR_NOOP("standard almost normal (tri-quad-oct)");
            if (coordSystem == regina::NS_AN_LEGACY)
                return QT_TR_NOOP("legacy almost normal (pruned tri-quad-oct)");
            if (coordSystem == regina::NS_QUAD)
                return QT_TR_NOOP("quad normal");
            if (coordSystem == regina::NS_AN_QUAD_OCT)
                return QT_TR_NOOP("quad-oct almost normal");
            if (coordSystem == regina::NS_EDGE_WEIGHT)
                return QT_TR_NOOP("edge weight");
            if (coordSystem == regina::NS_TRIANGLE_ARCS)
                return QT_TR_NOOP("triangle arc");
            if (coordSystem == regina::NS_ORIENTED)
                return QT_TR_NOOP("transversely oriented normal");
            if (coordSystem == regina::NS_ORIENTED_QUAD)
                return QT_TR_NOOP("transversely oriented quad normal");
            return QT_TR_NOOP("unknown");
        }
    }

    const char* adjective(NormalCoords coordSystem, bool capitalise) {
        if (capitalise) {
            if (coordSystem == regina::NS_STANDARD)
                return QT_TR_NOOP("Standard");
            if (coordSystem == regina::NS_AN_STANDARD)
                return QT_TR_NOOP("Almost normal");
            if (coordSystem == regina::NS_AN_LEGACY)
                return QT_TR_NOOP("Legacy");
            if (coordSystem == regina::NS_QUAD)
                return QT_TR_NOOP("Quad");
            if (coordSystem == regina::NS_AN_QUAD_OCT)
                return QT_TR_NOOP("Quad-oct");
            if (coordSystem == regina::NS_EDGE_WEIGHT)
                return QT_TR_NOOP("Edge weight");
            if (coordSystem == regina::NS_TRIANGLE_ARCS)
                return QT_TR_NOOP("Triangle arc");
            if (coordSystem == regina::NS_ORIENTED)
                return QT_TR_NOOP("Transversely oriented");
            if (coordSystem == regina::NS_ORIENTED_QUAD)
                return QT_TR_NOOP("Transversely oriented quad");
            return QT_TR_NOOP("Unknown");
        } else {
            if (coordSystem == regina::NS_STANDARD)
                return QT_TR_NOOP("standard");
            if (coordSystem == regina::NS_AN_STANDARD)
                return QT_TR_NOOP("almost normal");
            if (coordSystem == regina::NS_AN_LEGACY)
                return QT_TR_NOOP("legacy");
            if (coordSystem == regina::NS_QUAD)
                return QT_TR_NOOP("quad");
            if (coordSystem == regina::NS_AN_QUAD_OCT)
                return QT_TR_NOOP("quad-oct");
            if (coordSystem == regina::NS_EDGE_WEIGHT)
                return QT_TR_NOOP("edge weight");
            if (coordSystem == regina::NS_TRIANGLE_ARCS)
                return QT_TR_NOOP("triangle arc");
            if (coordSystem == regina::NS_ORIENTED)
                return QT_TR_NOOP("transversely oriented");
            if (coordSystem == regina::NS_ORIENTED_QUAD)
                return QT_TR_NOOP("transversely oriented quad");
            return QT_TR_NOOP("unknown");
        }
    }

    const char* name(HyperCoords coordSystem, bool capitalise) {
        if (capitalise) {
            if (coordSystem == regina::HS_STANDARD)
                return QT_TR_NOOP("Standard normal (tet-prism)");
            if (coordSystem == regina::HS_PRISM)
                return QT_TR_NOOP("Prism normal");
            if (coordSystem == regina::HS_EDGE_WEIGHT)
                return QT_TR_NOOP("Edge weight");
            return QT_TR_NOOP("Unknown");
        } else {
            if (coordSystem == regina::HS_STANDARD)
                return QT_TR_NOOP("standard normal (tet-prism)");
            if (coordSystem == regina::HS_PRISM)
                return QT_TR_NOOP("prism normal");
            if (coordSystem == regina::HS_EDGE_WEIGHT)
                return QT_TR_NOOP("edge weight");
            return QT_TR_NOOP("unknown");
        }
    }

    const char* adjective(HyperCoords coordSystem, bool capitalise) {
        if (capitalise) {
            if (coordSystem == regina::HS_STANDARD)
                return QT_TR_NOOP("Standard");
            if (coordSystem == regina::HS_PRISM)
                return QT_TR_NOOP("Prism");
            if (coordSystem == regina::HS_EDGE_WEIGHT)
                return QT_TR_NOOP("Edge weight");
            return QT_TR_NOOP("Unknown");
        } else {
            if (coordSystem == regina::HS_STANDARD)
                return QT_TR_NOOP("standard");
            if (coordSystem == regina::HS_PRISM)
                return QT_TR_NOOP("prism");
            if (coordSystem == regina::HS_EDGE_WEIGHT)
                return QT_TR_NOOP("edge weight");
            return QT_TR_NOOP("unknown");
        }
    }

    bool generatesAlmostNormal(NormalCoords coordSystem) {
        return (coordSystem == regina::NS_AN_STANDARD ||
                coordSystem == regina::NS_AN_LEGACY ||
                coordSystem == regina::NS_AN_QUAD_OCT);
    }

    size_t numColumns(NormalCoords coordSystem, regina::Triangulation<3>* tri) {
        if (coordSystem == regina::NS_STANDARD)
            return tri->size() * 7;
        else if (coordSystem == regina::NS_AN_STANDARD)
            return tri->size() * 10;
        else if (coordSystem == regina::NS_AN_LEGACY)
            return tri->size() * 10;
        else if (coordSystem == regina::NS_QUAD)
            return tri->size() * 3;
        else if (coordSystem == regina::NS_AN_QUAD_OCT)
            return tri->size() * 6;
        else if (coordSystem == regina::NS_EDGE_WEIGHT)
            return tri->countEdges();
        else if (coordSystem == regina::NS_TRIANGLE_ARCS)
            return tri->countTriangles() * 3;
        else if (coordSystem == regina::NS_ORIENTED)
            return tri->size() * 14;
        else if (coordSystem == regina::NS_ORIENTED_QUAD)
            return tri->size() * 6;
        else
            return 0;
    }

    size_t numColumns(HyperCoords coordSystem, regina::Triangulation<4>* tri) {
        if (coordSystem == regina::HS_STANDARD)
            return tri->size() * 15;
        else if (coordSystem == regina::HS_PRISM)
            return tri->size() * 10;
        else if (coordSystem == regina::HS_EDGE_WEIGHT)
            return tri->countEdges();
        else
            return 0;
    }

    QString columnName(NormalCoords coordSystem, size_t whichCoord,
            regina::Triangulation<3>* tri) {
        if (coordSystem == regina::NS_STANDARD) {
            if (whichCoord % 7 < 4)
                return QString("%1: %2").arg(whichCoord / 7).
                    arg(whichCoord % 7);
            else
                return QString("%1: %2").arg(whichCoord / 7).
                    arg(regina::quadString[(whichCoord % 7) - 4]);
        } else if (coordSystem == regina::NS_AN_STANDARD ||
                coordSystem == regina::NS_AN_LEGACY) {
            if (whichCoord % 10 < 4)
                return QString("T%1: %2").arg(whichCoord / 10).
                    arg(whichCoord % 10);
            else if (whichCoord % 10 < 7)
                return QString("Q%1: %2").arg(whichCoord / 10).
                    arg(regina::quadString[(whichCoord % 10) - 4]);
            else
                return QString("K%1: %2").arg(whichCoord / 10).
                    arg(regina::quadString[(whichCoord % 10) - 7]);
        } else if (coordSystem == regina::NS_QUAD) {
            return QString("%1: %2").arg(whichCoord / 3).
                arg(regina::quadString[whichCoord % 3]);
        } else if (coordSystem == regina::NS_AN_QUAD_OCT) {
            if (whichCoord % 6 < 3)
                return QString("Q%1: %2").arg(whichCoord / 6).
                    arg(regina::quadString[whichCoord % 6]);
            else
                return QString("K%1: %2").arg(whichCoord / 6).
                    arg(regina::quadString[(whichCoord % 6) - 3]);
        } else if (coordSystem == regina::NS_EDGE_WEIGHT) {
            if (! (tri && tri->edge(whichCoord)->isBoundary()))
                return QString::number(whichCoord);
            else if (ReginaPrefSet::global().displayUnicode)
                return QString("%1: \u2202").arg(whichCoord);
            else
                return QString("%1 (B)").arg(whichCoord);
        } else if (coordSystem == regina::NS_TRIANGLE_ARCS) {
            if (! (tri && tri->triangle(whichCoord / 3)->isBoundary()))
                return QString("%1: %2").
                    arg(whichCoord / 3).arg(whichCoord % 3);
            else if (ReginaPrefSet::global().displayUnicode)
                return QString("\u2202 %1: %2").
                    arg(whichCoord / 3).arg(whichCoord % 3);
            else
                return QString("B %1: %2").
                    arg(whichCoord / 3).arg(whichCoord % 3);
        } else if (coordSystem == regina::NS_ORIENTED) {
            size_t stdCoord = whichCoord / 2;
            if (whichCoord % 2 == 0) {
                // "true" orientation.
                // Note that quadDefn[i][0] == 0 always.
                if (stdCoord % 7 < 4)
                    return QString("%1: %2").arg(stdCoord / 7).
                        arg(stdCoord % 7);
                else
                    return QString("%1: %2%3").arg(stdCoord / 7).
                        arg(regina::quadDefn[(stdCoord % 7) - 4][0]).
                        arg(regina::quadDefn[(stdCoord % 7) - 4][1]);
            } else {
                // "false" orientation.
                if (stdCoord % 7 < 4)
                    return QString("%1: %2").arg(stdCoord / 7).
                        arg(Triangle<3>::ordering(stdCoord % 7).trunc3().c_str());
                else
                    return QString("%1: %2%3").arg(stdCoord / 7).
                        arg(regina::quadDefn[(stdCoord % 7) - 4][2]).
                        arg(regina::quadDefn[(stdCoord % 7) - 4][3]);
            }
        } else if (coordSystem == regina::NS_ORIENTED_QUAD) {
            size_t quadCoord = whichCoord / 2;
            if (whichCoord % 2 == 0) {
                // "true" orientation.
                // Note that quadDefn[i][0] == 0 always.
                return QString("%1: %2%3").arg(quadCoord / 3).
                    arg(regina::quadDefn[(quadCoord % 3)][0]).
                    arg(regina::quadDefn[(quadCoord % 3)][1]);
            } else {
                // "false" orientation.
                return QString("%1: %2%3").arg(quadCoord / 3).
                    arg(regina::quadDefn[(quadCoord % 3)][2]).
                    arg(regina::quadDefn[(quadCoord % 3)][3]);
            }
        }

        return QString("Unknown");
    }

    QString columnName(HyperCoords coordSystem, size_t whichCoord,
            regina::Triangulation<4>* tri) {
        if (coordSystem == regina::HS_STANDARD) {
            if (whichCoord % 15 < 5)
                return QString("%1: %2").arg(whichCoord / 15).
                    arg(whichCoord % 15);
            else
                return QString("%1: %2%3").arg(whichCoord / 15).
                    arg(regina::FaceNumbering<4, 1>::edgeVertex
                        [(whichCoord % 15) - 5][0]).
                    arg(regina::FaceNumbering<4, 1>::edgeVertex
                        [(whichCoord % 15) - 5][1]);
        } else if (coordSystem == regina::HS_PRISM) {
            return QString("%1: %2%3").arg(whichCoord / 10).
                arg(regina::FaceNumbering<4, 1>::edgeVertex
                    [whichCoord % 10][0]).
                arg(regina::FaceNumbering<4, 1>::edgeVertex
                    [whichCoord % 10][1]);
        } else if (coordSystem == regina::HS_EDGE_WEIGHT) {
            if (! (tri && tri->edge(whichCoord)->isBoundary()))
                return QString::number(whichCoord);
            else if (ReginaPrefSet::global().displayUnicode)
                return QString("%1: \u2202").arg(whichCoord);
            else
                return QString("%1 (B)").arg(whichCoord);
        }

        return QString("Unknown");
    }

    QString columnDesc(NormalCoords coordSystem, size_t whichCoord,
            const QObject *context, regina::Triangulation<3>* tri) {
        if (coordSystem == regina::NS_STANDARD) {
            if (whichCoord % 7 < 4)
                return context->tr("Tetrahedron %1, triangle about vertex %2").
                    arg(whichCoord / 7).arg(whichCoord % 7);
            else
                return context->tr("Tetrahedron %1, quad splitting vertices %2").
                    arg(whichCoord / 7).
                    arg(regina::quadString[(whichCoord % 7) - 4]);
        } else if (coordSystem == regina::NS_AN_STANDARD ||
                coordSystem == regina::NS_AN_LEGACY) {
            if (whichCoord % 10 < 4)
                return context->tr("Tetrahedron %1, triangle about vertex %2").
                    arg(whichCoord / 10).arg(whichCoord % 10);
            else if (whichCoord % 10 < 7)
                return context->tr("Tetrahedron %1, quad splitting vertices %2").
                    arg(whichCoord / 10).
                    arg(regina::quadString[(whichCoord % 10) - 4]);
            else
                return context->tr("Tetrahedron %1, oct partitioning vertices %2").
                    arg(whichCoord / 10).
                    arg(regina::quadString[(whichCoord % 10) - 7]);
        } else if (coordSystem == regina::NS_QUAD) {
            return context->tr("Tetrahedron %1, quad splitting vertices %2").
                arg(whichCoord / 3).
                arg(regina::quadString[whichCoord % 3]);
        } else if (coordSystem == regina::NS_AN_QUAD_OCT) {
            if (whichCoord % 6 < 3)
                return context->tr("Tetrahedron %1, quad splitting vertices %2").
                    arg(whichCoord / 6).
                    arg(regina::quadString[whichCoord % 6]);
            else
                return context->tr("Tetrahedron %1, oct partitioning vertices %2").
                    arg(whichCoord / 6).
                    arg(regina::quadString[(whichCoord % 6) - 3]);
        } else if (coordSystem == regina::NS_EDGE_WEIGHT) {
            if (tri) {
                if (tri->edge(whichCoord)->isBoundary())
                    return context->tr("Weight of (boundary) edge %1").
                        arg(whichCoord);
                else
                    return context->tr("Weight of (internal) edge %1").
                        arg(whichCoord);
            } else
                return context->tr("Weight of edge %1").arg(whichCoord);
        } else if (coordSystem == regina::NS_TRIANGLE_ARCS) {
            if (! (tri && tri->triangle(whichCoord / 3)->isBoundary()))
                return context->tr(
                    "Arcs on (internal) triangle %1 crossing triangle vertex %2").
                    arg(whichCoord / 3).arg(whichCoord % 3);
            else
                return context->tr(
                    "Arcs on (boundary) triangle %1 crossing triangle vertex %2").
                    arg(whichCoord / 3).arg(whichCoord % 3);
        } else if (coordSystem == regina::NS_ORIENTED) {
            size_t stdCoord = whichCoord / 2;
            if (whichCoord % 2 == 0) {
                // "true" orientation.
                // Note that quadDefn[i][0] == 0 always.
                if (stdCoord % 7 < 4)
                    return context->tr("Tetrahedron %1, "
                        "triangle oriented towards vertex %2").
                        arg(stdCoord / 7).
                        arg(stdCoord % 7);
                else
                    return context->tr("Tetrahedron %1, "
                        "quad oriented towards edge %2%3").
                        arg(stdCoord / 7).
                        arg(regina::quadDefn[(stdCoord % 7) - 4][0]).
                        arg(regina::quadDefn[(stdCoord % 7) - 4][1]);
            } else {
                // "false" orientation.
                if (stdCoord % 7 < 4)
                    return context->tr("Tetrahedron %1, "
                        "triangle oriented towards face %2").
                        arg(stdCoord / 7).
                        arg(Triangle<3>::ordering(stdCoord % 7).trunc3().c_str());
                else
                    return context->tr("Tetrahedron %1, "
                        "quad oriented towards edge %2%3").
                        arg(stdCoord / 7).
                        arg(regina::quadDefn[(stdCoord % 7) - 4][2]).
                        arg(regina::quadDefn[(stdCoord % 7) - 4][3]);
            }
        } else if (coordSystem == regina::NS_ORIENTED_QUAD) {
            size_t quadCoord = whichCoord / 2;
            if (whichCoord % 2 == 0) {
                // "true" orientation.
                // Note that quadDefn[i][0] == 0 always.
                return context->tr("Tetrahedron %1, "
                    "quad oriented towards edge %2%3").
                    arg(quadCoord / 3).
                    arg(regina::quadDefn[(quadCoord % 3)][0]).
                    arg(regina::quadDefn[(quadCoord % 3)][1]);
            } else {
                // "false" orientation.
                return context->tr("Tetrahedron %1, "
                    "quad oriented towards edge %2%3").
                    arg(quadCoord / 3).
                    arg(regina::quadDefn[(quadCoord % 3)][2]).
                    arg(regina::quadDefn[(quadCoord % 3)][3]);
            }
        }

        return context->tr("This coordinate system is not known");
    }

    QString columnDesc(HyperCoords coordSystem, size_t whichCoord,
            const QObject *context, regina::Triangulation<4>* tri) {
        if (coordSystem == regina::HS_STANDARD) {
            if (whichCoord % 15 < 5)
                return context->tr(
                    "Pentachoron %1, tetrahedron about vertex %2").
                    arg(whichCoord / 15).arg(whichCoord % 15);
            else
                return context->tr(
                    "Pentachoron %1, prism beside vertices %2 and %3").
                    arg(whichCoord / 15).
                    arg(regina::FaceNumbering<4, 1>::edgeVertex
                        [(whichCoord % 15) - 5][0]).
                    arg(regina::FaceNumbering<4, 1>::edgeVertex
                        [(whichCoord % 15) - 5][1]);
        } else if (coordSystem == regina::HS_PRISM) {
            return context->tr(
                "Pentachoron %1, prism beside vertices %2 and %3").
                arg(whichCoord / 10).
                arg(regina::FaceNumbering<4, 1>::edgeVertex
                    [whichCoord % 10][0]).
                arg(regina::FaceNumbering<4, 1>::edgeVertex
                    [whichCoord % 10][1]);
        } else if (coordSystem == regina::HS_EDGE_WEIGHT) {
            if (tri) {
                if (tri->edge(whichCoord)->isBoundary())
                    return context->tr("Weight of (boundary) edge %1").
                        arg(whichCoord);
                else
                    return context->tr("Weight of (internal) edge %1").
                        arg(whichCoord);
            } else
                return context->tr("Weight of edge %1").arg(whichCoord);
        }

        return context->tr("This coordinate system is not known");
    }

    regina::LargeInteger getCoordinate(NormalCoords coordSystem,
            const regina::NormalSurface& surface, size_t whichCoord) {
        if (coordSystem == regina::NS_STANDARD) {
            if (whichCoord % 7 < 4)
                return surface.triangles(whichCoord / 7, whichCoord % 7);
            else
                return surface.quads(whichCoord / 7, (whichCoord % 7) - 4);
        } else if (coordSystem == regina::NS_AN_STANDARD ||
                coordSystem == regina::NS_AN_LEGACY) {
            if (whichCoord % 10 < 4)
                return surface.triangles(whichCoord / 10, whichCoord % 10);
            else if (whichCoord % 10 < 7)
                return surface.quads(whichCoord / 10, (whichCoord % 10) - 4);
            else
                return surface.octs(whichCoord / 10, (whichCoord % 10) - 7);
        } else if (coordSystem == regina::NS_QUAD) {
            return surface.quads(whichCoord / 3, whichCoord % 3);
        } else if (coordSystem == regina::NS_AN_QUAD_OCT) {
            if (whichCoord % 6 < 3)
                return surface.quads(whichCoord / 6, whichCoord % 6);
            else
                return surface.octs(whichCoord / 6, (whichCoord % 6) - 3);
        } else if (coordSystem == regina::NS_EDGE_WEIGHT) {
            return surface.edgeWeight(whichCoord);
        } else if (coordSystem == regina::NS_TRIANGLE_ARCS) {
            return surface.arcs(whichCoord / 3, whichCoord % 3);
        } else if (coordSystem == regina::NS_ORIENTED) {
            bool orientation = (whichCoord % 2 == 0);
            whichCoord /= 2;
            if (whichCoord % 7 < 4)
                return surface.orientedTriangles(
                    whichCoord / 7, whichCoord % 7, orientation);
            else
                return surface.orientedQuads(
                    whichCoord / 7, (whichCoord % 7) - 4, orientation);
        } else if (coordSystem == regina::NS_ORIENTED_QUAD) {
            bool orientation = (whichCoord % 2 == 0);
            whichCoord /= 2;
            return surface.orientedQuads(
                whichCoord / 3, whichCoord % 3, orientation);
        }

        return (long)0;
    }

    regina::LargeInteger getCoordinate(HyperCoords coordSystem,
            const regina::NormalHypersurface& surface, size_t whichCoord) {
        if (coordSystem == regina::HS_STANDARD) {
            if (whichCoord % 15 < 5)
                return surface.tetrahedra(whichCoord / 15, whichCoord % 15);
            else
                return surface.prisms(whichCoord / 15, (whichCoord % 15) - 5);
        } else if (coordSystem == regina::HS_PRISM) {
            return surface.prisms(whichCoord / 10, whichCoord % 10);
        } else if (coordSystem == regina::HS_EDGE_WEIGHT) {
            return surface.edgeWeight(whichCoord);
        }

        return (long)0;
    }
};

