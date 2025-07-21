
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file census/gluingpermsearcher4.h
 *  \brief Supports searching through all possible sets of pentachoron
 *  gluing permutations for a given pentachoron facet pairing.
 */

#ifndef __REGINA_GLUINGPERMSEARCHER4_H
#ifndef __DOXYGEN
#define __REGINA_GLUINGPERMSEARCHER4_H
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
 * sets that correspond to a given pentachoron facet pairing.  Subclasses of
 * GluingPermSearcher<4> correspond to specialised (and heavily optimised)
 * search algorithms that may be used in sufficiently constrained scenarios.
 * The main class GluingPermSearcher<4> offers a default (but slower) search
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
 * The search algorithm used by this base class employs modified
 * union-find structures for both triangle and edge equivalence classes to
 * prune searches that are guaranteed to lead to invalid triangles or edges.
 * This is a 4-dimensional analogue to the algorithms described in
 * "Enumeration of non-orientable 3-manifolds using face-pairing graphs and
 * union-find", Benjamin A. Burton, Discrete Comput. Geom. 38 (2007), no. 3,
 * 527--571.
 *
 * This class is designed to manage the construction of a large census of
 * triangulations, and so it does not support copying, moving or swapping.
 *
 * \ingroup census
 */
template <>
class GluingPermSearcher<4> : public ShortOutput<GluingPermSearcher<4>> {
    protected:
        using ActionWrapper = std::function<void(const GluingPerms<4>&)>;
            /**< The type used to hold the user's action function and
                 arguments when enumerating gluing permutations. */

        static const int edgeLinkNextFacet[10][5];
            /**< Maintains an ordering of the three pentachoron facets
                 surrounding an edge in a pentachoron.  This ordering is
                 consistent with the orientations of triangles in the
                 edge link used by PentEdgeState::twistUp.

                 For edge e (0..9), the pentachoron facet that follows
                 f (0..5) in this ordering is \a edgeLinkNextFacet[e][f].
                 Note that 2/5 of the values in this array remain
                 unaccounted for; these remaining values are set to -1. */
        static const int edgeLinkPrevFacet[10][5];
            /**< Provides backwards links for the ordering described by
                 \a edgeLinkNextFacet.

                 For edge e (0..9), the pentachoron facet that follows
                 f (0..5) in this ordering is \a edgeLinkPrevFacet[e][f].
                 Again 2/5 of the values in this array remain unaccounted for,
                 and these remaining values are set to -1. */

    protected:
        /**
         * A structure used to track equivalence classes of pentachoron
         * edges as the gluing permutation set is constructed.  Two
         * edges are considered equivalent if they are identified within
         * the triangulation.
         *
         * Pentachoron edges are indexed linearly by pentachoron and
         * then edge number.  Specifically, edge e (0..9) of
         * pentachoron p (0..nPents-1) has index 10p+e.
         *
         * Each equivalence class of edges corresponds to a tree of
         * PentEdgeState objects, arranged to form a modified union-find
         * structure.
         *
         * Note that a single pentachoron edge (as described by this
         * structure) provides a single triangular piece of the overall
         * edge link.  This triangle piece is referred to in several of
         * the data members below.
         */
        struct PentEdgeState {
            ssize_t parent;
                /**< The index of the parent object in the current tree,
                     or -1 if this object is the root of the tree. */
            size_t rank;
                /**< The depth of the subtree beneath this object (where
                     a leaf node has depth zero). */
            size_t bdry;
                /**< The number of boundary triangle edges in the link
                     for this equivalence class of pentachoron edges.  Any
                     pentachoron facet whose gluing permutation has not yet
                     been decided is treated as a boundary facet.  This value
                     is only maintained correctly for the root of the
                     corresponding object tree; other objects in the tree will
                     have older values to facilitate backtracking. */
            char twistUpEdge;
                /**< Each pentachoron edge can be assigned an orientation
                     pointing from the lower numbered pentachoron vertex
                     to the higher.

                     The parameter \a twistUpEdge is 0 if the identification
                     of this object and its parent in the tree preserves this
                     orientation, or 1 if it does not.  If this object has no
                     parent, the value of \a twistUpEdge is undefined. */
            char twistUpTriangle;
                /**< The identification of this object and its parent in
                     the tree corresponds to a gluing of two triangles in the
                     edge link.  Each of these triangles in the edge link
                     can be labelled with its own vertices 0, 1 and 2 and
                     thereby be assigned a clockwise or anticlockwise
                     orientation.

                     The parameter \a twistUpTriangle is 0 if these two
                     triangles in the edge link are joined in a way that
                     preserves orientation, or 1 if the gluing does not
                     preserve orientation.

                     If this object has no parent, the value of \a
                     twistUpTriangle is undefined. */
            bool hadEqualRank;
                /**< Did this tree have rank equal to its parent
                     immediately before it was grafted beneath its parent?
                     This information is used to maintain the ranks correctly
                     when grafting operations are undone.  If this object is
                     still the root of its tree, this value is set to false. */
            uint8_t bdryEdges;
                /**< The number of edges of the triangular piece of 4-manifold
                     edge link that are in fact boundary edges of this link.
                     Equivalently, this measures the number of facets of this
                     pentachoron meeting this pentachoron edge that are not
                     yet joined to their partner facets.  This always takes
                     the value 0, 1, 2 or 3. */
            size_t bdryNext[2];
                /**< If the corresponding triangular piece of 4-manifold edge
                     link has any boundary edges, \a bdryNext stores the
                     indices of the pentachoron edges that provide the
                     boundary edges following on from either end of this
                     boundary segment.

                     Note that in most cases (see below) this is not the
                     present pentachoron edge.  For instance, if this
                     pentachoron edge provides two boundary edges for
                     the edge link, then this array describes the boundary
                     before the first edge and after the second.

                     The boundary segment described by \a bdryNext[1] follows
                     on from this segment in the direction described by the
                     \a edgeLinkNextFacet array.  The boundary segment in
                     the other direction is described by \a bdryNext[0].

                     If the 4-manifold edge link is just this one triangle
                     (i.e., all three facets of this pentachoron surrounding
                     this edge are boundary facets, or one is a boundary and
                     the other two are joined together), then both elements of
                     \a bdryNext refer to this pentachoron edge itself.  These
                     are the only situations in which \a bdryNext refers back
                     to this pentachoron edge.

                     If the triangle is internal to the 4-manifold edge link
                     (i.e., \a bdryEdges is zero), then this array
                     maintains the last values it had when there was at
                     least one boundary edge earlier in the search.

                     Each element of this array lies between 0 and
                     10p-1 inclusive, where \a p is the total number of
                     pentachora. */
            char bdryTwist[2];
                /**< Describes whether the orientation of this boundary
                     segment of the 4-manifold edge link is consistent with
                     the orientation of the adjacent segments on either side.

                     See \a bdryNext for further discussion of boundary
                     segments.  The \a bdryNext array defines an orientation
                     for this section of 4-manifold edge link, pointing from
                     the end described by \a bdryNext[0] to the end described
                     by \a bdryNext[1].

                     For each \a i, the value \a bdryTwist[i] is 0 if the
                     orientation of the adjacent segment described by
                     \a bdryNext[i] is the same as this segment (as defined
                     by the \a bdryNext values stored with the adjacent
                     pentachoron edge), or 1 if the orientations differ.

                     If the triangle supplied by this pentachoron edge is
                     internal to the edge link, this array maintains the last
                     values it had when there was at least one boundary edge
                     earlier in the search (just like the \a bdryNext array). */
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
             * Constructor for a standalone pentachoron edge in an
             * equivalence class all of its own.  Note that the edge
             * link will be a single triangle with three boundary edges.
             */
            PentEdgeState();

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
             * \param out the output stream to which the data should be
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
             * does _not_ test for end-of-file.
             *
             * \param in the input stream from which to read.
             * \param nStates the total number of edge states under
             * consideration (this must be ten times the number of tetrahedra).
             * \return \c false if any errors were encountered during
             * reading, or \c true otherwise.
             */
            bool readData(std::istream& in, size_t nStates);

            // Make this class non-copyable.
            PentEdgeState(const PentEdgeState&) = delete;
            PentEdgeState& operator = (const PentEdgeState&) = delete;
        };

        /**
         * A structure used to track equivalence classes of pentachoron
         * triangles as the gluing permutation set is constructed.  Two
         * triangles are considered equivalent if they are identified within
         * the 4-manifold triangulation.
         *
         * Pentachoron triangles are indexed linearly by pentachoron and
         * then triangle number.  Specifically, triangle f (0..9) of
         * pentachoron p (0..nPents-1) has index 10p+f.
         *
         * Each equivalence class of triangles corresponds to a tree of
         * PentTriangleState objects, arranged to form a modified union-find
         * structure.
         */
        struct PentTriangleState {
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
                /**< Does this equivalence class of pentachoron triangles
                     represent a boundary triangle?

                     If this equivalence class describes a complete loop
                     of pentachoron triangles then the value of \a bounded
                     is \c false.  If this equivalence class describes a
                     string of pentachoron triangles with two endpoints, the
                     value of \a bounded is \c true.  Here we treat any
                     facet whose gluing permutation has not yet been
                     decided as a boundary facet.

                     This value is only maintained correctly for the
                     root of the corresponding object tree; other
                     objects in the tree will have older values to
                     facilitate backtracking. */
            Perm<3> twistUp;
                /**< The vertices of each pentachoron triangle can be labelled
                     (0,1,2) by running through the underlying pentachoron
                     vertices from smallest index to largest index.

                     The parameter \a twistUp is a permutation that maps
                     vertices (0,1,2) of this triangle to vertices (0,1,2) of
                     its parent in the tree according to the way in which
                     the two triangles are identified.  If this object has
                     no parent, the value of \a twistUp is undefined. */
            bool hadEqualRank;
                /**< Did this tree have rank equal to its parent
                     immediately before it was grafted beneath its parent?
                     This information is used to maintain the ranks correctly
                     when grafting operations are undone.  If this object is
                     still the root of its tree, this value is set to false. */

            /**
             * Constructor for a standalone pentachoron triangle in an
             * equivalence class all of its own.
             */
            PentTriangleState();

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
             * \param out the output stream to which the data should be
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
             * This routine does test for bad input data, but it does _not_
             * test for end-of-file.
             *
             * \param in the input stream from which to read.
             * \param nStates the total number of triangle states under
             * consideration (this must be ten times the number of pentachora).
             * \return \c false if any errors were encountered during
             * reading, or \c true otherwise.
             */
            bool readData(std::istream& in, size_t nStates);

            // Make this class non-copyable.
            PentTriangleState(const PentTriangleState&) = delete;
            PentTriangleState& operator = (const PentTriangleState&) = delete;
        };

    public:
        static constexpr char dataTag = 'g';
            /**< A character used to identify this class when reading
                 and writing tagged data in text format. */

    protected:
        GluingPerms<4> perms_;
            /**< The set of gluing permutations under construction. */
        const FacetPairing<4>::IsoList autos_;
            /**< The set of isomorphisms that define equivalence of
                 gluing permutation sets.  Generally this is the set of all
                 automorphisms of the underlying facet pairing. */
        bool orientableOnly_;
            /**< Are we only searching for gluing permutations that
                 correspond to orientable triangulations? */
        bool finiteOnly_;
            /**< Are we only searching for gluing permutations that
                 correspond to finite (non-ideal) triangulations? */

        bool started_;
            /**< Has the search started yet?  This helps distinguish
                 between a new search and the resumption of a partially
                 completed search. */
        int* orientation_;
            /**< Keeps track of the orientation of each pentachoron in the
                 underlying triangulation.  Orientation is positive/negative,
                 or 0 if unknown.
                 Note that in some algorithms the orientation is simply
                 ±1, and in some algorithms the orientation counts
                 forwards or backwards from 0 according to how many
                 times the orientation has been set or verified. */

        FacetSpec<4>* order_;
            /**< Describes the order in which gluing permutations are
                 assigned to pentachoron facets.  Specifically, this order is
                 order_[0], order_[1], ..., order_[orderSize_-1].

                 Note that each element of this array corresponds to a
                 single edge of the underlying facet pairing graph, which in
                 turn represents a pentachoron facet and its image under
                 the given facet pairing.

                 The specific pentachoron facet stored in this array for each
                 edge of the underlying facet pairing graph will be the smaller
                 of the two identified pentachoron facets. */
        size_t orderSize_;
            /**< The total number of edges in the facet pairing graph, i.e.,
                 the number of elements of interest in the order_[] array. */
        ssize_t orderElt_;
            /**< Marks which element of order_[] we are currently examining
                 at this stage of the search. */

        size_t nEdgeClasses_;
            /**< The number of equivalence classes of identified
                 pentachoron edges. */
        PentEdgeState* edgeState_;
            /**< Used for tracking equivalence classes of identified
                 pentachoron edges.  See the PentEdgeState description
                 for details.  This array has size 10n, where edge e of
                 pentachoron p has index 10p+e. */
        ssize_t* edgeStateChanged_;
            /**< Tracks the way in which the edgeState_[] array has been
                 updated over time.  This array has size 25n.  Suppose
                 the gluing for order[i] affects facet k of pentachoron p.
                 Then element 10i+e of this array describes how the gluing for
                 order[i] affects edge e of pentachoron p.  Note that almost
                 half of this array will remain unused, since only six
                 edges of a pentachoron are affected by any one gluing.

                 If this identification of edges results in the tree
                 with root edgeState_[x] being grafted beneath the tree
                 with root edgeState_[y], this array will store the value x.
                 Otherwise it will store the value -1. */

        size_t nTriangleClasses_;
            /**< The number of equivalence classes of identified
                 pentachoron triangles. */
        PentTriangleState* triState_;
            /**< Used for tracking equivalence classes of identified
                 pentachoron triangles.  See the PentTriangleState description
                 for details.  This array has size 10n, where triangle f of
                 pentachoron p has index 10p+f. */
        ssize_t* triStateChanged_;
            /**< Tracks the way in which the triState_[] array has been
                 updated over time.  This array has size [25n/2].  Suppose
                 the gluing for order[i] affects facet k of pentachoron p.
                 Then element 5i+v of this array describes how the gluing for
                 order[i] affects the triangle of pentachoron p opposite
                 vertices k and v (note that a fifth of this array will remain
                 unused, since k and v are never equal).

                 If this identification of triangles results in the tree
                 with root triState_[x] being grafted beneath the tree
                 with root triState_[y], this array will store the value x.
                 Otherwise it will store the value -1. */

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
         * The appropriate use of parameters \a orientableOnly and
         * \a finiteOnly can significantly speed up the permutation
         * set generation.  For some combinations of these parameters
         * entirely different algorithms are used.
         *
         * Note that even if \a finiteOnly is set to \c true, some
         * ideal triangulations might still slip through the net
         * (since the full vertex links are not always constructed).
         * However, setting \a finiteOnly to \c true will
         * allow the census algorithm to take shortcuts and therefore
         * run faster.  The resulting triangulations may be tested for
         * finiteness (and other properties) by calling triangulate().
         *
         * \pre The given facet pairing is connected, i.e., it is possible
         * to reach any pentachoron from any other pentachoron via a
         * series of matched facet pairs.
         * \pre The given facet pairing is in canonical form as described
         * by FacetPairing<4>::isCanonical().  Note that all facet pairings
         * constructed by FacetPairing<4>::findAllPairings() are of this form.
         *
         * \param pairing the specific pairing of pentachoron facets
         * that the generated permutation sets will complement.
         * \param autos the collection of isomorphisms that define equivalence
         * of permutation sets.  These are used by runSearch(), which produces
         * each permutation set precisely once up to equivalence.  These
         * isomorphisms must all be automorphisms of the given facet pairing,
         * and will generally be the set of all such automorphisms (which
         * you can generate via `pairing.findAutomorphisms()`).
         * \param orientableOnly \c true if only gluing permutations
         * corresponding to orientable triangulations should be
         * generated, or \c false if no such restriction should be imposed.
         * \param finiteOnly \c true if only gluing permutations
         * corresponding to finite (non-ideal) triangulations are required, or
         * \c false if there is no such requirement.  Note that
         * regardless of this value, some ideal triangulations
         * might still be produced; see the notes above for details.
         */
        GluingPermSearcher(FacetPairing<4> pairing,
                FacetPairing<4>::IsoList autos, bool orientableOnly,
                bool finiteOnly);

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
         * given set of automorphisms of the given facet pairing.
         *
         * For each permutation set that is generated, this routine will call
         * \a action (which must be a function or some other callable object).
         *
         * - The first argument to \a action must be a const reference to a
         *   GluingPerms<4>.  This will be the permutation set that was found.
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
         * permutation sets that complement a given pentachoron facet pairing.
         *
         * This routine examines the search parameters, chooses the best
         * possible search algorithm, constructs an object of the
         * corresponding subclass of GluingPermSearcher<4> and then calls
         * runSearch().
         *
         * See the GluingPermSearcher<4> constructor for documentation on
         * the arguments to this routine.  See the runSearch() method
         * for documentation on how the search runs and returns its
         * results via \a action and \a args..
         *
         * \pre The given facet pairing is connected, i.e., it is possible
         * to reach any pentachoron from any other pentachoron via a
         * series of matched facet pairs.
         * \pre The given facet pairing is in canonical form as described
         * by FacetPairing<4>::isCanonical().  Note that all facet pairings
         * constructed by FacetPairing<4>::findAllPairings() are of this form.
         *
         * \python This function is available, and \a action may be
         * a pure Python function.  However, \a action cannot take any
         * additional arguments beyond the initial gluing permutation set
         * (and therefore the additional \a args list is omitted here).
         */
        template <typename Action, typename... Args>
        static void findAllPerms(FacetPairing<4> pairing,
                FacetPairing<4>::IsoList autos, bool orientableOnly,
                bool finiteOnly, Action&& action, Args&&... args);

        /**
         * Constructs a search manager of the best possible class for the
         * given search parameters.  Different subclasses of
         * GluingPermSearcher<4> provide optimised search algorithms for
         * different types of search.
         *
         * Calling this routine and then calling runSearch() on the
         * result has the same effect as the all-in-one routine
         * findAllPerms().  Unless you have specialised requirements
         * (such as partial searching), you are probably better calling
         * findAllPerms() instead.
         *
         * See the GluingPermSearcher<4> constructor for documentation on
         * the arguments to this routine.
         *
         * \pre The given facet pairing is connected, i.e., it is possible
         * to reach any pentachoron from any other pentachoron via a
         * series of matched facet pairs.
         * \pre The given facet pairing is in canonical form as described
         * by FacetPairing<4>::isCanonical().  Note that all facet pairings
         * constructed by FacetPairing<4>::findAllPairings() are of this form.
         *
         * \return the new search manager.
         */
        static std::unique_ptr<GluingPermSearcher<4>> bestSearcher(
                FacetPairing<4> pairing, FacetPairing<4>::IsoList autos,
                bool orientableOnly, bool finiteOnly);

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
        static std::unique_ptr<GluingPermSearcher<4>> fromTaggedData(
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
        static std::unique_ptr<GluingPermSearcher<4>> fromTaggedData(
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
         * preimage under each automorphism of the underlying facet pairing,
         * in order to see whether the current set is in canonical form
         * (i.e., is lexicographically smallest).
         *
         * \return \c true if the current set is in canonical form,
         * or \c false otherwise.
         */
        bool isCanonical() const;

        /**
         * Determines whether the permutations already constructed model
         * a 4-manifold triangulation with a (2-dimensional) triangle identified
         * with itself using a non-trivial rotation or reflection.
         *
         * Tests that do not refer to the gluing permutation for the
         * given pentachoron facet will not be run.
         *
         * This routine is not fussy about the order in which gluing
         * permutations are selected, as long as permutations not yet
         * selected have the corresponding element of permIndices[] set
         * to -1.
         *
         * \param facet the specific pentachoron facet upon which tests
         * will be based.
         * \return \c true if the permutations under construction will
         * lead to a triangle identified with itself using a non-trivial
         * rotation or reflection, or \c false if no such triangle is found.
         */
        bool badTriangleLink(const FacetSpec<4>& facet) const;

        /**
         * Returns the character used to identify this class when
         * storing tagged data in text format.
         *
         * \return the class tag.
         */
        virtual char dataTagInternal() const;

        /**
         * Returns the representative of the equivalence class containing
         * the given pentachoron triangle.  The class representative is
         * defined to be the root of the corresponding union-find tree.
         *
         * See the PentTriangleState class for further details.  See also
         * the other variant of findTriangleClass(), which is slower
         * but which also tracks triangle rotations and reflections.
         *
         * \param triID the index of a single pentachoron triangle; this
         * must be between 0 and 10p-1 inclusive, where \a p is the
         * number of pentachora.  See the PentTriangleState class notes for
         * details on triangle indexing.
         * \return the index of the pentachoron triangle at the root of the
         * union-find tree, i.e., the representative of the equivalence
         * class.
         */
        size_t findTriangleClass(size_t triID) const;

        /**
         * Returns the representative of the equivalence class containing
         * the given pentachoron triangle.  The class representative is
         * defined to be the root of the corresponding union-find tree.
         *
         * The argument \a twist is also modified to indicate what
         * rotation or reflection is used to identify vertices (0,1,2)
         * of the given triangle with vertices (0,1,2) of the class
         * representative.  Note that this argument is _not_ initialised.
         * Instead, the original \a twist will be multiplied on the left
         * by the mapping described above.
         *
         * See the PentTriangleState class for further details.  See also
         * the other variant of findTriangleClass(), which is faster
         * but which does not track triangle rotations and reflections.
         *
         * \param triID the index of a single pentachoron triangle; this
         * must be between 0 and 10p-1 inclusive, where \a p is the
         * number of pentachora.  See the PentTriangleState class notes for
         * details on triangle indexing.
         * \param twist used to track triangle rotations and reflections, as
         * described above.  This must be a mapping from (0,1,2) to (0,1,2)
         * as it is passed into the function, and it will also be a mapping
         * from (0,1,2) to (0,1,2) upon returning from the function.
         * \return the index of the pentachoron triangle at the root of the
         * union-find tree, i.e., the representative of the equivalence
         * class.
         */
        size_t findTriangleClass(size_t triID, Perm<3>& twist) const;

        /**
         * Merges the classes of pentachoron edges as required by the
         * new gluing made at stage \a orderElt of the search.
         *
         * See the PentEdgeState class for details.
         *
         * This routine returns a boolean that indicates whether this
         * merge creates an invalid edge (i.e., an edge with identified
         * with itself in reverse, or whose link is something other than a
         * (possibly) punctured 2-sphere).
         *
         * \return \c true if this merge creates an invalid edge, or
         * \c false if not.
         */
        bool mergeEdgeClasses();

        /**
         * Merges the classes of pentachoron triangles as required by the
         * new gluing made at stage \a orderElt of the search.
         *
         * See the PentTriangleState class for details.
         *
         * This routine returns a boolean that indicates whether this
         * merge creates an invalid triangle (i.e., a triangle identified with
         * itself using a non-trivial rotation or reflection).
         *
         * \return \c true if this merge creates an invalid triangle, or
         * \c false if not.
         */
        bool mergeTriangleClasses();

        /**
         * Splits the classes of pentachoron edges to mirror the undoing
         * of the gluing at stage \a orderElt of the search.
         *
         * See the PentEdgeState class for details.
         */
        void splitEdgeClasses();

        /**
         * Splits the classes of pentachoron triangles to mirror the undoing
         * of the gluing at stage \a orderElt of the search.
         *
         * See the PentTriangleState class for details.
         */
        void splitTriangleClasses();

        /**
         * Signifies that the boundary edges supplied by the linking triangles
         * for the two given pentachoron edges should be marked as adjacent.
         * The \a bdryNext and \a bdryTwist arrays for each pentachoron edge
         * will be adjusted to point to the other.
         *
         * See the PentEdgeState class for details.
         *
         * \param edgeID the first pentachoron edge on which to operate;
         * this must be between 0 and 10n-1 inclusive, where \a n is the number
         * of pentachora.
         * \param end specifies in which direction the adjacent boundary
         * edges lie.  This must be either 0 or 1, and its value should
         * correspond to the relevant index in the \a bdryNext and \a bdryTwist
         * arrays for edge \a edgeID.
         * \param adjEdgeID the pentachoron edge whose boundary edges are
         * adjacent to the boundary edges supplied by \a edgeID; this must
         * be between 0 and 10n-1 inclusive, where \a n is the number of
         * pentachora.
         * \param twist 0 if the orientations of the two boundary segments of
         * edge link are oriented in the same direction, or 1 if they are
         * oriented in opposite directions; see the \a bdryTwist
         * documentation for details.
         */
        void edgeBdryJoin(size_t edgeID, char end, size_t adjEdgeID,
            char twist);

        /**
         * Adjusts the \a bdryNext and \a bdryTwist arrays for
         * nearby pentachoron edges, to ensure that these arrays
         * are consistent with the \a bdryNext and \a bdryTwist arrays
         * stored with the given pentachoron edge.
         *
         * It is assumed that the linking triangle for the given
         * pentachoron edge contributes at least one boundary edge to the
         * 4-manifold edge link.  Recall from the PentEdgeState class notes
         * that the \a bdryNext and \a bdryTwist arrays for the given
         * pentachoron edge describe the boundary edges that follow on in
         * either direction from the boundary edges supplied by this triangle.
         *
         * This routine locates the pentachoron edges that provide the
         * neighbouring boundary edges of the link, and adjusts the \a bdryNext
         * and \a bdryTwist arrays for these neighbouring pentachoron edges to
         * point back to the given pentachoron edge.
         *
         * This routine is intended to assist with backtracking.  This
         * routine is safe to use if the given pentachoron edge points
         * to itself (i.e., it provides a complete boundary cycle of
         * three edges in the 4-manifold edge link).
         *
         * See the PentEdgeState class for further information.
         *
         * \pre The linking triangle for the given pentachoron edge
         * contributes at least one boundary edge to the 4-manifold edge link.
         *
         * \param edgeID the pentachoron edge to examine; this must
         * be between 0 and 10n-1 inclusive, where \a n is the number of
         * pentachora.
         */
        void edgeBdryFixAdj(size_t edgeID);

        /**
         * Copies the \a bdryNext and \a bdryTwist arrays to the
         * \a bdryNextOld and \a bdryTwistOld arrays for the given
         * pentachoron edge.
         *
         * See the PentEdgeState class for further information.
         *
         * \param edgeID the pentachoron edge on which to operate; this
         * must be between 0 and 10n-1 inclusive, where \a n is the number of
         * pentachora.
         */
        void edgeBdryBackup(size_t edgeID);

        /**
         * Copies the \a bdryNextOld and \a bdryTwistOld arrays to the
         * \a bdryNext and \a bdryTwist arrays for the given pentachoron
         * edge.
         *
         * See the PentEdgeState class for further information.
         *
         * \param edgeID the pentachoron edge on which to operate; this
         * must be between 0 and 10n-1 inclusive, where \a n is the number of
         * pentachora.
         */
        void edgeBdryRestore(size_t edgeID);

        /**
         * Assuming the given edge of the linking triangle for the
         * given pentachoron edge lies on the boundary of the link,
         * this routine identifies the adjacent boundary edges of the
         * link in each direction.  The given edge of the linking
         * triangle must belong to one of the two pentachoron facets
         * currently being joined.
         *
         * The pentachoron edge to examine is passed in \a edgeID,
         * \a pent and \a edge, and the particular edge of the
         * linking triangle to examine is specified by \a bdryFacet.
         * Details of the adjacent boundary edges are returned in the
         * arrays \a next and \a twist.
         *
         * Note that the values returned might or might not correspond
         * to the \a bdryNext and \a bdryTwist arrays of the
         * PentEdgeState class, since the PentEdgeState arrays skip
         * over adjacent edges belonging to the same linking triangle.
         *
         * If the given edge of the linking triangle is not a
         * boundary edge of the 4-manifold edge link, the behaviour of this
         * routine is undefined.
         *
         * See the PentEdgeState class for further information.
         *
         * \pre The pentachoron facet (\a pent, \a bdryFacet) is one of the
         * two facets that are currently being joined together.  That is,
         * this facet is either order_[orderElt_] or its partner in the
         * underlying pentachoron facet pairing.
         *
         * \param edgeID the pentachoron edge to examine; this must
         * be between 0 and 10n-1 inclusive, where \a n is the number of
         * pentachora.
         * \param pent the pentachoron described by \a edgeID; this
         * must be (edgeID / 10).  It is passed separately to avoid a
         * slow division operation.
         * \param edge the pentachoron edge number described by \a edgeID;
         * this must be (edgeID % 10).  It is passed separately to
         * avoid a slow modulus operation.
         * \param bdryFacet the facet number of the given pentachoron
         * containing the edge of the linking triangle that is
         * under consideration.  This must be between 0 and 4 inclusive.
         * \param next returns the pentachoron edge supplying each adjacent
         * boundary edge of the link; see the PentEdgeState::bdryNext
         * notes for details on which directions correspond to array
         * indices 0 and 1.
         * \param twist returns whether the orientations of the adjacent
         * boundary edges are consistent with the orientation of this
         * boundary edge; see the PentEdgeState::bdryTwist notes for
         * further information on orientations in the link.
         */
        void edgeBdryNext(size_t edgeID, size_t pent, int edge, int bdryFacet,
            size_t next[2], char twist[2]);

        /**
         * Determines whether one of the edges of the linking triangle for
         * the given pentachoron edge in fact forms an entire one-edge
         * boundary component of the overall 4-manifold edge link.
         *
         * See the PentEdgeState class for further information.
         *
         * \param edgeID the pentachoron edge to examine; this must
         * be between 0 and 10n-1 inclusive, where \a n is the number of
         * pentachora.
         * \return \c true if a one-edge boundary component is formed as
         * described above, or \c false otherwise.
         */
        bool edgeBdryLength1(size_t edgeID);

        /**
         * Determines whether edges of the linking triangles for each
         * of the given pentachoron edges combine to form an entire
         * two-edge boundary component of the overall 4-manifold edge link,
         * with one edge from each triangle.
         *
         * See the PentEdgeState class for further information.
         *
         * \param edgeID1 the first pentachoron edge to examine; this
         * must be between 0 and 10n-1 inclusive, where \a n is the number of
         * pentachora.
         * \param edgeID2 the second pentachoron edge to examine; this
         * must be between 0 and 10n-1 inclusive, where \a n is the number of
         * pentachora.
         * \return \c true if a two-edge boundary component is formed as
         * described above, or \c false otherwise.
         */
        bool edgeBdryLength2(size_t edgeID1, size_t edgeID2);

        /**
         * Runs a number of tests on all pentachoron edges to locate
         * consistency errors in the \a bdryEdges, \a bdryNext and
         * \a bdryTwist members of the PentEdgeState class.
         *
         * Any errors that are identified will be written to standard error.
         * Note that some errors might be harmless (for instance, when
         * a call to mergeEdgeClasses() leaves processing incomplete
         * because it has located a bad edge link and expects the
         * merge to be immediately undone).
         */
        void edgeBdryConsistencyCheck();

        /**
         * Dumps a summary of \a bdryNext, \a bdryTwist and \a bdryEdges
         * for every edge of every pentachoron to the given output stream.
         * The output format is relatively compact, and is subject to change
         * in future versions of Regina.  The output uses one line only, and
         * a final newline is written.
         *
         * See the PentEdgeState class for further information.
         *
         * \param out the output stream to which to write.
         */
        void edgeBdryDump(std::ostream& out);
};

// Inline functions for GluingPermSearcher<4>

template <typename Action, typename... Args>
inline void GluingPermSearcher<4>::runSearch(Action&& action, Args&&... args) {
    // Delegate to a de-templatised function.
    searchImpl(-1, ActionWrapper([&](const regina::GluingPerms<4>& p) {
        action(p, std::forward<Args>(args)...);
    }));
}

template <typename Action, typename... Args>
inline void GluingPermSearcher<4>::partialSearch(long maxDepth,
        Action&& action, Args&&... args) {
    // Delegate to a de-templatised function.
    searchImpl(maxDepth, ActionWrapper([&](const regina::GluingPerms<4>& p) {
        action(p, std::forward<Args>(args)...);
    }));
}

inline GluingPermSearcher<4>::PentEdgeState::PentEdgeState() :
        parent(-1), rank(0), bdry(3),
        twistUpEdge(0), twistUpTriangle(0), hadEqualRank(false) {
}

inline GluingPermSearcher<4>::PentTriangleState::PentTriangleState() :
        parent(-1), rank(0), size(1), bounded(true), twistUp() /* ID */,
        hadEqualRank(false) {
}

inline bool GluingPermSearcher<4>::isComplete() const {
    return (orderElt_ == static_cast<ssize_t>(orderSize_));
}

inline void GluingPermSearcher<4>::dumpTaggedData(std::ostream& out) const {
    out << dataTagInternal() << std::endl;
    dumpData(out);
}

inline std::string GluingPermSearcher<4>::taggedData() const {
    std::ostringstream out;
    dumpTaggedData(out);
    return out.str();
}

inline std::string GluingPermSearcher<4>::data() const {
    std::ostringstream out;
    dumpData(out);
    return out.str();
}

inline char GluingPermSearcher<4>::dataTagInternal() const {
    return GluingPermSearcher<4>::dataTag;
}

inline size_t GluingPermSearcher<4>::findTriangleClass(size_t triID) const {
    while (triState_[triID].parent >= 0)
        triID = triState_[triID].parent;

    return triID;
}

inline size_t GluingPermSearcher<4>::findTriangleClass(size_t triID, Perm<3>& twist)
        const {
    for ( ; triState_[triID].parent >= 0; triID = triState_[triID].parent)
        twist = triState_[triID].twistUp * twist;

    return triID;
}

inline void GluingPermSearcher<4>::edgeBdryJoin(size_t edgeID, char end,
        size_t adjEdgeID, char twist) {
    edgeState_[edgeID].bdryNext[end] = adjEdgeID;
    edgeState_[edgeID].bdryTwist[end] = twist;
    edgeState_[adjEdgeID].bdryNext[(end ^ 1) ^ twist] = edgeID;
    edgeState_[adjEdgeID].bdryTwist[(end ^ 1) ^ twist] = twist;
}

inline void GluingPermSearcher<4>::edgeBdryFixAdj(size_t edgeID) {
    if (edgeState_[edgeID].bdryNext[0] != edgeID) {
        edgeState_[edgeState_[edgeID].bdryNext[0]].
            bdryNext[1 ^ edgeState_[edgeID].bdryTwist[0]] = edgeID;
        edgeState_[edgeState_[edgeID].bdryNext[0]].
            bdryTwist[1 ^ edgeState_[edgeID].bdryTwist[0]] =
            edgeState_[edgeID].bdryTwist[0];
        edgeState_[edgeState_[edgeID].bdryNext[1]].
            bdryNext[0 ^ edgeState_[edgeID].bdryTwist[1]] = edgeID;
        edgeState_[edgeState_[edgeID].bdryNext[1]].
            bdryTwist[0 ^ edgeState_[edgeID].bdryTwist[1]] =
            edgeState_[edgeID].bdryTwist[1];
    }
}

inline void GluingPermSearcher<4>::edgeBdryBackup(size_t edgeID) {
    edgeState_[edgeID].bdryNextOld[0] = edgeState_[edgeID].bdryNext[0];
    edgeState_[edgeID].bdryNextOld[1] = edgeState_[edgeID].bdryNext[1];
    edgeState_[edgeID].bdryTwistOld[0] = edgeState_[edgeID].bdryTwist[0];
    edgeState_[edgeID].bdryTwistOld[1] = edgeState_[edgeID].bdryTwist[1];
}

inline void GluingPermSearcher<4>::edgeBdryRestore(size_t edgeID) {
    edgeState_[edgeID].bdryNext[0] = edgeState_[edgeID].bdryNextOld[0];
    edgeState_[edgeID].bdryNext[1] = edgeState_[edgeID].bdryNextOld[1];
    edgeState_[edgeID].bdryTwist[0] = edgeState_[edgeID].bdryTwistOld[0];
    edgeState_[edgeID].bdryTwist[1] = edgeState_[edgeID].bdryTwistOld[1];
}

inline bool GluingPermSearcher<4>::edgeBdryLength1(size_t edgeID) {
    return (edgeState_[edgeID].bdryNext[0] == edgeID &&
            edgeState_[edgeID].bdryEdges == 1);
}

inline bool GluingPermSearcher<4>::edgeBdryLength2(size_t edgeID1,
        size_t edgeID2) {
    return (edgeState_[edgeID1].bdryNext[0] == edgeID2 &&
            edgeState_[edgeID1].bdryNext[1] == edgeID2 &&
            edgeState_[edgeID1].bdryEdges == 1 &&
            edgeState_[edgeID2].bdryEdges == 1);
}

inline std::unique_ptr<GluingPermSearcher<4>>
        GluingPermSearcher<4>::bestSearcher(
        FacetPairing<4> pairing, FacetPairing<4>::IsoList autos,
        bool orientableOnly, bool finiteOnly) {
    // Do everything by brute force for now.
    // If we ever get to the point of choosing between different algorithms,
    // we should change findAllPerms() to call bestSearcher() also.
    return std::make_unique<GluingPermSearcher<4>>(std::move(pairing),
        std::move(autos), orientableOnly, finiteOnly);
}

template <typename Action, typename... Args>
inline void GluingPermSearcher<4>::findAllPerms(FacetPairing<4> pairing,
        FacetPairing<4>::IsoList autos, bool orientableOnly,
        bool finiteOnly, Action&& action, Args&&... args) {
    // We don't call bestSearcher() because at present there is only one
    // algorithm.  Just use it.
    GluingPermSearcher<4>(std::move(pairing), std::move(autos), orientableOnly,
        finiteOnly).
        runSearch(std::forward<Action>(action), std::forward<Args>(args)...);
}

inline std::unique_ptr<GluingPermSearcher<4>>
        GluingPermSearcher<4>::fromTaggedData(std::istream& in) {
    // Read the class marker.
    char c;
    in >> c;
    if (in.eof())
        throw InvalidInput("Missing class marker "
            "when reading tagged GluingPermSearcher<4> data");

    switch (c) {
        case GluingPermSearcher<4>::dataTag:
            return std::make_unique<GluingPermSearcher<4>>(in);
        default:
            throw InvalidInput("Invalid class marker "
                "when reading tagged GluingPermSearcher<4> data");
    }
}

inline std::unique_ptr<GluingPermSearcher<4>>
        GluingPermSearcher<4>::fromTaggedData(std::string data) {
    try {
        std::istringstream in(std::move(data));
        return fromTaggedData(in);
    } catch (const InvalidInput& exc) {
        throw InvalidArgument(exc.what());
    }
}

} // namespace regina

#endif

