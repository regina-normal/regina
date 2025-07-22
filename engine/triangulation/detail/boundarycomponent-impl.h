
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

/*! \file triangulation/detail/boundarycomponent-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  BoundaryComponent class template.
 *
 *  This file is _not_ included from boundarycomponent.h, and it is not
 *  shipped with Regina's development headers.  The routines it contains are
 *  explicitly instantiated in Regina's calculation engine for all dimensions.
 *
 *  The reason for "quarantining" this file is that the routines it defines
 *  require a definition of the lower-dimensional class Triangulation<dim-1>.
 *  By keeping their implementations safely out of the main headers, we avoid
 *  having Triangulation<dim> recursively instantiate _all_ triangulation
 *  classes Triangulation<dim-1>, Triangulation<dim-2>, ..., Triangulation<2>.
 *  This quarantining also helps us to keep the helper class ReorderIterator
 *  out of the main API.
 */

#ifndef __REGINA_BOUNDARYCOMPONENT_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_BOUNDARYCOMPONENT_IMPL_H_DETAIL
#endif

#include "triangulation/generic/boundarycomponent.h"

namespace regina::detail {

namespace {
    /**
     * A helper iterator class for
     * BoundaryComponentBase::reorderAndRelabelFaces().
     *
     * An input iterator that runs through the <i>subdim</i>-faces of
     * a boundary component in order and (when dereferenced) converts
     * them to the corresponding faces from some other triangulation \a tri.
     *
     * The iterator relies on an array \a map, where for each face \a f
     * of the boundary component, `map[f->index()]` is the
     * corresponding face of \a tri.  Note that `f->index()` is the
     * index of \a f in the underlying <i>dim</i>-dimensional triangulation,
     * _not_ the index of \a f in the boundary component's facet list.
     */
    template <int dim, int subdim>
    class ReorderIterator {
        private:
            using InternalIterator =
                typename std::vector<Face<dim, subdim>*>::const_iterator;
            InternalIterator it_;
            Face<dim - 1, subdim>** map_;

        public:
            ReorderIterator() : it_(), map_(nullptr) {
            }
            ReorderIterator(InternalIterator it,
                    Face<dim - 1, subdim>** map) : it_(it), map_(map) {
            }
            ReorderIterator(const ReorderIterator&) = default;
            ReorderIterator& operator = (const ReorderIterator&) = default;

            bool operator == (const ReorderIterator& rhs) const {
                return it_ == rhs.it_;
            }
            ReorderIterator& operator ++() {
                ++it_;
                return *this;
            }
            ReorderIterator operator ++(int) {
                ReorderIterator prev(*this);
                ++it_;
                return prev;
            }
            Face<dim - 1, subdim>* operator * () const {
                return map_[(*it_)->index()];
            }
    };
}

template <int dim>
BoundaryComponentBase<dim>::~BoundaryComponentBase() {
    if constexpr (canBuild)
        delete boundary_.value;
}

template <int dim>
Triangulation<dim-1>* BoundaryComponentBase<dim>::buildRealBoundary() const {
    // From the precondition, there is a positive number of (dim-1)-faces.
    const auto& allFacets = std::get<tupleIndex(dim-1)>(faces_);

    // Build a map from ((dim-1)-face index in underlying triangulation)
    // to ((dim-1)-face in boundary component).
    //
    // The way we build it ensures that (dim-1)-faces are added to the
    // new boundary triangulation in the same order as they appear in
    // the boundary component's list of (dim-1)-faces.
    Triangulation<dim>& mainTri = allFacets.front()->triangulation();
    auto* bdrySimplex = new Simplex<dim-1>*[
        mainTri.template countFaces<dim-1>()];

    // NOTE: If we ever change this to return by value (and so ans is
    // not a pointer), then we need to change the lambda at the end of
    // this function to capture by reference, not by value.
    auto* ans = new Triangulation<dim-1>();

    for (auto s : allFacets)
        bdrySimplex[s->index()] = ans->newSimplex();

    // Run through the (dim-1)-simplices and make all the face gluings.
    int facetOfSimp;
    Simplex<dim-1>* simpBdry;
    Face<dim, dim-1>* adjOuter;
    Face<dim, dim-2>* ridgeOuter;
    for (Face<dim, dim-1>* simpOuter : allFacets) {
        simpBdry = bdrySimplex[simpOuter->index()];
        for (facetOfSimp = 0; facetOfSimp < dim; ++facetOfSimp)
            if (! simpBdry->adjacentSimplex(facetOfSimp)) {
                // Find out who is glued to this facet.
                ridgeOuter = simpOuter->template face<dim-2>(facetOfSimp);

                // Remember that the link of ridgeOuter has simpOuter at
                // one end of the list, and the adjacent simplex at the other.
                const auto& embFront = ridgeOuter->front();
                const auto& embBack = ridgeOuter->back();
                if (embFront.simplex()->template face<dim-1>(
                            embFront.vertices()[dim]) == simpOuter &&
                        embFront.vertices()[dim-1] ==
                            simpOuter->front().vertices()[facetOfSimp]) {
                    adjOuter = embBack.simplex()->template face<dim-1>(
                        embBack.vertices()[dim-1]);
                    simpBdry->join(facetOfSimp, bdrySimplex[adjOuter->index()],
                        Perm<dim>::contract(
                            adjOuter->front().vertices().inverse() *
                            embBack.vertices() *
                            Perm<dim+1>(dim-1, dim) *
                            embFront.vertices().inverse() *
                            simpOuter->front().vertices()));
                } else {
                    adjOuter = embFront.simplex()->template face<dim-1>(
                        embFront.vertices()[dim]);
                    simpBdry->join(facetOfSimp, bdrySimplex[adjOuter->index()],
                        Perm<dim>::contract(
                            adjOuter->front().vertices().inverse() *
                            embFront.vertices() *
                            Perm<dim+1>(dim-1, dim) *
                            embBack.vertices().inverse() *
                            simpOuter->front().vertices()));
                }
            }
    }

    delete[] bdrySimplex;

    if constexpr (allFaces) {
        /**
         * We are storing all faces of boundary components, not just the
         * (dim-1)-dimensional facets.
         *
         * Now the triangulation is built, we need to reorder these lower-
         * dimensional (≤ dim-2)-faces to appear in the same order and with the
         * same vertex numbers as they do in the boundary component face lists.
         *
         * A problem: this relabelling does happen immediately after ans is
         * constructed, but not until *after* the skeletal calculations for ans.
         * Therefore we have problems if the skeletal calculations for ans
         * create additional structures that depend on this ordering/numbering.
         *
         * Currently the only such structures that we have to worry about are
         * the triangulated edge/vertex links in Triangulation<4>.
         * This means we only have problems in the case dim=5.
         * However: for dim=5, boundary components don't store lower-dimensional
         * faces, and so this ordering/numbering does not take place at all.
         *
         * TODO: Put in some kind of robust mechanism so that this issue
         * does not come back and bite us at a later date if/when the skeletal
         * computations are extended to do more than they do now.
         */
        static_assert(dim != 5,
            "There is a problem with relabelling/reordering faces in "
            "buildRealBoundary(); see the code comments for details.");
        ans->countComponents(); // ensures that the skeleton is calculated

        std::apply([this, ans](auto&&... kFaces){
            (reorderAndRelabelFaces(ans, kFaces), ...);
        }, faces_);
    }

    return ans;
}

template <int dim>
template <int subdim>
void BoundaryComponentBase<dim>::reorderAndRelabelFaces(
        Triangulation<dim - 1>* tri,
        const std::vector<Face<dim, subdim>*>& reference) const {
    if constexpr (subdim == dim - 1) {
        // The (dim-1) faces are already in perfect correspondence.
        return;
    } else {
        if (reference.empty())
            return; // Should never happen.

        // Check for pinched faces: if these are present then
        // the situation is hopeless, since such faces are
        // effectively duplicated when we triangulate the boundary,
        // and so the numbers of faces do not match.
        if (reference.size() != tri->template countFaces<subdim>())
            return;

        // Build a map from (subdim-face indices in the d-dim triangulation
        // that owns this boundary component) to (subdim-faces in tri).
        //
        // This is a partial function: it is only defined for indices
        // of *boundary* subdim-faces in this d-dim triang.
        // We leave the other values of the map uninitialised.
        auto* map = new Face<dim - 1, subdim>*[
            reference.front()->triangulation().template countFaces<subdim>()];

        for (Face<dim - 1, subdim>* f : tri->template faces<subdim>()) {
            const auto& emb = f->front();
            Face<dim, dim - 1>* outer = facet(emb.simplex()->index());
            map[outer->template face<subdim>(emb.face())->index()] = f;

            // While we have the two corresponding faces in front of us,
            // relabel the vertices of f now.
            //
            // The following two permutations should be made equal:
            //
            // 1. emb.simplex()->faceMapping<subdim>(emb.face())
            // 2. outer->faceMapping<subdim>(emb.face())
            //
            // Note that (1) is just emb.vertices().
            //
            Perm<dim> adjust = emb.vertices().inverse() *
                Perm<dim>::contract(
                    outer->template faceMapping<subdim>(emb.face()));
            adjust.clear(subdim + 1);
            tri->relabelFace(f, adjust);
        }

        tri->template reorderFaces<subdim>(
            ReorderIterator<dim, subdim>(reference.begin(), map),
            ReorderIterator<dim, subdim>(reference.end(), map));

        delete[] map;
    }
}

} // namespace regina::detail

#endif

