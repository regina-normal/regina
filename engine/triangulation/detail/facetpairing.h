
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

#ifndef __REGINA_FACETPAIRING_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_FACETPAIRING_H_DETAIL
#endif

/*! \file triangulation/detail/facetpairing.h
 *  \brief Implementation details for dual graphs of <i>dim</i>-dimensional
 *  triangulations.
 */

#include <iostream>
#include <optional>
#include <vector>
#include "regina-core.h"
#include "core/output.h"
#include "triangulation/cut.h"
#include "triangulation/facetspec.h"
#include "triangulation/forward.h"
#include "utilities/boolset.h"
#include "utilities/tightencoding.h"

namespace regina {
namespace detail {

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
 * Both this class and the "end user" class FacetPairing<dim> implement
 * C++ move semantics, and FacetPairing<dim> also adheres to the C++ Swappable
 * requirement.  These classes are designed to avoid deep copies wherever
 * possible, even when passing or returning objects by value.
 *
 * \python This base class is not present, but the "end user"
 * class FacetPairing<dim> is.
 *
 * \tparam dim the dimension of the triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \ingroup detail
 */
template <int dim>
class FacetPairingBase :
        public ShortOutput<FacetPairingBase<dim>>,
        public TightEncodable<FacetPairing<dim>> {
    static_assert(dim >= 2, "FacetPairing requires dimension >= 2.");

    public:
        /**
         * A list of isomorphisms on facet pairings.
         *
         * In particular, this class uses the IsoList type to return
         * the set of all _automorphisms_ of a facet pairing.
         */
        using IsoList = std::vector<Isomorphism<dim>>;

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
         * \name Constructors, Destructors and Assignment
         */
        /*@{*/

        /**
         * Creates a new copy of the given facet pairing.
         *
         * \param src the facet pairing to clone.
         */
        FacetPairingBase(const FacetPairingBase& src);

        /**
         * Moves the given facet pairing into this facet pairing.
         * This is a fast (constant time) operation.
         *
         * The facet pairing that is passed (\a src) will no longer be usable.
         *
         * \param src the facet pairing to move.
         */
        FacetPairingBase(FacetPairingBase&& src) noexcept;

        /**
         * Creates the facet pairing of given triangulation.  This
         * is the facet pairing that describes how the facets of
         * simplices in the given triangulation are joined together, as
         * described in the class notes.
         *
         * \pre The given triangulation is not empty.
         *
         * \param tri the triangulation whose facet pairing should
         * be constructed.
         */
        FacetPairingBase(const Triangulation<dim>& tri);

        /**
         * Reads a new facet pairing from the given input stream.  This
         * routine reads data in the format written by textRep().
         *
         * This routine will skip any initial whitespace in the given input
         * stream.  Once it finds its first non-whitespace character,
         * it will read the _entire_ line from the input stream and expect
         * that line to containin the text representation of a facet pairing.
         *
         * \exception InvalidInput The data found in the input stream is
         * invalid, incomplete, or incorrectly formatted.
         *
         * \param in the input stream from which to read.
         */
        FacetPairingBase(std::istream& in);

        /**
         * Deallocates any memory used by this structure.
         */
        ~FacetPairingBase();

        /**
         * Copies the given facet pairing into this facet pairing.
         *
         * It does not matter if this and the given facet pairing use
         * different numbers of top-dimensional simpilices; if they do
         * then this facet pairing will be resized accordingly.
         *
         * This operator induces a deep copy of \a src.
         *
         * \param src the facet pairing to copy.
         * \return a reference to this facet pairing.
         */
        FacetPairingBase& operator = (const FacetPairingBase& src);

        /**
         * Moves the given facet pairing into this facet pairing.
         * This is a fast (constant time) operation.
         *
         * It does not matter if this and the given facet pairing use
         * different numbers of top-dimensional simpilices; if they do
         * then this facet pairing will be resized accordingly.
         *
         * The facet pairing that is passed (\a src) will no longer be usable.
         *
         * \param src the facet pairing to move.
         * \return a reference to this facet pairing.
         */
        FacetPairingBase& operator = (FacetPairingBase&& src) noexcept;

        /**
         * Swaps the contents of this and the given facet pairing.
         *
         * \param other the facet pairing whose contents are to be
         * swapped with this.
         */
        void swap(FacetPairingBase& other) noexcept;

        /*@}*/
        /**
         * \name Basic Queries
         */
        /*@{*/

        /**
         * Returns the number of simplices whose facets are described by
         * this facet pairing.
         *
         * \python This is also used to implement the Python special
         * method __len__().
         *
         * \return the number of simplices under consideration.
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
         * \python This routine returns by value, not by reference,
         * since Python cannot enforce constness otherwise.
         *
         * \param source the facet under investigation.
         * \return the other facet to which the given facet is paired.
         */
        const FacetSpec<dim>& dest(const FacetSpec<dim>& source) const;

        /**
         * Returns the other facet to which the given simplex facet is
         * paired.  If the given facet is left deliberately unmatched,
         * the value returned will be boundary (as returned by
         * FacetSpec<dim>::isBoundary()).
         *
         * \python This routine returns by value, not by reference,
         * since Python cannot enforce constness otherwise.
         *
         * \param simp the simplex under investigation (this must be
         * strictly less than the total number of simplices under
         * consideration).
         * \param facet the facet of the given simplex under
         * investigation (between 0 and \a dim inclusive).
         * \return the other facet to which the given facet is paired.
         */
        const FacetSpec<dim>& dest(size_t simp, int facet) const;

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
         * \python This routine returns by value, not by reference,
         * since Python cannot enforce constness otherwise.
         *
         * \param source the facet under investigation.
         * \return the other facet to which the given facet is paired.
         */
        const FacetSpec<dim>& operator [](const FacetSpec<dim>& source) const;

        /**
         * Determines whether the given simplex facet has been left
         * deliberately unmatched.
         *
         * \pre The given facet is a real simplex facet (not boundary,
         * before-the-start or past-the-end).
         *
         * \param source the facet under investigation.
         * \return \c true if the given facet has been left unmatched, or
         * \c false if the given facet is paired with some other facet.
         */
        bool isUnmatched(const FacetSpec<dim>& source) const;

        /**
         * Determines whether the given simplex facet has been left
         * deliberately unmatched.
         *
         * \param simp the simplex under investigation (this must be
         * strictly less than the total number of simplices under
         * consideration).
         * \param facet the facet of the given simplex under
         * investigation (between 0 and \a dim inclusive).
         * \return \c true if the given facet has been left unmatched, or
         * \c false if the given facet is paired with some other facet.
         */
        bool isUnmatched(size_t simp, int facet) const;

        /**
         * Determines whether this facet pairing is closed.
         * A closed facet pairing has no unmatched facets.
         */
        bool isClosed() const;

        /**
         * Determines whether the underlying graph for this facet pairing
         * contains an edge of multiplicity \a k.
         *
         * This corresponds to two distinct top-dimensional simplices
         * joined to each other along \a k of their facets.
         *
         * Note that this routine does not consider loops in the underlying
         * graph, only edges between distinct nodes.
         *
         * This routine is the fastest way to detect edges of multiplicity \a k
         * if you know \a k at compile time.
         *
         * \nopython Instead use the variant `hasMultiEdge(k)`.
         *
         * \tparam k the multiplicity of edges to search for; this must be
         * between 2 and `dim+1` inclusive.
         *
         * \return \c true if and only if the underyling graph has an edge of
         * multiplicity \a k.
         */
        template <int k>
        bool hasMultiEdge() const;

        /**
         * Determines whether the underlying graph for this facet pairing
         * contains an edge of multiplicity \a k, where \a k does not need to
         * be known until runtime.
         *
         * An edge of multiplicity \a k corresponds to two distinct
         * top-dimensional simplices joined to each other along \a k of their
         * facets.
         *
         * Note that this routine does not consider loops in the underlying
         * graph, only edges between distinct nodes.
         *
         * For C++ programmers who know \a k at compile time, it is faster to
         * call the template function `hasMultiEdge<k>()` instead.
         *
         * \exception InvalidArgument The argument \a k is outside the
         * supported range.
         *
         * \param k the multiplicity of edges to search for; this must be
         * between 2 and `dim+1` inclusive.
         * \return \c true if and only if the underyling graph has an edge of
         * multiplicity \a k.
         */
        bool hasMultiEdge(int k) const;

        /**
         * Determines if this and the given facet pairing are identical.
         *
         * \param other the facet pairing to compare with this.
         * \return \c true if and only if this and the given facet pairing
         * are identical.
         */
        bool operator == (const FacetPairingBase<dim>& other) const;

        /*@}*/
        /**
         * \name Connected components
         */
        /*@{*/

        /**
         * Determines whether this facet pairing is connected.
         *
         * A facet pairing is _connected_ if it is possible to reach any
         * simplex from any other simplex via a series of matched facet pairs.
         *
         * For this purpose, the empty facet pairing is considered to be
         * connected.
         *
         * \return \c true if and only if this pairing is connected.
         */
        bool isConnected() const;

        /**
         * Returns a cut that divides this facet pairing into two
         * connected pieces, both of size at least \a minSide.
         *
         * If solutions exist, then the cut that is returned will have
         * minimum weight amongst all solutions (i.e., will have the
         * smallest number of matched simplex facets that cross the two
         * sides of the resulting partition).  If there are still multiple
         * solutions, then the cut that is returned will have the two pieces
         * with sizes that are as close as possible to equal.  If there are
         * _still_ multiple solutions, then the choice will be arbitrary.
         *
         * Note that it is possible that no solution exists (e.g. this
         * could happen if the matching is a star graph and \a minSide is
         * greater than 1).
         *
         * \warning Currently the implementation of this routine is
         * exhaustive, and so the running time is exponential in the
         * size of this facet pairing.
         *
         * \param minSide the minimum number of simplices in each of the
         * two connected pieces; this must be at least 1.
         * \return the best possible cut as described above, or \nullopt
         * if no such cut exists.
         */
        std::optional<Cut> divideConnected(size_t minSide) const;

        /*@}*/
        /**
         * \name Isomorphic Representations
         */
        /*@{*/

        /**
         * Determines whether this facet pairing is in canonical form.
         *
         * See the FacetPairing class notes for more information on
         * isomorphisms, automorphisms and canonical form.
         *
         * \pre This facet pairing is connected, i.e., it is possible
         * to reach any simplex from any other simplex via a
         * series of matched facet pairs.
         *
         * \return \c true if and only if this facet pairing is in
         * canonical form.
         */
        bool isCanonical() const;

        /**
         * Returns the canonical form of this facet pairing, along with one
         * isomorphism that transforms this pairing into canonial form.
         *
         * Note that, while the canoncial form is uniquely determined,
         * the isomorphism is not (since the facet pairing could have
         * non-trivial automorphisms).  If you need _all_ such isomorphisms
         * then you should call canonicalAll() instead.
         *
         * See the FacetPairing class notes for more information on
         * isomorphisms, automorphisms and canonical form.
         *
         * \pre This facet pairing is connected, i.e., it is possible
         * to reach any simplex from any other simplex via a
         * series of matched facet pairs.
         *
         * \return a pair (\a c, \a iso), where \a c is the canonical form
         * and \a iso is one isomorphism that converts this facet pairing
         * into \a c.
         */
        std::pair<FacetPairing<dim>, Isomorphism<dim>> canonical() const;

        /**
         * Returns the canonical form of this facet pairing, along with
         * the list of all isomorphisms that transform this pairing into
         * canonial form.
         *
         * Note that the list that is returned will be a left coset of
         * the automorphism group of this facet pairing, and also a
         * right coset of the automorphism group of the canonical form.
         *
         * If you only need one such isomorphism (not all), then you
         * should call canonical() instead.
         *
         * See the FacetPairing class notes for more information on
         * isomorphisms, automorphisms and canonical form.
         *
         * \pre This facet pairing is connected, i.e., it is possible
         * to reach any simplex from any other simplex via a
         * series of matched facet pairs.
         *
         * \return a pair (\a c, \a isos), where \a c is the canonical form
         * and \a isos is the list of all isomorphisms that convert this
         * facet pairing into \a c.
         */
        std::pair<FacetPairing<dim>, IsoList> canonicalAll() const;

        /**
         * Returns the set of all combinatorial automorphisms of this
         * facet pairing, assuming the pairing is already in canonical form.
         *
         * See the FacetPairing class notes for more information on
         * isomorphisms, automorphisms and canonical form.
         *
         * \pre This facet pairing is connected, i.e., it is possible
         * to reach any simplex from any other simplex via a
         * series of matched facet pairs.
         *
         * \pre This facet pairing is in canonical form.  This is crucial,
         * since this routine uses optimisations that can cause unpredictable
         * breakages if this facet pairing is not in canonical form.
         *
         * \return the list of all automorphisms.
         */
        IsoList findAutomorphisms() const;

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
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Returns a text-based representation that can be used to reconstruct
         * this facet pairing.  This reconstruction is done through the
         * routine fromTextRep().
         *
         * The text produced is not particularly readable; for a
         * human-readable text representation, see routine str() instead.
         *
         * The string returned will contain no newlines.
         *
         * \return a text-based representation of this facet pairing.
         */
        std::string textRep() const;

        /**
         * Deprecated routine that returns a text-based representation
         * that can be used to reconstruct this facet pairing.
         *
         * \deprecated This routine has been renamed to textRep().
         * See the textRep() documentation for further details.
         *
         * \return a text-based representation of this facet pairing.
         */
        [[deprecated]] std::string toTextRep() const;

        /**
         * Writes the tight encoding of this facet pairing to the given output
         * stream.  See the page on \ref tight "tight encodings" for details.
         *
         * \pre Every simplex facet is either (i) paired to another simplex
         * facet, (ii) marked as boundary, or (iii) paired to itself (which is
         * often used as a placeholder to indicate that the real destination
         * has not yet been decided).  In particular, before-the-start or
         * past-the-end destinations are not allowed.
         *
         * \exception FailedPrecondition Some simplex facet has a destination
         * that is explicitly disallowed by the precondition above.
         *
         * \nopython Use tightEncoding() instead, which returns a string.
         *
         * \param out the output stream to which the encoded string will
         * be written.
         */
        void tightEncode(std::ostream& out) const;

        /**
         * Writes the graph corresponding to this facet pairing in
         * the Graphviz DOT language.
         *
         * See dot() for further details on what this output contains.
         *
         * This routine is equivalent to (but faster than) writing the
         * string returned by dot() to the given output stream.
         *
         * \nopython Use dot() instead, which returns a string.
         *
         * \param out the output stream to which to write.
         * \param prefix a string to prepend to the name of each graph
         * vertex, and to include in the graph or subgraph name; see
         * dot() for details.
         * \param subgraph \c false if a complete standalone DOT graph
         * should be output, or \c true if a clustered subgraph should
         * be output for use in some larger DOT file.
         * \param labels indicates whether graph vertices will be labelled
         * with the corresponding simplex numbers.
         *
         * \see http://www.graphviz.org/
         */
        void writeDot(std::ostream& out, const char* prefix = nullptr,
            bool subgraph = false, bool labels = false) const;

        /**
         * Returns a Graphviz DOT representation of the graph that
         * describes this facet pairing.
         *
         * Every vertex of this graph represents a simplex, and every edge
         * represents a pair of simplex facets that are joined together.
         * Note that for a closed triangulation this graph will be entirely
         * (\a dim + 1)-valent; for triangulations with boundary facets,
         * some graph vertices will have degree \a dim or less.
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
         * larger DOT file.  Note that the output generated by dotHeader() or
         * writeDotHeader(), followed by one or more subgraphs and then
         * a closing curly brace will suffice.  The subgraph name will
         * begin with the string \c pairing_.
         *
         * The argument \a prefix will be prepended to the name of each
         * graph vertex, and will also be used in the name of the graph
         * or subgraph.  Using unique prefixes becomes important if you
         * are calling dot() or writeDot() several times to generate several
         * subgraphs for use in a single DOT file.  If the \a prefix
         * argument is null or empty then a default prefix will be used.
         *
         * Note that this routine generates undirected graphs, not
         * directed graphs.  The final DOT file should be used with
         * either the \a neato or \a fdp programs shipped with Graphviz.
         *
         * If you are simply writing this string to an output stream then you
         * should call writeDot() instead, which is more efficient.
         *
         * \param prefix a string to prepend to the name of each graph
         * vertex, and to include in the graph or subgraph name; see
         * above for details.
         * \param subgraph \c false if a complete standalone DOT graph
         * should be output, or \c true if a clustered subgraph should
         * be output for use in some larger DOT file.
         * \param labels indicates whether graph vertices will be labelled
         * with the corresponding simplex numbers.
         * \return the output of writeDot(), as outlined above.
         */
        std::string dot(const char* prefix = nullptr, bool subgraph = false,
            bool labels = false) const;

        /*@}*/

        /**
         * Reconstructs a facet pairing from a text-based representation.
         * This text-based representation must be in the format produced
         * by routine textRep().
         *
         * \exception InvalidArgument The given string was not a valid
         * text-based representation of a facet pairing on a positive
         * number of simplices.
         *
         * \param rep a text-based representation of a facet pairing, as
         * produced by routine textRep().
         * \return the corresponding facet pairing.
         */
        static FacetPairing<dim> fromTextRep(const std::string& rep);

        /**
         * Reconstructs a facet pairing from its given tight encoding.
         * See the page on \ref tight "tight encodings" for details.
         *
         * The tight encoding will be read from the given input stream.
         * If the input stream contains leading whitespace then it will be
         * treated as an invalid encoding (i.e., this routine will throw an
         * exception).  The input stream _may_ contain further data: if this
         * routine is successful then the input stream will be left positioned
         * immediately after the encoding, without skipping any trailing
         * whitespace.
         *
         * \exception InvalidInput The given input stream does not begin with
         * a tight encoding of a <i>dim</i>-dimensional facet pairing on
         * a positive number of simplices.
         *
         * \nopython Use tightDecoding() instead, which takes a string as
         * its argument.
         *
         * \param input an input stream that begins with the tight encoding
         * for a <i>dim</i>-dimensional facet pairing.
         * \return the facet pairing represented by the given tight encoding.
         */
        static FacetPairing<dim> tightDecode(std::istream& input);

        /**
         * Writes header information for a Graphviz DOT file that will
         * describe the graphs for one or more facet pairings.
         *
         * See dotHeader() for further details on what this output contains.
         *
         * This routine is equivalent to (but faster than) writing the
         * string returned by dotHeader() to the given output stream.
         *
         * \nopython Use dotHeader() instead, which returns a string.
         *
         * \param out the output stream to which to write.
         * \param graphName the name of the graph to write in the DOT header.
         * If this is null or empty then a default graph name will be used.
         *
         * \see http://www.graphviz.org/
         */
        static void writeDotHeader(std::ostream& out,
            const char* graphName = nullptr);

        /**
         * Returns header information for a Graphviz DOT file that will
         * describe the graphs for one or more facet pairings.  See the
         * dot() documentation for further information on such graphs.
         *
         * The output will be in the Graphviz DOT language, and will
         * include appropriate display settings for graphs, edges and
         * nodes.  The opening brace for a \c graph section of the DOT
         * file is included.
         *
         * This routine may be used with dot() or writeDot() to generate a
         * single DOT file containing the graphs for several different facet
         * pairings.  A complete DOT file can be produced by calling
         * this routine, then calling dot() or writeDot() in subgraph mode for
         * each facet pairing, then outputting a final closing curly brace.
         *
         * Note that if you require a DOT file containing the graph for
         * only a single facet pairing, this routine is unnecessary; you
         * may simply call dot() or writeDot() in full graph mode instead.
         *
         * This routine is suitable for generating undirected graphs, not
         * directed graphs.  The final DOT file should be used with
         * either the \a neato or \a fdp programs shipped with Graphviz.
         *
         * If you are simply writing this string to an output stream then you
         * should call writeDotHeader() instead, which is more efficient.
         *
         * \param graphName the name of the graph to write in the DOT header.
         * If this is null or empty then a default graph name will be used.
         * \return the DOT header information, as outlined above.
         *
         * \see http://www.graphviz.org/
         */
        static std::string dotHeader(const char* graphName = nullptr);

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
         * For each facet pairing that is generated, this routine will call
         * \a action (which must be a function or some other callable object).
         *
         * - The first argument to \a action must be a const reference to a
         *   FacetPairing<dim>.  This will be the facet pairing that was found.
         *   If \a action wishes to keep the facet pairing, it should take a
         *   deep copy (not a reference), since the facet pairing may be
         *   changed and reused after \a action returns.
         *
         * - If \a action takes a FacetPairing<dim>::IsoList as its second
         *   argument (which may be as a reference, and may have const/volatile
         *   qualifiers), then this will be the list of all automorphisms of
         *   the facet pairing that was found.  This list will be passed by
         *   value using move semantics.  If \a action does not take a second
         *   argument, or if the second argument is of a different type, then
         *   the list of automorphisms will not be passed.
         *
         * - If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - \a action must return \c void.
         *
         * Because this class cannot represent an empty facet pairing,
         * if the argument \a nSimplices is zero then no facet pairings
         * will be generated at all.
         *
         * \warning If you are allowing a large number of boundary facets,
         * then the automorphisms groups could be enormous.  In this case it is
         * highly recommended that your action does _not_ take the list of all
         * automorphisms as its second argument, since this will avoid the
         * enormous memory cost of storing and passing such a list.
         *
         * \todo \optlong When generating facet pairings, do some checking to
         * eliminate cases in which simplex (\a k > 0) can be swapped
         * with simplex 0 to produce a smaller representation of the same
         * pairing.
         * \todo \feature Allow cancellation of facet pairing generation.
         *
         * \python This function is available, and \a action may be a
         * pure Python function.  However, its form is more restricted:
         * \a action must take both a facet pairing and its automorphisms
         * (i.e., the automorphisms argument is not optional); moreover,
         * it cannot take any additional arguments beyond these.
         * As a consequence, the additional \a args list is omitted also.
         *
         * \param nSimplices the number of simplices whose facets should
         * be (potentially) matched.
         * \param boundary determines whether any facets may be left
         * unmatched.  This set should contain \c true if pairings with at
         * least one unmatched facet are to be generated, and should contain
         * \c false if pairings with no unmatched facets are to be generated.
         * \param nBdryFacets specifies the precise number of facets that
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
         * must be of the same parity as `nSimplices * (dim+1)`,
         * and can be at most `(dim-1) * nSimplices + 2`.
         * \param action a function (or other callable object) to call
         * for each facet pairing that is found.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial facet pairing argument and the
         * optional automorphism argument.
         */
        template <typename Action, typename... Args>
        static void findAllPairings(size_t nSimplices, BoolSet boundary,
            int nBdryFacets, Action&& action, Args&&... args);

    protected:
        /**
         * Creates a new facet pairing.  All internal arrays will be
         * allocated but not initialised.
         *
         * \pre \a size is at least 1.
         *
         * \param size the number of simplices under
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
         * \param source the facet under investigation.
         * \return the other facet to which the given facet is paired.
         */
        FacetSpec<dim>& dest(const FacetSpec<dim>& source);

        /**
         * Returns the other facet to which the given simplex facet is
         * paired.  If the given facet is left deliberately unmatched, the
         * value returned will be boundary (as returned by
         * FacetSpec<dim>::isBoundary()).
         *
         * \param simp the simplex under investigation (this must be
         * strictly less than the total number of simplices under
         * consideration).
         * \param facet the facet of the given simplex under
         * investigation (between 0 and \a dim inclusive).
         * \return the other facet to which the given facet is paired.
         */
        FacetSpec<dim>& dest(size_t simp, int facet);

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
         * \param source the facet under investigation.
         * \return the other facet to which the given facet is paired.
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
         * \param source the facet under investigation.
         * \return \c true if the matching for the given facet has not yet
         * been determined, or \c false otherwise.
         */
        bool noDest(const FacetSpec<dim>& source) const;

        /**
         * Determines whether the matching for the given simplex facet
         * has not yet been determined.  This is signalled by a facet
         * matched to itself.
         *
         * \param simp the simplex under investigation (this must be
         * strictly less than the total number of simplices under
         * consideration).
         * \param facet the facet of the given simplex under
         * investigation (between 0 and \a dim inclusive).
         * \return \c true if the matching for the given facet has not yet
         * been determined, or \c false otherwise.
         */
        bool noDest(size_t simp, int facet) const;

        /**
         * Determines whether this facet pairing is in canonical
         * (smallest lexicographical) form, given a small set of
         * assumptions.
         *
         * If the argument \a list is non-null, then:
         *
         * - If this facet pairing is in canonical form, the given list will
         *   be filled with the set of all combinatorial automorphisms of this
         *   facet pairing.
         *
         * - If not, the given list will be returned empty.
         *
         * \pre The given list (if one is provided) is empty.
         * \pre For each simplex \a t, the only case in which
         * `dest(t,i)` is greater than `dest(t,i+1)` is where
         * facets `(t,i)` and `(t,i+1)` are paired together.
         * \pre For each simplex \a t > 0, it is true that
         * `dest(t,0).simp < t`.
         * \pre The sequence `dest(1,0)`, `dest(2,0)`,
         * ..., `dest(n-1,0)` is strictly increasing, where
         * \a n is the total number of simplices under investigation.
         *
         * \param list the list into which automorphisms will be placed
         * if this facet pairing is indeed canonical, or \a null if the
         * automorphisms are not requred.
         * \return \c true if and only if this facet pairing is in
         * canonical form.
         */
        bool isCanonicalInternal(IsoList* list = nullptr) const;

    private:
        /**
         * The part of the return value for canonicalInternal() that
         * holds the isomorphism(s).
         */
        template <bool allIsos>
        using CanonicalIsos = std::conditional_t<allIsos,
            IsoList, Isomorphism<dim>>;

        /**
         * A single unified piece of code that implements both canonical() and
         * canonicalAll().  The template argument \a allIsos indicates which
         * of these two routines we are implementing.
         */
        template <bool allIsos>
        std::pair<FacetPairing<dim>, CanonicalIsos<allIsos>>
            canonicalInternal() const;

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
        template <typename Action, typename... Args>
        void enumerateInternal(BoolSet boundary, int nBdryFacets,
            Action&& action, Args&&... args);
};

} // namespace regina::detail -> namespace regina

/**
 * Swaps the contents of the given facet pairings.
 *
 * This global routine simply calls FacetPairing<dim>::swap(); it is provided
 * so that FacetPairing<dim> meets the C++ Swappable requirements.
 *
 * \param a the first facet pairing whose contents should be swapped.
 * \param b the second facet pairing whose contents should be swapped.
 *
 * \ingroup generic
 */
template <int dim>
void swap(FacetPairing<dim>& a, FacetPairing<dim>& b) noexcept {
    a.swap(b);
}

namespace detail {

// Inline functions for FacetPairingBase

template <int dim>
inline FacetPairingBase<dim>::FacetPairingBase(size_t size) :
        size_(size), pairs_(new FacetSpec<dim>[size * (dim + 1)]) {
}

template <int dim>
inline FacetPairingBase<dim>::FacetPairingBase(
        const FacetPairingBase<dim>& src) :
        size_(src.size_),
        pairs_(new FacetSpec<dim>[src.size_ * (dim + 1)]) {
    std::copy(src.pairs_, src.pairs_ + (size_ * (dim + 1)), pairs_);
}

template <int dim>
inline FacetPairingBase<dim>::FacetPairingBase(FacetPairingBase<dim>&& src)
        noexcept :
        size_(src.size_),
        pairs_(src.pairs_) {
    src.pairs_ = nullptr;
}

template <int dim>
inline FacetPairingBase<dim>::~FacetPairingBase() {
    delete[] pairs_;
}

template <int dim>
inline FacetPairingBase<dim>& FacetPairingBase<dim>::operator = (
        const FacetPairingBase& src) {
    // std::copy() exhibits undefined behaviour in the case of self-assignment.
    if (std::addressof(src) == this)
        return *this;

    if (size_ != src.size_) {
        delete[] pairs_;
        size_ = src.size_;
        pairs_ = new FacetSpec<dim>[size_ * (dim + 1)];
    }
    std::copy(src.pairs_, src.pairs_ + (size_ * (dim + 1)), pairs_);
    return *this;
}

template <int dim>
inline FacetPairingBase<dim>& FacetPairingBase<dim>::operator = (
        FacetPairingBase&& src) noexcept {
    size_ = src.size_;
    std::swap(pairs_, src.pairs_);
    // Let src dispose of the original contents in its own destructor.
    return *this;
}

template <int dim>
inline void FacetPairingBase<dim>::swap(FacetPairingBase& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(pairs_, other.pairs_);
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
        size_t simp, int facet) const {
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
        size_t simp, int facet) const {
    return pairs_[(dim + 1) * simp + facet].isBoundary(size_);
}

template <int dim>
inline FacetSpec<dim>& FacetPairingBase<dim>::dest(
        const FacetSpec<dim>& source) {
    return pairs_[(dim + 1) * source.simp + source.facet];
}

template <int dim>
inline FacetSpec<dim>& FacetPairingBase<dim>::dest(
        size_t simp, int facet) {
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
        size_t simp, int facet) const {
    FacetSpec<dim>& f = pairs_[(dim + 1) * simp + facet];
    return (f.simp == static_cast<ssize_t>(simp) && f.facet == facet);
}

template <int dim>
inline std::string FacetPairingBase<dim>::toTextRep() const {
    return textRep();
}

template <int dim>
void FacetPairingBase<dim>::tightEncode(std::ostream& out) const {
    regina::detail::tightEncodeIndex(out, size_);
    // Write each pairing from one side only, in the forward direction.
    // The test below will also write unmatched and undecided pairings
    // correctly.
    for (size_t i = 0; i < size_ * (dim + 1); ++i) {
        if (pairs_[i].simp < 0)
            throw FailedPrecondition("Before-the-start destinations "
                "are not allowed in tight encodings");

        size_t adjIdx = (dim + 1) * pairs_[i].simp + pairs_[i].facet;
        if (adjIdx >= i) {
            if (adjIdx > size_ * (dim + 1))
                throw FailedPrecondition("Past-the-end destinations "
                    "are not allowed in tight encodings");

            regina::detail::tightEncodeIndex(out, adjIdx);
        }
    }
}

template <int dim>
FacetPairing<dim> FacetPairingBase<dim>::tightDecode(std::istream& input) {
    auto size = regina::detail::tightDecodeIndex<size_t>(input);
    if (size <= 0)
        throw InvalidInput("The tight encoding has a non-positive number "
            "of simplices");

    FacetPairing<dim> ans(size);

    for (size_t i = 0; i < size * (dim+1); ++i)
        ans.pairs_[i].setBeforeStart();

    for (size_t i = 0; i < size * (dim+1); ++i) {
        if (! ans.pairs_[i].isBeforeStart())
            continue;

        auto adjIdx = regina::detail::tightDecodeIndex<size_t>(input);
        if (adjIdx > size * (dim+1))
            throw InvalidInput("The tight encoding contains "
                "invalid matchings of simplex facets");
        if (adjIdx < i)
            throw InvalidInput("The tight encoding contains "
                "unexpected matchings of simplex facets");

        ans.pairs_[i] = FacetSpec<dim>(adjIdx / (dim+1), adjIdx % (dim+1));

        if (adjIdx < size * (dim + 1) && adjIdx > i) {
            // This is a real gluing.  Make it from the other side also.
            if (ans.pairs_[adjIdx].isBeforeStart())
                ans.pairs_[adjIdx] = FacetSpec<dim>(i / (dim+1), i % (dim+1));
            else {
                // Some other pairing has already claimed this destination.
                throw InvalidInput("The tight encoding contains "
                    "inconsistent matchings of simplex facets");
            }
        }
    }

    return ans;
}

template <int dim>
inline std::pair<FacetPairing<dim>, Isomorphism<dim>>
        FacetPairingBase<dim>::canonical() const {
    return canonicalInternal<false>();
}

template <int dim>
inline std::pair<FacetPairing<dim>, typename FacetPairingBase<dim>::IsoList>
        FacetPairingBase<dim>::canonicalAll() const {
    return canonicalInternal<true>();
}

template <int dim>
inline typename FacetPairingBase<dim>::IsoList
        FacetPairingBase<dim>::findAutomorphisms() const {
    IsoList list;
    isCanonicalInternal(std::addressof(list));
    return list;
}

template <int dim>
template <typename Action, typename... Args>
inline void FacetPairingBase<dim>::findAllPairings(size_t nSimplices,
        BoolSet boundary, int nBdryFacets,
        Action&& action, Args&&... args) {
    FacetPairing<dim> pairing(nSimplices);
    pairing.enumerateInternal(boundary, nBdryFacets,
        std::forward<Action>(action), std::forward<Args>(args)...);
}

} } // namespace regina::detail

#include "triangulation/detail/facetpairing-impl.h"

#endif

