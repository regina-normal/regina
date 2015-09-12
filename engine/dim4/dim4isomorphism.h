
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file dim4/dim4isomorphism.h
 *  \brief Deals with combinatorial isomorphisms of 4-manifold triangulations.
 */

#ifndef __DIM4ISOMORPHISM_H
#ifndef __DOXYGEN
#define __DIM4ISOMORPHISM_H
#endif

#include "regina-core.h"
#include "generic/nfacetspec.h"
#include "generic/ngenericisomorphism.h"
#include "maths/nperm5.h"

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
 * In essence, a combinatorial isomorphism from triangulation \a T to
 * triangulation \a U is a one-to-one map from the pentachora of \a T to the
 * pentachora of \a U that allows relabelling of both the pentachora and
 * their facets (or equivalently, their vertices), and that preserves
 * gluings across adjacent pentachora.
 *
 * More precisely:  An isomorphism consists of (i) a one-to-one map \a f
 * from the pentachora of \a T to the pentachora of \a U, and (ii) for each
 * pentachoron \a S of \a T, a permutation \a f<sub>S</sub> of the facets
 * (0,1,2,3,4) of \a S, for which the following condition holds:
 *
 *   - If facet \a k of pentachoron \a S and facet \a k' of pentachoron \a S'
 *     are identified in \a T, then facet \a f<sub>S</sub>(\a k) of \a f(S)
 *     and facet \a f<sub>S'</sub>(\a k') of \a f(S') are identified in \a U.
 *     Moreover, their gluing is consistent with the facet/vertex permutations;
 *     that is, there is a commutative square involving the gluing maps in
 *     \a T and \a U and the permutations \a f<sub>S</sub> and
 *     \a f<sub>S'</sub>.
 *
 * Isomorphisms can be <i>boundary complete</i> or
 * <i>boundary incomplete</i>.  A boundary complete isomorphism
 * satisfies the additional condition:
 *
 *   - If facet \a x is a boundary facet of \a T then facet \a f(x) is a
 *     boundary facet of \a U.
 *
 * A boundary complete isomorphism thus indicates that a copy of
 * triangulation \a T is present as an entire component (or components) of \a U,
 * whereas a boundary incomplete isomorphism represents an embedding of a
 * copy of triangulation \a T as a subcomplex of some possibly larger component
 * (or components) of \a U.
 *
 * Note that for all types of isomorphism, triangulation \a U is allowed
 * to contain more pentachora than triangulation \a T.
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

