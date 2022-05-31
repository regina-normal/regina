
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

/*! \file utilities/tightencoding-impl.h
 *  \brief Contains implementation details for tight encoding and decoding
 *  functions.
 *
 *  This file is \a not included automatically by tightencoding.h.
 *  However, most end users should not need to include it, since Regina's
 *  calculation engine provides full explicit instantiations for all of the
 *  types that have corresponding global tightEncode() functions
 *  (e.g., signed and unsigned \c int, \c long, and \c long \c long, plus
 *  regina::Integer and regina::LargeInteger).
 */

#ifndef __REGINA_TIGHTENCODING_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_TIGHTENCODING_IMPL_H
#endif

#include "utilities/intutils.h"
#include "utilities/tightencoding.h"

namespace regina::detail {

template <typename Int>
void tightEncodeInteger(std::ostream& out, Int value) {
    static_assert((std::is_integral_v<Int> && ! std::is_same_v<Int, bool>)
            || IsReginaArbitraryPrecisionInteger<Int>::value,
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

template <typename Int, typename iterator>
Int tightDecodeInteger(iterator start, iterator limit, bool noTrailingData) {
    static_assert((std::is_integral_v<Int> && ! std::is_same_v<Int, bool>)
            || IsReginaArbitraryPrecisionInteger<Int>::value,
        "tightEncodeInteger() requires either a native C++ integer type "
        "or one of Regina's arbitrary precision integer types.");

    Int result;
    bool overflow = false;

    if (start == limit)
        throw InvalidInput("The tight encoding is incomplete");
    signed char c = *start++;
    if (c >= 33 && c <= 122) {
        // The result will fit into a single byte.
        if constexpr (std::is_unsigned_v<Int>) {
            if (c < 77)
                throw InvalidInput("The tight encoding describes "
                    "a negative integer but the integer type is unsigned");
        }
        result = c - 77;
    } else if (c == '~') {
        // The result will still fit into a single byte.
        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        c = *start++;
        if (c < 33 || c > 122) {
            throw InvalidInput("The tight encoding is invalid");
        } else if (c <= 77) {
            if constexpr (std::is_unsigned_v<Int>) {
                throw InvalidInput("The tight encoding describes "
                    "a negative integer but the integer type is unsigned");
            }
            result = c - 122;
        } else {
            result = c - 32;
        }
    } else if (c == '|') {
        // The result could need either 1 or 2 bytes.
        // It is guaranteed to fit within an int.
        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        c = (*start++) - 33;
        if (c < 0 || c >= 90)
            throw InvalidInput("The tight encoding is invalid");
        int val = c;

        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        c = (*start++) - 33;
        if (c < 0 || c >= 90)
            throw InvalidInput("The tight encoding is invalid");
        val += 90 * static_cast<int>(c);

        if (val < 4050) {
            // This encodes a negative number.
            if constexpr (std::is_unsigned_v<Int>) {
                throw InvalidInput("The tight encoding describes "
                    "a negative integer but the integer type is unsigned");
            }
            if constexpr (std::is_integral_v<Int> && sizeof(Int) == 1) {
                // One byte might not be enough.
                if (val < 4139 + int(std::numeric_limits<Int>::min())) {
                    overflow = true;
                    goto endDecoding;
                }
            }
            result = val - 4139;
        } else {
            // This encodes a positive number.
            if constexpr (std::is_integral_v<Int> && sizeof(Int) == 1) {
                // One byte might not be enough.
                if (val > 3959 + int(std::numeric_limits<Int>::max())) {
                    overflow = true;
                    goto endDecoding;
                }
            }
            result = val - 3959;
        }
    } else if (c == '}') {
        // The result could need either 2 or 4 bytes.
        // It is guaranteed to fit within a long.
        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        c = (*start++) - 33;
        if (c < 0 || c >= 90)
            throw InvalidInput("The tight encoding is invalid");
        long val = c;

        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        c = (*start++) - 33;
        if (c < 0 || c >= 90)
            throw InvalidInput("The tight encoding is invalid");
        val += 90 * static_cast<int>(c);

        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        c = (*start++) - 33;
        if (c < 0 || c >= 90)
            throw InvalidInput("The tight encoding is invalid");
        val += 8100 * static_cast<int>(c);

        if (val < 364500) {
            // This encodes a negative number.
            if constexpr (std::is_unsigned_v<Int>) {
                throw InvalidInput("The tight encoding describes "
                    "a negative integer but the integer type is unsigned");
            }
            if constexpr (std::is_integral_v<Int> && sizeof(Int) < 4) {
                if (val < 368639 + long(std::numeric_limits<Int>::min())) {
                    overflow = true;
                    goto endDecoding;
                }
            }
            result = static_cast<Int>(val - 368639);
        } else {
            // This encodes a positive number.
            if constexpr (std::is_integral_v<Int> && sizeof(Int) < 4) {
                if (val > 360359 + long(std::numeric_limits<Int>::max())) {
                    overflow = true;
                    goto endDecoding;
                }
            }
            result = static_cast<Int>(val - 360359);
        }
    } else if (c == '{') {
        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        c = *start++;
        if (c == '}') {
            // This encodes infinity.
            if constexpr (std::is_same_v<Int, regina::IntegerBase<true>>)
                result.makeInfinite();
            else
                throw InvalidInput("The tight encoding represents "
                    "infinity, which is not supported by the chosen "
                    "integer type");
        } else if (c < 33 || c > 122) {
            throw InvalidInput("The tight encoding is invalid");
        } else if constexpr (sizeof(Int) < 4) {
            // The result needs at least 4 bytes, possibly more.
            // This *will* overflow.

            // We still do the negativity check, since as a general rule
            // we prioritise "type is unsigned" errors over "type too small".
            if constexpr (std::is_unsigned_v<Int>)
                if (c > 77)
                    throw InvalidInput("The tight encoding describes "
                        "a negative integer but the integer type is unsigned");

            overflow = true;
            goto endDecoding;
        } else {
            // The result needs at least 4 bytes, but possibly more.
            // In this if/else branch we have a compile-time guarantee that
            // Int has >= 4 bytes.  This *might* overflow - we won't know
            // until we see more of the encoding.

            // Identify whether this encodes a positive or negative number.
            bool negative = (c > 77);

            if (negative) {
                if constexpr (std::is_unsigned_v<Int>) {
                    throw InvalidInput("The tight encoding describes "
                        "a negative integer but the integer type is unsigned");
                }
                result = -368562;
                result -= c;
            } else {
                result = 368607;
                result += c;
            }

            Int coeff = 45, coeffPrev = 0;
            while (true) {
                if (start == limit)
                    throw InvalidInput("The tight encoding is incomplete");
                c = *start++;
                if (c == '}')
                    break;
                if (c < 33 || c > 122)
                    throw InvalidInput("The tight encoding is invalid");

                if (coeffPrev != 0) {
                    // Step up to the next power of 90.
                    // This or a higher power should appear with a non-zero
                    // coefficient (either now or later in the encoding),
                    // so if this overflows then we can bail now.
                    if constexpr (std::is_integral_v<Int>) {
                        if (coeff > regina::maxSafeFactor<Int, 90>) {
                            overflow = true;
                            goto endDecoding;
                        }
                    }
                    coeff *= 90;
                }

                if constexpr (! std::is_integral_v<Int>) {
                    if (negative)
                        result -= (coeff * (c - 33));
                    else
                        result += (coeff * (c - 33));
                } else {
                    // We have to be careful about overflow here.
                    // First work out what we need to add/subtract.
                    Int term;
                    if (coeffPrev == 0) {
                        // This will not overflow.
                        term = coeff * static_cast<Int>(c - 33);
                    } else {
                        // The first multiplication here will
                        // not overflow; the second might.
                        term = coeffPrev * static_cast<Int>(c - 33);
                        if (term > regina::maxSafeFactor<Int, 90>) {
                            overflow = true;
                            goto endDecoding;
                        }
                        term *= 90;
                    }
                    // Now see if it's safe to add/subtract.
                    if (negative) {
                        if (result >=
                                std::numeric_limits<Int>::min() + term)
                            result -= term;
                        else {
                            overflow = true;
                            goto endDecoding;
                        }
                    } else {
                        if (result <=
                                std::numeric_limits<Int>::max() - term)
                            result += term;
                        else {
                            overflow = true;
                            goto endDecoding;
                        }
                    }
                }
                coeffPrev = coeff;
            }
        }
    } else
        throw InvalidInput("The tight encoding is invalid");

endDecoding:
    if (overflow)
        throw InvalidInput("The tight encoding describes an integer "
            "that is out of range for the chosen integer type");
    if (noTrailingData && (start != limit))
        throw InvalidInput("The tight encoding has trailing characters");
    return result;
}

template <typename Int>
void tightEncodeIndex(std::ostream& out, Int value) {
    static_assert(std::is_unsigned_v<Int> || std::is_same_v<Int, ssize_t>,
        "tightEncodeIndex() requires either ssize_t or an unsigned native "
        "C++ integer type.");
    static_assert(sizeof(Int) >= 2,
        "tightEncodeIndex() requires an integer type with at least 16 bits.");

    // Here we use the 90 values 33..122 as "digit" characters,
    // and the four values 123..126 as different types of markers.
    // As characters, the four markers are: { | } ~

    if constexpr (! std::is_unsigned_v<Int>)
        if (value < -1)
            throw InvalidArgument("tightEncodeIndex() can only encode "
                "integers >= -1");

    if (value <= 88) {
        out << char(value + 34); // 33 <= char <= 122
        return;
    }

    if (value <= 178) {
        out << '~' << char(value - 56); // 33 <= char <= 122
        return;
    }

    if (value <= 8278) {
        value -= 179;
        out << '|' << char((value % 90) + 33) << char((value / 90) + 33);
        return;
    }

    if constexpr (sizeof(Int) == 2) {
        // The next bound of 737278 is too large for our integer type.
        value -= 8279;
        out << '}' << char((value % 90) + 33);
        value /= 90;
        out << char((value % 90) + 33) << char((value / 90) + 33);
        return;
    } else {
        if (value <= 737278) {
            value -= 8279;
            out << '}' << char((value % 90) + 33);
            value /= 90;
            out << char((value % 90) + 33) << char((value / 90) + 33);
            return;
        }

        // Fall through to the general case.
        value -= 737279;
        out << '{';
        if (value == 0) {
            // Write one zero digit, as opposed to nothing at all.
            out << char(33);
        } else {
            while (value > 0) {
                out << char((value % 90) + 33);
                value /= 90;
            }
        }
        out << '}';
    }
}

template <typename Int>
Int tightDecodeIndex(std::istream& input) {
    static_assert(std::is_unsigned_v<Int> || std::is_same_v<Int, ssize_t>,
        "tightDecodeIndex() requires either ssize_t or an unsigned native "
        "C++ integer type.");
    static_assert(sizeof(Int) >= 2,
        "tightDecodeIndex() requires an integer type with at least 16 bits.");

    Int result;
    bool overflow = false;

    std::istreambuf_iterator<char> start(input);
    std::istreambuf_iterator<char> limit; // end-of-input

    if (start == limit)
        throw InvalidInput("The tight encoding is incomplete");
    signed char c = *start++;
    if (c >= 33 && c <= 122) {
        if constexpr (std::is_unsigned_v<Int>) {
            if (c == 33)
                throw InvalidInput("The tight encoding describes "
                    "a negative index but the integer type is unsigned");
        }
        result = c - 34;
    } else if (c == '~') {
        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        c = *start++;
        if (c < 33 || c > 122)
            throw InvalidInput("The tight encoding is invalid");
        else
            result = c + 56;
    } else if (c == '|') {
        result = 179;

        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        c = (*start++) - 33;
        if (c < 0 || c >= 90)
            throw InvalidInput("The tight encoding is invalid");
        result += c;

        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        c = (*start++) - 33;
        if (c < 0 || c >= 90)
            throw InvalidInput("The tight encoding is invalid");
        result += 90 * static_cast<Int>(c);
    } else if (c == '}') {
        // This is the first case where we could encounter overflow,
        // since the result could require either 2 or 4 bytes.
        // The integer that we read here *will* fit into a long (which
        // is guaranteed to hold at least 4 bytes), so we do our
        // initial arithmetic there.
        long val = 8279;

        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        c = (*start++) - 33;
        if (c < 0 || c >= 90)
            throw InvalidInput("The tight encoding is invalid");
        val += c;

        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        c = (*start++) - 33;
        if (c < 0 || c >= 90)
            throw InvalidInput("The tight encoding is invalid");
        val += 90 * static_cast<int>(c);

        if (start == limit)
            throw InvalidInput("The tight encoding is incomplete");
        c = (*start++) - 33;
        if (c < 0 || c >= 90)
            throw InvalidInput("The tight encoding is invalid");
        val += 8100 * static_cast<int>(c);

        if constexpr (sizeof(Int) < 4) {
            if (val > long(std::numeric_limits<Int>::max())) {
                overflow = true;
                goto endDecoding;
            }
        }
        result = static_cast<Int>(val);
    } else if (c == '{') {
        // The result needs at least 4 bytes, possibly more.
        if constexpr (sizeof(Int) < 4) {
            // This *will* overflow.
            overflow = true;
            goto endDecoding;
        } else {
            // In this if/else branch we have a compile-time guarantee that
            // Int has >= 4 bytes.  This *might* overflow - we won't know
            // until we see more of the encoding.

            result = 737279;
            Int coeff = 1, coeffPrev = 0;
            while (true) {
                if (start == limit)
                    throw InvalidInput("The tight encoding is incomplete");
                c = *start++;
                if (c == '}')
                    break;
                if (c < 33 || c > 122)
                    throw InvalidInput("The tight encoding is invalid");

                if (coeffPrev != 0) {
                    // We have reached at least the second base 90 digit
                    // in our encoding.
                    // Step up to the next power of 90.
                    // This or a higher power should appear with a non-zero
                    // coefficient (either now or later in the encoding),
                    // so if this overflows then we can bail now.
                    if (coeff > regina::maxSafeFactor<Int, 90>) {
                        overflow = true;
                        goto endDecoding;
                    }
                    coeff *= 90;
                }

                // We have to be careful about overflow here.
                // First work out what we need to add.
                Int term;
                if (coeffPrev == 0) {
                    // This will not overflow.
                    term = coeff * static_cast<Int>(c - 33);
                } else {
                    // The first multiplication here will
                    // not overflow; the second might.
                    term = coeffPrev * static_cast<Int>(c - 33);
                    if (term > regina::maxSafeFactor<Int, 90>) {
                        overflow = true;
                        goto endDecoding;
                    }
                    term *= 90;
                }
                // Now see if it's safe to add.
                if (result <= std::numeric_limits<Int>::max() - term)
                    result += term;
                else {
                    overflow = true;
                    goto endDecoding;
                }
                coeffPrev = coeff;
            }
        }
    } else
        throw InvalidInput("The tight encoding is invalid");

endDecoding:
    if (overflow)
        throw InvalidInput("The tight encoding describes an index "
            "that is out of range for the chosen integer type");
    return result;
}

} // namespace regina::detail

#endif
