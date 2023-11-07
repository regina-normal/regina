/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::BagComparison
static const char *BagComparison = R"doc(Indicates the relationship between two bags in a tree decomposition.)doc";

// Docstring regina::python::doc::NiceType
static const char *NiceType =
R"doc(Used to indicate the type of each bag in a _nice_ tree decomposition.

A nice tree decomposition is produced by calling
TreeDecomposition::makeNice(). As a result:

* every bag will be either an _introduce_ bag, a _forget_ bag, or a
  _join_ bag, as defined below;

* the root bag will be a forget bag, and will be empty;

* every leaf bag will be an introduce bag, containing precisely one
  node.

See TreeDecomposition::makeNice() for further details, and see
TreeBag::niceType() and TreeBag::niceIndex() for how to access this
information for each bag.)doc";

// Docstring regina::python::doc::TreeBag
static const char *TreeBag =
R"doc(Represents a single bag in a tree decomposition.

The class TreeDecomposition is used to build, manipulate and iterate
over tree decompositions of graphs. A tree decomposition of a graph
*G* consists of (i) an underlying tree *T*; and (ii) a _bag_ at every
node of this tree. Each bag is a set of zero or more nodes of *G*, and
these bags are subject to various constraints as described in the
TreeDecomposition class notes.

In Regina, the underlying tree *T* is a rooted tree, so that every
non-root bag has exactly one parent bag, and every bag has some number
of children (possibly many, possibly zero).

This class TreeBag represents a single bag in a tree decomposition.

* You can query which nodes of *G* the bag contains through the member
  functions size(), element() and contains(). It is assumed that the
  nodes of *G* are numbered 0,1,2,..., and so the nodes stored in this
  bag are simply represented as integers.

* You can query the location of the bag in the underlying tree *T*
  through the member functions parent(), children(), sibling() and
  isLeaf().

* You can iterate through all the bags in the tree decomposition with
  the help of member functions next(), nextPrefix() and index().

* If the underlying tree decomposition is a _nice_ tree decomposition
  (and this nice structure has actually been computed, typically via
  makeNice()), then you can call niceType() and niceIndex() to access
  the specific role that each bag plays in the nice structure.

To _build_ a tree decomposition of a graph, see the various
TreeDecomposition class constructors.

Note that a bag may be empty (indeed, if you call
TreeDecomposition::makeNice() then it is guaranteed that the root bag
will be empty).

Tree bags do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the TreeDecomposition to which they belong.)doc";

// Docstring regina::python::doc::TreeDecomposition
static const char *TreeDecomposition =
R"doc(Represents a tree decomposition of a graph.

Whilst this class can be used to build tree decompositions of
arbitrary graphs, it also offers a simple interface for building a
tree decomposition of the facet pairing graph of a given
triangulation. This is an important step in the implementation of
fixed-parameter tractable algorithms on triangulated manifolds.

Given a graph *G*, a tree decomposition of *G* consists of (i) an
underlying tree *T*; and (ii) a _bag_ at every node of this tree. Each
bag is a set of zero or more nodes of *G*, and these bags are subject
to the following constraints:

* Every node of *G* belongs to some bag;

* Every arc of *G* has both its endpoints in some common bag;

* For every node *v* of *G*, the set of _all_ bags containing *v*
  forms a (connected) subtree of *T*.

In Regina, the underlying tree *T* is a rooted tree, so that every
non-root bag has exactly one parent bag, and every bag has some number
of children (possibly many, possibly zero).

Tree decompositions are generally considered "better" if their bags
are smaller (i.e., contain fewer nodes of *G*). To this end, the
_width_ of a tree decomposition is one less than its largest bag size,
and the _treewidth_ of *G* is the minimum width over all tree
decompositions of *G*.

A tree decomposition is described by a single TreeDecomposition
object, and the class TreeBag is used to represent each individual
bag.

* You can build a tree decomposition using the various
  TreeDecomposition constructors, and manipulate it using member
  functions such as compress() and makeNice().

* To iterate through the bags of the tree decomposition, you can use
  TreeDecomposition::first() and TreeBag::next() (for a postfix
  iteration), or you can use TreeDecomposition::firstPrefix() and
  TreeBag::nextPrefix() (for a prefix iteration).

The bags themselves are stored as sets of integers: it is assumed that
the nodes of *G* are numbered 0,1,2,..., and so the bags simply store
the numbers of the nodes that they contain.

This class also numbers its bags 0,1,...,size()-1 in a leaves-to-root,
left-to-right manner:

* for each non-root bag *b*, the parent of *b* will have a higher
  index than *b*;

* for each bag *b* with a next sibling, the next sibling of *b* will
  have a higher index than *b*.

This bag numbering may be useful if you wish to store auxiliary
information alongside each bag in a separate array. You can access
this numbering through the function TreeBag::index(). However, note
that TreeDecomposition does _not_ store its bags in an array, and so
the "random access" function bag() is slow, with worst-case linear
time.

There are two broad classes of algorithms for building tree
decompositions: (i) _exact_ algorithms, which are slow but guarantee
to find a tree decomposition of the smallest possible width; and (ii)
_greedy_ algorithms, which are fast and which aim to keep the width
small but which do not promise minimality. Currently Regina only
offers greedy algorithms, though this may change in a future release.
See the TreeDecompositionAlg enumeration for a list of all algorithms
that are currently available.

Note that individual bags are allowed to be empty. Moreover, if the
underlying graph *G* is empty then the tree decomposition may contain
no bags at all.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::TreeDecompositionAlg
static const char *TreeDecompositionAlg =
R"doc(Indicates which algorithm should be used to compute a tree
decomposition of a graph.

Additional algorithms may be added to this list in future versions of
Regina.)doc";

namespace BagComparison_ {

// Docstring regina::python::doc::BagComparison_::Equal
static const char *Equal = R"doc(Indicates that the two bags have identical contents.)doc";

// Docstring regina::python::doc::BagComparison_::Subset
static const char *Subset = R"doc(Indicates that the first bag is a strict subset of the second.)doc";

// Docstring regina::python::doc::BagComparison_::Superset
static const char *Superset = R"doc(Indicates that the first bag is a strict superset of the second.)doc";

// Docstring regina::python::doc::BagComparison_::Unrelated
static const char *Unrelated = R"doc(Indicates that neither bag is a subset of the other.)doc";

}

namespace NiceType_ {

// Docstring regina::python::doc::NiceType_::Forget
static const char *Forget =
R"doc(Indicates a forget bag. A _forget_ bag has only one child bag. It
contains all of the nodes in this child bag except for exactly one
missing node, and contains no other nodes besides these.)doc";

// Docstring regina::python::doc::NiceType_::Introduce
static const char *Introduce =
R"doc(Indicates an introduce bag. An _introduce_ bag has only one child bag.
It contains all of the nodes in this child bag plus exactly one new
node, and contains no other nodes besides these.

As a special case, a leaf bag (which has no child bags at all) is also
considered to be an introduce bag. In this case, the leaf bag contains
exactly one node.)doc";

// Docstring regina::python::doc::NiceType_::Join
static const char *Join =
R"doc(Indicates a join bag. A _join_ bag has exactly two child bags, where
the join bag and both of its child bags are all identical.)doc";

// Docstring regina::python::doc::NiceType_::None
static const char *None =
R"doc(Indicates that either the underlying tree decomposition is not nice,
or the details of the nice tree decomposition have not yet been
computed.)doc";

}

namespace TreeBag_ {

// Docstring regina::python::doc::TreeBag_::children
static const char *children =
R"doc(Returns the first child of this bag in the underlying rooted tree.

If a bag has no children, then children() will be ``None``. If a bag
has many children, then these will be ``children()``,
``children()->sibling()``, ``children()->sibling()->sibling()``, and
so on.

Returns:
    the first child of this bag, or ``None`` if this is a leaf bag
    (i.e., it has no children).)doc";

// Docstring regina::python::doc::TreeBag_::compare
static const char *compare =
R"doc(Determines if there is a subset/superset relationship between this and
the given bag.

Recall that, in a tree decomposition of a graph *G*, each bag is a set
of nodes of *G*. This function will return one of the following
constants:

* BagComparison::Equal if this and *rhs* are equal;

* BagComparison::Subset if this bag is a strict subset of *rhs*;

* BagComparison::Superset if this bag is a strict superset of *rhs*;

* BagComparison::Unrelated if neither this nor *rhs* is a subset of
  the other.

Parameter ``rhs``:
    the bag to compare with this.

Returns:
    the relationship between the two bags, as outlined above.)doc";

// Docstring regina::python::doc::TreeBag_::contains
static const char *contains =
R"doc(Queries whether a given graph node is contained in this bag.

Suppose this is a bag in a tree decomposition of some graph *G*, whose
nodes are numbered 0,1,2,.... Then ``contains(x)`` queries whether the
node numbered *x* is contained in this bag.

Parameter ``element``:
    the number of some node in the graph *G*.

Returns:
    ``True`` if and only if the given node is in this bag.)doc";

// Docstring regina::python::doc::TreeBag_::element
static const char *element =
R"doc(Used to query the individual graph nodes stored in this bag.

Suppose this is a bag in a tree decomposition of some graph *G*, whose
nodes are numbered 0,1,2,.... Then ``element(i)`` returns the number
of the *i*th node stored in this bag.

Nodes are always stored in ascending order. This means that
``element(0) < element(1) < element(2) < ...``.

Parameter ``which``:
    indicates which node should be returned; this must be between 0
    and size()-1 inclusive.

Returns:
    the number of the corresponding node stored in this bag.)doc";

// Docstring regina::python::doc::TreeBag_::index
static const char *index =
R"doc(Returns the index of this bag within the full tree decomposition.

Suppose the entire tree decomposition contains *n* bags. Then these
bags are automatically numbered 0,1,...,*n*-1. This member function
returns the number of this particular bag.

The numbering of bags follows a leaves-to-root, left-to-right scheme:

* for any non-root bag *b*, we have ``b.index() <
  b.parent()->index()``;

* for any bag *b* with a next sibling, we have ``b.index() <
  b.sibling()->index()``;

Returns:
    the index of this bag within the full tree decomposition *d*; this
    will be between 0 and ``d.size()-1`` inclusive.)doc";

// Docstring regina::python::doc::TreeBag_::isLeaf
static const char *isLeaf =
R"doc(Determines if this is a leaf bag. A leaf bag is a bag with no children
in the underlying tree.

This is equivalent to testing whether children() is ``None``.

Returns:
    ``True`` if and only if this is a leaf bag.)doc";

// Docstring regina::python::doc::TreeBag_::next
static const char *next =
R"doc(Used for a postfix iteration through all of the bags in a tree
decomposition. Amongst other things, a _postfix_ iteration is one in
which all of the children of any bag *b* will be processed before *b*
itself.

If *d* is a non-empty tree decomposition, then you can complete a full
postfix iteration of bags as follows:

* the first bag in a postfix iteration is ``d.first()``;

* the next bag after *b* in the iteration is ``b.next()``;

* the iteration terminates when ``b.next()`` is ``None``.

This iteration processes the children of each bag in order; that is,
it processes each bag *b* before ``b.sibling()`` (if the latter
exists).

The bags in a tree decomposition are indexed as 0,1,2,..., as
described by the index() member function. This postfix iteration is
equivalent to iterating through bags 0,1,2,... in order.

Returns:
    the next bag after this in a postfix iteration of all bags, or
    ``None`` if this is the final bag in such an iteration (i.e., the
    root bag).)doc";

// Docstring regina::python::doc::TreeBag_::nextPrefix
static const char *nextPrefix =
R"doc(Used for a prefix iteration through all of the bags in a tree
decomposition. Amongst other things, a _prefix_ iteration is one in
which each bag will be processed before any of its children.

If *d* is a non-empty tree decomposition, then you can complete a full
prefix iteration of bags as follows:

* the first bag in a prefix iteration is ``d.firstPrefix()`` (or
  equivalently, ``d.root()``);

* the next bag after *b* in the iteration is ``b.nextPrefix()``;

* the iteration terminates when ``b.nextPrefix()`` is ``None``.

This iteration processes the children of each bag in order; that is,
it processes each bag *b* before ``b.sibling()`` (if the latter
exists).

Returns:
    the next bag after this in a prefix iteration of all bags, or
    ``None`` if this is the final bag in such an iteration.)doc";

// Docstring regina::python::doc::TreeBag_::niceIndex
static const char *niceIndex =
R"doc(Returns additional details on the role that an introduce or forget bag
plays in a nice tree decomposition.

This function is only relevant if niceType() returns either
NiceType::Introduce or NiceType::Forget. That is, the underlying tree
decomposition must be nice, _and_ this nice structure must have
actually been computed, _and_ this bag must be an introduce bag or a
forget bag.

In this case, niceIndex() gives information on which specific node of
the underyling graph has been added (in the case of an introduce bag)
or removed (in the case of a forget bag). This information will be
returned as an _index_ into either this bag or its child bag
respectively.

See TreeDecomposition::makeNice() for further information.

Returns:
    details on the role that an introduce or forget bag plays in a
    nice tree decomposition, or undefined if this information is
    unknown and/or irrelevant (i.e., niceType() does not return either
    NiceType::Introduce or NiceType::Forget).)doc";

// Docstring regina::python::doc::TreeBag_::niceType
static const char *niceType =
R"doc(Returns the role that this bag plays in a nice tree decomposition, if
this information is known.

This information is only available if the underlying tree
decomposition is nice _and_ this nice structure has actually been
computed. For this to happen, either:

* TreeDecomposition::makeNice() must have been called upon this tree
  decomposition; or

* this tree decomposition must have been copied, moved or assigned
  from some other nice tree decomposition for which this information
  had likewise been computed.

For introduce and forget bags (i.e., where niceType() returns either
NiceType::Introduce or NiceType::Forget), the function niceIndex()
returns additional information on the role that this bag plays within
the overall nice tree decomposition.

See TreeDecomposition::makeNice() for further information.

Returns:
    the role that this bag plays in a nice tree decomposition, or
    NiceType::None if this information is not available (either
    because the tree decomposition is not nice, or because its nice
    structure has not been computed).)doc";

// Docstring regina::python::doc::TreeBag_::parent
static const char *parent =
R"doc(Returns the parent of this bag in the underlying rooted tree.

Returns:
    the parent of this bag, or ``None`` if this bag is at the root of
    the tree.)doc";

// Docstring regina::python::doc::TreeBag_::sibling
static const char *sibling =
R"doc(Returns the next sibling of this bag in the underlying rooted tree.

Specifically, if the parent of this bag has many children, then
sibling() will return the next child after this.

More generally, all of the children of a bag *b* can be accessed as
``b.children()``, ``b.children()->sibling()``,
``b.children()->sibling()->sibling()``, and so on.

Returns:
    the next sibling of this bag, or ``None`` if either (i) this is
    the final child of the parent bag, or (ii) this is the root bag.)doc";

// Docstring regina::python::doc::TreeBag_::size
static const char *size =
R"doc(Returns the number of graph nodes stored in this bag.

Suppose this is a bag in a tree decomposition of some graph *G*. Then
each bag is a subset of the nodes of *G*, and this function simply
returns the size of this subset.

Returns:
    the number of graph nodes in this bag.)doc";

// Docstring regina::python::doc::TreeBag_::subtype
static const char *subtype =
R"doc(Deprecated function that returns additional details on the role that
an introduce or forget bag plays in a nice tree decomposition.

.. deprecated::
    This function has been renamed to niceIndex(). See niceIndex() for
    further details.

Returns:
    details on the role that an introduce or forget bag plays in a
    nice tree decomposition, or undefined if this information is
    unknown and/or irrelevant (i.e., niceType() does not return either
    NiceType::Introduce or NiceType::Forget).)doc";

// Docstring regina::python::doc::TreeBag_::type
static const char *type =
R"doc(Deprecated function that returns the role that this bag plays in a
nice tree decomposition, if this information is known.

.. deprecated::
    This function has been named to niceType(), which returns a
    properly-typed NiceType instead of an ``int``. See niceType() for
    further details.

Python:
    For Python users, this function returns a NiceType, not an
    ``int``, so that comparisons with the NiceType constants works as
    expected. This is because, now that NiceType is a scoped enum,
    Python comparisons between _any_ integer and _any_ NiceType
    constant will always return that the values are not equal.

Returns:
    the non-zero integer value of a NiceType constant indicating the
    role that this bag plays in a nice tree decomposition, or zero if
    this information is not available.)doc";

}

namespace TreeDecompositionAlg_ {

// Docstring regina::python::doc::TreeDecompositionAlg_::Upper
static const char *Upper =
R"doc(Indicates that a fast upper bound algorithm should be used.

This does not promise to find a tree decomposition of smallest
possible width (an NP-hard problem), but it does promise to run in
small polynomial time.

This constant *TreeDecompositionAlg::Upper* indicates that the "most
appropriate" upper bound algorithm should be used. This is a good
choice for users who just want a good tree decomposition and want it
quickly, without needing to know the details of how it was produced.)doc";

// Docstring regina::python::doc::TreeDecompositionAlg_::UpperGreedyFillIn
static const char *UpperGreedyFillIn =
R"doc(Indicates that the greedy fill-in heuristic should be used.

This does not promise to find a tree decomposition of smallest
possible width (an NP-hard problem), but it does promise to run in
small polynomial time.

The greedy fill-in heuristic has been found experimentally to perform
well on general graphs (T. van Dijk, J.-P. van den Heuvel and W. Slob,
"Computing treewidth with LibTW", www.treewidth.com, 2006).
Experimentation within Regina also suggests that it performs well in
the setting of face pairing graphs of 3-manifold triangulations.)doc";

}

namespace TreeDecomposition_ {

// Docstring regina::python::doc::TreeDecomposition_::__copy
static const char *__copy =
R"doc(Builds a new copy of the given tree decomposition.

This will be a deep copy, in the sense that all of the bags of *src*
will be cloned also.

Parameter ``src``:
    the tree decomposition to clone.)doc";

// Docstring regina::python::doc::TreeDecomposition_::__eq
static const char *__eq =
R"doc(Determines whether this and the given tree decomposition are
identical.

To be considered *identical*, the two tree decompositions must have
the same number of bags; moreover, the same numbered bags must contain
the same elements (i.e., numbered graph nodes), and must have the same
numbered child bags. Bag types and subtypes are ignored.

Parameter ``other``:
    the tree decomposition to compare with this.

Returns:
    ``True`` if and only if this and the given tree decomposition are
    identical.)doc";

// Docstring regina::python::doc::TreeDecomposition_::__init
static const char *__init =
R"doc(Builds a tree decomposition of the facet pairing graph of the given
triangulation.

The nodes of the graph will be numbered in the same way as the top-
dimensional simplices of the given triangulation.

Python:
    This constructor is only available in Python when *dim* is one of
    Regina's standard dimensions.

Parameter ``triangulation``:
    the triangulation whose facet pairing graph we are working with.

Parameter ``alg``:
    the algorithm that should be used to compute the tree
    decomposition; in particular, this specifies whether to use a slow
    exact algorithm or a fast greedy algorithm.)doc";

// Docstring regina::python::doc::TreeDecomposition_::__init_2
static const char *__init_2 =
R"doc(Builds a tree decomposition of the given facet pairing graph.

The nodes of the graph will be numbered in the same way as the top-
dimensional simplices of the given triangulation.

Python:
    This constructor is only available in Python when *dim* is one of
    Regina's standard dimensions.

Parameter ``pairing``:
    the facet pairing graph that we are working with.

Parameter ``alg``:
    the algorithm that should be used to compute the tree
    decomposition; in particular, this specifies whether to use a slow
    exact algorithm or a fast greedy algorithm.)doc";

// Docstring regina::python::doc::TreeDecomposition_::__init_3
static const char *__init_3 =
R"doc(Builds a tree decomposition of the planar multigraph corresponding to
the given knot or link diagram.

The nodes of the graph will be numbered in the same way as the
crossings of the given knot / link.

Parameter ``link``:
    the knot or link that we are working with.

Parameter ``alg``:
    the algorithm that should be used to compute the tree
    decomposition; in particular, this specifies whether to use a slow
    exact algorithm or a fast greedy algorithm.)doc";

// Docstring regina::python::doc::TreeDecomposition_::__init_4
static const char *__init_4 =
R"doc(Builds a tree decomposition of an arbitrary graph. The graph may be
directed or undirected.

The graph is specified by an adjacency matrix, expressed using
Regina's own matrix type.

Each entry *graph*[i][j] will be treated as a boolean, indicating
whether the graph contains an arc from node *i* to node *j*.

Exception ``InvalidArgument``:
    The adjacency matrix does not have the same number of rows as
    columns.

Python:
    The argument *graph* must be of type ``MatrixBool`` (which is the
    Python type corresponding to the C++ class Matrix<bool>).

Parameter ``graph``:
    the adjacency matrix of the graph.

Parameter ``alg``:
    the algorithm that should be used to compute the tree
    decomposition; in particular, this specifies whether to use a slow
    exact algorithm or a fast greedy algorithm.)doc";

// Docstring regina::python::doc::TreeDecomposition_::__init_5
static const char *__init_5 =
R"doc(Builds a tree decomposition of an arbitrary graph. The graph may be
directed or undirected.

The graph is specified by an adjacency matrix, given as a vector of
rows:

* The number of elements in each row should be equal to the number of
  rows (i.e., the adjacency matrix should be square).

* The individual elements of each row *r* should be accessible using a
  range-based ``for`` loop over *r*.

* Each entry in row *i*, column *j* will be treated as a boolean,
  indicating whether the graph contains an arc from node *i* to node
  *j*.

An example of a suitable type for the adjacency matrix could be
std::vector<std::vector<bool>>.

Exception ``InvalidArgument``:
    The adjacency matrix does not have the same number of rows as
    columns.

Python:
    The adjacency matrix should be given as a list of lists.

Parameter ``graph``:
    the adjacency matrix of the graph.

Parameter ``alg``:
    the algorithm that should be used to compute the tree
    decomposition; in particular, this specifies whether to use a slow
    exact algorithm or a fast greedy algorithm.)doc";

// Docstring regina::python::doc::TreeDecomposition_::__ne
static const char *__ne =
R"doc(Determines whether this and the given tree decomposition are
different.

To be considered identical (i.e., for this comparison to return
``False``), the two tree decompositions must have the same number of
bags; moreover, the same numbered bags must contain the same elements
(i.e., numbered graph nodes), and must have the same numbered child
bags. Bag types and subtypes are ignored.

Parameter ``other``:
    the tree decomposition to compare with this.

Returns:
    ``True`` if and only if this and the given tree decomposition are
    different.)doc";

// Docstring regina::python::doc::TreeDecomposition_::bag
static const char *bag =
R"doc(A slow (linear-time) routine that returns the bag at the given index.

Recall that the bags in a tree decomposition are numbered
0,1,...,size()-1. This routine returns the bag with the given number.

This routine is linear-time, and so you should _not_ use it to iterate
through all bags. Instead, to iterate through all bags, use
TreeDecomposition::first() and TreeBag::next().

.. warning::
    This routine is _slow_, with a worst-case linear time. This is
    because the bags are not stored internally in an array, and so
    this routine must search the tree from the root downwards to find
    the bag that is being requested.

Parameter ``index``:
    the number of a bag; this must be between 0 and size()-1
    inclusive.

Returns:
    the bag with the given number.)doc";

// Docstring regina::python::doc::TreeDecomposition_::compress
static const char *compress =
R"doc(Removes redundant bags from this tree decomposition.

Specifically, this routine "compresses" the tree decomposition as
follows: whenever two bags are adjacent in the underlying tree and one
is a subset of the other, these bags will be merged.

Note that some TreeBag objects may be destroyed (thereby invalidating
pointers or references to them), and for those bags that are not
destroyed, their indices (as returned by TreeBag::index()) may change.

Returns:
    ``True`` if and only if the tree decomposition was changed.)doc";

// Docstring regina::python::doc::TreeDecomposition_::dot
static const char *dot =
R"doc(Returns a Graphviz DOT representation of this tree decomposition. This
string can be saved as a standalone DOT file, which in turn can be run
through Graphviz in order to visualise the tree decomposition.

This routine generates a directed graph (with arrows running from
parent bags to their children). The nodes of this graph will be
labelled in a way that indicates the tetrahedra contained in each bag.
The resulting DOT file should be used with the *dot* program shipped
with Graphviz.

This routine simply returns the output of writeDot() as a string,
instead of dumping it to an output stream.

If you are writing this text representation to an output stream then
you should call writeDot() instead, which is more efficient.

Parameter ``dark``:
    ``True`` if the graph is intended to be displayed in dark mode (in
    particular, with black or darkly coloured surroundings), or
    ``False`` (the default) if it is intended to be displayed in light
    mode (with white or lightly coloured surroundings).

Returns:
    the DOT representation of this tree decomposition, as outlined
    above.)doc";

// Docstring regina::python::doc::TreeDecomposition_::first
static const char *first =
R"doc(Used for a postfix iteration through all of the bags in the tree
decomposition. Amongst other things, a _postfix_ iteration is one in
which all of the children of any bag *b* will be processed before *b*
itself.

If *d* is a non-empty tree decomposition, then you can complete a full
postfix iteration of bags as follows:

* the first bag in a postfix iteration is ``d.first()``;

* the next bag after *b* in the iteration is ``b.next()``;

* the iteration terminates when ``b.next()`` is ``None``.

This iteration processes the children of each bag in order; that is,
it processes each bag *b* before ``b.sibling()`` (if the latter
exists).

This postfix iteration is equivalent to iterating through bags
numbered 0,1,2,...; that is, following the order of TreeBag::index().

Returns:
    the first bag in a postfix iteration of all bags, or ``None`` if
    there are no bags (which means the underlying graph *G* is empty).)doc";

// Docstring regina::python::doc::TreeDecomposition_::firstPrefix
static const char *firstPrefix =
R"doc(Used for a prefix iteration through all of the bags in the tree
decomposition. Amongst other things, a _prefix_ iteration is one in
which each bag will be processed before any of its children.

If *d* is a non-empty tree decomposition, then you can complete a full
prefix iteration of bags as follows:

* the first bag in a prefix iteration is ``d.firstPrefix()``;

* the next bag after *b* in the iteration is ``b.nextPrefix()``;

* the iteration terminates when ``b.nextPrefix()`` is ``None``.

This iteration processes the children of each bag in order; that is,
it processes each bag *b* before ``b.sibling()`` (if the latter
exists).

Since the first bag in a prefix iteration must be the root bag, this
function is identical to calling root().

Returns:
    the first bag in a prefix iteration of all bags, or ``None`` if
    there are no bags (which means the underlying graph *G* is empty).)doc";

// Docstring regina::python::doc::TreeDecomposition_::fromPACE
static const char *fromPACE =
R"doc(Builds a tree decomposition from a string using the PACE text format.
The text format is described in detail at
https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/ .

In short, the format contains a number of lines of text:

* Any line beginning with ``c`` is considered a comment, and will be
  ignored.

* The first non-comment line should be of the form ``s td <num_bags>
  <max_bag_size> <num_vertices>``.

* The next *num_bags* non-comment lines should describe the contents
  of the bags. Each such line should be of the form ``b <bag_number>
  <element> <element> ...``. The bags are numbered 1,2,...,*num_bags*,
  and may appear in any order. Likewise, the vertices of the graph are
  numbered 1,2,...,*num_vertices*, and within each bag they may again
  appear in any order.

* The remaining *num_bags* - 1 non-comment lines should indicate the
  connections between the bags in the tree decomposition. Each such
  line should be of the form ``<first_bag_index> <second_bag_index>``,
  where *first_bag_index* is smaller than *second_bag_index*.

Bags may be empty, but there must be at least one bag, and the
connections between the bags must form a tree. This routine will
choose the root of the tree arbitrarily.

An example of this text format is as follows:

```
c A tree decomposition with 4 bags and width 2
s td 4 3 5
b 1 1 2 3
b 2 2 3 4
b 3 3 4 5
b 4
1 2
2 3
2 4
```

There are two variants of this routine. This variant contains a single
string containing the entire text representation. The other variant
takes an input stream, from which the text representation will be
read.

.. warning::
    While this routine does some basic error checking on the input,
    this checking is not exhaustive; in particular, it does not verify
    that the connections between bags actually form a tree.

Exception ``InvalidArgument``:
    The input was not a valid representation of a tree decomposition
    using the PACE text format. As noted above, the checks performed
    here are not exhaustive.

Parameter ``str``:
    a text representation of the tree decomposition using the PACE
    text format.

Returns:
    the corresponding tree decomposition.

See also:
    https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/)doc";

// Docstring regina::python::doc::TreeDecomposition_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given tree decompositions.

This global routine simply calls TreeDecomposition::swap(); it is
provided so that TreeDecomposition meets the C++ Swappable
requirements.

Parameter ``a``:
    the first tree decomposition whose contents should be swapped.

Parameter ``b``:
    the second tree decomposition whose contents should be swapped.)doc";

// Docstring regina::python::doc::TreeDecomposition_::makeNice
static const char *makeNice =
R"doc(Converts this into a nice tree decomposition.

A _nice_ tree decomposition is one in which every bag is one of the
following types:

* an _introduce_ bag, which has only one child bag, and which contains
  all of the nodes in this child bag plus exactly one new node (and
  nothing else);

* a _forget_ bag, which has only one child bag, and which contains all
  of the nodes in this child bag except for exactly one missing node
  (and nothing else);

* a _join_ bag, which has exactly two child bags, and where each child
  bag contains exactly the same nodes as the join bag itself.

As a special case, each leaf bag (which has no child bags at all) is
also considered to be an introduce bag, and will contain exactly one
node.

This routine will also ensure that the root bag is a forget bag,
containing no nodes at all.

This routine will set TreeBag::niceType() and TreeBag::niceIndex() for
each bag as follows:

* TreeBag::niceType() will be one of the enumeration constants
  NiceType::Introduce, NiceType::Forget or NiceType::Join, indicating
  whether the bag is an introduce, forget or join bag respectively.

* For an introduce bag *b*, TreeBag::niceIndex() will indicate which
  "new" node was introduced. Specifically, the new node will be
  ``b.element(b.niceIndex())``.

* For a forget bag *b*, TreeBag::niceIndex() will indicate which
  "missing" node was forgotten. Specifically, the missing node will be
  ``b.children()->element(b.niceIndex())``.

* For a join bag, TreeBag::niceIndex() will be undefined.

If the underlying graph is empty, then this routine will produce a
tree decomposition with no bags at all.

You may optionally pass an argument *heightHint*, which is an array
indicating how close to the root of the tree you would like each node
to be. At present, this only affects the final chain of forget bags
leading up to the root bag - if *heightHint* is passed, then this
chain will be ordered so that nodes with a larger *heightHint* will be
forgotten closer to the root bag. These should be considered hints
only, in that their effect on the final tree decomposition might
change in future versions of Regina.

.. warning::
    Note that TreeBag::niceIndex() is _not_ the number of the new or
    missing node, but instead gives the _index_ of the new or missing
    node within the relevant bag.

.. note::
    This routine calls compress() automatically, and so there is no
    need to explicitly call compress() before calling makeNice().

Python:
    If a *heightHint* argument is given, it should be passed as a
    Python list of integers.

Parameter ``heightHint``:
    an optional array where, for each node *i*, a higher value of
    ``heightHint[i]`` indicates that the node should be forgotten
    closer to the root bag. If this is non-null, then the size of this
    array should be the number of nodes in the underlying graph.)doc";

// Docstring regina::python::doc::TreeDecomposition_::pace
static const char *pace =
R"doc(Returns a text representation of this tree decomposition using the
PACE text format. This text format is described in detail at
https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/ , and
is documented in detail by the routine fromPACE(const std::string&).

If you write a tree decomposition using pace() or writePACE() and then
read it again using fromPACE(), you are _not_ guaranteed to obtain an
identical tree decomposition. This is because the PACE text format
stores the connections between bags as an undirected, unrooted tree.

If you are writing this text representation to an output stream then
you should call writePACE() instead, which is more efficient.

Returns:
    the PACE text representation of this tree decomposition.

See also:
    https://pacechallenge.wordpress.com/pace-2016/track-a-treewidth/)doc";

// Docstring regina::python::doc::TreeDecomposition_::reroot
static const char *reroot =
R"doc(Reverses child-parent relationships so that the given bag becomes the
root of the tree decomposition.

All pointers to bags will remain valid, and the contents of the bags
will not change. However:

* the bags will be reindexed, to reflect the changes in the leaves-to-
  root, left-to-right ordering;

* all bag types will be reset to 0, since in general rerooting may
  break whatever properties the bag types and subtypes represent.

If the given bag is already the root bag, then this routine does
nothing (and in particular, types and subtypes are preserved).

Parameter ``newRoot``:
    the bag that should become the root of this tree decomposition.
    This must already be a bag of this tree decomposition.)doc";

// Docstring regina::python::doc::TreeDecomposition_::reroot_2
static const char *reroot_2 =
R"doc(Reroots the tree by reversing child-parent relationships, in a way
that minimises a maximum estimated processing cost amongst all bags.

The user needs to supply three arrays, which are used to estimate the
cost of hanging the tree from any possible root. This routine then
identifies which root minimises this cost, and reroots the underlying
tree from that bag.

The three arrays play the following roles. Let *b* be some bag at
index *i* in the original tree decomposition, and let *p* be its
parent bag.

* ``costSame[i]`` indicates the cost of _preserving_ the parent-child
  relationship between *b* and *p* (i.e., after rerooting, *p* is
  still the parent bag of *b*). If *b* is the root bag of the original
  tree decomposition then ``costSame[i]`` is ignored.

* ``costReverse[i]`` indicates the cost of _reversing_ the parent-
  child relationship between *b* and *p* (i.e., after rerooting, *b*
  is now the parent bag of *p*). Again, if *b* is the root bag of the
  original tree decomposition then ``costReverse[i]`` is ignored.

* ``costRoot[i]`` is an additional cost that is incurred if and only
  if *b* becomes the new root bag. The argument *costRoot* may be
  ``None``, in which case these additional costs are all assumed to be
  zero.

It follows that, for each potential new root, there are size() costs
to aggregate: this comes from size()-1 costs from the arrays
*costSame* and/or *costReverse* (one for each connection between bags
in the underlying tree), and one cost from *costRoot*. These costs
will be aggregated by taking the _maximum_ over all individual costs.
This means that you do not need to estimate running times and/or
memory consumption accurately; instead you only need to find some
heuristic that aims to be _monotonic_ in time and/or memory.

So: in essence then, this routine minimises the maximum cost. In the
case of a tie, it then minimises multiplicity; that is, it minimises
the _number_ of times that this maximum cost occurs over the
individual size() costs that are being aggregated.

Note that the *costSame* and *costReverse* arrays are technically
defined as a cost per arc, not a cost per bag. If you wish to estimate
a cost per bag, the typical way of doing this would be:

* ``costSame[i]`` estimates the processing cost at bag *i* if its
  relationship with its parent is preserved;

* ``costReverse[i]`` estimates the processing cost at the original
  _parent_ of bag *i* if its relationship with bag *i* is reversed
  (i.e., it becomes a child of bag *i*);

* ``costRoot[i]`` estimates the processing cost at bag *i* if bag *i*
  becomes the root.

This scheme ensures that, for any possible rerooting, each bag is
costed exactly once amongst the three arrays.

After rerooting, all pointers to bags will remain valid, and the
contents of the bags will not change. However:

* the bags will be reindexed, to reflect the changes in the leaves-to-
  root, left-to-right ordering;

* all bag types will be reset to 0, since in general rerooting may
  break whatever properties the bag types and subtypes represent.

If the given bag is already the root bag, then this routine does
nothing (and in particular, types and subtypes are preserved).

Python:
    The *costSame* and *costReverse* arrays, as well as *costRoot* if
    it is given, should be passed as Python lists of real numbers.

Template parameter ``T``:
    the type being used to estimate costs. It must be possible to
    assign 0 to a variable of type *T* using both constructors and the
    assignment operator.

Parameter ``costSame``:
    An array of size() elements giving an estimated cost of preserving
    each child-parent connection;

Parameter ``costReverse``:
    An array of size() elements giving an estimated cost of reversing
    each child-parent connection;

Parameter ``costRoot``:
    An array of size() elements giving an additional estimated cost
    for each bag being the new root. This array may be ``None``.)doc";

// Docstring regina::python::doc::TreeDecomposition_::root
static const char *root =
R"doc(Returns the bag at the root of the underlying tree.

Returns:
    the root bag, or ``None`` if there are no bags (which means the
    underlying graph *G* is empty).)doc";

// Docstring regina::python::doc::TreeDecomposition_::size
static const char *size =
R"doc(Returns the number of bags in this tree decomposition.

Returns:
    the number of bags.)doc";

// Docstring regina::python::doc::TreeDecomposition_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given tree decomposition.

Parameter ``other``:
    the tree decomposition whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::TreeDecomposition_::width
static const char *width =
R"doc(Returns the width of this tree decomposition. This is one less than
the size of the largest bag.

Returns:
    the width of this tree decomposition.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

