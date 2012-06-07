
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

/*! \file census/dim2edgepairing.h
 *  \brief Deals with pairing off edges of triangles in a census of
 *  2-manifold triangulations.
 */

#ifndef __DIM2EDGEPAIRING_H
#ifndef __DOXYGEN
#define __DIM2EDGEPAIRING_H
#endif

#include "regina-core.h"
#include "census/ngenericfacetpairing.h"
#include "dim2/dim2isomorphism.h"

namespace regina {

/**
 * \weakgroup census
 * @{
 */

/**
 * Represents a specific pairwise matching of edges of triangles.
 * Given a fixed number of triangles, each triangle edge is either
 * paired with some other triangle edge (which is in turn paired with
 * it) or remains unmatched.  A triangle edge cannot be paired with itself.
 *
 * Such a matching models part of the structure of a 2-manifold triangulation,
 * in which each triangle edge is either glued to some other triangle edge
 * facet (which is in turn glued to it) or is an unglued boundary edge.
 *
 * Note that if this pairing is used to construct an actual 2-manifold
 * triangulation, the individual gluing orientations will still need to
 * be specified; they are not a part of this structure.
 *
 * \testpart
 */
class REGINA_API Dim2EdgePairing : public NGenericFacetPairing<2> {
    public:
        /**
         * Creates a new edge pairing that is a clone of the given edge
         * pairing.
         *
         * @param cloneMe the facet pairing to clone.
         */
        Dim2EdgePairing(const Dim2EdgePairing& cloneMe);

        /**
         * Creates the edge pairing of the given 2-manifold triangulation.
         * This is the edge pairing that describes how the triangle edges of
         * the given 2-manifold triangulation are joined together, as
         * described in the class notes.
         *
         * \pre The given triangulation is not empty.
         *
         * @param tri the 2-manifold triangulation whose edge pairing should
         * be constructed.
         */
        Dim2EdgePairing(const Dim2Triangulation& tri);

    private:
        /**
         * Creates a new edge pairing.  All internal arrays will be
         * allocated but not initialised.
         *
         * \pre \a nTriangles is at least 1.
         *
         * @param nTriangles the number of triangles under
         * consideration in this new edge pairing.
         */
        Dim2EdgePairing(unsigned nTriangles);

    // Make sure the parent class can call the private constructor.
    friend class NGenericFacetPairing<2>;
};

/*@}*/

// Inline functions for Dim2EdgePairing

inline Dim2EdgePairing::Dim2EdgePairing(const Dim2EdgePairing& cloneMe) :
        NGenericFacetPairing<2>(cloneMe) {
}

inline Dim2EdgePairing::Dim2EdgePairing(unsigned nTriangles) :
        NGenericFacetPairing<2>(nTriangles) {
}

inline Dim2EdgePairing::Dim2EdgePairing(const Dim2Triangulation& tri) :
        NGenericFacetPairing<2>(tri) {
}

} // namespace regina

#endif

