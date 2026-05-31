/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct Vertex4 {

// Docstring regina::python::doc::Vertex4::__class
static constexpr const char __class[] =
R"doc(Represents a vertex in the skeleton of a 4-manifold triangulation.

This is a specialisation of the generic Face class template; see the
generic documentation for Face for a general overview of how the face
classes work. In Python, you can read this generic documentation by
looking at faces in a higher dimension: try ``help(Vertex5)``.

These specialisations for Regina's standard dimensions offer
significant extra functionality.

Vertices do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Triangulation to which they belong.

Python:
    This class is available to Python users under the name Vertex4.)doc";

// Docstring regina::python::doc::Vertex4::isIdeal
static constexpr const char isIdeal[] =
R"doc(Determines if this vertex is an ideal vertex. To be an ideal, a vertex
must (i) be valid, and (ii) have a closed vertex link that is not a
3-sphere.

Returns:
    ``True`` if and only if this is an ideal vertex.)doc";

}; // struct Vertex4

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

