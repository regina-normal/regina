
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file triangulation/detail/boundarycomponent-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  BoundaryComponent class template.
 *
 *  This file is \e not included from boundarycomponent.h, but the routines
 *  it contains are explicitly instantiated in Regina's calculation engine.
 *  Therefore end users should never need to include this header.
 *
 *  The routines in this file are the ones that require a definition of
 *  the lower-dimensional class Triangulation<dim-1>.  Their implementations
 *  are "quarantined" here so that Triangulation<dim> does not
 *  end up automatically instantiating \e all triangulation classes
 *  Triangulation<dim-1>, Triangulation<dim-2>, ..., Triangulation<2>.
 */

#ifndef __BOUNDARYCOMPONENT_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __BOUNDARYCOMPONENT_IMPL_H_DETAIL
#endif

#include "triangulation/generic/boundarycomponent.h"

namespace regina {
namespace detail {

template <int dim, bool allFaces, bool allowVertex, bool canBuild>
BoundaryComponentStorage<dim, allFaces, allowVertex, canBuild>::
        ~BoundaryComponentStorage() {
    delete boundary_;
}

template <int dim, bool allFaces, bool allowVertex, bool canBuild_>
Triangulation<dim-1>*
        BoundaryComponentStorage<dim, allFaces, allowVertex, canBuild_>::
        buildRealBoundary() const {
    // From the precondition, there is a positive number of (dim-1)-faces.
    const auto& allFacets = facets();

    // Build a map from ((dim-1)-face index in underlying triangulation)
    // to ((dim-1)-face in boundary component).
    //
    // The way we build it ensures that (dim-1)-faces are added to the
    // new boundary triangulation in the same order as they appear in
    // the boundary component's list of (dim-1)-faces.
    Triangulation<dim>* mainTri = allFacets.front()->triangulation();
    Simplex<dim-1>** bdrySimplex = new Simplex<dim-1>*[
        mainTri->template countFaces<dim-1>()];

    Triangulation<dim-1>* ans = new Triangulation<dim-1>();
    typename Triangulation<dim-1>::ChangeEventSpan span(ans);

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

    /**
     * Now the triangulation is built, we need to reorder its
     * lower-dimensional faces to appear in the same order and with the
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
     * However: for dim=5, boundary components do not store lower-dimensional
     * faces, and so this ordering/numbering does not take place at all.
     *
     * TODO: Put in some kind of robust mechanism so that this issue
     * does not come back and bite us at a later date if/when the skeletal
     * computations are extended to do more than they do now.
     */
    ans->countComponents(); // ensures that the skeleton is calculated
    BoundaryComponentFaceStorage<dim, allFaces>::reorderAndRelabelFaces(ans);

    return ans;
}

} } // namespace regina::detail

#endif

