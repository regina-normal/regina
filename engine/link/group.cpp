
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "link.h"

namespace regina {

GroupPresentation Link::group(bool simplify) const {
    if (crossings_.empty()) {
        // This is a zero-crossing unlink.
        return GroupPresentation(components_.size());
    }

    // We have a non-zero number of crossings.
    // Build the Wirtinger presentation.
    //
    // We start with just the generators corresponding to sections of
    // the diagram that include crossings; we will pick up any additional
    // generators for zero-crossing unknot components when we traverse the
    // link shortly.
    GroupPresentation g(crossings_.size());

    // We will need to number the "over-segments" - coniguous sections
    // of the knot that consist entirely of over-crossings.
    // Construct a map from arc IDs to "over-segment" IDs, by traversing
    // each component one at a time.
    int* strandToSection = new int[2 * crossings_.size()];
    int currSegment = 0;

    for (StrandRef comp : components_) {
        if (! comp) {
            // This is a zero-crossing unknot component.
            g.addGenerator();
            continue;
        }

        // Start our traversal of each component from an under-crossing,
        // so we are guaranteed that this is the beginning of an over-segment.
        StrandRef start = comp;
        if (start.strand() > 0) {
            if (components_.size() == 1) {
                // Just jump immediately to the under-strand at this crossing.
                start.jump();
            } else {
                // There is no guarantee that the under-strand is part
                // of the same component.  Instead, walk along the component
                // until we find an under-strand.
                StrandRef s = start;
                do {
                    ++s;
                } while (s.strand() > 0 && s != start);

                start = s;

                // It is possible that we never found an under-strand.
                // This happens when the entire component is an unknot
                // with no self-crossings that is overlaid onto the diagram.
                //
                // How this affects us now is that the total number of
                // "over-segments" (i.e., the number of generators in our
                // group presentation) goes up by one.
                //
                // We will adjust this later.
            }
        }

        StrandRef s = start;
        do {
            strandToSection[s.id()] = currSegment;
            ++s;
            if (s.strand() == 0) {
                // We just passed under a crossing.
                ++currSegment;
            }
        } while (s != start);

        if (start.strand() > 0) {
            // This is the scenario noted above where some component
            // consists entirely of over-crossings.
            // We need to make two adjustments:
            //
            // - increment currSegment, since we are about to move to a new
            //   component but we did not increment it at the end of the
            //   loop just now; and
            //
            // - increment the total number of group generators, since we
            //   based our original count on the number of crossings, which
            //   only counts those over-segments with start and end points.

            ++currSegment;
            g.addGenerator();
        }
    }

    // Now build the presentation.
    for (Crossing* c : crossings_) {
        GroupExpression exp;
        if (c->sign() > 0) {
            exp.addTermLast(strandToSection[c->upper().id()], 1);
            exp.addTermLast(strandToSection[c->lower().id()], 1);
            exp.addTermLast(strandToSection[c->upper().id()], -1);
            exp.addTermLast(strandToSection[c->lower().prev().id()], -1);
        } else {
            exp.addTermLast(strandToSection[c->upper().id()], 1);
            exp.addTermLast(strandToSection[c->lower().prev().id()], 1);
            exp.addTermLast(strandToSection[c->upper().id()], -1);
            exp.addTermLast(strandToSection[c->lower().id()], -1);
        }
        g.addRelation(std::move(exp));
    }

    delete[] strandToSection;

    if (simplify)
        g.simplify();
    return g;
}

} // namespace regina

