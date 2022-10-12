/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::PrismSpec
static const char *PrismSpec =
R"doc(Specifies a single triangular prism in a tetrahedron.

If a tetrahedron contains normal quads, slicing along these quads
splits the tetrahedron into two triangular prisms (and possibly some
additional product regions). Each triangular prism contains two of the
vertices and one of the edges of the original tetrahedron.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Precondition:
    This class should only be used with *embedded* normal surfaces.)doc";

namespace PrismSpec_ {

// Docstring regina::python::doc::PrismSpec_::PrismSpec
static const char *PrismSpec = R"doc(Creates a new uninitialised prism specifier.)doc";

// Docstring regina::python::doc::PrismSpec_::PrismSpec_2
static const char *PrismSpec_2 =
R"doc(Creates a new prism specifier containing the given values.

Parameter ``newTetIndex``:
    the index in the triangulation of the tetrahedron containing the
    prism.

Parameter ``newEdge``:
    the edge of the tetrahedron that is contained in this prism; this
    must be between 0 and 5 inclusive.)doc";

// Docstring regina::python::doc::PrismSpec_::PrismSpec_3
static const char *PrismSpec_3 =
R"doc(Creates a new prism specifier that is a clone of the given specifier.

Parameter ``cloneMe``:
    the prism specifier to clone.)doc";

// Docstring regina::python::doc::PrismSpec_::__eq
static const char *__eq =
R"doc(Determines if this and the given prism specifier contain identical
information.

Parameter ``other``:
    the prism specifier to compare with this.

Returns:
    ``True`` if and only if this and the given prism specifier contain
    identical information.)doc";

// Docstring regina::python::doc::PrismSpec_::__ne
static const char *__ne =
R"doc(Determines if this and the given prism specifier contain different
information.

Parameter ``other``:
    the prism specifier to compare with this.

Returns:
    ``True`` if and only if this and the given prism specifier contain
    different information.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

