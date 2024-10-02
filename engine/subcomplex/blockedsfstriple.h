
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#ifndef __REGINA_BLOCKEDSFSTRIPLE_H
#ifndef __DOXYGEN
#define __REGINA_BLOCKEDSFSTRIPLE_H
#endif

#include "regina-core.h"
#include "maths/matrix2.h"
#include "subcomplex/satregion.h"
#include "subcomplex/standardtri.h"

namespace regina {

class SatRegion;

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
 * Note that the routines writeName() and writeTeXName() do _not_
 * offer enough information to uniquely identify the triangulation,
 * since this essentially requires 2-dimensional assemblings of
 * saturated blocks.  For full details, writeTextLong() may be used instead.
 *
 * The optional StandardTriangulation routine manifold() is
 * implemented for this class, but homology() is not.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.  Note, however, that
 * the only way to create objects of this class (aside from copying or moving)
 * is via the static member function recognise().
 *
 * \ingroup subcomplex
 */
class BlockedSFSTriple : public StandardTriangulation {
    private:
        SatRegion end_[2];
            /**< The two end regions, i.e., the saturated regions with
                 just one boundary annulus. */
        SatRegion centre_;
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
         * Creates a new copy of the given structure.
         * This will induce a deep copy of \a src.
         *
         * \param src the structure to copy.
         */
        BlockedSFSTriple(const BlockedSFSTriple& src) = default;
        /**
         * Moves the contents of the given structure into this new structure.
         * This is a constant time operation.
         *
         * The structure that was passed (\a src) will no longer be usable.
         *
         * \param src the structure to move from.
         */
        BlockedSFSTriple(BlockedSFSTriple&& src) noexcept = default;
        /**
         * Sets this to be a copy of the given structure.
         * This will induce a deep copy of \a src.
         *
         * \param src the structure to copy.
         * \return a reference to this structure.
         */
        BlockedSFSTriple& operator = (const BlockedSFSTriple& src) = default;
        /**
         * Moves the contents of the given structure into this structure.
         * This is a constant time operation.
         *
         * The structure that was passed (\a src) will no longer be usable.
         *
         * \param src the structure to move from.
         * \return a reference to this structure.
         */
        BlockedSFSTriple& operator = (BlockedSFSTriple&& src) noexcept =
            default;
        /**
         * Swaps the contents of this and the given structure.
         *
         * \param other the structure whose contents should be swapped
         * with this.
         */
        void swap(BlockedSFSTriple& other) noexcept;

        /**
         * Returns details of the requested end region, as described in
         * the class notes above.  The end regions are the two saturated
         * regions with one boundary annulus each, which are both joined
         * to the central region.
         *
         * \param which 0 if the first end region should be returned
         * (marked as end region 0 in the class notes), or 1 if the
         * second end region should be returned (marked as end region 1
         * in the class notes).
         * \return details of the requested end region.
         */
        const SatRegion& end(int which) const;

        /**
         * Returns details of the central saturated region, as described
         * in the class notes above.  This is the saturated region with
         * two boundary annuli, each of which is joined to one of the
         * end regions.
         *
         * \return details of the central region.
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
         * \param which 0 if the matrix returned should describe how the
         * central region is joined to the first end region (marked end
         * region 0 in the class notes), or 1 if the matrix returned
         * should describe how the central region is joined to the
         * second end region (marked end region 1 in the class notes).
         * \return the matrix describing how the requested region
         * boundaries are joined.
         */
        const Matrix2& matchingReln(int which) const;

        /**
         * Determines whether this and the given structure represent
         * the same type of blocked sequence of three Seifert fibred spaces.
         *
         * Specifically, two structures will compare as equal if and only if
         * both structures are formed from the same triple of combinatorial
         * presentations of saturated regions (as returned by the SatRegion
         * comparison operators), presented in the same order, and with their
         * torus boundaries joined using the same pair of 2-by-2 matrices.
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass is more specific than combinatorial isomorphism, since
         * this test does not account for the many symmetries in a
         * blocked Seifert fibred space).
         *
         * \param other the structure with which this will be compared.
         * \return \c true if and only if this and the given structure
         * represent the same type of blocked sequence of three Seifert
         * fibred spaces.
         */
        bool operator == (const BlockedSFSTriple& other) const;

        std::unique_ptr<Manifold> manifold() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

        /**
         * Determines if the given triangulation is a blocked sequence of
         * three Seifert fibred spaces, as described in the class notes
         * above.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * \param tri the triangulation to examine.
         * \return a structure containing details of the blocked triple, or
         * \c null if the given triangulation is not of this form.
         */
        static std::unique_ptr<BlockedSFSTriple> recognise(
            const Triangulation<3>& tri);

    private:
        /**
         * Constructs a new blocked sequence of three Seifert fibred spaces,
         * as described by the given saturated regions and matching relations.
         *
         * See the class notes above for details of terminology used here.
         *
         * Note that the new object must describe an existing triangulation.
         *
         * \param end0 the first end region.
         * \param centre the central region.
         * \param end1 the second end region.
         * \param matchingReln0 describes how the first end region is
         * joined to the central region.
         * \param matchingReln1 describes how the second end region is
         * joined to the central region.
         */
        BlockedSFSTriple(SatRegion&& end0, SatRegion&& centre,
            SatRegion&& end1, const Matrix2& matchingReln0,
            const Matrix2& matchingReln1);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls BlockedSFSTriple::swap(); it is provided
 * so that BlockedSFSTriple meets the C++ Swappable requirements.
 *
 * \param a the first structure whose contents should be swapped.
 * \param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(BlockedSFSTriple& a, BlockedSFSTriple& b) noexcept;

// Inline functions for BlockedSFSTriple

inline BlockedSFSTriple::BlockedSFSTriple(SatRegion&& end0,
        SatRegion&& centre, SatRegion&& end1, const Matrix2& matchingReln0,
        const Matrix2& matchingReln1) :
    end_ { std::move(end0), std::move(end1) },
    centre_(std::move(centre)),
    matchingReln_ { matchingReln0, matchingReln1 } {
}

inline void BlockedSFSTriple::swap(BlockedSFSTriple& other) noexcept {
    end_[0].swap(other.end_[0]);
    end_[1].swap(other.end_[1]);
    centre_.swap(other.centre_);
    matchingReln_[0].swap(other.matchingReln_[0]);
    matchingReln_[1].swap(other.matchingReln_[1]);
}

inline const SatRegion& BlockedSFSTriple::end(int which) const {
    return end_[which];
}

inline const SatRegion& BlockedSFSTriple::centre() const {
    return centre_;
}

inline const Matrix2& BlockedSFSTriple::matchingReln(int which) const {
    return matchingReln_[which];
}

inline bool BlockedSFSTriple::operator == (const BlockedSFSTriple& other)
        const {
    return end_[0] == other.end_[0] && end_[1] == other.end_[1] &&
        centre_ == other.centre_ &&
        matchingReln_[0] == other.matchingReln_[0] &&
        matchingReln_[1] == other.matchingReln_[1];
}

inline void swap(BlockedSFSTriple& a, BlockedSFSTriple& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

