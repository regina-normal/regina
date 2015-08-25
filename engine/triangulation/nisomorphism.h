
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file triangulation/nisomorphism.h
 *  \brief Deals with combinatorial isomorphisms of triangulations.
 */

#ifndef __NISOMORPHISM_H
#ifndef __DOXYGEN
#define __NISOMORPHISM_H
#endif

#include "regina-core.h"
#include "generic/ngenericisomorphism.h"
#include "maths/nperm4.h"
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
 * In essence, a combinatorial isomorphism from triangulation \a T to
 * triangulation \a U is a one-to-one map from the tetrahedra of \a T to the
 * tetrahedra of \a U that allows relabelling of both the tetrahedra and
 * their 2-dimensional faces (or equivalently, their vertices), and that
 * preserves gluings across adjacent tetrahedra.
 *
 * More precisely:  An isomorphism consists of (i) a one-to-one map \a f
 * from the tetrahedra of \a T to the tetrahedra of \a U, and (ii) for each
 * tetrahedron \a S of \a T, a permutation \a f<sub>S</sub> of the faces
 * (0,1,2,3) of \a S, for which the following condition holds:
 *
 *   - If face \a k of tetrahedron \a S and face \a k' of tetrahedron \a S'
 *     are identified in \a T, then face \a f<sub>S</sub>(\a k) of \a f(S)
 *     and face \a f<sub>S'</sub>(\a k') of \a f(S') are identified in \a U.
 *     Moreover, their gluing is consistent with the face/vertex permutations;
 *     that is, there is a commutative square involving the gluing maps in
 *     \a T and \a U and the permutations \a f<sub>S</sub> and
 *     \a f<sub>S'</sub>.
 *
 * Isomorphisms can be <i>boundary complete</i> or
 * <i>boundary incomplete</i>.  A boundary complete isomorphism
 * satisfies the additional condition:
 *
 *   - If face \a x is a boundary face of \a T then face \a f(x) is a
 *     boundary face of \a U.
 *
 * A boundary complete isomorphism thus indicates that a copy of
 * triangulation \a T is present as an entire component (or components) of \a U,
 * whereas a boundary incomplete isomorphism represents an embedding of a
 * copy of triangulation \a T as a subcomplex of some possibly larger component
 * (or components) of \a U.
 *
 * Note that for all types of isomorphism, triangulation \a U is allowed
 * to contain more tetrahedra than triangulation \a T.
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

