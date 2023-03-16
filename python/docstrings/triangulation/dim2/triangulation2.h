/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Triangulation
static const char *Triangulation =
R"doc(Represents a 2-manifold triangulation.

This is a specialisation of the generic Triangulation class template;
see the Triangulation documentation for a general overview of how the
triangulation classes work.

This 2-dimensional specialisation offers significant extra
functionality, including many functions specific to 2-manifolds.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace Triangulation_ {

// Docstring regina::python::doc::Triangulation_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given triangulation.

This will also clone any computed properties (such as homology,
fundamental group, and so on), as well as the skeleton (vertices,
edges, components, etc.). In particular, the same numbering and
labelling will be used for all skeletal objects.

If you want a "clean" copy that resets all properties to unknown and
leaves the skeleton uncomputed, you can use the two-argument copy
constructor instead.

Parameter ``src``:
    the triangulation to copy.)doc";

// Docstring regina::python::doc::Triangulation_::__default
static const char *__default =
R"doc(Default constructor.

Creates an empty triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::__init
static const char *__init =
R"doc(Creates a new copy of the given triangulation, with the option of
whether or not to clone its computed properties also.

If *cloneProps* is ``True``, then this constructor will also clone any
computed properties (such as homology, fundamental group, and so on),
as well as the skeleton (vertices, edges, components, etc.). In
particular, the same numbering and labelling will be used for all
skeletal objects in both triangulations.

If *cloneProps* is ``False``, then these properties and skeletal
objects will be marked as unknown in the new triangulation, and will
be recomputed on demand if/when they are required. Note in particular
that, when the skeleton is recomputed, there is no guarantee that the
numbering and labelling for skeletal objects will be the same as in
the source triangulation.

Parameter ``src``:
    the triangulation to copy.

Parameter ``cloneProps``:
    ``True`` if this should also clone any computed properties as well
    as the skeleton of the given triangulation, or ``False`` if the
    new triangulation should have such properties and skeletal data
    marked as unknown.)doc";

// Docstring regina::python::doc::Triangulation_::__init_2
static const char *__init_2 =
R"doc("Magic" constructor that tries to find some way to interpret the given
string as a triangulation.

At present, Regina understands the following types of strings (and
attempts to parse them in the following order):

* isomorphism signatures (see fromIsoSig()).

This list may grow in future versions of Regina.

Exception ``InvalidArgument``:
    Regina could not interpret the given string as representing a
    triangulation using any of the supported string types.

Parameter ``description``:
    a string that describes a 2-manifold triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::countBoundaryEdges
static const char *countBoundaryEdges =
R"doc(A dimension-specific alias for countBoundaryFacets().

See countBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::eulerChar
static const char *eulerChar =
R"doc(Returns the Euler characteristic of this triangulation. This will be
evaluated as ``V-E+F``.

This returns the same result as eulerCharTri().

Returns:
    the Euler characteristic of this triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::hasBoundaryEdges
static const char *hasBoundaryEdges =
R"doc(A dimension-specific alias for hasBoundaryFacets().

See hasBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::isBall
static const char *isBall =
R"doc(Determines whether this is a triangulation of a 2-ball.

Unlike the 3-dimensional version of this routine, isBall() for
2-manifolds is fast and simple.

Returns:
    ``True`` if and only if this is a 2-ball triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::isClosed
static const char *isClosed =
R"doc(Determines if this triangulation is closed. This is the case if and
only if it has no boundary components.

Returns:
    ``True`` if and only if this triangulation is closed.)doc";

// Docstring regina::python::doc::Triangulation_::isIdeal
static const char *isIdeal =
R"doc(Always returns ``False``.

This routine determines if this triangulation is ideal (has a non-
trivial vertex link); however, every vertex link in a 2-manifold
triangulation is either the interval or the circle, and so ideal
triangulations cannot exist. Therefore this routine always returns
``False``.

This no-op routine is provided for consistency with higher dimensional
triangulations, and to assist with writing dimension-agnostic code.

Returns:
    ``False``.)doc";

// Docstring regina::python::doc::Triangulation_::isMinimal
static const char *isMinimal =
R"doc(Determines whether this is a minimal triangulation of the underlying
2-manifold; that is, it uses the fewest possible triangles.

Testing for minimality is simple in two dimensions (unlike higher
dimensions, where it becomes extremely difficult). With the exception
of the sphere, disc and projective plane (which require a minimum of
2, 1 and 2 triangles respectively), a closed triangulation is minimal
if and only if it has one vertex, and a bounded triangulation is
minimal if and only if it has one vertex per boundary component and no
internal vertices.

The proof is based on a simple Euler characteristic calculation,
whereby the number of triangles ``T`` is ``T = 2I + B - 2C``, where
``I`` and ``B`` are the number of internal and boundary vertices
respectively, and where ``C`` is the Euler characteristic of the
underlying manifold.

Returns:
    ``True`` if and only if this is a minimal triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::isSphere
static const char *isSphere =
R"doc(Determines whether this is a triangulation of a 2-sphere.

Unlike the 3-dimensional version of this routine, isSphere() for
2-manifolds is fast and simple.

Returns:
    ``True`` if and only if this is a 2-sphere triangulation.)doc";

// Docstring regina::python::doc::Triangulation_::newTriangle
static const char *newTriangle =
R"doc(A dimension-specific alias for newSimplex().

See newSimplex() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::newTriangle_2
static const char *newTriangle_2 =
R"doc(A dimension-specific alias for newSimplex().

See newSimplex() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::newTriangles
static const char *newTriangles =
R"doc(A dimension-specific alias for newSimplices().

See newSimplices() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::newTriangles_2
static const char *newTriangles_2 =
R"doc(A dimension-specific alias for newSimplices().

See newSimplices() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::removeAllTriangles
static const char *removeAllTriangles =
R"doc(A dimension-specific alias for removeAllSimplices().

See removeAllSimplices() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::removeTriangle
static const char *removeTriangle =
R"doc(A dimension-specific alias for removeSimplex().

See removeSimplex() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::removeTriangleAt
static const char *removeTriangleAt =
R"doc(A dimension-specific alias for removeSimplexAt().

See removeSimplexAt() for further information.)doc";

// Docstring regina::python::doc::Triangulation_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given triangulation.

All triangles that belong to this triangulation will be moved to
*other*, and all triangles that belong to *other* will be moved to
this triangulation. Likewise, all skeletal objects (such as lower-
dimensional faces, components, and boundary components) and all cached
properties will be swapped.

In particular, any pointers or references to Triangle<2> and/or
Face<2, subdim> objects will remain valid.

This routine will behave correctly if *other* is in fact this
triangulation.

.. note::
    This swap function is _not_ marked ``noexcept``, since it fires
    change events on both triangulations which may in turn call
    arbitrary code via any registered packet listeners.

Parameter ``other``:
    the triangulation whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

