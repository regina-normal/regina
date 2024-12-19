/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Face
static const char *Face =
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
of by the Triangulation to which they belong.)doc";

namespace Face_ {

// Docstring regina::python::doc::Face_::isCone
static const char *isCone =
R"doc(Determines whether this triangle is wrapped up to form a cone.

Note that several different triangle types (as returned by type()) can
produce this result. Note also that a triangle can be both a Mobius
band *and* a cone.

Returns:
    ``True`` if and only if this triangle is a cone.)doc";

// Docstring regina::python::doc::Face_::isMobiusBand
static const char *isMobiusBand =
R"doc(Determines whether this triangle is wrapped up to form a Mobius band.

Note that several different triangle types (as returned by type()) can
produce this result. Note also that a triangle can be both a Mobius
band *and* a cone.

Returns:
    ``True`` if and only if this triangle is a Mobius band.)doc";

// Docstring regina::python::doc::Face_::linkingSurface
static const char *linkingSurface =
R"doc(Returns the link of this triangle as a normal surface.

Constructing the link of a triangle begins with building the frontier
of a regular neighbourhood of the triangle. If this is already a
normal surface, then then link is called _thin_. Otherwise the usual
normalisation steps are performed until the surface becomes normal;
note that these normalisation steps could change the topology of the
surface, and in some pathological cases could even reduce it to the
empty surface.

Returns:
    a pair (*s*, *thin*), where *s* is the triangle linking normal
    surface, and *thin* is ``True`` if and only if this link is thin
    (i.e., no additional normalisation steps were required).

Returns:
    the corresponding triangle linking normal surface.)doc";

// Docstring regina::python::doc::Face_::subtype
static const char *subtype =
R"doc(Return the triangle vertex or triangle edge that plays a special role
for the triangle type of this triangle. Note that this routine is only
relevant for some triangle types. The triangle type is returned by
type().

Returns:
    The vertex or edge that plays a special role (this will be 0, 1 or
    2), or -1 if this triangle type has no special vertex or edge.)doc";

// Docstring regina::python::doc::Face_::type
static const char *type =
R"doc(Returns a description of the triangle type. This will be one of the
eight shapes described by the TriangleType enumeration, indicating how
the edges and vertices of the triangle are identified.

Returns:
    the type of this triangle. This routine will never return
    TriangleType::Unknown.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

