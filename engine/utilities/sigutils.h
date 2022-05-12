
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

/*! \file utilities/sigutils.h
 *  \brief General helper tools for isomorphism signatures, knot signatures
 *  and the like.
 */

#ifndef __REGINA_SIGUTILS_H
#ifndef __DOXYGEN
#define __REGINA_SIGUTILS_H
#endif

#include <array>
#include "regina-core.h"

namespace regina {

/**
 * General helper tools for signatures that use base64 encodings.
 * These are (in particular) used in the default encodings for Regina's
 * own isomorphism signatures and knot signatures.
 *
 * This base64 encoding uses the characters: <tt>a..zA..Z0..9+-</tt>
 *
 * \warning Note that this base64 encoding uses a different set of
 * printable symbols from the encoding used in utilities/base64.h.
 * This should not be a problem: Regina uses this encoding exclusively for
 * signatures, and uses utilities/base64.h exclusively for encoding files.
 *
 * \ingroup utilities
 */
struct Base64SigEncoding {
    /**
     * Determine the integer value represented by the given base64 character.
     *
     * The inverse to this routine is encodeSingle().
     *
     * @param c a base64 character, which must be one of the 64 printable
     * characters described in the class notes.
     * @return the corresponding integer, which will be between 0 and 63
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
     * @param c an integer between 0 and 63 inclusive.
     * @return the corresponding printable base64 character.
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
     * @return \c true if and only if the given character is one of the
     * 64 printable characters described in the class notes.
     */
    static constexpr bool isValid(char c) {
        return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '+' || c == '-');
    }

    /**
     * Append a base64 encoding of the given integer to the given string.
     * The integer will be broken into \a nChars distinct 6-bit blocks, and
     * the lowest-significance blocks will be written first.
     *
     * The inverse to this routine is decodeInt().
     *
     * \pre The given integer \a val is non-negative, and fits within
     * 6<i>nChars</i> bits.
     *
     * \ifacespython The template argument \a IntType is taken to be a
     * native C++ \c long.
     *
     * \tparam IntType a native C++ integer type, such as \c uint8_t, or
     * \c unsigned, or <tt>long long</tt>.
     *
     * @param s the string that resulting characters should be appended to.
     * @param val the integer to encode.
     * @param nChars the number of base64 characters to use.
     */
    template <typename IntType>
    static void encodeInt(std::string& s, IntType val, unsigned nChars) {
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
     * The inverse to this routine is encodeInt().
     *
     * \pre The given string contains at least \a nChars characters.
     *
     * \ifacespython The template argument \a IntType is taken to be a
     * native C++ \c long.
     *
     * \tparam IntType a native C++ integer type, such as \c uint8_t,
     * or \c unsigned, or <tt>long long</tt>.  The result will be
     * assembled using bitwise OR and bitwise shift lefts, and it is
     * assumed that the programmer has chosen an integer type large enough
     * to contain whatever values they expect to read.
     *
     * @param s the string from which the encoded base64 characters
     * should be read.
     * @param nChars the number of base64 characters to read.
     * @return the native integer that was encoded.
     */
    template <typename IntType>
    static IntType decodeInt(const char* s, unsigned nChars) {
        IntType ans = 0;
        for (unsigned i = 0; i < nChars; ++i)
            ans |= (static_cast<IntType>(decodeSingle(s[i])) << (6 * i));
        return ans;
    }

    /**
     * Returns a single base64 character that encodes up to three trits,
     * given using an input iterator.  A \e trit is either 0, 1 or 2.
     *
     * The given trits will be packed into a single base64 character, with
     * the first trit representing the lowest-significance bits of the
     * underlying integer and so on.
     *
     * The inverse to this routine is decodeTrits().
     *
     * \ifacespython Not present; instead you can use the variant of
     * this routine that takes the trits as a fixed-size array.
     *
     * @param trits an input iterator pointing to the first trit to encode;
     * it must be possible to read and advance this iterator at least
     * \a nTrits times.  Each trit will be cast to a \c uint8_t, and must take
     * the value 0, 1 or 2.
     * @param nTrits the number of trits to encode; this must be at most 3.
     * @return the resulting printable base64 character.
     */
    template <typename InputIterator>
    static char encodeTrits(InputIterator trits, unsigned nTrits) {
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
     * given using a fixed-size array.  A \e trit is either 0, 1 or 2.
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
     * @param trits the array of trits to encode.  Each trit must take
     * the value 0, 1 or 2.
     * @return the resulting printable base64 character.
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
     * these using an output iterator.  A \e trit is either 0, 1 or 2.
     *
     * The inverse to this routine is encodeTrits(); see that routine
     * for details of the encoding.
     *
     * \ifacespython Not present; instead you can use the variant of
     * this routine that takes one argument and returns a fixed-size array.
     *
     * @param c the base64 character to decode.
     * @param result an output iterator pointing to the location where the
     * resulting trits will be stored; it must be possible to write and advance
     * this iterator at least three times.  Each trit will be written as a
     * \c uint8_t.
     */
    template <typename OutputIterator>
    static void decodeTrits(char c, OutputIterator result) {
        uint8_t val = static_cast<uint8_t>(decodeSingle(c));
        *result++ = val & 3;
        *result++ = (val >> 2) & 3;
        *result++ = (val >> 4) & 3;
    }

    /**
     * Decodes a single base64 character into three trits, and returns
     * these as a fixed-size array.  A \e trit is either 0, 1 or 2.
     *
     * The inverse to this routine is encodeTrits(); see that routine
     * for details of the encoding.
     *
     * @param c the base64 character to decode.
     * @return an array containing the three trits that had been
     * encoded in the given base64 character.
     */
    static constexpr std::array<uint8_t, 3> decodeTrits(char c) {
        uint8_t val = static_cast<uint8_t>(decodeSingle(c));
        return { static_cast<uint8_t>(val & 3),
                 static_cast<uint8_t>((val >> 2) & 3),
                 static_cast<uint8_t>((val >> 4) & 3) };
    }
};

} // namespace regina

#endif
