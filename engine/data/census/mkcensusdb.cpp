
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Create a census database from a list of key-value pairs               *
 *                                                                        *
 *  Copyright (c) 2014-2022, Ben Burton                                   *
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

#include "regina-config.h" // For key-value store macros

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "utilities/zstr.h"

#if defined(REGINA_KVSTORE_QDBM)
  #include <depot.h>
  #include <cabin.h>
  #include <villa.h>
  #define DB_CLOSE(x) vlclose(x);
#elif defined(REGINA_KVSTORE_TOKYOCABINET)
  #include <cstdbool>
  #include <cstdint>
  #include <tcbdb.h>
  #include <tcutil.h>
  #define DB_CLOSE(x) { tcbdbclose(x); tcbdbdel(x); }
#elif defined(REGINA_KVSTORE_LMDB)
  #include <cstring>
  #include <unistd.h>
  #include <lmdb.h>
  #define DB_CLOSE(x) { ::mdb_txn_abort(txn); ::mdb_env_close(x); }
#else
  #error "No key-value store library was detected!"
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

    // Initialise the database.
#if defined(REGINA_KVSTORE_QDBM)
    VILLA* db;
    if (! (db = vlopen(outputFile.c_str(),
            VL_OWRITER | VL_OCREAT | VL_OTRUNC | VL_OZCOMP | VL_ONOLCK,
            VL_CMPLEX))) {
        std::cerr << "ERROR: Could not open QDBM database: "
            << outputFile << std::endl;
        std::cerr << "Detail: " << dperrmsg(dpecode) << std::endl;
        std::exit(1);
    }
#elif defined(REGINA_KVSTORE_TOKYOCABINET)
    TCBDB* db = tcbdbnew();
    if (! tcbdbopen(db, outputFile.c_str(),
            BDBOWRITER | BDBOCREAT | BDBOTRUNC | BDBONOLCK)) {
        std::cerr << "ERROR: Could not open Tokyo Cabinet database: "
            << outputFile << std::endl;
        std::cerr << "Detail: " << tcbdberrmsg(tcbdbecode(db)) << std::endl;
        std::exit(1);
    }
#elif defined(REGINA_KVSTORE_LMDB)
    // LMDB does not offer an "open-and-truncate" option, and if we use
    // mdb_dbi_drop() and re-add the records to a database that was already
    // filled then its size can almost double.  So instead we just remove the
    // old database via the filesystem, before using LMDB at all.
    if (unlink(outputFile.c_str()) && errno != ENOENT) {
        std::cerr << "ERROR: Could not remove old LMDB database: "
            << outputFile << std::endl;
        std::cerr << "Detail: " << strerror(errno) << std::endl;
        std::exit(1);
    }

    MDB_env* db = nullptr;
    if (int rv = ::mdb_env_create(&db)) {
        std::cerr << "ERROR: Could not create LMDB environment." << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        std::exit(1);
    }
    // LMDB requires us to hard-code a maximum map size.
    // Here we set this to 20MB.
    // At the time of writing (3 April 2021) this is enough: the largest
    // database is closed-hyp-or-census, which is around 12MB in size.
    if (int rv = ::mdb_env_set_mapsize(db, 1024UL * 1024UL * 20)) {
        std::cerr << "ERROR: Could not set LMDB map size." << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_env_close(db);
        std::exit(1);
    }
    if (int rv = ::mdb_env_open(db, outputFile.c_str(),
            MDB_NOSUBDIR | MDB_NOLOCK, 0664)) {
        std::cerr << "ERROR: Could not open LMDB environment: "
            << outputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_env_close(db);
        std::exit(1);
    }
    MDB_txn* txn = nullptr;
    if (int rv = ::mdb_txn_begin(db, nullptr, 0, &txn)) {
        std::cerr << "ERROR: Could not create LMDB transaction: "
            << outputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_env_close(db);
        std::exit(1);
    }
    MDB_dbi dbi = 0;
    if (int rv = ::mdb_dbi_open(txn, nullptr, MDB_DUPSORT, &dbi)) {
        std::cerr << "ERROR: Could not open LMDB database: "
            << outputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_txn_abort(txn);
        ::mdb_env_close(db);
        std::exit(1);
    }
#endif

    unsigned long tot = 0;

    // The following try-catch block is to catch input decompression errors.
    try {
        // Prepare to decompress the input file.
        // Note that zstr can handle both compressed and uncompressed inputs.
        zstr::istream in(file);

        // Fill the database with the user-supplied key-value pairs.
        std::string sig, name;
        const char* pos;
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

    #if defined(REGINA_KVSTORE_QDBM)
            if (! vlput(db, sig.c_str(), sig.length(),
                    pos, -1 /* strlen */, VL_DDUP)) {
                std::cerr << "ERROR: Could not store the record for "
                    << sig << " in the database." << std::endl;
                std::cerr << "Detail: " << dperrmsg(dpecode) << std::endl;
                vlclose(db);
                std::exit(1);
            }
    #elif defined(REGINA_KVSTORE_TOKYOCABINET)
            if (! tcbdbputdup2(db, sig.c_str(), pos)) {
                std::cerr << "ERROR: Could not store the record for "
                    << sig << " in the database." << std::endl;
                std::cerr << "Detail: " << tcbdberrmsg(tcbdbecode(db))
                    << std::endl;
                tcbdbclose(db);
                tcbdbdel(db);
                std::exit(1);
            }
    #elif defined(REGINA_KVSTORE_LMDB)
            MDB_val key { sig.size(), sig.data() };
            MDB_val value { strlen(pos), const_cast<char*>(pos) };
            if (int rv = ::mdb_put(txn, dbi, &key, &value, 0)) {
                std::cerr << "ERROR: Could not store the record for "
                    << sig << " in the database." << std::endl;
                std::cerr << "LMDB error code: " << rv << std::endl;
                ::mdb_txn_abort(txn);
                ::mdb_env_close(db);
                std::exit(1);
            }
    #endif
            ++tot;
        }
    } catch (const zstr::Exception& e) {
        std::cerr << "ERROR: Could not read input: " << e.what() << std::endl;
        DB_CLOSE(db);
        std::exit(1);
    }

    // Close and tidy up.
#if defined(REGINA_KVSTORE_QDBM)
    if (! vloptimize(db)) {
        std::cerr << "ERROR: Could not optimise QDBM database: "
            << outputFile << std::endl;
        std::cerr << "Detail: " << dperrmsg(dpecode) << std::endl;
        vlclose(db);
        std::exit(1);
    }

    if (! vlclose(db)) {
        std::cerr << "ERROR: Could not close QDBM database: "
            << outputFile << std::endl;
        std::cerr << "Detail: " << dperrmsg(dpecode) << std::endl;
        std::exit(1);
    }
#elif defined(REGINA_KVSTORE_TOKYOCABINET)
    // The following call to tcbdboptimise() does not change any options
    // other than the bitwise compression option given in the final argument.
    if (! tcbdboptimize(db, 0, 0, 0, -1, -1, BDBTBZIP)) {
        std::cerr << "ERROR: Could not optimise Tokyo Cabinet database: "
            << outputFile << std::endl;
        std::cerr << "Detail: " << tcbdberrmsg(tcbdbecode(db)) << std::endl;
        tcbdbclose(db);
        tcbdbdel(db);
        std::exit(1);
    }

    if (! tcbdbclose(db)) {
        std::cerr << "ERROR: Could not close Tokyo Cabinet database: "
            << outputFile << std::endl;
        std::cerr << "Detail: " << tcbdberrmsg(tcbdbecode(db)) << std::endl;
        tcbdbdel(db);
        std::exit(1);
    }
    tcbdbdel(db);
#elif defined(REGINA_KVSTORE_LMDB)
    if (int rv = ::mdb_txn_commit(txn)) {
        std::cerr << "ERROR: Could not commit LMDB transaction: "
            << outputFile << std::endl;
        std::cerr << "LMDB error code: " << rv << std::endl;
        ::mdb_env_close(db);
        std::exit(1);
    }
    ::mdb_env_close(db);
#endif

    std::cout << "Success: " << tot << " records." << std::endl;
    return 0;
}
