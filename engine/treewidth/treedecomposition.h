
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

/*! \file treewidth/treedecomposition.h
 *  \brief Deals with treewidth and tree decompositions, in particular
 *  for facet pairing graphs.
 */

#ifndef __REGINA_TREEDECOMPOSITION_H
#ifndef __DOXYGEN
#define __REGINA_TREEDECOMPOSITION_H
#endif

#include <vector>
#include "regina-core.h"
#include "core/output.h"
#include "maths/matrix.h"
#include "triangulation/forward.h"
#include "utilities/exception.h"

namespace regina {

class Link;
class TreeBag;

/**
 * \defgroup treewidth Treewidth
 * Treewidth and tree decompositions.
 */

/**
 * Indicates which algorithm should be used to compute a tree decomposition
 * of a graph.
 *
 * Additional algorithms may be added to this list in future versions of
 * Regina.
 *
 * \ingroup treewidth
 */
enum class TreeDecompositionAlg {
    /**
     * Indicates that a fast upper bound algorithm should be used.
     *
     * This does not promise to find a tree decomposition of smallest
     * possible width (an NP-hard problem), but it does promise to run
     * in small polynomial time.
     *
     * This constant \a TreeDecompositionAlg::Upper indicates that the
     * "most appropriate" upper bound algorithm should be used.  This is a good
     * choice for users who just want a good tree decomposition and want it
     * quickly, without needing to know the details of how it was produced.
     */
    Upper = 0x0001,
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
    UpperGreedyFillIn = 0x0001
};

/**
 * A deprecated constant indicating an algorithm for computing tree
 * decompositions.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * TreeDecompositionAlg::Upper.
 */
[[deprecated]] inline constexpr TreeDecompositionAlg TD_UPPER =
    TreeDecompositionAlg::Upper;
/**
 * A deprecated constant indicating an algorithm for computing tree
 * decompositions.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * TreeDecompositionAlg::UpperGreedyFillIn.
 */
[[deprecated]] inline constexpr TreeDecompositionAlg TD_UPPER_GREEDY_FILL_IN =
    TreeDecompositionAlg::UpperGreedyFillIn;

/**
 * Indicates the relationship between two bags in a tree decomposition.
 *
 * \ingroup treewidth
 */
enum class BagComparison {
    /**
     * Indicates that the two bags have identical contents.
     */
    Equal = 0,
    /**
     * Indicates that the first bag is a strict subset of the second.
     */
    Subset = -1,
    /**
     * Indicates that the first bag is a strict superset of the second.
     */
    Superset = 1,
    /**
     * Indicates that neither bag is a subset of the other.
     */
    Unrelated = 2
};

/**
 * A deprecated constant indicating a relationship between bags in a tree
 * decomposition.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * BagComparison::Equal.
 */
[[deprecated]] inline constexpr BagComparison BAG_EQUAL = BagComparison::Equal;
/**
 * A deprecated constant indicating a relationship between bags in a tree
 * decomposition.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * BagComparison::Subset.
 */
[[deprecated]] inline constexpr BagComparison BAG_SUBSET =
    BagComparison::Subset;
/**
 * A deprecated constant indicating a relationship between bags in a tree
 * decomposition.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * BagComparison::Superset.
 */
[[deprecated]] inline constexpr BagComparison BAG_SUPERSET =
    BagComparison::Superset;
/**
 * A deprecated constant indicating a relationship between bags in a tree
 * decomposition.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * BagComparison::Unrelated.
 */
[[deprecated]] inline constexpr BagComparison BAG_UNRELATED =
    BagComparison::Unrelated;

/**
 * Used to indicate the type of each bag in a _nice_ tree decomposition.
 *
 * A nice tree decomposition is produced by calling
 * TreeDecomposition::makeNice().  As a result:
 *
 * - every bag will be either an _introduce_ bag, a _forget_ bag, or a
 *   _join_ bag, as defined below;
 *
 * - the root bag will be a forget bag, and will be empty;
 *
 * - every leaf bag will be an introduce bag, containing precisely one node.
 *
 * See TreeDecomposition::makeNice() for further details, and see
 * TreeBag::niceType() and TreeBag::niceIndex() for how to access this
 * information for each bag.
 *
 * \ingroup treewidth
 */
enum class NiceType {
    /**
     * Indicates that either the underlying tree decomposition is not nice,
     * or the details of the nice tree decomposition have not yet been
     * computed.
     */
    None = 0,
    /**
     * Indicates an introduce bag.  An _introduce_ bag has only one child bag.
     * It contains all of the nodes in this child bag plus exactly one
     * new node, and contains no other nodes besides these.
     *
     * As a special case, a leaf bag (which has no child bags at all) is
     * also considered to be an introduce bag.  In this case, the leaf bag
     * contains exactly one node.
     */
    Introduce = 1,
    /**
     * Indicates a forget bag.  A _forget_ bag has only one child bag.
     * It contains all of the nodes in this child bag except for exactly one
     * missing node, and contains no other nodes besides these.
     */
    Forget = 2,
    /**
     * Indicates a join bag.  A _join_ bag has exactly two child bags,
     * where the join bag and both of its child bags are all identical.
     */
    Join = 3
};

/**
 * A deprecated constant indicating a type of bag in a nice tree decomposition.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NiceType::Introduce.
 */
[[deprecated]] inline constexpr NiceType NICE_INTRODUCE = NiceType::Introduce;
/**
 * A deprecated constant indicating a type of bag in a nice tree decomposition.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NiceType::Forget.
 */
[[deprecated]] inline constexpr NiceType NICE_FORGET = NiceType::Forget;
/**
 * A deprecated constant indicating a type of bag in a nice tree decomposition.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NiceType::Join.
 */
[[deprecated]] inline constexpr NiceType NICE_JOIN = NiceType::Join;

/**
 * Represents a single bag in a tree decomposition.
 *
 * The class TreeDecomposition is used to build, manipulate and iterate
 * over tree decompositions of graphs.  A tree decomposition of a graph \a G
 * consists of (i) an underlying tree \a T; and (ii) a _bag_ at every node of
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
 * - If the underlying tree decomposition is a _nice_ tree decomposition (and
 *   this nice structure has actually been computed, typically via makeNice()),
 *   then you can call niceType() and niceIndex() to access the specific role
 *   that each bag plays in the nice structure.
 *
 * To _build_ a tree decomposition of a graph, see the various
 * TreeDecomposition class constructors.
 *
 * Note that a bag may be empty (indeed, if you call
 * TreeDecomposition::makeNice() then it is guaranteed that the root bag
 * will be empty).
 *
 * Tree bags do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the TreeDecomposition to which they belong.
 *
 * \ingroup treewidth
 */
class TreeBag : public ShortOutput<TreeBag> {
    private:
        size_t size_;
            /**< The number of nodes (of the graph \a G) stored in this bag. */
        size_t* elements_;
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
        NiceType niceType_;
            /**< Used where relevant to indicate the role that this bag plays
                 in a nice tree decomposition.  See niceType() for details. */
        ssize_t niceIndex_;
            /**< Used where relevant to give more precise information (in
                 addition to \a niceType_) on the role that this bag plays
                 in a nice tree decomposition.  See niceIndex() for details. */
        size_t index_;
            /**< Indicates the index of this bag within the underlying
                 tree decomposition, following a leaves-to-root, left-to-right
                 ordering of the bags.  See index() for details. */

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
         * \return the number of graph nodes in this bag.
         */
        size_t size() const;
        /**
         * Used to query the individual graph nodes stored in this bag.
         *
         * Suppose this is a bag in a tree decomposition of some graph \a G,
         * whose nodes are numbered 0,1,2,....
         * Then `element(i)` returns the number of the <i>i</i>th
         * node stored in this bag.
         *
         * Nodes are always stored in ascending order.  This means that
         * `element(0) < element(1) < element(2) < ...`.
         *
         * \param which indicates which node should be returned; this
         * must be between 0 and size()-1 inclusive.
         * \return the number of the corresponding node stored in this bag.
         */
        size_t element(size_t which) const;
        /**
         * Queries whether a given graph node is contained in this bag.
         *
         * Suppose this is a bag in a tree decomposition of some graph \a G,
         * whose nodes are numbered 0,1,2,....
         * Then `contains(x)` queries whether the node numbered \a x
         * is contained in this bag.
         *
         * \param element the number of some node in the graph \a G.
         * \return \c true if and only if the given node is in this bag.
         */
        bool contains(size_t element) const;

        /**
         * Returns the index of this bag within the full tree decomposition.
         *
         * Suppose the entire tree decomposition contains \a n bags.
         * Then these bags are automatically numbered 0,1,...,<i>n</i>-1.
         * This member function returns the number of this particular bag.
         *
         * The numbering of bags follows a leaves-to-root, left-to-right
         * scheme:
         *
         * - for any non-root bag \a b, we have
         *   `b.index() < b.parent()->index()`;
         *
         * - for any bag \a b with a next sibling, we have
         *   `b.index() < b.sibling()->index()`;
         *
         * \return the index of this bag within the full tree decomposition
         * \a d; this will be between 0 and `d.size()-1` inclusive.
         */
        size_t index() const;

        /**
         * Returns the role that this bag plays in a nice tree decomposition,
         * if this information is known.
         *
         * This information is only available if the underlying tree
         * decomposition is nice _and_ this nice structure has actually
         * been computed.  For this to happen, either:
         *
         * - TreeDecomposition::makeNice() must have been called upon this
         *   tree decomposition; or
         *
         * - this tree decomposition must have been copied, moved or assigned
         *   from some other nice tree decomposition for which this information
         *   had likewise been computed.
         *
         * For introduce and forget bags (i.e., where niceType() returns either
         * NiceType::Introduce or NiceType::Forget), the function niceIndex()
         * returns additional information on the role that this bag plays
         * within the overall nice tree decomposition.
         *
         * See TreeDecomposition::makeNice() for further information.
         *
         * \return the role that this bag plays in a nice tree decomposition,
         * or NiceType::None if this information is not available (either
         * because the tree decomposition is not nice, or because its nice
         * structure has not been computed).
         */
        NiceType niceType() const;
        /**
         * Deprecated function that returns the role that this bag plays in a
         * nice tree decomposition, if this information is known.
         *
         * \deprecated This function has been named to niceType(), which
         * returns a properly-typed NiceType instead of an \c int.
         * See niceType() for further details.
         *
         * \python For Python users, this function returns a NiceType, not an
         * \c int, so that comparisons with the NiceType constants works as
         * expected.  This is because, now that NiceType is a scoped enum,
         * Python comparisons between _any_ integer and _any_ NiceType constant
         * will always return that the values are not equal.
         *
         * \return the non-zero integer value of a NiceType constant indicating
         * the role that this bag plays in a nice tree decomposition, or zero
         * if this information is not available.
         */
        [[deprecated]] int type() const;
        /**
         * Returns additional details on the role that an introduce or forget
         * bag plays in a nice tree decomposition.
         *
         * This function is only relevant if niceType() returns either
         * NiceType::Introduce or NiceType::Forget.  That is, the underlying
         * tree decomposition must be nice, _and_ this nice structure must have
         * actually been computed, _and_ this bag must be an introduce bag or
         * a forget bag.
         *
         * In this case, niceIndex() gives information on which specific node
         * of the underyling graph has been added (in the case of an introduce
         * bag) or removed (in the case of a forget bag).  This information
         * will be returned as an _index_ into either this bag or its child bag
         * respectively.
         *
         * See TreeDecomposition::makeNice() for further information.
         *
         * \return details on the role that an introduce or forget bag plays
         * in a nice tree decomposition, or undefined if this information is
         * unknown and/or irrelevant (i.e., niceType() does not return either
         * NiceType::Introduce or NiceType::Forget).
         */
        ssize_t niceIndex() const;
        /**
         * Deprecated function that returns additional details on the role
         * that an introduce or forget bag plays in a nice tree decomposition.
         *
         * \deprecated This function has been renamed to niceIndex().
         * See niceIndex() for further details.
         *
         * \return details on the role that an introduce or forget bag plays
         * in a nice tree decomposition, or undefined if this information is
         * unknown and/or irrelevant (i.e., niceType() does not return either
         * NiceType::Introduce or NiceType::Forget).
         */
        [[deprecated]] ssize_t subtype() const;

        /**
         * Determines if there is a subset/superset relationship between
         * this and the given bag.
         *
         * Recall that, in a tree decomposition of a graph \a G, each
         * bag is a set of nodes of \a G.  This function will return one
         * of the following constants:
         *
         * - BagComparison::Equal if this and \a rhs are equal;
         * - BagComparison::Subset if this bag is a strict subset of \a rhs;
         * - BagComparison::Superset if this bag is a strict superset of \a rhs;
         * - BagComparison::Unrelated if neither this nor \a rhs is a subset
         *   of the other.
         *
         * \param rhs the bag to compare with this.
         * \return the relationship between the two bags, as outlined above.
         */
        BagComparison compare(const TreeBag& rhs) const;

        /**
         * Used for a postfix iteration through all of the bags in a tree
         * decomposition.  Amongst other things, a _postfix_ iteration is
         * one in which all of the children of any bag \a b will be processed
         * before \a b itself.
         *
         * If \a d is a non-empty tree decomposition, then you can complete a
         * full postfix iteration of bags as follows:
         *
         * - the first bag in a postfix iteration is `d.first()`;
         * - the next bag after \a b in the iteration is `b.next()`;
         * - the iteration terminates when `b.next()` is \c null.
         *
         * This iteration processes the children of each bag in order;
         * that is, it processes each bag \a b before `b.sibling()`
         * (if the latter exists).
         *
         * The bags in a tree decomposition are indexed as 0,1,2,...,
         * as described by the index() member function.  This postfix
         * iteration is equivalent to iterating through bags 0,1,2,...
         * in order.
         *
         * \return the next bag after this in a postfix iteration of all
         * bags, or \c null if this is the final bag in such an iteration
         * (i.e., the root bag).
         */
        const TreeBag* next() const;
        /**
         * Used for a prefix iteration through all of the bags in a tree
         * decomposition.  Amongst other things, a _prefix_ iteration is
         * one in which each bag will be processed before any of its children.
         *
         * If \a d is a non-empty tree decomposition, then you can complete a
         * full prefix iteration of bags as follows:
         *
         * - the first bag in a prefix iteration is `d.firstPrefix()`
         *   (or equivalently, `d.root()`);
         * - the next bag after \a b in the iteration is
         *   `b.nextPrefix()`;
         * - the iteration terminates when `b.nextPrefix()` is \c null.
         *
         * This iteration processes the children of each bag in order;
         * that is, it processes each bag \a b before `b.sibling()`
         * (if the latter exists).
         *
         * \return the next bag after this in a prefix iteration of all
         * bags, or \c null if this is the final bag in such an iteration.
         */
        const TreeBag* nextPrefix() const;

        /**
         * Returns the parent of this bag in the underlying rooted tree.
         *
         * \return the parent of this bag, or \c null if this bag is at
         * the root of the tree.
         */
        const TreeBag* parent() const;
        /**
         * Returns the first child of this bag in the underlying rooted tree.
         *
         * If a bag has no children, then children() will be \c null.
         * If a bag has many children, then these will be
         * `children()`, `children()->sibling()`,
         * `children()->sibling()->sibling()`, and so on.
         *
         * \return the first child of this bag, or \c null if this is a
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
         * `b.children()`, `b.children()->sibling()`,
         * `b.children()->sibling()->sibling()`, and so on.
         *
         * \return the next sibling of this bag, or \c null if either
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
         * \return \c true if and only if this is a leaf bag.
         */
        bool isLeaf() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        // Make this class non-copyable.
        // (There is a copy constructor, but it is private.)
        TreeBag& operator = (const TreeBag&) = delete;

    private:
        /**
         * Creates a new bag with space allocated to hold the given number of
         * graph nodes.
         *
         * Specifically, the member \a size_ will be set and the array
         * \e elements_ will be allocated to this size, but the _contents_ of
         * the array (i.e., the graph nodes themselves) will be left
         * uninitialised.
         *
         * This new bag will not be inserted into the tree, and will not
         * be assigned any other information (such as index, type or subtype).
         *
         * \param size the number of nodes that will be contained in the
         * new bag.
         */
        TreeBag(size_t size);

        /**
         * Creates a new bag containing the same graph nodes as the given bag.
         *
         * Specifically, only the list of nodes stored in the given bag will
         * be cloned.  This new bag will not be inserted into the tree
         * (instead all parent, child and sibling pointers will be \c null),
         * and it will not be assigned any other information (such as index,
         * type or subtype).
         *
         * \param src the bag whose contents should be cloned.
         */
        TreeBag(const TreeBag& src);

        /**
         * Inserts the given bag into the tree as the first child of this bag.
         *
         * Any existing children of this bag will become siblings of \a child.
         *
         * \pre The given bag has not yet been inserted into the tree.
         *
         * \param child the bag to insert into the tree.
         */
        void insertChild(TreeBag* child);

        /**
         * Swaps the contents of this and the given bag.
         *
         * This only swaps the nodes stored in each bag.  It does not
         * switch the positions of the two bags in the underlying tree.
         *
         * \param other the bag to swap contents with this.
         */
        void swapNodes(TreeBag& other) noexcept;

        /**
         * Adjusts the links between bags to make this bag the root of
         * the tree decomposition.  If the tree decomposition is still
         * being constructed and therefore consists of several disjoint trees,
         * this routine will only affect the tree containing this bag.
         *
         * This routine adjusts the \a parent_, \a children_ and \a sibling_
         * links of various bags, but nothing else.  The caller of this
         * routine must (if appropriate) separately set
         * TreeDecomposition::root_ to this bag, and must (eventually) call
         * TreeDecomposition::reindex() to update the bag indices to
         * follow a leaves-to-root, left-to-right ordering as expected.
         *
         * This routine runs in linear time.
         */
        void makeRoot();

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
 * (i) an underlying tree \a T; and (ii) a _bag_ at every node of
 * this tree.  Each bag is a set of zero or more nodes of \a G, and
 * these bags are subject to the following constraints:
 *
 * - Every node of \a G belongs to some bag;
 *
 * - Every arc of \a G has both its endpoints in some common bag;
 *
 * - For every node \a v of \a G, the set of _all_ bags containing \a v
 *   forms a (connected) subtree of \a T.
 *
 * In Regina, the underlying tree \a T is a rooted tree, so that every
 * non-root bag has exactly one parent bag, and every bag has some
 * number of children (possibly many, possibly zero).
 *
 * Tree decompositions are generally considered "better" if their bags
 * are smaller (i.e., contain fewer nodes of \a G).  To this end, the
 * _width_ of a tree decomposition is one less than its largest bag size,
 * and the _treewidth_ of \a G is the minimum width over all tree
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
 * This class also numbers its bags 0,1,...,size()-1 in a leaves-to-root,
 * left-to-right manner:
 *
 * - for each non-root bag \a b, the parent of \a b will have a higher index
 *   than \a b;
 *
 * - for each bag \a b with a next sibling, the next sibling of \a b will
 *   have a higher index than \a b.
 *
 * This bag numbering may be useful if you wish to store auxiliary information
 * alongside each bag in a separate array.  You can access this numbering
 * through the function TreeBag::index().  However, note that
 * TreeDecomposition does _not_ store its bags in an array, and so
 * the "random access" function bag() is slow, with worst-case linear time.
 *
 * There are two broad classes of algorithms for building tree
 * decompositions: (i) _exact_ algorithms, which are slow but guarantee to
 * find a tree decomposition of the smallest possible width; and
 * (ii) _greedy_ algorithms, which are fast and which aim to keep the width
 * small but which do not promise minimality.  Currently Regina only
 * offers greedy algorithms, though this may change in a future release.
 * See the TreeDecompositionAlg enumeration for a list of all algorithms
 * that are currently available.
 *
 * Note that individual bags are allowed to be empty.  Moreover, if the
 * underlying graph \a G is empty then the tree decomposition may
 * contain no bags at all.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup treewidth
 */
class TreeDecomposition : public Output<TreeDecomposition> {
    protected:
        /**
         * Represents a graph, which may be directed or undirected.
         *
         * This is an internal class, used to convert input graphs into
         * a common format before passing them to the main tree
         * decomposition algorithms.
         */
        struct Graph {
            size_t order_;
                /**< The number of nodes in the graph. */
            bool** adj_;
                /**< The adjacency matrix for the graph.  Specifically,
                     adj_[i][j] is \c true if and only if there is an
                     arc from node \a i to node \a j. */

            /**
             * Constructs a new graph with no arcs.
             *
             * \param order the number of nodes in the new graph.
             */
            Graph(size_t order);
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
             * \param out the output stream to which to write.
             */
            void dump(std::ostream& out) const;

            // Make this class non-copyable.
            Graph(const Graph&) = delete;
            Graph& operator = (const Graph&) = delete;
        };

    private:
        ssize_t width_;
            /**< The width of this tree decomposition; that is, one less
                 than the maximum bag size. */
        size_t size_;
            /**< The number of bags in this tree decomposition. */
        TreeBag* root_;
            /**< The bag at the root of the underlying tree. */

    public:
        /**
         * Builds a new copy of the given tree decomposition.
         *
         * This will be a deep copy, in the sense that all of the bags
         * of \a src will be cloned also.
         *
         * \param src the tree decomposition to clone.
         */
        TreeDecomposition(const TreeDecomposition& src);

        /**
         * Moves the contents of the given tree decomposition into this
         * new tree decomposition.  This is a fast (constant time) operation.
         *
         * The tree decomposition that was passed (\a src) will no longer be
         * usable.
         *
         * \param src the tree decomposition to move.
         */
        TreeDecomposition(TreeDecomposition&& src) noexcept;

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
         * \python This constructor is only available in Python when
         * \a dim is one of Regina's \ref stddim "standard dimensions".
         *
         * \param triangulation the triangulation whose facet pairing
         * graph we are working with.
         * \param alg the algorithm that should be used to compute the
         * tree decomposition; in particular, this specifies whether to
         * use a slow exact algorithm or a fast greedy algorithm.
         */
        template <int dim> requires (supportedDim(dim))
        TreeDecomposition(
            const Triangulation<dim>& triangulation,
            TreeDecompositionAlg alg = TreeDecompositionAlg::Upper);

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
         * \python This constructor is only available in Python when
         * \a dim is one of Regina's \ref stddim "standard dimensions".
         *
         * \param pairing the facet pairing graph that we are working with.
         * \param alg the algorithm that should be used to compute the
         * tree decomposition; in particular, this specifies whether to
         * use a slow exact algorithm or a fast greedy algorithm.
         */
        template <int dim> requires (supportedDim(dim))
        TreeDecomposition(
            const FacetPairing<dim>& pairing,
            TreeDecompositionAlg alg = TreeDecompositionAlg::Upper);

        /**
         * Builds a tree decomposition of the 4-valent multigraph
         * corresponding to the given knot or link diagram.
         *
         * The nodes of the graph will be numbered in the same way as
         * the crossings of the given knot / link.
         *
         * \param link the knot or link that we are working with.
         * \param alg the algorithm that should be used to compute the
         * tree decomposition; in particular, this specifies whether to
         * use a slow exact algorithm or a fast greedy algorithm.
         */
        TreeDecomposition(const Link& link,
            TreeDecompositionAlg alg = TreeDecompositionAlg::Upper);

        /**
         * Builds a tree decomposition of an arbitrary graph.
         * The graph may be directed or undirected.
         *
         * The graph is specified by an adjacency matrix, expressed
         * using Regina's own matrix type.
         *
         * Each entry \a graph[i][j] will be treated as a boolean, indicating
         * whether the graph contains an arc from node \a i to node \a j.
         *
         * \exception InvalidArgument The adjacency matrix does not have
         * the same number of rows as columns.
         *
         * \python The argument \a graph must be of type \c MatrixBool
         * (which is the Python type corresponding to the C++ class
         * Matrix<bool>).
         *
         * \param graph the adjacency matrix of the graph.
         * \param alg the algorithm that should be used to compute the
         * tree decomposition; in particular, this specifies whether to
         * use a slow exact algorithm or a fast greedy algorithm.
         */
        template <typename T>
        TreeDecomposition(const Matrix<T>& graph,
            TreeDecompositionAlg alg = TreeDecompositionAlg::Upper);

        /**
         * Builds a tree decomposition of an arbitrary graph.
         * The graph may be directed or undirected.
         *
         * The graph is specified by an adjacency matrix, given as a
         * vector of rows:
         *
         * - The number of elements in each row should be equal to the
         *   number of rows (i.e., the adjacency matrix should be square).
         *
         * - The individual elements of each row \a r should be accessible
         *   using a range-based \c for loop over \a r.
         *
         * - Each entry in row \a i, column \a j will be treated as a boolean,
         *   indicating whether the graph contains an arc from node \a i to
         *   node \a j.
         *
         * An example of a suitable type for the adjacency matrix could be
         * std::vector<std::vector<bool>>.
         *
         * \exception InvalidArgument The adjacency matrix does not have
         * the same number of rows as columns.
         *
         * \python The adjacency matrix should be given as a list of
         * lists.
         *
         * \param graph the adjacency matrix of the graph.
         * \param alg the algorithm that should be used to compute the
         * tree decomposition; in particular, this specifies whether to
         * use a slow exact algorithm or a fast greedy algorithm.
         */
        template <typename Row>
        TreeDecomposition(const std::vector<Row>& graph,
            TreeDecompositionAlg alg = TreeDecompositionAlg::Upper);

        /**
         * Destroys this tree decomposition and all of its bags.
         */
        ~TreeDecomposition();

        /**
         * Sets this to be a copy of the given tree decomposition.
         *
         * This will be a deep copy, in the sense that all of the bags
         * of \a src will be copied also.
         *
         * It does not matter if this and the given tree decomposition were
         * originally built from different and/or differently sized objects
         * or graphs.
         *
         * \param src the tree decomposition to copy.
         * \return a reference to this tree decomposition.
         */
        TreeDecomposition& operator = (const TreeDecomposition& src);

        /**
         * Moves the contents of the given tree decomposition into this
         * tree decomposition.  This is a fast (constant time) operation.
         *
         * The tree decomposition that was passed (\a src) will no longer be
         * usable.
         *
         * It does not matter if this and the given tree decomposition were
         * originally built from different and/or differently sized objects
         * or graphs.
         *
         * \param src the tree decomposition to move.
         * \return a reference to this tree decomposition.
         */
        TreeDecomposition& operator = (TreeDecomposition&& src) noexcept;

        /**
         * Swaps the contents of this and the given tree decomposition.
         *
         * \param other the tree decomposition whose contents should be
         * swapped with this.
         */
        void swap(TreeDecomposition& other) noexcept;

        /**
         * Returns the width of this tree decomposition.
         * This is one less than the size of the largest bag.
         *
         * \return the width of this tree decomposition.
         */
        ssize_t width() const;
        /**
         * Returns the number of bags in this tree decomposition.
         *
         * \return the number of bags.
         */
        size_t size() const;

        /**
         * Determines whether this and the given tree decomposition are
         * identical.
         *
         * To be considered \a identical, the two tree decompositions must
         * have the same number of bags; moreover, the same numbered bags must
         * contain the same elements (i.e., numbered graph nodes), and must
         * have the same numbered child bags.  Bag types and subtypes
         * are ignored.
         *
         * \param other the tree decomposition to compare with this.
         * \return \c true if and only if this and the given tree
         * decomposition are identical.
         */
        bool operator == (const TreeDecomposition& other) const;

        /**
         * Returns the bag at the root of the underlying tree.
         *
         * \return the root bag, or \c null if there are no bags (which
         * means the underlying graph \a G is empty).
         */
        const TreeBag* root() const;
        /**
         * Used for a postfix iteration through all of the bags in the tree
         * decomposition.  Amongst other things, a _postfix_ iteration is
         * one in which all of the children of any bag \a b will be processed
         * before \a b itself.
         *
         * If \a d is a non-empty tree decomposition, then you can complete a
         * full postfix iteration of bags as follows:
         *
         * - the first bag in a postfix iteration is `d.first()`;
         * - the next bag after \a b in the iteration is `b.next()`;
         * - the iteration terminates when `b.next()` is \c null.
         *
         * This iteration processes the children of each bag in order;
         * that is, it processes each bag \a b before `b.sibling()`
         * (if the latter exists).
         *
         * This postfix iteration is equivalent to iterating through bags
         * numbered 0,1,2,...; that is, following the order of
         * TreeBag::index().
         *
         * \return the first bag in a postfix iteration of all bags, or
         * \c null if there are no bags (which means the underlying
         * graph \a G is empty).
         */
        const TreeBag* first() const;
        /**
         * Used for a prefix iteration through all of the bags in the tree
         * decomposition.  Amongst other things, a _prefix_ iteration is
         * one in which each bag will be processed before any of its children.
         *
         * If \a d is a non-empty tree decomposition, then you can complete a
         * full prefix iteration of bags as follows:
         *
         * - the first bag in a prefix iteration is `d.firstPrefix()`;
         * - the next bag after \a b in the iteration is
         *   `b.nextPrefix()`;
         * - the iteration terminates when `b.nextPrefix()` is \c null.
         *
         * This iteration processes the children of each bag in order;
         * that is, it processes each bag \a b before `b.sibling()`
         * (if the latter exists).
         *
         * Since the first bag in a prefix iteration must be the root bag,
         * this function is identical to calling root().
         *
         * \return the first bag in a prefix iteration of all bags, or
         * \c null if there are no bags (which means the underlying
         * graph \a G is empty).
         */
        const TreeBag* firstPrefix() const;

        /**
         * A slow (linear-time) routine that returns the bag at the
         * given index.
         *
         * Recall that the bags in a tree decomposition are numbered
         * 0,1,...,size()-1.  This routine returns the bag with the
         * given number.
         *
         * This routine is linear-time, and so you should _not_ use it
         * to iterate through all bags.  Instead, to iterate through all
         * bags, use TreeDecomposition::first() and TreeBag::next().
         *
         * \warning This routine is _slow_, with a worst-case linear time.
         * This is because the bags are not stored internally in an
         * array, and so this routine must search the tree from the root
         * downwards to find the bag that is being requested.
         *
         * \param index the number of a bag; this must be between 0 and
         * size()-1 inclusive.
         * \return the bag with the given number.
         */
        const TreeBag* bag(size_t index) const;

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
         * \return \c true if and only if the tree decomposition was changed.
         */
        bool compress();
        /**
         * Converts this into a nice tree decomposition.
         *
         * A _nice_ tree decomposition is one in which every bag is one of
         * the following types:
         *
         * - an _introduce_ bag, which has only one child bag, and which
         *   contains all of the nodes in this child bag plus exactly one
         *   new node (and nothing else);
         *
         * - a _forget_ bag, which has only one child bag, and which contains
         *   all of the nodes in this child bag except for exactly one
         *   missing node (and nothing else);
         *
         * - a _join_ bag, which has exactly two child bags, and where each
         *   child bag contains exactly the same nodes as the join bag itself.
         *
         * As a special case, each leaf bag (which has no child bags at all)
         * is also considered to be an introduce bag, and will contain
         * exactly one node.
         *
         * This routine will also ensure that the root bag is a forget bag,
         * containing no nodes at all.
         *
         * This routine will set TreeBag::niceType() and TreeBag::niceIndex()
         * for each bag as follows:
         *
         * - TreeBag::niceType() will be one of the enumeration constants
         *   NiceType::Introduce, NiceType::Forget or NiceType::Join,
         *   indicating whether the bag is an introduce, forget or join bag
         *   respectively.
         *
         * - For an introduce bag \a b, TreeBag::niceIndex() will indicate
         *   which "new" node was introduced.  Specifically, the new node
         *   will be `b.element(b.niceIndex())`.
         *
         * - For a forget bag \a b, TreeBag::niceIndex() will indicate
         *   which "missing" node was forgotten.  Specifically, the missing
         *   node will be `b.children()->element(b.niceIndex())`.
         *
         * - For a join bag, TreeBag::niceIndex() will be undefined.
         *
         * If the underlying graph is empty, then this routine will
         * produce a tree decomposition with no bags at all.
         *
         * You may optionally pass an argument \a heightHint, which is an array
         * indicating how close to the root of the tree you would like each
         * node to be.  At present, this only affects the final chain of
         * forget bags leading up to the root bag - if \a heightHint is
         * passed, then this chain will be ordered so that nodes with a
         * larger \a heightHint will be forgotten closer to the root bag.
         * These should be considered hints only, in that their effect on the
         * final tree decomposition might change in future versions of Regina.
         *
         * \warning Note that TreeBag::niceIndex() is _not_ the number of
         * the new or missing node, but instead gives the _index_ of the
         * new or missing node within the relevant bag.
         *
         * \note This routine calls compress() automatically, and so
         * there is no need to explicitly call compress() before calling
         * makeNice().
         *
         * \python If a \e heightHint argument is given, it should
         * be passed as a Python list of integers.
         *
         * \param heightHint an optional array where, for each node \a i,
         * a higher value of `heightHint[i]` indicates that the node
         * should be forgotten closer to the root bag.  If this is non-null,
         * then the size of this array should be the number of nodes in
         * the underlying graph.
         */
        void makeNice(const int* heightHint = nullptr);

        /**
         * Reverses child-parent relationships so that the given bag
         * becomes the root of the tree decomposition.
         *
         * All pointers to bags will remain valid, and the contents of
         * the bags will not change.  However:
         *
         * - the bags will be reindexed, to reflect the changes in the
         *   leaves-to-root, left-to-right ordering;
         *
         * - all bag types will be reset to 0, since in general rerooting may
         *   break whatever properties the bag types and subtypes represent.
         *
         * If the given bag is already the root bag, then this routine
         * does nothing (and in particular, types and subtypes are preserved).
         *
         * \param newRoot the bag that should become the root of this
         * tree decomposition.  This must already be a bag of this tree
         * decomposition.
         */
        void reroot(TreeBag* newRoot);

        /**
         * Reroots the tree by reversing child-parent relationships, in a way
         * that minimises a maximum estimated processing cost amongst all bags.
         *
         * The user needs to supply three arrays, which are used to
         * estimate the cost of hanging the tree from any possible root.
         * This routine then identifies which root minimises this cost, and
         * reroots the underlying tree from that bag.
         *
         * The three arrays play the following roles.  Let \a b be some bag
         * at index \a i in the original tree decomposition, and let \a p be
         * its parent bag.
         *
         * - `costSame[i]` indicates the cost of _preserving_ the
         *   parent-child relationship between \a b and \a p (i.e.,
         *   after rerooting, \a p is still the parent bag of \a b).
         *   If \e b is the root bag of the original tree decomposition
         *   then `costSame[i]` is ignored.
         *
         * - `costReverse[i]` indicates the cost of _reversing_ the
         *   parent-child relationship between \a b and \a p (i.e.,
         *   after rerooting, \a b is now the parent bag of \a p).
         *   Again, if \e b is the root bag of the original tree decomposition
         *   then `costReverse[i]` is ignored.
         *
         * - `costRoot[i]` is an additional cost that is incurred
         *   if and only if \a b becomes the new root bag.  The argument
         *   \a costRoot may be \c null, in which case these additional
         *   costs are all assumed to be zero.
         *
         * It follows that, for each potential new root, there are size()
         * costs to aggregate: this comes from size()-1 costs from the arrays
         * \a costSame and/or \a costReverse (one for each connection
         * between bags in the underlying tree), and one cost from \a costRoot.
         * These costs will be aggregated by taking the _maximum_ over
         * all individual costs.  This means that you do not need to
         * estimate running times and/or memory consumption accurately;
         * instead you only need to find some heuristic that aims to be
         * _monotonic_ in time and/or memory.
         *
         * So: in essence then, this routine minimises the maximum cost.
         * In the case of a tie, it then minimises multiplicity; that is,
         * it minimises the _number_ of times that this maximum cost occurs
         * over the individual size() costs that are being aggregated.
         *
         * Note that the \a costSame and \a costReverse arrays are
         * technically defined as a cost per arc, not a cost per bag.
         * If you wish to estimate a cost per bag, the typical way of
         * doing this would be:
         *
         * - `costSame[i]` estimates the processing cost at bag \a i
         *   if its relationship with its parent is preserved;
         *
         * - `costReverse[i]` estimates the processing cost at the
         *   original _parent_ of bag \a i if its relationship with bag \a i
         *   is reversed (i.e., it becomes a child of bag \a i);
         *
         * - `costRoot[i]` estimates the processing cost at bag \a i
         *   if bag \a i becomes the root.
         *
         * This scheme ensures that, for any possible rerooting, each
         * bag is costed exactly once amongst the three arrays.
         *
         * After rerooting, all pointers to bags will remain valid, and the
         * contents of the bags will not change.  However:
         *
         * - the bags will be reindexed, to reflect the changes in the
         *   leaves-to-root, left-to-right ordering;
         *
         * - all bag types will be reset to 0, since in general rerooting may
         *   break whatever properties the bag types and subtypes represent.
         *
         * If the given bag is already the root bag, then this routine
         * does nothing (and in particular, types and subtypes are preserved).
         *
         * \headers This routine is implemented in a separate header
         * (treedecomposition-impl.h), which is not included automatically
         * by this file.  However, typical end users should never need this
         * extra header, since Regina's calculation engine already includes
         * explicit instantiations for common types.
         *
         * \python The \a costSame and \a costReverse arrays,
         * as well as \a costRoot if it is given, should be passed as
         * Python lists of real numbers.
         *
         * \tparam T the type being used to estimate costs.
         * It must be possible to assign 0 to a variable of type \a T
         * using both constructors and the assignment operator.
         *
         * \param costSame An array of size() elements giving an
         * estimated cost of preserving each child-parent connection;
         * \param costReverse An array of size() elements giving an
         * estimated cost of reversing each child-parent connection;
         * \param costRoot An array of size() elements giving an
         * additional estimated cost for each bag being the new root.
         * This array may be \c null.
         */
        template <typename T>
        void reroot(const T* costSame, const T* costReverse,
            const T* costRoot = nullptr);

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
         * Calling `td.writeDot(out)` is equivalent to `out << td.dot()`.
         * However, this routine is more efficient.
         *
         * \nopython Instead use the variant dot() that takes no arguments
         * and returns a string.
         *
         * \param out the output stream to which to write.
         * \param dark \c true if the graph is intended to be displayed in dark
         * mode (in particular, with black or darkly coloured surroundings), or
         * \c false (the default) if it is intended to be displayed in light
         * mode (with white or lightly coloured surroundings).
         *
         * \see http://www.graphviz.org/
         */
        void writeDot(std::ostream& out, bool dark = false) const;

        /**
         * Returns a Graphviz DOT representation of this tree decomposition.
         * This string can be saved as a standalone DOT file, which in turn
         * can be run through Graphviz in order to visualise the tree
         * decomposition.
         *
         * This routine generates a directed graph (with arrows running
         * from parent bags to their children).  The nodes of this graph
         * will be labelled in a way that indicates the tetrahedra
         * contained in each bag.  The resulting DOT file should be used
         * with the \a dot program shipped with Graphviz.
         *
         * This routine simply returns the output of writeDot() as a
         * string, instead of dumping it to an output stream.
         *
         * If you are writing this text representation to an output stream
         * then you should call writeDot() instead, which is more efficient.
         *
         * \param dark \c true if the graph is intended to be displayed in dark
         * mode (in particular, with black or darkly coloured surroundings), or
         * \c false (the default) if it is intended to be displayed in light
         * mode (with white or lightly coloured surroundings).
         *
         * \return the DOT representation of this tree decomposition,
         * as outlined above.
         */
        std::string dot(bool dark = false) const;

        /**
         * Outputs this tree decomposition using the PACE text format.
         * This text format is described in detail at
         * https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/ ,
         * and is documented in detail by the routine
         * fromPACE(const std::string&).
         *
         * If you write a tree decomposition using pace() or writePACE()
         * and then read it again using fromPACE(), you are _not_ guaranteed
         * to obtain an identical tree decomposition.  This is because
         * the PACE text format stores the connections between bags as an
         * undirected, unrooted tree.
         *
         * Calling `td.writePACE(out)` is equivalent to `out << td.pace()`.
         * However, this routine is more efficient.
         *
         * \nopython Instead use the variant pace() that takes no arguments
         * and returns a string.
         *
         * \param out the output stream to which to write.
         *
         * \see https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/
         */
        void writePACE(std::ostream& out) const;
        /**
         * Returns a text representation of this tree decomposition using the
         * PACE text format.  This text format is described in detail at
         * https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/ ,
         * and is documented in detail by the routine
         * fromPACE(const std::string&).
         *
         * If you write a tree decomposition using pace() or writePACE()
         * and then read it again using fromPACE(), you are _not_ guaranteed
         * to obtain an identical tree decomposition.  This is because
         * the PACE text format stores the connections between bags as an
         * undirected, unrooted tree.
         *
         * If you are writing this text representation to an output stream
         * then you should call writePACE() instead, which is more efficient.
         *
         * \return the PACE text representation of this tree decomposition.
         *
         * \see https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/
         */
        std::string pace() const;

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

        /**
         * Builds a tree decomposition from a string using the PACE text format.
         * The text format is described in detail at
         * https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/ .
         *
         * In short, the format contains a number of lines of text:
         *
         * - Any line beginning with \c c is considered a comment, and
         *   will be ignored.
         *
         * - The first non-comment line should be of the form
         *   `s td <num_bags> <max_bag_size> <num_vertices>`.
         *
         * - The next \a num_bags non-comment lines should describe the
         *   contents of the bags.  Each such line should be of the form
         *   `b <bag_number> <element> <element> ...`.
         *   The bags are numbered 1,2,...,\a num_bags, and may appear in any
         *   order.  Likewise, the vertices of the graph are numbered
         *   1,2,...,\a num_vertices, and within each bag they may again
         *   appear in any order.
         *
         * - The remaining \a num_bags - 1 non-comment lines should
         *   indicate the connections between the bags in the tree
         *   decomposition.  Each such line should be of the form
         *   `<first_bag_index> <second_bag_index>`,
         *   where \a first_bag_index is smaller than \a second_bag_index.
         *
         * Bags may be empty, but there must be at least one bag, and the
         * connections between the bags must form a tree.  This routine will
         * choose the root of the tree arbitrarily.
         *
         * An example of this text format is as follows:
         *
           \verbatim
           c A tree decomposition with 4 bags and width 2
           s td 4 3 5
           b 1 1 2 3
           b 2 2 3 4
           b 3 3 4 5
           b 4
           1 2
           2 3
           2 4
           \endverbatim
         *
         * There are two variants of this routine.  This variant
         * contains a single string containing the entire text representation.
         * The other variant takes an input stream, from which the text
         * representation will be read.
         *
         * \warning While this routine does some basic error checking on
         * the input, this checking is not exhaustive; in particular, it does
         * not verify that the connections between bags actually form a tree.
         *
         * \exception InvalidArgument The input was not a valid representation
         * of a tree decomposition using the PACE text format.
         * As noted above, the checks performed here are not exhaustive.
         *
         * \param str a text representation of the tree
         * decomposition using the PACE text format.
         * \return the corresponding tree decomposition.
         *
         * \see https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/
         */
        static TreeDecomposition fromPACE(const std::string& str);
        /**
         * Builds a tree decomposition from an input stream using the PACE
         * text format.  The text format is described in detail at
         * https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/ .
         *
         * See the routine fromPACE(const std::string&) for
         * a description of this text format.
         *
         * There are two variants of this routine.  The other variant
         * contains a single string containing the entire text representation.
         * This variant takes an input stream, from which the text
         * representation will be read.
         *
         * This routine assumes that it should exhaust the input stream
         * (i.e., it should contain no additional text after this text
         * representation).
         *
         * \exception InvalidArgument The input was not a valid representation
         * of a tree decomposition using the PACE text format.
         * As documented more thoroughly in the string variant of this
         * routine, the checks performed here are not exhaustive.
         *
         * \nopython Instead you can use the variant of fromPACE() that
         * takes a string.
         *
         * \param in an input stream that provides a text
         * representation of the tree decomposition using the PACE text format.
         * \return the corresponding tree decomposition.
         *
         * \see https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/
         */
        static TreeDecomposition fromPACE(std::istream& in);

    private:
        /**
         * A constructor that initialises all data members to zero
         * and/or \c null.
         */
        TreeDecomposition();
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
         *
         * This routine also recomputes the data member \a size_.
         */
        void reindex();
};

/**
 * Swaps the contents of the two given tree decompositions.
 *
 * This global routine simply calls TreeDecomposition::swap(); it is provided
 * so that TreeDecomposition meets the C++ Swappable requirements.
 *
 * \param a the first tree decomposition whose contents should be swapped.
 * \param b the second tree decomposition whose contents should be swapped.
 *
 * \ingroup treewidth
 */
void swap(TreeDecomposition& a, TreeDecomposition& b) noexcept;

// Inline functions for TreeBag

inline TreeBag::TreeBag(size_t size) :
        size_(size),
        elements_(new size_t[size_]),
        parent_(nullptr),
        sibling_(nullptr),
        children_(nullptr),
        niceType_(NiceType::None),
        niceIndex_(0) {
}

inline TreeBag::TreeBag(const TreeBag& src) :
        size_(src.size_),
        elements_(new size_t[src.size_]),
        parent_(nullptr),
        sibling_(nullptr),
        children_(nullptr),
        niceType_(NiceType::None),
        niceIndex_(0) {
    std::copy(src.elements_, src.elements_ + size_, elements_);
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

inline size_t TreeBag::size() const {
    return size_;
}

inline size_t TreeBag::element(size_t which) const {
    return elements_[which];
}

inline size_t TreeBag::index() const {
    return index_;
}

inline NiceType TreeBag::niceType() const {
    return niceType_;
}

inline int TreeBag::type() const {
    return static_cast<int>(niceType_);
}

inline ssize_t TreeBag::niceIndex() const {
    return niceIndex_;
}

inline ssize_t TreeBag::subtype() const {
    return niceIndex_;
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

inline void TreeBag::swapNodes(TreeBag& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(elements_, other.elements_);
}

// Inline functions for TreeDecomposition

inline TreeDecomposition::TreeDecomposition() :
        width_(0), size_(0), root_(nullptr) {
}

inline TreeDecomposition::TreeDecomposition(TreeDecomposition&& src) noexcept :
        width_(src.width_), size_(src.size_), root_(src.root_) {
    src.root_ = nullptr;
}

template <typename T>
TreeDecomposition::TreeDecomposition(const Matrix<T>& graph,
        TreeDecompositionAlg alg) : width_(0), root_(nullptr) {
    if (graph.rows() != graph.columns())
        throw InvalidArgument("The adjacency matrix must be square");

    Graph g(graph.rows());

    for (size_t i = 0; i < graph.rows(); ++i)
        for (size_t j = 0; j < graph.columns(); ++j)
            g.adj_[i][j] = graph.entry(i, j) || graph.entry(j, i);

    construct(g, alg);
}

template <typename Row>
TreeDecomposition::TreeDecomposition(const std::vector<Row>& graph,
        TreeDecompositionAlg alg) : width_(0), root_(nullptr) {
    size_t order = graph.size();
    Graph g(order);

    size_t r = 0;
    for (const auto& row : graph) {
        size_t c = 0;
        for (const auto& entry : row) {
            if (c >= order)
                throw InvalidArgument("The adjacency matrix must be square");
            if (entry)
                g.adj_[r][c] = g.adj_[c][r] = true;
            ++c;
        }
        if (c != order)
            throw InvalidArgument("The adjacency matrix must be square");
        ++r;
    }

    construct(g, alg);
}

inline TreeDecomposition::~TreeDecomposition() {
    delete root_;
}

inline TreeDecomposition& TreeDecomposition::operator = (
        const TreeDecomposition& src) {
    TreeDecomposition copy(src); // all the hard work is here
    swap(copy); // let copy dispose of our original contents
    return *this;
}

inline TreeDecomposition& TreeDecomposition::operator = (
        TreeDecomposition&& src) noexcept {
    width_ = src.width_;
    size_ = src.size_;
    std::swap(root_, src.root_);
    // Let src dispose of the original bags in its own destructor.
    return *this;
}

inline void TreeDecomposition::swap(TreeDecomposition& other) noexcept {
    std::swap(width_, other.width_);
    std::swap(size_, other.size_);
    std::swap(root_, other.root_);
}

inline ssize_t TreeDecomposition::width() const {
    return width_;
}

inline size_t TreeDecomposition::size() const {
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

inline void swap(TreeDecomposition& a, TreeDecomposition& b) noexcept {
    a.swap(b);
}

// Inline functions for TreeDecomposition::Graph

inline TreeDecomposition::Graph::Graph(size_t order) :
        order_(order), adj_(new bool*[order]) {
    for (size_t i = 0; i < order; ++i) {
        adj_[i] = new bool[order];
        std::fill(adj_[i], adj_[i] + order, false);
    }
}

inline TreeDecomposition::Graph::~Graph() {
    for (size_t i = 0; i < order_; ++i)
        delete[] adj_[i];
    delete[] adj_;
}

} // namespace regina

#endif

