
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <cstdlib>
#include <cctype>
#include <climits>
#include "maths/integer.h"
#include "utilities/stringutils.h"
#include "utilities/boolset.h"

namespace regina {

char* duplicate(const std::string& str) {
    char* ans = new char[str.length() + 1];

    char* pos = ans;
    for (std::string::const_iterator it = str.begin(); it != str.end(); it++)
        *(pos++) = *it;
    *pos = 0;

    return ans;
}

bool startsWith(const std::string& str, const std::string& prefix) {
    if (str.length() < prefix.length())
        return false;

    auto a = str.begin();
    auto b = prefix.begin();
    for ( ; b != prefix.end(); ++a, ++b)
        if (*a != *b)
            return false;
    return true;
}

std::string stripWhitespace(const std::string& str) {
    std::string::size_type start = 0;
    std::string::size_type end = str.length();

    while (start < end && isspace(str[start]))
        start++;
    while (start < end && isspace(str[end - 1]))
        end--;

    return str.substr(start, end - start);
}

bool valueOf(const std::string& str, int8_t& dest) {
    // TODO: Check errno, and check for overflow when casting back to int.
    char* endPtr;
    dest = static_cast<int8_t>(strtol(str.c_str(), &endPtr, 10));
    return ((! str.empty()) && (*endPtr == 0));
}

bool valueOf(const std::string& str, uint8_t& dest) {
    // TODO: Check errno, and check for overflow when casting back to unsigned.
    char* endPtr;
    dest = static_cast<uint8_t>(strtoul(str.c_str(), &endPtr, 10));
    return ((! str.empty()) && (*endPtr == 0));
}

bool valueOf(const std::string& str, short& dest) {
    // TODO: Check errno, and check for overflow when casting back to int.
    char* endPtr;
    dest = static_cast<short>(strtol(str.c_str(), &endPtr, 10));
    return ((! str.empty()) && (*endPtr == 0));
}

bool valueOf(const std::string& str, unsigned short& dest) {
    // TODO: Check errno, and check for overflow when casting back to unsigned.
    char* endPtr;
    dest = static_cast<unsigned short>(strtoul(str.c_str(), &endPtr, 10));
    return ((! str.empty()) && (*endPtr == 0));
}

bool valueOf(const std::string& str, int& dest) {
    // TODO: Check errno, and check for overflow when casting back to int.
    char* endPtr;
    dest = static_cast<int>(strtol(str.c_str(), &endPtr, 10));
    return ((! str.empty()) && (*endPtr == 0));
}

bool valueOf(const std::string& str, unsigned& dest) {
    // TODO: Check errno, and check for overflow when casting back to unsigned.
    char* endPtr;
    dest = static_cast<unsigned>(strtoul(str.c_str(), &endPtr, 10));
    return ((! str.empty()) && (*endPtr == 0));
}

bool valueOf(const std::string& str, long& dest) {
    char* endPtr;
    dest = strtol(str.c_str(), &endPtr, 10);
    return ((! str.empty()) && (*endPtr == 0));
}

bool valueOf(const std::string& str, unsigned long& dest) {
    char* endPtr;
    dest = strtoul(str.c_str(), &endPtr, 10);
    return ((! str.empty()) && (*endPtr == 0));
}

bool valueOf(const std::string& str, long long& dest) {
    char* endPtr;
    dest = strtoll(str.c_str(), &endPtr, 10);
    return ((! str.empty()) && (*endPtr == 0));
}

bool valueOf(const std::string& str, unsigned long long& dest) {
    char* endPtr;
    dest = strtoull(str.c_str(), &endPtr, 10);
    return ((! str.empty()) && (*endPtr == 0));
}

bool valueOf(const std::string& str, double& dest) {
    char* endPtr;
    dest = strtod(str.c_str(), &endPtr);
    return ((! str.empty()) && (*endPtr == 0));
}

bool valueOf(const std::string& str, bool& dest) {
    if (str.empty()) {
        dest = false;
        return false;
    }
    if (str[0] == 't' || str[0] == 'T' || str[0] == '1') {
        dest = true;
        return true;
    }
    dest = false;
    return (str[0] == 'F' || str[0] == 'f' || str[0] == '0');
}

bool valueOf(const std::string& str, BoolSet& dest) {
    if (str.length() != 2) {
        dest = BoolSet::sNone;
        return false;
    }
    char t = str[0];
    char f = str[1];
    if (t != '-' && t != 'T' && t != 't') {
        dest = BoolSet::sNone;
        return false;
    }
    if (f != '-' && f != 'F' && f != 'f') {
        dest = BoolSet::sNone;
        return false;
    }

    dest = BoolSet(t != '-', f != '-');
    return true;
}

std::string stringToToken(const char* str) {
    std::string ans(str);
    for (std::string::iterator it = ans.begin(); it != ans.end(); it++)
        if (isspace(*it))
            *it = '_';
    return ans;
}

std::string stringToToken(const std::string& str) {
    std::string ans(str);
    for (std::string::iterator it = ans.begin(); it != ans.end(); it++)
        if (isspace(*it))
            *it = '_';
    return ans;
}

} // namespace regina
