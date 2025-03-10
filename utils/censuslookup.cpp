
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Look up an isomorphism signature in Regina's census databases         *
 *                                                                        *
 *  Copyright (c) 2014-2025, Ben Burton                                   *
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

#include <cstring>
#include <iostream>
#include "regina-config.h"
#include "census/census.h"
#include "file/globaldirs.h"
#include "triangulation/dim3.h"

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:" << std::endl;
    std::cerr << "    " << progName << " <isosig> ..." << std::endl;
    std::cerr << "    " << progName
        << " [ -v, --version | -?, --help ]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "    -v, --version : Show which version of Regina "
        "is being used" << std::endl;
    std::cerr << "    -?, --help    : Display this help" << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    // Parse the command line.
    if (argc < 2)
        usage(argv[0], "Please specify one or more isomorphism signatures.");

    // Check for standard arguments:
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0)
            usage(argv[0]);
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            if (argc != 2)
                usage(argv[0],
                    "Option --version cannot be used with "
                        "any other arguments.");
            std::cout << PACKAGE_BUILD_STRING << std::endl;
            exit(0);
        }
    }

    // Locate the census data files.
    regina::GlobalDirs::deduceDirs(argv[0]);

    // Search for each signature.
    for (int i = 1; i < argc; ++i) {
        auto hits = regina::Census::lookup(argv[i]);

        size_t n = hits.size();
        std::cout << argv[i] << ": " << n
            << (n == 1 ? " hit" : " hits") << std::endl;

        for (const auto& hit : hits)
            std::cout << "    " << hit.name() << " -- "
                << hit.db().desc() << std::endl;

        std::cout << std::endl;
    }

    return 0;
}
