
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

class CensusHits;

/**
 * \addtogroup census Census of Face Pairings?
 * Census lookup and enumeration for face pairings? of various dimensions.
 * @{
 */

/**
 * Stores the location of one of Regina's enumeration * databases. Only used
 * when enumerating a new census.
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
        bool lookup(const std::string& facePairing, CensusHits* hits);


        /**
         * Ease of use function for lookup(). See alternate description of lookup()
         * above for more details.
         */
        CensusHits* lookup(const NFacePairing& facePairing);
};

/**
 * Stores a single "hit", or a triangulation which has been located in one of
 * Regina's in-built census databases.
 *
 * Note that a given triangulation might have several hits across Regina's
 * databases.  Therefore hits are typically returned using the separate
 * CensusHits class, which essentially represents a list of individual
 * CensusHit objects.
 */
class CensusHit : public boost::noncopyable {
    private:
        const NTriangulation* tri_;
            /**< The human-readable name associated with the triangulation
                 in the database. */
        const EnumerationDB* db_;
            /**< The database in which the triangulation was found. */
        CensusHit* next_;
            /**< A pointer to the next hit in the list that was found for the
                 same triangulation, or 0 if this is the last hit. */

    public:
        /**
         * Returns details of the census database in which the
         * triangulation was found.
         *
         * @return the database for this hit.
         */
        const EnumerationDB* db() const;

        /**
         * Returns the next hit for the same triangulation, or 0 if
         * there are no more hits.
         *
         * Recall that hits are typically returned using the CensusHits
         * class, which stores a list of individual CensusHit objects.
         * To iterate through this list, you should begin by calling
         * CensusHits::first() to retrieve the first hit, and then for each
         * hit call CensusHit::next() (this function) to retrieve the next hit.
         *
         * @return the next hit after this in the list, or 0 if this is the
         * last hit.
         */
        const CensusHit* next() const;

    private:
        /**
         * Constructs a hit with the given details.  The pointer \a next_
         * will be set to null.
         */
        CensusHit(const NTriangulation* tri, const EnumerationDB* db);

    friend class EnumerationDB;
    friend class CensusHits;
};

/**
 * Stores a list of "hits" indicating all the places in which a given
 * triangulation has been located in Regina's various in-built census databases.
 *
 * A given triangulation might have several hits across Regina's databases
 * (or even in the same database).  The complete set of hits for a given
 * triangulation is represented by a single CensusHits object (which is
 * essentially a linked list of individual CensusHit objects).  To iterate
 * through this list, you begin by calling CensusHits::first(), which will
 * return the first hit (or 0 if there are no hits at all).  Then, for each
 * individual hit, you can call CensusHit::next() to retrieve the next hit
 * in the list (this will return 0 if no more hits were found).
 */
class CensusHits : public boost::noncopyable {
    private:
        CensusHit* first_;
            /**< The first hit in the list, or 0 if there are no hits. */
        CensusHit* last_;
            /**< The last hit in the list, or 0 if there are no hits. */
        size_t count_;
            /**< The total number of hits in the list. */

    public:
        /**
         * Constructs an empty set of hits.
         */
        CensusHits();
        /**
         * Destroys this object and all of the individual CensusHit objects
         * that it contains.
         */
        ~CensusHits();
        /**
         * Returns the first hit in this list.
         *
         * To continue iterating, you can call CensusHit::next() upon each
         * individual hit to retrieve the next hit in the list.
         *
         * @return the first hit, or 0 if there are no hits at all.
         */
        CensusHit* first() const;
        /**
         * Returns the total number of hits in this list.
         *
         * This routine is fast, since the number of hits is cached (it
         * does not need to walk through the entire list to count).
         *
         * @return the total number of hits.
         */
        size_t count() const;
        /**
         * Returns whether there are any hits at all.
         *
         * @return \c true if this list is empty (i.e., there are no hits),
         * or \c false if the list is non-empty (i.e., there are one or
         * more hits).
         */
        bool empty() const;

        /**
         * Appends a new hit to the end of this list.  This list will
         * take ownership of the given object.
         *
         * @param hit the hit to append to this list.
         */
        void append(CensusHit* hit);
};

/*@}*/

// Inline functions for EnumerationDB:

inline EnumerationDB::EnumerationDB(const char* filename) :
        filename_(filename) {
}

inline const std::string& EnumerationDB::filename() const {
    return filename_;
}

// Inline functions for CensusHit:

inline CensusHit::CensusHit(const NTriangulation* tri, const EnumerationDB* db) :
        tri_(tri), db_(db), next_(0) {
}

inline const EnumerationDB* CensusHit::db() const {
    return db_;
}

inline const CensusHit* CensusHit::next() const {
    return next_;
}

// Inline functions for CensusHits:

inline CensusHits::CensusHits() : first_(0), last_(0), count_(0) {
}

inline CensusHits::~CensusHits() {
    CensusHit* tmp;
    do {
        tmp = first_;
        first_ = first_->next_;
        delete tmp;
    } while (first_ != last_);
}

inline CensusHit* CensusHits::first() const {
    return first_;
}

inline size_t CensusHits::count() const {
    return count_;
}

inline bool CensusHits::empty() const {
    return ! first_;
}

inline void CensusHits::append(CensusHit* hit) {
    if (! first_)
        last_ = first_ = hit;
    else
        last_ = last_->next_ = hit; // Assigns last_->next_, then last_.
    ++count_;
}

} // namespace regina

#endif

