
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file nngblockedsfstriple.h
 *  \brief Deals with non-geometric triangulations formed from three
 *  saturated regions joined along boundary tori.
 */

#ifndef __NNGBLOCKEDSFSTRIPLE_H
#ifndef __DOXYGEN
#define __NNGBLOCKEDSFSTRIPLE_H
#endif

#include "subcomplex/nstandardtri.h"

namespace regina {

class NSatRegion;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * TODO: Fix all documentation for this class!
 *
 * Represents a blocked non-geometric pair of Seifert fibred spaces.
 * This is a particular type of triangulation, formed from two saturated
 * regions whose torus boundaries are identified.  An optional layering
 * may be placed between the two torus boundaries to allow a more
 * interesting relationship between the boundary curves of each region.
 * For more detail on saturated regions and their constituent saturated
 * blocks, see the NSatRegion class; for more detail on layerings, see
 * the NLayering class.
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
 * The optional NStandardTriangulation routine getManifold() is
 * implemented for this class, but getHomologyH1() is not.
 */
class NNGBlockedSFSTriple : public NStandardTriangulation {
    private:
        NSatRegion* end_[2];
            /**< The two saturated regions whose boundaries are joined. */
        NSatRegion* centre_;
        NMatrix2 matchingReln_[2];
            /**< Specifies how the two region boundaries are joined, as
                 described in the class notes above. */

    public:
        /**
         * Destroys this structure and its constituent components.
         */
        ~NNGBlockedSFSTriple();

        NManifold* getManifold() const;
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
        static NNGBlockedSFSTriple* isNGBlockedSFSTriple(NTriangulation* tri);

    private:
        /**
         * Constructs a new blocked pair of Seifert fibred spaces, as
         * described by the given saturated regions and matching
         * relation.  The new object will take ownership of each of the
         * regions passed.
         *
         * @param region0 the first saturated region.
         * @param region1 the second saturated region.
         * @param matchingReln describes how the first and second region
         * boundaries are joined, as detailed in the class notes above.
         */
        NNGBlockedSFSTriple(NSatRegion* end0, NSatRegion* centre,
            NSatRegion* end1, const NMatrix2& matchingReln0,
            const NMatrix2& matchingReln1);
};

/*@}*/

// Inline functions for NNGBlockedSFSTriple

inline NNGBlockedSFSTriple::NNGBlockedSFSTriple(NSatRegion* end0,
        NSatRegion* centre, NSatRegion* end1, const NMatrix2& matchingReln0,
        const NMatrix2& matchingReln1) {
    end_[0] = end0;
    centre_ = centre;
    end_[1] = end1;
    matchingReln_[0] = matchingReln0;
    matchingReln_[1] = matchingReln1;
}

} // namespace regina

#endif

