
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

/*! \file subcomplex/blockedsfstriple.h
 *  \brief Supports joined sequences of three Seifert fibred spaces that
 *  are triangulated using saturated blocks.
 */

#ifndef __BLOCKEDSFSTRIPLE_H
#ifndef __DOXYGEN
#define __BLOCKEDSFSTRIPLE_H
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
 * Represents a blocked sequence of three Seifert fibred spaces joined
 * along connecting tori.  This is a particular type of triangulation
 * of a graph manifold, formed from three saturated regions whose
 * various torus boundaries are identified as described below.
 * Optional layerings may be placed between torus boundaries to
 * allow for more interesting relationships between the
 * respective boundary curves of each region.  For more detail on
 * saturated regions and their constituent saturated blocks, see the
 * SatRegion class; for more detail on layerings, see the Layering class.
 *
 * The three saturated regions must be joined together as illustrated
 * below.  Each large box represents a saturated region, and the small
 * tunnels show where the region boundaries are joined (possibly via
 * layerings).
 *
 * <pre>
 *     /----------------\   /------------------\   /----------------\
 *     |                |   |                  |   |                |
 *     |  End region 0   ---   Central region   ---   End region 1  |
 *     |                 ---                    ---                 |
 *     |                |   |                  |   |                |
 *     \----------------/   \------------------/   \----------------/
 * </pre>
 *
 * Each of the end regions must have precisely one boundary component
 * formed from just one saturated annulus.  The central region may have
 * two boundary components formed from one saturated annulus each.
 * Alternatively, it may have one boundary formed from two saturated
 * annuli, where this boundary is pinched together so that each annulus
 * becomes a two-sided torus joined to one of the end regions.  None of
 * the boundary components (or the two-sided tori discussed above) may
 * be twisted (i.e., they must be tori, not Klein bottles).
 *
 * The ways in which the various region boundaries are identified are
 * specified by 2-by-2 matrices, which express curves representing the
 * fibres and base orbifold of each end region in terms of the central
 * region (see the page on \ref sfsnotation for terminology).
 *
 * Specifically, consider the matrix \a M that describes the joining of
 * the central region and the first end region (marked in the diagram
 * above as end region 0).  Suppose that \a f and \a o are directed
 * curves on the central region boundary and \a f0 and \a o0 are directed
 * curves on the first end region boundary, where \a f and \a f0 represent
 * the fibres of each region and \a o and \a o0 represent the base orbifolds.
 * Then the boundaries are joined according to the following relation:
 *
 * <pre>
 *     [f0]       [f ]
 *     [  ] = M * [  ]
 *     [o0]       [o ]
 * </pre>
 *
 * Likewise, let \a M' be the matrix describing how the central region
 * and the second end region (marked in the diagram as end region 1) are
 * joined.  Let \a f' and \a o' be directed curves on the other central
 * region boundary and \a f1 and \a o1 be directed curves on the second
 * end region boundary, where \a f' and \a f1 represent fibres and
 * \a o and \a o1 represent the base orbifolds.  Then the boundaries are
 * joined according to the relation:
 *
 * <pre>
 *     [f1]        [f']
 *     [  ] = M' * [  ]
 *     [o1]        [o']
 * </pre>
 *
 * If a layering is present between two regions, then the corresponding
 * boundary curves are not identified directly.  In this case, the relevant
 * matrix \a M or \a M' shows how the layering relates the curves on each
 * region boundary.
 *
 * Note that the routines writeName() and writeTeXName() do \e not
 * offer enough information to uniquely identify the triangulation,
 * since this essentially requires 2-dimensional assemblings of
 * saturated blocks.  For full details, writeTextLong() may be used instead.
 *
 * The optional StandardTriangulation routine manifold() is
 * implemented for this class, but homology() is not.
 */
class REGINA_API BlockedSFSTriple : public StandardTriangulation {
    private:
        SatRegion* end_[2];
            /**< The two end regions, i.e., the saturated regions with
                 just one boundary annulus. */
        SatRegion* centre_;
            /**< The central region, i.e., the saturated region with two
                 boundary annuli that meets both end regions. */
        Matrix2 matchingReln_[2];
            /**< Specifies how the various region boundaries are joined
                 (possibly via layerings), as described in the class notes
                 above.  In particular, \a matchingReln_[i] describes how
                 end region \a i is joined to the central region.  See the
                 class notes for further details. */

    public:
        /**
         * Destroys this structure and its constituent components.
         */
        ~BlockedSFSTriple();

        /**
         * Returns details of the requested end region, as described in
         * the class notes above.  The end regions are the two saturated
         * regions with one boundary annulus each, which are both joined
         * to the central region.
         *
         * @param which 0 if the first end region should be returned
         * (marked as end region 0 in the class notes), or 1 if the
         * second end region should be returned (marked as end region 1
         * in the class notes).
         * @return details of the requested end region.
         */
        const SatRegion& end(int which) const;

        /**
         * Returns details of the central saturated region, as described
         * in the class notes above.  This is the saturated region with
         * two boundary annuli, each of which is joined to one of the
         * end regions.
         *
         * @return details of the central region.
         */
        const SatRegion& centre() const;

        /**
         * Returns the matrix describing how the given end region is
         * joined to the central region.  Note that if a layering is
         * placed between the two respective region boundaries, then
         * any changes to the boundary relationships caused by the
         * layering are included in this matrix.
         *
         * See the class notes above for precise information on how each
         * matrix is presented.
         *
         * @param which 0 if the matrix returned should describe how the
         * central region is joined to the first end region (marked end
         * region 0 in the class notes), or 1 if the matrix returned
         * should describe how the central region is joined to the
         * second end region (marked end region 1 in the class notes).
         * @return the matrix describing how the requested region
         * boundaries are joined.
         */
        const Matrix2& matchingReln(int which) const;

        Manifold* manifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        /**
         * Determines if the given triangulation is a blocked sequence of
         * three Seifert fibred spaces, as described in the class notes
         * above.
         *
         * @param tri the triangulation to examine.
         * @return a newly created structure containing details of the
         * blocked triple, or \c null if the given triangulation is not of
         * this form.
         */
        static BlockedSFSTriple* isBlockedSFSTriple(Triangulation<3>* tri);

    private:
        /**
         * Constructs a new blocked sequence of three Seifert fibred spaces,
         * as described by the given saturated regions and matching relations.
         * The new object will take ownership of each of the regions passed.
         *
         * See the class notes above for details of terminology used here.
         *
         * Note that the new object must describe an existing triangulation.
         *
         * @param end0 the first end region.
         * @param centre the central region.
         * @param end1 the second end region.
         * @param matchingReln0 describes how the first end region is
         * joined to the central region.
         * @param matchingReln1 describes how the second end region is
         * joined to the central region.
         */
        BlockedSFSTriple(SatRegion* end0, SatRegion* centre,
            SatRegion* end1, const Matrix2& matchingReln0,
            const Matrix2& matchingReln1);
};

/*@}*/

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NBlockedSFSTriple has now been renamed to
 * BlockedSFSTriple.
 */
REGINA_DEPRECATED typedef BlockedSFSTriple NBlockedSFSTriple;

// Inline functions for BlockedSFSTriple

inline BlockedSFSTriple::BlockedSFSTriple(SatRegion* end0,
        SatRegion* centre, SatRegion* end1, const Matrix2& matchingReln0,
        const Matrix2& matchingReln1) {
    end_[0] = end0;
    centre_ = centre;
    end_[1] = end1;
    matchingReln_[0] = matchingReln0;
    matchingReln_[1] = matchingReln1;
}

inline const SatRegion& BlockedSFSTriple::end(int which) const {
    return *end_[which];
}

inline const SatRegion& BlockedSFSTriple::centre() const {
    return *centre_;
}

inline const Matrix2& BlockedSFSTriple::matchingReln(int which) const {
    return matchingReln_[which];
}

} // namespace regina

#endif

