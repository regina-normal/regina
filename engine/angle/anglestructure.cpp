
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

std::weak_ordering AngleStructure::operator <=> (const AngleStructure& rhs)
        const {
    if (triangulation_->size() != rhs.triangulation_->size())
        return triangulation_->size() <=> rhs.triangulation_->size();

    return std::lexicographical_compare_three_way(
        vector_.begin(), vector_.end(), rhs.vector_.begin(), rhs.vector_.end());
}

Rational AngleStructure::angle(size_t tetIndex, int edgePair) const {
    const Integer& num = vector_[3 * tetIndex + edgePair];
    const Integer& den = vector_[3 * triangulation_->size()];

    Integer gcd = den.gcd(num); // Guaranteed non-negative
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
    size_t vecLen = vector_.size();
    out << "  <struct len=\"" << vecLen << "\"> ";

    // Write the non-zero elements.
    Integer entry;
    for (size_t i = 0; i < vecLen; i++) {
        entry = vector_[i];
        if (entry != 0)
            out << i << ' ' << entry << ' ';
    }

    // Write the closing tag.
    out << "</struct>\n";
}

void AngleStructure::calculateType() const {
    size_t size = vector_.size();
    if (size == 1) {
        // We have no tetrahedra, which means this angle structure has it all:
        // strict, taut and veering.
        flags_ |= flagStrict;
        flags_ |= flagTaut;
        flags_ |= flagVeering;
        flags_ |= flagCalculatedType;
        return;
    }

    bool taut = true;
    bool strict = true;

    // Run through the tetrahedra one by one.
    const Integer& scale = vector_[size - 1];
    size_t pair;
    for (size_t base = 0; base < size - 1; base += 3) {
        for (pair = 0; pair < 3; pair++) {
            if (vector_[base + pair] == scale) {
                // We have a pi; thus all three angles in this
                // tetrahedron are pi or zero.
                strict = false;
                break;
            } else if (vector_[base + pair] == 0)
                strict = false;
            else
                taut = false;
        }
        if ((! strict) && (! taut))
            break;
    }

    // Update the flags as appropriate.
    if (strict)
        flags_ |= flagStrict;
    else
        flags_ &= (~flagStrict);

    if (taut) {
        // This structure is taut.
        flags_ |= flagTaut;

        // Get a local reference to the triangulation so we do not have
        // to repeatedly bounce through the snapshot.
        const Triangulation<3>& tri(*triangulation_);

        // Is it veering also?
        bool veering = true;
        if (tri.isOrientable()) {
            long nEdges = tri.countEdges();
            int* edgeColour = new int[nEdges];
            std::fill(edgeColour, edgeColour + nEdges, (int)0);
            const Tetrahedron<3>* tet;
            int orient;
            long e;
            for (unsigned i = 0; i < tri.size();
                    ++i) {
                tet = tri.tetrahedron(i);
                orient = tet->orientation();
                if (vector_[3 * i] > 0) {
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
                } else if (vector_[3 * i + 1] > 0) {
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
                } else if (vector_[3 * i + 2] > 0) {
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
            flags_ |= flagVeering;
        else
            flags_ &= (~flagVeering);
    } else {
        // Not taut, and therefore not veering either.
        flags_ &= (~flagTaut);
        flags_ &= (~flagVeering);
    }

    flags_ |= flagCalculatedType;
}

} // namespace regina

