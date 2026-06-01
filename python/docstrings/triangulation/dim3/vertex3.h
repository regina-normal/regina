/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct Vertex3 {

// Docstring regina::python::doc::Vertex3::__class
static constexpr const char __class[] =
R"doc(Represents a vertex in the skeleton of a 3-manifold triangulation.

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
    This class is available to Python users under the name Vertex3.)doc";

// Docstring regina::python::doc::Vertex3::isIdeal
static constexpr const char isIdeal[] =
R"doc(Determines if this vertex is an ideal vertex. This requires the vertex
link to be closed and not a 2-sphere.

Returns:
    ``True`` if and only if this is an ideal vertex.)doc";

// Docstring regina::python::doc::Vertex3::isLinkClosed
static constexpr const char isLinkClosed[] =
R"doc(Determines if the link of this vertex is closed.

Returns:
    ``True`` if and only if the link of this vertex is closed.)doc";

// Docstring regina::python::doc::Vertex3::isStandard
static constexpr const char isStandard[] =
R"doc(Determines if this vertex is standard. This requires the vertex link
to be a sphere, disc, torus or Klein bottle.

Returns:
    ``True`` if and only if this vertex is standard.)doc";

// Docstring regina::python::doc::Vertex3::linkType
static constexpr const char linkType[] =
R"doc(Returns a broad categorisation of the link of the vertex. This
considers topological information only, not the combinatorics of how
the link is triangulated.

This routine does not require a full triangulation of the vertex link,
and so can be much faster than analysing the result of buildLink().

This is the routine that used to be called link() in Regina 6.0.1 and
earlier. It was renamed to linkType() in Regina 7.0, to clear the way
for a different routine to be called link() in the future.

Returns:
    a broad categorisation of the vertex link.)doc";

struct Link {

// Docstring regina::python::doc::Vertex3::Link::Disc
static constexpr const char Disc[] =
R"doc(Specifies a vertex link that is a disc. In other words, the vertex
lies on a real boundary component.)doc";

// Docstring regina::python::doc::Vertex3::Link::Invalid
static constexpr const char Invalid[] =
R"doc(Specifies a vertex link that has boundary and is not a disc. In other
words, this vertex makes the triangulation invalid.)doc";

// Docstring regina::python::doc::Vertex3::Link::KleinBottle
static constexpr const char KleinBottle[] =
R"doc(Specifies a vertex link that is a Klein bottle. In other words, this
is an ideal vertex representing a Klein bottle cusp.)doc";

// Docstring regina::python::doc::Vertex3::Link::NonStandardCusp
static constexpr const char NonStandardCusp[] =
R"doc(Specifies a vertex link that is closed and is not a sphere, torus or
Klein bottle. In other words, this is an ideal vertex but not one of
the standard ideal vertex types.)doc";

// Docstring regina::python::doc::Vertex3::Link::Sphere
static constexpr const char Sphere[] =
R"doc(Specifies a vertex link that is a sphere. In other words, the vertex
is internal.)doc";

// Docstring regina::python::doc::Vertex3::Link::Torus
static constexpr const char Torus[] =
R"doc(Specifies a vertex link that is a torus. In other words, this is an
ideal vertex representing a torus cusp.)doc";

// Docstring regina::python::doc::Vertex3::Link::__class
static constexpr const char __class[] =
R"doc(Categorises the possible links of a vertex into a small number of
common types. Here a vertex link is considered only up to its topology
(not the combinatorics of its triangulation).

See also:
    link)doc";

}; // struct Link

}; // struct Vertex3

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

