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
constexpr const char *Face =
R"doc(Represents a *subdim*-face in the skeleton of a *dim*-dimensional
triangulation. There are two substantially different cases:

* The case *subdim* < *dim* represents a lower-dimensional face in a
  triangulation. This is implemented by the generic class template (as
  documented here), and is specialised for Regina's standard
  dimensions (as discussed below).

* The case *subdim* = *dim* represents a top-dimensional simplex in a
  triangulation. This class has a very different interface, and is
  implemented by the partial specialisation Face<dim, dim> (and again
  specialised further for standard dimensions). This case is typically
  referred to using the type alias Simplex<dim>, to make the
  distinction clear. See the documentation for the specialisation
  Face<dim, dim> for details on the interface that it provides.

For small-dimensional faces, this class is typically described using
dimension-specific type aliases: Vertex<dim>, Edge<dim>,
Triangle<dim>, Tetrahedron<dim> and Pentachoron<dim> refer to the
cases *subdim* = 0, 1, 2, 3 and 4 respectively.

A given *subdim*-face *F* of the triangulation may appear many times
within the various top-dimensional simplices of the underlying
triangulation. As an extreme example, in a 1-vertex triangulation of a
3-manifold, the single vertex makes 4*n* such appearances, where *n*
is the total number of tetrahedra.

Each such appearance is described by a single FaceEmbedding object.
You can iterate through these appearances using begin() and end(), or
using a range-based ``for`` loop:

```
for (auto& emb : face) { ... }
```

You can count these appearances by calling degree(), and you can also
examine them using routines such as front(), back() and embedding().

.. warning::
    Face objects are highly temporary: whenever a triangulation
    changes, all its face objects will be deleted and new ones will be
    created in their place.

For some types of faces in dimensions *dim* = 3 and 4, this template
is specialised to offer additional functionality. In order to use
these specialised classes, you will need to include the corresponding
triangulation headers (triangulation/dim3.h or triangulation/dim4.h
respectively).

Faces do not support value semantics: they cannot be copied, swapped,
or manually constructed. Their location in memory defines them, and
they are often passed and compared by pointer. End users are never
responsible for their memory management; this is all taken care of by
the Triangulation to which they belong.

Python:
    Python does not support templates. Instead this class can be used
    by appending dimensions *dim* and *subdim* as suffices (e.g.,
    Face2_1 and Face3_0 for the two examples above).

Template parameter ``dim``:
    the dimension of the underlying triangulation. This must be
    between 2 and 15 inclusive.

Template parameter ``subdim``:
    the dimension of the faces that this class represents. This must
    be between 0 and *dim* inclusive.)doc";

// Docstring regina::python::doc::FaceEmbedding
constexpr const char *FaceEmbedding =
R"doc(Details how a *subdim*-face of a *dim*-dimensional triangulation
appears within each top-dimensional simplex.

For small-dimensional faces, this class is typically described using
dimension-specific type aliases: VertexEmbedding<dim>,
EdgeEmbedding<dim>, TriangleEmbedding<dim>, TetrahedronEmbedding<dim>
and PentachoronEmbedding<dim> refer to the cases *subdim* = 0, 1, 2, 3
and 4 respectively.

For a *dim*-dimensional triangulation *T*, each *subdim*-face *F*
typically belongs to many top-dimensional simplices of *T*, and
therefore has many associated FaceEmbedding objects. These individual
FaceEmbedding objects correspond to the top-dimensional simplices of
the link of *F* (which is a (*dim* - *subdim* - 1)-dimensional
triangulation).

As of Regina 7.0, a FaceEmbedding can happily outlive its face - even
if the underlying Face object is destroyed (e.g., because the
triangulation changed), if you made a local copy of a FaceEmbedding
beforehand then its simplex(), face() and vertices() routines will
continue to return the same values as they did before, back when the
underlying Face still existed. A FaceEmbedding cannot, however,
outlive its top-dimensional simplex, because internally a
FaceEmbedding references the Simplex object in which it lives (i.e.,
it does not just store an integer simplex index).

If *dim* is one of Regina's standard dimensions, then this template is
specialised to offer additional dimension-specific aliases. In order
to use these specialised classes, you will need to include the
corresponding triangulation headers (e.g., triangulation/dim2.h for
*dim* = 2, or triangulation/dim3.h for *dim* = 3).

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Python:
    Python does not support templates. Instead this class can be used
    by appending dimensions *dim* and *subdim* as suffices (e.g.,
    FaceEmbedding2_1 and FaceEmbedding3_0 for the two examples above).

Template parameter ``dim``:
    the dimension of the underlying triangulation. This must be
    between 2 and 15 inclusive.

Template parameter ``subdim``:
    the dimension of the faces of the underlying triangulation. This
    must be between 0 and *dim*-1 inclusive.)doc";

namespace FaceEmbedding_ {

// Docstring regina::python::doc::FaceEmbedding_::__copy
constexpr const char *__copy =
R"doc(Creates a new copy of the given object.

Parameter ``cloneMe``:
    the object to copy.)doc";

// Docstring regina::python::doc::FaceEmbedding_::__init
constexpr const char *__init =
R"doc(Creates a new object containing the given data.

Parameter ``simplex``:
    the top-dimensional simplex in which the underlying *subdim*-face
    of the triangulation is contained.

Parameter ``vertices``:
    a mapping from the vertices of the underlying *subdim*-face of the
    triangulation to the corresponding vertex numbers of *simplex*.
    See FaceEmbeddingBase::vertices() for details of how this
    permutation should be structured.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

