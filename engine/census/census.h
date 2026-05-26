
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

/*! \file census/census.h
 *  \brief Provides facilities for looking up 3-manifold triangulations in
 *  Regina's in-built census databases.
 */

#ifndef __REGINA_CENSUS_H
#ifndef __DOXYGEN
#define __REGINA_CENSUS_H
#endif

#include <list>
#include "regina-core.h"
#include "concepts/core.h"
#include "triangulation/facetpairing3.h"
#include "utilities/boolset.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

class CensusHit;

/**
 * \defgroup census Census of Triangulations
 * Census lookup and enumeration for triangulations of various dimensions.
 */

/**
 * Stores the location and description of one of Regina's in-built census
 * databases.
 *
 * A census database stores a list of key-value pairs.  The keys are
 * isomorphism signatures of triangulations or knot/link signatures (currently
 * second-generation signatures as returned by `Triangulation<dim>::neoSig()`
 * or `Link::neoSig()`, but see the notes below).  The values are
 * human-readable names (typically the names of the triangulations, the links,
 * and/or the underlying manifolds).  A key may appear multiple times
 * (associated with different human-readable names) within the same database.
 *
 * Ordinary users should not need to interact with CensusDB directly;
 * instead you would typically use one of the high-level Census::lookup()
 * routines, which searches all of Regina's in-built databases using the
 * correct type of search key(s).  There are two reasons for this:
 *
 * - The _keys_ used for census databases are subject to change in future
 *   versions of Regina.  Currently (as of Regina 8.0) these keys are
 *   second-generation signatures.
 *
 * - The _format_ used to store census databases is an internal implementation
 *   detail, also subject to change in future releases of Regina.  Even if you
 *   are accessing a specific database via CensusDB, you should only search a
 *   database using the high-level routine CensusDB::lookupKey() and not
 *   attempt to open the database file directly.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup census
 */
class CensusDB {
    private:
        std::string filename_;
            /**< The filename where the database is stored. */
        std::string desc_;
            /**< A human-readable description of this database. */

    public:
        /**
         * Creates a new reference to one of Regina's census databases.
         *
         * This constructor will not run any checks (e.g., it will not verify
         * that the database exists, or that it is stored in the correct
         * format).
         *
         * \param filename the filename where the database is stored.
         * \param desc a human-readable description of the database.
         * See the desc() routine for further information on how this
         * description might be used.
         */
        CensusDB(std::string filename, std::string desc);

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
         * \return the database filename.
         */
        const std::string& filename() const;

        /**
         * Returns a human-readable description of this database.
         *
         * The description could (for instance) be shown to users when
         * giving a list of all available databases, or when identifying
         * in which particular database a match was found.
         *
         * \return the database description.
         */
        const std::string& desc() const;

        /**
         * Searches for the given key in this database.
         *
         * For each match that is found (if any), this routine will call
         * \a action (which must be a function or some other callable type).
         * This action must take exactly one CensusHit argument, which will be
         * passed as a prvalue (so the argument type for \a action could be
         * any of `CensusHit`, `const CensusHit&`, or `CensusHit&&`).
         * The return value of \a action will be ignored (typically \a action
         * would return `void`).
         *
         * Note that the database will be opened and closed every time
         * this routine is called.
         *
         * The argument \a key should be a <i>g</i>th-generation isomorphism
         * signature or knot/link signature, where the generation \a g is
         * passed as a template argument.  Only one value of \a g is allowed:
         * the same generation of signature that the database uses internally
         * for its keys.  This means that the way you call lookupKey() will
         * change if/when the database key type changes; this is by design,
         * since you will of course need to change what you pass as an argument
         * also.
         *
         * If you are using this routine yourself, you will need to
         * include the extra header census/census-impl.h (which is _not_
         * automatically included from this file).  Typically, however,
         * end users can simply use the catch-all Census::lookup() routines
         * and will not need to call this more fine-grained routine.
         *
         * \python This function is available in Python, and the \a action
         * argument may be a pure Python function.  Since Python does not
         * support C++ templates, the generation should be passed as an
         * initial argument at runtime: `lookupKey(generation, isoSig, action)`.
         * If \a generation does not match the one allowed value (i.e., the
         * format used internally by the database), then this routine will
         * throw an InvalidArgument exception.
         *
         * \tparam generation the generation of isomorphism signature or
         * knot/link signature that you are passing in the argument \a sig.
         * Currently \a generation _must_ be 2, since Regina's databases
         * currently use second-generation signatures as their keys.
         *
         * \exception FileError An error occurred at the database level (e.g.,
         * the database could not be opened).
         *
         * \param sig the isomorphism signature or knot/link signature to
         * search for; this must be of the same generation that is passed as a
         * template parameter.
         * \param action a function (or other callable type) that will
         * be called for each match in the database.
         */
        template <int generation, VoidCallback<CensusHit&&> Action>
        requires (generation == 2)
        void lookupKey(const std::string& sig, Action&& action) const;

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

        /**
         * Swaps the contents of this and the given database reference.
         *
         * \param other the database reference whose contents are to be
         * swapped with this.
         */
        void swap(CensusDB& other) noexcept;

        /**
         * Tests whether this and the given object represent the same database.
         *
         * Two databases are considered the same if they have identical
         * filenames (as returned by the filename() function).  The database
         * descriptions are irrelevant here.
         *
         * \param rhs the database to compare this against.
         * \return \c true if and only if this and the given object represent
         * the same database.
         */
        bool operator == (const CensusDB& rhs) const;
};

/**
 * Swaps the contents of the given database references.
 *
 * This global routine simply calls CensusDB::swap(); it is provided
 * so that CensusDB meets the C++ Swappable requirements.
 *
 * \param a the first database reference whose contents should be swapped.
 * \param b the second database reference whose contents should be swapped.
 *
 * \ingroup census
 */
void swap(CensusDB& a, CensusDB& b) noexcept;

/**
 * Stores a single "hit" indicating that some given triangulation or link
 * diagram has been located in one of Regina's in-built census databases.
 *
 * You cannot construct or modify instances of this class yourself,
 * other than through the standard copy/move/swap operations.
 * The only way to create "genuinely" new objects of this class is via
 * the various static Census::lookup() routines.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup census
 */
class CensusHit {
    private:
        std::string name_;
            /**< The human-readable name associated with the triangulation
                 or link diagram in the database. */
        const CensusDB* db_;
            /**< The database in which the triangulation or link diagram
                 was found. */

    public:
        /**
         * Creates a new copy of the given census hit.
         */
        CensusHit(const CensusHit&) = default;
        /**
         * Moves the contents of the given census hit into this new object.
         *
         * The census hit that is passed will no longer be usable.
         */
        CensusHit(CensusHit&&) noexcept = default;
        /**
         * Copies the given census hit into this object.
         */
        CensusHit& operator = (const CensusHit&) = default;
        /**
         * Moves the contents of the given census hit into this object.
         *
         * The census hit that is passed will no longer be usable.
         */
        CensusHit& operator = (CensusHit&&) noexcept = default;
        /**
         * Swaps the contents of this and the given census hit.
         *
         * \param other the census hit whose contents are to be swapped
         * with this.
         */
        void swap(CensusHit& other) noexcept;
        /**
         * Returns the human-readable name associated with the triangulation
         * or link diagram in the database.  This would typically contain the
         * name of the triangulation, the link, and/or the underlying manifold.
         *
         * \return the human-readable name for this hit.
         */
        const std::string& name() const;
        /**
         * Returns details of the census database in which the
         * triangulation or link diagram was found.
         *
         * \return the database for this hit.
         */
        const CensusDB& db() const;

        /**
         * Tests whether this and the given object represent the same
         * census hit.
         *
         * Two census hits are considered the same if they have the same
         * human-readable name and also come from the same database
         * (as identified by the CensusDB comparison operators).
         *
         * \param rhs the census hit to compare this against.
         * \return \c true if and only if this and the given hit are the same.
         */
        bool operator == (const CensusHit& rhs) const;

    private:
        /**
         * Constructs a hit with the given details.
         */
        CensusHit(const char* name, const CensusDB* db);
        /**
         * Constructs a hit with the given details.
         */
        CensusHit(std::string name, const CensusDB* db);

    friend class CensusDB;
};

/**
 * Swaps the contents of the given census hits.
 *
 * This global routine simply calls CensusHit::swap(); it is provided
 * so that CensusHit meets the C++ Swappable requirements.
 *
 * \param a the first census hit whose contents should be swapped.
 * \param b the second census hit whose contents should be swapped.
 *
 * \ingroup census
 */
void swap(CensusHit& a, CensusHit& b) noexcept;

/**
 * A utility class used to search for triangulations across one or more
 * 3-manifold census databases.
 *
 * This class consists of static routines only.  The main entry point
 * (and typically the only way that you would use this class) is via the
 * various static lookup() routines.
 *
 * \warning This class is not thread-safe, in that it performs some global
 * initialisation the first time one of the lookup() functions is called.
 * If you need thread-safety, you can always call lookup() with an empty
 * string when initialising your program, and ensure this has finished before
 * you allow any subsequent "normal" calls to lookup() from other threads.
 *
 * \ingroup census
 */
class Census {
    private:
        static CensusDB* closedOr_;
            /**< The census of closed orientable prime 3-manifold
                 triangulations that are shipped with Regina.
                 This will only be initialised when lookup() is first called. */
        static CensusDB* closedNor_;
            /**< The census of closed non-orientable P²-irreducible 3-manifold
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
         * have an isomorphism signature for the triangulation, then you
         * can call the variant lookup(const std::string&) instead, which
         * (if your signature is of the right generation) will be faster since
         * it avoids some extra overhead.
         *
         * Note that there may be many hits (possibly from multiple databases,
         * and in some cases possibly even within the same database).
         * Therefore a _list_ of hits will be returned, which you can
         * iterate through the individual matches.  Even if there are no
         * matches at all, a list will still be returned; you can call
         * empty() on this list to test whether any matches were found.
         *
         * This routine is fast: it first computes the relevant isomorphism
         * signature of the triangulation, and then performs a
         * logarithmic-time lookup in each database (here "logarithmic"
         * means logarithmic in the size of the database).
         *
         * \exception FileError An error occurred within one of the databases.
         * Typically this would indicate that some database could not be opened
         * (e.g., it might not be installed correctly on the system).
         *
         * \param tri the triangulation that you wish to search for.
         * \return a list of all database matches.
         */
        static std::list<CensusHit> lookup(const Triangulation<3>& tri);
        /**
         * Searches for the given triangulation through all of Regina's
         * in-built census databases.
         *
         * For this routine you specify the triangulation by giving its
         * isomorphism signature.  This may be either second-generation (as
         * returned by `Triangulation<dim>::neoSig()`), or first-generation
         * (as returned by `Triangulation<dim>::isoSig()`); either form of
         * signature will yield the same results.
         *
         * Calling lookup() on an isomorphism signature _may_ be faster than
         * calling lookup() on the triangulation itself:
         *
         * - If the signature is of the same generation as is used internally
         *   by the census databases, this will avoid some overhead (since the
         *   triangulation variant must compute the signature to use as a
         *   lookup key).
         *
         * - If the signature is of a different generation from the one used
         *   internally by the census databases, then this will add overhead
         *   (since this routine will need to reconstruct the triangulation
         *   and then compute the generation of signature that it needs to
         *   perform the internal database lookups).
         *
         * A general rule of thumb is this: if you already have an isomorphism
         * signature, you should call this string-based routine (regardless of
         * which generation of signature you have), since reconstruction is
         * reasonably fast.  If you do not already have an isomorphism
         * signature, just call the triangulation-based lookup routine.
         *
         * Note that there may be many hits (possibly from multiple databases,
         * and in some cases possibly even within the same database).
         * Therefore a _list_ of hits will be returned, which you can
         * iterate through the individual matches.  Even if there are no
         * matches at all, a list will still be returned; you can call
         * empty() on this list to test whether any matches were found.
         *
         * This routine is fast: it first recomputes the generation of
         * isomorphism signature that it needs (but only if the given signature
         * is not already of the correct generation), and then it performs a
         * logarithmic-time lookup in each database (where "logarithmic" means
         * logarithmic in the size of the database).
         *
         * \exception FileError An error occurred within one of the databases.
         * Typically this would indicate that some database could not be opened
         * (e.g., it might not be installed correctly on the system).
         *
         * \param isoSig the isomorphism signature of the triangulation that
         * you wish to search for; this may be either first-generation or
         * second-generation.
         * \return a list of all database matches.
         */
        static std::list<CensusHit> lookup(const std::string& isoSig);

        // Make this class non-constructible.
        Census() = delete;

    private:
        /**
         * Constructs a CensusDB object for one of Regina's in-built
         * census databases, stored in the standard census database
         * location on the filesystem.
         *
         * \param filename the filename for the database, without directory
         * information.  This routine will build the full pathname by joining
         * the given filename with the standard census database directory.
         * \param desc a human-readable description for the database.
         * \return the new database specifier.
         */
        static CensusDB* standardDB(const char* filename, const char* desc);
};

// Inline functions for CensusDB:

inline CensusDB::CensusDB(std::string filename, std::string desc) :
        filename_(std::move(filename)), desc_(std::move(desc)) {
}

inline const std::string& CensusDB::filename() const {
    return filename_;
}

inline const std::string& CensusDB::desc() const {
    return desc_;
}

inline void CensusDB::swap(CensusDB& other) noexcept {
    filename_.swap(other.filename_);
    desc_.swap(other.desc_);
}

inline bool CensusDB::operator == (const CensusDB& rhs) const {
    return filename_ == rhs.filename_;
}

inline void swap(CensusDB& a, CensusDB& b) noexcept {
    a.swap(b);
}

// Inline functions for CensusHit:

inline CensusHit::CensusHit(const char* name, const CensusDB* db) :
        name_(name), db_(db) {
}

inline CensusHit::CensusHit(std::string name, const CensusDB* db) :
        name_(std::move(name)), db_(db) {
}

inline void CensusHit::swap(CensusHit& other) noexcept {
    name_.swap(other.name_);
    std::swap(db_, other.db_);
}

inline const std::string& CensusHit::name() const {
    return name_;
}

inline const CensusDB& CensusHit::db() const {
    return *db_;
}

inline bool CensusHit::operator == (const CensusHit& rhs) const {
    return ((*db_) == (*rhs.db_) && name_ == rhs.name_);
}

inline void swap(CensusHit& a, CensusHit& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

