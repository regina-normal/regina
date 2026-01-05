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

// Docstring regina::python::doc::detail::FaceNumberingAPI
constexpr const char *FaceNumberingAPI =
R"doc(Placeholder class that outlines the functions provided by
FaceNumbering<dim, subdim>. This class exists mainly to help with
documentation.

The class FaceNumbering<dim, subdim> details how the *subdim*-faces of
a *dim*-dimensional simplex are numbered. Its implementation involves
many template specialisations, and so this base class FaceNumberingAPI
serves to put all the documentation and class constants in one place.

End users should access the numbering scheme through either
FaceNumbering<dim, subdim> or its child class Face<dim, subdim>. There
is no need to refer to FaceNumberingAPI directly.

The member functions described here are never implemented in the base
class FaceNumberingAPI; instead they are redeclared and implemented in
the various specialisations of the child class FaceNumberingImpl.

See the FaceNumbering template class notes for further information,
including details of how the face numbering scheme works.

Python:
    This base class is not present, and neither is FaceNumbering<dim,
    subdim>. Python users can access these routines through the class
    Face<dim, subdim> (which in Python becomes Face*dim*_*subdim*, or
    one of the type aliases such as Vertex3, Edge2 and so on).

Template parameter ``dim``:
    the dimension of the simplex whose faces are being numbered. Note
    that dimension 1 _is_ supported for the purpose of face numbering,
    even though it is not supported for building fully-fledged
    triangulations.

Template parameter ``subdim``:
    the dimension of the faces being numbered.)doc";

// Docstring regina::python::doc::detail::FaceNumberingImpl
constexpr const char *FaceNumberingImpl =
R"doc(Implementation details for numbering *subdim*-faces of a
*dim*-dimensional simplex.

This numbering scheme can be accessed via FaceNumbering<dim, subdim>
or Face<dim, subdim>, both of which use this as a base class. End
users should not need to refer to FaceNumberingImpl directly.

See the FaceNumbering template class notes for further information,
including details of how the face numbering scheme works.

Python:
    This base class is not present, and neither is FaceNumbering<dim,
    subdim>. Python users can access these routines through the class
    Face<dim, subdim> (which in Python becomes Face*dim*_*subdim*, or
    one of the type aliases such as Vertex3, Edge2 and so on).

Template parameter ``dim``:
    the dimension of the simplex whose faces are being numbered.

Template parameter ``subdim``:
    the dimension of the faces being numbered.

Template parameter ``codim``:
    the codimension ``(dim-subdim-1)`` of the faces being numbered.
    Ideally this would be specified directly as ``dim-subdim-1`` in
    the partial template specialisation, and this _should_ be legal
    according to CWG1315; however, it fails to build under some
    versions of gcc (e.g., 10.2.0).)doc";

}

namespace detail::FaceNumberingAPI_ {

// Docstring regina::python::doc::detail::FaceNumberingAPI_::containsVertex
constexpr const char *containsVertex =
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

// Docstring regina::python::doc::detail::FaceNumberingAPI_::faceNumber
constexpr const char *faceNumber =
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

// Docstring regina::python::doc::detail::FaceNumberingAPI_::ordering
constexpr const char *ordering =
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

