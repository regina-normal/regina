
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

#include <cstdlib>
#include <cctype>
#include <climits>
#include "maths/integer.h"
#include "utilities/stringutils.h"
#include "utilities/boolset.h"

namespace regina {

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
        ++start;
    while (start < end && isspace(str[end - 1]))
        --end;

    return str.substr(start, end - start);
}

bool valueOf(const std::string& str, double& dest) {
    if (str.empty() || std::isspace(str.front()))
        return false;

    size_t pos = 0;
    try {
        double ans = std::stod(str, std::addressof(pos));
        if (pos != str.size())
            return false;
        dest = ans;
        return true;
    } catch (const std::logic_error&) {
        // Either the string was unconvertible, or the value was out of range.
        return false;
    }
}

bool valueOf(const std::string& str, bool& dest) {
    if (str.empty())
        return false;

    if (str[0] == 't' || str[0] == 'T' || str[0] == '1') {
        dest = true;
        return true;
    }
    if (str[0] == 'F' || str[0] == 'f' || str[0] == '0') {
        dest = false;
        return true;
    }
    return false;
}

bool valueOf(const std::string& str, BoolSet& dest) {
    return dest.setStringCode(str);
}

std::string stringToToken(std::string str) {
    for (auto& c : str)
        if (isspace(c))
            c = '_';
    return str; // I *believe* this is a move, not a copy (C++ Core Issue 1148).
}

std::vector<std::string> basicTokenise(const std::string& str) {
    std::vector<std::string> ans;

    std::string::size_type len = str.length();
    std::string::size_type pos = 0;

    // Skip initial whitespace.
    while (pos < len && isspace(str[pos]))
        ++pos;

    if (pos == len)
        return {};

    // Extract each token.
    std::string::size_type tokStart;
    while (pos < len) {
        // Find the characters making up this token.
        tokStart = pos;
        while (pos < len && ! isspace(str[pos]))
            ++pos;
        ans.push_back(str.substr(tokStart, pos - tokStart));

        // Skip the subsequent whitespace.
        while (pos < len && isspace(str[pos]))
            ++pos;
    }

    return ans;
}

} // namespace regina
