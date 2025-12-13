
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

/*! \file utilities/sigutils.h
 *  \brief General helper tools for isomorphism signatures, knot signatures
 *  and the like.
 */

#ifndef __REGINA_SIGUTILS_H
#ifndef __DOXYGEN
#define __REGINA_SIGUTILS_H
#endif

#include <array>
#include <cstdint>
#include <string>
#include "regina-core.h"
#include "concepts/core.h"
#include "concepts/iterator.h"
#include "utilities/exception.h"
#include "utilities/fixedarray.h"

namespace regina {

/**
 * A deprecated set of helper tools for signatures that use base64 encodings.
 * These are (in particular) used in the default encodings for Regina's
 * own isomorphism signatures and knot signatures.
 *
 * This base64 encoding uses the characters: `a..zA..Z0..9+-`
 *
 * \warning Note that this base64 encoding uses a different set of
 * printable symbols from the encoding used in utilities/base64.h.
 * This should not be a problem: Regina uses this encoding exclusively for
 * signatures, and uses utilities/base64.h exclusively for encoding files.
 *
 * \deprecated This is now deprecated in favour of the new classes
 * Base64SigEncoder and Base64SigDecoder, which carry state and have better
 * error handling.
 *
 * \ingroup utilities
 */
struct [[deprecated]] Base64SigEncoding {
    /**
     * Determine the integer value represented by the given base64 character.
     *
     * The inverse to this routine is encodeSingle().
     *
     * \param c a base64 character, which must be one of the 64 printable
     * characters described in the class notes.
     * \return the corresponding integer, which will be between 0 and 63
     * inclusive.
     */
    static constexpr unsigned decodeSingle(char c) {
        if (c >= 'a' && c <= 'z')
            return (c - 'a');
        if (c >= 'A' && c <= 'Z')
            return (c - 'A' + 26);
        if (c >= '0' && c <= '9')
            return (c - '0' + 52);
        if (c == '+')
            return 62;
        return 63;
    }

    /**
     * Determine the base64 character that represents the given integer value.
     *
     * The inverse to this routine is decodeSingle().
     *
     * \param c an integer between 0 and 63 inclusive.
     * \return the corresponding printable base64 character.
     */
    static constexpr char encodeSingle(unsigned c) {
        if (c < 26)
            return (char(c) + 'a');
        if (c < 52)
            return (char(c - 26) + 'A');
        if (c < 62)
            return (char(c - 52) + '0');
        if (c == 62)
            return '+';
        return '-';
    }

    /**
     * Is the given character a valid base64 character?
     *
     * \return \c true if and only if the given character is one of the
     * 64 printable characters described in the class notes.
     */
    static constexpr bool isValid(char c) {
        return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '+' || c == '-');
    }

    /**
     * Append a base64 encoding of the given native C++ integer to the given
     * string.  The integer will be broken into \a nChars distinct 6-bit blocks,
     * and the lowest-significance blocks will be written first.
     *
     * The inverse to this routine is decodeInt().
     *
     * \pre The given integer \a val is non-negative, and fits within
     * 6<i>nChars</i> bits.
     *
     * \python The template argument \a IntType is taken to be a
     * native C++ \c long.
     *
     * \param s the string that resulting characters should be appended to.
     * \param val the integer to encode.
     * \param nChars the number of base64 characters to use.
     */
    template <CppInteger IntType>
    static void encodeInt(std::string& s, IntType val, int nChars) {
        for ( ; nChars > 0; --nChars) {
            s += encodeSingle(val & 0x3F);
            val >>= 6;
        }
    }

    /**
     * Read the integer that is encoded at the beginning of the given
     * base64 string.
     *
     * It will be assumed that the integer is encoded using \a nChars
     * base64 characters, each containing 6 bits of the integer,
     * with the lowest-significance bits encoded in the first characters.
     *
     * The result will be assembled using the integer type \a IntType,
     * via bitwise OR and bitwise shift lefts.  It is assumed that the
     * programmer has chosen an integer type large enough to contain whatever
     * values they expect to read.
     *
     * The inverse to this routine is encodeInt().
     *
     * \pre The given string contains at least \a nChars characters.
     *
     * \python The template argument \a IntType is taken to be a
     * native C++ \c long.
     *
     * \param s the string from which the encoded base64 characters
     * should be read.
     * \param nChars the number of base64 characters to read.
     * \return the native integer that was encoded.
     */
    template <CppInteger IntType>
    static IntType decodeInt(const char* s, int nChars) {
        IntType ans = 0;
        for (int i = 0; i < nChars; ++i)
            ans |= (static_cast<IntType>(decodeSingle(s[i])) << (6 * i));
        return ans;
    }

    /**
     * Returns a single base64 character that encodes up to three trits,
     * given using an input iterator.  A _trit_ is either 0, 1 or 2.
     *
     * The given trits will be packed into a single base64 character, with
     * the first trit representing the lowest-significance bits of the
     * underlying integer and so on.
     *
     * The inverse to this routine is decodeTrits().
     *
     * \nopython Instead you can use the variant of this routine that takes
     * the trits as a fixed-size array.
     *
     * \param trits an input iterator pointing to the first trit to encode;
     * it must be possible to read and advance this iterator at least
     * \a nTrits times.  Each trit must take the value 0, 1 or 2.
     * \param nTrits the number of trits to encode; this must be at most 3.
     * \return the resulting printable base64 character.
     */
    template <InputIteratorFor<uint8_t> iterator>
    static char encodeTrits(iterator trits, int nTrits) {
        uint8_t ans = 0;
        if (nTrits >= 1)
            ans |= static_cast<uint8_t>(*trits++);
        if (nTrits >= 2)
            ans |= (static_cast<uint8_t>(*trits++) << 2);
        if (nTrits >= 3)
            ans |= (static_cast<uint8_t>(*trits++) << 4);
        return encodeSingle(ans);
    }

    /**
     * Returns a single base64 character that encodes up to three trits,
     * given using a fixed-size array.  A _trit_ is either 0, 1 or 2.
     *
     * The given trits will be packed into a single base64 character, with
     * the first trit representing the lowest-significance bits of the
     * underlying integer and so on.
     *
     * The inverse to this routine is decodeTrits().
     *
     * \tparam nTrits the number of trits to encode; this must be between
     * 0 and 3 inclusive.
     *
     * \param trits the array of trits to encode.  Each trit must take
     * the value 0, 1 or 2.
     * \return the resulting printable base64 character.
     */
    template <int nTrits>
    static constexpr char encodeTrits(
            const std::array<uint8_t, nTrits>& trits) {
        static_assert(nTrits >= 0 && nTrits <= 3,
            "encodeTrits() will only encode between 0 and 3 trits inclusive.");

        uint8_t ans = 0;
        if constexpr (nTrits >= 1)
            ans |= trits[0];
        if constexpr (nTrits >= 2)
            ans |= (trits[1] << 2);
        if constexpr (nTrits >= 3)
            ans |= (trits[2] << 4);
        return encodeSingle(ans);
    }

    /**
     * Decodes a single base64 character into three trits, and returns
     * these using an output iterator.  A _trit_ is either 0, 1 or 2.
     *
     * The inverse to this routine is encodeTrits(); see that routine
     * for details of the encoding.
     *
     * \nopython Instead you can use the variant of this routine that takes
     * one argument and returns a fixed-size array.
     *
     * \param c the base64 character to decode.
     * \param result an output iterator pointing to the location where the
     * resulting trits will be stored; it must be possible to write and advance
     * this iterator at least three times.
     */
    template <std::output_iterator<uint8_t> iterator>
    static void decodeTrits(char c, iterator result) {
        auto val = static_cast<uint8_t>(decodeSingle(c));
        *result++ = static_cast<uint8_t>(val & 3);
        *result++ = static_cast<uint8_t>((val >> 2) & 3);
        *result++ = static_cast<uint8_t>((val >> 4) & 3);
    }

    /**
     * Decodes a single base64 character into three trits, and returns
     * these as a fixed-size array.  A _trit_ is either 0, 1 or 2.
     *
     * The inverse to this routine is encodeTrits(); see that routine
     * for details of the encoding.
     *
     * \param c the base64 character to decode.
     * \return an array containing the three trits that had been
     * encoded in the given base64 character.
     */
    static constexpr std::array<uint8_t, 3> decodeTrits(char c) {
        auto val = static_cast<uint8_t>(decodeSingle(c));
        return { static_cast<uint8_t>(val & 3),
                 static_cast<uint8_t>((val >> 2) & 3),
                 static_cast<uint8_t>((val >> 4) & 3) };
    }
};

/**
 * A helper class for writing signatures that use base64 encodings.
 * These are (in particular) used in the default encodings for Regina's
 * own isomorphism signatures and knot signatures.
 *
 * To use this class: create a new Base64SigEncoder, call one or more of its
 * member functions to write values to the encoding, and then call str() to
 * extract the resulting base64 string.
 *
 * This base64 encoding uses the characters: `a..zA..Z0..9+-`
 *
 * Baes64 encoders are single-use objects: they cannot be copied, moved or
 * swapped.
 *
 * \warning Note that this base64 encoding uses a different set of
 * printable symbols from the encoding used in utilities/base64.h.
 * This should not be a problem: Regina uses this encoding exclusively for
 * signatures, and uses utilities/base64.h exclusively for encoding files.
 *
 * \ingroup utilities
 */
class Base64SigEncoder {
    public:
        /**
         * A table of printable characters that are _not_ amongst the base64
         * characters used by Base64SigEncoder and Base64SigDecoder.
         *
         * These characters could (for example) be used to mark the boundaries
         * of base64 blocks, or to indicate special cases.
         *
         * These characters are presented as a string of length at least 3.
         * Future versions of Regina may append new characters to the end of
         * this string, but the existing characters `spare[0..2]` will not
         * change.
         */
        constexpr static char spare[] = "_./";

    private:
        std::string base64_;
            /**< The base64 encoding that has been constructed thus far. */

    public:
        /**
         * Creates a new encoder, with an empty base64 string.
         */
        Base64SigEncoder() = default;

        /**
         * Returns the base64 encoding that has been constructed thus far.
         *
         * \return The current base64 encoding.
         */
        const std::string& str() const & {
            return base64_;
        }

        /**
         * Moves the base64 encoding that has been constructed thus far
         * out of this encoder.
         *
         * After calling this function, this encoder object will be unusable.
         *
         * \nopython Instead use the variant of str() that returns its string
         * by constant reference.
         *
         * \return The current base64 encoding.
         */
        std::string&& str() && {
            return std::move(base64_);
        }

        /**
         * Encodes the given 6-bit integer using a single base64 character.
         *
         * The inverse to this routine is Base64SigDecoder::decodeSingle().
         *
         * \exception InvalidArgument The given integer is not between
         * 0 and 63 inclusive.
         *
         * \python The template argument \a IntType is taken to be a
         * native C++ \c long.
         *
         * \param c an integer between 0 and 63 inclusive.
         */
        template <CppInteger IntType>
        void encodeSingle(IntType c) {
            if (c < 0)
                throw InvalidArgument("Base64SigEncoder::encodeSingle(): "
                    "integer argument cannot be negative");
            else if (c < 26)
                base64_ += (char(c) + 'a');
            else if (c < 52)
                base64_ += (char(c - 26) + 'A');
            else if (c < 62)
                base64_ += (char(c - 52) + '0');
            else if (c == 62)
                base64_ += '+';
            else if (c == 63)
                base64_ += '-';
            else
                throw InvalidArgument("Base64SigEncoder::encodeSingle(): "
                    "integer argument out of range");
        }

        /**
         * Encodes the given non-negative integer (typically representing the
         * size of some object), without knowing in advance how many base64
         * characters will be required.
         *
         * A typical use case would be where \a size represents the number of
         * top-dimensional simplices in a triangulation, or the number of
         * crossings in a link diagram.
         *
         * This routine also computes the smallest integer \a b with the
         * property that any integer \a x between 0 and \a size inclusive can
         * be encoded using \a b base64 characters.  In other words, any such
         * \a x can be encoded by calling `encodeInt(x, b)`.  Typically
         * these \a x would be _indices_ into an object (e.g., top-dimensional
         * simplex numbers, or crossing numbers).  Note that encodeSize()
         * itself might write more than \a b characters.
         *
         * The inverse to this routine is Base64SigDecoder::decodeSize().
         *
         * \param size the non-negative integer to encode.
         * \return nChars the number of base64 characters required to write
         * any integer between 0 and \a size inclusive.
         */
        int encodeSize(size_t size) {
            // There is a theoretical upper limit on \a size: the return value
            // b must fit into a native int.  Even if int is only 16-bit, this
            // translates to size < 2^(6×2^15), which is not going to be a
            // problem for any native IntType.

            if (size < 63) {
                // Keep it simple for small objects: 1 character per integer.
                encodeSingle(size);
                return 1;
            } else {
                // For large objects, start with a special marker followed by
                // the number of characters per integer.
                int charsPerInt = 0;
                size_t tmp = size;
                while (tmp > 0) {
                    tmp >>= 6;
                    ++charsPerInt;
                }
                encodeSingle(63);
                encodeSingle(charsPerInt);
                encodeInt(size, charsPerInt);
                return charsPerInt;
            }
        }

        /**
         * Encodes the given non-negative native C++ integer using a fixed
         * number of base64 characters.
         *
         * Specifically, the integer \a val will be broken into \a nChars
         * distinct 6-bit blocks, which will be encoded in order from lowest
         * to highest significance.
         *
         * The inverse to this routine is Base64SigDecoder::decodeInt().
         *
         * \exception InvalidArgument The given integer \a val is negative,
         * or requires more than `6 × nChars` bits.
         *
         * \python The template argument \a IntType is taken to be a
         * native C++ \c long.
         *
         * \param val the non-negative integer to encode.
         * \param nChars the number of base64 characters to use; typically
         * this would be obtained through an earlier call to encodeSize().
         */
        template <CppInteger IntType>
        void encodeInt(IntType val, int nChars) {
            if (val < 0)
                throw InvalidArgument("Base64SigEncoder::encodeInt(): "
                    "integer argument cannot be negative");

            for ( ; nChars > 0; --nChars) {
                encodeSingle(val & 0x3F);
                val >>= 6;
            }

            if (val != 0)
                throw InvalidArgument("Base64SigEncoder::encodeInt(): "
                    "integer argument out of range");
        }

        /**
         * Encodes a sequence of non-negative native C++ integers, each using a
         * fixed number of base64 characters.
         *
         * Each integer in the sequence will be encoded using encodeInt().
         * That is, each integer will be broken into \a nChars distinct
         * 6-bit blocks, which will be encoded in order from lowest to highest
         * significance.
         *
         * The inverse to this routine is Base64SigDecoder::decodeInts().
         *
         * \exception InvalidArgument Some integer in the sequence is negative,
         * or requires more than `6 × nChars` bits.
         *
         * \python Instead of a begin/end pair of iterators, this routine takes
         * a Python sequence of integers.  Each Python integer will be read as
         * a native C++ `long`.
         *
         * \param begin an iterator pointing to the first integer to encode.
         * \param end a past-the-end iterator pointing beyond the last integer
         * to encode.
         * \param nChars the number of base64 characters to use for each
         * integer; typically this would be obtained through an earlier call
         * to encodeSize().
         */
        template <std::input_iterator iterator>
        requires CppInteger<typename std::iterator_traits<iterator>::value_type>
        void encodeInts(iterator begin, iterator end, int nChars) {
            for (auto it = begin; it != end; ++it)
                encodeInt(*it, nChars);
        }

        /**
         * Encodes a sequence of trits.  A _trit_ is either 0, 1 or 2.
         *
         * The trits will be packed into base64 characters, three at a time.
         * For each individual base64 character, the three trits will use bits
         * of the underlying 6-bit integer in order from lowest to highest
         * significance.  (The last base64 character might of course encode
         * just one or two trits instead.)
         *
         * Each trit will be obtained by dereferencing an iterator, which
         * (as noted above) must yield the value 0, 1 or 2.
         *
         * The inverse to this routine is Base64SigDecoder::decodeTrits(),
         * though that function only decodes three trits at a time.
         *
         * \python This routine takes a single argument, which is a Python
         * sequence of integer trits.
         *
         * \param beginTrits an iterator pointing to the first trit to encode.
         * \param endTrits a past-the-end iterator pointing beyond the last
         * trit to encode.
         */
        template <InputIteratorFor<uint8_t> iterator>
        void encodeTrits(iterator beginTrits, iterator endTrits) {
            auto it = beginTrits;
            while (it != endTrits) {
                uint8_t packed = static_cast<uint8_t>(*it++);
                if (it == endTrits) {
                    encodeSingle(packed);
                    return;
                }
                packed |= (static_cast<uint8_t>(*it++) << 2);
                if (it == endTrits) {
                    encodeSingle(packed);
                    return;
                }
                packed |= (static_cast<uint8_t>(*it++) << 4);
                encodeSingle(packed);
            }
        }

        /**
         * Appends the given character verbatim to this encoding.
         *
         * \param c the character to append.  This need not be one of the 64
         * characters used in this base64 encoding; however, ideally it should
         * be printable.
         */
        void append(char c) {
            base64_ += c;
        }

        Base64SigEncoder(const Base64SigEncoder&) = delete;
        Base64SigEncoder& operator = (const Base64SigEncoder&) = delete;
};

/**
 * A helper class for reading signatures that use base64 encodings.
 * These are (in particular) used in the default encodings for Regina's
 * own isomorphism signatures and knot signatures.
 *
 * To use this class: create a new Base64SigDecoder by passing details of the
 * encoded string to its constructor, and then call its `decode...()` member
 * functions to read values sequentially from the encoding.
 *
 * This class will keep track of a current position in the encoded string.
 * Each call to a `decode...()` member function will advance this position
 * accordingly (but never beyond the end of the string).
 *
 * This base64 encoding uses the characters: `a..zA..Z0..9+-`
 *
 * Baes64 decoders are single-use objects: they cannot be copied, moved or
 * swapped.
 *
 * \warning Note that this base64 encoding uses a different set of
 * printable symbols from the encoding used in utilities/base64.h.
 * This should not be a problem: Regina uses this encoding exclusively for
 * signatures, and uses utilities/base64.h exclusively for encoding files.
 *
 * \python The type \a Iterator is an implementation detail, and is hidden
 * from Python users.  Just use the unadorned type name `Base64SigDecoder`.
 *
 * \ingroup utilities
 */
template <ForwardIteratorFor<char> Iterator>
class Base64SigDecoder {
    private:
        Iterator next_;
            /**< The current position in the encoded string. */
        Iterator end_;
            /**< The end of the encoded string (specifically, a past-the-end
                 location, as is usual for an iterator range). */

    public:
        /**
         * Creates a new decoder for the given encoded string.
         *
         * The string itself should be passed as an iterator range.
         * This iterator range must remain valid for the entire lifespan
         * of this decoder.
         *
         * \python Instead of an iterator range, this constructor takes a
         * Python string.  In Python (but not C++), the decoder will also keep
         * a deep copy of the string, to ensure the lifespan requirements.
         *
         * \param encoding an iterator pointing to the beginning of the
         * encoded string.
         * \param end a past-the-end iterator that marks the end of the
         * encoded string.
         * \param skipInitialWhitespace \c true if the current position should
         * immediately advance past any initial whitespace in the given string.
         */
        Base64SigDecoder(Iterator encoding, Iterator end,
                bool skipInitialWhitespace = true) :
                next_(encoding), end_(end) {
            if (skipInitialWhitespace) {
                while (next_ != end_ && ::isspace(*next_))
                    ++next_;
            }
        }

        /**
         * Moves the current position past any whitespace.
         *
         * The movement will stop upon reaching either a non-whitespace
         * character or the end of the string.
         */
        void skipWhitespace() {
            while (next_ != end_ && ::isspace(*next_))
                ++next_;
        }

        /**
         * Determines whether the current position has reached the end of the
         * string.
         *
         * \param ignoreWhitespace \c true if we should ignore any trailing
         * whitespace.  If there is whitespace at the current position, the
         * current position will not be changed; this will merely make the
         * test succeed if _only_ whitespace characters remain.
         * \return \c true if and only if the current position is the end of
         * the string.
         */
        bool done(bool ignoreWhitespace = true) const {
            if (ignoreWhitespace) {
                for (Iterator pos = next_; pos != end_; ++pos)
                    if (! ::isspace(*pos))
                        return false;
                return true;
            } else {
                return next_ == end_;
            }
        }

        /**
         * Returns the character at the current position in the encoded string.
         * The current position will not move.
         *
         * \return the character at the current position, or 0 if there are no
         * more characters available.
         */
        char peek() const {
            return (next_ == end_ ? 0 : *next_);
        }

        /**
         * Advances to the next position in the encoded string.
         *
         * \pre The current position has not yet reached the end of the string.
         */
        void skip() {
            ++next_;
        }

        /**
         * Decodes the 6-bit integer value represented by the next single
         * base64 character.
         *
         * The inverse to this routine is Base64SigEncoder::encodeSingle().
         *
         * \exception InvalidInput There are no more characters remaining in
         * the encoded string, or the next character is not a valid base64
         * character.
         *
         * \python The template argument \a IntType is taken to be a
         * native C++ \c long.
         *
         * \return the corresponding integer, which will be between 0 and 63
         * inclusive.
         */
        template <CppInteger IntType>
        IntType decodeSingle() {
            if (next_ == end_)
                throw InvalidInput("Base64SigDecoder: "
                    "unexpected end of encoded string");

            char c = *next_++;
            if (c >= 'a' && c <= 'z')
                return (c - 'a');
            if (c >= 'A' && c <= 'Z')
                return (c - 'A' + 26);
            if (c >= '0' && c <= '9')
                return (c - '0' + 52);
            if (c == '+')
                return 62;
            if (c == '-')
                return 63;
            throw InvalidInput("Base64SigDecoder: "
                "invalid base64 character in encoded string");
        }

        /**
         * Decodes the next non-negative integer value (typically representing
         * the size of some object), without knowing in advance how many
         * base64 characters were used to encode it.  This integer value must
         * have been encoded using Base64SigEncoder::encodeSize().
         *
         * A typical use case would be where \a size represents the number of
         * top-dimensional simplices in a triangulation, or the number of
         * crossings in a link diagram.
         *
         * This routine also returns the smallest integer \a b with the property
         * that any integer \a x between 0 and \a size inclusive can be encoded
         * using \a b base64 characters.  Typically these \a x would be
         * _indices_ into an object (e.g., top-dimensional simplex numbers, or
         * crossing numbers).  More precisely, \a b is the same integer that
         * was returned when \a size was encoded using encodeSize().
         * Typically you would pass \a b to subsequent calls to decodeInt().
         *
         * The inverse to this routine is Base64SigEncoder::encodeSize().
         *
         * \exception InvalidInput There are not enough characters available
         * in the encoded string, or a character was encountered that was not
         * a valid base64 character.
         *
         * \return a pair (\a size, \a b), where \a size is the integer that
         * was decoded, and \a b is the number of base64 characters described
         * above.
         */
        std::pair<size_t, int> decodeSize() {
            int first = decodeSingle<int>();
            if (first < 63)
                return { first, 1 };
            else {
                int charsPerInt = decodeSingle<int>();
                size_t n = decodeInt<size_t>(charsPerInt);
                return { n, charsPerInt };
            }
        }

        /**
         * Decodes the next non-negative integer value, assuming this uses
         * a fixed number of base64 characters.  This integer value would
         * typically have been encoded using Base64SigEncoder::encodeInt(),
         * using the same \a nChars argument.
         *
         * Specifically, it will be assumed that the integer has been broken
         * into \a nChars 6-bit blocks, with each block encoded as a single
         * base64 character, and with the blocks presented in order from
         * lowest to highest significance.
         *
         * The result will be assembled using the integer type \a IntType,
         * via bitwise OR and bitwise shift lefts.  It is assumed that the
         * programmer has chosen an integer type large enough to contain
         * whatever values they expect to read.
         *
         * The inverse to this routine is Base64SigEncoder::encodeInt().
         *
         * \exception InvalidInput There are fewer than \a nChars characters
         * available in the encoded string, or a character was encountered
         * that was not a valid base64 character.
         *
         * \python The template argument \a IntType is taken to be a
         * native C++ \c long.
         *
         * \param nChars the number of base64 characters to read.
         * \return the integer that was decoded.
         */
        template <CppInteger IntType>
        IntType decodeInt(int nChars) {
            IntType ans = 0;
            for (int i = 0; i < nChars; ++i)
                ans |= (decodeSingle<IntType>() << (6 * i));
            return ans;
        }

        /**
         * Decodes a sequence of non-negative integer values, assuming that
         * each individual value uses a fixed number of base64 characters,
         * and returns these as native C++ integers via an output iterator.
         * Each integer to be decoded would typically have been encoded using
         * Base64SigEncoder::encodeInt() or Base64SigEncoder::encodeInts(),
         * using the same \a nChars argument.
         *
         * Specifically, it will be assumed that each integer has been broken
         * into \a nChars 6-bit blocks, with each block encoded as a single
         * base64 character, and with the blocks presented in order from
         * lowest to highest significance.
         *
         * Each resulting integer will be assembled using the output iterator's
         * `value_type`, via bitwise OR and bitwise shift lefts.  It is assumed
         * that the programmer has chosen an output iterator whose `value_type`
         * is large enough to contain whatever integer values they expect to
         * read.
         *
         * The inverse to this routine is Base64SigEncoder::encodeInts().
         *
         * \exception InvalidInput There are fewer than `count × nChars`
         * characters available in the encoded string, or a character was
         * encountered that was not a valid base64 character.
         *
         * \nopython Instead you can use the variant of this routine that does
         * not take an output iterator, but instead returns the sequence of
         * integers that were decoded.
         *
         * \param output an iterator to use for output.  Each integer that
         * is decoded will be passed to this iterator using the usual
         * dereference-assign-increment pattern (`*output++ = value`).
         * It is assumed that this output iterator is able to accept \a count
         * values in this way.
         * \param count the number of integers to decode.
         * \param nChars the number of base64 characters to read.
         */
        template <std::output_iterator<int> iterator>
        requires CppInteger<typename std::iterator_traits<iterator>::value_type>
        void decodeInts(iterator output, size_t count, int nChars) {
            using IntType = typename std::iterator_traits<iterator>::value_type;
            for (size_t i = 0; i < count; ++i)
                *output++ = decodeInt<IntType>(nChars);
        }

        /**
         * Decodes a sequence of non-negative integer values, assuming that
         * each individual value uses a fixed number of base64 characters,
         * and returns these as an array of native C++ integers.
         * Each integer to be decoded would typically have been encoded using
         * Base64SigEncoder::encodeInt() or Base64SigEncoder::encodeInts(),
         * with the same \a nChars argument.
         *
         * Specifically, it will be assumed that each integer has been broken
         * into \a nChars 6-bit blocks, with each block encoded as a single
         * base64 character, and with the blocks presented in order from
         * lowest to highest significance.
         *
         * Each resulting integer will be assembled using the integer type
         * \a IntType, via bitwise OR and bitwise shift lefts.  It is assumed
         * that the programmer has chosen an integer type large enough to
         * contain whatever values they expect to read.
         *
         * The inverse to this routine is Base64SigEncoder::encodeInts().
         *
         * \exception InvalidInput There are fewer than `count × nChars`
         * characters available in the encoded string, or a character was
         * encountered that was not a valid base64 character.
         *
         * \python The template argument \a IntType is taken to be a
         * native C++ \c long.  This routine returns a Python list of integers.
         *
         * \param count the number of integers to decode.
         * \param nChars the number of base64 characters to read.
         * \return the sequence of integers that were decoded.
         */
        template <CppInteger IntType>
        FixedArray<IntType> decodeInts(size_t count, int nChars) {
            FixedArray<IntType> ans(count);
            for (auto it = ans.begin(); it != ans.end(); ++it)
                *it = decodeInt<IntType>(nChars);
            return ans;
        }

        /**
         * Decodes three trits from a single base64 character, and returns
         * these using an output iterator.  A _trit_ is either 0, 1 or 2.
         *
         * The inverse to this routine is Base64SigEncoder::encodeTrits(); see
         * that routine for details of the encoding.
         *
         * \exception InvalidInput There are no more characters remaining in
         * the encoded string, or the next character is not a valid base64
         * character.
         *
         * \nopython Instead you can use the variant of this routine that takes
         * no arguments and returns a fixed-size array.
         *
         * \param result an output iterator pointing to the location where the
         * resulting trits will be stored; it must be possible to write and
         * advance this iterator at least three times.
         */
        template <std::output_iterator<uint8_t> iterator>
        void decodeTrits(iterator result) {
            uint8_t val = decodeSingle<uint8_t>();
            *result++ = static_cast<uint8_t>(val & 3);
            *result++ = static_cast<uint8_t>((val >> 2) & 3);
            *result++ = static_cast<uint8_t>((val >> 4) & 3);
        }

        /**
         * Decodes three trits from a single base64 character, and returns
         * these as a fixed-size array.  A _trit_ is either 0, 1 or 2.
         *
         * The inverse to this routine is Base64SigEncoder::encodeTrits(); see
         * that routine for details of the encoding.
         *
         * \exception InvalidInput There are no more characters remaining in
         * the encoded string, or the next character is not a valid base64
         * character.
         *
         * \return an array containing the three trits that were decoded.
         */
        std::array<uint8_t, 3> decodeTrits() {
            uint8_t val = decodeSingle<uint8_t>();
            return { static_cast<uint8_t>(val & 3),
                     static_cast<uint8_t>((val >> 2) & 3),
                     static_cast<uint8_t>((val >> 4) & 3) };
        }

        /**
         * Is the given character one of the printable base64 characters
         * recognised by this class?
         *
         * \param c the character to examine.
         * \return \c true if and only if \a c is one of the 64 printable
         * characters described in the class notes.
         */
        static constexpr bool isValid(char c) {
            return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                (c >= '0' && c <= '9') || c == '+' || c == '-');
        }

        Base64SigDecoder(const Base64SigDecoder&) = delete;
        Base64SigDecoder& operator = (const Base64SigDecoder&) = delete;
};

} // namespace regina

#endif
