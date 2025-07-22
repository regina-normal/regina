
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file subcomplex/blockedsfspair.h
 *  \brief Supports joined pairs of Seifert fibred spaces that are
 *  triangulated using saturated blocks.
 */

#ifndef __REGINA_BLOCKEDSFSPAIR_H
#ifndef __DOXYGEN
#define __REGINA_BLOCKEDSFSPAIR_H
#endif

#include "regina-core.h"
#include "maths/matrix2.h"
#include "subcomplex/satregion.h"
#include "subcomplex/standardtri.h"

namespace regina {

class SatRegion;

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
class BlockedSFSPair : public StandardTriangulation {
    private:
        SatRegion region_[2];
            /**< The two saturated regions whose boundaries are joined. */
        Matrix2 matchingReln_;
            /**< Specifies how the two region boundaries are joined, as
                 described in the class notes above. */

    public:
        /**
         * Creates a new copy of the given structure.
         * This will induce a deep copy of \a src.
         *
         * \param src the structure to copy.
         */
        BlockedSFSPair(const BlockedSFSPair& src) = default;
        /**
         * Moves the contents of the given structure into this new structure.
         * This is a constant time operation.
         *
         * The structure that was passed (\a src) will no longer be usable.
         *
         * \param src the structure to move from.
         */
        BlockedSFSPair(BlockedSFSPair&& src) noexcept = default;
        /**
         * Sets this to be a copy of the given structure.
         * This will induce a deep copy of \a src.
         *
         * \param src the structure to copy.
         * \return a reference to this structure.
         */
        BlockedSFSPair& operator = (const BlockedSFSPair& src) = default;
        /**
         * Moves the contents of the given structure into this structure.
         * This is a constant time operation.
         *
         * The structure that was passed (\a src) will no longer be usable.
         *
         * \param src the structure to move from.
         * \return a reference to this structure.
         */
        BlockedSFSPair& operator = (BlockedSFSPair&& src) noexcept = default;
        /**
         * Swaps the contents of this and the given structure.
         *
         * \param other the structure whose contents should be swapped
         * with this.
         */
        void swap(BlockedSFSPair& other) noexcept;

        /**
         * Returns details of one of the two bounded saturated regions that
         * form this triangulation.  See the class notes above for further
         * information.
         *
         * \param which 0 if the first region should be returned, or
         * 1 if the second region should be returned.
         * \return details of the requested saturated region.
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
         * \return the matrix describing how the region boundaries are
         * joined.
         */
        const Matrix2& matchingReln() const;

        /**
         * Determines whether this and the given structure represent
         * the same type of blocked pair of Seifert fibred spaces.
         *
         * Specifically, two structures will compare as equal if and only if
         * both structures are formed from the same pair of combinatorial
         * presentations of saturated regions (as returned by the SatRegion
         * comparison operators), presented in the same order, and with their
         * torus boundaries joined using the same 2-by-2 matrix.
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
         * represent the same type of blocked pair of Seifert fibred spaces.
         */
        bool operator == (const BlockedSFSPair& other) const;

        std::unique_ptr<Manifold> manifold() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

        /**
         * Determines if the given triangulation is a blocked pair of
         * Seifert fibred spaces, as described by this class.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * \param tri the triangulation to examine.
         * \return a structure containing details of the blocked pair, or
         * \c null if the given triangulation is not of this form.
         */
        static std::unique_ptr<BlockedSFSPair> recognise(
            const Triangulation<3>& tri);

    private:
        /**
         * Constructs a new blocked pair of Seifert fibred spaces, as
         * described by the given saturated regions and matching relation.
         *
         * Note that the new object must describe an existing triangulation.
         *
         * \param region0 the first saturated region.
         * \param region1 the second saturated region.
         * \param matchingReln describes how the first and second region
         * boundaries are joined, as detailed in the class notes above.
         */
        BlockedSFSPair(SatRegion&& region0, SatRegion&& region1,
            const Matrix2& matchingReln);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls BlockedSFSPair::swap(); it is provided
 * so that BlockedSFSPair meets the C++ Swappable requirements.
 *
 * \param a the first structure whose contents should be swapped.
 * \param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(BlockedSFSPair& a, BlockedSFSPair& b) noexcept;

// Inline functions for BlockedSFSPair

inline BlockedSFSPair::BlockedSFSPair(SatRegion&& region0,
        SatRegion&& region1, const Matrix2& matchingReln) :
        region_ { std::move(region0), std::move(region1) },
        matchingReln_(matchingReln) {
}

inline void BlockedSFSPair::swap(BlockedSFSPair& other) noexcept {
    region_[0].swap(other.region_[0]);
    region_[1].swap(other.region_[1]);
    matchingReln_.swap(other.matchingReln_);
}

inline const SatRegion& BlockedSFSPair::region(int which) const {
    return region_[which];
}

inline const Matrix2& BlockedSFSPair::matchingReln() const {
    return matchingReln_;
}

inline bool BlockedSFSPair::operator == (const BlockedSFSPair& other) const {
    return region_[0] == other.region_[0] && region_[1] == other.region_[1] &&
        matchingReln_ == other.matchingReln_;
}

inline void swap(BlockedSFSPair& a, BlockedSFSPair& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

