
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "link/tangle.h"

namespace regina {

bool Tangle::r1(Crossing* crossing, bool check, bool perform) {
    // Note that, for a planar knot or tangle diagram, if crossing->next(1)
    // returns to the same crossing then it must be the lower strand.

    if (! crossing) {
        // The move cannot be performed.
        // We should just return false, but only if check is true.
        return ! check;
    }

    StrandRef from, to;

    if (crossing->next(1).crossing() == crossing) {
        // The move is legal.
        if (! perform)
            return true;

        // We must have: ... -> crossing(upper) -> crossing(lower) -> ...
        from = crossing->prev_[1];
        to = crossing->next_[0];

        // Reroute the tangle to skip over the crossing.
        rerouteTo(crossing->upper(), crossing->next(0));
        rerouteFrom(crossing->lower(), crossing->prev(1));
    } else if (crossing->prev(1).crossing() == crossing) {
        // The move is legal.
        if (! perform)
            return true;

        // We must have: ... -> crossing(lower) -> crossing(upper) -> ...
        from = crossing->prev_[0];
        to = crossing->next_[1];

        // Reroute the tangle to skip over the crossing.
        rerouteTo(crossing->lower(), crossing->next(1));
        rerouteFrom(crossing->upper(), crossing->prev(0));
    } else {
        // The move cannot be performed.
        return ! check;
    }

    // Destroy the crossing entirely.
    crossings_.erase(crossings_.begin() + crossing->index());
    delete crossing;

    // The move was successfully performed.
    return true;
}

bool Tangle::r2(StrandRef arc, bool check, bool perform) {
    if (! arc) {
        // The move cannot be performed.
        // We should just return false, but only if check is true.
        return ! check;
    }

    StrandRef to = arc.next();
    if (! to) {
        // We reached the end of a string.
        return ! check;
    }

    // Now we know that arc moves from one real crossing to another.

    // The following test also ensures (by planarity) that [arc] and [to]
    // represent different crossings.
    if (arc.strand() != to.strand())
        return ! check;

    StrandRef arc2 = arc;
    arc2.jump();

    // Does the second arc run forwards or backwards?
    // Note that, for a planar knot or tangle diagram, we are guaranteed that if
    // the other strand of [arc] *does* also connect with [to], then it does
    // so on the other strand of [to].
    bool forward = (arc2.next().crossing() == to.crossing());
    bool backward = (arc2.prev().crossing() == to.crossing());

    if (! (forward || backward)) {
        // The move cannot be performed.
        return ! check;
    }

    // The move can be performed!
    if (! perform)
        return true;

    // The situation: (arc, arc2) represent opposite strands of one crossing,
    // and (to, to2) represent opposite strands of another crossing.
    // (The variable to2 has not yet been set, but we will do this shortly.)
    //
    // If forward is true, then we have:
    //
    //   arc   ->  to
    //   arc2  ->  to2
    //
    // If backward is true, then we have:
    //
    //   arc   ->  to
    //   arc2  <-  to2
    //
    // For a tangle, we cannot have both situations simultaneously.

    // When we strip crossings out, there are some pathological cases where
    // it's not just (essentially) pulling two items out of a linked list:
    //
    // (i)   Both arcs represent the same string, and are directly
    //       linked together as arc -> to -> to2 -> arc2.
    //       By planarity, this is true iff
    //       to.next().crossing() == to.crossing().
    //
    // (ii)  Both arcs represent the same string, and are directly
    //       linked together as to2 -> arc2 -> arc -> to.
    //       By planarity, this is true iff
    //       arc.prev().crossing() == arc.crossing().
    //
    // For a tangle, we cannot have both (i) and (ii) simultaneously.
    //
    // Note that, again by planarity, the only way to link both arcs
    // together directly is by method (i) or (ii) above.  That is, we
    // cannot have to joined with arc2, or to2 joined with arc.

    // Strip the two crossings out of the link.
    StrandRef x, y;

    // First we handle cases (i) and (ii) above separately.
    if (to.next() && to.next().crossing() == to.crossing()) {
        // Case (i)
        // x -> arc -> to -> to2 -> arc2 -> y
        rerouteTo(arc, arc2.next());
        rerouteFrom(arc2, arc.prev());
    } else if (arc.prev() && arc.prev().crossing() == arc.crossing()) {
        // Case (ii)
        // x -> to2 -> arc2 -> arc -> to -> y
        StrandRef to2 = arc2.prev();
        rerouteTo(to2, to.next());
        rerouteFrom(to, to2.prev());
    } else {
        // We are not in either case (i) or (ii).

        // Strip the two crossings out of the first arc.
        // x -> arc -> to -> y

        // Since we do not allow closed components in tangles, we cannot
        // have x == to, or y == arc.

        rerouteTo(arc, to.next());
        rerouteFrom(to, arc.prev());

        // Now strip the two crossings out of the second arc.
        if (forward) {
            // x -> arc2 -> to2 -> y
            StrandRef to2 = arc2.next();
            rerouteTo(arc2, to2.next());
            rerouteFrom(to2, arc2.prev());
        } else {
            // x -> to2 -> arc2 -> y
            StrandRef to2 = arc2.prev();
            rerouteTo(to2, arc2.next());
            rerouteFrom(arc2, to2.prev());
        }
    }

    // Finally: destroy the two crossings entirely.
    crossings_.erase(crossings_.begin() + arc.crossing()->index());
    // Note that to.crossing() may have been reindexed.  This is okay,
    // since we still hold the pointer to the crossing.
    crossings_.erase(crossings_.begin() + to.crossing()->index());

    delete arc.crossing();
    delete to.crossing();

    return true;
}

} // namespace regina

