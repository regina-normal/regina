
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
#include "utilities/intutils.h"

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
 *   33 to 126 inclusive).
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
 * Regina does not provide \e decoding routines, though (as noted above) this
 * should be possible if the underlying types are known.  This is because
 * tight encodings were originally designed for applications such as
 * perfect hashing, where the aim is essentially to "compress" the data in a
 * short printable string whilst preserving the correctness of equality tests.
 *
 * For native C++ data types where tight encodings are supported, these
 * are provided in the header utilities/tightencoding.h through overloads of
 * the global functions tightEncoding() (which returns a string) and
 * tightEncode() (which writes to an output stream).
 *
 * For Regina's own data types where tight encodings are supported, these are
 * provided through tightEncode() and tightEncoding() member functions of the
 * corresopnding classes.
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

// Implementations of template functions:

namespace detail {
    template <typename Int>
    void tightEncodeInteger(std::ostream& out, Int value) {
        static_assert(std::is_integral_v<Int> ||
                IsReginaArbitraryPrecisionInteger<Int>::value,
            "tightEncodeInteger() requires either a native C++ integer type "
            "or one of Regina's arbitrary precision integer types.");

        // Here we use the 90 values 33..122 as "digit" characters,
        // and the four values 123..126 as different types of markers.
        // As characters, the four markers are: { | } ~

        // Get the special case of infinity out of the way.
        if constexpr (IsReginaArbitraryPrecisionInteger<Int>::value) {
            if (value.isInfinite()) {
                out << "{}";
                return;
            }
        }

        // The best-case scenario: a single "digit" character.
        if constexpr (std::is_unsigned_v<Int>) {
            if (value <= 45) {
                out << char(value + 77); // char <= 122
                return;
            }
        } else {
            if (value > -45 && value <= 45) {
                if constexpr (IsReginaArbitraryPrecisionInteger<Int>::value) {
                    out << char(value.longValue() + 77); // 33 <= char <= 122
                } else {
                    out << char(value + 77); // 33 <= char <= 122
                }
                return;
            }
        }

        // From here, the original value must have been >= 2 decimal digits.
        if (value < 0)
            value += 45;
        else
            value -= 45;

        // The next-best scenario: marker plus one "digit" character.
        if constexpr (std::is_unsigned_v<Int>) {
            if (value <= 45) {
                out << '~' << char(value + 77);
                return;
            }
        } else {
            if (value > -45 && value <= 45) {
                if constexpr (IsReginaArbitraryPrecisionInteger<Int>::value) {
                    out << '~' << char(value.longValue() + 77);
                } else {
                    out << '~' << char(value + 77);
                }
                return;
            }
        }

        // From here, the original value must have been >= 3 decimal digits.
        if (value < 0)
            value += 45;
        else
            value -= 45;

        // The next-best scenario: marker plus two "digit" characters.
        if constexpr (std::is_unsigned_v<Int>) {
            if (value <= 4050) {
                // Note: T could be char, so cast to an unsigned int before we
                // start doing any arithmetic.
                unsigned i = unsigned(value) + 4049; // i < 8100 = 90*90
                out << '|' << char((i % 90) + 33) << char((i / 90) + 33);
                return;
            }
        } else {
            if (value > -4050 && value <= 4050) {
                // Note: T could be char, so cast to an int before we
                // start doing any arithmetic.
                int i;
                if constexpr (IsReginaArbitraryPrecisionInteger<Int>::value) {
                    i = int(value.longValue()) + 4049; // 0 <= i < 8100 = 90*90
                } else {
                    i = int(value) + 4049; // 0 <= i < 8100 = 90*90
                }
                out << '|' << char((i % 90) + 33) << char((i / 90) + 33);
                return;
            }
        }

        // From here, the original value must have been >= 4 decimal digits,
        // and the type T must be at least 16-bit.
        if (value < 0)
            value += 4050;
        else
            value -= 4050;

        // The next-best scenario: marker plus three "digit" characters.
        if constexpr (std::is_unsigned_v<Int>) {
            if (value <= 364500) {
                // Note: T could still be unsigned short, so cast to an
                // unsigned long before we start doing any arithmetic.
                unsigned long i =
                    (unsigned long)(value) + 364499; // i < 729000 = 90^3
                out << '}' << char((i % 90) + 33);
                i /= 90;
                out << char((i % 90) + 33) << char((i / 90) + 33);
                return;
            }
        } else {
            if (value > -364500 && value <= 364500) {
                // Note: T could still be short int, so cast to a long before
                // we start doing any arithmetic.
                long i;
                if constexpr (IsReginaArbitraryPrecisionInteger<Int>::value) {
                    i = value.longValue() + 364499; // 0 <= i < 729000 = 90^3
                } else {
                    i = long(value) + 364499; // 0 <= i < 729000 = 90^3
                }
                out << '}' << char((i % 90) + 33);
                i /= 90;
                out << char((i % 90) + 33) << char((i / 90) + 33);
                return;
            }
        }

        // From here, the original value must have been >= 6 decimal digits,
        // and the type T must be at least 32-bit.
        if (value < 0)
            value += 364500;
        else
            value -= 364500;

        // From here we switch to a generic scheme:
        // (1) marker
        // (2) combined sign and digit mod 45
        // (3) sequence of digits mod 90
        // (4) marker to terminate

        int next;
        if constexpr (IsReginaArbitraryPrecisionInteger<Int>::value) {
            // value might be out of bounds for a native integer,
            // but (value % 45) will not.
            next = (value % 45).template nativeValue<sizeof(int)>();
        } else {
            next = value % 45;
        }
        if constexpr (std::is_unsigned_v<Int>) {
            if (value > 0) {
                value /= 45;
            } else {
                // value == 0, since the type is unsigned.
                next = 45; // for consistency with the signed case
            }
        } else {
            if (value > 0) {
                value /= 45;
            } else {
                if (next < 0)
                    next = -next;
                next += 45;
                value = -(value / 45);
            }
        }
        out << '{' << char(next + 33);

        while (value > 0) {
            if constexpr (IsReginaArbitraryPrecisionInteger<Int>::value) {
                out << char((value % 90).longValue() + 33);
            } else {
                out << char((value % 90) + 33);
            }
            value /= 90;
        }

        out << '}';
    }
}

} // namespace regina

#endif
