
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

/*! \file census/gluingpermsearcher2.h
 *  \brief Supports searching through all possible sets of triangle
 *  gluing permutations for a given triangle edge pairing.
 */

#ifndef __REGINA_GLUINGPERMSEARCHER2_H
#ifndef __DOXYGEN
#define __REGINA_GLUINGPERMSEARCHER2_H
#endif

#include <functional>
#include <memory>
#include <sstream>
#include "core/output.h"
#include "census/gluingperms.h"
#include "census/gluingpermsearcher.h"
#include "triangulation/facetpairing.h"
#include "triangulation/generic/isomorphism.h"
#include "utilities/exception.h"

namespace regina {

/**
 * A utility class for searching through all possible gluing permutation
 * sets that correspond to a given triangle edge pairing.  In the future,
 * there may be subclasses of GluingPermSearcher<2> that correspond to
 * specialised search algorithms for use in certain scenarios.
 * The main class GluingPermSearcher<2> offers a default search
 * algorithm that may be used in a general context.
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
class GluingPermSearcher<2> : public ShortOutput<GluingPermSearcher<2>> {
    public:
        static constexpr char dataTag = 'g';
            /**< A character used to identify this class when reading
                 and writing tagged data in text format. */

    protected:
        using ActionWrapper = std::function<void(const GluingPerms<2>&)>;
            /**< The type used to hold the user's action function and
                 arguments when enumerating gluing permutations. */

        GluingPerms<2> perms_;
            /**< The set of gluing permutations under construction. */
        const FacetPairing<2>::IsoList autos_;
            /**< The set of isomorphisms that define equivalence of
                 gluing permutation sets.  Generally this is the set of all
                 automorphisms of the underlying edge pairing. */
        bool orientableOnly_;
            /**< Are we only searching for gluing permutations that
                 correspond to orientable triangulations? */

        bool started;
            /**< Has the search started yet?  This helps distinguish
                 between a new search and the resumption of a partially
                 completed search. */
        int* orientation;
            /**< Keeps track of the orientation of each triangle in the
                 underlying triangulation.  Orientation is positive/negative,
                 or 0 if unknown.
                 Note that in some algorithms the orientation is simply
                 ±1, and in some algorithms the orientation counts
                 forwards or backwards from 0 according to how many
                 times the orientation has been set or verified. */

    protected:
        FacetSpec<2>* order;
            /**< Describes the order in which gluing permutations are
                 assigned to edges.  Specifically, this order is
                 order[0], order[1], ..., order[orderSize-1].

                 Note that each element of this array corresponds to a
                 single edge of the underlying edge pairing graph, which in
                 turn represents a triangle edge and its image under
                 the given edge pairing.

                 The specific triangle edge stored in this array for each
                 edge of the underlying edge pairing graph will be the smaller
                 of the two identified triangle edges (unless otherwise
                 specified by a subclass that uses a specialised search
                 algorithm. */
        size_t orderSize;
            /**< The total number of edges in the edge pairing graph, i.e.,
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
         * \pre The given edge pairing is connected, i.e., it is possible
         * to reach any triangle from any other triangle via a
         * series of matched edge pairs.
         * \pre The given edge pairing is in canonical form as described
         * by FacetPairing<2>::isCanonical().  Note that all edge pairings
         * constructed by FacetPairing<2>::findAllPairings() are of this form.
         *
         * \param pairing the specific pairing of triangle edges
         * that the generated permutation sets will complement.
         * \param autos the collection of isomorphisms that define equivalence
         * of permutation sets.  These are used by runSearch(), which produces
         * each permutation set precisely once up to equivalence.  These
         * isomorphisms must all be automorphisms of the given edge pairing,
         * and will generally be the set of all such automorphisms (which
         * you can generate via `pairing.findAutomorphisms()`).
         * \param orientableOnly \c true if only gluing permutations
         * corresponding to orientable triangulations should be
         * generated, or \c false if no such restriction should be imposed.
         */
        GluingPermSearcher(FacetPairing<2> pairing,
                FacetPairing<2>::IsoList autos, bool orientableOnly);

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
         * \param in the input stream from which to read.
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
         * given set of automorphisms of the given edge pairing.
         *
         * For each permutation set that is generated, this routine will call
         * \a action (which must be a function or some other callable object).
         *
         * - The first argument to \a action must be a const reference to a
         *   GluingPerms<2>.  This will be the permutation set that was found.
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
         * \python This function is available, and \a action may be
         * a pure Python function.  However, \a action cannot take any
         * additional arguments beyond the initial gluing permutation set
         * (and therefore the additional \a args list is omitted here).
         *
         * \param action a function (or other callable object) to call
         * for each permutation set that is found.
         * \param args any additional arguments that should be passed to
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
         * \python This function is available, and \a action may be
         * a pure Python function.  However, \a action cannot take any
         * additional arguments beyond the initial gluing permutation set
         * (and therefore the additional \a args list is omitted here).
         *
         * \param maxDepth the depth of the partial search to run.
         * A negative number indicates that a full search should be run.
         * \param action a function (or other callable object) to call
         * for each permutation set (partial or complete) that is found.
         * \param args any additional arguments that should be passed to
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
         * \return \c true if a complete gluing permutation set is held,
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
         * \param out the output stream to which the data should be
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
         * \return all of this object's internal data in plain text format.
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
         * \param out the output stream to which the data should be written.
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
         * \python This routine is available, but the matching
         * input stream constructor is not.  Python users should use
         * taggedData() and fromTaggedData() instead.
         *
         * \return all of this object's internal data in plain text format.
         */
        std::string data() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * The main entry routine for running a search for all gluing
         * permutation sets that complement a given edge pairing.
         *
         * This routine examines the search parameters, chooses the best
         * possible search algorithm, constructs an object of the
         * corresponding subclass of GluingPermSearcher<2> and then calls
         * runSearch().
         *
         * See the GluingPermSearcher<2> constructor for documentation on
         * the arguments to this routine.  See the runSearch() method
         * for documentation on how the search runs and returns its
         * results via \a action and \a args.
         *
         * \pre The given edge pairing is connected, i.e., it is possible
         * to reach any triangle from any other triangle via a
         * series of matched edge pairs.
         * \pre The given edge pairing is in canonical form as described
         * by FacetPairing<2>::isCanonical().  Note that all edge pairings
         * constructed by FacetPairing<2>::findAllPairings() are of this form.
         *
         * \python This function is available, and \a action may be
         * a pure Python function.  However, \a action cannot take any
         * additional arguments beyond the initial gluing permutation set
         * (and therefore the additional \a args list is omitted here).
         */
        template <typename Action, typename... Args>
        static void findAllPerms(FacetPairing<2> pairing,
                FacetPairing<2>::IsoList autos, bool orientableOnly,
                Action&& action, Args&&... args);

        /**
         * Constructs a search manager of the best possible class for the
         * given search parameters.  Different subclasses of
         * GluingPermSearcher<2> provide optimised search algorithms for
         * different types of search.
         *
         * Calling this routine and then calling runSearch() on the
         * result has the same effect as the all-in-one routine
         * findAllPerms().  Unless you have specialised requirements
         * (such as partial searching), you are probably better calling
         * findAllPerms() instead.
         *
         * See the GluingPermSearcher<2> constructor for documentation on
         * the arguments to this routine.
         *
         * \pre The given edge pairing is connected, i.e., it is possible
         * to reach any triangle from any other triangle via a
         * series of matched edge pairs.
         * \pre The given edge pairing is in canonical form as described
         * by FacetPairing<2>::isCanonical().  Note that all edge pairings
         * constructed by FacetPairing<2>::findAllPairings() are of this form.
         *
         * \return the new search manager.
         */
        static std::unique_ptr<GluingPermSearcher<2>> bestSearcher(
                FacetPairing<2> pairing, FacetPairing<2>::IsoList autos,
                bool orientableOnly);

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
         * \param in the input stream from which to read.
         * \return the new search manager, or \c null if the data in the
         * input stream was invalid or incorrectly formatted.
         */
        static std::unique_ptr<GluingPermSearcher<2>> fromTaggedData(
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
         * \param data the tagged data from which to reconstruct a
         * search manager.
         * \return the new search manager, or \c null if the data in the
         * given string was invalid or incorrectly formatted.
         */
        static std::unique_ptr<GluingPermSearcher<2>> fromTaggedData(
                std::string data);

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
         * preimage under each automorphism of the underlying edge pairing,
         * in order to see whether the current set is in canonical form
         * (i.e., is lexicographically smallest).
         *
         * \return \c true if the current set is in canonical form,
         * or \c false otherwise.
         */
        bool isCanonical() const;

        /**
         * Returns the character used to identify this class when
         * storing tagged data in text format.
         *
         * \return the class tag.
         */
        virtual char dataTagInternal() const;
};

// Inline functions for GluingPermSearcher<2>

template <typename Action, typename... Args>
inline void GluingPermSearcher<2>::runSearch(Action&& action, Args&&... args) {
    // Delegate to a de-templatised function.
    searchImpl(-1, ActionWrapper([&](const regina::GluingPerms<2>& p) {
        action(p, std::forward<Args>(args)...);
    }));
}

template <typename Action, typename... Args>
inline void GluingPermSearcher<2>::partialSearch(long maxDepth,
        Action&& action, Args&&... args) {
    // Delegate to a de-templatised function.
    searchImpl(maxDepth, ActionWrapper([&](const regina::GluingPerms<2>& p) {
        action(p, std::forward<Args>(args)...);
    }));
}

inline bool GluingPermSearcher<2>::isComplete() const {
    return (orderElt == static_cast<ssize_t>(orderSize));
}

inline void GluingPermSearcher<2>::dumpTaggedData(std::ostream& out) const {
    out << dataTagInternal() << std::endl;
    dumpData(out);
}

inline std::string GluingPermSearcher<2>::taggedData() const {
    std::ostringstream out;
    dumpTaggedData(out);
    return out.str();
}

inline std::string GluingPermSearcher<2>::data() const {
    std::ostringstream out;
    dumpData(out);
    return out.str();
}

inline char GluingPermSearcher<2>::dataTagInternal() const {
    return GluingPermSearcher<2>::dataTag;
}

inline std::unique_ptr<GluingPermSearcher<2>>
        GluingPermSearcher<2>::fromTaggedData(std::istream& in) {
    // Read the class marker.
    char c;
    in >> c;
    if (in.eof())
        throw InvalidInput("Missing class marker "
            "when reading tagged GluingPermSearcher<2> data");

    switch (c) {
        case GluingPermSearcher<2>::dataTag:
            return std::make_unique<GluingPermSearcher<2>>(in);
        default:
            throw InvalidInput("Invalid class marker "
                "when reading tagged GluingPermSearcher<2> data");
    }
}

inline std::unique_ptr<GluingPermSearcher<2>>
        GluingPermSearcher<2>::fromTaggedData(std::string data) {
    try {
        std::istringstream in(std::move(data));
        return fromTaggedData(in);
    } catch (const InvalidInput& exc) {
        throw InvalidArgument(exc.what());
    }
}

inline std::unique_ptr<GluingPermSearcher<2>>
        GluingPermSearcher<2>::bestSearcher(
        FacetPairing<2> pairing, FacetPairing<2>::IsoList autos,
        bool orientableOnly) {
    // We only have one algorithm for now.
    // If we ever get to the point of choosing, we should change
    // findAllPerms() to call bestSearcher() also.
    return std::make_unique<GluingPermSearcher<2>>(std::move(pairing),
        std::move(autos), orientableOnly);
}

template <typename Action, typename... Args>
void GluingPermSearcher<2>::findAllPerms(FacetPairing<2> pairing,
        FacetPairing<2>::IsoList autos, bool orientableOnly,
        Action&& action, Args&&... args) {
    // We don't call bestSearcher() because at present there is only one
    // algorithm.  Just use it.
    GluingPermSearcher<2>(std::move(pairing), std::move(autos), orientableOnly).
        runSearch(std::forward<Action>(action), std::forward<Args>(args)...);
}

} // namespace regina

#endif

