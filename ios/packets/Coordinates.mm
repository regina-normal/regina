
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

#import "Coordinates.h"
#import "hypersurface/normalhypersurfaces.h"
#import "surfaces/normalsurfaces.h"
#import "triangulation/dim3.h"
#import "triangulation/dim4.h"

using regina::HyperCoords;
using regina::NormalHypersurfaces;
using regina::NormalSurfaces;
using regina::NormalCoords;
using regina::Triangle;

@implementation Coordinates

+ (NSString*)name:(regina::NormalCoords)coordSystem capitalise:(BOOL)capitalise
{
    if (capitalise) {
        if (coordSystem == regina::NS_STANDARD)
            return @"Standard normal (tri-quad)";
        if (coordSystem == regina::NS_AN_STANDARD)
            return @"Standard almost normal (tri-quad-oct)";
        if (coordSystem == regina::NS_AN_LEGACY)
            return @"Legacy almost normal (pruned tri-quad-oct)";
        if (coordSystem == regina::NS_QUAD)
            return @"Quad normal";
        if (coordSystem == regina::NS_AN_QUAD_OCT)
            return @"Quad-oct almost normal";
        if (coordSystem == regina::NS_EDGE_WEIGHT)
            return @"Edge weight";
        if (coordSystem == regina::NS_TRIANGLE_ARCS)
            return @"Triangle arc";
        if (coordSystem == regina::NS_ORIENTED)
            return @"Transversely oriented normal";
        if (coordSystem == regina::NS_ORIENTED_QUAD)
            return @"Transversely oriented quad normal";
        return @"Unknown";
    } else {
        if (coordSystem == regina::NS_STANDARD)
            return @"standard normal (tri-quad)";
        if (coordSystem == regina::NS_AN_STANDARD)
            return @"standard almost normal (tri-quad-oct)";
        if (coordSystem == regina::NS_AN_LEGACY)
            return @"legacy almost normal (pruned tri-quad-oct)";
        if (coordSystem == regina::NS_QUAD)
            return @"quad normal";
        if (coordSystem == regina::NS_AN_QUAD_OCT)
            return @"quad-oct almost normal";
        if (coordSystem == regina::NS_EDGE_WEIGHT)
            return @"edge weight";
        if (coordSystem == regina::NS_TRIANGLE_ARCS)
            return @"triangle arc";
        if (coordSystem == regina::NS_ORIENTED)
            return @"transversely oriented normal";
        if (coordSystem == regina::NS_ORIENTED_QUAD)
            return @"transversely oriented quad normal";
        return @"unknown";
    }
}

+ (NSString*)adjective:(regina::NormalCoords)coordSystem capitalise:(BOOL)capitalise
{
    if (capitalise) {
        if (coordSystem == regina::NS_STANDARD)
            return @"Standard";
        if (coordSystem == regina::NS_AN_STANDARD)
            return @"Almost normal";
        if (coordSystem == regina::NS_AN_LEGACY)
            return @"Legacy";
        if (coordSystem == regina::NS_QUAD)
            return @"Quad";
        if (coordSystem == regina::NS_AN_QUAD_OCT)
            return @"Quad-oct";
        if (coordSystem == regina::NS_EDGE_WEIGHT)
            return @"Edge weight";
        if (coordSystem == regina::NS_TRIANGLE_ARCS)
            return @"Triangle arc";
        if (coordSystem == regina::NS_ORIENTED)
            return @"Transversely oriented";
        if (coordSystem == regina::NS_ORIENTED_QUAD)
            return @"Transversely oriented quad";
        return @"Unknown";
    } else {
        if (coordSystem == regina::NS_STANDARD)
            return @"standard";
        if (coordSystem == regina::NS_AN_STANDARD)
            return @"almost normal";
        if (coordSystem == regina::NS_AN_LEGACY)
            return @"legacy";
        if (coordSystem == regina::NS_QUAD)
            return @"quad";
        if (coordSystem == regina::NS_AN_QUAD_OCT)
            return @"quad-oct";
        if (coordSystem == regina::NS_EDGE_WEIGHT)
            return @"edge weight";
        if (coordSystem == regina::NS_TRIANGLE_ARCS)
            return @"triangle arc";
        if (coordSystem == regina::NS_ORIENTED)
            return @"transversely oriented";
        if (coordSystem == regina::NS_ORIENTED_QUAD)
            return @"transversely oriented quad";
        return @"unknown";
    }
}

+ (BOOL)generatesAlmostNormal:(regina::NormalCoords)coordSystem
{
    return (coordSystem == regina::NS_AN_STANDARD ||
            coordSystem == regina::NS_AN_LEGACY ||
            coordSystem == regina::NS_AN_QUAD_OCT);
}

+ (unsigned long)numColumns:(regina::NormalCoords)coordSystem tri:(regina::Triangulation<3>*)tri
{
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

+ (NSString*)columnName:(regina::NormalCoords)coordSystem whichCoord:(unsigned long)whichCoord tri:(regina::Triangulation<3>*)tri
{
    if (coordSystem == regina::NS_STANDARD) {
        if (whichCoord % 7 < 4)
            return [NSString stringWithFormat:@"%ld: %ld", (whichCoord / 7), (whichCoord % 7)];
        else
            return [NSString stringWithFormat:@"%ld: %s", (whichCoord / 7), regina::quadString[(whichCoord % 7) - 4]];
    } else if (coordSystem == regina::NS_AN_STANDARD || coordSystem == regina::NS_AN_LEGACY) {
        if (whichCoord % 10 < 4)
            return [NSString stringWithFormat:@"T%ld: %ld", (whichCoord / 10), (whichCoord % 10)];
        else if (whichCoord % 10 < 7)
            return [NSString stringWithFormat:@"Q%ld: %s", (whichCoord / 10), regina::quadString[(whichCoord % 10) - 4]];
        else
            return [NSString stringWithFormat:@"K%ld: %s", (whichCoord / 10), regina::quadString[(whichCoord % 10) - 7]];
    } else if (coordSystem == regina::NS_QUAD) {
        return [NSString stringWithFormat:@"%ld: %s", (whichCoord / 3), regina::quadString[whichCoord % 3]];
    } else if (coordSystem == regina::NS_AN_QUAD_OCT) {
        if (whichCoord % 6 < 3)
            return [NSString stringWithFormat:@"Q%ld: %s", (whichCoord / 6), regina::quadString[whichCoord % 6]];
        else
            return [NSString stringWithFormat:@"K%ld: %s", (whichCoord / 6), regina::quadString[(whichCoord % 6) - 3]];
    } else if (coordSystem == regina::NS_EDGE_WEIGHT) {
        if (! (tri && tri->edge(whichCoord)->isBoundary()))
            return [NSString stringWithFormat:@"%ld", whichCoord];
        else
            return [NSString stringWithFormat:@"%ld: ∂", whichCoord];
    } else if (coordSystem == regina::NS_TRIANGLE_ARCS) {
        if (! (tri && tri->triangle(whichCoord / 3)->isBoundary()))
            return [NSString stringWithFormat:@"%ld: %ld", (whichCoord / 3), (whichCoord % 3)];
        else
            return [NSString stringWithFormat:@"%ld: %ld: ∂", (whichCoord / 3), (whichCoord % 3)];
    } else if (coordSystem == regina::NS_ORIENTED) {
        unsigned long stdCoord = whichCoord / 2;
        if (whichCoord % 2 == 0) {
            // "true" orientation.
            // Note that quadDefn[i][0] == 0 always.
            if (stdCoord % 7 < 4)
                return [NSString stringWithFormat:@"%ld: %ld", (stdCoord / 7), (stdCoord % 7)];
            else
                return [NSString stringWithFormat:@"%ld: %d%d", (stdCoord / 7), regina::quadDefn[(stdCoord % 7) - 4][0], regina::quadDefn[(stdCoord % 7) - 4][1]];
        } else {
            // "false" orientation.
            if (stdCoord % 7 < 4)
                return [NSString stringWithFormat:@"%ld: %s", (stdCoord / 7), Triangle<3>::ordering(stdCoord % 7).trunc3().c_str()];
            else
                return [NSString stringWithFormat:@"%ld: %d%d", (stdCoord / 7), regina::quadDefn[(stdCoord % 7) - 4][2], regina::quadDefn[(stdCoord % 7) - 4][3]];
        }
    } else if (coordSystem == regina::NS_ORIENTED_QUAD) {
        unsigned long quadCoord = whichCoord / 2;
        if (whichCoord % 2 == 0) {
            // "true" orientation.
            // Note that quadDefn[i][0] == 0 always.
            return [NSString stringWithFormat:@"%ld: %d%d", (quadCoord / 3), regina::quadDefn[(quadCoord % 3)][0], regina::quadDefn[(quadCoord % 3)][1]];
        } else {
            // "false" orientation.
            return [NSString stringWithFormat:@"%ld: %d%d", (quadCoord / 3), regina::quadDefn[(quadCoord % 3)][2], regina::quadDefn[(quadCoord % 3)][3]];
        }
    }

    return @"Unknown";
}

+ (NSString*)longestColumnName:(regina::NormalCoords)coordSystem tri:(regina::Triangulation<3>*)tri
{
    switch (coordSystem) {
        case regina::NS_STANDARD:
        case regina::NS_QUAD:
        case regina::NS_AN_STANDARD:
        case regina::NS_AN_QUAD_OCT:
        case regina::NS_AN_LEGACY:
            return [NSString stringWithFormat:@"Q%ld: 00/00", tri->size()];
        case regina::NS_EDGE_WEIGHT:
            if (tri->hasBoundaryTriangles())
                return [NSString stringWithFormat:@"%ld: ∂", tri->countEdges()];
            else
                return [NSString stringWithFormat:@"%ld", tri->countEdges()];
            break;
        case regina::NS_TRIANGLE_ARCS:
            if (tri->hasBoundaryTriangles())
                return [NSString stringWithFormat:@"%ld: 0: ∂", tri->countTriangles()];
            else
                return [NSString stringWithFormat:@"%ld: 0", tri->countTriangles()];
            break;
        case regina::NS_ORIENTED:
        case regina::NS_ORIENTED_QUAD:
            return [NSString stringWithFormat:@"%ld: 000", tri->size()];
            break;
        case regina::NS_ANGLE:
            // Should never appear.
        default:
            return @"";
    }
}

+ (regina::LargeInteger)getCoordinate:(regina::NormalCoords)coordSystem surface:(const regina::NormalSurface&)surface whichCoord:(unsigned long)whichCoord
{
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

@end

@implementation HyperCoordinates

+ (NSString*)name:(regina::HyperCoords)coordSystem capitalise:(BOOL)capitalise
{
    if (capitalise) {
        if (coordSystem == regina::HS_STANDARD)
            return @"Standard normal (tet-prism)";
        if (coordSystem == regina::HS_PRISM)
            return @"Prism normal";
        if (coordSystem == regina::HS_EDGE_WEIGHT)
            return @"Edge weight";
        return @"Unknown";
    } else {
        if (coordSystem == regina::HS_STANDARD)
            return @"standard normal (tet-prism)";
        if (coordSystem == regina::HS_PRISM)
            return @"prism normal";
        if (coordSystem == regina::HS_EDGE_WEIGHT)
            return @"edge weight";
        return @"unknown";
    }
}

+ (NSString*)adjective:(regina::HyperCoords)coordSystem capitalise:(BOOL)capitalise
{
    if (capitalise) {
        if (coordSystem == regina::HS_STANDARD)
            return @"Standard";
        if (coordSystem == regina::HS_PRISM)
            return @"Prism";
        if (coordSystem == regina::HS_EDGE_WEIGHT)
            return @"Edge weight";
        return @"Unknown";
    } else {
        if (coordSystem == regina::HS_STANDARD)
            return @"standard";
        if (coordSystem == regina::HS_PRISM)
            return @"prism";
        if (coordSystem == regina::HS_EDGE_WEIGHT)
            return @"edge weight";
        return @"unknown";
    }
}

+ (size_t)numColumns:(regina::HyperCoords)coordSystem tri:(regina::Triangulation<4>*)tri
{
    if (coordSystem == regina::HS_STANDARD)
        return tri->size() * 15;
    else if (coordSystem == regina::HS_PRISM)
        return tri->size() * 10;
    else if (coordSystem == regina::HS_EDGE_WEIGHT)
        return tri->countEdges();
    else
        return 0;
}

+ (NSString*)columnName:(regina::HyperCoords)coordSystem whichCoord:(size_t)whichCoord tri:(regina::Triangulation<4>*)tri
{
    if (coordSystem == regina::HS_STANDARD) {
        if (whichCoord % 15 < 5)
            return [NSString stringWithFormat:@"%ld: %ld", (whichCoord / 15), (whichCoord % 15)];
        else
            return [NSString stringWithFormat:@"%ld: %d%d", (whichCoord / 15),
                    regina::FaceNumbering<4, 1>::edgeVertex[(whichCoord % 15) - 5][0],
                    regina::FaceNumbering<4, 1>::edgeVertex[(whichCoord % 15) - 5][1]];
    } else if (coordSystem == regina::HS_PRISM) {
        return [NSString stringWithFormat:@"%ld: %d%d", (whichCoord / 10),
                regina::FaceNumbering<4, 1>::edgeVertex[whichCoord % 10][0],
                regina::FaceNumbering<4, 1>::edgeVertex[whichCoord % 10][1]];
    } else if (coordSystem == regina::HS_EDGE_WEIGHT) {
        if (! (tri && tri->edge(whichCoord)->isBoundary()))
            return [NSString stringWithFormat:@"%ld", whichCoord];
        else
            return [NSString stringWithFormat:@"%ld: ∂", whichCoord];
    }

    return @"Unknown";
}

+ (NSString*)longestColumnName:(regina::HyperCoords)coordSystem tri:(regina::Triangulation<4>*)tri
{
    switch (coordSystem) {
        case regina::HS_STANDARD:
        case regina::HS_PRISM:
            return [NSString stringWithFormat:@"P%ld: 00", tri->size()];
        case regina::HS_EDGE_WEIGHT:
            if (tri->hasBoundaryFacets())
                return [NSString stringWithFormat:@"%ld: ∂", tri->countEdges()];
            else
                return [NSString stringWithFormat:@"%ld", tri->countEdges()];
            break;
        default:
            return @"";
    }
}

+ (regina::LargeInteger)getCoordinate:(regina::HyperCoords)coordSystem surface:(const regina::NormalHypersurface&)surface whichCoord:(size_t)whichCoord
{
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

@end
