
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file treewidth/ntreedecomposition.h
 *  \brief Deals with treewidth and tree decompositions, in particular
 *  for facet pairing graphs.
 */

#ifndef __NTREEDECOMPOSITION_H
#ifndef __DOXYGEN
#define __NTREEDECOMPOSITION_H
#endif

#include "regina-core.h"
#include "shareableobject.h"
#include "census/ngenericfacetpairing.h"
#include "generic/ngenerictriangulation.h"

namespace regina {

/**
 * \addtogroup treewidth Treewidth
 * Treewidth and tree decompositions.
 * @{
 */

class NTreeBag;

/**
 * Indicates which algorithm should be used to compute a tree decomposition
 * of a graph.
 *
 * Additional algorithms may be added to this list in future versions of
 * Regina.
 */
enum TreeDecompositionAlg {
    /**
     * Indicates that a fast upper bound algorithm should be used.
     *
     * This does not promise to find a tree decomposition of smallest
     * possible width (an NP-hard problem), but it does promise to run
     * in small polynomial time.
     *
     * This constant \a TD_UPPER indicates that the "most appropriate"
     * upper bound algorithm should be used.  This is a good choice for
     * users who just want a good tree decomposition and want it quickly,
     * without needing to know the details of how it was produced.
     */
    TD_UPPER = 0x0001,
    /**
     * Indicates that the greedy fill-in heuristic should be used.
     *
     * This does not promise to find a tree decomposition of smallest
     * possible width (an NP-hard problem), but it does promise to run
     * in small polynomial time.
     *
     * The greedy fill-in heuristic has been found experimentally to perform
     * well on general graphs (T. van Dijk, J.-P. van den Heuvel and W. Slob,
     * "Computing treewidth with LibTW", www.treewidth.com, 2006).
     * Experimentation within Regina also suggests that it performs well in
     * the setting of face pairing graphs of 3-manifold triangulations.
     */
    TD_UPPER_GREEDY_FILL_IN = 0x0001
};

/**
 * Indicates the relationship between two bags in a tree decomposition.
 */
enum BagComparison {
    /**
     * Indicates that the two bags have identical contents.
     */
    BAG_EQUAL = 0,
    /**
     * Indicates that the first bag is a strict subset of the second.
     */
    BAG_SUBSET = -1,
    /**
     * Indicates that the first bag is a strict superset of the second.
     */
    BAG_SUPERSET = 1,
    /**
     * Indicates that neither bag is a subset of the other.
     */
    BAG_UNRELATED = 2
};

/**
 * Used to indicate the type of each bag in a \e nice tree decomposition.
 *
 * A nice tree decomposition is produced by calling
 * NTreeDecomposition::makeNice().  As a result:
 *
 * - every bag will be either an \e introduce bag, a \e forget bag, or a
 *   \e join bag, as defined below;
 *
 * - the root bag will be a forget bag, and will be empty;
 *
 * - every leaf bag will be an introduce bag, containing precisely one node.
 */
enum NiceType {
    /**
     * Indicates an introduce bag.  An \e introduce bag has only one child bag.
     * It contains all of the nodes in this child bag plus exactly one
     * new node, and contains no other nodes besides these.
     */
    NICE_INTRODUCE = 1,
    /**
     * Indicates a forget bag.  A \e forget bag has only one child bag.
     * It contains all of the nodes in this child bag except for exactly one
     * missing node, and contains no other nodes besides these.
     */
    NICE_FORGET = 2,
    /**
     * Indicates a join bag.  A \e join bag has exactly two child bags,
     * where the join bag and both of its child bags are all identical.
     */
    NICE_JOIN = 3
};

/**
 * Represents a single bag in a tree decomposition.
 *
 * The class NTreeDecomposition is used to build, manipulate and iterate
 * over tree decompositions of graphs.  A tree decomposition of a graph \a G
 * consists of (i) an underlying tree \a T; and (ii) a \e bag at every node of
 * this tree.  Each bag is a set of zero or more nodes of \a G, and
 * these bags are subject to various constraints as described in the
 * NTreeDecomposition class notes.
 *
 * In Regina, the underlying tree \a T is a rooted tree, so that every
 * non-root bag has exactly one parent bag, and every bag has some
 * number of children (possibly many, possibly zero).
 *
 * This class NTreeBag represents a single bag in a tree decomposition.
 *
 * - You can query which nodes of \a G the bag contains through the member
 *   functions size(), element() and contains().  It is assumed that the
 *   nodes of \a G are numbered, and so the nodes stored in this bag are
 *   simply represented as integers.
 *
 * - You can query the location of the bag in the underlying tree \a T
 *   through the member functions parent(), children(), sibling() and isLeaf().
 *
 * - You can iterate through all the bags in the tree decomposition
 *   with the help of member functions next(), nextPrefix() and index().
 *
 * - If the tree decomposition is of a special type (such as a \e nice
 *   tree decomposition), then each bag may be adorned with some additional
 *   information; you can access this through the member functions type()
 *   and subtype().
 */
class REGINA_API NTreeBag : public ShareableObject {
    private:
        int size_;
            /**< The number of nodes (of the graph \a G) stored in this bag. */
        int* elements_;
            /**< The individual nodes of this bag, sorted in ascending order. */
        NTreeBag* parent_;
            /**< The parent of this bag in the underlying tree \a T,
                 or \c null if this is the root bag. */
        NTreeBag* sibling_;
            /**< The next sibling of this bag in the underlying tree \a T,
                 or \c null if this is the final child of the parent bag. */
        NTreeBag* children_;
            /**< The first child of this bag in the underlying tree \a T,
                 or \c null if this bag is a leaf of the tree. */
        int type_;
            /**< Used where necessary to indicate the role that this bag
                 plays in the tree decomposition.  See type() for details. */
        int subtype_;
            /**< Used where necessary to give more precise information
                 (in addition to \a type_) on the role that this bag plays
                 in the tree decomposition.  See subtype() for details. */
        int index_;
            /**< Indicates the index of this bag within the underlying
                 tree decomposition, following a leaves-to-root ordering
                 of the bags.  See index() for details. */

    public:
        /**
         * Destroys this bag.
         */
        ~NTreeBag();

        /**
         * Returns the number of graph nodes stored in this bag.
         *
         * Suppose this is a bag in a tree decomposition of some graph \a G.
         * Then each bag is a subset of the nodes of \a G, and this
         * function simply returns the size of this subset.
         *
         * @return the number of graph nodes in this bag.
         */
        int size() const;
        /**
         * Used to query the individual graph nodes stored in this bag.
         *
         * Suppose this is a bag in a tree decomposition of some graph \a G;
         * recall that we assume that the nodes of \a G are numbered.
         * Then <tt>element(i)</tt> returns the number of the <i>i</i>th
         * node stored in this bag.
         *
         * Nodes are always stored in ascending order.  This means that
         * <tt>element(0) &lt; element(1) &lt; element(2) &lt; ...</tt>.
         *
         * @param which indicates which node should be returned; this
         * must be between 0 and size()-1 inclusive.
         * @return the number of the corresponding node stored in this bag.
         */
        int element(int which) const;
        /**
         * Queries whether a given graph node is contained in this bag.
         *
         * Suppose this is a bag in a tree decomposition of some graph \a G;
         * recall that we assume that the nodes of \a G are numbered.
         * Then <tt>contains(x)</tt> queries whether the node numbered \a x
         * is contained in this bag.
         *
         * @param element the number of some node in the graph \a G.
         * @return \c true if and only if the given node is in this bag.
         */
        bool contains(int element) const;

        /**
         * Returns the index of this bag within the full tree decomposition.
         *
         * Suppose the entire tree decomposition contains \a n bags.
         * Then these bags are automatically numbered 0,1,...,<i>n</i>-1.
         * This member function returns the number of this particular bag.
         *
         * The numbering of bags follows a leaves-to-root scheme.  In other
         * words, for any non-root bag \a b we have
         * <tt>b.index() &lt; b.parent()->index()</tt>.
         *
         * @return the index of this bag within the full tree decomposition
         * \a d; this will be between 0 and <tt>d.size()-1</tt> inclusive.
         */
        int index() const;

        /**
         * Returns auxiliary information associated with bags in special
         * classes of tree decompositions.
         *
         * If the underlying tree decomposition is of a special type,
         * then each bag may be adorned with some additional information
         * indicating the particular role that the bag plays.  This
         * additional information can be accessed through the member
         * functions type() and subtype().
         *
         * - If there is no type and/or subtype information stored for this
         *   bag, then type() will return zero, and subtype() will be undefined.
         *
         * - If there is type and/or subtype information stored for this
         *   bag, then the return value of type() is guaranteed to be non-zero.
         *   The specific meaning of subtype() (and indeed whether it is even
         *   defined) will typically depend on the return value of type().
         *
         * At present, types and subtypes are only stored for
         * \e nice tree decompositions.  See NTreeDecomposition::makeNice()
         * for details on what type() and subtype() represent.
         *
         * @return a non-zero value indicating the role that this bag plays
         * in this tree decomposition, or zero if type and subtype information
         * are not stored.
         */
        int type() const;
        /**
         * Returns a secondary level of auxiliary information associated with
         * bags in special classes of tree decompositions.
         *
         * If the underlying tree decomposition is of a special type,
         * then each bag may be adorned with some additional information
         * indicating the particular role that the bag plays.  This
         * additional information can be accessed through the member
         * functions type() and subtype().
         *
         * - If there is no type and/or subtype information stored for this
         *   bag, then type() will return zero, and subtype() will be undefined.
         *
         * - If there is type and/or subtype information stored for this
         *   bag, then type() will be non-zero, and the specific meaning of
         *   subtype() (and indeed whether it is even defined) will depend
         *   on the value of type().
         *
         * At present, types and subtypes are only stored for
         * \e nice tree decompositions.  See NTreeDecomposition::makeNice()
         * for details on what type() and subtype() represent.
         *
         * @return additional information indicating the role that this
         * bag plays in this tree decomposition, or undefined if no
         * additional subtype information is stored for this bag.
         */
        int subtype() const;

        /**
         * Determines if there is a subset/superset relationship between
         * this and the given bag.
         *
         * Recall that, in a tree decomposition of a graph \a G, each
         * bag is a set of nodes of \a G.  This function will return one
         * of the following constants:
         *
         * - BAG_EQUAL if this and \a rhs are equal;
         * - BAG_SUBSET if this bag is a strict subset of \a rhs;
         * - BAG_SUPERSET if this bag is a strict superset of \a rhs;
         * - BAG_UNRELATED if neither this nor \a rhs is a subset of the other.
         *
         * @param rhs the bag to compare with this.
         * @return the relationship between the two bags, as outlined above.
         */
        BagComparison compare(const NTreeBag& rhs) const;

        /**
         * Used for a postfix iteration through all of the bags in a tree
         * decomposition.  Amongst other things, a \e postfix iteration is
         * one in which all of the children of any bag \a b will be processed
         * before \a b itself.
         *
         * If \a d is a tree decomposition, then you can complete a full
         * postfix iteration of bags as follows:
         *
         * - the first bag in a postfix iteration is <tt>d.first()</tt>;
         * - the next bag after \a b in the iteration is <tt>b.next()</tt>;
         * - the iteration terminates when <tt>b.next()</tt> is \c null.
         *
         * This iteration processes the children of each bag in order;
         * that is, it processes each bag \a b before <tt>b.sibling()</tt>
         * (if the latter exists).
         *
         * The bags in a tree decomposition are indexed as 0,1,2,...,
         * as described by the index() member function.  This postfix
         * iteration is equivalent to iterating through bags 0,1,2,...
         * in order.
         *
         * @return the next bag after this in a postfix iteration of all
         * bags, or \c null if this is the final bag in such an iteration
         * (i.e., the root bag).
         */
        const NTreeBag* next() const;
        /**
         * Used for a prefix iteration through all of the bags in a tree
         * decomposition.  Amongst other things, a \e prefix iteration is
         * one in which each bag will be processed before any of its children.
         *
         * If \a d is a tree decomposition, then you can complete a full
         * prefix iteration of bags as follows:
         *
         * - the first bag in a prefix iteration is <tt>d.firstPrefix()</tt>
         *   (or equivalently, <tt>d.root()</tt>);
         * - the next bag after \a b in the iteration is
         *   <tt>b.nextPrefix()</tt>;
         * - the iteration terminates when <tt>b.nextPrefix()</tt> is \c null.
         *
         * This iteration processes the children of each bag in order;
         * that is, it processes each bag \a b before <tt>b.sibling()</tt>
         * (if the latter exists).
         *
         * @return the next bag after this in a prefix iteration of all
         * bags, or \c null if this is the final bag in such an iteration.
         */
        const NTreeBag* nextPrefix() const;

        /**
         * Returns the parent of this bag in the underlying rooted tree.
         *
         * @return the parent of this bag, or \c null if this bag is at
         * the root of the tree.
         */
        const NTreeBag* parent() const;
        /**
         * Returns the first child of this bag in the underlying rooted tree.
         *
         * If a bag has no children, then children() will be \c null.
         * If a bag has many children, then these will be
         * <tt>children()</tt>, <tt>children()->sibling()</tt>,
         * <tt>children()->sibling()->sibling()</tt>, and so on.
         *
         * @return the first child of this bag, or \c null if this is a
         * leaf bag (i.e., it has no children).
         */
        const NTreeBag* children() const;
        /**
         * Returns the next sibling of this bag in the underlying rooted tree.
         *
         * Specifically, if the parent of this bag has many children,
         * then sibling() will return the next child after this.
         *
         * More generally, all of the children of a bag \a b can be accessed as
         * <tt>b.children()</tt>, <tt>b.children()->sibling()</tt>,
         * <tt>b.children()->sibling()->sibling()</tt>, and so on.
         *
         * @return the next sibling of this bag, or \c null if either
         * (i) this is the final child of the parent bag, or
         * (ii) this is the root bag.
         */
        const NTreeBag* sibling() const;
        /**
         * Determines if this is a leaf bag.  A leaf bag is a bag with
         * no children in the underlying tree.
         *
         * This is equivalent to testing whether children() is \c null.
         *
         * @return \c true if and only if this is a leaf bag.
         */
        bool isLeaf() const;

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new bag with space allocated to hold the given number of
         * graph nodes.
         *
         * Specifically, the member \a size_ will be set and the array
         * \e elements_ will be allocated to this size, but the \e contents of
         * the array (i.e., the graph nodes themselves) will be left
         * uninitialised.
         *
         * This new bag will not be inserted into the tree, and will not
         * be assigned any other information (such as index, type or subtype).
         *
         * @param size the number of nodes that will be contained in the
         * new bag.
         */
        NTreeBag(int size);

        /**
         * Creates a new bag containing the same graph nodes as the given bag.
         *
         * Specifically, only the list of nodes stored in the given bag will
         * be cloned.  This new bag will not be inserted into the tree, and
         * will not be assigned any other information (such as index,
         * type or subtype).
         *
         * @param cloneMe the bag whose contents should be cloned.
         */
        NTreeBag(const NTreeBag& cloneMe);

        /**
         * Inserts the given bag into the tree as the first child of this bag.
         *
         * Any existing children of this bag will become siblings of \a child.
         *
         * \pre The given bag has not yet been inserted into the tree.
         *
         * @param child the bag to insert into the tree.
         */
        void insertChild(NTreeBag* child);

        /**
         * Swaps the contents of this and the given bag.
         *
         * This only swaps the nodes stored in each bag.  It does not
         * switch the positions of the two bags in the underlying tree.
         *
         * @param other the bag to swap contents with this.
         */
        void swapContents(NTreeBag& other);

    friend class NTreeDecomposition;
};

/**
 * TODO: HERE.
 */
class REGINA_API NTreeDecomposition : public ShareableObject {
    protected:
        /**
         * Note: loops are ignored.
         *
         * \ifacespython Not present.
         */
        struct Graph {
            int order_;
            bool** adj_;

            /**
             * Fills with false.
             */
            Graph(int order);
            ~Graph();

            void dump(std::ostream& out) const;
        };

    private:
        int width_;
            /**< The width of the tree decomposition; that is, one less
                 than the maximum bag size. */
        int size_;
        NTreeBag* root_;

    public:
        /**
         * \ifacespython The first argument must be of type NTriangulation
         * or Dim2Triangulation.
         */
        template <int dim>
        NTreeDecomposition(
            const NGenericTriangulation<dim>& triangulation,
            TreeDecompositionAlg alg = TD_UPPER);

        /**
         * \ifacespython The first argument must be of type NFacePairing
         * or Dim2EdgePairing.
         */
        template <int dim>
        NTreeDecomposition(
            const NGenericFacetPairing<dim>& pairing,
            TreeDecompositionAlg alg = TD_UPPER);

        /**
         * Note: if the matrix is asymmetric (a digraph), then the
         * undirected graph will be used.  Loops are ignored.
         */
        template <typename T>
        NTreeDecomposition(unsigned order, T const** const graph,
            TreeDecompositionAlg alg = TD_UPPER);

        ~NTreeDecomposition();

        int width() const;
        int size() const;

        const NTreeBag* root() const;
        const NTreeBag* first() const;
        const NTreeBag* firstPrefix() const;

        /**
         * Merge adjacent bags where one is a subset of another.
         */
        bool compress();
        void makeNice();

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;
#if 0
        /**
         * Verifies that (i) all bag elements are in range;
         * (ii) all elements appear in some bag; and
         * (iii) the path condition holds.
         * Does not verify the edge condition (since we do not have
         * access to the edges of the underlying graph).
         */
        bool verify(int order) const;

        template <int dim>
        bool verify(const NGenericTriangulation<dim>& triangulation) const;
#endif
    private:
        /**
         * Note: graph may be modified during this routine.
         */
        void construct(Graph& graph, TreeDecompositionAlg alg);
        void greedyFillIn(Graph& graph);
        void reindex();
};

/*@}*/

// Inline functions for NTreeBag

inline NTreeBag::NTreeBag(int size) :
        size_(size),
        elements_(new int[size_]),
        parent_(0),
        sibling_(0),
        children_(0),
        type_(0),
        subtype_(0) {
}

inline NTreeBag::NTreeBag(const NTreeBag& cloneMe) :
        size_(cloneMe.size_),
        elements_(new int[cloneMe.size_]),
        parent_(0),
        sibling_(0),
        children_(0),
        type_(0),
        subtype_(0) {
    for (int i = 0; i < size_; ++i)
        elements_[i] = cloneMe.elements_[i];
}

inline NTreeBag::~NTreeBag() {
    NTreeBag* tmp;
    while (children_) {
        tmp = children_;
        children_ = children_->sibling_;
        delete tmp;
    }
    delete[] elements_;
}

inline int NTreeBag::size() const {
    return size_;
}

inline int NTreeBag::element(int which) const {
    return elements_[which];
}

inline int NTreeBag::index() const {
    return index_;
}

inline int NTreeBag::type() const {
    return type_;
}

inline int NTreeBag::subtype() const {
    return subtype_;
}

inline const NTreeBag* NTreeBag::parent() const {
    return parent_;
}

inline const NTreeBag* NTreeBag::children() const {
    return children_;
}

inline const NTreeBag* NTreeBag::sibling() const {
    return sibling_;
}

inline bool NTreeBag::isLeaf() const {
    return (! children_);
}

inline void NTreeBag::insertChild(NTreeBag* child) {
    child->parent_ = this;
    child->sibling_ = children_;
    children_ = child;
}

inline void NTreeBag::swapContents(NTreeBag& other) {
    int s = size_; size_ = other.size_; other.size_ = s;
    int* e = elements_; elements_ = other.elements_; other.elements_ = e;
}

// Inline functions for NTreeDecomposition

inline NTreeDecomposition::~NTreeDecomposition() {
    delete root_;
}

inline int NTreeDecomposition::width() const {
    return width_;
}

inline int NTreeDecomposition::size() const {
    return size_;
}

inline const NTreeBag* NTreeDecomposition::root() const {
    return root_;
}

inline const NTreeBag* NTreeDecomposition::firstPrefix() const {
    return root_;
}

inline void NTreeDecomposition::reindex() {
    size_ = 0;
    for (const NTreeBag* b = first(); b; b = b->next())
        const_cast<NTreeBag*>(b)->index_ = size_++;
}

// Inline functions for NTreeDecomposition::Graph

inline NTreeDecomposition::Graph::Graph(int order) :
        order_(order), adj_(new bool*[order]) {
    int i, j;
    for (i = 0; i < order; ++i) {
        adj_[i] = new bool[order];
        for (j = 0; j < order; ++j)
            adj_[i][j] = false;
    }
}

inline NTreeDecomposition::Graph::~Graph() {
    for (int i = 0; i < order_; ++i)
        delete[] adj_[i];
    delete[] adj_;
}

} // namespace regina

#endif

