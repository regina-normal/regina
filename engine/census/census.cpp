
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include <algorithm>
#include <cstdlib>
#include <sstream>
#if defined(REGINA_KVSTORE_QDBM)
  #include <depot.h>
  #include <cabin.h>
  #include <villa.h>
#elif defined(REGINA_KVSTORE_TOKYOCABINET)
  #include <cstdbool>
  #include <cstdint>
  #include <tcbdb.h>
  #include <tcutil.h>
#elif defined(REGINA_KVSTORE_LMDB)
  #include <lmdb.h>
#else
  #error "No key-value store library was detected!"
#endif
#include "census/census.h"
#include "file/globaldirs.h"
#include "triangulation/dim3.h"

namespace regina {

CensusDB* Census::closedOr_ = 0;
CensusDB* Census::closedNor_ = 0;
CensusDB* Census::closedHyp_ = 0;
CensusDB* Census::cuspedHypOr_ = 0;
CensusDB* Census::cuspedHypNor_ = 0;
CensusDB* Census::christy_ = 0;
bool Census::dbInit_ = false;

bool CensusDB::lookup(const std::string& isoSig, CensusHits* hits) const {
    // On some platforms, looking up an empty key triggers the
    // error MDB_BAD_VALSIZE when using LMDB.
    if (isoSig.empty())
        return true;

#if defined(REGINA_KVSTORE_QDBM)
    VILLA* db;
    if (! (db = vlopen(filename_.c_str(), VL_OREADER | VL_ONOLCK, VL_CMPLEX))) {
        std::cerr << "ERROR: Could not open QDBM database: "
            << filename_ << std::endl;
        std::cerr << "       -> " << dperrmsg(dpecode) << std::endl;
        return false;
    }

    CBLIST* records = vlgetlist(db, isoSig.c_str(), isoSig.length());
    if (records) {
        int n = cblistnum(records);
        for (int i = 0; i < n; ++i)
            hits->append(new CensusHit(cblistval(records, i, 0), this));
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
        return false;
    }

    TCLIST* records = tcbdbget4(db, isoSig.c_str(), isoSig.length());
    if (records) {
        int n = tclistnum(records);
        for (int i = 0; i < n; ++i)
            hits->append(new CensusHit(tclistval2(records, i), this));
        tclistdel(records);
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
        return false;
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
        return false;
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
        return false;
    }
    MDB_txn* txn = nullptr;
    if ((rv = ::mdb_txn_begin(db, nullptr, MDB_RDONLY, &txn))) {
        std::cerr << "ERROR: Could not create LMDB transaction: "
            << filename_ << std::endl;
        std::cerr << "       -> error code " << rv << std::endl;
        ::mdb_env_close(db);
        return false;
    }
    MDB_dbi dbi = 0;
    if ((rv = ::mdb_dbi_open(txn, nullptr, MDB_DUPSORT, &dbi))) {
        std::cerr << "ERROR: Could not open LMDB database: "
            << filename_ << std::endl;
        std::cerr << "       -> error code " << rv << std::endl;
        ::mdb_txn_abort(txn);
        ::mdb_env_close(db);
        return false;
    }
    MDB_cursor* cursor = nullptr;
    if ((rv = ::mdb_cursor_open(txn, dbi, &cursor))) {
        std::cerr << "ERROR: Could not create LMDB cursor: "
            << filename_ << std::endl;
        std::cerr << "       -> error code " << rv << std::endl;
        ::mdb_txn_abort(txn);
        ::mdb_env_close(db);
        return false;
    }
    MDB_val key { isoSig.size(), const_cast<char*>(isoSig.data()) };
    MDB_val value;
    rv = mdb_cursor_get(cursor, &key, &value, MDB_SET_KEY);
    while (true) {
        if (rv == MDB_SUCCESS) {
            hits->append(new CensusHit(
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
            return false;
        }
    }
    ::mdb_cursor_close(cursor);
    ::mdb_txn_abort(txn);
    ::mdb_env_close(db);
#endif

    return true;
}

CensusHits* Census::lookup(const Triangulation<3>& tri) {
    return lookup(tri.isoSig());
}

CensusHits* Census::lookup(const std::string& isoSig) {
    if (! dbInit_) {
        closedOr_ = standardDB("closed-or-census-11." REGINA_DB_EXT,
            "Closed census (orientable)");
        closedNor_ = standardDB("closed-nor-census-11." REGINA_DB_EXT,
            "Closed census (non-orientable)");
        closedHyp_ = standardDB("closed-hyp-census-full." REGINA_DB_EXT,
            "Hodgson-Weeks closed hyperbolic census");
        cuspedHypOr_ = standardDB("cusped-hyp-or-census-9." REGINA_DB_EXT,
            "Cusped hyperbolic census (orientable)");
        cuspedHypNor_ = standardDB("cusped-hyp-nor-census-9." REGINA_DB_EXT,
            "Cusped hyperbolic census (non-orientable)");
        christy_ = standardDB("christy-knots-links." REGINA_DB_EXT,
            "Christy's collection of knot/link complements");
        dbInit_ = true;
    }

    CensusHits* hits = new CensusHits;

    closedOr_->lookup(isoSig, hits);
    closedNor_->lookup(isoSig, hits);
    closedHyp_->lookup(isoSig, hits);
    cuspedHypOr_->lookup(isoSig, hits);
    cuspedHypNor_->lookup(isoSig, hits);
    christy_->lookup(isoSig, hits);

    return hits;
}

CensusDB* Census::standardDB(const char* filename, const char* desc) {
    return new CensusDB(GlobalDirs::census() + "/" + filename, desc);
}

} // namespace regina

