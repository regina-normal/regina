
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

#include "link/link.h"
#include "packet/container.h"
#include "triangulation/dim3.h"

namespace regina {

Triangulation<3> Link::longComplement(StrandRef breakOpen, bool simplify)
        const {
    if (isEmpty())
        throw FailedPrecondition("longComplement() requires a non-empty knot");
    if (countComponents() > 1)
        throw FailedPrecondition("longComplement() only works with knots, "
            "not multiple component links");
    if (! isClassical())
        throw FailedPrecondition("longComplement() only works with classical "
            "knots, not virtual knots");

    if (size() == 0) {
        // We have a zero-crossing unknot.  We need at least one crossing.
        return Link::fromData({ 1 }, { 1, -1 }).longComplement({}, simplify);
    }

    if (! breakOpen) {
        // Choose an arbitrary crossing at which to break the knot open.
        breakOpen = crossings_.front()->upper();
    }

    Triangulation<3> ans = internalComplement(breakOpen);
    if (simplify)
        ans.simplify();
    return ans;
}

Triangulation<3> Link::internalComplement(StrandRef breakOpen) const {
    // This implementation produces an oriented triangluation.
    // The orientation follows a right-hand rule, where the thumb points
    // from vertices 0 to 1, and the fingers point from vertice 2 to 3.
    //
    // Our algorithm follows Jeff Weeks' method, which is described in
    // marvellous detail in the comments of link_complement.c from the
    // SnapPea kernel (which you can find in Regina's source tree as
    // engine/snappea/kernel/link_complement.c).
    //
    // We do however make some changes:
    //
    // - We ignore zero-crossing components completely; instead we
    //   connect sum with the appropriate number of unknot complements before
    //   we return the final triangulation.
    //
    // - Jeff's documentation insists that the underlying 4-valent graph is
    //   connected, and he performs spurious R2 moves where it is necessary
    //   to ensure this.  We ignore the issue here; the side-effect is that
    //   our triangulation might be disconnected, and we fix this before
    //   returning by joining the pieces together.
    //
    // As for breakOpen: if this is non-null then we are guaranteed that we
    // have a classical knot diagram with exactly one component and at least
    // one crossing.  This means that none of the special cases above apply,
    // and so we can produce the mixed real/ideal boundary long knot
    // complement by just failing to glue together the triangles that meet the
    // given arc.

    Triangulation<3> ans;

    // Empty link?  Just return the 3-sphere.
    // Note: breakOpen must be null in this case.
    if (isEmpty()) {
        Tetrahedron<3>* t = ans.newSimplex();
        t->join(0, t, {0,1});
        t->join(2, t, {2,3});
        return ans;
    }

    size_t n = size();
    FixedArray<std::array<Tetrahedron<3>*, 4>> ctet(n);

    /**
     *
     * Tetrahedra, for -ve crossing:
     *   ctet[i][0]: upper forward -> lower forward
     *   ctet[i][1]: lower forward -> upper backward
     *   ctet[i][2]: upper backward -> lower backward
     *   ctet[i][3]: lower backward -> upper forward
     *
     * Tetrahedra, for +ve crossing:
     *   replace upper <-> lower in the list above
     *
     * Tetrahedron vertices:
     *   0 = north pole
     *   1 = south pole
     *   2->3 represents the arrow in the tetrahedron list above
     */

    // Create the local structure around each crossing:
    for (size_t i = 0; i < n; ++i) {
        ctet[i] = ans.newSimplices<4>();
        if (crossing(i)->sign() > 0) {
            ctet[i][0]->join(0, ctet[i][1], {2,3});
            ctet[i][1]->join(1, ctet[i][2], {2,3});
            ctet[i][2]->join(0, ctet[i][3], {2,3});
            ctet[i][3]->join(1, ctet[i][0], {2,3});
        } else {
            ctet[i][0]->join(1, ctet[i][1], {2,3});
            ctet[i][1]->join(0, ctet[i][2], {2,3});
            ctet[i][2]->join(1, ctet[i][3], {2,3});
            ctet[i][3]->join(0, ctet[i][0], {2,3});
        }
    }

    // Connect the structures for adjacent crossings:
    StrandRef s, t;
    for (size_t i = 0; i < n; ++i) {
        const Crossing* cr = crossing(i);

        // We will connect the structure for this crossing to the structures
        // for the adjacent crossings that we reach by moving away from this
        // crossing in the forwards direction.
        //
        // Make s the next strand after this crossing when stepping forwards
        // on the left, and make t the next strand after this crossing when
        // stepping forwards on the right.
        //
        // If breakOpen is non-null, then when stepping forward along that
        // particular arc, we will set the corresponding strand (s or t) to
        // null.  This is our indication that the corresponding connection
        // should _not_ be made.

        if (cr->sign() > 0) {
            s = cr->next(0);
            t = cr->next(1);

            if (cr == breakOpen.crossing()) {
                if (breakOpen.strand() == 0)
                    s = StrandRef();
                else
                    t = StrandRef();
            }
        } else {
            s = cr->next(1);
            t = cr->next(0);

            if (cr == breakOpen.crossing()) {
                if (breakOpen.strand() == 0)
                    t = StrandRef();
                else
                    s = StrandRef();
            }
        }

        const Crossing* adj = s.crossing();
        if (adj) {
            if ((adj->sign() > 0 && s.strand() == 1) ||
                    (adj->sign() < 0 && s.strand() == 0)) {
                ctet[i][3]->join(2, ctet[adj->index()][3], {2,3});
                ctet[i][0]->join(3, ctet[adj->index()][2], {2,3});
            } else {
                ctet[i][3]->join(2, ctet[adj->index()][2], {2,3});
                ctet[i][0]->join(3, ctet[adj->index()][1], {2,3});
            }
        }

        adj = t.crossing();
        if (adj) {
            if ((adj->sign() > 0 && t.strand() == 1) ||
                    (adj->sign() < 0 && t.strand() == 0)) {
                ctet[i][0]->join(2, ctet[adj->index()][3], {2,3});
                ctet[i][1]->join(3, ctet[adj->index()][2], {2,3});
            } else {
                ctet[i][0]->join(2, ctet[adj->index()][2], {2,3});
                ctet[i][1]->join(3, ctet[adj->index()][1], {2,3});
            }
        }
    }

    // Identify any link components that consist entirely of over-crossings, or
    // entirely of under-crossings.  (We ignore zero-crossing components here.)
    // Note: such components can only exist if we have a link with two or more
    // components (which also means that breakOpen must be null).
    if (components_.size() > 1) {
        for (StrandRef c : components_) {
            if (! c)
                continue;

            bool missing[2] = { true, true }; // [ missing under, missing over ]
            StrandRef s = c;
            do {
                if (missing[s.strand()]) {
                    missing[s.strand()] = false;
                    if (! missing[s.strand() ^ 1])
                        break;
                }
                ++s;
            } while (s != c);

            if (missing[0] || missing[1]) {
                // This component contains entirely over-crossings or entirely
                // under-crossings.  As described in Jeff's documentation, we
                // need to add an R1 twist to avoid unintentionally breaking the
                // topology by collapsing a cycle of bigons in the complement.
                // Here we do this by splicing in the four tetrahedra that would
                // come from such a twist.
                Tetrahedron<3> *left, *right;
                if ((c.crossing()->sign() > 0 && c.strand() == 0) ||
                        (c.crossing()->sign() < 0 && c.strand() == 1)) {
                    left = ctet[c.crossing()->index()][3];
                    right = ctet[c.crossing()->index()][0];
                } else {
                    left = ctet[c.crossing()->index()][0];
                    right = ctet[c.crossing()->index()][1];
                }

                Tetrahedron<3>* adjLeft = left->adjacentSimplex(2);
                Tetrahedron<3>* adjRight = right->adjacentSimplex(3);
                // We already know both gluing permutations must be 2 <-> 3.

                auto [t0, t1, t2, t3] = ans.newTetrahedra<4>();

                t0->join(0, t1, {2,3});
                t0->join(1, t3, {2,3});
                t0->join(3, t2, {2,3});
                t1->join(1, t2, {2,3});
                t2->join(0, t3, {2,3});
                t3->join(2, t3, {2,3});

                left->unjoin(2);
                right->unjoin(3);
                left->join(2, t2, {2,3});
                right->join(3, t1, {2,3});
                adjLeft->join(3, t0, {2,3});
                adjRight->join(2, t1, {2,3});
            }
        }
    }

    // Account for any zero-crossing unknot components.
    // Note: if breakOpen is non-null, it is guaranteed that there will be
    // no such components.
    for (size_t i = 0; i < countTrivialComponents(); ++i) {
        // Insert a separate unknot complement.
        //
        // We use the same tetrahedron vertex numbering as before:
        // vertices 0,1 are the north/south poles, and vertices 2,3 are on
        // the ideal boundary (i.e., they represent the unknot itself).
        //
        // The following gluings were obtained by running the above complement
        // code on the unknot diagram with one positive crossing: + ( ^0 _0 ).

        auto [t0, t1, t2, t3] = ans.newTetrahedra<4>();
        t0->join(0, t1, {2,3}); t0->join(1, t3, {2,3});
        t0->join(2, t2, {2,3}); t0->join(3, t2, {2,3});
        t1->join(1, t2, {2,3}); t1->join(2, t1, {2,3});
        t2->join(0, t3, {2,3}); t3->join(2, t3, {2,3});
    }

    // At this point we have one triangulation component for every connected
    // diagram component (including any zero-crossing unknot components).
    // As a side effect, this means our triangulation is not empty (since we
    // dealt with the empty link earlier).

    while (ans.countComponents() > 1) {
        // Join two of our components together.  (We will keep doing this
        // until the entire triangulation is connected.)
        //
        // When joining two components, we do this in such a way that the two
        // north poles become identified, and likewise for the two south poles.
        // If we imagine truncating the north and south poles (as we would for
        // a virtual link diagram), then this operation essentially drills out
        // a tube from each component connecting the two poles and then joins
        // the two resulting annulus boundaries together.  If either component
        // is classical then the north and south poles of that component will
        // become finite vertices, and this entire operation reduces to just a
        // connected sum.
        //
        // Remember: in all of the tetrahedra we have inserted, vertex 0 is
        // the north pole, vertex 1 is the south pole, and vertices 2,3
        // represent the link itself.
        //
        // Our strategy will be to pry open triangle 012 on tetrahedron 0 of
        // each component.  Note that these triangles are always embedded,
        // since their three vertices are distinct.

        // In tet[] and adj[], the array index is the component number.
        Tetrahedron<3>* tet[2];
        Tetrahedron<3>* adj[2];

        for (int i = 0; i < 2; ++i) {
            tet[i] = ans.component(i)->tetrahedron(0);
            adj[i] = tet[i]->adjacentSimplex(3);
            // Again, we already know that the gluing permutation is 2 <-> 3.
        }

        tet[0]->unjoin(3);
        tet[1]->unjoin(3);

        // We need to join the components in a way that identifies the poles
        // (vertices 0,1) in each component but keeps separate the links
        // (vertices 2,3) in each component.

        auto [t0, t1] = ans.newTetrahedra<2>();

        t0->join(0, t1, {2,3});
        t0->join(1, t1, {2,3});
        tet[0]->join(3, t0, {2,3});
        adj[0]->join(2, t1, {2,3});
        tet[1]->join(3, t1, {2,3});
        adj[1]->join(2, t0, {2,3});
    }

    // Done!
    return ans;
}

} // namespace regina

