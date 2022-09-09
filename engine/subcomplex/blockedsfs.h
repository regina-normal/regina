
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file subcomplex/blockedsfs.h
 *  \brief Supports Seifert fibred spaces that are triangulated using
 *  saturated blocks.
 */

#ifndef __REGINA_BLOCKEDSFS_H
#ifndef __DOXYGEN
#define __REGINA_BLOCKEDSFS_H
#endif

#include "regina-core.h"
#include "subcomplex/satregion.h"
#include "subcomplex/standardtri.h"

namespace regina {

class SatBlock;
class SatRegion;

/**
 * Represents a blocked Seifert fibred space (possibly with boundary).
 * This is a particular type of triangulation of a Seifert fibred space,
 * where this triangulation is formed from a single saturated region.
 * A saturated region is in turn formed from saturated blocks by identifying
 * their boundary annuli in pairs; see the SatRegion class for details.
 *
 * Note that the routines writeName() and writeTeXName() do \e not
 * offer enough information to uniquely identify the triangulation,
 * since this essentially requires a 2-dimensional assembling of
 * saturated blocks.  For full detail, writeTextLong() may be used instead.
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
class BlockedSFS : public StandardTriangulation {
    private:
        SatRegion region_;
            /**< The single saturated region that describes this entire
                 triangulation. */

    public:
        /**
         * Creates a new copy of this structure.
         * This will induce a deep copy of \a src.
         *
         * @param src the structure to copy.
         */
        BlockedSFS(const BlockedSFS& src) = default;
        /**
         * Moves the contents of the given structure into this new structure.
         * This is a constant time operation.
         *
         * The structure that was passed (\a src) will no longer be usable.
         *
         * @param src the structure to move from.
         */
        BlockedSFS(BlockedSFS&& src) noexcept = default;
        /**
         * Sets this to be a copy of the given structure.
         * This will induce a deep copy of \a src.
         *
         * @param src the structure to copy.
         * @return a reference to this structure.
         */
        BlockedSFS& operator = (const BlockedSFS& src) = default;
        /**
         * Moves the contents of the given structure into this structure.
         * This is a constant time operation.
         *
         * The structure that was passed (\a src) will no longer be usable.
         *
         * @param src the structure to move from.
         * @return a reference to this structure.
         */
        BlockedSFS& operator = (BlockedSFS&& src) noexcept = default;
        /**
         * Swaps the contents of this and the given structure.
         *
         * @param other the structure whose contents should be swapped
         * with this.
         */
        void swap(BlockedSFS& other) noexcept;

        /**
         * Returns details of the single saturated region that
         * fills this triangulation.
         *
         * @return the single saturated region.
         */
        const SatRegion& region() const;

        /**
         * Determines whether this and the given structure represent
         * the same type of blocked Seifert fibred space.
         *
         * Specifically, two structures will compare as equal if and only if
         * both structures are formed from the same combinatorial presentation
         * of a saturated region (as returned by the SatRegion
         * comparison operators).
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass is more specific than combinatorial isomorphism, since
         * this test does not account for the many symmetries in a
         * blocked Seifert fibred space).
         *
         * @param other the structure with which this will be compared.
         * @return \c true if and only if this and the given structure
         * represent the same type of blocked Seifert fibred space.
         */
        bool operator == (const BlockedSFS& other) const;

        /**
         * Determines whether this and the given structure do not represent
         * the same type of blocked Seifert fibred space.
         *
         * Specifically, two structures will compare as equal if and only if
         * both structures are formed from the same combinatorial presentation
         * of a saturated region (as returned by the SatRegion
         * comparison operators).
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass is more specific than combinatorial isomorphism, since
         * this test does not account for the many symmetries in a
         * blocked Seifert fibred space).
         *
         * @param other the structure with which this will be compared.
         * @return \c true if and only if this and the given structure
         * do not represent the same type of blocked Seifert fibred space.
         */
        bool operator != (const BlockedSFS& other) const;

        /**
         * Determines whether this triangulation is a plugged thin
         * I-bundle or a plugged thick I-bundle.  These structures are
         * described in "Structures of small closed non-orientable
         * 3-manifold triangulations", Benjamin A. Burton,
         * J. Knot Theory Ramifications 16 (2007), 545--574.
         *
         * @return the name of the plugged thin/thick I-bundle, if the
         * triangulation is of this form, or no value if it is not.
         */
        std::optional<std::string> isPluggedIBundle() const;

        std::unique_ptr<Manifold> manifold() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

        /**
         * Determines if the given triangulation is a blocked Seifert
         * fibred space.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * @param tri the triangulation to examine.
         * @return a structure containing details of the blocked Seifert
         * fibred space, or \c null if the given triangulation is not a
         * blocked Seifert fibred space.
         */
        static std::unique_ptr<BlockedSFS> recognise(
            const Triangulation<3>& tri);

    private:
        /**
         * Constructs a new blocked Seifert fibred space, as described by
         * the given saturated region.
         *
         * Note that the new object must describe an existing triangulation.
         *
         * @param region the region describing this entire triangulation.
         */
        BlockedSFS(SatRegion&& region);

        /**
         * Attempts to identify the solid torus plugs in a plugged thin
         * I-bundle or a plugged thick I-bundle.  This routine is
         * internal to isPluggedIBundle().
         *
         * It is assumed that the plugged thin/thick I-bundle has been
         * completely identified, with the exception of the two solid
         * torus plugs.  Corresponding parameters describing the
         * core I-bundle must be passed, along with two blocks that
         * should correspond to the two plugs.
         *
         * If the two blocks are indeed solid torus plugs (either layered
         * solid tori or Mobius bands), the full name of the plugged
         * thin/thick I-bundle will be returned.  Note that this name may be
         * normalised or otherwise modified to return a simpler set of
         * parameters for the same triangulation.  If either block is not a
         * solid torus plug then no value will be returned.
         *
         * @param thin \c true if the overall structure being identified
         * is a plugged thin I-bundle, or \c false if it is a plugged
         * thick I-bundle.
         * @param id identifies the particular thin/thick twisted
         * I-bundle into which the solid tori are plugged.  This must be
         * 1, 2, 3 or 4, to distinguish between the four thin twisted
         * I-bundles or the four thick twisted I-bundles described in
         * the paper "Structures of small closed non-orientable
         * 3-manifold triangulations" (see isPluggedIBundle for details).
         * @param torus0 the block that should provide the solid torus plug
         * corresponding to the first pair of integers in the plugged
         * thin/thick I-bundle parameters.
         * @param horiz0 \c true if the first pair of integers in the
         * plugged thin/thick I-bundle parameters should measure the
         * number of times the meridinal curve cuts the vertical and
         * horizontal edges of the adjacent block (not the block
         * \a torus0, but its neighbour), or \c false if the vertical
         * and diagonal edges should be used instead.
         * @param torus1 the block that should provide the solid torus plug
         * corresponding to the second pair of integers in the plugged
         * thin/thick I-bundle parameters.
         * @param horiz1 \c true if the second pair of integers in the
         * plugged thin/thick I-bundle parameters should measure the
         * number of times the meridinal curve cuts the vertical and
         * horizontal edges of the adjacent block (not the block
         * \a torus1, but its neighbour), or \c false if the vertical
         * and diagonal edges should be used instead.
         * @return the full parameterised name of this triangulation, if the
         * two given blocks are both solid torus plugs (either layered
         * solid tori or Mobius bands), or no value otherwise.
         */
        static std::optional<std::string> findPluggedTori(bool thin, int id,
            const SatBlock* torus0, bool horiz0,
            const SatBlock* torus1, bool horiz1);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls BlockedSFS::swap(); it is provided
 * so that BlockedSFS meets the C++ Swappable requirements.
 *
 * @param a the first structure whose contents should be swapped.
 * @param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(BlockedSFS& a, BlockedSFS& b) noexcept;

// Inline functions for BlockedSFS

inline BlockedSFS::BlockedSFS(SatRegion&& region) : region_(std::move(region)) {
}

inline void BlockedSFS::swap(BlockedSFS& other) noexcept {
    region_.swap(other.region_);
}

inline const SatRegion& BlockedSFS::region() const {
    return region_;
}

inline bool BlockedSFS::operator == (const BlockedSFS& other) const {
    return region_ == other.region_;
}

inline bool BlockedSFS::operator != (const BlockedSFS& other) const {
    return region_ != other.region_;
}

inline void swap(BlockedSFS& a, BlockedSFS& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

