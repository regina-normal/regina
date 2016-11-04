
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file census/census.h
 *  \brief Provides facilities for looking up 3-manifold triangulations in
 *  Regina's in-built census databases.
 */

#ifndef __CENSUS_H
#ifndef __DOXYGEN
#define __CENSUS_H
#endif

#include <boost/noncopyable.hpp>
#include "regina-core.h"
#include "triangulation/facetpairing3.h"
#include "utilities/boolset.h"

namespace regina {

class CensusHits;

/**
 * \addtogroup census Census of Triangulations
 * Census lookup and enumeration for triangulations of various dimensions.
 * @{
 */

/**
 * Stores the location and description of one of Regina's in-built census
 * databases.
 *
 * A census database stores a list of key-value pairs.  The keys are
 * isomorphism signatures of triangulations (as returned by
 * Triangulation<3>::isoSig(), for instance), and the values are
 * human-readable names (typically the names of the triangulations
 * and/or the names of the underlying manifolds).  An isomorphism
 * signature may appear multiple times (with different names)
 * within the same database.
 *
 * The format used to store census databases is an internal implementation
 * detail that may change in future releases of Regina.  End users
 * should only search census databases using high-level routines such as
 * Census::lookup() and CensusDB::lookup().
 */
class REGINA_API CensusDB {
    private:
        std::string filename_;
            /**< The filename where the database is stored. */
        std::string desc_;
            /**< A human-readable description of this database. */

    public:
        /**
         * Creates a new reference to one of Regina's census databases.
         *
         * This constructor will not run any checks (e.g., it will not
         * verify that the database exists, or that it is stored in the correct
         * format).  Note that even if the database does not exist, the
         * lookup() routine will fail gracefully.
         *
         * @param filename the filename where the database is stored.
         * @param desc a human-readable description of the database.
         * See the desc() routine for further information on how this
         * description might be used.
         */
        CensusDB(const std::string& filename, const std::string& desc);

        /**
         * Returns the filename where this database is stored.
         *
         * @return the database filename.
         */
        const std::string& filename() const;

        /**
         * Returns a human-readable description of this database.
         *
         * The description could (for instance) be shown to users when
         * giving a list of all available databases, or when identifying
         * in which particular database a match was found.
         *
         * @return the database description.
         */
        const std::string& desc() const;

        /**
         * Searches for the given isomorphism signature in this database.
         * All matches (if any) will be appended to the given list of hits.
         *
         * Note that the database will be opened and closed every time
         * this routine is called.
         *
         * @param isoSig the isomorphism signature to search for.
         * @param hits the list of hits to which all matches will be appended.
         * @return \c true if the lookup was correctly performed, or \c false
         * if some error occurred (e.g., the database could not be opened).
         * Note in particular that if there were no matches but no errors,
         * then the return value will be \c true.
         */
        bool lookup(const std::string& isoSig, CensusHits* hits) const;
};

/**
 * Stores a single "hit" indicating that some given triangulation has
 * been located in one of Regina's in-built census databases.
 *
 * Note that a given triangulation might have several hits across Regina's
 * databases.  Therefore hits are typically returned using the separate
 * CensusHits class, which essentially represents a list of individual
 * CensusHit objects.
 */
class REGINA_API CensusHit : public boost::noncopyable {
    private:
        const std::string name_;
            /**< The human-readable name associated with the triangulation
                 in the database. */
        const CensusDB* db_;
            /**< The database in which the triangulation was found. */
        CensusHit* next_;
            /**< A pointer to the next hit in the list that was found for the
                 same triangulation, or 0 if this is the last hit. */

    public:
        /**
         * Returns the human-readable name associated with the
         * triangulation in the database.  This typically contains the name of
         * the triangulation and/or the name of the underlying manifold.
         *
         * @return the human-readable name for this hit.
         */
        const std::string& name() const;
        /**
         * Returns details of the census database in which the
         * triangulation was found.
         *
         * @return the database for this hit.
         */
        const CensusDB* db() const;
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
        CensusHit(const char* name, const CensusDB* db);
        /**
         * Constructs a hit with the given details.  The pointer \a next_
         * will be set to null.
         */
        CensusHit(const std::string& name, const CensusDB* db);

    friend class CensusDB;
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
class REGINA_API CensusHits : public boost::noncopyable {
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

/**
 * A utility class used to search for triangulations across one or more
 * 3-manifold census databases.
 */
class REGINA_API Census {
    private:
        static CensusDB* closedOr_;
            /**< The census of closed orientable prime 3-manifold
                 triangulations that are shipped with Regina.
                 This will only be initialised when lookup() is first called. */
        static CensusDB* closedNor_;
            /**< The census of closed non-orientable P^2-irreducible 3-manifold
                 triangulations that are shipped with Regina.
                 This will only be initialised when lookup() is first called. */
        static CensusDB* closedHyp_;
            /**< The census of closed hyperbolic 3-manifold
                 triangulations that are shipped with Regina.
                 This will only be initialised when lookup() is first called. */
        static CensusDB* cuspedHypOr_;
            /**< The census of cusped hyperbolic orientable 3-manifold
                 triangulations that are shipped with Regina.
                 This will only be initialised when lookup() is first called. */
        static CensusDB* cuspedHypNor_;
            /**< The census of cusped hyperbolic non-orientable 3-manifold
                 triangulations that are shipped with Regina.
                 This will only be initialised when lookup() is first called. */
        static CensusDB* hypKnotLink_;
            /**< The census of cusped hyperbolic knot and link complements
                 that are shipped with Regina.
                 This will only be initialised when lookup() is first called. */
        static bool dbInit_;
            /**< Have the census databases been initialised yet? */

    public:
        /**
         * Searches for the given triangulation through all of Regina's
         * in-built census databases.
         *
         * Internally, the census databases store isomorphism signatures
         * as opposed to fully fleshed-out triangulations.  If you already
         * have the isomorphism signature of the triangulation, then you
         * can call the variant lookup(const std::string&) instead, which
         * will be faster since it avoids some extra overhead.
         *
         * Note that there may be many hits (possibly from multiple databases,
         * and in some cases possibly even within the same database).
         * The list of hits will be returned as a CensusHits object,
         * which you can use to iterate through the individual matches.
         * Even if there are no matches at all, a CensusHits object will
         * still be returned; you can call CensusHits::empty() to test
         * whether any matches were found.
         *
         * The CensusHits object that is returned will be newly
         * allocated, and it is the caller's responsibility to destroy it.
         *
         * This routine is fast: it first computes the isomorphism
         * signature of the triangulation, and then performs a
         * logarithmic-time lookup in each database (here "logarithmic"
         * means logarithmic in the size of the database).
         *
         * @param tri the triangulation that you wish to search for.
         * @return a newly created list of all database matches.
         */
        static CensusHits* lookup(const Triangulation<3>& tri);
        /**
         * Searches for the given triangulation through all of Regina's
         * in-built census databases.
         *
         * For this routine you specify the triangulation by giving its
         * isomorphism signature, as returned by Triangulation<3>::isoSig().
         * This is faster than the variant lookup(const Triangulation<3>&),
         * since Regina's census databases store isomorphism signatures
         * internally.  If you do not already know the isomorphism signature,
         * it is fine to just call lookup(const Triangulation<3>&) instead.
         *
         * Note that there may be many hits (possibly from multiple databases,
         * and in some cases possibly even within the same database).
         * The list of hits will be returned as a CensusHits object,
         * which you can use to iterate through the individual matches.
         * Even if there are no matches at all, a CensusHits object will
         * still be returned; you can call CensusHits::empty() to test
         * whether any matches were found.
         *
         * The CensusHits object that is returned will be newly
         * allocated, and it is the caller's responsibility to destroy it.
         *
         * This routine is fast: it first computes the isomorphism
         * signature of the triangulation, and then performs a
         * logarithmic-time lookup in each database (here "logarithmic"
         * means logarithmic in the size of the database).
         *
         * @param isoSig the isomorphism signature of the triangulation
         * that you wish to search for.
         * @return a newly created list of all database matches.
         */
        static CensusHits* lookup(const std::string& isoSig);

    private:
        /**
         * Constructs a CensusDB object for one of Regina's in-built
         * census databases, stored in the standard census database
         * location on the filesystem.
         *
         * @param filename the filename for the database, without directory
         * information.  This routine will build the full pathname by joining
         * the given filename with the standard census database directory.
         * @param desc a human-readable description for the database.
         * @return the new database specifier.
         */
        static CensusDB* standardDB(const char* filename, const char* desc);
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NCensusDB has now been renamed to CensusDB.
 */
REGINA_DEPRECATED typedef CensusDB NCensusDB;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NCensusHit has now been renamed to CensusHit.
 */
REGINA_DEPRECATED typedef CensusHit NCensusHit;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NCensusHits has now been renamed to CensusHits.
 */
REGINA_DEPRECATED typedef CensusHits NCensusHits;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NCensus has now been renamed to Census.
 */
REGINA_DEPRECATED typedef Census NCensus;

/*@}*/

// Inline functions for CensusDB:

inline CensusDB::CensusDB(const std::string& filename,
        const std::string& desc) : filename_(filename), desc_(desc) {
}

inline const std::string& CensusDB::filename() const {
    return filename_;
}

inline const std::string& CensusDB::desc() const {
    return desc_;
}

// Inline functions for CensusHit:

inline CensusHit::CensusHit(const char* name, const CensusDB* db) :
        name_(name), db_(db), next_(0) {
}

inline CensusHit::CensusHit(const std::string& name, const CensusDB* db) :
        name_(name), db_(db), next_(0) {
}

inline const std::string& CensusHit::name() const {
    return name_;
}

inline const CensusDB* CensusHit::db() const {
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
    while (first_) {
        tmp = first_;
        first_ = first_->next_;
        delete tmp;
    }
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

