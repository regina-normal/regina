
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

#include "angle/anglestructure.h"
#include "maths/matrix.h"
#include "triangulation/dim3.h"
#include "utilities/xmlutils.h"

namespace regina {

const unsigned long AngleStructure::flagStrict = 1;
const unsigned long AngleStructure::flagTaut = 2;
const unsigned long AngleStructure::flagCalculatedType = 4;
const unsigned long AngleStructure::flagVeering = 8;

MatrixInt* AngleStructureVector::makeAngleEquations(
        const Triangulation<3>* tri) {
    size_t n = tri->size();
    size_t cols = 3 * n + 1;

    // We have one equation per non-boundary edge plus one per tetrahedron.
    long rows = long(tri->countEdges()) + long(tri->size());
    for (Triangulation<3>::BoundaryComponentIterator bit =
            tri->boundaryComponents().begin();
            bit != tri->boundaryComponents().end(); bit++)
        rows -= (*bit)->countEdges();

    MatrixInt* eqns = new MatrixInt(rows, cols);
    size_t row = 0;

    size_t index;
    for (Triangulation<3>::EdgeIterator eit = tri->edges().begin();
            eit != tri->edges().end(); eit++) {
        if ((*eit)->isBoundary())
            continue;
        for (auto& emb : **eit) {
            index = emb.tetrahedron()->index();
            if (emb.edge() < 3)
                eqns->entry(row, 3 * index + emb.edge()) += 1;
            else
                eqns->entry(row, 3 * index + 5 - emb.edge()) += 1;
        }
        eqns->entry(row, cols - 1) = -2;
        ++row;
    }
    for (index = 0; index < n; index++) {
        eqns->entry(row, 3 * index) = 1;
        eqns->entry(row, 3 * index + 1) = 1;
        eqns->entry(row, 3 * index + 2) = 1;
        eqns->entry(row, cols - 1) = -1;
        ++row;
    }

    return eqns;
}

AngleStructure* AngleStructure::clone() const {
    AngleStructure* ans = new AngleStructure(triangulation_,
        new AngleStructureVector(*vector));
    ans->flags = flags;
    return ans;
}

Rational AngleStructure::angle(size_t tetIndex, int edgePair)
        const {
    const LargeInteger& num = (*vector)[3 * tetIndex + edgePair];
    const LargeInteger& den =
        (*vector)[3 * triangulation_->size()];

    LargeInteger gcd = den.gcd(num);
    if (gcd < 0)
        gcd.negate();
    return Rational(num.divExact(gcd), den.divExact(gcd));
}

void AngleStructure::writeTextShort(std::ostream& out) const {
    size_t nTets = triangulation_->size();
    unsigned j;
    for (size_t tet = 0; tet < nTets; tet++) {
        if (tet > 0)
            out << " ; ";
        for (j=0; j<3; j++) {
            if (j > 0)
                out << ' ';
            out << angle(tet, j);
        }
    }
}

void AngleStructure::writeXMLData(std::ostream& out) const {
    // Write the vector length.
    size_t vecLen = vector->size();
    out << "  <struct len=\"" << vecLen << "\"> ";

    // Write the non-zero elements.
    LargeInteger entry;
    for (size_t i = 0; i < vecLen; i++) {
        entry = (*vector)[i];
        if (entry != 0)
            out << i << ' ' << entry << ' ';
    }

    // Write properties.
    /** Flags in data files are deprecated as of Regina 4.93.
    out << regina::xml::xmlValueTag("flags", flags);
    */

    // Write the closing tag.
    out << "</struct>\n";
}

void AngleStructure::calculateType() const {
    size_t size = vector->size();
    if (size == 1) {
        // We have no tetrahedra, which means this angle structure has it all:
        // strict, taut and veering.
        flags |= flagStrict;
        flags |= flagTaut;
        flags |= flagVeering;
        flags |= flagCalculatedType;
        return;
    }

    bool taut = true;
    bool strict = true;

    // Run through the tetrahedra one by one.
    const LargeInteger& scale = (*vector)[size - 1];
    size_t pair;
    for (size_t base = 0; base < size - 1; base += 3) {
        for (pair = 0; pair < 3; pair++) {
            if ((*vector)[base + pair] == scale) {
                // We have a pi; thus all three angles in this
                // tetrahedron are pi or zero.
                strict = false;
                break;
            } else if ((*vector)[base + pair] == LargeInteger::zero)
                strict = false;
            else
                taut = false;
        }
        if ((! strict) && (! taut))
            break;
    }

    // Update the flags as appropriate.
    if (strict)
        flags |= flagStrict;
    else
        flags &= (~flagStrict);

    if (taut) {
        // This structure is taut.
        flags |= flagTaut;

        // Is it veering also?
        bool veering = true;
        if (triangulation_->isOrientable()) {
            long nEdges = triangulation_->countEdges();
            int* edgeColour = new int[nEdges];
            std::fill(edgeColour, edgeColour + nEdges, (int)0);
            const Tetrahedron<3>* tet;
            int orient;
            long e;
            for (unsigned i = 0; i < triangulation_->size();
                    ++i) {
                tet = triangulation_->tetrahedron(i);
                orient = tet->orientation();
                if ((*vector)[3 * i] > 0) {
                    // Edges 0,5 are marked as pi.
                    // For a positively oriented tetrahedron:
                    // Edges 1,4 vs 2,3 are of colour +1 vs -1.
                    e = tet->edge(1)->index();
                    if (edgeColour[e] == -orient)
                        veering = false;
                    else
                        edgeColour[e] = orient;

                    e = tet->edge(4)->index();
                    if (edgeColour[e] == -orient)
                        veering = false;
                    else
                        edgeColour[e] = orient;

                    e = tet->edge(2)->index();
                    if (edgeColour[e] == orient)
                        veering = false;
                    else
                        edgeColour[e] = -orient;

                    e = tet->edge(3)->index();
                    if (edgeColour[e] == orient)
                        veering = false;
                    else
                        edgeColour[e] = -orient;
                } else if ((*vector)[3 * i + 1] > 0) {
                    // Edges 1,4 are marked as pi.
                    // For a positively oriented tetrahedron:
                    // Edges 2,3 vs 0,5 are of colour +1 vs -1.
                    e = tet->edge(2)->index();
                    if (edgeColour[e] == -orient)
                        veering = false;
                    else
                        edgeColour[e] = orient;

                    e = tet->edge(3)->index();
                    if (edgeColour[e] == -orient)
                        veering = false;
                    else
                        edgeColour[e] = orient;

                    e = tet->edge(0)->index();
                    if (edgeColour[e] == orient)
                        veering = false;
                    else
                        edgeColour[e] = -orient;

                    e = tet->edge(5)->index();
                    if (edgeColour[e] == orient)
                        veering = false;
                    else
                        edgeColour[e] = -orient;
                } else if ((*vector)[3 * i + 2] > 0) {
                    // Edges 2,3 are marked as pi.
                    // For a positively oriented tetrahedron:
                    // Edges 0,5 vs 1,4 are of colour +1 vs -1.
                    e = tet->edge(0)->index();
                    if (edgeColour[e] == -orient)
                        veering = false;
                    else
                        edgeColour[e] = orient;

                    e = tet->edge(5)->index();
                    if (edgeColour[e] == -orient)
                        veering = false;
                    else
                        edgeColour[e] = orient;

                    e = tet->edge(1)->index();
                    if (edgeColour[e] == orient)
                        veering = false;
                    else
                        edgeColour[e] = -orient;

                    e = tet->edge(4)->index();
                    if (edgeColour[e] == orient)
                        veering = false;
                    else
                        edgeColour[e] = -orient;
                }
                if (! veering)
                    break;
            }
            delete[] edgeColour;
        } else {
            // Only orientable triangulations can be veering.
            veering = false;
        }

        if (veering)
            flags |= flagVeering;
        else
            flags &= (~flagVeering);
    } else {
        // Not taut, and therefore not veering either.
        flags &= (~flagTaut);
        flags &= (~flagVeering);
    }

    flags |= flagCalculatedType;
}

} // namespace regina

