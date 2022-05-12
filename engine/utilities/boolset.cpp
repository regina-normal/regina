
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

#include "utilities/boolset.h"
#include "utilities/stringutils.h"

#include <iostream>

namespace regina {

bool BoolSet::setStringCode(const std::string& code) {
    // We need to use a temporary, since valueOf() will change ans even
    // if code is not valid.
    BoolSet ans;
    if (valueOf(code, ans)) {
        elements = ans.elements;
        return true;
    } else
        return false;
}

std::ostream& operator << (std::ostream& out, BoolSet set) {
    if (set == BoolSet())
        out << "{ }";
    else if (set == true)
        out << "{ true }";
    else if (set == false)
        out << "{ false }";
    else
        out << "{ true, false }";
    return out;
}

} // namespace regina

