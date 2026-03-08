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

* For low-dimensional faces (``subdim < dim / 2``), faces are numbered
  in lexicographical order according to their vertices. For example,
  in a 3-dimensional triangulation, edges 0,...,5 contain vertices 01,
  02, 03, 12, 13, 23 respectively.

* For high-dimensional faces (``subdim ≥ dim / 2``), faces are
  numbered in _reverse_ lexicographical order according to their
  vertices. For example, in a 3-dimensional triangulation, triangles
  0,...,3 contain vertices 123, 023, 013, 012 respectively.

* As a consequence, unless ``subdim = (dim-1)/2``, we always have
  *subdim*-face number *i* opposite (*dim*-1-*subdim*)-face number
  *i*. For the special "halfway case" ``subdim = (dim-1)/2``, where
  each *subdim*-face is opposite another *subdim*-face, we always have
  *subdim*-face number *i* opposite *subdim*-face number
  ``(nFaces-1-i)``.

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
    the dimension of the simplex whose faces we are numbering. Note
    that dimension 1 _is_ supported for the purpose of face numbering,
    even though it is not supported for building fully-fledged
    triangulations.

Template parameter ``subdim``:
    the dimension of the faces that we are numbering.)doc";

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
    the dimension of simplex that we are working with.

Parameter ``i``:
    the first vertex of an edge in a *dim*-dimensional simplex. This
    must be between 0 and *dim* inclusive.

Parameter ``j``:
    the second vertex of an edge in a *dim*-dimensional simplex. This
    must be between 0 and *dim* inclusive, and must be different from
    *i*.

Returns:
    the number of the (*dim*-2)-face opposite the given edge.)doc";

namespace FaceNumbering_ {

// Docstring regina::python::doc::FaceNumbering_::containsVertex
static const char *containsVertex =
R"doc(Tests whether the given *subdim*-face of a *dim*-dimensional simplex
contains the given vertex of the simplex.

Parameter ``face``:
    a *subdim*-face number in a *dim*-simplex; this must be between 0
    and (*dim*+1 choose *subdim*+1)-1 inclusive.

Parameter ``vertex``:
    a vertex number in a *dim*-simplex; this must be between 0 and
    *dim* inclusive.

Returns:
    ``True`` if and only if the given *subdim*-face contains the given
    vertex.)doc";

// Docstring regina::python::doc::FaceNumbering_::faceNumber
static const char *faceNumber =
R"doc(Identifies which *subdim*-face in a *dim*-dimensional simplex is
represented by the first (*subdim* + 1) elements of the given
permutation.

In other words, this routine identifies which *subdim*-face number
within a *dim*-dimensional simplex spans vertices ``vertices[0, ...,
subdim]``.

For the special case ``subdim == 1`` (i.e., _edges_ in a
*dim*-dimensional simplex), you can also call this function in the
form ``faceNumber(u, v)``, where *u* and *v* are two distinct vertex
numbers in the range `0 ≤ u,v ≤ dim`: this will return the number of
the edge spanned by simplex vertices *u* and *v*. It does not matter
whether *u* is smaller or larger than *v*.

Parameter ``vertices``:
    a permutation whose first (*subdim* + 1) elements represent some
    vertex numbers in a *dim*-simplex.

Returns:
    the corresponding *subdim*-face number in the *dim*-simplex. This
    will be between 0 and (*dim*+1 choose *subdim*+1)-1 inclusive.)doc";

// Docstring regina::python::doc::FaceNumbering_::faceNumber_2
static const char *faceNumber_2 =
R"doc(Identifies which edge in a *dim*-dimensional simplex joins the two
given vertices of the simplex.

This is essentially a slicker implementation of ``faceNumber(Perm<dim
+ 1>)``, specifically for edge numbering, that does not require an
entire permutation to be passed as input.

The two given vertex numbers must be distinct, and may appear in
either order (i.e., they do not need to be sorted).

Parameter ``vertex0``:
    some vertex number of a simplex; this must be between 0 and *dim*
    inclusive.

Parameter ``vertex1``:
    another vertex number of a simplex; this must also be between 0
    and *dim* inclusive, and must be different from *vertex0*.

Returns:
    the number of the simplex edge spanned by the two given vertices.
    This will be between 0 and ``(dim+1 choose 2)-1`` inclusive.)doc";

// Docstring regina::python::doc::FaceNumbering_::ordering
static const char *ordering =
R"doc(Given a *subdim*-face number within a *dim*-dimensional simplex,
returns the corresponding canonical ordering of the simplex vertices.

If this canonical ordering is *c*, then *c*[0,...,*subdim*] will be
the vertices of the given face in increasing numerical order. That is,
*c*[0] < ... < *c*[*subdim*]. The remaining images *c*[(*subdim* +
1),...,*dim*] will be ordered arbitrarily.

Note that this is _not_ the same permutation as returned by
Simplex<dim>::faceMapping<subdim>():

* ordering() is a static function, which returns the same permutation
  for the same face number, regardless of which *dim*-simplex we are
  looking at. The images of 0,...,*subdim* will always appear in
  increasing order, and the images of (*subdim* + 1),...,*dim* will be
  arbitrary.

* faceMapping() examines the underlying face *F* of the triangulation
  and, across all appearances of *F* in different *dim*-simplices: (i)
  chooses the images of 0,...,*subdim* to map to the same respective
  vertices of *F*; and (ii) chooses the images of (*subdim* +
  1),...,*dim* to maintain a "consistent orientation" constraint.

Parameter ``face``:
    identifies which *subdim*-face of a *dim*-dimensional simplex to
    query. This must be between 0 and (*dim*+1 choose *subdim*+1)-1
    inclusive.

Returns:
    the corresponding canonical ordering of the simplex vertices.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

