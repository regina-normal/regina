/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::ModelLinkGraph
static const char *ModelLinkGraph =
R"doc(Represents an undirected 4-valent planar graph with a specific planar
embedding. This can be used as the model graph for a knot or link
diagram, where each node of the graph becomes a crossing.

Current this class does not support circular graph components (which,
in a link diagram, would correspond to zero-crossing unknot components
of the link).

This class is primarily designed for *enumerating* knots and links. If
you wish to study the underlying graph of an existing link, you do not
need to create a ModelLinkGraph - instead the Link class already gives
you direct access to the graph structure. In particular, if you
include link/graph.h, you can use a Link directly as a directed graph
type with the Boost Graph Library.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc
static const char *ModelLinkGraphArc =
R"doc(A reference to an outgoing edge from a node of a model graph for a
knot or link.

Edges of model graphs are not directed, and so the same edge will
appear twice as a ModelLinkGraphArc (once from each of its endpoints).

This class is a simple wrapper that stores (i) a pointer to the
relevant node of the graph; and (ii) an integer to denote which of the
four outgoing arcs we are using from that node. Recall that the four
outgoing arcs for each node are indexed in clockwise order.

A "null arc" is one whose node is the null pointer.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells
static const char *ModelLinkGraphCells =
R"doc(Describes the cellular decomposition of the sphere that is induced by
a given planar 4-valent graph.

The graph is represented by an object of type ModelLinkGraph, which
also encodes a specific planar embedding of the graph. The nodes and
arcs of this graph then form the vertices and edges of a cellular
decomposition; the main purpose of this class is to deduce and
describe the resulting 2-cells.

At present, this class insists that each 2-cell is a topological disc.
As a consequence, this class cannot work with empty or disconnected
graphs.

Cellular decompositions do not support value semantics: they cannot be
copied, swapped, or manually constructed. Instead they are computed
properties of model graphs, and are only accessible via const
reference through the member function ModelLinkGraph::cells().)doc";

// Docstring regina::python::doc::ModelLinkGraphNode
static const char *ModelLinkGraphNode =
R"doc(Represents a single node in a model graph for a knot or link.

If a graph has *n* nodes, then these are numbered 0,...,*n*-1. The
number assigned to this node can be accessed by calling index(). Note
that nodes may be reindexed when other nodes are added or removed - if
you wish to track a particular node through such operations then you
should use a pointer to the relevant ModelLinkGraphNode instead.

Graph nodes do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the ModelLinkGraph to which they belong.)doc";

namespace ModelLinkGraphArc_ {

// Docstring regina::python::doc::ModelLinkGraphArc_::__bool
static const char *__bool =
R"doc(Tests whether this is a non-null arc.

Returns:
    ``True`` if this is not a null arc (i.e., node() does not return a
    null pointer), or ``False`` if this is a null arc.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::__copy
static const char *__copy = R"doc(Default copy constructor.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::__dec
static const char *__dec =
R"doc(Changes to the previous outgoing link arc from the same node.

This effectively rotates the arc in an anticlockwise direction around
the node. In particular, it decrements the value returned by arc(),
modulo 4.

This is a postdecrement operator: the object will be changed, but a
copy of the original arc will be returned.

Precondition:
    This is not a null arc, i.e., node() does not return ``null``.

Python:
    This routine is available under the name dec().

Returns:
    a copy of this object before the change took place.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::__default
static const char *__default =
R"doc(Initialises this to a null arc.

The pointer returned by node() will be ``null``, and the integer
returned by arc() will be 0.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::__eq
static const char *__eq =
R"doc(Tests whether this and the given arc reference are identical.

Two references are identical if and only if they return the same
values for both node() and arc().

.. warning::
    If you create a null arc by calling ModelLinkGraphArc(``null``,
    *i*) for some non-zero *i*, then this will *not* be considered
    equal to the null arc created by calling ModelLinkGraphArc(),
    since the latter is equivalent to calling
    ModelLinkGraphArc(``null``, 0).

``True`` if and only if this and *rhs* are identical.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::__inc
static const char *__inc =
R"doc(Changes to the next outgoing link arc from the same node.

This effectively rotates the arc in a clockwise direction around the
node. In particular, it increments the value returned by arc(), modulo
4.

This is a postincrement operator: the object will be changed, but a
copy of the original arc will be returned.

Precondition:
    This is not a null arc, i.e., node() does not return ``null``.

Python:
    This routine is available under the name inc().

Returns:
    a copy of this object before the change took place.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::__init
static const char *__init =
R"doc(Initialises this to the given arc exiting the given node of a model
graph.

Recall that the four arcs exiting a node are numbered 0,1,2,3 in a
clockwise order around the node.

The given node may be ``null``, in which case this will become a null
arc. If you are creating a null arc, then it is highly recommended
that you pass *arc* as 0 also, so that comparison tests treat this
null reference as equal to a null reference created by the zero-
argument constructor.

Parameter ``node``:
    the node of the model graph that this arc exits.

Parameter ``arc``:
    an integer in the range 0 to 3 inclusive, indicating which of the
    four arcs exiting *node* this represents.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::__ne
static const char *__ne =
R"doc(Tests whether this and the given arc reference are not identical.

Two references are identical if and only if they return the same
values for both node() and arc().

.. warning::
    If you create a null arc by calling ModelLinkGraphArc(``null``,
    *i*) for some non-zero *i*, then this will *not* be considered
    equal to the null arc created by calling ModelLinkGraphArc(),
    since the latter is equivalent to calling
    ModelLinkGraphArc(``null``, 0).

``True`` if and only if this and *rhs* are not identical.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::arc
static const char *arc =
R"doc(Indicates which arc this is amongst the four arcs exiting the
underlying node of the model graph.

For each node of a model graph, the four arcs exiting that node are
numbered 0,1,2,3 in a clockwise order.

Returns:
    an integer between 0 and 3 inclusive indicating one of the four
    arcs exiting node().)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::next
static const char *next =
R"doc(Returns the next arc after this when walking through the graph as
though it were a link, in a direction away from the current node.

This routine will move to the other endpoint of the graph edge
described by this arc, and will then return the *opposite* arc at the
resulting node (i.e., not just pointing backwards along the same
edge).

For any arc *a*, calling ``a.next()`` is equivalent to calling
``a.traverse().opposite()``.

Precondition:
    This is not a null arc, i.e., node() does not return ``null``.

Returns:
    the next arc after this when walking through the graph as though
    it were a link.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::node
static const char *node =
R"doc(The node of the model graph from which this arc exits.

Returns:
    the corresponding node, or ``null`` if this is a null arc.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::opposite
static const char *opposite =
R"doc(Returns the arc that exits the same node as this, but from the
opposite side.

Recall that the four arcs exiting each node are numbered in clockwise
order. The return value will therefore have the same node() as this,
but its arc() value will be two more than this (modulo 4).

Note that, for any arc *a*, ``a.opposite().opposite()`` is identical
to *a*.

Precondition:
    This is not a null arc, i.e., node() does not return ``null``.

Returns:
    the opposite arc exiting the same node.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::prev
static const char *prev =
R"doc(Returns the previous arc before this when walking through the graph as
though it were a link, in a direction away from the* current node.

This routine will jump to the opposite arc at the current node, and
then move to the other endpoint of the graph edge described by that
opposite arc.

For any arc *a*, calling ``a.prev()`` is equivalent to calling
``a.opposite().traverse()``.

Precondition:
    This is not a null arc, i.e., node() does not return ``null``.

Returns:
    the previous arc before this when walking through the graph as
    though it were a link.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::traverse
static const char *traverse =
R"doc(Returns the same edge of the model graph, but seen from the other
endpoint.

Recall that each undirected edge of a model graph has two
corresponding ModelLinkGraphArc objects, one for each of its
endpoints. If this object represents one of these arcs for some
underlying edge of the graph, then then return value represents the
other.

Note that, for any arc *a*, ``a.traverse().traverse()`` is identical
to *a*.

Precondition:
    This is not a null arc, i.e., node() does not return ``null``.

Returns:
    the arc at the other end of the underlying edge of the model
    graph.)doc";

}

namespace ModelLinkGraphCells_ {

// Docstring regina::python::doc::ModelLinkGraphCells_::__eq
static const char *__eq =
R"doc(Determines if this and the given cellular decomposition are
combinatorially identical.

Here "identical" means that both decompositions have the same number
of cells, these cells are presented in the same order, and their
boundaries enter and exit the same numbered arcs of the same numbered
nodes, using the same directions of traversal and the same starting
points on each cell boundary.

Parameter ``other``:
    the cellular decomposition to compare with this.

Returns:
    ``True`` if and only if the two cellular decompositions are
    combinatorially identical.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::__ne
static const char *__ne =
R"doc(Determines if this and the given cellular decomposition are not
combinatorially identical.

Here "identical" means that both decompositions have the same number
of cells, these cells are presented in the same order, and their
boundaries enter and exit the same numbered arcs of the same numbered
nodes, using the same directions of traversal and the same starting
points on each cell boundary.

Parameter ``other``:
    the cellular decomposition to compare with this.

Returns:
    ``True`` if and only if the two cellular decompositions are not
    combinatorially identical.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::arc
static const char *arc =
R"doc(Returns the given arc along the boundary of the given 2-cell.

For each cell, the arcs along the boundary are given in order as you
walk anticlockwise around the cell (so the cell is on the left of each
arc as you walk around the cell boundary).

Each arc is described in the form of an *outgoing* arc from some node
of the underlying graph (so if the return ModelLinkGraphArc is *a*
then this describes an outgoing arc from a.node()). It follows that,
if the underlying graph has *n* nodes, then each of the 4*n* possible
ModelLinkGraphArc values appears exactly once as ``arc(cell, which)``
for some integers *cell* and *which*.

Precondition:
    The underlying ModelLinkGraph is non-empty, connected, and
    describes a planar graph embedding. Note that connectivity is
    already required by the class constructor, and you can test the
    remaining conditions by calling isValid().

Parameter ``cell``:
    indicates which cell to query; this must be between 0 and
    countCells()-1 inclusive.

Parameter ``which``:
    indicates which arc along the boundary of the corresponding cell
    to return; this must be between 0 and ``size(cell)-1`` inclusive.

Returns:
    the requested arc on the boundary of the given 2-cell.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::arcs
static const char *arcs =
R"doc(Returns an object that allows iteration through and random access to
all arcs along the boundary of the given 2-cell.

Suppose that the *i*th cell is a *k*-gon. Then this object gives
access to the *k* arcs along the boundary of the *i*th cell in the
same order as described by arc(); that is, walking anticlockwise
around the cell boundary with the cell to the left of each arc.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. The elements of the list will be read-only
objects of type ModelLinkGraphArc, and so your code might look like:

```
for (const ModelLinkGraphArc& a : cells.arcs(cell)) { ... }
```

Using ``arcs(cell)`` is equivalent to iterating over the iterator
range (``begin(cell)``, ``end(cell)``). Using arcs() generates a tiny
amount of extra overhead, but you may also find it more readable.

Parameter ``cell``:
    indicates which cell to query; this must be between 0 and
    countCells()-1 inclusive.

Returns:
    access to the list of all arcs along the boundary of the given
    cell.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::cell
static const char *cell =
R"doc(Returns the 2-cell that lies to the left of the given arc.

Specifically, this function returns the number of the cell that lies
to the left of the given arc as you walk along it away from
``arc.node()``.

For any arc *a*, calling ``arc(cell(a), cellPos(a))`` will return the
same arc *a* again.

Precondition:
    The underlying ModelLinkGraph is non-empty, connected, and
    describes a planar graph embedding. Note that connectivity is
    already required by the class constructor, and you can test the
    remaining conditions by calling isValid().

Parameter ``arc``:
    the given arc of the underlying graph.

Returns:
    the number of the cell that lies to the left of the given arc;
    this will be an integer between 0 and ``countCells()-1``
    inclusive.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::cellPos
static const char *cellPos =
R"doc(Returns where the given arc appears along the boundary of the 2-cell
to its left.

Consider the cell *c* to the left of the given arc as you follow the
arc away from ``arc.node()``. The routine arc() can be used to
enumerate the sequence of arcs along the boundary of this cell *c*, in
order as you walk anticlockwise around the cell boundary. The purpose
of this routine is to identify *where* in this sequence the given arc
occurs.

For any arc *a*, calling ``arc(cell(a), cellPos(a))`` will return the
same arc *a* again.

Precondition:
    The underlying ModelLinkGraph is non-empty, connected, and
    describes a planar graph embedding. Note that connectivity is
    already required by the class constructor, and you can test the
    remaining conditions by calling isValid().

Parameter ``arc``:
    the given arc of the underlying graph.

Returns:
    the position of the given arc on the boundary of the cell to its
    left; this will be an integer between 0 and ``size(cell(arc))-1``
    inclusive.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::countCells
static const char *countCells =
R"doc(Returns the number of 2-cells in this cellular decomposition.

If isValid() returns ``False`` (i.e., the underlying ModelLinkGraph is
either empty or does not describe a planar embedding), then this
routine will return 0 instead. Note that this routine *cannot* be used
to test for connectivity, which is a non-negotiable precondition
required by the class constructor.

Note that, if isValid() returns ``True``, then countCells() will
always return *n*+2 where *n* is the number of nodes in the underlying
graph.

Returns:
    a strictly positive number of 2-cells if isValid() returns
    ``True``, or 0 if isValid() returns ``False``.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::isValid
static const char *isValid =
R"doc(Determines whether the underlying graph is non-empty with a planar
embedding, assuming that it is already known to be connected.

As described in the class notes, this class can only work with non-
empty connected graphs where the corresponding ModelLinkGraph object
also describes a planar embedding.

The constructor for this class requires you to pass a graph that is
already known to be connected. However, *assuming* the graph is
connected, the constructor then tests for the remaining conditions.
This routine returns the results of these tests: if the underlying
graph is empty or does not describe a planar embedding, then this
routine will return ``False``.

This routine is constant time, since the necessary work will have
already been completed by the class constructor.

.. warning::
    Most of the routines in this class require isValid() to return
    ``True``. Essentially, if isValid() returns ``False``, you should
    not attempt to query the details of the cell decomposition. See
    the preconditions on individual routines for further details.

Returns:
    ``True`` if and only if the underlying ModelLinkGraph describes a
    planar embedding of a non-empty graph.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::size
static const char *size =
R"doc(Returns the number of arcs aloung the boundary of the given 2-cell. If
the given cell is a *k*-gon, then this routine returns the integer
*k*.

Precondition:
    The underlying ModelLinkGraph is non-empty, connected, and
    describes a planar graph embedding. Note that connectivity is
    already required by the class constructor, and you can test the
    remaining conditions by calling isValid().

Parameter ``cell``:
    indicates which cell to query; this must be between 0 and
    countCells()-1 inclusive.

Returns:
    the size of the correpsonding 2-cell.)doc";

}

namespace ModelLinkGraphNode_ {

// Docstring regina::python::doc::ModelLinkGraphNode_::adj
static const char *adj =
R"doc(Returns the arc at the other end of the given graph edge that exits
this node.

Let *e* be the undirected edge of the underlying model graph that
corresponds to the given outgoing arc from this node. Recall that
there are two ModelLinkGraphArc objects corresponding to *e*, one for
each of its endpoints. One of these will be
ModelLinkGraphArc(``this``, *which*); this routine returns the *other*
object, which is the ModelLinkGraphArc describing the other endpoint
of *e*.

Note that for a node *n*, calling ``n.adj(i)`` is equivalent to
calling ``n.arc(i).traverse()``.

Parameter ``which``:
    an integer in the range 0 to 3 inclusive, indicating which of the
    four arcs exiting this node we should examine.

Returns:
    a reference to the other end of the same undirected edge of the
    underlying model graph.)doc";

// Docstring regina::python::doc::ModelLinkGraphNode_::arc
static const char *arc =
R"doc(Returns a reference to one of the four arcs of the graph that exit
this node. This is equivalent to directly constructing
ModelLinkGraphArc(``this``, *which*).

The four arcs exiting this node are numbered 0,1,2,3 in a clockwise
order around the node.

Parameter ``which``:
    an integer in the range 0 to 3 inclusive, indicating which of the
    four arcs exiting this node we should return.

Returns:
    a reference to the corresponding arc exiting this node.)doc";

// Docstring regina::python::doc::ModelLinkGraphNode_::index
static const char *index =
R"doc(Returns the index of this node within the overall graph. If the graph
contains *n* nodes, then the index will be a number between 0 and
*n*-1 inclusive.

.. warning::
    The index of this node might change if other nodes are added or
    removed.

Returns:
    the index of this node.)doc";

}

namespace ModelLinkGraph_ {

// Docstring regina::python::doc::ModelLinkGraph_::__copy
static const char *__copy =
R"doc(Constructs a new copy of the given graph.

Parameter ``copy``:
    the graph to copy.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::__default
static const char *__default = R"doc(Constructs an empty graph.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::__eq
static const char *__eq =
R"doc(Determines if this graph is combinatorially identical to the given
graph.

Here "identical" means that both graphs have the same number of nodes,
and in both graphs the same pairs of outgoing arcs of numbered nodes
are connected by edges.

Parameter ``other``:
    the graph to compare with this.

Returns:
    ``True`` if and only if the two graphs are combinatorially
    identical.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::__init
static const char *__init =
R"doc(Constructs the graph that models the given link.

Any zero-component unknot components of the link will be ignored.

The nodes of this graph will be numbered in the same way as the
crossings of *link*. For each node, arc 0 will represent the outgoing
lower strand of the corresponding crossing.

Using this constructor is identical to calling Link::graph().

Parameter ``link``:
    the link that this new graph will model.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::__ne
static const char *__ne =
R"doc(Determines if this graph is not combinatorially identical to the given
graph.

Here "identical" means that both graphs have the same number of nodes,
and in both graphs the same pairs of outgoing arcs of numbered nodes
are connected by edges.

Parameter ``other``:
    the graph to compare with this.

Returns:
    ``True`` if and only if the two graphs are not combinatorially
    identical.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::canonicalPlantri
static const char *canonicalPlantri =
R"doc(Outputs a text representation of this graph in a variant of the
*plantri* ASCII format, using a canonical relabelling of nodes and
arcs, and with optional compression.

This routine is similar to plantri(), but with two significant
differences:

* This routine does not preserve the labelling of nodes and the order
  of arcs around each node. Instead it reorders the nodes and arcs so
  that any two relabellings of the "same" planar embedding will
  produce the same canonicalPlantri() output. By "same" we allow for
  relabelling and isotopy (sliding the graph around the sphere); if
  the argument *useReflection* is ``True`` then we allow for
  reflection also.

* If the argument *tight* is ``True``, then this routine uses an
  abbreviated output format. The resulting compression is only trivial
  (it reduces the length by roughly 40%), but the resulting string is
  still human-parseable (though with a little more effort required).
  This compression will simply remove the commas, and for each node it
  will suppress the destination of the first arc (since this can be
  deduced from the canonical labelling).

Regardless of whether *tight* is ``True`` or ``False``, the resulting
string can be parsed by fromPlantri() to reconstruct the original
graph. Note however that, due to the canonical labelling, the
resulting graph might be a relabelling of the original (and might even
be a reflection of the original, if *useReflection* was passed as
``True``).

See plantri() for further details on the ASCII format itself,
including the ways in which Regina's implementation of this format
differs from *plantri*'s for graphs with more than 26 nodes.

The running time for this routine is quadratic in the size of the
graph.

Precondition:
    This graph is connected.

Precondition:
    This graph has between 1 and 52 nodes inclusive.

Precondition:
    The dual to this graph is a *simple* quadrangulation of the
    sphere. In particular, the dual must not have any parallel edges.
    Note that any graph that fails this condition will the model graph
    for a link diagram that is an "obvious" connected sum.

Exception ``FailedPrecondition``:
    This graph has more than 52 nodes.

Parameter ``useReflection``:
    ``True`` if a graph and its reflection should be considered the
    same (i.e., produce the same canonical output), or ``False`` if
    they should be considered different. Of course, if a graph is
    symmetric under reflection then the graph and its reflection will
    produce the same canonical output regardless of this parameter.

Parameter ``tight``:
    ``False`` if the usual *plantri* ASCII format should be used (as
    described by plantri() and fromPlantri()), or ``True`` if the
    abbreviated format should be used as described above.

Returns:
    an optionally compressed *plantri* ASCII representation of this
    graph.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::cells
static const char *cells =
R"doc(Returns details of the cellular decomposition of the sphere that is
induced by this graph.

This cellular decomposition will only be computed on demand. This
means that the first call to this function will take linear time (as
the decomposition is computed), but subsequent calls will be constant
time (since the decomposition is cached).

Precondition:
    This graph is connected.

Returns:
    the induced cellular decomposition of the sphere.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::findFlype
static const char *findFlype =
R"doc(Identifies the smallest flype that can be performed on this graph from
the given starting location.

Here we use the same notation as in the three-argument flype()
function, where you perform a flype by passing three arcs *from*,
*left* and *right*. Read the flype() documentation now if you have not
done so already; this includes a full description of the flype
operation as well as diagrams with the arcs *from*, *left* and *right*
clearly marked.

The given arc *from* identifies the node to the left of the flype
disc. The aim of this routine is to identify two suitable arcs *left*
and *right* that exit through the right of the flype disc. Together,
these three arcs uniquely identify the entire flype disc, and
therefore prescribe the operation precisely.

Here, by "suitable arcs", we mean a pair of arcs (*left*, *right*) for
which the three arcs (*from*, *left*, *right*) together satisfy the
preconditions for the flype() routine.

There are several possible outcomes:

* It is possible that there are *no* suitable arcs *left* and *right*.
  In this case, this routine returns a pair of null arcs.

* It is possible that there is exactly one pair of suitable arcs
  (*left*, *right*). In this case, this pair will be returned.

* It is possible that there are *many* pairs of suitable arcs. In this
  case, it can be shown that the suitable pairs have an ordering
  *P_1*, ..., *P_k* in which the flype disc for *P_i* is completely
  contained within the flype disc for *P_j* whenever *i* < *j*. In
  this case, this routine returns the *smallest* pair *P_1*; that is,
  the pair (*left*, *right*) that gives the smallest possible flype
  disc.

It should be noted that choosing only the smallest flype is not a
serious restriction: assuming the graph does not model a composition
of non-trivial knot diagrams, *any* suitable flype can be expressed as
a composition of minimal flypes in this sense.

Precondition:
    This graph is connected.

Parameter ``from``:
    the arc that indicates where the flype disc should begin. This is
    the arc labelled *from* in the diagrams for the three-argument
    flype() function: it is the lower of the two arcs that enter the
    flype disc from the node *X* to the left of the disc. This should
    be presented as an arc of the node *X*.

Returns:
    the pair (*left*, *right*) representing the smallest suitable
    flype beginning at *from*, or a pair of null arcs if there are no
    suitable pairs (*left*, *right*).)doc";

// Docstring regina::python::doc::ModelLinkGraph_::flype
static const char *flype =
R"doc(Performs a flype on this graph at the given location.

A *flype* is an operation on a disc in the plane. The boundary of the
disc must cut through four arcs of the graph (and otherwise must not
meet the graph at all), as indicated in the diagram below. Moreover,
the two arcs that exit the disc on the left must meet at a common node
just outside the disc. (The punctuation symbols drawn inside the disc
are just to help illustrate how the transformation works.)

```
         ______                       ______
        /      \                     /      \
__   __| ##  ** |_______     _______| ::  <> |__   __
  \ /  |        |                   |        |  \ /
   X   |  Disc  |        ==>        |        |   X
__/ \__|        |_______     _______|        |__/ \__
       | ::  <> |                   | ##  ** |
        \______/                     \______/
```

The operation involves:

* reflecting this disc in a horizontal axis (so the two arcs on the
  left switch places, and the two arcs on the right switch places);

* removing the node outside the disc on the left, where the two arcs
  meet;

* introducing a new node on the right instead, where the two arcs on
  the right will now meet.

The equivalent operation on a knot diagram involves twisting the
entire region inside the disc about a horizontal axis, in a way that
undoes the crossing on the left but introduces a new crossing on the
right instead.

You will need to pass arguments to indicate where the flype should
take place. For this, we will label some of the features of the
initial diagram (before the move takes place): see the diagram below.
Here the labels *from*, *left* and *right* all refer to arcs. The
labels *A*, *B*, *C* and *D* all refer to dual 2-cells in the plane;
these are not passed as arguments, but they do appear in the list of
preconditions for this routine.

```
                 ______
Cell A          /      \
__   __________|        |_________ left
  \ /          |        |
   X   Cell B  |        |  Cell D
__/ \__________|        |_________ right
         from  |        |
Cell C          \______/
```

The arc *from* must be given as an arc of the node *outside* the disc
(i.e., the node to the left of cell *B*). The arcs *left* and *right*
must be given as arcs of their respective nodes *inside* the disc.

Precondition:
    This graph is connected.

Precondition:
    The arcs *from*, *left* and *right* are laid out as in the diagram
    above. In particular: *from* and *right* have the same cell to
    their right (cell *C*); *left* and the arc to the left of *from*
    have the same cell to their left (cell *A*); and *left* and
    *right* have the same cell between them (cell *D*).

Precondition:
    Neither of the arcs *left* or *right*, when followed in the
    direction away from the disc, end back at the node on the left of
    the diagram. That is, neither ``left.traverse().node()`` nor
    ``right.traverse().node()`` is equal to ``from.node()``. (If this
    fails, then either the flype simply reflects the entire graph, or
    else the graph models a composition of two non-trivial knot
    diagrams.)

Precondition:
    Cells *A* and *C* are distinct (that is, the node on the left of
    the diagram is not a cut-vertex of the graph).

Precondition:
    Cells *B* and *D* are distinct (that is, the disc actually
    contains one or more nodes, and the graph does not model a
    composition of two non-trivial knot diagrams).

Exception ``InvalidArgument``:
    One or more of the preconditions above fails to hold. Be warned
    that the connectivity precondition will not be checked - this is
    the user's responsibility - but all other preconditions *will* be
    checked, and an exception will be thrown if any of them fails.

Parameter ``from``:
    the first arc that indicates where the flype should take place, as
    labelled on the diagram above. This should be presented as an arc
    of the node outside the disc, to the left.

Parameter ``left``:
    the second arc that indicates where the flype should take place,
    as labelled on the diagram above. This should be presented as an
    arc of the node that it meets inside the disc.

Parameter ``right``:
    the third arc that indicates where the flype should take place, as
    labelled on the diagram above. This should be presented as an arc
    of the node that it meets inside the disc.

Returns:
    the graph obtained by performing the flype.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::flype_2
static const char *flype_2 =
R"doc(Performs the smallest possible flype on this graph from the given
starting location.

This is a convenience routine that simply calls findFlype() to
identify the smallest possible flype from the given starting location,
and then calls the three-argument flype() to actually perform it. If
there is no possible flype from the given starting location then this
routine throws an exception.

See the documentation for the three-argument flype() for further
details on the flype operation, and see findFlype() for a discussion
on what is meant by "smallest possible".

Precondition:
    This graph is connected.

Exception ``InvalidArgument``:
    There is no suitable flype on this graph from the given starting
    location (that is, findFlype() returns a pair of null arcs).

Parameter ``from``:
    the arc that indicates where the flype disc should begin. This is
    the arc labelled *from* in the diagrams for the three-argument
    flype() function: it is the lower of the two arcs that enter the
    flype disc from the node *X* to the left of the disc. This should
    be presented as an arc of the node *X*.

Returns:
    the graph obtained by performing the flype.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::fromPlantri
static const char *fromPlantri =
R"doc(Builds a graph from a line of *plantri* output, using Regina's variant
of the *plantri* ASCII format.

The software *plantri*, by Gunnar Brinkmann and Brendan McKay, can be
used to enumerate 4-valent planar graphs (amongst many other things).
This routine converts a piece of output from *plantri*, or the
encoding of a graph using Regina's own plantri() or canonicalPlantri()
functions, into a ModelLinkGraph object that Regina can work with
directly.

If you are converting output from *plantri*, this output must be in
ASCII format, and must be the dual graph of a simple quadrangulation
of the sphere. The corresponding flags that must be passed to
*plantri* to obtain such output are ``-adq`` (although you will may
wish to pass additional flags to expand or restrict the classes of
graphs that *plantri* builds).

When run with these flags, *plantri* produces output in the following
form:

```
6 bbcd,adca,abee,affb,cffc,deed
6 bcdd,aeec,abfd,acfa,bffb,ceed
6 bcde,affc,abfd,acee,addf,becb
```

Each line consists of an integer (the number of nodes in the graph),
followed by a comma-separated sequence of alphabetical strings that
encode the edges leaving each node.

This function *only* takes the comma-separated sequence of
alphabetical strings. So, for example, to construct the graph
corresponding to the second line of output above, you could call:

```
fromPlantri("bcdd,aeec,abfd,acfa,bffb,ceed");
```

Regina uses its own variant of *plantri*'s output format, which is
identical for smaller graphs but which differs from *plantri*'s own
output format for larger graphs. In particular:

* For graphs with ≤ 26 nodes, Regina and *plantri* use identical
  formats. Here Regina can happily recognise the output from *plantri*
  as described above, as well as the output from Regina's own
  plantri() and canonicalPlantri() functions.

* For graphs with 27-52 nodes, Regina's and *plantri*'s formats
  differ: whereas *plantri* uses punctuation for higher-index nodes,
  Regina uses the upper-case letters ``A``,...,``Z``. For these larger
  graphs, Regina can only recognise Regina's own plantri() and
  canonicalPlantri() output, not *plantri*'s punctuation-based
  encodings.

* For graphs with 53 nodes or more, Regina cannot encode or decode
  such graphs using *plantri* format at all.

Even for graphs with at most 26 nodes, the given string does not
*need* to be come from the program *plantri* itself. Whereas *plantri*
always outputs graphs with a particular canonical labelling, this
function can accept an arbitrary ordering of nodes and arcs - in
particular, it can accept the string ``g.plantri()`` for any graph *g*
that meets the preconditions below. Nevertheless, the graph must still
meet these preconditions, since otherwise the *plantri* format might
not be enough to uniquely reconstruct the graph and its planar
embedding.

This routine can also interpret the "tight" format that is optionally
produced by the member function canonicalPlantri() (even though such
output would certainly *not* be produced by the program *plantri*).

.. warning::
    While this routine does some basic error checking on the input,
    these checks are not exhaustive. In particular, it does *not* test
    for planarity of the graph. (Of course *plantri* does not output
    non-planar graphs, but a user could still construct one by hand
    and passes it to this routine, in which case the resulting
    behaviour is undefined.)

Precondition:
    The graph being described is connected.

Precondition:
    The graph being described is dual to a *simple* quadrangulation of
    the sphere. In particular, the dual must not have any parallel
    edges. Note that any graph that fails this condition will the
    model graph for a link diagram that is an "obvious" connected sum.

Exception ``InvalidArgument``:
    The input was not a valid representation of a graph using the
    *plantri* output format. As noted above, the checks performed here
    are not exhaustive.

Parameter ``plantri``:
    a string containing the comma-separated sequence of alphabetical
    strings in *plantri* format, as described above.

Returns:
    the resulting graph.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::generateMinimalLinks
static const char *generateMinimalLinks =
R"doc(Exhaustively generates potentially-minimal knot diagrams that are
modelled by this graph.

Here *potentially*-minimal means there are no "obvious" simplification
moves (such as a simplifying type II Reidemeister move, for example).
The list of "obvious" moves considered here is subject to change in
future versions of Regina.

By *exhaustive*, we mean:

* Every minimal knot diagram modelled by this graph will be generated
  by this routine, up to reflection and/or reversal (as noted below).

* If a knot diagram is non-minimal and modelled by this graph, it
  *might* still be generated by this routine.

In other words, this routine will generate all minimal knot diagrams
modelled by this graph, but there is no promise that all of the
diagrams generated are minimal.

Labelled diagrams are only generated once up to reflection and/or
reversal, in that this routine will fix the orientation of the knot
(always following arc 0 away from node 0 of the graph) and the sign of
the crossing at node 0 (always positive).

In each knot diagram that is generated, crossing *k* will always
correspond to node *k* of this graph.

For each knot diagram that is generated, this routine will call
*action* (which must be a function or some other callable object).

* The first argument passed to *action* will be the knot diagram that
  was generated (of type Link). This will be passed as an rvalue; a
  typical action could (for example) take it by const reference and
  query it, or take it by value and modify it, or take it by rvalue
  reference and move it into more permanent storage.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* *action* must return ``void``.

Precondition:
    This graph does not model multiple-component links (i.e., either
    it models single-component knots, or it is the empty graph).

Exception ``FailedPrecondition``:
    This graph models links with more than one component.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.

Python:
    This function is available in Python, and the *action* argument
    may be a pure Python function. However, its form is more
    restricted: the argument *args* is removed, so you simply call it
    as generateMinimalLinks(action). Moreover, *action* must take
    exactly one argument (the knot diagram).

Parameter ``action``:
    a function (or other callable object) to call for each knot
    diagram that is generated.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial knot diagram argument.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given graphs.

This global routine simply calls ModelLinkGraph::swap(); it is
provided so that ModelLinkGraph meets the C++ Swappable requirements.

See ModelLinkGraph::swap() for more details.

Parameter ``lhs``:
    the graph whose contents should be swapped with *rhs*.

Parameter ``rhs``:
    the graph whose contents should be swapped with *lhs*.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::node
static const char *node =
R"doc(Returns the node at the given index within this graph.

For a graph with *n* nodes, the nodes are numbered from 0 to *n*-1
inclusive.

.. warning::
    If some nodes are added or removed then the indices of other nodes
    might change. If you wish to track a particular node through such
    operations then you should use the pointer to the relevant
    ModelLinkGraphNode object instead.

Parameter ``index``:
    the index of the requested node. This must be between 0 and
    size()-1 inclusive.

Returns:
    the node at the given index.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::nodes
static const char *nodes =
R"doc(Returns an object that allows iteration through and random access to
all nodes in this graph.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. Note that the elements of the list will be
pointers, so your code might look like:

```
for (ModelLinkGraphNode* n : graph.nodes()) { ... }
```

The object that is returned will remain up-to-date and valid for as
long as the graph exists: even if nodes are added and/or removed, it
will always reflect the nodes that are currently in the graph.
Nevertheless, it is recommended to treat this object as temporary
only, and to call nodes() again each time you need it.

Returns:
    access to the list of all nodes.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::plantri
static const char *plantri =
R"doc(Outputs this graph in a variant of the ASCII text format used by
*plantri*.

The software *plantri*, by Gunnar Brinkmann and Brendan McKay, can be
used to enumerate 4-valent planar graphs (amongst many other things).
This routine outputs this graph in a format that mimics *plantri*'s
own dual ASCII format (i.e., the format that *plantri* outputs when
run with the flags ``-adq``).

Specifically, the output will be a comma-separated sequence of
alphabetical strings. The *i*th such string will consist of four
letters, encoding the endpoints of the four edges in clockwise order
that leave node *i*. The lower-case letters ``a``,``b``,...,``z``
represent nodes 0,1,...,25 respectively, and the upper-case letters
``A``,``B``,...,``Z`` represent nodes 26,27,...,51 respectively. An
example of such a string is:

```
bcdd,aeec,abfd,acfa,bffb,ceed
```

For graphs with at most 26 nodes, this is identical to *plantri*'s own
dual ASCII format. For larger graphs, this format differs: *plantri*
uses punctuation to represent higher-index nodes, whereas Regina uses
upper-case letters.

This routine is an inverse to fromPlantri(): for any graph *g* that
satisfies the preconditions below, ``fromPlantri(g.plantri())`` is
identical to *g*. Likewise, for any string *s* that satisfies the
preconditions for fromPlantri(), calling ``fromPlantri(s).plantri()``
will recover the original string *s*.

It is important to note the preconditions below: in particular, that
this graph must be dual to a *simple* quadrangulation of the sphere.
This is because the planar embeddings for more general graphs (i.e.,
the duals of non-simple quadrangulations) cannot always be uniquely
reconstructed from their *plantri* output.

.. note::
    The output of this function might not correspond to any possible
    output from the program *plantri* itself, even if only lower-case
    letters are used. This is because *plantri* only outputs graphs
    with a certain canonical labelling. In contrast, plantri() can be
    called on any graph that satisfies the preconditions below, and it
    will preserve the labels of the nodes and the order of the arcs
    around each node.

Precondition:
    This graph is connected.

Precondition:
    This graph has between 1 and 52 nodes inclusive.

Precondition:
    The dual to this graph is a *simple* quadrangulation of the
    sphere. In particular, the dual must not have any parallel edges.
    Note that any graph that fails this condition will the model graph
    for a link diagram that is an "obvious" connected sum.

Exception ``FailedPrecondition``:
    This graph has more than 52 nodes.

Returns:
    a *plantri* format ASCII representation of this graph.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::reflect
static const char *reflect =
R"doc(Converts this graph into its reflection.

This routine simply reverses (and also cycles) the order of outgoing
arcs around every node.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::size
static const char *size =
R"doc(Returns the number of nodes in this graph.

Returns:
    the number of nodes.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given graph.

All nodes that belong to this graph will be moved to *other*, and all
nodes that belong to *other* will be moved to this graph.

In particular, any ModelLinkGraphNode pointers or references and any
ModelLinkGraphArc objects will remain valid.

This routine will behave correctly if *other* is in fact this graph.

Parameter ``other``:
    the graph whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

