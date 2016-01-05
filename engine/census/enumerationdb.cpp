
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

#include "regina-config.h" // For QDBM-related macros

#include <algorithm>
#include <cstdlib>
#include <sstream>
#ifdef QDBM_AS_TOKYOCABINET
#include <depot.h>
#include <cabin.h>
#include <villa.h>
#else
#include <stdbool.h> // cstdbool only works for c++11
#include <stdint.h> // cstdint only works for c++11
#include <tcbdb.h>
#include <tcutil.h>
#endif
#include "census/enumerationdb.h"
#include "file/nglobaldirs.h"
#include "triangulation/ntriangulation.h"
#include "utilities/memutils.h"

namespace regina {

bool EnumerationDB::lookup(const std::string& fpg,
        std::list<NTriangulation*> **result) {
    try {
        *result = & store.at(fpg);
        return true;
    } catch (const std::out_of_range& oor) {
        // Just means this fpg has not been cached into memory yet.
        *result = new std::list<NTriangulation*>; // Creates empty list
#ifdef QDBM_AS_TOKYOCABINET
        VILLA* db;
        if (! (db = vlopen(filename_.c_str(), VL_OREADER, VL_CMPLEX))) {
            std::cerr << "ERROR: Could not open QDBM database: "
                << filename_ << std::endl;
            return false;
        }

        CBLIST* records = vlgetlist(db, fpg.c_str(), fpg.length());
        if (records) {
            int n = cblistnum(records);
            for (int i = 0; i < n; ++i) {
                NTriangulation *tri =
                    NTriangulation::fromIsoSig(cblistval(records, i, 0));
                (*result)->push_back(tri);
            }
            cblistclose(records);
        }

        vlclose(db);
#else
        TCBDB* db = tcbdbnew();
        if (! tcbdbopen(db, filename_.c_str(), BDBOREADER)) {
            std::cerr << "ERROR: Could not open Tokyo Cabinet database: "
                << filename_ << std::endl;
            return false;
        }
        TCLIST* records = tcbdbget4(db, fpg.c_str(), fpg.length());
        if (records) {
            int n = tclistnum(records);
            for (int i = 0; i < n; ++i) {
                NTriangulation *tri =
                    NTriangulation::fromIsoSig(tclistval2(records, i));
                (*result)->push_back(tri);
            }
            tclistdel(records);
        }

        tcbdbclose(db);
        tcbdbdel(db);
#endif
        store[fpg] = **result;
    }
    return true;
}

std::list<NTriangulation*> EnumerationDB::lookup(const NFacePairing& fp) {
    std::list<NTriangulation*> ** result = new std::list<NTriangulation*>*;
    lookup(fp.str(),result);
    return **result;
}


} // namespace regina

