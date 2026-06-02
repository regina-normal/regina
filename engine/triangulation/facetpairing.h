
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

#ifndef __REGINA_FACETPAIRING_H
#ifndef __DOXYGEN
#define __REGINA_FACETPAIRING_H
#endif

/*! \file triangulation/facetpairing.h
 *  \brief Support for for dual graphs of <i>dim</i>-dimensional
 *  triangulations.
 */

#include <iostream>
#include <optional>
#include <vector>
#include "regina-core.h"
#include "concepts/core.h"
#include "core/output.h"
#include "triangulation/cut.h"
#include "triangulation/facetspec.h"
#include "triangulation/forward.h"
#include "utilities/boolset.h"
#include "utilities/tightencoding.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

class FacePair;

/**
 * Represents the dual graph of a <i>dim</i>-manifold triangulation; that is,
 * the pairwise matching of facets of <i>dim</i>-dimensional simplices.
 *
 * Given a fixed number of <i>dim</i>-dimensional simplices, each facet of
 * each simplex is either paired with some other simplex facet (which is
 * in turn paired with it) or remains unmatched.  A simplex facet cannot be
 * paired with itself.
 *
 * Such a matching models part of the structure of a <i>dim</i>-manifold
 * triangulation, in which each simplex facet is either glued to some
 * other simplex facet (which is in turn glued to it) or is an unglued
 * boundary facet.  Note however that a facet pairing does not contain
 * enough information to fully reconstruct a triangulation, since
 * the permutations used for each individual gluing are not stored.
 *
 * Facet pairings are _labelled_, in that the simplices are explicitly
 * numbered 0,1,..., and the facets of each simplex are explicitly numbered
 * 0,...,\a dim (just like in a triangulation).  Facet pairings do also come
 * with code to help identify and work with relabellings, via isomorphisms,
 * automorphisms, and canonical representations.  In this context:
 *
 * - An _isomorphism_ of a facet pairing means a relabelling of the simplices
 *   and a relabelling of the (\a dim + 1) facets within each simplex;
 *   this can be represented by the same class Isomorphism<dim> that is used
 *   for isomorphisms of triangulations.
 *
 * - An _automorphism_ of a facet pairing is an isomorphism that, when
 *   applied, results in an identical facet pairing (i.e., where exactly the
 *   same pairs of labelled simplex facets are matched together).
 *
 * - A facet pairing is in _canonical form_ if it is a
 *   lexicographically minimal representative of its isomorphism class.
 *   Here we order facet pairings by lexicographical comparison of the
 *   sequence `dest(0,0)`, `dest(0,1)`, ...,
 *   `dest(size()-1, dim)` (which in turn uses the ordering
 *   defined by FacetSpec<dim>, where each simplex facet is ordered
 *   first by simplex number and then by facet number, and where the
 *   boundary is ordered last).
 *
 * In dimension 3, this class offers extra functionality compared to other
 * dimensions.  In particular, it offers several extra routines for finding
 * informative subgraphs within the dual graph.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \python Python does not support templates.  Instead
 * this class can be used by appending the dimension as a suffix
 * (e.g., FacetPairing2 and FacetPairing3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \headerfile triangulation/triangulation.h
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
class FacetPairing :
        public ShortOutput<FacetPairing<dim>>,
        public TightEncodable<FacetPairing<dim>> {
    public:
        /**
         * A list of isomorphisms on facet pairings.
         *
         * In particular, this class uses the IsoList type to return
         * the set of all _automorphisms_ of a facet pairing.
         */
        using IsoList = std::vector<Isomorphism<dim>>;

    private:
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
        FacetPairing(const FacetPairing& src);

        /**
         * Moves the given facet pairing into this facet pairing.
         * This is a fast (constant time) operation.
         *
         * The facet pairing that is passed (\a src) will no longer be usable.
         *
         * \param src the facet pairing to move.
         */
        FacetPairing(FacetPairing&& src) noexcept;

        /**
         * Creates the dual graph of the given triangulation.
         * This is the facet pairing that describes how the facets of
         * top-dimensional simplices in the given triangulation are joined
         * together, as described in the class notes.
         *
         * Calling `FacetPairing<dim>(tri)` is equivalent to calling
         * `tri.pairing()`.
         *
         * \pre The given triangulation is not empty.
         *
         * \param tri the triangulation whose facet pairing should
         * be constructed.
         */
        FacetPairing(const Triangulation<dim>& tri);

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
         * \nopython Instead use fromTextRep(), which reads this same text
         * format in string form.  The main differences between this
         * constructor and fromTextRep() are: (i) fromTextRep() does not
         * skip over initial whitespace; and (ii) fromTextRep() throws
         * InvalidArgument exceptions on error (not InvalidInput).
         *
         * \param in the input stream from which to read.
         */
        FacetPairing(std::istream& in);

        /**
         * Deallocates any memory used by this structure.
         */
        ~FacetPairing();

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
        FacetPairing& operator = (const FacetPairing& src);

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
        FacetPairing& operator = (FacetPairing&& src) noexcept;

        /**
         * Swaps the contents of this and the given facet pairing.
         *
         * \param other the facet pairing whose contents are to be
         * swapped with this.
         */
        void swap(FacetPairing& other) noexcept;

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
         * Determines if this and the given facet pairing are identical.
         *
         * \param other the facet pairing to compare with this.
         * \return \c true if and only if this and the given facet pairing
         * are identical.
         */
        bool operator == (const FacetPairing& other) const;

        /*@}*/
        /**
         * \name Structure and subgraphs
         */
        /*@{*/

        /**
         * Determines whether the underlying graph for this facet pairing
         * contains an edge of multiplicity \a k.
         *
         * This corresponds to two distinct top-dimensional simplices
         * joined to each other along at least \a k of their facets.
         *
         * Note that this routine does not consider loops in the underlying
         * graph, only edges between distinct nodes.
         *
         * This routine is the fastest way to detect edges of multiplicity \a k
         * if you know \a k at compile time.
         *
         * For the cases `k = 2` and `k = 3`, you can also access this
         * functionality via the aliases hasDoubleEdge() and hasTripleEdge().
         *
         * \nopython Instead use the variant `hasMultiEdge(k)`.
         *
         * \tparam k the multiplicity of edges to search for.
         *
         * \return \c true if and only if the underyling graph has an edge of
         * multiplicity \a k.
         */
        template <int k> requires (k >= 2 && k <= dim + 1)
        bool hasMultiEdge() const;

        /**
         * Determines whether the underlying graph for this facet pairing
         * contains an edge of multiplicity \a k, where \a k does not need to
         * be known until runtime.
         *
         * An edge of multiplicity \a k corresponds to two distinct
         * top-dimensional simplices joined to each other along at least \a k
         * of their facets.
         *
         * Note that this routine does not consider loops in the underlying
         * graph, only edges between distinct nodes.
         *
         * For C++ programmers who know \a k at compile time, it is faster to
         * call the template function `hasMultiEdge<k>()` instead.
         *
         * For the cases `k = 2` and `k = 3`, you can also access this
         * functionality via the aliases hasDoubleEdge() and hasTripleEdge().
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
         * Determines whether the underlying graph for this facet pairing
         * contains a double edge.
         *
         * This corresponds to two distinct top-dimensional simplices
         * joined to each other along at least two of their facets.
         *
         * Note that this routine does not consider loops in the underlying
         * graph, only edges between distinct nodes.
         *
         * This routine is identical to calling `hasMultiEdge<2>()`.
         *
         * \return `true` if and only if the underlying graph has a double edge.
         */
        bool hasDoubleEdge() const;

        /**
         * Determines whether the underlying graph for this facet pairing
         * contains a triple edge.
         *
         * This corresponds to two distinct top-dimensional simplices
         * joined to each other along at least three of their facets.
         *
         * Note that this routine does not consider loops in the underlying
         * graph, only edges between distinct nodes.
         *
         * In dimension 3, a facet pairing containing a triple edge cannot
         * model a closed minimal irreducible P²-irreducible 3-manifold
         * triangulation on more than two tetrahedra.  See "Face pairing graphs
         * and 3-manifold enumeration", Benjamin A. Burton, J. Knot Theory
         * Ramifications 13 (2004), 1057--1101.
         *
         * This routine is identical to calling `hasMultiEdge<3>()`.
         *
         * \return `true` if and only if the underlying graph has a triple edge.
         */
        bool hasTripleEdge() const;

        /**
         * Follows a chain in a 3-dimensional facet pairing as far as possible
         * from the given point.
         *
         * A chain in a 3-dimensional facet pairing is the underlying facet
         * pairing for a layered chain; specifically it involves one tetrahedron
         * joined to a second along two faces, the remaining two faces of the
         * second tetrahedron joined to a third and so on.  A chain can involve
         * as few as one tetrahedron or as many as we like.  Note that the
         * remaining two faces of the first tetrahedron and the remaining two
         * faces of the final tetrahedron remain unaccounted for by this
         * structure.
         *
         * This routine begins with two faces of a given tetrahedron,
         * described by parameters \a tet and \a faces.
         * If these two faces are both joined to some different
         * tetrahedron, parameter \a tet will be changed to this new
         * tetrahedron and parameter \a faces will be changed to the
         * remaining faces of this new tetrahedron (i.e., the two faces
         * that were not joined to the original faces of the original
         * tetrahedron).
         *
         * This procedure is repeated as far as possible until either
         * the two faces in question join to two different tetrahedra,
         * the two faces join to each other, or at least one of the
         * two faces is unmatched.
         *
         * Thus, given one end of a chain, this procedure can be used to
         * follow the face pairings through to the other end of the chain.
         *
         * \warning You must be sure when calling this routine that you
         * are not inside a chain that loops back onto itself!
         * If the facet pairing forms a large loop with each tetrahedron
         * joined by two faces to the next, this routine will cycle around
         * the loop forever and never return.
         *
         * \param tet the index in the underlying triangulation of the
         * tetrahedron to begin at.  This parameter will be modified
         * directly by this routine as a way of returning the results.
         * \param faces the pair of face numbers in the given
         * tetrahedron at which we begin.  This parameter will also be
         * modified directly by this routine as a way of returning results.
         */
        void followChain(ssize_t& tet, FacePair& faces) const
            requires (dim == 3);

        /**
         * Determines whether this 3-dimensional facet pairing contains a
         * broken double-ended chain.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * facet pairing for a layered solid torus).  A double-ended
         * chain is a chain in which the first tetrahedron is joined to
         * itself along one face and the final tetrahedron is also
         * joined to itself along one face (i.e., the underlying
         * facet pairing for a layered lens space).
         *
         * A broken double-ended chain consists of two one-ended chains
         * (using distinct sets of tetrahedra) joined together along one
         * face.  The remaining two faces (one from each chain)
         * that were unaccounted for by the individual one-ended chains
         * remain unaccounted for by this broken double-ended chain.
         *
         * In this routine we are interested specifically in finding a
         * broken double-ended chain that is not a part of a complete
         * double-ended chain, i.e., the final two unaccounted faces are
         * not joined together.
         *
         * A facet pairing containing a broken double-ended chain cannot
         * model a closed minimal irreducible P²-irreducible 3-manifold
         * triangulation on more than two tetrahedra.  See "Face pairing
         * graphs and 3-manifold enumeration", Benjamin A. Burton,
         * J. Knot Theory Ramifications 13 (2004), 1057--1101.
         *
         * \return \c true if and only if this facet pairing contains a
         * broken double-ended chain that is not part of a complete
         * double-ended chain.
         */
        bool hasBrokenDoubleEndedChain() const requires (dim == 3);

        /**
         * Determines whether this 3-dimensional facet pairing contains a
         * one-ended chain with a double handle.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * facet pairing for a layered solid torus).
         *
         * A one-ended chain with a double handle begins with a one-ended
         * chain.  The two faces that are unaccounted for by this
         * one-ended chain must be joined
         * to two different tetrahedra, and these two tetrahedra must be
         * joined to each other along two faces.  The remaining two
         * faces of these two tetrahedra remain unaccounted for by this
         * structure.
         *
         * A facet pairing containing a one-ended chain with a double handle
         * cannot model a closed minimal irreducible P²-irreducible
         * 3-manifold triangulation on more than two tetrahedra.  See
         * "Face pairing graphs and 3-manifold enumeration",
         * Benjamin A. Burton, J. Knot Theory Ramifications 13 (2004),
         * 1057--1101.
         *
         * \return \c true if and only if this facet pairing contains a
         * one-ended chain with a double handle.
         */
        bool hasOneEndedChainWithDoubleHandle() const requires (dim == 3);

        /**
         * Determines whether this 3-dimensional facet pairing contains a
         * wedged double-ended chain.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * facet pairing for a layered solid torus).  A double-ended
         * chain is a chain in which the first tetrahedron is joined to
         * itself along one face and the final tetrahedron is also
         * joined to itself along one face (i.e., the underlying
         * facet pairing for a layered lens space).
         *
         * A wedged double-ended chain is created from two one-ended
         * chains as follows.  Two new tetrahedra are added, and each
         * one-ended chain is joined to each of the new tetrahedra along
         * a single face.  In addition, the two new tetrahedra are
         * joined to each other along a single face.  The remaining two
         * faces (one from each of the new tetrahedra) remain
         * unaccounted for by this structure.
         *
         * An alternative way of viewing a wedged double-ended chain is
         * as an ordinary double-ended chain, where one of the internal
         * tetrahedra is removed and replaced with a pair of tetrahedra
         * joined to each other.  Whereas the original tetrahedron met its
         * two neighbouring tetrahedra along two faces each (giving a
         * total of four face identifications), the two new tetrahedra now
         * meet each of the two neighbouring tetrahedra along a single
         * face each (again giving four face identifications).
         *
         * Note that if this alternate construction is used to replace
         * one of the end tetrahedra of the double-ended chain (not an
         * internal tetrahedron), the resulting structure will either be
         * a triple edge or a one-ended chain with a double handle
         * (according to whether the original chain has zero or positive
         * length).  See hasTripleEdge() and
         * hasOneEndedChainWithDoubleHandle() for further details.
         *
         * A facet pairing containing a wedged double-ended chain
         * cannot model a closed minimal irreducible P²-irreducible
         * 3-manifold triangulation on more than two tetrahedra.  See
         * "Enumeration of non-orientable 3-manifolds using face-pairing
         * graphs and union-find", Benjamin A. Burton,
         * Discrete Comput. Geom. 38 (2007), no. 3, 527--571.
         *
         * \return \c true if and only if this facet pairing contains a
         * wedged double-ended chain.
         */
        bool hasWedgedDoubleEndedChain() const requires (dim == 3);

        /**
         * Determines whether this 3-dimensional facet pairing contains a
         * one-ended chain with a stray bigon.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * facet pairing for a layered solid torus).
         *
         * A one-ended chain with a stray bigon describes the following
         * structure.  We begin with a one-ended chain.  Two new
         * tetrahedra are added; these are joined to each other along
         * two pairs of faces, and one of the new tetrahedra is joined
         * to the end of the one-ended chain.  We then ensure that:
         * - This configuration is not part of a longer one-ended chain
         *   that encompasses all of the aforementioned tetrahedra;
         * - There is no extra tetrahedron that is joined to both the
         *   two new tetrahedra and the end of the chain;
         * - There is no extra tetrahedron that is joined to the end of
         *   the chain along one face and the far new tetrahedron along
         *   two additional faces (where by "the far new tetrahedron"
         *   we mean the new tetrahedron that was not originally joined to
         *   the chain).
         *
         * Aside from these constraints, the remaining four tetrahedron faces
         * (two faces of the far new tetrahedron, one face of the other new
         * tetrahedron, and one face at the end of the chain) remain
         * unaccounted for by this structure.
         *
         * A facet pairing containing a structure of this type
         * cannot model a closed minimal irreducible P²-irreducible
         * 3-manifold triangulation on more than two tetrahedra.  See
         * "Enumeration of non-orientable 3-manifolds using face-pairing
         * graphs and union-find", Benjamin A. Burton,
         * Discrete Comput. Geom. 38 (2007), no. 3, 527--571.
         *
         * \return \c true if and only if this facet pairing contains a
         * one-ended chain with a stray bigon.
         */
        bool hasOneEndedChainWithStrayBigon() const requires (dim == 3);

        /**
         * Determines whether this 3-dimensional facet pairing contains a
         * triple one-ended chain.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * facet pairing for a layered solid torus).
         *
         * A triple one-ended chain is created from three one-ended
         * chains as follows.  Two new tetrahedra are added, and each
         * one-ended chain is joined to each of the new tetrahedra along
         * a single face.  The remaining two faces (one from each of the
         * new tetrahedra) remain unaccounted for by this structure.
         *
         * A facet pairing containing a triple one-ended chain
         * cannot model a closed minimal irreducible P²-irreducible
         * 3-manifold triangulation on more than two tetrahedra.  See
         * "Enumeration of non-orientable 3-manifolds using face-pairing
         * graphs and union-find", Benjamin A. Burton,
         * Discrete Comput. Geom. 38 (2007), no. 3, 527--571.
         *
         * \return \c true if and only if this facet pairing contains a
         * triple one-ended chain.
         */
        bool hasTripleOneEndedChain() const requires (dim == 3);

        /**
         * Determines whether this 3-dimensional facet pairing contains a
         * single-edged star.
         *
         * A single-edged star involves two tetrahedra that are adjacent
         * along a single face, where the six remaining faces of these
         * tetrahedra are joined to six entirely new tetrahedra (so that
         * none of the eight tetrahedra described in this structure are
         * the same).
         *
         * \return \c true if and only if this facet pairing contains a
         * single-edged star.
         */
        bool hasSingleStar() const requires (dim == 3);

        /**
         * Determines whether this 3-dimensional facet pairing contains a
         * double-edged star.
         *
         * A double-edged star involves two tetrahedra that are adjacent
         * along two separate pairs of faces, where the four remaining
         * faces of these tetrahedra are joined to four entirely new
         * tetrahedra (so that none of the six tetrahedra described in
         * this structure are the same).
         *
         * \return \c true if and only if this facet pairing contains a
         * double-edged star.
         */
        bool hasDoubleStar() const requires (dim == 3);

        /**
         * Determines whether this 3-dimensional facet pairing contains a
         * double-edged square.
         *
         * A double-edged square involves four distinct tetrahedra that
         * meet each other as follows.  Two pairs of tetrahedra are
         * joined along two pairs of faces each.  Then each tetrahedron
         * is joined along a single face to one tetrahedron of the other
         * pair.  The four tetrahedron faces not yet joined to anything
         * (one from each tetrahedron) remain unaccounted for by this
         * structure.
         *
         * \return \c true if and only if this facet pairing contains a
         * double-edged square.
         */
        bool hasDoubleSquare() const requires (dim == 3);

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
        std::pair<FacetPairing, Isomorphism<dim>> canonical() const;

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
        std::pair<FacetPairing, IsoList> canonicalAll() const;

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
        static FacetPairing fromTextRep(const std::string& rep);

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
        static FacetPairing tightDecode(std::istream& input);

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
         * \a action (which must be a function or some other callable type).
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
         * - The return value of \a action will be ignored; typically it would
         *   return \c void.
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
         * \param action a function (or other callable type) to call
         * for each facet pairing that is found.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial facet pairing argument and the
         * optional automorphism argument.
         */
        template <typename Action, typename... Args>
        requires
            VoidCallback<Action, const FacetPairing&, Args...> ||
            VoidCallback<Action, const FacetPairing&,
                typename FacetPairing::IsoList, Args...>
        static void findAllPairings(size_t nSimplices, BoolSet boundary,
            int nBdryFacets, Action&& action, Args&&... args);

    private:
        /**
         * Creates a new facet pairing.  All internal arrays will be
         * allocated but not initialised.
         *
         * \pre \a size is at least 1.
         *
         * \param size the number of top-dimensional simplicies under
         * consideration in this new facet pairing.
         */
        FacetPairing(size_t size);

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
        std::pair<FacetPairing, CanonicalIsos<allIsos>> canonicalInternal()
            const;

        /**
         * Internal to findAllPairings().
         *
         * Performs the actual enumeration of facet pairings.  At most one
         * copy of this routine should be running at any given time for a
         * particular FacetPairing instance.
         */
        template <typename Action, typename... Args>
        requires
            VoidCallback<Action, const FacetPairing&, Args...> ||
            VoidCallback<Action, const FacetPairing&,
                typename FacetPairing::IsoList, Args...>
        void enumerateInternal(BoolSet boundary, int nBdryFacets,
            Action&& action, Args&&... args);

        /**
         * Internal to the 3-D query hasBrokenDoubleEndedChain().
         * This routine assumes that the give face of the given tetrahedron
         * is glued to this same tetrahedron, and attempts to find a broken
         * double-ended chain for which this tetrahedron is at the end of one
         * of the one-ended chains therein.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this facet pairing.
         *
         * \param tet the index in the triangulation of the given
         * tetrahedron.
         * \param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * \return \c true if and only if this facet pairing contains a
         * broken double-ended chain as described above.
         */
        bool hasBrokenDoubleEndedChain(size_t tet, int face) const
            requires (dim == 3);

        /**
         * Internal to the 3-D query hasOneEndedChainWithDoubleHandle().
         * This routine assumes that the give face of the given tetrahedron
         * is glued to this same tetrahedron, and attempts to find a one-ended
         * chain with a double handle for which this tetrahedron is at
         * the end of the chain contained therein.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this facet pairing.
         *
         * \param tet the index in the triangulation of the given
         * tetrahedron.
         * \param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * \return \c true if and only if this facet pairing contains a
         * one-ended chain with a double handle as described above.
         */
        bool hasOneEndedChainWithDoubleHandle(size_t tet, int face) const
            requires (dim == 3);

        /**
         * Internal to the 3-D query hasWedgedDoubleEndedChain().
         * This routine assumes that the give face of the given tetrahedron
         * is glued to this same tetrahedron, and attempts to find a wedged
         * double-ended chain for which this tetrahedron is at one end of the
         * double-ended chain.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this facet pairing.
         *
         * \param tet the index in the triangulation of the given
         * tetrahedron.
         * \param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * \return \c true if and only if this facet pairing contains a
         * wedged double-ended chain as described above.
         */
        bool hasWedgedDoubleEndedChain(size_t tet, int face) const
            requires (dim == 3);

        /**
         * Internal to the 3-D query hasOneEndedChainWithStrayBigon().
         * This routine assumes that the give face of the given tetrahedron
         * is glued to this same tetrahedron, and attempts to find a one-ended
         * chain with stray bigon for which this tetrahedron is at the end of
         * the one-ended chain.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this facet pairing.
         *
         * \param tet the index in the triangulation of the given
         * tetrahedron.
         * \param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * \return \c true if and only if this facet pairing contains a
         * one-ended chain with stray bigon as described above.
         */
        bool hasOneEndedChainWithStrayBigon(size_t tet, int face) const
            requires (dim == 3);

        /**
         * Internal to the 3-D query hasTripleOneEndedChain().
         * This routine assumes that the give face of the given tetrahedron
         * is glued to this same tetrahedron, and attempts to find a triple
         * one-ended chain for which this tetrahedron is at the end of one of
         * the individual one-ended chains.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this facet pairing.
         *
         * \param tet the index in the triangulation of the given
         * tetrahedron.
         * \param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * \return \c true if and only if this facet pairing contains a
         * triple one-ended chain as described above.
         */
        bool hasTripleOneEndedChain(size_t tet, int face) const
            requires (dim == 3);

    // Facet pairings are largely read-only: allow application of isomorphisms
    // and graph cuts.
    friend class Isomorphism<dim>;
    friend class Cut;
};

/**
 * Swaps the contents of the given facet pairings.
 *
 * This global routine simply calls FacetPairing<dim>::swap(); it is provided
 * so that FacetPairing<dim> meets the C++ Swappable requirements.
 *
 * \param a the first facet pairing whose contents should be swapped.
 * \param b the second facet pairing whose contents should be swapped.
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
void swap(FacetPairing<dim>& a, FacetPairing<dim>& b) noexcept {
    a.swap(b);
}

// Inline functions for FacetPairing

template <int dim> requires (supportedDim(dim))
inline FacetPairing<dim>::FacetPairing(size_t size) :
        size_(size), pairs_(new FacetSpec<dim>[size * (dim + 1)]) {
}

template <int dim> requires (supportedDim(dim))
inline FacetPairing<dim>::FacetPairing(const FacetPairing<dim>& src) :
        size_(src.size_),
        pairs_(new FacetSpec<dim>[src.size_ * (dim + 1)]) {
    std::copy(src.pairs_, src.pairs_ + (size_ * (dim + 1)), pairs_);
}

template <int dim> requires (supportedDim(dim))
inline FacetPairing<dim>::FacetPairing(FacetPairing<dim>&& src) noexcept :
        size_(src.size_), pairs_(src.pairs_) {
    src.pairs_ = nullptr;
}

template <int dim> requires (supportedDim(dim))
inline FacetPairing<dim>::~FacetPairing() {
    delete[] pairs_;
}

template <int dim> requires (supportedDim(dim))
inline FacetPairing<dim>& FacetPairing<dim>::operator = (
        const FacetPairing& src) {
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

template <int dim> requires (supportedDim(dim))
inline FacetPairing<dim>& FacetPairing<dim>::operator = (FacetPairing&& src)
        noexcept {
    size_ = src.size_;
    std::swap(pairs_, src.pairs_);
    // Let src dispose of the original contents in its own destructor.
    return *this;
}

template <int dim> requires (supportedDim(dim))
inline void FacetPairing<dim>::swap(FacetPairing& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(pairs_, other.pairs_);
}

template <int dim> requires (supportedDim(dim))
inline size_t FacetPairing<dim>::size() const {
    return size_;
}

template <int dim> requires (supportedDim(dim))
inline const FacetSpec<dim>& FacetPairing<dim>::dest(
        const FacetSpec<dim>& source) const {
    return pairs_[(dim + 1) * source.simp + source.facet];
}

template <int dim> requires (supportedDim(dim))
inline const FacetSpec<dim>& FacetPairing<dim>::dest(
        size_t simp, int facet) const {
    return pairs_[(dim + 1) * simp + facet];
}

template <int dim> requires (supportedDim(dim))
inline const FacetSpec<dim>& FacetPairing<dim>::operator [](
        const FacetSpec<dim>& source) const {
    return pairs_[(dim + 1) * source.simp + source.facet];
}

template <int dim> requires (supportedDim(dim))
inline bool FacetPairing<dim>::isUnmatched(const FacetSpec<dim>& source) const {
    return pairs_[(dim + 1) * source.simp + source.facet].isBoundary(size_);
}

template <int dim> requires (supportedDim(dim))
inline bool FacetPairing<dim>::isUnmatched(size_t simp, int facet) const {
    return pairs_[(dim + 1) * simp + facet].isBoundary(size_);
}

template <int dim> requires (supportedDim(dim))
inline FacetSpec<dim>& FacetPairing<dim>::dest(const FacetSpec<dim>& source) {
    return pairs_[(dim + 1) * source.simp + source.facet];
}

template <int dim> requires (supportedDim(dim))
inline FacetSpec<dim>& FacetPairing<dim>::dest(size_t simp, int facet) {
    return pairs_[(dim + 1) * simp + facet];
}

template <int dim> requires (supportedDim(dim))
inline FacetSpec<dim>& FacetPairing<dim>::operator [](
        const FacetSpec<dim>& source) {
    return pairs_[(dim + 1) * source.simp + source.facet];
}

template <int dim> requires (supportedDim(dim))
inline bool FacetPairing<dim>::noDest(const FacetSpec<dim>& source) const {
    return dest(source) == source;
}

template <int dim> requires (supportedDim(dim))
inline bool FacetPairing<dim>::noDest(size_t simp, int facet) const {
    FacetSpec<dim>& f = pairs_[(dim + 1) * simp + facet];
    return (f.simp == static_cast<ssize_t>(simp) && f.facet == facet);
}

template <int dim> requires (supportedDim(dim))
inline bool FacetPairing<dim>::hasDoubleEdge() const {
    return hasMultiEdge<2>();
}

template <int dim> requires (supportedDim(dim))
inline bool FacetPairing<dim>::hasTripleEdge() const {
    return hasMultiEdge<3>();
}

template <int dim> requires (supportedDim(dim))
inline std::string FacetPairing<dim>::toTextRep() const {
    return textRep();
}

template <int dim> requires (supportedDim(dim))
void FacetPairing<dim>::tightEncode(std::ostream& out) const {
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

template <int dim> requires (supportedDim(dim))
FacetPairing<dim> FacetPairing<dim>::tightDecode(std::istream& input) {
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

template <int dim> requires (supportedDim(dim))
inline std::pair<FacetPairing<dim>, Isomorphism<dim>>
        FacetPairing<dim>::canonical() const {
    return canonicalInternal<false>();
}

template <int dim> requires (supportedDim(dim))
inline std::pair<FacetPairing<dim>, typename FacetPairing<dim>::IsoList>
        FacetPairing<dim>::canonicalAll() const {
    return canonicalInternal<true>();
}

template <int dim> requires (supportedDim(dim))
inline typename FacetPairing<dim>::IsoList
        FacetPairing<dim>::findAutomorphisms() const {
    IsoList list;
    isCanonicalInternal(std::addressof(list));
    return list;
}

template <int dim> requires (supportedDim(dim))
template <typename Action, typename... Args>
requires
    VoidCallback<Action, const FacetPairing<dim>&, Args...> ||
    VoidCallback<Action, const FacetPairing<dim>&,
        typename FacetPairing<dim>::IsoList, Args...>
inline void FacetPairing<dim>::findAllPairings(size_t nSimplices,
        BoolSet boundary, int nBdryFacets,
        Action&& action, Args&&... args) {
    FacetPairing<dim> pairing(nSimplices);
    pairing.enumerateInternal(boundary, nBdryFacets,
        std::forward<Action>(action), std::forward<Args>(args)...);
}

} // namespace regina

#include "triangulation/detail/facetpairing-impl.h"

#endif

