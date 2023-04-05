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
R"doc(A cut that separates a triangulation or facet pairing into two pieces.
This is essentially the same concept as a cut in graph theory.

Specifically, a _cut_ in a triangulation or facet pairing partitions
the top-dimensional simplices into two _sides_. This effectively
splits the triangulation or facet pairing into two pieces, by removing
all gluings between simplices on opposite sides. The two sides of a
cut are numbered 0 and 1.

In Regina, a cut has a _size_ and a _weight:_

* The _size_ refers to the size of the underlying triangulation or
  facet pairing (i.e., it indicates the total number of top-
  dimensional simplices).

* The _weight_ refers to the number of gluings that are undone by the
  cut. This is the usual concept of weight from graph theory (i.e.,
  the number of edges in the underlying graph that cross the
  partition).

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
of simplices into sides 0 and 1.

It does not matter if this and the given cut have different sizes; in
this case they will be considered different.

Parameter ``rhs``:
    the cut to compare with this.

Returns:
    ``True`` if and only if this and the given cut are identical.)doc";

// Docstring regina::python::doc::Cut_::__init
static const char *__init =
R"doc(Creates a new trivial cut on the given number of top-dimensional
simplices.

All simplices will be on side 0.

Parameter ``size``:
    the number of top-dimensional simplices in the underlying
    triangulation or facet pairing.)doc";

// Docstring regina::python::doc::Cut_::__init_2
static const char *__init_2 =
R"doc(Creates a new cut with the given partition sizes.

The total number of top-dimensional simplices under consideration will
be (*side0* + *side1*); the first *side0* simplices will be on side 0,
and the remaining *side1* simplices will be on side 1.

Parameter ``side0``:
    the number of top-dimensional simplices on side 0 of the
    partition.

Parameter ``side1``:
    the number of top-dimensional simplices on side 1 of the
    partition.)doc";

// Docstring regina::python::doc::Cut_::__init_3
static const char *__init_3 =
R"doc(Creates a new cut using the given partition.

Here a cut on *n* top-dimensional simplices is described by a sequence
of *n* integers, each equal to 0 or 1, indicating which side of the
partition each top-dimensional simplex lies on.

Precondition:
    The type *iterator*, when dereferenced, can be cast to an ``int``.

.. warning::
    This routine computes the number of top-dimensional simplices by
    subtracting ``end - begin``, and so ideally *iterator* should be a
    random access iterator type for which this operation is constant
    time.

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

// Docstring regina::python::doc::Cut_::__ne
static const char *__ne =
R"doc(Determines if this and the given cut are different.

Two cuts are considered identical if they describe the same partition
of simplices into sides 0 and 1.

It does not matter if this and the given cut have different sizes; in
this case they will be considered different.

Parameter ``rhs``:
    the cut to compare with this.

Returns:
    ``True`` if and only if this and the given cut are different.)doc";

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

The total number of top-dimensional simplices will stay the same, but
the number on each side of the partition may change.

To iterate through all cuts of the given size, you should create a new
``Cut(size)`` and then make repeated calls to inc().

If this is already the last partition in such an iteration (i.e., all
top-dimensional simplices are already on side 1), then this routine
will return ``False`` and convert this into the *first* such
partition.

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

Specifically, the number of top-dimensional simplices on each side of
the partition will remain the same.

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
R"doc(Determines whether this cut places all top-dimensional simplices on
the same side of the partition.

Returns:
    ``True`` if all simplices are on side 0 or all simplices are on
    side 1, or ``False`` if both sides of the partition are non-empty.)doc";

// Docstring regina::python::doc::Cut_::set
static const char *set =
R"doc(Allows you to set which side of the partition the given simplex lies
on.

Exception ``InvalidArgument``:
    The given side is not 0 or 1.

Parameter ``simplex``:
    the simplex being changed; this must be between 0 and size()-1
    inclusive.

Parameter ``newSide``:
    the side of the partition that the given simplex should lie on;
    this must be either 0 or 1.)doc";

// Docstring regina::python::doc::Cut_::side
static const char *side =
R"doc(Indicates which side of the partition the given simplex lies on.

Parameter ``simplex``:
    the simplex being queried; this must be between 0 and size()-1
    inclusive.

Returns:
    the corresponding side of the partition; this will be either 0 or
    1.)doc";

// Docstring regina::python::doc::Cut_::size
static const char *size =
R"doc(Returns the total number of top-dimensional simplices in the
underlying triangulation or facet pairing.

In other words, this returns the size of the underlying triangulation
or facet pairing.

Returns:
    the total number of top-dimensional simplices.)doc";

// Docstring regina::python::doc::Cut_::size_2
static const char *size_2 =
R"doc(Returns the number of top-dimensional simplices on the given side of
the partition described by this cut.

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
    the number of top-dimensional simplices on the given side.)doc";

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

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

