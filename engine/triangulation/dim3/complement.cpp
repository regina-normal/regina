
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
#include "packet/container.h"
#include "triangulation/dim3.h"

namespace regina {

Triangulation<3>::Triangulation(const Link& link, bool simplify) {
    // This implementation produces an oriented triangluation.
    // The orientation follows a right-hand rule, where the thumb points
    // from vertices 0 to 1, and the fingers point from vertice 2 to 3.

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

    // Empty link?  Just return the 3-sphere.
    if (link.isEmpty()) {
        Tetrahedron<3>* t = newSimplexRaw();
        t->joinRaw(0, t, {0,1});
        t->joinRaw(2, t, {2,3});
        return;
    }

    size_t n = link.size();
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
        ctet[i] = newSimplicesRaw<4>();
        if (link.crossing(i)->sign() > 0) {
            ctet[i][0]->joinRaw(0, ctet[i][1], {2,3});
            ctet[i][1]->joinRaw(1, ctet[i][2], {2,3});
            ctet[i][2]->joinRaw(0, ctet[i][3], {2,3});
            ctet[i][3]->joinRaw(1, ctet[i][0], {2,3});
        } else {
            ctet[i][0]->joinRaw(1, ctet[i][1], {2,3});
            ctet[i][1]->joinRaw(0, ctet[i][2], {2,3});
            ctet[i][2]->joinRaw(1, ctet[i][3], {2,3});
            ctet[i][3]->joinRaw(0, ctet[i][0], {2,3});
        }
    }

    // Connect the structures for adjacent crossings:
    StrandRef s, t;
    for (size_t i = 0; i < n; ++i) {
        const Crossing* cr = link.crossing(i);
        // Make s the strand that follows forwards on the left, and
        // make t the strand that follows forwards on the right.
        if (cr->sign() > 0) {
            s = cr->next(0);
            t = cr->next(1);
        } else {
            s = cr->next(1);
            t = cr->next(0);
        }

        const Crossing* adj = s.crossing();
        if ((adj->sign() > 0 && s.strand() == 1) ||
                (adj->sign() < 0 && s.strand() == 0)) {
            ctet[i][3]->joinRaw(2, ctet[adj->index()][3], {2,3});
            ctet[i][0]->joinRaw(3, ctet[adj->index()][2], {2,3});
        } else {
            ctet[i][3]->joinRaw(2, ctet[adj->index()][2], {2,3});
            ctet[i][0]->joinRaw(3, ctet[adj->index()][1], {2,3});
        }

        adj = t.crossing();
        if ((adj->sign() > 0 && t.strand() == 1) ||
                (adj->sign() < 0 && t.strand() == 0)) {
            ctet[i][0]->joinRaw(2, ctet[adj->index()][3], {2,3});
            ctet[i][1]->joinRaw(3, ctet[adj->index()][2], {2,3});
        } else {
            ctet[i][0]->joinRaw(2, ctet[adj->index()][2], {2,3});
            ctet[i][1]->joinRaw(3, ctet[adj->index()][1], {2,3});
        }
    }

    // Identify any link components that consist entirely of over-crossings, or
    // entirely of under-crossings.  (We ignore zero-crossing components here.)
    for (StrandRef c : link.components()) {
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
            // This component consists entirely of over-crossings or entirely
            // of under-crossings.  As described in Jeff's documentation, we
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

            auto [t0, t1, t2, t3] = newTetrahedra<4>();

            t0->joinRaw(0, t1, {2,3});
            t0->joinRaw(1, t3, {2,3});
            t0->joinRaw(3, t2, {2,3});
            t1->joinRaw(1, t2, {2,3});
            t2->joinRaw(0, t3, {2,3});
            t3->joinRaw(2, t3, {2,3});

            left->unjoinRaw(2);
            right->unjoinRaw(3);
            left->joinRaw(2, t2, {2,3});
            right->joinRaw(3, t1, {2,3});
            adjLeft->joinRaw(3, t0, {2,3});
            adjRight->joinRaw(2, t1, {2,3});
        }
    }

    // Account for any zero-crossing unknot components.
    for (size_t i = 0; i < link.countTrivialComponents(); ++i) {
        // Insert a separate unknot complement.
        //
        // We use the same tetrahedron vertex numbering as before:
        // vertices 0,1 are the north/south poles, and vertices 2,3 are on
        // the ideal boundary (i.e., they represent the unknot itself).
        //
        // The following gluings were obtained by running the above complement
        // code on the unknot diagram with one positive crossing: + ( ^0 _0 ).

        auto [t0, t1, t2, t3] = newTetrahedra<4>();
        t0->joinRaw(0, t1, {2,3}); t0->joinRaw(1, t3, {2,3});
        t0->joinRaw(2, t2, {2,3}); t0->joinRaw(3, t2, {2,3});
        t1->joinRaw(1, t2, {2,3}); t1->joinRaw(2, t1, {2,3});
        t2->joinRaw(0, t3, {2,3}); t3->joinRaw(2, t3, {2,3});
    }

    // At this point we have one triangulation component for every connected
    // diagram component (including any zero-crossing unknot components).
    // As a side effect, this means our triangulation is not empty (since we
    // dealt with the empty link earlier).

    // The following call to countComponents() has the side-effect of computing
    // the full skeleton.  Therefore we will stop using joinRaw() / unjoinRaw(),
    // since we want join() and unjoin() to do their extra work of clearing
    // computed properties (amongst other things).

    while (countComponents() > 1) {
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
            tet[i] = component(i)->tetrahedron(0);
            adj[i] = tet[i]->adjacentSimplex(3);
            // Again, we already know that the gluing permutation is 2 <-> 3.
        }

        tet[0]->unjoin(3);
        tet[1]->unjoin(3);

        // We need to join the components in a way that identifies the poles
        // (vertices 0,1) in each component but keeps separate the links
        // (vertices 2,3) in each component.

        auto [t0, t1] = newTetrahedra<2>();

        t0->join(0, t1, {2,3});
        t0->join(1, t1, {2,3});
        tet[0]->join(3, t0, {2,3});
        adj[0]->join(2, t1, {2,3});
        tet[1]->join(3, t1, {2,3});
        adj[1]->join(2, t0, {2,3});
    }

    // Done!
    if (simplify)
        Triangulation<3>::simplify();
}

} // namespace regina

