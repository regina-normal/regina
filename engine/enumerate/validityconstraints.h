
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

/*! \file enumerate/validityconstraints.h
 *  \brief Deals with validity constraints in polytope vertex enumeration.
 */

#ifndef __REGINA_ENUMCONSTRAINTS_H
#ifndef __DOXYGEN
#define __REGINA_ENUMCONSTRAINTS_H
#endif

#include <algorithm>
#include <cstddef>
#include <vector>
#include "core/output.h"

namespace regina {

/**
 * Represents a set of combinatorial validity constraints for use with
 * polytope vertex enumeration.
 *
 * Vertex enumeration routines such as DoubleDescription::enumerate() take a
 * cone (specifically the non-negative orthant), form the intersection of that
 * cone with a given linear subspace, and return the extremal rays of the new
 * cone that results.
 *
 * In some cases we are only interested in _valid_ rays of the new cone.
 * The ValidityConstraints class stores a number of "validity constraints";
 * a ray is then "valid" if it satisfies all of these constraints.
 *
 * Each individual constraint encodes a subset \a S of coordinate positions,
 * and requires that a ray can only be non-zero on at most _one_ of those
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
 *   _blocks_, each of the same size.  For instance, in standard coordinates
 *   for normal surfaces, there is one block per tetrahedron, each of size
 *   seven (since each tetrahedron provides four triangle coordinates and
 *   three quadrilateral coordinates).
 *
 * - It is okay if there are additional coordinates that appear beyond these
 *   blocks (e.g., the final scaling coordinate in angle structure coordinates).
 *   However, these additional coordinates cannot be used in any validity
 *   constraints.
 *
 * It is assumed that all constraints are either _local_ or _global:_
 *
 * - A _local_ constraint involves coordinates within a single block only.
 *   It is assumed that, if local constraints are used, then _every_ block
 *   will use analogous local constraints (where "analagous" means they use
 *   the same coordinate positions relative to the start of each block).
 *   An example of a local constraint is the quadrilateral constraints from
 *   normal surface theory, which require that each tetrahedron has at most
 *   one non-zero quadrilateral coordinate.
 *
 * - A _global_ constraint involves coordinates in every block; moreover,
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
 * \ingroup enumerate
 */
class ValidityConstraints : public Output<ValidityConstraints> {
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
         * \param blockSize the number of coordinates in each block.
         * For example, for vectors describing normal surfaces in standard
         * coordinates, this block size would be 7 (representing the three
         * triangle and four quadrilateral coordinates for each tetrahedron).
         * \param nBlocks the number of consecutive blocks of size \a blockSize.
         * For example, for vectors describing normal surfaces in standard
         * coordinates, this number of blocks would be the number of
         * tetrahedra in the underlying triangulation.
         * \param reserveLocal indicates that we should reserve space
         * for \a reserveLocal calls to addLocal().  This is purely for
         * optimisation; it is safe to leave this as 0 (the default).
         * \param reserveGlobal indicates that we should reserve space
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
         * \return a reference to this constraint set.
         */
        ValidityConstraints& operator = (const ValidityConstraints&) = default;
        /**
         * Moves the contents of the given constraint set into this
         * constraint set.
         *
         * The constraint set that was passed will no longer be usable.
         *
         * \return a reference to this constraint set.
         */
        ValidityConstraints& operator = (ValidityConstraints&&) noexcept =
            default;

        /**
         * Adds a new family of local constraints to this set.
         *
         * A single call to addLocal() will effectively add one local
         * constraint for every block.  Each local constraint will
         * constraint the coordinates in the given iterator range, where these
         * coordinate positions are given relative to the start of each block.
         *
         * For example, to encode the quadrilateral constraints for
         * normal surfaces in standard coordinates, you can pass an
         * iterator range that encodes the three integers 4, 5, 6.
         *
         * \pre The iterator type \a iterator, when dereferenced, can be
         * assigned to a native C++ \c int.
         *
         * \python Instead of the iterators \a begin and \a end,
         * this routine takes a python list of integers.
         *
         * \param begin the beginning of the list of coordinates to constraint
         * within each block, relative to the start of the block, as outlined
         * above.
         * \param end a past-the-end iterator indicating the end of the list of
         * coordinates to constraint within each block.
         */
        template <typename iterator>
        void addLocal(iterator begin, iterator end);
        /**
         * Adds a new family of hard-coded local constraints to this set.
         *
         * A single call to addLocal() will effectively add one local
         * constraint for every block.  Each local constraint will
         * constraint the coordinates in \a pattern, where these
         * coordinate positions are given relative to the start of each block.
         *
         * For example, to encode the quadrilateral constraints for
         * normal surfaces in standard coordinates, you can make the
         * single call `addLocal({4, 5, 6})`.
         *
         * \nopython Instead, use the Python version of this function that
         * takes the coordinate pattern as a Python list (which need not be
         * constant).
         *
         * \param pattern the coordinates to constraint within each block,
         * relative to the start of the block.
         */
        void addLocal(std::initializer_list<int> pattern);
        /**
         * Adds one new global constraint to this set.
         *
         * A call to addGlobal() will add a single global constraint, using
         * coordinates from every block.  Within each block, the coordinates
         * that are constrained will be those listed in the given iterator
         * range, where these coordinate positions are given relative to
         * the start of each block.
         *
         * As an example, for almost normal surfaces in standard coordinates,
         * you can encode the constraint that there is at most one octagon
         * in the entire surface by passing an iterator range that
         * encodes the three integers 7, 8, 9.
         *
         * \pre The iterator type \a iterator, when dereferenced, can be
         * assigned to a native C++ \c int.
         *
         * \python Instead of the iterators \a begin and \a end,
         * this routine takes a python list of integers.
         *
         * \param begin the beginning of the list of coordinates to constraint
         * within each block, relative to the start of the block, as outlined
         * above.
         * \param end a past-the-end iterator indicating the end of the list of
         * coordinates to constraint within each block.
         */
        template <typename iterator>
        void addGlobal(iterator begin, iterator end);
        /**
         * Adds one new hard-coded global constraint to this set.
         *
         * A call to addGlobal() will add a single global constraint, using
         * coordinates from every block.  Within each block, the coordinates
         * that are constrained will be those listed in \a pattern, where
         * the coordinate positions in \a pattern are given relative to
         * the start of each block.
         *
         * As an example, for almost normal surfaces in standard coordinates,
         * you can encode the constraint that there is at most one octagon
         * in the entire surface by calling `addGlobal({7, 8, 9})`.
         *
         * \nopython Instead, use the Python version of this function that
         * takes the coordinate pattern as a Python list (which need not be
         * constant).
         *
         * \param pattern the coordinates to constraint within each block,
         * relative to the start of the block.
         */
        void addGlobal(std::initializer_list<int> pattern);

        /**
         * Swaps the contents of this and the given constraint set.
         *
         * \param other the constraint set whose contents should be swapped
         * with this.
         */
        void swap(ValidityConstraints& other) noexcept;

        /**
         * Returns the list of all individual validity constraints, each
         * expressed as a bitmask of the given length.
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
         * \python This routine uses the bitmask type regina::Bitmask.
         *
         * \tparam BitmaskType the bitmask type used to encode each constraint;
         * this must be one of Regina's own bitmask types, such as Bitmask,
         * Bitmask1 or Bitmask2.
         *
         * \param len the total number of coordinates in the vectors being
         * constrained.  Each bitmask will be created with this length.
         * \return the list of bitmasks describing the full set of validity
         * constraints.
         */
        template <typename BitmaskType>
        std::vector<BitmaskType> bitmasks(size_t len) const;

        /**
         * Returns the list of all individual validity constraints, each
         * expressed as a bitmask of the smallest possible length.
         *
         * Calling `bitmasks()` is equivalent to calling `bitmasks(len)`,
         * where \a len is the block size multiplied by the number of blocks.
         *
         * As an example, this is appropriate for normal surface coordinate
         * systems, where the normal coordinates incorporate precisely
         * one block for each tetrahedron and nothing more.  However, it is
         * not appropriate for angle structure coordinates, where there is an
         * additional "scaling coordinate" that appears after all the blocks.
         *
         * Each local constraint "pattern" that was added using addLocal()
         * will produce many bitmasks, since there will be one local constraint
         * for every block.  Each global constraint that was added using
         * addGlobal() will produce just one bitmask.
         *
         * The bits corresponding to coordinate positions that are constrained
         * will be set to \c true; all other bits will be set to \c false.
         *
         * \pre A bitmask of type \a BitmaskType is large enough to store
         * \a len bits, where \a len is the block size multiplied by the
         * number of blocks.  Each bitmask that is returned will be created
         * with this length.
         *
         * \python This routine uses the bitmask type regina::Bitmask.
         *
         * \tparam BitmaskType the bitmask type used to encode each constraint;
         * this must be one of Regina's own bitmask types, such as Bitmask,
         * Bitmask1 or Bitmask2.
         *
         * \return the list of bitmasks describing the full set of validity
         * constraints.
         */
        template <typename BitmaskType>
        std::vector<BitmaskType> bitmasks() const;

        /**
         * Determines whether this and the given set contain the same
         * constraints.
         *
         * This test compares the number of coordinates in each block,
         * the total number of blocks, the set of local constraints, and
         * the set of global constraints.  The local and global constraints
         * may appear in any order, and their individual coordinates may
         * likewise appear in any order; such reorderings will not
         * affect the outcome of this test.
         *
         * \warning Because this test allows for reordering, the comparison is
         * not very efficient.  It is assumed that this will not be a problem,
         * because typical constraint sets are extremely small.
         *
         * \param other the constraint set to compare against this.
         * \return \c true if and only if this and the given set
         * contain the same constraints.
         */
        bool operator == (const ValidityConstraints& other) const;

        /**
         * Determines whether this and the given set do not contain the same
         * constraints.
         *
         * This test compares the number of coordinates in each block,
         * the total number of blocks, the set of local constraints, and
         * the set of global constraints.  The local and global constraints
         * may appear in any order, and their individual coordinates may
         * likewise appear in any order; such reorderings will not
         * affect the outcome of this test.
         *
         * \warning Because this test allows for reordering, the comparison is
         * not very efficient.  It is assumed that this will not be a problem,
         * because typical constraint sets are extremely small.
         *
         * \param other the constraint set to compare against this.
         * \return \c true if and only if this and the given set
         * do not contain the same constraints.
         */
        bool operator != (const ValidityConstraints& other) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

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
 * \param a the first constraint set whose contents should be swapped.
 * \param b the second constraint set whose contents should be swapped.
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

template <typename iterator>
inline void ValidityConstraints::addLocal(iterator begin, iterator end) {
    local_.emplace_back(begin, end);
}
inline void ValidityConstraints::addLocal(std::initializer_list<int> pattern) {
    local_.emplace_back(pattern);
}
template <typename iterator>
inline void ValidityConstraints::addGlobal(iterator begin, iterator end) {
    global_.emplace_back(begin, end);
}
inline void ValidityConstraints::addGlobal(std::initializer_list<int> pattern) {
    global_.emplace_back(pattern);
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

template <typename BitmaskType>
inline std::vector<BitmaskType> ValidityConstraints::bitmasks() const {
    return bitmasks<BitmaskType>(blockSize_ * nBlocks_);
}

inline bool ValidityConstraints::operator == (const ValidityConstraints& other)
        const {
    if (blockSize_ != other.blockSize_ || nBlocks_ != other.nBlocks_)
        return false;

    // We could do better than is_permutation (which is quadratic in the
    // vector size), but sets are typically small and probably nobody is
    // using this operator anyway.

    if (! std::is_permutation(local_.begin(), local_.end(),
            other.local_.begin(), other.local_.end(),
            [](const std::vector<int>& a, const std::vector<int>& b) {
                return std::is_permutation(
                    a.begin(), a.end(), b.begin(), b.end());
            }))
        return false;

    if (! std::is_permutation(global_.begin(), global_.end(),
            other.global_.begin(), other.global_.end(),
            [](const std::vector<int>& a, const std::vector<int>& b) {
                return std::is_permutation(
                    a.begin(), a.end(), b.begin(), b.end());
            }))
        return false;

    return true;
}

inline bool ValidityConstraints::operator != (const ValidityConstraints& other)
        const {
    return ! ((*this) == other);
}

inline void ValidityConstraints::writeTextShort(std::ostream& out) const {
    out << "Blocks: " << nBlocks_ << " x " << blockSize_;
    if (! local_.empty()) {
        out << ", local: ";
        bool first = true;
        for (const auto& v : local_) {
            if (first)
                first = false;
            else
                out << ", ";
            out << "{ ";
            for (int i : v)
                out << i << ' ';
            out << '}';
        }
    }
    if (! global_.empty()) {
        out << ", global: ";
        bool first = true;
        for (const auto& v : global_) {
            if (first)
                first = false;
            else
                out << ", ";
            out << "{ ";
            for (int i : v)
                out << i << ' ';
            out << '}';
        }
    }
}

inline void ValidityConstraints::writeTextLong(std::ostream& out) const {
    out << nBlocks_ << " block(s) of size " << blockSize_ << std::endl;
    if (local_.empty()) {
        out << "No local constraints" << std::endl;
    } else {
        out << "Local: ";
        bool first = true;
        for (const auto& v : local_) {
            if (first)
                first = false;
            else
                out << ", ";
            out << "{ ";
            for (int i : v)
                out << i << ' ';
            out << '}';
        }
        out << std::endl;
    }
    if (global_.empty()) {
        out << "No global constraints" << std::endl;
    } else {
        out << "Global: ";
        bool first = true;
        for (const auto& v : global_) {
            if (first)
                first = false;
            else
                out << ", ";
            out << "{ ";
            for (int i : v)
                out << i << ' ';
            out << '}';
        }
        out << std::endl;
    }
}

inline void swap(ValidityConstraints& a, ValidityConstraints& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

