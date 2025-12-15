/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Cut
static const char *Cut =
R"doc(A cut that separates a triangulation, facet pairing or link diagram
into two pieces. This is essentially the same concept as a cut in
graph theory.

Specifically:

* A _cut_ in a triangulation or facet pairing partitions the top-
  dimensional simplices into two _sides_. This describes how to split
  the triangulation or facet pairing into two pieces, by removing all
  gluings between simplices on opposite sides.

* A cut in a link diagram or a model link graph likewise partitions
  the crossings or nodes into two sides. However, since links in
  Regina cannot have free ends, cuts cannot be used to divide a link
  diagram into two smaller pieces (and likewise for model link
  graphs).

In general, we will use the word _nodes_ to refer to the objects that
are partitioned by the cut (e.g., the top-dimensional simplices of a
triangulation, or the crossings of a link diagram), and we will use
the word _arcs_ to refer to the connections between these objects
(e.g., the individual gluings between top-dimensional simplices, or
the arcs between crossings in a link diagram).

The two sides of a cut are numbered 0 and 1. In Regina, a cut has a
_size_ and a _weight:_

* The _size_ refers to the size of the underlying graph-like object;
  that is, the total number of nodes (using the terminology above).

* The _weight_ is the usual concept of weight from graph theory; that
  is, the number of arcs that cross between the two sides of the
  partition. In particular, for triangulations and facet pairings, it
  counts the number of gluings between top-dimensional simplices that
  would be undone when using the cut to subdivide the object into two
  pieces.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace Cut_ {

// Docstring regina::python::doc::Cut_::__call
static const char *__call =
R"doc(Partitions the given triangulation using this cut.

This routine will return _two_ triangulations: the first will contain
all the top-dimensional simplices on side 0 of this cut, and the
second will contain all the top-dimensional simplices on side 1. All
gluings within the same side of the partition will be preserved, but
any gluings that cross the partition will be lost (and so the
corresponding simplex facets will become boundary).

You can call inclusion() if you need to know how the simplex numbers
of the resulting triangulations correspond to the simplex numbers of
the original triangulation.

If any of the facets that cross the partition are locked in the source
triangulation *tri*, this will not prevent the operation from
occurring (since the source triangulation will not be changed). The
two triangulations that are returned will have no simplex and/or facet
locks at all.

Precondition:
    The given triangulation has precisely size() top-dimensional
    simplices.

Exception ``InvalidArgument``:
    The given triangulation does not have precisely size() top-
    dimensional simplices.

Parameter ``tri``:
    the triangulation to partition.

Returns:
    the two resulting triangulations, one for each side of the
    partition.)doc";

// Docstring regina::python::doc::Cut_::__call_2
static const char *__call_2 =
R"doc(Partitions the given facet pairing using this cut.

This routine will return _two_ facet pairings: the first will contain
all the top-dimensional simplices on side 0 of this cut, and the
second will contain all the top-dimensional simplices on side 1. All
matchings between simplex facets within the same side of the partition
will be preserved, but any matchings that cross the partition will be
lost (and so the corresponding simplex facets will become unmatched).

You can call inclusion() if you need to know how the simplex numbers
of the resulting pairings correspond to the simplex numbers of the
original pairing.

Precondition:
    The given facet pairing has precisely size() top-dimensional
    simplices.

Precondition:
    Since empty facet pairings are not allowed, this cut must have at
    least one top-dimensional simplex on each side.

Exception ``InvalidArgument``:
    The given facet pairing does not have precisely size() top-
    dimensional simplices.

Exception ``FailedPrecondition``:
    This cut has all of its top-dimensional simplices on the same
    side.

Parameter ``pairing``:
    the facet pairing to partition.

Returns:
    the two resulting facet pairings, one for each side of the
    partition.)doc";

// Docstring regina::python::doc::Cut_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given cut.

Parameter ``src``:
    the cut to copy.)doc";

// Docstring regina::python::doc::Cut_::__eq
static const char *__eq =
R"doc(Determines if this and the given cut are identical.

Two cuts are considered identical if they describe the same partition
of nodes into sides 0 and 1.

It does not matter if this and the given cut have different sizes; in
this case they will be considered different.

Parameter ``rhs``:
    the cut to compare with this.

Returns:
    ``True`` if and only if this and the given cut are identical.)doc";

// Docstring regina::python::doc::Cut_::__init
static const char *__init =
R"doc(Creates a new trivial cut on the given number of nodes.

All nodes will be on side 0.

Parameter ``size``:
    the number of nodes in the underlying graph-like object.)doc";

// Docstring regina::python::doc::Cut_::__init_2
static const char *__init_2 =
R"doc(Creates a new cut with the given partition sizes.

The total number of nodes under consideration will be ``side0 +
side1``; the first *side0* nodes will be on side 0, and the remaining
*side1* nodes will be on side 1.

Parameter ``side0``:
    the number of nodes on side 0 of the partition.

Parameter ``side1``:
    the number of nodes on side 1 of the partition.)doc";

// Docstring regina::python::doc::Cut_::__init_3
static const char *__init_3 =
R"doc(Creates a new cut using the given partition.

Here a cut on *n* nodes is described by a sequence of *n* integers,
each equal to 0 or 1, indicating which side of the partition each node
lies on.

The iterator type must be random access because this allows the
implementation to compute the number of nodes in constant time.

Exception ``InvalidArgument``:
    Some element of the given sequence is neither 0 nor 1.

Python:
    Instead of a pair of iterators, this routine takes a python list
    of integers.

Parameter ``begin``:
    an iterator pointing to the beginning of the 0-1 sequence of
    sides.

Parameter ``end``:
    a past-the-end iterator indicating the end of the 0-1 sequence of
    sides.)doc";

// Docstring regina::python::doc::Cut_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given cuts.

This global routine simply calls Cut::swap(); it is provided so that
Cut meets the C++ Swappable requirements.

Parameter ``a``:
    the first cut whose contents should be swapped.

Parameter ``b``:
    the second cut whose contents should be swapped.)doc";

// Docstring regina::python::doc::Cut_::inc
static const char *inc =
R"doc(Converts this into the next cut of the same size.

The total number of nodes will stay the same, but the number on each
side of the partition may change.

To iterate through all cuts of the given size, you should create a new
``Cut(size)`` and then make repeated calls to inc().

If this is already the last partition in such an iteration (i.e., all
nodes are already on side 1), then this routine will return ``False``
and convert this into the *first* such partition.

The order of iteration using inc() is lexicographical in the sequence
of sides. In particular, if you wish to avoid seeing each cut again
with sides 0 and 1 swapped, then you can use the fact that all cuts
with ``side(0) == 0`` will be seen before any cuts with ``side(0) ==
1``.

Returns:
    ``True`` if the partition was successfully incremented, or
    ``False`` if this was already the last partition in such an
    iteration.)doc";

// Docstring regina::python::doc::Cut_::incFixedSizes
static const char *incFixedSizes =
R"doc(Converts this into the next cut with the same partition sizes.

Specifically, the number of nodes on each side of the partition will
remain the same.

To iterate through all cuts with the given parititon sizes, you should
create a new ``Cut(side0, side1)`` and then make repeated calls to
incFixedSizes().

If this is already the last partition in such an iteration, then this
routine will return ``False`` and convert this into the _first_ such
permutation.

The order of iteration using incFixedSizes() is lexicographical in the
sequence of sides. In particular, if you wish to avoid seeing each cut
again with sides 0 and 1 swapped, then you can use the fact that all
cuts with ``side(0) == 0`` will be seen before any cuts with ``side(0)
== 1``.

Returns:
    ``True`` if the partition was successfully incremented, or
    ``False`` if this was already the last partition in such an
    iteration.)doc";

// Docstring regina::python::doc::Cut_::inclusion
static const char *inclusion =
R"doc(Returns the relationships between simplex numbers before and after
this cut is used to partition a triangulation or facet pairing into
two pieces.

Specifically: let *from* be a trianglation or facet pairing, and let
(*a*, *b*) be the result of partitioning *from* using this cut, so
``(a, b) = cut(from)``.

Then this routine returns two isomorphisms *p* and *q*, where *p*
describes how *a* appears as a subcomplex of *from*, and *q* describes
how *b* appears as a subcomplex of *from*. These isomorphisms will be
in the direction from *a* and *b* to *from*.

The only interesting parts of these isomorphisms are the mappings
between the indices of top-dimensional simplices; all of the facet
permutations within each top-dimensional simplex will be identity
permutations.

Python:
    Since Python does not support templates, the dimension *dim*
    should be passed as an argument to this function.

Template parameter ``dim``:
    indicates which type of isomorphisms to return. Specifically, this
    integer parameter indicates the dimension of triangulation on
    which these isomorphisms act.

Returns:
    the two inclusion maps corresponding to this partition.)doc";

// Docstring regina::python::doc::Cut_::isTrivial
static const char *isTrivial =
R"doc(Determines whether this cut places all nodes on the same side of the
partition.

Returns:
    ``True`` if all nodes are on side 0 or all nodes are on side 1, or
    ``False`` if both sides of the partition are non-empty.)doc";

// Docstring regina::python::doc::Cut_::set
static const char *set =
R"doc(Allows you to set which side of the partition the given node lies on.

Exception ``InvalidArgument``:
    The given side is not 0 or 1.

Parameter ``node``:
    the node being changed; this must be between 0 and size()-1
    inclusive.

Parameter ``newSide``:
    the side of the partition that the given node should lie on; this
    must be either 0 or 1.)doc";

// Docstring regina::python::doc::Cut_::side
static const char *side =
R"doc(Indicates which side of the partition the given node lies on.

Parameter ``node``:
    the node being queried; this must be between 0 and size()-1
    inclusive.

Returns:
    the corresponding side of the partition; this will be either 0 or
    1.)doc";

// Docstring regina::python::doc::Cut_::size
static const char *size =
R"doc(Returns the total number of nodes in the underlying graph-like object.

In particular, if you are working with a triangulation or facet
pairing, then this returns the number of top-dimensional simplices.

Returns:
    the total number of nodes.)doc";

// Docstring regina::python::doc::Cut_::size_2
static const char *size_2 =
R"doc(Returns the number of nodes on the given side of the partition
described by this cut.

It will always be true that ``size(0) + size(1) == size()``.

.. warning::
    This routine runs in linear time, since the sizes of the
    individual sides are not cached. This is in contrast to the
    overall total size(), which _is_ cached, and which runs in
    constant time.

Exception ``InvalidArgument``:
    The given side is not 0 or 1.

Parameter ``whichSide``:
    the side of the partition to query; this must be either 0 or 1.

Returns:
    the number of nodes on the given side.)doc";

// Docstring regina::python::doc::Cut_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given cut.

Parameter ``other``:
    the cut whose contents are to be swapped with this.)doc";

// Docstring regina::python::doc::Cut_::weight
static const char *weight =
R"doc(Returns the weight of this cut with respect to the dual graph of the
given triangulation. This is the number of gluings in the given
triangulation that cross the partition described by this cut.

In other words, this routine counts the number of facets of top-
dimensional simplices on side 0 of the cut that are glued to a facet
of some top-dimensional simplex on side 1.

Precondition:
    The given triangulation has precisely size() top-dimensional
    simplices.

Exception ``InvalidArgument``:
    The given triangulation does not have precisely size() top-
    dimensional simplices.

Parameter ``tri``:
    the triangulation under consideration.

Returns:
    the weight of this cut with respect to *tri*.)doc";

// Docstring regina::python::doc::Cut_::weight_2
static const char *weight_2 =
R"doc(Returns the weight of this cut with respect to the given facet
pairing. This is the number of matchings between facets of simplices
in the given pairing that cross the partition described by this cut.

In other words, this routine counts the number of facets of top-
dimensional simplices on side 0 of the cut that are paired with a
facet of some top-dimensional simplex on side 1.

Precondition:
    The given facet pairing has precisely size() top-dimensional
    simplices.

Exception ``InvalidArgument``:
    The given facet pairing does not have precisely size() top-
    dimensional simplices.

Parameter ``pairing``:
    the facet pairing under consideration.

Returns:
    the weight of this cut with respect to *pairing*.)doc";

// Docstring regina::python::doc::Cut_::weight_3
static const char *weight_3 =
R"doc(Returns the weight of this cut with respect to the given link diagram.
This is the number of arcs in the link diagram that cross the
partition described by this cut.

Precondition:
    The given link diagram has precisely size() crossings.

Exception ``InvalidArgument``:
    The given link diagram does not have precisely size() crossings.

Parameter ``link``:
    the link diagram under consideration.

Returns:
    the weight of this cut with respect to *link*.)doc";

// Docstring regina::python::doc::Cut_::weight_4
static const char *weight_4 =
R"doc(Returns the weight of this cut with respect to the given model link
graph. This is the number of arcs in the graph that cross the
partition described by this cut.

Precondition:
    The given graph has precisely size() nodes.

Exception ``InvalidArgument``:
    The given graph does not have precisely size() nodes.

Parameter ``graph``:
    the model link graph under consideration.

Returns:
    the weight of this cut with respect to *graph*.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

