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

struct ComponentBase {

// Docstring regina::python::doc::detail::ComponentBase::__class
static constexpr const char __class[] =
R"doc(Helper class that provides core functionality for a connected
component of a *dim*-manifold triangulation.

Each connected component is represented by the class Component<dim>,
which uses this as a base class. End users should not need to refer to
ComponentBase directly.

See the Component class notes for further information.

Neither this class nor the "end user" class Component<dim> support
value semantics: they cannot be copied, swapped, or manually
constructed. Their memory is managed by the Triangulation class, and
their locations in memory define them. See Component<dim> for further
details.

Python:
    This base class is not present, but the "end user" class
    Component<dim> is.

Template parameter ``dim``:
    the dimension of the underlying triangulation.)doc";

// Docstring regina::python::doc::detail::ComponentBase::boundaryComponent
static constexpr const char boundaryComponent[] =
R"doc(Returns the boundary component at the given index in this component.

Note that the index of a boundary component within this component may
not be the same as its index within the overall triangulation.

Parameter ``index``:
    specifies which boundary component to return; this should be
    between 0 and countBoundaryComponents()-1 inclusive.

Returns:
    the requested boundary component.)doc";

// Docstring regina::python::doc::detail::ComponentBase::boundaryComponents
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

// Docstring regina::python::doc::detail::ComponentBase::countBoundaryComponents
static constexpr const char countBoundaryComponents[] =
R"doc(Returns the number of boundary components in this component.

Returns:
    the number of boundary components.)doc";

// Docstring regina::python::doc::detail::ComponentBase::countBoundaryFacets
static constexpr const char countBoundaryFacets[] =
R"doc(Returns the number of boundary facets in this component.

A boundary facet is a (*dim*-1)-dimensional facet of a top-dimensional
simplex that is not joined to any adjacent simplex.

This routine runs in constant time (since the result is computed in
advance, when the component is first created).

Returns:
    the total number of boundary facets.)doc";

// Docstring regina::python::doc::detail::ComponentBase::countEdges
static constexpr const char countEdges[] =
R"doc(A dimension-specific alias for countFaces<1>().

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::countFacets
static constexpr const char countFacets[] =
R"doc(Returns the number of `(dim-1)`-faces in this component.

This is available (and constant time) for components in all
dimensions, even though the list of `(dim-1)`-faces is only stored for
components in Regina's standard dimensions.

Returns:
    the number of `(dim-1)`-faces.)doc";

// Docstring regina::python::doc::detail::ComponentBase::countPentachora
static constexpr const char countPentachora[] =
R"doc(A dimension-specific alias for countFaces<4>().

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::countTetrahedra
static constexpr const char countTetrahedra[] =
R"doc(A dimension-specific alias for countFaces<3>().

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::countTriangles
static constexpr const char countTriangles[] =
R"doc(A dimension-specific alias for countFaces<2>().

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::countVertices
static constexpr const char countVertices[] =
R"doc(A dimension-specific alias for countFaces<0>().

See countFaces() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::edge
static constexpr const char edge[] =
R"doc(A dimension-specific alias for face<1>().

See face() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::edges
static constexpr const char edges[] =
R"doc(A dimension-specific alias for faces<1>().

See faces() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::hasBoundaryFacets
static constexpr const char hasBoundaryFacets[] =
R"doc(Determines if this component has any boundary facets.

This routine returns ``True`` if and only if this component contains
some top-dimensional simplex with at least one facet that is not glued
to an adjacent simplex.

Returns:
    ``True`` if and only if this component has boundary facet(s).)doc";

// Docstring regina::python::doc::detail::ComponentBase::hasLocks
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

// Docstring regina::python::doc::detail::ComponentBase::index
static constexpr const char index[] =
R"doc(Returns the index of this component within the underlying
triangulation.

Returns:
    the index of this component.)doc";

// Docstring regina::python::doc::detail::ComponentBase::isOrientable
static constexpr const char isOrientable[] =
R"doc(Determines if this component is orientable.

This routine runs in constant time (since orientability is determined
in advance, when the component is first created).

Returns:
    ``True`` if and only if this component is orientable.)doc";

// Docstring regina::python::doc::detail::ComponentBase::isValid
static constexpr const char isValid[] =
R"doc(Determines if this component is valid.

This uses the same criteria as Triangulation<dim>::isValid(); see the
Triangulation<dim>::isValid() documentation for details.

As with Triangulation<dim>, this tests for bad self-identifications in
all dimensions, but only tests for bad links in Regina's standard
dimensions.

Returns:
    ``True`` if and only if this component is valid.)doc";

// Docstring regina::python::doc::detail::ComponentBase::pentachora
static constexpr const char pentachora[] =
R"doc(A dimension-specific alias for faces<4>(), or an alias for simplices()
in dimension *dim* = 4.

See faces() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::pentachoron
static constexpr const char pentachoron[] =
R"doc(A dimension-specific alias for face<4>().

See face() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::simplex
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

// Docstring regina::python::doc::detail::ComponentBase::simplices
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

// Docstring regina::python::doc::detail::ComponentBase::size
static constexpr const char size[] =
R"doc(Returns the number of top-dimensional simplices in this component.

Returns:
    the number of top-dimensional simplices.)doc";

// Docstring regina::python::doc::detail::ComponentBase::tetrahedra
static constexpr const char tetrahedra[] =
R"doc(A dimension-specific alias for faces<3>(), or an alias for simplices()
in dimension *dim* = 3.

See faces() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::tetrahedron
static constexpr const char tetrahedron[] =
R"doc(A dimension-specific alias for face<3>().

See face() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::triangle
static constexpr const char triangle[] =
R"doc(A dimension-specific alias for face<2>().

See face() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::triangles
static constexpr const char triangles[] =
R"doc(A dimension-specific alias for faces<2>(), or an alias for simplices()
in dimension *dim* = 2.

See faces() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::triangulation
static constexpr const char triangulation[] =
R"doc(Returns the triangulation to which this component belongs.

Returns:
    a reference to the triangulation containing this component.)doc";

// Docstring regina::python::doc::detail::ComponentBase::vertex
static constexpr const char vertex[] =
R"doc(A dimension-specific alias for face<0>().

See face() for further information.)doc";

// Docstring regina::python::doc::detail::ComponentBase::vertices
static constexpr const char vertices[] =
R"doc(A dimension-specific alias for faces<0>().

See faces() for further information.)doc";

}; // struct ComponentBase

} // namespace detail

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

