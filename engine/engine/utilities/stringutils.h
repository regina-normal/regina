
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

/*! \file stringutils.h
 *  \brief Provides various routines for use with C++ strings.
 */

#ifndef __STRINGUTILS_H
#ifndef __DOXYGEN
#define __STRINGUTILS_H
#endif

#include <string>

namespace regina {

class NLargeInteger;
class NBoolSet;
class NTriBool;

/**
 * \weakgroup utilities
 * @{
 */

/**
 * Creates a new C string that is a duplicate of the given C++ string.
 *
 * The deallocation of the new C string is the responsibility of
 * the caller of this routine.
 *
 * \ifacespython Not present.
 *
 * @param str the C++ string to duplicate.
 * @return the new duplicate C string.
 */
char* duplicate(const std::string& str);

/**
 * Determines whether the given C++ string begins with the given prefix.
 *
 * \ifacespython Not present.
 *
 * @param str the full C++ string to examine.
 * @param prefix the prefix whose presence we are testing for.
 * @return \c true if and only if \a str begins with \a prefix.
 */
bool startsWith(const std::string& str, const std::string& prefix);

/**
 * Strips all whitespace from the beginning and end of the given C++ string.
 * The new stripped string is returned; the original string is not
 * altered.
 *
 * \ifacespython Not present.
 *
 * @param str the string to be stripped.
 * @return the resulting stripped string.
 */
std::string stripWhitespace(const std::string& str);

/**
 * Converts the entire given string to an integer and reports whether
 * this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \ifacespython Not present.
 *
 * @param str the string to convert.
 * @param dest the variable in which to store the resulting integer.
 * @return \c true if the conversion was completely successful or \c false
 * otherwise.
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
 * \ifacespython Not present.
 *
 * @param str the string to convert.
 * @param dest the variable in which to store the resulting unsigned integer.
 * @return \c true if the conversion was completely successful or \c false
 * otherwise.
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
 * \ifacespython Not present.
 *
 * @param str the string to convert.
 * @param dest the variable in which to store the resulting long integer.
 * @return \c true if the conversion was completely successful or \c false
 * otherwise.
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
 * \ifacespython Not present.
 *
 * @param str the string to convert.
 * @param dest the variable in which to store the resulting unsigned long
 * integer.
 * @return \c true if the conversion was completely successful or \c false
 * otherwise.
 */
bool valueOf(const std::string& str, unsigned long& dest);
/**
 * Converts the entire given string to an arbitrary precision integer and
 * reports whether this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the integer that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \ifacespython Not present.
 *
 * @param str the string to convert.
 * @param dest the variable in which to store the resulting arbitrary
 * precision integer.
 * @return \c true if the conversion was completely successful or \c false
 * otherwise.
 */
bool valueOf(const std::string& str, NLargeInteger& dest);
/**
 * Converts the entire given string to a double precision real number and
 * reports whether this conversion was successful.
 *
 * The given string should contain no whitespace or other characters
 * that are not a part of the real number that the string represents.
 * If any unexpected characters are encountered, the routine will convert
 * the string as best it can but \c false will be returned.
 *
 * \ifacespython Not present.
 *
 * @param str the string to convert.
 * @param dest the variable in which to store the resulting real number.
 * @return \c true if the conversion was completely successful or \c false
 * otherwise.
 */
bool valueOf(const std::string& str, double& dest);
/**
 * Converts the entire given string to a boolean and reports whether
 * this conversion was successful.
 *
 * If the given string begins with <tt>T</tt> or <tt>F</tt> (either
 * upper- or lower-case), the string will be successfully converted to
 * \c true or \c false respectively.  Otherwise the conversion will be
 * unsuccessful and argument \a dest will be set to \c false.
 *
 * \ifacespython Not present.
 *
 * @param str the string to convert.
 * @param dest the variable in which to store the resulting boolean.
 * @return \c true if the conversion was completely successful or \c false
 * otherwise.
 */
bool valueOf(const std::string& str, bool& dest);
/**
 * Converts the entire given string to a three-way boolean (true, false or
 * unknown) and reports whether this conversion was successful.
 *
 * If the given string begins with <tt>T</tt>, <tt>F</tt> or <tt>U</tt>
 * (either upper- or lower-case), the string will be successfully converted
 * to true, false or unknown respectively.  If the string is one of
 * <tt>1</tt>, <tt>-1</tt> or <tt>0</tt>, the string will likewise be
 * converted to true, false or unknown respectively.  Otherwise the
 * conversion will be unsuccessful and argument \a dest will be set to
 * unknown.
 *
 * \ifacespython Not present.
 *
 * @param str the string to convert.
 * @param dest the variable in which to store the resulting set of booleans.
 * @return \c true if the conversion was successful or \c false otherwise.
 */
bool valueOf(const std::string& str, NTriBool& dest);
/**
 * Converts the entire given string to a set of booleans and reports whether
 * this conversion was successful.
 *
 * A set of booleans is represented by one of the four strings
 * <tt>--</tt>, <tt>T-</tt>, <tt>-F</tt> or <tt>TF</tt>.  If the
 * conversion is unsuccessful, argument \a dest will be set to
 * NBoolSet::sNone and \c false will be returned.
 *
 * \ifacespython Not present.
 *
 * @param str the string to convert.
 * @param dest the variable in which to store the resulting set of booleans.
 * @return \c true if the conversion was successful or \c false otherwise.
 */
bool valueOf(const std::string& str, NBoolSet& dest);

/**
 * Decomposes the given string into tokens.
 * This is an extremely simple tokeniser; tokens are defined to be
 * separated by whitespace.
 *
 * \ifacespython Not present.
 *
 * @param results the output iterator to which the resulting tokens will
 * be written; this must accept objects of type <tt>const std::string&</tt>.
 * @param str the string to decompose.
 * @return the number of tokens found.
 */
template <class OutputIterator>
unsigned basicTokenise(OutputIterator results, const std::string& str);

/*@}*/

} // namespace regina

// Template definitions

#include "utilities/stringutils.tcc"

#endif

