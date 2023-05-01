
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

/*! \file triangulation/detail/pachner-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  Triangulation class template.
 *
 *  This file is _not_ included from triangulate.h, and it is not
 *  shipped with Regina's development headers.  The routines it contains are
 *  explicitly instantiated in Regina's calculation engine for all dimensions.
 *
 *  The reason for "quarantining" this file is so that the helper function and
 *  classes it defines are not inadvertently made accessible to end users.
 */

#ifndef __REGINA_PACHNER_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_PACHNER_IMPL_H_DETAIL
#endif

#include "triangulation/generic/triangulation.h"

namespace regina::detail {

#ifndef __DOXYGEN
    // The template function movePerm(), which is implemented and then
    // specialised below, is purely for use within the generic implementation
    // of pachner(), and does not form part of Regina's public API.
    //
    // It is not a private member of the PachnerHelper class, because we
    // wish to specialise it for standard dimensions without causing the
    // compiler to instantiate the entire Triangulation<dim> class.

    /**
     * Calculates how the vertices of an old simplex correspond to the vertices
     * of a new simplex in a (\a dim - \a k + 1)-(\a k + 1) Pachner move about
     * a <i>k</i>-face of a <i>dim</i>-dimensional triangulation.
     *
     * The old and new simplices will have precisely \a dim vertices in
     * common, which together identify an external facet of the
     * topological ball that is replace by this Pachner move.
     *
     * This mapping of vertices is relative to Regina's own "canonical"
     * labelling of the old and new simplices that make up this
     * topological ball.  For the new simplices, this will be the actual
     * labelling of the simplices (since Regina gets to create the
     * new simplices).  For the old simplices, however, this will
     * _not_ be the actual labelling of the simplices (since this is
     * provided by the user, and is out of Regina's control).
     *
     * For the details of Regina's "canonical" labelling of simplices,
     * and for a detailed description of what this mapping looks like,
     * see the implementation of PachnerHelper<dim, k>::pachner().
     *
     * \tparam dim the dimension of the underlying triangulation.
     * \tparam k the dimension of the face about which we are performing a
     * Pachner move.
     *
     * \param oldSimp identifies one of the old simplices that will
     * be removed by this Pachner move; this must be between
     * 0 and (\a dim - \a k) inclusive.
     * \param newSimp identifies one of the new simplices that will
     * be added by this Pachner move; this must be between
     * 0 and \a k inclusive.
     */
    template <int dim, int k>
    Perm<dim + 1> movePerm(int oldSimp, int newSimp) {
        static_assert(0 < k && k < dim,
            "movePerm() may not be called for 0-faces or dim-faces.");
        static_assert(! standardDim(dim),
            "The generic movePerm() implementation cannot be used in "
            "standard dimensions.");

        std::array<int, dim + 1> image;
        int i;
        int oldFacet, newFacet;

        for (i = 0; i < k; ++i)
            if (newSimp != i)
                image[i] = dim - k + i;
            else
                oldFacet = i;
        if (newSimp != k)
            image[k + oldSimp] = dim;
        else
            oldFacet = k + oldSimp;

        for (i = 0; i < dim-k; ++i)
            if (oldSimp != i)
                image[k + i] = i;
            else
                newFacet = i;
        if (oldSimp != dim-k)
            image[dim] = dim - k + newSimp;
        else
            newFacet = dim - k + newSimp;

        image[oldFacet] = newFacet;

        return Perm<dim + 1>(image);
    }

    // Precalculated values of movePerm() for use in standard dimensions.
    // These are not part of Regina's public API.  The calculation engine
    // already links in implementations of all possible Pachner moves, and
    // so the end user should have no need to refer to these arrays.
    inline constexpr Perm<3>::Code movePerm_2_1[2][2] = {
        { 1 /* 021 */, 3 /* 102 */ }, { 3 /* 102 */, 3 /* 102 */ } };

    inline constexpr Perm<4>::Code2 movePerm_3_1[3][2] = {
        { 4  /* 0312 */, 12 /* 2013 */ },
        { 6  /* 1032 */, 12 /* 2013 */ },
        { 12 /* 2013 */, 12 /* 2013 */ } };

    inline constexpr Perm<4>::Code2 movePerm_3_2[2][3] = {
        { 2 /* 0231 */, 6 /* 1032 */, 8 /* 1203 */ },
        { 8 /* 1203 */, 8 /* 1203 */, 8 /* 1203 */ } };

    inline constexpr Perm<5>::Code2 movePerm_4_1[4][2] = {
        { 19 /* 04123 */, 73 /* 30124 */ },
        { 29 /* 10423 */, 73 /* 30124 */ },
        { 49 /* 20143 */, 73 /* 30124 */ },
        { 73 /* 30124 */, 73 /* 30124 */ } };

    inline constexpr Perm<5>::Code2 movePerm_4_2[3][3] = {
        { 16 /* 03412 */ , 52 /* 20413 */, 60 /* 23014 */ },
        { 36 /* 13042 */ , 54 /* 21043 */, 60 /* 23014 */ },
        { 60 /* 23014 */ , 60 /* 23014 */, 60 /* 23014 */ } };

    inline constexpr Perm<5>::Code2 movePerm_4_3[2][4] = {
        { 9  /* 02341 */, 27 /* 10342 */, 31 /* 12043 */, 33 /* 12304 */ },
        { 33 /* 12304 */, 33 /* 12304 */, 33 /* 12304 */, 33 /* 12304 */ } };

    // Specialised implementations of movePerm() that use these
    // precalculated values.
    template <>
    inline Perm<3> movePerm<2, 1>(int oldSimp, int newSimp) {
        return Perm<3>::fromPermCode(movePerm_2_1[oldSimp][newSimp]);
    }

    template <>
    inline Perm<4> movePerm<3, 1>(int oldSimp, int newSimp) {
        return Perm<4>::fromPermCode2(movePerm_3_1[oldSimp][newSimp]);
    }

    template <>
    inline Perm<4> movePerm<3, 2>(int oldSimp, int newSimp) {
        return Perm<4>::fromPermCode2(movePerm_3_2[oldSimp][newSimp]);
    }

    template <>
    inline Perm<5> movePerm<4, 1>(int oldSimp, int newSimp) {
        return Perm<5>::fromPermCode2(movePerm_4_1[oldSimp][newSimp]);
    }

    template <>
    inline Perm<5> movePerm<4, 2>(int oldSimp, int newSimp) {
        return Perm<5>::fromPermCode2(movePerm_4_2[oldSimp][newSimp]);
    }

    template <>
    inline Perm<5> movePerm<4, 3>(int oldSimp, int newSimp) {
        return Perm<5>::fromPermCode2(movePerm_4_3[oldSimp][newSimp]);
    }
#endif // __DOXYGEN

template <int dim>
template <int k>
bool TriangulationBase<dim>::pachner(Face<dim, k>* f, bool check,
        bool perform) {
    static_assert(0 <= k && k <= dim,
        "pachner() requires a facial dimension between 0 and dim inclusive.");

    using LockMask = typename Simplex<dim>::LockMask;

    if constexpr (k == 0) {
        // Pachner move on a vertex:
        if (check) {
            // Both invalid and ideal vertices are considered boundary.
            if (f->isBoundary())
                return false;
            if (f->degree() != dim + 1)
                return false;
        }

        // Prepare to record where there are any locks on exterior facets
        // that must be preserved.
        LockMask oldLocks = 0;

        // f must meet (dim+1) distinct top-dimensional simplices, which must be
        // glued around the vertex in a way that gives a dim-simplex link.
        // Find these simplices.
        Simplex<dim>* oldSimp[dim + 1];
        Perm<dim + 1> oldVertices[dim + 1];

        // Conceptually, we label the vertices of these simplices so:
        // - oldSimp[i] has vertex i = f (the internal vertex)
        // - oldSimp[i] <-> oldSimp[j] with permutation i <-> j
        //
        // This is possible iff we have a dim-simplex link.
        //
        // Moreover, this induces a labelling of the vertices of the new
        // simplex that will replace the old ones.
        //
        // The permutation oldVertices[i] maps these "conceptual" vertex
        // labels to the actual vertex labels seen in oldSimp[i].

        oldSimp[0] = f->front().simplex();
        oldVertices[0] = f->front().vertices(); // maps 0 -> f
        if (oldSimp[0]->locks_) {
            if (oldSimp[0]->locks_ != (LockMask(1) << oldVertices[0][0])) {
                if (check)
                    return false;
                if (perform)
                    throw LockViolation("An attempt was made to perform a "
                        "Pachner move using a locked simplex and/or facet");
            }
            oldLocks |= LockMask(1); // this will be facet 0 of the new simplex
        }

        if (oldVertices[0].sign() < 0) {
            // We need to preserve orientation.
            oldVertices[0] = oldVertices[0] * Perm<dim + 1>(dim - 1, dim);
        }

        for (int i = 1; i <= dim; ++i) {
            oldSimp[i] = oldSimp[0]->adjacentSimplex(oldVertices[0][i]);
            if (check)
                for (int j = 0; j < i; ++j)
                    if (oldSimp[i] == oldSimp[j])
                        return false;
            oldVertices[i] = oldSimp[0]->adjacentGluing(oldVertices[0][i]) *
                oldVertices[0] * Perm<dim + 1>(0, i);
            if (oldSimp[i]->locks_) {
                if (oldSimp[i]->locks_ != (LockMask(1) << oldVertices[i][i])) {
                    if (check)
                        return false;
                    if (perform)
                        throw LockViolation("An attempt was made to perform a "
                            "Pachner move using a locked simplex and/or facet");
                }
                oldLocks |= (LockMask(1) << i);
            }
        }

        if (check) {
            for (int i = 1; i <= dim; ++i)
                for (int j = 1; j < i; ++j) {
                    if (oldSimp[i] != oldSimp[j]->adjacentSimplex(
                            oldVertices[j][i]))
                        return false;
                    if (oldVertices[i] !=
                            oldSimp[j]->adjacentGluing(oldVertices[j][i]) *
                            oldVertices[j] * Perm<dim + 1>(i, j))
                        return false;
                }
        }

        // The move is legal, and there are no locks that will get in the way.

        if (! perform)
            return true;

        // Perform the move.
        // The following takeSnapshot() and ChangeAndClearSpan are essential,
        // since we use "raw" routines (newSimplexRaw, joinRaw, etc.) below.
        TopologyLock lock(*this);
        Snapshottable<Triangulation<dim>>::takeSnapshot();
        typename Triangulation<dim>::ChangeAndClearSpan span(
            static_cast<Triangulation<dim>&>(*this));

        Simplex<dim>* newSimp = newSimplexRaw();

        // Find where their facets need to be glued.
        // Old simplex i, "conceptual" facet i <-> New simplex, facet i.
        Simplex<dim>* adjSimp[dim + 1];
        Perm<dim + 1> adjGlue[dim + 1];
        for (int i = 0; i <= dim; ++i) {
            if ((adjSimp[i] = oldSimp[i]->adjacentSimplex(oldVertices[i][i]))) {
                adjGlue[i] = oldSimp[i]->adjacentGluing(oldVertices[i][i]) *
                    oldVertices[i];

                // Are we are gluing the new simplex to itself?
                for (int j = 0; j <= dim; ++j) {
                    if (adjSimp[i] == oldSimp[j]) {
                        // This glues to old simplex j, facet oldVertices[j].
                        // Adjust this to point the new simplex instead, but
                        // also ensure the gluing happens in one direction only.
                        if (i < j) {
                            adjSimp[i] = newSimp;
                            adjGlue[i] = oldVertices[j].inverse() * adjGlue[i];
                        } else
                            adjSimp[i] = nullptr;
                        break;
                    }
                }
            }
        }

        // Delete the old simplices.
        for (int i = 0; i <= dim; ++i)
            removeSimplexRaw(oldSimp[i]);

        // Now go ahead and make the gluings.
        for (int i = 0; i <= dim; ++i)
            if (adjSimp[i])
                newSimp->joinRaw(i, adjSimp[i], adjGlue[i]);

        // Put back any facet locks from the inside.
        // They should already be in place from the outside.
        newSimp->locks_ = oldLocks;

        // All done!
        return true;
    } else if constexpr (k == dim) {
        // Pachner move on a top-dimensional simplex:

        // First check for lock violations.
        if (f->isLocked()) {
            if (check)
                return false;
            if (perform)
                throw LockViolation("An attempt was made to perform a "
                    "Pachner move on a locked top-dimensional simplex");
        }

        // Next check the legality of the move.
        // For a 1-(dim+1) move, this is always legal.
        if (! perform )
            return true;

        // Perform the move.
        // The following takeSnapshot() and ChangeAndClearSpan are essential,
        // since we use "raw" routines (newSimplexRaw, joinRaw, etc.) below.
        TopologyLock lock(*this);
        Snapshottable<Triangulation<dim>>::takeSnapshot();
        typename Triangulation<dim>::ChangeAndClearSpan span(
            static_cast<Triangulation<dim>&>(*this));

        // Remember any facet locks on f - we will need to restore these later.
        LockMask oldLock = f->locks_;

        // Create the new simplices.
        // Facet i of the old simplex will become a facet of newSimp[i].
        // Vertex i of newSimp[i] will become the new internal vertex, and the
        // other dim vertices of newSimp[i] will keep the same vertex numbers
        // that they had in the old simplex.
        //
        // We insert the new simplices in reverse order so that the new
        // vertex becomes vertex 0 of the last simplex of the triangulation.
        Simplex<dim>* newSimp[dim + 1];
        for (int i = dim; i >= 0; --i)
            newSimp[i] = newSimplexRaw();

        // Before we unglue, record how the adjacent simplices are glued to f.
        Simplex<dim>* adjSimp[dim + 1];
        Perm<dim + 1> adjGlue[dim + 1];
        for (int i = 0; i <= dim; i++) {
            if ((adjSimp[i] = f->adjacentSimplex(i))) {
                adjGlue[i] = f->adjacentGluing(i);

                // Were we gluing the old simplex to itself?
                if (adjSimp[i] == f) {
                    // Adjust this to point to one of the new simplices instead,
                    // but also ensure the gluing happens in one direction only.
                    int j = adjGlue[i][i];
                    if (i < j)
                        adjSimp[i] = newSimp[j];
                    else
                        adjSimp[i] = nullptr;
                }
            }
        }

        // Delete the old simplex.
        removeSimplexRaw(f);

        // Glue the new simplices to each other internally.
        for (int i = 0; i <= dim; ++i)
            for (int j = i + 1; j <= dim; ++j)
                newSimp[i]->joinRaw(j, newSimp[j], {i, j});

        // Attach the new simplices to the old triangulation.
        for (int i = 0; i <= dim; ++i)
            if (adjSimp[i])
                newSimp[i]->joinRaw(i, adjSimp[i], adjGlue[i]);

        // Put back any facet locks from the inside.
        // They should already be in place from the outside.
        if (oldLock)
            for (int i = 0; i <= dim; ++i) {
                LockMask lockBit = (LockMask(1) << i);
                if (oldLock & lockBit)
                    newSimp[i]->locks_ = lockBit;
            }

        // All done!
        return true;
    } else {
        // Pachner move on a face of dimension 1..(dim-1):
        if (check) {
            // The face must be valid and non-boundary.
            if (f->isBoundary() || ! f->isValid())
                return false;
            // f must have the right degree.
            if (f->degree() != dim + 1 - k)
                return false;
        }

        // f must meet (dim + 1 - k) distinct top-dimensional simplices, which
        // must be glued around the face in a way that makes the link of f
        // the standard simplex boundary.

        /**
         * Our numbering scheme is as follows.
         *
         * 1) Suppose we have a k-face f, meeting (dim+1-k) top-dimensional
         * simplices, and with the correct link as required for this move.
         * Then we will label these (dim+1-k) simplices so that:
         *
         * - For simplex i, face f is formed from vertices 0..(k-1) and (i+k);
         * - For i != j, simplex i facet (j+k) is glued to
         *   simplex j facet (i+k), with the permutation that swaps i+k <-> j+k.
         *
         * 2) Suppose we have a k-face f meeting (dim+1-k) top-dimensional
         * simplices as before, and we wish to replace this with a
         * (dim-k)-face g meeting (k+1) top-dimensional simplices.  Then:
         *
         * - Consider the external facet of the overall structure that is
         *   common to old simplex i and new simplex i'.  Then the vertices
         *   of i map to the vertices of i' as follows:
         *
         *   * The vertices of old face f map
         *     0..(k-1),(k+i) of simplex i -> (d-k)..d of simplex i',
         *     excluding vertex (d-k+i') of simplex i' and its preimage in i;
         *
         *   * The vertices of new face g map
         *     k..d of simplex i -> 0..(d-k-1),(d-k+i') of simplex i',
         *     excluding vertex (k+i) of simplex i and its image in i';
         *
         *   This maps d vertices in total (i.e., the entire facet).
         *   The missing vertex of f (which would have mapped to (d-k+i')) is
         *   the index of this facet in i, and the missing vertex of g (which
         *   would have mapped from (k+i)) is the index of this facet in i'.
         *
         * - Consider the external facet of the overall structure that is
         *   facet j of old simplex i (i ≤ d-k; j is one of 0..(k-1),(k+i)).
         *   Then this maps to facet j' of new simplex i', where:
         *
         *   * i' = { j if j < k ; k if j = k+i }
         *
         *   * j' = { i if i < d-k ; d-k+i' if i = d-k }
         *
         * - Note that this mapping is inverse to the corresponding mapping for
         *   the inverse Pachner move (i.e., a Pachner move on a (dim-k)-face).
         *
         * All of the new simplices will have a consistent orientation.
         * However, this will be *opposite* to the the orientation of the
         * original simplices when d is even and k is odd.  Otherwise the
         * original and new orientations will match.
         */

        // Prepare to record whether there are any locks on exterior facets
        // that must be preserved.  These are indexed with respect to the
        // *new* top-dimensional simplices and their facets.
        LockMask locks[k + 1];
        std::fill(locks, locks + k + 1, 0);

        // Find the original top-dimensional simplices.
        // We will let oldVertices[i] describe our numbering scheme for the
        // vertices of simplex i, as described in (1) above.  Specifically,
        // oldVertices[i] maps the "conceptual" labels described above for
        // old simplex i to the actual vertex labels.
        Simplex<dim>* oldSimp[dim + 1 - k];
        Perm<dim + 1> oldVertices[dim + 1 - k];

        oldSimp[0] = f->front().simplex();
        oldVertices[0] = f->front().vertices();

        bool fixOrientation;
        if ((dim % 2 == 0) && (k % 2 == 1)) {
            // The old and new simplices will switch orientation in our
            // numbering scheme.
            fixOrientation = (oldVertices[0].sign() > 0);
        } else {
            // The old and new simplices will have the same orientation.
            fixOrientation = (oldVertices[0].sign() < 0);
        }

        if (fixOrientation) {
            // We can fix the orientation now by permuting two elements
            // of oldVertices[0].  These must either both refer to the
            // vertices of the given k-face, or must both *not* refer to
            // vertices of the given k-face.
            if (k < dim - 1)
                oldVertices[0] = oldVertices[0] * Perm<dim + 1>(dim - 1, dim);
            else
                oldVertices[0] = oldVertices[0] * Perm<dim + 1>(0, 1);
        }

        if (oldSimp[0]->locks_) {
            if (oldSimp[0]->isLocked()) {
                if (check)
                    return false;
                if (perform)
                    throw LockViolation("An attempt was made to perform a "
                        "Pachner move using a locked top-dimensional simplex");
            }
            for (int v = 0; v <= k; ++v)
                if (oldSimp[0]->isFacetLocked(oldVertices[0][v])) {
                    // New simplex v, facet 0
                    locks[v] |= LockMask(1);
                }
            for (int v = k + 1; v <= dim; ++v)
                if (oldSimp[0]->isFacetLocked(oldVertices[0][v])) {
                    if (check)
                        return false;
                    if (perform)
                        throw LockViolation("An attempt was made to perform a "
                            "Pachner move using a locked facet");
                }
        }

        for (int i = 1; i <= dim - k; ++i) {
            oldSimp[i] = oldSimp[0]->adjacentSimplex(oldVertices[0][i + k]);
            if (check)
                for (int j = 0; j < i; ++j)
                    if (oldSimp[i] == oldSimp[j])
                        return false;
            oldVertices[i] = oldSimp[0]->adjacentGluing(oldVertices[0][i + k]) *
                oldVertices[0] * Perm<dim + 1>(k, i + k);
            if (oldSimp[i]->locks_) {
                if (oldSimp[i]->isLocked()) {
                    if (check)
                        return false;
                    if (perform)
                        throw LockViolation("An attempt was made to perform a "
                            "Pachner move using a locked top-dimensional "
                            "simplex");
                }
                for (int v = 0; v < k; ++v)
                    if (oldSimp[i]->isFacetLocked(oldVertices[i][v])) {
                        // New simplex v, facet (i<d-k ? i : d-k+v)
                        locks[v] |= (LockMask(1) <<
                            (i < dim - k ? i : dim - k + v));
                    }
                if (oldSimp[i]->isFacetLocked(oldVertices[i][k + i])) {
                    // New simplex k, facet (i<d-k ? i : d)
                    locks[k] |= (LockMask(1) << (i < dim - k ? i : dim));
                }
                for (int v = k; v <= dim; ++v)
                    if (v != k + i)
                        if (oldSimp[i]->isFacetLocked(oldVertices[i][v])) {
                            if (check)
                                return false;
                            if (perform)
                                throw LockViolation("An attempt was made to "
                                    "perform a Pachner move using a "
                                    "locked facet");
                        }
            }
        }

        if (check) {
            for (int i = 1; i <= dim - k; ++i)
                for (int j = 1; j < i; ++j) {
                    if (oldSimp[i] != oldSimp[j]->adjacentSimplex(
                            oldVertices[j][i + k]))
                        return false;
                    if (oldVertices[i] !=
                            oldSimp[j]->adjacentGluing(oldVertices[j][i + k]) *
                            oldVertices[j] * Perm<dim + 1>(i + k, j + k))
                        return false;
                }
        }

        // The move is legal, and there are no locks that will get in the way.

        if (! perform)
            return true;

        // Perform the move.
        // The following takeSnapshot() and ChangeAndClearSpan are essential,
        // since we use "raw" routines (newSimplexRaw, joinRaw, etc.) below.
        TopologyLock lock(*this);
        Snapshottable<Triangulation<dim>>::takeSnapshot();
        typename Triangulation<dim>::ChangeAndClearSpan span(
            static_cast<Triangulation<dim>&>(*this));

        // Create (k + 1) new top-dimensional simplices.
        // We insert them in reverse order to ensure that the new internal
        // (dim-k)-face is formed from vertices 0,...,(dim-k) of the last
        // simplex in the resulting triangulation.
        Simplex<dim>* newSimp[k + 1];
        for (int i = k; i >= 0; --i)
            newSimp[i] = newSimplexRaw();

        // Find where their facets need to be glued.
        // The following arrays adj*[i][j] store the destination of
        // new simplex i, facet j, with the exception that facet dim - k
        // should be interpreted to mean facet (i + dim - k) instead.
        Simplex<dim>* adjSimp[k + 1][dim + 1];
        Perm<dim + 1> adjGluing[k + 1][dim + 1];
        for (int i = 0; i <= k; ++i) { // new simplices
            for (int j = 0; j <= dim - k; ++j) { // new facets
                // This facet belongs to old simplex j.
                // Find the facet of this old simplex, in our numbering scheme.
                int oldFacet = (i < k ? i : k + j);
                adjSimp[i][j] = oldSimp[j]->adjacentSimplex(
                    oldVertices[j][oldFacet]);
                adjGluing[i][j] = oldSimp[j]->adjacentGluing(
                    oldVertices[j][oldFacet]) *
                    oldVertices[j] * movePerm<dim, dim - k>(i, j);

                // Is the destination of the gluing one of the old simplices
                // that we are about to remove?
                for (int l = 0; l <= dim - k; ++l) { // old simplex
                    if (adjSimp[i][j] == oldSimp[l]) {
                        // Yes!  The destination is old simplex l.
                        // We only need to fix the gluing from one side only.
                        if (j < l /* the case j == l comes later */) {
                            adjSimp[i][j] = nullptr;
                            break;
                        }
                        // Which facet of the old simplex is the destination
                        // of the gluing (using our own numbering scheme)?
                        int destFacet = oldVertices[l].pre(
                            adjGluing[i][j][j < dim - k ? j : i + dim - k]);

                        if (j == l && oldFacet < destFacet) {
                            // Again: make the gluing from one side only.
                            adjSimp[i][j] = nullptr;
                            break;
                        }

                        // Which new simplex does this translate to?
                        if (destFacet > k)
                            destFacet = k;

                        // Glued to the facet of old simplex l which is
                        // shared by new simplex destFacet.

                        // Adjust the gluing to point to the new simplex.
                        adjSimp[i][j] = newSimp[destFacet];
                        adjGluing[i][j] =
                            movePerm<dim, k>(l, destFacet) *
                            oldVertices[l].inverse() *
                            adjGluing[i][j];

                        break;
                    }
                }
            }
        }

        // Delete the old simplices.
        for (int i = 0; i <= dim - k; ++i)
            removeSimplexRaw(oldSimp[i]);

        // Now go ahead and make the gluings.
        for (int i = 0; i <= k; ++i) {
            int j = 0;
            for ( ; j < dim - k; ++j)
                if (adjSimp[i][j])
                    newSimp[i]->joinRaw(j, adjSimp[i][j], adjGluing[i][j]);
            // Now j == dim - k.
            if (adjSimp[i][j])
                newSimp[i]->joinRaw(i + dim - k, adjSimp[i][j],
                    adjGluing[i][j]);
        }

        // Make the internal gluings for the new simplices.
        for (int i = 1; i <= k; ++i)
            for (int j = 0; j < i; ++j)
                newSimp[i]->joinRaw(j + dim - k, newSimp[j],
                    Perm<dim + 1>(i + dim - k, j + dim - k));

        // Put back any facet locks from the inside.
        // They should already be in place from the outside.
        for (int i = 0; i <= k; ++i)
            newSimp[i]->locks_ = locks[i];

        // All done!
        return true;
    }
}

} // namespace regina::detail

#endif
