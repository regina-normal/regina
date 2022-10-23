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

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

