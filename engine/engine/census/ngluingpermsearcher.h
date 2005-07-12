
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file ngluingpermsearcher.h
 *  \brief Supports searching through all possible gluing permutations sets
 *  for a given face pairing.
 */

#ifndef __NGLUINGPERMSEARCHER_H
#ifndef __DOXYGEN
#define __NGLUINGPERMSEARCHER_H
#endif

#include "census/ngluingperms.h"

namespace regina {

/**
 * \weakgroup census
 * @{
 */

class NGluingPermSearcher;

/**
 * A routine used to do arbitrary processing upon a particular set of
 * gluing permutations.  Such routines are used to process permutation
 * sets found when running NGluingPermSearcher::findAllPerms().
 *
 * The first parameter passed will be a set of gluing permutations
 * (in fact it will be of the subclass NGluingPermSearcher in order to
 * support partial searches as well as full searches).  This set of
 * gluing permutations must not be deallocated by this routine, since it
 * may be used again by the caller.  The second parameter may contain
 * arbitrary data as passed to NGluingPerms::findAllPerms().
 *
 * Note that the first parameter passed might be \c null to signal that
 * gluing permutation generation has finished.
 */
typedef void (*UseGluingPerms)(const NGluingPermSearcher*, void*);

/**
 * A utility class for searching through all possible gluing permutation
 * sets that correspond to a given face pairing.  Subclasses of
 * NGluingPermSearcher correspond to specialised (and heavily optimised)
 * search algorithms that may be used in sufficiently constrained scenarios.
 * The main class NGluingPermSearcher offers a default (but slower) search
 * algorithm that may be used in more general contexts.
 *
 * The simplest way of performing a search through all possible gluing
 * permutations is by calling the static method findAllPerms().  This will
 * examine the search parameters and ensure that the best possible algorithm
 * is used.  Alternatively, a specific algorithm can be forced by explicitly
 * constructing an object of the corresponding class and calling runSearch()
 * directly.
 *
 * Note that this class derives from NGluingPerms.  The search will
 * involve building and repeatedly modifying the inherited NGluingPerms
 * data in-place.
 *
 * \ifacespython Not present.
 */
class NGluingPermSearcher : public NGluingPerms {
    protected:
        const NFacePairingIsoList* autos_;
            /**< The set of isomorphisms that define equivalence of
                 gluing permutation sets.  Generally this is the set of all
                 automorphisms of the underlying face pairing. */
        bool autosNew;
            /**< Did we create the isomorphism list autos_ ourselves (in
                 which case we must destroy it also)? */
        bool orientableOnly_;
            /**< Are we only searching for gluing permutations that
                 correspond to orientable triangulations? */
        bool finiteOnly_;
            /**< Are we only searching for gluing permutations that
                 correspond to finite triangulations? */
        int whichPurge_;
            /**< Are there any types of triangulation that we may optionally
                 avoid constructing?  See the constructor documentation for
                 further details on this search parameter. */
        UseGluingPerms use_;
            /**< A routine to call each time a gluing permutation set is
                 found during the search. */
        void* useArgs_;
            /**< Additional user-supplied data to be passed as the second
                 argument to the \a use_ routine. */

        bool started;
            /**< Has the search started yet?  This helps distinguish
                 between a new search and the resumption of a partially
                 completed search. */
        int* orientation;
            /**< Keeps track of the orientation of each tetrahedron in the
                 underlying triangulation.  Orientation is positive/negative,
                 or 0 if unknown.
                 Note that in some algorithms the orientation is simply
                 +/-1, and in some algorithms the orientation counts
                 forwards or backwards from 0 according to how many
                 times the orientation has been set or verified. */

    private:
        NTetFace currFace;
            /**< The face that we are currently examining at this stage
                 of the search. */

    public:
        /**
         * Initialises a new search for gluing permutation sets.  The
         * search is started by calling runSearch().  Note that the
         * static method findAllPerms() handles both construction and
         * searching, and is the preferred entry point for end users.
         *
         * The arguments to this constructor describe the search
         * parameters in detail, as well as what should be done with
         * each gluing permutation set that is found.
         *
         * Parameter \a whichPurge may be used to avoid constructing
         * permutation sets that correspond to triangulations satisfying
         * certain constraints (such as non-minimality).  The use of
         * this parameter, combined with parameters \a orientableOnly
         * and \a finiteOnly, can significantly speed up the permutation
         * set generation.  For some combinations of these parameters
         * entirely different algorithms are used.
         *
         * Note that not all permutation sets described by parameter
         * \a whichPurge will be avoided (i.e., you may get gluing
         * permutation sets that you did not want).  It is guaranteed however
         * that every permutation set whose corresonding triangulation does
         * \e not satisfy the \a whichPurge constraints will be generated.
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by NFacePairing::isCanonical().  Note that all face pairings
         * constructed by NFacePairing::findAllPairings() are of this form.
         *
         * @param pairing the specific pairing of tetrahedron faces
         * that the generated permutation sets will complement.
         * @param autos the collection of isomorphisms that define equivalence
         * of permutation sets.  These are used by runSearch(), which produces
         * each permutation set precisely once up to equivalence.  These
         * isomorphisms must all be automorphisms of the given face pairing,
         * and will generally be the set of all such automorphisms.  This
         * parameter may be 0, in which case the set of all automorphisms
         * of the given face pairing will be generated and used.
         * @param orientableOnly \c true if only gluing permutations
         * corresponding to orientable triangulations should be
         * generated, or \c false if no such restriction should be imposed.
         * @param finiteOnly \c true if only gluing permutations
         * corresponding to finite triangulations should be
         * generated, or \c false if no such restriction should be imposed.
         * @param whichPurge specifies which permutation sets we may
         * avoid constructing (see the function notes above for details).
         * This should be a bitwise OR of purge constants from class NCensus,
         * or 0 if we should simply generate every possible permutation set.
         * If a variety of purge constants are bitwise ORed together, a
         * permutation set whose triangulation satisfies \e any of these
         * constraints may be avoided.  Note that not all such
         * permutation sets will be avoided, but enough are avoided that
         * the performance increase is noticeable.
         * @param use the function to call upon each permutation set that
         * is found.  The first parameter passed to this function will be
         * a gluing permutation set.  The second parameter will be
         * parameter \a useArgs as was passed to this routine.
         * @param useArgs the pointer to pass as the final parameter for
         * the function \a use which will be called upon each permutation
         * set found.
         */
        NGluingPermSearcher(const NFacePairing* pairing,
                const NFacePairingIsoList* autos,
                bool orientableOnly, bool finiteOnly, int whichPurge,
                UseGluingPerms use, void* useArgs = 0);

        /**
         * Initialises a new search manager based on data read from the
         * given input stream.  This may be a new search or a partially
         * completed search.  This routine reads data in the format written
         * by dumpData().
         *
         * If the data found in the input stream is invalid or incorrectly
         * formatted, the routine inputError() will return \c true but
         * the contents of this object will be otherwise undefined.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * @param in the input stream from which to read.
         * @param use as for the main NGluingPermSearcher constructor.
         * @param useArgs as for the main NGluingPermSearcher constructor.
         */
        NGluingPermSearcher(std::istream& in,
            UseGluingPerms use, void* useArgs = 0);

        /**
         * Destroys this search manager and all supporting data
         * structures.
         */
        virtual ~NGluingPermSearcher();

        /**
         * Generates all possible gluing permutation sets that satisfy
         * the current search criteria.  The search criteria are
         * specified in the class constructor, or through the static
         * method findAllPerms().
         *
         * Each set of gluing permutations will be produced precisely
         * once up to equivalence, where equivalence is defined by the
         * given set of automorphisms of the given face pairing.
         *
         * For each permutation set that is generated, routine \a use_ (as
         * passed to the class constructor) will be called with that
         * permutation set as an argument.
         *
         * Once the generation of permutation sets has finished, routine
         * \a use_ will be called once more, this time with \c null as its
         * first (permutation set) argument.
         *
         * Subclasses corresponding to more specialised search criteria
         * should override this routine to use a better optimised algorithm
         * where possible.
         *
         * It is possible to run only a partial search, branching to a
         * given depth but no further.  In this case, rather than
         * producing complete gluing permutation sets, the search will
         * produce a series of partially-complete NGluingPermSearcher
         * objects.  These partial searches may then be restarted by
         * calling runSearch() once more (usually after being frozen or
         * passed on to a different processor).  If necessary, the \a use_
         * routine may call completePermSet() to distinguish between
         * a complete set of gluing permutations and a partial search state.
         *
         * Note that a restarted search will never drop below its
         * initial depth.  That is, calling runSearch() with a fixed
         * depth can be used to subdivide the overall search space into
         * many branches, and then calling runSearch() on each resulting
         * partial search will complete each of these branches without overlap.
         *
         * \todo \feature Allow cancellation of permutation set generation.
         *
         * @param maxDepth the depth of the partial search to run, or a
         * negative number if a full search should be run (the default).
         */
        virtual void runSearch(long maxDepth = -1);

        /**
         * Determines whether this search manager holds a complete
         * gluing permutation set or just a partially completed search
         * state.
         *
         * This may assist the \a use_ routine when running partial
         * depth-based searches.  See runSearch() for further details.
         *
         * @return \c true if a complete gluing permutation set is held,
         * or \c false otherwise.
         */
        virtual bool completePermSet() const;

        /**
         * The main entry routine for running a search for all gluing
         * permutation sets that complement a given face pairing.
         *
         * This routine examines the search parameters, chooses the best
         * possible search algorithm, constructs an object of the
         * corresponding subclass of NGluingPermSearcher and then calls
         * runSearch().
         *
         * See the NGluingPermSearcher constructor for documentation on
         * the arguments to this routine.  See the runSearch() method
         * for documentation on how the search runs and returns its
         * results.
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by NFacePairing::isCanonical().  Note that all face pairings
         * constructed by NFacePairing::findAllPairings() are of this form.
         */
        static void findAllPerms(const NFacePairing* pairing,
                const NFacePairingIsoList* autos,
                bool orientableOnly, bool finiteOnly, int whichPurge,
                UseGluingPerms use, void* useArgs = 0);

        // Overridden methods:
        virtual void dumpData(std::ostream& out) const;

    protected:
        /**
         * Compares the current set of gluing permutations with its
         * preimage under each automorphism of the underlying face pairing,
         * in order to see whether the current set is in canonical form
         * (i.e., is lexicographically smallest).
         *
         * @return \c true if the current set is in canonical form,
         * or \c false otherwise.
         */
        bool isCanonical() const;

        /**
         * Determines whether the permutations already constructed model
         * a triangulation with an edge identified with itself in reverse.
         * Note that such edges can only occur in non-orientable
         * triangulations.
         *
         * Tests that do not refer to the gluing permutation for the
         * given face will not be run.
         *
         * This routine is not fussy about the order in which gluing
         * permutations are selected, as long as permutations not yet
         * selected have the corresponding element of permIndices[] set
         * to -1.
         *
         * If \a finiteOnly_ is \c true in the search criteria, additional
         * tests will be run that can eliminate triangulations with
         * non-orientable vertex links.  Although these tests are not
         * searching for bad edge links per se, they can be performed within
         * this routine with very little additional work needing to be done.
         *
         * @param face the specific tetrahedron face upon which tests
         * will be based.
         * @return \c true if the permutations under construction will
         * lead to an edge identified with itself in reverse, or \c false
         * if no such edge is identified.
         */
        bool badEdgeLink(const NTetFace& face) const;

        /**
         * Determines whether the permutations already constructed model
         * a triangulation with a low degree edge.  Precisely which
         * types of low degree edges are identified must be specified
         * through parameters \a testDegree12 and \a testDegree3.
         *
         * Tests that do not refer to the gluing permutation for the
         * given face will not be run.
         *
         * This routine is not fussy about the order in which gluing
         * permutations are selected, as long as permutations not yet
         * selected have the corresponding element of permIndices[] set
         * to -1.
         *
         * @param face the specific tetrahedron face upon which tests
         * will be based.
         * @param testDegree12 \c true if we should test for
         * non-boundary edges of degree 1 or 2.
         * @param testDegree3 \c true if we should test for non-boundary
         * edges of degree 3 involving three distinct tetrahedra.
         * @return \c true if the permutations under construction will
         * lead to a low-degree edge as specified by parameters
         * \a testDegree12 and \a testDegree3, or \c false if no such
         * edge is identified.
         */
        bool lowDegreeEdge(const NTetFace& face, bool testDegree12,
            bool testDegree3) const;

        /**
         * Determines whether the permutations under construction are
         * doomed to model a triangulation that can be purged from the
         * census.  The conditions under which a triangulation may be
         * purged are specified by the search parameter \a whichPurge_.
         *
         * Note that this routine will not identify all triangulations
         * that satisfy the given conditions; however, whenever this
         * routine \e does return \c true it is guaranteed that the
         * permutations under construction will only lead to
         * triangulations that do meet the given conditions.
         *
         * Tests that do not refer to the gluing permutation for the
         * given face will not be run.
         *
         * This routine is not fussy about the order in which gluing
         * permutations are selected, as long as permutations not yet
         * selected have the corresponding element of permIndices[] set
         * to -1.
         *
         * @param face the specific tetrahedron face upon which tests
         * will be based.
         * @return \c true if the permutations under construction will only
         * lead to triangulations that may be purged, or \c false if the
         * results are inconclusive.
         */
        bool mayPurge(const NTetFace& face) const;
};

/**
 * A gluing permutation search class that offers a specialised search
 * algorithm for when (i) only closed prime minimal P2-irreducible
 * triangulations are required, and (ii) the given face pairing has
 * order at least three.
 *
 * The search algorithm is significantly different from the default
 * algorithm provided by NGluingPermSearcher.  It is heavily optimised
 * and takes advantage of a number of results regarding the underlying
 * face pairing graph.
 *
 * Note that additional unwanted triangulations (e.g., non-prime or
 * non-minimal triangulations) may still be produced by this search.
 * However, significantly fewer unwanted triangulations will be produced
 * when using this class instead of NGluingPermSearcher.
 *
 * \ifacespython Not present.
 */
class NClosedPrimeMinSearcher : public NGluingPermSearcher {
    private:
        static const unsigned EDGE_CHAIN_END;
            /**< Represents the end of a one-ended chain in a face
                 pairing graph. */
        static const unsigned EDGE_CHAIN_INTERNAL_FIRST;
            /**< Represents the first edge of a double edge within a
                 one-ended chain in a face pairing graph.  The corresponding
                 element of order[] stores the face closest to the loop at
                 the end of this chain. */
        static const unsigned EDGE_CHAIN_INTERNAL_SECOND;
            /**< Represents the second edge of a double edge within a
                 one-ended chain in a face pairing graph.  The corresponding
                 element of order[] stores the face closest to the loop at
                 the end of this chain. */
        static const unsigned EDGE_DOUBLE_FIRST;
            /**< Represents the first edge of a miscellaneous double
                 edge in a face pairing graph.  The corresponding element of
                 order[] stores the face belonging to the lower numbered
                 tetrahedron. */
        static const unsigned EDGE_DOUBLE_SECOND;
            /**< Represents the second edge of a miscellaneous double
                 edge in a face pairing graph.  The corresponding element of
                 order[] stores the face belonging to the lower numbered
                 tetrahedron. */
        static const unsigned EDGE_MISC;
            /**< Represents a miscellaneous edge in a face pairing graph. */

    private:
        NTetFace* order;
            /**< Describes the order in which gluing permutations are
                 assigned to faces.  Specifically, this order is
                 order[0], order[1], ..., order[2n-1].

                 Note that each element of this array corresponds to a
                 single edge of the underlying face pairing graph, which in
                 turn represents a tetrahedron face and its image under
                 the given face pairing.

                 The specific tetrahedron face stored in this array for each
                 edge of the underlying face pairing grpah will be the smaller
                 of the two identified tetrahedron faces (unless otherwise
                 specified for a particular edge type). */
        unsigned* orderType;
            /**< For each edge in the face pairing graph stored in the
                 order[] array, a corresponding category for this edge is
                 stored in the orderType[] array.  Categories are described
                 by the EDGE_... constants defined in this class. */

        unsigned nChainEdges;
            /**< The number of edges in the face pairing graph belonging
                 to one-ended chains. */
        int* chainPermIndices;
            /**< Stores the two possible gluing permutations that must be
                 tried for each face in the order[] array of type
                 EDGE_CHAIN_END or EDGE_CHAIN_INTERNAL_FIRST.  These two
                 possible permutations can be derived using theoretical
                 results regarding the underlying face pairing graph.

                 Note that for each face of type EDGE_CHAIN_INTERNAL_SECOND,
                 the gluing permutation can be derived from the permutation
                 chosen for the previous face (of type
                 EDGE_CHAIN_INTERNAL_FIRST).  In this case we store the two
                 permutations for this face that correspond to the two
                 possible permutations for the previous face.  */

        int orderElt;
            /**< Marks which element of order[] we are currently examining
                 at this stage of the search. */

    public:
        /**
         * Creates a new search manager for use when (i) only closed prime
         * minimal P2-irreducible triangulations are required, and (ii) the
         * given face pairing has order at least three.  Note that other
         * unwanted triangulations may still be produced (e.g.,
         * non-prime or non-minimal triangulations), but there will be
         * far fewer of these than when using the NGluingPermSearcher
         * class directly.
         *
         * For details on how a search manager is used, see the
         * NGluingPermSearcher documentation.  Note in particular that
         * this class will be automatically used by
         * NGluingPermSearcher::findAllPerms() if possible, so there is
         * often no need for an end user to instantiate this class
         * directly.
         *
         * All constructor arguments are the same as for the
         * NGluingPermSearcher constructor, though some arguments (such as
         * \a finiteOnly and \a whichPurge) are not needed here since they
         * are already implied by the specialised search context.
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by NFacePairing::isCanonical().  Note that all face pairings
         * constructed by NFacePairing::findAllPairings() are of this form.
         * \pre The given face pairing has no boundary faces and has at
         * least three tetrahedra.
         */
        NClosedPrimeMinSearcher(const NFacePairing* pairing,
                const NFacePairingIsoList* autos,
                bool orientableOnly, UseGluingPerms use, void* useArgs = 0);

        /**
         * Initialises a new search manager based on data read from the
         * given input stream.  This may be a new search or a partially
         * completed search.  This routine reads data in the format written
         * by dumpData().
         *
         * If the data found in the input stream is invalid or incorrectly
         * formatted, the routine inputError() will return \c true but
         * the contents of this object will be otherwise undefined.
         *
         * The arguments \a use and \a useArgs are the same as for the
         * NGluingPermSearcher constructor.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * @param in the input stream from which to read.
         */
        NClosedPrimeMinSearcher(std::istream& in,
            UseGluingPerms use, void* useArgs = 0);

        /**
         * Destroys this search manager and all supporting data
         * structures.
         */
        virtual ~NClosedPrimeMinSearcher();

        // Overridden methods:
        virtual void dumpData(std::ostream& out) const;
        virtual void runSearch(long maxDepth = -1);
        virtual bool completePermSet() const;

    private:
        /**
         * Initialises the internal arrays (specifically those relating
         * to face orderings and properties of chains) to accurately
         * reflect the underlying face pairing.
         */
        void initOrder();
};

/*@}*/

// Inline functions for NGluingPermSearcher

inline NGluingPermSearcher::~NGluingPermSearcher() {
    delete[] orientation;
    if (autosNew)
        delete const_cast<NFacePairingIsoList*>(autos_);
}

inline bool NGluingPermSearcher::completePermSet() const {
    return (currFace.tet == static_cast<int>(pairing->getNumberOfTetrahedra()));
}

// Inline functions for NClosedPrimeMinSearcher

inline NClosedPrimeMinSearcher::~NClosedPrimeMinSearcher() {
    delete[] order;
    delete[] orderType;
    if (chainPermIndices)
        delete[] chainPermIndices;
}

inline bool NClosedPrimeMinSearcher::completePermSet() const {
    return (orderElt == static_cast<int>(pairing->getNumberOfTetrahedra()) * 2);
}

} // namespace regina

#endif

