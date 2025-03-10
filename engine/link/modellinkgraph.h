
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

/*! \file link/modellinkgraph.h
 *  \brief Deals with model 4-valent planar graphs for knots and links.
 */

#ifndef __REGINA_MODELLINKGRAPH_H
#ifndef __DOXYGEN
#define __REGINA_MODELLINKGRAPH_H
#endif

#include <vector>
#include "core/output.h"
#include "utilities/exception.h"
#include "utilities/listview.h"
#include "utilities/markedvector.h"

namespace regina {

class Link;
class ModelLinkGraph;
class ModelLinkGraphArc;
class ModelLinkGraphCells;
class ModelLinkGraphNode;
class StrandRef;

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
            /**< The node from which this arc exits.  This may be null. */
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
         * as though it were a link, in a direction away from the* current node.
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
        ModelLinkGraphArc adj_[4];
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
         * For a connected 4-valent graph, this is equivalent to the
         * number of 1-gons in the dual cell decomposition that are incident
         * with this node.
         *
         * If the underlying graph is disconnected, this routine will
         * ignore the presence of any other connected components.  In
         * particular, it will not worry about whether other disjoint
         * components have been placed inside or outside any 1-gons.
         *
         * \return The number of incident loops, which will be between 0 and 2
         * inclusive.
         */
        int loops() const;

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
         * If the underlying graph is disconnected, this routine will
         * ignore the presence of any other connected components.  In
         * particular, it will not worry about whether other disjoint
         * components have been placed inside or outside any bigons.
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
         * If the underlying graph is disconnected, this routine will
         * ignore the presence of any other connected components.  In
         * particular, it will not worry about whether other disjoint
         * components have been placed inside or outside any triangles.
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
 * Represents an undirected 4-valent planar graph with a specific planar
 * embedding.  This can be used as the model graph for a knot or link diagram,
 * where each node of the graph becomes a crossing.
 *
 * Current this class does not support circular graph components (which,
 * in a link diagram, would correspond to zero-crossing unknot components
 * of the link).
 *
 * This class is primarily designed for _enumerating_ knots and links.
 * If you wish to study the underlying graph of an existing link, you do
 * not need to create a ModelLinkGraph - instead the Link class already
 * gives you direct access to the graph structure.  In particular, if
 * you include link/graph.h, you can use a Link directly as a directed graph
 * type with the Boost Graph Library.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup link
 */
class ModelLinkGraph : public Output<ModelLinkGraph> {
    private:
        MarkedVector<ModelLinkGraphNode> nodes_;
            /**< The nodes of this graph. */
        ModelLinkGraphCells* cells_;
            /**< The induced cellular decomposition of the sphere, or \c null
                 if this has not yet been computed. */

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
         * Returns details of the cellular decomposition of the sphere
         * that is induced by this graph.
         *
         * This cellular decomposition will only be computed on demand.
         * This means that the first call to this function will take
         * linear time (as the decomposition is computed), but subsequent
         * calls will be constant time (since the decomposition is cached).
         *
         * \pre This graph is connected.
         *
         * \return the induced cellular decomposition of the sphere.
         */
        const ModelLinkGraphCells& cells() const;

        /**
         * Identifies whether this graph is connected.
         *
         * For the purposes of this routine, an empty graph is considered to
         * be connected.
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
         * \pre This graph is connected.
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
         * \pre This graph is connected.
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
         * \exception InvalidArgument One or more of the preconditions
         * above fails to hold.  Be warned that the connectivity precondition
         * will not be checked - this is the user's responsibility - but all
         * other preconditions _will_ be checked, and an exception will
         * be thrown if any of them fails.
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
         * \pre This graph is connected.
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
         * correspond to node \a k of this graph.
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
         *   (as noted below).
         *
         * - If a link diagram is non-minimal and modelled by this graph,
         *   it _might_ still be generated by this routine.
         *
         * In other words, this routine will generate all minimal link diagrams
         * modelled by this graph, but there is no promise that all of the
         * diagrams generated are minimal.
         *
         * Labelled diagrams are only generated once up to reflection of the
         * diagram and/or reversal of each component.  That is, this routine
         * will fix the orientation of each link component (always following
         * the smallest numbered available arc away from the smallest index
         * graph node in each link component), and it will fix the sign of the
         * crossing at node 0 (always positive).
         *
         * In each link diagram that is generated, crossing \a k will always
         * correspond to node \a k of this graph.
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
         * simply call it as generateMinimalLinks(action).  Moreover,
         * \a action must take exactly one argument (the link diagram).
         *
         * \param action a function (or other callable object) to call
         * for each link diagram that is generated.
         * \param args any additional arguments that should be passed to
         * \a action, following the initial link diagram argument.
         */
        template <typename Action, typename... Args>
        void generateMinimalLinks(Action&& action, Args&&... args) const;

        /**
         * Outputs this graph in a variant of the ASCII text format used
         * by \e plantri.
         *
         * The software \e plantri, by Gunnar Brinkmann and Brendan McKay,
         * can be used to enumerate 4-valent planar graphs (amongst many
         * other things).  This routine outputs this graph in a format
         * that mimics \e plantri's own dual ASCII format (i.e., the format
         * that \e plantri outputs when run with the flags `-adq`).
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
         * \e plantri's own dual ASCII format.  For larger graphs, this
         * format differs: \e plantri uses punctuation to represent
         * higher-index nodes, whereas Regina uses upper-case letters.
         *
         * This routine is an inverse to fromPlantri(): for any graph \a g
         * that satisfies the preconditions below,
         * `fromPlantri(g.plantri())` is identical to \a g.  Likewise,
         * for any string \a s that satisfies the preconditions for
         * fromPlantri(), calling `fromPlantri(s).plantri()` will
         * recover the original string \a s.
         *
         * It is important to note the preconditions below: in particular,
         * that this graph must be dual to a _simple_ quadrangulation of
         * the sphere.  This is because the planar embeddings for more general
         * graphs (i.e., the duals of non-simple quadrangulations) cannot
         * always be uniquely reconstructed from their \e plantri output.
         *
         * \note The output of this function might not correspond to any
         * possible output from the program \e plantri itself, even if only
         * lower-case letters are used.  This is because \e plantri only
         * outputs graphs with a certain canonical labelling.  In contrast,
         * plantri() can be called on any graph that satisfies the
         * preconditions below, and it will preserve the labels of the nodes
         * and the order of the arcs around each node.
         *
         * \pre This graph is connected.
         * \pre This graph has between 1 and 52 nodes inclusive.
         * \pre The dual to this graph is a _simple_ quadrangulation of the
         * sphere.  In particular, the dual must not have any parallel edges.
         * Note that any graph that fails this condition will the model
         * graph for a link diagram that is an "obvious" connected sum.
         *
         * \exception FailedPrecondition This graph has more than 52 nodes.
         *
         * \return a \e plantri format ASCII representation of this graph.
         */
        std::string plantri() const;

        /**
         * Outputs a text representation of this graph in a variant of the
         * \e plantri ASCII format, using a canonical relabelling of nodes
         * and arcs, and with optional compression.
         *
         * This routine is similar to plantri(), but with two
         * significant differences:
         *
         * - This routine does not preserve the labelling of nodes and
         *   the order of arcs around each node.  Instead it reorders the
         *   nodes and arcs so that any two relabellings of the "same"
         *   planar embedding will produce the same canonicalPlantri() output.
         *   By "same" we allow for relabelling and isotopy (sliding the
         *   graph around the sphere); if the argument \a useReflection is
         *   \c true then we allow for reflection also.
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
         * of the original, if \a useReflection was passed as \c true).
         *
         * See plantri() for further details on the ASCII format itself,
         * including the ways in which Regina's implementation of this format
         * differs from \e plantri's for graphs with more than 26 nodes.
         *
         * The running time for this routine is quadratic in the size of
         * the graph.
         *
         * \pre This graph is connected.
         * \pre This graph has between 1 and 52 nodes inclusive.
         * \pre The dual to this graph is a _simple_ quadrangulation of the
         * sphere.  In particular, the dual must not have any parallel edges.
         * Note that any graph that fails this condition will the model
         * graph for a link diagram that is an "obvious" connected sum.
         *
         * \exception FailedPrecondition This graph has more than 52 nodes.
         *
         * \param useReflection \c true if a graph and its reflection
         * should be considered the same (i.e., produce the same canonical
         * output), or \c false if they should be considered different.
         * Of course, if a graph is symmetric under reflection then the
         * graph and its reflection will produce the same canonical
         * output regardless of this parameter.
         * \param tight \c false if the usual \e plantri ASCII format should
         * be used (as described by plantri() and fromPlantri()), or \c true
         * if the abbreviated format should be used as described above.
         * \return an optionally compressed \e plantri ASCII representation
         * of this graph.
         */
        std::string canonicalPlantri(bool useReflection = true,
            bool tight = false) const;

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
         * Builds a graph from a line of \e plantri output, using
         * Regina's variant of the \e plantri ASCII format.
         *
         * The software \e plantri, by Gunnar Brinkmann and Brendan McKay,
         * can be used to enumerate 4-valent planar graphs (amongst many
         * other things).  This routine converts a piece of output from
         * \e plantri, or the encoding of a graph using Regina's own plantri()
         * or canonicalPlantri() functions, into a ModelLinkGraph object
         * that Regina can work with directly.
         *
         * If you are converting output from \e plantri, this output must be
         * in ASCII format, and must be the dual graph of a simple
         * quadrangulation of the sphere.  The corresponding flags that must
         * be passed to \e plantri to obtain such output are `-adq`
         * (although you will may wish to pass additional flags to expand or
         * restrict the classes of graphs that \e plantri builds).
         *
         * When run with these flags, \e plantri produces output in the
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
         * Regina uses its own variant of \e plantri's output format, which is
         * identical for smaller graphs but which differs from \e plantri's
         * own output format for larger graphs.  In particular:
         *
         * - For graphs with ≤ 26 nodes, Regina and \e plantri use identical
         *   formats.  Here Regina can happily recognise the output from
         *   \e plantri as described above, as well as the output from
         *   Regina's own plantri() and canonicalPlantri() functions.
         *
         * - For graphs with 27-52 nodes, Regina's and \e plantri's formats
         *   differ: whereas \e plantri uses punctuation for higher-index
         *   nodes, Regina uses the upper-case letters \c A,...,\c Z.
         *   For these larger graphs, Regina can only recognise Regina's own
         *   plantri() and canonicalPlantri() output, not \e plantri's
         *   punctuation-based encodings.
         *
         * - For graphs with 53 nodes or more, Regina cannot encode or
         *   decode such graphs using \e plantri format at all.
         *
         * Even for graphs with at most 26 nodes, the given string does not
         * _need_ to be come from the program \e plantri itself.  Whereas
         * \e plantri always outputs graphs with a particular canonical
         * labelling, this function can accept an arbitrary ordering of nodes
         * and arcs - in particular, it can accept the string
         * `g.plantri()` for any graph \a g that meets the preconditions
         * below.  Nevertheless, the graph must still meet these preconditions,
         * since otherwise the \e plantri format might not be enough to
         * uniquely reconstruct the graph and its planar embedding.
         *
         * This routine can also interpret the "tight" format that is
         * optionally produced by the member function canonicalPlantri()
         * (even though such output would certainly _not_ be produced by
         * the program \e plantri).
         *
         * \warning While this routine does some basic error checking on the
         * input, these checks are not exhaustive.  In particular, it does
         * _not_ test for planarity of the graph.  (Of course \e plantri
         * does not output non-planar graphs, but a user could still construct
         * one by hand and passes it to this routine, in which case the
         * resulting behaviour is undefined.)
         *
         * \pre The graph being described is connected.
         * \pre The graph being described is dual to a _simple_ quadrangulation
         * of the sphere.  In particular, the dual must not have any parallel
         * edges.  Note that any graph that fails this condition will the model
         * graph for a link diagram that is an "obvious" connected sum.
         *
         * \exception InvalidArgument The input was not a valid
         * representation of a graph using the \e plantri output format.
         * As noted above, the checks performed here are not exhaustive.
         *
         * \param plantri a string containing the comma-separated sequence of
         * alphabetical strings in \e plantri format, as described above.
         * \return the resulting graph.
         */
        static ModelLinkGraph fromPlantri(const std::string& plantri);

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
 * Describes the cellular decomposition of the sphere that is induced by a
 * given planar 4-valent graph.
 *
 * The graph is represented by an object of type ModelLinkGraph, which
 * also encodes a specific planar embedding of the graph.  The nodes
 * and arcs of this graph then form the vertices and edges of a cellular
 * decomposition; the main purpose of this class is to deduce and
 * describe the resulting 2-cells.
 *
 * At present, this class insists that each 2-cell is a topological disc.
 * As a consequence, this class cannot work with empty or disconnected graphs.
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
        ModelLinkGraphArc* arcs_;
            /**< Stores the boundary of each cell.  Specifically, for cell
                 number \a i, positions start_[i], ..., (start_[i+1]-1) of
                 this array store the arcs in order as they would appear if
                 you walked anticlockwise around the cell boundary (so the
                 2-cell is on the left of each arc as you follow them around).
                 Each arc is described as an _outgoing_ arc as you exit each
                 node in turn.  Note that this array contains every arc of the
                 underlying graph exactly once. */
        size_t* start_;
            /**< Indicates where in the \a arcs_ array the boundary of each
                 cell begins and end.  This array has length (nCells_+1). */
        size_t* cell_;
            /**< For the <i>k</i>th arc exiting node \a n of the underlying
                 graph, cell_[4n+k] identifies which cell sits to the left
                 of the arc as you walk along it away from node \a n. */
        size_t* step_;
            /**< Let \a a be the <i>k</i>th arc exiting node \a n of the
                 underlying graph, and let \a c be the cell to the left
                 of the arc (as stored in the \a cell_ array).  Then
                 step_[4n+k] identifies where in the boundary of cell \a c
                 the arc \a a appears.  Specifically, arc \a a appears in the
                 \a arcs_ array as element arcs_[start_[c]+step_[4n+k]]. */
        size_t nCells_;
            /**< The number of cells, or 0 if the underlying graph either
                 has a non-planar embedding or is empty. */

    public:
        /**
         * Destroys this cellular decomposition.
         */
        ~ModelLinkGraphCells();

        /**
         * Determines whether the underlying graph is non-empty with a
         * planar embedding, assuming that it is already known to be connected.
         *
         * As described in the class notes, this class can only work
         * with non-empty connected graphs where the corresponding
         * ModelLinkGraph object also describes a planar embedding.
         *
         * The constructor for this class requires you to pass a graph
         * that is already known to be connected.  However, _assuming_
         * the graph is connected, the constructor then tests for the
         * remaining conditions.  This routine returns the results of these
         * tests: if the underlying graph is empty or does not describe a
         * planar embedding, then this routine will return \c false.
         *
         * This routine is constant time, since the necessary work will
         * have already been completed by the class constructor.
         *
         * \warning Most of the routines in this class require isValid() to
         * return \c true.  Essentially, if isValid() returns \c false, you
         * should not attempt to query the details of the cell decomposition.
         * See the preconditions on individual routines for further details.
         *
         * \return \c true if and only if the underlying ModelLinkGraph
         * describes a planar embedding of a non-empty graph.
         */
        bool isValid() const;
        /**
         * Returns the number of 2-cells in this cellular decomposition.
         *
         * If isValid() returns \c false (i.e., the underlying ModelLinkGraph
         * is either empty or does not describe a planar embedding), then
         * this routine will return 0 instead.  Note that this routine
         * _cannot_ be used to test for connectivity, which is a
         * non-negotiable precondition required by the class constructor.
         *
         * Note that, if isValid() returns \c true, then countCells()
         * will always return <i>n</i>+2 where \a n is the number of
         * nodes in the underlying graph.
         *
         * \return a strictly positive number of 2-cells if isValid()
         * returns \c true, or 0 if isValid() returns \c false.
         */
        size_t countCells() const;

        /**
         * Returns the number of arcs aloung the boundary of the given 2-cell.
         * If the given cell is a <i>k</i>-gon, then this routine returns the
         * integer \a k.
         *
         * \pre The underlying ModelLinkGraph is non-empty, connected,
         * and describes a planar graph embedding.  Note that connectivity
         * is already required by the class constructor, and you can test
         * the remaining conditions by calling isValid().
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
         * \pre The underlying ModelLinkGraph is non-empty, connected,
         * and describes a planar graph embedding.  Note that connectivity
         * is already required by the class constructor, and you can test
         * the remaining conditions by calling isValid().
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
         * \pre The underlying ModelLinkGraph is non-empty, connected,
         * and describes a planar graph embedding.  Note that connectivity
         * is already required by the class constructor, and you can test
         * the remaining conditions by calling isValid().
         *
         * \nopython Python users can iterate over arcs(\a cell) instead.
         *
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
         * \pre The underlying ModelLinkGraph is non-empty, connected,
         * and describes a planar graph embedding.  Note that connectivity
         * is already required by the class constructor, and you can test
         * the remaining conditions by calling isValid().
         *
         * \nopython Python users can iterate over arcs(\a cell) instead.
         *
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
         * \pre The underlying ModelLinkGraph is non-empty, connected,
         * and describes a planar graph embedding.  Note that connectivity
         * is already required by the class constructor, and you can test
         * the remaining conditions by calling isValid().
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
         * \pre The underlying ModelLinkGraph is non-empty, connected,
         * and describes a planar graph embedding.  Note that connectivity
         * is already required by the class constructor, and you can test
         * the remaining conditions by calling isValid().
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
         * Creates a new cellular decomposition for the given planar
         * 4-valent graph.
         *
         * As described in the class notes: this class can only work
         * with graphs that are non-empty and connected (so that each
         * resulting 2-cell is a topological disc).
         *
         * The caller of this routine must ensure beforehand that the graph
         * \a g is connected.  However, this constructor will detect if \a g
         * is empty or does not describe a planar embedding, and isValid()
         * will return \c false in such cases.
         *
         * \warning This object contains references into the graph \a g,
         * and so \a g must not be destroyed until after this cellular
         * decomposition is destroyed.
         *
         * \pre The graph \a g must be connected.
         *
         * \param g the 4-valent graph, including its planar embedding, that
         * defines this new cellular decomposition.
         */
        ModelLinkGraphCells(const ModelLinkGraph& g);
        /**
         * Creates a duplicate copy of the given cellular decomposition.
         * Both decompositions will refer to the same underlying ModelLinkGraph.
         *
         * It is allowed for `cloneMe.isValid()` to return \c false;
         * in this case, the validity data will be carried across to the
         * new decomposition and `this->isValid()` will return
         * \c false also.
         *
         * \param cloneMe the cellular decomposition to clone.
         */
        ModelLinkGraphCells(const ModelLinkGraphCells& cloneMe);

    friend class ModelLinkGraph;
};

// Inline functions that need to be defined before *other* inline funtions
// that use them (this fixes DLL-related warnings in the windows port)

inline ModelLinkGraphCells::~ModelLinkGraphCells() {
    delete[] arcs_;
    delete[] start_;
    delete[] cell_;
    delete[] step_;
}

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

inline ModelLinkGraph::ModelLinkGraph() : cells_(nullptr) {
}

inline ModelLinkGraph::ModelLinkGraph(ModelLinkGraph&& src) noexcept :
        nodes_(std::move(src.nodes_)),
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

inline ModelLinkGraphNode* ModelLinkGraph::node(size_t index) const {
    return nodes_[index];
}

inline auto ModelLinkGraph::nodes() const {
    return ListView(nodes_);
}

inline ModelLinkGraph& ModelLinkGraph::operator = (ModelLinkGraph&& src)
        noexcept {
    nodes_.swap(src.nodes_);
    std::swap(cells_, src.cells_);
    // Leave src to dispose of the original contents.
    return *this;
}

inline void ModelLinkGraph::swap(ModelLinkGraph& other) noexcept {
    if (&other != this) {
        nodes_.swap(other.nodes_);
        std::swap(cells_, other.cells_);
    }
}

inline const ModelLinkGraphCells& ModelLinkGraph::cells() const {
    if (! cells_)
        const_cast<ModelLinkGraph*>(this)->cells_ =
            new ModelLinkGraphCells(*this);
    return *cells_;
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

inline bool ModelLinkGraphCells::isValid() const {
    return (nCells_ > 0);
}

inline size_t ModelLinkGraphCells::size(size_t cell) const {
    return start_[cell + 1] - start_[cell];
}

inline const ModelLinkGraphArc& ModelLinkGraphCells::arc(size_t cell,
        size_t which) const {
    return arcs_[start_[cell] + which];
}

inline auto ModelLinkGraphCells::arcs(size_t cell) const {
    return ListView(arcs_ + start_[cell], arcs_ + start_[cell + 1]);
}

inline ModelLinkGraphCells::ArcIterator ModelLinkGraphCells::begin(size_t cell)
        const {
    return arcs_ + start_[cell];
}

inline ModelLinkGraphCells::ArcIterator ModelLinkGraphCells::end(size_t cell)
        const {
    return arcs_ + start_[cell + 1];
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

