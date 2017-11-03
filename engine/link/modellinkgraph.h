
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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
#include <boost/noncopyable.hpp>
#include "output.h"
#include "utilities/markedvector.h"

namespace regina {

/**
 * \weakgroup link
 * @{
 */

class ModelLinkGraph;
class ModelLinkGraphArc;
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
        public Output<ModelLinkGraphNode>, public boost::noncopyable {
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
class REGINA_API ModelLinkGraph :
        public Output<ModelLinkGraph>, public boost::noncopyable {
    private:
        MarkedVector<ModelLinkGraphNode> nodes_;
            /**< The nodes of this graph. */

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
         * be the dual graph of a quadrangulation of the sphere.  The
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
         * \warning While this routine does some error checking on the
         * input, it does \e not test for planarity of the graph.
         * Of course \e plantri does not output non-planar graphs, but
         * if a user constructs one by hand and passes it to this
         * routine then the resulting behaviour is undefined.
         *
         * @param plantri a string containing the comma-separated
         * sequence of alphabetical strings output by \e plantri, as
         * described above.
         * @return a newly constructed graph, or \c null if the input
         * was found to be invalid.
         */
        static ModelLinkGraph* fromPlantri(const std::string& plantri);
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

inline ModelLinkGraph::ModelLinkGraph() {
}

inline ModelLinkGraph::~ModelLinkGraph() {
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
    if (&other != this)
        nodes_.swap(other.nodes_);
}

inline void ModelLinkGraph::reflect() {
    for (ModelLinkGraphNode* n : nodes_)
        std::swap(n->adj_[1], n->adj_[3]);
}

} // namespace regina

#endif

