
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Create a census database from a list of key-value pairs               *
 *                                                                        *
 *  Copyright (c) 2014-2016, Ben Burton                                   *
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

#include "regina-config.h" // For QDBM-related macros

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>
#ifdef QDBM_AS_TOKYOCABINET
#include <depot.h>
#include <cabin.h>
#include <villa.h>
#else
#include <cstdbool>
#include <cstdint>
#include <tcbdb.h>
#include <tcutil.h>
#endif
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#ifdef QDBM_AS_TOKYOCABINET
  #define DB_CLOSE(x) vlclose(x);
#else
  #define DB_CLOSE(x) { tcbdbclose(x); tcbdbdel(x); }
#endif

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " <input-file> <output-file>\n";
    std::cerr << std::endl;
    std::cerr << "Key-value data will be read from the input file:\n";
    std::cerr << "<isosig> <name>\n";
    std::cerr << "<isosig> <name>\n";
    std::cerr << "...\n";
    exit(1);
}

int main(int argc, char* argv[]) {
    // Parse the command line.
    if (argc != 3)
        usage(argv[0]);
    std::string outputFile = argv[2];

    // Open the input file.
    std::cout << "Processing: " << argv[1] << std::endl;
    std::ifstream file(argv[1], std::ios_base::in | std::ios_base::binary);
    if (! file) {
        std::cerr << "ERROR: Could not open input file: " << argv[1]
            << std::endl;
        std::exit(1);
    }

    ::boost::iostreams::filtering_istream in;
    if (file.peek() == 0x1f)
        in.push(::boost::iostreams::gzip_decompressor());
    in.push(file);

    // Initialise the database.
#ifdef QDBM_AS_TOKYOCABINET
    VILLA* db;
    if (! (db = vlopen(outputFile.c_str(),
            VL_OWRITER | VL_OCREAT | VL_OTRUNC | VL_OZCOMP, VL_CMPLEX))) {
        std::cerr << "ERROR: Could not open QDBM database: "
            << outputFile << std::endl;
        std::exit(1);
    }
#else
    TCBDB* db = tcbdbnew();
    if (! tcbdbopen(db, outputFile.c_str(),
            BDBOWRITER | BDBOCREAT | BDBOTRUNC)) {
        std::cerr << "ERROR: Could not open Tokyo Cabinet database: "
            << outputFile << std::endl;
        std::exit(1);
    }
#endif

    // Fill the database with the user-supplied key-value pairs.
    std::string sig, name;
    const char* pos;
    unsigned long tot = 0;
    while (true) {
        in >> sig;
        if (in.eof())
            break;

        std::getline(in, name);
        if (in.eof()) {
            std::cerr << "ERROR: Signature " << sig
                << " is missing a corresponding name.\n\n";
            DB_CLOSE(db);
            usage(argv[0]);
        }

        // Skip initial whitespace in the manifold name (which will
        // always be present, since the previous in >> sig
        // does not eat the separating whitespace).
        pos = name.c_str();
        while (*pos && std::isspace(*pos))
            ++pos;
        if (! *pos) {
            std::cerr << "ERROR: Signature " << sig
                << " has an empty name.\n\n";
            DB_CLOSE(db);
            usage(argv[0]);
        }

#ifdef QDBM_AS_TOKYOCABINET
        if (! vlput(db, sig.c_str(), sig.length(),
                pos, -1 /* strlen */, VL_DDUP)) {
#else
        if (! tcbdbputdup2(db, sig.c_str(), pos)) {
#endif
            std::cerr << "ERROR: Could not store the record for "
                << sig << " in the database." << std::endl;
            DB_CLOSE(db);
            std::exit(1);
        }
        ++tot;
    }

    // Close and tidy up.
#ifdef QDBM_AS_TOKYOCABINET
    if (! vloptimize(db)) {
        std::cerr << "ERROR: Could not optimise QDBM database: "
            << outputFile << std::endl;
        DB_CLOSE(db);
        std::exit(1);
    }

    if (! vlclose(db)) {
        std::cerr << "ERROR: Could not close QDBM database: "
            << outputFile << std::endl;
        std::exit(1);
    }
#else
    // The following call to tcbdboptimise() does not change any options
    // other than the bitwise compression option given in the final argument.
    if (! tcbdboptimize(db, 0, 0, 0, -1, -1, BDBTBZIP)) {
        std::cerr << "ERROR: Could not optimise Tokyo Cabinet database: "
            << outputFile << std::endl;
        std::cerr << "Tokyo cabinet error: " << tcerrmsg(tcbdbecode(db))
            << std::endl;
        DB_CLOSE(db);
        std::exit(1);
    }

    if (! tcbdbclose(db)) {
        std::cerr << "ERROR: Could not close Tokyo Cabinet database: "
            << outputFile << std::endl;
        tcbdbdel(db);
        std::exit(1);
    }
    tcbdbdel(db);
#endif

    std::cout << "Success: " << tot << " records." << std::endl;
    return 0;
}
