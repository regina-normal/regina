
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "algebra/nabeliangroup.h"
#include "manifold/nsnappeacensusmfd.h"
#include "subcomplex/nsnappeacensustri.h"
#include "triangulation/ncomponent.h"
#include "triangulation/nedge.h"
#include "triangulation/nface.h"
#include "triangulation/nvertex.h"

namespace regina {

const char NSnapPeaCensusTri::SEC_5 = 'm';
const char NSnapPeaCensusTri::SEC_6_OR = 's';
const char NSnapPeaCensusTri::SEC_6_NOR = 'x';
const char NSnapPeaCensusTri::SEC_7_OR = 'v';
const char NSnapPeaCensusTri::SEC_7_NOR = 'y';

NSnapPeaCensusTri* NSnapPeaCensusTri::isSmallSnapPeaCensusTri(
        const NComponent* comp) {
    // Currently this routine can recognise SnapPea triangulations
    // m000 -- m004 as well as m129.

    // Since the triangulations are so small we can use census results
    // (from a census of all small valid ideal triangulations) to recognise
    // the triangulations by properties alone.

    // Before we do any further checks, make sure the number of
    // tetrahedra is within the supported range.

    if (comp->getNumberOfTetrahedra() > 4)
        return 0;

    // Start with property checks to see if it has a chance of being
    // in the SnapPea census at all.  The component must not be
    // closed, every edge must be valid and every vertex link must be
    // either a torus or a Klein bottle.  Note that this implies
    // that there are no boundary faces.

    if (comp->isClosed())
        return 0;

    unsigned long nVertices = comp->getNumberOfVertices();
    unsigned long nEdges = comp->getNumberOfEdges();
    unsigned long i;
    int link;
    for (i = 0; i < nVertices; i++) {
        link = comp->getVertex(i)->getLink();
        if (link != NVertex::TORUS && link != NVertex::KLEIN_BOTTLE)
            return 0;
    }
    for (i = 0; i < nEdges; i++)
        if (! comp->getEdge(i)->isValid())
            return 0;

    // Now search for specific triangulations.

    if (comp->getNumberOfTetrahedra() == 1) {
        // At this point it must be m000, since there are no others
        // that fit these constraints.  But test orientability
        // anyway just to be safe.
        if (comp->isOrientable())
            return 0;
        return new NSnapPeaCensusTri(SEC_5, 0);
    } else if (comp->getNumberOfTetrahedra() == 2) {
        if (comp->isOrientable()) {
            // Orientable.  Looking for m003 or m004.
            if (comp->getNumberOfVertices() != 1)
                return 0;
            if (comp->getNumberOfEdges() != 2)
                return 0;
            if (comp->getEdge(0)->getNumberOfEmbeddings() != 6 ||
                    comp->getEdge(1)->getNumberOfEmbeddings() != 6)
                return 0;

            // Now we know it's either m003 or m004.  We distinguish
            // between them by face types, since all of m003's faces
            // are Mobius bands and all of m004's faces are horns.
            if (comp->getFace(0)->getType() == NFace::MOBIUS)
                return new NSnapPeaCensusTri(SEC_5, 3);
            else
                return new NSnapPeaCensusTri(SEC_5, 4);
        } else {
            // Non-orientable.  Looking for m001 or m002.
            if (comp->getNumberOfVertices() == 1) {
                // Looking for m001.
                if (comp->getNumberOfEdges() != 2)
                    return 0;
                if (! ((comp->getEdge(0)->getNumberOfEmbeddings() == 4 &&
                        comp->getEdge(1)->getNumberOfEmbeddings() == 8) ||
                       (comp->getEdge(0)->getNumberOfEmbeddings() == 8 &&
                        comp->getEdge(1)->getNumberOfEmbeddings() == 4)))
                    return 0;
                // Census says it's m001 if no face forms a dunce hat.
                for (int i = 0; i < 4; i++)
                    if (comp->getFace(i)->getType() == NFace::DUNCEHAT)
                        return 0;
                return new NSnapPeaCensusTri(SEC_5, 1);
            } else if (comp->getNumberOfVertices() == 2) {
                // Looking for m002.
                if (comp->getNumberOfEdges() != 2)
                    return 0;
                if (comp->getEdge(0)->getNumberOfEmbeddings() != 6 ||
                        comp->getEdge(1)->getNumberOfEmbeddings() != 6)
                    return 0;
                // Census says it's m002 if some face forms a dunce hat.
                for (int i = 0; i < 4; i++)
                    if (comp->getFace(i)->getType() == NFace::DUNCEHAT)
                        return new NSnapPeaCensusTri(SEC_5, 2);
                return 0;
            }
        }
    } else if (comp->getNumberOfTetrahedra() == 4) {
        if (comp->isOrientable()) {
            // Search for the Whitehead link complement.
            // Note that this could be done with a smaller set of tests
            // since some can be deduced from others, but these tests
            // aren't terribly expensive anyway.
            if (comp->getNumberOfVertices() != 2)
                return 0;
            if (comp->getNumberOfEdges() != 4)
                return 0;
            if (comp->getVertex(0)->getLink() != NVertex::TORUS)
                return 0;
            if (comp->getVertex(1)->getLink() != NVertex::TORUS)
                return 0;
            if (comp->getVertex(0)->getNumberOfEmbeddings() != 8)
                return 0;
            if (comp->getVertex(1)->getNumberOfEmbeddings() != 8)
                return 0;
            // Census says it's the Whitehead link if some edge has
            // degree 8.
            for (int i = 0; i < 4; i++)
                if (comp->getEdge(i)->getNumberOfEmbeddings() == 8)
                    return new NSnapPeaCensusTri(SEC_5, 129);
            return 0;
        }
    }

    // Not recognised after all.
    return 0;
}

NManifold* NSnapPeaCensusTri::getManifold() const {
    return new NSnapPeaCensusManifold(section, index);
}

NAbelianGroup* NSnapPeaCensusTri::getHomologyH1() const {
    // Hard-code the smallest cases.
    if (section == SEC_5) {
        if (index == 0 || index == 4) {
            NAbelianGroup* ans = new NAbelianGroup();
            ans->addRank();
            return ans;
        }
        if (index == 1 || index == 2) {
            NAbelianGroup* ans = new NAbelianGroup();
            ans->addRank();
            ans->addTorsionElement(2);
            return ans;
        }
        if (index == 3) {
            NAbelianGroup* ans = new NAbelianGroup();
            ans->addRank();
            ans->addTorsionElement(5);
            return ans;
        }
        if (index == 129) {
            // Whitehead link complement.
            NAbelianGroup* ans = new NAbelianGroup();
            ans->addRank(2);
            return ans;
        }
    }

    // Leave it as an unknown.
    return 0;
}

std::ostream& NSnapPeaCensusTri::writeName(std::ostream& out) const {
    out << "SnapPea " << section;

    // Pad the index with leading zeroes.
    // All sections are written with three-digit indices, except for
    // 7-tetrahedron orientable which uses four-digit indices.
    if (section == SEC_7_OR && index < 1000)
        out << '0';
    if (index < 100)
        out << '0';
    if (index < 10)
        out << '0';
    out << index;

    return out;
}

std::ostream& NSnapPeaCensusTri::writeTeXName(std::ostream& out) const {
    out << section << "_{";

    // Pad the index with leading zeroes.
    // All sections are written with three-digit indices, except for
    // 7-tetrahedron orientable which uses four-digit indices.
    if (section == SEC_7_OR && index < 1000)
        out << '0';
    if (index < 100)
        out << '0';
    if (index < 10)
        out << '0';
    out << index << '}';

    return out;
}

} // namespace regina

