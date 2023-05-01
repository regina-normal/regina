/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::LayeredChainPair
static const char *LayeredChainPair =
R"doc(Represents a layered chain pair component of a triangulation.

A layered chain pair consists of two layered chains (as described by
class LayeredChain) glued together in a particular way.

Orient the hinge edges and diagonals of each chain so they all point
in the same direction around the solid tori formed by each layered
chain (a _diagonal_ is an edge between the two top triangular faces or
an edge between the two bottom triangular faces of a layered chain).

The two top faces of the first chain are glued to a top and bottom
face of the second chain, and the two bottom faces of the first chain
are glued to a top and bottom face of the second chain.

The four oriented diagonals are all identified as a single edge. Of
the remaining unglued edges (two hinge edges and two non-hinge edges
per chain), each hinge edge of one chain must be identified to a non-
hinge edge of the other chain and vice versa. From here the face
identifications are uniquely determined.

Note that a layered chain pair in which one of the chains contains
only one tetrahedron is in fact a layered loop with a twist (see class
LayeredLoop).

All optional StandardTriangulation routines are implemented for this
class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
StandardTriangulation subclasses. Note that the only way to create
these objects (aside from copying or moving) is via the static member
function recognise().)doc";

namespace LayeredChainPair_ {

// Docstring regina::python::doc::LayeredChainPair_::__copy
static const char *__copy = R"doc(Creates a new copy of the given structure.)doc";

// Docstring regina::python::doc::LayeredChainPair_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
type of layered chain pair.

Specifically, two layered chain pairs will compare as equal if and
only if they their constituent layered chains each have the same
respective lengths, _and_ appear in the same order.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass is
more specific than combinatorial isomorphism, due to the ordering
requirement).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent the
    same type of layered chain pair.)doc";

// Docstring regina::python::doc::LayeredChainPair_::__ne
static const char *__ne =
R"doc(Determines whether this and the given structure represent different
types of layered chain pair.

Specifically, two layered chain pairs will compare as equal if and
only if they their constituent layered chains each have the same
respective lengths, _and_ appear in the same order.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass is
more specific than combinatorial isomorphism, due to the ordering
requirement).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent
    different types of layered chain pair.)doc";

// Docstring regina::python::doc::LayeredChainPair_::chain
static const char *chain =
R"doc(Returns the requested layered chain used to form this structure. If
the two chains have different lengths, the shorter chain will be chain
0 and the longer chain will be chain 1.

Parameter ``which``:
    specifies which chain to return; this must be 0 or 1.

Returns:
    the requested layered chain.)doc";

// Docstring regina::python::doc::LayeredChainPair_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls LayeredChainPair::swap(); it is
provided so that LayeredChainPair meets the C++ Swappable
requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::LayeredChainPair_::recognise
static const char *recognise =
R"doc(Determines if the given triangulation component is a layered chain
pair.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``comp``:
    the triangulation component to examine.

Returns:
    a structure containing details of the layered chain pair, or
    ``None`` if the given component is not a layered chain pair.)doc";

// Docstring regina::python::doc::LayeredChainPair_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

