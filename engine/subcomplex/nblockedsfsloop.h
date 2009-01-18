
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file nblockedsfsloop.h
 *  \brief Supports self-identified Seifert fibred spaces that are
 *  triangulated using saturated blocks.
 */

#ifndef __NBLOCKEDSFSLOOP_H
#ifndef __DOXYGEN
#define __NBLOCKEDSFSLOOP_H
#endif

#include "subcomplex/nstandardtri.h"
#include "utilities/nmatrix2.h"

namespace regina {

class NSatRegion;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a blocked Seifert fibred space with two boundary tori that
 * are joined together.  This is a particular type of triangulation of a
 * graph manifold, formed from a single saturated region whose two torus
 * boundaries are identified.  An optional layering may be placed
 * between the two torus boundaries to allow for a more interesting
 * relationship between the two sets of boundary curves.  For more
 * detail on saturated regions and their constituent saturated blocks,
 * see the NSatRegion class; for more detail on layerings, see the
 * NLayering class.
 *
 * The saturated region may have two boundary components formed from one
 * saturated annulus each.  Alternatively, it may have one boundary
 * formed from two saturated annuli, where this boundary is pinched
 * together so that each annulus becomes a two-sided torus (both of which
 * are later joined together).  None of the boundary components (or the
 * two-sided tori discussed above) may be twisted (i.e., they must be
 * tori, not Klein bottles).
 *
 * The way in which the two torus boundaries are identified is specified
 * by a 2-by-2 matrix, which expresses curves representing the fibres
 * and base orbifold on the second boundary in terms of such curves on
 * the first boundary (see the page on \ref sfsnotation for terminology).
 *
 * More specifically, suppose that \a f0 and \a o0 are directed curves
 * on the first boundary torus and \a f1 and \a o1 are directed curves
 * on the second boundary torus, where \a f0 and \a f1 represent the
 * fibres of the region and \a o0 and \a o1 represent the base orbifold.
 * Then the boundaries are joined according to the following relation:
 *
 * <pre>
 *     [f1]       [f0]
 *     [  ] = M * [  ]
 *     [o1]       [o0]
 * </pre>
 *
 * If a layering is present between the two torus boundaries, then the
 * corresponding boundary curves are not identified directly.  In this
 * case, the matrix \a M shows how the layering relates the curves on
 * each boundary.
 *
 * Note that the routines writeName() and writeTeXName() do \e not
 * offer enough information to uniquely identify the triangulation,
 * since this essentially requires 2-dimensional assemblings of
 * saturated blocks.  For full details, writeTextLong() may be used instead.
 *
 * The optional NStandardTriangulation routine getManifold() is
 * implemented for this class, but getHomologyH1() is not.
 */
class NBlockedSFSLoop : public NStandardTriangulation {
    private:
        NSatRegion* region_;
            /**< The saturated region whose two torus boundaries are joined. */
        NMatrix2 matchingReln_;
            /**< Specifies how the two boundary tori are joined, as described
                 in the class notes above. */

    public:
        /**
         * Destroys this structure and its constituent components.
         */
        ~NBlockedSFSLoop();

        /**
         * Returns details of the saturated region from which this
         * triangulation is formed.  See the class notes above for
         * further information.
         *
         * @return details of the saturated region.
         */
        const NSatRegion& region() const;
        /**
         * Returns the matrix describing how the two torus boundaries of
         * the saturated region are joined.  Note that if a layering is
         * placed between the two boundary tori, then any changes to the
         * boundary relationships caused by the layering are included in
         * this matrix.
         *
         * See the class notes above for precise information on how this
         * matrix is presented.
         *
         * @return the matrix describing how the boundaries of the
         * region are joined.
         */
        const NMatrix2& matchingReln() const;

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        /**
         * Determines if the given triangulation is a blocked Seifert
         * fibred space with identified boundaries, as described by this
         * class.
         *
         * @param tri the triangulation to examine.
         * @return a newly created structure containing details of the
         * blocked self-identified Seifert fibred space, or \c null if
         * the given triangulation is not of this form.
         */
        static NBlockedSFSLoop* isBlockedSFSLoop(NTriangulation* tri);

    private:
        /**
         * Constructs a new blocked self-identified Seifert fibred
         * space, as described by the given saturated region and
         * matching relation.  The new object will take ownership of the
         * given region.
         *
         * Note that the new object must describe an existing triangulation.
         *
         * @param region the saturated region from which the structure
         * is formed.
         * @param matchingReln describes how the two boundaries of the
         * region are joined, as described in the class notes above.
         */
        NBlockedSFSLoop(NSatRegion* region, const NMatrix2& matchingReln);
};

/*@}*/

// Inline functions for NBlockedSFSLoop

inline NBlockedSFSLoop::NBlockedSFSLoop(NSatRegion* region,
        const NMatrix2& matchingReln) :
        region_(region), matchingReln_(matchingReln) {
}

inline const NSatRegion& NBlockedSFSLoop::region() const {
    return *region_;
}

inline const NMatrix2& NBlockedSFSLoop::matchingReln() const {
    return matchingReln_;
}

} // namespace regina

#endif

