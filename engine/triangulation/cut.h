
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

/*! \file triangulation/cut.h
 *  \brief Supports cuts in triangulations and facet pairings.
 */

#ifndef __REGINA_CUT_H
#ifndef __DOXYGEN
#define __REGINA_CUT_H
#endif

#include <algorithm>
#include <tuple>
#include <utility>
#include "regina-core.h"
#include "core/output.h"
#include "utilities/exception.h"

namespace regina {

template <int dim> class FacetPairing;
template <int dim> struct FacetSpec;
template <int dim> class Isomorphism;
template <int dim> class Triangulation;
template <int n> class Perm;

/**
 * A cut that separates a triangulation or facet pairing into two pieces.
 * This is essentially the same concept as a cut in graph theory.
 *
 * Specifically, a _cut_ in a triangulation or facet pairing partitions
 * the top-dimensional simplices into two _sides_.  This effectively splits
 * the triangulation or facet pairing into two pieces, by removing all
 * gluings between simplices on opposite sides.  The two sides of a cut are
 * numbered 0 and 1.
 *
 * In Regina, a cut has a _size_ and a _weight:_
 *
 * - The _size_ refers to the size of the underlying triangulation or
 *   facet pairing (i.e., it indicates the total number of top-dimensional
 *   simplices).
 *
 * - The _weight_ refers to the number of gluings that are undone by the cut.
 *   This is the usual concept of weight from graph theory (i.e., the number
 *   of edges in the underlying graph that cross the partition).
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup triangulation
 */
class Cut : public ShortOutput<Cut> {
    private:
        size_t size_;
            /**< The total number of top-dimensional simplices in the
                 underlying triangulation or facet pairing. */
        int* side_;
            /**< Indicates which side of the cut each top-dimensional
                 simplex lies on.  This array has size \a size_, and
                 every element is either 0 or 1. */

    public:
        /**
         * Creates a new trivial cut on the given number of top-dimensional
         * simplices.
         *
         * All simplices will be on side 0.
         *
         * \param size the number of top-dimensional simplices in the
         * underlying triangulation or facet pairing.
         */
        Cut(size_t size);

        /**
         * Creates a new cut with the given partition sizes.
         *
         * The total number of top-dimensional simplices under consideration
         * will be (\a side0 + \a side1); the first \a side0 simplices will
         * be on side 0, and the remaining \a side1 simplices will be on
         * side 1.
         *
         * \param side0 the number of top-dimensional simplices on side 0
         * of the partition.
         * \param side1 the number of top-dimensional simplices on side 1
         * of the partition.
         */
        Cut(size_t side0, size_t side1);

        /**
         * Creates a new copy of the given cut.
         *
         * \param src the cut to copy.
         */
        Cut(const Cut& src);

        /**
         * Moves the given cut into this new cut.
         * This is a fast (constant time) operation.
         *
         * The cut that is passed (\a src) will no longer be usable.
         *
         * \param src the cut to move.
         */
        Cut(Cut&& src) noexcept;

        /**
         * Creates a new cut using the given partition.
         *
         * Here a cut on \a n top-dimensional simplices is described by
         * a sequence of \a n integers, each equal to 0 or 1, indicating
         * which side of the partition each top-dimensional simplex lies on.
         *
         * \pre The type \a iterator, when dereferenced, can be cast to
         * an \c int.
         *
         * \warning This routine computes the number of top-dimensional
         * simplices by subtracting `end - begin`, and so ideally
         * \a iterator should be a random access iterator type for which
         * this operation is constant time.
         *
         * \exception InvalidArgument Some element of the given sequence
         * is neither 0 nor 1.
         *
         * \python Instead of a pair of iterators, this routine
         * takes a python list of integers.
         *
         * \param begin an iterator pointing to the beginning of the
         * 0-1 sequence of sides.
         * \param end a past-the-end iterator indicating the end of the
         * 0-1 sequence of sides.
         */
        template <typename iterator>
        Cut(iterator begin, iterator end);

        /**
         * Destroys this cut.
         */
        ~Cut();

        /**
         * Returns the total number of top-dimensional simplices in the
         * underlying triangulation or facet pairing.
         *
         * In other words, this returns the size of the underlying
         * triangulation or facet pairing.
         *
         * \return the total number of top-dimensional simplices.
         */
        size_t size() const;

        /**
         * Returns the number of top-dimensional simplices on the given
         * side of the partition described by this cut.
         *
         * It will always be true that `size(0) + size(1) == size()`.
         *
         * \warning This routine runs in linear time, since the sizes of
         * the individual sides are not cached.  This is in contrast to
         * the overall total size(), which _is_ cached, and which runs
         * in constant time.
         *
         * \exception InvalidArgument The given side is not 0 or 1.
         *
         * \param whichSide the side of the partition to query; this
         * must be either 0 or 1.
         * \return the number of top-dimensional simplices on the given side.
         */
        size_t size(int whichSide) const;

        /**
         * Indicates which side of the partition the given simplex lies on.
         *
         * \param simplex the simplex being queried; this must be
         * between 0 and size()-1 inclusive.
         * \return the corresponding side of the partition; this will be
         * either 0 or 1.
         */
        int side(size_t simplex) const;

        /**
         * Allows you to set which side of the partition the given simplex
         * lies on.
         *
         * \exception InvalidArgument The given side is not 0 or 1.
         *
         * \param simplex the simplex being changed; this must be
         * between 0 and size()-1 inclusive.
         * \param newSide the side of the partition that the given simplex
         * should lie on; this must be either 0 or 1.
         */
        void set(size_t simplex, int newSide);

        /**
         * Determines whether this cut places all top-dimensional simplices
         * on the same side of the partition.
         *
         * \return \c true if all simplices are on side 0 or all simplices
         * are on side 1, or \c false if both sides of the partition are
         * non-empty.
         */
        bool isTrivial() const;

        /**
         * Returns the weight of this cut with respect to the dual graph
         * of the given triangulation.  This is the number of gluings in the
         * given triangulation that cross the partition described by this cut.
         *
         * In other words, this routine counts the number of facets of
         * top-dimensional simplices on side 0 of the cut that are glued
         * to a facet of some top-dimensional simplex on side 1.
         *
         * \pre The given triangulation has precisely size() top-dimensional
         * simplices.
         *
         * \exception InvalidArgument The given triangulation does not
         * have precisely size() top-dimensional simplices.
         *
         * \param tri the triangulation under consideration.
         * \return the weight of this cut with respect to \a tri.
         */
        template <int dim>
        size_t weight(const Triangulation<dim>& tri) const;

        /**
         * Returns the weight of this cut with respect to the given
         * facet pairing.  This is the number of matchings between facets of
         * simplices in the given pairing that cross the partition described
         * by this cut.
         *
         * In other words, this routine counts the number of facets of
         * top-dimensional simplices on side 0 of the cut that are paired with
         * a facet of some top-dimensional simplex on side 1.
         *
         * \pre The given facet pairing has precisely size() top-dimensional
         * simplices.
         *
         * \exception InvalidArgument The given facet pairing does not
         * have precisely size() top-dimensional simplices.
         *
         * \param pairing the facet pairing under consideration.
         * \return the weight of this cut with respect to \a pairing.
         */
        template <int dim>
        size_t weight(const FacetPairing<dim>& pairing) const;

        /**
         * Sets this to be a copy of the given cut.
         *
         * It does not matter if this and the given cut have different
         * sizes (i.e., work with different number of top-dimensional
         * simplices); if they do then this cut will be resized as a result.
         *
         * \param src the cut to copy.
         * \return a reference to this cut.
         */
        Cut& operator = (const Cut& src);

        /**
         * Moves the given cut into this cut.
         * This is a fast (constant time) operation.
         *
         * It does not matter if this and the given cut have different
         * sizes (i.e., work with different number of top-dimensional
         * simplices); if they do then this cut will be resized as a result.
         *
         * The cut that is passed (\a src) will no longer be usable.
         *
         * \param src the cut to move.
         * \return a reference to this cut.
         */
        Cut& operator = (Cut&& src) noexcept;

        /**
         * Swaps the contents of this and the given cut.
         *
         * \param other the cut whose contents are to be swapped with this.
         */
        void swap(Cut& other) noexcept;

        /**
         * Determines if this and the given cut are identical.
         *
         * Two cuts are considered identical if they describe the same
         * partition of simplices into sides 0 and 1.
         *
         * It does not matter if this and the given cut have different
         * sizes; in this case they will be considered different.
         *
         * \param rhs the cut to compare with this.
         * \return \c true if and only if this and the given cut are identical.
         */
        bool operator == (const Cut& rhs) const;

        /**
         * Determines if this and the given cut are different.
         *
         * Two cuts are considered identical if they describe the same
         * partition of simplices into sides 0 and 1.
         *
         * It does not matter if this and the given cut have different
         * sizes; in this case they will be considered different.
         *
         * \param rhs the cut to compare with this.
         * \return \c true if and only if this and the given cut are different.
         */
        bool operator != (const Cut& rhs) const;

        /**
         * Partitions the given triangulation using this cut.
         *
         * This routine will return _two_ triangulations: the first
         * will contain all the top-dimensional simplices on side 0 of
         * this cut, and the second will contain all the top-dimensional
         * simplices on side 1.  All gluings within the same side of the
         * partition will be preserved, but any gluings that cross the
         * partition will be lost (and so the corresponding simplex facets
         * will become boundary).
         *
         * You can call inclusion() if you need to know how the simplex
         * numbers of the resulting triangulations correspond to the
         * simplex numbers of the original triangulation.
         *
         * If any of the facets that cross the partition are locked in the
         * source triangulation \a tri, this will not prevent the operation
         * from occurring (since the source triangulation will not be changed).
         * The two triangulations that are returned will have no simplex
         * and/or facet locks at all.
         *
         * \pre The given triangulation has precisely size() top-dimensional
         * simplices.
         *
         * \exception InvalidArgument The given triangulation does not
         * have precisely size() top-dimensional simplices.
         *
         * \param tri the triangulation to partition.
         * \return the two resulting triangulations, one for each side
         * of the partition.
         */
        template <int dim>
        std::pair<Triangulation<dim>, Triangulation<dim>> operator() (
            const Triangulation<dim>& tri) const;

        /**
         * Partitions the given facet pairing using this cut.
         *
         * This routine will return _two_ facet pairings: the first
         * will contain all the top-dimensional simplices on side 0 of
         * this cut, and the second will contain all the top-dimensional
         * simplices on side 1.  All matchings between simplex facets within
         * the same side of the partition will be preserved, but any matchings
         * that cross the partition will be lost (and so the corresponding
         * simplex facets will become unmatched).
         *
         * You can call inclusion() if you need to know how the simplex
         * numbers of the resulting pairings correspond to the
         * simplex numbers of the original pairing.
         *
         * \pre The given facet pairing has precisely size() top-dimensional
         * simplices.
         * \pre Since empty facet pairings are not allowed, this cut
         * must have at least one top-dimensional simplex on each side.
         *
         * \exception InvalidArgument The given facet pairing does not
         * have precisely size() top-dimensional simplices.
         *
         * \exception FailedPrecondition This cut has all of its
         * top-dimensional simplices on the same side.
         *
         * \param pairing the facet pairing to partition.
         * \return the two resulting facet pairings, one for each side
         * of the partition.
         */
        template <int dim>
        std::pair<FacetPairing<dim>, FacetPairing<dim>> operator() (
            const FacetPairing<dim>& pairing) const;

        /**
         * Returns the relationships between simplex numbers before and
         * after this cut is used to partition a triangulation or facet
         * pairing into two pieces.
         *
         * Specifically: let \a from be a trianglation or facet pairing,
         * and let (\a a, \a b) be the result of partitioning \a from
         * using this cut, so `(a, b) = cut(from)`.
         *
         * Then this routine returns two isomorphisms \a p and \a q,
         * where \a p describes how \a a appears as a subcomplex of \a from,
         * and \a q describes how \a b appears as a subcomplex of \a from.
         * These isomorphisms will be in the direction from \a a and \a b
         * to \a from.
         *
         * The only interesting parts of these isomorphisms are the
         * mappings between the indices of top-dimensional simplices;
         * all of the facet permutations within each top-dimensional
         * simplex will be identity permutations.
         *
         * \python Since Python does not support templates, the
         * dimension \a dim should be passed as an argument to this function.
         *
         * \tparam dim indicates which type of isomorphisms to return.
         * Specifically, this integer parameter indicates the dimension of
         * triangulation on which these isomorphisms act.
         *
         * \return the two inclusion maps corresponding to this partition.
         */
        template <int dim>
        std::pair<Isomorphism<dim>, Isomorphism<dim>> inclusion() const;

        /**
         * Converts this into the next cut of the same size.
         *
         * The total number of top-dimensional simplices will stay the
         * same, but the number on each side of the partition may change.
         *
         * To iterate through all cuts of the given size, you should create
         * a new `Cut(size)` and then make repeated calls to inc().
         *
         * If this is already the last partition in such an iteration
         * (i.e., all top-dimensional simplices are already on side 1),
         * then this routine will return \c false and convert this into
         * the \a first such partition.
         *
         * The order of iteration using inc() is lexicographical in the
         * sequence of sides.  In particular, if you wish to avoid
         * seeing each cut again with sides 0 and 1 swapped, then you
         * can use the fact that all cuts with `side(0) == 0`
         * will be seen before any cuts with `side(0) == 1`.
         *
         * \return \c true if the partition was successfully incremented, or
         * \c false if this was already the last partition in such an iteration.
         */
        bool inc();

        /**
         * Converts this into the next cut with the same partition sizes.
         *
         * Specifically, the number of top-dimensional simplices on each side
         * of the partition will remain the same.
         *
         * To iterate through all cuts with the given parititon sizes, you
         * should create a new `Cut(side0, side1)` and then make
         * repeated calls to incFixedSizes().
         *
         * If this is already the last partition in such an iteration,
         * then this routine will return \c false and convert this into
         * the _first_ such permutation.
         *
         * The order of iteration using incFixedSizes() is lexicographical in
         * the sequence of sides.  In particular, if you wish to avoid
         * seeing each cut again with sides 0 and 1 swapped, then you
         * can use the fact that all cuts with `side(0) == 0`
         * will be seen before any cuts with `side(0) == 1`.
         *
         * \return \c true if the partition was successfully incremented, or
         * \c false if this was already the last partition in such an iteration.
         */
        bool incFixedSizes();

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
};

/**
 * Swaps the contents of the given cuts.
 *
 * This global routine simply calls Cut::swap(); it is provided so that
 * Cut meets the C++ Swappable requirements.
 *
 * \param a the first cut whose contents should be swapped.
 * \param b the second cut whose contents should be swapped.
 *
 * \ingroup triangulation
 */
void swap(Cut& a, Cut& b) noexcept;

// Inline functions for Cut

inline Cut::Cut(size_t size) : size_(size), side_(new int[size_]) {
    std::fill(side_, side_ + size_, 0);
}

inline Cut::Cut(size_t side0, size_t side1) : size_(side0 + side1) {
    side_ = new int[size_];
    std::fill(side_, side_ + side0, 0);
    std::fill(side_ + side0, side_ + size_, 1);
}

inline Cut::Cut(const Cut& src) : size_(src.size_), side_(new int[src.size_]) {
    std::copy(src.side_, src.side_ + src.size_, side_);
}

inline Cut::Cut(Cut&& src) noexcept : size_(src.size_), side_(src.side_) {
    src.side_ = nullptr;
}

template <typename iterator>
Cut::Cut(iterator begin, iterator end) : size_(end - begin) {
    side_ = new int[size_];

    int* out = side_;
    while (begin != end) {
        int s = *begin++;
        if (s != 0 && s != 1)
            throw InvalidArgument("The Cut sequence-based constructor requires "
                "every side to be 0 or 1.");
        *out++ = s;
    }
}

inline Cut::~Cut() {
    delete[] side_;
}

inline size_t Cut::size() const {
    return size_;
}

inline size_t Cut::size(int whichSide) const {
    return std::count(side_, side_ + size_, whichSide);
}

inline int Cut::side(size_t simplex) const {
    return side_[simplex];
}

inline void Cut::set(size_t simplex, int newSide) {
    if (newSide != 0 && newSide != 1)
        throw InvalidArgument("Cut::set() requires the side to be 0 or 1.");
    side_[simplex] = newSide;
}

template <int dim>
size_t Cut::weight(const Triangulation<dim>& tri) const {
    if (tri.size() != size_)
        throw InvalidArgument("Cut::weight() requires a triangulation "
            "with the same size as the cut.");

    size_t ans = 0;
    for (size_t i = 0; i < size_; ++i)
        if (side(i) == 0) {
            auto left = tri.simplex(i);
            for (int j = 0; j <= dim; ++j) {
                auto dest = left->adjacentSimplex(j);
                if (dest && side_[dest->index()] == 1)
                    ++ans;
            }
        }

    return ans;
}

template <int dim>
size_t Cut::weight(const FacetPairing<dim>& pairing) const {
    if (pairing.size() != size_)
        throw InvalidArgument("Cut::weight() requires a facet pairing "
            "with the same size as the cut.");

    size_t ans = 0;
    for (size_t i = 0; i < size_; ++i)
        if (side(i) == 0)
            for (int j = 0; j <= dim; ++j) {
                auto dest = pairing.dest(i, j);
                if ((! dest.isBoundary(size_)) && side_[dest.simp] == 1)
                    ++ans;
            }

    return ans;
}

inline Cut& Cut::operator = (const Cut& src) {
    // std::copy does not promise to behave well under self-assignment.
    if (std::addressof(src) == this)
        return *this;

    if (size_ != src.size_) {
        size_ = src.size_;

        delete[] side_;
        side_ = new int[size_];
    }

    std::copy(src.side_, src.side_ + size_, side_);
    return *this;
}

inline Cut& Cut::operator = (Cut&& src) noexcept {
    size_ = src.size_;
    std::swap(side_, src.side_);
    // Leave src to delete the original side_ array in its own destructor.
    return *this;
}

inline void Cut::swap(Cut& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(side_, other.side_);
}

inline bool Cut::operator == (const Cut& rhs) const {
    return size_ == rhs.size_ && std::equal(side_, side_ + size_, rhs.side_);
}

inline bool Cut::operator != (const Cut& rhs) const {
    return size_ != rhs.size_ || ! std::equal(side_, side_ + size_, rhs.side_);
}

template <int dim>
std::pair<Triangulation<dim>, Triangulation<dim>> Cut::operator() (
        const Triangulation<dim>& tri) const {
    if (tri.size() != size_)
        throw InvalidArgument("To apply a cut to a triangulation, the "
            "triangulation must have the same size as the cut.");

    if (size_ == 0)
        return {};

    auto* reverse = new size_t[size_];
    size_t part[2] { 0, 0 };
    for (size_t i = 0; i < size_; ++i)
        reverse[i] = part[side_[i]]++;

    std::pair<Triangulation<dim>, Triangulation<dim>> ans;

    for (size_t i = 0; i < part[0]; ++i)
        ans.first.newSimplex();
    for (size_t i = 0; i < part[1]; ++i)
        ans.second.newSimplex();

    for (size_t i = 0; i < size_; ++i) {
        auto origFrom = tri.simplex(i);
        if (side_[i] == 0) {
            auto newFrom = ans.first.simplex(reverse[i]);
            for (int j = 0; j <= dim; ++j) {
                auto origTo = origFrom->adjacentSimplex(j);
                if (origTo && side_[origTo->index()] == 0)
                    newFrom->join(j,
                        ans.first.simplex(reverse[origTo->index()]),
                        origFrom->adjacentGluing(j));
            }
        } else {
            auto newFrom = ans.second.simplex(reverse[i]);
            for (int j = 0; j <= dim; ++j) {
                auto origTo = origFrom->adjacentSimplex(j);
                if (origTo && side_[origTo->index()] == 1)
                    newFrom->join(j,
                        ans.second.simplex(reverse[origTo->index()]),
                        origFrom->adjacentGluing(j));
            }
        }
    }

    delete[] reverse;
    return ans;
}

template <int dim>
std::pair<FacetPairing<dim>, FacetPairing<dim>> Cut::operator() (
        const FacetPairing<dim>& pairing) const {
    if (pairing.size() != size_)
        throw InvalidArgument("To apply a cut to a facet pairing, the "
            "pairing must have the same size as the cut.");

    auto* reverse = new size_t[size_];
    size_t part[2] { 0, 0 };
    for (size_t i = 0; i < size_; ++i)
        reverse[i] = part[side_[i]]++;

    if (part[0] == 0 || part[1] == 0)
        throw FailedPrecondition("To apply a cut to a facet pairing, the "
            "cut cannot have all its simplices on the same side of the "
            "partition.");

    std::pair<FacetPairing<dim>, FacetPairing<dim>> ans {
        FacetPairing<dim>(part[0]), FacetPairing<dim>(part[1]) };

    for (size_t i = 0; i < size_; ++i) {
        if (side_[i] == 0) {
            for (int j = 0; j <= dim; ++j) {
                auto origTo = pairing.dest(i, j);
                if (origTo.isBoundary(size_) || side_[origTo.simp] != 0)
                    ans.first.dest(reverse[i], j).setBoundary(part[0]);
                else
                    ans.first.dest(reverse[i], j) = FacetSpec<dim>(
                        reverse[origTo.simp], origTo.facet);
            }
        } else {
            for (int j = 0; j <= dim; ++j) {
                auto origTo = pairing.dest(i, j);
                if (origTo.isBoundary(size_) || side_[origTo.simp] != 1)
                    ans.second.dest(reverse[i], j).setBoundary(part[1]);
                else
                    ans.second.dest(reverse[i], j) = FacetSpec<dim>(
                        reverse[origTo.simp], origTo.facet);
            }
        }
    }

    delete[] reverse;
    return ans;
}

template <int dim>
std::pair<Isomorphism<dim>, Isomorphism<dim>> Cut::inclusion() const {
    size_t part[2] { 0, 0 };
    for (size_t i = 0; i < size_; ++i)
        ++part[side_[i]];

    std::pair<Isomorphism<dim>, Isomorphism<dim>> ans(std::piecewise_construct,
        std::make_tuple(part[0]), std::make_tuple(part[1]));

    size_t next[2] = { 0, 0 };
    for (size_t i = 0; i < size_; ++i) {
        if (side_[i] == 0) {
            ans.first.simpImage(next[0]) = i;
            ans.first.facetPerm(next[0]) = Perm<dim + 1>();
            ++next[0];
        } else {
            ans.second.simpImage(next[1]) = i;
            ans.second.facetPerm(next[1]) = Perm<dim + 1>();
            ++next[1];
        }
    }

    return ans;
}

inline bool Cut::incFixedSizes() {
    return std::next_permutation(side_, side_ + size_);
}

inline void swap(Cut& a, Cut& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

