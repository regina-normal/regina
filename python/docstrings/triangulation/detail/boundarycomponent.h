/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


namespace detail {

// Docstring regina::python::doc::detail::BoundaryComponentBase
constexpr const char *BoundaryComponentBase =
R"doc(Helper class that provides core functionality for a boundary component
of a *dim*-dimensional triangulation.

Each boundary component is represented by the class
BoundaryComponent<dim>, which uses this as a base class. End users
should not need to refer to BoundaryComponentBase directly.

See the BoundaryComponent class notes for further information.

Neither this class nor the "end user" class BoundaryComponent<dim>
support value semantics: they cannot be copied, swapped, or manually
constructed. Their memory is managed by the Triangulation class, and
their locations in memory define them. See BoundaryComponent<dim> for
further details.

Python:
    This base class is not present, but the "end user" class
    BoundaryComponent<dim> is.

Template parameter ``dim``:
    the dimension of the underlying triangulation. This must be
    between 2 and 15 inclusive.)doc";

}

namespace detail::BoundaryComponentBase_ {

// Docstring regina::python::doc::detail::BoundaryComponentBase_::build
constexpr const char *build =
R"doc(Returns the full (*dim*-1)-dimensional triangulation of this boundary
component. Note that this triangulation is read-only (though of course
you can clone it and then operate upon the clone).

If this is a real boundary component (i.e., it is built from one or
more (*dim*-1)-faces), then the triangulation of this boundary
component is as follows:

* Let *i* lie between 0 and size()-1 inclusive. Then simplex *i* of
  the returned (*dim*-1)-dimensional triangulation is a copy of
  ``facet(i)`` of this boundary component, and its vertices
  0,...,*dim*-1 are numbered in the same way. To relate these
  (*dim*-1)-face vertex numbers to the vertex numbers of top-
  dimensional simplices in the overall *dim*-dimensional
  triangulation, see Simplex<dim>::faceMapping<dim-1>().

* If this boundary component stores lower-dimensional faces (i.e., if
  the class constant *allFaces* is ``True``), then a similar
  correspondence holds for these lower-dimensional faces also: for
  each *i*, *k*-face *i* of the returned triangulation is a copy of
  ``face<k>(i)`` of this boundary component, and its vertices are
  numbered in the same way. As an exception, this correspondence will
  not hold for dimensions *k* where there exist _pinched_ *k*-faces on
  the boundary (i.e., faces where different sections of the boundary
  are pinched together, meaning that these faces must be duplicated
  when the boundary is triangulated).

If this boundary component consists only of a single vertex (i.e.,
this is an ideal or invalid vertex boundary component), then this
routine returns the triangulation of the corresponding vertex link.
See Vertex::buildLink() for details.

This routine is fast, since it caches the boundary triangulation.
Moreover, it is guaranteed that the full skeleton of this
(*dim*-1)-dimensional triangulation will have been generated already.

Precondition:
    The dimension *dim* is greater than 2.

Returns:
    the triangulation of this boundary component.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::component
constexpr const char *component =
R"doc(Returns the connected component of the triangulation to which this
boundary component belongs.

Returns:
    the component containing this boundary component.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::countEdges
constexpr const char *countEdges =
R"doc(A dimension-specific alias for countFaces<1>().

This alias is available only when *dim* is one of Regina's standard
dimensions.

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::countFaces
constexpr const char *countFaces =
R"doc(Returns the number of *subdim*-faces in this boundary component.

Python:
    Python does not support templates. Instead, Python users should
    call this function in the form ``countFaces(subdim)``; that is,
    the template parameter *subdim* becomes the first argument of the
    function.

Template parameter ``subdim``:
    the dimension of the faces to query. If *dim* is one of Regina's
    standard dimensions, then *subdim* must be between 0 and *dim*-1
    inclusive. Otherwise, the only allowable values of *subdim* are
    the facet dimension (*dim*-1) and the ridge dimension (*dim*-2).

Returns:
    the number of *subdim*-faces.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::countPentachora
constexpr const char *countPentachora =
R"doc(A dimension-specific alias for countFaces<4>().

This alias is only available for dimensions *dim* = 5 and 6.

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::countRidges
constexpr const char *countRidges =
R"doc(Returns the number of (*dim*-2)-faces in this boundary component.

If this is an ideal or invalid vertex boundary component, then this
routine will return 0.

Returns:
    the number of (*dim*-2)-faces in this boundary component.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::countTetrahedra
constexpr const char *countTetrahedra =
R"doc(A dimension-specific alias for countFaces<3>().

This alias is only available for dimensions *dim* = 4 and 5.

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::countTriangles
constexpr const char *countTriangles =
R"doc(A dimension-specific alias for countFaces<2>().

This alias is available only when *dim* is one of Regina's standard
dimensions and *dim* ≥ 3.

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::countVertices
constexpr const char *countVertices =
R"doc(A dimension-specific alias for countFaces<0>().

This alias is available only when *dim* is one of Regina's standard
dimensions.

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::edge
constexpr const char *edge =
R"doc(A dimension-specific alias for face<1>().

This alias is available only when *dim* is one of Regina's standard
dimensions.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::edges
constexpr const char *edges =
R"doc(A dimension-specific alias for faces<1>().

This alias is available only when *dim* is one of Regina's standard
dimensions.

See faces() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::eulerChar
constexpr const char *eulerChar =
R"doc(Returns the Euler characteristic of this boundary component. If the
boundary component consists of a single vertex and nothing else (e.g.,
it is an ideal vertex), then the Euler characteristic of the vertex
link will be returned.

This function is, in all "normal" cases, equivalent to triangulating
the boundary component via build() and then calling
Triangulation<dim-1>::eulerCharTri() on the result.

The exception comes from triangulations with "pinched" faces whose
links have multiple boundary components (e.g., a vertex whose link is
a multiply-punctured sphere, marking a point where different parts of
the boundary are "pinched together"). If there are such faces, then
this routine will return a well-defined but topologically meaningless
result. Essentially, this routine only counts such faces once, even
though they "should" be counted multiple times on the boundary since
they can be "seen" from distinct sections of the (*dim*-1)-dimensional
boundary. Of course such a triangulation cannot represent a
*dim*-manifold anyway, and so if you do have pinched faces then you
almost certainly have bigger problems to deal with.

Precondition:
    *dim* is one of Regina's standard dimensions.

.. warning::
    If this boundary component itself forms an ideal
    (*dim*-1)-dimensional triangulation, then again this result is
    well-defined but topologically meaningless (since it is equivalent
    to calling eulerCharTri() and not eulerCharManifold() on the
    triangulated boundary). However, again such boundary components
    cannot appear in a *dim*-manifold, and so if you have such
    boundary components then you almost certainly have bigger problems
    than this.

Returns:
    the Euler characteristic of this boundary component.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::face
constexpr const char *face =
R"doc(Returns the requested *subdim*-face in this boundary component.

Note that the index of a face in the boundary component need not be
the index of the same face in the overall triangulation. However, if
this is a real boundary component (i.e., it is built from one or more
(*dim*-1)-faces), then the index of each *subdim*-face in this
boundary component will match the index of the corresponding
*subdim*-face in the (*dim*-1)-manifold triangulation returned by
build().

Python:
    Python does not support templates. Instead, Python users should
    call this function in the form ``face(subdim, index)``; that is,
    the template parameter *subdim* becomes the first argument of the
    function.

Template parameter ``subdim``:
    the dimension of the faces to query. If *dim* is one of Regina's
    standard dimensions, then *subdim* must be between 0 and *dim*-1
    inclusive. Otherwise, the only allowable value of *subdim* is the
    facet dimension (*dim*-1).

Parameter ``index``:
    the index of the desired face, ranging from 0 to
    countFaces<subdim>()-1 inclusive.

Returns:
    the requested face.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::faces
constexpr const char *faces =
R"doc(Returns an object that allows iteration through and random access to
all *subdim*-faces in this boundary component.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. Note that the elements of the list will be
pointers, so your code might look like:

```
for (Face<dim, subdim>* f : bc.faces<subdim>()) { ... }
```

The object that is returned will remain valid only for as long as this
boundary component object exists. In particular, the object will
become invalid any time that the triangulation changes (since all
boundary component objects will be destroyed and others rebuilt in
their place). Therefore it is best to treat this object as temporary
only, and to call faces() again each time you need it.

Python:
    Python does not support templates. Instead, Python users should
    call this function in the form ``faces(subdim)``.

Template parameter ``subdim``:
    the dimension of the faces to query. If *dim* is one of Regina's
    standard dimensions, then *subdim* must be between 0 and *dim*-1
    inclusive. Otherwise, the only allowable value of *subdim* is the
    facet dimension (*dim*-1).

Returns:
    access to the list of all *subdim*-faces.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::facet
constexpr const char *facet =
R"doc(Returns the requested (*dim*-1)-face in this boundary component. These
are the top-dimensional faces for a real boundary component.

Note that the index of a face in the boundary component need not be
the index of the same face in the overall triangulation. However, if
this is a real boundary component (i.e., it is built from one or more
(*dim*-1)-faces), then the index of each (*dim*-1)-face in this
boundary component will match the index of the corresponding top-
dimensional simplex in the (*dim*-1)-manifold triangulation returned
by build().

Parameter ``index``:
    the index of the desired face, ranging from 0 to size()-1
    inclusive.

Returns:
    the requested face.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::facets
constexpr const char *facets =
R"doc(Returns an object that allows iteration through and random access to
all (*dim*-1)-faces in this boundary component.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. Note that the elements of the list will be
pointers, so your code might look like:

```
for (Face<dim, dim-1>* f : bc.facets()) { ... }
```

The object that is returned will remain valid only for as long as this
boundary component object exists. In particular, the object will
become invalid any time that the triangulation changes (since all
boundary component objects will be destroyed and others rebuilt in
their place). Therefore it is best to treat this object as temporary
only, and to call facets() again each time you need it.

Returns:
    access to the list of all (*dim*-1)-faces.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::index
constexpr const char *index =
R"doc(Returns the index of this boundary component in the underlying
triangulation.

Returns:
    the index of this boundary component.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::isIdeal
constexpr const char *isIdeal =
R"doc(Determines if this boundary component is ideal. This is the case if
and only if it consists of a single ideal vertex and no faces of any
other dimensions.

See the BoundaryComponent class notes for an overview of ideal
boundary components, which can only occur in dimensions ≥ 3, and which
are only recognised where *dim* is one of Regina's standard
dimensions.

Note that a boundary component formed from a single _invalid_ vertex
is _not_ considered to be ideal. This means that, if a boundary
component contains no faces of positive dimension, then one and only
one of isIdeal() and isInvalidVertex() will return ``True``.

.. note::
    If *dim* is not one of Regina's standard dimensions, then real
    boundary components are the only types of boundary component that
    Regina will recognise, which means that this routine will always
    return ``False``.

Returns:
    ``True`` if and only if this boundary component is ideal.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::isInvalidVertex
constexpr const char *isInvalidVertex =
R"doc(Determines if this boundary component consists of a single invalid
vertex and nothing else. In particular, such a boundary component must
contain no faces of any positive dimension.

See the BoundaryComponent class notes for an overview of invalid
vertex boundary components, which can only occur in dimensions ≥ 4,
and which are only recognised where *dim* is one of Regina's standard
dimensions.

An invalid vertex is only placed in its own boundary component if it
does not already belong to some larger boundary component (for
instance, if its link is an ideal (*dim*-1)-manifold triangulation).
This means that, for a boundary component consisting of one or more
(*dim*-1)-faces, this routine will return ``False`` even if the
boundary component also includes one or more invalid vertices.

Note that, if a boundary component contains no faces of positive
dimension, then one and only one of isIdeal() and isInvalidVertex()
will return ``True``.

.. note::
    If *dim* is not one of Regina's standard dimensions, then real
    boundary components are the only types of boundary component that
    Regina will recognise, which means that this routine will always
    return ``False``.

Returns:
    ``True`` if and only if this boundary component consists of a
    single invalid vertex and nothing else.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::isOrientable
constexpr const char *isOrientable =
R"doc(Determines if this boundary component is orientable. If this is an
ideal or invalid vertex boundary component, then the orientability of
the corresponding vertex link is returned.

This routine is fast; in particular, it is pre-computed and does not
build a full triangulation of the boundary component.

Returns:
    ``True`` if and only if this boundary component is orientable.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::isReal
constexpr const char *isReal =
R"doc(Determines if this boundary component is real. This is the case if and
only if it is formed from one or more (dim-1)-faces.

See the BoundaryComponent class notes for an overview of real, ideal,
and invalid vertex boundary components.

.. note::
    If *dim* is not one of Regina's standard dimensions, then real
    boundary components are the only types of boundary component that
    Regina will recognise, which means that this routine will always
    return ``True``.

Returns:
    ``True`` if and only if this boundary component is real.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::pentachora
constexpr const char *pentachora =
R"doc(A dimension-specific alias for faces<4>().

This alias is only available for dimension *dim* = 5.

See faces() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::pentachoron
constexpr const char *pentachoron =
R"doc(A dimension-specific alias for face<4>().

This alias is only available for dimension *dim* = 5.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::size
constexpr const char *size =
R"doc(Returns the number of (*dim*-1)-faces in this boundary component.
These are the top-dimensional faces for a real boundary component.

If this is an ideal or invalid vertex boundary component, then this
routine will return 0.

Returns:
    the number of (*dim*-1)-faces in this boundary component.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::tetrahedra
constexpr const char *tetrahedra =
R"doc(A dimension-specific alias for faces<3>().

This alias is only available for dimension *dim* = 4.

See faces() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::tetrahedron
constexpr const char *tetrahedron =
R"doc(A dimension-specific alias for face<3>().

This alias is only available for dimension *dim* = 4.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::triangle
constexpr const char *triangle =
R"doc(A dimension-specific alias for face<2>().

This alias is available only when *dim* is one of Regina's standard
dimensions and *dim* ≥ 3.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::triangles
constexpr const char *triangles =
R"doc(A dimension-specific alias for faces<2>().

This alias is available only when *dim* is one of Regina's standard
dimensions and *dim* ≥ 3.

See faces() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::triangulation
constexpr const char *triangulation =
R"doc(Returns the triangulation to which this boundary component belongs.

Returns:
    a reference to the triangulation containing this boundary
    component.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::vertex
constexpr const char *vertex =
R"doc(A dimension-specific alias for face<0>().

This alias is available only when *dim* is one of Regina's standard
dimensions.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::BoundaryComponentBase_::vertices
constexpr const char *vertices =
R"doc(A dimension-specific alias for faces<0>().

This alias is available only when *dim* is one of Regina's standard
dimensions.

See faces() for further information.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

