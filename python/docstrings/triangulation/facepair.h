/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct FacePair {

// Docstring regina::python::doc::FacePair::__class
static constexpr const char __class[] =
R"doc(Represents a pair of tetrahedron face numbers.

Note that we are not storing triangle numbers in a triangulation
skeleton, but merely face numbers 0, 1, 2 and 3 in a single
tetrahedron. Thus there are only six possible face pairs; this class
merely makes working with such pairs more convenient.

As well as providing the six possible face pairs, there is also a
before-the-start and a past-the-end value; this (combined with the
increment operator) allows for iteration through face pairs.

When iterating through face pairs, the ordering will be the
lexicographical ordering (0,1), (0,2), (0,3), (1,2), (1,3), (2,3).

The before-the-start and past-the-end values are internally
represented as (0,0) and (3,3) respectively.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.)doc";

// Docstring regina::python::doc::FacePair::__cmp
static constexpr const char __cmp[] =
R"doc(Compares two face pairs lexicographically.

This generates all of the usual comparison operators, including ``<``,
``<=``, ``>``, and ``>=``.

Python:
    This spaceship operator ``x <=> y`` is not available, but the
    other comparison operators that it generates _are_ available.

Returns:
    the result of the lexicographical comparison between this and the
    given face pair.)doc";

// Docstring regina::python::doc::FacePair::__copy
static constexpr const char __copy[] = R"doc(Creates a new face pair that is a clone of the given pair.)doc";

// Docstring regina::python::doc::FacePair::__dec
static constexpr const char __dec[] =
R"doc(Decrements this face pair. It will be set to the previous face pair in
the lexicographical ordering, or to a before-the-start value if there
are no previous face pairs.

This is a postdecrement operator: the object will be changed, but a
copy of the original reference will be returned.

Precondition:
    This face pair is not currently before-the-start.

Python:
    This routine is available under the name dec().

Returns:
    a copy of this object before the change took place.)doc";

// Docstring regina::python::doc::FacePair::__default
static constexpr const char __default[] = R"doc(Creates a new face pair (0,1).)doc";

// Docstring regina::python::doc::FacePair::__eq
static constexpr const char __eq[] =
R"doc(Determines if this and the given face pair are equal.

Returns:
    ``True`` if and only if this and the given pair are equal.)doc";

// Docstring regina::python::doc::FacePair::__inc
static constexpr const char __inc[] =
R"doc(Increments this face pair. It will be set to the following face pair
in the lexicographical ordering, or to a past-the-end value if there
are no more face pairs.

This is a postincrement operator: the object will be changed, but a
copy of the original reference will be returned.

Precondition:
    This face pair is not currently past-the-end.

Python:
    This routine is available under the name inc().

Returns:
    a copy of this object before the change took place.)doc";

// Docstring regina::python::doc::FacePair::__init
static constexpr const char __init[] =
R"doc(Creates a new face pair from the two given face numbers. The two given
numbers need not be in any particular order.

Precondition:
    The two given face numbers must be distinct integers between 0 and
    3 inclusive.

Parameter ``first``:
    the first face number in the new pair.

Parameter ``second``:
    the second face number in the new pair.)doc";

// Docstring regina::python::doc::FacePair::commonEdge
static constexpr const char commonEdge[] =
R"doc(Identifies the tetrahedron edge that is common to both faces in this
pair.

This will be a tetrahedron edge number, between 0 and 5 inclusive.
Note that edges commonEdge() and oppositeEdge() will be opposite edges
in the tetrahedron.

Precondition:
    This face pair is neither before-the-start nor past-the-end.

Returns:
    the edge that belongs to both faces in this pair.)doc";

// Docstring regina::python::doc::FacePair::complement
static constexpr const char complement[] =
R"doc(Returns the complement of this face pair. The complement is the pair
containing the two faces not included in this face pair.

Precondition:
    This face pair is neither before-the-start nor past-the-end.

Returns:
    the complement of this face pair.)doc";

// Docstring regina::python::doc::FacePair::isBeforeStart
static constexpr const char isBeforeStart[] =
R"doc(Determines if this face pair represents a before-the-start value.

Returns:
    ``True`` if and only if this face pair is before-the-start.)doc";

// Docstring regina::python::doc::FacePair::isPastEnd
static constexpr const char isPastEnd[] =
R"doc(Determines if this face pair represents a past-the-end value.

Returns:
    ``True`` if and only if this face pair is past-the-end.)doc";

// Docstring regina::python::doc::FacePair::lower
static constexpr const char lower[] =
R"doc(Returns the smaller of the two face numbers in this pair.

Precondition:
    This face pair is neither before-the-start or past-the-end.

Returns:
    the lower face number.)doc";

// Docstring regina::python::doc::FacePair::oppositeEdge
static constexpr const char oppositeEdge[] =
R"doc(Identifies the tetrahedron edge that belongs to neither of the two
faces in this pair.

This will be a tetrahedron edge number, between 0 and 5 inclusive.
Note that edges commonEdge() and oppositeEdge() will be opposite edges
in the tetrahedron.

Precondition:
    This face pair is neither before-the-start nor past-the-end.

Returns:
    the edge that does not belong to either of these two faces.)doc";

// Docstring regina::python::doc::FacePair::upper
static constexpr const char upper[] =
R"doc(Returns the larger of the two face numbers in this pair.

Precondition:
    This face pair is neither before-the-start or past-the-end.

Returns:
    the upper face number.)doc";

}; // struct FacePair

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

