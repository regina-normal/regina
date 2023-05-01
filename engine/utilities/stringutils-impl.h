
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

/*! \file utilities/stringutils-impl.h
 *  \brief Contains implementations of template functions from stringutils.h.
 *
 *  This file is automatically included from stringutils.h; there
 *  is no need for end users to include it explicitly.
 */

// Make sure stringutils.h is included first.
#include "utilities/stringutils.h"
#include "utilities/exception.h"

#ifndef __REGINA_STRINGUTILS_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_STRINGUTILS_IMPL_H
#endif

#include <cctype>
#include "maths/integer.h"

namespace regina {

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

} // namespace regina

#endif
