
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

/*! \file census/dim4facetpairing.h
 *  \brief Deals with pairing off pentachoron facets in a census of
 *  4-manifold triangulations.
 */

#ifndef __DIM4FACETPAIRING_H
#ifndef __DOXYGEN
#define __DIM4FACETPAIRING_H
#endif

#include "regina-core.h"
#include "census/ngenericfacetpairing.h"
#include "dim4/dim4isomorphism.h"

namespace regina {

template <int> class Triangulation;
typedef Triangulation<4> Dim4Triangulation;

/**
 * \weakgroup census
 * @{
 */

/**
 * Represents a specific pairwise matching of pentachoron facets.
 * Given a fixed number of pentachora, each pentachoron facet is either
 * paired with some other pentachoron facet (which is in turn paired with
 * it) or remains unmatched.  A pentachoron facet cannot be paired with
 * itself.
 *
 * Such a matching models part of the structure of a 4-manifold triangulation,
 * in which each pentachoron facet is either glued to some other pentachoron
 * facet (which is in turn glued to it) or is an unglued boundary facet.
 *
 * Note that if this pairing is used to construct an actual 4-manifold
 * triangulation, the individual gluing permutations will still need to
 * be specified; they are not a part of this structure.
 */
class REGINA_API Dim4FacetPairing : public NGenericFacetPairing<4> {
    public:
        /**
         * Creates a new facet pairing that is a clone of the given facet
         * pairing.
         *
         * @param cloneMe the facet pairing to clone.
         */
        Dim4FacetPairing(const Dim4FacetPairing& cloneMe);

        /**
         * Creates the facet pairing of the given 4-manifold triangulation.
         * This is the facet pairing that describes how the pentachoron facets
         * of the given 4-manifold triangulation are joined together, as
         * described in the class notes.
         *
         * \pre The given triangulation is not empty.
         *
         * @param tri the 4-manifold triangulation whose facet pairing should
         * be constructed.
         */
        Dim4FacetPairing(const Dim4Triangulation& tri);

    private:
        /**
         * Creates a new facet pairing.  All internal arrays will be
         * allocated but not initialised.
         *
         * \pre \a nPentachora is at least 1.
         *
         * @param nPentachora the number of pentachora under
         * consideration in this new facet pairing.
         */
        Dim4FacetPairing(unsigned nPentachora);

    // Make sure the parent class can call the private constructor.
    friend class NGenericFacetPairing<4>;
};

/*@}*/

// Inline functions for Dim4FacetPairing

inline Dim4FacetPairing::Dim4FacetPairing(const Dim4FacetPairing& cloneMe) :
        NGenericFacetPairing<4>(cloneMe) {
}

inline Dim4FacetPairing::Dim4FacetPairing(unsigned nPentachora) :
        NGenericFacetPairing<4>(nPentachora) {
}

inline Dim4FacetPairing::Dim4FacetPairing(const Dim4Triangulation& tri) :
        NGenericFacetPairing<4>(tri) {
}

} // namespace regina

#endif

