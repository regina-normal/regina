
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

/*! \file census/ncensus.h
 *  \brief Provides facilities for looking up 3-manifold triangulations in
 *  Regina's in-built census databases.
 */

#ifndef __NCENSUS_H
#ifndef __DOXYGEN
#define __NCENSUS_H
#endif

#include <boost/noncopyable.hpp>
#include "regina-core.h"
#include "triangulation/nfacepairing.h"
#include "utilities/nbooleans.h"

namespace regina {

class NCensusHits;
class NGluingPerms;
class NGluingPermSearcher;
class NPacket;

template <int> class Triangulation;
typedef Triangulation<3> NTriangulation;

/**
 * \addtogroup census Census of Triangulations
 * Census lookup and enumeration for triangulations of various dimensions.
 * @{
 */

/**
 * A legacy typedef that is identical to NCensus::AcceptTriangulation.
 * See NCensus::AcceptTriangulation for further details.
 *
 * \deprecated This global typedef is now deprecated.  Please use the
 * identical class typedef NCensus::AcceptTriangulation instead.  However,
 * be aware that the NCensus enumeration facilities are on their
 * way out of Regina, and in the future the NCensus class will be
 * used purely for census lookups.  If you wish to build a census
 * yourself, you should call NFacePairing::findAllPairings() and
 * NGluingPermSearcher::findAllPerms() directly.
 */
REGINA_DEPRECATED typedef bool (*AcceptTriangulation)(NTriangulation*, void*);

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
class REGINA_API NCensusDB {
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
        NCensusDB(const std::string& filename, const std::string& desc);

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
        bool lookup(const std::string& isoSig, NCensusHits* hits) const;
};

/**
 * Stores a single "hit" indicating that some given triangulation has
 * been located in one of Regina's in-built census databases.
 *
 * Note that a given triangulation might have several hits across Regina's
 * databases.  Therefore hits are typically returned using the separate
 * NCensusHits class, which essentially represents a list of individual
 * NCensusHit objects.
 */
class REGINA_API NCensusHit : public boost::noncopyable {
    private:
        const std::string name_;
            /**< The human-readable name associated with the triangulation
                 in the database. */
        const NCensusDB* db_;
            /**< The database in which the triangulation was found. */
        NCensusHit* next_;
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
        const NCensusDB* db() const;
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
        const NCensusHit* next() const;

    private:
        /**
         * Constructs a hit with the given details.  The pointer \a next_
         * will be set to null.
         */
        NCensusHit(const char* name, const NCensusDB* db);
        /**
         * Constructs a hit with the given details.  The pointer \a next_
         * will be set to null.
         */
        NCensusHit(const std::string& name, const NCensusDB* db);

    friend class NCensusDB;
    friend class NCensusHits;
};

/**
 * Stores a list of "hits" indicating all the places in which a given
 * triangulation has been located in Regina's various in-built census databases.
 *
 * A given triangulation might have several hits across Regina's databases
 * (or even in the same database).  The complete set of hits for a given
 * triangulation is represented by a single NCensusHits object (which is
 * essentially a linked list of individual NCensusHit objects).  To iterate
 * through this list, you begin by calling NCensusHits::first(), which will
 * return the first hit (or 0 if there are no hits at all).  Then, for each
 * individual hit, you can call NCensusHit::next() to retrieve the next hit
 * in the list (this will return 0 if no more hits were found).
 */
class REGINA_API NCensusHits : public boost::noncopyable {
    private:
        NCensusHit* first_;
            /**< The first hit in the list, or 0 if there are no hits. */
        NCensusHit* last_;
            /**< The last hit in the list, or 0 if there are no hits. */
        size_t count_;
            /**< The total number of hits in the list. */

    public:
        /**
         * Constructs an empty set of hits.
         */
        NCensusHits();
        /**
         * Destroys this object and all of the individual NCensusHit objects
         * that it contains.
         */
        ~NCensusHits();
        /**
         * Returns the first hit in this list.
         *
         * To continue iterating, you can call NCensusHit::next() upon each
         * individual hit to retrieve the next hit in the list.
         *
         * @return the first hit, or 0 if there are no hits at all.
         */
        NCensusHit* first() const;
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
        void append(NCensusHit* hit);
};

/**
 * A utility class used to search for triangulations across one or more
 * 3-manifold census databases.
 */
class REGINA_API NCensus {
    public:
        /**
         * This constant indicates that non-minimal triangulations
         * may be ignored.
         *
         * \deprecated This constant is deprecated; please use
         * NGluingPermSearcher::PURGE_NON_MINIMAL instead.
         */
        REGINA_DEPRECATED static const int PURGE_NON_MINIMAL;
        /**
         * This constant indicates that any triangulation that is not
         * prime (i.e., can be written as a non-trivial connected sum)
         * and any bounded triangulation that is reducible over a disc
         * may be ignored.
         *
         * \deprecated This constant is deprecated; please use
         * NGluingPermSearcher::PURGE_NON_PRIME instead.
         */
        REGINA_DEPRECATED static const int PURGE_NON_PRIME;
        /**
         * This constant indicates that any triangulation that is not
         * prime (i.e., can be written as a non-trivial connected sum),
         * any bounded triangulation that is reducible over a disc and
         * any triangulation that is non-minimal may be ignored.
         * Note that this is simply a combination of the constants
         * \a PURGE_NON_MINIMAL and \a PURGE_NON_PRIME.
         *
         * \deprecated This constant is deprecated; please use
         * NGluingPermSearcher::PURGE_NON_MINIMAL_PRIME instead.
         */
        REGINA_DEPRECATED static const int PURGE_NON_MINIMAL_PRIME;
        /**
         * This constant indicates that any triangulation containing an
         * embedded two-sided projective plane may be ignored.
         *
         * \deprecated This constant is deprecated; please use
         * NGluingPermSearcher::PURGE_NON_MINIMAL instead.
         */
        REGINA_DEPRECATED static const int PURGE_P2_REDUCIBLE;

        /**
         * A routine used to determine whether a particular triangulation
         * should be included in a census.  Routines of this type are used by
         * NCensus::formCensus().
         *
         * The first parameter passed should be a triangulation currently
         * under consideration.
         * The second parameter may contain arbitrary data as passed to
         * NCensus::formCensus().
         *
         * The return value should be \c true if the triangulation passed
         * should be included in the census, or \c false otherwise.
         *
         * \deprecated The NCensus enumeration facilities are on their
         * way out of Regina, and in the future the NCensus class will be
         * used purely for census lookups.  If you wish to build a census
         * yourself, you should call NFacePairing::findAllPairings() and
         * NGluingPermSearcher::findAllPerms() directly.
         */
        REGINA_DEPRECATED typedef bool (*AcceptTriangulation)(
            NTriangulation*, void*);

    private:
        static NCensusDB* closedOr_;
            /**< The census of closed orientable prime 3-manifold
                 triangulations that are shipped with Regina.
                 This will only be initialised when lookup() is first called. */
        static NCensusDB* closedNor_;
            /**< The census of closed non-orientable P^2-irreducible 3-manifold
                 triangulations that are shipped with Regina.
                 This will only be initialised when lookup() is first called. */
        static NCensusDB* closedHyp_;
            /**< The census of closed hyperbolic 3-manifold
                 triangulations that are shipped with Regina.
                 This will only be initialised when lookup() is first called. */
        static NCensusDB* cuspedHypOr_;
            /**< The census of cusped hyperbolic orientable 3-manifold
                 triangulations that are shipped with Regina.
                 This will only be initialised when lookup() is first called. */
        static NCensusDB* cuspedHypNor_;
            /**< The census of cusped hyperbolic non-orientable 3-manifold
                 triangulations that are shipped with Regina.
                 This will only be initialised when lookup() is first called. */
        static NCensusDB* hypKnotLink_;
            /**< The census of cusped hyperbolic knot and link complements
                 that are shipped with Regina.
                 This will only be initialised when lookup() is first called. */
        static bool dbInit_;
            /**< Have the census databases been initialised yet? */

        NPacket* parent;
            /**< The argument passed to formCensus(). */
        NBoolSet finiteness;
            /**< The argument passed to formCensus(). */
        NBoolSet orientability;
            /**< The argument passed to formCensus(). */

        int whichPurge;
            /**< The argument passed to formCensus(). */

        bool (*sieve)(NTriangulation*, void*);
            /**< The arbitrary constraint function to run triangulations
                 through.  This is of type AcceptTriangulation, but because
                 that typedef is deprecated we do not use it here. */
        void* sieveArgs;
            /**< The second argument to pass to function \a sieve. */

        unsigned long whichSoln;
            /**< The number of the solution we are up to. */

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
         * The list of hits will be returned as an NCensusHits object,
         * which you can use to iterate through the individual matches.
         * Even if there are no matches at all, an NCensusHits object will
         * still be returned; you can call NCensusHits::empty() to test
         * whether any matches were found.
         *
         * The NCensusHits object that is returned will be newly
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
        static NCensusHits* lookup(const NTriangulation& tri);
        /**
         * Searches for the given triangulation through all of Regina's
         * in-built census databases.
         *
         * For this routine you specify the triangulation by giving its
         * isomorphism signature, as returned by NTriangulation::isoSig().
         * This is faster than the variant lookup(const NTriangulation&),
         * since Regina's census databases store isomorphism signatures
         * internally.  If you do not already know the isomorphism signature,
         * it is fine to just call lookup(const NTriangulation&) instead.
         *
         * Note that there may be many hits (possibly from multiple databases,
         * and in some cases possibly even within the same database).
         * The list of hits will be returned as an NCensusHits object,
         * which you can use to iterate through the individual matches.
         * Even if there are no matches at all, an NCensusHits object will
         * still be returned; you can call NCensusHits::empty() to test
         * whether any matches were found.
         *
         * The NCensusHits object that is returned will be newly
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
        static NCensusHits* lookup(const std::string& isoSig);

        /**
         * Deprecated routine that fills the given packet with all
         * triangulations in a census of 3-manifold triangulations satisfying
         * the given constraints.  Each triangulation in the census will
         * appear as a child of the given packet.
         *
         * This routine will conduct a census of all valid triangulations
         * containing a given number of tetrahedra.  All such triangulations
         * are included in the census up to combinatorial isomorphism; given
         * any isomorphism class, exactly one representative will appear in
         * the census.
         *
         * The census can be optionally restricted to only include
         * triangulations satisfying further constraints (such as
         * orientability and finiteness); see the individual parameter
         * descriptions for further details.  In particular, parameter
         * \a sieve can be used to impose arbitrary restrictions that are
         * not hard-coded into this class.
         *
         * Note that if constraints may be imposed using the hard-coded
         * parameters (such as orientability and finiteness), it is
         * generally better to do this than to use the arbitrary
         * constraint parameter \a sieve.  Hard-coded parameters will be
         * tested earlier, and some (such as orientability) can be
         * incorporated directly into the census algorithm to give a vast
         * performance increase.
         *
         * Parameter \a whichPurge may be used to further avoid constructing
         * triangulations satisfying particular constraints (such as
         * non-minimality).  This can significantly speed up the census.
         * In this case however not all such triangulations will be
         * avoided, but it is guaranteed that every triangulation that
         * does \e not satisfy the constraints defined by \a whichPurge
         * will be produced.
         *
         * Only valid triangulations will be produced; see
         * NTriangulation::isValid() for further details.
         *
         * Note that this routine should only be used if the census
         * contains a small enough total number of triangulations to
         * avoid any memory disasters.
         *
         * \deprecated The NCensus enumeration facilities are on their
         * way out of Regina, and in the future the NCensus class will be
         * used purely for census lookups.  To perform the kind of
         * enumeration that is described here, you should call
         * NFacePairing::findAllPairings() and
         * NGluingPermSearcher::findAllPerms() directly.
         *
         * \ifacespython Parameters \a sieve and \a sieveArgs
         * are not present (and will be treated as 0).
         *
         * @param parent the packet beneath which members of the census will
         * be placed.
         * @param nTetrahedra the number of tetrahedra in each triangulation
         * in the census.
         * @param finiteness determines whether to include finite and/or ideal
         * triangulations.  The set should contain \c true if finite (non-ideal)
         * triangulations are to be included, and should contain \c false if
         * ideal triangulations are to be included.
         * @param orientability determines whether to include orientable
         * and/or non-orientable triangulations.  The set should contain \c true
         * if orientable triangulations are to be included, and should contain
         * \c false if non-orientable triangulations are to be included.
         * @param boundary determines whether to include triangulations with
         * and/or without boundary triangles.  The set should contain \c true
         * if triangulations with boundary triangles are to be included, and
         * should contain \c false if triangulations with only internal
         * triangles are to be included.
         * @param nBdryTris specifies the precise number of boundary triangles
         * that should be present in the triangulations produced.  If this
         * parameter is negative, it is ignored and no additional restriction
         * is imposed.  See the documentation for routine
         * NFacePairing::findAllPairings() for details regarding this
         * parameter and how it interacts with parameter \a boundary.
         * @param whichPurge specifies which triangulations we may further 
         * avoid constructing (see the function notes above for details).
         * This should be a bitwise OR of purge constants defined in this
         * class, or 0 if no additional pruning should take place.
         * If a variety of purge constants are bitwise ORed together, a
         * triangulation satisfying \e any of these constraints may be
         * avoided.  Note that not all such triangulations will be
         * avoided, but enough are avoided that the performance increase
         * is noticeable.
         * @param sieve an additional constraint function that may be
         * used to exclude certain triangulations from the census.  If
         * this parameter is non-zero, each triangulation produced (after
         * passing all other criteria) will be passed through this
         * function.  If this function returns \c true then the triangulation
         * will be included in the census; otherwise it will not.
         * When this function is called, the first (triangulation)
         * argument will be a triangulation under consideration for
         * inclusion in the census.  The second argument will be
         * parameter \a sieveArgs as passed to formCensus().
         * Parameter \a sieve may be passed as \c null (in which case no
         * additional constraint function will be used).
         * @param sieveArgs the pointer to pass as the final parameter
         * for the function \a sieve which will be called upon each
         * triangulation found.  If \a sieve is \c null then \a sieveArgs
         * will be ignored.
         * @return the number of triangulations produced in the census.
         */
        REGINA_DEPRECATED static unsigned long formCensus(NPacket* parent,
            unsigned nTetrahedra, NBoolSet finiteness, NBoolSet orientability,
            NBoolSet boundary, int nBdryTris, int whichPurge,
            AcceptTriangulation sieve = 0, void* sieveArgs = 0);

        /**
         * Deprecated routine that fills the given packet with all
         * triangulations in a partial census
         * of 3-manifold triangulations satisfying the given constraints.
         * Each triangulation in the partial census will appear as a child of
         * the given packet.
         *
         * This routine will conduct a census of all valid triangulations
         * that are modelled by the given tetrahedron face pairing.
         * All such triangulations are included in the census up to
         * combinatorial isomorphism; given any isomorphism class, exactly
         * one representative will appear in the census.
         *
         * The census can be optionally restricted to only include
         * triangulations satisfying further constraints (such as
         * orientability and finiteness); see the individual parameter
         * descriptions for further details.  In particular, parameter
         * \a sieve can be used to impose arbitrary restrictions that are
         * not hard-coded into this class.
         *
         * Note that if constraints may be imposed using the hard-coded
         * parameters (such as orientability and finiteness), it is
         * generally better to do this than to use the arbitrary
         * constraint parameter \a sieve.  Hard-coded parameters will be
         * tested earlier, and some (such as orientability) can be
         * incorporated directly into the census algorithm to give a vast
         * performance increase.
         *
         * Parameter \a whichPurge may be used to further avoid constructing
         * triangulations satisfying particular constraints (such as
         * non-minimality).  The use of this parameter, combined with
         * parameters \a finiteness and \a orientability, can significantly
         * speed up the census.  For some combinations of these
         * parameters entirely different algorithms are used.
         *
         * Note however that not all triangulations described by
         * parameter \a whichPurge will be avoided.  It is guaranteed
         * however that every triangulation that does \e not satisfy the
         * constraints defined by \a whichPurge will be produced.
         *
         * Only valid triangulations will be produced; see
         * NTriangulation::isValid() for further details.
         *
         * Note that this routine should only be used if the partial census
         * contains a small enough total number of triangulations to
         * avoid any memory disasters.
         *
         * The partial census will run in the current thread.  This
         * routine will only return once the partial census is complete.
         *
         * \deprecated The NCensus enumeration facilities are on their
         * way out of Regina, and in the future the NCensus class will be
         * used purely for census lookups.  To perform the kind of
         * enumeration that is described here, you should call
         * NGluingPermSearcher::findAllPerms() directly.
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by NFacePairing::isCanonical().  Note that all face pairings
         * constructed by NFacePairing::findAllPairings() are of this form.
         *
         * \ifacespython Parameters \a sieve and \a sieveArgs
         * are not present (and will be treated as 0).
         *
         * @param pairing the tetrahedron face pairing that
         * triangulations in this partial census must be modelled by.
         * @param parent the packet beneath which members of the partial
         * census will be placed.
         * @param finiteness determines whether to include finite and/or ideal
         * triangulations.  The set should contain \c true if finite (non-ideal)
         * triangulations are to be included, and should contain \c false if
         * ideal triangulations are to be included.
         * @param orientability determines whether to include orientable
         * and/or non-orientable triangulations.  The set should contain \c true
         * if orientable triangulations are to be included, and should contain
         * \c false if non-orientable triangulations are to be included.
         * @param whichPurge specifies which triangulations we may further 
         * avoid constructing (see the function notes above for details).
         * This should be a bitwise OR of purge constants defined in this
         * class, or 0 if no additional pruning should take place.
         * If a variety of purge constants are bitwise ORed together, a
         * triangulation satisfying \e any of these constraints may be
         * avoided.  Note that not all such triangulations will be
         * avoided, but enough are avoided that the performance increase
         * is noticeable.
         * @param sieve an additional constraint function that may be
         * used to exclude certain triangulations from the census.  If
         * this parameter is non-zero, each triangulation produced (after
         * passing all other criteria) will be passed through this
         * function.  If this function returns \c true then the triangulation
         * will be included in the census; otherwise it will not.
         * When this function is called, the first (triangulation)
         * argument will be a triangulation under consideration for
         * inclusion in the census.  The second argument will be
         * parameter \a sieveArgs as passed to formPartialCensus().
         * Parameter \a sieve may be passed as \c null (in which case no
         * additional constraint function will be used).
         * @param sieveArgs the pointer to pass as the final parameter
         * for the function \a sieve which will be called upon each
         * triangulation found.  If \a sieve is \c null then \a sieveArgs
         * will be ignored.
         * @return the number of triangulations produced in the partial census.
         */
        REGINA_DEPRECATED static unsigned long formPartialCensus(
            const NFacePairing* pairing, NPacket* parent, NBoolSet finiteness,
            NBoolSet orientability, int whichPurge,
            AcceptTriangulation sieve = 0, void* sieveArgs = 0);

        /**
         * Deprecated routine to determine whether the given triangulation
         * even has a chance at being minimal.  This routine can be passed as
         * parameter \a sieve to routine NCensus::formCensus() to exclude
         * obviously non-minimal triangulations from a census.
         *
         * A variety of tests will be performed; these tests are subject
         * to change between Regina releases.  Currently this routine
         * counts vertices and also tries to simplify the triangulation using
         * NTriangulation::simplifyToLocalMinimum().
         *
         * Currently this routine is only useful for triangulations whose
         * triangles are all internal; if the given triangulation has
         * boundary triangles then this routine will simply return \c true.
         *
         * \deprecated The NCensus enumeration facilities are on their way
         * out of Regina, and so this routine is no longer necessary.
         * For a fast test as to whether a triangulation might be minimal,
         * you should simply call NTriangulation::simplifyToLocalMinimum(false).
         *
         * \ifacespython Parameter \a ignore is not present.
         *
         * @param tri the triangulation to examine.
         * @param ignore a parameter that is ignored.
         * @return \c false if the given triangulation is known to be
         * non-minimal, or \c true if minimality of the given
         * triangulation has not been determined.
         */
        REGINA_DEPRECATED static bool mightBeMinimal(NTriangulation* tri,
            void* ignore = 0);

    private:
        /**
         * Constructs a NCensusDB object for one of Regina's in-built
         * census databases, stored in the standard census database
         * location on the filesystem.
         *
         * @param filename the filename for the database, without directory
         * information.  This routine will build the full pathname by joining
         * the given filename with the standard census database directory.
         * @param desc a human-readable description for the database.
         * @return the new database specifier.
         */
        static NCensusDB* standardDB(const char* filename, const char* desc);

        /**
         * Creates a new structure to hold the given information.
         * All parameters not explained are taken directly from
         * formCensus().
         *
         * \deprecated The NCensus enumeration facilities are on their
         * way out of Regina, and in the future the NCensus class will be
         * used purely for census lookups.  If you wish to build a census
         * yourself, you should call NFacePairing::findAllPairings() and
         * NGluingPermSearcher::findAllPerms() directly.
         */
        REGINA_DEPRECATED NCensus(NPacket* newParent,
            const NBoolSet& newFiniteness, const NBoolSet& newOrientability,
            int newWhichPurge, AcceptTriangulation newSieve,
            void* newSieveArgs);

        /**
         * Called when a particular tetrahedron face pairing has been
         * found.  This routine hooks up the face pairing generation with
         * the gluing permutation generation.
         *
         * @param pairing the face pairing that has been found.
         * @param autos the set of all automorphisms of the given face
         * pairing.
         * @param census the census currently being generated;
         * this must really be of class NCensus.
         */
        static void foundFacePairing(const NFacePairing* pairing,
            const NFacePairing::IsoList* autos, void* census);

        /**
         * Called when a particular set of gluing permutations has been
         * found.  This routine generates the corresponding triangulation
         * and decides whether it really belongs in the census.
         *
         * \pre The given set of gluing permutations is complete, i.e.,
         * it is not just a partially-complete search state.  Partial
         * searches are formed by calling NGluingPermSearcher::runSearch()
         * with a non-negative depth argument.
         *
         * @param perms the gluing permutation set that has been found.
         * @param census the census currently being generated;
         * this must really be of class NCensus.
         */
        static void foundGluingPerms(const NGluingPermSearcher* perms,
            void* census);
};

/*@}*/

// Inline functions for NCensusDB:

inline NCensusDB::NCensusDB(const std::string& filename,
        const std::string& desc) : filename_(filename), desc_(desc) {
}

inline const std::string& NCensusDB::filename() const {
    return filename_;
}

inline const std::string& NCensusDB::desc() const {
    return desc_;
}

// Inline functions for NCensusHit:

inline NCensusHit::NCensusHit(const char* name, const NCensusDB* db) :
        name_(name), db_(db), next_(0) {
}

inline NCensusHit::NCensusHit(const std::string& name, const NCensusDB* db) :
        name_(name), db_(db), next_(0) {
}

inline const std::string& NCensusHit::name() const {
    return name_;
}

inline const NCensusDB* NCensusHit::db() const {
    return db_;
}

inline const NCensusHit* NCensusHit::next() const {
    return next_;
}

// Inline functions for NCensusHits:

inline NCensusHits::NCensusHits() : first_(0), last_(0), count_(0) {
}

inline NCensusHits::~NCensusHits() {
    NCensusHit* tmp;
    while (first_) {
        tmp = first_;
        first_ = first_->next_;
        delete tmp;
    }
}

inline NCensusHit* NCensusHits::first() const {
    return first_;
}

inline size_t NCensusHits::count() const {
    return count_;
}

inline bool NCensusHits::empty() const {
    return ! first_;
}

inline void NCensusHits::append(NCensusHit* hit) {
    if (! first_)
        last_ = first_ = hit;
    else
        last_ = last_->next_ = hit; // Assigns last_->next_, then last_.
    ++count_;
}

} // namespace regina

#endif

