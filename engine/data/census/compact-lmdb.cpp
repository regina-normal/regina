
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Clone and compact an LMDB database                                    *
 *                                                                        *
 *  This program clones a database in a way that omits free pages.        *
 *  This is found to solve the problem under Windows where a database     *
 *  occupies the entire maximum map size that was set upon creation.      *
 *                                                                        *
 *  Copyright (c) 2021-2025, Ben Burton                                   *
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

#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include <lmdb.h>

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " <input-file> <output-file>\n";
    exit(1);
}

int main(int argc, char* argv[]) {
    // Parse the command line.
    if (argc != 3)
        usage(argv[0]);
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    int rv;

    // Remove the output file if it exists.
    rv = unlink(outputFile.c_str());
    if (rv && errno != ENOENT) {
        std::cerr << "ERROR: Could not remove old LMDB database: "
            << outputFile << std::endl;
        std::cerr << "Detail: " << strerror(errno) << std::endl;
        std::exit(1);
    }

    std::cout << "Compacting: " << inputFile << std::endl;

    MDB_env* envIn = nullptr;
    if ((rv = ::mdb_env_create(&envIn))) {
        std::cerr << "ERROR: Could not create LMDB environment." << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        std::exit(1);
    }
    if ((rv = ::mdb_env_open(envIn, inputFile.c_str(),
            MDB_RDONLY | MDB_NORDAHEAD | MDB_NOSUBDIR | MDB_NOLOCK, 0664))) {
        std::cerr << "ERROR: Could not open LMDB environment: "
            << inputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_env_close(envIn);
        std::exit(1);
    }

    if ((rv = ::mdb_env_copy2(envIn, outputFile.c_str(), MDB_CP_COMPACT))) {
        std::cerr << "ERROR: Could not copy LMDB environment: "
            << outputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_env_close(envIn);
        std::exit(1);
    }

    ::mdb_env_close(envIn);

    std::cout << "Success: " << outputFile << std::endl;
    return 0;
}
