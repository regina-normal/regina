
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

#include <regina-config.h>

#include "engine.h"
#include "utilities/intutils.h"

namespace regina {

const char* versionString() {
    return PACKAGE_VERSION;
}

int versionMajor() {
    return PACKAGE_VERSION_MAJOR;
}

int versionMinor() {
    return PACKAGE_VERSION_MINOR;
}

bool versionUsesUTF8(const char* version) {
    // No version is fine.
    // Also any version that does not begin with 0-4 is fine.
    if (version[0] < '0' || version[0] > '4')
        return true;

    if (version[0] < '4') {
        // Deal with 0.x .. 3.x.
        // Unless this is the beginning of a larger number,
        // this is a bad version.
        return (version[0] != '0' &&
            version[1] >= '0' && version[1] <= '9');
    } else {
        // The 4.x case is a bit messier to deal with.

        // If it's the beginning of a larger number, we're fine.
        if (version[1] >= '0' && version[1] <= '9')
            return true;

        // Definitely begins with 4 and 4 alone.
        // The only way to save ourselves is to have 4.x for x >= 5.
        if (version[1] != '.')
            return false;

        // We definitely begin with "4.".
        // The only good possibilities now are to begin with:
        // - 4.[number larger than 4]
        if (version[2] == '0')
            return false;
        else if (version[2] >= '1' && version[2] <= '4')
            return (version[3] >= '0' && version[3] <= '9');
        else if (version[2] >= '5' && version[2] <= '9')
            return true;
        else
            return false;
    }
}

const char* versionSnapPy() {
    return SNAPPY_VERSION;
}

const char* versionSnapPea() {
    return SNAPPY_VERSION;
}

bool hasInt128() {
#ifdef INT128_AVAILABLE
    return true;
#else
    return false;
#endif
}

int testEngine(int value) {
    return value;
}

} // namespace regina

