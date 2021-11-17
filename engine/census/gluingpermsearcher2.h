
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
#include "regina-core.h"
#include "census/gluingperms.h"
#include "census/gluingpermsearcher.h"
#include "triangulation/facetpairing.h"
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
class GluingPermSearcher<2> {
    public:
        static constexpr char dataTag_ = 'g';
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
        ActionWrapper action_;
            /**< The action to perform each time a gluing permutation set is
                 found during the search.  This incorporates the user's
                 callback function along with any extra arguments that
                 the user wants passed to it. */

        bool started;
            /**< Has the search started yet?  This helps distinguish
                 between a new search and the resumption of a partially
                 completed search. */
        int* orientation;
            /**< Keeps track of the orientation of each triangle in the
                 underlying triangulation.  Orientation is positive/negative,
                 or 0 if unknown.
                 Note that in some algorithms the orientation is simply
                 +/-1, and in some algorithms the orientation counts
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
        int orderSize;
            /**< The total number of edges in the edge pairing graph, i.e.,
                 the number of elements of interest in the order[] array. */
        int orderElt;
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
         * For each facet pairing that is generated, this routine will call
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
         * \pre The given edge pairing is connected, i.e., it is possible
         * to reach any triangle from any other triangle via a
         * series of matched edge pairs.
         * \pre The given edge pairing is in canonical form as described
         * by FacetPairing<2>::isCanonical().  Note that all edge pairings
         * constructed by FacetPairing<2>::findAllPairings() are of this form.
         *
         * \ifacespython This constructor is available, and \a action may be
         * a pure Python function.  However, \a action cannot take any
         * additional arguments beyond the initial gluing permutation set
         * (and therefore the additional \a args list is omitted here).
         *
         * @param pairing the specific pairing of triangle edges
         * that the generated permutation sets will complement.
         * @param autos the collection of isomorphisms that define equivalence
         * of permutation sets.  These are used by runSearch(), which produces
         * each permutation set precisely once up to equivalence.  These
         * isomorphisms must all be automorphisms of the given edge pairing,
         * and will generally be the set of all such automorphisms (which
         * you can generate via <tt>pairing.findAutomorphisms()</tt>).
         * @param orientableOnly \c true if only gluing permutations
         * corresponding to orientable triangulations should be
         * generated, or \c false if no such restriction should be imposed.
         * @param action a function (or other callable object) to call
         * for each permutation set that is found.
         * @param args any additional arguments that should be passed to
         * \a action, following the initial permutation set argument.
         */
        template <typename Action, typename... Args>
        GluingPermSearcher(FacetPairing<2> pairing,
                FacetPairing<2>::IsoList autos, bool orientableOnly,
                Action&& action, Args&&... args);

        /**
         * Initialises a new search manager based on data read from the
         * given input stream.  This may be a new search or a partially
         * completed search.
         *
         * This routine reads data in the format written by dumpData().
         * If you wish to read data whose precise class is unknown,
         * consider using dumpTaggedData() and readTaggedData() instead.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * \exception InvalidInput the data found in the input stream is
         * invalid, incomplete, or incorrectly formatted.
         *
         * \ifacespython Not present, since this constructor is fundamentally
         * designed around working through a single input stream as we make
         * our way from base class constructors down to subclass constructors.
         * Python users should use taggedData() and readTaggedData() instead,
         * which incorporate this same text data as part of their richer text
         * format.
         *
         * @param in the input stream from which to read.
         * @param action a function (or other callable object) to call
         * for each permutation set that is found when the search is run.
         * @param args any additional arguments that should be passed to
         * \a action, following the initial permutation set argument.
         */
        template <typename Action, typename... Args>
        GluingPermSearcher(std::istream& in, Action&& action, Args&&... args);

        /**
         * Destroys this search manager and all supporting data
         * structures.
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
         * For each permutation set \a p that is generated, the function
         * or callable object \a action_ (as passed to the class constructor)
         * will be called with a const reference to \a p as its first argument.
         *
         * Subclasses corresponding to more specialised search criteria
         * should override this routine to use a better optimised algorithm
         * where possible.
         *
         * It is possible to run only a partial search, branching to a
         * given depth but no further.  In this case, rather than
         * producing complete gluing permutation sets, the search will
         * produce a series of partially-complete GluingPermSearcher<2>
         * objects.  These partial searches may then be restarted by
         * calling runSearch() once more (usually after being frozen or
         * passed on to a different processor).  If necessary, the \a action_
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
         * This may assist the \a action_ routine when running partial
         * depth-based searches.  See runSearch() for further details.
         *
         * @return \c true if a complete gluing permutation set is held,
         * or \c false otherwise.
         */
        bool completePermSet() const;

        /**
         * Dumps all internal data in a plain text format, along with a
         * marker to signify which precise class the data belongs to.
         * This routine can be used with readTaggedData() to transport
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
         * \ifacespython Not present; instead use taggedData(), which
         * returns this same information as a string.
         *
         * @param out the output stream to which the data should be
         * written.
         */
        void dumpTaggedData(std::ostream& out) const;

        /**
         * Returns all internal data in a plain text format, along with a
         * marker to signify which precise class the data belongs to.
         * This routine can be used with readTaggedData() to transport
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
         * \ifacespython Not present; instead use data(), which returns this
         * same information as a string.  However, the matching input stream
         * constructor is not available in Python either, so it is recommended
         * that Python users use taggedData() and readTaggedData() instead.
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
         * taggedData() and readTaggedData() instead.
         *
         * @param all of this object's internal data in plain text format.
         */
        std::string data() const;

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
         * results.
         *
         * \pre The given edge pairing is connected, i.e., it is possible
         * to reach any triangle from any other triangle via a
         * series of matched edge pairs.
         * \pre The given edge pairing is in canonical form as described
         * by FacetPairing<2>::isCanonical().  Note that all edge pairings
         * constructed by FacetPairing<2>::findAllPairings() are of this form.
         *
         * \ifacespython This function is available, and \a action may be
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
         * \ifacespython This function is available, and \a action may be
         * a pure Python function.  However, \a action cannot take any
         * additional arguments beyond the initial gluing permutation set
         * (and therefore the additional \a args list is omitted here).
         *
         * @return the new search manager.
         */
        template <typename Action, typename... Args>
        static std::unique_ptr<GluingPermSearcher<2>> bestSearcher(
                FacetPairing<2> pairing, FacetPairing<2>::IsoList autos,
                bool orientableOnly, Action&& action, Args&&... args);

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
         * \ifacespython Not present; instead you can use the variant of
         * readTaggedData() that takes its input as a string.
         *
         * @param in the input stream from which to read.
         * @param action a function (or other callable object) to call
         * for each permutation set that is found when the search is run.
         * @param args any additional arguments that should be passed to
         * \a action, following the initial permutation set argument.
         * @return the new search manager, or \c null if the data in the
         * input stream was invalid or incorrectly formatted.
         */
        template <typename Action, typename... Args>
        static std::unique_ptr<GluingPermSearcher<2>> readTaggedData(
                std::istream& in, Action&& action, Args&&... args);

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
         * \ifacespython This function is available, and \a action may be
         * a pure Python function.  However, \a action cannot take any
         * additional arguments beyond the initial gluing permutation set
         * (and therefore the additional \a args list is omitted here).
         *
         * @param data the tagged data from which to reconstruct a
         * search manager.
         * @param action a function (or other callable object) to call
         * for each permutation set that is found when the search is run.
         * @param args any additional arguments that should be passed to
         * \a action, following the initial permutation set argument.
         * @return the new search manager, or \c null if the data in the
         * given string was invalid or incorrectly formatted.
         */
        template <typename Action, typename... Args>
        static std::unique_ptr<GluingPermSearcher<2>> readTaggedData(
                const std::string& data, Action&& action, Args&&... args);

        // Make this class non-copyable.
        GluingPermSearcher(const GluingPermSearcher&) = delete;
        GluingPermSearcher& operator = (const GluingPermSearcher&) = delete;

    protected:
        /**
         * A de-templatised version of the main public constructor.
         *
         * Here the templated action plus arguments are bundled together
         * in a wrapper whose full type is known in advance.
         *
         * See the public input stream constructor for further details.
         *
         * We will eventually be storing \a pairing, \a autos and \a action,
         * and so we insist on rvalue references so these can be moved instead
         * of copied.
         */
        GluingPermSearcher(FacetPairing<2>&& pairing,
                FacetPairing<2>::IsoList&& autos,
                bool orientableOnly, ActionWrapper&& action);

        /**
         * A de-templatised version of the public input stream constructor.
         *
         * Here the templated action plus arguments are bundled together
         * in a wrapper whose full type is known in advance.
         *
         * See the public input stream constructor for further details.
         *
         * We will eventually be storing \a action, and so we insist on
         * an rvalue reference so this can be moved instead of copied.
         */
        GluingPermSearcher(std::istream& in, ActionWrapper&& action);

        /**
         * Compares the current set of gluing permutations with its
         * preimage under each automorphism of the underlying edge pairing,
         * in order to see whether the current set is in canonical form
         * (i.e., is lexicographically smallest).
         *
         * @return \c true if the current set is in canonical form,
         * or \c false otherwise.
         */
        bool isCanonical() const;

        /**
         * Returns the character used to identify this class when
         * storing tagged data in text format.
         *
         * @return the class tag.
         */
        virtual char dataTag() const;
};

// Inline functions for GluingPermSearcher<2>

template <typename Action, typename... Args>
inline GluingPermSearcher<2>::GluingPermSearcher(
        // NOLINTNEXTLINE(performance-unnecessary-value-param)
        FacetPairing<2> pairing, FacetPairing<2>::IsoList autos,
        bool orientableOnly, Action&& action, Args&&... args) :
        // Delegate to a de-templatised constructor.
        GluingPermSearcher<2>(std::move(pairing), std::move(autos),
            orientableOnly,
            ActionWrapper([&](const regina::GluingPerms<2>& p) {
                action(p, std::forward<Args>(args)...);
            })) {
}

template <typename Action, typename... Args>
inline GluingPermSearcher<2>::GluingPermSearcher(std::istream& in,
        Action&& action, Args&&... args) :
        // Delegate to a de-templatised constructor.
        GluingPermSearcher<2>(in,
            ActionWrapper([&](const regina::GluingPerms<2>& p) {
                action(p, std::forward<Args>(args)...);
            })) {
}

inline bool GluingPermSearcher<2>::completePermSet() const {
    return (orderElt == orderSize);
}

inline void GluingPermSearcher<2>::dumpTaggedData(std::ostream& out) const {
    out << dataTag() << std::endl;
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

inline char GluingPermSearcher<2>::dataTag() const {
    return GluingPermSearcher<2>::dataTag_;
}

template <typename Action, typename... Args>
inline std::unique_ptr<GluingPermSearcher<2>>
        GluingPermSearcher<2>::readTaggedData(
        std::istream& in, Action&& action, Args&&... args) {
    // Read the class marker.
    char c;
    in >> c;
    if (in.eof())
        return nullptr;

    try {
        switch (c) {
            case GluingPermSearcher<2>::dataTag_:
                return std::make_unique<GluingPermSearcher<2>>(in,
                    std::forward<Action>(action), std::forward<Args>(args)...);
            default:
                return nullptr;
        }
    } catch (const InvalidInput&) {
        return nullptr;
    }
}

template <typename Action, typename... Args>
inline std::unique_ptr<GluingPermSearcher<2>>
        GluingPermSearcher<2>::readTaggedData(
        const std::string& data, Action&& action, Args&&... args) {
    // With C++20 we will be able to move the string into the input stream,
    // which means the argument should become a string (not const string&).
    std::istringstream in(data);
    return readTaggedData(in, std::forward<Action>(action),
        std::forward<Args>(args)...);
}

template <typename Action, typename... Args>
inline std::unique_ptr<GluingPermSearcher<2>>
        GluingPermSearcher<2>::bestSearcher(
        FacetPairing<2> pairing, FacetPairing<2>::IsoList autos,
        bool orientableOnly, Action&& action, Args&&... args) {
    // We only have one algorithm for now.
    // If we ever get to the point of choosing, we should change
    // findAllPerms() to call bestSearcher() also.
    return std::make_unique<GluingPermSearcher<2>>(std::move(pairing),
        std::move(autos), orientableOnly, std::forward<Action>(action),
        std::forward<Args>(args)...);
}

template <typename Action, typename... Args>
void GluingPermSearcher<2>::findAllPerms(FacetPairing<2> pairing,
        FacetPairing<2>::IsoList autos, bool orientableOnly,
        Action&& action, Args&&... args) {
    // We don't call bestSearcher() because at present there is only one
    // algorithm.  Just use it.
    GluingPermSearcher<2>(std::move(pairing), std::move(autos), orientableOnly,
        std::forward<Action>(action), std::forward<Args>(args)...).
        runSearch();
}

} // namespace regina

#endif

