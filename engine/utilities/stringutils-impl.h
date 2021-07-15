
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

/*! \file utilities/stringutils-impl.h
 *  \brief Contains implementations of template functions from stringutils.h.
 *
 *  This file is automatically included from stringutils.h; there
 *  is no need for end users to include it explicitly.
 */

// Make sure stringutils.h is included first.
#include "utilities/stringutils.h"

#ifndef __REGINA_STRINGUTILS_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_STRINGUTILS_IMPL_H
#endif

#include <cctype>
#include "maths/integer.h"
#include "utilities/intutils.h"

namespace regina {

template <bool supportInfinity>
bool valueOf(const std::string& str, IntegerBase<supportInfinity>& dest) {
    bool valid;
    dest = IntegerBase<supportInfinity>(str.c_str(), 10, &valid);
    return valid;
}

template <class OutputIterator>
unsigned basicTokenise(OutputIterator results, const std::string& str) {
    std::string::size_type len = str.length();
    std::string::size_type pos = 0;

    // Skip initial whitespace.
    while (pos < len && isspace(str[pos]))
        pos++;

    if (pos == len)
        return 0;

    // Extract each token.
    std::string::size_type total = 0;
    std::string::size_type tokStart;
    while (pos < len) {
        // Find the characters making up this token.
        tokStart = pos;
        while (pos < len && ! isspace(str[pos]))
            pos++;
        *results++ = str.substr(tokStart, pos - tokStart);
        total++;

        // Skip the subsequent whitespace.
        while (pos < len && isspace(str[pos]))
            pos++;
    }

    return static_cast<unsigned>(total);
}

template <typename T>
std::string superscript(T value) {
    std::string s = std::to_string(value);
    std::string ans;
    for (auto c : s)
        switch (c) {
            case '0': ans += "\u2070"; break;
            case '1': ans += "\u00B9"; break;
            case '2': ans += "\u00B2"; break;
            case '3': ans += "\u00B3"; break;
            case '4': ans += "\u2074"; break;
            case '5': ans += "\u2075"; break;
            case '6': ans += "\u2076"; break;
            case '7': ans += "\u2077"; break;
            case '8': ans += "\u2078"; break;
            case '9': ans += "\u2079"; break;
            case '+': ans += "\u207A"; break;
            case '-': ans += "\u207B"; break;
            default: ans += "?"; break;
        }
    return ans;
}

template <bool supportInfinity>
std::string superscript(const IntegerBase<supportInfinity>& value) {
    std::string s = value.stringValue();
    std::string ans;
    for (auto c : s)
        switch (c) {
            case '0': ans += "\u2070"; break;
            case '1': ans += "\u00B9"; break;
            case '2': ans += "\u00B2"; break;
            case '3': ans += "\u00B3"; break;
            case '4': ans += "\u2074"; break;
            case '5': ans += "\u2075"; break;
            case '6': ans += "\u2076"; break;
            case '7': ans += "\u2077"; break;
            case '8': ans += "\u2078"; break;
            case '9': ans += "\u2079"; break;
            case '+': ans += "\u207A"; break;
            case '-': ans += "\u207B"; break;
            default: ans += "?"; break;
        }
    return ans;
}

template <typename T>
std::string subscript(T value) {
    std::string s = std::to_string(value);
    std::string ans;
    for (auto c : s)
        switch (c) {
            case '0': ans += "\u2080"; break;
            case '1': ans += "\u2081"; break;
            case '2': ans += "\u2082"; break;
            case '3': ans += "\u2083"; break;
            case '4': ans += "\u2084"; break;
            case '5': ans += "\u2085"; break;
            case '6': ans += "\u2086"; break;
            case '7': ans += "\u2087"; break;
            case '8': ans += "\u2088"; break;
            case '9': ans += "\u2089"; break;
            case '+': ans += "\u208A"; break;
            case '-': ans += "\u208B"; break;
            default: ans += "?"; break;
        }
    return ans;
}

template <bool supportInfinity>
std::string subscript(const IntegerBase<supportInfinity>& value) {
    std::string s = value.stringValue();
    std::string ans;
    for (auto c : s)
        switch (c) {
            case '0': ans += "\u2080"; break;
            case '1': ans += "\u2081"; break;
            case '2': ans += "\u2082"; break;
            case '3': ans += "\u2083"; break;
            case '4': ans += "\u2084"; break;
            case '5': ans += "\u2085"; break;
            case '6': ans += "\u2086"; break;
            case '7': ans += "\u2087"; break;
            case '8': ans += "\u2088"; break;
            case '9': ans += "\u2089"; break;
            case '+': ans += "\u208A"; break;
            case '-': ans += "\u208B"; break;
            default: ans += "?"; break;
        }
    return ans;
}

template <typename Int>
void streamEncode(std::ostream& out, Int value) {
    static_assert(std::is_integral<Int>::value ||
            IsReginaArbitraryPrecisionInteger<Int>::value,
        "streamEncode() requires either a native C++ integer type or "
        "one of Regina's arbitrary precision integer types.");

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
    if (value > -45 && value <= 45) {
        if constexpr (IsReginaArbitraryPrecisionInteger<Int>::value) {
            out << char(value.longValue() + 77); // 33 <= char <= 122
        } else {
            out << char(value + 77); // 33 <= char <= 122
        }
        return;
    }

    // From here, the original value must have been >= 2 decimal digits.
    if (value < 0)
        value += 45;
    else
        value -= 45;

    // The next-best scenario: marker plus one "digit" character.
    if (value > -45 && value <= 45) {
        if constexpr (IsReginaArbitraryPrecisionInteger<Int>::value) {
            out << '~' << char(value.longValue() + 77);
        } else {
            out << '~' << char(value + 77);
        }
        return;
    }

    // From here, the original value must have been >= 3 decimal digits.
    if (value < 0)
        value += 45;
    else
        value -= 45;

    // The next-best scenario: marker plus two "digit" characters.
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

    // From here, the original value must have been >= 4 decimal digits,
    // and the type T must be at least 16-bit.
    if (value < 0)
        value += 4050;
    else
        value -= 4050;

    // The next-best scenario: marker plus three "digit" characters.
    if (value > -364500 && value <= 364500) {
        // Note: T could still be short int, so cast to a long before we
        // start doing any arithmetic.
        long i;
        if constexpr (IsReginaArbitraryPrecisionInteger<Int>::value) {
            i = value.longValue() + 364499; // 0 <= i < 729000 = 90*90*90
        } else {
            i = long(value) + 364499; // 0 <= i < 729000 = 90*90*90
        }
        out << '}' << char((i % 90) + 33);
        i /= 90;
        out << char((i % 90) + 33) << char((i / 90) + 33);
        return;
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
        // value might exceed the bounds of a long, but (value % 45) will not.
        next = (value % 45).longValue();
    } else {
        next = value % 45;
    }
    if (value > 0) {
        value /= 45;
    } else {
        if (next < 0)
            next = -next;
        next += 45;
        value = -(value / 45);
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

} // namespace regina

#endif
