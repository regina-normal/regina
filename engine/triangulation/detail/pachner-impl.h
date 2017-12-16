
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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
 *  This file is \e not included from triangulation.h, but the routines
 *  it contains are explicitly instantiated in Regina's calculation engine.
 *  Therefore end users should never need to include this header.
 */

#ifndef __PACHNER_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __PACHNER_IMPL_H_DETAIL
#endif

#include "triangulation/generic/triangulation.h"

namespace regina {
namespace detail {

template <int dim>
bool TriangulationBase<dim>::pachner(Vertex<dim>* v, bool check, bool perform) {
    if (check) {
        // Recall that both invalid and ideal vertices are considered boundary.
        if (v->isBoundary())
            return false;
        if (v->degree() != dim + 1)
            return false;
    }

    // v must meet (dim+1) distinct top-dimensional simplices, which must be
    // glued around the vertex in a way that gives a dim-simplex link.
    // Find these simplices.
    Simplex<dim>* oldSimp[dim + 1];
    Perm<dim + 1> oldVertices[dim + 1]; // 0 -> vertex, 1..dim -> link

    // We will permute oldVertices so that:
    // - oldSimp[i] has vertex i = v
    // - oldSimp[i] <-> oldSimp[j] with permutation i <-> j
    // This is possible iff we have a dim-simplex link.

    oldSimp[0] = v->front().simplex();
    oldVertices[0] = v->front().vertices();

    int i,j;
    for (i = 1; i <= dim; ++i) {
        oldSimp[i] = oldSimp[0]->adjacentSimplex(oldVertices[0][i]);
        if (check)
            for (j = 0; j < i; ++j)
                if (oldSimp[i] == oldSimp[j])
                    return false;
        oldVertices[i] = oldSimp[0]->adjacentGluing(oldVertices[0][i]) *
            oldVertices[0] * Perm<dim + 1>(0, i);
    }

    if (check) {
        for (i = 1; i <= dim; ++i)
            for (j = 1; j < i; ++j) {
                if (oldSimp[i] != oldSimp[j]->adjacentSimplex(
                        oldVertices[j][i]))
                    return false;
                if (oldVertices[i] !=
                        oldSimp[j]->adjacentGluing(oldVertices[j][i]) *
                        oldVertices[j] * Perm<dim + 1>(i, j))
                    return false;
            }
    }

    if (! perform)
        return true;

    // Perform the move.
    bool rememberSimpleLinks = knownSimpleLinks_;

    typename Triangulation<dim>::ChangeEventSpan span(
        static_cast<Triangulation<dim>*>(this));

    Simplex<dim>* newSimp = newSimplex();

    // Find where their facets need to be glued.
    // Old simplex i, facet i <-> New simplex, facet i.
    Simplex<dim>* adjSimp[dim + 1];
    Perm<dim + 1> adjGluing[dim + 1];
    for (i = 0; i <= dim; ++i) {
        adjSimp[i] = oldSimp[i]->adjacentSimplex(oldVertices[i][i]);
        adjGluing[i] = oldSimp[i]->adjacentGluing(oldVertices[i][i]) *
            oldVertices[i];

        // Are we are gluing the new simplex to itself?
        for (j = 0; j <= dim; ++j) {
            if (adjSimp[i] == oldSimp[j]) {
                // This glues to old simplex j, facet oldVertices[j].
                if (i > j) {
                    // Ensure we make the gluing in just one
                    // direction, not both directions.
                    adjSimp[i] = 0;
                } else {
                    // Adjust the gluing to point to the new simplex.
                    adjSimp[i] = newSimp;
                    adjGluing[i] = oldVertices[j].inverse() * adjGluing[i];
                }
                break;
            }
        }
    }

    // Now go ahead and make the gluings.
    for (i = 0; i <= dim; ++i)
        oldSimp[i]->isolate();
    for (i = 0; i <= dim; ++i)
        if (adjSimp[i])
            newSimp->join(i, adjSimp[i], adjGluing[i]);

    // Delete the old simplices.
    for (i = 0; i <= dim; ++i)
        removeSimplex(oldSimp[i]);

    // All done!
    knownSimpleLinks_ = rememberSimpleLinks;
    return true;
}

template <int dim>
bool TriangulationBase<dim>::pachner(Simplex<dim>* simp, bool /* check */,
        bool perform) {
    if ( !perform )
        return true; // You can always do this move.

    bool rememberSimpleLinks = knownSimpleLinks_;

    typename Triangulation<dim>::ChangeEventSpan span(
        static_cast<Triangulation<dim>*>(this));

    // Before we unglue, record how the adjacent simplices are glued to simp.
    Simplex<dim>* adjSimp[dim + 1];
    Perm<dim + 1> adjGlue[dim + 1];
    unsigned i, j;
    for (i = 0; i <= dim; i++) {
        adjSimp[i] = simp->adjacentSimplex(i);
        if (adjSimp[i])
            adjGlue[i] = simp->adjacentGluing(i);
    }

    // Unglue the old simplex.
    simp->isolate();

    // The new simplices.
    // Facet i of the old simplex will become a facet of newSimp[i].
    // Vertex i of newSimp[i] will become the new internal vertex, and
    // the other dim vertices of newSimp[i] will keep the same vertex numbers
    // that they had in the old simplex.
    Simplex<dim>* newSimp[dim + 1];
    for (i = 0; i <= dim; ++i)
        newSimp[i] = newSimplex();

    // Glue the new simplices to each other internally.
    for (i = 0; i <= dim; ++i)
        for (j = i + 1; j <= dim; ++j)
            newSimp[i]->join(j, newSimp[j], Perm<dim + 1>(i, j));

    // Attach the new simplices to the old triangulation.
    for (i = 0; i <= dim; ++i) {
        if (adjSimp[i] == simp) {
            // The old simplex was glued to itself.

            // We might have already made this gluing from the other side:
            if (newSimp[i]->adjacentSimplex(i))
                continue;

            // Nope, do it now.
            newSimp[i]->join(i, newSimp[adjGlue[i][i]], adjGlue[i]);
        } else if (adjSimp[i]) {
            // The old simplex was glued elsewhere.
            newSimp[i]->join(i, adjSimp[i], adjGlue[i]);
        }
    }

    // Delete the old simplex.
    removeSimplex(simp);

    // All done!
    knownSimpleLinks_ = rememberSimpleLinks;
    return true;
}

} } // namespace regina::detail

#endif
