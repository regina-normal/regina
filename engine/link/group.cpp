
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "link.h"

namespace regina {

GroupPresentation Link::internalGroup(bool flip, bool simplify) const {
    if (crossings_.empty()) {
        // This is a zero-crossing unlink.
        return { components_.size() };
    }

    // We have a non-zero number of crossings.
    // Build the Wirtinger presentation.
    //
    // We start with just the generators corresponding to sections of
    // the diagram that include crossings; we will pick up any additional
    // generators for zero-crossing unknot components when we traverse the
    // link shortly.
    GroupPresentation g(crossings_.size());

    // We will need to number the "segments" - contiguous sections of the link
    // that consist entirely of over-crossings (if flip is false), or entirely
    // of under-crossings (if flip is true).
    // Construct a map from arc IDs to segment IDs, by traversing each
    // component one at a time.
    FixedArray<size_t> strandToSegment(2 * crossings_.size());
    size_t currSegment = 0;

    // The kind of strand at which we start a new segment as we traverse.
    int breakAt = (flip ? 1 : 0);

    for (StrandRef comp : components_) {
        if (! comp) {
            // This is a zero-crossing unknot component.
            g.addGenerator();
            continue;
        }

        // Start our traversal of each component at the beginning of a segment.
        StrandRef start = comp;
        if (start.strand() != breakAt) {
            if (components_.size() == 1) {
                // Just jump immediately to the other strand at this crossing.
                start.jump();
            } else {
                // There is no guarantee that the other strand is part of the
                // same component.  Instead, walk along the component until we
                // find a viable starting point.
                StrandRef s = start;
                do {
                    ++s;
                } while (s.strand() != breakAt && s != start);

                start = s;

                // It is possible that we never found a good starting point.
                // This happens when the entire component is an knot with
                // no self-crossings that is overlaid above the diagram
                // (if flip is false) or under the diagram (if flip is true).
                //
                // How this affects us now is that the total number of
                // segments (i.e., the number of generators in our
                // group presentation) goes up by one.
                //
                // We will adjust this later.
            }
        }

        StrandRef s = start;
        do {
            strandToSegment[s.id()] = currSegment;
            ++s;
            if (s.strand() == breakAt) {
                // We just passed through a crossing that ended a segment.
                ++currSegment;
            }
        } while (s != start);

        if (start.strand() != breakAt) {
            // This is the scenario noted above where some component
            // consists entirely of a closed segment that never gets broken.
            // We need to make two adjustments:
            //
            // - increment currSegment, since we are about to move to a new
            //   component but we did not increment it at the end of the
            //   loop just now; and
            //
            // - increment the total number of group generators, since we
            //   based our original count on the number of crossings, which
            //   only counts those segments with start and end points.

            ++currSegment;
            g.addGenerator();
        }
    }

    // Now build the presentation.
    for (Crossing* c : crossings_) {
        GroupExpression exp;
        if (flip) {
            if (c->sign() < 0) {
                exp.addTermLast(strandToSegment[c->lower().id()], 1);
                exp.addTermLast(strandToSegment[c->upper().id()], 1);
                exp.addTermLast(strandToSegment[c->lower().id()], -1);
                exp.addTermLast(strandToSegment[c->upper().prev().id()], -1);
            } else {
                exp.addTermLast(strandToSegment[c->lower().id()], 1);
                exp.addTermLast(strandToSegment[c->upper().prev().id()], 1);
                exp.addTermLast(strandToSegment[c->lower().id()], -1);
                exp.addTermLast(strandToSegment[c->upper().id()], -1);
            }
        } else {
            if (c->sign() > 0) {
                exp.addTermLast(strandToSegment[c->upper().id()], 1);
                exp.addTermLast(strandToSegment[c->lower().id()], 1);
                exp.addTermLast(strandToSegment[c->upper().id()], -1);
                exp.addTermLast(strandToSegment[c->lower().prev().id()], -1);
            } else {
                exp.addTermLast(strandToSegment[c->upper().id()], 1);
                exp.addTermLast(strandToSegment[c->lower().prev().id()], 1);
                exp.addTermLast(strandToSegment[c->upper().id()], -1);
                exp.addTermLast(strandToSegment[c->lower().id()], -1);
            }
        }
        g.addRelation(std::move(exp));
    }

    if (simplify)
        g.simplify();
    return g;
}

GroupPresentation Link::internalExtendedGroup(bool flip, bool simplify) const {
    if (crossings_.empty()) {
        // This is a zero-crossing unlink.
        return { components_.size() + 1 };
    }

    // We have a non-zero number of crossings.
    // Build the Wirtinger-like presentation as given by Silver and Williams.
    //
    // For strand s, we use generator number s.id() + 1.
    // For the special generator x, we use generator number 0.
    // For zero-crossing unknot components, we use additional generators
    // beyond these indices (which will never appear in any relations).
    GroupPresentation g(2 * crossings_.size() + 1 + countTrivialComponents());

    for (Crossing* c : crossings_) {
        // The first relation is the same regardless of whether we flip.
        GroupExpression r1;
        if (c->sign() > 0) {
            r1.addTermLast(c->upper().id() + 1, 1);
            r1.addTermLast(c->lower().id() + 1, 1);
            r1.addTermLast(c->upper().prev().id() + 1, -1);
            r1.addTermLast(c->lower().prev().id() + 1, -1);
        } else {
            r1.addTermLast(c->upper().prev().id() + 1, 1);
            r1.addTermLast(c->lower().prev().id() + 1, 1);
            r1.addTermLast(c->upper().id() + 1, -1);
            r1.addTermLast(c->lower().id() + 1, -1);
        }
        g.addRelation(std::move(r1));

        // The second relation changes.
        GroupExpression r2;
        if (flip) {
            if (c->sign() > 0) {
                r2.addTermLast(c->lower().id() + 1, 1);
                r2.addTermLast(0, 1);
                r2.addTermLast(c->lower().prev().id() + 1, -1);
                r2.addTermLast(0, -1);
            } else {
                r2.addTermLast(c->lower().prev().id() + 1, 1);
                r2.addTermLast(0, 1);
                r2.addTermLast(c->lower().id() + 1, -1);
                r2.addTermLast(0, -1);
            }
        } else {
            if (c->sign() > 0) {
                r2.addTermLast(c->upper().prev().id() + 1, 1);
                r2.addTermLast(0, 1);
                r2.addTermLast(c->upper().id() + 1, -1);
                r2.addTermLast(0, -1);
            } else {
                r2.addTermLast(c->upper().id() + 1, 1);
                r2.addTermLast(0, 1);
                r2.addTermLast(c->upper().prev().id() + 1, -1);
                r2.addTermLast(0, -1);
            }
        }
        g.addRelation(std::move(r2));
    }

    if (simplify)
        g.simplify();
    return g;
}

} // namespace regina

