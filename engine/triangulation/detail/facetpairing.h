
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#ifndef __FACETPAIRING_H_DETAIL
#ifndef __DOXYGEN
#define __FACETPAIRING_H_DETAIL
#endif

/*! \file triangulation/detail/facetpairing.h
 *  \brief Implementation details for dual graphs of <i>dim</i>-dimensional
 *  triangulations.
 */

#include <boost/noncopyable.hpp>
#include <iostream>
#include <list>
#include "regina-core.h"
#include "output.h"
#include "triangulation/facetspec.h"
#include "triangulation/forward.h"
#include "utilities/boolset.h"

namespace regina {
namespace detail {

/**
 * \weakgroup detail
 * @{
 */

/**
 * Provides core functionality for facet pairings (that is, dual graphs) of
 * <i>dim</i>-dimensional triangulations.
 *
 * Such a facet pairing is represented by the class FacetPairing<dim>,
 * which uses this as a base class.  End users should not need to refer
 * to FacetPairingBase directly.
 *
 * See the FacetPairing class notes for further information.
 *
 * \ifacespython This base class is not present, but the "end user"
 * class FacetPairing<dim> is.
 *
 * \tparam dim the dimension of the triangulation.
 * This must be between 2 and 15 inclusive.
 */
template <int dim>
class FacetPairingBase :
        public ShortOutput<FacetPairingBase<dim>>,
        public boost::noncopyable {
    static_assert(dim >= 2, "FacetPairing requires dimension >= 2.");

    public:
        /**
         * A list of isomorphisms on facet pairings.
         * Such an isomorphism can be used to convert one facet pairing
         * into another.
         *
         * This type is used to store all \e automorphisms of a facet pairing;
         * that is, all isomorphisms that map the facet pairing to itself.
         */
        typedef std::list<Isomorphism<dim>*> IsoList;

        /**
         * A routine that can do arbitrary processing upon a facet pairing
         * and its automorphisms.  Such routines are used to process
         * pairings that are found when running findAllPairings().
         *
         * The first parameter passed should be a facet pairing
         * (this should not be deallocated by this routine).
         * The second parameter should be a list of all automorphisms of
         * this pairing (this should not be deallocated either).
         * The third parameter may contain arbitrary data as passed to
         * findAllPairings().
         *
         * Note that the first two parameters passed might be \c null to
         * signal that facet pairing generation has finished.
         */
        typedef void (*Use)(const FacetPairing<dim>*, const IsoList*, void*);

    protected:
        size_t size_;
            /**< The number of simplices under consideration. */

        FacetSpec<dim>* pairs_;
            /**< The other facet to which each simplex facet is paired.
                 If a simplex facet is left unmatched, the corresponding
                 element of this array will be boundary (as returned by
                 FacetSpec<dim>::isBoundary()).
                 If the destination for a particular facet has not yet been
                 decided, the facet will be paired to itself. */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Creates a new facet pairing that is a clone of the given
         * facet pairing.
         *
         * @param cloneMe the facet pairing to clone.
         */
        FacetPairingBase(const FacetPairingBase& cloneMe);

        /**
         * Creates the facet pairing of given triangulation.  This
         * is the facet pairing that describes how the facets of
         * simplices in the given triangulation are joined together, as
         * described in the class notes.
         *
         * \pre The given triangulation is not empty.
         *
         * @param tri the triangulation whose facet pairing should
         * be constructed.
         */
        FacetPairingBase(const Triangulation<dim>& tri);

        /**
         * Deallocates any memory used by this structure.
         */
        ~FacetPairingBase();

        /*@}*/
        /**
         * \name Basic Queries
         */
        /*@{*/

        /**
         * Returns the number of simplices whose facets are described by
         * this facet pairing.
         *
         * @return the number of simplices under consideration.
         */
        size_t size() const;

        /**
         * Returns the other facet to which the given simplex facet is
         * paired.  If the given facet is left deliberately unmatched,
         * the value returned will be boundary (as returned by
         * FacetSpec<dim>::isBoundary()).
         *
         * \pre The given facet is a real simplex facet (not boundary,
         * before-the-start or past-the-end).
         *
         * @param source the facet under investigation.
         * @return the other facet to which the given facet is paired.
         */
        const FacetSpec<dim>& dest(const FacetSpec<dim>& source) const;

        /**
         * Returns the other facet to which the given simplex facet is
         * paired.  If the given facet is left deliberately unmatched,
         * the value returned will be boundary (as returned by
         * FacetSpec<dim>::isBoundary()).
         *
         * @param simp the simplex under investigation (this must be
         * strictly less than the total number of simplices under
         * consideration).
         * @param facet the facet of the given simplex under
         * investigation (between 0 and \a dim inclusive).
         * @return the other facet to which the given facet is paired.
         */
        const FacetSpec<dim>& dest(size_t simp, unsigned facet) const;

        /**
         * Returns the other facet to which the given simplex facet is
         * paired.  This is a convenience operator whose behaviour is
         * identical to that of dest(const FacetSpec<dim>&).
         *
         * If the given facet is left deliberately unmatched, the value
         * returned will be boundary (as returned by
         * FacetSpec<dim>::isBoundary()).
         *
         * \pre The given facet is a real simplex facet (not boundary,
         * before-the-start or past-the-end).
         *
         * @param source the facet under investigation.
         * @return the other facet to which the given facet is paired.
         */
        const FacetSpec<dim>& operator [](const FacetSpec<dim>& source) const;

        /**
         * Determines whether the given simplex facet has been left
         * deliberately unmatched.
         *
         * \pre The given facet is a real simplex facet (not boundary,
         * before-the-start or past-the-end).
         *
         * @param source the facet under investigation.
         * @return \c true if the given facet has been left unmatched, or
         * \c false if the given facet is paired with some other facet.
         */
        bool isUnmatched(const FacetSpec<dim>& source) const;

        /**
         * Determines whether the given simplex facet has been left
         * deliberately unmatched.
         *
         * @param simp the simplex under investigation (this must be
         * strictly less than the total number of simplices under
         * consideration).
         * @param facet the facet of the given simplex under
         * investigation (between 0 and \a dim inclusive).
         * @return \c true if the given facet has been left unmatched, or
         * \c false if the given facet is paired with some other facet.
         */
        bool isUnmatched(size_t simp, unsigned facet) const;

        /**
         * Determines whether this facet pairing is closed.
         * A closed facet pairing has no unmatched facets.
         */
        bool isClosed() const;

        /*@}*/
        /**
         * \name Isomorphic Representations
         */
        /*@{*/

        /**
         * Determines whether this facet pairing is in canonical form,
         * i.e., is a lexicographically minimal representative of its
         * isomorphism class.
         *
         * Isomorphisms of facet pairings correspond to relabellings of
         * simplices and relabellings of the (\a dim + 1) facets within each
         * simplex.
         *
         * Facet pairings are ordered by lexicographical comparison of
         * <tt>dest(0,0)</tt>, <tt>dest(0,1)</tt>, ...,
         * <tt>dest(size()-1,\a dim)</tt>.
         *
         * \pre This facet pairing is connected, i.e., it is possible
         * to reach any simplex from any other simplex via a
         * series of matched facet pairs.
         *
         * @return \c true if and only if this facet pairing is in
         * canonical form.
         */
        bool isCanonical() const;

        /**
         * Fills the given list with the set of all combinatorial
         * automorphisms of this facet pairing.
         *
         * An automorphism is a relabelling of the simplices and/or a
         * renumbering of the (\a dim + 1) facets of each simplex resulting
         * in precisely the same facet pairing.
         *
         * This routine uses optimisations that can cause unpredictable
         * breakages if this facet pairing is not in canonical form.
         *
         * The automorphisms placed in the given list will be newly
         * created; it is the responsibility of the caller of this
         * routine to deallocate them.
         *
         * \pre The given list is empty.
         * \pre This facet pairing is connected, i.e., it is possible
         * to reach any simplex from any other simplex via a
         * series of matched facet pairs.
         * \pre This facet pairing is in canonical form as described by
         * isCanonical().
         *
         * \ifacespython Not present, even in the dimension-specific
         * subclasses.
         *
         * @param list the list into which the newly created automorphisms
         * will be placed.
         */
        void findAutomorphisms(IsoList& list) const;

        /*@}*/
        /**
         * \name Input and Output
         */
        /*@{*/

        /**
         * Writes a human-readable representation of this facet pairing
         * to the given output stream.
         *
         * The string returned will contain no newlines.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Returns a text-based representation of this facet pairing that can be
         * used to reconstruct the facet pairing.  This reconstruction is
         * done through routine fromTextRep().
         *
         * The text produced is not particularly readable; for a
         * human-readable text representation, see routine str() instead.
         *
         * The string returned will contain no newlines.
         *
         * @return a text-based representation of this facet pairing.
         */
        std::string toTextRep() const;

        /**
         * Writes the graph corresponding to this facet pairing in
         * the Graphviz DOT language.  Every vertex of this graph
         * represents a simplex, and every edge represents a pair of
         * simplex facets that are joined together.  Note that for a
         * closed triangulation this graph will be entirely (\a dim + 1)-valent;
         * for triangulations with boundary facets, some graph vertices
         * will have degree \a dim or less.
         *
         * The graph can either be written as a complete DOT graph, or
         * as a clustered subgraph within some larger DOT graph
         * (according to whether the argument \a subgraph is passed as
         * \c false or \c true).
         *
         * If a complete DOT graph is being written, the output may be
         * used as a standalone DOT file ready for use with Graphviz.
         *
         * If a subgraph is being written, the output will contain a
         * single \c subgraph section that should be inserted into some
         * larger DOT file.  Note that the output generated by
         * writeDotHeader(), followed by one or more subgraphs and then
         * a closing curly brace will suffice.  The subgraph name will
         * begin with the string \c pairing_.
         *
         * The argument \a prefix will be prepended to the name of each
         * graph vertex, and will also be used in the name of the graph
         * or subgraph.  Using unique prefixes becomes important if you
         * are calling writeDot() several times to generate several
         * subgraphs for use in a single DOT file.  If the \a prefix
         * argument is null or empty then a default prefix will be used.
         *
         * Note that this routine generates undirected graphs, not
         * directed graphs.  The final DOT file should be used with
         * either the \a neato or \a fdp programs shipped with Graphviz.
         *
         * \ifacespython The \a out argument is not present; instead
         * standard output is assumed.
         *
         * @param out the output stream to which to write.
         * @param prefix a string to prepend to the name of each graph
         * vertex, and to include in the graph or subgraph name; see
         * above for details.
         * @param subgraph \c false if a complete standalone DOT graph
         * should be output, or \c true if a clustered subgraph should
         * be output for use in some larger DOT file.
         * @param labels indicates whether graph vertices will be
         * labelled with the corresponding simplex numbers.
         * This feature is currently experimental, and the default is
         * \c false.
         *
         * @see http://www.graphviz.org/
         */
        void writeDot(std::ostream& out, const char* prefix = 0,
            bool subgraph = false, bool labels = false) const;

        /**
         * Returns a Graphviz DOT representation of the graph that
         * describes this facet pairing.
         *
         * This routine simply returns the output of writeDot() as a
         * string, instead of dumping it to an output stream.
         *
         * All arguments are the same as for writeDot(); see the
         * writeDot() notes for further details.
         *
         * @return the output of writeDot(), as outlined above.
         */
        std::string dot(const char* prefix = 0, bool subgraph = false,
            bool labels = false) const;

        /*@}*/

        /**
         * Reconstructs a facet pairing from a text-based representation.
         * This text-based representation must be in the format produced
         * by routine toTextRep().
         *
         * The facet pairing returned will be newly constructed; it is the
         * responsibility of the caller of this routine to deallocate it.
         *
         * \pre The facet pairing to be reconstructed involves at least
         * one simplex.
         *
         * @param rep a text-based representation of a facet pairing, as
         * produced by routine toTextRep().
         * @return the corresponding newly constructed facet pairing, or
         * \c null if the given text-based representation was invalid.
         */
        static FacetPairing<dim>* fromTextRep(const std::string& rep);

        /**
         * Writes header information for a Graphviz DOT file that will
         * describe the graphs for one or more facet pairings.  See
         * the writeDot() documentation for further information on such
         * graphs.
         *
         * The output will be in the Graphviz DOT language, and will
         * include appropriate display settings for graphs, edges and
         * nodes.  The opening brace for a \c graph section of the DOT
         * file is included.
         *
         * This routine may be used with writeDot() to generate a single
         * DOT file containing the graphs for several different facet
         * pairings.  A complete DOT file can be produced by calling
         * this routine, then calling writeDot() in subgraph mode for
         * each facet pairing, then outputting a final closing curly brace.
         *
         * Note that if you require a DOT file containing the graph for
         * only a single facet pairing, this routine is unnecessary; you
         * may simply call writeDot() in full graph mode instead.
         *
         * This routine is suitable for generating undirected graphs, not
         * directed graphs.  The final DOT file should be used with
         * either the \a neato or \a fdp programs shipped with Graphviz.
         *
         * \ifacespython The \a out argument is not present; instead
         * standard output is assumed.
         *
         * @param out the output stream to which to write.
         * @param graphName the name of the graph in the DOT file.
         * If this is null or empty then a default graph name will be used.
         *
         * @see http://www.graphviz.org/
         */
        static void writeDotHeader(std::ostream& out,
            const char* graphName = 0);

        /**
         * Returns header information for a Graphviz DOT file that will
         * describe the graphs for one or more facet pairings.
         *
         * This routine simply returns the output of writeDotHeader() as
         * a string, instead of dumping it to an output stream.
         *
         * All arguments are the same as for writeDotHeader(); see the
         * writeDotHeader() notes for further details.
         *
         * @return the output of writeDotHeader(), as outlined above.
         */
        static std::string dotHeader(const char* graphName = 0);

        /**
         * Generates all possible facet pairings satisfying the given
         * constraints.  Only connected facet pairings (pairings in which
         * each simplex can be reached from each other via a series of
         * individual matched facets) will be produced.
         *
         * Each facet pairing will be produced precisely once up to
         * isomorphism.  Facet pairings are considered isomorphic if they
         * are related by a relabelling of the simplices and/or a
         * renumbering of the (\a dim + 1) facets of each simplex.  Each facet
         * pairing that is generated will be a lexicographically minimal
         * representative of its isomorphism class, i.e., will be in
         * canonical form as described by isCanonical().
         *
         * For each facet pairing that is generated, routine \a use (as
         * passed to this function) will be called with that pairing and
         * its automorphisms as arguments.
         *
         * Once the generation of facet pairings has finished, routine
         * \a use will be called once more, this time with \c null as its
         * first two arguments (for the facet pairing and its automorphisms).
         *
         * Because this class cannot represent an empty facet pairing,
         * if the argument \a nSimplices is zero then no facet pairings
         * will be generated at all.
         *
         * \todo \optlong When generating facet pairings, do some checking to
         * eliminate cases in which simplex (\a k > 0) can be swapped
         * with simplex 0 to produce a smaller representation of the same
         * pairing.
         * \todo \feature Allow cancellation of facet pairing generation.
         *
         * \ifacespython Not present, even in the dimension-specific
         * subclasses.
         *
         * @param nSimplices the number of simplices whose facets should
         * be (potentially) matched.
         * @param boundary determines whether any facets may be left
         * unmatched.  This set should contain \c true if pairings with at
         * least one unmatched facet are to be generated, and should contain
         * \c false if pairings with no unmatched facets are to be generated.
         * @param nBdryFacets specifies the precise number of facets that
         * should be left unmatched.  If this parameter is negative, it
         * is ignored and no additional restriction is imposed.  If
         * parameter \a boundary does not contain \c true, this parameter
         * is likewise ignored.  If parameter \a boundary does contain
         * true and this parameter is non-negative, only pairings with
         * precisely this many unmatched facets will be generated.
         * In particular, if this parameter is positive then pairings
         * with no unmatched facets will not be produced irrespective of
         * whether \c false is contained in parameter \a boundary.
         * Note that, in order to produce any pairings at all, this parameter
         * must be of the same parity as <tt>nSimplices * (dim+1)</tt>,
         * and can be at most <tt>(dim-1) * nSimplices + 2</tt>.
         * @param use the function to call upon each facet pairing that is
         * found.  The first parameter passed to this function will be a
         * facet pairing.  The second parameter will be a list of all its
         * automorphisms (relabellings of simplices and individual
         * simplex facets that produce the exact same pairing).
         * The third parameter will be parameter \a useArgs as was passed
         * to this routine.
         * @param useArgs the pointer to pass as the final parameter for
         * the function \a use which will be called upon each pairing found.
         */
        static void findAllPairings(size_t nSimplices,
            BoolSet boundary, int nBdryFacets, Use use, void* useArgs = 0);

    protected:
        /**
         * Creates a new facet pairing.  All internal arrays will be
         * allocated but not initialised.
         *
         * \pre \a size is at least 1.
         *
         * @param size the number of simplices under
         * consideration in this new facet pairing.
         */
        FacetPairingBase(size_t size);

        /**
         * Returns the other facet to which the given simplex facet is
         * paired.  If the given facet is left deliberately unmatched, the
         * value returned will be boundary (as returned by
         * FacetSpec<dim>::isBoundary()).
         *
         * \pre The given facet is a real simplex facet (not boundary,
         * before-the-start or past-the-end).
         *
         * @param source the facet under investigation.
         * @return the other facet to which the given facet is paired.
         */
        FacetSpec<dim>& dest(const FacetSpec<dim>& source);

        /**
         * Returns the other facet to which the given simplex facet is
         * paired.  If the given facet is left deliberately unmatched, the
         * value returned will be boundary (as returned by
         * FacetSpec<dim>::isBoundary()).
         *
         * @param simp the simplex under investigation (this must be
         * strictly less than the total number of simplices under
         * consideration).
         * @param facet the facet of the given simplex under
         * investigation (between 0 and \a dim inclusive).
         * @return the other facet to which the given facet is paired.
         */
        FacetSpec<dim>& dest(size_t simp, unsigned facet);

        /**
         * Returns the other facet to which the given simplex facet is
         * paired.  This is a convenience operator whose behaviour is
         * identical to that of dest(const FacetSpec<dim>&).
         *
         * If the given facet is left deliberately unmatched, the value
         * returned will be boundary (as returned by
         * FacetSpec<dim>::isBoundary()).
         *
         * \pre The given facet is a real simplex facet (not boundary,
         * before-the-start or past-the-end).
         *
         * @param source the facet under investigation.
         * @return the other facet to which the given facet is paired.
         */
        FacetSpec<dim>& operator [](const FacetSpec<dim>& source);

        /**
         * Determines whether the matching for the given simplex facet
         * has not yet been determined.  This is signalled by a facet
         * matched to itself.
         *
         * \pre The given facet is a real simplex facet (not boundary,
         * before-the-start or past-the-end).
         *
         * @param source the facet under investigation.
         * @return \c true if the matching for the given facet has not yet
         * been determined, or \c false otherwise.
         */
        bool noDest(const FacetSpec<dim>& source) const;

        /**
         * Determines whether the matching for the given simplex facet
         * has not yet been determined.  This is signalled by a facet
         * matched to itself.
         *
         * @param simp the simplex under investigation (this must be
         * strictly less than the total number of simplices under
         * consideration).
         * @param facet the facet of the given simplex under
         * investigation (between 0 and \a dim inclusive).
         * @return \c true if the matching for the given facet has not yet
         * been determined, or \c false otherwise.
         */
        bool noDest(size_t simp, unsigned facet) const;

        /**
         * Determines whether this facet pairing is in canonical
         * (smallest lexicographical) form, given a small set of
         * assumptions.
         *
         * If this facet pairing is in canonical form, the given list
         * will be filled with the set of all combinatorial automorphisms
         * of this facet pairing.  If not, the given list will be left empty.
         *
         * \pre The given list is empty.
         * \pre For each simplex \a t, the only case in which
         * <tt>dest(t,i)</tt> is greater than <tt>dest(t,i+1)</tt> is where
         * facets <tt>(t,i)</tt> and <tt>(t,i+1)</tt> are paired together.
         * \pre For each simplex \a t > 0, it is true that
         * <tt>dest(t,0).simp < t</tt>.
         * \pre The sequence <tt>dest(1,0)</tt>, <tt>dest(2,0)</tt>,
         * ..., <tt>dest(n-1,0)</tt> is strictly increasing, where
         * \a n is the total number of simplices under investigation.
         *
         * @param list the list into which automorphisms will be placed
         * if appropriate.
         * @return \c true if and only if this facet pairing is in
         * canonical form.
         */
        bool isCanonicalInternal(IsoList& list) const;

    private:
        /**
         * Internal to findAllPairings().
         *
         * Performs the actual enumeration of facet pairings.  At most one
         * copy of this routine should be running at any given time for a
         * particular FacetPairingBase instance.
         *
         * \pre This object is known to be of the dimension-specific subclass
         * FacetPairing<dim>, not an instance of the parent class
         * FacetPairingBase<dim>.
         */
        void enumerateInternal(BoolSet boundary, int nBdryFacets,
            Use use, void* useArgs);
};

/*@}*/

// Inline functions for FacetPairingBase

template <int dim>
inline FacetPairingBase<dim>::FacetPairingBase(size_t size) :
        size_(size), pairs_(new FacetSpec<dim>[size * (dim + 1)]) {
}

template <int dim>
inline FacetPairingBase<dim>::~FacetPairingBase() {
    delete[] pairs_;
}

template <int dim>
inline size_t FacetPairingBase<dim>::size() const {
    return size_;
}

template <int dim>
inline const FacetSpec<dim>& FacetPairingBase<dim>::dest(
        const FacetSpec<dim>& source) const {
    return pairs_[(dim + 1) * source.simp + source.facet];
}

template <int dim>
inline const FacetSpec<dim>& FacetPairingBase<dim>::dest(
        size_t simp, unsigned facet) const {
    return pairs_[(dim + 1) * simp + facet];
}

template <int dim>
inline const FacetSpec<dim>& FacetPairingBase<dim>::operator [](
        const FacetSpec<dim>& source) const {
    return pairs_[(dim + 1) * source.simp + source.facet];
}

template <int dim>
inline bool FacetPairingBase<dim>::isUnmatched(
        const FacetSpec<dim>& source) const {
    return pairs_[(dim + 1) * source.simp + source.facet].isBoundary(size_);
}

template <int dim>
inline bool FacetPairingBase<dim>::isUnmatched(
        size_t simp, unsigned facet) const {
    return pairs_[(dim + 1) * simp + facet].isBoundary(size_);
}

template <int dim>
inline FacetSpec<dim>& FacetPairingBase<dim>::dest(
        const FacetSpec<dim>& source) {
    return pairs_[(dim + 1) * source.simp + source.facet];
}

template <int dim>
inline FacetSpec<dim>& FacetPairingBase<dim>::dest(
        size_t simp, unsigned facet) {
    return pairs_[(dim + 1) * simp + facet];
}

template <int dim>
inline FacetSpec<dim>& FacetPairingBase<dim>::operator [](
        const FacetSpec<dim>& source) {
    return pairs_[(dim + 1) * source.simp + source.facet];
}

template <int dim>
inline bool FacetPairingBase<dim>::noDest(
        const FacetSpec<dim>& source) const {
    return dest(source) == source;
}

template <int dim>
inline bool FacetPairingBase<dim>::noDest(
        size_t simp, unsigned facet) const {
    FacetSpec<dim>& f = pairs_[(dim + 1) * simp + facet];
    return (f.simp == static_cast<int>(simp) &&
        f.facet == static_cast<int>(facet));
}

template <int dim>
inline void FacetPairingBase<dim>::findAutomorphisms(
        typename FacetPairingBase<dim>::IsoList& list) const {
    isCanonicalInternal(list);
}

template <int dim>
inline void FacetPairingBase<dim>::findAllPairings(size_t nSimplices,
        BoolSet boundary, int nBdryFacets,
        typename FacetPairingBase<dim>::Use use, void* useArgs) {
    FacetPairing<dim> pairing(nSimplices);
    pairing.enumerateInternal(boundary, nBdryFacets, use, useArgs);
}

} } // namespace regina::detail

#include "triangulation/detail/facetpairing-impl.h"

#endif

