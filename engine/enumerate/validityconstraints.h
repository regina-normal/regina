
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file enumerate/validityconstraints.h
 *  \brief Deals with validity constraints in polytope vertex enumeration.
 */

#ifndef __REGINA_ENUMCONSTRAINTS_H
#ifndef __DOXYGEN
#define __REGINA_ENUMCONSTRAINTS_H
#endif

#include <cstddef>
#include <vector>
#include "regina-core.h"

namespace regina {

/**
 * Represents a set of combinatorial validity constraints for use with
 * polytope vertex enumeration.
 *
 * Vertex enumeration routines such as
 * DoubleDescription::enumerateExtremalRays() take a cone (specifically the
 * non-negative orthant), form the intersection of that cone with a given
 * linear subspace, and return the extremal rays of the new cone that results.
 *
 * In some cases we are only interested in \e valid rays of the new cone.
 * The ValidityConstraints class stores a number of "validity constraints";
 * a ray is then "valid" if it satisfies all of these constraints.
 *
 * Each individual constraint encodes a subset \a S of coordinate positions,
 * and requires that a ray can only be non-zero on at most \e one of those
 * coordinate positions.  Equivalently, if we were to assume that the
 * linear subspace is in general position with respect to the coordinate
 * axes (which it is often not), then the <i>i</i>th facet of the cone would
 * come from the hyperplane perpendicular to <i>i</i>th coordinate; such a
 * constraint would then require that a ray can only lie outside at most
 * one of the facets corresponding to the coordinate positions in \a S.
 *
 * This class is tailored specifically to constraints that come from
 * normal surfaces and angle structures:
 *
 * - We assume that the coordinate positions are grouped into consecutive
 *   \e blocks, each of the same size.  For instance, in standard coordinates
 *   for normal surfaces, there is one block per tetrahedron, each of size
 *   seven (since each tetrahedron provides four triangle coordinates and
 *   three quadrilateral coordinates).
 *
 * - It is okay if there are additional coordinates that appear beyond these
 *   blocks (e.g., the final scaling coordinate in angle structure coordinates).
 *   However, these additional coordinates cannot be used in any validity
 *   constraints.
 *
 * It is assumed that all constraints are either \e local or \e global:
 *
 * - A \e local constraint involves coordinates within a single block only.
 *   It is assumed that, if local constraints are used, then \e every block
 *   will use analogous local constraints (where "analagous" means they use
 *   the same coordinate positions relative to the start of each block).
 *   An example of a local constraint is the quadrilateral constraints from
 *   normal surface theory, which require that each tetrahedron has at most
 *   one non-zero quadrilateral coordinate.
 *
 * - A \e global constraint involves coordinates in every block; moreover,
 *   each block must constraint the same coordinates relative to the start
 *   of the block.  An example of a global constraint is with almost normal
 *   surfaces, where we require that the entire surface has at most one
 *   non-zero octagonal coordinate.
 *
 * You can add a full set of local constraints (one for each block) using a
 * single call to addLocal(), and you can add a single global constraint by
 * calling addGlobal().
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ifacespython Not present.
 *
 * \ingroup enumerate
 */
class ValidityConstraints {
    public:
        /**
         * An empty set of constraints.
         */
        static const ValidityConstraints none;

    private:
        int blockSize_ { 0 };
            /**< The number of coordinates in each block. */
        size_t nBlocks_ { 0 };
            /**< The total number of blocks. */
        std::vector<std::vector<int>> local_;
            /**< The local constraints.  Each element of this vector
                 stores coordinate positions within a single block,
                 relative to the beginning of the block. */
        std::vector<std::vector<int>> global_;
            /**< The global constraints.  Each element of this vector
                 stores coordinate positions within a single block,
                 relative to the beginning of the block. */

    public:
        /**
         * Creates an empty set of validity constraints for vectors with
         * the given block structure.
         *
         * See the class notes for details on how vector coordinates are
         * assumed to be grouped into \a nBlocks consecutive blocks,
         * each containing \a blockSize coordinates (possibly with some
         * additional coordinates beyond this block structure that are
         * not used in any validity constraints).
         *
         * @param blockSize the number of coordinates in each block.
         * For example, for vectors describing normal surfaces in standard
         * coordinates, this block size would be 7 (representing the three
         * triangle and four quadrilateral coordinates for each tetrahedron).
         * @param nBlocks the number of consecutive blocks of size \a blockSize.
         * For example, for vectors describing normal surfaces in standard
         * coordinates, this number of blocks would be the number of
         * tetrahedra in the underlying triangulation.
         * @param reserveLocal indicates that we should reserve space
         * for \a reserveLocal calls to addLocal().  This is purely for
         * optimisation; it is safe to leave this as 0 (the default).
         * @param reserveGlobal indicates that we should reserve space
         * for \a reserveGlobal calls to addGlobal().  This is purely for
         * optimisation; it is safe to leave this as 0 (the default).
         */
        ValidityConstraints(int blockSize, size_t nBlocks,
            size_t reserveLocal = 0, size_t reserveGlobal = 0);

        /**
         * Creates a clone of the given constraint set.
         */
        ValidityConstraints(const ValidityConstraints&) = default;
        /**
         * Moves the contents of the given constraint set into this new
         * constraint set.
         *
         * The constraint set that was passed will no longer be usable.
         */
        ValidityConstraints(ValidityConstraints&&) noexcept = default;

        /**
         * Sets this to be a copy of the given constraint set.
         *
         * @return a reference to this constraint set.
         */
        ValidityConstraints& operator = (const ValidityConstraints&) = default;
        /**
         * Moves the contents of the given constraint set into this
         * constraint set.
         *
         * The constraint set that was passed will no longer be usable.
         *
         * @return a reference to this constraint set.
         */
        ValidityConstraints& operator = (ValidityConstraints&&) noexcept =
            default;

        /**
         * Adds a new family of local constraints to this set.
         *
         * A single call to addLocal() will effectively add one local
         * constraint for every block.  Each local constraint will
         * constraint the coordinates in \a pattern, where these
         * coordinate positions are given relative to the start of each block.
         *
         * For example, to encode the quadrilateral constraints for
         * normal surfaces in standard coordinates, you can make the
         * single call <tt>addLocal({4, 5, 6})</tt>.
         *
         * \ifacespython Not present.
         *
         * @param pattern the coordinates to constraint within each block,
         * relative to the start of the block.
         */
        void addLocal(std::initializer_list<int> pattern);
        /**
         * Adds one new global constraint to this set.
         *
         * A call to addGlobal() will add a single global constraint, using
         * coordinates from every block.  Within each block, the coordinates
         * that are constrained will be those listed in \a pattern, where
         * the coordinate positions in \a pattern are given relative to
         * the start of each block.
         *
         * As an example, for almost normal surfaces in standard coordinates,
         * you can encode the constraint that there is at most one octagon
         * in the entire surface by calling <tt>addGlobal({7, 8, 9})</tt>.
         *
         * \ifacespython Not present.
         *
         * @param pattern the coordinates to constraint within each block,
         * relative to the start of the block.
         */
        void addGlobal(std::initializer_list<int> pattern);

        /**
         * Swaps the contents of this and the given constraint set.
         *
         * @param other the constraint set whose contents should be swapped
         * with this.
         */
        void swap(ValidityConstraints& other) noexcept;

        /**
         * Returns the list of all individual validity constraints, each
         * expressed as a bitmask.
         *
         * Each local constraint "pattern" that was added using addLocal()
         * will produce many bitmasks, since there will be one local constraint
         * for every block.  Each global constraint that was added using
         * addGlobal() will produce just one bitmask.
         *
         * The bits corresponding to coordinate positions that are constrained
         * will be set to \c true; all other bits will be set to \c false.
         *
         * \pre The given length \a len is at least the number of blocks
         * multiplied by the block size.
         *
         * \pre A bitmask of type \a BitmaskType is large enough to store
         * \a len bits.
         *
         * \ifacespython This routine is only available in Python for the
         * bitmask type regina::Bitmask.
         *
         * \tparam BitmaskType the bitmask type used to encode each constraint;
         * this must be one of Regina's own bitmask types, such as Bitmask,
         * Bitmask1 or Bitmask2.
         *
         * @param len the total number of coordinates in the vectors being
         * constrained.  Each bitmask will be created with this length.
         * @return the list of bitmasks describing the full set of validity
         * constraints.
         */
        template <typename BitmaskType>
        std::vector<BitmaskType> bitmasks(size_t len) const;

    private:
        /**
         * Constructs an empty set of constraints.
         *
         * Since \a blockSize_ and \a nBlocks_ are not initialised, you
         * cannot add any constraints to this set.  Therefore this
         * constructor is private, and is only used to build the
         * constant ValidityConstraints::none.
         */
        ValidityConstraints() = default;
};

/**
 * Swaps the contents of the two given constraint sets.
 *
 * This global routine simply calls ValidityConstraints::swap(); it is provided
 * so that ValidityConstraints meets the C++ Swappable requirements.
 *
 * @param a the first constraint set whose contents should be swapped.
 * @param b the second constraint set whose contents should be swapped.
 *
 * \ingroup enumerate
 */
void swap(ValidityConstraints& a, ValidityConstraints& b) noexcept;

inline const ValidityConstraints ValidityConstraints::none;

// Inline functions for ValidityConstraints

inline ValidityConstraints::ValidityConstraints(int blockSize, size_t nBlocks,
        size_t reserveLocal, size_t reserveGlobal) :
        blockSize_(blockSize), nBlocks_(nBlocks) {
    local_.reserve(reserveLocal);
    global_.reserve(reserveGlobal);
}

inline void ValidityConstraints::addLocal(std::initializer_list<int> pattern) {
    local_.emplace_back(std::move(pattern));
}
inline void ValidityConstraints::addGlobal(std::initializer_list<int> pattern) {
    global_.emplace_back(std::move(pattern));
}

inline void ValidityConstraints::swap(ValidityConstraints& other) noexcept {
    std::swap(blockSize_, other.blockSize_);
    std::swap(nBlocks_, other.nBlocks_);
    local_.swap(other.local_);
    global_.swap(other.global_);
}

template <typename BitmaskType>
std::vector<BitmaskType> ValidityConstraints::bitmasks(size_t len) const {
    std::vector<BitmaskType> ans;
    ans.reserve(local_.size() * nBlocks_ + global_.size());

    for (const auto& v : local_) {
        size_t base = 0;
        for (size_t b = 0; b < nBlocks_; ++b, base += blockSize_) {
            BitmaskType mask(len);
            for (int i : v)
                mask.set(base + i, true);
            ans.push_back(std::move(mask));
        }
    }

    for (const auto& v : global_) {
        BitmaskType mask(len);
        size_t base = 0;
        for (size_t b = 0; b < nBlocks_; ++b, base += blockSize_) {
            for (int i : v)
                mask.set(base + i, true);
        }
        ans.push_back(std::move(mask));
    }

    return ans;
}

inline void swap(ValidityConstraints& a, ValidityConstraints& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

