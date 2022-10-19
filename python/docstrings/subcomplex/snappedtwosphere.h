/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SnappedTwoSphere
static const char *SnappedTwoSphere =
R"doc(Represents a 2-sphere made from two snapped 3-balls in a
triangulation. This occurs when two snapped 3-balls are glued together
at their equators (note that this gluing does not have to extend to
triangular faces). Each 3-ball has a central disc (bounded by the
3-ball's equator and bisecting its internal degree one edge), and
these two discs together form an embedded 2-sphere in the
triangulation.

This 2-sphere can be cut along and the two resulting 2-sphere
boundaries filled in with 3-balls, and the resulting triangulation has
the same number of tetrahedra as the original. If the snapped 2-sphere
was separating, the resulting triangulation will contain the two terms
of the corresponding connected sum.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions. However, the only way to create them (aside from copying or
moving) is via the static member function recognise().)doc";

namespace SnappedTwoSphere_ {

// Docstring regina::python::doc::SnappedTwoSphere_::__copy
static const char *__copy = R"doc(Creates a new copy of this structure.)doc";

// Docstring regina::python::doc::SnappedTwoSphere_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object represent the same
specific presentation of a snapped 2-sphere.

Specifically, two snapped 2-spheres will compare as equal if and only
if they slice through the same pair of numbered tetrahedra, presented
in the same order, and bisecting the same numbered degree one edges
within corresponding tetrahedra.

Since this test looks at tetrahedron numbers and not the specific
Tetrahedron objects, it is meaningful to compare snapped 2-spheres
within different triangulations.

Parameter ``other``:
    the snapped 2-sphere to compare with this.

Returns:
    ``True`` if and only if this and the given object represent the
    same specific presentation of a snapped 2-sphere.)doc";

// Docstring regina::python::doc::SnappedTwoSphere_::__ne
static const char *__ne =
R"doc(Determines whether this and the given object represent different
specific presentations of a snapped 2-sphere.

Specifically, two snapped 2-spheres will compare as equal if and only
if they slice through the same pair of numbered tetrahedra, presented
in the same order, and bisecting the same numbered edges within
corresponding tetrahedra.

Since this test looks at tetrahedron numbers and not the specific
Tetrahedron objects, it is meaningful to compare snapped 2-spheres
within different triangulations.

Parameter ``other``:
    the snapped 2-sphere to compare with this.

Returns:
    ``True`` if and only if this and the given object represent
    different specific presentations of a snapped 2-sphere.)doc";

// Docstring regina::python::doc::SnappedTwoSphere_::recognise
static const char *recognise =
R"doc(Determines if the two given tetrahedra together form a snapped
2-sphere.

Even though SnappedTwoSphere is a two-dimensional class and so does
not inherit from StandardTriangulation, this routine nevertheless
returns by (smart) pointer for consistency with the
StandardTriangulation recognition routines (which use pointers because
of the polymorphic StandardTriangulation class hierarchy).

Precondition:
    The two given tetrahedra are distinct.

Parameter ``tet1``:
    the first tetrahedron to examine.

Parameter ``tet2``:
    the second tetrahedron to examine.

Returns:
    a structure containing details of the snapped 2-sphere, or
    ``null`` if the given tetrahedra do not form a snapped 2-sphere.)doc";

// Docstring regina::python::doc::SnappedTwoSphere_::recognise_2
static const char *recognise_2 =
R"doc(Determines if the two given snapped 3-balls together form a snapped
2-sphere.

If this is the case, the snapped 3-balls stored in the structure
returned will be clones of the original 3-balls, not the original
3-balls themselves.

Even though SnappedTwoSphere is a two-dimensional class and so does
not inherit from StandardTriangulation, this routine nevertheless
returns by (smart) pointer for consistency with the
StandardTriangulation recognition routines (which use pointers because
of the polymorphic StandardTriangulation class hierarchy).

Precondition:
    The two given snapped 3-balls use distinct tetrahedra.

Parameter ``ball1``:
    the first snapped 3-ball to examine.

Parameter ``ball2``:
    the second snapped 3-ball to examine.

Returns:
    a structure containing details of the snapped 2-sphere, or
    ``null`` if the given snapped 3-balls do not form a snapped
    2-sphere.)doc";

// Docstring regina::python::doc::SnappedTwoSphere_::snappedBall
static const char *snappedBall =
R"doc(Returns one of the two snapped 3-balls whose equators are joined.

Parameter ``index``:
    specifies which of the two 3-balls to return; this must be either
    0 or 1.

Returns:
    the corresponding snapped 3-ball.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

