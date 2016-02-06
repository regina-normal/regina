
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

#include "utilities/osutils.h"

#include <cstdio>
#include <ctime>
#include <iostream>

namespace regina {

void writeResUsage(std::ostream& out) {
    FILE* stat = fopen("/proc/self/stat", "r");
    if (! stat) {
        out << "no /proc/self/stat";
        return;
    }

    unsigned long utime, stime, vsize;

    int dtmp;
    char stmp[256];
    unsigned long lutmp;
    long ldtmp;

    if (fscanf(stat, "%d%255s%255s%d%d%d%d%d%lu%lu%lu%lu%lu"
            "%lu%lu%ld%ld%ld%ld%ld%ld%lu%lu",
            &dtmp, stmp, stmp, &dtmp, &dtmp, &dtmp, &dtmp, &dtmp,
            &lutmp, &lutmp, &lutmp, &lutmp, &lutmp,
            &utime, &stime, &ldtmp, &ldtmp, &ldtmp, &ldtmp, &ldtmp, &ldtmp,
            &lutmp, &vsize) == 23)
        out << "utime=" << utime << ", stime=" << stime << ", vsize=" << vsize;
    else
        out << "could not parse /proc/self/stat";

    fclose(stat);
}

} // namespace regina

