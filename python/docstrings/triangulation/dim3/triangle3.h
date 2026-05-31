/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct Triangle3 {

// Docstring regina::python::doc::Triangle3::__class
static constexpr const char __class[] =
R"doc(Represents a triangle in the skeleton of a 3-manifold triangulation.

This is a specialisation of the generic Face class template; see the
generic documentation for Face for a general overview of how the face
classes work. In Python, you can read this generic documentation by
looking at faces in a higher dimension: try ``help(Triangle5)``.

These specialisations for Regina's standard dimensions offer
significant extra functionality.

Triangles do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Triangulation to which they belong.

Python:
    This class is available to Python users under the name Triangle3.)doc";

// Docstring regina::python::doc::Triangle3::isCone
static constexpr const char isCone[] =
R"doc(Deprecated function that determines whether this triangle is wrapped
up to form a cone, possibly with or without additional identifications
between its vertices and/or edges.

.. deprecated::
    This has been renamed to formsCone(), and is now available for
    triangulations of all dimensions ≥ 3. See formsCone() for further
    information.

Returns:
    ``True`` if and only if this triangle forms a cone.)doc";

// Docstring regina::python::doc::Triangle3::isMobiusBand
static constexpr const char isMobiusBand[] =
R"doc(Deprecated function that determines whether this triangle is wrapped
up to form a Möbius band, possibly with or without additional
identifications between its vertices and/or edges.

.. deprecated::
    This has been renamed to formsMobiusBand(), and is now available
    for triangulations of all dimensions ≥ 3. See formsMobiusBand()
    for further information.

Returns:
    ``True`` if and only if this triangle forms a Möbius band.)doc";

// Docstring regina::python::doc::Triangle3::subtype
static constexpr const char subtype[] =
R"doc(Deprecated function that returns the vertex or edge number in this
triangle that plays a special role for this triangle's combinatorial
type.

.. deprecated::
    This has been renamed to triangleSubtype(), and is now available
    for triangulations of all dimensions ≥ 3. See triangleSubtype()
    for further information.

Returns:
    the vertex or edge number (0, 1 or 2) that plays a special role,
    or -1 if this triangle's combinatorial type has no special vertex
    or edge.)doc";

// Docstring regina::python::doc::Triangle3::type
static constexpr const char type[] =
R"doc(Deprecated function that returns the combinatorial type of this
triangle.

.. deprecated::
    This has been renamed to triangleType(), and is now available for
    triangulations of all dimensions ≥ 3. See triangleType() for
    further information.

Returns:
    the combinatorial type of this triangle, which will never be
    TriangleType::Unknown.)doc";

}; // struct Triangle3

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

