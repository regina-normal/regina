
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file census/gluingpermsearcher3.h
 *  \brief Supports searching through all possible sets of tetrahedron
 *  gluing permutations for a given tetrahedron face pairing.
 */

#ifndef __REGINA_GLUINGPERMSEARCHER3_H
#ifndef __DOXYGEN
#define __REGINA_GLUINGPERMSEARCHER3_H
#endif

#include <functional>
#include <memory>
#include "core/output.h"
#include "census/gluingperms.h"
#include "census/gluingpermsearcher.h"
#include "census/purgeflags.h"
#include "triangulation/facetpairing3.h"
#include "utilities/exception.h"
#include "utilities/qitmask.h"

/**
 * Specifies whether the ClosedPrimeMinSearcher census generation code
 * should prune on high-degree edges.
 *
 * It is well known that a closed prime minimal P^2-irreducible triangulation
 * formed from at least three tetrahedra can never have an edge of degree
 * one or two.  Combining this with the fact that such a triangulation
 * must always have one vertex, a simple Euler characteristic
 * calculation shows that there must be precisely n+1 edges, where \a n
 * is the number of tetrahedra.
 *
 * A little arithmetic then shows that, for any \a k edges, the sum of
 * their edge degrees can be no more than 3(n+k-1); otherwise one of the
 * remaining edges will be forced to have degree one or two.  This
 * observation is the basis behind the high-degree edge pruning that
 * this option controls.
 *
 * To enable pruning on high-degree edges, set this macro to 1 (the default
 * for Regina's main source distribution); to disable it, set it to 0.
 *
 * \ingroup census
 */
#define PRUNE_HIGH_DEG_EDGE_SET 1

namespace regina {

/**
 * A utility class for searching through all possible gluing permutation
 * sets that correspond to a given tetrahedron face pairing.  Subclasses of
 * GluingPermSearcher<3> correspond to specialised (and heavily optimised)
 * search algorithms that may be used in sufficiently constrained scenarios.
 * The main class GluingPermSearcher<3> offers a default (but slower) search
 * algorithm that may be used in more general contexts.
 *
 * The simplest way of performing a search through all possible gluing
 * permutations is by calling the static method findAllPerms().  This will
 * examine the search parameters and ensure that the best possible algorithm
 * is used.  For finer control over the program flow, the static method
 * bestSearcher() can be used to create a search manager of the most
 * suitable class and then runSearch() can be called on this object directly.
 * For absolute control, a specific algorithm can be forced by explicitly
 * constructing an object of the corresponding class (and again
 * calling runSearch() on that object directly).
 *
 * This class is designed to manage the construction of a large census of
 * triangulations, and so it does not support copying, moving or swapping.
 *
 * \ingroup census
 */
template <>
class GluingPermSearcher<3> : public ShortOutput<GluingPermSearcher<3>> {
    public:
        static constexpr char dataTag = 'g';
            /**< A character used to identify this class when reading
                 and writing tagged data in text format. */

    protected:
        using ActionWrapper = std::function<void(const GluingPerms<3>&)>;
            /**< The type used to hold the user's action function and
                 arguments when enumerating gluing permutations. */

        GluingPerms<3> perms_;
            /**< The set of gluing permutations under construction. */
        const FacetPairing<3>::IsoList autos_;
            /**< The set of isomorphisms that define equivalence of
                 gluing permutation sets.  Generally this is the set of all
                 automorphisms of the underlying face pairing. */
        bool orientableOnly_;
            /**< Are we only searching for gluing permutations that
                 correspond to orientable triangulations? */
        bool finiteOnly_;
            /**< Are we only searching for gluing permutations that
                 correspond to finite triangulations? */
        CensusPurge whichPurge_;
            /**< Are there any types of triangulation that we may optionally
                 avoid constructing?  This should be a bitwise OR of constants
                 from the CensusPurgeFlags enumeration.  See the constructor
                 documentation for further details on this search parameter. */

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

    protected:
        FacetSpec<3>* order;
            /**< Describes the order in which gluing permutations are
                 assigned to faces.  Specifically, this order is
                 order[0], order[1], ..., order[orderSize-1].

                 Note that each element of this array corresponds to a
                 single edge of the underlying face pairing graph, which in
                 turn represents a tetrahedron face and its image under
                 the given face pairing.

                 The specific tetrahedron face stored in this array for each
                 edge of the underlying face pairing graph will be the smaller
                 of the two identified tetrahedron faces (unless otherwise
                 specified for a particular edge type; see
                 ClosedPrimeMinSearcher for examples). */
        size_t orderSize;
            /**< The total number of edges in the face pairing graph, i.e.,
                 the number of elements of interest in the order[] array. */
        ssize_t orderElt;
            /**< Marks which element of order[] we are currently examining
                 at this stage of the search. */

    public:
        /**
         * Initialises a new search for gluing permutation sets.  The
         * search is started by calling runSearch().  Note that the
         * static method findAllPerms() handles both construction and
         * searching, and is the preferred entry point for end users.
         *
         * The arguments to this constructor describe the search
         * parameters in detail.
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
         * Similarly, even if \a finiteOnly is set to \c true, some
         * non-finite triangulations might still slip through the net
         * (since the full vertex links are not always constructed).
         * However, like \a whichPurge, setting \a finiteOnly to \c true
         * allow the census algorithm to take shortcuts and therefore
         * run faster.  The resulting triangulations may be tested for
         * finiteness (and other properties) by calling triangulate().
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by FacetPairing<3>::isCanonical().  Note that all face pairings
         * constructed by FacetPairing<3>::findAllPairings() are of this form.
         *
         * @param pairing the specific pairing of tetrahedron faces
         * that the generated permutation sets will complement.
         * @param autos the collection of isomorphisms that define equivalence
         * of permutation sets.  These are used by runSearch(), which produces
         * each permutation set precisely once up to equivalence.  These
         * isomorphisms must all be automorphisms of the given face pairing,
         * and will generally be the set of all such automorphisms (which
         * you can generate via <tt>pairing.findAutomorphisms()</tt>).
         * @param orientableOnly \c true if only gluing permutations
         * corresponding to orientable triangulations should be
         * generated, or \c false if no such restriction should be imposed.
         * @param finiteOnly \c true if only gluing permutations
         * corresponding to finite triangulations are required, or
         * \c false if there is no such requirement.  Note that
         * regardless of this value, some non-finite triangulations
         * might still be produced; see the notes above for details.
         * @param whichPurge specifies which permutation sets we may avoid
         * constructing (see the function notes above for details).  This
         * should be a bitwise OR of constants from the CensusPurgeFlags
         * enumeration, or PURGE_NONE if we should simply generate every
         * possible permutation set.
         * If a variety of purge constants are bitwise ORed together, a
         * permutation set whose triangulation satisfies \e any of these
         * constraints may be avoided.  Note that not all such
         * permutation sets will be avoided, but enough are avoided that
         * the performance increase is noticeable.
         */
        GluingPermSearcher(FacetPairing<3> pairing,
                FacetPairing<3>::IsoList autos, bool orientableOnly,
                bool finiteOnly, CensusPurge whichPurge);

        /**
         * Initialises a new search manager based on data read from the
         * given input stream.  This may be a new search or a partially
         * completed search.
         *
         * This routine reads data in the format written by dumpData().
         * If you wish to read data whose precise class is unknown,
         * consider using dumpTaggedData() and fromTaggedData() instead.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * \exception InvalidInput The data found in the input stream is
         * invalid, incomplete, or incorrectly formatted.
         *
         * \nopython This constructor is fundamentally designed around working
         * through a single input stream as we make our way from base class
         * constructors down to subclass constructors.  Python users should
         * use taggedData() and fromTaggedData() instead, which incorporate
         * this same text data as part of their richer text format.
         *
         * @param in the input stream from which to read.
         */
        GluingPermSearcher(std::istream& in);

        /**
         * Destroys this search manager and all supporting data structures.
         */
        virtual ~GluingPermSearcher();

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
         * For each permutation set that is generated, this routine will call
         * \a action (which must be a function or some other callable object).
         *
         * - The first argument to \a action must be a const reference to a
         *   GluingPerms<3>.  This will be the permutation set that was found.
         *   If \a action wishes to keep the permutation set, it should take a
         *   deep copy (not a reference), since the permutation set may be
         *   changed and reused after \a action returns.
         *
         * - If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - \a action must return \c void.
         *
         * It is possible to run only a partial search, branching to a
         * given depth but no further; for this you should use the
         * separate routine partialSearch(), not runSearch().
         *
         * \todo \feature Allow cancellation of permutation set generation.
         *
         * \ifacespython This function is available, and \a action may be
         * a pure Python function.  However, \a action cannot take any
         * additional arguments beyond the initial gluing permutation set
         * (and therefore the additional \a args list is omitted here).
         *
         * @param action a function (or other callable object) to call
         * for each permutation set that is found.
         * @param args any additional arguments that should be passed to
         * \a action, following the initial permutation set argument.
         */
        template <typename Action, typename... Args>
        void runSearch(Action&& action, Args&&... args);

        /**
         * Runs a partial search for all possible gluing permutations
         * that satisfy the search criteria, branching only to the
         * given depth and no further.
         *
         * This routine essentially does some but not all of the work of
         * runSearch().  See the runSearch() documentation for a detailed
         * overview of what the full search aims to achieve.
         *
         * If runSearch() enumerates an entire search tree, then you can
         * think of partialSearch() as only enumerating the first
         * \a maxDepth levels of this search tree.  Rather than
         * producing complete gluing permutation sets, this search will
         * produce a series of partially-constructed permutation sets.
         * A partial search can be continued by calling runSearch()
         * again on the underlying GluingPermSearcher (perhaps after being
         * frozen, or passed on to a different processor via taggedData() and
         * fromTaggedData()).  If necessary, the \a action routine may call
         * isComplete() to distinguish between a complete set of
         * gluing permutations and a partial search state.
         *
         * Note that a restarted search will never drop below its
         * initial depth.  That is, calling runSearch() with a fixed
         * depth can be used to subdivide the overall search space into
         * many branches, and then calling runSearch() on each resulting
         * partial search will complete each of these branches without overlap.
         *
         * If the search tree is shallow enough (or if \a maxDepth is
         * large enough), it is possible that this routine will produce
         * complete gluing permutation sets.
         *
         * @param maxDepth the depth of the partial search to run.
         * A negative number indicates that a full search should be run.
         * @param action a function (or other callable object) to call
         * for each permutation set (partial or complete) that is found.
         * @param args any additional arguments that should be passed to
         * \a action, following the initial permutation set argument.
         */
        template <typename Action, typename... Args>
        void partialSearch(long maxDepth, Action&& action, Args&&... args);

        /**
         * Determines whether this search manager holds a complete
         * gluing permutation set or just a partially completed search
         * state.
         *
         * This may assist the \a action routine when running partial
         * depth-based searches.  See partialSearch() for further details.
         *
         * @return \c true if a complete gluing permutation set is held,
         * or \c false otherwise.
         */
        bool isComplete() const;

        /**
         * Dumps all internal data in a plain text format, along with a
         * marker to signify which precise class the data belongs to.
         * This routine can be used with fromTaggedData() to transport
         * objects from place to place whose precise class is unknown.
         *
         * This routine outputs the same information that taggedData() returns.
         *
         * The key difference between dumpData() and dumpTaggedData() is that
         * dumpTaggedData() preserves all internal information even if this
         * object belongs to a subclass of GluingPermSearcher, whereas
         * dumpData() only writes information pertaining to this base class.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * \nopython Instead use taggedData(), which returns this same
         * information as a string.
         *
         * @param out the output stream to which the data should be
         * written.
         */
        void dumpTaggedData(std::ostream& out) const;

        /**
         * Returns all internal data in a plain text format, along with a
         * marker to signify which precise class the data belongs to.
         * This routine can be used with fromTaggedData() to transport
         * objects from place to place whose precise class is unknown.
         *
         * This routine returns the same information that dumpTaggedData()
         * writes.
         *
         * The key difference between data() and taggedData() is that
         * taggedData() preserves all internal information even if this
         * object belongs to a subclass of GluingPermSearcher, whereas
         * data() only writes information pertaining to this base class.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * @return all of this object's internal data in plain text format.
         */
        std::string taggedData() const;

        /**
         * Dumps all internal data in a plain text format to the given
         * output stream.  This object can be recreated from this text data
         * by calling the input stream constructor for the appropriate class.
         *
         * This routine may be useful for transferring objects from
         * one processor to another.
         *
         * If subclasses override this function, they should write subclass
         * data after superclass data.  This means it is safe to dump data
         * from a subclass and then recreate a new superclass object from
         * that data (though subclass-specific information will be lost).
         *
         * This routine outputs the same information that data() returns.
         *
         * The key difference between dumpData() and dumpTaggedData() is that
         * dumpTaggedData() preserves all internal information even if this
         * object belongs to a subclass of GluingPermSearcher, whereas
         * dumpData() only writes information pertaining to this base class.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * \nopython You can instead use data(), which returns this same
         * information as a string.  However, the matching input stream
         * constructor is not available in Python either, so it is recommended
         * that Python users use taggedData() and fromTaggedData() instead.
         *
         * @param out the output stream to which the data should be written.
         */
        virtual void dumpData(std::ostream& out) const;

        /**
         * Returns all internal data in a plain text format.
         * This object can be recreated from this text data by calling the
         * input stream constructor for the appropriate class.
         *
         * This routine may be useful for transferring objects from
         * one processor to another.
         *
         * If subclasses override this function, they should write subclass
         * data after superclass data.  This means it is safe to dump data
         * from a subclass and then recreate a new superclass object from
         * that data (though subclass-specific information will be lost).
         *
         * This routine returns the same information that dumpData() writes.
         *
         * The key difference between data() and taggedData() is that
         * taggedData() preserves all internal information even if this
         * object belongs to a subclass of GluingPermSearcher, whereas
         * data() only writes information pertaining to this base class.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * \ifacespython This routine is available, but the matching
         * input stream constructor is not.  Python users should use
         * taggedData() and fromTaggedData() instead.
         *
         * @return all of this object's internal data in plain text format.
         */
        std::string data() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * The main entry routine for running a search for all gluing
         * permutation sets that complement a given face pairing.
         *
         * This routine examines the search parameters, chooses the best
         * possible search algorithm, constructs an object of the
         * corresponding subclass of GluingPermSearcher<3> and then calls
         * runSearch().
         *
         * See the GluingPermSearcher<3> constructor for documentation on
         * the arguments to this routine.  See the runSearch() method
         * for documentation on how the search runs and returns its
         * results via \a action and \a args..
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by FacetPairing<3>::isCanonical().  Note that all face pairings
         * constructed by FacetPairing<3>::findAllPairings() are of this form.
         *
         * \ifacespython This function is available, and \a action may be
         * a pure Python function.  However, \a action cannot take any
         * additional arguments beyond the initial gluing permutation set
         * (and therefore the additional \a args list is omitted here).
         */
        template <typename Action, typename... Args>
        static void findAllPerms(FacetPairing<3> pairing,
                FacetPairing<3>::IsoList autos, bool orientableOnly,
                bool finiteOnly, CensusPurge whichPurge,
                Action&& action, Args&&... args);

        /**
         * Constructs a search manager of the best possible class for the
         * given search parameters.  Different subclasses of
         * GluingPermSearcher<3> provide optimised search algorithms for
         * different types of search.
         *
         * Calling this routine and then calling runSearch() on the
         * result has the same effect as the all-in-one routine
         * findAllPerms().  Unless you have specialised requirements
         * (such as partial searching), you are probably better calling
         * findAllPerms() instead.
         *
         * See the GluingPermSearcher<3> constructor for documentation on
         * the arguments to this routine.
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by FacetPairing<3>::isCanonical().  Note that all face pairings
         * constructed by FacetPairing<3>::findAllPairings() are of this form.
         *
         * @return the new search manager.
         */
        static std::unique_ptr<GluingPermSearcher<3>> bestSearcher(
                FacetPairing<3> pairing, FacetPairing<3>::IsoList autos,
                bool orientableOnly, bool finiteOnly, CensusPurge whichPurge);

        /**
         * Creates a new search manager based on tagged data read from
         * the given input stream.  This may be a new search or a
         * partially completed search.
         *
         * The tagged data should be in the format written by
         * dumpTaggedData().  The precise class of the search manager
         * will be determined from the tagged data, and does not need to
         * be known in advance.  This is in contrast to dumpData() and
         * the input stream constructors, where the class of the data being
         * read must be known at compile time.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * \exception InvalidInput The data found in the given input stream
         * is invalid, incomplete, or incorrectly formatted.
         *
         * \nopython Instead use the variant of fromTaggedData() that takes
         * its input as a string.
         *
         * @param in the input stream from which to read.
         * @return the new search manager, or \c null if the data in the
         * input stream was invalid or incorrectly formatted.
         */
        static std::unique_ptr<GluingPermSearcher<3>> fromTaggedData(
                std::istream& in);

        /**
         * Creates a new search manager based on tagged data stored in
         * the given string.  This may be a new search or a
         * partially completed search.
         *
         * The tagged data should be in the format returned by taggedData().
         * The precise class of the search manager
         * will be determined from the tagged data, and does not need to
         * be known in advance.  This is in contrast to dumpData() and
         * the input stream constructors, where the class of the data being
         * read must be known at compile time.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * \exception InvalidArgument The data found in the given string
         * is invalid, incomplete, or incorrectly formatted.
         *
         * @param data the tagged data from which to reconstruct a
         * search manager.
         * @return the new search manager, or \c null if the data in the
         * given string was invalid or incorrectly formatted.
         */
        static std::unique_ptr<GluingPermSearcher<3>> fromTaggedData(
                const std::string& data);

        // Make this class non-copyable.
        GluingPermSearcher(const GluingPermSearcher&) = delete;
        GluingPermSearcher& operator = (const GluingPermSearcher&) = delete;

    protected:
        /**
         * A de-templatised implementation of runSearch() and partialSearch().
         *
         * Here the templated action plus arguments are bundled together
         * in a wrapper whose full type is known in advance.
         *
         * Subclasses corresponding to more specialised search criteria
         * should override this routine to use a better optimised algorithm
         * where possible.
         *
         * See runSearch() and partialSearch() for further details.
         */
        virtual void searchImpl(long maxDepth, ActionWrapper&& action);

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
         * if no such edge is found.
         */
        bool badEdgeLink(const FacetSpec<3>& face) const;

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
         * edge is found.
         */
        bool lowDegreeEdge(const FacetSpec<3>& face, bool testDegree12,
            bool testDegree3) const;

        /**
         * Returns the character used to identify this class when
         * storing tagged data in text format.
         *
         * @return the class tag.
         */
        virtual char dataTagInternal() const;
};

/**
 * A gluing permutation search class that offers a specialised search algorithm
 * for when all vertex links must all have a given fixed Euler characteristic.
 * Examples might be Euler characteristic 2 (for closed manifolds),
 * or Euler characteristic 0 (for manifolds with torus and/or Klein
 * bottle cusps).  In addition, we require that every edge must be valid
 * (i.e., not identified with itself in reverse).
 *
 * Vertices on boundary triangles are treated a little differently.
 * If the underlying face pairing includes boundary triangles and the
 * given Euler characteristic is \a E, then boundary vertex links must have
 * Euler characteristic \a E-1, and must have exactly one puncture.
 * For instance, if \a E is 2 and the face pairing includes boundary faces,
 * then all vertex links must be either spheres (for internal vertices)
 * or discs (for boundary vertices).
 *
 * The search algorithm uses modified union-find structures on both
 * edge and vertex equivalence classes to prune searches that are
 * guaranteed to lead to bad edge or vertex links.  For details see
 * "Enumeration of non-orientable 3-manifolds using face-pairing graphs and
 * union-find", Benjamin A. Burton, Discrete Comput. Geom. 38 (2007), no. 3,
 * 527--571; and "Detecting genus in vertex links for the fast enumeration
 * of 3-manifold triangulations", Benjamin A. Burton, in "ISSAC 2011:
 * Proceedings of the 36th International Symposium on Symbolic and
 * Algebraic Computation", ACM, 2011, pp. 59-66.
 *
 * No additional unwanted triangulations will be produced by this search
 * (in contrast to other search classes, such as ClosedPrimeMinSearcher).
 * That is, \e only 3-manifolds with the required vertex links will be produced.
 *
 * This class is designed to manage the construction of a large census of
 * triangulations, and so it does not support copying, moving or swapping.
 *
 * \ingroup census
 */
class EulerSearcher : public GluingPermSearcher<3> {
    protected:
        static constexpr char VLINK_CLOSED = 1;
            /**< Signifies that a vertex link has been closed off (i.e.,
                 the link has no remaining boundary edges). */
        static constexpr char VLINK_BAD_EULER = 2;
            /**< Signifies that a vertex link has either (i) accumulated too
                 high a genus (so when its punctures are filled the Euler
                 characteristic will be too low), or has (ii) been closed
                 off with too low a genus (so its final Euler characteristic
                 is too high). */

        /**
         * Constants used in the \a vertexStateChanged array to indicate
         * how a partial vertex link becomes glued to itself along two
         * of its boundary edges.  See \a vertexStateChanged for details.
         */
        enum {
            VLINK_JOIN_INIT = -1,
                /**< A placeholder value for a join that has not yet
                     been made. */
            VLINK_JOIN_HANDLE = -2,
                /**< Signifies that two distinct boundary components of
                     a vertex link have been connected, forming a handle
                     (either orientable or non-orientable). */
            VLINK_JOIN_BRIDGE = -3,
                /**< Signifies that a single boundary component of the
                     vertex link has had two edges joined together in an
                     orientation-preserving fashion. */
            VLINK_JOIN_TWIST = -4
                /**< Signifies that a single boundary component of the
                     vertex link has had two edges joined together in an
                     orientation-reversing fashion. */
        };

    protected:
        static const int vertexLinkNextFace[4][4];
            /**< Maintains an ordering of the three tetrahedron faces
                 surrounding a vertex in a tetrahedron.  This ordering
                 is consistent with the orientations of triangles in
                 the vertex link used by TetVertexState::twistUp.

                 For vertex v (0..3), the tetrahedron face that follows
                 f (0..3) in this ordering is \a vertexLinkNextFace[v][f].
                 The remaining array elements \a vertexLinkNextFace[v][v]
                 are all -1. */

        static const int vertexLinkPrevFace[4][4];
            /**< Provides backwards links for the ordering described by
                 \a vertexLinkNextFace.

                 For vertex v (0..3), the tetrahedron face that precedes
                 f (0..3) in this ordering is \a vertexLinkPrevFace[v][f].
                 The remaining array elements \a vertexLinkPrevFace[v][v]
                 are all -1. */

    protected:
        /**
         * A structure used to track equivalence classes of tetrahedron
         * vertices as the gluing permutation set is constructed.  Two
         * vertices are considered equivalent if they are identified
         * within the triangulation.
         *
         * Tetrahedron vertices are indexed linearly by tetrahedron and
         * then vertex number.  Specifically, vertex v (0..3) of
         * tetrahedron t (0..nTets-1) has index 4t+v.
         *
         * Each equivalence class of vertices corresponds to a tree of
         * TetVertexState objects, arranged to form a modified union-find
         * structure.
         *
         * Note that a single tetrahedron vertex (as described by this
         * structure) provides a single triangular piece of the overall
         * vertex link.  This triangle piece is referred to in several
         * of the data members below.
         */
        struct TetVertexState {
            ssize_t parent;
                /**< The index of the parent object in the current tree,
                     or -1 if this object is the root of the tree. */
            size_t rank;
                /**< The depth of the subtree beneath this object (where
                     a leaf node has depth zero). */
            size_t bdry;
                /**< The number of boundary edges in the vertex link for
                     this equivalence class of vertices.  Any face whose
                     gluing permutation has not yet been decided is
                     treated as a boundary face.  This value is only
                     maintained correctly for the root of the corresponding
                     object tree; other objects in the tree will have
                     older values to facilitate backtracking. */
            int euler;
                /**< The Euler characteristic that the vertex link would have
                     if its punctures were all filled.  As above, this value
                     is only maintained correctly for the root of the
                     corresponding object tree.

                     This is of type \c int, since the search algorithm
                     ensures it will never drop more than a small constant
                     below EulerSearcher::euler_. */
            char twistUp;
                /**< The identification of this object and its parent in
                     the tree corresponds to a gluing of two triangles in the
                     vertex link.  Each of these triangles in the vertex link
                     can be labelled with its own vertices 0, 1 and 2 and
                     thereby be assigned a clockwise or anticlockwise
                     orientation.

                     The parameter \a twistUp is 0 if these two triangles
                     in the vertex link are joined in a way that preserves
                     orientation, or 1 if the gluing does not preserve
                     orientation.

                     If this object has no parent, the value of \a twistUp
                     is undefined. */
            bool hadEqualRank;
                /**< Did this tree have rank equal to its parent
                     immediately before it was grafted beneath its parent?
                     This information is used to maintain the ranks correctly
                     when grafting operations are undone.  If this object is
                     still the root of its tree, this value is set to false. */
            uint8_t bdryEdges;
                /**< The number of edges of the triangular piece of vertex
                     link that are in fact boundary edges of the vertex link.
                     Equivalently, this measures the number of faces of this
                     tetrahedron meeting this vertex that are not yet joined
                     to their partner faces.  This always takes the value
                     0, 1, 2 or 3. */
            size_t bdryNext[2];
                /**< If the corresponding triangular piece of vertex link has
                     any boundary edges, \a bdryNext stores the indices of the
                     tetrahedron vertices that provide the boundary edges
                     following on from either end of this boundary segment.

                     Note that in most cases (see below) this is not the
                     present vertex.  For instance, if this vertex provides two
                     boundary edges, then this array describes the boundary
                     before the first edge and after the second.

                     The boundary segment described by \a bdryNext[1] follows
                     on from this segment in the direction described by the
                     \a vertexLinkNextFace array.  The boundary segment in
                     the other direction is described by \a bdryNext[0].

                     If the vertex link is just this one triangle (i.e.,
                     all three faces of this tetrahedron surrounding this
                     vertex are boundary faces, or one is a boundary and
                     the other two are joined together), then both elements of
                     \a bdryNext refer to this vertex itself.  These are the
                     only situations in which \a bdryNext refers back to this
                     vertex.

                     If the triangle is internal to the vertex link
                     (i.e., \a bdryEdges is zero), then this array
                     maintains the last values it had when there was at
                     least one boundary edge earlier in the search.

                     Each element of this array lies between 0 and
                     4t-1 inclusive, where \a t is the total number of
                     tetrahedra. */
            char bdryTwist[2];
                /**< Describes whether the orientation of this boundary
                     segment of the vertex link is consistent with the
                     orientation of the adjacent segments on either side.

                     See \a bdryNext for further discussion of boundary
                     segments.  The \a bdryNext array defines an orientation
                     for this section of vertex link, pointing from the end
                     described by \a bdryNext[0] to the end described by
                     \a bdryNext[1].

                     For each \a i, the value \a bdryTwist[i] is 0 if the
                     orientation of the adjacent segment described by
                     \a bdryNext[i] is the same as this segment (as defined
                     by the \a bdryNext values stored with the adjacent
                     vertex), or 1 if the orientations differ.

                     If the triangle supplied by this vertex is internal to
                     the vertex link, this array maintains the last values
                     it had when there was at least one boundary edge earlier
                     in the search (just like the \a bdryNext array). */
            ssize_t bdryNextOld[2];
                /**< Stores a snapshot of the values in the \a bdryNext
                     array from the last point in the search when
                     \a bdryEdges was precisely two.  If \a bdryEdges is
                     still two or three, then this array is undefined. */
            char bdryTwistOld[2];
                /**< Stores a snapshot of the values in the \a bdryTwist
                     array from the last point in the search when
                     \a bdryEdges was precisely two.  If \a bdryEdges is
                     still two or three, then this array is undefined. */

            /**
             * Constructor for a standalone tetrahedron vertex in an
             * equivalence class all of its own.  Note that the vertex
             * link will be a single triangle with three boundary edges.
             */
            TetVertexState();

            /**
             * Dumps all internal data in a plain text format to the
             * given output stream.  This state can be recreated from
             * this text data by calling readData().
             *
             * This routine may be useful for transferring objects from
             * one processor to another.
             *
             * \warning The data format is liable to change between Regina
             * releases.  Data in this format should be used on a short-term
             * temporary basis only.
             *
             * @param out the output stream to which the data should be
             * written.
             */
            void dumpData(std::ostream& out) const;

            /**
             * Fills this state with data read from the given input stream.
             * This routine reads data in the format written by dumpData().
             *
             * \warning The data format is liable to change between Regina
             * releases.  Data in this format should be used on a short-term
             * temporary basis only.
             *
             * This routine does test for bad input data, but it
             * does \e not test for end-of-file.
             *
             * @param in the input stream from which to read.
             * @param nStates the total number of vertex states under
             * consideration (this must be four times the number of
             * tetrahedra).
             * @return \c false if any errors were encountered during
             * reading, or \c true otherwise.
             */
            bool readData(std::istream& in, size_t nStates);

            // Mark this class as non-copyable.
            TetVertexState(const TetVertexState&) = delete;
            TetVertexState& operator = (const TetVertexState&) = delete;
        };

        /**
         * A structure used to track equivalence classes of tetrahedron
         * edges as the gluing permutation set is constructed.  Two edges
         * are considered equivalent if they are identified within the
         * triangulation.
         *
         * Tetrahedron edges are indexed linearly by tetrahedron and
         * then edge number.  Specifically, edge e (0..5) of
         * tetrahedron t (0..nTets-1) has index 6t+e.
         *
         * Each equivalence class of edges corresponds to a tree of
         * TetEdgeState objects, arranged to form a modified union-find
         * structure.
         */
        struct TetEdgeState {
            ssize_t parent;
                /**< The index of the parent object in the current tree,
                     or -1 if this object is the root of the tree. */
            size_t rank;
                /**< The depth of the subtree beneath this object (where
                     a leaf node has depth zero). */
            size_t size;
                /**< The total number of objects in the subtree descending
                     from this object (where this object is counted also). */
            bool bounded;
                /**< Does this equivalence class of tetrahedron edges
                     represent a boundary edge?

                     If this equivalence class describes a complete loop
                     of tetrahedron edges then the value of \a bounded
                     is \c false.  If this equivalence class describes a
                     string of tetrahedron edges with two endpoints, the
                     value of \a bounded is \c true.  Here we treat any
                     face whose gluing permutation has not yet been
                     decided as a boundary face.

                     This value is only maintained correctly for the root
                     of the corresponding object tree; other objects in
                     the tree will have older values to facilitate
                     backtracking. */
            char twistUp;
                /**< Each tetrahedron edge can be assigned an
                     orientation pointing from the lower numbered
                     tetrahedron vertex to the higher.

                     The parameter \a twistUp is 0 if the identification of
                     this object and its parent in the tree preserves this
                     orientation, or 1 if it does not.  If this object has
                     no parent, the value of \a twistUp is undefined. */
            bool hadEqualRank;
                /**< Did this tree have rank equal to its parent
                     immediately before it was grafted beneath its parent?
                     This information is used to maintain the ranks correctly
                     when grafting operations are undone.  If this object is
                     still the root of its tree, this value is set to false. */

            Qitmask1<uint64_t> facesPos;
                /**< Indicates how many times this edge runs along the
                     boundary of each tetrahedron face in the positive
                     direction.  Specifically, the (4t+i)th trit counts
                     how many times it runs in the positive direction
                     around the boundary of face \a i of tetrahedron \a t.
                     Which direction is "positive" is chosen arbitrarily
                     for each face; for details see the implementation
                     of the EulerSearcher constructor.

                     Because of the fixed-size data type, this only
                     stores information for the faces of the first 16
                     tetrahedra.

                     Currently this data member is initialised by the
                     EulerSearcher constructors (since it belongs to
                     TetEdgeState), but it is only used and updated in
                     the subclass ClosedPrimeMinSearcher (where it
                     allows us to optimise the census algorithm). */
            Qitmask1<uint64_t> facesNeg;
                /**< Indicates how many times this edge runs along the
                     boundary of each tetrahedron face in the negative
                     direction.  Specifically, the (4t+i)th trit counts
                     how many times it runs in the negative direction
                     around the boundary of face \a i of tetrahedron \a t.
                     Which direction is "negative" is chosen arbitrarily
                     for each face; for details see the implementation
                     of the EulerSearcher constructor.

                     Because of the fixed-size data type, this only
                     stores information for the faces of the first 16
                     tetrahedra.

                     Currently this data member is initialised by the
                     EulerSearcher constructors (since it belongs to
                     TetEdgeState), but it is only used and updated in
                     the subclass ClosedPrimeMinSearcher (where it
                     allows us to optimise the census algorithm). */

            /**
             * Constructor for a standalone tetrahedron edge in an
             * equivalence class all of its own.
             */
            TetEdgeState();

            /**
             * Dumps all internal data in a plain text format to the
             * given output stream.  This state can be recreated from
             * this text data by calling readData().
             *
             * This routine may be useful for transferring objects from
             * one processor to another.
             *
             * \warning The data format is liable to change between Regina
             * releases.  Data in this format should be used on a short-term
             * temporary basis only.
             *
             * @param nTets the number of tetrahedra under consideration
             * in the census.
             * @param out the output stream to which the data should be
             * written.
             */
            void dumpData(std::ostream& out, size_t nTets) const;

            /**
             * Fills this state with data read from the given input stream.
             * This routine reads data in the format written by dumpData().
             *
             * \warning The data format is liable to change between Regina
             * releases.  Data in this format should be used on a short-term
             * temporary basis only.
             *
             * This routine does test for bad input data, but it
             * does \e not test for end-of-file.
             *
             * @param in the input stream from which to read.
             * @param nTets the number of tetrahedra under consideration
             * in the census.
             * @return \c false if any errors were encountered during
             * reading, or \c true otherwise.
             */
            bool readData(std::istream& in, size_t nTets);

            // Mark this class as non-copyable.
            TetEdgeState(const TetEdgeState&) = delete;
            TetEdgeState& operator = (const TetEdgeState&) = delete;
        };

    public:
        static constexpr char dataTag = 'e';
            /**< A character used to identify this class when reading
                 and writing tagged data in text format. */

    protected:
        int euler_;
            /**< The Euler characteristic that vertex links must have.
                 For boundary vertices, this is the Euler characteristic
                 of the closed surface that would be obtained if the
                 puncture in the vertex link were filled. */

        size_t nVertexClasses;
            /**< The number of equivalence classes of identified
                 tetrahedron vertices. */
        TetVertexState* vertexState;
            /**< Used for tracking equivalence classes of identified
                 tetrahedron vertices.  See the TetVertexState description
                 for details.  This array has size 4n, where vertex v of
                 tetrahedron t has index 4t+v. */
        std::make_signed_t<size_t>* vertexStateChanged;
            /**< Tracks the way in which the vertexState[] array has been
                 updated over time.  This array has size 8n, where element
                 4i+v describes how the gluing for order[i] affects vertex v
                 of the corresponding tetrahedron (thus a quarter of this
                 array will remain unused, since only three vertices are
                 affected for each gluing).

                 If this identification of vertices results in the tree
                 with root vertexState[p] being grafted beneath the tree
                 with root vertexState[q] (so two distinct vertex links
                 become joined together), this array will store the value p.
                 Otherwise it will store one of the values \a VLINK_JOIN_HANDLE,
                 \a VLINK_JOIN_BRIDGE or \a VLINK_JOIN_TWIST, indicating how
                 the corresponding partial link is glued to itself.

                 The value \a VLINK_JOIN_INIT will be stored for positions in
                 the array that correspond to gluings that have not yet been
                 made.

                 This uses a signed variant of \c size_t, since
                 it can take on several possible negative values (whereas
                 \c ssize_t only strictly supports -1). */

        size_t nEdgeClasses;
            /**< The number of equivalence classes of identified
                 tetrahedron edges. */
        TetEdgeState* edgeState;
            /**< Used for tracking equivalence classes of identified
                 tetrahedron edges.  See the TetEdgeState description
                 for details.  This array has size 6n, where edge e of
                 tetrahedron t has index 6t+e. */
        ssize_t* edgeStateChanged;
            /**< Tracks the way in which the edgeState[] array has been
                 updated over time.  This array has size 8n.  Suppose the
                 gluing for order[i] affects face f of tetrahedron t.  Then
                 element 4i+v of this array describes how the gluing for
                 order[i] affects the edge of tetrahedron t opposite vertices
                 f and v (note that a quarter of this array will remain
                 unused, since f and v are never equal).

                 If this identification of edges results in the tree
                 with root edgeState[p] being grafted beneath the tree
                 with root edgeState[q], this array will store the value p.
                 Otherwise it will store the value -1. */

    public:
        /**
         * Creates a new search manager that restricts Euler characteristic
         * on the vertex links, as described in the class overview.
         *
         * For details on how a search manager is used, see the
         * GluingPermSearcher<3> documentation.  Note in particular that
         * this class will be automatically used by
         * GluingPermSearcher<3>::findAllPerms() if possible, so there is
         * often no need for an end user to instantiate this class
         * directly.
         *
         * All constructor arguments except for \a euler are the same as for
         * the GluingPermSearcher<3> constructor, though some arguments (such as
         * \a finiteOnly) are not needed here since they are already implied
         * by the specialised search context.
         *
         * \pre The given Euler characteristic is at most 2.
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by FacetPairing<3>::isCanonical().  Note that all face pairings
         * constructed by FacetPairing<3>::findAllPairings() are of this form.
         *
         * @param useEuler the Euler characteristic that vertex links must
         * have.  For boundary vertices, this is the Euler characteristic
         * of the closed surface that would be obtained if the puncture in
         * the vertex link were filled.
         */
        EulerSearcher(int useEuler, FacetPairing<3> pairing,
                FacetPairing<3>::IsoList autos, bool orientableOnly,
                CensusPurge whichPurge);

        /**
         * Initialises a new search manager based on data read from the
         * given input stream.  This may be a new search or a partially
         * completed search.
         *
         * This routine reads data in the format written by dumpData().
         * If you wish to read data whose precise class is unknown,
         * consider using dumpTaggedData() and fromTaggedData() instead.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * \exception InvalidInput The data found in the input stream is
         * invalid, incomplete, or incorrectly formatted.
         *
         * \nopython This constructor is fundamentally designed around working
         * through a single input stream as we make our way from base class
         * constructors down to subclass constructors.  Python users should
         * use taggedData() and fromTaggedData() instead, which incorporate
         * this same text data as part of their richer text format.
         *
         * @param in the input stream from which to read.
         */
        EulerSearcher(std::istream& in);

        /**
         * Destroys this search manager and all supporting data structures.
         */
        ~EulerSearcher() override;

        // Overridden methods:
        void dumpData(std::ostream& out) const override;

    protected:
        // Overridden methods:
        void searchImpl(long maxDepth, ActionWrapper&& action) override;
        char dataTagInternal() const override;

    protected:
        /**
         * Returns the representative of the equivalence class containing
         * the given tetrahedron edge.  The class representative is
         * defined to be the root of the corresponding union-find tree.
         *
         * See the TetEdgeState class for further details.  See also the
         * other variant of findEdgeClass(), which is slightly slower
         * but which also tracks edge orientation.
         *
         * @param edgeID the index of a single tetrahedron edge; this
         * must be between 0 and 6t-1 inclusive, where \a t is the
         * number of tetrahedra.  See the TetEdgeState class notes for
         * details on edge indexing.
         * @return the index of the tetrahedron edge at the root of the
         * union-find tree, i.e., the representative of the equivalence
         * class.
         */
        size_t findEdgeClass(size_t edgeID) const;

        /**
         * Returns the representative of the equivalence class containing
         * the given tetrahedron edge.  The class representative is
         * defined to be the root of the corresponding union-find tree.
         *
         * The argument \a twisted is also modified to indicate whether
         * or not the identification of the given edge with the class
         * representative preserves orientation.  Note that this arugment
         * is \e not initialised.  Instead, if the identification
         * is orientation-preserving then \a twisted will be left
         * unmodified, and if it is orientation-reversing then \a twisted
         * will be changed from 0 to 1 or vice-versa.
         *
         * See the TetEdgeState class for further details.  See also the
         * other variant of findEdgeClass(), which is slightly faster
         * but which does not track edge orientation.
         *
         * @param edgeID the index of a single tetrahedron edge; this
         * must be between 0 and 6t-1 inclusive, where \a t is the
         * number of tetrahedra.  See the TetEdgeState class notes for
         * details on edge indexing.
         * @param twisted used to track edge orientation, as described
         * above.  This must be either 0 or 1 as it is passed into the
         * function, and it will also be either 0 or 1 upon returning
         * from the function.
         * @return the index of the tetrahedron edge at the root of the
         * union-find tree, i.e., the representative of the equivalence
         * class.
         */
        size_t findEdgeClass(size_t edgeID, char& twisted) const;

        /**
         * Merge the classes of tetrahedron vertices as required by the
         * new gluing made at stage \a orderElt of the search.
         *
         * See the TetVertexState class for details.
         *
         * This routine returns a bitwise (OR) combination of the
         * VLINK_... flags defined earlier in this class.  These
         * flags describe what happened to the vertex links during
         * this particular merge.  In particular, they note when a
         * vertex link is closed off, or enters a state where it will be
         * forced to have the wrong Euler characteristic.
         *
         * @return a combination of VLINK_... flags describing how
         * the vertex links were changed, or 0 if none of the changes
         * described by these flags were observed.
         */
        int mergeVertexClasses();

        /**
         * Merge the classes of tetrahedron edges as required by the
         * new gluing made at stage \a orderElt of the search.
         *
         * See the TetEdgeState class for details.
         *
         * This routine returns a boolean that indicates whether this
         * merge creates an invalid edge (i.e., an edge identified with
         * itself in reverse).
         *
         * @return \c true if this merge creates an invalid edge, or
         * \c false if not.
         */
        bool mergeEdgeClasses();

        /**
         * Split the classes of tetrahedron vertices to mirror the
         * undoing of the gluing at stage \a orderElt of the search.
         *
         * See the TetVertexState class for details.
         */
        void splitVertexClasses();

        /**
         * Split the classes of tetrahedron edges to mirror the undoing
         * of the gluing at stage \a orderElt of the search.
         *
         * See the TetEdgeState class for details.
         */
        void splitEdgeClasses();

        /**
         * Signifies that the boundary edges supplied by the vertex
         * linking triangles for the two given tetrahedron vertices
         * should be marked as adjacent.  The \a bdryNext and \a bdryTwist
         * arrays for each vertex will be adjusted to point to the other.
         *
         * See the TetVertexState class for details.
         *
         * @param vertexID the first tetrahedron vertex on which to operate;
         * this must be between 0 and 4n-1 inclusive, where \a n is the number
         * of tetrahedra.
         * @param end specifies in which direction the adjacent boundary
         * edges lie.  This must be either 0 or 1, and its value should
         * correspond to the relevant index in the \a bdryNext and \a bdryTwist
         * arrays for vertex \a vertexID.
         * @param adjVertexID the tetrahedron vertex whose boundary edges are
         * adjacent to the boundary edges supplied by \a vertexID; this must
         * be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         * @param twist 0 if the orientations of the two boundary segments of
         * vertex link are oriented in the same direction, or 1 if they are
         * oriented in opposite directions; see the \a bdryTwist
         * documentation for details.
         */
        void vtxBdryJoin(size_t vertexID, char end, size_t adjVertexID,
            char twist);

        /**
         * Adjusts the \a bdryNext and \a bdryTwist arrays for
         * nearby tetrahedron vertices, to ensure that these arrays
         * are consistent with the \a bdryNext and \a bdryTwist arrays
         * stored with the given vertex.
         *
         * It is assumed that the vertex linking triangle for the given
         * tetrahedron vertex contributes at least one boundary edge to
         * the vertex link.  Recall from the TetVertexState class notes
         * that the \a bdryNext and \a bdryTwist arrays for the given
         * vertex describe the boundary edges that follow on in either
         * direction from the boundary edges supplied by this triangle.
         *
         * This routine locates the tetrahedron vertices that provide
         * the neighbouring boundary edges, and adjusts the \a bdryNext
         * and \a bdryTwist arrays for these neighbouring vertices to
         * point back to the given vertex.
         *
         * This routine is intended to assist with backtracking.  This
         * routine is safe to use if the given tetrahedron vertex points
         * to itself (i.e., it provides a complete boundary cycle of
         * three edges in the vertex link).
         *
         * See the TetVertexState class for further information.
         *
         * \pre The vertex linking triangle for the given tetrahedron
         * vertex contributes at least one boundary edge to the vertex link.
         *
         * @param vertexID the tetrahedron vertex to examine; this must
         * be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         */
        void vtxBdryFixAdj(size_t vertexID);

        /**
         * Copies the \a bdryNext and \a bdryTwist arrays to the
         * \a bdryNextOld and \a bdryTwistOld arrays for the given
         * tetrahedron vertex.
         *
         * See the TetVertexState class for further information.
         *
         * @param vertexID the tetrahedron vertex on which to operate; this
         * must be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         */
        void vtxBdryBackup(size_t vertexID);

        /**
         * Copies the \a bdryNextOld and \a bdryTwistOld arrays to the
         * \a bdryNext and \a bdryTwist arrays for the given tetrahedron
         * vertex.
         *
         * See the TetVertexState class for further information.
         *
         * @param vertexID the tetrahedron vertex on which to operate; this
         * must be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         */
        void vtxBdryRestore(size_t vertexID);

        /**
         * Assuming the given edge of the vertex linking triangle for the
         * given tetrahedron vertex lies on the boundary of the vertex link,
         * this routine identifies the adjacent boundary edges of the vertex
         * link in each direction.  The given edge of the vertex linking
         * triangle must belong to one of the two tetrahedron faces
         * currently being joined.
         *
         * The tetrahedron vertex to examine is passed in \a vertexID,
         * \a tet and \a vertex, and the particular edge of the vertex
         * linking triangle to examine is specified by \a bdryFace.
         * Details of the adjacent boundary edges are returned in the
         * arrays \a next and \a twist.
         *
         * Note that the values returned might or might not correspond
         * to the \a bdryNext and \a bdryTwist arrays of the
         * TetVertexState class, since the TetVertexState arrays skip
         * over adjacent edges belonging to the same vertex linking triangle.
         *
         * If the given edge of the vertex linking triangle is not a
         * boundary edge of the vertex link, the behaviour of this
         * routine is undefined.
         *
         * See the TetVertexState class for further information.
         *
         * \pre The tetrahedron face (\a tet, \a bdryFace) is one of the
         * two faces that are currently being joined together.  That is,
         * this face is either order[orderElt] or its partner in the
         * underlying face pairing.
         *
         * @param vertexID the tetrahedron vertex to examine; this must
         * be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         * @param tet the tetrahedron described by \a vertexID; this
         * must be (vertexID / 4).  It is passed separately to avoid a
         * slow division operation.
         * @param vertex the tetrahedron vertex number described by \a vertexID;
         * this must be (vertexID % 4).  It is passed separately to
         * avoid a slow modulus operation.
         * @param bdryFace the face number of the given tetrahedron
         * containing the edge of the vertex linking triangle that is
         * under consideration.  This must be between 0 and 3 inclusive,
         * and it may not be equal to \a vertex.
         * @param next returns the tetrahedron vertex supplying each
         * adjacent boundary edge; see the TetVertexState::bdryNext
         * notes for details on which directions correspond to array
         * indices 0 and 1.
         * @param twist returns whether the orientations of the adjacent
         * boundary edges are consistent with the orientation of this
         * boundary edge; see the TetVertexState::bdryTwist notes for
         * further information on orientations in the vertex link.
         */
        void vtxBdryNext(size_t vertexID, size_t tet, int vertex, int bdryFace,
            size_t next[2], char twist[2]);

        /**
         * Determines whether one of the edges of the vertex linking
         * triangle for the given tetrahedron vertex in fact forms an
         * entire one-edge boundary component of the overall vertex link.
         *
         * See the TetVertexState class for further information.
         *
         * @param vertexID the tetrahedron vertex to examine; this must
         * be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         * @return \c true if a one-edge boundary component is formed as
         * described above, or \c false otherwise.
         */
        bool vtxBdryLength1(size_t vertexID);

        /**
         * Determines whether edges of the vertex linking triangles for each
         * of the given tetrahedron vertices combine to form an entire
         * two-edge boundary component of the overall vertex link, with one
         * edge from each triangle.
         *
         * See the TetVertexState class for further information.
         *
         * @param vertexID1 the first tetrahedron vertex to examine; this
         * must be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         * @param vertexID2 the second tetrahedron vertex to examine; this
         * must be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         * @return \c true if a two-edge boundary component is formed as
         * described above, or \c false otherwise.
         */
        bool vtxBdryLength2(size_t vertexID1, size_t vertexID2);

        /**
         * Runs a number of tests on all tetrahedron vertices to locate
         * consistency errors in the \a bdryEdges, \a bdryNext and
         * \a bdryTwist members of the TetVertexState class.
         *
         * Any errors that are identified will be written to standard error.
         * Note that some errors might be harmless (for instance, when
         * a call to mergeVertexClasses() leaves processing incomplete
         * because it has located a bad vertex link and expects the
         * merge to be immediately undone).
         */
        void vtxBdryConsistencyCheck();

        /**
         * Dumps a summary of \a bdryNext, \a bdryTwist and \a bdryEdges
         * for every vertex of every tetrahedron to the given output stream.
         * The output format is relatively compact, and is subject to change
         * in future versions of Regina.  The output uses one line only, and
         * a final newline is written.
         *
         * See the TetVertexState class for further information.
         *
         * @param out the output stream to which to write.
         */
        void vtxBdryDump(std::ostream& out);
};

/**
 * A gluing permutation search class that offers a specialised search
 * algorithm for when only compact (finite) 3-manifold triangulations are
 * required.  The only constraints placed upon a triangulation are that
 * every edge must be valid (i.e., not identified with itself in reverse),
 * and that the link of every vertex must be a disk or a sphere.
 *
 * The search algorithm uses modified union-find structures on both
 * edge and vertex equivalence classes to prune searches that are
 * guaranteed to lead to bad edge or vertex links.  For details see
 * "Enumeration of non-orientable 3-manifolds using face-pairing graphs and
 * union-find", Benjamin A. Burton, Discrete Comput. Geom. 38 (2007), no. 3,
 * 527--571; and "Detecting genus in vertex links for the fast enumeration
 * of 3-manifold triangulations", Benjamin A. Burton, in "ISSAC 2011:
 * Proceedings of the 36th International Symposium on Symbolic and
 * Algebraic Computation", ACM, 2011, pp. 59-66.
 *
 * No additional unwanted triangulations will be produced by this search
 * (in contrast to other search classes, such as ClosedPrimeMinSearcher).
 * That is, \e only compact 3-manifolds will be produced.
 *
 * This class is designed to manage the construction of a large census of
 * triangulations, and so it does not support copying, moving or swapping.
 *
 * \ingroup census
 */
class CompactSearcher : public GluingPermSearcher<3> {
    protected:
        static constexpr char VLINK_CLOSED = 1;
            /**< Signifies that a vertex link has been closed off (i.e.,
                 the link has no remaining boundary edges). */
        static constexpr char VLINK_NON_SPHERE = 2;
            /**< Signifies that a vertex link has been made into something
                 other than a 2-sphere with zero or more punctures. */

    protected:
        static const int vertexLinkNextFace[4][4];
            /**< Maintains an ordering of the three tetrahedron faces
                 surrounding a vertex in a tetrahedron.  This ordering
                 is consistent with the orientations of triangles in
                 the vertex link used by TetVertexState::twistUp.

                 For vertex v (0..3), the tetrahedron face that follows
                 f (0..3) in this ordering is \a vertexLinkNextFace[v][f].
                 The remaining array elements \a vertexLinkNextFace[v][v]
                 are all -1. */

        static const int vertexLinkPrevFace[4][4];
            /**< Provides backwards links for the ordering described by
                 \a vertexLinkNextFace.

                 For vertex v (0..3), the tetrahedron face that precedes
                 f (0..3) in this ordering is \a vertexLinkPrevFace[v][f].
                 The remaining array elements \a vertexLinkPrevFace[v][v]
                 are all -1. */

    protected:
        /**
         * A structure used to track equivalence classes of tetrahedron
         * vertices as the gluing permutation set is constructed.  Two
         * vertices are considered equivalent if they are identified
         * within the triangulation.
         *
         * Tetrahedron vertices are indexed linearly by tetrahedron and
         * then vertex number.  Specifically, vertex v (0..3) of
         * tetrahedron t (0..nTets-1) has index 4t+v.
         *
         * Each equivalence class of vertices corresponds to a tree of
         * TetVertexState objects, arranged to form a modified union-find
         * structure.
         *
         * Note that a single tetrahedron vertex (as described by this
         * structure) provides a single triangular piece of the overall
         * vertex link.  This triangle piece is referred to in several
         * of the data members below.
         */
        struct TetVertexState {
            ssize_t parent;
                /**< The index of the parent object in the current tree,
                     or -1 if this object is the root of the tree. */
            size_t rank;
                /**< The depth of the subtree beneath this object (where
                     a leaf node has depth zero). */
            size_t bdry;
                /**< The number of boundary edges in the vertex link for
                     this equivalence class of vertices.  Any face whose
                     gluing permutation has not yet been decided is
                     treated as a boundary face.  This value is only
                     maintained correctly for the root of the corresponding
                     object tree; other objects in the tree will have
                     older values to facilitate backtracking. */
            char twistUp;
                /**< The identification of this object and its parent in
                     the tree corresponds to a gluing of two triangles in the
                     vertex link.  Each of these triangles in the vertex link
                     can be labelled with its own vertices 0, 1 and 2 and
                     thereby be assigned a clockwise or anticlockwise
                     orientation.

                     The parameter \a twistUp is 0 if these two triangles
                     in the vertex link are joined in a way that preserves
                     orientation, or 1 if the gluing does not preserve
                     orientation.

                     If this object has no parent, the value of \a twistUp
                     is undefined. */
            bool hadEqualRank;
                /**< Did this tree have rank equal to its parent
                     immediately before it was grafted beneath its parent?
                     This information is used to maintain the ranks correctly
                     when grafting operations are undone.  If this object is
                     still the root of its tree, this value is set to false. */
            uint8_t bdryEdges;
                /**< The number of edges of the triangular piece of vertex
                     link that are in fact boundary edges of the vertex link.
                     Equivalently, this measures the number of faces of this
                     tetrahedron meeting this vertex that are not yet joined
                     to their partner faces.  This always takes the value
                     0, 1, 2 or 3. */
            size_t bdryNext[2];
                /**< If the corresponding triangular piece of vertex link has
                     any boundary edges, \a bdryNext stores the indices of the
                     tetrahedron vertices that provide the boundary edges
                     following on from either end of this boundary segment.

                     Note that in most cases (see below) this is not the
                     present vertex.  For instance, if this vertex provides two
                     boundary edges, then this array describes the boundary
                     before the first edge and after the second.

                     The boundary segment described by \a bdryNext[1] follows
                     on from this segment in the direction described by the
                     \a vertexLinkNextFace array.  The boundary segment in
                     the other direction is described by \a bdryNext[0].

                     If the vertex link is just this one triangle (i.e.,
                     all three faces of this tetrahedron surrounding this
                     vertex are boundary faces, or one is a boundary and
                     the other two are joined together), then both elements of
                     \a bdryNext refer to this vertex itself.  These are the
                     only situations in which \a bdryNext refers back to this
                     vertex.

                     If the triangle is internal to the vertex link
                     (i.e., \a bdryEdges is zero), then this array
                     maintains the last values it had when there was at
                     least one boundary edge earlier in the search.

                     Each element of this array lies between 0 and
                     4t-1 inclusive, where \a t is the total number of
                     tetrahedra. */
            char bdryTwist[2];
                /**< Describes whether the orientation of this boundary
                     segment of the vertex link is consistent with the
                     orientation of the adjacent segments on either side.

                     See \a bdryNext for further discussion of boundary
                     segments.  The \a bdryNext array defines an orientation
                     for this section of vertex link, pointing from the end
                     described by \a bdryNext[0] to the end described by
                     \a bdryNext[1].

                     For each \a i, the value \a bdryTwist[i] is 0 if the
                     orientation of the adjacent segment described by
                     \a bdryNext[i] is the same as this segment (as defined
                     by the \a bdryNext values stored with the adjacent
                     vertex), or 1 if the orientations differ.

                     If the triangle supplied by this vertex is internal to
                     the vertex link, this array maintains the last values
                     it had when there was at least one boundary edge earlier
                     in the search (just like the \a bdryNext array). */
            ssize_t bdryNextOld[2];
                /**< Stores a snapshot of the values in the \a bdryNext
                     array from the last point in the search when
                     \a bdryEdges was precisely two.  If \a bdryEdges is
                     still two or three, then this array is undefined. */
            char bdryTwistOld[2];
                /**< Stores a snapshot of the values in the \a bdryTwist
                     array from the last point in the search when
                     \a bdryEdges was precisely two.  If \a bdryEdges is
                     still two or three, then this array is undefined. */

            /**
             * Constructor for a standalone tetrahedron vertex in an
             * equivalence class all of its own.  Note that the vertex
             * link will be a single triangle with three boundary edges.
             */
            TetVertexState();

            /**
             * Dumps all internal data in a plain text format to the
             * given output stream.  This state can be recreated from
             * this text data by calling readData().
             *
             * This routine may be useful for transferring objects from
             * one processor to another.
             *
             * \warning The data format is liable to change between Regina
             * releases.  Data in this format should be used on a short-term
             * temporary basis only.
             *
             * @param out the output stream to which the data should be
             * written.
             */
            void dumpData(std::ostream& out) const;

            /**
             * Fills this state with data read from the given input stream.
             * This routine reads data in the format written by dumpData().
             *
             * \warning The data format is liable to change between Regina
             * releases.  Data in this format should be used on a short-term
             * temporary basis only.
             *
             * This routine does test for bad input data, but it
             * does \e not test for end-of-file.
             *
             * @param in the input stream from which to read.
             * @param nStates the total number of vertex states under
             * consideration (this must be four times the number of
             * tetrahedra).
             * @return \c false if any errors were encountered during
             * reading, or \c true otherwise.
             */
            bool readData(std::istream& in, size_t nStates);

            // Mark this class as non-copyable.
            TetVertexState(const TetVertexState&) = delete;
            TetVertexState& operator = (const TetVertexState&) = delete;
        };

        /**
         * A structure used to track equivalence classes of tetrahedron
         * edges as the gluing permutation set is constructed.  Two edges
         * are considered equivalent if they are identified within the
         * triangulation.
         *
         * Tetrahedron edges are indexed linearly by tetrahedron and
         * then edge number.  Specifically, edge e (0..5) of
         * tetrahedron t (0..nTets-1) has index 6t+e.
         *
         * Each equivalence class of edges corresponds to a tree of
         * TetEdgeState objects, arranged to form a modified union-find
         * structure.
         */
        struct TetEdgeState {
            ssize_t parent;
                /**< The index of the parent object in the current tree,
                     or -1 if this object is the root of the tree. */
            size_t rank;
                /**< The depth of the subtree beneath this object (where
                     a leaf node has depth zero). */
            size_t size;
                /**< The total number of objects in the subtree descending
                     from this object (where this object is counted also). */
            bool bounded;
                /**< Does this equivalence class of tetrahedron edges
                     represent a boundary edge?

                     If this equivalence class describes a complete loop
                     of tetrahedron edges then the value of \a bounded
                     is \c false.  If this equivalence class describes a
                     string of tetrahedron edges with two endpoints, the
                     value of \a bounded is \c true.  Here we treat any
                     face whose gluing permutation has not yet been
                     decided as a boundary face.

                     This value is only maintained correctly for the root
                     of the corresponding object tree; other objects in
                     the tree will have older values to facilitate
                     backtracking. */
            char twistUp;
                /**< Each tetrahedron edge can be assigned an
                     orientation pointing from the lower numbered
                     tetrahedron vertex to the higher.

                     The parameter \a twistUp is 0 if the identification of
                     this object and its parent in the tree preserves this
                     orientation, or 1 if it does not.  If this object has
                     no parent, the value of \a twistUp is undefined. */
            bool hadEqualRank;
                /**< Did this tree have rank equal to its parent
                     immediately before it was grafted beneath its parent?
                     This information is used to maintain the ranks correctly
                     when grafting operations are undone.  If this object is
                     still the root of its tree, this value is set to false. */

            Qitmask1<uint64_t> facesPos;
                /**< Indicates how many times this edge runs along the
                     boundary of each tetrahedron face in the positive
                     direction.  Specifically, the (4t+i)th trit counts
                     how many times it runs in the positive direction
                     around the boundary of face \a i of tetrahedron \a t.
                     Which direction is "positive" is chosen arbitrarily
                     for each face; for details see the implementation
                     of the CompactSearcher constructor.

                     Because of the fixed-size data type, this only
                     stores information for the faces of the first 16
                     tetrahedra.

                     Currently this data member is initialised by the
                     CompactSearcher constructors (since it belongs to
                     TetEdgeState), but it is only used and updated in
                     the subclass ClosedPrimeMinSearcher (where it
                     allows us to optimise the census algorithm). */
            Qitmask1<uint64_t> facesNeg;
                /**< Indicates how many times this edge runs along the
                     boundary of each tetrahedron face in the negative
                     direction.  Specifically, the (4t+i)th trit counts
                     how many times it runs in the negative direction
                     around the boundary of face \a i of tetrahedron \a t.
                     Which direction is "negative" is chosen arbitrarily
                     for each face; for details see the implementation
                     of the CompactSearcher constructor.

                     Because of the fixed-size data type, this only
                     stores information for the faces of the first 16
                     tetrahedra.

                     Currently this data member is initialised by the
                     CompactSearcher constructors (since it belongs to
                     TetEdgeState), but it is only used and updated in
                     the subclass ClosedPrimeMinSearcher (where it
                     allows us to optimise the census algorithm). */

            /**
             * Constructor for a standalone tetrahedron edge in an
             * equivalence class all of its own.
             */
            TetEdgeState();

            /**
             * Dumps all internal data in a plain text format to the
             * given output stream.  This state can be recreated from
             * this text data by calling readData().
             *
             * This routine may be useful for transferring objects from
             * one processor to another.
             *
             * \warning The data format is liable to change between Regina
             * releases.  Data in this format should be used on a short-term
             * temporary basis only.
             *
             * @param nTets the number of tetrahedra under consideration
             * in the census.
             * @param out the output stream to which the data should be
             * written.
             */
            void dumpData(std::ostream& out, size_t nTets) const;

            /**
             * Fills this state with data read from the given input stream.
             * This routine reads data in the format written by dumpData().
             *
             * \warning The data format is liable to change between Regina
             * releases.  Data in this format should be used on a short-term
             * temporary basis only.
             *
             * This routine does test for bad input data, but it
             * does \e not test for end-of-file.
             *
             * @param in the input stream from which to read.
             * @param nTets the number of tetrahedra under consideration
             * in the census.
             * @return \c false if any errors were encountered during
             * reading, or \c true otherwise.
             */
            bool readData(std::istream& in, size_t nTets);

            // Mark this class as non-copyable.
            TetEdgeState(const TetEdgeState&) = delete;
            TetEdgeState& operator = (const TetEdgeState&) = delete;
        };

    public:
        static constexpr char dataTag = 'f';
            /**< A character used to identify this class when reading
                 and writing tagged data in text format. */

    protected:
        size_t nVertexClasses;
            /**< The number of equivalence classes of identified
                 tetrahedron vertices. */
        TetVertexState* vertexState;
            /**< Used for tracking equivalence classes of identified
                 tetrahedron vertices.  See the TetVertexState description
                 for details.  This array has size 4n, where vertex v of
                 tetrahedron t has index 4t+v. */
        ssize_t* vertexStateChanged;
            /**< Tracks the way in which the vertexState[] array has been
                 updated over time.  This array has size 8n, where element
                 4i+v describes how the gluing for order[i] affects vertex v
                 of the corresponding tetrahedron (thus a quarter of this
                 array will remain unused, since only three vertices are
                 affected for each gluing).

                 If this identification of vertices results in the tree
                 with root vertexState[p] being grafted beneath the tree
                 with root vertexState[q], this array will store the value p.
                 Otherwise it will store the value -1. */

        size_t nEdgeClasses;
            /**< The number of equivalence classes of identified
                 tetrahedron edges. */
        TetEdgeState* edgeState;
            /**< Used for tracking equivalence classes of identified
                 tetrahedron edges.  See the TetEdgeState description
                 for details.  This array has size 6n, where edge e of
                 tetrahedron t has index 6t+e. */
        ssize_t* edgeStateChanged;
            /**< Tracks the way in which the edgeState[] array has been
                 updated over time.  This array has size 8n.  Suppose the
                 gluing for order[i] affects face f of tetrahedron t.  Then
                 element 4i+v of this array describes how the gluing for
                 order[i] affects the edge of tetrahedron t opposite vertices
                 f and v (note that a quarter of this array will remain
                 unused, since f and v are never equal).

                 If this identification of edges results in the tree
                 with root edgeState[p] being grafted beneath the tree
                 with root edgeState[q], this array will store the value p.
                 Otherwise it will store the value -1. */

    public:
        /**
         * Creates a new search manager for use when only compact 3-manifold
         * triangulations are required.
         *
         * For details on how a search manager is used, see the
         * GluingPermSearcher<3> documentation.  Note in particular that
         * this class will be automatically used by
         * GluingPermSearcher<3>::findAllPerms() if possible, so there is
         * often no need for an end user to instantiate this class
         * directly.
         *
         * All constructor arguments are the same as for the
         * GluingPermSearcher<3> constructor, though some arguments (such as
         * \a finiteOnly) are not needed here since they are already implied
         * by the specialised search context.
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by FacetPairing<3>::isCanonical().  Note that all face pairings
         * constructed by FacetPairing<3>::findAllPairings() are of this form.
         */
        CompactSearcher(FacetPairing<3> pairing, FacetPairing<3>::IsoList autos,
                bool orientableOnly, CensusPurge whichPurge);

        /**
         * Initialises a new search manager based on data read from the
         * given input stream.  This may be a new search or a partially
         * completed search.
         *
         * This routine reads data in the format written by dumpData().
         * If you wish to read data whose precise class is unknown,
         * consider using dumpTaggedData() and fromTaggedData() instead.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * \exception InvalidInput The data found in the input stream is
         * invalid, incomplete, or incorrectly formatted.
         *
         * \nopython This constructor is fundamentally designed around working
         * through a single input stream as we make our way from base class
         * constructors down to subclass constructors.  Python users should
         * use taggedData() and fromTaggedData() instead, which incorporate
         * this same text data as part of their richer text format.
         *
         * @param in the input stream from which to read.
         */
        CompactSearcher(std::istream& in);

        /**
         * Destroys this search manager and all supporting data structures.
         */
        ~CompactSearcher() override;

        // Overridden methods:
        void dumpData(std::ostream& out) const override;

    protected:
        // Overridden methods:
        void searchImpl(long maxDepth, ActionWrapper&& action) override;
        char dataTagInternal() const override;

    protected:
        /**
         * Returns the representative of the equivalence class containing
         * the given tetrahedron edge.  The class representative is
         * defined to be the root of the corresponding union-find tree.
         *
         * See the TetEdgeState class for further details.  See also the
         * other variant of findEdgeClass(), which is slightly slower
         * but which also tracks edge orientation.
         *
         * @param edgeID the index of a single tetrahedron edge; this
         * must be between 0 and 6t-1 inclusive, where \a t is the
         * number of tetrahedra.  See the TetEdgeState class notes for
         * details on edge indexing.
         * @return the index of the tetrahedron edge at the root of the
         * union-find tree, i.e., the representative of the equivalence
         * class.
         */
        size_t findEdgeClass(size_t edgeID) const;

        /**
         * Returns the representative of the equivalence class containing
         * the given tetrahedron edge.  The class representative is
         * defined to be the root of the corresponding union-find tree.
         *
         * The argument \a twisted is also modified to indicate whether
         * or not the identification of the given edge with the class
         * representative preserves orientation.  Note that this arugment
         * is \e not initialised.  Instead, if the identification
         * is orientation-preserving then \a twisted will be left
         * unmodified, and if it is orientation-reversing then \a twisted
         * will be changed from 0 to 1 or vice-versa.
         *
         * See the TetEdgeState class for further details.  See also the
         * other variant of findEdgeClass(), which is slightly faster
         * but which does not track edge orientation.
         *
         * @param edgeID the index of a single tetrahedron edge; this
         * must be between 0 and 6t-1 inclusive, where \a t is the
         * number of tetrahedra.  See the TetEdgeState class notes for
         * details on edge indexing.
         * @param twisted used to track edge orientation, as described
         * above.  This must be either 0 or 1 as it is passed into the
         * function, and it will also be either 0 or 1 upon returning
         * from the function.
         * @return the index of the tetrahedron edge at the root of the
         * union-find tree, i.e., the representative of the equivalence
         * class.
         */
        size_t findEdgeClass(size_t edgeID, char& twisted) const;

        /**
         * Merge the classes of tetrahedron vertices as required by the
         * new gluing made at stage \a orderElt of the search.
         *
         * See the TetVertexState class for details.
         *
         * This routine returns a bitwise (OR) combination of the
         * VLINK_... flags defined earlier in this class.  These
         * flags describe what happened to the vertex links during
         * this particular merge.  In particular, they note when a
         * vertex link is closed off, or is made into something other
         * than a punctured 2-sphere.
         *
         * @return a combination of VLINK_... flags describing how
         * the vertex links were changed, or 0 if none of the changes
         * described by these flags were observed.
         */
        int mergeVertexClasses();

        /**
         * Merge the classes of tetrahedron edges as required by the
         * new gluing made at stage \a orderElt of the search.
         *
         * See the TetEdgeState class for details.
         *
         * This routine returns a boolean that indicates whether this
         * merge creates an invalid edge (i.e., an edge identified with
         * itself in reverse).
         *
         * @return \c true if this merge creates an invalid edge, or
         * \c false if not.
         */
        bool mergeEdgeClasses();

        /**
         * Split the classes of tetrahedron vertices to mirror the
         * undoing of the gluing at stage \a orderElt of the search.
         *
         * See the TetVertexState class for details.
         */
        void splitVertexClasses();

        /**
         * Split the classes of tetrahedron edges to mirror the undoing
         * of the gluing at stage \a orderElt of the search.
         *
         * See the TetEdgeState class for details.
         */
        void splitEdgeClasses();

        /**
         * Signifies that the boundary edges supplied by the vertex
         * linking triangles for the two given tetrahedron vertices
         * should be marked as adjacent.  The \a bdryNext and \a bdryTwist
         * arrays for each vertex will be adjusted to point to the other.
         *
         * See the TetVertexState class for details.
         *
         * @param vertexID the first tetrahedron vertex on which to operate;
         * this must be between 0 and 4n-1 inclusive, where \a n is the number
         * of tetrahedra.
         * @param end specifies in which direction the adjacent boundary
         * edges lie.  This must be either 0 or 1, and its value should
         * correspond to the relevant index in the \a bdryNext and \a bdryTwist
         * arrays for vertex \a vertexID.
         * @param adjVertexID the tetrahedron vertex whose boundary edges are
         * adjacent to the boundary edges supplied by \a vertexID; this must
         * be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         * @param twist 0 if the orientations of the two boundary segments of
         * vertex link are oriented in the same direction, or 1 if they are
         * oriented in opposite directions; see the \a bdryTwist
         * documentation for details.
         */
        void vtxBdryJoin(size_t vertexID, char end, size_t adjVertexID,
            char twist);

        /**
         * Adjusts the \a bdryNext and \a bdryTwist arrays for
         * nearby tetrahedron vertices, to ensure that these arrays
         * are consistent with the \a bdryNext and \a bdryTwist arrays
         * stored with the given vertex.
         *
         * It is assumed that the vertex linking triangle for the given
         * tetrahedron vertex contributes at least one boundary edge to
         * the vertex link.  Recall from the TetVertexState class notes
         * that the \a bdryNext and \a bdryTwist arrays for the given
         * vertex describe the boundary edges that follow on in either
         * direction from the boundary edges supplied by this triangle.
         *
         * This routine locates the tetrahedron vertices that provide
         * the neighbouring boundary edges, and adjusts the \a bdryNext
         * and \a bdryTwist arrays for these neighbouring vertices to
         * point back to the given vertex.
         *
         * This routine is intended to assist with backtracking.  This
         * routine is safe to use if the given tetrahedron vertex points
         * to itself (i.e., it provides a complete boundary cycle of
         * three edges in the vertex link).
         *
         * See the TetVertexState class for further information.
         *
         * \pre The vertex linking triangle for the given tetrahedron
         * vertex contributes at least one boundary edge to the vertex link.
         *
         * @param vertexID the tetrahedron vertex to examine; this must
         * be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         */
        void vtxBdryFixAdj(size_t vertexID);

        /**
         * Copies the \a bdryNext and \a bdryTwist arrays to the
         * \a bdryNextOld and \a bdryTwistOld arrays for the given
         * tetrahedron vertex.
         *
         * See the TetVertexState class for further information.
         *
         * @param vertexID the tetrahedron vertex on which to operate; this
         * must be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         */
        void vtxBdryBackup(size_t vertexID);

        /**
         * Copies the \a bdryNextOld and \a bdryTwistOld arrays to the
         * \a bdryNext and \a bdryTwist arrays for the given tetrahedron
         * vertex.
         *
         * See the TetVertexState class for further information.
         *
         * @param vertexID the tetrahedron vertex on which to operate; this
         * must be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         */
        void vtxBdryRestore(size_t vertexID);

        /**
         * Assuming the given edge of the vertex linking triangle for the
         * given tetrahedron vertex lies on the boundary of the vertex link,
         * this routine identifies the adjacent boundary edges of the vertex
         * link in each direction.  The given edge of the vertex linking
         * triangle must belong to one of the two tetrahedron faces
         * currently being joined.
         *
         * The tetrahedron vertex to examine is passed in \a vertexID,
         * \a tet and \a vertex, and the particular edge of the vertex
         * linking triangle to examine is specified by \a bdryFace.
         * Details of the adjacent boundary edges are returned in the
         * arrays \a next and \a twist.
         *
         * Note that the values returned might or might not correspond
         * to the \a bdryNext and \a bdryTwist arrays of the
         * TetVertexState class, since the TetVertexState arrays skip
         * over adjacent edges belonging to the same vertex linking triangle.
         *
         * If the given edge of the vertex linking triangle is not a
         * boundary edge of the vertex link, the behaviour of this
         * routine is undefined.
         *
         * See the TetVertexState class for further information.
         *
         * \pre The tetrahedron face (\a tet, \a bdryFace) is one of the
         * two faces that are currently being joined together.  That is,
         * this face is either order[orderElt] or its partner in the
         * underlying face pairing.
         *
         * @param vertexID the tetrahedron vertex to examine; this must
         * be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         * @param tet the tetrahedron described by \a vertexID; this
         * must be (vertexID / 4).  It is passed separately to avoid a
         * slow division operation.
         * @param vertex the tetrahedron vertex number described by \a vertexID;
         * this must be (vertexID % 4).  It is passed separately to
         * avoid a slow modulus operation.
         * @param bdryFace the face number of the given tetrahedron
         * containing the edge of the vertex linking triangle that is
         * under consideration.  This must be between 0 and 3 inclusive,
         * and it may not be equal to \a vertex.
         * @param next returns the tetrahedron vertex supplying each
         * adjacent boundary edge; see the TetVertexState::bdryNext
         * notes for details on which directions correspond to array
         * indices 0 and 1.
         * @param twist returns whether the orientations of the adjacent
         * boundary edges are consistent with the orientation of this
         * boundary edge; see the TetVertexState::bdryTwist notes for
         * further information on orientations in the vertex link.
         */
        void vtxBdryNext(size_t vertexID, size_t tet, int vertex, int bdryFace,
            size_t next[2], char twist[2]);

        /**
         * Determines whether one of the edges of the vertex linking
         * triangle for the given tetrahedron vertex in fact forms an
         * entire one-edge boundary component of the overall vertex link.
         *
         * See the TetVertexState class for further information.
         *
         * @param vertexID the tetrahedron vertex to examine; this must
         * be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         * @return \c true if a one-edge boundary component is formed as
         * described above, or \c false otherwise.
         */
        bool vtxBdryLength1(size_t vertexID);

        /**
         * Determines whether edges of the vertex linking triangles for each
         * of the given tetrahedron vertices combine to form an entire
         * two-edge boundary component of the overall vertex link, with one
         * edge from each triangle.
         *
         * See the TetVertexState class for further information.
         *
         * @param vertexID1 the first tetrahedron vertex to examine; this
         * must be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         * @param vertexID2 the second tetrahedron vertex to examine; this
         * must be between 0 and 4n-1 inclusive, where \a n is the number of
         * tetrahedra.
         * @return \c true if a two-edge boundary component is formed as
         * described above, or \c false otherwise.
         */
        bool vtxBdryLength2(size_t vertexID1, size_t vertexID2);

        /**
         * Runs a number of tests on all tetrahedron vertices to locate
         * consistency errors in the \a bdryEdges, \a bdryNext and
         * \a bdryTwist members of the TetVertexState class.
         *
         * Any errors that are identified will be written to standard error.
         * Note that some errors might be harmless (for instance, when
         * a call to mergeVertexClasses() leaves processing incomplete
         * because it has located a bad vertex link and expects the
         * merge to be immediately undone).
         */
        void vtxBdryConsistencyCheck();

        /**
         * Dumps a summary of \a bdryNext, \a bdryTwist and \a bdryEdges
         * for every vertex of every tetrahedron to the given output stream.
         * The output format is relatively compact, and is subject to change
         * in future versions of Regina.  The output uses one line only, and
         * a final newline is written.
         *
         * See the TetVertexState class for further information.
         *
         * @param out the output stream to which to write.
         */
        void vtxBdryDump(std::ostream& out);
};

/**
 * A gluing permutation search class that offers a specialised search
 * algorithm for when (i) only closed prime minimal P2-irreducible
 * triangulations are required, and (ii) the given face pairing has
 * order at least three.
 *
 * The search algorithm is significantly different from the default
 * algorithm provided by GluingPermSearcher<3>.  It is heavily optimised
 * and takes advantage of a number of results regarding the underlying
 * face pairing graph.
 *
 * Note that additional unwanted triangulations (e.g., non-prime or
 * non-minimal triangulations) may still be produced by this search.
 * However, significantly fewer unwanted triangulations will be produced
 * when using this class instead of GluingPermSearcher<3>.
 *
 * This class is designed to manage the construction of a large census of
 * triangulations, and so it does not support copying, moving or swapping.
 *
 * \ingroup census
 */
class ClosedPrimeMinSearcher : public CompactSearcher {
    private:
        static constexpr unsigned EDGE_CHAIN_END = 1;
            /**< Represents the end of a one-ended chain in a face
                 pairing graph. */
        static constexpr unsigned EDGE_CHAIN_INTERNAL_FIRST = 2;
            /**< Represents the first edge of a double edge within a
                 one-ended chain in a face pairing graph.  The corresponding
                 element of order[] stores the face closest to the loop at
                 the end of this chain. */
        static constexpr unsigned EDGE_CHAIN_INTERNAL_SECOND = 3;
            /**< Represents the second edge of a double edge within a
                 one-ended chain in a face pairing graph.  The corresponding
                 element of order[] stores the face closest to the loop at
                 the end of this chain. */
        static constexpr unsigned EDGE_DOUBLE_FIRST = 4;
            /**< Represents the first edge of a miscellaneous double
                 edge in a face pairing graph.  The corresponding element of
                 order[] stores the face belonging to the lower numbered
                 tetrahedron. */
        static constexpr unsigned EDGE_DOUBLE_SECOND = 5;
            /**< Represents the second edge of a miscellaneous double
                 edge in a face pairing graph.  The corresponding element of
                 order[] stores the face belonging to the lower numbered
                 tetrahedron. */
        static constexpr unsigned EDGE_MISC = 6;
            /**< Represents a miscellaneous edge in a face pairing graph. */

    private:
        static constexpr char ECLASS_TWISTED = 1;
            /**< Signifies that an edge has been identified with itself
                 in reverse. */
        static constexpr char ECLASS_LOWDEG = 2;
            /**< Signifies that a set of tetrahedron edges have been
                 identified to form an internal edge of low degree
                 (degree 1 or 2 of any type, or degree 3 with three
                 distinct tetrahedra). */
        static constexpr char ECLASS_HIGHDEG = 4;
            /**< Signifies that a set of tetrahedron edges have been
                 identified to form an edge of such a high degree that
                 either a degree 1 or 2 edge must be formed elsewhere,
                 or else the final number of edges must be too low. */
        static constexpr char ECLASS_CONE = 8;
            /**< Signifies that two edges of a face have been identified
                 to form a cone (with no constraints on any additional
                 identifications that might be taking place). */
        static constexpr char ECLASS_L31 = 16;
            /**< Signifies that all three edges of a face have been
                 identified to form an L(3,1) spine. */

    private:
        static const unsigned coneEdge[12][2];
            /**< Lists all twelve possible ways in which two edges of a
                 tetrahedron could be identified to create a conical face.
                 For the ith such method, tetrahedron edges coneEdge[i][0]
                 and coneEdge[i][1] are identified.  Every element of this
                 array is between 0 and 5 inclusive. */
        static const char coneNoTwist[12];
            /**< Combines with the \a coneEdge array to list all twelve
                 possible ways in which two edges of a tetrahedron could
                 be identified to create a conical face.

                 For the ith such method, coneNoTwist[i] is 1 if tetrahedron
                 edges coneEdge[i][0,1] should be identified according to
                 their natural orientations, and coneNoTwist[i] is 0 if
                 one of these two edges must be reversed.

                 The natural orientation of a tetrahedron edge is defined
                 to point from the lower-numbered tetrahedron vertex to the
                 higher.  This is consistent with the orientation used in
                 the TetEdgeState class. */

    public:
        static constexpr char dataTag = 'c';
            /**< A character used to identify this class when reading
                 and writing tagged data in text format. */

    private:
        unsigned* orderType;
            /**< For each edge in the face pairing graph stored in the
                 order[] array, a corresponding category for this edge is
                 stored in the orderType[] array.  Categories are described
                 by the EDGE_... constants defined in this class. */

        size_t nChainEdges;
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

#if PRUNE_HIGH_DEG_EDGE_SET
        size_t highDegLimit;
            /**< The lowest allowable edge degree.  If the underlying
                 face pairing graph supports a (1,3,4) layered solid
                 torus, this will be 3.  Otherwise it will be 4.
                 We use \c size_t for compatibilitywith \a highDegSum. */
        size_t highDegSum;
            /**< The sum of (\a degree - \a highDegLimit) over all edges whose
                 degree is \a highDegLimit or higher.  This sum is updated
                 throughout the search as part of the high-degree edge pruning
                 code.  See the PRUNE_HIGH_DEG_EDGE_SET macro for further
                 details. */
        size_t highDegBound;
            /**< The maximum allowable value of \a highDegSum.  If the
                 sum \a highDegSum exceeds this bound then it can be proven
                 that some edge of the final triangulation must have degree
                 less than \a highDegLimit.  This is part of the high-degree
                 edge pruning code; see the PRUNE_HIGH_DEG_EDGE_SET macro for
                 further details. */
#endif

    public:
        /**
         * Creates a new search manager for use when (i) only closed prime
         * minimal P2-irreducible triangulations are required, and (ii) the
         * given face pairing has order at least three.  Note that other
         * unwanted triangulations may still be produced (e.g.,
         * non-prime or non-minimal triangulations), but there will be
         * far fewer of these than when using the GluingPermSearcher<3>
         * class directly.
         *
         * For details on how a search manager is used, see the
         * GluingPermSearcher<3> documentation.  Note in particular that
         * this class will be automatically used by
         * GluingPermSearcher<3>::findAllPerms() if possible, so there is
         * often no need for an end user to instantiate this class
         * directly.
         *
         * All constructor arguments are the same as for the
         * GluingPermSearcher<3> constructor, though some arguments (such as
         * \a finiteOnly and \a whichPurge) are not needed here since they
         * are already implied by the specialised search context.
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by FacetPairing<3>::isCanonical().  Note that all face pairings
         * constructed by FacetPairing<3>::findAllPairings() are of this form.
         * \pre The given face pairing has no boundary faces and has at
         * least three tetrahedra.
         */
        ClosedPrimeMinSearcher(FacetPairing<3> pairing,
                FacetPairing<3>::IsoList autos, bool orientableOnly);

        /**
         * Initialises a new search manager based on data read from the
         * given input stream.  This may be a new search or a partially
         * completed search.
         *
         * This routine reads data in the format written by dumpData().
         * If you wish to read data whose precise class is unknown,
         * consider using dumpTaggedData() and fromTaggedData() instead.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * \exception InvalidInput The data found in the input stream is
         * invalid, incomplete, or incorrectly formatted.
         *
         * \nopython This constructor is fundamentally designed around working
         * through a single input stream as we make our way from base class
         * constructors down to subclass constructors.  Python users should
         * use taggedData() and fromTaggedData() instead, which incorporate
         * this same text data as part of their richer text format.
         *
         * @param in the input stream from which to read.
         */
        ClosedPrimeMinSearcher(std::istream& in);

        /**
         * Destroys this search manager and all supporting data structures.
         */
        ~ClosedPrimeMinSearcher() override;

        // Overridden methods:
        void dumpData(std::ostream& out) const override;

    protected:
        // Overridden methods:
        void searchImpl(long maxDepth, ActionWrapper&& action) override;
        char dataTagInternal() const override;

    private:
        /**
         * Merge the classes of tetrahedron edges as required by the
         * new gluing made at stage \a orderElt of the search.
         *
         * This overrides CompactSearcher::mergeEdgeClasses(), and
         * tests for additional structures that, whilst valid, cannot
         * appear in a closed prime minimal P2-irreducible triangulation.
         *
         * This routine returns a bitwise (OR) combination of the
         * ECLASS_... flags defined earlier in this class.  These
         * flags describe what happened to the edge classes during
         * this particular merge.  In particular, they note when edge
         * identifications form a structure that cannot possibly appear
         * in a closed prime minimal P2-irreducible triangulation.
         *
         * Note that, if multiple ECLASS_... flags are appropriate, only
         * a subset of these flags might be returned.  This is because
         * this routine might exit early after one bad structure has been
         * detected, without spending time testing for others.  It is
         * guaranteed that if at least one such flag is appropriate then
         * at least one such flag will be returned.
         *
         * @return a combination of ECLASS_... flags describing how
         * the edge links were changed, or 0 if none of the changes
         * described by these flags were observed.
         */
        int mergeEdgeClasses();

        /**
         * Split the classes of tetrahedron edges to mirror the undoing
         * of the gluing at stage \a orderElt of the search.
         *
         * This overrides CompactSearcher::splitEdgeClasses(), so that
         * we can undo the additional work performed in the overridden
         * mergeEdgeClasses().
         */
        void splitEdgeClasses();
};

/**
 * A gluing permutation search class that offers a specialised search
 * algorithm for when only minimal ideal triangulations of cusped
 * finite-volume hyperbolic 3-manifolds are required.  Here every vertex
 * link will be a torus or Klein bottle.
 *
 * Note that this searches for \e any triangulations of such hyperbolic
 * manifolds, not just \e geometric triangulations.
 *
 * Note that additional unwanted triangulations (e.g., non-hyperbolic or
 * non-minimal triangulations) may still be produced by this search.
 * However, significantly fewer unwanted triangulations will be produced
 * when using this class instead of GluingPermSearcher<3>.
 *
 * This class is designed to manage the construction of a large census of
 * triangulations, and so it does not support copying, moving or swapping.
 *
 * \ingroup census
 */
class HyperbolicMinSearcher : public EulerSearcher {
    private:
        static constexpr char ECLASS_TWISTED = 1;
            /**< Signifies that an edge has been identified with itself
                 in reverse. */
        static constexpr char ECLASS_LOWDEG = 2;
            /**< Signifies that a set of tetrahedron edges have been
                 identified to form an internal edge of low degree
                 (degree 1 or 2 of any type, or degree 3 with three
                 distinct tetrahedra). */

    public:
        static constexpr char dataTag = 'h';
            /**< A character used to identify this class when reading
                 and writing tagged data in text format. */

    public:
        /**
         * Creates a new search manager for use when only minimal ideal
         * triangulations of cusped finite-volume hyperbolic 3-manifolds are
         * required.  Here every vertex link will be a torus or Klein bottle.
         * Note that other unwanted triangulations may still be produced
         * (e.g., non-hyperbolic or non-minimal triangulations), but there
         * will be far fewer of these than when using the GluingPermSearcher<3>
         * class directly.
         *
         * For details on how a search manager is used, see the
         * GluingPermSearcher<3> documentation.
         *
         * All constructor arguments are the same as for the
         * GluingPermSearcher<3> constructor, though some arguments (such as
         * \a finiteOnly and \a whichPurge) are not needed here since they
         * are already implied by the specialised search context.
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by FacetPairing<3>::isCanonical().  Note that all face pairings
         * constructed by FacetPairing<3>::findAllPairings() are of this form.
         * \pre The given face pairing has no boundary faces.
         */
        HyperbolicMinSearcher(FacetPairing<3> pairing,
                FacetPairing<3>::IsoList autos, bool orientableOnly);

        /**
         * Initialises a new search manager based on data read from the
         * given input stream.  This may be a new search or a partially
         * completed search.
         *
         * This routine reads data in the format written by dumpData().
         * If you wish to read data whose precise class is unknown,
         * consider using dumpTaggedData() and fromTaggedData() instead.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * \exception InvalidInput The data found in the input stream is
         * invalid, incomplete, or incorrectly formatted.
         *
         * \nopython This constructor is fundamentally designed around working
         * through a single input stream as we make our way from base class
         * constructors down to subclass constructors.  Python users should
         * use taggedData() and fromTaggedData() instead, which incorporate
         * this same text data as part of their richer text format.
         *
         * @param in the input stream from which to read.
         */
        HyperbolicMinSearcher(std::istream& in);

        // Overridden methods:
        void dumpData(std::ostream& out) const override;

    protected:
        // Overridden methods:
        void searchImpl(long maxDepth, ActionWrapper&& action) override;
        char dataTagInternal() const override;

    private:
        /**
         * Merge the classes of tetrahedron edges as required by the
         * new gluing made at stage \a orderElt of the search.
         *
         * This overrides EulerSearcher::mergeEdgeClasses(), and
         * tests for additional structures that, whilst valid, cannot
         * appear in a minimal ideal triangulation of a finite-volume
         * hyperbolic manifold.
         *
         * This routine returns a bitwise (OR) combination of the
         * ECLASS_... flags defined earlier in this class.  These
         * flags describe what happened to the edge classes during
         * this particular merge.  In particular, they note when edge
         * identifications form a structure that cannot possibly appear
         * in the types of triangulations for which we are searching.
         *
         * Note that, if multiple ECLASS_... flags are appropriate, only
         * a subset of these flags might be returned.  This is because
         * this routine might exit early after one bad structure has been
         * detected, without spending time testing for others.  It is
         * guaranteed that if at least one such flag is appropriate then
         * at least one such flag will be returned.
         *
         * @return a combination of ECLASS_... flags describing how
         * the edge links were changed, or 0 if none of the changes
         * described by these flags were observed.
         */
        int mergeEdgeClasses();

        /**
         * Split the classes of tetrahedron edges to mirror the undoing
         * of the gluing at stage \a orderElt of the search.
         *
         * This overrides EulerSearcher::splitEdgeClasses(), so that
         * we can undo the additional work performed in the overridden
         * mergeEdgeClasses().
         */
        void splitEdgeClasses();
};

// Inline functions for GluingPermSearcher<3>

template <typename Action, typename... Args>
inline void GluingPermSearcher<3>::runSearch(Action&& action, Args&&... args) {
    // Delegate to a de-templatised function.
    searchImpl(-1, ActionWrapper([&](const regina::GluingPerms<3>& p) {
        action(p, std::forward<Args>(args)...);
    }));
}

template <typename Action, typename... Args>
inline void GluingPermSearcher<3>::partialSearch(long maxDepth,
        Action&& action, Args&&... args) {
    // Delegate to a de-templatised function.
    searchImpl(maxDepth, ActionWrapper([&](const regina::GluingPerms<3>& p) {
        action(p, std::forward<Args>(args)...);
    }));
}

inline bool GluingPermSearcher<3>::isComplete() const {
    return (orderElt == static_cast<ssize_t>(orderSize));
}

inline void GluingPermSearcher<3>::dumpTaggedData(std::ostream& out) const {
    out << dataTagInternal() << std::endl;
    dumpData(out);
}

inline std::string GluingPermSearcher<3>::taggedData() const {
    std::ostringstream out;
    dumpTaggedData(out);
    return out.str();
}

inline std::string GluingPermSearcher<3>::data() const {
    std::ostringstream out;
    dumpData(out);
    return out.str();
}

inline char GluingPermSearcher<3>::dataTagInternal() const {
    return GluingPermSearcher<3>::dataTag;
}

inline std::unique_ptr<GluingPermSearcher<3>>
        GluingPermSearcher<3>::fromTaggedData(std::istream& in) {
    // Read the class marker.
    char c;
    in >> c;
    if (in.eof())
        throw InvalidInput("Missing class marker "
            "when reading tagged GluingPermSearcher<3> data");

    switch (c) {
        case GluingPermSearcher<3>::dataTag:
            return std::make_unique<GluingPermSearcher<3>>(in);
        case CompactSearcher::dataTag:
            return std::make_unique<CompactSearcher>(in);
        case ClosedPrimeMinSearcher::dataTag:
            return std::make_unique<ClosedPrimeMinSearcher>(in);
        case EulerSearcher::dataTag:
            return std::make_unique<EulerSearcher>(in);
        case HyperbolicMinSearcher::dataTag:
            return std::make_unique<HyperbolicMinSearcher>(in);
        default:
            throw InvalidInput("Invalid class marker "
                "when reading tagged GluingPermSearcher<3> data");
    }
}

inline std::unique_ptr<GluingPermSearcher<3>>
        GluingPermSearcher<3>::fromTaggedData(const std::string& data) {
    // With C++20 we will be able to move the string into the input stream,
    // which means the argument should become a string (not const string&).
    try {
        std::istringstream in(data);
        return fromTaggedData(in);
    } catch (const InvalidInput& exc) {
        throw InvalidArgument(exc.what());
    }
}

inline std::unique_ptr<GluingPermSearcher<3>>
        GluingPermSearcher<3>::bestSearcher(
        FacetPairing<3> pairing, FacetPairing<3>::IsoList autos,
        bool orientableOnly, bool finiteOnly, CensusPurge whichPurge) {
    // Use an optimised algorithm if possible.
    if (finiteOnly) {
        if (pairing.isClosed() && pairing.size() >= 3 &&
                whichPurge.has(PURGE_NON_MINIMAL) &&
                whichPurge.has(PURGE_NON_PRIME) &&
                (orientableOnly || whichPurge.has(PURGE_P2_REDUCIBLE))) {
            // Closed prime minimal P2-irreducible triangulations with >= 3
            // tetrahedra.
            return std::make_unique<ClosedPrimeMinSearcher>(std::move(pairing),
                std::move(autos), orientableOnly);
        }
        return std::make_unique<CompactSearcher>(std::move(pairing),
            std::move(autos), orientableOnly, whichPurge);
    }

    if (pairing.isClosed() && whichPurge.has(PURGE_NON_MINIMAL_HYP))
        return std::make_unique<HyperbolicMinSearcher>(std::move(pairing),
            std::move(autos), orientableOnly);

    return std::make_unique<GluingPermSearcher<3>>(std::move(pairing),
        std::move(autos), orientableOnly, finiteOnly, whichPurge);
}

template <typename Action, typename... Args>
inline void GluingPermSearcher<3>::findAllPerms(FacetPairing<3> pairing,
        FacetPairing<3>::IsoList autos, bool orientableOnly, bool finiteOnly,
        CensusPurge whichPurge, Action&& action, Args&&... args) {
    bestSearcher(std::move(pairing), std::move(autos), orientableOnly,
        finiteOnly, whichPurge)->
        runSearch(std::forward<Action>(action), std::forward<Args>(args)...);
}

// Inline functions for EulerSearcher

inline EulerSearcher::TetVertexState::TetVertexState() :
        parent(-1), rank(0), bdry(3), euler(2), twistUp(0),
        hadEqualRank(false) {
}

inline EulerSearcher::TetEdgeState::TetEdgeState() :
        parent(-1), rank(0), size(1), bounded(true), twistUp(0),
        hadEqualRank(false) {
}

inline EulerSearcher::~EulerSearcher() {
    delete[] vertexState;
    delete[] vertexStateChanged;
    delete[] edgeState;
    delete[] edgeStateChanged;
}

inline char EulerSearcher::dataTagInternal() const {
    return EulerSearcher::dataTag;
}

inline size_t EulerSearcher::findEdgeClass(size_t edgeID) const {
    while (edgeState[edgeID].parent >= 0)
        edgeID = edgeState[edgeID].parent;

    return edgeID;
}

inline size_t EulerSearcher::findEdgeClass(size_t edgeID, char& twisted) const {
    for ( ; edgeState[edgeID].parent >= 0; edgeID = edgeState[edgeID].parent)
        twisted ^= edgeState[edgeID].twistUp;

    return edgeID;
}

inline void EulerSearcher::vtxBdryJoin(size_t vertexID, char end,
        size_t adjVertexID, char twist) {
    vertexState[vertexID].bdryNext[end] = adjVertexID;
    vertexState[vertexID].bdryTwist[end] = twist;
    vertexState[adjVertexID].bdryNext[(end ^ 1) ^ twist] = vertexID;
    vertexState[adjVertexID].bdryTwist[(end ^ 1) ^ twist] = twist;
}

inline void EulerSearcher::vtxBdryFixAdj(size_t vertexID) {
    if (vertexState[vertexID].bdryNext[0] != vertexID) {
        vertexState[vertexState[vertexID].bdryNext[0]].
            bdryNext[1 ^ vertexState[vertexID].bdryTwist[0]] = vertexID;
        vertexState[vertexState[vertexID].bdryNext[0]].
            bdryTwist[1 ^ vertexState[vertexID].bdryTwist[0]] =
            vertexState[vertexID].bdryTwist[0];
        vertexState[vertexState[vertexID].bdryNext[1]].
            bdryNext[0 ^ vertexState[vertexID].bdryTwist[1]] = vertexID;
        vertexState[vertexState[vertexID].bdryNext[1]].
            bdryTwist[0 ^ vertexState[vertexID].bdryTwist[1]] =
            vertexState[vertexID].bdryTwist[1];
    }
}

inline void EulerSearcher::vtxBdryBackup(size_t vertexID) {
    vertexState[vertexID].bdryNextOld[0] = vertexState[vertexID].bdryNext[0];
    vertexState[vertexID].bdryNextOld[1] = vertexState[vertexID].bdryNext[1];
    vertexState[vertexID].bdryTwistOld[0] = vertexState[vertexID].bdryTwist[0];
    vertexState[vertexID].bdryTwistOld[1] = vertexState[vertexID].bdryTwist[1];
}

inline void EulerSearcher::vtxBdryRestore(size_t vertexID) {
    vertexState[vertexID].bdryNext[0] = vertexState[vertexID].bdryNextOld[0];
    vertexState[vertexID].bdryNext[1] = vertexState[vertexID].bdryNextOld[1];
    vertexState[vertexID].bdryTwist[0] = vertexState[vertexID].bdryTwistOld[0];
    vertexState[vertexID].bdryTwist[1] = vertexState[vertexID].bdryTwistOld[1];
}

inline void EulerSearcher::vtxBdryNext(size_t vertexID,
        size_t tet, int vertex, int bdryFace, size_t next[2], char twist[2]) {
    switch (vertexState[vertexID].bdryEdges) {
        case 3: next[0] = next[1] = vertexID;
                twist[0] = twist[1] = 0;
                break;
        case 2: if (perms_.permIndex(tet,
                        vertexLinkNextFace[vertex][bdryFace]) < 0) {
                    next[0] = vertexState[vertexID].bdryNext[0];
                    twist[0] = vertexState[vertexID].bdryTwist[0];
                    next[1] = vertexID;
                    twist[1] = 0;
                } else if (perms_.permIndex(tet,
                        vertexLinkPrevFace[vertex][bdryFace]) < 0) {
                    next[0] = vertexID;
                    twist[0] = 0;
                    next[1] = vertexState[vertexID].bdryNext[1];
                    twist[1] = vertexState[vertexID].bdryTwist[1];
                } else {
                    // We must be in the process of gluing a tetrahedron
                    // to itself, and one of the gluings hasn't happened
                    // yet (hence bdryEdges == 2 but only one boundary
                    // edge shows up in the gluing permutations).
                    // The boundary that we're not seeing must belong
                    // to either the tetrahedron face we are currently
                    // working with or its adjacent partner.
                    int ghostFace = (bdryFace == order[orderElt].facet ?
                        perms_.pairing()[order[orderElt]].facet :
                        order[orderElt].facet);
                    if (vertexLinkNextFace[vertex][bdryFace] == ghostFace) {
                        next[0] = vertexState[vertexID].bdryNext[0];
                        twist[0] = vertexState[vertexID].bdryTwist[0];
                        next[1] = vertexID;
                        twist[1] = 0;
                    } else {
                        // Sanity check.
                        if (vertexLinkPrevFace[vertex][bdryFace] != ghostFace)
                            std::cerr << "ERROR: Inconsistent vertex link "
                                "boundary information!" << std::endl;
                        next[0] = vertexID;
                        twist[0] = 0;
                        next[1] = vertexState[vertexID].bdryNext[1];
                        twist[1] = vertexState[vertexID].bdryTwist[1];
                    }
                }
                break;
        case 1: next[0] = vertexState[vertexID].bdryNext[0];
                next[1] = vertexState[vertexID].bdryNext[1];
                twist[0] = vertexState[vertexID].bdryTwist[0];
                twist[1] = vertexState[vertexID].bdryTwist[1];
                break;
    }
}

inline bool EulerSearcher::vtxBdryLength1(size_t vertexID) {
    return (vertexState[vertexID].bdryNext[0] == vertexID &&
            vertexState[vertexID].bdryEdges == 1);
}

inline bool EulerSearcher::vtxBdryLength2(
        size_t vertexID1, size_t vertexID2) {
    return (vertexState[vertexID1].bdryNext[0] == vertexID2 &&
            vertexState[vertexID1].bdryNext[1] == vertexID2 &&
            vertexState[vertexID1].bdryEdges == 1 &&
            vertexState[vertexID2].bdryEdges == 1);
}

// Inline functions for CompactSearcher

inline CompactSearcher::TetVertexState::TetVertexState() :
        parent(-1), rank(0), bdry(3), twistUp(0), hadEqualRank(false) {
}

inline CompactSearcher::TetEdgeState::TetEdgeState() :
        parent(-1), rank(0), size(1), bounded(true), twistUp(0),
        hadEqualRank(false) {
}

inline CompactSearcher::~CompactSearcher() {
    delete[] vertexState;
    delete[] vertexStateChanged;
    delete[] edgeState;
    delete[] edgeStateChanged;
}

inline char CompactSearcher::dataTagInternal() const {
    return CompactSearcher::dataTag;
}

inline size_t CompactSearcher::findEdgeClass(size_t edgeID) const {
    while (edgeState[edgeID].parent >= 0)
        edgeID = edgeState[edgeID].parent;

    return edgeID;
}

inline size_t CompactSearcher::findEdgeClass(size_t edgeID, char& twisted)
        const {
    for ( ; edgeState[edgeID].parent >= 0; edgeID = edgeState[edgeID].parent)
        twisted ^= edgeState[edgeID].twistUp;

    return edgeID;
}

inline void CompactSearcher::vtxBdryJoin(size_t vertexID, char end,
        size_t adjVertexID, char twist) {
    vertexState[vertexID].bdryNext[end] = adjVertexID;
    vertexState[vertexID].bdryTwist[end] = twist;
    vertexState[adjVertexID].bdryNext[(end ^ 1) ^ twist] = vertexID;
    vertexState[adjVertexID].bdryTwist[(end ^ 1) ^ twist] = twist;
}

inline void CompactSearcher::vtxBdryFixAdj(size_t vertexID) {
    if (vertexState[vertexID].bdryNext[0] != vertexID) {
        vertexState[vertexState[vertexID].bdryNext[0]].
            bdryNext[1 ^ vertexState[vertexID].bdryTwist[0]] = vertexID;
        vertexState[vertexState[vertexID].bdryNext[0]].
            bdryTwist[1 ^ vertexState[vertexID].bdryTwist[0]] =
            vertexState[vertexID].bdryTwist[0];
        vertexState[vertexState[vertexID].bdryNext[1]].
            bdryNext[0 ^ vertexState[vertexID].bdryTwist[1]] = vertexID;
        vertexState[vertexState[vertexID].bdryNext[1]].
            bdryTwist[0 ^ vertexState[vertexID].bdryTwist[1]] =
            vertexState[vertexID].bdryTwist[1];
    }
}

inline void CompactSearcher::vtxBdryBackup(size_t vertexID) {
    vertexState[vertexID].bdryNextOld[0] = vertexState[vertexID].bdryNext[0];
    vertexState[vertexID].bdryNextOld[1] = vertexState[vertexID].bdryNext[1];
    vertexState[vertexID].bdryTwistOld[0] = vertexState[vertexID].bdryTwist[0];
    vertexState[vertexID].bdryTwistOld[1] = vertexState[vertexID].bdryTwist[1];
}

inline void CompactSearcher::vtxBdryRestore(size_t vertexID) {
    vertexState[vertexID].bdryNext[0] = vertexState[vertexID].bdryNextOld[0];
    vertexState[vertexID].bdryNext[1] = vertexState[vertexID].bdryNextOld[1];
    vertexState[vertexID].bdryTwist[0] = vertexState[vertexID].bdryTwistOld[0];
    vertexState[vertexID].bdryTwist[1] = vertexState[vertexID].bdryTwistOld[1];
}

inline void CompactSearcher::vtxBdryNext(size_t vertexID,
        size_t tet, int vertex, int bdryFace, size_t next[2], char twist[2]) {
    switch (vertexState[vertexID].bdryEdges) {
        case 3: next[0] = next[1] = vertexID;
                twist[0] = twist[1] = 0;
                break;
        case 2: if (perms_.permIndex(tet,
                        vertexLinkNextFace[vertex][bdryFace]) < 0) {
                    next[0] = vertexState[vertexID].bdryNext[0];
                    twist[0] = vertexState[vertexID].bdryTwist[0];
                    next[1] = vertexID;
                    twist[1] = 0;
                } else if (perms_.permIndex(tet,
                        vertexLinkPrevFace[vertex][bdryFace]) < 0) {
                    next[0] = vertexID;
                    twist[0] = 0;
                    next[1] = vertexState[vertexID].bdryNext[1];
                    twist[1] = vertexState[vertexID].bdryTwist[1];
                } else {
                    // We must be in the process of gluing a tetrahedron
                    // to itself, and one of the gluings hasn't happened
                    // yet (hence bdryEdges == 2 but only one boundary
                    // edge shows up in the gluing permutations).
                    // The boundary that we're not seeing must belong
                    // to either the tetrahedron face we are currently
                    // working with or its adjacent partner.
                    int ghostFace = (bdryFace == order[orderElt].facet ?
                        perms_.pairing()[order[orderElt]].facet :
                        order[orderElt].facet);
                    if (vertexLinkNextFace[vertex][bdryFace] == ghostFace) {
                        next[0] = vertexState[vertexID].bdryNext[0];
                        twist[0] = vertexState[vertexID].bdryTwist[0];
                        next[1] = vertexID;
                        twist[1] = 0;
                    } else {
                        // Sanity check.
                        if (vertexLinkPrevFace[vertex][bdryFace] != ghostFace)
                            std::cerr << "ERROR: Inconsistent vertex link "
                                "boundary information!" << std::endl;
                        next[0] = vertexID;
                        twist[0] = 0;
                        next[1] = vertexState[vertexID].bdryNext[1];
                        twist[1] = vertexState[vertexID].bdryTwist[1];
                    }
                }
                break;
        case 1: next[0] = vertexState[vertexID].bdryNext[0];
                next[1] = vertexState[vertexID].bdryNext[1];
                twist[0] = vertexState[vertexID].bdryTwist[0];
                twist[1] = vertexState[vertexID].bdryTwist[1];
                break;
    }
}

inline bool CompactSearcher::vtxBdryLength1(size_t vertexID) {
    return (vertexState[vertexID].bdryNext[0] == vertexID &&
            vertexState[vertexID].bdryEdges == 1);
}

inline bool CompactSearcher::vtxBdryLength2(
        size_t vertexID1, size_t vertexID2) {
    return (vertexState[vertexID1].bdryNext[0] == vertexID2 &&
            vertexState[vertexID1].bdryNext[1] == vertexID2 &&
            vertexState[vertexID1].bdryEdges == 1 &&
            vertexState[vertexID2].bdryEdges == 1);
}

// Inline functions for ClosedPrimeMinSearcher

inline ClosedPrimeMinSearcher::~ClosedPrimeMinSearcher() {
    delete[] orderType;
    if (chainPermIndices)
        delete[] chainPermIndices;
}

inline char ClosedPrimeMinSearcher::dataTagInternal() const {
    return ClosedPrimeMinSearcher::dataTag;
}

// Inline functions for HyperbolicMinSearcher

inline HyperbolicMinSearcher::HyperbolicMinSearcher(std::istream& in) :
        EulerSearcher(in) {
}

inline char HyperbolicMinSearcher::dataTagInternal() const {
    return HyperbolicMinSearcher::dataTag;
}

} // namespace regina

#endif

