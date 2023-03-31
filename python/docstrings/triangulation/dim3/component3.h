/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Component
static const char *Component =
R"doc(Represents a connected component of a 3-manifold triangulation.

This is a specialisation of the generic Component class template; see
the generic Component documentation for an overview of how the
component classes work. In Python, you can read this generic
documentation by looking at a higher dimension: try
``help(Component5)``.

This 3-dimensional specialisation contains some extra functionality.
In particular, each 3-dimensional component also stores details on
lower-dimensional faces (i.e., vertices, edges and triangles).

Components do not support value semantics: they cannot be copied,
swapped, or manually constructed. Their location in memory defines
them, and they are often passed and compared by pointer. End users are
never responsible for their memory management; this is all taken care
of by the Triangulation to which they belong.)doc";

namespace Component_ {

// Docstring regina::python::doc::Component_::countBoundaryTriangles
static const char *countBoundaryTriangles =
R"doc(A dimension-specific alias for countBoundaryFacets().

See countBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Component_::countFaces
static const char *countFaces =
R"doc(Returns the number of *subdim*-faces in this component.

For convenience, this routine explicitly supports the case *subdim* =
3. This is _not_ the case for the routines face() and faces(), which
give access to individual faces (the reason relates to the fact that
tetrahedra are built manually, whereas lower-dimensional faces are
deduced properties).

Python:
    Python does not support templates. Instead, Python users should
    call this function in the form ``countFaces(subdim)``; that is,
    the template parameter *subdim* becomes the first argument of the
    function.

Template parameter ``subdim``:
    the face dimension; this must be between 0 and 3 inclusive.

Returns:
    the number of *subdim*-faces.)doc";

// Docstring regina::python::doc::Component_::face
static const char *face =
R"doc(Returns the requested *subdim*-face in this component.

Note that the index of a face in the component need not be the index
of the same face in the overall triangulation.

Python:
    Python does not support templates. Instead, Python users should
    call this function in the form ``face(subdim, index)``; that is,
    the template parameter *subdim* becomes the first argument of the
    function.

Template parameter ``subdim``:
    the face dimension; this must be between 0 and 2 inclusive.

Parameter ``index``:
    the index of the desired face, ranging from 0 to
    countFaces<subdim>()-1 inclusive.

Returns:
    the requested face.)doc";

// Docstring regina::python::doc::Component_::faces
static const char *faces =
R"doc(Returns an object that allows iteration through and random access to
all *subdim*-faces in this component.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. Note that the elements of the list will be
pointers, so your code might look like:

```
for (Face<dim, subdim>* f : comp.faces<subdim>()) { ... }
```

The object that is returned will remain valid only for as long as this
component object exists. In particular, the object will become invalid
any time that the triangulation changes (since all component objects
will be destroyed and others rebuilt in their place). Therefore it is
best to treat this object as temporary only, and to call faces() again
each time you need it.

Python:
    Python does not support templates. Instead, Python users should
    call this function in the form ``faces(subdim)``.

Template parameter ``subdim``:
    the face dimension; this must be between 0 and 2 inclusive.

Returns:
    access to the list of all *subdim*-faces.)doc";

// Docstring regina::python::doc::Component_::hasBoundaryTriangles
static const char *hasBoundaryTriangles =
R"doc(A dimension-specific alias for hasBoundaryFacets().

See hasBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Component_::isClosed
static const char *isClosed =
R"doc(Determines if this component is closed. This is the case if and only
if it has no boundary. Note that ideal components are not closed.

Returns:
    ``True`` if and only if this component is closed.)doc";

// Docstring regina::python::doc::Component_::isIdeal
static const char *isIdeal =
R"doc(Determines if this component is ideal. This is the case if and only if
it contains an ideal vertex as described by Vertex<3>::isIdeal().

Returns:
    ``True`` if and only if this component is ideal.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

