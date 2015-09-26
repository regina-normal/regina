
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

/*! \file census/ncensus.h
 *  \brief Provides facilities for looking up 3-manifold triangulations in
 *  Regina's in-built census databases.
 */

#ifndef __PARTIALCENSUSDB_H
#ifndef __DOXYGEN
#define __PARTIALCENSUSDB_H
#endif

#include <unordered_map>
#include <boost/noncopyable.hpp>
#include "regina-core.h"
#include "census/ngluingpermsearcher.h"

namespace regina {

class NFacePairing;
class PartialCensusDB;

class PartialTriangulationData {
    public:
//        OneStepSearcher::TetVertexState *vertexState;
//        int *vertexStateChanged;
//        int nVertexClasses;
//        OneStepSearcher::TetEdgeState *edgeState;
//        int *edgeStateChanged;
//        int nEdgeClasses;
        int *permIndices_;
//        int *orientation;
        unsigned nTets;

        PartialTriangulationData(const OneStepSearcher *s, int size);
        ~PartialTriangulationData();
};

/**
 * \addtogroup census Census of Triangulations
 * Census lookup and enumeration for triangulations of various dimensions.
 * @{
 */

class PartialCensusHit : public boost::noncopyable {

    private:
        const std::string rep_;
            /**< The human-readable name associated with the triangulation
                 in the database. */
        const PartialCensusDB* db_;
            /**< The database in which the triangulation was found. */
        PartialCensusHit* next_;
            /**< A pointer to the next hit in the list that was found for the
                 same triangulation, or 0 if this is the last hit. */
        PartialTriangulationData* data_;

    public:
        PartialCensusHit(const OneStepSearcher *, int s);
        ~PartialCensusHit();
        /**
         * Returns details of the census database in which the
         * triangulation was found.
         *
         * @return the database for this hit.
         */
        const PartialCensusDB* db() const;
        /**
         * Returns the next hit for the same triangulation, or 0 if
         * there are no more hits.
         *
         * Recall that hits are typically returned using the NCensusHits
         * class, which stores a list of individual NCensusHit objects.
         * To iterate through this list, you should begin by calling
         * NCensusHits::first() to retrieve the first hit, and then for each
         * hit call NCensusHit::next() (this function) to retrieve the next hit.
         *
         * @return the next hit after this in the list, or 0 if this is the
         * last hit.
         */
        PartialCensusHit * next() const;

        const PartialCensusHit * operator++(int);
        const bool operator!=(const PartialCensusHit &) const;

        const PartialTriangulationData * data() { return data_; }


    friend class PartialCensusDB;
    friend class PartialCensusHits;
};

class PartialCensusHits : public boost::noncopyable {
    private:
        PartialCensusHit* first_;
            /**< The first hit in the list, or 0 if there are no hits. */
        PartialCensusHit* last_;
            /**< The last hit in the list, or 0 if there are no hits. */
        size_t count_;
            /**< The total number of hits in the list. */

    public:
        /**
         * Constructs an empty set of hits.
         */
        PartialCensusHits();
        /**
         * Destroys this object and all of the individual NCensusHit objects
         * that it contains.
         */
        ~PartialCensusHits();
        /**
         * Returns the first hit in this list.
         *
         * To continue iterating, you can call NCensusHit::next() upon each
         * individual hit to retrieve the next hit in the list.
         *
         * @return the first hit, or 0 if there are no hits at all.
         */
        PartialCensusHit* first() const;
        /**
         * Returns the first hit in this list.
         *
         * To continue iterating, you can call NCensusHit::next() upon each
         * individual hit to retrieve the next hit in the list.
         *
         * @return the first hit, or 0 if there are no hits at all.
         */
        PartialCensusHit* begin() const;
        /**
         * Returns the first hit in this list.
         *
         * To continue iterating, you can call NCensusHit::next() upon each
         * individual hit to retrieve the next hit in the list.
         *
         * @return the first hit, or 0 if there are no hits at all.
         */
        PartialCensusHit* end() const;
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
        void append(const OneStepSearcher* s, int size);
};

/**
 * Stores the location and description of one of Regina's in-built census
 * databases.
 *
 * A census database stores a list of key-value pairs.  The keys are
 * isomorphism signatures of triangulations (as returned by
 * NTriangulation::isoSig(), for instance), and the values are
 * human-readable names (typically the names of the triangulations
 * and/or the names of the underlying manifolds).  An isomorphism
 * signature may appear multiple times (with different names)
 * within the same database.
 *
 * The format used to store census databases is an internal implementation
 * detail that may change in future releases of Regina.  End users
 * should only search census databases using high-level routines such as
 * NCensus::lookup() and NCensusDB::lookup().
 */
class PartialCensusDB {
    public:
        enum class DBStatus {
            NotFound,
            Found,
            Running
        };

    private:
        std::string filename_;
            /**< The filename where the database is stored. */
        std::string desc_;
            /**< A human-readable description of this database. */

        int minSize_;
        int maxBoundary;

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
        PartialCensusDB(int minSize, int maxBoundary);
        virtual ~PartialCensusDB();

        virtual bool viable(const NFacePairing *) const;

        /**
         * Returns the filename where this database is stored.
         *
         * @return the database filename.
         */
        virtual DBStatus request(const std::string) const;

        virtual PartialCensusHits * retrieve(const std::string);

        virtual void store(const OneStepSearcher *, int, std::string);
};


/*@}*/

// Inline functions for PartialCensusDB:

inline PartialCensusDB::PartialCensusDB(int min, int max) : minSize_(min),
    maxBoundary(max) {
}
inline PartialCensusDB::~PartialCensusDB() {
}

inline bool PartialCensusDB::viable(const NFacePairing *p) const {
    if (p->size() < minSize_)
        return false;
    NTetFace f;
    int boundaryCount = 0;
    for (f.setFirst() ; ! f.isPastEnd(p->size(),false); f++) {
        if (p->dest(f).isBoundary(p->size())) {
            boundaryCount++;
            if (boundaryCount > maxBoundary)
                return false;
        }
    }
    return true;
}

inline PartialCensusDB::DBStatus PartialCensusDB::request(const std::string) const {
    return DBStatus::NotFound;
}

inline void PartialCensusDB::store(const OneStepSearcher *, int, std::string) {
}

inline PartialCensusHits * PartialCensusDB::retrieve(const std::string) {
    return NULL;
}

inline PartialCensusHit * PartialCensusHits::begin() const {
    return first_;
}

inline PartialCensusHit * PartialCensusHits::end() const {
    return last_;
}

inline PartialCensusHits::PartialCensusHits() : count_(0) {
    first_ = last_ = NULL;
}

inline PartialCensusHit * PartialCensusHit::next() const {
    return next_;
}

inline const PartialCensusHit * PartialCensusHit::operator++(int i) {
//    const PartialCensusHit *n = this;
//    while (( i > 0 ) && (n->next())) {
//        n = n->next();
//        i--;
//    }
    return this->next();
}

inline const bool PartialCensusHit::operator!=(const PartialCensusHit &other) const {
    return (data_ == other.data_);
}


class InMemoryDB : public PartialCensusDB {
    public:
        InMemoryDB(int size, int maxBoundary);
        virtual ~InMemoryDB();

    private:
        std::unordered_map<std::string, PartialCensusHits*> data_;

    public:
        virtual PartialCensusDB::DBStatus request(const std::string) const;

        virtual PartialCensusHits * retrieve(const std::string);

        virtual void store(const OneStepSearcher *, int size, std::string);
};

inline InMemoryDB::InMemoryDB(int s, int b) : PartialCensusDB(s,b) {
}

inline InMemoryDB::~InMemoryDB() {
    unsigned long l=0;
    for (auto theHits : data_) {
        l+= theHits.second->count();
        std::cout << "Graph: " << theHits.first << " has " <<
            theHits.second->count() << " hits." << std::endl;
    }
    std::cout << "In memory storing " << l << " triangulations in total." << std::endl;
}

inline PartialCensusDB::DBStatus InMemoryDB::request(const std::string rep)
    const {
    if (data_.count(rep))
        return PartialCensusDB::DBStatus::Found;
    return PartialCensusDB::DBStatus::NotFound;
}

inline void InMemoryDB::store(const OneStepSearcher *s, int size, std::string
        rep) {
    // Note that this creates the element if it doesn't exist already.
    PartialCensusHits * hits = data_[rep];
    if (hits == NULL) {
        hits = new PartialCensusHits;
        data_[rep] = hits;
    }
    hits->append(s, size);
    //unsigned long l=0;
    //for (auto theHits : data_) {
    //    l+= theHits.second->count();
    //}
    //std::cout << "In memory storing " << l << " triangulations." << std::endl;
}

inline PartialCensusHits * InMemoryDB::retrieve(const std::string rep) {
    // Hope it exists, else this will create an empty (NULL) entry.
    return data_[rep];
}

inline void PartialCensusHits::append(const OneStepSearcher *s, int size) {
    PartialCensusHit * hit = new PartialCensusHit(s, size);
    if (first_) {
        last_->next_ = hit;
        last_ = hit;
    } else {
        first_ = last_ = hit;
    }
    count_++;
}

inline size_t PartialCensusHits::count() const {
    return count_;
}

inline PartialCensusHit::PartialCensusHit(const OneStepSearcher *s, int size) :
    next_(NULL) {
    data_ = new PartialTriangulationData(s,size);
}

inline PartialCensusHit::~PartialCensusHit() {
    delete data_;
}


} // namespace regina

#endif

