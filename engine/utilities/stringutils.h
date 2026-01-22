
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

/*! \file utilities/stringutils.h
 *  \brief Provides various routines for use with C++ strings.
 */

#ifndef __REGINA_STRINGUTILS_H
#ifndef __DOXYGEN
#define __REGINA_STRINGUTILS_H
#endif

#include <algorithm>
#include <cstdint>
#include <cctype>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include "regina-core.h"
#include "concepts/core.h"
#include "concepts/io.h"

namespace regina {

class BoolSet;

template <bool>
class IntegerBase;

/**
 * Determines whether the given C++ string begins with the given prefix.
 *
 * \param str the full C++ string to examine.
 * \param prefix the prefix whose presence we are testing for.
 * \return \c true if and only if \a str begins with \a prefix.
 *
 * \ingroup utilities
 */
bool startsWith(const std::string& str, const std::string& prefix);

/**
 * Strips all whitespace from the beginning and end of the given C++ string.
 * The new stripped string is returned; the original string is not
 * altered.
 *
 * \warning This routine treats all strings as plain ASCII.  In
 * particular, characters are examined one at a time, and the C routine
 * isspace() is used to identify whitespace.  Use it on strings with
 * international characters at your own peril.
 *
 * \param str the string to be stripped.
 * \return the resulting stripped string.
 *
 * \ingroup utilities
 */
std::string stripWhitespace(const std::string& str);

/**
 * Converts the entire given string to a signed native integer of type \a T
 * and reports whether this conversion was successful.
 *
 * If the integer encoded by the given string is out of range for type \a T,
 * then this routine will return \c false and the value of \a dest will be
 * left unchanged.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are found (including leading or trailing
 * whitespace), then again this routine will return \c false and the value of
 * \a dest will be left unchanged.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these are tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting signed native
 * integer.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
template <SignedCppInteger T>
bool valueOf(const std::string& str, T& dest) {
    if (str.empty() || std::isspace(str.front()))
        return false;

    if constexpr (sizeof(T) <= sizeof(long long)) {
        size_t pos = 0;
        try {
            if constexpr (sizeof(T) <= sizeof(long)) {
                long ans = std::stol(str, std::addressof(pos), 10);
                if (pos != str.size())
                    return false;
                if constexpr (sizeof(T) < sizeof(long))
                    if (ans < std::numeric_limits<T>::min() ||
                            ans > std::numeric_limits<T>::max())
                        return false;
                dest = static_cast<T>(ans);
                return true;
            } else {
                long long ans = std::stoll(str, std::addressof(pos), 10);
                if (pos != str.size())
                    return false;
                if constexpr (sizeof(T) < sizeof(long long))
                    if (ans < std::numeric_limits<T>::min() ||
                            ans > std::numeric_limits<T>::max())
                        return false;
                dest = static_cast<T>(ans);
                return true;
            }
        } catch (const std::logic_error&) {
            // Either the string was unconvertible, or the value was out of
            // range.
            return false;
        }
    } else {
        // We avoid using std::istream extraction, since it looks like this
        // does not check for overflow.
        //
        // Remember: we know the string is non-empty.
        auto pos = str.begin();
        bool negative = (*pos == '-');
        if (negative) {
            if (++pos == str.end())
                return false;
        }

        T ans = 0;
        while (pos != str.end()) {
            char c = *pos++;
            if (c < '0' || c > '9')
                return false;
            if (negative)
                ans = ans * 10 - int(c - '0');
            else
                ans = ans * 10 + unsigned(c - '0');
            // TODO: Check for overflow.
        }
        dest = ans;
        return true;
    }
}

/**
 * Converts the entire given string to an unsigned native integer of type \a T
 * and reports whether this conversion was successful.
 *
 * If the integer encoded by the given string is out of range for type \a T,
 * then this routine will return \c false and the value of \a dest will be
 * left unchanged.  In particular, this will happen if \a str encodes a
 * negative number (since \a T is an unsigned type).
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are found (including leading or trailing
 * whitespace), then again this routine will return \c false and the value of
 * \a dest will be left unchanged.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these are tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting unsigned native
 * integer.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
template <UnsignedCppInteger T>
bool valueOf(const std::string& str, T& dest) {
    if (str.empty() || std::isspace(str.front()) || str.front() == '-')
        return false;

    if constexpr (sizeof(T) <= sizeof(unsigned long long)) {
        size_t pos = 0;
        try {
            if constexpr (sizeof(T) <= sizeof(unsigned long)) {
                unsigned long ans = std::stoul(str, std::addressof(pos), 10);
                if (pos != str.size())
                    return false;
                if constexpr (sizeof(T) < sizeof(unsigned long))
                    if (ans > std::numeric_limits<T>::max())
                        return false;
                dest = static_cast<T>(ans);
                return true;
            } else {
                unsigned long long ans = std::stoull(str, std::addressof(pos),
                    10);
                if (pos != str.size())
                    return false;
                if constexpr (sizeof(T) < sizeof(unsigned long long))
                    if (ans > std::numeric_limits<T>::max())
                        return false;
                dest = static_cast<T>(ans);
                return true;
            }
        } catch (const std::logic_error&) {
            // Either the string was unconvertible, or the value was out of
            // range.
            return false;
        }
    } else {
        // We avoid using std::istream extraction, since it looks like this
        // does not check for overflow.
        //
        // Remember: we know the string is non-empty.
        T ans = 0;
        for (char c : str) {
            if (c < '0' || c > '9')
                return false;
            ans = ans * 10 + unsigned(c - '0');
            // TODO: Check for overflow.
        }
        dest = ans;
        return true;
    }
}

/**
 * Converts the entire given string to a double precision real number and
 * reports whether this conversion was successful.
 *
 * If the number encoded by the given string is out of range for a \c double,
 * then this routine will return \c false and the value of \a dest will be
 * left unchanged.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the real number that the string represents.
 * If any unexpected characters are found (including leading or trailing
 * whitespace), then again this routine will return \c false, and the value of
 * \a dest will be left unchanged.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these are tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting real number.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, double& dest);

/**
 * Converts the entire given string to a boolean and reports whether
 * this conversion was successful.
 *
 * If the given string begins with `T`, `t` or `1`,
 * then the string will be successfully converted to \c true.
 * If the given string begins with `F`, `f` or `0`,
 * then the string will be successfully converted to \c false.
 * Otherwise the conversion will be unsuccessful and the argument \a dest will
 * be left unchanged.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these are tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting boolean.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, bool& dest);

/**
 * Converts the entire given string to a set of booleans and reports whether
 * this conversion was successful.
 *
 * A set of booleans is represented by one of the four string codes
 * `--`, `T-`, `-F` or `TF`, as returned by BoolSet::stringCode().
 * If the conversion is unsuccessful, the argument \a dest will be left
 * unchanged and \c false will be returned.  Both upper-case and lower-case
 * codes (or a mix of the two) are accepted by this routine.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python.
 * For this variant, you can instead use BoolSet::setStringCode(), which
 * performs the same task.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting set of booleans.
 * \return \c true if the conversion was successful or \c false otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, BoolSet& dest);

/**
 * Converts the given native C++ integer to a string, with explicit support
 * for 128-bit integers on those platforms that support them.
 *
 * For _standard_ C++ integer types, this routine is identical to calling
 * `std::to_string(value)`.  The reason this routine exists at all is that,
 * on platforms that support native 128-bit integers, `std::to_string()` and
 * the output stream operators are nevertheless _not_ always available for
 * 128-bit integer arguments.  In contract, for those platforms that support
 * native 128-bit integer types, `regina::toString()` is guaranteed to work
 * for 128-bit integer arguments.
 *
 * \nopython None of Regina's toString() functions are wrapped in Python,
 * since these are tailored to the many different native C++ numeric types.
 * Instead, use Python's own native number-to-string mechanisms.
 *
 * \param value the integer value to convert.
 * \return a string representation of \a value.
 *
 * \ingroup utilities
 */
template <CppInteger T>
std::string toString(T value) {
    if constexpr (StandardStringifiable<T>) {
        return std::to_string(value);
    } else {
        // Presumably we have a 128-bit integer, for which std::to_string()
        // and/or std::ostream output do not exist on some platforms.
        static_assert(sizeof(T) >= 16,
            "std::to_string() should be available for all native C++ "
            "integer types with < 128 bits.  Please report this to the "
            "Regina developers.");
        if (value == 0) {
            return "0";
        } else {
            // Build the string in reverse.
            std::string ans;
            if (value >= 0 /* always true for unsigned types */) {
                for (T x = value ; x != 0; x /= 10)
                    ans += char('0' + int(x % 10));
            } else {
                for (T x = value ; x != 0; x /= 10) {
                    int digit = int(x % 10);
                    if (digit > 0)
                        digit -= 10;
                    ans += char('0' - digit);
                }
                ans += '-';
            }
            std::reverse(ans.begin(), ans.end());
            return ans;
        }
    }
}

/**
 * Decomposes the given string into tokens.
 *
 * This is an extremely simple tokeniser; tokens are defined to be
 * separated by arbitrary blocks of whitespace.  Any leading or trailing
 * whitespace will be ignored.
 *
 * \warning This routine treats all strings as plain ASCII.  In
 * particular, characters are examined one at a time, and the C routine
 * isspace() is used to identify whitespace.  Use it on strings with
 * international characters at your own peril.
 *
 * \param str the string to decompose.
 * \return the resulting list of tokens.
 *
 * \ingroup utilities
 */
std::vector<std::string> basicTokenise(const std::string& str);

/**
 * Returns a token derived from the given string.
 * All whitespace characters in the given string will be replaced with
 * an underscore.
 *
 * \param str the string on which to base the token.
 * \return the corresponding token.
 *
 * \ingroup utilities
 */
std::string stringToToken(std::string str);

/**
 * Converts the given native C++ integer into a unicode superscript string.
 *
 * The resulting string will be encoded using UTF-8.
 *
 * \python The type \a T is assumed to be \c long.
 *
 * \param value the integer to convert.
 * \return the given integer as a superscript string.
 *
 * \ingroup utilities
 */
template <StandardCppInteger T>
std::string superscript(T value);

/**
 * Converts the given Regina integer into a unicode superscript string.
 *
 * The resulting string will be encoded using UTF-8.
 *
 * \pre The given value is not infinity.
 *
 * \param value the integer to convert.
 * \return the given integer as a superscript string.
 *
 * \ingroup utilities
 */
template <ArbitraryPrecisionInteger T>
std::string superscript(const T& value);

/**
 * Converts the given native C++ integer into a unicode subscript string.
 *
 * The resulting string will be encoded using UTF-8.
 *
 * \python The type \a T is assumed to be \c long.
 *
 * \param value the integer to convert.
 * \return the given integer as a subscript string.
 *
 * \ingroup utilities
 */
template <StandardCppInteger T>
std::string subscript(T value);

/**
 * Converts the given Regina integer into a unicode subscript string.
 *
 * The resulting string will be encoded using UTF-8.
 *
 * \pre The given value is not infinity.
 *
 * \param value the integer to convert.
 * \return the given integer as a subscript string.
 *
 * \ingroup utilities
 */
template <ArbitraryPrecisionInteger T>
std::string subscript(const T& value);

} // namespace regina

// Template definitions

#include "utilities/stringutils-impl.h"

#endif

