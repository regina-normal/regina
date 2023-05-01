
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

/*! \file utilities/stringutils.h
 *  \brief Provides various routines for use with C++ strings.
 */

#ifndef __REGINA_STRINGUTILS_H
#ifndef __DOXYGEN
#define __REGINA_STRINGUTILS_H
#endif

#include <string>
#include <vector>
#include "regina-core.h"

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
 * Converts the entire given string to an 8-bit integer and reports whether
 * this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting 8-bit integer.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, int8_t& dest);
/**
 * Converts the entire given string to an unsigned 8-bit integer and reports
 * whether this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting unsigned
 * 8-bit integer.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, uint8_t& dest);
/**
 * Converts the entire given string to a short integer and reports whether
 * this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting short integer.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, short& dest);
/**
 * Converts the entire given string to an unsigned short integer and reports
 * whether this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting unsigned
 * short integer.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, unsigned short& dest);
/**
 * Converts the entire given string to an integer and reports whether
 * this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting integer.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, int& dest);
/**
 * Converts the entire given string to an unsigned integer and reports
 * whether this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting unsigned integer.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, unsigned& dest);
/**
 * Converts the entire given string to a long integer and reports whether
 * this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting long integer.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, long& dest);
/**
 * Converts the entire given string to an unsigned long integer and reports
 * whether this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting unsigned long
 * integer.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, unsigned long& dest);
/**
 * Converts the entire given string to a long long integer and reports whether
 * this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting long long integer.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, long long& dest);
/**
 * Converts the entire given string to an unsigned long long integer and reports
 * whether this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these tailored to the many different native C++ numeric types.
 * Instead, use Python's own native string-to-number mechanisms.
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting unsigned long long
 * integer.
 * \return \c true if the conversion was completely successful or \c false
 * otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, unsigned long long& dest);
/**
 * Converts the entire given string to a double precision real number and
 * reports whether this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the real number that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these tailored to the many different native C++ numeric types.
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
 * Otherwise the conversion will be unsuccessful and argument \a dest will
 * be set to \c false.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python,
 * since these tailored to the many different native C++ numeric types.
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
 * `--`, `T-`, `-F` or `TF`, as returned by
 * BoolSet::stringCode().  If the conversion is unsuccessful, argument
 * \a dest will be set to the empty set and \c false will be returned.
 *
 * \nopython None of Regina's valueOf() functions are wrapped in Python.
 * For this variant, you can instead use BoolSet::setStringCode(), which
 * performs the same task (with the minor difference that it does not change
 * \a dest if the conversion is unsuccessful).
 *
 * \param str the string to convert.
 * \param dest the variable in which to store the resulting set of booleans.
 * \return \c true if the conversion was successful or \c false otherwise.
 *
 * \ingroup utilities
 */
bool valueOf(const std::string& str, BoolSet& dest);

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
 * Converts the given C++ integer into a unicode superscript string.
 *
 * The resulting string will be encoded using UTF-8.
 *
 * \pre The template argument \a T is either (i) a native C++ integer type,
 * for which the standard C++11 library routine std::to_string(T) is defined;
 * or (ii) a const reference to Integer or LargeInteger.
 *
 * \python This template function is instantiated in Python for types
 * \a T = \c long, as well as const references to Integer and LargeInteger.
 *
 * \ingroup utilities
 */
template <typename T>
std::string superscript(T value);

/**
 * Converts the given C++ integer into a unicode subscript string.
 *
 * The resulting string will be encoded using UTF-8.
 *
 * \pre The template argument \a T is either (i) a native C++ integer type,
 * for which the standard C++11 library routine std::to_string(T) is defined;
 * or (ii) a const reference to Integer or LargeInteger.
 *
 * \python This template function is instantiated in Python for types
 * \a T = \c long, as well as const references to Integer and LargeInteger.
 *
 * \ingroup utilities
 */
template <typename T>
std::string subscript(T value);

} // namespace regina

// Template definitions

#include "utilities/stringutils-impl.h"

#endif

