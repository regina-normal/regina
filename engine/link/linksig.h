
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

/*! \file link/linksig.h
 *  \brief Defines different encodings for knot/link signatures.
 */

#ifndef __REGINA_LINKSIG_H
#ifndef __DOXYGEN
#define __REGINA_LINKSIG_H
#endif

#include <string>
#include "regina-core.h"
#include "concepts/core.h"
#include "utilities/boolset.h"
#include "utilities/fixedarray.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

class Link;

/**
 * Holds all the data required to reconstruct a single connected component
 * of a link diagram.  This is a halfway point between link diagrams and
 * knot/link signatures: the data is purely numerical, making it easy to
 * minimise amongst all possible labellings, and also easy to encode or
 * decode in an appropriate signature form (e.g., a base64 string).
 *
 * Specifically, this data consists of a sequence of integer triples
 * `(crossing_index, strand, crossing_sign)`, describing traversals along each
 * of the _topological_ link components within this diagram component, with
 * sentinels `(n, 0, 0)` placed between topological link components (where
 * \a n is the total number of crossings in the diagram component).  There is
 * no sentinel at the beginning or end of the overall sequence.  Crossings
 * indices are in the range `0,...,n-1`, and strands are either 0 or 1 for
 * lower or upper strands respectively.
 *
 * A knot/link signature will encode a labelling that minimises this sequence
 * lexicographically.  The ordering used for individual triples places lower
 * crossing indices first, then upper strands first, then positive signs first.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \apinotfinal
 *
 * \ingroup link
 */
class LinkSigData {
    public:
        /**
         * An integer triple `(crossing_index, strand, crossing_sign)`,
         * describing a single step in the traversal along some topological
         * link component.
         *
         * See the LinkSigData class notes for further details.
         *
         * This is a very simple struct, and its constructors and assignment
         * operators are provided automatically by the compiler.  These objects
         * are small enough to pass by value and swap with `std::swap()`, with
         * no need for any specialised move operations or swap functions.
         *
         * \python This is a read-only struct: its data members cannot be
         * modified.  Moreover, objects of this type cannot be explicitly
         * constructed; instead you would typically retrieve them from a link
         * diagram data set by calling `LinkSigData::traversal()`.
         */
        struct Term {
            /**
             * The zero-based crossing index.
             */
            size_t crossing;
            /**
             * 0 or 1 for the lower or upper strand respectively.
             */
            int strand;
            /**
             * ±1 indicating the sign of the crossing.
             */
            int sign;

            /**
             * Determines whether this and the given triple are identical.
             *
             * \return \c true if and only if this and the given triple are
             * identical.
             */
            bool operator == (const Term&) const = default;

            /**
             * Compares two triples, using the same ordering as described in
             * the LinkSigData class notes.
             *
             * This generates all of the usual comparison operators, including
             * `<`, `<=`, `>`, and `>=`.
             *
             * \param rhs the triple to compare with this.
             * \return the result of the comparison between this and the given
             * triple.
             */
            std::strong_ordering operator <=> (const Term& rhs) const {
                // Smaller crossing indices first.
                if (auto c = crossing <=> rhs.crossing; c != 0)
                    return c;
                // Upper strands before lower.
                if (auto c = rhs.strand <=> strand; c != 0)
                    return c;
                // Positive crossings before negative.
                return rhs.sign <=> sign;
            }

            /**
             * Sets this to be a sentinel triple `(n, 0, 0)`, as described in
             * the LinkSigData class notes, where \a n is the total number of
             * crossings in the diagram component being described.
             *
             * \param size the total number of crossings in the diagram
             * component (i.e., the integer \a n that is used above).
             */
            void makeSentinel(size_t size) {
                crossing = size;
                strand = sign = 0;
            }
        };

        /**
         * The data structure used to hold the full sequence of triples for a
         * connected diagram component, as described in the LinkSigData class
         * notes.
         */
        using Traversal = FixedArray<Term>;

    private:
        size_t size_;
            /**< The total number of crossings in this diagram component. */
        Traversal sequence_;
            /**< The full sequence of triples that describe this diagram
                 component. */

    public:
        /**
         * Creates a new data set for the given connected link diagram, using
         * a labelling that minimises the sequence of triples.  See the
         * LinkSigData class notes for details on how this ordering works.
         *
         * \pre The given link diagram has exactly one connected component.
         * In particular, `link.isEmpty()` must return `false`, and
         * `link.isConnected()` must return `true`.
         *
         * \param link the connected link diagram that this data set should
         * represent.
         * \param reflectionOptions indicates whether we may reflect the entire
         * link diagram when minimising the sequence of triples.  This is a
         * BoolSet, which means reflection may be mandatory, forbidden, or
         * optional.  This BoolSet cannot be empty.
         * \param allowReversal \c true if and only if reversing some or all
         * link components is allowed when minimising the sequence of triples.
         * \param rotationOptions indicates whether we may rotate the entire
         * link diagram when minimising the sequence of triples.  This is a
         * BoolSet, which means rotation may be mandatory, forbidden, or
         * optional.  This BoolSet cannot be empty.
         */
        LinkSigData(const Link& link, BoolSet reflectionOptions,
                bool allowReversal, BoolSet rotationOptions);

        /**
         * Makes a new deep copy of the given data set.
         */
        LinkSigData(const LinkSigData&) = default;

        /**
         * Moves the contents of the given data set into this new data set.
         * This is a fast (constant time) operation.
         *
         * The data set that was passed will no longer be usable.
         */
        LinkSigData(LinkSigData&&) noexcept = default;

        /**
         * Returns the total number of crossings in the connected diagram
         * component that this data set describes.
         *
         * \return the total number of crossings.
         */
        size_t size() const {
            return size_;
        }

        /**
         * Gives read-only access to the full sequence of triples that
         * describe this diagram component.
         *
         * \python This routine returns a deep copy (not a reference), in the
         * form of a Python list.
         *
         * \return a reference to the full sequence of triples.
         */
        const Traversal& sequence() const {
            return sequence_;
        }

        /**
         * Moves the contents of the given data set into this data set.
         *
         * It does not matter whether this and the given data set refer to
         * diagram components of the same size, and/or with the same number of
         * topological components.
         *
         * The data set that was passed will no longer be usable.
         *
         * \return a reference to this data set.
         */
        LinkSigData& operator = (LinkSigData&&) noexcept = default;

        // Disable copy assignment, since we do not want to worry about
        // reallocation if the sequences of triples have different lengths.
        LinkSigData& operator = (const LinkSigData&) = delete;

        /**
         * Swaps the contents of this and the given diagram component data.
         *
         * This routine will behave correctly if \a other is in fact
         * this data set.
         *
         * \param other the component data whose contents should be swapped
         * with this.
         */
        void swap(LinkSigData& other) noexcept {
            std::swap(size_, other.size_);
            sequence_.swap(other.sequence_);
        }

        /**
         * Determines whether this and the given diagram component data hold
         * identical information.
         *
         * \return \c true if and only if this and the given data are identical.
         */
        bool operator == (const LinkSigData&) const = default;

        /**
         * Compares two sets of diagram component data.  Such comparisons are
         * useful when creating a signature for a disconnected link diagram.
         *
         * Broadly speaking, the intent is for the most significant components
         * of the link diagram to be sorted first.  Specifically, a data set
         * will be sorted first if it involves more crossings, and then if it
         * involves more topological link components; after this we fall back
         * to a lexicographical comparison of sequences of triples as
         * described in the LinkSigData class notes.
         *
         * This generates all of the usual comparison operators, including
         * `<`, `<=`, `>`, and `>=`.
         *
         * \param rhs the component data to compare with this.
         * \return the result of the comparison between this and the given
         * data.
         */
        std::strong_ordering operator <=> (const LinkSigData& rhs) const {
            // More crossings first.
            if (auto c = rhs.size_ <=> size_; c != 0)
                return c;
            // Longer sequences (i.e., more topological link components) first.
            if (auto c = rhs.sequence_.size() <=> sequence_.size(); c != 0)
                return c;
            // Lexicographically smaller sequences first.
            return std::lexicographical_compare_three_way(
                sequence_.begin(), sequence_.end(),
                rhs.sequence_.begin(), rhs.sequence_.end());
        }
};

/**
 * Swaps the contents of the given diagram component data sets.
 *
 * This global routine simply calls LinkSigData::swap(); it is provided so
 * that LinkSigData meets the C++ Swappable requirements.
 *
 * \param a the first component data set whose contents should be swapped.
 * \param b the second component data set whose contents should be swapped.
 *
 * \ingroup link
 */
inline void swap(LinkSigData& a, LinkSigData& b) noexcept {
    a.swap(b);
};

/**
 * Represents an encoding that can be used for knot/link signatures.
 * Essentially, the job of an encoding algorithm is to pack the information
 * describing a single connected diagram component into a small piece of data
 * (such as a string) that is easily transported.
 *
 * An encoding should provide a type alias `Signature`, indicating the type
 * that holds the final knot/link signature (e.g., `std::string`).  In addition,
 * it should provide the following static routines:
 *
 * - `encodeEmpty()`, which encodes the empty link;
 *
 * - `encodeUnknot()`, which encodes the zero-crossing unknot diagram;
 *
 * - `encode(const LinkSigData&)`, which encodes the information describing a
 *   single connected diagram component.  This routine may assume that the
 *   given data set has at least one crossing, and is minimal amongst all
 *   allowed relabellings of the underlying connected link diagram.  (Here
 *   "allowed" accounts for the fact that reflection, reversal and/or rotation
 *   may or may not be permitted depending upon context.)
 *
 * All three routines should return the type `Signature`.
 *
 * \apinotfinal
 *
 * \ingroup link
 */
template <typename T>
concept LinkSigEncoding =
    requires(const Link link, const LinkSigData data) {
        typename T::Signature;
        requires ConcatenableSequence<typename T::Signature>;
        { T::encodeEmpty() } -> std::convertible_to<typename T::Signature>;
        { T::encodeUnknot() } -> std::convertible_to<typename T::Signature>;
        { T::encode(data) } -> std::convertible_to<typename T::Signature>;
    };

/**
 * The default encoding to use for knot/link signatures.
 *
 * This printable encoding is the one that was used for knot/link signatures
 * in Regina ≤ 7.x.  It represents a signature as a `std::string`, using only
 * printable characters from the 7-bit ASCII range.
 *
 * See the LinkSigEncoding concept documentation for general details on
 * encodings for knot/link signatures.
 *
 * This class is designed to be used as a template parameter for Link::sig().
 * Typical users would have no need to call any of its functions directly.
 *
 * \python Python does not support C++ templates.  To use this encoding in
 * Python, you can simply call `Link::sig()` (since this encoding is the
 * default).
 *
 * \apinotfinal
 *
 * \ingroup link
 */
class LinkSigPrintable {
    public:
        /**
         * The data type that this encoding uses to hold the final signature.
         */
        using Signature = std::string;

        /**
         * Encodes the signature of the empty link.
         *
         * Note that LinkSigPrintable and LinkSigCompact do _not_ return an
         * empty signature for this; instead they both return the special
         * string `_`.
         *
         * \return the signature of the empty link.
         */
        static Signature encodeEmpty();

        /**
         * Encodes the signature of the zero-crossing unknot diagram.
         *
         * Both LinkSigPrintable and LinkSigCompact return the same signature
         * `a` in this case.
         *
         * \return the signature of the empty link.
         */
        static Signature encodeUnknot();

        /**
         * Encodes a single connected diagram component.
         *
         * \pre The given diagram component has at least one crossing, and is
         * minimal amongst all allowed relabellings of the underlying connected
         * link diagram.
         *
         * \param data the data describing a connected diagram component.
         * \return the given data encoded as a knot/link signature.
         */
        static Signature encode(const LinkSigData& data);

        /**
         * Precomputes the length of the signature that encodes the given
         * connected diagram component.
         *
         * \pre The given diagram component has at least one crossing, and is
         * minimal amongst all allowed relabellings of the underlying connected
         * link diagram.
         *
         * \param data the data describing a connected diagram component.
         * \return the length of the knot/link signature that encodes \a data.
         */
        static size_t length(const LinkSigData& data);

        // Make this class non-constructible.
        LinkSigPrintable() = delete;
};

/**
 * A compact string-based encoding for use with knot/link signatures.
 *
 * Like LinkSigPrintable, this encodes a signature as a `std::string` using
 * only printable characters from the 7-bit ASCII range.  However, the strings
 * it produces are significantly shorter (a little over half the length in
 * general).  These signatures are not compatible with the knot/link signatures
 * that were used in Regina ≤ 7.x; they are designed for scenarios where
 * memory usage needs to be kept as small as possible.
 *
 * See the LinkSigEncoding concept documentation for general details on
 * encodings for knot/link signatures.
 *
 * This class is designed to be used as a template parameter for Link::sig().
 * Typical users would have no need to call any of its functions directly.
 *
 * \python Python does not support C++ templates.  To use this encoding in
 * Python, you can call `Link::sig_Compact()`.
 *
 * \apinotfinal
 *
 * \ingroup link
 */
class LinkSigCompact {
    public:
        /**
         * The data type that this encoding uses to hold the final signature.
         */
        using Signature = std::string;

        /**
         * Encodes the signature of the empty link.
         *
         * Note that LinkSigPrintable and LinkSigCompact do _not_ return an
         * empty signature for this; instead they both return the special
         * string `_`.
         *
         * \return the signature of the empty link.
         */
        static Signature encodeEmpty();

        /**
         * Encodes the signature of the zero-crossing unknot diagram.
         *
         * Both LinkSigPrintable and LinkSigCompact return the same signature
         * `a` in this case.
         *
         * \return the signature of the empty link.
         */
        static Signature encodeUnknot();

        /**
         * Encodes a single connected diagram component.
         *
         * \pre The given diagram component has at least one crossing, and is
         * minimal amongst all allowed relabellings of the underlying connected
         * link diagram.
         *
         * \param data the data describing a connected diagram component.
         * \return the given data encoded as a knot/link signature.
         */
        static Signature encode(const LinkSigData& data);

        /**
         * Precomputes the length of the signature that encodes the given
         * connected diagram component.
         *
         * \pre The given diagram component has at least one crossing, and is
         * minimal amongst all allowed relabellings of the underlying connected
         * link diagram.
         *
         * \param data the data describing a connected diagram component.
         * \return the length of the knot/link signature that encodes \a data.
         */
        static size_t length(const LinkSigData& data);

        // Make this class non-constructible.
        LinkSigCompact() = delete;
};

} // namespace regina

#endif

