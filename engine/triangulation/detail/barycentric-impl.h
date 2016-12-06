
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

/*! \file triangulation/detail/barycentric-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  Triangulation class template.
 *
 *  This file is automatically included from triangulation.h; there is
 *  no need for end users to include it explicitly.
 */

#ifndef __BARYCENTRIC_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __BARYCENTRIC_IMPL_H_DETAIL
#endif

namespace regina {
namespace detail {

// IMPORTANT: If this code is ever rewritten (and in particular, if
// the labelling of new tetrahedra ever changes), then the
// drillEdge() code must be rewritten as well (since it relies on
// the specific labelling scheme that we use here).

template <int dim>
void TriangulationBase<dim>::barycentricSubdivision()
{
    typename Triangulation<dim>::ChangeEventSpan span(
        static_cast<Triangulation<dim>*>(this));

    // The subsimplices in the barycentric subdivision are naturally
    // parametrized by pairs (original simplex s, permutation p) such that
    // vertex 0 of subsimplex (s,p) is at vertex p[0] of s
    // vertex 1 of subsimplex (s,p) is at the center of the edge p[0] p[1] of s
    // ...
    // vertex k of subsimplex (s,p) is at the center of the face of s spanned by
    //                                       p[0], p[1], ..., p[k]
    //
    // The ordering of simplices on the original triangulation and the
    // lexicographic ordering on permutations gives a natural ordering of the
    // pairs (s,p) which we use here.
    //
    // (dim+1) factorial
    static constexpr size_t nPerms = Perm<dim+1>::nPerms;
    
    // Allocate the new subsimplices
    MarkedVector<Simplex<dim>> newSimplices;
    for (size_t i = 0; i < nPerms * simplices_.size(); i++) {
        newSimplices.push_back(
            new Simplex<dim>(static_cast<Triangulation<dim>*>(this)));
    }
    
    // The internal gluings of the barycentric subdivision along the faces
    // f = 0, 1, ..., dim-1.
    // Two neighboring subsimplices in the barycentric subdivision of a simplex
    // touching along face f have permutations related by a transposition
    // f <-> f+1

    // For all simplices s of the original permutation
    for (size_t i = 0; i < simplices_.size(); i++) {
        // For all permutations p
        for (size_t j = 0; j < nPerms; j++) {
            // Permutation p this subsimplex corresponds to
            const Perm<dim+1> perm = Perm<dim+1>::atIndex(j);
            // Subsimplex (s, p)
            Simplex<dim>* simp = newSimplices[i * nPerms + j];
            for (int f = 0; f < dim; f++) {
                // Only glue if not already glued earlier (we see each face
                // twice)
                if (not simp->adjacentSimplex(f)) {
                    // Permutation the neighboring subsimplex corresponds to
                    const Perm<dim+1> otherPerm = perm * Perm<dim+1>(f, f+1);
                    Simplex<dim>* otherSimp =
                        newSimplices[i * nPerms + otherPerm.index()];
                    simp->join(f, otherSimp, Perm<dim+1>());
                }
            }
        }
    }
    
    // It is left to glue the faces f = dim by translating the gluings
    // of the original triangulation.
    
    // For all simplices s of the original triangulation
    for (size_t i = 0; i < simplices_.size(); i++) {
        const Simplex<dim>* originalSimp = simplices_[i];
        // For all corresponding subsimplices (s,p)
        for (size_t j = 0; j < nPerms; j++) {
            // The permutation p
            const Perm<dim+1> perm = Perm<dim+1>::atIndex(j);
            // The subsimplex (s, p) 
            Simplex<dim>* simp = newSimplices[i * nPerms + j];
            // Don't glue if we already glued earlier (we encounter each face
            // twice in the process)
            if (not simp->adjacentSimplex(dim)) {
                // The face f of the original simplex s this subsimplex (s,p)
                // is adjacent to
                const int f = perm[dim];
                
                // Find the simplex neighboring the original simplex s along
                // the face f.
                // The face f might be unglued.
                if (const Simplex<dim>* otherOriginalSimp =
                    originalSimp->adjacentSimplex(f)) {
                    
                    // The gluing on the face f
                    const Perm<dim+1> originalGluing =
                        originalSimp->adjacentGluing(f);
                    const size_t otherOriginalIndex =
                        otherOriginalSimp->index();
                    
                    // Find the other (s',p') by letting the gluing act
                    // on p.
                    const Perm<dim+1> otherPerm = originalGluing * perm;

                    // The other (s', p')
                    Simplex<dim>* otherSimp =
                        newSimplices[
                            otherOriginalIndex * nPerms + otherPerm.index()];
                    
                    simp->join(dim, otherSimp, Perm<dim+1>());
                }
            }
        }
    }
    
    // Free the original simplices and swap
    for (int i = 0; i < simplices_.size(); i++) {
        delete simplices_[i];
    }
    simplices_.swap(newSimplices);
    
    static_cast<Triangulation<dim>*>(this)->clearAllProperties();
}

}
}

#endif
