
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

/*! \file census/census.h
 *  \brief Provides facilities for looking up 3-manifold triangulations in
 *  Regina's in-built census databases.
 */

#ifndef __CENSUS_H
#ifndef __DOXYGEN
#define __CENSUS_H
#endif

#include "regina-core.h"
#include "triangulation/facetpairing3.h"
#include "utilities/boolset.h"

namespace regina {

class CensusHits;
class CensusHitIterator;

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
         * Creates a new clone of the given database reference.
         */
        CensusDB(const CensusDB&) = default;

        /**
         * Moves the given database reference into this new object.
         *
         * The reference that was passed will no longer be usable.
         */
        CensusDB(CensusDB&&) noexcept = default;

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

        /**
         * Sets this to be a clone of the given database reference.
         */
        CensusDB& operator = (const CensusDB&) = default;

        /**
         * Moves the given database reference into this object.
         *
         * The reference that was passed will no longer be usable.
         */
        CensusDB& operator = (CensusDB&&) noexcept = default;
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
class REGINA_API CensusHit {
    private:
        const std::string name_;
            /**< The human-readable name associated with the triangulation
                 in the database. */
        const CensusDB* db_;
            /**< The database in which the triangulation was found. */
        CensusHit* next_;
            /**< A pointer to the next hit in the list that was found for the
                 same triangulation, or \c null if this is the last hit. */

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
         * Returns the next hit for the same triangulation, or \c null if
         * there are no more hits.
         *
         * Recall that hits are typically returned using the CensusHits
         * class, which stores a list of individual CensusHit objects.
         * To iterate through this list, you should begin by calling
         * CensusHits::first() to retrieve the first hit, and then for each
         * hit call CensusHit::next() (this function) to retrieve the next hit.
         *
         * @return the next hit after this in the list, or \c null if this is
         * the last hit.
         */
        const CensusHit* next() const;

        // Make this class non-copyable.
        CensusHit(const CensusHit&) = delete;
        CensusHit& operator = (const CensusHit&) = delete;

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
 * essentially a linked list of individual CensusHit objects).
 *
 * The simplest way to iterate through this list is using a C++11
 * range-based \c for loop:
 *
 * \code{.cpp}
 * CensusHits* hits = Census::lookup(...);
 * for (const CensusHit* h : *hits) { ... }
 * \endcode
 *
 * In Python, you can treat this list as an iterable object:
 *
 * \code{.py}
 * hits = Census.lookup(...)
 * for h in hits:
 *     ...
 * \endcode
 *
 * If you wish to iterate through this list manually, you can begin by
 * calling CensusHits::first(), which will return the first hit (or \c null
 * if there are no hits at all).  Then, for each individual hit, you can call
 * CensusHit::next() to retrieve the next hit in the list (this will return
 * \c null if no more hits were found).
 */
class REGINA_API CensusHits {
    private:
        CensusHit* first_;
            /**< The first hit in the list, or \c null if there are no hits. */
        CensusHit* last_;
            /**< The last hit in the list, or \c null if there are no hits. */
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
         * @return the first hit, or \c null if there are no hits at all.
         */
        const CensusHit* first() const;
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
         * Returns an iterator at the beginning of this list of hits.
         *
         * The begin() and end() routines allow you to iterate through
         * all hits in this list using C++11 range-based \c for loops:
         *
         * \code{.cpp}
         * CensusHits* hits = Census::lookup(...);
         * for (const CensusHit* h : *hits) { ... }
         * \endcode
         *
         * In Python, a CensusHits object can be treated as an iterable object:
         *
         * \code{.py}
         * hits = Census.lookup(...)
         * for h in hits:
         *     ...
         * \endcode
         *
         * @return an iterator at the beginning of this list of hits.
         */
        CensusHitIterator begin() const;

        /**
         * Returns an iterator beyond the end of this list of hits.
         *
         * In C++, the begin() and end() routines allow you to iterate through
         * an entire list of census hits using C++11 range-based \c for loops.
         * In Python, a CensusHits object can be treated as an iterable object.
         *
         * See the begin() documentation for further details.
         *
         * @return an iterator beyond the end of this list of hits.
         */
        CensusHitIterator end() const;

        /**
         * Appends a new hit to the end of this list.  This list will
         * take ownership of the given object.
         *
         * This operation does not invalidate any CensusHitIterator iterators.
         *
         * \ifacespython Not present.
         *
         * @param hit the hit to append to this list.
         */
        void append(CensusHit* hit);

        // Make this class non-copyable.
        CensusHits(const CensusHits&) = delete;
        CensusHits& operator = (const CensusHits&) = delete;
};

/**
 * A forward iterator that walks through all hits stored by a single
 * CensusHits object.
 *
 * This header also specialises std::iterator_traits for this iterator class.
 *
 * \ifacespython Instead of the C++ interface described here, in Python
 * the classes CensusHits and CensusHitIterator together implement the
 * Python iterable/iterator interface.  The class CensusHits has just
 * the single function <tt>__iter__()</tt>, which returns a CensusHitIterator;
 * then CensusHitIterator implements <tt>next()</tt>, which either returns
 * the next hit in the census or else throws a <tt>StopException</tt> if
 * there are no more hits to return.
 */
class REGINA_API CensusHitIterator {
    private:
        const CensusHit* current_;
            /**< The hit that this iterator is pointing to, or
                 \c null for a past-the-end iterator. */

    public:
        /**
         * Creates a past-the-end iterator.
         */
        CensusHitIterator();
        /**
         * Default copy constructor.
         */
        CensusHitIterator(const CensusHitIterator&) = default;
        /**
         * Creates a new iterator pointing to the given census hit.
         *
         * @param current the census hit that the new iterator should
         * point to, or \c null if the new iterator should be past-the-end.
         */
        CensusHitIterator(const CensusHit* current);

        /**
         * Default copy assignment operator.
         */
        CensusHitIterator& operator = (const CensusHitIterator&) = default;

        /**
         * Tests whether this and the given iterator are equal.
         *
         * @return true if and only if the two iterators are equal.
         */
        bool operator == (const CensusHitIterator& rhs) const;
        /**
         * Tests whether this and the given iterator are different.
         *
         * @return true if and only if the two iterators are different.
         */
        bool operator != (const CensusHitIterator& rhs) const;

        /**
         * Preincrement operator.  This moves the iterator to point to the
         * next hit for the same triangulation in the census databases,
         * as defined by CensusHit::next().
         *
         * \ifacespython Not present; instead this class implements
         * <tt>next()</tt>, which either returns the current census hit and
         * increments the iterator, or else throws a <tt>StopIteration</tt>
         * exception if the iterator is past-the-end.
         *
         * @return a reference to this iterator.
         */
        CensusHitIterator& operator ++ ();
        /**
         * Postincrement operator.  This moves the iterator to point to the
         * next hit for the same triangulation in the census databases,
         * as defined by CensusHit::next().
         *
         * \ifacespython Not present; instead this class implements
         * <tt>next()</tt>, which either returns the current census hit and
         * increments the iterator, or else throws a <tt>StopIteration</tt>
         * exception if the iterator is past-the-end.
         *
         * @return a a copy of this iterator before it was incremented.
         */
        CensusHitIterator operator ++ (int);

        /**
         * Returns the census hit that this iterator is currently pointing to,
         * or \c null if this iterator is past-the-end.
         *
         * \ifacespython Not present; instead this class implements
         * <tt>next()</tt>, which either returns the current census hit and
         * increments the iterator, or else throws a <tt>StopIteration</tt>
         * exception if the iterator is past-the-end.
         *
         * @return the current census hit, or \c null if this iterator
         * is past-the-end.
         */
        const CensusHit* operator * () const;
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
        static CensusDB* christy_;
            /**< Joe Christy's collection of knot and link complements,
                 which were shipped as a Regina sample file until version 5.96.
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

        // Make this class non-constructible.
        Census() = delete;

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
[[deprecated]] typedef CensusDB NCensusDB;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NCensusHit has now been renamed to CensusHit.
 */
[[deprecated]] typedef CensusHit NCensusHit;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NCensusHits has now been renamed to CensusHits.
 */
[[deprecated]] typedef CensusHits NCensusHits;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NCensus has now been renamed to Census.
 */
[[deprecated]] typedef Census NCensus;

} // namespace regina

/*@}*/

namespace std {
    template <>
    struct iterator_traits<regina::CensusHitIterator> {
        typedef ptrdiff_t difference_type;
        typedef const regina::CensusHit* value_type;
        typedef regina::CensusHit const* const* pointer_type;
        typedef regina::CensusHit const* const& reference_type;
        typedef std::forward_iterator_tag iterator_category;
    };
} // namespace std

namespace regina {

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
        name_(name), db_(db), next_(nullptr) {
}

inline CensusHit::CensusHit(const std::string& name, const CensusDB* db) :
        name_(name), db_(db), next_(nullptr) {
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

inline CensusHits::CensusHits() :
        first_(nullptr), last_(nullptr), count_(0) {
}

inline CensusHits::~CensusHits() {
    CensusHit* tmp;
    while (first_) {
        tmp = first_;
        first_ = first_->next_;
        delete tmp;
    }
}

inline const CensusHit* CensusHits::first() const {
    return first_;
}

inline size_t CensusHits::count() const {
    return count_;
}

inline bool CensusHits::empty() const {
    return ! first_;
}

inline CensusHitIterator CensusHits::begin() const {
    return CensusHitIterator(first_);
}

inline CensusHitIterator CensusHits::end() const {
    return CensusHitIterator(nullptr);
}

inline void CensusHits::append(CensusHit* hit) {
    if (! first_)
        last_ = first_ = hit;
    else
        last_ = last_->next_ = hit; // Assigns last_->next_, then last_.
    ++count_;
}

// Inline functions for CensusHitIterator:

inline CensusHitIterator::CensusHitIterator() : current_(nullptr) {
}

inline CensusHitIterator::CensusHitIterator(const CensusHit* current) :
        current_(current) {
}

inline bool CensusHitIterator::operator == (const CensusHitIterator& rhs)
        const {
    return (current_ == rhs.current_);
}

inline bool CensusHitIterator::operator != (const CensusHitIterator& rhs)
        const {
    return (current_ != rhs.current_);
}

inline CensusHitIterator& CensusHitIterator::operator ++ () {
    current_ = current_->next();
    return *this;
}

inline CensusHitIterator CensusHitIterator::operator ++ (int) {
    CensusHitIterator ans(*this);
    current_ = current_->next();
    return ans;
}

inline const CensusHit* CensusHitIterator::operator * () const {
    return current_;
}

} // namespace regina

#endif

