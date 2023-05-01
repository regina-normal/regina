/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::FaceNumbering
static const char *FaceNumbering =
R"doc(Specifies how *subdim*-faces are numbered within a *dim*-dimensional
simplex.

Regina uses the following general scheme for numbering faces:

* For low-dimensional faces (*subdim* < *dim* / 2), faces are numbered
  in lexicographical order according to their vertices. For example,
  in a 3-dimensional triangulation, edges 0,...,5 contain vertices 01,
  02, 03, 12, 13, 23 respectively.

* For high-dimensional faces (*subdim* ≥ *dim* / 2), faces are
  numbered in _reverse_ lexicographical order according to their
  vertices. For example, in a 3-dimensional triangulation, triangles
  0,...,3 contain vertices 123, 023, 013, 012 respectively.

* As a consequence, unless *subdim* = (*dim*-1)/2, we always have
  *subdim*-face number *i* opposite (*dim*-1-*subdim*)-face number
  *i*. For the special "halfway case" *subdim* = (*dim*-1)/2, where
  each *subdim*-face is opposite another *subdim*-face, we always have
  *subdim*-face number *i* opposite *subdim*-face number
  (*nFaces*-1-*i*).

Every class Face<dim, subdim> inherits from this class, which means
you can access these routines as Face<dim, subdim>::ordering(),
Face<dim, subdim>::faceNumber(), and so on.

An advantage of referring to FaceNumbering<dim, subdim> directly (as
opposed to Face<dim, subdim>) is that its header is lightweight: it
does not pull in the large and complex headers required by Face<dim,
subdim>.

This class is specialised (and optimised) in Regina's standard
dimensions.

Python:
    This class is not available in Python. However, all of its
    routines can be accessed through Face<dim, subdim> (which in
    Python becomes Face*dim*_*subdim*, or one of the type aliases such
    as Vertex3, Edge2 and so on).

Template parameter ``dim``:
    the dimension of the simplex whose faces we are numbering. This
    must be between 1 and 15 inclusive.

Template parameter ``subdim``:
    the dimension of the faces that we are numbering. This must be
    between 0 and *dim*-1 inclusive.)doc";

// Docstring regina::python::doc::faceOppositeEdge
static const char *faceOppositeEdge =
R"doc(Returns the (*dim*-2)-face number that is opposite the edge joining
vertices *i* and *j* in a *dim*-dimensional simplex.

This function is offered because its implementation is faster than
working through the FaceNumbering class.

The arguments *i* and *j* do not need to appear in ascending order.

Python:
    Python does not support templates. Instead, Python users should
    call this function in the form ``faceOppositeEdge(dim, i, j)``;
    that is, the template parameter *dim* becomes the first argument
    of the function.

Template parameter ``dim``:
    the dimension of simplex that we are working with. This must be
    between 2 and 15 inclusive.

Parameter ``i``:
    the first vertex of an edge in a *dim*-dimensional simplex. This
    must be between 0 and *dim* inclusive.

Parameter ``j``:
    the second vertex of an edge in a *dim*-dimensional simplex. This
    must be between 0 and *dim* inclusive, and must be different from
    *i*.

Returns:
    the number of the (*dim*-2)-face opposite the given edge.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

