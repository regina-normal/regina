
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

/*! \file treewidth/treedecomposition.h
 *  \brief Deals with treewidth and tree decompositions, in particular
 *  for facet pairing graphs.
 */

#ifndef __TREEDECOMPOSITION_H
#ifndef __DOXYGEN
#define __TREEDECOMPOSITION_H
#endif

#include "regina-core.h"
#include "output.h"
#include "triangulation/forward.h"
#include <boost/noncopyable.hpp>

namespace regina {

/**
 * \addtogroup treewidth Treewidth
 * Treewidth and tree decompositions.
 * @{
 */

class TreeBag;

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
 * TreeDecomposition::makeNice().  As a result:
 *
 * - every bag will be either an \e introduce bag, a \e forget bag, or a
 *   \e join bag, as defined below;
 *
 * - the root bag will be a forget bag, and will be empty;
 *
 * - every leaf bag will be an introduce bag, containing precisely one node.
 *
 * See TreeDecomposition::makeNice() for further details, including how
 * TreeBag::type() and TreeBag::subtype() are defined for a nice tree
 * decomposition.
 */
enum NiceType {
    /**
     * Indicates an introduce bag.  An \e introduce bag has only one child bag.
     * It contains all of the nodes in this child bag plus exactly one
     * new node, and contains no other nodes besides these.
     *
     * As a special case, a leaf bag (which has no child bags at all) is
     * also considered to be an introduce bag.  In this case, the leaf bag
     * contains exactly one node.
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
 * The class TreeDecomposition is used to build, manipulate and iterate
 * over tree decompositions of graphs.  A tree decomposition of a graph \a G
 * consists of (i) an underlying tree \a T; and (ii) a \e bag at every node of
 * this tree.  Each bag is a set of zero or more nodes of \a G, and
 * these bags are subject to various constraints as described in the
 * TreeDecomposition class notes.
 *
 * In Regina, the underlying tree \a T is a rooted tree, so that every
 * non-root bag has exactly one parent bag, and every bag has some
 * number of children (possibly many, possibly zero).
 *
 * This class TreeBag represents a single bag in a tree decomposition.
 *
 * - You can query which nodes of \a G the bag contains through the member
 *   functions size(), element() and contains().  It is assumed that the
 *   nodes of \a G are numbered 0,1,2,..., and so the nodes stored in this
 *   bag are simply represented as integers.
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
 *
 * To \e build a tree decomposition of a graph, see the various
 * TreeDecomposition class constructors.
 *
 * Note that a bag may be empty (indeed, if you call
 * TreeDecomposition::makeNice() then it is guaranteed that the root bag
 * will be empty).
 */
class REGINA_API TreeBag :
        public ShortOutput<TreeBag>,
        public boost::noncopyable {
    private:
        int size_;
            /**< The number of nodes (of the graph \a G) stored in this bag. */
        int* elements_;
            /**< The individual nodes of this bag, sorted in ascending order. */
        TreeBag* parent_;
            /**< The parent of this bag in the underlying tree \a T,
                 or \c null if this is the root bag. */
        TreeBag* sibling_;
            /**< The next sibling of this bag in the underlying tree \a T,
                 or \c null if this is the final child of the parent bag. */
        TreeBag* children_;
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
        ~TreeBag();

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
         * Suppose this is a bag in a tree decomposition of some graph \a G,
         * whose nodes are numbered 0,1,2,....
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
         * Suppose this is a bag in a tree decomposition of some graph \a G,
         * whose nodes are numbered 0,1,2,....
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
         * \e nice tree decompositions.  See TreeDecomposition::makeNice()
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
         * \e nice tree decompositions.  See TreeDecomposition::makeNice()
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
        BagComparison compare(const TreeBag& rhs) const;

        /**
         * Used for a postfix iteration through all of the bags in a tree
         * decomposition.  Amongst other things, a \e postfix iteration is
         * one in which all of the children of any bag \a b will be processed
         * before \a b itself.
         *
         * If \a d is a non-empty tree decomposition, then you can complete a
         * full postfix iteration of bags as follows:
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
        const TreeBag* next() const;
        /**
         * Used for a prefix iteration through all of the bags in a tree
         * decomposition.  Amongst other things, a \e prefix iteration is
         * one in which each bag will be processed before any of its children.
         *
         * If \a d is a non-empty tree decomposition, then you can complete a
         * full prefix iteration of bags as follows:
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
        const TreeBag* nextPrefix() const;

        /**
         * Returns the parent of this bag in the underlying rooted tree.
         *
         * @return the parent of this bag, or \c null if this bag is at
         * the root of the tree.
         */
        const TreeBag* parent() const;
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
        const TreeBag* children() const;
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
        const TreeBag* sibling() const;
        /**
         * Determines if this is a leaf bag.  A leaf bag is a bag with
         * no children in the underlying tree.
         *
         * This is equivalent to testing whether children() is \c null.
         *
         * @return \c true if and only if this is a leaf bag.
         */
        bool isLeaf() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
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
        TreeBag(int size);

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
        TreeBag(const TreeBag& cloneMe);

        /**
         * Inserts the given bag into the tree as the first child of this bag.
         *
         * Any existing children of this bag will become siblings of \a child.
         *
         * \pre The given bag has not yet been inserted into the tree.
         *
         * @param child the bag to insert into the tree.
         */
        void insertChild(TreeBag* child);

        /**
         * Swaps the contents of this and the given bag.
         *
         * This only swaps the nodes stored in each bag.  It does not
         * switch the positions of the two bags in the underlying tree.
         *
         * @param other the bag to swap contents with this.
         */
        void swapContents(TreeBag& other);

    friend class TreeDecomposition;
};

/**
 * Represents a tree decomposition of a graph.
 *
 * Whilst this class can be used to build tree decompositions of
 * arbitrary graphs, it also offers a simple interface for building a
 * tree decomposition of the facet pairing graph of a given triangulation.
 * This is an important step in the implementation of fixed-parameter
 * tractable algorithms on triangulated manifolds.
 *
 * Given a graph \a G, a tree decomposition of \a G consists of
 * (i) an underlying tree \a T; and (ii) a \e bag at every node of
 * this tree.  Each bag is a set of zero or more nodes of \a G, and
 * these bags are subject to the following constraints:
 *
 * - Every node of \a G belongs to some bag;
 *
 * - Every arc of \a G has both its endpoints in some common bag;
 *
 * - For every node \a v of \a G, the set of \e all bags containing \a v
 *   forms a (connected) subtree of \a T.
 *
 * In Regina, the underlying tree \a T is a rooted tree, so that every
 * non-root bag has exactly one parent bag, and every bag has some
 * number of children (possibly many, possibly zero).
 *
 * Tree decompositions are generally considered "better" if their bags
 * are smaller (i.e., contain fewer nodes of \a G).  To this end, the
 * \e width of a tree decomposition is one less than its largest bag size,
 * and the \e treewidth of \a G is the minimum width over all tree
 * decompositions of \a G.
 *
 * A tree decomposition is described by a single TreeDecomposition object,
 * and the class TreeBag is used to represent each individual bag.
 *
 * - You can build a tree decomposition using the various
 *   TreeDecomposition constructors, and manipulate it using member
 *   functions such as compress() and makeNice().
 *
 * - To iterate through the bags of the tree decomposition, you can use
 *   TreeDecomposition::first() and TreeBag::next() (for a postfix
 *   iteration), or you can use TreeDecomposition::firstPrefix() and
 *   TreeBag::nextPrefix() (for a prefix iteration).
 *
 * The bags themselves are stored as sets of integers: it is assumed that the
 * nodes of \a G are numbered 0,1,2,..., and so the bags simply store the
 * numbers of the nodes that they contain.
 *
 * This class also numbers its bags 0,1,...,size()-1 in a leaves-to-root
 * manner; that is, for each non-root bag \a b, the parent of \a b will
 * have a higher index than \a b itself.  This may be useful if you wish
 * to store auxiliary information alongside each bag in a separate array.
 * You can access this numbering through the function TreeBag::index().
 * (Note that TreeDecomposition does not store its bags in an array, and so
 * does not offer a "random access" function to access the bag at an
 * arbitrary index.)
 *
 * There are two broad classes of algorithms for building tree
 * decompositions: (i) \e exact algorithms, which are slow but guarantee to
 * find a tree decomposition of the smallest possible width; and
 * (ii) \e greedy algorithms, which are fast and which aim to keep the width
 * small but which do not promise minimality.  Currently Regina only
 * offers greedy algorithms, though this may change in a future release.
 * See the TreeDecompositionAlg enumeration for a list of all algorithms
 * that are currently available.
 *
 * Note that individual bags are allowed to be empty.  Moreover, if the
 * underlying graph \a G is empty then the tree decomposition may
 * contain no bags at all.
 */
class REGINA_API TreeDecomposition :
        public Output<TreeDecomposition>,
        public boost::noncopyable {
    protected:
        /**
         * Represents a graph, which may be directed or undirected.
         *
         * This is an internal class, used to convert input graphs into
         * a common format before passing them to the main tree
         * decomposition algorithms.
         *
         * \ifacespython Not present.
         */
        struct Graph {
            int order_;
                /**< The number of nodes in the graph. */
            bool** adj_;
                /**< The adjacency matrix for the graph.  Specifically,
                     adj_[i][j] is \c true if and only if there is an
                     arc from node \a i to node \a j. */

            /**
             * Constructs a new graph with no arcs.
             *
             * @param order the number of nodes in the new graph.
             */
            Graph(int order);
            /**
             * Destroys this graph.
             */
            ~Graph();

            /**
             * Writes the adjacency matrix of this graph in a compact
             * format to the given output stream.
             *
             * The output will be formatted as a matrix, and will be
             * spread across multiple lines.
             *
             * @param out the output stream to which to write.
             */
            void dump(std::ostream& out) const;
        };

    private:
        int width_;
            /**< The width of this tree decomposition; that is, one less
                 than the maximum bag size. */
        int size_;
            /**< The number of bags in this tree decomposition. */
        TreeBag* root_;
            /**< The bag at the root of the underlying tree. */

    public:
        /**
         * Builds a tree decomposition of the facet pairing graph of the
         * given triangulation.
         *
         * The nodes of the graph will be numbered in the same way as
         * the top-dimensional simplices of the given triangulation.
         *
         * \headers This routine is implemented in a separate header
         * (treedecomposition-impl.h), which is not included automatically
         * by this file.  However, typical end users should never need this
         * extra header, since Regina's calculation engine already includes
         * explicit instantiations for \ref stddim "standard dimensions".
         *
         * \ifacespython This constructor is only available in Python when
         * \a dim is one of Regina's \ref stddim "standard dimensions".
         *
         * @param triangulation the triangulation whose facet pairing
         * graph we are working with.
         * @param alg the algorithm that should be used to compute the
         * tree decomposition; in particular, this specifies whether to
         * use a slow exact algorithm or a fast greedy algorithm.
         */
        template <int dim>
        TreeDecomposition(
            const Triangulation<dim>& triangulation,
            TreeDecompositionAlg alg = TD_UPPER);

        /**
         * Builds a tree decomposition of the given facet pairing graph.
         *
         * The nodes of the graph will be numbered in the same way as
         * the top-dimensional simplices of the given triangulation.
         *
         * \headers This routine is implemented in a separate header
         * (treedecomposition-impl.h), which is not included automatically
         * by this file.  However, typical end users should never need this
         * extra header, since Regina's calculation engine already includes
         * explicit instantiations for \ref stddim "standard dimensions".
         *
         * \ifacespython This constructor is only available in Python when
         * \a dim is one of Regina's \ref stddim "standard dimensions".
         *
         * @param pairing the facet pairing graph that we are working with.
         * @param alg the algorithm that should be used to compute the
         * tree decomposition; in particular, this specifies whether to
         * use a slow exact algorithm or a fast greedy algorithm.
         */
        template <int dim>
        TreeDecomposition(
            const FacetPairing<dim>& pairing,
            TreeDecompositionAlg alg = TD_UPPER);

        /**
         * Builds a tree decomposition of an arbitrary graph.
         * The graph may be directed or undirected.
         *
         * The graph is specified by an adjacency matrix.  The matrix
         * may contain any data type (this is the template argument \a T).
         * However, the contents of this matrix will be interpreted as
         * booleans: an arc runs from node \a i to node \a j if and
         * only if \a graph[i][j] is \c true when interpreted as a boolean.
         *
         * \headers This routine is implemented in a separate header
         * (treedecomposition-impl.h), which is not included automatically
         * by this file.  Regina's calculation engine already includes
         * explicit instantiations for types \c bool and \c int, but for
         * other types you will need to include treedecomposition-impl.h
         * along with this header.
         *
         * \ifacespython The adjacency matrix should be given as a
         * list of lists.  There is no need to use the same data type \a T
         * throughout: each element of the matrix will be individually
         * interpreted as a boolean as described above.
         *
         * @param order the number of nodes in the graph.
         * @param graph the adjacency matrix of the graph.
         * @param alg the algorithm that should be used to compute the
         * tree decomposition; in particular, this specifies whether to
         * use a slow exact algorithm or a fast greedy algorithm.
         */
        template <typename T>
        TreeDecomposition(unsigned order, T const** const graph,
            TreeDecompositionAlg alg = TD_UPPER);

        /**
         * Destroys this tree decomposition and all of its bags.
         */
        ~TreeDecomposition();

        /**
         * Returns the width of this tree decomposition.
         * This is one less than the size of the largest bag.
         *
         * @return the width of this tree decomposition.
         */
        int width() const;
        /**
         * Returns the number of bags in this tree decomposition.
         *
         * @return the number of bags.
         */
        int size() const;

        /**
         * Returns the bag at the root of the underlying tree.
         *
         * @return the root bag, or \c null if there are no bags (which
         * means the underlying graph \a G is empty).
         */
        const TreeBag* root() const;
        /**
         * Used for a postfix iteration through all of the bags in the tree
         * decomposition.  Amongst other things, a \e postfix iteration is
         * one in which all of the children of any bag \a b will be processed
         * before \a b itself.
         *
         * If \a d is a non-empty tree decomposition, then you can complete a
         * full postfix iteration of bags as follows:
         *
         * - the first bag in a postfix iteration is <tt>d.first()</tt>;
         * - the next bag after \a b in the iteration is <tt>b.next()</tt>;
         * - the iteration terminates when <tt>b.next()</tt> is \c null.
         *
         * This iteration processes the children of each bag in order;
         * that is, it processes each bag \a b before <tt>b.sibling()</tt>
         * (if the latter exists).
         *
         * This postfix iteration is equivalent to iterating through bags
         * numbered 0,1,2,...; that is, following the order of
         * TreeBag::index().
         *
         * @return the first bag in a postfix iteration of all bags, or
         * \c null if there are no bags (which means the underlying
         * graph \a G is empty).
         */
        const TreeBag* first() const;
        /**
         * Used for a prefix iteration through all of the bags in the tree
         * decomposition.  Amongst other things, a \e prefix iteration is
         * one in which each bag will be processed before any of its children.
         *
         * If \a d is a non-empty tree decomposition, then you can complete a
         * full prefix iteration of bags as follows:
         *
         * - the first bag in a prefix iteration is <tt>d.firstPrefix()</tt>;
         * - the next bag after \a b in the iteration is
         *   <tt>b.nextPrefix()</tt>;
         * - the iteration terminates when <tt>b.nextPrefix()</tt> is \c null.
         *
         * This iteration processes the children of each bag in order;
         * that is, it processes each bag \a b before <tt>b.sibling()</tt>
         * (if the latter exists).
         *
         * Since the first bag in a prefix iteration must be the root bag,
         * this function is identical to calling root().
         *
         * @return the first bag in a prefix iteration of all bags, or
         * \c null if there are no bags (which means the underlying
         * graph \a G is empty).
         */
        const TreeBag* firstPrefix() const;

        /**
         * Removes redundant bags from this tree decomposition.
         *
         * Specifically, this routine "compresses" the tree decomposition
         * as follows: whenever two bags are adjacent in the underlying
         * tree and one is a subset of the other, these bags will be merged.
         *
         * Note that some TreeBag objects may be destroyed (thereby
         * invalidating pointers or references to them), and for those
         * bags that are not destroyed, their indices (as returned by
         * TreeBag::index()) may change.
         *
         * @return \c true if and only if the tree decomposition was changed.
         */
        bool compress();
        /**
         * Converts this into a nice tree decomposition.
         *
         * A \e nice tree decomposition is one in which every bag is one of
         * the following types:
         *
         * - an \e introduce bag, which has only one child bag, and which
         *   contains all of the nodes in this child bag plus exactly one
         *   new node (and nothing else);
         *
         * - a \e forget bag, which has only one child bag, and which contains
         *   all of the nodes in this child bag except for exactly one
         *   missing node (and nothing else);
         *
         * - a \e join bag, which has exactly two child bags, and where each
         *   child bag contains exactly the same nodes as the join bag itself.
         *
         * As a special case, each leaf bag (which has no child bags at all)
         * is also considered to be an introduce bag, and will contain
         * exactly one node.
         *
         * This routine will also ensure that the root bag is a forget bag,
         * containing no nodes at all.
         *
         * This routine will set TreeBag::type() and TreeBag::subtype()
         * for each bag as follows:
         *
         * - TreeBag::type() will be one of the constants from the
         *   NiceType enumeration, indicating whether the bag is an
         *   introduce, forget or join bag.
         *
         * - For an introduce bag \a b, TreeBag::subtype() will indicate
         *   which "new" node was introduced.  Specifically, the new node
         *   will be <tt>b.element(b.subtype())</tt>.
         *
         * - For a forget bag \a b, TreeBag::subtype() will indicate
         *   which "missing" node was forgotten.  Specifically, the missing
         *   node will be <tt>b.children()->element(b.subtype())</tt>.
         *
         * - For a join bag, TreeBag::subtype() will be undefined.
         *
         * If the underlying graph is empty, then this routine will
         * produce a tree decomposition with no bags at all.
         *
         * \warning Note that TreeBag::subtype() is \e not the number of
         * the new or missing node, but instead gives the \e index of the
         * new or missing node within the relevant bag.
         *
         * \note This routine calls compress() automatically, and so
         * there is no need to explicitly call compress() before calling
         * makeNice().
         */
        void makeNice();

        /**
         * Outputs this tree decomposition in the Graphviz DOT language.
         * This produces a standalone DOT file that can be run through
         * Graphviz in order to visualise the tree decomposition.
         *
         * This routine generates a directed graph (with arrows running
         * from parent bags to their children).  The nodes of this graph
         * will be labelled in a way that indicates the tetrahedra
         * contained in each bag.  The resulting DOT file should be used
         * with the \a dot program shipped with Graphviz.
         *
         * \ifacespython The \a out argument is not present; instead
         * standard output is assumed.
         *
         * @param out the output stream to which to write.
         *
         * @see http://www.graphviz.org/
         */
        void writeDot(std::ostream& out) const;

        /**
         * Returns a Graphviz DOT representation of this tree decomposition.
         *
         * This routine simply returns the output of writeDot() as a
         * string, instead of dumping it to an output stream.
         *
         * See the writeDot() notes for further details.
         *
         * @return the output of writeDot(), as outlined above.
         */
        std::string dot() const;

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

    private:
        /**
         * Called by the various constructors to build this tree
         * decomposition from the given graph, using the given algorithm.
         *
         * The given graph may (and typically will) be modified, sometimes
         * severely and irrepairably, by this routine.
         */
        void construct(Graph& graph, TreeDecompositionAlg alg);
        /**
         * Called by construct() to build this tree decomposition from the
         * given graph, using the greedy fill-in algorithm.
         *
         * The given graph may (and typically will) be modified, sometimes
         * severely and irrepairably, by this routine.
         */
        void greedyFillIn(Graph& graph);
        /**
         * Reindexes the bags in this tree decomposition to follow a
         * postfix iteration over the tree.
         *
         * The postfix iteration is the same iteration defined by
         * TreeDecomposition::first() and TreeBag::next().
         */
        void reindex();
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NTreeBag has now been renamed to TreeBag.
 */
REGINA_DEPRECATED typedef TreeBag NTreeBag;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NTreeDecomposition has now been renamed to
 * TreeDecomposition.
 */
REGINA_DEPRECATED typedef TreeDecomposition NTreeDecomposition;

/*@}*/

// Help the compiler by noting which explicit instantiations we offer.

// Hide these from doxygen, which seems to have trouble with templated
// member functions of template classes.
#ifndef __DOXYGEN
extern template REGINA_API TreeDecomposition::TreeDecomposition(
    const Triangulation<2>&, TreeDecompositionAlg);
extern template REGINA_API TreeDecomposition::TreeDecomposition(
    const Triangulation<3>&, TreeDecompositionAlg);
extern template REGINA_API TreeDecomposition::TreeDecomposition(
    const Triangulation<4>&, TreeDecompositionAlg);

extern template REGINA_API TreeDecomposition::TreeDecomposition(
    const FacetPairing<2>&, TreeDecompositionAlg);
extern template REGINA_API TreeDecomposition::TreeDecomposition(
    const FacetPairing<3>&, TreeDecompositionAlg);
extern template REGINA_API TreeDecomposition::TreeDecomposition(
    const FacetPairing<4>&, TreeDecompositionAlg);

extern template REGINA_API TreeDecomposition::TreeDecomposition(
    unsigned, bool const** const, TreeDecompositionAlg);
extern template REGINA_API TreeDecomposition::TreeDecomposition(
    unsigned, int const** const, TreeDecompositionAlg);
#endif // ! __DOXYGEN

// Inline functions for TreeBag

inline TreeBag::TreeBag(int size) :
        size_(size),
        elements_(new int[size_]),
        parent_(0),
        sibling_(0),
        children_(0),
        type_(0),
        subtype_(0) {
}

inline TreeBag::TreeBag(const TreeBag& cloneMe) :
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

inline TreeBag::~TreeBag() {
    TreeBag* tmp;
    while (children_) {
        tmp = children_;
        children_ = children_->sibling_;
        delete tmp;
    }
    delete[] elements_;
}

inline int TreeBag::size() const {
    return size_;
}

inline int TreeBag::element(int which) const {
    return elements_[which];
}

inline int TreeBag::index() const {
    return index_;
}

inline int TreeBag::type() const {
    return type_;
}

inline int TreeBag::subtype() const {
    return subtype_;
}

inline const TreeBag* TreeBag::parent() const {
    return parent_;
}

inline const TreeBag* TreeBag::children() const {
    return children_;
}

inline const TreeBag* TreeBag::sibling() const {
    return sibling_;
}

inline bool TreeBag::isLeaf() const {
    return (! children_);
}

inline void TreeBag::insertChild(TreeBag* child) {
    child->parent_ = this;
    child->sibling_ = children_;
    children_ = child;
}

inline void TreeBag::swapContents(TreeBag& other) {
    int s = size_; size_ = other.size_; other.size_ = s;
    int* e = elements_; elements_ = other.elements_; other.elements_ = e;
}

// Inline functions for TreeDecomposition

inline TreeDecomposition::~TreeDecomposition() {
    delete root_;
}

inline int TreeDecomposition::width() const {
    return width_;
}

inline int TreeDecomposition::size() const {
    return size_;
}

inline const TreeBag* TreeDecomposition::root() const {
    return root_;
}

inline const TreeBag* TreeDecomposition::firstPrefix() const {
    return root_;
}

inline void TreeDecomposition::reindex() {
    size_ = 0;
    for (const TreeBag* b = first(); b; b = b->next())
        const_cast<TreeBag*>(b)->index_ = size_++;
}

// Inline functions for TreeDecomposition::Graph

inline TreeDecomposition::Graph::Graph(int order) :
        order_(order), adj_(new bool*[order]) {
    int i, j;
    for (i = 0; i < order; ++i) {
        adj_[i] = new bool[order];
        for (j = 0; j < order; ++j)
            adj_[i][j] = false;
    }
}

inline TreeDecomposition::Graph::~Graph() {
    for (int i = 0; i < order_; ++i)
        delete[] adj_[i];
    delete[] adj_;
}

} // namespace regina

#endif

