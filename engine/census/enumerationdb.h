
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2015, Ben Burton                                   *
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

/*! \file census/enumerationdb.h
 *  \brief Provides facilities for looking up 3-manifold triangulations in
 *  Regina's in-built census databases. Note that this differs from ncensus.h
 *  in that triangulations are returned when a corresponding face-pairing graph
 *  is given.
 */

#ifndef __ENUMERATIONDB_H
#ifndef __DOXYGEN
#define __ENUMERATIONDB_H
#endif

#include <boost/noncopyable.hpp>
#include "regina-core.h"
#include "census/nfacepairing.h"
#include "triangulation/ntriangulation.h"
#include "utilities/nbooleans.h"

namespace regina {

/**
 * \addtogroup census Census of Face Pairings?
 * Census lookup and enumeration for face pairings? of various dimensions.
 * @{
 */

/**
 * Stores the one of Regina's enumeration databases. Only used when enumerating
 * a new census.
 */
class EnumerationDB {
    private:
        std::string filename_;
            /**< The name of the file (including full path) which stores the
             * on-disk database. */

    private:
        /**
         * Stores all triangulations loaded so far.
         *
         * When a face pairing graph is looked up, this class will first check
         * to see if it exists in this store. If so, it can just return a list
         * of triangulation pointers. If it does not exist yet, it populates
         * the list (creating NTriangulation objects) and returns it.
         */
         std::map<std::string, std::list<NTriangulation*>> store;

    public:
        /**
         * Creates a new reference to one of Regina's enumeration databases.
         *
         * This constructor will not run any checks (e.g., it will not
         * verify that the database exists, or that it is stored in the correct
         * format).  Note that even if the database does not exist, the
         * lookup() routine will fail gracefully.
         *
         * @param filename the full path and filename of the database to access
         */
        EnumerationDB(const char* filename);

        /**
         * Destructor. Need to make sure that store is emptied out.
         */
        ~EnumerationDB();

        /**
         * Returns the full name and path of the database.
         *
         *
         * @return the database path and filename.
         */
        const std::string& filename() const;

        /**
         * Searches for the given face pairing graph in this database.
         * All matches (if any) will be appended to the given list of hits.
         *
         * Note that the database will be opened and closed every time
         * this routine is called.
         *
         * @param facePairing the canonical representation of the face pairing
         * to search for.
         * @param hits the list of hits to which all matches will be appended.
         * @return \c true if the lookup was correctly performed, or \c false
         * if some error occurred (e.g., the database could not be opened).
         * Note in particular that if there were no matches but no errors,
         * then the return value will be \c true.
         */
        bool lookup(const std::string& facePairing,
                std::list<NTriangulation*>** results);


        /**
         * Ease of use function for lookup(). See alternate description of lookup()
         * above for more details.
         */
        std::list<NTriangulation*> lookup(const NFacePairing& facePairing);
};


/*@}*/

// Inline functions for EnumerationDB:

inline EnumerationDB::EnumerationDB(const char* filename) :
        filename_(filename) {
}

inline const std::string& EnumerationDB::filename() const {
    return filename_;
}

} // namespace regina

#endif

