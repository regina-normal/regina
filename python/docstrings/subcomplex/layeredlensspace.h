/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::LayeredLensSpace
static const char *LayeredLensSpace =
R"doc(Represents a layered lens space component of a triangulation. A
layered lens space is considered to be any layered solid torus glued
to a degenerate (2,1,1) layered solid torus (i.e., a one-triangle
mobius strip). Note that the three possible gluing options represent
the three possible ways of closing the initial torus - either twisting
it shut (in one of two possible ways) or snapping it shut without any
twist.

A layered lens space must contain at least one tetrahedron.

All optional StandardTriangulation routines are implemented for this
class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
StandardTriangulation subclasses. Note that the only way to create
these objects (aside from copying or moving) is via the static member
function recognise().)doc";

namespace LayeredLensSpace_ {

// Docstring regina::python::doc::LayeredLensSpace_::__copy
static const char *__copy = R"doc(Creates a new copy of the given structure.)doc";

// Docstring regina::python::doc::LayeredLensSpace_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
type of layered lens space.

Specifically, two layered lens spaces will compare as equal if and
only if they are formed from layered solid tori with the same
parameters, closed off in the same way.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass is
more specific than combinatorial isomorphism, since the same layered
lens space can be built from a layered solid torus starting at either
of its two "ends").

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent the
    same type of layered lens space.)doc";

// Docstring regina::python::doc::LayeredLensSpace_::__ne
static const char *__ne =
R"doc(Determines whether this and the given structure represent different
types of layered lens space.

Specifically, two layered lens spaces will compare as equal if and
only if they are formed from layered solid tori with the same
parameters, closed off in the same way.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass is
more specific than combinatorial isomorphism, since the same layered
lens space can be built from a layered solid torus starting at either
of its two "ends").

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent
    different types of layered lens space.)doc";

// Docstring regina::python::doc::LayeredLensSpace_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls LayeredLensSpace::swap(); it is
provided so that LayeredLensSpace meets the C++ Swappable
requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::LayeredLensSpace_::isSnapped
static const char *isSnapped =
R"doc(Determines if the layered solid torus that forms the basis for this
lens space is snapped shut (folded closed without a twist).

Returns:
    ``True`` if and only if the torus is snapped shut.)doc";

// Docstring regina::python::doc::LayeredLensSpace_::isTwisted
static const char *isTwisted =
R"doc(Determines if the layered solid torus that forms the basis for this
lens space is twisted shut (folded closed with a twist).

Returns:
    ``True`` if and only if the torus is twisted shut.)doc";

// Docstring regina::python::doc::LayeredLensSpace_::mobiusBoundaryGroup
static const char *mobiusBoundaryGroup =
R"doc(Determines which edge of the layered solid torus is glued to the
boundary of the mobius strip (i.e., the weight 2 edge of the
degenerate (2,1,1) layered solid torus). The return value will be one
of the three top level tetrahedron edge groups in the layered solid
torus; see LayeredSolidTorus::topEdge() for further details about edge
groups.

Returns:
    the top level edge group of the layered solid torus to which the
    mobius strip boundary is glued.)doc";

// Docstring regina::python::doc::LayeredLensSpace_::p
static const char *p =
R"doc(Returns the first parameter *p* of this lens space L(p,q).

Returns:
    the first parameter *p*.)doc";

// Docstring regina::python::doc::LayeredLensSpace_::q
static const char *q =
R"doc(Returns the second parameter *q* of this lens space L(p,q).

Returns:
    the second parameter *q*.)doc";

// Docstring regina::python::doc::LayeredLensSpace_::recognise
static const char *recognise =
R"doc(Determines if the given triangulation component is a layered lens
space.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``comp``:
    the triangulation component to examine.

Returns:
    a structure containing details of the layered lens space, or
    ``null`` if the given component is not a layered lens space.)doc";

// Docstring regina::python::doc::LayeredLensSpace_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::LayeredLensSpace_::torus
static const char *torus =
R"doc(Returns the layered solid torus to which the mobius strip is glued.

Returns:
    the layered solid torus.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

