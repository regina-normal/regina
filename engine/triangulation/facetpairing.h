
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

/*! \file triangulation/facetpairing.h
 *  \brief Includes headers for working with facet pairings in all dimensions
 *  except for dimension 3.  Specifically, this includes headers for the
 *  class FacetPairing<dim> for all \a dim &ne; 3.
 */

#ifndef __FACETPAIRING_H
#ifndef __DOXYGEN
#define __FACETPAIRING_H
#endif

#include "triangulation/generic/facetpairing.h"

namespace regina {

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim2EdgePairing, you should use
 * the full class name FacetPairing<2>.
 */
REGINA_DEPRECATED typedef FacetPairing<2> Dim2EdgePairing;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4FacetPairing, you should use
 * the full class name FacetPairing<4>.
 */
REGINA_DEPRECATED typedef FacetPairing<4> Dim4FacetPairing;

} // namespace regina

#endif

