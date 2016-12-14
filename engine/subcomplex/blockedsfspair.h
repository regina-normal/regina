
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

/*! \file subcomplex/blockedsfspair.h
 *  \brief Supports joined pairs of Seifert fibred spaces that are
 *  triangulated using saturated blocks.
 */

#ifndef __BLOCKEDSFSPAIR_H
#ifndef __DOXYGEN
#define __BLOCKEDSFSPAIR_H
#endif

#include "regina-core.h"
#include "maths/matrix2.h"
#include "subcomplex/standardtri.h"

namespace regina {

class SatRegion;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a blocked pair of Seifert fibred spaces joined along a single
 * connecting torus.  This is a particular type of triangulation of a graph
 * manifold, formed from two saturated regions whose torus boundaries are
 * identified.  An optional layering may be placed between the two torus
 * boundaries to allow for a more interesting relationship between the boundary
 * curves of each region.  For more detail on saturated regions and their
 * constituent saturated blocks, see the SatRegion class; for more detail
 * on layerings, see the Layering class.
 *
 * Each of the two saturated regions must have precisely one boundary
 * component formed from just one saturated annulus, and this boundary may
 * not be twisted (i.e., it must be a torus, not a Klein bottle).  The
 * way in which the boundaries from each region are identified is
 * specified by a 2-by-2 matrix \a M, which expresses curves
 * representing the fibres and base orbifold of the second region in
 * terms of the first (see the page on \ref sfsnotation for terminology).
 *
 * More specifically, suppose that \a f0 and \a o0 are directed curves on
 * the first region boundary and \a f1 and \a o1 are directed curves on the
 * second region boundary, where \a f0 and \a f1 represent the fibres of
 * each region and \a o0 and \a o1 represent the base orbifolds.  Then
 * the boundaries are joined according to the following relation:
 *
 * <pre>
 *     [f1]       [f0]
 *     [  ] = M * [  ]
 *     [o1]       [o0]
 * </pre>
 *
 * If a layering is present between the two boundaries, then the
 * boundary curves are not identified directly.  In this case, the matrix
 * \a M shows how the layering relates the curves on each region boundary.
 *
 * Note that the routines writeName() and writeTeXName() do \e not
 * offer enough information to uniquely identify the triangulation,
 * since this essentially requires 2-dimensional assemblings of
 * saturated blocks.  For full details, writeTextLong() may be used instead.
 *
 * The optional StandardTriangulation routine manifold() is
 * implemented for this class, but homology() is not.
 */
class REGINA_API BlockedSFSPair : public StandardTriangulation {
    private:
        SatRegion* region_[2];
            /**< The two saturated regions whose boundaries are joined. */
        Matrix2 matchingReln_;
            /**< Specifies how the two region boundaries are joined, as
                 described in the class notes above. */

    public:
        /**
         * Destroys this structure and its constituent components.
         */
        ~BlockedSFSPair();

        /**
         * Returns details of one of the two bounded saturated regions that
         * form this triangulation.  See the class notes above for further
         * information.
         *
         * @param which 0 if the first region should be returned, or
         * 1 if the second region should be returned.
         * @return details of the requested saturated region.
         */
        const SatRegion& region(int which) const;

        /**
         * Returns the matrix describing how the two saturated region
         * boundaries are joined.  Note that if a layering is placed
         * between the two region boundaries, then any changes to the
         * boundary relationships caused by the layering are included
         * in this matrix.
         *
         * See the class notes above for precise information on how this
         * matrix is presented.
         *
         * @return the matrix describing how the region boundaries are
         * joined.
         */
        const Matrix2& matchingReln() const;

        Manifold* manifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        /**
         * Determines if the given triangulation is a blocked pair of
         * Seifert fibred spaces, as described by this class.
         *
         * @param tri the triangulation to examine.
         * @return a newly created structure containing details of the
         * blocked pair, or \c null if the given triangulation is not of
         * this form.
         */
        static BlockedSFSPair* isBlockedSFSPair(Triangulation<3>* tri);

    private:
        /**
         * Constructs a new blocked pair of Seifert fibred spaces, as
         * described by the given saturated regions and matching
         * relation.  The new object will take ownership of each of the
         * regions passed.
         *
         * Note that the new object must describe an existing triangulation.
         *
         * @param region0 the first saturated region.
         * @param region1 the second saturated region.
         * @param matchingReln describes how the first and second region
         * boundaries are joined, as detailed in the class notes above.
         */
        BlockedSFSPair(SatRegion* region0, SatRegion* region1,
            const Matrix2& matchingReln);
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NBlockedSFSPair has now been renamed to
 * BlockedSFSPair.
 */
REGINA_DEPRECATED typedef BlockedSFSPair NBlockedSFSPair;

/*@}*/

// Inline functions for BlockedSFSPair

inline BlockedSFSPair::BlockedSFSPair(SatRegion* region0,
        SatRegion* region1, const Matrix2& matchingReln) :
        matchingReln_(matchingReln) {
    region_[0] = region0;
    region_[1] = region1;
}

inline const SatRegion& BlockedSFSPair::region(int which) const {
    return *region_[which];
}

inline const Matrix2& BlockedSFSPair::matchingReln() const {
    return matchingReln_;
}

} // namespace regina

#endif

