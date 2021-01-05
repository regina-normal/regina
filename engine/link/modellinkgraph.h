
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

/*! \file link/modellinkgraph.h
 *  \brief Deals with model 4-valent planar graphs for knots and links.
 */

#ifndef __MODELLINKGRAPH_H
#ifndef __DOXYGEN
#define __MODELLINKGRAPH_H
#endif

#include <vector>
#include "core/output.h"
#include "utilities/markedvector.h"

namespace regina {

/**
 * \weakgroup link
 * @{
 */

class Link;
class ModelLinkGraph;
class ModelLinkGraphArc;
class ModelLinkGraphCells;
class ModelLinkGraphNode;

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
 * These objects are small enough to pass around by value.
 */
class REGINA_API ModelLinkGraphArc {
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
         * @param node the node of the model graph that this arc exits.
         * @param arc an integer in the range 0 to 3 inclusive,
         * indicating which of the four arcs exiting \a node this represents.
         */
        ModelLinkGraphArc(ModelLinkGraphNode* node, int arc);

        /**
         * Default copy constructor.
         */
        ModelLinkGraphArc(const ModelLinkGraphArc&) = default;

        /**
         * The node of the model graph from which this arc exits.
         *
         * @return the corresponding node, or \c null if this is a null arc.
         */
        ModelLinkGraphNode* node() const;

        /**
         * Indicates which arc this is amongst the four arcs exiting the
         * underlying node of the model graph.
         *
         * For each node of a model graph, the four arcs exiting that
         * node are numbered 0,1,2,3 in a clockwise order.
         *
         * @return an integer between 0 and 3 inclusive indicating one
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
         * will \e not be considered equal to the null arc created by calling
         * ModelLinkGraphArc(), since the latter is equivalent to calling
         * ModelLinkGraphArc(\c null, 0).
         *
         * \c true if and only if this and \a rhs are identical.
         */
        bool operator == (const ModelLinkGraphArc& rhs) const;
        /**
         * Tests whether this and the given arc reference are not identical.
         *
         * Two references are identical if and only if they return the
         * same values for both node() and arc().
         *
         * \warning If you create a null arc by calling
         * ModelLinkGraphArc(\c null, \a i) for some non-zero \a i, then this
         * will \e not be considered equal to the null arc created by calling
         * ModelLinkGraphArc(), since the latter is equivalent to calling
         * ModelLinkGraphArc(\c null, 0).
         *
         * \c true if and only if this and \a rhs are not identical.
         */
        bool operator != (const ModelLinkGraphArc& rhs) const;
        /**
         * Default assignment operator.
         *
         * @return a reference to this object.
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
         * <tt>a.opposite().opposite()</tt> is identical to \a a.
         *
         * \pre This is not a null arc, i.e., node() does not
         * return \c null.
         *
         * @return the opposite arc exiting the same node.
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
         * <tt>a.traverse().traverse()</tt> is identical to \a a.
         *
         * \pre This is not a null arc, i.e., node() does not return \c null.
         *
         * @return the arc at the other end of the underlying edge of the
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
         * For any arc \a a, calling <tt>a.next()</tt> is equivalent to
         * calling <tt>a.traverse().opposite()</tt>.
         *
         * \pre This is not a null arc, i.e., node() does not return \c null.
         *
         * @return the next arc after this when walking through the graph as
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
         * For any arc \a a, calling <tt>a.prev()</tt> is equivalent to
         * calling <tt>a.opposite().traverse()</tt>.
         *
         * \pre This is not a null arc, i.e., node() does not return \c null.
         *
         * @return the previous arc before this when walking through the graph
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
         * \ifacespython This routine is not available; however, the
         * postincrement operator is available under the name inc().
         *
         * @return a reference to this object.
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
         * \ifacespython This routine is available under the name inc().
         *
         * @return a copy of this object before the change took place.
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
         * \ifacespython This routine is not available; however, the
         * postdecrement operator is available under the name dec().
         *
         * @return a reference to this object.
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
         * \ifacespython This routine is available under the name dec().
         *
         * @return a copy of this object before the change took place.
         */
        ModelLinkGraphArc operator -- (int);

        /**
         * Tests whether this is a non-null arc.
         *
         * \ifacespython This is not available to python users.
         * Instead you can simply test whether <tt>node() == None</tt>.
         *
         * @return \c true if this is not a null arc (i.e., node()
         * does not return a null pointer), or \c false if this is a null
         * arc.
         */
        operator bool() const;

    friend class ModelLinkGraph;
};

/**
 * Writes a depiction of the given arc reference to the given output stream.
 *
 * @param out the output stream to which to write.
 * @param a the arc reference to write.
 * @return a reference to the given output stream.
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
 */
class ModelLinkGraphNode : public MarkedElement,
        public Output<ModelLinkGraphNode> {
    private:
        ModelLinkGraphArc adj_[4];
            /**< Stores the arcs at the \e other endpoints of the four
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
         * @return the index of this node.
         */
        int index() const;
        /**
         * Returns a reference to one of the four arcs of the graph that
         * exit this node.  This is equivalent to directly constructing
         * ModelLinkGraphArc(\c this, \a which).
         *
         * The four arcs exiting this node are numbered 0,1,2,3 in a
         * clockwise order around the node.
         *
         * @param which an integer in the range 0 to 3 inclusive, indicating
         * which of the four arcs exiting this node we should return.
         * @return a reference to the corresponding arc exiting this node.
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
         * \e other object, which is the ModelLinkGraphArc describing the other
         * endpoint of \a e.
         *
         * Note that for a node \a n, calling <tt>n.adj(i)</tt>
         * is equivalent to calling <tt>n.arc(i).traverse()</tt>.
         *
         * @param which an integer in the range 0 to 3 inclusive, indicating
         * which of the four arcs exiting this node we should examine.
         * @return a reference to the other end of the same undirected
         * edge of the underlying model graph.
         */
        const ModelLinkGraphArc& adj(int which) const;

        /**
         * Writes a short text representation of this node to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this node to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

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
 * This class is primarily designed for \e enumerating knots and links.
 * If you wish to study the underlying graph of an existing link, you do
 * not need to create a ModelLinkGraph - instead the Link class already
 * gives you direct access to the graph structure.  In particular, if
 * you include link/graph.h, you can use a Link directly as a directed graph
 * type with the Boost Graph Library.
 */
class REGINA_API ModelLinkGraph : public Output<ModelLinkGraph> {
    public:
        /**
         * A routine that can do arbitrary processing upon a knot or link.
         * Such routines are used to process links that are found when
         * running generateMinimalLinks().
         *
         * The first parameter passed should be a link, which \e must be
         * deallocated by this routine.  The second parameter may
         * contain arbitrary data as passed to generateMinimalLinks().
         *
         * Note that the first parameter might be \c null to signal that
         * link generation has finished.
         *
         * \apinotfinal
         */
        typedef void (*Use)(Link*, void*);

    private:
        MarkedVector<ModelLinkGraphNode> nodes_;
            /**< The nodes of this graph. */
        ModelLinkGraphCells* cells_;
            /**< The induced cellular decomposition of the sphere, or 0
                 if this has not yet been computed. */

    public:
        /**
         * Constructs an empty graph.
         */
        ModelLinkGraph();
        /**
         * Constructs a new copy of the given graph.
         *
         * @param copy the graph to copy.
         */
        ModelLinkGraph(const ModelLinkGraph& copy);

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
         * @return the number of nodes.
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
         * @param index the index of the requested node.  This must
         * be between 0 and size()-1 inclusive.
         * @return the node at the given index.
         */
        ModelLinkGraphNode* node(size_t index) const;

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
         * @param other the graph whose contents should be swapped with this.
         */
        void swapContents(ModelLinkGraph& other);

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
         * @return the induced cellular decomposition of the sphere.
         */
        const ModelLinkGraphCells& cells() const;

        /**
         * TODO: Flype is between arc-- and arc, i.e., over the region
         * defined by cell(arc).  Returns (null, null) iff flype() will
         * refuse to work with this.
         * Otherwise returns (left outgoing arc, right outgoing arc).
         *
         * Conditions that explicitly return \c null:
         *
         * - The upper and lower cells are the same.
         * - The common cell is the inside cell at from.node().
         *
         * \pre This graph is connected and TODO: valid.
         *
         * \ifacespython Instead of a C++ pair, this routine returns a
         * Python tuple containing two ModelLinkGraphArc objects.
         */
        std::pair<ModelLinkGraphArc, ModelLinkGraphArc> findFlype(
            const ModelLinkGraphArc& from) const;

        /**
         * TODO: Document.
         *
           \verbatim
                  Cell A
          
               __   __
                 \ /                    ----> left
                  X         Cell B
               __/ \__from              ----> right
          
                  Cell C
           \endverbatim
         *
         * Conditions that explicitly return 0:
         *
         * - Neither left nor right ends at from.node().
         * - The upper and lower bounding cells are distinct,
         * - The cell between left and right is not the inside cell
         * where the flype begins from from.node().
         */
        ModelLinkGraph* flype(const ModelLinkGraphArc& from,
            const ModelLinkGraphArc& left, const ModelLinkGraphArc& right)
            const;

        /**
         * TODO: Document.
         */
        ModelLinkGraph* flype(const ModelLinkGraphArc& from) const;

        /**
         * TODO: Document.  Only aims for minimal, ignores reflections.
         *
         * Node n will become crossing n.
         *
         * Arc (0,0) will always be forwards, and crossing 0 will always
         * be positive.
         *
         * TODO: PRE: Knot, not link.
         *
         * \apinotfinal
         */
        void generateMinimalLinks(Use use, void* useArgs = 0) const;

        /**
         * Outputs this graph in the ASCII text format used by \e plantri.
         *
         * The software \e plantri, by Gunnar Brinkmann and Brendan McKay,
         * can be used to enumerate 4-valent planar graphs (amongst many
         * other things).  This routine outputs this graph in a format
         * that mimics \e plantri's own dual ASCII format (i.e., the format
         * that \e plantri outputs when run with the flags <tt>-adq</tt>).
         *
         * Specifically, the output will be a comma-separated sequence
         * of alphabetical strings.  The <i>i</i>th such string will consist
         * of four lower-case letters, encoding the endpoints of the four
         * edges in clockwise order that leave node <i>i</i>.  The letters
         * \c a,\c b,\c c,... represent nodes 0,1,2,... respectively.
         * An example of such a string is:
         *
           \verbatim
           bcdd,aeec,abfd,acfa,bffb,ceed
           \endverbatim
         *
         * This routine is an inverse to fromPlantri(): for any graph \a g
         * that satisfies the preconditions below,
         * <tt>fromPlantri(g.plantri())</tt> is identical to \a g.  Likewise,
         * for any string \a s that satisfies the preconditions for
         * fromPlantri(), calling <tt>fromPlantri(s).plantri()</tt> will
         * recover the original string \a s.
         *
         * It is important to note the preconditions below: in particular,
         * that this graph must be dual to a \e simple quadrangulation of
         * the sphere.  This is because the planar embeddings for more general
         * graphs (i.e., the duals of non-simple quadrangulations) cannot
         * always be uniquely reconstructed from their \e plantri output.
         *
         * \note The output of this function might not correspond to any
         * possible output from the program \e plantri itself.  This is
         * because \e plantri only outputs graphs with a certain canonical
         * labelling.  In contrast, plantri() can be called on any graph
         * that satisfies the preconditions below, and it will preserve
         * the labels of the nodes and the order of the arcs around each node.
         *
         * \pre This graph is connected.
         * \pre This graph has between 1 and 26 nodes inclusive.
         * \pre The dual to this graph is a \e simple quadrangulation of the
         * sphere.  In particular, the dual must not have any parallel edges.
         * Note that any graph that fails this condition will the model
         * graph for a link diagram that is an "obvious" connected sum.
         *
         * @return a \e plantri format ASCII representation of this graph.
         */
        std::string plantri() const;

        /**
         * Outputs a text representation of this graph in the \e plantri
         * ASCII format, using a canonical relabelling of nodes and arcs,
         * and with optional compression.
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
         * See plantri() for further details on the ASCII format itself.
         *
         * The running time for this routine is quadratic in the size of
         * the graph.
         *
         * \pre This graph is connected.
         * \pre This graph has between 1 and 26 nodes inclusive.
         * \pre The dual to this graph is a \e simple quadrangulation of the
         * sphere.  In particular, the dual must not have any parallel edges.
         * Note that any graph that fails this condition will the model
         * graph for a link diagram that is an "obvious" connected sum.
         *
         * @param useReflection \c true if a graph and its reflection
         * should be considered the same (i.e., produce the same canonical
         * output), or \c false if they should be considered different.
         * Of course, if a graph is symmetric under reflection then the
         * graph and its reflection will produce the same canonical
         * output regardless of this parameter.
         * @param tight \c false if the usual \e plantri ASCII format should
         * be used (as described by plantri() and fromPlantri()), or \c true
         * if the abbreviated format should be used as described above.
         * @return an optionally compressed \e plantri ASCII representation
         * of this graph.
         */
        std::string canonicalPlantri(bool useReflection = true,
            bool tight = false) const;

        /**
         * Writes a short text representation of this graph to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this graph to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        /**
         * Builds a graph from a line of \e plantri output.
         *
         * The software \e plantri, by Gunnar Brinkmann and Brendan McKay,
         * can be used to enumerate 4-valent planar graphs (amongst many
         * other things).  This routine converts a piece of output from
         * \e plantri into a ModelLinkGraph object that Regina can work
         * with directly.
         *
         * The output from \e plantri must be in ASCII format, and must
         * be the dual graph of a simple quadrangulation of the sphere.  The
         * corresponding flags that must be passed to \e plantri to
         * obtain such output are <tt>-adq</tt> (although you will
         * may wish to pass additional flags to expand or restrict
         * the classes of graphs that \e plantri builds).
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
         * This function \e only takes the comma-separated sequence of
         * alphabetical strings.  So, for example, to construct the graph
         * correpsonding to the second line of output above, you could call:
         *
         * \code{.cpp}
         * fromPlantri("bcdd,aeec,abfd,acfa,bffb,ceed");
         * \endcode
         *
         * Regina can only recognise graphs in this format with up to 26
         * nodes.  If the graph contains more than 27 nodes then the
         * \e plantri output will contain punctuation, Regina will not
         * be able to parse it, and this function will return \c null.
         *
         * The given string does not \e need to be come from the program
         * \e plantri itself.  Whereas \e plantri always outputs graphs
         * with a particular canonical labelling, this function can
         * accept an arbitrary ordering of nodes and arcs - in particular,
         * it can accept the string <tt>g.plantri()</tt> for any graph
         * \a g that meets the preconditions below.  Nevertheless, the graph
         * must still meet these preconditions, since otherwise the \e plantri
         * format might not be enough to uniquely reconstruct the graph and
         * its planar embedding.
         *
         * This routine can also interpret the "tight" format that is output
         * by the member function canonicalPlantri() (even though such output
         * would certainly \e not be produced by the program \e plantri).
         *
         * \warning While this routine does some error checking on the
         * input, it does \e not test for planarity of the graph.
         * Of course \e plantri does not output non-planar graphs, but
         * if a user constructs one by hand and passes it to this
         * routine then the resulting behaviour is undefined.
         *
         * \pre The graph being described is connected.
         * \pre The graph being described has between 1 and 26 nodes inclusive.
         * \pre The graph being described is dual to a \e simple quadrangulation
         * of the sphere.  In particular, the dual must not have any parallel
         * edges.  Note that any graph that fails this condition will the model
         * graph for a link diagram that is an "obvious" connected sum.
         *
         * @param plantri a string containing the comma-separated
         * sequence of alphabetical strings output by \e plantri, as
         * described above.
         * @return a newly constructed graph, or \c null if the input
         * was found to be invalid.
         */
        static ModelLinkGraph* fromPlantri(const std::string& plantri);

        // Make this class non-assignable.
        ModelLinkGraph& operator = (const ModelLinkGraph&) = delete;
};

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
 */
class REGINA_API ModelLinkGraphCells : public Output<ModelLinkGraphCells> {
    private:
        ModelLinkGraphArc* arcs_;
            /**< Stores the boundary of each cell.  Specifically, for cell
                 number \a i, positions start_[i], ..., (start_[i+1]-1) of
                 this array store the arcs in order as they would appear if
                 you walked anticlockwise around the cell boundary (so the
                 2-cell is on the left of each arc as you follow them around).
                 Each arc is described as an \e outgoing arc as you exit each
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
         * that is already known to be connected.  However, \e assuming
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
         * @return \c true if and only if the underlying ModelLinkGraph
         * describes a planar embedding of a non-empty graph.
         */
        bool isValid() const;
        /**
         * Returns the number of 2-cells in this cellular decomposition.
         *
         * If isValid() returns \c false (i.e., the underlying ModelLinkGraph
         * is either empty or does not describe a planar embedding), then
         * this routine will return 0 instead.  Note that this routine
         * \e cannot be used to test for connectivity, which is a
         * non-negotiable precondition required by the class constructor.
         *
         * Note that, if isValid() returns \c true, then countCells()
         * will always return <i>n</i>+2 where \a n is the number of
         * nodes in the underlying graph.
         *
         * @return a strictly positive number of 2-cells if isValid()
         * returns \c true, or 0 if isValid() returns \c false.
         */
        size_t countCells() const;

        /**
         * Returns the given arc along the boundary of the given 2-cell.
         *
         * For each cell, the arcs along the boundary are given in order
         * as you walk anticlockwise around the cell (so the cell is on
         * the left of each arc as you walk around the cell boundary).
         *
         * Each arc is described in the form of an \e outgoing arc from some
         * node of the underlying graph (so if the return ModelLinkGraphArc
         * is \a a then this describes an outgoing arc from a.node()).
         * It follows that, if the underlying graph has \a n nodes, then
         * each of the 4<i>n</i> possible ModelLinkGraphArc values
         * appears exactly once as <tt>arc(cell, which)</tt> for some
         * integers \a cell and \a which.
         *
         * \pre The underlying ModelLinkGraph is non-empty, connected,
         * and describes a planar graph embedding.  Note that connectivity
         * is already required by the class constructor, and you can test
         * the remaining conditions by calling isValid().
         *
         * @param cell indicates which cell to query; this must be
         * between 0 and countCells()-1 inclusive.
         * @param which indicates which arc along the boundary of the
         * corresponding cell to return; this must be between 0 and
         * <tt>size(cell)-1</tt> inclusive.
         * @return the requested arc on the boundary of the given 2-cell.
         */
        const ModelLinkGraphArc& arc(size_t cell, size_t which) const;
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
         * @param cell indicates which cell to query; this must be
         * between 0 and countCells()-1 inclusive.
         * @return the size of the correpsonding 2-cell.
         */
        size_t size(size_t cell) const;
        /**
         * Returns the beginning of an iterator range for walking around the
         * boundary of the given 2-cell.
         *
         * Suppose that the <i>i</i>th cell is a <i>k</i>-gon.  Then the
         * iterator range described by <tt>begin(i)</tt> and <tt>end(i)</tt>
         * will iterate through the \a k arcs along the boundary of the
         * <i>i</i>th cell in the same order as described by arc(); that
         * is, walking anticlockwise around the cell boundary with the
         * cell to the left of each arc.
         *
         * Dereferencing the <i>j</i>th iterator in this range gives the
         * same result as calling <tt>arc(cell, j)</tt>.
         *
         * \pre The underlying ModelLinkGraph is non-empty, connected,
         * and describes a planar graph embedding.  Note that connectivity
         * is already required by the class constructor, and you can test
         * the remaining conditions by calling isValid().
         *
         * \ifacespython Not present.  Use arc() and size() instead.
         *
         * @return the beginning of an iterator range for the boundary
         * of the given cell.
         */
        const ModelLinkGraphArc* begin(size_t cell) const;
        /**
         * Returns the end of an iterator range for walking around the
         * boundary of the given 2-cell.  As is usual for iterator
         * ranges, this is a past-the-end value (i.e., this iterator
         * cannot be dereferenced).
         *
         * Suppose that the <i>i</i>th cell is a <i>k</i>-gon.  Then the
         * iterator range described by <tt>begin(i)</tt> and <tt>end(i)</tt>
         * will iterate through the \a k arcs along the boundary of the
         * <i>i</i>th cell in the same order as described by arc(); that
         * is, walking anticlockwise around the cell boundary with the
         * cell to the left of each arc.
         *
         * Dereferencing the <i>j</i>th iterator in this range gives the
         * same result as calling <tt>arc(cell, j)</tt>.
         *
         * \pre The underlying ModelLinkGraph is non-empty, connected,
         * and describes a planar graph embedding.  Note that connectivity
         * is already required by the class constructor, and you can test
         * the remaining conditions by calling isValid().
         *
         * \ifacespython Not present.  Use arc() and size() instead.
         *
         * @return the end of an iterator range for the boundary
         * of the given cell.
         */
        const ModelLinkGraphArc* end(size_t cell) const;

        /**
         * Returns the 2-cell that lies to the left of the given arc.
         *
         * Specifically, this function returns the number of the cell
         * that lies to the left of the given arc as you walk along it
         * away from <tt>arc.node()</tt>.
         *
         * For any arc \a a, calling <tt>arc(cell(a), cellPos(a))</tt>
         * will return the same arc \a a again.
         *
         * \pre The underlying ModelLinkGraph is non-empty, connected,
         * and describes a planar graph embedding.  Note that connectivity
         * is already required by the class constructor, and you can test
         * the remaining conditions by calling isValid().
         *
         * @param arc the given arc of the underlying graph.
         * @return the number of the cell that lies to the left of the
         * given arc; this will be an integer between 0 and
         * <tt>countCells()-1</tt> inclusive.
         */
        size_t cell(const ModelLinkGraphArc& arc) const;
        /**
         * Returns where the given arc appears along the boundary of the
         * 2-cell to its left.
         *
         * Consider the cell \a c to the left of the given arc as you follow
         * the arc away from <tt>arc.node()</tt>.  The routine arc()
         * can be used to enumerate the sequence of arcs along the boundary of
         * this cell \a c, in order as you walk anticlockwise around the cell
         * boundary.  The purpose of this routine is to identify \e where in
         * this sequence the given arc occurs.
         *
         * For any arc \a a, calling <tt>arc(cell(a), cellPos(a))</tt>
         * will return the same arc \a a again.
         *
         * \pre The underlying ModelLinkGraph is non-empty, connected,
         * and describes a planar graph embedding.  Note that connectivity
         * is already required by the class constructor, and you can test
         * the remaining conditions by calling isValid().
         *
         * @param arc the given arc of the underlying graph.
         * @return the position of the given arc on the boundary of the
         * cell to its left; this will be an integer between 0 and
         * <tt>size(cell(arc))-1</tt> inclusive.
         */
        size_t cellPos(const ModelLinkGraphArc& arc) const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        // Make this class non-assignable.
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
         * @param g the 4-valent graph, including its planar embedding, that
         * defines this new cellular decomposition.
         */
        ModelLinkGraphCells(const ModelLinkGraph& g);
        /**
         * Creates a duplicate copy of the given cellular decomposition.
         * Both decompositions will refer to the same underlying ModelLinkGraph.
         *
         * It is allowed for <tt>cloneMe.isValid()</tt> to return \c false;
         * in this case, the validity data will be carried across to the
         * new decomposition and <tt>this->isValid()</tt> will return
         * \c false also.
         *
         * @param cloneMe the cellular decomposition to clone.
         */
        ModelLinkGraphCells(const ModelLinkGraphCells& cloneMe);

    friend class ModelLinkGraph;
};

/*@}*/

// Inline functions for ModelLinkGraphArc

inline ModelLinkGraphArc::ModelLinkGraphArc() : node_(0), arc_(0) {
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

inline bool ModelLinkGraphArc::operator == (const ModelLinkGraphArc& rhs)
        const {
    return (node_ == rhs.node_ && arc_ == rhs.arc_);
}

inline bool ModelLinkGraphArc::operator != (const ModelLinkGraphArc& rhs)
        const {
    return (node_ != rhs.node_ || arc_ != rhs.arc_);
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
    return (node_ != 0);
}

inline std::ostream& operator << (std::ostream& out,
        const ModelLinkGraphArc& a) {
    if (a.node())
        return out << a.node()->index() << ':' << a.arc();
    else
        return out << "(null)";
}

// Inline functions for ModelLinkGraphNode

inline int ModelLinkGraphNode::index() const {
    return markedIndex();
}

inline ModelLinkGraphArc ModelLinkGraphNode::arc(int which) {
    return ModelLinkGraphArc(this, which);
}

inline const ModelLinkGraphArc& ModelLinkGraphNode::adj(int which) const {
    return adj_[which];
}

inline void ModelLinkGraphNode::writeTextShort(std::ostream& out) const {
    out << "node " << index();
}

inline void ModelLinkGraphNode::writeTextLong(std::ostream& out) const {
    out << "Node " << index() << ":\n";
    for (int i = 0; i < 4; ++i)
        out << "  Arc " << i << " -> " << adj_[i] << std::endl;
}

// Inline functions for ModelLinkGraph

inline ModelLinkGraph::ModelLinkGraph() : cells_(nullptr) {
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

inline void ModelLinkGraph::swapContents(ModelLinkGraph& other) {
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

inline ModelLinkGraph* ModelLinkGraph::flype(const ModelLinkGraphArc& from)
        const {
    auto use = findFlype(from);
    return (use.first ? flype(from, use.first, use.second) : nullptr);
}

// Inline functions for ModelLinkGraphCells

inline ModelLinkGraphCells::~ModelLinkGraphCells() {
    delete[] arcs_;
    delete[] start_;
    delete[] cell_;
    delete[] step_;
}

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

inline const ModelLinkGraphArc* ModelLinkGraphCells::begin(size_t cell) const {
    return arcs_ + start_[cell];
}

inline const ModelLinkGraphArc* ModelLinkGraphCells::end(size_t cell) const {
    return arcs_ + start_[cell + 1];
}

inline size_t ModelLinkGraphCells::cell(const ModelLinkGraphArc& arc) const {
    return cell_[(arc.node()->index() << 2) | arc.arc()];
}

inline size_t ModelLinkGraphCells::cellPos(const ModelLinkGraphArc& arc) const {
    return step_[(arc.node()->index() << 2) | arc.arc()];
}

} // namespace regina

#endif

