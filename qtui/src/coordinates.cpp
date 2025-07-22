
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "hypersurface/normalhypersurfaces.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

#include "reginaprefset.h"

#include <QObject>
#include <QString>

using regina::HyperCoords;
using regina::NormalCoords;
using regina::Triangle;

// #define SUPPORT_TONS

namespace Coordinates {
    const char* name(NormalCoords coordSystem, bool capitalise) {
        if (capitalise) {
            if (coordSystem == NormalCoords::Standard)
                return QT_TR_NOOP("Standard normal (tri-quad)");
            if (coordSystem == NormalCoords::AlmostNormal)
                return QT_TR_NOOP("Standard almost normal (tri-quad-oct)");
            if (coordSystem == NormalCoords::LegacyAlmostNormal)
                return QT_TR_NOOP("Legacy almost normal (pruned tri-quad-oct)");
            if (coordSystem == NormalCoords::Quad)
                return QT_TR_NOOP("Quad normal");
            if (coordSystem == NormalCoords::QuadClosed)
                return QT_TR_NOOP("Closed quad (non-spun)");
            if (coordSystem == NormalCoords::QuadOct)
                return QT_TR_NOOP("Quad-oct almost normal");
            if (coordSystem == NormalCoords::QuadOctClosed)
                return QT_TR_NOOP("Closed quad-oct (non-spun)");
            if (coordSystem == NormalCoords::Edge)
                return QT_TR_NOOP("Edge weight");
            if (coordSystem == NormalCoords::Arc)
                return QT_TR_NOOP("Triangle arc");
#ifdef SUPPORT_TONS
            if (coordSystem == NormalCoords::Oriented)
                return QT_TR_NOOP("Transversely oriented normal");
            if (coordSystem == NormalCoords::OrientedQuad)
                return QT_TR_NOOP("Transversely oriented quad normal");
#endif
            return QT_TR_NOOP("Unknown");
        } else {
            if (coordSystem == NormalCoords::Standard)
                return QT_TR_NOOP("standard normal (tri-quad)");
            if (coordSystem == NormalCoords::AlmostNormal)
                return QT_TR_NOOP("standard almost normal (tri-quad-oct)");
            if (coordSystem == NormalCoords::LegacyAlmostNormal)
                return QT_TR_NOOP("legacy almost normal (pruned tri-quad-oct)");
            if (coordSystem == NormalCoords::Quad)
                return QT_TR_NOOP("quad normal");
            if (coordSystem == NormalCoords::QuadClosed)
                return QT_TR_NOOP("closed quad (non-spun)");
            if (coordSystem == NormalCoords::QuadOct)
                return QT_TR_NOOP("quad-oct almost normal");
            if (coordSystem == NormalCoords::QuadOctClosed)
                return QT_TR_NOOP("closed quad-oct (non-spun)");
            if (coordSystem == NormalCoords::Edge)
                return QT_TR_NOOP("edge weight");
            if (coordSystem == NormalCoords::Arc)
                return QT_TR_NOOP("triangle arc");
#ifdef SUPPORT_TONS
            if (coordSystem == NormalCoords::Oriented)
                return QT_TR_NOOP("transversely oriented normal");
            if (coordSystem == NormalCoords::OrientedQuad)
                return QT_TR_NOOP("transversely oriented quad normal");
#endif
            return QT_TR_NOOP("unknown");
        }
    }

    const char* adjective(NormalCoords coordSystem, bool capitalise) {
        if (capitalise) {
            if (coordSystem == NormalCoords::Standard)
                return QT_TR_NOOP("Standard");
            if (coordSystem == NormalCoords::AlmostNormal)
                return QT_TR_NOOP("Almost normal");
            if (coordSystem == NormalCoords::LegacyAlmostNormal)
                return QT_TR_NOOP("Legacy");
            if (coordSystem == NormalCoords::Quad)
                return QT_TR_NOOP("Quad");
            if (coordSystem == NormalCoords::QuadClosed)
                return QT_TR_NOOP("Closed quad");
            if (coordSystem == NormalCoords::QuadOct)
                return QT_TR_NOOP("Quad-oct");
            if (coordSystem == NormalCoords::QuadOctClosed)
                return QT_TR_NOOP("Closed quad-oct");
            if (coordSystem == NormalCoords::Edge)
                return QT_TR_NOOP("Edge weight");
            if (coordSystem == NormalCoords::Arc)
                return QT_TR_NOOP("Triangle arc");
#ifdef SUPPORT_TONS
            if (coordSystem == NormalCoords::Oriented)
                return QT_TR_NOOP("Transversely oriented");
            if (coordSystem == NormalCoords::OrientedQuad)
                return QT_TR_NOOP("Transversely oriented quad");
#endif
            return QT_TR_NOOP("Unknown");
        } else {
            if (coordSystem == NormalCoords::Standard)
                return QT_TR_NOOP("standard");
            if (coordSystem == NormalCoords::AlmostNormal)
                return QT_TR_NOOP("almost normal");
            if (coordSystem == NormalCoords::LegacyAlmostNormal)
                return QT_TR_NOOP("legacy");
            if (coordSystem == NormalCoords::Quad)
                return QT_TR_NOOP("quad");
            if (coordSystem == NormalCoords::QuadClosed)
                return QT_TR_NOOP("closed quad");
            if (coordSystem == NormalCoords::QuadOct)
                return QT_TR_NOOP("quad-oct");
            if (coordSystem == NormalCoords::QuadOctClosed)
                return QT_TR_NOOP("closed quad-oct");
            if (coordSystem == NormalCoords::Edge)
                return QT_TR_NOOP("edge weight");
            if (coordSystem == NormalCoords::Arc)
                return QT_TR_NOOP("triangle arc");
#ifdef SUPPORT_TONS
            if (coordSystem == NormalCoords::Oriented)
                return QT_TR_NOOP("transversely oriented");
            if (coordSystem == NormalCoords::OrientedQuad)
                return QT_TR_NOOP("transversely oriented quad");
#endif
            return QT_TR_NOOP("unknown");
        }
    }

    const char* name(HyperCoords coordSystem, bool capitalise) {
        if (capitalise) {
            if (coordSystem == HyperCoords::Standard)
                return QT_TR_NOOP("Standard normal (tet-prism)");
            if (coordSystem == HyperCoords::Prism)
                return QT_TR_NOOP("Prism normal");
            if (coordSystem == HyperCoords::Edge)
                return QT_TR_NOOP("Edge weight");
            return QT_TR_NOOP("Unknown");
        } else {
            if (coordSystem == HyperCoords::Standard)
                return QT_TR_NOOP("standard normal (tet-prism)");
            if (coordSystem == HyperCoords::Prism)
                return QT_TR_NOOP("prism normal");
            if (coordSystem == HyperCoords::Edge)
                return QT_TR_NOOP("edge weight");
            return QT_TR_NOOP("unknown");
        }
    }

    const char* adjective(HyperCoords coordSystem, bool capitalise) {
        if (capitalise) {
            if (coordSystem == HyperCoords::Standard)
                return QT_TR_NOOP("Standard");
            if (coordSystem == HyperCoords::Prism)
                return QT_TR_NOOP("Prism");
            if (coordSystem == HyperCoords::Edge)
                return QT_TR_NOOP("Edge weight");
            return QT_TR_NOOP("Unknown");
        } else {
            if (coordSystem == HyperCoords::Standard)
                return QT_TR_NOOP("standard");
            if (coordSystem == HyperCoords::Prism)
                return QT_TR_NOOP("prism");
            if (coordSystem == HyperCoords::Edge)
                return QT_TR_NOOP("edge weight");
            return QT_TR_NOOP("unknown");
        }
    }

    bool generatesAlmostNormal(NormalCoords coordSystem) {
        return (coordSystem == NormalCoords::AlmostNormal ||
                coordSystem == NormalCoords::LegacyAlmostNormal ||
                coordSystem == NormalCoords::QuadOct ||
                coordSystem == NormalCoords::QuadOctClosed);
    }

    size_t numColumns(NormalCoords coordSystem,
            const regina::Triangulation<3>& tri) {
        if (coordSystem == NormalCoords::Standard)
            return tri.size() * 7;
        else if (coordSystem == NormalCoords::AlmostNormal)
            return tri.size() * 10;
        else if (coordSystem == NormalCoords::LegacyAlmostNormal)
            return tri.size() * 10;
        else if (coordSystem == NormalCoords::Quad)
            return tri.size() * 3;
        else if (coordSystem == NormalCoords::QuadClosed)
            return tri.size() * 3;
        else if (coordSystem == NormalCoords::QuadOct)
            return tri.size() * 6;
        else if (coordSystem == NormalCoords::QuadOctClosed)
            return tri.size() * 6;
        else if (coordSystem == NormalCoords::Edge)
            return tri.countEdges();
        else if (coordSystem == NormalCoords::Arc)
            return tri.countTriangles() * 3;
#ifdef SUPPORT_TONS
        else if (coordSystem == NormalCoords::Oriented)
            return tri.size() * 14;
        else if (coordSystem == NormalCoords::OrientedQuad)
            return tri.size() * 6;
#endif
        else
            return 0;
    }

    size_t numColumns(HyperCoords coordSystem,
            const regina::Triangulation<4>& tri) {
        if (coordSystem == HyperCoords::Standard)
            return tri.size() * 15;
        else if (coordSystem == HyperCoords::Prism)
            return tri.size() * 10;
        else if (coordSystem == HyperCoords::Edge)
            return tri.countEdges();
        else
            return 0;
    }

    QString columnName(NormalCoords coordSystem, size_t whichCoord,
            const regina::Triangulation<3>& tri) {
        if (coordSystem == NormalCoords::Standard) {
            if (whichCoord % 7 < 4)
                return QString("%1: %2").arg(whichCoord / 7).
                    arg(whichCoord % 7);
            else
                return QString("%1: %2").arg(whichCoord / 7).
                    arg(regina::quadString[(whichCoord % 7) - 4].c_str());
        } else if (coordSystem == NormalCoords::AlmostNormal ||
                coordSystem == NormalCoords::LegacyAlmostNormal) {
            if (whichCoord % 10 < 4)
                return QString("T%1: %2").arg(whichCoord / 10).
                    arg(whichCoord % 10);
            else if (whichCoord % 10 < 7)
                return QString("Q%1: %2").arg(whichCoord / 10).
                    arg(regina::quadString[(whichCoord % 10) - 4].c_str());
            else
                return QString("K%1: %2").arg(whichCoord / 10).
                    arg(regina::quadString[(whichCoord % 10) - 7].c_str());
        } else if (coordSystem == NormalCoords::Quad ||
                coordSystem == NormalCoords::QuadClosed) {
            return QString("%1: %2").arg(whichCoord / 3).
                arg(regina::quadString[whichCoord % 3].c_str());
        } else if (coordSystem == NormalCoords::QuadOct ||
                coordSystem == NormalCoords::QuadOctClosed) {
            if (whichCoord % 6 < 3)
                return QString("Q%1: %2").arg(whichCoord / 6).
                    arg(regina::quadString[whichCoord % 6].c_str());
            else
                return QString("K%1: %2").arg(whichCoord / 6).
                    arg(regina::quadString[(whichCoord % 6) - 3].c_str());
        } else if (coordSystem == NormalCoords::Edge) {
            if (! tri.edge(whichCoord)->isBoundary())
                return QString::number(whichCoord);
            else if (ReginaPrefSet::global().displayUnicode)
                return QString("%1: \u2202").arg(whichCoord);
            else
                return QString("%1 (B)").arg(whichCoord);
        } else if (coordSystem == NormalCoords::Arc) {
            if (! tri.triangle(whichCoord / 3)->isBoundary())
                return QString("%1: %2").
                    arg(whichCoord / 3).arg(whichCoord % 3);
            else if (ReginaPrefSet::global().displayUnicode)
                return QString("\u2202 %1: %2").
                    arg(whichCoord / 3).arg(whichCoord % 3);
            else
                return QString("B %1: %2").
                    arg(whichCoord / 3).arg(whichCoord % 3);
#ifdef SUPPORT_TONS
        } else if (coordSystem == NormalCoords::Oriented) {
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
        } else if (coordSystem == NormalCoords::OrientedQuad) {
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
#endif
        }

        return QString("Unknown");
    }

    QString columnName(HyperCoords coordSystem, size_t whichCoord,
            const regina::Triangulation<4>& tri) {
        if (coordSystem == HyperCoords::Standard) {
            if (whichCoord % 15 < 5)
                return QString("%1: %2").arg(whichCoord / 15).
                    arg(whichCoord % 15);
            else
                return QString("%1: %2%3").arg(whichCoord / 15).
                    arg(regina::FaceNumbering<4, 1>::edgeVertex
                        [(whichCoord % 15) - 5][0]).
                    arg(regina::FaceNumbering<4, 1>::edgeVertex
                        [(whichCoord % 15) - 5][1]);
        } else if (coordSystem == HyperCoords::Prism) {
            return QString("%1: %2%3").arg(whichCoord / 10).
                arg(regina::FaceNumbering<4, 1>::edgeVertex
                    [whichCoord % 10][0]).
                arg(regina::FaceNumbering<4, 1>::edgeVertex
                    [whichCoord % 10][1]);
        } else if (coordSystem == HyperCoords::Edge) {
            if (! tri.edge(whichCoord)->isBoundary())
                return QString::number(whichCoord);
            else if (ReginaPrefSet::global().displayUnicode)
                return QString("%1: \u2202").arg(whichCoord);
            else
                return QString("%1 (B)").arg(whichCoord);
        }

        return QString("Unknown");
    }

    QString columnDesc(NormalCoords coordSystem, size_t whichCoord,
            const QObject *context, const regina::Triangulation<3>& tri) {
        if (coordSystem == NormalCoords::Standard) {
            if (whichCoord % 7 < 4)
                return context->tr("Tetrahedron %1, triangle about vertex %2").
                    arg(whichCoord / 7).arg(whichCoord % 7);
            else
                return context->tr("Tetrahedron %1, quad splitting vertices %2").
                    arg(whichCoord / 7).
                    arg(regina::quadString[(whichCoord % 7) - 4].c_str());
        } else if (coordSystem == NormalCoords::AlmostNormal ||
                coordSystem == NormalCoords::LegacyAlmostNormal) {
            if (whichCoord % 10 < 4)
                return context->tr("Tetrahedron %1, triangle about vertex %2").
                    arg(whichCoord / 10).arg(whichCoord % 10);
            else if (whichCoord % 10 < 7)
                return context->tr("Tetrahedron %1, quad splitting vertices %2").
                    arg(whichCoord / 10).
                    arg(regina::quadString[(whichCoord % 10) - 4].c_str());
            else
                return context->tr("Tetrahedron %1, oct partitioning vertices %2").
                    arg(whichCoord / 10).
                    arg(regina::quadString[(whichCoord % 10) - 7].c_str());
        } else if (coordSystem == NormalCoords::Quad ||
                coordSystem == NormalCoords::QuadClosed) {
            return context->tr("Tetrahedron %1, quad splitting vertices %2").
                arg(whichCoord / 3).
                arg(regina::quadString[whichCoord % 3].c_str());
        } else if (coordSystem == NormalCoords::QuadOct ||
                coordSystem == NormalCoords::QuadOctClosed) {
            if (whichCoord % 6 < 3)
                return context->tr("Tetrahedron %1, quad splitting vertices %2").
                    arg(whichCoord / 6).
                    arg(regina::quadString[whichCoord % 6].c_str());
            else
                return context->tr("Tetrahedron %1, oct partitioning vertices %2").
                    arg(whichCoord / 6).
                    arg(regina::quadString[(whichCoord % 6) - 3].c_str());
        } else if (coordSystem == NormalCoords::Edge) {
            if (tri.edge(whichCoord)->isBoundary())
                return context->tr("Weight of (boundary) edge %1").
                    arg(whichCoord);
            else
                return context->tr("Weight of (internal) edge %1").
                    arg(whichCoord);
        } else if (coordSystem == NormalCoords::Arc) {
            if (! tri.triangle(whichCoord / 3)->isBoundary())
                return context->tr(
                    "Arcs on (internal) triangle %1 crossing triangle vertex %2").
                    arg(whichCoord / 3).arg(whichCoord % 3);
            else
                return context->tr(
                    "Arcs on (boundary) triangle %1 crossing triangle vertex %2").
                    arg(whichCoord / 3).arg(whichCoord % 3);
#ifdef SUPPORT_TONS
        } else if (coordSystem == NormalCoords::Oriented) {
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
        } else if (coordSystem == NormalCoords::OrientedQuad) {
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
#endif
        }

        return context->tr("This coordinate system is not known");
    }

    QString columnDesc(HyperCoords coordSystem, size_t whichCoord,
            const QObject *context, const regina::Triangulation<4>& tri) {
        if (coordSystem == HyperCoords::Standard) {
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
        } else if (coordSystem == HyperCoords::Prism) {
            return context->tr(
                "Pentachoron %1, prism beside vertices %2 and %3").
                arg(whichCoord / 10).
                arg(regina::FaceNumbering<4, 1>::edgeVertex
                    [whichCoord % 10][0]).
                arg(regina::FaceNumbering<4, 1>::edgeVertex
                    [whichCoord % 10][1]);
        } else if (coordSystem == HyperCoords::Edge) {
            if (tri.edge(whichCoord)->isBoundary())
                return context->tr("Weight of (boundary) edge %1").
                    arg(whichCoord);
            else
                return context->tr("Weight of (internal) edge %1").
                    arg(whichCoord);
        }

        return context->tr("This coordinate system is not known");
    }

    regina::LargeInteger getCoordinate(NormalCoords coordSystem,
            const regina::NormalSurface& surface, size_t whichCoord) {
        if (coordSystem == NormalCoords::Standard) {
            if (whichCoord % 7 < 4)
                return surface.triangles(whichCoord / 7, whichCoord % 7);
            else
                return surface.quads(whichCoord / 7, (whichCoord % 7) - 4);
        } else if (coordSystem == NormalCoords::AlmostNormal ||
                coordSystem == NormalCoords::LegacyAlmostNormal) {
            if (whichCoord % 10 < 4)
                return surface.triangles(whichCoord / 10, whichCoord % 10);
            else if (whichCoord % 10 < 7)
                return surface.quads(whichCoord / 10, (whichCoord % 10) - 4);
            else
                return surface.octs(whichCoord / 10, (whichCoord % 10) - 7);
        } else if (coordSystem == NormalCoords::Quad ||
                coordSystem == NormalCoords::QuadClosed) {
            return surface.quads(whichCoord / 3, whichCoord % 3);
        } else if (coordSystem == NormalCoords::QuadOct ||
                coordSystem == NormalCoords::QuadOctClosed) {
            if (whichCoord % 6 < 3)
                return surface.quads(whichCoord / 6, whichCoord % 6);
            else
                return surface.octs(whichCoord / 6, (whichCoord % 6) - 3);
        } else if (coordSystem == NormalCoords::Edge) {
            return surface.edgeWeight(whichCoord);
        } else if (coordSystem == NormalCoords::Arc) {
            return surface.arcs(whichCoord / 3, whichCoord % 3);
#ifdef SUPPORT_TONS
        } else if (coordSystem == NormalCoords::Oriented) {
            bool orientation = (whichCoord % 2 == 0);
            whichCoord /= 2;
            if (whichCoord % 7 < 4)
                return surface.orientedTriangles(
                    whichCoord / 7, whichCoord % 7, orientation);
            else
                return surface.orientedQuads(
                    whichCoord / 7, (whichCoord % 7) - 4, orientation);
        } else if (coordSystem == NormalCoords::OrientedQuad) {
            bool orientation = (whichCoord % 2 == 0);
            whichCoord /= 2;
            return surface.orientedQuads(
                whichCoord / 3, whichCoord % 3, orientation);
#endif
        }

        return (long)0;
    }

    regina::LargeInteger getCoordinate(HyperCoords coordSystem,
            const regina::NormalHypersurface& surface, size_t whichCoord) {
        if (coordSystem == HyperCoords::Standard) {
            if (whichCoord % 15 < 5)
                return surface.tetrahedra(whichCoord / 15, whichCoord % 15);
            else
                return surface.prisms(whichCoord / 15, (whichCoord % 15) - 5);
        } else if (coordSystem == HyperCoords::Prism) {
            return surface.prisms(whichCoord / 10, whichCoord % 10);
        } else if (coordSystem == HyperCoords::Edge) {
            return surface.edgeWeight(whichCoord);
        }

        return (long)0;
    }
};

