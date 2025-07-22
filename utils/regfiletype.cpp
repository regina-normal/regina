
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Identify the type (binary/XML) and version of a data file             *
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

#include "file/fileinfo.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " <file> ...\n";
    std::cerr << "    " << progName
        << " [ -v, --version | -?, --help ]\n\n";
    std::cerr << "    -v, --version : Show which version of Regina "
        "is being used\n";
    std::cerr << "    -?, --help    : Display this help\n";
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc < 2)
        usage(argv[0], "Please specify one or more files.");

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

    for (int i = 1; i < argc; i++) {
        if (argc != 2)
            std::cout << "[ " << argv[i] << " ]\n";

        auto info = regina::FileInfo::identify(argv[i]);
        if (info)
            info->writeTextLong(std::cout);
        else
            std::cout << "Unknown file format or file could not be opened.\n";

        if (argc != 2)
            std::cout << std::endl;
    }

    return 0;
}

