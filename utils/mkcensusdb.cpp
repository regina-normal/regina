
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Create a census database from a list of key-value pairs               *
 *                                                                        *
 *  Copyright (c) 2014, Ben Burton                                        *
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

/* end stub */

#include <cctype>
#include <cstdlib>
#include <cstdbool>
#include <cstdint>
#include <tcbdb.h>
#include <tcutil.h>
#include "packet/ncontainer.h"

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " <output-file>\n";
    std::cerr << std::endl;
    std::cerr << "You should provide key-value data via standard input:\n";
    std::cerr << "<isosig> <name>\n";
    std::cerr << "<isosig> <name>\n";
    std::cerr << "...\n";
    exit(1);
}

int main(int argc, char* argv[]) {
    // Parse the command line.
    if (argc != 2)
        usage(argv[0]);
    std::string outputFile = argv[1];

    // Initialise the database.
    TCBDB* db = tcbdbnew();
    if (! tcbdbopen(db, outputFile.c_str(),
            BDBOWRITER | BDBOCREAT | BDBOTRUNC)) {
        std::cerr << "ERROR: Could not open database: " << outputFile
            << std::endl;
        std::exit(1);
    }

    // Fill the database with the user-supplied key-value pairs.
    std::string sig, name;
    const char* pos;
    while (true) {
        std::cin >> sig;
        if (std::cin.eof())
            break;

        std::getline(std::cin, name);
        if (std::cin.eof()) {
            std::cerr << "ERROR: Signature " << sig
                << " is missing a corresponding name.\n\n";
            tcbdbclose(db);
            tcbdbdel(db);
            usage(argv[0]);
        }

        // Skip initial whitespace in the manifold name (which will
        // always be present, since the previous std::cin >> sig
        // does not eat the separating whitespace).
        const char* pos = name.c_str();
        while (*pos && std::isspace(*pos))
            ++pos;
        if (! *pos) {
            std::cerr << "ERROR: Signature " << sig
                << " has an empty name.\n\n";
            tcbdbclose(db);
            tcbdbdel(db);
            usage(argv[0]);
        }

        if (! tcbdbputdup2(db, sig.c_str(), name.c_str())) {
            std::cerr << "ERROR: Could not store the record for "
                << sig << " in the database." << std::endl;
            tcbdbclose(db);
            tcbdbdel(db);
            std::exit(1);
        }
    }

    // Close and tidy up.
    // The following call to tcbdboptimise() does not change any options
    // other than the bitwise compression option given in the final argument.
    if (! tcbdboptimize(db, 0, 0, 0, -1, -1, BDBTBZIP)) {
        std::cerr << "ERROR: Could not optimise database: " << outputFile
            << std::endl;
        tcbdbclose(db);
        tcbdbdel(db);
        std::exit(1);
    }

    if (! tcbdbclose(db)) {
        std::cerr << "ERROR: Could not close database: " << outputFile
            << std::endl;
        tcbdbdel(db);
        std::exit(1);
    }

    tcbdbdel(db);
    return 0;
}
