
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Combine several data files into a single larger data file             *
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

#include <cstdlib>
#include <cstring>
#include <list>
#include "packet/container.h"

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName <<
        " [ -o <output-file> ] <data-file> ...\n"
        "    " << progName << " [ -v, --version | -?, --help ]\n";
    std::cerr << std::endl;
    std::cerr << "    -o <output-file> : Write to the given data file "
        "(otherwise standard\n";
    std::cerr << "                       output is used)\n";
    std::cerr << "    -v, --version    : Show which version of Regina "
        "is being used\n";
    std::cerr << "    -?, --help       : Display this help\n";
    exit(1);
}

int main(int argc, char* argv[]) {
    // Parse the command line.
    std::list<std::string> files;
    std::string outputFile;
    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '-') {
            // Option.

            // Standard arguments:
            if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0)
                usage(argv[0]);
            if (strcmp(argv[i], "-v") == 0 ||
                    strcmp(argv[i], "--version") == 0) {
                if (argc != 2)
                    usage(argv[0], "Option --version cannot be "
                        "used with any other arguments.");
                std::cout << PACKAGE_BUILD_STRING << std::endl;
                exit(0);
            }

            // Is it an argument we don't understand?
            if (strcmp(argv[i], "-o") != 0)
                usage(argv[0], std::string("Invalid option: ") + argv[i]);

            // Are we missing the actual output filename?
            if (i == argc - 1)
                usage(argv[0], "Option -o is missing an output filename.");

            // Have we already specified an output filename?
            if (! outputFile.empty())
                usage(argv[0],
                    "More than one output filename has been supplied.");

            // Suck in the output filename.
            outputFile = argv[++i];
        } else if (*argv[i]) {
            // Just an ordinary filename.
            files.emplace_back(argv[i]);
        } else
            usage(argv[0], "Empty arguments are not allowed.");
    }

    if (files.empty())
        usage(argv[0], "No data files have been supplied.");

    // Read the input files one by one.
    std::shared_ptr<regina::Packet> ans = std::make_shared<regina::Container>();
    ans->setLabel("Combined Data");

    bool error = false;
    for (const auto& f : files) {
        std::shared_ptr<regina::Packet> data = regina::open(f.c_str());
        if (!data) {
            std::cerr << "File " << f << " could not be read.\n";
            error = true;
            continue;
        }

        ans->append(data);
    }

    // Tidy up the final data file and write it.
    if (outputFile.empty())
        ans->writeXMLFile(std::cout);
    else if (! ans->save(outputFile.c_str())) {
        std::cerr << "File " << outputFile << " could not be written.\n";
        error = true;
    }

    return (error ? 1 : 0);
}

