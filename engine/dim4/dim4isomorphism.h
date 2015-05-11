
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
#include "generic/isomorphism.h"
#include "maths/nperm5.h"

namespace regina {

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a combinatorial isomorphism from one 4-manifold triangulation
 * into another.
 *
 * This is a specialisation of the generic Isomorphism class template;
 * see the Isomorphism documentation for an overview of how this class works.
 *
 * This 4-dimensional specialisation contains additional dimension-specific
 * aliases for some commonly-used routines.
 */
template <>
class REGINA_API Isomorphism<4> : public IsomorphismBase<4> {
    public:
        /**
         * Creates a new isomorphism with no initialisation.
         * The images of the simplices and their vertices must be
         * explicitly set using simpImage() and facetPerm)().
         *
         * \ifacespython Not present.
         *
         * @param nPentachora the number of pentachora in the source
         * triangulation associated with this isomorphism.
         * This is allowed to be zero.
         */
        Isomorphism(unsigned nPentachora);
        /**
         * Creates a new copy of the given isomorphism.
         *
         * @param copy the isomorphism to copy.
         */
        Isomorphism(const Isomorphism& copy);

        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        int& pentImage(unsigned sourcePent);

        /**
         * A dimension-specific alias for simpImage().
         *
         * See simpImage() for further information.
         */
        int pentImage(unsigned sourcePent) const;
};

/**
 * A convenience typedef for Isomorphism<4>.
 */
typedef Isomorphism<4> Dim4Isomorphism;

/*@}*/

// Inline functions for Isomorphism<4>

inline Isomorphism<4>::Isomorphism(unsigned sourcePentachora) :
        IsomorphismBase<4>(sourcePentachora) {
}

inline Isomorphism<4>::Isomorphism(const Dim4Isomorphism& cloneMe) :
        IsomorphismBase<4>(cloneMe) {
}

inline int& Isomorphism<4>::pentImage(unsigned sourcePent) {
    return simpImage_[sourcePent];
}

inline int Isomorphism<4>::pentImage(unsigned sourcePent) const {
    return simpImage_[sourcePent];
}

} // namespace regina

#endif

