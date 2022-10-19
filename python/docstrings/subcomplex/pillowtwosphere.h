/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::PillowTwoSphere
static const char *PillowTwoSphere =
R"doc(Represents a 2-sphere made from two triangles glued together along
their three edges. The two triangles must be distinct and the three
edges of each triangle must also be distinct. Neither of the triangles
may be boundary triangles. These two triangless together form an
embedded 2-sphere in the triangulation (with the exception that two or
three points of the sphere corresponding to the triangles vertices may
be identified).

This 2-sphere can be cut along and the two resulting 2-sphere
boundaries filled in with 3-balls, and the resulting triangulation has
the same number of tetrahedra as the original. If the original
2-sphere was separating, the resulting triangulation will contain the
two terms of the corresponding connected sum.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions. However, the only way to create them (aside from copying or
moving) is via the static member function recognise().)doc";

namespace PillowTwoSphere_ {

// Docstring regina::python::doc::PillowTwoSphere_::__copy
static const char *__copy = R"doc(Creates a new copy of the given structure.)doc";

// Docstring regina::python::doc::PillowTwoSphere_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object represent the same
specific presentation of a pillow 2-sphere.

Specifically, two pillow 2-spheres will compare as equal if and only
if they join the same two numbered triangles, presented in the same
order, using the same mapping of triangle vertices.

Since this test looks at triangle numbers and not the specific
Triangle objects, it is meaningful to compare pillow 2-spheres within
different triangulations.

Parameter ``other``:
    the pillow 2-sphere to compare with this.

Returns:
    ``True`` if and only if this and the given object represent the
    same specific presentation of a pillow 2-sphere.)doc";

// Docstring regina::python::doc::PillowTwoSphere_::__ne
static const char *__ne =
R"doc(Determines whether this and the given object represent different
specific presentations of a pillow 2-sphere.

Specifically, two pillow 2-spheres will compare as equal if and only
if they join the same two numbered triangles, presented in the same
order, using the same mapping of triangle vertices.

Since this test looks at triangle numbers and not the specific
Triangle objects, it is meaningful to compare pillow 2-spheres within
different triangulations.

Parameter ``other``:
    the pillow 2-sphere to compare with this.

Returns:
    ``True`` if and only if this and the given object represent
    different specific presentations of a pillow 2-sphere.)doc";

// Docstring regina::python::doc::PillowTwoSphere_::recognise
static const char *recognise =
R"doc(Determines if the two given triangles together form a pillow 2-sphere.

Even though PillowTwoSphere is a two-dimensional class and so does not
inherit from StandardTriangulation, this routine nevertheless returns
by (smart) pointer for consistency with the StandardTriangulation
recognition routines (which use pointers because of the polymorphic
StandardTriangulation class hierarchy).

Precondition:
    The two given triangles are distinct.

Parameter ``tri1``:
    the first triangle to examine.

Parameter ``tri2``:
    the second triangle to examine.

Returns:
    a structure containing details of the pillow 2-sphere, or ``null``
    if the given triangles do not form a pillow 2-sphere.)doc";

// Docstring regina::python::doc::PillowTwoSphere_::triangle
static const char *triangle =
R"doc(Returns one of the two triangles whose boundaries are joined.

Parameter ``index``:
    specifies which of the two triangles to return; this must be
    either 0 or 1.

Returns:
    the corresponding triangle.)doc";

// Docstring regina::python::doc::PillowTwoSphere_::triangleMapping
static const char *triangleMapping =
R"doc(Returns a permutation describing how the boundaries of the two
triangles are joined.

The permutation will map vertices (0,1,2) of ``triangle(0)`` to
vertices (0,1,2) of ``triangle(1)``. The map will represent how the
vertices of the triangles are identified by the three edge gluings.

Returns:
    a permutation describing how the triangle boundaries are joined.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

