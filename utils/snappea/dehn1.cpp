
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Perform a Dehn filling on a one-cusp SnapPea census manifold          *
 *                                                                        *
 *  Copyright (c) 2004-2016, Ben Burton                                   *
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

#include <SnapPea.h>
#include <unix_cusped_census.h>
#include <unix_file_io.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

std::string triName(Triangulation* tri, double vol) {
    std::string ans;

    switch (get_orientability(tri)) {
        case oriented_manifold:
            ans = "or_"; break;
        case nonorientable_manifold:
            ans = "nor_"; break;
        default:
            ans = "unk_"; break;
    }

    std::ostringstream volStr;
    volStr << std::setprecision(9) << vol;
    while (volStr.str().length() < 10)
        volStr << '0';
    return ans + volStr.str();
}

int main(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
        std::cerr << "Usage: " << argv[0] << " <tri> <m> <l> [output-file]\n";
        std::cerr << "Example: " << argv[0] << " m003 -3 1\n";
        return 1;
    }

    long m = atoi(argv[2]);
    long l = atoi(argv[3]);

    int tets;
    bool orbl;
    switch (argv[1][0]) {
        case 'm':
            tets = 5; orbl = true /* ignored */; break;
        case 's':
            tets = 6; orbl = true; break;
        case 'x':
            tets = 6; orbl = false; break;
        case 'v':
            tets = 7; orbl = true; break;
        case 'y':
            tets = 7; orbl = false; break;
        default:
            std::cerr << "Cannot interpret triangulation name "
                << argv[1] << ".\n";
            return 1;
    }

    Triangulation* tri = GetCuspedCensusManifold(tets,
        orbl ? oriented_manifold : nonorientable_manifold,
        atoi(argv[1] + 1));
    if (! tri) {
        std::cerr << "Could not load triangulation " << argv[1] << ".\n";
        return 1;
    }

    set_cusp_info(tri, 0, 0, m, l);
    do_Dehn_filling(tri);
    double vol = volume(tri, 0);

    Triangulation* filled = fill_cusps(tri, 0,
        const_cast<char*>("filled"), 1);

    std::string name = triName(filled, vol);
    set_triangulation_name(filled, const_cast<char*>(name.c_str()));

    std::cout << "Writing " << name << "... ";
    save_triangulation(filled,
        argc > 4 ? argv[4] : const_cast<char*>(name.c_str()));
    std::cout << "done.\n";

    return 0;
}

