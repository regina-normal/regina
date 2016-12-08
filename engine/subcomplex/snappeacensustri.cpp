
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "algebra/abeliangroup.h"
#include "manifold/snappeacensusmfd.h"
#include "subcomplex/snappeacensustri.h"
#include "triangulation/dim3.h"

namespace regina {

const char SnapPeaCensusTri::SEC_5 = 'm';
const char SnapPeaCensusTri::SEC_6_OR = 's';
const char SnapPeaCensusTri::SEC_6_NOR = 'x';
const char SnapPeaCensusTri::SEC_7_OR = 'v';
const char SnapPeaCensusTri::SEC_7_NOR = 'y';

SnapPeaCensusTri* SnapPeaCensusTri::isSmallSnapPeaCensusTri(
        const Component<3>* comp) {
    // Currently this routine can recognise SnapPea triangulations
    // m000 -- m004 as well as m129.

    // Since the triangulations are so small we can use census results
    // (from a census of all small valid ideal triangulations) to recognise
    // the triangulations by properties alone.

    // Before we do any further checks, make sure the number of
    // tetrahedra is within the supported range.

    if (comp->size() > 4)
        return 0;

    // Start with property checks to see if it has a chance of being
    // in the SnapPea census at all.  The component must not be
    // closed, every edge must be valid and every vertex link must be
    // either a torus or a Klein bottle.  Note that this implies
    // that there are no boundary triangles.

    if (comp->isClosed())
        return 0;

    unsigned long nVertices = comp->countVertices();
    unsigned long nEdges = comp->countEdges();
    unsigned long i;
    int link;
    for (i = 0; i < nVertices; i++) {
        link = comp->vertex(i)->link();
        if (link != Vertex<3>::TORUS && link != Vertex<3>::KLEIN_BOTTLE)
            return 0;
    }
    for (i = 0; i < nEdges; i++)
        if (! comp->edge(i)->isValid())
            return 0;

    // Now search for specific triangulations.

    if (comp->size() == 1) {
        // At this point it must be m000, since there are no others
        // that fit these constraints.  But test orientability
        // anyway just to be safe.
        if (comp->isOrientable())
            return 0;
        return new SnapPeaCensusTri(SEC_5, 0);
    } else if (comp->size() == 2) {
        if (comp->isOrientable()) {
            // Orientable.  Looking for m003 or m004.
            if (comp->countVertices() != 1)
                return 0;
            if (comp->countEdges() != 2)
                return 0;
            if (comp->edge(0)->degree() != 6 ||
                    comp->edge(1)->degree() != 6)
                return 0;

            // Now we know it's either m003 or m004.  We distinguish
            // between them by triangle types, since all of m003's triangles
            // are Mobius bands and all of m004's triangles are horns.
            if (comp->triangle(0)->type() == Triangle<3>::MOBIUS)
                return new SnapPeaCensusTri(SEC_5, 3);
            else
                return new SnapPeaCensusTri(SEC_5, 4);
        } else {
            // Non-orientable.  Looking for m001 or m002.
            if (comp->countVertices() == 1) {
                // Looking for m001.
                if (comp->countEdges() != 2)
                    return 0;
                if (! ((comp->edge(0)->degree() == 4 &&
                        comp->edge(1)->degree() == 8) ||
                       (comp->edge(0)->degree() == 8 &&
                        comp->edge(1)->degree() == 4)))
                    return 0;
                // Census says it's m001 if no triangle forms a dunce hat.
                for (int i = 0; i < 4; i++)
                    if (comp->triangle(i)->type() == Triangle<3>::DUNCEHAT)
                        return 0;
                return new SnapPeaCensusTri(SEC_5, 1);
            } else if (comp->countVertices() == 2) {
                // Looking for m002.
                if (comp->countEdges() != 2)
                    return 0;
                if (comp->edge(0)->degree() != 6 ||
                        comp->edge(1)->degree() != 6)
                    return 0;
                // Census says it's m002 if some triangle forms a dunce hat.
                for (int i = 0; i < 4; i++)
                    if (comp->triangle(i)->type() == Triangle<3>::DUNCEHAT)
                        return new SnapPeaCensusTri(SEC_5, 2);
                return 0;
            }
        }
    } else if (comp->size() == 4) {
        if (comp->isOrientable()) {
            // Search for the Whitehead link complement.
            // Note that this could be done with a smaller set of tests
            // since some can be deduced from others, but these tests
            // aren't terribly expensive anyway.
            if (comp->countVertices() != 2)
                return 0;
            if (comp->countEdges() != 4)
                return 0;
            if (comp->vertex(0)->link() != Vertex<3>::TORUS)
                return 0;
            if (comp->vertex(1)->link() != Vertex<3>::TORUS)
                return 0;
            if (comp->vertex(0)->degree() != 8)
                return 0;
            if (comp->vertex(1)->degree() != 8)
                return 0;
            // Census says it's the Whitehead link if some edge has
            // degree 8.
            for (int i = 0; i < 4; i++)
                if (comp->edge(i)->degree() == 8)
                    return new SnapPeaCensusTri(SEC_5, 129);
            return 0;
        }
    }

    // Not recognised after all.
    return 0;
}

Manifold* SnapPeaCensusTri::manifold() const {
    return new SnapPeaCensusManifold(section_, index_);
}

AbelianGroup* SnapPeaCensusTri::homology() const {
    return SnapPeaCensusManifold(section_, index_).homology();
}

std::ostream& SnapPeaCensusTri::writeName(std::ostream& out) const {
    out << "SnapPea " << section_;

    // Pad the index with leading zeroes.
    // All sections are written with three-digit indices, except for
    // 7-tetrahedron orientable which uses four-digit indices.
    if (section_ == SEC_7_OR && index_ < 1000)
        out << '0';
    if (index_ < 100)
        out << '0';
    if (index_ < 10)
        out << '0';
    out << index_;

    return out;
}

std::ostream& SnapPeaCensusTri::writeTeXName(std::ostream& out) const {
    out << section_ << "_{";

    // Pad the index with leading zeroes.
    // All sections are written with three-digit indices, except for
    // 7-tetrahedron orientable which uses four-digit indices.
    if (section_ == SEC_7_OR && index_ < 1000)
        out << '0';
    if (index_ < 100)
        out << '0';
    if (index_ < 10)
        out << '0';
    out << index_ << '}';

    return out;
}

} // namespace regina

