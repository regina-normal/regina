
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

/*! \file triangulation/nisomorphism.h
 *  \brief Deals with combinatorial isomorphisms of triangulations.
 */

#ifndef __NISOMORPHISM_H
#ifndef __DOXYGEN
#define __NISOMORPHISM_H
#endif

#include "regina-core.h"
#include "shareableobject.h"
#include "maths/nperm4.h"
#include "triangulation/ngenericisomorphism.h"
#include "triangulation/ntetface.h"

namespace regina {

class NTriangulation;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Represents a combinatorial isomorphism from one triangulation into
 * another.
 *
 * In essence, a combinatorial isomorphism from triangulation T to
 * triangulation U is a one-to-one map from the tetrahedra of T to the
 * tetrahedra of U that allows relabelling of both the tetrahedra and
 * their faces (or equivalently, their vertices), and that preserves
 * gluings across adjacent tetrahedra.
 *
 * More precisely:  An isomorphism consists of (i) a one-to-one map f
 * from the tetrahedra of T to the tetrahedra of U, and (ii) for each
 * tetrahedron S of T, a permutation f_S of the facets (0,1,2,3) of S,
 * for which the following condition holds:
 *
 *   - If face k of tetrahedron S and face k' of tetrahedron S'
 *     are identified in T, then face f_S(k) of f(S) and face f_S'(k')
 *     of f(S') are identified in U.  Moreover, their gluing is consistent
 *     with the face/vertex permutations; that is, there is a commutative
 *     square involving the gluing maps in T and U and the permutations
 *     f_S and f_S'.
 *
 * Isomorphisms can be <i>boundary complete</i> or
 * <i>boundary incomplete</i>.  A boundary complete isomorphism
 * satisfies the additional condition:
 *
 *   - If face x is a boundary face of T then face f(x) is a boundary
 *     face of U.
 *
 * A boundary complete isomorphism thus indicates that a copy of
 * triangulation T is present as an entire component (or components) of U,
 * whereas a boundary incomplete isomorphism represents an embedding of a
 * copy of triangulation T as a subcomplex of some possibly larger component
 * (or components) of U.
 *
 * Note that in all cases triangulation U may contain more tetrahedra
 * than triangulation T.
 *
 * \testpart
 *
 * \todo \feature Composition of isomorphisms.
 */
class REGINA_API NIsomorphism : public NGenericIsomorphism<3> {
    public:
        /**
         * Creates a new isomorphism with no initialisation.
         *
         * \ifacespython Not present.
         *
         * @param sourceTetrahedra the number of tetrahedra in the source
         * triangulation associated with this isomorphism; this may be zero.
         */
        NIsomorphism(unsigned sourceTetrahedra);
        /**
         * Creates a new isomorphism identical to the given isomorphism.
         *
         * @param cloneMe the isomorphism upon which to base the new
         * isomorphism.
         */
        NIsomorphism(const NIsomorphism& cloneMe);

        /**
         * Returns the number of tetrahedra in the source triangulation
         * associated with this isomorphism.  Note that this is always
         * less than or equal to the number of tetrahedra in the
         * destination triangulation.
         *
         * This is a convenience routine specific to three dimensions, and is
         * identical to the dimension-agnostic routine getSourceSimplices().
         *
         * @return the number of tetrahedra in the source triangulation.
         */
        unsigned getSourceTetrahedra() const;

        /**
         * Determines the image of the given source tetrahedron under
         * this isomorphism.
         *
         * This is a convenience routine specific to three dimensions, and is
         * identical to the dimension-agnostic routine simpImage().
         *
         * \ifacespython Not present, though the read-only version of
         * this routine is.
         *
         * @param sourceTet the index of the source tetrahedron; this must
         * be between 0 and <tt>getSourceSimplices()-1</tt> inclusive.
         * @return a reference to the index of the destination tetrahedron
         * that the source tetrahedron maps to.
         */
        int& tetImage(unsigned sourceTet);
        /**
         * Determines the image of the given source tetrahedron under
         * this isomorphism.
         *
         * This is a convenience routine specific to three dimensions, and is
         * identical to the dimension-agnostic routine simpImage().
         *
         * @param sourceTet the index of the source tetrahedron; this must
         * be between 0 and <tt>getSourceSimplices()-1</tt> inclusive.
         * @return the index of the destination tetrahedron
         * that the source tetrahedron maps to.
         */
        int tetImage(unsigned sourceTet) const;
        /**
         * Returns a read-write reference to the permutation that is
         * applied to the four faces of the given source tetrahedron
         * under this isomorphism.
         * Face \a i of source tetrahedron \a sourceTet will be mapped to
         * face <tt>facePerm(sourceTet)[i]</tt> of tetrahedron
         * <tt>tetImage(sourceTet)</tt>.
         *
         * This is a convenience routine specific to three dimensions, and is
         * identical to the dimension-agnostic routine facetPerm().
         *
         * \ifacespython Not present, though the read-only version of this
         * routine is.
         *
         * @param sourceTet the index of the source tetrahedron containing
         * the original four faces; this must be between 0 and
         * <tt>getSourceSimplices()-1</tt> inclusive.
         * @return a read-write reference to the permutation applied to the
         * four faces of the source tetrahedron.
         */
        NPerm4& facePerm(unsigned sourceTet);
        /**
         * Determines the permutation that is applied to the four faces
         * of the given source tetrahedron under this isomorphism.
         * Face \a i of source tetrahedron \a sourceTet will be mapped to
         * face <tt>facePerm(sourceTet)[i]</tt> of tetrahedron
         * <tt>tetImage(sourceTet)</tt>.
         *
         * This is a convenience routine specific to three dimensions, and is
         * identical to the dimension-agnostic routine facetPerm().
         *
         * @param sourceTet the index of the source tetrahedron containing
         * the original four faces; this must be between 0 and
         * <tt>getSourceSimplices()-1</tt> inclusive.
         * @return the permutation applied to the four faces of the
         * source tetrahedron.
         */
        NPerm4 facePerm(unsigned sourceTet) const;
};

/**
 * A deprecated synonym for NIsomorphism, provided for backward
 * compatibility only.  See NIsomorphism for further details (and please
 * use the NIsomorphism class instead).
 *
 * \deprecated All of the functionality that NIsomorphismDirect used to
 * provide in old versions of Regina has now been moved into the parent class
 * NIsomorphism.  The NIsomorphismDirect class is now an empty subclass of
 * NIsomorphism, provided for backward compatibility only, and should not
 * be used in new applications.  This class will be removed from Regina
 * in the near future.
 *
 * \ifacespython Not present.
 */
class REGINA_API NIsomorphismDirect : public NIsomorphism {
    public:
        /**
         * Creates a new isomorphism with no initialisation.
         *
         * @param sourceTetrahedra the number of tetrahedra in the source
         * triangulation associated with this isomorphism; this may be zero.
         */
        NIsomorphismDirect(unsigned sourceTetrahedra);
        /**
         * Creates a new isomorphism identical to the given isomorphism.
         *
         * @param cloneMe the isomorphism upon which to base the new
         * isomorphism.
         */
        NIsomorphismDirect(const NIsomorphism& cloneMe);
};

/*@}*/

// Inline functions for NIsomorphism

inline NIsomorphism::NIsomorphism(unsigned sourceTetrahedra) :
        NGenericIsomorphism<3>(sourceTetrahedra) {
}

inline NIsomorphism::NIsomorphism(const NIsomorphism& cloneMe) :
        NGenericIsomorphism<3>(cloneMe) {
}

inline unsigned NIsomorphism::getSourceTetrahedra() const {
    return nSimplices_;
}

inline int& NIsomorphism::tetImage(unsigned sourceTet) {
    return simpImage_[sourceTet];
}

inline int NIsomorphism::tetImage(unsigned sourceTet) const {
    return simpImage_[sourceTet];
}

inline NPerm4& NIsomorphism::facePerm(unsigned sourceTet) {
    return facetPerm_[sourceTet];
}
inline NPerm4 NIsomorphism::facePerm(unsigned sourceTet) const {
    return facetPerm_[sourceTet];
}

// Inline functions for NIsomorphismDirect

inline NIsomorphismDirect::NIsomorphismDirect(unsigned sourceTetrahedra) :
        NIsomorphism(sourceTetrahedra) {
}
inline NIsomorphismDirect::NIsomorphismDirect(const NIsomorphism& cloneMe) :
        NIsomorphism(cloneMe) {
}

} // namespace regina

#endif

