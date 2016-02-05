
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

/*! \file triangulation/nisomorphism.h
 *  \brief Deals with combinatorial isomorphisms of 3-manifold triangulations.
 */

#ifndef __NISOMORPHISM_H
#ifndef __DOXYGEN
#define __NISOMORPHISM_H
#endif

#include "regina-core.h"
#include "generic/isomorphism.h"
#include "maths/nperm4.h"

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Represents a combinatorial isomorphism from one 3-manifold triangulation
 * into another.
 *
 * This is a specialisation of the generic Isomorphism class template;
 * see the Isomorphism documentation for an overview of how this class works.
 *
 * This 3-dimensional specialisation contains additional dimension-specific
 * aliases for some commonly-used routines.
 */
template <>
class REGINA_API Isomorphism<3> : public detail::IsomorphismBase<3> {
    public:
        /**
         * Creates a new isomorphism with no initialisation.
         * The images of the simplices and their vertices must be
         * explicitly set using simpImage() and facetPerm().
         *
         * \ifacespython Not present.
         *
         * @param nTetrahedra the number of simplices in the source
         * triangulation associated with this isomorphism.
         * This is allowed to be zero.
         */
        Isomorphism(unsigned nTetrahedra);
        /**
         * Creates a new copy of the given isomorphism.
         *
         * @param copy the isomorphism to copy.
         */
        Isomorphism(const Isomorphism& copy);

        /**
         * A deprecated dimension-specific alias for size().
         *
         * \deprecated Simply call size() instead.
         *
         * See size() for further information.
         */
        REGINA_DEPRECATED unsigned getSourceTetrahedra() const;

        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        int& tetImage(unsigned sourceTet);
        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        int tetImage(unsigned sourceTet) const;
        /**
         * A dimension-specific alias for facetPerm().
         *
         * See facetPerm() for further information.
         */
        NPerm4& facePerm(unsigned sourceTet);
        /**
         * A dimension-specific alias for facetPerm().
         *
         * See facetPerm() for further information.
         */
        NPerm4 facePerm(unsigned sourceTet) const;
};

/**
 * A convenience typedef for Isomorphism<3>.
 */
typedef Isomorphism<3> NIsomorphism;

/*@}*/

// Inline functions for Isomorphism<3>

inline Isomorphism<3>::Isomorphism(unsigned sourceTetrahedra) :
        detail::IsomorphismBase<3>(sourceTetrahedra) {
}

inline Isomorphism<3>::Isomorphism(const Isomorphism& cloneMe) :
        detail::IsomorphismBase<3>(cloneMe) {
}

inline unsigned Isomorphism<3>::getSourceTetrahedra() const {
    return nSimplices_;
}

inline int& Isomorphism<3>::tetImage(unsigned sourceTet) {
    return simpImage_[sourceTet];
}

inline int Isomorphism<3>::tetImage(unsigned sourceTet) const {
    return simpImage_[sourceTet];
}

inline NPerm4& Isomorphism<3>::facePerm(unsigned sourceTet) {
    return facetPerm_[sourceTet];
}
inline NPerm4 Isomorphism<3>::facePerm(unsigned sourceTet) const {
    return facetPerm_[sourceTet];
}

} // namespace regina

#endif

