/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct Component3 {

// Docstring regina::python::doc::Component3::__class
static constexpr const char __class[] =
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
of by the Triangulation to which they belong.

Python:
    This class is available to Python users under the name Component3.)doc";

// Docstring regina::python::doc::Component3::countBoundaryTriangles
static constexpr const char countBoundaryTriangles[] =
R"doc(A dimension-specific alias for countBoundaryFacets().

See countBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Component3::countFaces
static constexpr const char countFaces[] =
R"doc(Returns the number of *subdim*-faces in this component, where the face
dimension does not need to be known until runtime.

For C++ programmers who know *subdim* at compile time, you are better
off using the template function ``countFaces<subdim>()`` instead,
which is (slightly) faster.

For convenience, this routine explicitly supports the case *subdim* =
3. This is _not_ the case for the routines face() and faces(), which
give access to individual faces (the reason relates to the fact that
tetrahedra are built manually, whereas lower-dimensional faces are
deduced properties).

Exception ``InvalidArgument``:
    The face dimension *subdim* is outside the supported range (i.e.,
    negative or greater than 3).

Parameter ``subdim``:
    the face dimension; this must be between 0 and 3 inclusive.

Returns:
    the number of *subdim*-faces.)doc";

// Docstring regina::python::doc::Component3::face
static constexpr const char face[] =
R"doc(Returns the requested *subdim*-face in this component, in a way that
is optimised for Python programmers.

C++ users should not use this routine. The return type must be fixed
at compile time, and so it is typically a ``std::variant`` that could
store a pointer to any class ``Face<3, ...>``. This means you cannot
access the face directly: you will still need some kind of compile-
time knowledge of *subdim* before you can extract and use an
appropriate ``Face<3, subdim>`` object from *v*. However, once you
know *subdim* at compile time, you are better off using the (simpler
and faster) routine ``face<subdim>()`` instead.

For Python users, this routine is much more useful: the return type
can be chosen at runtime, and so this routine simply returns a
``Face<3, subdim>`` object of the appropriate face dimension that you
can use immediately.

The specific return type for C++ programmers will be
``std::variant<Face<3, 0>*, Face<3, 1>*, Face<3, 2>*>``.

Note that the index of a face in the component need not be the index
of the same face in the overall triangulation.

Exception ``InvalidArgument``:
    The face dimension *subdim* is outside the supported range (i.e.,
    negative, or greater than 2).

Parameter ``subdim``:
    the face dimension; this must be between 0 and 2 inclusive.

Parameter ``index``:
    the index of the desired face, ranging from 0 to
    countFaces<subdim>()-1 inclusive.

Returns:
    the requested face.)doc";

// Docstring regina::python::doc::Component3::faces
static constexpr const char faces[] =
R"doc(Returns an object that allows iteration through and random access to
all *subdim*-faces in this component, in a way that is optimised for
Python programmers.

C++ users should not use this routine. The return type must be fixed
at compile time, and so it is typically a ``std::variant`` that can
hold any of the lightweight rview types returned from the templated
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
    negative, or greater than 2).

Parameter ``subdim``:
    the face dimension; this must be between 0 and 2 inclusive.

Returns:
    access to the list of all *subdim*-faces.)doc";

// Docstring regina::python::doc::Component3::hasBoundaryTriangles
static constexpr const char hasBoundaryTriangles[] =
R"doc(A dimension-specific alias for hasBoundaryFacets().

See hasBoundaryFacets() for further information.)doc";

// Docstring regina::python::doc::Component3::isClosed
static constexpr const char isClosed[] =
R"doc(Determines if this component is closed. This is the case if and only
if it has no boundary. Note that ideal components are not closed.

Returns:
    ``True`` if and only if this component is closed.)doc";

// Docstring regina::python::doc::Component3::isIdeal
static constexpr const char isIdeal[] =
R"doc(Determines if this component is ideal. This is the case if and only if
it contains an ideal vertex as described by Vertex<3>::isIdeal().

Returns:
    ``True`` if and only if this component is ideal.)doc";

}; // struct Component3

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

