
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
#include <vector>
#include "regina-core.h"
#include "concepts/maths.h"
#include "file/globaldirs.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

class CensusHit;
class Link;
template <int dim> requires (supportedDim(dim)) class Triangulation;

/**
 * \defgroup census Census of Triangulations
 * Census lookup and enumeration for triangulations of various dimensions.
 */

/**
 * Stores the location and other identifying information for one of Regina's
 * in-built census databases.
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
            /**< A detailed human-readable description of this database. */
        std::string tag_;
            /**< A short human-readable string that identifies this database,
                 or the empty string if no such tag is necessary. */
        size_t maxSize_;
            /**< The maximum number of top-dimensional simplices and/or
                 crossings for any entry in the database, or 0 if this is not
                 known. */

    public:
        /**
         * Creates a new reference to a census database.
         *
         * The database should use the same format as Regina's in-built census
         * databases (and this format depends upon your build configuration);
         * however, it does not actually need to be one of those in-built
         * databases, and it may be located anywhere on the filesystem.
         *
         * This constructor will not run any checks (e.g., it will not verify
         * that the database exists, that it is stored in the correct format,
         * or that the \a maxSize argument is correct).
         *
         * \param filename the filename where the database is stored.
         * \param desc a human-readable description of the database.
         * See the desc() routine for further information on how this
         * description might be used.
         * \param tag a short human-readable string that identifies this
         * database, or the empty string if no such tag is necessary.
         * See tag() for further information on how tags are used.
         * \param maxSize the maximum number of top-dimensional simplices and/or
         * crossings for any entry in the database, or 0 (the default) if this
         * is not known.  This can be used to optimise database lookups.
         */
        CensusDB(std::string filename, std::string desc,
            std::string tag = {}, size_t maxSize = 0);

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
         * Returns a reference to one of Regina's in-built census databases,
         * stored in the standard census database location on the filesystem.
         *
         * The database will be assumed to live in the directory
         * `GlobalDirs::census()`.
         *
         * This variant of global() will not give the database an identifying
         * tag.
         *
         * \param basename the base of the database filename, with no file
         * extension and no directory information.
         * \param desc a human-readable description for the database.
         * \param maxSize the maximum number of top-dimensional simplices and/or
         * crossings for any entry in the database, or 0 (the default) if this
         * is not known.  This can be used to optimise database lookups.
         * \return the resulting database reference.
         */
        static CensusDB global(const char* basename, const char* desc,
            size_t maxSize = 0);

        /**
         * Returns a reference to one of Regina's in-built census databases,
         * stored in the standard census database location on the filesystem.
         *
         * The database will be assumed to live in the directory
         * `GlobalDirs::census()`.
         *
         * \param basename the base of the database filename, with no file
         * extension and no directory information.
         * \param desc a human-readable description for the database.
         * \param tag a short human-readable string that identifies this
         * database; see tag() for further information on how tags are used.
         * \param maxSize the maximum number of top-dimensional simplices and/or
         * crossings for any entry in the database, or 0 (the default) if this
         * is not known.  This can be used to optimise database lookups.
         * \return the resulting database reference.
         */
        static CensusDB global(const char* basename, const char* desc,
            const char* tag, size_t maxSize = 0);

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
         * Returns a short human-readable string that identifies this database,
         * or the empty string if no such tag has been deemed necessary.
         *
         * Tags may be used in scenarios where triangulations or link diagrams
         * are likely to appear in multiple databases, and where the name of
         * a triangulation or link may be confusing without knowledge of
         * which database it came from.
         *
         * An example is Regina's virtual knot census versus Regina's classical
         * knot census, which use the tags `virtual` and `classical`
         * respectively.
         *
         * \return the database tag, or the empty string if there is no tag.
         */
        const std::string& tag() const;

        /**
         * Returns the maximum number of top-dimensional simplices and/or
         * crossings for any entry in this database, or 0 if this information
         * is not known.
         *
         * If this _is_ known, it can be used to optimise database lookups
         * (in particular, to avoid lookups entirely when it is known that the
         * key will not be found).
         *
         * \return the maximum number of top-dimensional simplices and/or
         * crossings.
         */
        size_t maxSize() const;

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
         * initial argument at runtime: `lookupKey(generation, sig, action)`.
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
         *
         * \return a reference to this object.
         */
        CensusDB& operator = (const CensusDB&) = default;

        /**
         * Moves the given database reference into this object.
         *
         * The reference that was passed will no longer be usable.
         *
         * \return a reference to this object.
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
         * descriptions and tags are irrelevant here.
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
 * A collection of references to in-built census databases, all of which store
 * the same type of topological object.
 *
 * Ordinary users should not need to interact with CensusCollection directly;
 * instead you would typically use one of the high-level Census::lookup()
 * routines, which searches all of Regina's in-built databases using the
 * correct type of search key(s).
 *
 * This class is essentially a halfway point between the CensusDB (which
 * manages a single database) and Census (which offers high-level lookup
 * routines for end users).  See those two classes for further information.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \tparam ObjectType the type of object stored in this collection of databases;
 * this would typically be `Triangulation<3>` or `Link`.
 *
 * \ingroup census
 */
template <SignatureReconstructible ObjectType>
class CensusCollection {
    private:
        std::vector<CensusDB> databases_;
            /**< The databases in this collection. */
        size_t maxSize_ { 0 };
            /**< The maximum size (number of crossings, or number of
                 top-dimensional simplices) of any object stored in any of
                 these databases. */

    public:
        /**
         * Creates an empty collection of databases.
         */
        CensusCollection() = default;
        /**
         * Creates a new copy of the given collection.
         */
        CensusCollection(const CensusCollection&) = default;
        /**
         * Moves the given collection into this new object.
         *
         * The collection that was passed will no longer be usable.
         */
        CensusCollection(CensusCollection&&) noexcept = default;
        /**
         * Sets this to be a copy of the given collection.
         *
         * \return a reference to this collection.
         */
        CensusCollection& operator = (const CensusCollection&) = default;
        /**
         * Moves the given collectioninto this object.
         *
         * The collection that was passed will no longer be usable.
         *
         * \return a reference to this collection.
         */
        CensusCollection& operator = (CensusCollection&&) noexcept = default;
        /**
         * Swaps the contents of this and the given collection.
         *
         * \param other the collection whose contents are to be swapped with
         * this.
         */
        void swap(CensusCollection& other) noexcept;

        /**
         * Determines whether this collection of databases is empty.
         *
         * \return `true` if and only if there are no databases in this
         * collection.
         */
        bool empty() const;

        /**
         * Appends a new database to the end of this collection.
         * The database will not be given an identifying tag.
         *
         * The database is assumed to be one of Regina's in-built census
         * databases, stored in the standard census database location
         * described by `GlobalDirs::census()`.
         *
         * \param basename the base of the database filename, with no file
         * extension and no directory information.
         * \param desc a human-readable description for the database.
         * \param maxSize the maximum number of top-dimensional simplices and/or
         * crossings for any entry in the database.  This must not be zero.
         */
        void appendDB(const char* basename, const char* desc, size_t maxSize);

        /**
         * Reserves space for the given number of databases in this collection.
         *
         * This is like `std::vector::reserve()`: it is ultimately harmless if
         * the given size estimate turns out to be wrong.
         *
         * \param the new capacity for this collection of databases.
         */
        void reserve(size_t capacity);

        /**
         * Appends a new database to the end of this collection.
         *
         * The database is assumed to be one of Regina's in-built census
         * databases, stored in the standard census database location
         * described by `GlobalDirs::census()`.
         *
         * \param basename the base of the database filename, with no file
         * extension and no directory information.
         * \param desc a human-readable description for the database.
         * \param tag a short human-readable string that identifies this
         * database; see CensusDB::tag() for further information on how tags
         * are used.
         * \param maxSize the maximum number of top-dimensional simplices and/or
         * crossings for any entry in the database.  This must not be zero.
         */
        void appendDB(const char* basename, const char* desc, const char* tag,
                size_t maxSize);

        /**
         * Searches for the given object within all of the databases in this
         * collection.
         *
         * \exception FileError An error occurred within one of the databases.
         * Typically this would indicate that some database could not be opened
         * (e.g., it might not be installed correctly on the system).
         *
         * \param object the object that you wish to search for.
         * \return a list of all database matches.
         */
        std::list<CensusHit> lookup(const ObjectType& object);

        /**
         * Searches for the object with the given signature within all of the
         * databases in this collection.
         *
         * The given signature may be either second-generation or
         * first-generation; either will yield the same results.
         *
         * Calling lookup() on a signature will yield the same results as
         * calling lookup() on the corresponding object (i.e., the
         * triangulation or link diagram, but it offers different performance:
         *
         * - If the signature is of the _same_ generation as is used internally
         *   by the census databases, then passing a signature directly will
         *   avoid the overhead of computing it.
         *
         * - If the signature is of a _different_ generation from the one used
         *   internally by the census databases, then passing a signature will
         *   _add_ overhead (since it must reconstruct the object and then
         *   compute the generation of signature that the databases need).
         *
         * \exception FileError An error occurred within one of the databases.
         * Typically this would indicate that some database could not be opened
         * (e.g., it might not be installed correctly on the system).
         *
         * \param sig the isomorphism signature or knot/link signature of the
         * object that you wish to search for; this may be either
         * first-generation or second-generation.
         * \return a list of all database matches.
         */
        std::list<CensusHit> lookup(const std::string& sig);
};

/**
 * Swaps the contents of the given database collections.
 *
 * This global routine simply calls CensusCollection::swap(); it is provided
 * so that CensusCollection meets the C++ Swappable requirements.
 *
 * \param a the first database collection whose contents should be swapped.
 * \param b the second database collection whose contents should be swapped.
 *
 * \ingroup census
 */
template <SignatureReconstructible ObjectType>
void swap(CensusCollection<ObjectType>& a, CensusCollection<ObjectType>& b)
    noexcept;

/**
 * Searches for triangulations and link diagrams across Regina's in-built
 * census databases.
 *
 * This class consists of static routines only.  The main entry point
 * (and typically the only way that you would use this class) is via the
 * various static routines lookup() and lookupAs().
 *
 * This class does not initialise the list of databases until the first time
 * they are needed by a lookup routine; moreover, it initialises the 3-manifold
 * databases independently from the link databases.  This means that you _can_
 * call GlobalDirs::setDirs() or GlobalDirs::deduceDirs() if you need to;
 * however, to ensure that the new directory configuration is respected, you
 * must make this GlobalDirs call _before_ the first census lookup.
 *
 * \warning This class is not thread-safe, since (as noted above) it performs
 * global initialisation the first time that a 3-manifold lookup is performed,
 * and also the first time that a link lookup is performed.  If you need thread
 * safety, you can always call lookup() with an empty string when initialising
 * your program, before spawning any other threads.
 *
 * \ingroup census
 */
class Census {
    private:
        /**
         * The in-built census databases for the given type of object, or the
         * empty collection if these have not yet been initialised.
         */
        template <SignatureReconstructible ObjectType>
        inline static CensusCollection<ObjectType> databases_ {};

    public:
        /**
         * Searches for the given triangulation or link diagram within all of
         * Regina's in-built census databases.
         *
         * Internally, the databases store isomorphism signatures and knot/link
         * signature (not fully fleshed-out triangulations and links).  If you
         * already have a signature for your object then you can call the
         * variant `lookupAs<ObjectType>(const std::string&)` instead, which
         * (assuming your signature is of the right generation) will be faster
         * since it avoids some extra overhead.
         *
         * Note that there may be many hits (possibly from multiple databases,
         * and in some cases possibly even within the same database).
         * Therefore a _list_ of hits will be returned, which you can
         * iterate through the individual matches.  Even if there are no
         * matches at all, a list will still be returned; you can call
         * empty() on this list to test whether any matches were found.
         *
         * This routine is fast: it first computes the relevant signature of
         * the object, and then performs a logarithmic-time lookup in each
         * relevant database (here "logarithmic" means logarithmic in the size
         * of the database).
         *
         * \exception FileError An error occurred within one of the databases.
         * Typically this would indicate that some database could not be opened
         * (e.g., it might not be installed correctly on the system).
         *
         * \tparam ObjectType the type of object that you are searching for;
         * this would typically be `Triangulation<3>` or `Link`.
         *
         * \param object the triangulation or link diagram that you wish to
         * search for.
         * \return a list of all database matches.
         */
        template <SignatureReconstructible ObjectType>
        static std::list<CensusHit> lookup(const ObjectType& object);
        /**
         * Searches for the triangulation or link diagram with the given
         * signature within all of Regina's in-built census databases.
         *
         * This routine assumes you know what kind of object you are searching
         * for (i.e., whether it is a 3-manifold triangulation or a link
         * diagram).  You specify the type of object through the template
         * argument \a ObjectType, and you specify the object itself by passing
         * its signature (either an isomorphism signature for a triangulation,
         * or a knot/link signature for a link diagram).  The signature may be
         * either second-generation (from `Triangulation<dim>::neoSig()` or
         * `Link::neoSig()`), or first-generation (from
         * `Triangulation<dim>::isoSig()` or `Link::knotSig()`); either
         * generation of signature will yield the same results.
         *
         * Calling lookupAs() on a signature will yield the same results as
         * calling lookup() on the corresponding triangulation or link diagram,
         * but it offers different performance:
         *
         * - If the signature is of the _same_ generation as is used internally
         *   by the census databases, then passing a signature to lookupAs()
         *   will avoid some overhead (since the variant of lookup() that
         *   takes a triangulation or link diagram must otherwise compute the
         *   signature to use as a lookup key).
         *
         * - If the signature is of a _different_ generation from the one used
         *   internally by the census databases, then lookupAs() will _add_
         *   overhead (since it will need to reconstruct the triangulation
         *   or link diagram and _then_ compute the generation of signature
         *   that it needs to perform the internal database lookups).
         *
         * A general rule of thumb is this: if you already have a signature,
         * you should call this string-based routine (regardless of which
         * generation of signature you have), since reconstruction is
         * reasonably fast.  If you do not already have a signature, just call
         * the triangulation-based or link-based lookup().
         *
         * Note that there may be many hits (possibly from multiple databases,
         * and in some cases possibly even within the same database).
         * Therefore a _list_ of hits will be returned, which you can
         * iterate through the individual matches.  Even if there are no
         * matches at all, a list will still be returned; you can call
         * empty() on this list to test whether any matches were found.
         *
         * This routine is fast: it first recomputes the generation of
         * signature that it needs (but only if the given signature is not
         * already of the correct generation), and then it performs a
         * logarithmic-time lookup in each database (where "logarithmic" means
         * logarithmic in the size of the database).
         *
         * \python Python does not support C++ templates.  Instead you should
         * pass the object type at runtime, using the argument order
         * `lookupAs(objectType, sig)`.  An example that uses the signature of
         * the figure eight knot is `lookupAs(Link, "eputWe")`.
         *
         * \exception FileError An error occurred within one of the databases.
         * Typically this would indicate that some database could not be opened
         * (e.g., it might not be installed correctly on the system).
         *
         * \tparam ObjectType the type of object that you are searching for;
         * this would typically be `Triangulation<3>` or `Link`.
         *
         * \param sig the isomorphism signature or knot/link signature of the
         * triangulation or link diagram that you wish to search for; this may
         * be either first-generation or second-generation.
         * \return a list of all database matches.
         */
        template <SignatureReconstructible ObjectType>
        static std::list<CensusHit> lookupAs(const std::string& sig);
        /**
         * Searches for any triangulation or link diagram with the given
         * signature within all of Regina's in-built census databases, without
         * knowing in advance the original object type.
         *
         * This routine is similar to lookupAs(), except that you do not need
         * to provide an object type in advance.  Instead it will search _all_
         * databases, including triangulations _and_ link diagrams, and will
         * return all hits that it finds.  It is possible (though unlikely)
         * that the hits will include a mix of different types of object.
         *
         * As with lookupAs(), the given signature may be either
         * second-generation (as returned by `Triangulation<dim>::neoSig()` or
         * `Link::neoSig()`), or first-generation (as returned by
         * `Triangulation<dim>::isoSig()` or `Link::knotSig()`).
         *
         * See lookupAs() for further information.
         *
         * \exception FileError An error occurred within one of the databases.
         * Typically this would indicate that some database could not be opened
         * (e.g., it might not be installed correctly on the system).
         *
         * \param sig an isomorphism signature or knot/link signature that you
         * wish to search for; this may be either first-generation or
         * second-generation.
         * \return a list of all database matches.
         */
        static std::list<CensusHit> lookup(const std::string& sig);

        // Make this class non-constructible.
        Census() = delete;

    private:
        /**
         * Fills the database collection for the given object type with the
         * relevant in-built databases that are shipped with Regina.
         *
         * \tparam ObjectType the type of object stored in the collection in
         * question; this would typically be `Triangulation<3>` or `Link`.
         */
        template <SignatureReconstructible ObjectType>
        static void fillDatabases();
};

// Inline functions for CensusDB:

inline CensusDB::CensusDB(std::string filename, std::string desc,
        std::string tag, size_t maxSize) : filename_(std::move(filename)),
        desc_(std::move(desc)), tag_(std::move(tag)), maxSize_(maxSize) {
}

inline CensusDB CensusDB::global(const char* basename, const char* desc,
        size_t maxSize) {
    return CensusDB(GlobalDirs::census() + "/" + basename + "." REGINA_DB_EXT,
        desc, {} /* no tag */, maxSize);
}

inline CensusDB CensusDB::global(const char* basename, const char* desc,
        const char* tag, size_t maxSize) {
    return CensusDB(GlobalDirs::census() + "/" + basename + "." REGINA_DB_EXT,
        desc, tag, maxSize);
}

inline const std::string& CensusDB::filename() const {
    return filename_;
}

inline const std::string& CensusDB::desc() const {
    return desc_;
}

inline const std::string& CensusDB::tag() const {
    return tag_;
}

inline size_t CensusDB::maxSize() const {
    return maxSize_;
}

inline void CensusDB::swap(CensusDB& other) noexcept {
    filename_.swap(other.filename_);
    desc_.swap(other.desc_);
    tag_.swap(other.tag_);
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

// Inline functions for CensusCollection:

template <SignatureReconstructible ObjectType>
inline void CensusCollection<ObjectType>::swap(CensusCollection& other)
        noexcept {
    databases_.swap(other);
    std::swap(maxSize_, other.maxSize_);
}

template <SignatureReconstructible ObjectType>
inline bool CensusCollection<ObjectType>::empty() const {
    return databases_.empty();
}

template <SignatureReconstructible ObjectType>
inline void CensusCollection<ObjectType>::appendDB(const char* basename,
        const char* desc, size_t maxSize) {
    databases_.push_back(CensusDB::global(basename, desc, maxSize));
    if (maxSize > maxSize_)
        maxSize_ = maxSize;
}

template <SignatureReconstructible ObjectType>
inline void CensusCollection<ObjectType>::reserve(size_t capacity) {
    databases_.reserve(capacity);
}

template <SignatureReconstructible ObjectType>
inline void CensusCollection<ObjectType>::appendDB(const char* basename,
        const char* desc, const char* tag, size_t maxSize) {
    databases_.push_back(CensusDB::global(basename, desc, tag, maxSize));
    if (maxSize > maxSize_)
        maxSize_ = maxSize;
}

template <SignatureReconstructible ObjectType>
inline void swap(CensusCollection<ObjectType>& a,
        CensusCollection<ObjectType>& b) noexcept {
    a.swap(b);
}

// Inline functions for Census:

template <SignatureReconstructible ObjectType>
inline std::list<CensusHit> Census::lookup(const ObjectType& object) {
    if (databases_<ObjectType>.empty())
        fillDatabases<ObjectType>();
    return databases_<ObjectType>.lookup(object);
}

template <SignatureReconstructible ObjectType>
inline std::list<CensusHit> Census::lookupAs(const std::string& sig) {
    if (databases_<ObjectType>.empty())
        fillDatabases<ObjectType>();
    return databases_<ObjectType>.lookup(sig);
}

} // namespace regina

#endif

