
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

/*! \file dim4/dim4isomorphism.h
 *  \brief Deals with combinatorial isomorphisms of 4-manifold triangulations.
 */

#ifndef __DIM4ISOMORPHISM_H
#ifndef __DOXYGEN
#define __DIM4ISOMORPHISM_H
#endif

#include "regina-core.h"
#include "shareableobject.h"
#include "maths/nperm5.h"
#include "triangulation/nfacetspec.h"
#include "triangulation/ngeneralisomorphism.h"

namespace regina {

class Dim4Triangulation;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a combinatorial isomorphism from one 4-manifold triangulation
 * into another.
 *
 * In essence, a combinatorial isomorphism from triangulation T to
 * triangulation U is a one-to-one map from the pentachora of T to the
 * pentachora of U that allows relabelling of both the pentachora and
 * their facets (or equivalently, their vertices), and that preserves
 * gluings across adjacent pentachora.
 *
 * More precisely:  An isomorphism consists of (i) a one-to-one map f
 * from the pentachora of T to the pentachora of U, and (ii) for each
 * pentachoron S of T, a permutation f_S of the facets (0,1,2,3,4) of S,
 * for which the following condition holds:
 *
 *   - If facet k of pentachoron S and facet k' of pentachoron S'
 *     are identified in T, then facet f_S(k) of f(S) and facet f_S'(k')
 *     of f(S') are identified in U.  Moreover, their gluing is consistent
 *     with the facet/vertex permutations; that is, there is a commutative
 *     square involving the gluing maps in T and U and the permutations
 *     f_S and f_S'.
 *
 * Isomorphisms can be <i>boundary complete</i> or
 * <i>boundary incomplete</i>.  A boundary complete isomorphism
 * satisfies the additional condition:
 *
 *   - If facet x is a boundary facet of T then facet f(x) is a boundary
 *     facet of U.
 *
 * A boundary complete isomorphism thus indicates that a copy of
 * triangulation T is present as an entire component (or components) of U,
 * whereas a boundary incomplete isomorphism represents an embedding of a
 * copy of triangulation T as a subcomplex of some possibly larger component
 * (or components) of U.
 *
 * Note that in all cases triangulation U may contain more pentachora
 * than triangulation T.
 */
class REGINA_API Dim4Isomorphism : public NGenericIsomorphism<4> {
    public:
        /**
         * Creates a new isomorphism with no initialisation.
         *
         * \ifacespython Not present.
         *
         * @param sourcePentachora the number of pentachora in the source
         * triangulation associated with this isomorphism; this may be zero.
         */
        Dim4Isomorphism(unsigned sourcePentachora);
        /**
         * Creates a new isomorphism identical to the given isomorphism.
         *
         * @param cloneMe the isomorphism upon which to base the new
         * isomorphism.
         */
        Dim4Isomorphism(const Dim4Isomorphism& cloneMe);

        /**
         * Returns the number of pentachora in the source triangulation
         * associated with this isomorphism.  Note that this is always
         * less than or equal to the number of pentachora in the
         * destination triangulation.
         *
         * This is a convenience routine specific to four dimensions, and is
         * identical to the dimension-agnostic routine getSourceSimplices().
         *
         * @return the number of pentachora in the source triangulation.
         */
        unsigned getSourcePentachora() const;

        /**
         * Determines the image of the given source pentachoron under
         * this isomorphism.
         *
         * This is a convenience routine specific to four dimensions, and is
         * identical to the dimension-agnostic routine simpImage().
         *
         * \ifacespython Not present, though the read-only version of
         * this routine is.
         *
         * @param sourcePent the index of the source pentachoron; this must
         * be between 0 and <tt>getSourceSimplices()-1</tt> inclusive.
         * @return a reference to the index of the destination pentachoron
         * that the source pentachoron maps to.
         */
        int& pentImage(unsigned sourcePent);

        /**
         * Determines the image of the given source pentachoron under
         * this isomorphism.
         *
         * This is a convenience routine specific to four dimensions, and is
         * identical to the dimension-agnostic routine simpImage().
         *
         * @param sourcePent the index of the source pentachoron; this must
         * be between 0 and <tt>getSourceSimplices()-1</tt> inclusive.
         * @return the index of the destination pentachoron
         * that the source pentachoron maps to.
         */
        int pentImage(unsigned sourcePent) const;
};

/*@}*/

// Inline functions for Dim4Isomorphism

inline Dim4Isomorphism::Dim4Isomorphism(unsigned sourcePentachora) :
        NGenericIsomorphism<4>(sourcePentachora) {
}

inline Dim4Isomorphism::Dim4Isomorphism(const Dim4Isomorphism& cloneMe) :
        NGenericIsomorphism<4>(cloneMe) {
}

inline unsigned Dim4Isomorphism::getSourcePentachora() const {
    return nSimplices_;
}

inline int& Dim4Isomorphism::pentImage(unsigned sourcePent) {
    return simpImage_[sourcePent];
}

inline int Dim4Isomorphism::pentImage(unsigned sourcePent) const {
    return simpImage_[sourcePent];
}

} // namespace regina

#endif

