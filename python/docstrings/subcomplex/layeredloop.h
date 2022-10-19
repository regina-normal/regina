/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::LayeredLoop
static const char *LayeredLoop =
R"doc(Represents a layered loop component of a triangulation.

A layered loop is a layered chain of *n* tetrahedra whose bottom
tetrahedron is layered onto its top tetrahedron to form a complete
loop. See the LayeredChain class notes for a description of a layered
chain.

To make a layered chain into a layered loop, the bottom faces of the
first tetrahedron will be layered upon the top faces of the last
tetrahedron, completing the loop. At this stage there is a choice. The
layering can be done in the usual fashion, or there may be a *twist*
in which the upper square (the bottom faces of the first tetrahedron)
is rotated 180 degrees before being layered on the lower annulus (the
top faces of the last tetrahedron).

If there is a twist, the two hinge edges become identified and the
entire component has a single vertex. If there is no twist, the two
hinge edges remain distinct (and disjoint) and the entire component
has two vertices.

The *length* of the layered loop is the number of tetrahedra it
contains. A layered loop must contain at least one tetrahedron.

All optional StandardTriangulation routines are implemented for this
class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
StandardTriangulation subclasses. Note that the only way to create
these objects (aside from copying or moving) is via the static member
function recognise().)doc";

namespace LayeredLoop_ {

// Docstring regina::python::doc::LayeredLoop_::__copy
static const char *__copy = R"doc(Creates a new copy of this structure.)doc";

// Docstring regina::python::doc::LayeredLoop_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
type of layered loop.

Specifically, two layered loops will compare as equal if they have the
same length and either both are twisted or both are untwisted.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass means
they describe isomorphic structures).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent the
    same type of layered loop.)doc";

// Docstring regina::python::doc::LayeredLoop_::__ne
static const char *__ne =
R"doc(Determines whether this and the given structure represent different
types of layered loop.

Specifically, two layered loops will compare as equal if they have the
same length and either both are twisted or both are untwisted.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass means
they describe isomorphic structures).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent
    different types of layered loop.)doc";

// Docstring regina::python::doc::LayeredLoop_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls LayeredLoop::swap(); it is provided
so that LayeredLoop meets the C++ Swappable requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::LayeredLoop_::hinge
static const char *hinge =
R"doc(Returns the requested hinge edge of this layered loop. See the general
class notes for further details. If there is only one hinge but
parameter *which* is 1, ``null`` will be returned.

Parameter ``which``:
    specifies which hinge to return; this must be 0 or 1.

Returns:
    the requested hinge edge.)doc";

// Docstring regina::python::doc::LayeredLoop_::isTwisted
static const char *isTwisted =
R"doc(Determines if this layered loop contains a twist. See the general
class notes for further details.

Returns:
    ``True`` if and only if this layered loop contains a twist.)doc";

// Docstring regina::python::doc::LayeredLoop_::length
static const char *length =
R"doc(Returns the length of this layered loop. See the general class notes
for further details.

Returns:
    the length of this layered loop.)doc";

// Docstring regina::python::doc::LayeredLoop_::recognise
static const char *recognise =
R"doc(Determines if the given triangulation component is a layered loop.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``comp``:
    the triangulation component to examine.

Returns:
    a structure containing details of the layered loop, or ``null`` if
    the given component is not a layered loop.)doc";

// Docstring regina::python::doc::LayeredLoop_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

