
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
 * \ifacespython Not present.
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
     * \tparam IntType a native C++ integer type, such as \c char, or
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
     * \tparam IntType a native C++ integer type, such as \c char,
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
     * Returns a single base64 character that encodes up to three trits.
     * A \e trit is either 0, 1 or 2.
     *
     * The given trits will be packed into a single base64 character, with
     * the first trit representing the lowest-significance bits of the
     * underlying integer and so on.
     *
     * The inverse to this routine is decodeTrits().
     *
     * @param trits an array containing the trits to encode; this must
     * contain at least \a nTrits characters, each of which is 0, 1 or 2.
     * @param nTrits the number of trits to encode; this must be at most 3.
     * @return the resulting printable base64 character.
     */
    static char encodeTrits(const char* trits, unsigned nTrits) {
        char ans = 0;
        if (nTrits >= 1)
            ans |= trits[0];
        if (nTrits >= 2)
            ans |= (trits[1] << 2);
        if (nTrits >= 3)
            ans |= (trits[2] << 4);
        return encodeSingle(ans);
    }

    /**
     * Decodes a single base64 into three trits.
     * A \e trit is either 0, 1 or 2.
     *
     * The inverse to this routine is encodeTrits(); see that routine
     * for details of the encoding.
     *
     * @param c the base64 character to decode.
     * @param result an array into which the resulting trits should be placed;
     * this must have size at least 3.
     */
    static void decodeTrits(char c, char* result) {
        unsigned val = decodeSingle(c);
        result[0] = val & 3;
        result[1] = (val >> 2) & 3;
        result[2] = (val >> 4) & 3;
    }
};

} // namespace regina

#endif
