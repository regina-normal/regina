
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

/*! \file triangulation/detail/link-impl.h
 *  \brief Contains some of the implementation details for triangulating links
 *  of faces.
 *
 *  This file is _not_ included from face.h, and it is not shipped with Regina's
 *  development headers.  The routines it contains are explicitly instantiated
 *  in Regina's calculation engine for all relevant dimensions.
 *
 *  The reason for "quarantining" this file is that the routines it defines
 *  require definitions of lower-dimensional triangulation classes.
 *  By keeping their implementations safely out of the main headers, we avoid
 *  having Triangulation<dim> recursively instantiate _all_ triangulation
 *  classes Triangulation<dim-1>, Triangulation<dim-2>, ..., Triangulation<2>.
 */

#ifndef __REGINA_LINK_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_LINK_IMPL_H_DETAIL
#endif

#include "triangulation/triangulation.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

template <int dim, int subdim>
requires (supportedDim(dim) && subdim >= 0 && subdim < dim)
void Face<dim, subdim>::destroyLink()
        requires (standardDim(dim) && dim - subdim >= 3) {
    // Deleting null is always safe.
    delete link_.value;
}

template <int dim, int subdim>
requires (supportedDim(dim) && subdim >= 0 && subdim < dim)
const SafeTriangulation<dim - subdim - 1>& Face<dim, subdim>::buildLink() const
        requires (standardDim(dim) && dim - subdim >= 3) {
    // This code relies on a subdim-face having the same face number as the
    // opposite (dim - subdim - 1)-face within a simplex.  This is not true in
    // the special case where both faces have the same dimension (e.g.,
    // triangle links in a 5-manifold triangulation); however, in standard
    // dimensions such a special case never occurs (since subdim ≥ 2).
    // If we ever enable this code in higher dimensions we will need to
    // adjust things accordingly.
    static constexpr int linkDim = dim - subdim - 1;
    static_assert(subdim != linkDim);

    if (! link_.value) {
        auto* ans = new Triangulation<linkDim>();
        ans->newSimplices(degree());

        // Build a map from (simplex, face) pairs to indices into
        // embeddings().  This avoids an expensive search each time we
        // cross from one simplex to an adjacent simplex.
        static constexpr auto nFaces = Face<dim, subdim>::nFaces;
        FixedArray<size_t> map(nFaces * triangulation().size());
        {
            size_t i = 0;
            for (const auto& emb : embeddings())
                map[nFaces * emb.simplex()->index() + emb.face()] = i++;
        }

        for (const auto& emb : embeddings()) {
            // Glue this piece of link to any adjacent pieces of link.
            size_t simpIdx = emb.simplex()->index();
            for (int exitFacet = 0; exitFacet <= dim; ++exitFacet) {
                if (Face<dim, subdim>::containsVertex(emb.face(), exitFacet))
                    continue;

                auto adjSimp = emb.simplex()->adjacentSimplex(exitFacet);
                if (! adjSimp)
                    continue;
                size_t adjSimpIdx = adjSimp->index();

                // Make sure we perform each gluing in one direction only.
                if (adjSimpIdx > simpIdx)
                    continue;
                if (adjSimpIdx == simpIdx &&
                        emb.simplex()->adjacentFacet(exitFacet) > exitFacet)
                    continue;

                // Go ahead and make the gluing.
                int adjFace = Face<dim, subdim>::faceNumber(
                    emb.simplex()->adjacentGluing(exitFacet) *
                    emb.vertices());
                Perm<dim + 1> mapping = emb.simplex()->
                    template faceMapping<linkDim>(emb.face());
                Perm<dim + 1> adjMapping = adjSimp->
                    template faceMapping<linkDim>(adjFace);
                ans->simplex(map[nFaces * simpIdx + emb.face()])->join(
                    mapping.pre(exitFacet),
                    ans->simplex(map[nFaces * adjSimpIdx + adjFace]),
                    Perm<linkDim + 1>::contract(
                        adjMapping.inverse() *
                        emb.simplex()->adjacentGluing(exitFacet) *
                        mapping));
            }
        }

        // This is a construct-on-demand member: cast away constness to
        // set it here.
        const_cast<Face<dim, subdim>*>(this)->link_.value = ans;
    }
    return *link_.value;
}

template <int dim, int subdim>
requires (supportedDim(dim) && subdim >= 0 && subdim < dim)
Isomorphism<dim> Face<dim, subdim>::buildLinkInclusion() const
        requires (standardDim(dim) && dim - subdim >= 3) {
    // This code relies on a subdim-face having the same face number as the
    // opposite (dim - subdim - 1)-face within a simplex.  See the comments in
    // buildLink() for further discussion on this.
    static constexpr int linkDim = dim - subdim - 1;
    static_assert(subdim != linkDim);

    Isomorphism<dim> inclusion(degree());

    size_t i = 0;
    for (auto it = begin(); it != end(); ++it, ++i) {
        inclusion.simpImage(i) = it->simplex()->index();
        if constexpr (subdim == 0) {
            inclusion.facetPerm(i) =
                it->simplex()->template faceMapping<linkDim>(it->vertex());
        } else {
            static_assert(subdim == 1); // since dim <= 4
            Perm<dim + 1> perm =
                it->simplex()->template faceMapping<linkDim>(it->face());
            if (perm[dim - 1] == it->vertices()[0])
                inclusion.facetPerm(i) = perm;
            else
                inclusion.facetPerm(i) = perm * Perm<dim + 1>(dim - 1, dim);
        }
    }

    return inclusion;
}

} // namespace regina

#endif

