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
R"doc(Represents an undirected 4-valent graph with a specific embedding in
some closed orientable surface. This class only stores the graph and a
local description of the embedding (i.e., a cyclic ordering of arcs
around each node). It does not store the surface explicitly, though
the surface is implied from the embedding - if you need it you can
always access a full description of the surface by calling cells().

In particular, the surface is assumed to be the minimal genus surface
in which the graph embeds. Each connected component of the graph is
embedded in a separate connected component of the surface, and each
component of the surface is formed from a collection of discs (or
_cells_) whose boundaries follow the nodes and arcs of the graph
according to the local embedding.

Regina uses graphs like these as model graphs for classical or virtual
link diagrams, where each node of the graph becomes a classical
crossing. If the surface is a collection of 2-spheres, then the graph
is planar and models a _classical_ link diagram. If the surface has
genus, then the graph is non-planar and instead models a _virtual_
link diagram.

Currently this class does not support circular graph components
(which, in a link diagram, would correspond to zero-crossing unknot
components of the link).

For Boost users: if you wish to study the underlying graph of an
existing link, you do not need to create a ModelLinkGraph - instead
you can include link/graph.h and then use Link directly as a directed
graph type with the Boost Graph Library.

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
R"doc(Describes the cellular decomposition of a closed orientable surface
induced by a 4-valent graph embedded within it.

The graph is represented by an object of type ModelLinkGraph, which
encodes a local embedding of the graph within the surface (i.e., a
cyclic ordering of arcs around each graph node). The nodes and arcs of
this graph form the vertices and edges of the cellular decomposition,
and the 2-cells are topological discs whose boundaries follow these
nodes and arcs according to their local embeddings. The main purpose
of this class is to deduce and describe those 2-cells.

As of Regina 7.4, this class can now work with graphs that are non-
planar (resulting in a surface with positive genus), disconnected
(resulting in a surface that is likewise disconnected), and/or empty
(resulting in an empty surface).

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

// Docstring regina::python::doc::ModelLinkGraphArc_::__as_bool
static const char *__as_bool =
R"doc(Tests whether this is a non-null arc.

Returns:
    ``True`` if this is not a null arc (i.e., node() does not return a
    null pointer), or ``False`` if this is a null arc.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::__copy
static const char *__copy = R"doc(Creates a new copy of the given arc reference.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::__dec
static const char *__dec =
R"doc(Changes to the previous outgoing link arc from the same node.

This effectively rotates the arc in an anticlockwise direction around
the node. In particular, it decrements the value returned by arc(),
modulo 4.

This is a postdecrement operator: the object will be changed, but a
copy of the original arc will be returned.

Precondition:
    This is not a null arc, i.e., node() does not return ``None``.

Python:
    This routine is available under the name dec().

Returns:
    a copy of this object before the change took place.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::__default
static const char *__default =
R"doc(Initialises this to a null arc.

The pointer returned by node() will be ``None``, and the integer
returned by arc() will be 0.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::__eq
static const char *__eq =
R"doc(Tests whether this and the given arc reference are identical.

Two references are identical if and only if they return the same
values for both node() and arc().

.. warning::
    If you create a null arc by calling ModelLinkGraphArc(``None``,
    *i*) for some non-zero *i*, then this will _not_ be considered
    equal to the null arc created by calling ModelLinkGraphArc(),
    since the latter is equivalent to calling
    ModelLinkGraphArc(``None``, 0).

Returns:
    ``True`` if and only if this and the given arc reference are
    identical.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::__inc
static const char *__inc =
R"doc(Changes to the next outgoing link arc from the same node.

This effectively rotates the arc in a clockwise direction around the
node. In particular, it increments the value returned by arc(), modulo
4.

This is a postincrement operator: the object will be changed, but a
copy of the original arc will be returned.

Precondition:
    This is not a null arc, i.e., node() does not return ``None``.

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

The given node may be ``None``, in which case this will become a null
arc. If you are creating a null arc, then it is highly recommended
that you pass *arc* as 0 also, so that comparison tests treat this
null reference as equal to a null reference created by the zero-
argument constructor.

Parameter ``node``:
    the node of the model graph that this arc exits.

Parameter ``arc``:
    an integer in the range 0 to 3 inclusive, indicating which of the
    four arcs exiting *node* this represents.)doc";

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
    This is not a null arc, i.e., node() does not return ``None``.

Returns:
    the next arc after this when walking through the graph as though
    it were a link.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::node
static const char *node =
R"doc(The node of the model graph from which this arc exits.

Returns:
    the corresponding node, or ``None`` if this is a null arc.)doc";

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
    This is not a null arc, i.e., node() does not return ``None``.

Returns:
    the opposite arc exiting the same node.)doc";

// Docstring regina::python::doc::ModelLinkGraphArc_::prev
static const char *prev =
R"doc(Returns the previous arc before this when walking through the graph as
though it were a link, in a direction away from the current node.

This routine will jump to the opposite arc at the current node, and
then move to the other endpoint of the graph edge described by that
opposite arc.

For any arc *a*, calling ``a.prev()`` is equivalent to calling
``a.opposite().traverse()``.

Precondition:
    This is not a null arc, i.e., node() does not return ``None``.

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
    This is not a null arc, i.e., node() does not return ``None``.

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

// Docstring regina::python::doc::ModelLinkGraphCells_::arc
static const char *arc =
R"doc(Returns the given arc along the boundary of the given 2-cell.

For each cell, the arcs along the boundary are given in order as you
walk anticlockwise around the cell (so the cell is on the left of each
arc as you walk around the cell boundary).

Each arc is described in the form of an _outgoing_ arc from some node
of the underlying graph (so if the return ModelLinkGraphArc is *a*
then this describes an outgoing arc from a.node()). It follows that,
if the underlying graph has *n* nodes, then each of the 4*n* possible
ModelLinkGraphArc values appears exactly once as ``arc(cell, which)``
for some integers *cell* and *which*.

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
of this routine is to identify _where_ in this sequence the given arc
occurs.

For any arc *a*, calling ``arc(cell(a), cellPos(a))`` will return the
same arc *a* again.

Parameter ``arc``:
    the given arc of the underlying graph.

Returns:
    the position of the given arc on the boundary of the cell to its
    left; this will be an integer between 0 and ``size(cell(arc))-1``
    inclusive.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::countArcs
static const char *countArcs =
R"doc(Returns the total number of directed arcs in the underlying graph.
This is always four times the number of nodes in the graph.

Recall that each undirected edge of the graph corresponds to two
directed arcs (one exiting each endpoint of the edge).

Returns:
    the total number of directed arcs.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::countCells
static const char *countCells =
R"doc(Returns the total number of 2-cells in this cellular decomposition.

In the common case where this surface is the 2-sphere (i.e., the
underlying graph models a knot diagram), this will be exactly two more
than the number of nodes in the underlying graph.

.. note::
    As of Regina 7.4, this routine will only return 0 when the
    underlying graph is empty (and so this surface is empty also). In
    previous versions of Regina, this routine also returned 0 if the
    graph was non-planar (a scenario that was previously unsupported).

Returns:
    the total number of 2-cells.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::countComponents
static const char *countComponents =
R"doc(Returns the number of connected components in this surface. This will
be the same as the number of components of the underlying graph.

Returns:
    the number of connected components.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::countEdges
static const char *countEdges =
R"doc(Returns the total number of (undirected) edges in this cellular
decomposition. This is always twice the number of nodes in the
underlying graph.

Returns:
    the total number of edges.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::countNodes
static const char *countNodes =
R"doc(Returns the total number of vertices in this cellular decomposition;
that is, the total number of nodes in the underlying graph.

Returns:
    the total number of nodes.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::genus
static const char *genus =
R"doc(Returns the genus of this closed orientable surface. If the surface
has multiple components then this will sum the genus over each
component.

Returns:
    the genus of this surface.)doc";

// Docstring regina::python::doc::ModelLinkGraphCells_::size
static const char *size =
R"doc(Returns the number of arcs aloung the boundary of the given 2-cell. If
the given cell is a *k*-gon, then this routine returns the integer
*k*.

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
ModelLinkGraphArc(``this``, *which*); this routine returns the _other_
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

// Docstring regina::python::doc::ModelLinkGraphNode_::bigons
static const char *bigons =
R"doc(Returns the number of embedded bigons in the dual cell decomposition
that are incident with this node.

Here _embedded_ means that we do not count bigons where both vertices
are the same. Note that a _non-embedded_ incident bigon would imply
that all four arcs at this node were joined together to form two
loops, each bounding its own 1-gon (which models a 1-crossing unknot
component of a link diagram).

Returns:
    The number of incident embedded bigons, which will be between 0
    and 4 inclusive.)doc";

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

// Docstring regina::python::doc::ModelLinkGraphNode_::loops
static const char *loops =
R"doc(Returns the number of loops incident with this node.

Regarding loops versus 1-gons:

* For a planar 4-valent graph (i.e., a graph that models a classical
  link diagram), every loop bounds a 1-gon in the dual cell
  decomposition, and vice versa. In particular, for a planar graph, at
  every node we have ``0 ≤ monogons() == loops() ≤ 2``.

* For a non-planar graph (which could be used to model a virtual link
  diagram), there could be loops that do not bound 1-gons. So, for a
  non-planar graph, the only guarantee we have at each node is that
  ``0 ≤ monogons() ≤ loops() ≤ 2``.

Returns:
    The number of incident loops, which will be between 0 and 2
    inclusive.)doc";

// Docstring regina::python::doc::ModelLinkGraphNode_::monogons
static const char *monogons =
R"doc(Returns the number of 1-gons in the dual cell decomposition that are
incident with this node.

Regarding loops versus 1-gons:

* For a planar 4-valent graph (i.e., a graph that models a classical
  link diagram), every loop bounds a 1-gon in the dual cell
  decomposition, and vice versa. In particular, for a planar graph, at
  every node we have ``0 ≤ monogons() == loops() ≤ 2``.

* For a non-planar graph (which could be used to model a virtual link
  diagram), there could be loops that do not bound 1-gons. So, for a
  non-planar graph, the only guarantee we have at each node is that
  ``0 ≤ monogons() ≤ loops() ≤ 2``.

Returns:
    The number of incident 1-gons, which will be between 0 and 2
    inclusive.)doc";

// Docstring regina::python::doc::ModelLinkGraphNode_::triangles
static const char *triangles =
R"doc(Returns the number of embedded triangles in the dual cell
decomposition that are incident with this node.

Here _embedded_ means that we do not count triangles where two
vertices are the same. Note that a _non-embedded_ incident triangle
would imply that the underlying graph contains a loop bounding a 1-gon
(which models a trivial twist in a link diagram).

Returns:
    The number of incident embedded triangles, which will be between 0
    and 4 inclusive.)doc";

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

// Docstring regina::python::doc::ModelLinkGraph_::__init_2
static const char *__init_2 =
R"doc("Magic" constructor that tries to find some way to interpret the given
string as a 4-valent graph with embedding.

At present, Regina understands the following types of strings (and
attempts to parse them in the following order):

* Regina's variants of the _plantri_ format, including the default
  format as well as the tight and extended variants, as produced by
  plantri(), canonicalPlantri() and extendedPlantri().

This list may grow in future versions of Regina.

Exception ``InvalidArgument``:
    Regina could not interpret the given string as representing a
    graph using any of the supported string types.

Parameter ``description``:
    a string that describes a 4-valent graph with embedding.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::canonical
static const char *canonical =
R"doc(Returns the canonical relabelling of this graph.

Here "relabelling" allows for any combination of:

* a relabelling of the nodes;

* a relabelling of the arcs around each node, whilst preserving the
  cyclic order;

* if *allowReflection* is ``True``, a reversal of the cyclic order of
  the arcs around _every_ node (i.e., a reflection of the surface in
  which the graph embeds).

Two graphs are related under such a relabelling if and only if their
canonical relabellings are identical.

There is no promise that this will be the same canonical labelling as
used by canonicalPlantri().

The running time for this routine is quadratic in the size of the
graph.

Precondition:
    This graph is connected.

Parameter ``allowReflection``:
    ``True`` if we allow reflection of the surface in which the graph
    embeds; that is, a graph and its reflection should produce the
    same canonical relabelling.

Returns:
    the canonical relabelling of this graph.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::canonicalPlantri
static const char *canonicalPlantri =
R"doc(Outputs a text representation of this graph in a variant of the
_plantri_ ASCII format, using a canonical relabelling of nodes and
arcs, and with optional compression.

This routine is similar to plantri(), but with two significant
differences:

* This routine uses a canonical relabelling of the graph.
  Specifically, two graphs will have the same canonicalPlantri()
  output if and only if they are related under some combination of:
  (i) relabelling nodes; (ii) relabelling the arcs around each node
  whilst preserving their cyclic order; and (iii) if *allowReflection*
  is ``True``, optionally reversing the cyclic order of the arcs
  around _every_ node. This corresponds to a homeomorphism between the
  surfaces in which the graphs embed that maps one graph to the other;
  the argument *allowReflection* indicates whether this homeomorphism
  is allowed to reverse orientation. While this has a similar aim to
  canonical(), there is no promise that both routines will use the
  same "canonical relabelling".

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
be a reflection of the original, if *allowReflection* was passed as
``True``).

See plantri() for further details on the ASCII format itself,
including how Regina's implementation differs from _plantri_'s for
graphs with more than 26 nodes.

The running time for this routine is quadratic in the size of the
graph.

Precondition:
    This graph is connected.

Precondition:
    This graph has between 1 and 52 nodes inclusive.

Precondition:
    The dual to this graph is a _simple_ quadrangulation of the
    surface in which it embeds; see plantri() for a discussion on why
    this condition is needed.

Exception ``FailedPrecondition``:
    This graph is empty or has more than 52 nodes.

Parameter ``allowReflection``:
    ``True`` if a graph and its reflection should be considered the
    same (i.e., produce the same canonical output), or ``False`` if
    they should be considered different. Of course, if a graph is
    symmetric under reflection then the graph and its reflection will
    produce the same canonical output regardless of this parameter.

Parameter ``tight``:
    ``False`` if the usual _plantri_ ASCII format should be used (as
    described by plantri() and fromPlantri()), or ``True`` if the
    abbreviated format should be used as described above.

Returns:
    an optionally compressed _plantri_ ASCII representation of this
    graph.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::cells
static const char *cells =
R"doc(Returns the cellular decomposition of the closed orientable surface in
which this graph embeds. This will be the decomposition induced by
this graph; in particular, it will be formed from discs bounded by the
nodes and arcs of this graph.

This cellular decomposition will only be computed on demand. This
means that the first call to this function will take linear time (as
the decomposition is computed), but subsequent calls will be constant
time (since the decomposition is cached).

Note that, as of Regina 7.4, you can call this routine even if the
graph is non-planar and/or disconnected.

.. warning::
    This routine is not thread-safe.

Exception ``InvalidArgument``:
    This graph induces more cells than should ever be possible. This
    should never occur unless the graph is malformed in some way.

Returns:
    the induced cellular decomposition of the surface in which this
    graph embeds.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::countComponents
static const char *countComponents =
R"doc(Returns the number of connected components in this graph.

.. warning::
    This routine is not thread-safe, since it caches the number of
    components after computing it for the first time.

.. note::
    These are components in the graph theoretical sense, not link
    components. So, for example, the graph that models the Hopf link
    is considered to be connected with just one component.

Returns:
    the number of connected components.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::countTraversals
static const char *countTraversals =
R"doc(Returns the number of traversals in this graph.

A _traversal_ is a closed path through the graph that always enters
and exits a node through opposite arcs. If this graph models a diagram
for some link *L*, then the number of traversals in this graph will be
precisely the number of link components in *L*.

This routine runs in linear time (and the result is not cached).

Returns:
    the number of traversals.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::extendedPlantri
static const char *extendedPlantri =
R"doc(Outputs this graph using Regina's extended variant of the _plantri_
text format, which is better suited for non-planar graphs.

See plantri() for a discussion of the _plantri_ text format. A
limitation of the _plantri_ format is that it requires the graph to be
dual to a _simple_ quadrangulation of the surface in which it embeds.
This is a reasonable requirement for planar graphs, but not so for
non-planar graphs (which, in particular, are used to model virtual
link diagrams).

This routine extends the _plantri_ format to more explicitly encode
the embedding of the graph, which means we can remove the problematic
requirement on the dual quadrangulation. The format is Regina's own
(i.e., it is not compatible with the Brinkmann-McKay _plantri_
software).

The output will be a comma-separated sequence of alphanumeric strings.
The *i*th such string will consist of four letter-number pairs,
encoding the endpoints of the four edges in clockwise order that leave
node *i*. The letters represent nodes (with ``a..zA..Z`` representing
nodes 0 to 51 respectively). The numbers represent arcs (with ``0..3``
representing the four arcs around each node in clockwise order). An
example of such a string (describing a genus one graph that models the
virtual trefoil) is:

```
b3b2b0b1,a2a3a1a0
```

This routine is an inverse to fromExtendedPlantri(). That is, for any
graph *g* of a supported size,
``fromExtendedPlantri(g.extendedPlantri())`` will be identical to *g*.
Likewise, for any string *s* that satisfies the preconditions for
fromExtendedPlantri(), calling
``fromExtendedPlantri(s).extendedPlantri()`` will recover the original
string *s*.

Precondition:
    This graph has between 1 and 52 nodes inclusive.

Exception ``FailedPrecondition``:
    This graph is empty or has more than 52 nodes.

Returns:
    a representation of this graph in the extended _plantri_ format.)doc";

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

* It is possible that there are _no_ suitable arcs *left* and *right*.
  In this case, this routine returns a pair of null arcs.

* It is possible that there is exactly one pair of suitable arcs
  (*left*, *right*). In this case, this pair will be returned.

* It is possible that there are _many_ pairs of suitable arcs. In this
  case, it can be shown that the suitable pairs have an ordering
  *P_1*, ..., *P_k* in which the flype disc for *P_i* is completely
  contained within the flype disc for *P_j* whenever *i* < *j*. In
  this case, this routine returns the _smallest_ pair *P_1*; that is,
  the pair (*left*, *right*) that gives the smallest possible flype
  disc.

It should be noted that choosing only the smallest flype is not a
serious restriction: assuming the graph does not model a composition
of non-trivial knot diagrams, _any_ suitable flype can be expressed as
a composition of minimal flypes in this sense.

Precondition:
    This graph is planar.

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

A _flype_ is an operation on a disc in the plane. The boundary of the
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
    This graph is planar.

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
    that the connectivity and planarity preconditions will not be
    checked - these are the user's responsibility - but all other
    preconditions _will_ be checked, and an exception will be thrown
    if any of them fails.

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
    This graph is planar.

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

// Docstring regina::python::doc::ModelLinkGraph_::fromExtendedPlantri
static const char *fromExtendedPlantri =
R"doc(Builds a graph from a text representation using Regina's extended
variant of the _plantri_ format, which is better suited for non-planar
graphs.

See extendedPlantri() for a detailed description of Regina's extended
_plantri_ text format. In essence, this extends the original
Brinkmann-McKay _plantri_ format to more explicitly encode the
embedding of the graph, thereby removing the original _plantri_
requirement that the graph be dual to a simple quadrangulation of the
surface in which it embeds. Removing this requirement is important for
non-planar graphs (which are used to model virtual link diagrams).

As an example, the string below is the extended _plantri_
representation of a genus one graph that models the virtual trefoil:

```
b3b2b0b1,a2a3a1a0
```

Exception ``InvalidArgument``:
    The input was not a valid representation of a graph using Regina's
    extended _plantri_ format.

Parameter ``text``:
    the representation of a graph using Regina's extended _plantri_
    format, as described in extendedPlantri().

Returns:
    the resulting graph.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::fromPlantri
static const char *fromPlantri =
R"doc(Builds a graph from a line of _plantri_ output, using Regina's variant
of the _plantri_ ASCII format.

The software _plantri_, by Gunnar Brinkmann and Brendan McKay, can be
used to enumerate 4-valent planar graphs (amongst many other things).
This routine converts a piece of output from _plantri_, or the
encoding of a graph using Regina's more general plantri() or
canonicalPlantri() functions, into a ModelLinkGraph object that Regina
can work with directly.

Graphs encoded using Regina's plantri() or canonicalPlantri()
functions may be disconnected and/or non-planar. However, such a graph
must be dual to a simple quadrangulation of the surface in which it
embeds - otherwise the _plantri_ format does not contain enough
information to recover the embedding of the graph. This in particular
is a problem for non-planar graphs (which model virtual links). If
this is an issue for you, you can use Regina's extended _plantri_
format instead; see extendedPlantri() and fromExtendedPlantri().

If you are working with output directly from the software _plantri_,
this output must be in ASCII format, and must likewise be the dual
graph of a simple quadrangulation of the sphere. The flags that must
be passed to _plantri_ to obtain such output are ``-adq`` (although
you may wish to pass additional flags to expand or restrict the
classes of graphs that _plantri_ builds).

When run with these flags, _plantri_ produces output in the following
form:

```
6 bbcd,adca,abee,affb,cffc,deed
6 bcdd,aeec,abfd,acfa,bffb,ceed
6 bcde,affc,abfd,acee,addf,becb
```

Each line consists of an integer (the number of nodes in the graph),
followed by a comma-separated sequence of alphabetical strings that
encode the edges leaving each node.

This function _only_ takes the comma-separated sequence of
alphabetical strings. So, for example, to construct the graph
corresponding to the second line of output above, you could call:

```
fromPlantri("bcdd,aeec,abfd,acfa,bffb,ceed");
```

Regina uses its own variant of _plantri_'s output format, which is
identical for smaller graphs but which differs from _plantri_'s own
output format for larger graphs. In particular:

* For graphs with ≤ 26 nodes, Regina and _plantri_ use identical
  formats. Here Regina can happily recognise the output from _plantri_
  as described above, as well as the output from Regina's own
  plantri() and canonicalPlantri() functions.

* For graphs with 27-52 nodes, Regina's and _plantri_'s formats
  differ: whereas _plantri_ uses punctuation for higher-index nodes,
  Regina uses the upper-case letters ``A,...,Z``. For these larger
  graphs, Regina can only recognise Regina's own plantri() and
  canonicalPlantri() output, not _plantri_'s punctuation-based
  encodings.

* For graphs with 53 nodes or more, Regina cannot encode or decode
  such graphs using _plantri_ format at all.

Note that, whilst the software _plantri_ always outputs graphs using a
particular canonical labelling, this function has no such restriction:
it can accept an arbitrary ordering of nodes and arcs - in particular,
it can accept the string ``g.plantri()`` for any graph *g* that meets
the preconditions below.

This routine can also interpret the "tight" format that is optionally
produced by the member function canonicalPlantri() (even though such
output would certainly _not_ be produced by the software _plantri_).
Note that, by design, the tight format can only represented connected
graphs.

.. warning::
    While this routine does some basic error checking on the input,
    these checks are not exhaustive. In particular, it does _not_ test
    that the graph is dual to a simple quadrangulation.

Precondition:
    The graph being described is dual to a _simple_ quadrangulation of
    the surface in which it embeds; see plantri() for further
    discussion on why this condition is needed.

Exception ``InvalidArgument``:
    The input was not a valid representation of a graph using the
    _plantri_ output format.

Parameter ``plantri``:
    a string containing the comma-separated sequence of alphabetical
    strings in _plantri_ format, as described above.

Returns:
    the resulting graph.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::generateAllEmbeddings
static const char *generateAllEmbeddings =
R"doc(Generates all possible local embeddings of the given 4-valent graph
into some closed orientable surface.

The input 4-valent graph (which does _not_ contain any embedding data)
should be presented as a closed 3-dimensional facet pairing (since
these can be generated efficiently using Regina).

This routine will, up to canonical relabelling, generate all local
embeddings of the given graph into a closed orientable surface (i.e.,
all ModelLinkGraph objects corresponding to the input graph), each
exactly once.

The graphs that are generated will be labelled canonically as
described by canonical(). In particular, the argument
*allowReflection* will be passed through to canonical().

This routine is a work in progress. Currently it is _very_ inefficient
and memory-hungry; the algorithm will be improved over time if/when it
becomes important to do so.

If *allowReflection* is ``False``, then if we run all possible facet
pairings through this routine, the combined results should be
precisely those graphs described by OEIS sequence A292206. If
*allowReflection* is ``True``, then (once we reach three nodes or
more) the output set should be smaller.

For each graph that is generated, this routine will call *action*
(which must be a function or some other callable object).

* The first argument passed to *action* will be the graph that was
  generated (of type ModelLinkGraph). This will be passed as an
  rvalue; a typical action could (for example) take it by const
  reference and query it, or take it by value and modify it, or take
  it by rvalue reference and move it into more permanent storage.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* *action* must return ``void``.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.

Precondition:
    The given facet pairing is connected, and is also closed (i.e.,
    has no unmatched facets).

Python:
    This function is available in Python, and the *action* argument
    may be a pure Python function. However, its form is more
    restricted: the argument *args* is removed, so you simply call it
    as ``generateAllEmbeddings(pairing, allowReflection, action)``.
    Moreover, *action* must take exactly one argument (the graph).

Exception ``InvalidArgument``:
    The given pairing is disconnected and/or has unmatched facets.

Parameter ``pairing``:
    the 4-valent graph for which we wish to produce local embeddings.

Parameter ``allowReflection``:
    ``True`` if we consider a reflection of the surface in which the
    graph embeds to produce the same embedding.

Parameter ``action``:
    a function (or other callable object) to call for each graph that
    is generated.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial graph argument.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::generateAllLinks
static const char *generateAllLinks =
R"doc(Exhaustively generates all link diagrams that are modelled by this
graph, up to reversal of individual link components. If this graph has
*n* nodes, then there will be ``2^n`` link diagrams generated in
total.

This routine is provided mainly to help with exhaustive testing. If
you are not interested in "obviously" non-minimal link diagrams, then
you should call generateMinimalLinks() instead.

Labelled diagrams are only generated once up to reversal of each
component. Specifically, this routine will fix the orientation of each
link component (always following the smallest numbered available arc
away from the smallest index graph node in each link component).

In each link diagram that is generated, crossing *k* will always
correspond to node *k* of this graph. If this graph is non-planar,
then the resulting link diagrams will all be virtual.

For each link diagram that is generated, this routine will call
*action* (which must be a function or some other callable object).

* The first argument passed to *action* will be the link diagram that
  was generated (of type Link). This will be passed as an rvalue; a
  typical action could (for example) take it by const reference and
  query it, or take it by value and modify it, or take it by rvalue
  reference and move it into more permanent storage.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* *action* must return ``void``.

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
    as generateAllLinks(action). Moreover, *action* must take exactly
    one argument (the link diagram).

Parameter ``action``:
    a function (or other callable object) to call for each link
    diagram that is generated.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial link diagram argument.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::generateAnyLink
static const char *generateAnyLink =
R"doc(Generates an arbitrary link diagram that is modelled by this graph.

All link diagrams modelled by this graph are identical up to switching
of individual crossings and/or reversal of individual link components.
This routine will generate just one of these many possible link
diagrams. If you wish to generate _all_ such diagrams, consider
whether generateMinimalLinks() might be more appropriate for what you
need.

Unlike generateMinimalLinks(), there is no guarantee that the diagram
produced by this routine is minimal or even locally minimal in any
sense. For example, it is entirely possible that the link diagram
returned by this routine will have a reducing Reidemeister move.

In the link diagram that is generated, crossing *k* will always
correspond to node *k* of this graph. If this graph is non-planar,
then the resulting link diagram will be virtual.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::generateMinimalLinks
static const char *generateMinimalLinks =
R"doc(Exhaustively generates potentially-minimal link diagrams that are
modelled by this graph.

Here _potentially-minimal_ means there are no "obvious" simplification
moves (such as a simplifying type II Reidemeister move, for example).
The list of "obvious" moves considered here is subject to change in
future versions of Regina.

By _exhaustive_, we mean:

* Every minimal link diagram modelled by this graph will be generated
  by this routine, up to reflection and/or reversal (as explained
  below).

* If a link diagram is non-minimal and modelled by this graph, it
  _might_ still be generated by this routine.

In other words, this routine will generate all minimal link diagrams
modelled by this graph, but there is no promise that all of the
diagrams generated are minimal.

Labelled diagrams are only generated once up to reflection of the
diagram and/or reversal of each component. Here "reflection"
corresponds to the function Link::changeAll(), which reflects the link
diagram in the surface that contains it. Specifically, this routine
will fix the orientation of each link component (always following the
smallest numbered available arc away from the smallest index graph
node in each link component), and it will fix the upper and lower
strands at node 0 so that the corresponding crossing is always
positive.

In each link diagram that is generated, crossing *k* will always
correspond to node *k* of this graph. If this graph is non-planar,
then the resulting link diagrams will all be virtual.

For each link diagram that is generated, this routine will call
*action* (which must be a function or some other callable object).

* The first argument passed to *action* will be the link diagram that
  was generated (of type Link). This will be passed as an rvalue; a
  typical action could (for example) take it by const reference and
  query it, or take it by value and modify it, or take it by rvalue
  reference and move it into more permanent storage.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* *action* must return ``void``.

.. warning::
    The API for this class or function has not yet been finalised.
    This means that the interface may change in new versions of
    Regina, without maintaining backward compatibility. If you use
    this class directly in your own code, please check the detailed
    changelog with each new release to see if you need to make changes
    to your code.

Precondition:
    The cell decomposition induced by this graph has no 1-gons (which,
    in any link diagram that the graph models, would yield a reducing
    type I Reidemeister move).

Python:
    This function is available in Python, and the *action* argument
    may be a pure Python function. However, its form is more
    restricted: the argument *args* is removed, so you simply call it
    as generateMinimalLinks(action). Moreover, *action* must take
    exactly one argument (the link diagram).

Exception ``FailedPrecondition``:
    There is a 1-gon in the cell decomposition induced by this graph.

Parameter ``action``:
    a function (or other callable object) to call for each link
    diagram that is generated.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial link diagram argument.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::genus
static const char *genus =
R"doc(Returns the genus of the closed orientable surface in which this graph
embeds.

As described in the class notes, this surface is chosen to have the
smallest possible genus: it is built from a collection of discs whose
boundaries follow the nodes and arcs of this graph according to the
local embedding.

If this graph is disconnected (and therefore the surface is also
disconnected), then this routine will return the sum of the genus over
all components.

Returns:
    the genus of the surface in which this graph embeds.)doc";

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

// Docstring regina::python::doc::ModelLinkGraph_::insertGraph
static const char *insertGraph =
R"doc(Inserts a copy of the given graph into this graph.

The nodes of *source* will be copied into this graph, and placed after
any pre-existing nodes. Specifically, if the original number of nodes
in this graph was *N*, then node *i* of *source* will be copied to a
new node ``N+i`` of this graph.

This routine behaves correctly when *source* is this graph.

Parameter ``source``:
    the graph whose copy will be inserted.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::isConnected
static const char *isConnected =
R"doc(Identifies whether this graph is connected.

For the purposes of this routine, an empty graph is considered to be
connected.

.. warning::
    This routine is not thread-safe, since it caches the number of
    components after computing it for the first time.

Returns:
    ``True`` if and only if this graph is connected.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::isEmpty
static const char *isEmpty =
R"doc(Determines whether this graph is empty. An empty graph is one with no
nodes at all.

Returns:
    ``True`` if and only if this graph is empty.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::isSimple
static const char *isSimple =
R"doc(Identifies whether this graph is simple; that is, has no loops or
multiple edges.

Returns:
    ``True`` if and only if this graph is simple.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::moveContentsTo
static const char *moveContentsTo =
R"doc(Moves the contents of this graph into the given destination graph,
leaving this graph empty but otherwise usable.

The nodes of this graph will be moved directly into *dest*, and placed
after any pre-existing nodes. Specifically, if the original number of
nodes in *dest* was *N*, then node *i* of this graph will become node
``N+i`` of *dest*.

This graph will become empty as a result, but it will otherwise remain
a valid and usable ModelLinkGraph object. Any arc references or node
pointers that referred to either this graph or *dest* will remain
valid (and will all now refer to *dest*), though if they originally
referred to this graph then they will now return different numerical
node indices.

Calling ``graph.moveContentsTo(dest)`` is similar to calling
``dest.insertGraph(std::move(graph))``; it is a little slower but it
comes with the benefit of leaving this graph in a usable state.

Precondition:
    *dest* is not this graph.

Parameter ``dest``:
    the graph into which the contents of this graph should be moved.)doc";

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
_plantri_.

The software _plantri_, by Gunnar Brinkmann and Brendan McKay, can be
used to enumerate 4-valent planar graphs (amongst many other things).
This routine outputs this graph in a format that mimics _plantri_'s
own dual ASCII format (i.e., the format that _plantri_ outputs when
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

For graphs with at most 26 nodes, this is identical to _plantri_'s own
dual ASCII format. For larger graphs, this format differs: _plantri_
uses punctuation to represent higher-index nodes, whereas Regina uses
upper-case letters.

Although _plantri_ is designed to work with graphs that are connected
and planar, this routine will happily produce output for disconnected
and/or non-planar graphs. However, there remains an unavoidable
requirement: the graph must be dual to a _simple_ quadrangulation. In
detail:

* The dual to this 4-valent graph will be a quadrangulation of the
  surface in which it embeds. The _plantri_ format inherently requires
  that this quadrangulation is _simple_: that is, the dual must have
  no loops or parallel edges.

* This requirement exists because, if the dual is _not_ simple, the
  embedding of the original graph cannot be uniquely reconstructed
  from its _plantri_ output. In particular, the embedding becomes
  ambiguous around parallel edges in the original 4-valent graph.

* For _planar_ graphs, this requirement is relatively harmless: a
  parity condition shows that loops in the dual are impossible, and
  parallel edges in the dual mean that any link diagram that this
  graph models is an "obvious" connected sum.

* For _non-planar_ graphs, this requirement is more problematic. For
  example, consider the graph that models the virtual trefoil: the
  dual quadrangulation of the torus contains both loops and parallel
  edges. This makes the _plantri_ format unusable in practice for
  graps that model virtual links.

If this constraint is too onerous (e.g., you are working with virtual
links), you could use extendedPlantri() instead, which is not
compatible with the Brinkmann-McKay _plantri_ software but which
removes this requirement for the dual quadrangulation to be simple.

For graphs that the _plantri_ format _does_ support, this routine is
an inverse to fromPlantri(). That is, for any graph *g* that satisfies
the preconditions below, ``fromPlantri(g.plantri())`` is identical to
*g*. Likewise, for any string *s* that satisfies the preconditions for
fromPlantri(), calling ``fromPlantri(s).plantri()`` will recover the
original string *s*.

.. note::
    The output of this function might not correspond to any possible
    output from the program _plantri_ itself, even if the graph is
    connected and planar, the dual quadrangulation is simple, and only
    lower-case letters are used. This is because _plantri_ only
    outputs graphs with a certain canonical labelling. In contrast,
    plantri() can be called on any graph that satisfies the
    preconditions below, and it will preserve the labels of the nodes
    and the order of the arcs around each node.

Precondition:
    This graph has between 1 and 52 nodes inclusive.

Precondition:
    The dual to this graph is a _simple_ quadrangulation of the
    surface in which it embeds.

Exception ``FailedPrecondition``:
    This graph is empty or has more than 52 nodes.

Returns:
    a _plantri_ format ASCII representation of this graph.)doc";

// Docstring regina::python::doc::ModelLinkGraph_::randomise
static const char *randomise =
R"doc(Randomly relabels this graph in an orientation-preserving manner.

The nodes will be relabelled arbitrarily. Around each node, the four
outgoing arcs will be relabelled in a random way that preserves their
cyclic order (thereby preserving the local embedding of the graph,
without reflection).

This routine is thread-safe, and uses RandomEngine for its random
number generation.)doc";

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

