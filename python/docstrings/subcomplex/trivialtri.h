/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::TrivialTri
static const char *TrivialTri =
R"doc(Represents one of a few particular hard-coded trivial triangulations
that do not belong to any of the other larger families.

All optional StandardTriangulation routines are implemented for this
class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
StandardTriangulation subclasses. Note that the only way to create
these objects (aside from copying or moving) is via the static member
function recognise().)doc";

namespace TrivialTri_ {

// Docstring regina::python::doc::TrivialTri_::__copy
static const char *__copy = R"doc(Creates a new copy of the given structure.)doc";

// Docstring regina::python::doc::TrivialTri_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
type of trivial triangulation.

This is the same as testing whether type() returns the same value for
this and the given triangulation.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass means
they describe isomorphic structures).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent the
    same type of trivial triangulation.)doc";

// Docstring regina::python::doc::TrivialTri_::__ne
static const char *__ne =
R"doc(Determines whether this and the given structure represent different
types of trivial triangulation.

This is the same as testing whether type() returns different values
for this and the given triangulation.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass means
they describe isomorphic structures).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent
    different types of trivial triangulation.)doc";

// Docstring regina::python::doc::TrivialTri_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls TrivialTri::swap(); it is provided so
that TrivialTri meets the C++ Swappable requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::TrivialTri_::recognise
static const char *recognise =
R"doc(Determines if the given triangulation component is one of the trivial
triangulations recognised by this class.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``comp``:
    the triangulation component to examine.

Returns:
    a structure representing the trivial triangulation, or ``None`` if
    the given component is not one of the triangulations recognised by
    this class.)doc";

// Docstring regina::python::doc::TrivialTri_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::TrivialTri_::type
static const char *type =
R"doc(Returns the specific trivial triangulation being represented.

Returns:
    the specific triangulation. This will be one of the triangulation
    constants defined in this class.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

