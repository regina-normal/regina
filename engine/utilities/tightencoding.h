
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

/*! \file utilities/tightencoding.h
 *  \brief Provides short ASCII printable encodings for various objects.
 */

#ifndef __REGINA_TIGHTENCODING_H
#ifndef __DOXYGEN
#define __REGINA_TIGHTENCODING_H
#endif

#include <iostream>
#include <string>
#include <sstream>
#include "utilities/exception.h"

namespace regina {

template <bool>
class IntegerBase;

/*! \page tight Tight encodings of data
 *
 * Regina includes support for <i>tight encodings</i>, which are
 * encodings of various data types as short printable strings.
 *
 * Tight encodings have the following properties:
 *
 * - They use only printable ASCII characters (the 94 ASCII values from
 *   33 to 126 inclusive), and do not contain any whitespace.
 *
 * - They aim to be short (typically much shorter than the usual human-readable
 *   representations, such as decimal representations of integers or full text
 *   representations of polynomials).
 *
 * - When reading an encoded object character-by-character, the encoding
 *   contains enough information to know when the last character has been read
 *   (even without the foreknowledge of whether the input stream contains more
 *   characters).
 *
 * - Objects with the same inherent value, even if they use different
 *   underlying C++ types, will encode to the same string.  For example,
 *   the integer 7 will have the same encoding regardless of whether it
 *   is stored as an \c int, a \c long, or a regina::Integer.  Note that
 *   this guarantee only extends to types that "conceptually" intend to
 *   represent the same broad types of objects, possibly with different
 *   limitations.  So, for example, there is no guarantee that the integer 7,
 *   the rational 7/1, and/or the constant \e polynomial 7 would encode to
 *   the same string.
 *
 * - Conversely, objects of the same type but with different inherent values
 *   will encode to different strings.  So, for example, the integers 7 and -7
 *   will have different encodings.
 *
 * A consequence of the last two points is that, if the \e type of an object
 * is known in advance, then its \e value can in theory be recovered from its
 * encoding.  However, the encoding does not contain enough information
 * to deduce the type if this is not already known.
 *
 * Because encodings contain enough information to identify where they end,
 * this means that you can encode a \e sequence of objects by concatenating
 * the individual encodings with no separators, and (assuming the types
 * of the objects are fixed) this will be enough to guarantee that
 * different \e sequences likewise have different encodngs.
 *
 * For most types, Regina also provides \e decoding routines.  Although these
 * might not be offered for all types, it should (as noted above) be possible in
 * theory if the underlying types are known.  The main reason for not offering
 * a decoding function is simply that decoding is often not a priority for the
 * developers - tight encodings were originally designed for applications such
 * as perfect hashing, where the aim is essentially to "compress" the data in a
 * short printable string whilst preserving the correctness of equality tests.
 *
 * For native C++ data types where tight encodings and/or decodings are
 * supported, these are provided in the header utilities/tightencoding.h
 * through overloads of the global functions tightEncoding() (which returns a
 * string), tightEncode() (which writes to an output stream), and tightDecode()
 * (which takes either a string or an input stream as its argument).
 *
 * For Regina's own data types where tight encodings and/or decodings are
 * supported, these are provided through tightEncode(), tightEncoding(), and
 * static tightDecode() member functions of the corresponding classes.
 */

/**
 * Writes the tight encoding of the given signed integer to the given
 * output stream.  See the page on \ref tight "tight encodings" for details.
 *
 * \ifacespython Not present; use regina::tightEncoding(int) instead.
 *
 * @param out the output stream to which the encoded string will be written.
 * @param value the integer to encode.
 *
 * \ingroup utilities
 */
void tightEncode(std::ostream& out, int value);

/**
 * Returns the tight encoding of the given signed integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * @param value the integer to encode.
 * @return the resulting encoded string.
 *
 * \ingroup utilities
 */
std::string tightEncoding(int value);

/**
 * Writes the tight encoding of the given signed long integer to the given
 * output stream.  See the page on \ref tight "tight encodings" for details.
 *
 * \ifacespython Not present; use regina::tightEncoding(long) instead.
 *
 * @param out the output stream to which the encoded string will be written.
 * @param value the integer to encode.
 *
 * \ingroup utilities
 */
void tightEncode(std::ostream& out, long value);

/**
 * Returns the tight encoding of the given signed long integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * @param value the integer to encode.
 * @return the resulting encoded string.
 *
 * \ingroup utilities
 */
std::string tightEncoding(long value);

/**
 * Writes the tight encoding of the given signed long long integer to the given
 * output stream.  See the page on \ref tight "tight encodings" for details.
 *
 * \ifacespython Not present; use regina::tightEncoding(long long) instead.
 *
 * @param out the output stream to which the encoded string will be written.
 * @param value the integer to encode.
 *
 * \ingroup utilities
 */
void tightEncode(std::ostream& out, long long value);

/**
 * Returns the tight encoding of the given signed long long integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * @param value the integer to encode.
 * @return the resulting encoded string.
 *
 * \ingroup utilities
 */
std::string tightEncoding(long long value);

/**
 * Writes the tight encoding of the given unsigned integer to the given
 * output stream.  See the page on \ref tight "tight encodings" for details.
 *
 * \ifacespython Not present; use regina::tightEncoding(int) instead.
 *
 * @param out the output stream to which the encoded string will be written.
 * @param value the integer to encode.
 *
 * \ingroup utilities
 */
void tightEncode(std::ostream& out, unsigned value);

/**
 * Returns the tight encoding of the given unsigned integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * @param value the integer to encode.
 * @return the resulting encoded string.
 *
 * \ingroup utilities
 */
std::string tightEncoding(unsigned value);

/**
 * Writes the tight encoding of the given unsigned long integer to the given
 * output stream.  See the page on \ref tight "tight encodings" for details.
 *
 * \ifacespython Not present; use regina::tightEncoding(long) instead.
 *
 * @param out the output stream to which the encoded string will be written.
 * @param value the integer to encode.
 *
 * \ingroup utilities
 */
void tightEncode(std::ostream& out, unsigned long value);

/**
 * Returns the tight encoding of the given unsigned long integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * @param value the integer to encode.
 * @return the resulting encoded string.
 *
 * \ingroup utilities
 */
std::string tightEncoding(unsigned long value);

/**
 * Writes the tight encoding of the given unsigned long long integer to the
 * given output stream.  See the page on \ref tight "tight encodings" for
 * details.
 *
 * \ifacespython Not present; use regina::tightEncoding(long long) instead.
 *
 * @param out the output stream to which the encoded string will be written.
 * @param value the integer to encode.
 *
 * \ingroup utilities
 */
void tightEncode(std::ostream& out, unsigned long long value);

/**
 * Returns the tight encoding of the given unsigned long long integer.
 * See the page on \ref tight "tight encodings" for details.
 *
 * @param value the integer to encode.
 * @return the resulting encoded string.
 *
 * \ingroup utilities
 */
std::string tightEncoding(unsigned long long value);

/**
 * Reconstructs an integer from its given tight encoding.
 * See the page on \ref tight "tight encodings" for details.
 *
 * The tight encoding will be given as a string.  If this string contains
 * leading whitespace or any trailing characters at all (including trailing
 * whitespace), then it will be treated as an invalid encoding (i.e., this
 * routine will throw an exception).
 *
 * This routine does recognise infinity in the case where \a Int is the type
 * regina::LargeInteger.
 *
 * If \a Int is one of Regina's arbitrary precision integer types, then
 * this routine is identical to calling Int::tightDecode().
 *
 * \headers Some components of this routine are implemented in a separate
 * header (tightencoding-impl.h), which is not included automatically by this
 * file.  Most end users should not need this extra header, since Regina's
 * calculation engine already includes explicit instantiations for all of the
 * integer types that have corresponding global tightEncode() functions
 * (i.e., signed and unsigned \c int, \c long, and \c long \c long, plus
 * regina::Integer and regina::LargeInteger).
 *
 * \exception InvalidArgument the given string is not a tight encoding of
 * an integer of type \a Int.  This includes the case where the encoding
 * \e is a valid integer encoding but the integer itself is outside the
 * allowed range for the \a Int type.
 *
 * \ifacespython Since Python does not support templates, the interface
 * for this routine is a little different.  The global routine
 * regina::tightDecode() will return a Python integer; since these are
 * arbitrary precision, the decoding will never encounter an out-of-range
 * exceptions as it might with a native C++ integer type.  If you want one of
 * Regina's arbitrary precision integer types, you should call the corresponding
 * tightDecode() member function instead (e.g., regina::Integer::tightDecode()).
 *
 * \tparam Int The type of integer to reconstruct; this must be either (i) a
 * native C++ integer type (and not \c bool), or (ii) one of Regina's arbitrary
 * precision integer types (i.e., regina::Integer or regina::LargeInteger).
 *
 * @param enc the tight encoding for an integer.
 * @return the integer represented by the given tight encoding.
 */
template<typename Int>
Int tightDecode(const std::string& enc);

/**
 * Reconstructs an integer from its given tight encoding.
 * See the page on \ref tight "tight encodings" for details.
 *
 * The tight encoding will be read from the given input stream.  If the input
 * stream contains leading whitespace then it will be treated as an invalid
 * encoding (i.e., this routine will throw an exception).  The input routine
 * \e may contain further data: if this routine is successful then the input
 * stream will be left positioned immediately after the encoding, without
 * skipping any trailing whitespace.
 *
 * This routine does recognise infinity in the case where \a Int is the type
 * regina::LargeInteger.
 *
 * If \a Int is one of Regina's arbitrary precision integer types, then
 * this routine is identical to calling Int::tightDecode().
 *
 * \headers Some components of this routine are implemented in a separate
 * header (tightencoding-impl.h), which is not included automatically by this
 * file.  Most end users should not need this extra header, since Regina's
 * calculation engine already includes explicit instantiations for all of the
 * integer types that have corresponding global tightEncode() functions
 * (i.e., signed and unsigned \c int, \c long, and \c long \c long, plus
 * regina::Integer and regina::LargeInteger).
 *
 * \exception InvalidInput the given input stream does not begin with
 * a tight encoding of an integer of type \a Int.  This includes the case
 * where the encoding \e is a valid integer encoding but the integer itself
 * is outside the allowed range for the \a Int type.
 *
 * \ifacespython Not present, but the string version of this routine
 * is available.
 *
 * \tparam Int The type of integer to reconstruct; this must be either (i) a
 * native C++ integer type (and not \c bool), or (ii) one of Regina's arbitrary
 * precision integer types (i.e., regina::Integer or regina::LargeInteger).
 *
 * @param input an input stream that begins with the tight encoding for an
 * integer.
 * @return the integer represented by the given tight encoding.
 */
template<typename Int>
Int tightDecode(std::istream& input);

namespace detail {
    /**
     * Internal function that writes the tight encoding of the given integer
     * to the given output stream.  This should not be called directly;
     * its purpose is to provide a common implementation for tightEncode()
     * and tightEncoding() for all integer types.
     *
     * This routine does support passing infinity as the given value (which is
     * only relevant when the integer type \a Int is regina::LargeInteger).
     *
     * \ifacespython Not present; use regina::tightEncoding(...) instead.
     *
     * \tparam Int The type of integer to encode; this must be either
     * (i) a native C++ integer type, or (ii) one of Regina's arbitrary
     * precision integer types (i.e., regina::Integer or regina::LargeInteger).
     *
     * @param out the output stream to which the encoded string will be written.
     * @param value the integer to encode.
     *
     * \ingroup utilities
     */
    template <typename Int>
    void tightEncodeInteger(std::ostream& out, Int value);

    /**
     * Internal function that reconstructs an integer from its given tight
     * encoding.  This should not be called directly; its purpose is to
     * provide a common implementation for tightDecode() for all integer types.
     *
     * The tight encoding will be extracted one character at a time
     * beginning with the iterator \a start, in a single pass, without
     * skipping any leading whitespace.  If the iterator ever reaches
     * \a limit before the encoding is complete then the encoding is
     * treated as invalid (i.e., this routine will throw an exception).
     *
     * If \a noTrailingData is \c true then the iterator is required to
     * \e finish at \a limit, or else the encoding will be considered
     * invalid also; if \a noTrailingData is \c false then there is no
     * constraint on the final state of the iterator.
     *
     * \exception InvalidArgument the given iterator does not point to
     * a tight encoding of an integer of type \a Int.  This includes the
     * case where the encoding \e is a valid integer encoding but the integer
     * itself is outside the allowed range for the \a Int type.
     *
     * This routine does recognise infinity in the case where \a Int is
     * the type regina::LargeInteger.
     *
     * \ifacespython Not present; use regina::tightDecode(...) instead.
     *
     * \tparam Int The type of integer to reconstruct; this must be either
     * (i) a native C++ integer type, or (ii) one of Regina's arbitrary
     * precision integer types (i.e., regina::Integer or regina::LargeInteger).
     *
     * \tparam iterator an input iterator type.
     *
     * @param start an iterator that points to the beginning of a
     * tight encoding.
     * @param limit an iterator that, if reached, indicates that no more
     * characters are available.
     * @param noTrailingData \c true if iteration should reach \a limit
     * immediately after the encoding is read, or \c false if there is
     * allowed to be additional unread data.
     * @return the integer represented by the given tight encoding.
     *
     * \ingroup utilities
     */
    template <typename Int, typename iterator>
    Int tightDecodeInteger(iterator start, iterator limit, bool noTrailingData);
}

// Inline functions:

inline void tightEncode(std::ostream& out, int value) {
    regina::detail::tightEncodeInteger(out, value);
}

inline std::string tightEncoding(int value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, value);
    return out.str();
}

inline void tightEncode(std::ostream& out, long value) {
    regina::detail::tightEncodeInteger(out, value);
}

inline std::string tightEncoding(long value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, value);
    return out.str();
}

inline void tightEncode(std::ostream& out, long long value) {
    regina::detail::tightEncodeInteger(out, value);
}

inline std::string tightEncoding(long long value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, value);
    return out.str();
}

inline void tightEncode(std::ostream& out, unsigned value) {
    regina::detail::tightEncodeInteger(out, value);
}

inline std::string tightEncoding(unsigned value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, value);
    return out.str();
}

inline void tightEncode(std::ostream& out, unsigned long value) {
    regina::detail::tightEncodeInteger(out, value);
}

inline std::string tightEncoding(unsigned long value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, value);
    return out.str();
}

inline void tightEncode(std::ostream& out, unsigned long long value) {
    regina::detail::tightEncodeInteger(out, value);
}

inline std::string tightEncoding(unsigned long long value) {
    std::ostringstream out;
    regina::detail::tightEncodeInteger(out, value);
    return out.str();
}

template<typename Int>
inline Int tightDecode(const std::string& enc) {
    return regina::detail::tightDecodeInteger<Int>(
        enc.begin(), enc.end(), true);
}

template<typename Int>
inline Int tightDecode(std::istream& input) {
    try {
        return regina::detail::tightDecodeInteger<Int>(
            std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>(), false);
    } catch (const InvalidArgument& exc) {
        // For input streams we use a different exception type.
        throw InvalidInput(exc.what());
    }
}

} // namespace regina

#endif
