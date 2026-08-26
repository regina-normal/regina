
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#include <numeric>
#include "link/link.h"

namespace regina {

std::vector<long> Link::longRotations(StrandRef breakOpen) const {
    if (isEmpty())
        throw FailedPrecondition("longRotation() requires a non-empty knot");
    if (countComponents() > 1)
        throw FailedPrecondition("longRotation() only works with knots, "
            "not multiple component links");
    if (! isClassical())
        throw FailedPrecondition("longRotation() only works with classical "
            "knots, not virtual knots");

    if (size() == 0) {
        // We have a zero-crossing unknot.
        return { 0 }; // std::initializer_list constructor
    }

    if (! breakOpen) {
        // Choose an arbitrary crossing at which to break the knot open.
        breakOpen = crossings_.front()->upper();
    }

    // We need to assign rotation numbers so that, for any cycle in the
    // underlying graph (i.e., with no repeated crossing):
    //
    // - if the cycle uses the broken arc, then the adjusted sum of rotation
    //   numbers is 0;
    // - otherwise, the adjusted sum of rotation numbers is +1 or -1 according
    //   to whether we walk anticlockwise or clockwise around the boundary of
    //   the disc that the cycle bounds in the plane.
    //
    // Here "adjusted sum" means:
    //
    // - walking backwards along a directed arc means we subtract that arc's
    //   rotation number (instead of adding);
    // - when switching between forward vs backward traversal along arcs, we
    //   need to add or subtract half-turns according to how the two arcs
    //   enter or exit the crossing at which they meet.
    //
    // Our plan:
    //
    // - build a tree of arcs rooted at the _endpoint_ of the broken arc that
    //   acts as a spanning tree for the underlying graph, and where every path
    //   from the root to a leaf only traverses arcs in the forward direction;
    // - assign every arc of this tree a rotation number of 0, and also assign
    //   the broken arc a rotation number of 0;
    // - for every other arc, consider the cycle formed by traversing this arc
    //   and walking between its endpoints through the spanning tree, and
    //   assign that arc a rotation number of -1, 0, or +1 in order to satisfy
    //   the cycle sum criterion above.
    //
    // To do this, we need to know:
    //
    // - (i) for each node, whether the path from the root enters via the left
    //   or right incoming arc (or, for the root itself, whether the broken arc
    //   enters via the left or right incoming arc);
    // - (ii) for each node, the time at which we enter and exit the node in a
    //   left-to-right depth-first traversal of the tree, which we use to help
    //   determine whether one node is an ancestor of another, or if not,
    //   whether the path between them through the tree turns left or right at
    //   their common ancestor.

    size_t n = size();
    StrandRef next = breakOpen.next();

    FixedArray<uint8_t> done(n, 0); // number of outgoing arcs processed
    FixedArray<Crossing*> stack(n);
    stack.front() = next.crossing();
    auto top = stack.begin();

    FixedArray<bool> enterLeft(n);
    FixedArray<size_t> entryTime(n);
    FixedArray<size_t> exitTime(n);

    if (next.strand() == 0)
        enterLeft[next.crossing()->index()] = (next.crossing()->sign() < 0);
    else
        enterLeft[next.crossing()->index()] = (next.crossing()->sign() > 0);

    size_t time = 0;
    while (true) {
        Crossing* curr = *top;
        size_t currIndex = curr->index();
        switch (done[currIndex]) {
            case 0:
                // We are entering this crossing.
                entryTime[currIndex] = time++;
                // Fall through to the common code below for cases 0 and 1.
            case 1:
                // Follow the left (case 0) or right (case 1) outgoing arc.
                if (done[currIndex] == 0)
                    next = curr->next(curr->sign() > 0 ? 0 : 1);
                else
                    next = curr->next(curr->sign() > 0 ? 1 : 0);

                ++done[currIndex];
                {
                    size_t nextIdx = next.crossing()->index();
                    if (! done[nextIdx]) {
                        *(++top) = next.crossing();
                        if (next.strand() == 0)
                            enterLeft[nextIdx] = (next.crossing()->sign() < 0);
                        else
                            enterLeft[nextIdx] = (next.crossing()->sign() > 0);
                    }
                }
                break;
            default /* 2 */:
                // Time to exit this crossing.
                exitTime[currIndex] = time++;
                if (top == stack.begin())
                    goto allDone; // break out of the while loop, not the switch
                --top;
                break;
        }
    }
allDone:

    // Deduce the rotation numbers for all arcs, in order of strand ID.
    std::vector<long> ans(2 * n);
    auto it = ans.begin();
    for (auto c : crossings_)
        for (int s = 0; s < 2; ++s) {
            StrandRef src = c->strand(s);
            StrandRef dest = src.next();

            size_t destIdx = dest.crossing()->index();
            bool destEnterLeft;
            if (dest.strand() == 0)
                destEnterLeft = (dest.crossing()->sign() < 0);
            else
                destEnterLeft = (dest.crossing()->sign() > 0);

            // Is this arc part of the spanning tree, or equal to the arc at
            // which we broke the knot open?
            if (destEnterLeft == enterLeft[destIdx]) {
                *it++ = 0;
                continue;
            }

            // Is dest a (non-strict) ancestor of src in the tree?
            size_t srcIdx = src.crossing()->index();
            if (entryTime[destIdx] <= entryTime[srcIdx] &&
                    exitTime[srcIdx] <= exitTime[destIdx]) {
                // Yes.  The arc loops back around towards the root.
                *it++ = (destEnterLeft ? 1 : -1);
                continue;
            }

            bool srcExitLeft;
            if (src.strand() == 0)
                srcExitLeft = (src.crossing()->sign() > 0);
            else
                srcExitLeft = (src.crossing()->sign() < 0);

            // Is src a (strict) ancestor of dest in the tree?
            if (entryTime[srcIdx] < entryTime[destIdx] &&
                    exitTime[destIdx] < exitTime[srcIdx]) {
                // Yes.  The arc runs up away from the root, _possibly_
                // requiring a loop.
                if (srcExitLeft == destEnterLeft)
                    *it++ = 0;
                else
                    *it++ = (destEnterLeft ? 1 : -1);
                continue;
            }

            // This arc is not in the spanning tree, and is not the break point.
            // What we do now depends on the relative positions of src and dest
            // in the spanning tree.
            if (entryTime[srcIdx] < entryTime[destIdx]) {
                // The arc moves up and to the right over the tree.
                *it++ = (destEnterLeft ? 0 : -1);
            } else {
                // The arc moves up and to the left over the tree.
                *it++ = (destEnterLeft ? 1 : 0);
            }
        }

    return ans;
}

const Laurent2<Integer>& Link::theta() const {
    if (components_.size() != 1)
        throw FailedPrecondition("Regina can only compute theta "
            "polynomials for links with exactly one component");
    if (! isClassical())
        throw FailedPrecondition("Regina can only compute theta "
            "polynomials for classical knots, not virtual knots");

    if (theta_.has_value())
        return *theta_;

    if (size() == 0)
        return *(theta_ = Laurent2<Integer>{}); // TODO: what??

    // Choose an arbitrary crossing at which to break the knot open.
    StrandRef breakOpen = crossings_.front()->upper();

    // Build a map that numbers the arcs in order of traversal through the
    // corresponding long knot.  Here *breakOpen* is numbered both 0 (at the
    // end of the arc) and 2n (at the beginning of the arc); we store 0 here.
    size_t n = size();
    FixedArray<size_t> arcOrder(2 * n); // strand ID -> traversal order
    {
        StrandRef s = breakOpen;
        size_t step = 0;
        do {
            arcOrder[(s++).id()] = step++;
        } while (s != breakOpen);
    }

    // Build the matrix A whose determinant yields (after appropriate
    // normalisation) the Alexander polynomial.
    auto a = Matrix<Laurent<Integer>>::identity(2 * n + 1);
    for (auto c : crossings_) {
        size_t i = arcOrder[c->upper().prev().id()];
        size_t j = arcOrder[c->lower().prev().id()];
        a.entry(j, j + 1).set(0, a.entry(j, j + 1)[0] - 1);
        a.entry(i, j + 1).set(0, a.entry(i, j + 1)[0] - 1);
        a.entry(i, i + 1).set(c->sign(), a.entry(i, i + 1)[c->sign()] - 1);
        a.entry(i, j + 1).set(c->sign(), a.entry(i, j + 1)[c->sign()] + 1);
    }

    // Now compute rotation numbers for arcs and extract the Alexander
    // polynomial.  Note that this differs from the polynomial returned by
    // alexander(), since the latter scales by powers of t and/or -1 to ensure
    // that all exponents are non-negative and the constant term is positive.
    //
    // Ad-hoc experimentation suggests that the Alexander computation here is
    // quite a bit slower than alexander(), mostly likely because the matrix
    // is twice as large in each dimension.
    //
    // We use Faddeev-Leverrier for the adjugate computation because the matrix
    // A is sparse and multiplication of Laurent polynomials is expensive.
    auto rot = longRotations(breakOpen);
    auto [adj, det] = a.adjugate(AdjugateAlgorithm::FaddeevLeverrier);

    long shift = -(writhe() + std::reduce(rot.begin(), rot.end())) / 2;
    // The Alexander polynomial is (det * x^shift).
    // TODO: Set alexander_ if we don't already know it.

    // We are trying to avoid denominators, so we do not compute F1, F2 and F3
    // directly.  See below for what we compute instead.

    using L = Laurent2<Integer>;

    // u[0,1,2] = T_{1,2,3} - 1
    // v[0,1,2] = T_{1,2,3}^(-1) - 1
    // w = T_2 + 1
    // x[0,1] = T_2^{1,-1} - 2
    static const L u[3] {
        {{1,0,1}, {0,0,-1}}, {{0,1,1}, {0,0,-1}}, {{1,1,1}, {0,0,-1}} };
    static const L v[3] {
        {{-1,0,1}, {0,0,-1}}, {{0,-1,1}, {0,0,-1}}, {{-1,-1,1}, {0,0,-1}} };
    static const L w {{0,1,1}, {0,0,1}};
    static const L x[2] { {{0,1,1}, {0,0,-2}}, {{0,-1,1}, {0,0,-2}} };

    L delta[3] { {det, 1, 0}, {det, 0, 1}, {det, 1, 1} };

    // sum1 = sum_{crossings c} 2 F_1(c) * (T_2 - 1) ∆_1 ∆_2 ∆_3
    L sum1;
    for (auto c : crossings_) {
        size_t i = arcOrder[c->upper().prev().id()];
        size_t j = arcOrder[c->lower().prev().id()];
        if (c->sign() > 0) {
            L term1 =
                L(adj.entry(j, i), 0, 1) *
                    ( delta[2] * L(adj.entry(i, i), 1, 0).shifted(0, 1)
                    + delta[0] * u[2] * L(adj.entry(j, i), 1, 1)
                    - delta[0] * L(adj.entry(j, j), 1, 1).shifted(0, 1)
                    - delta[0] * u[1] * L(adj.entry(i, i), 1, 1))
                + L(adj.entry(j, j), 0, 1) *
                    ( delta[0] * L(adj.entry(i, i), 1, 1) * 2
                    - delta[2] * L(adj.entry(i, i), 1, 0))
                + L(adj.entry(j, j), 1, 1) *
                    ( delta[1] * L(adj.entry(i, i), 1, 0)
                    - delta[0] * L(adj.entry(i, i), 0, 1))
                - delta[0] * delta[1] * L(adj.entry(i, i), 1, 1);
            sum1 += u[1] * (delta[0] * delta[1] * delta[2] +
                std::move(term1) * 2);

            L term2 = u[0].shifted(0, 1) * L(adj.entry(j, i), 1, 0) *
                ( delta[1] * L(adj.entry(j, j), 1, 1)
                + delta[2] *
                    ( L(adj.entry(j, i).shifted(1), 0, 1)
                    - L(adj.entry(j, j), 0, 1)));
            L term3 = u[2] * L(adj.entry(j, i), 1, 1) *
                ( delta[0] * delta[1]
                - delta[1] * L(adj.entry(i, i), 1, 0).shifted(0, 1)
                + delta[0] * L(adj.entry(i, j), 0, 1)
                + delta[0] * x[0] * L(adj.entry(j, j), 0, 1)
                - delta[1] * u[0] * w * L(adj.entry(j, i), 1, 0));
            sum1 += (std::move(term2) + std::move(term3)) * 2;
        } else {
            // TODO
        }
    }

    // sum2 = sum_{crossings c0, c1} F_2(c0, c1) (T_2 - 1) ∆_1 ∆_2 ∆_3
    L sum2;
    for (auto c0 : crossings_) {
        size_t i0 = arcOrder[c0->upper().prev().id()];
        size_t j0 = arcOrder[c0->lower().prev().id()];
        for (auto c1 : crossings_) {
            size_t i1 = arcOrder[c1->upper().prev().id()];
            size_t j1 = arcOrder[c1->lower().prev().id()];
            if (c0->sign() > 0) {
                if (c1->sign() > 0) {
                    sum2 += u[0] * u[2] *
                        L(adj.entry(j1, i0), 1, 0) *
                        L(adj.entry(j0, i1), 1, 1) *
                        L(
                            adj.entry(i1, i0).shifted(1) +
                            adj.entry(j1, j0) -
                            adj.entry(j1, i0).shifted(1) -
                            adj.entry(i1, j0),
                            0, 1);
                } else {
                    sum2 += u[0] * v[2] *
                        L(adj.entry(j1, i0), 1, 0) *
                        L(adj.entry(j0, i1), 1, 1) *
                        L(
                            adj.entry(i1, i0).shifted(2) +
                            adj.entry(j1, j0).shifted(1) -
                            adj.entry(j1, i0).shifted(2) -
                            adj.entry(i1, j0).shifted(1),
                            0, 1);
                }
            } else {
                if (c1->sign() > 0) {
                    sum2 += v[0] * u[2] *
                        L(adj.entry(j1, i0), 1, 0) *
                        L(adj.entry(j0, i1), 1, 1) *
                        L(
                            adj.entry(i1, i0).shifted(-1) +
                            adj.entry(j1, j0) -
                            adj.entry(j1, i0).shifted(-1) -
                            adj.entry(i1, j0),
                            0, 1);
                } else {
                    sum2 += v[0] * v[2] *
                        L(adj.entry(j1, i0), 1, 0) *
                        L(adj.entry(j0, i1), 1, 1) *
                        L(
                            adj.entry(i1, i0) +
                            adj.entry(j1, j0).shifted(1) -
                            adj.entry(j1, i0) -
                            adj.entry(i1, j0).shifted(1),
                            0, 1);
                }
            }
        }
    }

    // sum3 = sum_{edges k} 2 F_3(k) * ∆_3, as a Laurent polynomial in T3 := xy
    Laurent<Integer> sum3;
    size_t k;
    for (k = 0; k < 2 * n; ++k)
        if (rot[k])
            sum3 += (adj.entry(k, k) * 2 - det) * rot[k];
    if (rot[0])
        sum3 += (adj.entry(k, k) * 2 - det) * rot[0]; // here k == 2 * n

    // The following line computes 2 (T_2 - 1) θ :
    auto ans = std::move(sum1) + std::move(sum2) * 2 +
        L(std::move(sum3), 1, 1) * delta[0] * delta[1];

    // TODO: Strip out the factor of 2 (T_2 - 1)
    return *(theta_ = std::move(ans));
}

} // namespace regina

