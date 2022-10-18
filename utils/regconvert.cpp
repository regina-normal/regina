
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Convert old-style binary data files to new-style XML                  *
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

#include "packet/packet.h"
#include <cstdlib>

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " [ -2 | -3 ] [ -x | -u ]"
        << " <old-file> [ <new-file> ]\n"
        "    " << progName << " [ -v, --version | -?, --help ]\n";
    std::cerr << std::endl;
    std::cerr << "    -2 : Convert to the old second-generation format used by Regina 3.0-6.0.1\n";
    std::cerr << "    -3 : Convert to the current third-generation file format (default)\n";
    std::cerr << std::endl;
    std::cerr << "    -x : Convert to compressed XML (default)\n";
    std::cerr << "    -u : Convert to uncompressed XML\n\n";
    std::cerr << "    -v, --version : Show which version of Regina "
        "is being used\n";
    std::cerr << "    -?, --help    : Display this help\n";
    std::cerr << std::endl;
    std::cerr << "    <new-file> may be the same as <old-file>.\n";
    std::cerr << "    <new-file> defaults to standard output (implies -u).\n";
    exit(1);
}

int main(int argc, char* argv[]) {
    std::string oldFile, newFile;
    char typeOpt = 0;
    char versionOpt = 0;

    // Parse command line.
    char optChar;
    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '-') {
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

            // Option.
            if (! argv[i][1])
                usage(argv[0], std::string("Invalid option: ") + argv[i]);
            else if (argv[i][2])
                usage(argv[0], std::string("Invalid option: ") + argv[i]);

            // Argument has length 2.
            optChar = argv[i][1];
            if (optChar == 'x' || optChar == 'u') {
                if (typeOpt)
                    usage(argv[0],
                        "More than one file type has been specified.");
                else
                    typeOpt = optChar;
            } else if (optChar == '2' || optChar == '3') {
                if (versionOpt)
                    usage(argv[0],
                        "More than one file format version has been specified.");
                else
                    versionOpt = optChar;
            } else
                usage(argv[0], std::string("Invalid option: ") + argv[i]);
        } else if (*argv[i]) {
            // Filename.
            if (oldFile.empty())
                oldFile = argv[i];
            else if (newFile.empty())
                newFile = argv[i];
            else
                usage(argv[0], "More than two files have been specified.");
        } else
            usage(argv[0], "Empty arguments are not allowed.");
    }

    if (oldFile.empty())
        usage(argv[0], "No files have been specified.");

    // Add default options.
    if (! typeOpt)
        typeOpt = (newFile.empty() ? 'u' : 'x');
    regina::FileFormat version = (versionOpt == '2' ?
        regina::REGINA_XML_GEN_2 : regina::REGINA_XML_GEN_3);

    // Check we're allowed to use stdout if we've asked for it.
    if (newFile.empty() && typeOpt != 'u')
        usage(argv[0],
            "Only uncompressed XML can be written to standard output.");

    // Read the old file.
    std::shared_ptr<regina::Packet> tree = regina::open(oldFile.c_str());
    if (! tree) {
        std::cerr << "File " << oldFile << " could not be read.\n";
        return 1;
    }

    // Write the new file.
    bool result;
    if (newFile.empty()) {
        // Standard output
        if (typeOpt == 'u') {
            tree->writeXMLFile(std::cout, version);
            result = true;
        } else
            result = false;
        newFile = "<stdout>"; // (for error messages)
    } else {
        // Real output file.
        // Use compressed / uncompressed XML
        result = tree->save(newFile.c_str(), typeOpt == 'x', version);
    }

    if (! result) {
        std::cerr << "File " << newFile << " could not be written.\n";
        return 1;
    }

    return 0;
}

