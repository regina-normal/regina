
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nfacepairing.h
 *  \brief Deals with pairing off tetrahedron faces in a triangulation
 *  census.
 */

#ifndef __NFACEPAIRING_H
#ifndef __DOXYGEN
#define __NFACEPAIRING_H
#endif

#include <iostream>
#include <list>
#include "utilities/nbooleans.h"
#include "utilities/nthread.h"
#include "triangulation/nisomorphism.h"

namespace regina {

class NFacePair;
class NFacePairing;
class NTriangulation;

/**
 * \weakgroup census
 * @{
 */

/**
 * A list of isomorphisms on pairwise matchings of tetrahedron faces.
 *
 * Specifically, such an isomorphism can be used to convert one
 * pairwise matching of tetrahedron faces (as described by class
 * NFacePairing) into another.
 */
typedef std::list<NIsomorphismDirect*> NFacePairingIsoList;

/**
 * A routine used to do arbitrary processing upon a pairwise matching of
 * tetrahedron faces and its automorphisms.  Such routines are used to
 * process pairings found when running NFacePairing::findAllPairings().
 *
 * The first parameter passed should be a pairwise matching of
 * tetrahedron faces (this should not be deallocated by this routine).
 * The second parameter should be a list of all automorphisms of this pairing
 * (this should not be deallocated either).
 * The third parameter may contain arbitrary data as passed to
 * NFacePairing::findAllPairings().
 *
 * Note that the first two parameters passed might be \c null to signal that
 * face pairing generation has finished.
 */
typedef void (*UseFacePairing)(const NFacePairing*, const NFacePairingIsoList*,
    void*);

/**
 * Represents a specific pairwise matching of tetrahedron faces.
 * Given a fixed number of tetrahedra, each tetrahedron face is either
 * paired with some other tetrahedron face (which is in turn paired with
 * it) or remains unmatched.  A tetrahedron face cannot be paired with
 * itself.
 *
 * Such a matching models part of the structure of a triangulation, in
 * which each tetrahedron face is either glued to some other tetrahedron
 * face (which is in turn glued to it) or is an unglued boundary face.
 *
 * Note that if this pairing is used to construct an actual
 * triangulation, the individual gluing permutations will still need to
 * be specified; they are not a part of this structure.
 *
 * \testpart
 */
class NFacePairing : public NThread {
    private:
        unsigned nTetrahedra;
            /**< The number of tetrahedra under consideration. */

        NTetFace* pairs;
            /**< The other face to which each tetrahedron face is paired.
                 If a tetrahedron face is left unmatched, the corresponding
                 element of this array will be boundary (as returned by
                 NTetFace::isBoundary()).
                 If the destination for a particular face has not yet been
                 decided, the face will be paired to itself. */

    public:
        /**
         * \name Constructors and Destructors
         */
        /*@{*/

        /**
         * Creates a new face pairing that is a clone of the given face
         * pairing.
         *
         * @param cloneMe the face pairing to clone.
         */
        NFacePairing(const NFacePairing& cloneMe);

        /**
         * Creates the face pairing of given triangulation.  This is the
         * face pairing that describes how the tetrahedron faces of the
         * given triangulation are joined together, as described in the
         * class notes.
         *
         * \pre The given triangulation is not empty.
         *
         * @param tri the triangulation whose face pairing should be
         * constructed.
         */
        NFacePairing(const NTriangulation& tri);

        /**
         * Deallocates any memory used by this structure.
         */
        virtual ~NFacePairing();

        /*@}*/
        /**
         * (end: Constructors and Destructors)
         */

        /**
         * \name Basic Queries
         */
        /*@{*/

        /**
         * Returns the number of tetrahedra whose faces are (potentially)
         * paired in this particular matching.
         *
         * @return the number of tetrahedra under consideration.
         */
        unsigned getNumberOfTetrahedra() const;

        /**
         * Returns the other face to which the given tetrahedron face is
         * paired.  If the given face is left deliberately unmatched, the value
         * returned will be boundary (as returned by
         * NTetFace::isBoundary()).
         *
         * \pre The given face is a real tetrahedron face (not boundary,
         * before-the-start or past-the-end).
         *
         * @param source the face under investigation.
         * @return the other face to which the given face is paired.
         */
        const NTetFace& dest(const NTetFace& source) const;

        /**
         * Returns the other face to which the given tetrahedron face is
         * paired.  If the given face is left deliberately unmatched, the value
         * returned will be boundary (as returned by
         * NTetFace::isBoundary()).
         *
         * @param tet the tetrahedron under investigation (this must be
         * strictly less than the total number of tetrahedra under
         * consideration).
         * @param face the face of the given tetrahedron under
         * investigation (between 0 and 3 inclusive).
         * @return the other face to which the given face is paired.
         */
        const NTetFace& dest(unsigned tet, unsigned face) const;

        /**
         * Returns the other face to which the given tetrahedron face is
         * paired.  This is a convenience operator whose behaviour is
         * identical to that of dest().
         *
         * If the given face is left deliberately unmatched, the value
         * returned will be boundary (as returned by NTetFace::isBoundary()).
         *
         * \pre The given face is a real tetrahedron face (not boundary,
         * before-the-start or past-the-end).
         *
         * @param source the face under investigation.
         * @return the other face to which the given face is paired.
         */
        const NTetFace& operator [](const NTetFace& source) const;

        /**
         * Determines whether the given tetrahedron face has been left
         * deliberately unmatched.
         *
         * \pre The given face is a real tetrahedron face (not boundary,
         * before-the-start or past-the-end).
         *
         * @param source the face under investigation.
         * @return \c true if the given face has been left unmatched, or
         * \c false if the given face is paired with some other face.
         */
        bool isUnmatched(const NTetFace& source) const;

        /**
         * Determines whether the given tetrahedron face has been left
         * deliberately unmatched.
         *
         * @param tet the tetrahedron under investigation (this must be
         * strictly less than the total number of tetrahedra under
         * consideration).
         * @param face the face of the given tetrahedron under
         * investigation (between 0 and 3 inclusive).
         * @return \c true if the given face has been left unmatched, or
         * \c false if the given face is paired with some other face.
         */
        bool isUnmatched(unsigned tet, unsigned face) const;

        /**
         * Determines whether this face pairing is closed.
         * A closed face pairing has no unmatched faces.
         */
        bool isClosed() const;

        /*@}*/
        /**
         * (end: Basic Queries)
         */

        /**
         * \name Isomorphic Representations
         */
        /*@{*/

        /**
         * Determines whether this face pairing is in canonical form,
         * i.e., is a minimal representative of its isomorphism class.
         *
         * Isomorphisms of face pairings correspond to relabellings of
         * tetrahedra and relabellings of the four faces within each
         * tetrahedron.
         *
         * Face pairings are ordered by lexicographical comparison of
         * <tt>dest(0,0)</tt>, <tt>dest(0,1)</tt>, ...,
         * <tt>dest(n-1,3)</tt>, where <tt>n</tt> is the value of
         * <tt>getNumberOfTetrahedra()</tt>.
         *
         * @return \c true if and only if this face pairing is in
         * canonical form.
         */
        bool isCanonical() const;

        /**
         * Fills the given list with the set of all combinatorial
         * automorphisms of this face pairing.
         *
         * An automorphism is a relabelling of the tetrahedra and/or a
         * renumbering of the four faces of each tetrahedron resulting
         * in precisely the same face pairing.
         *
         * This routine uses optimisations that can cause unpredictable
         * breakages if this face pairing is not in canonical form.
         *
         * The automorphisms placed in the given list will be newly
         * created; it is the responsibility of the caller of this
         * routine to deallocate them.
         *
         * \pre The given list is empty.
         * \pre This face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre This face pairing is in canonical form as described by
         * isCanonical().
         *
         * \ifacespython Not present.
         *
         * @param list the list into which the newly created automorphisms
         * will be placed.
         */
        void findAutomorphisms(NFacePairingIsoList& list) const;

        /*@}*/
        /**
         * (end: Isomorphic Representations)
         */

        /**
         * \name Input and Output
         */
        /*@{*/

        /**
         * Returns a human-readable representation of this face pairing.
         *
         * The string returned will contain no newlines.
         *
         * @return a string representation of this pairing.
         */
        std::string toString() const;

        /**
         * Returns a text-based representation of this face pairing that can be
         * used to reconstruct the face pairing.  This reconstruction is
         * done through routine fromTextRep().
         *
         * The text produced is not particularly readable; for a
         * human-readable text representation, see routine toString()
         * instead.
         *
         * The string returned will contain no newlines.
         *
         * @return a text-based representation of this face pairing.
         */
        std::string toTextRep() const;

        /**
         * Writes the graph corresponding to this face pairing in
         * the Graphviz DOT language.  Every vertex of this graph
         * represents a tetrahedron, and every edge represents a pair of
         * tetrahedron faces that are joined together.  Note that for a
         * closed triangulation this graph will be entirely 4-valent;
         * for triangulations with boundary faces, some graph vertices
         * will have degree three or less.
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
         * begin with the string \c cluster_, so that Graphviz identifies
         * it as a cluster accordingly.
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
         *
         * @see http://www.graphviz.org/
         */
        void writeDot(std::ostream& out, const char* prefix = 0,
            bool subgraph = false) const;

        /*@}*/
        /**
         * (end: Input and Output)
         */

        /**
         * Reconstructs a face pairing from a text-based representation.
         * This text-based representation must be in the format produced
         * by routine toTextRep().
         *
         * The face pairing returned will be newly constructed; it is the
         * responsibility of the caller of this routine to deallocate it.
         *
         * \pre The face pairing to be reconstructed involves at least
         * one tetrahedron.
         *
         * @param rep a text-based representation of a face pairing, as
         * produced by routine toTextRep().
         * @return the corresponding newly constructed face pairing, or
         * \c null if the given text-based representation was invalid.
         */
        static NFacePairing* fromTextRep(const std::string& rep);

        /**
         * Writes header information for a Graphviz DOT file that will
         * describe the graphs for one or more face pairings.  See
         * the writeDot() documentation for further information on such
         * graphs.
         *
         * The output will be in the Graphviz DOT language, and will
         * include appropriate display settings for graphs, edges and
         * nodes.  The opening brace for a \c graph section of the DOT
         * file is included.
         *
         * This routine may be used with writeDot() to generate a single
         * DOT file containing the graphs for several different face
         * pairings.  A complete DOT file can be produced by calling
         * this routine, then calling writeDot() in subgraph mode for
         * each face pairing, then outputting a final closing curly brace.
         *
         * Note that if you require a DOT file containing the graph for
         * only a single face pairing, this routine is unnecessary; you
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
         * \name Subgraph Queries
         */
        /*@{*/

        /**
         * Follows a chain as far as possible from the given point.
         *
         * A chain is the underlying face pairing for a layered chain;
         * specifically it involves one tetrahedron joined to a second
         * along two faces, the remaining two faces of the second
         * tetrahedron joined to a third and so on.  A chain can involve
         * as few as one tetrahedron or as many as we like.  Note that
         * the remaining two faces of the first tetrahedron and the
         * remaining two faces of the final tetrahedron remain
         * unaccounted for by this structure.
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
         * If the face pairing forms a
         * large loop with each tetrahedron joined by two faces to the
         * next, this routine will cycle around the loop forever and
         * never return.
         *
         * @param tet the index in the underlying triangulation of the
         * tetrahedron to begin at.  This parameter will be modified
         * directly by this routine as a way of returning the results.
         * @param faces the pair of face numbers in the given
         * tetrahedron at which we begin.  This parameter will also be
         * modified directly by this routine as a way of returning results.
         */
        void followChain(unsigned& tet, NFacePair& faces) const;

        /**
         * Determines whether this face pairing contains a triple edge.
         * A triple edge is where two different tetrahedra are joined
         * along three of their faces.
         *
         * A face pairing containing a triple edge cannot model a closed
         * minimal irreducible P^2-irreducible 3-manifold triangulation on
         * more than two tetrahedra.  See "Face pairing graphs and 3-manifold
         * enumeration", Benjamin A. Burton, J. Knot Theory Ramifications
         * 13 (2004), 1057--1101.
         *
         * @return \c true if and only if this face pairing contains a
         * triple edge.
         */
        bool hasTripleEdge() const;

        /**
         * Determines whether this face pairing contains a broken
         * double-ended chain.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * face pairing for a layered solid torus).  A double-ended
         * chain is a chain in which the first tetrahedron is joined to
         * itself along one face and the final tetrahedron is also
         * joined to itself along one face (i.e., the underlying
         * face pairing for a layered lens space).
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
         * A face pairing containing a broken double-ended chain cannot
         * model a closed minimal irreducible P^2-irreducible 3-manifold
         * triangulation on more than two tetrahedra.  See "Face pairing
         * graphs and 3-manifold enumeration", Benjamin A. Burton,
         * J. Knot Theory Ramifications 13 (2004), 1057--1101.
         *
         * @return \c true if and only if this face pairing contains a
         * broken double-ended chain that is not part of a complete
         * double-ended chain.
         */
        bool hasBrokenDoubleEndedChain() const;

        /**
         * Determines whether this face pairing contains a one-ended chain
         * with a double handle.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * face pairing for a layered solid torus).
         *
         * A one-ended chain with a double handle begins with a one-ended
         * chain.  The two faces that are unaccounted for by this
         * one-ended chain must be joined
         * to two different tetrahedra, and these two tetrahedra must be
         * joined to each other along two faces.  The remaining two
         * faces of these two tetrahedra remain unaccounted for by this
         * structure.
         *
         * A face pairing containing a one-ended chain with a double handle
         * cannot model a closed minimal irreducible P^2-irreducible
         * 3-manifold triangulation on more than two tetrahedra.  See
         * "Face pairing graphs and 3-manifold enumeration",
         * Benjamin A. Burton, J. Knot Theory Ramifications 13 (2004),
         * 1057--1101.
         *
         * @return \c true if and only if this face pairing contains a
         * one-ended chain with a double handle.
         */
        bool hasOneEndedChainWithDoubleHandle() const;

        /**
         * Determines whether this face pairing contains a wedged
         * double-ended chain.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * face pairing for a layered solid torus).  A double-ended
         * chain is a chain in which the first tetrahedron is joined to
         * itself along one face and the final tetrahedron is also
         * joined to itself along one face (i.e., the underlying
         * face pairing for a layered lens space).
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
         * A face pairing containing a wedged double-ended chain
         * cannot model a closed minimal irreducible P^2-irreducible
         * 3-manifold triangulation on more than two tetrahedra.  See
         * "Enumeration of non-orientable 3-manifolds using face pairing
         * graphs and union-find", Benjamin A. Burton, math.GT/0604584,
         * to appear in Discrete and Comput. Geom.
         *
         * @return \c true if and only if this face pairing contains a
         * wedged double-ended chain.
         */
        bool hasWedgedDoubleEndedChain() const;

        /**
         * Determines whether this face pairing contains a one-ended
         * chain with a stray bigon.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * face pairing for a layered solid torus).
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
         * A face pairing containing a structure of this type
         * cannot model a closed minimal irreducible P^2-irreducible
         * 3-manifold triangulation on more than two tetrahedra.  See
         * "Enumeration of non-orientable 3-manifolds using face pairing
         * graphs and union-find", Benjamin A. Burton, math.GT/0604584,
         * to appear in Discrete and Comput. Geom.
         *
         * @return \c true if and only if this face pairing contains a
         * one-ended chain with a stray bigon.
         */
        bool hasOneEndedChainWithStrayBigon() const;

        /**
         * Determines whether this face pairing contains a triple
         * one-ended chain.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * face pairing for a layered solid torus).
         *
         * A triple one-ended chain is created from three one-ended
         * chains as follows.  Two new tetrahedra are added, and each
         * one-ended chain is joined to each of the new tetrahedra along
         * a single face.  The remaining two faces (one from each of the
         * new tetrahedra) remain unaccounted for by this structure.
         *
         * A face pairing containing a triple one-ended chain
         * cannot model a closed minimal irreducible P^2-irreducible
         * 3-manifold triangulation on more than two tetrahedra.  See
         * "Enumeration of non-orientable 3-manifolds using face pairing
         * graphs and union-find", Benjamin A. Burton, math.GT/0604584,
         * to appear in Discrete and Comput. Geom.
         *
         * @return \c true if and only if this face pairing contains a
         * triple one-ended chain.
         */
        bool hasTripleOneEndedChain() const;

        /**
         * Determines whether this face pairing contains a single-edged
         * star.
         *
         * A single-edged star involves two tetrahedra that are adjacent
         * along a single face, where the six remaining faces of these
         * tetrahedra are joined to six entirely new tetrahedra (so that
         * none of the eight tetrahedra described in this structure are
         * the same).
         *
         * @return \c true if and only if this face pairing contains a
         * single-edged star.
         */
        bool hasSingleStar() const;

        /**
         * Determines whether this face pairing contains a double-edged
         * star.
         *
         * A double-edged star involves two tetrahedra that are adjacent
         * along two separate pairs of faces, where the four remaining
         * faces of these tetrahedra are joined to four entirely new
         * tetrahedra (so that none of the six tetrahedra described in
         * this structure are the same).
         *
         * @return \c true if and only if this face pairing contains a
         * double-edged star.
         */
        bool hasDoubleStar() const;

        /**
         * Determines whether this face pairing contains a double-edged
         * square.
         *
         * A double-edged square involves four distinct tetrahedra that
         * meet each other as follows.  Two pairs of tetrahedra are
         * joined along two pairs of faces each.  Then each tetrahedron
         * is joined along a single face to one tetrahedron of the other
         * pair.  The four tetrahedron faces not yet joined to anything
         * (one from each tetrahedron) remain unaccounted for by this
         * structure.
         *
         * @return \c true if and only if this face pairing contains a
         * double-edged square.
         */
        bool hasDoubleSquare() const;

        /*@}*/
        /**
         * (end: Subgraph Queries)
         */

        /**
         * \name Graph Enumeration
         */
        /*@{*/

        /**
         * Internal to findAllPairings().  This routine should never be
         * called directly.
         *
         * Performs the actual generation of face pairings, possibly as a
         * separate thread.  At most one copy of this routine should be
         * running at any given time for a particular NFacePairing instance.
         *
         * \ifacespython Not present.
         *
         * @param param a structure containing the parameters that were
         * passed to findAllPairings().
         * @return the value 0.
         */
        void* run(void* param);

        /*@}*/
        /**
         * (end: Graph Enumeration)
         */

        /**
         * Generates all possible face pairings satisfying the given
         * constraints.  Only connected face pairings (pairings in which
         * each tetrahedron can be reached from each other via a series of
         * individual matched faces) will be produced.
         *
         * Each face pairing will be produced precisely once up to
         * isomorphism.  Face pairings are considered isomorphic if they
         * are related by a relabelling of the tetrahedra and/or a
         * renumbering of the four faces of each tetrahedron.  Each face
         * pairing that is generated will be a minimal representative of
         * its isomorphism class, i.e., will be in canonical form as
         * described by isCanonical().
         *
         * For each face pairing that is generated, routine \a use (as
         * passed to this function) will be called with that pairing and
         * its automorphisms as arguments.
         *
         * Once the generation of face pairings has finished, routine
         * \a use will be called once more, this time with \c null as its
         * first two arguments (the face pairing and its automorphisms).
         *
         * The face pairing generation may be run in the current thread
         * or as a separate thread.
         *
         * \todo \optlong When generating face pairings, do some checking to
         * eliminate cases in which tetrahedron (<i>k</i> > 0) can be swapped
         * with tetrahedron 0 to produce a smaller representation of the same
         * pairing.
         * \todo \feature Allow cancellation of face pairing generation.
         *
         * \ifacespython Not present.
         *
         * @param nTetrahedra the number of tetrahedra whose faces should
         * be (potentially) matched.
         * @param boundary determines whether any faces may be left
         * unmatched.  This set should contain \c true if pairings with at
         * least one unmatched face are to be generated, and should contain
         * \c false if pairings with no unmatched faces are to be generated.
         * @param nBdryFaces specifies the precise number of faces that
         * should be left unmatched.  If this parameter is negative, it
         * is ignored and no additional restriction is imposed.  If
         * parameter \a boundary does not contain \c true, this parameter
         * is likewise ignored.  If parameter \a boundary does contain
         * true and this parameter is non-negative, only pairings with
         * precisely this many unmatched faces will be generated.
         * In particular, if this parameter is positive then pairings
         * with no unmatched faces will not be produced irrespective of
         * whether \c false is contained in parameter \a boundary.
         * Note that, in order to produce any pairings at all, this
         * parameter must be even and at most 2<i>T</i>+2, where <i>T</i>
         * is the number of tetrahedra requested.
         * @param use the function to call upon each face pairing that is
         * found.  The first parameter passed to this function will be a
         * face pairing.  The second parameter will be a list of all its
         * automorphisms (relabellings of tetrahedra and individual
         * tetrahedron faces that produce the exact same pairing).
         * The third parameter will be parameter \a useArgs as was passed
         * to this routine.
         * @param useArgs the pointer to pass as the final parameter for
         * the function \a use which will be called upon each pairing found.
         * @param newThread \c true if face pairing generation should be
         * performed in a separate thread, or \c false if generation
         * should take place in the current thread.  If this parameter is
         * \c true, this routine will exit immediately (after spawning
         * the new thread).
         * @return \c true if the new thread was successfully started (or
         * if face pairing generation has taken place in the current thread),
         * or \c false if the new thread could not be started.
         */
        static bool findAllPairings(unsigned nTetrahedra,
            NBoolSet boundary, int nBdryFaces, UseFacePairing use,
            void* useArgs = 0, bool newThread = false);

    private:
        /**
         * Creates a new face pairing.  All internal arrays will be
         * allocated but not initialised.
         *
         * \pre \a newNTetrahedra is at least 1.
         *
         * @param newTetrahedra the number of tetrahedra under
         * consideration in this new face pairing.
         */
        NFacePairing(unsigned newNTetrahedra);

        /**
         * Returns the other face to which the given tetrahedron face is
         * paired.  If the given face is left deliberately unmatched, the value
         * returned will be boundary (as returned by
         * NTetFace::isBoundary()).
         *
         * \pre The given face is a real tetrahedron face (not boundary,
         * before-the-start or past-the-end).
         *
         * @param source the face under investigation.
         * @return the other face to which the given face is paired.
         */
        NTetFace& dest(const NTetFace& source);

        /**
         * Returns the other face to which the given tetrahedron face is
         * paired.  If the given face is left deliberately unmatched, the value
         * returned will be boundary (as returned by
         * NTetFace::isBoundary()).
         *
         * @param tet the tetrahedron under investigation (this must be
         * strictly less than the total number of tetrahedra under
         * consideration).
         * @param face the face of the given tetrahedron under
         * investigation (between 0 and 3 inclusive).
         * @return the other face to which the given face is paired.
         */
        NTetFace& dest(unsigned tet, unsigned face);

        /**
         * Returns the other face to which the given tetrahedron face is
         * paired.  This is a convenience operator whose behaviour is
         * identical to that of dest().
         *
         * If the given face is left deliberately unmatched, the value
         * returned will be boundary (as returned by NTetFace::isBoundary()).
         *
         * \pre The given face is a real tetrahedron face (not boundary,
         * before-the-start or past-the-end).
         *
         * @param source the face under investigation.
         * @return the other face to which the given face is paired.
         */
        NTetFace& operator [](const NTetFace& source);

        /**
         * Determines whether the matching for the given tetrahedron face
         * has not yet been determined.  This is signalled by a face
         * matched to itself.
         *
         * \pre The given face is a real tetrahedron face (not boundary,
         * before-the-start or past-the-end).
         *
         * @param source the face under investigation.
         * @return \c true if the matching for the given face has not yet
         * been determined, or \c false otherwise.
         */
        bool noDest(const NTetFace& source) const;

        /**
         * Determines whether the matching for the given tetrahedron face
         * has not yet been determined.  This is signalled by a face
         * matched to itself.
         *
         * @param tet the tetrahedron under investigation (this must be
         * strictly less than the total number of tetrahedra under
         * consideration).
         * @param face the face of the given tetrahedron under
         * investigation (between 0 and 3 inclusive).
         * @return \c true if the matching for the given face has not yet
         * been determined, or \c false otherwise.
         */
        bool noDest(unsigned tet, unsigned face) const;

        /**
         * Determines whether this face pairing is in canonical
         * (smallest lexicographical) form, given a small set of
         * assumptions.
         *
         * If this face pairing is in canonical form, the given list
         * will be filled with the set of all combinatorial automorphisms
         * of this face pairing.  If not, the given list will be left empty.
         *
         * \pre The given list is empty.
         * \pre For each tetrahedron \a t, it is true that
         * the sequence <tt>dest(t,0)</tt>, <tt>dest(t,1)</tt>,
         * <tt>dest(t,2)</tt>, <tt>dest(t,3)</tt> is non-decreasing.
         * \pre For each tetrahedron \a t > 0, it is true that
         * <tt>dest(t,0).tet < t</tt>.
         * \pre The sequence <tt>dest(1,0)</tt>, <tt>dest(2,0)</tt>,
         * ..., <tt>dest(n-1,0)</tt> is strictly increasing, where
         * \a n is the total number of tetrahedra under investigation.
         *
         * @param list the list into which automorphisms will be placed
         * if appropriate.
         * @return \c true if and only if this face pairing is in
         * canonical form.
         */
        bool isCanonicalInternal(NFacePairingIsoList& list) const;

        /**
         * Internal to hasBrokenDoubleEndedChain().  This routine assumes
         * that the give face of the given tetrahedron is glued to this
         * same tetrahedron, and attempts to find a broken double-ended
         * chain for which this tetrahedron is at the end of one of the
         * one-ended chains therein.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this face pairing.
         *
         * @param tet the index in the triangulation of the given
         * tetrahedron.
         * @param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * @return \c true if and only if this face pairing contains a
         * broken double-ended chain as described above.
         */
        bool hasBrokenDoubleEndedChain(unsigned tet, unsigned face) const;

        /**
         * Internal to hasOneEndedChainWithDoubleHandle().  This routine
         * assumes that the give face of the given tetrahedron is glued
         * to this same tetrahedron, and attempts to find a one-ended
         * chain with a double handle for which this tetrahedron is at
         * the end of the chain contained therein.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this face pairing.
         *
         * @param tet the index in the triangulation of the given
         * tetrahedron.
         * @param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * @return \c true if and only if this face pairing contains a
         * one-ended chain with a double handle as described above.
         */
        bool hasOneEndedChainWithDoubleHandle(unsigned tet,
            unsigned face) const;

        /**
         * Internal to hasWedgedDoubleEndedChain().  This routine assumes
         * that the give face of the given tetrahedron is glued to this
         * same tetrahedron, and attempts to find a wedged double-ended
         * chain for which this tetrahedron is at one end of the
         * double-ended chain.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this face pairing.
         *
         * @param tet the index in the triangulation of the given
         * tetrahedron.
         * @param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * @return \c true if and only if this face pairing contains a
         * wedged double-ended chain as described above.
         */
        bool hasWedgedDoubleEndedChain(unsigned tet, unsigned face) const;

        /**
         * Internal to hasOneEndedChainWithStrayBigon().  This routine assumes
         * that the give face of the given tetrahedron is glued to this
         * same tetrahedron, and attempts to find a one-ended chain with
         * stray bigon for which this tetrahedron is at the end of the
         * one-ended chain.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this face pairing.
         *
         * @param tet the index in the triangulation of the given
         * tetrahedron.
         * @param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * @return \c true if and only if this face pairing contains a
         * one-ended chain with stray bigon as described above.
         */
        bool hasOneEndedChainWithStrayBigon(unsigned tet, unsigned face) const;

        /**
         * Internal to hasTripleOneEndedChain().  This routine assumes
         * that the give face of the given tetrahedron is glued to this
         * same tetrahedron, and attempts to find a triple one-ended
         * chain for which this tetrahedron is at the end of one of the
         * individual one-ended chains.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this face pairing.
         *
         * @param tet the index in the triangulation of the given
         * tetrahedron.
         * @param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * @return \c true if and only if this face pairing contains a
         * triple one-ended chain as described above.
         */
        bool hasTripleOneEndedChain(unsigned tet, unsigned face) const;
};

/*@}*/

// Inline functions for NFacePairing

inline NFacePairing::NFacePairing(unsigned newNTetrahedra) :
        nTetrahedra(newNTetrahedra), pairs(new NTetFace[newNTetrahedra * 4]) {
}

inline NFacePairing::~NFacePairing() {
    delete[] pairs;
}

inline unsigned NFacePairing::getNumberOfTetrahedra() const {
    return nTetrahedra;
}

inline NTetFace& NFacePairing::dest(const NTetFace& source) {
    return pairs[4 * source.tet + source.face];
}
inline NTetFace& NFacePairing::operator [](const NTetFace& source) {
    return pairs[4 * source.tet + source.face];
}
inline NTetFace& NFacePairing::dest(unsigned tet, unsigned face) {
    return pairs[4 * tet + face];
}
inline const NTetFace& NFacePairing::dest(const NTetFace& source) const {
    return pairs[4 * source.tet + source.face];
}
inline const NTetFace& NFacePairing::operator [](const NTetFace& source)
        const {
    return pairs[4 * source.tet + source.face];
}
inline const NTetFace& NFacePairing::dest(unsigned tet, unsigned face) const {
    return pairs[4 * tet + face];
}
inline bool NFacePairing::isUnmatched(const NTetFace& source) const {
    return pairs[4 * source.tet + source.face].isBoundary(nTetrahedra);
}
inline bool NFacePairing::isUnmatched(unsigned tet, unsigned face) const {
    return pairs[4 * tet + face].isBoundary(nTetrahedra);
}

inline bool NFacePairing::noDest(const NTetFace& source) const {
    return dest(source) == source;
}
inline bool NFacePairing::noDest(unsigned tet, unsigned face) const {
    NTetFace& f = pairs[4 * tet + face];
    return (f.tet == static_cast<int>(tet) &&
        f.face == static_cast<int>(face));
}

inline void NFacePairing::findAutomorphisms(NFacePairingIsoList& list) const {
    isCanonicalInternal(list);
}

} // namespace regina

#endif

