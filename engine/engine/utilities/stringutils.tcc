
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/* To be included from stringutils.h. */

#include <cctype>

namespace regina {

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

    return total;
}
    
} // namespace regina

