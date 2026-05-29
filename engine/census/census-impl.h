
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

/*! \file census/gluingperms-impl.h
 *  \brief Contains implementation details for the CensusDB::lookupKey()
 *  template function.
 *
 *  This file is _not_ included automatically by census.h.
 *  However, typical end users should not need to include it, since
 *  the main entry point to the census lookup functions is via the global
 *  Census::lookup() routines.
 */

#ifndef __REGINA_CENSUS_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_CENSUS_IMPL_H
#endif

#include "regina-config.h" // For key-value store macros (REGINA_KVSTORE_*)
#include "utilities/exception.h"

#include <iostream>
#include <cstdlib>
#if defined(REGINA_KVSTORE_QDBM)
  #include <depot.h>
  #include <cabin.h>
  #include <villa.h>
#elif defined(REGINA_KVSTORE_TOKYOCABINET)
  #include <cstdint>
  #include <tcbdb.h>
  #include <tcutil.h>
#elif defined(REGINA_KVSTORE_LMDB)
  #include <lmdb.h>
#else
  #error "No key-value store library was detected!"
#endif

#include "census/census.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

template <int generation, VoidCallback<CensusHit&&> Action>
requires (generation == 2)
void CensusDB::lookupKey(const std::string& sig, Action&& action) const {
    // On some platforms, looking up an empty key triggers the
    // error MDB_BAD_VALSIZE when using LMDB.
    if (sig.empty())
        return; // no hits

#if defined(REGINA_KVSTORE_QDBM)
    VILLA* db;
    if (! (db = vlopen(filename_.c_str(), VL_OREADER | VL_ONOLCK, VL_CMPLEX))) {
        std::cerr << "ERROR: Could not open QDBM database: "
            << filename_ << std::endl;
        std::cerr << "       -> " << dperrmsg(dpecode) << std::endl;
        throw FileError("Could not open QDBM database");
    }

    CBLIST* records = vlgetlist(db, sig.c_str(), sig.length());
    if (records) {
        int n = cblistnum(records);
        for (int i = 0; i < n; ++i)
            action(CensusHit(cblistval(records, i, 0), this));
        cblistclose(records);
    }

    vlclose(db);
#elif defined(REGINA_KVSTORE_TOKYOCABINET)
    TCBDB* db = tcbdbnew();
    if (! tcbdbopen(db, filename_.c_str(), BDBOREADER | BDBONOLCK)) {
        std::cerr << "ERROR: Could not open Tokyo Cabinet database: "
            << filename_ << std::endl;
        std::cerr << "       -> " << tcbdberrmsg(tcbdbecode(db)) << std::endl;
        tcbdbdel(db);
        throw FileError("Could not open Tokyo Cabinet database");
    }

    if (sig.length() > INT_MAX) {
        // This key is too long for Tokyo Cabinet to handle.
        // However.. this also means that we know the database does not
        // contain it.  So instead of writing an error, just treat this
        // as item-not-found.
    } else {
        TCLIST* records = tcbdbget4(db, sig.c_str(),
            static_cast<int>(sig.length()));
        if (records) {
            int n = tclistnum(records);
            for (int i = 0; i < n; ++i)
                action(CensusHit(tclistval2(records, i), this));
            tclistdel(records);
        }
    }

    tcbdbclose(db);
    tcbdbdel(db);
#elif defined(REGINA_KVSTORE_LMDB)
    MDB_env* db = nullptr;
    int rv;
    if ((rv = ::mdb_env_create(&db))) {
        std::cerr << "ERROR: Could not create LMDB environment: "
            << filename_ << std::endl;
        std::cerr << "       -> error code " << rv << std::endl;
        throw FileError("Could not create LMDB environment");
    }
    /*
    // LMDB normally requires that you set the maximum map size before calling
    // mdb_env_open.  However, AFAICT this is not required for read-only access.
    // We therefore skip the call to mdb_env_set_mapsize, since it is better
    // not to hard-code our databases sizes here in the source code.
    if ((rv = ::mdb_env_set_mapsize(db, 1024UL * 1024UL * 50))) {
        std::cerr << "ERROR: Could not set LMDB map size: "
            << filename_ << std::endl;
        std::cerr << "       -> error code " << rv << std::endl;
        ::mdb_env_close(db);
        throw FileError("Could not set LMDB map size");
    }
    */
    // We still need to pass a file mode to mdb_env_open, and we use 0664 here;
    // however, AFAICT this should be ignored in read-only mode because it is
    // only used for newly created files.
    if ((rv = ::mdb_env_open(db, filename_.c_str(),
            MDB_RDONLY | MDB_NOSUBDIR | MDB_NOLOCK, 0664))) {
        std::cerr << "ERROR: Could not open LMDB environment: "
            << filename_ << std::endl;
        std::cerr << "       -> error code " << rv << std::endl;
        ::mdb_env_close(db);
        throw FileError("Could not open LMDB environment");
    }
    MDB_txn* txn = nullptr;
    if ((rv = ::mdb_txn_begin(db, nullptr, MDB_RDONLY, &txn))) {
        std::cerr << "ERROR: Could not create LMDB transaction: "
            << filename_ << std::endl;
        std::cerr << "       -> error code " << rv << std::endl;
        ::mdb_env_close(db);
        throw FileError("Could not create LMDB transaction");
    }
    MDB_dbi dbi = 0;
    if ((rv = ::mdb_dbi_open(txn, nullptr, MDB_DUPSORT, &dbi))) {
        std::cerr << "ERROR: Could not open LMDB database: "
            << filename_ << std::endl;
        std::cerr << "       -> error code " << rv << std::endl;
        ::mdb_txn_abort(txn);
        ::mdb_env_close(db);
        throw FileError("Could not open LMDB database");
    }
    MDB_cursor* cursor = nullptr;
    if ((rv = ::mdb_cursor_open(txn, dbi, &cursor))) {
        std::cerr << "ERROR: Could not create LMDB cursor: "
            << filename_ << std::endl;
        std::cerr << "       -> error code " << rv << std::endl;
        ::mdb_txn_abort(txn);
        ::mdb_env_close(db);
        throw FileError("Could not create LMDB cursor");
    }
    MDB_val key { sig.size(), const_cast<char*>(sig.data()) };
    MDB_val value;
    rv = mdb_cursor_get(cursor, &key, &value, MDB_SET_KEY);
    while (true) {
        if (rv == MDB_SUCCESS) {
            action(CensusHit(
                std::string(static_cast<char*>(value.mv_data), value.mv_size),
                this));
            rv = mdb_cursor_get(cursor, &key, &value, MDB_NEXT_DUP);
        } else if (rv == MDB_NOTFOUND) {
            break;
        } else {
            std::cerr << "ERROR: Could not search LMDB database: "
                << filename_ << std::endl;
            std::cerr << "       -> error code " << rv << std::endl;
            ::mdb_cursor_close(cursor);
            ::mdb_txn_abort(txn);
            ::mdb_env_close(db);
            throw FileError("Could not search LMDB database");
        }
    }
    ::mdb_cursor_close(cursor);
    ::mdb_txn_abort(txn);
    ::mdb_env_close(db);
#endif
}

template <SignatureReconstructible ObjectType>
std::list<CensusHit> CensusCollection<ObjectType>::lookup(
        const ObjectType& object) {
    if (databases_.empty())
        init();

    size_t size = object.size();
    if (size > maxSize_)
        return {}; // there will be no hits

    std::string sig = object.neoSig();

    std::list<CensusHit> hits;
    auto push = [&hits](CensusHit hit) {
        hits.push_back(std::move(hit));
    };

    // The calls to CensusDB::lookupKey() below could throw a FileError.
    for (const auto& db : databases_)
        if (size <= db.maxSize())
            db.template lookupKey<2>(sig, push);
    return hits;
}

template <SignatureReconstructible ObjectType>
std::list<CensusHit> CensusCollection<ObjectType>::lookup(
        const std::string& sig) {
    if (databases_.empty())
        init();

    size_t size;
    try {
        if constexpr (std::same_as<ObjectType, Link>) {
            size = ObjectType::sigDiagramComponentSize(sig);
        } else {
            size = ObjectType::isoSigComponentSize(sig);
        }
    } catch (const InvalidArgument&) {
        return {}; // invalid signature - there will be no hits
    }
    if (size > maxSize_)
        return {}; // too large - there will be no hits

    int generation = ObjectType::sigGeneration(sig);
    if (generation == 0)
        return {}; // invalid signature - there will be no hits

    std::list<CensusHit> hits;
    auto push = [&hits](CensusHit hit) {
        hits.push_back(std::move(hit));
    };

    // The census databases currently use second-generation signatures as keys.
    // Note: "a" is both a first-generation and second-generation signature
    // (for both triangulations and link diagrams).  However, sigGeneration("a")
    // guarantees to return 2 and so it will be handled correctly here.

    // The calls to CensusDB::lookupKey() below could throw a FileError.
    if (generation == 2) {
        for (const auto& db : databases_)
            if (size <= db.maxSize())
                db.template lookupKey<2>(sig, push);
    } else {
        // We need to do the lookup with a second-generation signature.
        try {
            std::string neoSig = ObjectType::fromSig(sig).neoSig();
            for (const auto& db : databases_)
                if (size <= db.maxSize())
                    db.template lookupKey<2>(neoSig, push);
        } catch (const InvalidArgument&) {
            return {}; // invalid signature - there will be no hits
        }
    }
    return hits;
}

} // namespace regina

#endif

