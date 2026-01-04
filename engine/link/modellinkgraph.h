
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

/*! \file link/modellinkgraph.h
 *  \brief Deals with model 4-valent graphs for knots and links.
 */

#ifndef __REGINA_MODELLINKGRAPH_H
#ifndef __DOXYGEN
#define __REGINA_MODELLINKGRAPH_H
#endif

#include <array>
#include <vector>
#include "core/output.h"
#include "utilities/exception.h"
#include "utilities/fixedarray.h"
#include "utilities/flags.h"
#include "utilities/listview.h"
#include "utilities/markedvector.h"
#include "utilities/tightencoding.h"

namespace regina {

class Link;
class ModelLinkGraph;
class ModelLinkGraphArc;
class ModelLinkGraphCells;
class ModelLinkGraphNode;
class StrandRef;

template <int dim> requires (supportedDim(dim)) class FacetPairing;

/**
 * Represents different classes of graph embeddings that one might want to
 * generate.  Specifically, this enumeration type is used with the routine
 * ModelLinkGraph::generateAllEmbeddings().
 *
 * These values can be combined using the bitwise OR operator, resulting in an
 * object of type `Flags<GraphConstraint>`.  If a graph generation function
 * takes an argument of type `Flags<GraphConstraint>`, then it will only
 * generate those graphs that satisfy _all_ of the constraints that have been
 * ORed together.  For such an argument, you can pass a single GraphConstraint
 * constant, or a bitwise combination of such constants `(flag1 | flag2)`, or
 * empty braces `{}` to indicate no flags at all (which is equivalent to
 * passing `GraphConstraint::All`).
 *
 * \ingroup link
 */
enum class GraphConstraint {
    /**
     * Indicates that all graph embeddings should be generated.
     */
    All = 0x00,
    /**
     * Indicates that only graph embeddings without twists should be generated.
     *
     * By a _twist_, we mean that the embedding has some node with two
     * adjacent arcs connected together.  An embedding that fails this
     * constraint must always model knot or links with twists that can be
     * undone using type I Reidemeister moves.
     */
    NoTwists = 0x01,
    /**
     * Indicates that only graph embeddings with a single traversal should be
     * generated.  That is, for every embedding \a e that is generated,
     * `e.countTraversals()` should be precisely 1.
     *
     * An embedding that satisfies this constraint must always model knots
     * (classical or virtual).  An embedding that fails this constraint
     * must either be empty, or must always model multiple-component links.
     */
    SingleTraversal = 0x02
};

/**
 * Returns the bitwise OR of the two given flags.
 *
 * \param lhs the first flag to combine.
 * \param rhs the second flag to combine.
 * \return the combination of both flags.
 *
 * \ingroup link
 */
inline Flags<GraphConstraint> operator | (GraphConstraint lhs,
        GraphConstraint rhs) {
    return Flags<GraphConstraint>(lhs) | rhs;
}

/**
 * A reference to an outgoing edge from a node of a model graph for a
 * knot or link.
 *
 * Edges of model graphs are not directed, and so the same edge will
 * appear twice as a ModelLinkGraphArc (once from each of its endpoints).
 *
 * This class is a simple wrapper that stores (i) a pointer to the
 * relevant node of the graph; and (ii) an integer to denote which of
 * the four outgoing arcs we are using from that node.  Recall that the
 * four outgoing arcs for each node are indexed in clockwise order.
 *
 * A "null arc" is one whose node is the null pointer.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \ingroup link
 */
class ModelLinkGraphArc {
    private:
        ModelLinkGraphNode* node_;
            /**< The node from which this arc exits.  This may be \c null. */
        int arc_;
            /**< An integer between 0 and 3 inclusive to indicate which
                 of the four outgoing arcs of \a node_ this represents. */

    public:
        /**
         * Initialises this to a null arc.
         *
         * The pointer returned by node() will be \c null,
         * and the integer returned by arc() will be 0.
         */
        ModelLinkGraphArc();

        /**
         * Initialises this to the given arc exiting the given node of a
         * model graph.
         *
         * Recall that the four arcs exiting a node are numbered 0,1,2,3
         * in a clockwise order around the node.
         *
         * The given node may be \c null, in which case this will
         * become a null arc.  If you are creating a null arc,
         * then it is highly recommended that you pass \a arc as 0 also,
         * so that comparison tests treat this null reference as equal to
         * a null reference created by the zero-argument constructor.
         *
         * \param node the node of the model graph that this arc exits.
         * \param arc an integer in the range 0 to 3 inclusive,
         * indicating which of the four arcs exiting \a node this represents.
         */
        ModelLinkGraphArc(ModelLinkGraphNode* node, int arc);

        /**
         * Creates a new copy of the given arc reference.
         */
        ModelLinkGraphArc(const ModelLinkGraphArc&) = default;

        /**
         * The node of the model graph from which this arc exits.
         *
         * \return the corresponding node, or \c null if this is a null arc.
         */
        ModelLinkGraphNode* node() const;

        /**
         * Indicates which arc this is amongst the four arcs exiting the
         * underlying node of the model graph.
         *
         * For each node of a model graph, the four arcs exiting that
         * node are numbered 0,1,2,3 in a clockwise order.
         *
         * \return an integer between 0 and 3 inclusive indicating one
         * of the four arcs exiting node().
         */
        int arc() const;

        /**
         * Tests whether this and the given arc reference are identical.
         *
         * Two references are identical if and only if they return the
         * same values for both node() and arc().
         *
         * \warning If you create a null arc by calling
         * ModelLinkGraphArc(\c null, \a i) for some non-zero \a i, then this
         * will _not_ be considered equal to the null arc created by calling
         * ModelLinkGraphArc(), since the latter is equivalent to calling
         * ModelLinkGraphArc(\c null, 0).
         *
         * \return \c true if and only if this and the given arc reference
         * are identical.
         */
        bool operator == (const ModelLinkGraphArc&) const = default;
        /**
         * Sets this to be a copy of the given arc reference.
         *
         * \return a reference to this object.
         */
        ModelLinkGraphArc& operator = (const ModelLinkGraphArc&) = default;

        /**
         * Returns the arc that exits the same node as this, but from
         * the opposite side.
         *
         * Recall that the four arcs exiting each node are numbered in
         * clockwise order.  The return value will therefore have the
         * same node() as this, but its arc() value will be two more than
         * this (modulo 4).
         *
         * Note that, for any arc \a a,
         * `a.opposite().opposite()` is identical to \a a.
         *
         * \pre This is not a null arc, i.e., node() does not
         * return \c null.
         *
         * \return the opposite arc exiting the same node.
         */
        ModelLinkGraphArc opposite() const;

        /**
         * Returns the same edge of the model graph, but seen from the
         * other endpoint.
         *
         * Recall that each undirected edge of a model graph has two
         * corresponding ModelLinkGraphArc objects, one for each of its
         * endpoints.  If this object represents one of these arcs for
         * some underlying edge of the graph, then then return value
         * represents the other.
         *
         * Note that, for any arc \a a,
         * `a.traverse().traverse()` is identical to \a a.
         *
         * \pre This is not a null arc, i.e., node() does not return \c null.
         *
         * \return the arc at the other end of the underlying edge of the
         * model graph.
         */
        ModelLinkGraphArc traverse() const;

        /**
         * Returns the next arc after this when walking through the graph as
         * though it were a link, in a direction away from the current node.
         *
         * This routine will move to the other endpoint of the graph edge
         * described by this arc, and will then return the \a opposite
         * arc at the resulting node (i.e., not just pointing backwards along
         * the same edge).
         *
         * For any arc \a a, calling `a.next()` is equivalent to
         * calling `a.traverse().opposite()`.
         *
         * \pre This is not a null arc, i.e., node() does not return \c null.
         *
         * \return the next arc after this when walking through the graph as
         * though it were a link.
         */
        ModelLinkGraphArc next() const;
        /**
         * Returns the previous arc before this when walking through the graph
         * as though it were a link, in a direction away from the current node.
         *
         * This routine will jump to the opposite arc at the current node,
         * and then move to the other endpoint of the graph edge described by
         * that opposite arc.
         *
         * For any arc \a a, calling `a.prev()` is equivalent to
         * calling `a.opposite().traverse()`.
         *
         * \pre This is not a null arc, i.e., node() does not return \c null.
         *
         * \return the previous arc before this when walking through the graph
         * as though it were a link.
         */
        ModelLinkGraphArc prev() const;

        /**
         * Changes to the next outgoing link arc from the same node.
         *
         * This effectively rotates the arc in a clockwise direction
         * around the node.  In particular, it increments the value
         * returned by arc(), modulo 4.
         *
         * This is a preincrement operator: the object will be changed,
         * and then a reference to it will be returned.
         *
         * \pre This is not a null arc, i.e., node() does not
         * return \c null.
         *
         * \nopython The postincrement operator is available in Python
         * under the name inc().
         *
         * \return a reference to this object.
         */
        ModelLinkGraphArc& operator ++ ();
        /**
         * Changes to the next outgoing link arc from the same node.
         *
         * This effectively rotates the arc in a clockwise direction
         * around the node.  In particular, it increments the value
         * returned by arc(), modulo 4.
         *
         * This is a postincrement operator: the object will be changed,
         * but a copy of the original arc will be returned.
         *
         * \pre This is not a null arc, i.e., node() does not
         * return \c null.
         *
         * \python This routine is available under the name inc().
         *
         * \return a copy of this object before the change took place.
         */
        ModelLinkGraphArc operator ++ (int);
        /**
         * Changes to the previous outgoing link arc from the same node.
         *
         * This effectively rotates the arc in an anticlockwise direction
         * around the node.  In particular, it decrements the value
         * returned by arc(), modulo 4.
         *
         * This is a predecrement operator: the object will be changed,
         * and then a reference to it will be returned.
         *
         * \pre This is not a null arc, i.e., node() does not
         * return \c null.
         *
         * \nopython The postdecrement operator is available in Python
         * under the name dec().
         *
         * \return a reference to this object.
         */
        ModelLinkGraphArc& operator -- ();
        /**
         * Changes to the previous outgoing link arc from the same node.
         *
         * This effectively rotates the arc in an anticlockwise direction
         * around the node.  In particular, it decrements the value
         * returned by arc(), modulo 4.
         *
         * This is a postdecrement operator: the object will be changed,
         * but a copy of the original arc will be returned.
         *
         * \pre This is not a null arc, i.e., node() does not
         * return \c null.
         *
         * \python This routine is available under the name dec().
         *
         * \return a copy of this object before the change took place.
         */
        ModelLinkGraphArc operator -- (int);

        /**
         * Tests whether this is a non-null arc.
         *
         * \return \c true if this is not a null arc (i.e., node()
         * does not return a null pointer), or \c false if this is a null
         * arc.
         */
        operator bool() const;

    friend class ModelLinkGraph;
};

/**
 * Writes a depiction of the given arc reference to the given output stream.
 *
 * \param out the output stream to which to write.
 * \param a the arc reference to write.
 * \return a reference to the given output stream.
 *
 * \ingroup link
 */
std::ostream& operator << (std::ostream& out, const ModelLinkGraphArc& a);

/**
 * Represents a single node in a model graph for a knot or link.
 *
 * If a graph has \a n nodes, then these are numbered 0,...,<i>n</i>-1.
 * The number assigned to this node can be accessed by calling index().
 * Note that nodes may be reindexed when other nodes are added or removed -
 * if you wish to track a particular node through such operations then you
 * should use a pointer to the relevant ModelLinkGraphNode instead.
 *
 * Graph nodes do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the ModelLinkGraph to which they belong.
 *
 * \ingroup link
 */
class ModelLinkGraphNode : public MarkedElement,
        public ShortOutput<ModelLinkGraphNode> {
    private:
        std::array<ModelLinkGraphArc, 4> adj_;
            /**< Stores the arcs at the _other_ endpoints of the four
                 graph edges that exit this node.  The four arc references
                 are stored in a clockwise order around this node. */

    public:
        /**
         * Returns the index of this node within the overall graph.
         * If the graph contains \a n nodes, then the index will be a
         * number between 0 and <i>n</i>-1 inclusive.
         *
         * \warning The index of this node might change if other
         * nodes are added or removed.
         *
         * \return the index of this node.
         */
        size_t index() const;
        /**
         * Returns a reference to one of the four arcs of the graph that
         * exit this node.  This is equivalent to directly constructing
         * ModelLinkGraphArc(\c this, \a which).
         *
         * The four arcs exiting this node are numbered 0,1,2,3 in a
         * clockwise order around the node.
         *
         * \param which an integer in the range 0 to 3 inclusive, indicating
         * which of the four arcs exiting this node we should return.
         * \return a reference to the corresponding arc exiting this node.
         */
        ModelLinkGraphArc arc(int which);
        /**
         * Returns the arc at the other end of the given graph edge that
         * exits this node.
         *
         * Let \a e be the undirected edge of the underlying model graph that
         * corresponds to the given outgoing arc from this node.
         * Recall that there are two ModelLinkGraphArc objects corresponding
         * to \a e, one for each of its endpoints.  One of these will
         * be ModelLinkGraphArc(\c this, \a which); this routine returns the
         * _other_ object, which is the ModelLinkGraphArc describing the other
         * endpoint of \a e.
         *
         * Note that for a node \a n, calling `n.adj(i)`
         * is equivalent to calling `n.arc(i).traverse()`.
         *
         * \param which an integer in the range 0 to 3 inclusive, indicating
         * which of the four arcs exiting this node we should examine.
         * \return a reference to the other end of the same undirected
         * edge of the underlying model graph.
         */
        const ModelLinkGraphArc& adj(int which) const;

        /**
         * Returns the number of loops incident with this node.
         *
         * Regarding loops versus 1-gons:
         *
         * - For a planar 4-valent graph (i.e., a graph that models a classical
         *   link diagram), every loop bounds a 1-gon in the dual cell
         *   decomposition, and vice versa.  In particular, for a planar graph,
         *   at every node we have `0 ≤ monogons() == loops() ≤ 2`.
         *
         * - For a non-planar graph (which could be used to model a virtual
         *   link diagram), there could be loops that do not bound 1-gons.
         *   So, for a non-planar graph, the only guarantee we have at each
         *   node is that `0 ≤ monogons() ≤ loops() ≤ 2`.
         *
         * \return The number of incident loops, which will be between 0 and 2
         * inclusive.
         */
        int loops() const;

        /**
         * Returns the number of 1-gons in the dual cell decomposition that
         * are incident with this node.
         *
         * Regarding loops versus 1-gons:
         *
         * - For a planar 4-valent graph (i.e., a graph that models a classical
         *   link diagram), every loop bounds a 1-gon in the dual cell
         *   decomposition, and vice versa.  In particular, for a planar graph,
         *   at every node we have `0 ≤ monogons() == loops() ≤ 2`.
         *
         * - For a non-planar graph (which could be used to model a virtual
         *   link diagram), there could be loops that do not bound 1-gons.
         *   So, for a non-planar graph, the only guarantee we have at each
         *   node is that `0 ≤ monogons() ≤ loops() ≤ 2`.
         *
         * \return The number of incident 1-gons, which will be between 0 and 2
         * inclusive.
         */
        int monogons() const;

        /**
         * Returns the number of embedded bigons in the dual cell
         * decomposition that are incident with this node.
         *
         * Here _embedded_ means that we do not count bigons where both
         * vertices are the same.  Note that a _non-embedded_ incident bigon
         * would imply that all four arcs at this node were joined together to
         * form two loops, each bounding its own 1-gon (which models a
         * 1-crossing unknot component of a link diagram).
         *
         * \return The number of incident embedded bigons, which will be
         * between 0 and 4 inclusive.
         */
        int bigons() const;

        /**
         * Returns the number of embedded triangles in the dual cell
         * decomposition that are incident with this node.
         *
         * Here _embedded_ means that we do not count triangles where two
         * vertices are the same.  Note that a _non-embedded_ incident triangle
         * would imply that the underlying graph contains a loop bounding a
         * 1-gon (which models a trivial twist in a link diagram).
         *
         * \return The number of incident embedded triangles, which will be
         * between 0 and 4 inclusive.
         */
        int triangles() const;

        /**
         * Writes a short text representation of this node to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        // Make this class non-copyable.
        ModelLinkGraphNode(const ModelLinkGraphNode&) = delete;
        ModelLinkGraphNode& operator = (const ModelLinkGraphNode&) = delete;

    private:
        /**
         * Initialises the \a adj_ array to contain null arcs.
         */
        ModelLinkGraphNode() = default;

    friend class ModelLinkGraph;
};

/**
 * Represents an undirected 4-valent graph with a specific embedding in some
 * closed orientable surface.  This class only stores the graph and a local
 * description of the embedding (i.e., a cyclic ordering of arcs around each
 * node).  It does not store the surface explicitly, though the surface is
 * implied from the embedding - if you need it you can always access a full
 * description of the surface by calling cells().
 *
 * In particular, the surface is assumed to be the minimal genus surface in
 * which the graph embeds.  Each connected component of the graph is embedded
 * in a separate connected component of the surface, and each component of the
 * surface is formed from a collection of discs (or _cells_) whose boundaries
 * follow the nodes and arcs of the graph according to the local embedding.
 *
 * Regina uses graphs like these as model graphs for classical or virtual link
 * diagrams, where each node of the graph becomes a classical crossing.
 * If the surface is a collection of 2-spheres, then the graph is planar and
 * models a _classical_ link diagram.  If the surface has genus, then the
 * graph is non-planar and instead models a _virtual_ link diagram.
 *
 * Currently this class does not support circular graph components (which,
 * in a link diagram, would correspond to zero-crossing unknot components
 * of the link).
 *
 * For Boost users: if you wish to study the underlying graph of an existing
 * link, you do not need to create a ModelLinkGraph - instead you can include
 * link/graph.h and then use Link directly as a directed graph type with the
 * Boost Graph Library.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup link
 */
class ModelLinkGraph :
        public Output<ModelLinkGraph>,
        public TightEncodable<ModelLinkGraph> {
    private:
        MarkedVector<ModelLinkGraphNode> nodes_;
            /**< The nodes of this graph. */
        mutable ssize_t nComponents_;
            /**< The number of connected components of this graph, or -1 if
                 this has not yet been computed. */
        ModelLinkGraphCells* cells_;
            /**< The induced cellular decomposition of the surface in which
                 the graph embeds, or \c null if this has not yet been
                 computed. */

    public:
        /**
         * Constructs an empty graph.
         */
        ModelLinkGraph();
        /**
         * Constructs the graph that models the given link.
         *
         * Any zero-component unknot components of the link will be ignored.
         *
         * The nodes of this graph will be numbered in the same way as
         * the crossings of \a link.  For each node, arc 0 will represent
         * the outgoing lower strand of the corresponding crossing.
         *
         * Using this constructor is identical to calling Link::graph().
         *
         * \param link the link that this new graph will model.
         */
        ModelLinkGraph(const Link& link);
        /**
         * Constructs a new copy of the given graph.
         *
         * \param copy the graph to copy.
         */
        ModelLinkGraph(const ModelLinkGraph& copy);
        /**
         * Moves the given graph into this new graph.
         * This is a fast (constant time) operation.
         *
         * All nodes and cells that belong to \a src will be moved into this
         * graph, and so any ModelLinkGraphNode or ModelLinkGraphCells
         * pointers or references will remain valid.
         *
         * The graph that is passed (\a src) will no longer be usable.
         *
         * \param src the graph to move.
         */
        ModelLinkGraph(ModelLinkGraph&& src) noexcept;
        /**
         * "Magic" constructor that tries to find some way to interpret
         * the given string as a 4-valent graph with embedding.
         *
         * At present, Regina understands the following types of strings
         * (and attempts to parse them in the following order):
         *
         * - Regina's variants of the _plantri_ format, including the default
         *   format as well as the tight and extended variants, as produced by
         *   plantri(), canonicalPlantri() and extendedPlantri().
         *
         * This list may grow in future versions of Regina.
         *
         * \exception InvalidArgument Regina could not interpret the given
         * string as representing a graph using any of the supported string
         * types.
         *
         * \param description a string that describes a 4-valent graph with
         * embedding.
         */
        ModelLinkGraph(const std::string& description);

        /**
         * Destroys this graph.
         *
         * The ModelLinkGraphNode objects contained in this graph will also
         * be destroyed.
         */
        ~ModelLinkGraph();

        /**
         * Returns the number of nodes in this graph.
         *
         * \return the number of nodes.
         */
        size_t size() const;

        /**
         * Determines whether this graph is empty.
         * An empty graph is one with no nodes at all.
         *
         * \return \c true if and only if this graph is empty.
         */
        bool isEmpty() const;

        /**
         * Returns the number of connected components in this graph.
         *
         * \warning This routine is not thread-safe, since it caches the
         * number of components after computing it for the first time.
         *
         * \note These are components in the graph theoretical sense, not link
         * components. So, for example, the graph that models the Hopf link is
         * considered to be connected with just one component.
         *
         * \return the number of connected components.
         */
        size_t countComponents() const;

        /**
         * Returns the number of traversals in this graph.
         *
         * A _traversal_ is a closed path through the graph that always enters
         * and exits a node through opposite arcs.  If this graph models a
         * diagram for some link \a L, then the number of traversals in this
         * graph will be precisely the number of link components in \a L.
         *
         * This routine runs in linear time (and the result is not cached).
         *
         * \return the number of traversals.
         */
        size_t countTraversals() const;

        /**
         * Returns the node at the given index within this graph.
         *
         * For a graph with \a n nodes, the nodes are numbered
         * from 0 to <i>n</i>-1 inclusive.
         *
         * \warning If some nodes are added or removed then the indices
         * of other nodes might change.  If you wish to track a particular
         * node through such operations then you should use the pointer
         * to the relevant ModelLinkGraphNode object instead.
         *
         * \param index the index of the requested node.  This must
         * be between 0 and size()-1 inclusive.
         * \return the node at the given index.
         */
        ModelLinkGraphNode* node(size_t index) const;
        /**
         * Returns an object that allows iteration through and random access
         * to all nodes in this graph.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * range-based \c for loops.  Note that the elements of the list
         * will be pointers, so your code might look like:
         *
         * \code{.cpp}
         * for (ModelLinkGraphNode* n : graph.nodes()) { ... }
         * \endcode
         *
         * The object that is returned will remain up-to-date and valid for as
         * long as the graph exists: even if nodes are added and/or removed,
         * it will always reflect the nodes that are currently in the graph.
         * Nevertheless, it is recommended to treat this object as temporary
         * only, and to call nodes() again each time you need it.
         *
         * \return access to the list of all nodes.
         */
        auto nodes() const;

        /**
         * Sets this to be a (deep) copy of the given graph.
         *
         * \param src the graph to copy.
         * \return a reference to this graph.
         */
        ModelLinkGraph& operator = (const ModelLinkGraph& src);
        /**
         * Moves the contents of the given graph into this graph.
         * This is a fast (constant time) operation.
         *
         * All nodes and cells that belong to \a src will be moved into this
         * graph, and so any ModelLinkGraphNode or ModelLinkGraphCells
         * pointers or references will remain valid.
         *
         * The graph that is passed (\a src) will no longer be usable.
         *
         * \param src the graph to move.
         * \return a reference to this graph.
         */
        ModelLinkGraph& operator = (ModelLinkGraph&& src) noexcept;

        /**
         * Swaps the contents of this and the given graph.
         *
         * All nodes that belong to this graph will be moved to \a other,
         * and all nodes that belong to \a other will be moved to this graph.
         *
         * In particular, any ModelLinkGraphNode pointers or references and any
         * ModelLinkGraphArc objects will remain valid.
         *
         * This routine will behave correctly if \a other is in fact
         * this graph.
         *
         * \param other the graph whose contents should be swapped with this.
         */
        void swap(ModelLinkGraph& other) noexcept;

        /**
         * Inserts a copy of the given graph into this graph.
         *
         * The nodes of \a source will be copied into this graph, and placed
         * after any pre-existing nodes.  Specifically, if the original number
         * of nodes in this graph was \a N, then node \a i of \a source will be
         * copied to a new node `N+i` of this graph.
         *
         * This routine behaves correctly when \a source is this graph.
         *
         * \param source the graph whose copy will be inserted.
         */
        void insertGraph(const ModelLinkGraph& source);

        /**
         * Moves the contents of the given graph into this graph.
         *
         * The nodes of \a source will be moved directly into this graph, and
         * placed after any pre-existing nodes.  Specifically, if the original
         * number of nodes in this graph was \a N, then node \a i of \a source
         * will become node `N+i` of this graph.
         *
         * As is normal for an rvalue reference, after calling this function
         * \a source will be unusable.  Any arc references or node pointers
         * that referred to either this graph or \a source will remain
         * valid (and will all now refer to this graph), though if they
         * originally referred to \a source then they will now return
         * different numerical node indices.
         *
         * Calling `graph.insertGraph(source)` (where \a source is an rvalue
         * reference) is similar to calling `source.moveContentsTo(graph)`,
         * but it is a little faster since it does not need to leave
         * \a source in a usable state.
         *
         * \pre \a source is not this graph.
         *
         * \nopython Only the copying version of this function is available
         * (i.e., the version that takes \a source as a const reference).
         * If you want a fast move operation, call
         * `source.moveContentsTo(this)`.
         *
         * \param source the graph whose contents should be moved.
         */
        void insertGraph(ModelLinkGraph&& source);

        /**
         * Moves the contents of this graph into the given destination graph,
         * leaving this graph empty but otherwise usable.
         *
         * The nodes of this graph will be moved directly into \a dest, and
         * placed after any pre-existing nodes.  Specifically, if the original
         * number of nodes in \a dest was \a N, then node \a i of this graph
         * will become node `N+i` of \a dest.
         *
         * This graph will become empty as a result, but it will otherwise
         * remain a valid and usable ModelLinkGraph object.  Any arc references
         * or node pointers that referred to either this graph or \a dest will
         * remain valid (and will all now refer to \a dest), though if they
         * originally referred to this graph then they will now return
         * different numerical node indices.
         *
         * Calling `graph.moveContentsTo(dest)` is similar to calling
         * `dest.insertGraph(std::move(graph))`; it is a little slower but it
         * comes with the benefit of leaving this graph in a usable state.
         *
         * \pre \a dest is not this graph.
         *
         * \param dest the graph into which the contents of this graph should be
         * moved.
         */
        void moveContentsTo(ModelLinkGraph& dest);

        /**
         * Determines if this graph is combinatorially identical to the
         * given graph.
         *
         * Here "identical" means that both graphs have the same number
         * of nodes, and in both graphs the same pairs of outgoing arcs of
         * numbered nodes are connected by edges.
         *
         * \param other the graph to compare with this.
         * \return \c true if and only if the two graphs are
         * combinatorially identical.
         */
        bool operator == (const ModelLinkGraph& other) const;

        /**
         * Converts this graph into its reflection.
         *
         * This routine simply reverses (and also cycles) the order of
         * outgoing arcs around every node.
         */
        void reflect();

        /**
         * Returns the cellular decomposition of the closed orientable surface
         * in which this graph embeds.  This will be the decomposition induced
         * by this graph; in particular, it will be formed from discs bounded
         * by the nodes and arcs of this graph.
         *
         * This cellular decomposition will only be computed on demand.
         * This means that the first call to this function will take
         * linear time (as the decomposition is computed), but subsequent
         * calls will be constant time (since the decomposition is cached).
         *
         * Note that, as of Regina 7.4, you can call this routine even if the
         * graph is non-planar and/or disconnected.
         *
         * \warning This routine is not thread-safe.
         *
         * \exception InvalidArgument This graph induces more cells than
         * should ever be possible.  This should never occur unless the graph
         * is malformed in some way.
         *
         * \return the induced cellular decomposition of the surface in which
         * this graph embeds.
         */
        const ModelLinkGraphCells& cells() const;

        /**
         * Identifies whether this graph is connected.
         *
         * For the purposes of this routine, an empty graph is considered to
         * be connected.
         *
         * \warning This routine is not thread-safe, since it caches the
         * number of components after computing it for the first time.
         *
         * \return \c true if and only if this graph is connected.
         */
        bool isConnected() const;

        /**
         * Identifies whether this graph is simple; that is, has no loops or
         * multiple edges.
         *
         * \return \c true if and only if this graph is simple.
         */
        bool isSimple() const;

        /**
         * Returns the genus of the closed orientable surface in which this
         * graph embeds.
         *
         * As described in the class notes, this surface is chosen to have the
         * smallest possible genus: it is built from a collection of discs
         * whose boundaries follow the nodes and arcs of this graph according
         * to the local embedding.
         *
         * If this graph is disconnected (and therefore the surface is also
         * disconnected), then this routine will return the sum of the genus
         * over all components.
         *
         * \return the genus of the surface in which this graph embeds.
         */
        size_t genus() const;

        /**
         * Identifies the smallest flype that can be performed on this
         * graph from the given starting location.
         *
         * Here we use the same notation as in the three-argument flype()
         * function, where you perform a flype by passing three arcs
         * \a from, \a left and \a right.  Read the flype() documentation
         * now if you have not done so already; this includes a full
         * description of the flype operation as well as diagrams with
         * the arcs \a from, \a left and \a right clearly marked.
         *
         * The given arc \a from identifies the node to the left of the flype
         * disc.  The aim of this routine is to identify two suitable arcs
         * \a left and \a right that exit through the right of the flype
         * disc.  Together, these three arcs uniquely identify the entire
         * flype disc, and therefore prescribe the operation precisely.
         *
         * Here, by "suitable arcs", we mean a pair of arcs (\a left, \a right)
         * for which the three arcs (\a from, \a left, \a right) together
         * satisfy the preconditions for the flype() routine.
         *
         * There are several possible outcomes:
         *
         * - It is possible that there are _no_ suitable arcs \a left and
         *   \a right.  In this case, this routine returns a pair of null arcs.
         *
         * - It is possible that there is exactly one pair of suitable arcs
         *   (\a left, \a right).  In this case, this pair will be returned.
         *
         * - It is possible that there are _many_ pairs of suitable arcs.
         *   In this case, it can be shown that the suitable pairs have an
         *   ordering \a P_1, ..., \a P_k in which the flype disc for \a P_i
         *   is completely contained within the flype disc for \a P_j whenever
         *   \a i < \a j.  In this case, this routine returns the _smallest_
         *   pair \a P_1; that is, the pair (\a left, \a right) that gives
         *   the smallest possible flype disc.
         *
         * It should be noted that choosing only the smallest flype is not
         * a serious restriction: assuming the graph does not model a
         * composition of non-trivial knot diagrams, _any_ suitable flype
         * can be expressed as a composition of minimal flypes in this sense.
         *
         * \pre This graph is planar.
         *
         * \param from the arc that indicates where the flype disc should
         * begin.  This is the arc labelled \a from in the diagrams for the
         * three-argument flype() function: it is the lower of the two arcs
         * that enter the flype disc from the node \a X to the left of the
         * disc.  This should be presented as an arc of the node \a X.
         * \return the pair (\a left, \a right) representing the smallest
         * suitable flype beginning at \a from, or a pair of null arcs
         * if there are no suitable pairs (\a left, \a right).
         */
        std::pair<ModelLinkGraphArc, ModelLinkGraphArc> findFlype(
            const ModelLinkGraphArc& from) const;

        /**
         * Performs a flype on this graph at the given location.
         *
         * A _flype_ is an operation on a disc in the plane.  The boundary
         * of the disc must cut through four arcs of the graph (and otherwise
         * must not meet the graph at all), as indicated in the diagram below.
         * Moreover, the two arcs that exit the disc on the left must meet at
         * a common node just outside the disc.  (The punctuation symbols
         * drawn inside the disc are just to help illustrate how the
         * transformation works.)
         *
           \verbatim
                    ______                       ______
                   /      \                     /      \
           __   __| ##  ** |_______     _______| ::  <> |__   __
             \ /  |        |                   |        |  \ /
              X   |  Disc  |        ==>        |        |   X
           __/ \__|        |_______     _______|        |__/ \__
                  | ::  <> |                   | ##  ** |
                   \______/                     \______/
           \endverbatim
         *
         * The operation involves:
         *
         * - reflecting this disc in a horizontal axis (so the two arcs
         *   on the left switch places, and the two arcs on the right
         *   switch places);
         *
         * - removing the node outside the disc on the left, where the
         *   two arcs meet;
         *
         * - introducing a new node on the right instead, where the two
         *   arcs on the right will now meet.
         *
         * The equivalent operation on a knot diagram involves twisting the
         * entire region inside the disc about a horizontal axis, in a way
         * that undoes the crossing on the left but introduces a new crossing
         * on the right instead.
         *
         * You will need to pass arguments to indicate where the flype should
         * take place.  For this, we will label some of the features of the
         * initial diagram (before the move takes place): see the diagram below.
         * Here the labels \a from, \a left and \a right all refer to arcs.
         * The labels \a A, \a B, \a C and \a D all refer to dual 2-cells in
         * the plane; these are not passed as arguments, but they do appear
         * in the list of preconditions for this routine.
         *
           \verbatim
                            ______
           Cell A          /      \
           __   __________|        |_________ left
             \ /          |        |
              X   Cell B  |        |  Cell D
           __/ \__________|        |_________ right
                    from  |        |
           Cell C          \______/
           \endverbatim
         *
         * The arc \a from must be given as an arc of the node \a outside
         * the disc (i.e., the node to the left of cell \a B).  The arcs
         * \a left and \a right must be given as arcs of their respective
         * nodes \a inside the disc.
         *
         * \pre This graph is planar.
         *
         * \pre The arcs \a from, \a left and \a right are laid out as
         * in the diagram above.  In particular: \a from and \a right
         * have the same cell to their right (cell \a C); \a left and the
         * arc to the left of \a from have the same cell to their left
         * (cell \a A); and \a left and \a right have the same cell between
         * them (cell \a D).
         *
         * \pre Neither of the arcs \a left or \a right, when followed in the
         * direction away from the disc, end back at the node on the left of
         * the diagram.  That is, neither `left.traverse().node()` nor
         * `right.traverse().node()` is equal to `from.node()`.
         * (If this fails, then either the flype simply reflects the entire
         * graph, or else the graph models a composition of two non-trivial
         * knot diagrams.)
         *
         * \pre Cells \a A and \a C are distinct (that is, the node on
         * the left of the diagram is not a cut-vertex of the graph).
         *
         * \pre Cells \a B and \a D are distinct (that is, the disc actually
         * contains one or more nodes, and the graph does not model a
         * composition of two non-trivial knot diagrams).
         *
         * \exception InvalidArgument One or more of the preconditions above
         * fails to hold.  Be warned that the connectivity and planarity
         * preconditions will not be checked - these are the user's
         * responsibility - but all other preconditions _will_ be checked,
         * and an exception will be thrown if any of them fails.
         *
         * \param from the first arc that indicates where the flype should
         * take place, as labelled on the diagram above.  This should be
         * presented as an arc of the node outside the disc, to the left.
         * \param left the second arc that indicates where the flype should
         * take place, as labelled on the diagram above.  This should be
         * presented as an arc of the node that it meets inside the disc.
         * \param right the third arc that indicates where the flype should
         * take place, as labelled on the diagram above.  This should be
         * presented as an arc of the node that it meets inside the disc.
         * \return the graph obtained by performing the flype.
         */
        ModelLinkGraph flype(const ModelLinkGraphArc& from,
            const ModelLinkGraphArc& left, const ModelLinkGraphArc& right)
            const;

        /**
         * Performs the smallest possible flype on this graph from the given
         * starting location.
         *
         * This is a convenience routine that simply calls findFlype() to
         * identify the smallest possible flype from the given starting
         * location, and then calls the three-argument flype() to actually
         * perform it.  If there is no possible flype from the given starting
         * location then this routine throws an exception.
         *
         * See the documentation for the three-argument flype() for further
         * details on the flype operation, and see findFlype() for a
         * discussion on what is meant by "smallest possible".
         *
         * \pre This graph is planar.
         *
         * \exception InvalidArgument There is no suitable flype on this
         * graph from the given starting location (that is, findFlype()
         * returns a pair of null arcs).
         *
         * \param from the arc that indicates where the flype disc should
         * begin.  This is the arc labelled \a from in the diagrams for the
         * three-argument flype() function: it is the lower of the two arcs
         * that enter the flype disc from the node \a X to the left of the
         * disc.  This should be presented as an arc of the node \a X.
         * \return the graph obtained by performing the flype.
         */
        ModelLinkGraph flype(const ModelLinkGraphArc& from) const;

        /**
         * Generates an arbitrary link diagram that is modelled by this graph.
         *
         * All link diagrams modelled by this graph are identical up to
         * switching of individual crossings and/or reversal of individual
         * link components.  This routine will generate just one of these many
         * possible link diagrams.  If you wish to generate _all_ such
         * diagrams, consider whether generateMinimalLinks() might be more
         * appropriate for what you need.
         *
         * Unlike generateMinimalLinks(), there is no guarantee that the
         * diagram produced by this routine is minimal or even locally minimal
         * in any sense.  For example, it is entirely possible that the link
         * diagram returned by this routine will have a reducing Reidemeister
         * move.
         *
         * In the link diagram that is generated, crossing \a k will always
         * correspond to node \a k of this graph.  If this graph is non-planar,
         * then the resulting link diagram will be virtual.
         */
        Link generateAnyLink() const;

        /**
         * Exhaustively generates potentially-minimal link diagrams that
         * are modelled by this graph.
         *
         * Here _potentially-minimal_ means there are no "obvious"
         * simplification moves (such as a simplifying type II Reidemeister
         * move, for example).  The list of "obvious" moves considered
         * here is subject to change in future versions of Regina.
         *
         * By _exhaustive_, we mean:
         *
         * - Every minimal link diagram modelled by this graph will be
         *   generated by this routine, up to reflection and/or reversal
         *   (as explained below).
         *
         * - If a link diagram is non-minimal and modelled by this graph,
         *   it _might_ still be generated by this routine.
         *
         * In other words, this routine will generate all minimal link diagrams
         * modelled by this graph, but there is no promise that all of the
         * diagrams generated are minimal.
         *
         * Labelled diagrams are only generated once up to reflection of the
         * diagram and/or reversal of each component.  Here "reflection"
         * corresponds to the function Link::changeAll(), which reflects the
         * link diagram in the surface that contains it.  Specifically, this
         * routine will fix the orientation of each link component (always
         * following the smallest numbered available arc away from the
         * smallest index graph node in each link component), and it will
         * fix the upper and lower strands at node 0 so that the corresponding
         * crossing is always positive.
         *
         * In each link diagram that is generated, crossing \a k will always
         * correspond to node \a k of this graph.  If this graph is non-planar,
         * then the resulting link diagrams will all be virtual.
         *
         * For each link diagram that is generated, this routine will call
         * \a action (which must be a function or some other callable object).
         *
         * - The first argument passed to \a action will be the link diagram
         *   that was generated (of type Link).  This will be passed as an
         *   rvalue; a typical action could (for example) take it by const
         *   reference and query it, or take it by value and modify it, or take
         *   it by rvalue reference and move it into more permanent storage.
         *
         * - If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - \a action must return \c void.
         *
         * \apinotfinal
         *
         * \pre The cell decomposition induced by this graph has no 1-gons
         * (which, in any link diagram that the graph models, would yield a
         * reducing type I Reidemeister move).
         *
         * \python This function is available in Python, and the
         * \a action argument may be a pure Python function.  However, its
         * form is more restricted: the argument \a args is removed, so you
         * simply call it as generateMinimalLinks(action).  Moreover,
         * \a action must take exactly one argument (the link diagram).
         *
         * \exception FailedPrecondition There is a 1-gon in the cell
         * decomposition induced by this graph.
         *
         * \param action a function (or other callable object) to call
         * for each link diagram that is generated.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial link diagram argument.
         */
        template <typename Action, typename... Args>
        void generateMinimalLinks(Action&& action, Args&&... args) const;

        /**
         * Exhaustively generates all link diagrams that are modelled by this
         * graph, up to reversal of individual link components.  If this graph
         * has \a n nodes, then there will be `2^n` link diagrams generated in
         * total.
         *
         * This routine is provided mainly to help with exhaustive testing.
         * If you are not interested in "obviously" non-minimal link diagrams,
         * then you should call generateMinimalLinks() instead.
         *
         * Labelled diagrams are only generated once up to reversal of each
         * component.  Specifically, this routine will fix the orientation of
         * each link component (always following the smallest numbered available
         * arc away from the smallest index graph node in each link component).
         *
         * In each link diagram that is generated, crossing \a k will always
         * correspond to node \a k of this graph.  If this graph is non-planar,
         * then the resulting link diagrams will all be virtual.
         *
         * For each link diagram that is generated, this routine will call
         * \a action (which must be a function or some other callable object).
         *
         * - The first argument passed to \a action will be the link diagram
         *   that was generated (of type Link).  This will be passed as an
         *   rvalue; a typical action could (for example) take it by const
         *   reference and query it, or take it by value and modify it, or take
         *   it by rvalue reference and move it into more permanent storage.
         *
         * - If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - \a action must return \c void.
         *
         * \apinotfinal
         *
         * \python This function is available in Python, and the
         * \a action argument may be a pure Python function.  However, its
         * form is more restricted: the argument \a args is removed, so you
         * simply call it as generateAllLinks(action).  Moreover, \a action
         * must take exactly one argument (the link diagram).
         *
         * \param action a function (or other callable object) to call
         * for each link diagram that is generated.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial link diagram argument.
         */
        template <typename Action, typename... Args>
        void generateAllLinks(Action&& action, Args&&... args) const;

        /**
         * Returns the canonical relabelling of this graph.
         *
         * Here "relabelling" allows for any combination of:
         *
         * - a relabelling of the nodes;
         *
         * - a relabelling of the arcs around each node, whilst preserving the
         *   cyclic order;
         *
         * - if \a allowReflection is \c true, a reversal of the cyclic order
         *   of the arcs around _every_ node (i.e., a reflection of the
         *   surface in which the graph embeds).
         *
         * Two graphs are related under such a relabelling if and only if
         * their canonical relabellings are identical.
         *
         * There is no promise that this will be the same canonical labelling
         * as used by canonicalPlantri().
         *
         * The running time for this routine is quadratic in the size of
         * the graph.
         *
         * \pre This graph is connected.
         *
         * \param allowReflection \c true if we allow reflection of the surface
         * in which the graph embeds; that is, a graph and its reflection
         * should produce the same canonical relabelling.
         * \return the canonical relabelling of this graph.
         */
        ModelLinkGraph canonical(bool allowReflection = true) const;

        /**
         * Randomly relabels this graph in an orientation-preserving manner.
         *
         * The nodes will be relabelled arbitrarily.  Around each node, the
         * four outgoing arcs will be relabelled in a random way that
         * preserves their cyclic order (thereby preserving the local
         * embedding of the graph, without reflection).
         *
         * This routine is thread-safe, and uses RandomEngine for its random
         * number generation.
         */
        void randomise();

        /**
         * Generates all possible local embeddings of the given 4-valent graph
         * into some closed orientable surface.
         *
         * The input 4-valent graph (which does _not_ contain any embedding
         * data) should be presented as a closed 3-dimensional facet pairing
         * (since these can be generated efficiently using Regina).
         *
         * This routine will, up to canonical relabelling, generate all
         * local embeddings of the given graph into a closed orientable surface
         * (i.e., all ModelLinkGraph objects corresponding to the input graph),
         * each exactly once.
         *
         * The graphs that are generated will be labelled canonically as
         * described by canonical().  This means that the nodes of the graph
         * might use a different labelling from the simplices of the given
         * facet pairing.  The argument \a allowReflection will be passed
         * through to canonical().
         *
         * This routine is a work in progress.  Currently it is _very_
         * inefficient and memory-hungry; the algorithm will be improved over
         * time if/when it becomes important to do so.
         *
         * If \a allowReflection is \c false, then if we run all possible facet
         * pairings through this routine, the combined results should be
         * precisely those graphs described by OEIS sequence A292206.
         * If \a allowReflection is \c true, then (once we reach three nodes
         * or more) the output set should be smaller.
         *
         * For each graph that is generated, this routine will call \a action
         * (which must be a function or some other callable object).
         *
         * - The first argument passed to \a action will be the graph that was
         *   generated (of type ModelLinkGraph).  This will be passed as an
         *   rvalue; a typical action could (for example) take it by const
         *   reference and query it, or take it by value and modify it, or take
         *   it by rvalue reference and move it into more permanent storage.
         *
         * - If there are any additional arguments supplied in the list \a args,
         *   then these will be passed as subsequent arguments to \a action.
         *
         * - \a action must return \c void.
         *
         * \apinotfinal
         *
         * \pre The given facet pairing is connected, and is also closed
         * (i.e., has no unmatched facets).
         *
         * \python This function is available in Python, and the \a action
         * argument may be a pure Python function.  However, its form is more
         * restricted: the argument \a args is removed, so you simply call it
         * as `generateAllEmbeddings(pairing, allowReflection, action)`.
         * Moreover, \a action must take exactly one argument (the graph).
         *
         * \exception InvalidArgument The given pairing is disconnected and/or
         * has unmatched facets.
         *
         * \param pairing the 4-valent graph for which we wish to produce
         * local embeddings.
         * \param allowReflection \c true if we consider a reflection of the
         * surface in which the graph embeds to produce the same embedding.
         * \param constraints indicates any constraints that the embeddings
         * that we generate must satisfy.  This should be a bitwise OR of
         * constants from the GraphConstraint enumeration, or else
         * `GraphConstraint::All` (or just empty braces `{}`) if we should
         * generate every possible embedding.  If several constraints are ORed
         * together, then only embeddings that satisfy _all_ of the these
         * constraints will be produced.
         * \param action a function (or other callable object) to call
         * for each graph that is generated.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial graph argument.
         */
        template <typename Action, typename... Args>
        static void generateAllEmbeddings(const FacetPairing<3>& pairing,
            bool allowReflection, Flags<GraphConstraint> constraints,
            Action&& action, Args&&... args);

        /**
         * Outputs this graph in a variant of the ASCII text format used
         * by _plantri_.
         *
         * The software _plantri_, by Gunnar Brinkmann and Brendan McKay,
         * can be used to enumerate 4-valent planar graphs (amongst many
         * other things).  This routine outputs this graph in a format
         * that mimics _plantri_'s own dual ASCII format (i.e., the format
         * that _plantri_ outputs when run with the flags `-adq`).
         *
         * Specifically, the output will be a comma-separated sequence
         * of alphabetical strings.  The <i>i</i>th such string will consist
         * of four letters, encoding the endpoints of the four edges in
         * clockwise order that leave node \a i.  The lower-case letters
         * \c a,\c b,...,\c z represent nodes 0,1,...,25 respectively,
         * and the upper-case letters \c A,\c B,...,\c Z represent nodes
         * 26,27,...,51 respectively.  An example of such a string is:
         *
           \verbatim
           bcdd,aeec,abfd,acfa,bffb,ceed
           \endverbatim
         *
         * For graphs with at most 26 nodes, this is identical to
         * _plantri_'s own dual ASCII format.  For larger graphs, this
         * format differs: _plantri_ uses punctuation to represent
         * higher-index nodes, whereas Regina uses upper-case letters.
         *
         * Although _plantri_ is designed to work with graphs that are
         * connected and planar, this routine will happily produce output for
         * disconnected and/or non-planar graphs.  However, there remains an
         * unavoidable requirement: the graph must be dual to a _simple_
         * quadrangulation.  In detail:
         *
         * - The dual to this 4-valent graph will be a quadrangulation of the
         *   surface in which it embeds.  The _plantri_ format inherently
         *   requires that this quadrangulation is _simple_: that is,
         *   the dual must have no loops or parallel edges.
         *
         * - This requirement exists because, if the dual is _not_ simple, the
         *   embedding of the original graph cannot be uniquely reconstructed
         *   from its _plantri_ output.  In particular, the embedding becomes
         *   ambiguous around parallel edges in the original 4-valent graph.
         *
         * - For _planar_ graphs, this requirement is relatively harmless:
         *   a parity condition shows that loops in the dual are impossible,
         *   and parallel edges in the dual mean that any link diagram that
         *   this graph models is an "obvious" connected sum.
         *
         * - For _non-planar_ graphs, this requirement is more problematic.
         *   For example, consider the graph that models the virtual trefoil:
         *   the dual quadrangulation of the torus contains both loops and
         *   parallel edges.  This makes the _plantri_ format unusable in
         *   practice for graps that model virtual links.
         *
         * If this constraint is too onerous (e.g., you are working with
         * virtual links), you could use extendedPlantri() instead, which is
         * not compatible with the Brinkmann-McKay _plantri_ software but which
         * removes this requirement for the dual quadrangulation to be simple.
         *
         * For graphs that the _plantri_ format _does_ support, this routine
         * is an inverse to fromPlantri().  That is, for any graph \a g that
         * satisfies the preconditions below, `fromPlantri(g.plantri())` is
         * identical to \a g.  Likewise, for any string \a s that satisfies
         * the preconditions for fromPlantri(), calling
         * `fromPlantri(s).plantri()` will recover the original string \a s.
         *
         * \note The output of this function might not correspond to any
         * possible output from the program _plantri_ itself, even if the graph
         * is connected and planar, the dual quadrangulation is simple, and
         * only lower-case letters are used.  This is because _plantri_ only
         * outputs graphs with a certain canonical labelling.  In contrast,
         * plantri() can be called on any graph that satisfies the
         * preconditions below, and it will preserve the labels of the nodes
         * and the order of the arcs around each node.
         *
         * \pre This graph has between 1 and 52 nodes inclusive.
         * \pre The dual to this graph is a _simple_ quadrangulation of the
         * surface in which it embeds.
         *
         * \exception FailedPrecondition This graph is empty or has more
         * than 52 nodes.
         *
         * \return a _plantri_ format ASCII representation of this graph.
         */
        std::string plantri() const;

        /**
         * Outputs a text representation of this graph in a variant of the
         * _plantri_ ASCII format, using a canonical relabelling of nodes
         * and arcs, and with optional compression.
         *
         * This routine is similar to plantri(), but with two
         * significant differences:
         *
         * - This routine uses a canonical relabelling of the graph.
         *   Specifically, two graphs will have the same canonicalPlantri()
         *   output if and only if they are related under some combination of:
         *   (i) relabelling nodes; (ii) relabelling the arcs around each node
         *   whilst preserving their cyclic order; and (iii) if
         *   \a allowReflection is \c true, optionally reversing the cyclic
         *   order of the arcs around _every_ node.  This corresponds to a
         *   homeomorphism between the surfaces in which the graphs embed that
         *   maps one graph to the other; the argument \a allowReflection
         *   indicates whether this homeomorphism is allowed to reverse
         *   orientation.  While this has a similar aim to canonical(), there
         *   is no promise that both routines will use the same "canonical
         *   relabelling".
         *
         * - If the argument \a tight is \c true, then this routine uses
         *   an abbreviated output format.  The resulting compression is
         *   only trivial (it reduces the length by roughly 40%), but
         *   the resulting string is still human-parseable (though with a
         *   little more effort required).  This compression will simply
         *   remove the commas, and for each node it will suppress the
         *   destination of the first arc (since this can be deduced from
         *   the canonical labelling).
         *
         * Regardless of whether \e tight is \c true or \c false, the
         * resulting string can be parsed by fromPlantri() to
         * reconstruct the original graph.  Note however that, due to
         * the canonical labelling, the resulting graph might be a
         * relabelling of the original (and might even be a reflection
         * of the original, if \a allowReflection was passed as \c true).
         *
         * See plantri() for further details on the ASCII format itself,
         * including how Regina's implementation differs from _plantri_'s for
         * graphs with more than 26 nodes.
         *
         * The running time for this routine is quadratic in the size of
         * the graph.
         *
         * \pre This graph is connected.
         * \pre This graph has between 1 and 52 nodes inclusive.
         * \pre The dual to this graph is a _simple_ quadrangulation of the
         * surface in which it embeds; see plantri() for a discussion on why
         * this condition is needed.
         *
         * \exception FailedPrecondition This graph is empty or has more
         * than 52 nodes.
         *
         * \param allowReflection \c true if a graph and its reflection
         * should be considered the same (i.e., produce the same canonical
         * output), or \c false if they should be considered different.
         * Of course, if a graph is symmetric under reflection then the
         * graph and its reflection will produce the same canonical
         * output regardless of this parameter.
         * \param tight \c false if the usual _plantri_ ASCII format should
         * be used (as described by plantri() and fromPlantri()), or \c true
         * if the abbreviated format should be used as described above.
         * \return an optionally compressed _plantri_ ASCII representation
         * of this graph.
         */
        std::string canonicalPlantri(bool allowReflection = true,
            bool tight = false) const;

        /**
         * Outputs this graph using Regina's extended variant of the _plantri_
         * text format, which is better suited for non-planar graphs.
         *
         * See plantri() for a discussion of the _plantri_ text format.
         * A limitation of the _plantri_ format is that it requires the graph
         * to be dual to a _simple_ quadrangulation of the surface in which it
         * embeds.  This is a reasonable requirement for planar graphs, but
         * not so for non-planar graphs (which, in particular, are used to
         * model virtual link diagrams).
         *
         * This routine extends the _plantri_ format to more explicitly encode
         * the embedding of the graph, which means we can remove the problematic
         * requirement on the dual quadrangulation.  The format is Regina's
         * own (i.e., it is not compatible with the Brinkmann-McKay _plantri_
         * software).
         *
         * The output will be a comma-separated sequence of alphanumeric
         * strings.  The <i>i</i>th such string will consist of four
         * letter-number pairs, encoding the endpoints of the four edges in
         * clockwise order that leave node \a i.  The letters represent nodes
         * (with `a..zA..Z` representing nodes 0 to 51 respectively).
         * The numbers represent arcs (with `0..3` representing the four arcs
         * around each node in clockwise order).
         * An example of such a string (describing a genus one graph that
         * models the virtual trefoil) is:
         *
           \verbatim
           b3b2b0b1,a2a3a1a0
           \endverbatim
         *
         * This routine is an inverse to fromExtendedPlantri().  That is, for
         * any graph \a g of a supported size,
         * `fromExtendedPlantri(g.extendedPlantri())` will be identical to \a g.
         * Likewise, for any string \a s that satisfies
         * the preconditions for fromExtendedPlantri(), calling
         * `fromExtendedPlantri(s).extendedPlantri()` will recover the
         * original string \a s.
         *
         * \pre This graph has between 1 and 52 nodes inclusive.
         *
         * \exception FailedPrecondition This graph is empty or has more
         * than 52 nodes.
         *
         * \return a representation of this graph in the extended _plantri_
         * format.
         */
        std::string extendedPlantri() const;

        /**
         * Writes the tight encoding of this graph to the given output stream.
         * See the page on \ref tight "tight encodings" for details.
         *
         * \nopython Use tightEncoding() instead, which returns a string.
         *
         * \param out the output stream to which the encoded string will
         * be written.
         */
        void tightEncode(std::ostream& out) const;

        /**
         * Writes a short text representation of this graph to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this graph to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        /**
         * Builds a graph from a line of _plantri_ output, using
         * Regina's variant of the _plantri_ ASCII format.
         *
         * The software _plantri_, by Gunnar Brinkmann and Brendan McKay,
         * can be used to enumerate 4-valent planar graphs (amongst many
         * other things).  This routine converts a piece of output from
         * _plantri_, or the encoding of a graph using Regina's more general
         * plantri() or canonicalPlantri() functions, into a ModelLinkGraph
         * object that Regina can work with directly.
         *
         * Graphs encoded using Regina's plantri() or canonicalPlantri()
         * functions may be disconnected and/or non-planar.  However, such a
         * graph must be dual to a simple quadrangulation of the surface in
         * which it embeds - otherwise the _plantri_ format does not contain
         * enough information to recover the embedding of the graph.  This in
         * particular is a problem for non-planar graphs (which model virtual
         * links).  If this is an issue for you, you can use Regina's
         * extended _plantri_ format instead; see extendedPlantri() and
         * fromExtendedPlantri().
         *
         * If you are working with output directly from the software _plantri_,
         * this output must be in ASCII format, and must likewise be the dual
         * graph of a simple quadrangulation of the sphere.  The flags that
         * must be passed to _plantri_ to obtain such output are `-adq`
         * (although you may wish to pass additional flags to expand or
         * restrict the classes of graphs that _plantri_ builds).
         *
         * When run with these flags, _plantri_ produces output in the
         * following form:
         *
           \verbatim
           6 bbcd,adca,abee,affb,cffc,deed
           6 bcdd,aeec,abfd,acfa,bffb,ceed
           6 bcde,affc,abfd,acee,addf,becb
           \endverbatim
         *
         * Each line consists of an integer (the number of nodes in the graph),
         * followed by a comma-separated sequence of alphabetical strings
         * that encode the edges leaving each node.
         *
         * This function _only_ takes the comma-separated sequence of
         * alphabetical strings.  So, for example, to construct the graph
         * corresponding to the second line of output above, you could call:
         *
         * \code{.cpp}
         * fromPlantri("bcdd,aeec,abfd,acfa,bffb,ceed");
         * \endcode
         *
         * Regina uses its own variant of _plantri_'s output format, which is
         * identical for smaller graphs but which differs from _plantri_'s
         * own output format for larger graphs.  In particular:
         *
         * - For graphs with ≤ 26 nodes, Regina and _plantri_ use identical
         *   formats.  Here Regina can happily recognise the output from
         *   _plantri_ as described above, as well as the output from
         *   Regina's own plantri() and canonicalPlantri() functions.
         *
         * - For graphs with 27-52 nodes, Regina's and _plantri_'s formats
         *   differ: whereas _plantri_ uses punctuation for higher-index
         *   nodes, Regina uses the upper-case letters `A,...,Z`.
         *   For these larger graphs, Regina can only recognise Regina's own
         *   plantri() and canonicalPlantri() output, not _plantri_'s
         *   punctuation-based encodings.
         *
         * - For graphs with 53 nodes or more, Regina cannot encode or
         *   decode such graphs using _plantri_ format at all.
         *
         * Note that, whilst the software _plantri_ always outputs graphs using
         * a particular canonical labelling, this function has no such
         * restriction: it can accept an arbitrary ordering of nodes and arcs -
         * in particular, it can accept the string `g.plantri()` for any graph
         * \a g that meets the preconditions below.
         *
         * This routine can also interpret the "tight" format that is
         * optionally produced by the member function canonicalPlantri()
         * (even though such output would certainly _not_ be produced by
         * the software _plantri_).  Note that, by design, the tight format
         * can only represented connected graphs.
         *
         * \warning While this routine does some basic error checking on the
         * input, these checks are not exhaustive.  In particular, it does
         * _not_ test that the graph is dual to a simple quadrangulation.
         *
         * \pre The graph being described is dual to a _simple_ quadrangulation
         * of the surface in which it embeds; see plantri() for further
         * discussion on why this condition is needed.
         *
         * \exception InvalidArgument The input was not a valid
         * representation of a graph using the _plantri_ output format.
         *
         * \param plantri a string containing the comma-separated sequence of
         * alphabetical strings in _plantri_ format, as described above.
         * \return the resulting graph.
         */
        static ModelLinkGraph fromPlantri(const std::string& plantri);

        /**
         * Builds a graph from a text representation using Regina's extended
         * variant of the _plantri_ format, which is better suited for
         * non-planar graphs.
         *
         * See extendedPlantri() for a detailed description of Regina's
         * extended _plantri_ text format.  In essence, this extends the
         * original Brinkmann-McKay _plantri_ format to more explicitly encode
         * the embedding of the graph, thereby removing the original _plantri_
         * requirement that the graph be dual to a simple quadrangulation of
         * the surface in which it embeds.  Removing this requirement is
         * important for non-planar graphs (which are used to model virtual
         * link diagrams).
         *
         * As an example, the string below is the extended _plantri_
         * representation of a genus one graph that models the virtual trefoil:
         *
           \verbatim
           b3b2b0b1,a2a3a1a0
           \endverbatim
         *
         * \exception InvalidArgument The input was not a valid representation
         * of a graph using Regina's extended _plantri_ format.
         *
         * \param text the representation of a graph using Regina's extended
         * _plantri_ format, as described in extendedPlantri().
         * \return the resulting graph.
         */
        static ModelLinkGraph fromExtendedPlantri(const std::string& text);

        /**
         * Reconstructs a graph from its given tight encoding.
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
         * a tight encoding of a graph.
         *
         * \nopython Use tightDecoding() instead, which takes a string as
         * its argument.
         *
         * \param input an input stream that begins with the tight encoding
         * for a graph.
         * \return the graph represented by the given tight encoding.
         */
        static ModelLinkGraph tightDecode(std::istream& input);

    private:
        /**
         * A helper array used by generateMinimalLinks().
         */
        static constexpr int upperOutArc[2 /* 0,1 for -,+ */][13 /* dir */] = {
            { -1, -1, -1, 0, -1, -1, 1, -1, -1, 3, -1, -1, 2 },
            { -1, -1, -1, 1, -1, -1, 2, -1, -1, 0, -1, -1, 3 }
        };

        /**
         * Internal to the constructor that takes a link to model.
         *
         * This function returns the outgoing arc of this graph that
         * corresponds to the outgoing upper/lower strand at the given crossing.
         */
        ModelLinkGraphArc outgoingArc(const StrandRef& s);
        /**
         * Internal to the constructor that takes a link to model.
         *
         * This function returns the outgoing arc of this graph that
         * corresponds to the incoming upper/lower strand at the given crossing.
         */
        ModelLinkGraphArc incomingArc(const StrandRef& s);

        /**
         * Computes the number of connected components in this graph.
         * The result will be stored in the data member \a nComponents_.
         */
        void computeComponents() const;
};

/**
 * Swaps the contents of the two given graphs.
 *
 * This global routine simply calls ModelLinkGraph::swap(); it is provided
 * so that ModelLinkGraph meets the C++ Swappable requirements.
 *
 * See ModelLinkGraph::swap() for more details.
 *
 * \param lhs the graph whose contents should be swapped with \a rhs.
 * \param rhs the graph whose contents should be swapped with \a lhs.
 *
 * \ingroup link
 */
void swap(ModelLinkGraph& lhs, ModelLinkGraph& rhs) noexcept;

/**
 * Describes the cellular decomposition of a closed orientable surface induced
 * by a 4-valent graph embedded within it.
 *
 * The graph is represented by an object of type ModelLinkGraph, which encodes
 * a local embedding of the graph within the surface (i.e., a cyclic ordering
 * of arcs around each graph node).  The nodes and arcs of this graph form the
 * vertices and edges of the cellular decomposition, and the 2-cells are
 * topological discs whose boundaries follow these nodes and arcs according to
 * their local embeddings.  The main purpose of this class is to deduce and
 * describe those 2-cells.
 *
 * As of Regina 7.4, this class can now work with graphs that are non-planar
 * (resulting in a surface with positive genus), disconnected (resulting in a
 * surface that is likewise disconnected), and/or empty (resulting in an empty
 * surface).
 *
 * Cellular decompositions do not support value semantics: they cannot be
 * copied, swapped, or manually constructed.  Instead they are computed
 * properties of model graphs, and are only accessible via const reference
 * through the member function ModelLinkGraph::cells().
 *
 * \ingroup link
 */
class ModelLinkGraphCells : public Output<ModelLinkGraphCells> {
    public:
        /**
         * An iterator type used when traversing the boundary of a 2-cell.
         */
        using ArcIterator = const ModelLinkGraphArc*;

    private:
        size_t nCells_;
            /**< The total number of cells. */
        size_t nComponents_;
            /**< The total number of connected components.  This should always
                 be known (i.e., unlike ModelLinkGraph::nComponents_, it is
                 not computed on demand). */
        FixedArray<ModelLinkGraphArc> arcs_;
            /**< Stores the boundary of each cell.  Specifically, for cell
                 number \a i, positions start_[i], ..., (start_[i+1]-1) of
                 this array store the arcs in order as they would appear if
                 you walked anticlockwise around the cell boundary (so the
                 2-cell is on the left of each arc as you follow them around).
                 Each arc is described as an _outgoing_ arc as you exit each
                 node in turn.  Note that this array contains every arc of the
                 underlying graph exactly once. */
        FixedArray<size_t> start_;
            /**< Indicates where in the \a arcs_ array the boundary of each
                 cell begins and end.  This array has length (nCells_+1).
                 In particular, start_[nCells_] is the total number of arcs. */
        FixedArray<size_t> cell_;
            /**< For the <i>k</i>th arc exiting node \a n of the underlying
                 graph, cell_[4n+k] identifies which cell sits to the left
                 of the arc as you walk along it away from node \a n. */
        FixedArray<size_t> step_;
            /**< Let \a a be the <i>k</i>th arc exiting node \a n of the
                 underlying graph, and let \a c be the cell to the left
                 of the arc (as stored in the \a cell_ array).  Then
                 step_[4n+k] identifies where in the boundary of cell \a c
                 the arc \a a appears.  Specifically, arc \a a appears in the
                 \a arcs_ array as element arcs_[start_[c]+step_[4n+k]]. */

    public:
        /**
         * Returns the total number of 2-cells in this cellular decomposition.
         *
         * In the common case where this surface is the 2-sphere (i.e., the
         * underlying graph models a knot diagram), this will be exactly
         * two more than the number of nodes in the underlying graph.
         *
         * \note As of Regina 7.4, this routine will only return 0 when the
         * underlying graph is empty (and so this surface is empty also).
         * In previous versions of Regina, this routine also returned 0 if the
         * graph was non-planar (a scenario that was previously unsupported).
         *
         * \return the total number of 2-cells.
         */
        size_t countCells() const;
        /**
         * Returns the total number of (undirected) edges in this cellular
         * decomposition.  This is always twice the number of nodes in the
         * underlying graph.
         *
         * \return the total number of edges.
         */
        size_t countEdges() const;
        /**
         * Returns the total number of directed arcs in the underlying graph.
         * This is always four times the number of nodes in the graph.
         *
         * Recall that each undirected edge of the graph corresponds to
         * two directed arcs (one exiting each endpoint of the edge).
         *
         * \return the total number of directed arcs.
         */
        size_t countArcs() const;
        /**
         * Returns the total number of vertices in this cellular decomposition;
         * that is, the total number of nodes in the underlying graph.
         *
         * \return the total number of nodes.
         */
        size_t countNodes() const;
        /**
         * Returns the number of connected components in this surface.  This
         * will be the same as the number of components of the underlying graph.
         *
         * \return the number of connected components.
         */
        size_t countComponents() const;
        /**
         * Returns the genus of this closed orientable surface.  If the
         * surface has multiple components then this will sum the genus over
         * each component.
         *
         * \return the genus of this surface.
         */
        size_t genus() const;

        /**
         * Returns the number of arcs aloung the boundary of the given 2-cell.
         * If the given cell is a <i>k</i>-gon, then this routine returns the
         * integer \a k.
         *
         * \param cell indicates which cell to query; this must be
         * between 0 and countCells()-1 inclusive.
         * \return the size of the correpsonding 2-cell.
         */
        size_t size(size_t cell) const;
        /**
         * Returns the given arc along the boundary of the given 2-cell.
         *
         * For each cell, the arcs along the boundary are given in order
         * as you walk anticlockwise around the cell (so the cell is on
         * the left of each arc as you walk around the cell boundary).
         *
         * Each arc is described in the form of an _outgoing_ arc from some
         * node of the underlying graph (so if the return ModelLinkGraphArc
         * is \a a then this describes an outgoing arc from a.node()).
         * It follows that, if the underlying graph has \a n nodes, then
         * each of the 4<i>n</i> possible ModelLinkGraphArc values
         * appears exactly once as `arc(cell, which)` for some
         * integers \a cell and \a which.
         *
         * \param cell indicates which cell to query; this must be
         * between 0 and countCells()-1 inclusive.
         * \param which indicates which arc along the boundary of the
         * corresponding cell to return; this must be between 0 and
         * `size(cell)-1` inclusive.
         * \return the requested arc on the boundary of the given 2-cell.
         */
        const ModelLinkGraphArc& arc(size_t cell, size_t which) const;
        /**
         * Returns an object that allows iteration through and random access
         * to all arcs along the boundary of the given 2-cell.
         *
         * Suppose that the <i>i</i>th cell is a <i>k</i>-gon.  Then this
         * object gives access to the \a k arcs along the boundary of the
         * <i>i</i>th cell in the same order as described by arc(); that
         * is, walking anticlockwise around the cell boundary with the
         * cell to the left of each arc.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * range-based \c for loops.  The elements of the list will be
         * read-only objects of type ModelLinkGraphArc, and so your code might
         * look like:
         *
         * \code{.cpp}
         * for (const ModelLinkGraphArc& a : cells.arcs(cell)) { ... }
         * \endcode
         *
         * Using `arcs(cell)` is equivalent to iterating over the
         * iterator range (`begin(cell)`, `end(cell)`).
         * Using arcs() generates a tiny amount of extra overhead, but you may
         * also find it more readable.
         *
         * \param cell indicates which cell to query; this must be
         * between 0 and countCells()-1 inclusive.
         * \return access to the list of all arcs along the boundary of
         * the given cell.
         */
        auto arcs(size_t cell) const;
        /**
         * Returns the beginning of an iterator range for walking around the
         * boundary of the given 2-cell.
         *
         * Suppose that the <i>i</i>th cell is a <i>k</i>-gon.  Then the
         * iterator range described by `begin(i)` and `end(i)`
         * will iterate through the \a k arcs along the boundary of the
         * <i>i</i>th cell in the same order as described by arc(); that
         * is, walking anticlockwise around the cell boundary with the
         * cell to the left of each arc.
         *
         * Dereferencing the <i>j</i>th iterator in this range gives the
         * same result as calling `arc(cell, j)`, and iterating
         * over the entire range (`begin(cell)`, `end(cell)`)
         * is equivalent to iterating over `arcs(cell)`.
         *
         * \nopython Python users can iterate over arcs(\a cell) instead.
         *
         * \param cell indicates which cell to walk around; this must be
         * between 0 and countCells()-1 inclusive.
         * \return the beginning of an iterator range for the boundary
         * of the given cell.
         */
        ArcIterator begin(size_t cell) const;
        /**
         * Returns the end of an iterator range for walking around the
         * boundary of the given 2-cell.  As is usual for iterator
         * ranges, this is a past-the-end value (i.e., this iterator
         * cannot be dereferenced).
         *
         * Suppose that the <i>i</i>th cell is a <i>k</i>-gon.  Then the
         * iterator range described by `begin(i)` and `end(i)`
         * will iterate through the \a k arcs along the boundary of the
         * <i>i</i>th cell in the same order as described by arc(); that
         * is, walking anticlockwise around the cell boundary with the
         * cell to the left of each arc.
         *
         * Dereferencing the <i>j</i>th iterator in this range gives the
         * same result as calling `arc(cell, j)`, and iterating
         * over the entire range (`begin(cell)`, `end(cell)`)
         * is equivalent to iterating over `arcs(cell)`.
         *
         * \nopython Python users can iterate over arcs(\a cell) instead.
         *
         * \param cell indicates which cell to walk around; this must be
         * between 0 and countCells()-1 inclusive.
         * \return the end of an iterator range for the boundary
         * of the given cell.
         */
        ArcIterator end(size_t cell) const;

        /**
         * Returns the 2-cell that lies to the left of the given arc.
         *
         * Specifically, this function returns the number of the cell
         * that lies to the left of the given arc as you walk along it
         * away from `arc.node()`.
         *
         * For any arc \a a, calling `arc(cell(a), cellPos(a))`
         * will return the same arc \a a again.
         *
         * \param arc the given arc of the underlying graph.
         * \return the number of the cell that lies to the left of the
         * given arc; this will be an integer between 0 and
         * `countCells()-1` inclusive.
         */
        size_t cell(const ModelLinkGraphArc& arc) const;
        /**
         * Returns where the given arc appears along the boundary of the
         * 2-cell to its left.
         *
         * Consider the cell \a c to the left of the given arc as you follow
         * the arc away from `arc.node()`.  The routine arc()
         * can be used to enumerate the sequence of arcs along the boundary of
         * this cell \a c, in order as you walk anticlockwise around the cell
         * boundary.  The purpose of this routine is to identify _where_ in
         * this sequence the given arc occurs.
         *
         * For any arc \a a, calling `arc(cell(a), cellPos(a))`
         * will return the same arc \a a again.
         *
         * \param arc the given arc of the underlying graph.
         * \return the position of the given arc on the boundary of the
         * cell to its left; this will be an integer between 0 and
         * `size(cell(arc))-1` inclusive.
         */
        size_t cellPos(const ModelLinkGraphArc& arc) const;

        /**
         * Determines if this and the given cellular decomposition are
         * combinatorially identical.
         *
         * Here "identical" means that both decompositions have the same
         * number of cells, these cells are presented in the same order,
         * and their boundaries enter and exit the same numbered arcs of the
         * same numbered nodes, using the same directions of traversal and
         * the same starting points on each cell boundary.
         *
         * \param other the cellular decomposition to compare with this.
         * \return \c true if and only if the two cellular decompositions are
         * combinatorially identical.
         */
        bool operator == (const ModelLinkGraphCells& other) const;

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
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        // Make this class non-assignable.
        // There is a copy constructor, but it is private.
        ModelLinkGraphCells& operator = (const ModelLinkGraphCells&) = delete;

    private:
        /**
         * Creates a new cellular decomposition for the given 4-valent graph.
         *
         * \warning This object contains references into the graph \a g,
         * and so \a g must not be destroyed until after this cellular
         * decomposition is destroyed.
         *
         * \warning This routine is not thread-safe for the graph \a g,
         * since it will compute and cache the number of components in \a g
         * if this information is not already known.
         *
         * \pre The graph \a g must be connected.
         *
         * \exception InvalidArgument The given graph induces more cells than
         * should ever be possible.  This should never occur unless the graph
         * is malformed in some way.
         *
         * \param g the 4-valent graph (including its local embedding) that
         * defines this new cellular decomposition.
         */
        ModelLinkGraphCells(const ModelLinkGraph& g);
        /**
         * Creates a duplicate copy of the given cellular decomposition.
         * Both decompositions will refer to the same underlying ModelLinkGraph.
         */
        ModelLinkGraphCells(const ModelLinkGraphCells&) = default;

    friend class ModelLinkGraph;
};

// Inline functions for ModelLinkGraphArc

inline ModelLinkGraphArc::ModelLinkGraphArc() : node_(nullptr), arc_(0) {
}

inline ModelLinkGraphArc::ModelLinkGraphArc(ModelLinkGraphNode* node, int arc) :
        node_(node), arc_(arc) {
}

inline ModelLinkGraphNode* ModelLinkGraphArc::node() const {
    return node_;
}

inline int ModelLinkGraphArc::arc() const {
    return arc_;
}

inline ModelLinkGraphArc ModelLinkGraphArc::opposite() const {
    return ModelLinkGraphArc(node_, arc_ ^ 2);
}

inline ModelLinkGraphArc ModelLinkGraphArc::traverse() const {
    return node_->adj(arc_);
}

inline ModelLinkGraphArc ModelLinkGraphArc::next() const {
    ModelLinkGraphArc ans = node_->adj(arc_);
    ans.arc_ ^= 2;
    return ans;
}

inline ModelLinkGraphArc ModelLinkGraphArc::prev() const {
    return node_->adj(arc_ ^ 2);
}

inline ModelLinkGraphArc& ModelLinkGraphArc::operator ++ () {
    if (arc_ == 3)
        arc_ = 0;
    else
        ++arc_;
    return *this;
}

inline ModelLinkGraphArc ModelLinkGraphArc::operator ++ (int) {
    int old(arc_);
    if (arc_ == 3)
        arc_ = 0;
    else
        ++arc_;
    return ModelLinkGraphArc(node_, old);
}

inline ModelLinkGraphArc& ModelLinkGraphArc::operator -- () {
    if (arc_ == 0)
        arc_ = 3;
    else
        --arc_;
    return *this;
}

inline ModelLinkGraphArc ModelLinkGraphArc::operator -- (int) {
    int old(arc_);
    if (arc_ == 0)
        arc_ = 3;
    else
        --arc_;
    return ModelLinkGraphArc(node_, old);
}

inline ModelLinkGraphArc::operator bool() const {
    return (node_ != nullptr);
}

inline std::ostream& operator << (std::ostream& out,
        const ModelLinkGraphArc& a) {
    if (a.node())
        return out << a.node()->index() << ':' << a.arc();
    else
        return out << "(null)";
}

// Inline functions for ModelLinkGraphNode

inline size_t ModelLinkGraphNode::index() const {
    return markedIndex();
}

inline ModelLinkGraphArc ModelLinkGraphNode::arc(int which) {
    return ModelLinkGraphArc(this, which);
}

inline const ModelLinkGraphArc& ModelLinkGraphNode::adj(int which) const {
    return adj_[which];
}

inline void ModelLinkGraphNode::writeTextShort(std::ostream& out) const {
    out << "Node " << index() << ": arcs 0, 1, 2, 3 -> ";
    for (int i = 0; i < 4; ++i) {
        if (i > 0)
            out << ", ";
        out << adj_[i];
    }
}

// Inline functions for ModelLinkGraph

inline ModelLinkGraph::ModelLinkGraph() : nComponents_(-1), cells_(nullptr) {
}

inline ModelLinkGraph::ModelLinkGraph(ModelLinkGraph&& src) noexcept :
        nodes_(std::move(src.nodes_)),
        nComponents_(src.nComponents_),
        cells_(src.cells_) {
    src.cells_ = nullptr;
}

inline ModelLinkGraph::~ModelLinkGraph() {
    delete cells_;
    for (ModelLinkGraphNode* n : nodes_)
        delete n;
}

inline size_t ModelLinkGraph::size() const {
    return nodes_.size();
}

inline bool ModelLinkGraph::isEmpty() const {
    return nodes_.empty();
}

inline size_t ModelLinkGraph::countComponents() const {
    if (nComponents_ < 0)
        computeComponents();
    return nComponents_;
}

inline ModelLinkGraphNode* ModelLinkGraph::node(size_t index) const {
    return nodes_[index];
}

inline auto ModelLinkGraph::nodes() const {
    return ListView(nodes_);
}

inline ModelLinkGraph& ModelLinkGraph::operator = (ModelLinkGraph&& src)
        noexcept {
    nodes_.swap(src.nodes_);
    nComponents_ = src.nComponents_;
    std::swap(cells_, src.cells_);
    // Leave src to dispose of the original contents.
    return *this;
}

inline void ModelLinkGraph::swap(ModelLinkGraph& other) noexcept {
    if (&other != this) {
        nodes_.swap(other.nodes_);
        std::swap(nComponents_, other.nComponents_);
        std::swap(cells_, other.cells_);
    }
}

inline const ModelLinkGraphCells& ModelLinkGraph::cells() const {
    if (! cells_)
        const_cast<ModelLinkGraph*>(this)->cells_ =
            new ModelLinkGraphCells(*this);
    return *cells_;
}

inline bool ModelLinkGraph::isConnected() const {
    if (nComponents_ < 0)
        computeComponents();
    return nComponents_ <= 1;
}

inline size_t ModelLinkGraph::genus() const {
    return cells().genus();
}

inline ModelLinkGraph ModelLinkGraph::flype(const ModelLinkGraphArc& from)
        const {
    auto use = findFlype(from);
    if (use.first)
        return flype(from, use.first, use.second);
    else
        throw InvalidArgument("No flype is available from this arc");
}

inline void swap(ModelLinkGraph& lhs, ModelLinkGraph& rhs) noexcept {
    lhs.swap(rhs);
}

// Inline functions for ModelLinkGraphCells

inline size_t ModelLinkGraphCells::countCells() const {
    return nCells_;
}

inline size_t ModelLinkGraphCells::countEdges() const {
    return start_[nCells_] >> 1;
}

inline size_t ModelLinkGraphCells::countArcs() const {
    return start_[nCells_];
}

inline size_t ModelLinkGraphCells::countNodes() const {
    return start_[nCells_] >> 2;
}

inline size_t ModelLinkGraphCells::countComponents() const {
    return nComponents_;
}

inline size_t ModelLinkGraphCells::genus() const {
    // Per component:
    //   euler = V - E + F = #cells - #nodes
    //   genus = (2 - euler) / 2 = (#nodes + 2 - #cells) / 2
    //
    // Also: start_[nCells_] = #arcs = 4 * #nodes
    return ((start_[nCells_] >> 2) + (nComponents_ << 1) - nCells_) >> 1;
}

inline size_t ModelLinkGraphCells::size(size_t cell) const {
    return start_[cell + 1] - start_[cell];
}

inline const ModelLinkGraphArc& ModelLinkGraphCells::arc(size_t cell,
        size_t which) const {
    return arcs_[start_[cell] + which];
}

inline auto ModelLinkGraphCells::arcs(size_t cell) const {
    return ListView(arcs_.begin() + start_[cell],
        arcs_.begin() + start_[cell + 1]);
}

inline ModelLinkGraphCells::ArcIterator ModelLinkGraphCells::begin(size_t cell)
        const {
    return arcs_.begin() + start_[cell];
}

inline ModelLinkGraphCells::ArcIterator ModelLinkGraphCells::end(size_t cell)
        const {
    return arcs_.begin() + start_[cell + 1];
}

inline size_t ModelLinkGraphCells::cell(const ModelLinkGraphArc& arc) const {
    return cell_[(arc.node()->index() << 2) | arc.arc()];
}

inline size_t ModelLinkGraphCells::cellPos(const ModelLinkGraphArc& arc) const {
    return step_[(arc.node()->index() << 2) | arc.arc()];
}

} // namespace regina

#include "link/modellinkgraph-impl.h"

#endif

