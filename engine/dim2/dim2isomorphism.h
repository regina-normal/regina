
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

/*! \file dim2/dim2isomorphism.h
 *  \brief Deals with combinatorial isomorphisms of 2-manifold triangulations.
 */

#ifndef __DIM2ISOMORPHISM_H
#ifndef __DOXYGEN
#define __DIM2ISOMORPHISM_H
#endif

#include "regina-core.h"
#include "shareableobject.h"
#include "maths/nperm3.h"
#include "triangulation/nfacetspec.h"
#include "triangulation/ngenericisomorphism.h"

namespace regina {

class Dim2Triangulation;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Represents a combinatorial isomorphism from one 2-manifold triangulation
 * into another.
 *
 * In essence, a combinatorial isomorphism from triangulation T to
 * triangulation U is a one-to-one map from the triangles of T to the
 * triangles of U that allows relabelling of both the triangles and
 * their edges (or equivalently, their vertices), and that preserves
 * gluings across adjacent triangles.
 *
 * More precisely:  An isomorphism consists of (i) a one-to-one map f
 * from the triangles of T to the triangles of U, and (ii) for each
 * triangle S of T, a permutation f_S of the edges (0,1,2) of S,
 * for which the following condition holds:
 *
 *   - If edge k of triangle S and edge k' of triangle S'
 *     are identified in T, then edge f_S(k) of f(S) and edge f_S'(k')
 *     of f(S') are identified in U.  Moreover, their gluing is consistent
 *     with the edge/vertex permutations; that is, there is a commutative
 *     square involving the gluing maps in T and U and the permutations
 *     f_S and f_S'.
 *
 * Isomorphisms can be <i>boundary complete</i> or
 * <i>boundary incomplete</i>.  A boundary complete isomorphism
 * satisfies the additional condition:
 *
 *   - If edge x is a boundary edge of T then edge f(x) is a boundary
 *     edge of U.
 *
 * A boundary complete isomorphism thus indicates that a copy of
 * triangulation T is present as an entire component (or components) of U,
 * whereas a boundary incomplete isomorphism represents an embedding of a
 * copy of triangulation T as a subcomplex of some possibly larger component
 * (or components) of U.
 *
 * Note that in all cases triangulation U may contain more triangles
 * than triangulation T.
 */
class REGINA_API Dim2Isomorphism : public NGenericIsomorphism<2> {
    public:
        /**
         * Creates a new isomorphism with no initialisation.
         *
         * \ifacespython Not present.
         *
         * @param sourceTriangles the number of triangles in the source
         * triangulation associated with this isomorphism; this may be zero.
         */
        Dim2Isomorphism(unsigned sourceTriangles);
        /**
         * Creates a new isomorphism identical to the given isomorphism.
         *
         * @param cloneMe the isomorphism upon which to base the new
         * isomorphism.
         */
        Dim2Isomorphism(const Dim2Isomorphism& cloneMe);

        /**
         * Returns the number of triangles in the source triangulation
         * associated with this isomorphism.  Note that this is always
         * less than or equal to the number of triangles in the
         * destination triangulation.
         *
         * This is a convenience routine specific to two dimensions, and is
         * identical to the dimension-agnostic routine getSourceSimplices().
         *
         * @return the number of triangles in the source triangulation.
         */
        unsigned getSourceTriangles() const;

        /**
         * Determines the image of the given source triangle under
         * this isomorphism.
         *
         * This is a convenience routine specific to two dimensions, and is
         * identical to the dimension-agnostic routine simpImage().
         *
         * \ifacespython Not present, though the read-only version of this
         * routine is.
         *
         * @param sourceTriangle the index of the source triangle; this must
         * be between 0 and <tt>getSourceSimplices()-1</tt> inclusive.
         * @return a reference to the index of the destination triangle
         * that the source triangle maps to.
         */
        int& triImage(unsigned sourceTriangle);

        /**
         * Determines the image of the given source triangle under
         * this isomorphism.
         *
         * This is a convenience routine specific to two dimensions, and is
         * identical to the dimension-agnostic routine simpImage().
         *
         * @param sourceTriangle the index of the source triangle; this must
         * be between 0 and <tt>getSourceSimplices()-1</tt> inclusive.
         * @return the index of the destination triangle
         * that the source triangle maps to.
         */
        int triImage(unsigned sourceTriangle) const;

        /**
         * Returns a read-write reference to the permutation that is
         * applied to the three edges of the given source triangle
         * under this isomorphism.
         * Edge \a i of source triangle \a sourceTriangle will be mapped to
         * edge <tt>facetPerm(sourceTriangle)[i]</tt> of triangle
         * <tt>simpImage(sourceTriangle)</tt>.
         *
         * This is a convenience routine specific to two dimensions, and is
         * identical to the dimension-agnostic routine facetPerm().
         *
         * \ifacespython Not present, though the read-only version of this
         * routine is.
         *
         * @param sourceTriangle the index of the source triangle containing
         * the original three edges; this must be between 0 and
         * <tt>getSourceTriangles()-1</tt> inclusive.
         * @return a read-write reference to the permutation applied to the
         * three edges of the source triangle.
         */
        NPerm3& edgePerm(unsigned sourceTriangle);

        /**
         * Determines the permutation that is applied to the three edges
         * of the given source triangle under this isomorphism.
         * Edge \a i of source triangle \a sourceTriangle will be mapped to
         * triangle <tt>facetPerm(sourceTriangle)[i]</tt> of triangle
         * <tt>simpImage(sourceTriangle)</tt>.
         *
         * This is a convenience routine specific to two dimensions, and is
         * identical to the dimension-agnostic routine facetPerm().
         *
         * @param sourceTriangle the index of the source triangle containing
         * the original three edges; this must be between 0 and
         * <tt>getSourceTriangles()-1</tt> inclusive.
         * @return the permutation applied to the three edges of the
         * source triangle.
         */
        NPerm3 edgePerm(unsigned sourceTriangle) const;
};

/*@}*/

// Inline functions for Dim2Isomorphism

inline Dim2Isomorphism::Dim2Isomorphism(unsigned sourceTriangles) :
        NGenericIsomorphism<2>(sourceTriangles) {
}

inline Dim2Isomorphism::Dim2Isomorphism(const Dim2Isomorphism& cloneMe) :
        NGenericIsomorphism<2>(cloneMe) {
}

inline unsigned Dim2Isomorphism::getSourceTriangles() const {
    return nSimplices_;
}

inline int& Dim2Isomorphism::triImage(unsigned sourceTriangle) {
    return simpImage_[sourceTriangle];
}

inline int Dim2Isomorphism::triImage(unsigned sourceTriangle) const {
    return simpImage_[sourceTriangle];
}

inline NPerm3& Dim2Isomorphism::edgePerm(unsigned sourceTriangle) {
    return facetPerm_[sourceTriangle];
}
inline NPerm3 Dim2Isomorphism::edgePerm(unsigned sourceTriangle) const {
    return facetPerm_[sourceTriangle];
}

} // namespace regina

#endif

