
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file nlayering.h
 *  \brief Supports the analysis of layering tetrahedra upon a torus boundary.
 */

#ifndef __NLAYERING_H
#ifndef __DOXYGEN
#define __NLAYERING_H
#endif

#include "triangulation/nperm.h"
#include "utilities/boostutils.h"
#include "utilities/nmatrix2.h"

namespace regina {

class NTetrahedron;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a layered solid torus in a triangulation.
 * A layered solid torus must contain at least one tetrahedron.
 *
 * Note that this class \b only represents layered solid tori with a
 * (3,2,1) at their base.  Thus triangulations that begin with a
 * degenerate (2,1,1) mobius strip and layer over the mobius strip
 * boundary (including the minimal (1,1,0) triangulation) are not
 * described by this class.
 *
 * All optional NStandardTriangulation routines are implemented for this
 * class.
 */
class NLayering : public boost::noncopyable {
    private:
        unsigned long size;
            /**< The number of layered tetrahedra that have been added. */

        NTetrahedron* oldBdryTet[2];
        NPerm oldBdryRoles[2];

        NTetrahedron* newBdryTet[2];
        NPerm newBdryRoles[2];

        NMatrix2 reln;
            /**< Guarantee that determinant is 1. */

    public:
        NLayering(NTetrahedron* oldBdry0, NPerm oldRoles0,
            NTetrahedron* oldBdry1, NPerm oldRoles1);

        unsigned long getSize() const;

        NTetrahedron* getOldBoundaryTet(unsigned which) const;
        NPerm getOldBoundaryRoles(unsigned which) const;
        NTetrahedron* getNewBoundaryTet(unsigned which) const;
        NPerm getNewBoundaryRoles(unsigned which) const;

        bool extendOne();
        unsigned long extend();

        bool matchesTop(NTetrahedron* upperBdry0, NPerm upperRoles0,
            NTetrahedron* upperBdry1, NPerm upperRoles1,
            NMatrix2& upperReln) const;
};

/*@}*/

// Inline functions for NLayering

inline unsigned long NLayering::getSize() const {
    return size;
}

inline NTetrahedron* NLayering::getOldBoundaryTet(unsigned which) const {
    return oldBdryTet[which];
}

inline NPerm NLayering::getOldBoundaryRoles(unsigned which) const {
    return oldBdryRoles[which];
}

inline NTetrahedron* NLayering::getNewBoundaryTet(unsigned which) const {
    return newBdryTet[which];
}

inline NPerm NLayering::getNewBoundaryRoles(unsigned which) const {
    return newBdryRoles[which];
}

} // namespace regina

#endif

