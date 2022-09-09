
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#include "utilities/exception.h"
#include "utilities/osutils.h"

#include <cstdio>
#include <ctime>
#include <iostream>

namespace regina {

std::tuple<unsigned long, unsigned long, unsigned long> resUsage() {
    FILE* stat = fopen("/proc/self/stat", "r");
    if (! stat)
        throw FileError("Could not read /proc/self/stat");

    std::tuple<unsigned long, unsigned long, unsigned long> ans;

    int dtmp;
    char stmp[256];
    unsigned long lutmp;
    long ldtmp;

    if (fscanf(stat, "%d%255s%255s%d%d%d%d%d%lu%lu%lu%lu%lu"
            "%lu%lu%ld%ld%ld%ld%ld%ld%lu%lu",
            &dtmp, stmp, stmp, &dtmp, &dtmp, &dtmp, &dtmp, &dtmp,
            &lutmp, &lutmp, &lutmp, &lutmp, &lutmp,
            &std::get<0>(ans), &std::get<1>(ans),
            &ldtmp, &ldtmp, &ldtmp, &ldtmp, &ldtmp, &ldtmp, &lutmp,
            &std::get<2>(ans)) == 23) {
        fclose(stat);
        return ans;
    } else {
        fclose(stat);
        throw FileError("Could not parse /proc/self/stat");
    }
}

void writeResUsage(std::ostream& out) {
    try {
        auto [utime, stime, vsize] = resUsage();
        out << "utime=" << utime << ", stime=" << stime << ", vsize=" << vsize;
    } catch (const FileError& err) {
        out << err.what();
    }
}

} // namespace regina

