/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct Component {

// Docstring regina::python::doc::Component::__class
static constexpr const char __class[] =
R"doc(A connected component of a *dim*-manifold triangulation.

Components are highly temporary: whenever a triangulation changes, all
of its component objects will be deleted and new ones will be created
in their place.

Components do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Triangulation to which they belong.

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g., Component2 and
    Component3 for dimensions 2 and 3).

Template parameter ``dim``:
    the dimension of the underlying triangulation.)doc";

// Docstring regina::python::doc::Component::boundaryComponent
static constexpr const char boundaryComponent[] =
R"doc(Returns the boundary component at the given index in this component.

Note that the index of a boundary component within this component may
not be the same as its index within the overall triangulation.

Parameter ``index``:
    specifies which boundary component to return; this should be
    between 0 and countBoundaryComponents()-1 inclusive.

Returns:
    the requested boundary component.)doc";

// Docstring regina::python::doc::Component::boundaryComponents
static constexpr const char boundaryComponents[] =
R"doc(Returns an object that allows iteration through and random access to
all boundary components in this component.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be a lightweight view type from
the ``std::ranges`` library, which means it supports range-based
``for`` loops. Note that the elements of the view will be pointers, so
your code might look like:

```
for (BoundaryComponent<dim>* bc : comp.boundaryComponents()) { ... }
```

The object that is returned will remain valid only for as long as this
component object exists. In particular, the object will become invalid
any time that the triangulation changes (since all component objects
will be destroyed and others rebuilt in their place). Therefore it is
best to treat this object as temporary only, and to call
boundaryComponents() again each time you need it.

Returns:
    access to the list of all boundary components.)doc";

// Docstring regina::python::doc::Component::countBoundaryComponents
static constexpr const char countBoundaryComponents[] =
R"doc(Returns the number of boundary components in this component.

Returns:
    the number of boundary components.)doc";

// Docstring regina::python::doc::Component::countBoundaryEdges
static constexpr const char countBoundaryEdges[] =
R"doc(An alias for countBoundaryFacets() in dimension 2.

See countBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Component::countBoundaryFacets
static constexpr const char countBoundaryFacets[] =
R"doc(Returns the number of boundary facets in this component.

A boundary facet is a (*dim*-1)-dimensional facet of a top-dimensional
simplex that is not joined to any adjacent simplex.

This routine runs in constant time (since the result is computed in
advance, when the component is first created).

Returns:
    the total number of boundary facets.)doc";

// Docstring regina::python::doc::Component::countBoundaryTetrahedra
static constexpr const char countBoundaryTetrahedra[] =
R"doc(An alias for countBoundaryFacets() in dimension 4.

See countBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Component::countBoundaryTriangles
static constexpr const char countBoundaryTriangles[] =
R"doc(An alias for countBoundaryFacets() in dimension 3.

See countBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Component::countEdges
static constexpr const char countEdges[] =
R"doc(A dimension-specific alias for countFaces<1>().

See countFaces() for further information.)doc";

// Docstring regina::python::doc::Component::countFaces
static constexpr const char countFaces[] =
R"doc(Returns the number of *subdim*-faces in this component, where the face
dimension does not need to be known until runtime.

For C++ programmers who know *subdim* at compile time, you are better
off using the template function ``countFaces<subdim>()`` instead,
which is (slightly) faster.

For convenience, this routine explicitly supports the case ``subdim ==
dim``. This is _not_ the case for the routines face() and faces(),
which give access to individual faces (the reason relates to the fact
that top-dimensional simplices are built manually, whereas lower-
dimensional faces are deduced properties).

Exception ``InvalidArgument``:
    The face dimension *subdim* is outside the supported range (i.e.,
    negative or greater than *dim*).

Parameter ``subdim``:
    the face dimension; this must be between 0 and *dim* inclusive.

Returns:
    the number of *subdim*-faces.)doc";

// Docstring regina::python::doc::Component::countFacets
static constexpr const char countFacets[] =
R"doc(Returns the number of `(dim-1)`-faces in this component.

This is available (and constant time) for components in all
dimensions, even though the list of `(dim-1)`-faces is only stored for
components in Regina's standard dimensions.

Returns:
    the number of `(dim-1)`-faces.)doc";

// Docstring regina::python::doc::Component::countPentachora
static constexpr const char countPentachora[] =
R"doc(A dimension-specific alias for countFaces<4>().

See countFaces() for further information.)doc";

// Docstring regina::python::doc::Component::countTetrahedra
static constexpr const char countTetrahedra[] =
R"doc(A dimension-specific alias for countFaces<3>().

See countFaces() for further information.)doc";

// Docstring regina::python::doc::Component::countTriangles
static constexpr const char countTriangles[] =
R"doc(A dimension-specific alias for countFaces<2>().

See countFaces() for further information.)doc";

// Docstring regina::python::doc::Component::countVertices
static constexpr const char countVertices[] =
R"doc(A dimension-specific alias for countFaces<0>().

See countFaces() for further information.)doc";

// Docstring regina::python::doc::Component::edge
static constexpr const char edge[] =
R"doc(A dimension-specific alias for face<1>().

See face() for further information.)doc";

// Docstring regina::python::doc::Component::edges
static constexpr const char edges[] =
R"doc(A dimension-specific alias for faces<1>().

See faces() for further information.)doc";

// Docstring regina::python::doc::Component::face
static constexpr const char face[] =
R"doc(Returns the requested *subdim*-face in this component, in a way that
is optimised for Python programmers.

C++ users should not use this routine. The return type must be fixed
at compile time, and so it is typically a ``std::variant`` that could
store a pointer to any class ``Face<dim, ...>``. This means you cannot
access the face directly: you will still need some kind of compile-
time knowledge of *subdim* before you can extract and use an
appropriate ``Face<dim, subdim>`` object from *v*. However, once you
know *subdim* at compile time, you are better off using the (simpler
and faster) routine ``face<subdim>()`` instead.

For Python users, this routine is much more useful: the return type
can be chosen at runtime, and so this routine simply returns a
``Face<dim, subdim>`` object of the appropriate face dimension that
you can use immediately.

The specific return type for C++ programmers will be
``std::variant<Face<dim, 0>*, ..., Face<dim, dim-1>*>``.

Note that the index of a face in the component need not be the index
of the same face in the overall triangulation.

Exception ``InvalidArgument``:
    The face dimension *subdim* is outside the supported range (i.e.,
    negative, or greater than ``dim-1``).

Parameter ``subdim``:
    the face dimension; this must be between 0 and ``dim-1``
    inclusive.

Parameter ``index``:
    the index of the desired face, ranging from 0 to
    countFaces<subdim>()-1 inclusive.

Returns:
    the requested face.)doc";

// Docstring regina::python::doc::Component::faces
static constexpr const char faces[] =
R"doc(Returns an object that allows iteration through and random access to
all *subdim*-faces in this component, in a way that is optimised for
Python programmers.

C++ users should not use this routine. The return type must be fixed
at compile time, and so it is typically a ``std::variant`` that can
hold any of the lightweight view types returned from the templated
``faces<subdim>()`` function. This means that the return value will
still need compile-time knowledge of *subdim* to extract and use the
appropriate face objects. However, once you know *subdim* at compile
time, you are much better off using the (simpler and faster) routine
``faces<subdim>()`` instead.

For Python users, this routine is much more useful: the return type
can be chosen at runtime, and so this routine returns a single
lightweight view granting access to all of the *subdim*-faces of the
component, which you can use immediately.

Exception ``InvalidArgument``:
    The face dimension *subdim* is outside the supported range (i.e.,
    negative, or greater than ``dim-1``).

Parameter ``subdim``:
    the face dimension; this must be between 0 and ``dim-1``
    inclusive.

Returns:
    access to the list of all *subdim*-faces.)doc";

// Docstring regina::python::doc::Component::hasBoundaryEdges
static constexpr const char hasBoundaryEdges[] =
R"doc(An alias for hasBoundaryFacets() in dimension 2.

See hasBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Component::hasBoundaryFacets
static constexpr const char hasBoundaryFacets[] =
R"doc(Determines if this component has any boundary facets.

This routine returns ``True`` if and only if this component contains
some top-dimensional simplex with at least one facet that is not glued
to an adjacent simplex.

Returns:
    ``True`` if and only if this component has boundary facet(s).)doc";

// Docstring regina::python::doc::Component::hasBoundaryTetrahedra
static constexpr const char hasBoundaryTetrahedra[] =
R"doc(An alias for hasBoundaryFacets() in dimension 4.

See hasBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Component::hasBoundaryTriangles
static constexpr const char hasBoundaryTriangles[] =
R"doc(An alias for hasBoundaryFacets() in dimension 3.

See hasBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Component::hasLocks
static constexpr const char hasLocks[] =
R"doc(Identifies whether any top-dimensional simplices in this component
and/or any of their facets are locked.

In short, locking a top-dimensional simplex and/or some of its facets
means that that the simplex and/or facets must not be changed. See
Simplex<dim>::lock() and Simplex<dim>::lockFacet() for full details on
how locks work and what their implications are.

Returns:
    ``True`` if and only if there is at least one locked top-
    dimensional simplex or at least one locked facet of a top-
    dimensional simplex within this component.)doc";

// Docstring regina::python::doc::Component::index
static constexpr const char index[] =
R"doc(Returns the index of this component within the underlying
triangulation.

Returns:
    the index of this component.)doc";

// Docstring regina::python::doc::Component::isClosed
static constexpr const char isClosed[] =
R"doc(Determines if this component is closed. This is the case if and only
if it has no boundary.

Note that ideal components and components with invalid vertices are
not closed (these can only appear in dimensions ≥ 3). See
``Vertex<dim>::isBoundary()`` for details.

Returns:
    ``True`` if and only if this component is closed.)doc";

// Docstring regina::python::doc::Component::isIdeal
static constexpr const char isIdeal[] =
R"doc(Determines if this component is ideal. This is the case if and only if
it contains an ideal vertex as described by
``Vertex<dim>::isIdeal()``.

Returns:
    ``True`` if and only if this component is ideal.)doc";

// Docstring regina::python::doc::Component::isOrientable
static constexpr const char isOrientable[] =
R"doc(Determines if this component is orientable.

This routine runs in constant time (since orientability is determined
in advance, when the component is first created).

Returns:
    ``True`` if and only if this component is orientable.)doc";

// Docstring regina::python::doc::Component::isValid
static constexpr const char isValid[] =
R"doc(Determines if this component is valid.

This uses the same criteria as Triangulation<dim>::isValid(); see the
Triangulation<dim>::isValid() documentation for details.

As with Triangulation<dim>, this tests for bad self-identifications in
all dimensions, but only tests for bad links in Regina's standard
dimensions.

Returns:
    ``True`` if and only if this component is valid.)doc";

// Docstring regina::python::doc::Component::pentachora
static constexpr const char pentachora[] =
R"doc(A dimension-specific alias for faces<4>(), or an alias for simplices()
in dimension *dim* = 4.

See faces() for further information.)doc";

// Docstring regina::python::doc::Component::pentachoron
static constexpr const char pentachoron[] =
R"doc(A dimension-specific alias for face<4>().

See face() for further information.)doc";

// Docstring regina::python::doc::Component::simplex
static constexpr const char simplex[] =
R"doc(Returns the top-dimensional simplex at the given index in this
component.

Note that the index within this component may not be the same as the
index within the overall triangulation.

Parameter ``index``:
    specifies which simplex to return; this value should be between 0
    and size()-1 inclusive.

Returns:
    the *index*th top-dimensional simplex.)doc";

// Docstring regina::python::doc::Component::simplices
static constexpr const char simplices[] =
R"doc(Returns an object that allows iteration through and random access to
all top-dimensional simplices in this component.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be a lightweight view type from
the ``std::ranges`` library, which means it supports range-based
``for`` loops. Note that the elements of the view will be pointers, so
your code might look like:

```
for (Simplex<dim>* s : comp.simplices()) { ... }
```

The object that is returned will remain valid only for as long as this
component object exists. In particular, the object will become invalid
any time that the triangulation changes (since all component objects
will be destroyed and others rebuilt in their place). Therefore it is
best to treat this object as temporary only, and to call simplices()
again each time you need it.

Returns:
    access to the list of all top-dimensional simplices.)doc";

// Docstring regina::python::doc::Component::size
static constexpr const char size[] =
R"doc(Returns the number of top-dimensional simplices in this component.

Returns:
    the number of top-dimensional simplices.)doc";

// Docstring regina::python::doc::Component::tetrahedra
static constexpr const char tetrahedra[] =
R"doc(A dimension-specific alias for faces<3>(), or an alias for simplices()
in dimension *dim* = 3.

See faces() for further information.)doc";

// Docstring regina::python::doc::Component::tetrahedron
static constexpr const char tetrahedron[] =
R"doc(A dimension-specific alias for face<3>().

See face() for further information.)doc";

// Docstring regina::python::doc::Component::triangle
static constexpr const char triangle[] =
R"doc(A dimension-specific alias for face<2>().

See face() for further information.)doc";

// Docstring regina::python::doc::Component::triangles
static constexpr const char triangles[] =
R"doc(A dimension-specific alias for faces<2>(), or an alias for simplices()
in dimension *dim* = 2.

See faces() for further information.)doc";

// Docstring regina::python::doc::Component::triangulation
static constexpr const char triangulation[] =
R"doc(Returns the triangulation to which this component belongs.

Returns:
    a reference to the triangulation containing this component.)doc";

// Docstring regina::python::doc::Component::vertex
static constexpr const char vertex[] =
R"doc(A dimension-specific alias for face<0>().

See face() for further information.)doc";

// Docstring regina::python::doc::Component::vertices
static constexpr const char vertices[] =
R"doc(A dimension-specific alias for faces<0>().

See faces() for further information.)doc";

}; // struct Component

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

