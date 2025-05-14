
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

#include "link/link.h"

namespace regina {

bool Link::internalR1(Crossing* crossing, bool check, bool perform) {
    if (! crossing) {
        // The move cannot be performed.
        // We should just return false, but only if check is true.
        return ! check;
    }

    if (crossing->next(1) == crossing->lower()) {
        // The move is legal.
        if (! perform)
            return true;

        ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

        if (crossing->next(0).crossing() == crossing) {
            // This is a 1-crossing unknot component, and we will convert it
            // to a zero-crossing unknot component.
            for (StrandRef& c : components_)
                if (c.crossing() == crossing) {
                    // We found the component!
                    c = StrandRef();
                    break;
                }
        } else {
            // The twist runs from the upper strand to the lower.
            StrandRef from = crossing->prev_[1];
            StrandRef to = crossing->next_[0];
            from.crossing()->next_[from.strand()] = to;
            to.crossing()->prev_[to.strand()] = from;

            // Ensure that no component uses this crossing as it starting point.
            for (StrandRef& c : components_)
                if (c.crossing() == crossing) {
                    c = to;
                    break;
                }
        }

        // Destroy the crossing entirely.
        // This has to happen before the ChangeAndClearSpan goes out of scope.
        crossings_.erase(crossings_.begin() + crossing->index());
        delete crossing;
    } else if (crossing->next(0) == crossing->upper()) {
        // The move is legal.
        if (! perform)
            return true;

        ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

        // The twist runs from the lower strand to the upper.
        StrandRef from = crossing->prev_[0];
        StrandRef to = crossing->next_[1];
        from.crossing()->next_[from.strand()] = to;
        to.crossing()->prev_[to.strand()] = from;

        // Ensure that no component uses this crossing as it starting point.
        for (StrandRef& c : components_)
            if (c.crossing() == crossing) {
                c = to;
                break;
            }

        // Destroy the crossing entirely.
        // This has to happen before the ChangeAndClearSpan goes out of scope.
        crossings_.erase(crossings_.begin() + crossing->index());
        delete crossing;
    } else {
        // The move cannot be performed.
        return ! check;
    }

    // The move was successfully performed.
    return true;
}

bool Link::internalR1(StrandRef arc, int side, int sign, bool check,
        bool perform) {
    if (! arc) {
        // A null reference.  Find the first zero-crossing component.
        for (StrandRef& comp : components_)
            if (! comp) {
                // Found it!
                if (perform) {
                    ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

                    auto* c = new Crossing(sign);
                    c->next_[0] = c->prev_[0] = StrandRef(c, 1);
                    c->next_[1] = c->prev_[1] = StrandRef(c, 0);

                    crossings_.push_back(c);

                    // We can start the component at either strand of c.
                    // However, it is perhaps nice to not break up the
                    // R1 twist (as opposed to the remaining part of the
                    // unknot, which also becomes a twist in its own right).
                    if ((side == 0 && sign < 0) || (side == 1 && sign > 0))
                        comp = StrandRef(c, 1);
                    else
                        comp = StrandRef(c, 0);
                }

                return true;
            }

        // We didn't find a zero-crossing component.
        // This is an invalid move.
        // We should just return false, but only if check is true.
        return ! check;
    }

    // We have an actual arc, which means the move is always legal.
    if (! perform)
        return true;

    ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

    // Insert the twist.
    auto* c = new Crossing(sign);
    StrandRef to = arc.next();
    if ((side == 0 && sign > 0) || (side == 1 && sign < 0)) {
        // The link follows (c, lower) then (c, upper).
        c->prev_[0] = arc;
        c->next_[1] = to;
        to.crossing()->prev_[to.strand()] = c->next_[0] = StrandRef(c, 1);
        arc.crossing()->next_[arc.strand()] = c->prev_[1] = StrandRef(c, 0);
    } else {
        // The link follows (c, upper) then (c, lower).
        c->prev_[1] = arc;
        c->next_[0] = to;
        to.crossing()->prev_[to.strand()] = c->next_[1] = StrandRef(c, 0);
        arc.crossing()->next_[arc.strand()] = c->prev_[0] = StrandRef(c, 1);
    }
    crossings_.push_back(c);

    return true;
}

bool Link::internalR2(StrandRef arc, bool check, bool perform) {
    if (! arc) {
        // The move cannot be performed.
        // We should just return false, but only if check is true.
        return ! check;
    }

    StrandRef to = arc.next();

    // Note: by testing that the two crossings have different signs, we also
    // ensure that they are indeed different crossings.
    if (arc.strand() != to.strand() ||
            arc.crossing()->sign() == to.crossing()->sign())
        return ! check;

    StrandRef arc2 = arc;
    arc2.jump();
    StrandRef to2 = to;
    to2.jump();

    // Does the second arc run forwards or backwards (or both)?
    bool forward = (arc2.next() == to2);
    bool backward = (arc2.prev() == to2);

    if (! (forward || backward)) {
        // The move cannot be performed.
        return ! check;
    }

    // The move can be performed!
    if (! perform)
        return true;

    ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

    // The situation: (arc, arc2) represent opposite strands of one crossing,
    // and (to, to2) represent opposite strands of another crossing.
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
    // Of course it is possible to have both situations simultaneously;
    // this happens iff, after the move, the second arc becomes a
    // zero-crossing unknot component.

    // When we strip crossings out, there are some pathological cases
    // where it's not just (essentially) pulling two items out of a linked
    // list:
    //
    // (i)    Both arcs represent different components and the first arc
    //        becomes a zero-crossing component; i.e., arc -> to -> arc.
    //        This case has two variants: either arc2 -> to2, or to2 -> arc2.
    //
    // (ii)   Both arcs represent different components and the second arc
    //        becomes a zero-crossing component; i.e., arc2 -> to2 -> arc2.
    //        This is true iff (forward && backward).
    //
    // (iii)  Both (i) and (ii) occur simultaneously; i.e., we have two
    //        two-crossing components that both become zero-crossing components.
    //
    // (iv)   Both arcs represent the same component, and are directly
    //        linked together as arc -> to -> to2 -> arc2.
    //        This is true iff to.next().crossing() == to.crossing().
    //
    // (v)    Both arcs represent the same component, and are directly
    //        linked together as to2 -> arc2 -> arc -> to.
    //        This is true iff arc.prev().crossing() == arc.crossing().
    //
    // (vi)   Both (iv) and (v) occur simultaneously; i.e., we have a
    //        four-crossing component that becomes a zero-crossing component.
    //
    // (vii)  Both arcs represent the same component, and are directly
    //        linked together as arc -> to -> arc2 -> to2.
    //        This can only occur with virtual link diagrams.
    //
    // (viii) Both arcs represent the same component, and are directly
    //        linked together as arc2 -> to2 -> arc -> to.
    //        This can only occur with virtual link diagrams.
    //
    // (ix)   Both (vii) and (viii) occur simultaneously, so we have a
    //        four-crossing virtual component that becomes a zero-crossing
    //        component.

    // The boolean unknot1 tracks whether the component involving arc and to
    // becomes a zero-crossing unknot.
    // The boolean unknot2 tracks whether the component involving arc2 and to2
    // becomes a zero-crossing unknot.
    // Note that these could both be the same link component (which is fine).
    // We keep this information for when we update components_ later on.
    bool unknot1 = false;
    bool unknot2 = false;

    // Strip the two crossings out of the link.
    // In the tests below, recall that we already know that arc -> to, and we
    // know that either arc2 -> to2 or to2 -> arc2.

    // First we handle cases (iv, v, vi) separately.
    if (to.next().crossing() == to.crossing()) {
        // We have arc -> to -> to2, and therefore arc -> to -> to2 -> arc2.
        // This means we have case (iv) or (vi).
        if (arc.prev().crossing() == arc.crossing()) {
            // Case (vi): arc2 -> arc -> to -> to2 -> arc2
            unknot1 = unknot2 = true;
        } else {
            // Case (iv): ... -> arc -> to -> to2 -> arc2 -> ...
            Link::join(arc.prev(), arc2.next());
        }
    } else if (arc.prev().crossing() == arc.crossing()) {
        // We have arc2 -> arc -> to, and therefore to2 -> arc2 -> arc -> to.
        // This means case (v) or (vi), but we have already handled (vi).
        // Therefore we have case (v): ... -> to2 -> arc2 -> arc -> to -> ...
        Link::join(to2.prev(), to.next());
    } else {
        // We do not have to -> to2, and we do not have arc2 -> arc.
        // In particular, we are not in any of cases (iv, v, vi).
        if (arc.prev().crossing() == to.crossing()) {
            // Either to -> arc -> to, or to2 -> arc -> to, which means
            // either to -> arc -> to, or arc2 -> to2 -> arc -> to.
            // This means we have case (i), (iii), (viii), or (ix).
            if (backward) {
                // We have to2 -> arc2, which from the information above
                // means to -> arc -> to && to2 -> arc2.
                unknot1 = true;
                if (forward) {
                    // Case (iii): to -> arc -> to && to2 -> arc2 -> to2
                    unknot2 = true;
                } else {
                    // Case (i): to -> arc -> to && ... -> to2 -> arc2 -> ...
                    Link::join(to2.prev(), arc2.next());
                }
            } else {
                // We have arc2 -> to2, and to2 -/-> arc2.  This means
                // either to -> arc -> to && ... -> arc2 -> to2 -> ...,
                // or arc2 -> to2 -> arc -> to.
                // This means case (i), (viii) or (ix).
                if (arc2.prev().crossing() == to.crossing()) {
                    // This means to -> arc2, and therefore
                    // case (ix): arc2 -> to2 -> arc -> to -> arc2.
                    unknot1 = unknot2 = true;
                } else {
                    // This means to -/-> arc2 and to2 -/-> arc2.  That is,
                    // either to -> arc -> to && ... -> arc2 -> to2 -> ...,
                    // or ... -> arc2 -> to2 -> arc -> to -> ... .
                    if (to2.next().crossing() == arc.crossing()) {
                        // Case (viii): ... -> arc2 -> to2 -> arc -> to -> ...
                        Link::join(arc2.prev(), to.next());
                    } else {
                        // Case (i): to -> arc -> to && ... -> arc2 -> to2 -> ...
                        unknot1 = true;
                        Link::join(arc2.prev(), to2.next());
                    }
                }
            }
        } else {
            // We have to -/-> to2, arc2 -/-> arc, to -/-> arc, to2 -/-> arc.
            // In particular, we are not in any of cases
            // (i), (iii), (iv), (v), (vi), (viii), (ix).
            // Therefore we have case (ii), (vii), or nothing.
            if (forward) {
                if (backward) {
                    // We have arc -> to && arc2 -> to2 -> arc2, which means
                    // case (ii): ... -> arc -> to -> ... && arc2 -> to2 -> arc2
                    unknot2 = true;
                    Link::join(arc.prev(), to.next());
                } else {
                    // We have arc -> to, arc2 -> to2 && to2 -/-> arc2.
                    // This means case (vii) or nothing:
                    // ... -> arc -> to && arc2 -> to2 -> ...
                    if (to.next().crossing() == arc.crossing()) {
                        // Case (vii): ... -> arc -> to -> arc2 -> to2 -> ...
                        Link::join(arc.prev(), to2.next());
                    } else {
                        // No special case (forward version):
                        // ... -> arc -> to -> ... && ... -> arc2 -> to2 -> ...
                        Link::join(arc.prev(), to.next());
                        Link::join(arc2.prev(), to2.next());
                    }
                }
            } else {
                // We have arc -> to, to2 -> arc2 && arc2 -/-> to2.  This means
                // ... -> arc -> to -> ... && ... -> to2 -> arc2 -> ...;
                // that is, no special case (backward version).
                Link::join(arc.prev(), to.next());
                Link::join(to2.prev(), arc2.next());
            }
        }
    }

    // Now fix components_.
    // Here we ensure that any zero-crossing unknots are marked as such,
    // and that any other components that start at one of the to-be-deleted
    // crossings have their start points moved.
    int fixed = 0;
    for (StrandRef& comp : components_) {
        if (comp.crossing() == arc.crossing() ||
                comp.crossing() == to.crossing()) {
            // In the test below, we use the fact that arc.strand() is
            // known to be the same as to.strand().
            if ((unknot1 && (comp.strand() == arc.strand())) ||
                    (unknot2 && (comp.strand() == arc2.strand()))) {
                // This component becomes a zero-crossing unknot.
                comp = StrandRef();
            } else {
                // This component still has crossings; we just need to
                // advance the start point out of the crossings that are
                // being removed.
                do
                    ++comp;
                while (comp.crossing() == arc.crossing() ||
                        comp.crossing() == to.crossing());
            }

            // There are at most two components that need fixing.
            if (++fixed == 2)
                break;
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

bool Link::internalR2(StrandRef upperArc, int upperSide, StrandRef lowerArc,
        int lowerSide, bool classicalOnly, bool perform) {
    std::vector<StrandRef>::iterator upperUnknot, lowerUnknot;

    if (! (upperArc && lowerArc)) {
        // We have references to one or two zero-crossing components.
        // Find them.
        auto it = components_.begin();

        if (! upperArc) {
            for ( ; it != components_.end(); ++it)
                if (! it->crossing()) {
                    upperUnknot = it;
                    break;
                }
            if (it == components_.end()) {
                return false;
            }

            // Step forwards so we can search for lowerUnknot if need be.
            ++it;
        }

        if (! lowerArc) {
            for ( ; it != components_.end(); ++it)
                if (! it->crossing()) {
                    lowerUnknot = it;
                    break;
                }
            if (it == components_.end()) {
                return false;
            }
        }
    }

    // Carry out any remaining checks.
    if (classicalOnly && upperArc && lowerArc) {
        // Ensure that the two given sides-of-arcs belong to the
        // same 2-cell in the knot diagram, or else belong to different link
        // components entirely.

        // We walk around the 2-cell from upperArc, ensuring that we
        // always turn left.
        //
        // At each stage we consider an edge of this 2-cell:
        //
        // - ref points to the strand of the crossing at the beginning
        //   of the edge, with respect to the direction in which we are
        //   walking around the 2-cell;
        // - arc points to the strand of the crossing at the beginning
        //   of the edge, with respect to the orientation of the link.
        // - forward indicates whether these two directions are the same.
        //
        // Note that we don't actually set arc until we get to the test at the
        // end of the while loop.
        //
        StrandRef ref = upperArc;
        bool forward;
        if (upperSide == 0) {
            forward = true;
        } else {
            // Since we are traversing the arc backwards, we need to
            // jump to the other endpoint.
            ref = ref.next();
            forward = false;
        }

        StrandRef arc;
        while (true) {
            // Move to the next edge of the boundary of this 2-cell.
            if (forward) {
                ref = ref.next();
                ref.jump();

                // forward remains true for (sign, strand):
                // +, 0
                // -, 1
                if (ref.crossing()->sign() > 0)
                    forward = (0 == ref.strand());
                else
                    forward = (0 != ref.strand());
            } else {
                ref = ref.prev();
                ref.jump();

                // forward becomes true for (sign, strand):
                // -, 0
                // +, 1
                if (ref.crossing()->sign() > 0)
                    forward = (0 != ref.strand());
                else
                    forward = (0 == ref.strand());
            }

            arc = (forward ? ref : ref.prev());

            if (arc == upperArc &&
                    ((forward && upperSide == 0) ||
                     ((! forward) && upperSide != 0))) {
                // We completed the cycle and never found the correct side of
                // lowerArc.  The move is still legal, however, iff upperArc
                // and lowerArc are in different connected components of the
                // underlying 4-valent graph.
                if (connected(upperArc.crossing(), lowerArc.crossing()))
                    return false; // move is illegal
                else
                    break; // move is legal
            }
            if (arc == lowerArc &&
                    ((forward && lowerSide == 0) ||
                     ((! forward) && lowerSide != 0))) {
                // We found the correct side of lowerArc - the move is legal!
                break;
            }
        }
        // If we made it out of the while loop, then we found the
        // correct side of lowerArc on our 2-cell.
    }

    // The move can be performed!
    if (! perform)
        return true;

    ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

    auto* pos = new Crossing(1);
    auto* neg = new Crossing(-1);

    // Graft the new crossings into the upper arc.
    if (lowerSide == 0) {
        // Upper strand: pos -> neg
        pos->next_[1] = neg->strand(1);
        neg->prev_[1] = pos->strand(1);

        if (upperArc) {
            StrandRef to = upperArc.next();
            upperArc.crossing()->next_[upperArc.strand()] = pos->strand(1);
            pos->prev_[1] = upperArc;
            to.crossing()->prev_[to.strand()] = neg->strand(1);
            neg->next_[1] = to;
        } else {
            // Complete the new 2-crossing cycle.
            neg->next_[1] = pos->strand(1);
            pos->prev_[1] = neg->strand(1);
            *upperUnknot = pos->strand(1);
        }
    } else {
        // Upper strand: neg -> pos
        neg->next_[1] = pos->strand(1);
        pos->prev_[1] = neg->strand(1);

        if (upperArc) {
            StrandRef to = upperArc.next();
            upperArc.crossing()->next_[upperArc.strand()] = neg->strand(1);
            neg->prev_[1] = upperArc;
            to.crossing()->prev_[to.strand()] = pos->strand(1);
            pos->next_[1] = to;
        } else {
            // Complete the new 2-crossing cycle.
            pos->next_[1] = neg->strand(1);
            neg->prev_[1] = pos->strand(1);
            *upperUnknot = neg->strand(1);
        }
    }

    // Graft the new crossings into the lower arc.
    if (upperSide == 1) {
        // Lower strand: pos -> neg
        pos->next_[0] = neg->strand(0);
        neg->prev_[0] = pos->strand(0);

        if (lowerArc) {
            StrandRef to = lowerArc.next();
            lowerArc.crossing()->next_[lowerArc.strand()] = pos->strand(0);
            pos->prev_[0] = lowerArc;
            to.crossing()->prev_[to.strand()] = neg->strand(0);
            neg->next_[0] = to;
        } else {
            // Complete the new 2-crossing cycle.
            neg->next_[0] = pos->strand(0);
            pos->prev_[0] = neg->strand(0);
            *lowerUnknot = pos->strand(0);
        }
    } else {
        // Lower strand: neg -> pos
        neg->next_[0] = pos->strand(0);
        pos->prev_[0] = neg->strand(0);

        if (lowerArc) {
            StrandRef to = lowerArc.next();
            lowerArc.crossing()->next_[lowerArc.strand()] = neg->strand(0);
            neg->prev_[0] = lowerArc;
            to.crossing()->prev_[to.strand()] = pos->strand(0);
            pos->next_[0] = to;
        } else {
            // Complete the new 2-crossing cycle.
            pos->next_[0] = neg->strand(0);
            neg->prev_[0] = pos->strand(0);
            *lowerUnknot = neg->strand(0);
        }
    }

    // Clean up.
    crossings_.push_back(pos);
    crossings_.push_back(neg);

    return true;
}

bool Link::internalR3(StrandRef arc, int side, bool check, bool perform) {
    if (! arc) {
        // The move cannot be performed.
        // We should just return false, but only if check is true.
        return ! check;
    }

    // Find the three crossings at the vertices of the triangle, and
    // determine whether the three edges of the triangle leave them in the
    // forward or backward directions.
    StrandRef s[4];
    bool forward[4];

    s[0] = arc;
    forward[0] = true;

    int i;
    for (i = 1; i < 4; ++i) {
        if (forward[i-1]) {
            s[i] = s[i-1].next();
            s[i].jump();

            // forward[i] is true for (side, sign, strand):
            // 0, +, 0
            // 1, -, 0
            // 0, -, 1
            // 1, +, 1
            if (s[i].crossing()->sign() > 0)
                forward[i] = (side == s[i].strand());
            else
                forward[i] = (side != s[i].strand());
        } else {
            s[i] = s[i-1].prev();
            s[i].jump();

            // forward[i] is true for (side, sign, strand):
            // 1, +, 0
            // 0, -, 0
            // 1, -, 1
            // 0, +, 1
            if (s[i].crossing()->sign() > 0)
                forward[i] = (side != s[i].strand());
            else
                forward[i] = (side == s[i].strand());
        }
    }

    if (check) {
        if (s[3] != s[0] || forward[3] != forward[0])
            return false;
        if (s[0].crossing() == s[1].crossing() ||
                s[1].crossing() == s[2].crossing() ||
                s[0].crossing() == s[2].crossing())
            return false;
        if (s[0].strand() == s[1].strand() && s[1].strand() == s[2].strand())
            return false;
    }

    if (! perform)
        return true;

    ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

    // Reorder the two crossings on each of the three edges.
    StrandRef x, first, second, y;
    for (i = 0; i < 3; ++i) {
        if (forward[i]) {
            first = s[i];
            second = first.next();
        } else {
            second = s[i];
            first = second.prev();
        }

        x = first.prev();
        if (x == second) {
            // This means that (first, second) is a 2-crossing cycle.
            // Swapping the crossings will have no effect.
            continue;
        }
        y = second.next();

        // We have: x -> first -> second -> y
        // We want: x -> second -> first -> y

        x.crossing()->next_[x.strand()] = second;
        second.crossing()->next_[second.strand()] = first;
        first.crossing()->next_[first.strand()] = y;

        y.crossing()->prev_[y.strand()] = first;
        first.crossing()->prev_[first.strand()] = second;
        second.crossing()->prev_[second.strand()] = x;
    }

    return true;
}

} // namespace regina

