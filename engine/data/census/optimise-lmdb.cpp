
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Clone an LMDB database in a way that may reduce its file size         *
 *                                                                        *
 *  This program fills a new database with its records added in sorted    *
 *  order (as opposed to random access).  This solves the problem where   *
 *  adding records in random order inflates the occupied file size.       *
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

    MDB_env* envIn = nullptr;
    MDB_env* envOut = nullptr;
    if ((rv = ::mdb_env_create(&envIn))) {
        std::cerr << "ERROR: Could not create LMDB environment." << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        std::exit(1);
    }
    if ((rv = ::mdb_env_create(&envOut))) {
        std::cerr << "ERROR: Could not create LMDB environment." << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        std::exit(1);
    }
    // LMDB requires us to hard-code a maximum map size for the output file.
    // Here we set this to 20MB.
    // At the time of writing (3 April 2021) this is enough: the largest
    // database is closed-hyp-or-census, which is around 12MB in size.
    if ((rv = ::mdb_env_set_mapsize(envOut, 1024UL * 1024UL * 20))) {
        std::cerr << "ERROR: Could not set LMDB map size." << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_env_close(envIn);
        ::mdb_env_close(envOut);
        std::exit(1);
    }
    if ((rv = ::mdb_env_open(envIn, inputFile.c_str(),
            MDB_RDONLY | MDB_NORDAHEAD | MDB_NOSUBDIR | MDB_NOLOCK, 0664))) {
        std::cerr << "ERROR: Could not open LMDB environment: "
            << inputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_env_close(envIn);
        ::mdb_env_close(envOut);
        std::exit(1);
    }
    if ((rv = ::mdb_env_open(envOut, outputFile.c_str(),
            MDB_NORDAHEAD | MDB_NOSUBDIR | MDB_NOLOCK, 0664))) {
        std::cerr << "ERROR: Could not open LMDB environment: "
            << outputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_env_close(envIn);
        ::mdb_env_close(envOut);
        std::exit(1);
    }

    MDB_txn* txnIn = nullptr;
    MDB_txn* txnOut = nullptr;
    if ((rv = ::mdb_txn_begin(envIn, nullptr, MDB_RDONLY, &txnIn))) {
        std::cerr << "ERROR: Could not create LMDB transaction: "
            << inputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_env_close(envIn);
        ::mdb_env_close(envOut);
        std::exit(1);
    }
    if ((rv = ::mdb_txn_begin(envOut, nullptr, 0, &txnOut))) {
        std::cerr << "ERROR: Could not create LMDB transaction: "
            << outputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_txn_abort(txnIn);
        ::mdb_env_close(envIn);
        ::mdb_env_close(envOut);
        std::exit(1);
    }

    MDB_dbi dbiIn = 0;
    MDB_dbi dbiOut = 0;
    if ((rv = ::mdb_dbi_open(txnIn, nullptr, MDB_DUPSORT, &dbiIn))) {
        std::cerr << "ERROR: Could not open LMDB database: "
            << inputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_txn_abort(txnIn);
        ::mdb_txn_abort(txnOut);
        ::mdb_env_close(envIn);
        ::mdb_env_close(envOut);
        std::exit(1);
    }
    if ((rv = ::mdb_dbi_open(txnOut, nullptr, MDB_DUPSORT, &dbiOut))) {
        std::cerr << "ERROR: Could not open LMDB database: "
            << outputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_txn_abort(txnIn);
        ::mdb_txn_abort(txnOut);
        ::mdb_env_close(envIn);
        ::mdb_env_close(envOut);
        std::exit(1);
    }

    MDB_cursor* cursorIn = nullptr;
    if ((rv = ::mdb_cursor_open(txnIn, dbiIn, &cursorIn))) {
        std::cerr << "ERROR: Could not create LMDB cursor: "
            << inputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_txn_abort(txnIn);
        ::mdb_txn_abort(txnOut);
        ::mdb_env_close(envIn);
        ::mdb_env_close(envOut);
        std::exit(1);
    }

    MDB_val key, value;

    // Our strategy here is just to add all the records in sorted order.
    // This seems to help avoid "holes" in the database (as compared to
    // adding records in an arbitrary order).
    size_t tot = 0;
    rv = mdb_cursor_get(cursorIn, &key, &value, MDB_FIRST);
    while (true) {
        if (rv == MDB_SUCCESS) {
            if ((rv = ::mdb_put(txnOut, dbiOut, &key, &value, 0))) {
                std::cerr << "ERROR: Could not store LMDB record: "
                    << outputFile << std::endl;
                std::cerr << "LMDB error code: " << rv << std::endl;
                ::mdb_cursor_close(cursorIn);
                ::mdb_txn_abort(txnIn);
                ::mdb_txn_abort(txnOut);
                ::mdb_env_close(envIn);
                ::mdb_env_close(envOut);
                std::exit(1);
            }
            ++tot;
            rv = mdb_cursor_get(cursorIn, &key, &value, MDB_NEXT);
        } else if (rv == MDB_NOTFOUND) {
            break;
        } else {
            std::cerr << "ERROR: Could not fetch LMDB record: "
                << inputFile << std::endl;
            std::cerr << "LMDB error code: " << rv << std::endl;
            ::mdb_cursor_close(cursorIn);
            ::mdb_txn_abort(txnIn);
            ::mdb_txn_abort(txnOut);
            ::mdb_env_close(envIn);
            ::mdb_env_close(envOut);
            std::exit(1);
        }
    }

    ::mdb_cursor_close(cursorIn);
    ::mdb_txn_abort(txnIn);
    ::mdb_env_close(envIn);

    if ((rv = ::mdb_txn_commit(txnOut))) {
        std::cerr << "ERROR: Could not commit LMDB transaction: "
            << outputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_env_close(envOut);
        std::exit(1);
    }
    ::mdb_env_close(envOut);

    std::cout << "Success: " << tot << " records." << std::endl;
    return 0;
}
