
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

/*! \file dim2/dim2isomorphism.h
 *  \brief Deals with combinatorial isomorphisms of 2-manifold triangulations.
 */

#ifndef __DIM2ISOMORPHISM_H
#ifndef __DOXYGEN
#define __DIM2ISOMORPHISM_H
#endif

#include "regina-core.h"
#include "generic/facetspec.h"
#include "generic/isomorphism.h"

namespace regina {

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Represents a combinatorial isomorphism from one 2-manifold triangulation
 * into another.
 *
 * This is a specialisation of the generic Isomorphism class template;
 * see the Isomorphism documentation for an overview of how this class works.
 *
 * This 2-dimensional specialisation contains additional dimension-specific
 * aliases for some commonly-used routines.
 */
template <>
class REGINA_API Isomorphism<2> : public detail::IsomorphismBase<2> {
    public:
        /**
         * Creates a new isomorphism with no initialisation.
         * The images of the simplices and their vertices must be
         * explicitly set using simpImage() and facetPerm().
         *
         * \ifacespython Not present.
         *
         * @param nTriangles the number of simplices in the source
         * triangulation associated with this isomorphism.
         * This is allowed to be zero.
         */
        Isomorphism(unsigned nTriangles);
        /**
         * Creates a new copy of the given isomorphism.
         *
         * @param copy the isomorphism to copy.
         */
        Isomorphism(const Isomorphism<2>& copy);

        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        int& triImage(unsigned sourceTriangle);

        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        int triImage(unsigned sourceTriangle) const;

        /**
         * A dimension-specific alias for facetPerm().
         *
         * See facetPerm() for further information.
         */
        NPerm<3>& edgePerm(unsigned sourceTriangle);

        /**
         * A dimension-specific alias for facetPerm().
         *
         * See facetPerm() for further information.
         */
        NPerm<3> edgePerm(unsigned sourceTriangle) const;
};

/**
 * A convenience typedef for Isomorphism<2>.
 */
typedef Isomorphism<2> Dim2Isomorphism;

/*@}*/

// Inline functions for Isomorphism<2>

inline Isomorphism<2>::Isomorphism(unsigned sourceTriangles) :
        detail::IsomorphismBase<2>(sourceTriangles) {
}

inline Isomorphism<2>::Isomorphism(const Isomorphism<2>& cloneMe) :
        detail::IsomorphismBase<2>(cloneMe) {
}

inline int& Isomorphism<2>::triImage(unsigned sourceTriangle) {
    return simpImage_[sourceTriangle];
}

inline int Isomorphism<2>::triImage(unsigned sourceTriangle) const {
    return simpImage_[sourceTriangle];
}

inline NPerm<3>& Isomorphism<2>::edgePerm(unsigned sourceTriangle) {
    return facetPerm_[sourceTriangle];
}
inline NPerm<3> Isomorphism<2>::edgePerm(unsigned sourceTriangle) const {
    return facetPerm_[sourceTriangle];
}

} // namespace regina

#endif

