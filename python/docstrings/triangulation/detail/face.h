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

// Docstring regina::python::doc::detail::FaceBase
constexpr const char *FaceBase =
R"doc(Helper class that provides core functionality for a *subdim*-face in
the skeleton of a *dim*-dimensional triangulation.

Each *subdim*-face is described by a Face<dim, subdim> object, which
uses this as a base class. End users should not need to refer to
FaceBase directly.

See the Face template class notes for further information.

Neither this class nor the "end user" class Face<dim> support value
semantics: they cannot be copied, swapped, or manually constructed.
Their memory is managed by the Triangulation class, and their
locations in memory define them. See Face<dim> for further details.

Python:
    This base class is not present, but the "end user" class Face<dim,
    subdim> is.

Template parameter ``dim``:
    the dimension of the underlying triangulation. This must be
    between 2 and 15 inclusive.

Template parameter ``subdim``:
    the dimension of the faces that this class represents. This must
    be between 0 and *dim*-1 inclusive.)doc";

// Docstring regina::python::doc::detail::FaceEmbeddingBase
constexpr const char *FaceEmbeddingBase =
R"doc(Helper class that provides core functionality for describing how a
*subdim*-face of a *dim*-dimensional triangulation appears within each
top-dimensional simplex.

Each such appearance is described by a FaceEmbedding<dim, subdim>
object, which uses this as a base class. End users should not need to
refer to FaceEmbeddingBase directly.

See the FaceEmbedding template class notes for further information.

Python:
    This base class is not present, but the "end user" class
    FaceEmbedding<dim, subdim> is.

Template parameter ``dim``:
    the dimension of the underlying triangulation. This must be
    between 2 and 15 inclusive.

Template parameter ``subdim``:
    the dimension of the faces of the underlying triangulation. This
    must be between 0 and *dim*-1 inclusive.)doc";

}

namespace detail::FaceBase_ {

// Docstring regina::python::doc::detail::FaceBase_::__iter__
constexpr const char *__iter__ =
R"doc(Provides Python support for iterating through all appearances of this
face within the various top-dimensional simplices of the underlying
triangulation.

In most cases, the ordering of appearances is arbitrary. The exception
is for codimension 2, where these appearances are ordered in a way
that follows the link around the face (which in codimension 2 is
always a path or a cycle).

Iterating over this face will run through degree() appearances in
total. This is also equivalent to iterating over embeddings():
iterating directly over a face generates a tiny bit less overhead, but
you may also find it to be less readable. In particular, the following
three blocks of code are all equivalent:

```
for emb in face:
    ...
```

```
for emb in face.embeddings():
    ...
```

```
for i in range(face.degree()):
    emb = face.embedding(i)
    ...
```

Returns:
    an iterator over all the appearances of this face.)doc";

// Docstring regina::python::doc::detail::FaceBase_::back
constexpr const char *back =
R"doc(Returns the last appearance of this face within a top-dimensional
simplex of the underlying triangluation.

This is equivalent to calling ``embedding(degree()-1)``.

In most cases, the ordering of appearances is arbitrary. The exception
is for codimension 2, where the appearances of a face are ordered in a
way that follows the link around the face (which in codimension 2 is
always a path or a cycle). In particular, for a boundary face of
codimension 2, both front() and back() will refer to the two
appearances of this face on the (*dim*-1)-dimensional boundary.

Returns:
    details of the last appearance.)doc";

// Docstring regina::python::doc::detail::FaceBase_::boundaryComponent
constexpr const char *boundaryComponent =
R"doc(Returns the boundary component of the triangulation to which this face
belongs.

See the note in the BoundaryComponent overview regarding what happens
if the link of the face itself has more than one boundary component.
Note that such a link makes both the face and the underlying
triangulation invalid.

For dimensions in which ideal and/or invalid vertices are both
possible and recognised: an ideal vertex will have its own individual
boundary component to which it belongs, and so will an invalid vertex
boundary component if the invalid vertex does not already belong to
some real boundary component.

Returns:
    the boundary component containing this face, or ``None`` if this
    face does not lie entirely within the boundary of the
    triangulation.)doc";

// Docstring regina::python::doc::detail::FaceBase_::component
constexpr const char *component =
R"doc(Returns the component of the triangulation to which this face belongs.

Returns:
    the component containing this face.)doc";

// Docstring regina::python::doc::detail::FaceBase_::degree
constexpr const char *degree =
R"doc(Returns the degree of this face. This is the number of different ways
in which the face appears within the various top-dimensional simplices
of the underlying triangulation.

Note that if this face appears multiple times within the same top-
dimensional simplex, then it will be counted multiple times by this
routine.

Returns:
    the degree of this face.)doc";

// Docstring regina::python::doc::detail::FaceBase_::edge
constexpr const char *edge =
R"doc(A dimension-specific alias for face<1>().

This alias is available for all facial dimensions *subdim*.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase_::edgeMapping
constexpr const char *edgeMapping =
R"doc(A dimension-specific alias for faceMapping<1>().

This alias is available for all facial dimensions *subdim*.

See faceMapping() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase_::embedding
constexpr const char *embedding =
R"doc(Returns one of the ways in which this face appears within a top-
dimensional simplex of the underlying triangluation.

For convenience, you can also use begin() and end() to iterate through
all such appearances, or equivalently you can iterate over the
lightweight object returned by embeddings().

In most cases, the ordering of appearances is arbitrary. The exception
is for codimension 2, where these appearances are ordered in a way
that follows the link around the face (which in codimension 2 is
always a path or a cycle).

Parameter ``index``:
    the index of the requested appearance. This must be between 0 and
    degree()-1 inclusive.

Returns:
    details of the requested appearance.)doc";

// Docstring regina::python::doc::detail::FaceBase_::embeddings
constexpr const char *embeddings =
R"doc(Returns an object that allows iteration through and random access to
all of the ways in which this face appears within a top-dimensional
simplex of the underlying triangluation.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be an instance of ListView, which
means it offers basic container-like functions and supports range-
based ``for`` loops. The elements of the list will be read-only
objects of type FaceEmbedding<dim, subdim>. For example, your code
might look like:

```
Face<dim, subdim>* face = ...;
for (const auto& emb : face->embeddings()) { ... }
```

In most cases, the ordering of appearances is arbitrary. The exception
is for codimension 2, where these appearances are ordered in a way
that follows the link around the face (which in codimension 2 is
always a path or a cycle).

Using embeddings() is equivalent to iterating over the face itself. It
generates a tiny amount of extra overhead, but also may be considered
more readable. In particular, the code above is equivalent to both of
the following alternatives:

```
Face<dim, subdim>* face = ...;
for (const auto& emb : *face) { ... }
```

```
Face<dim, subdim>* face = ...;
for (size_t i = 0; i < face->degree(); ++i) {
    const auto& emb = face->embedding(i);
    ...
}
```

Returns:
    access to the list of all appearances of this face within a top-
    dimensional simplex of the underlying triangulation.)doc";

// Docstring regina::python::doc::detail::FaceBase_::face
constexpr const char *face =
R"doc(Returns the *lowerdim*-face of the underlying triangulation that
appears as the given *lowerdim*-dimensional subface of this face.

The argument *face* must represent a *lowerdim*-face number within a
*subdim*-simplex. This *lowerdim*-face number will be interpreted with
respect to the inherent labelling (0, ..., *subdim*) of the vertices
of this *subdim*-face. See FaceEmbedding<dim, subdim>::vertices() for
details on how these map to the vertex numbers of the
*dim*-dimensional simplices that contain this face in the overall
triangulation.

See FaceNumbering<subdim, lowerdim> for the conventions of how
*lowerdim*-faces are numbered within a *subdim*-simplex.

Precondition:
    The dimension of this face (*subdim*) is strictly positive (i.e.,
    this face is not a vertex). Note that, without this constraint,
    there are no possible values for the template parameter
    *lowerdim*.

Python:
    Python does not support templates. Instead, Python users should
    call this function in the form ``face(lowerdim, face)``; that is,
    the template parameter *lowerdim* becomes the first argument of
    the function.

Template parameter ``lowerdim``:
    the dimension of subface to examine. This must be between 0 and
    (*subdim* - 1) inclusive.

Parameter ``face``:
    the *lowerdim*-face of this *subdim*-face to examine. This should
    be between 0 and (*subdim*+1 choose *lowerdim*+1)-1 inclusive.

Returns:
    the corresponding *lowerdim*-face of the triangulation.)doc";

// Docstring regina::python::doc::detail::FaceBase_::faceMapping
constexpr const char *faceMapping =
R"doc(Examines the given *lowerdim*-dimensional subface of this face, and
returns the mapping between the underlying *lowerdim*-face of the
triangulation and the individual vertices of this face.

The argument *face* must represent a *lowerdim*-face number within a
*subdim*-simplex. This *lowerdim*-face number will be interpreted with
respect to the inherent labelling (0, ..., *subdim*) of the vertices
of this *subdim*-face. See FaceEmbedding<dim, subdim>::vertices() for
details on how these map to the vertex numbers of the
*dim*-dimensional simplices that contain this face in the overall
triangulation.

Let *F* denote this *subdim*-face of the triangulation, and let *L*
denote the *lowerdim*-face of the triangulation that corresponds to
the given subface of *F*. Then the permutation returned by this
routine maps the vertex numbers (0, ..., *lowerdim*) of *L* to the
corresponding vertex numbers of *F*. This is with respect to the
inherent labellings (0, ..., *lowerdim*) and (0, ..., *subdim*) of the
vertices of *L* and *F* respectively.

In particular, if this routine returns the permutation *p*, then the
images *p*[0,...,*lowerdim*] will be some permutation of the vertices
Face<subdim, lowerdim>::ordering(face)[0,...,*lowerdim*].

This routine differs from Simplex<dim>::faceMapping<lowerdim>() in how
it handles the images of (*lowerdim*+1, ..., *dim*):

* This routine will map (*lowerdim*+1, ..., *subdim*) to the remaining
  vertices of this face in an arbitrary order, and will map
  (*subdim*+1, ..., *dim*) to (*subdim*+1, ..., *dim*) again in an
  arbitrary order.

* In contrast, Simplex<dim>::faceMapping<lowerdim>() chooses the
  images of (*lowerdim*+1, ..., *dim*) to satisfy an additional
  orientability constraint.

See FaceNumbering<subdim, lowerdim> for the conventions of how
*lowerdim*-faces are numbered within a *subdim*-simplex.

Precondition:
    The dimension of this face (*subdim*) is strictly positive (i.e.,
    this face is not a vertex). Note that, without this constraint,
    there are no possible values for the template parameter
    *lowerdim*.

Python:
    Python does not support templates. Instead, Python users should
    call this function in the form ``faceMapping(lowerdim, face)``;
    that is, the template parameter *lowerdim* becomes the first
    argument of the function.

Template parameter ``lowerdim``:
    the dimension of subface to examine. This must be between 0 and
    (*subdim* - 1) inclusive.

Parameter ``face``:
    the *lowerdim*-face of this *subdim*-face to examine. This should
    be between 0 and (*subdim*+1 choose *lowerdim*+1)-1 inclusive.

Returns:
    a mapping from the vertices of the underlying *lowerdim*-face of
    the triangulation to the vertices of this *subdim*-face.)doc";

// Docstring regina::python::doc::detail::FaceBase_::front
constexpr const char *front =
R"doc(Returns the first appearance of this face within a top-dimensional
simplex of the underlying triangluation.

This is equivalent to calling ``*begin()``, or ``embedding(0)``.

In most cases, the ordering of appearances is arbitrary. The exception
is for codimension 2, where the appearances of a face are ordered in a
way that follows the link around the face (which in codimension 2 is
always a path or a cycle). In particular, for a boundary face of
codimension 2, both front() and back() will refer to the two
appearances of this face on the (*dim*-1)-dimensional boundary.

Returns:
    details of the first appearance.)doc";

// Docstring regina::python::doc::detail::FaceBase_::hasBadIdentification
constexpr const char *hasBadIdentification =
R"doc(Determines if this face is identified with itself under a non-identity
permutation. For example, if this face is an edge then this routine
tests whether the edge is identified with itself in reverse.

Such a face will always be marked as invalid. Note that, for standard
dimensions *dim*, there are other types of invalid faces also. See
isValid() for a full discussion of what it means for a face to be
valid.

Returns:
    ``True`` if and only if this face is identified with itself under
    a non-identity permutation.)doc";

// Docstring regina::python::doc::detail::FaceBase_::hasBadLink
constexpr const char *hasBadLink =
R"doc(Determines if this face does not have an appropriate link. See
condition (2) in the documentation for isValid() for a full
description of what "appropriate" means.

A face whose link is not appropriate will always be marked as invalid.
Note that there are other types of invalid faces also. See isValid()
for a full discussion of what it means for a face to be valid.

Precondition:
    The dimension *dim* is one of Regina's standard dimensions. Any
    attempt to use this routine in higher dimensions *dim* will result
    in a compile-time error. This is because testing for bad links in
    higher dimensions can require solutions to problems that are
    proven to be undecidable.

Returns:
    ``True`` if the link of this face is not appropriate (thereby
    making the face invalid), or ``False`` if the link is appropriate.)doc";

// Docstring regina::python::doc::detail::FaceBase_::inMaximalForest
constexpr const char *inMaximalForest =
R"doc(Determines whether a codimension-1-face represents a dual edge in the
maximal forest that has been chosen for the dual 1-skeleton of the
triangulation.

This routine is only available for faces of codimension 1; that is,
(*dim*-1)-faces of a *dim*-dimensional triangulation.

When the skeletal structure of a triangulation is first computed, a
maximal forest in the dual 1-skeleton of the triangulation is also
constructed. Each dual edge in this maximal forest represents a
(*dim*-1)-face of the (primal) triangulation.

This maximal forest will remain fixed until the triangulation changes,
at which point it will be recomputed (as will all other skeletal
objects, such as connected components and so on). There is no
guarantee that, when it is recomputed, the maximal forest will use the
same dual edges as before.

This routine identifies whether this (*dim*-1)-face belongs to the
dual forest. In this sense it performs a similar role to
Simplex::facetInMaximalForest(), but this routine is typically easier
to use.

If the skeleton has already been computed, then this routine is very
fast (since it just returns a precomputed answer).

Precondition:
    The facial dimension *subdim* is precisely *dim*-1.

Returns:
    ``True`` if and only if this (*dim*-1)-face represents a dual edge
    in the maximal forest.)doc";

// Docstring regina::python::doc::detail::FaceBase_::index
constexpr const char *index =
R"doc(Returns the index of this face within the underlying triangulation.

Returns:
    the index of this face.)doc";

// Docstring regina::python::doc::detail::FaceBase_::isBoundary
constexpr const char *isBoundary =
R"doc(Determines if this face lies entirely on the boundary of the
triangulation.

For dimensions in which ideal and/or invalid vertices are both
possible and recognised: both ideal and invalid vertices are
considered to be on the boundary.

Returns:
    ``True`` if and only if this face lies on the boundary.)doc";

// Docstring regina::python::doc::detail::FaceBase_::isLinkOrientable
constexpr const char *isLinkOrientable =
R"doc(Determines if the link of this face is orientable.

This routine is fast: it uses pre-computed information, and does not
need to build a full triangulation of the link.

.. warning::
    If this face is identified with itself under a non-identity
    permutation (which makes the face invalid), then the return value
    of this routine is undefined.

Returns:
    ``True`` if and only if the link is orientable.)doc";

// Docstring regina::python::doc::detail::FaceBase_::isLocked
constexpr const char *isLocked =
R"doc(Determines whether this codimension-1-face is locked.

Essentially, locking a face of dimension (*dim*-1) means that the face
must not change. See Simplex<dim>::lockFacet() for full details on how
locks work and what their implications are.

This is equivalent to calling Simplex<dim>::isFacetLocked() from one
of the simplices on either side of this (*dim*-1)-face.

See Triangulation<dim>::hasLocks() for a convenient way to test
whether any top-dimensional simplex and/or (*dim*-1)-face is locked
across an entire triangulation.

Precondition:
    The facial dimension *subdim* is precisely *dim*-1.

Returns:
    ``True`` if and only if this (*dim*-1)-face is locked.)doc";

// Docstring regina::python::doc::detail::FaceBase_::isLoop
constexpr const char *isLoop =
R"doc(For edges, determines whether this face is a loop. A _loop_ is an edge
whose two endpoints are identified.

Precondition:
    The facial dimension *subdim* is precisely 1.

Returns:
    ``True`` if and only if this edge is a loop.)doc";

// Docstring regina::python::doc::detail::FaceBase_::isValid
constexpr const char *isValid =
R"doc(Determines if this face is valid.

There are several conditions that might make a *subdim*-face of a
*dim*-dimensional triangulation invalid:

1. if the face is identified with itself under a non-identity
   permutation (e.g., an edge is identified with itself in reverse, or
   a triangle is identified with itself under a rotation);

2. if the face does not have an appropriate link. Here the meaning of
   "appropriate" depends upon the type of face:

   * for a face that belongs to some boundary facet(s) of the
     triangulation, its link must be a topological ball;

   * for a vertex that does not belong to any boundary facets, its
     link must be a closed (*dim* - 1)-manifold;

   * for a (*subdim* ≥ 1)-face that does not belong to any boundary
     facets, its link must be a topological sphere.

Condition (1) is tested for all dimensions *subdim* and *dim*.
Condition (2) is more difficult, since it relies on undecidable
problems. As a result, (2) is _only_ tested when *dim* is one of
Regina's standard dimensions.

If this face is invalid, then it is possible to find out why. In non-
standard dimensions, this must mean that the face fails condition (1)
above. In standard dimensions, you can call the functions
hasBadIdentification() and/or hasBadLink() to determine whether the
failure is due to conditions (1) or (2) respectively.

Returns:
    for standard dimensions *dim*, returns ``True`` if and only if
    this face is valid according to both conditions (1) and (2) above;
    for non-standard dimensions *dim*, returns ``True`` if and only if
    this face is valid according to condition (1).)doc";

// Docstring regina::python::doc::detail::FaceBase_::lock
constexpr const char *lock =
R"doc(Locks this codimension-1-face.

Essentially, locking a face of dimension (*dim*-1) means that the face
must not change. See Simplex<dim>::lockFacet() for full details on how
locks work and what their implications are.

These locks are actually stored within the top-dimensional simplices
on either side of this facet. This means that, even if the underlying
triangulation changes (which means all (*dim*-1)-faces will be
destroyed and re-created as part of the skeleton recomputation), this
lock will nevertheless be preserved.

This is equivalent to calling Simplex<dim>::lockFacet() from one of
the simplices on either side of this (*dim*-1)-face.

It is safe to call this function even if this face is already locked.

Precondition:
    The facial dimension *subdim* is precisely *dim*-1.)doc";

// Docstring regina::python::doc::detail::FaceBase_::pentachoron
constexpr const char *pentachoron =
R"doc(A dimension-specific alias for face<4>().

This alias is available for facial dimensions *subdim* ≥ 5.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase_::pentachoronMapping
constexpr const char *pentachoronMapping =
R"doc(A dimension-specific alias for faceMapping<4>().

This alias is available for facial dimensions *subdim* ≥ 5.

See faceMapping() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase_::tetrahedron
constexpr const char *tetrahedron =
R"doc(A dimension-specific alias for face<3>().

This alias is available for facial dimensions *subdim* ≥ 4.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase_::tetrahedronMapping
constexpr const char *tetrahedronMapping =
R"doc(A dimension-specific alias for faceMapping<3>().

This alias is available for facial dimensions *subdim* ≥ 4.

See faceMapping() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase_::triangle
constexpr const char *triangle =
R"doc(A dimension-specific alias for face<2>().

This alias is available for facial dimensions *subdim* ≥ 3.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase_::triangleMapping
constexpr const char *triangleMapping =
R"doc(A dimension-specific alias for faceMapping<2>().

This alias is available for facial dimensions *subdim* ≥ 3.

See faceMapping() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase_::triangulation
constexpr const char *triangulation =
R"doc(Returns the triangulation to which this face belongs.

Returns:
    a reference to the triangulation containing this face.)doc";

// Docstring regina::python::doc::detail::FaceBase_::unlock
constexpr const char *unlock =
R"doc(Unlocks this codimension-1-face.

Essentially, locking a face of dimension (*dim*-1) means that the face
must not change. See Simplex<dim>::lockFacet() for full details on how
locks work and what their implications are.

This is equivalent to calling Simplex<dim>::unlockFacet() from one of
the simplices on either side of this (*dim*-1)-face.

It is safe to call this function even if this face is already
unlocked.

See Triangulation<dim>::unlockAll() for a convenient way to unlock all
top-dimensional simplices and (*dim*-1)-faces across an entire
triangulation.

Precondition:
    The facial dimension *subdim* is precisely *dim*-1.)doc";

// Docstring regina::python::doc::detail::FaceBase_::vertex
constexpr const char *vertex =
R"doc(A dimension-specific alias for face<0>().

This alias is available for all facial dimensions *subdim*.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase_::vertexMapping
constexpr const char *vertexMapping =
R"doc(A dimension-specific alias for faceMapping<0>().

This alias is available for all facial dimensions *subdim*.

See faceMapping() for further information.)doc";

}

namespace detail::FaceEmbeddingBase_ {

// Docstring regina::python::doc::detail::FaceEmbeddingBase_::__copy
constexpr const char *__copy =
R"doc(Creates a new copy of the given object.

Parameter ``cloneMe``:
    the object to copy.)doc";

// Docstring regina::python::doc::detail::FaceEmbeddingBase_::__eq
constexpr const char *__eq =
R"doc(Tests whether this and the given object are identical.

Here _identical_ means that two FaceEmbedding objects refer to the
same-numbered face of the same-numbered simplex, _and_ have the same
embedding permutations as returned by vertices().

In particular, since this test only examines face/simplex/vertex
_numbers_ (not object pointers), it is meaningful to compare two
FaceEmbedding objects from different underlying triangulations.

.. warning::
    The meaning of this comparison changed in Regina 7.0. In older
    versions of Regina, to compare as equal, two FaceEmbedding objects
    (i) had to be faces of the same Simplex object (a stronger
    requirement that effectively restricted this test to faces of the
    same triangulation); but also (ii) only had to refer to the same-
    numbered face, not use the same full embedding permutations (a
    weaker requirement that nowadays would incur an unacceptable
    performance cost).

Parameter ``rhs``:
    the object to compare with this.

Returns:
    ``True`` if and only if both object are identical.)doc";

// Docstring regina::python::doc::detail::FaceEmbeddingBase_::__init
constexpr const char *__init =
R"doc(Creates a new object containing the given data.

Parameter ``simplex``:
    the top-dimensional simplex in which the underlying *subdim*-face
    of the triangulation is contained.

Parameter ``vertices``:
    a mapping from the vertices of the underlying *subdim*-face of the
    triangulation to the corresponding vertex numbers of *simplex*.
    See vertices() for details of how this permutation should be
    structured.)doc";

// Docstring regina::python::doc::detail::FaceEmbeddingBase_::face
constexpr const char *face =
R"doc(Returns the corresponding face number of simplex(). This identifies
which face of the top-dimensional simplex simplex() refers to the
underlying *subdim*-face of the triangulation.

If the face dimension *subdim* is at most 4, then you can also access
this face number through a dimension-specific alias (e.g., edge() in
the case *subdim* = 1).

Returns:
    the corresponding face number of the top-dimensional simplex. This
    will be between 0 and (*dim*+1 choose *subdim*+1)-1 inclusive.)doc";

// Docstring regina::python::doc::detail::FaceEmbeddingBase_::simplex
constexpr const char *simplex =
R"doc(Returns the top-dimensional simplex in which the underlying
*subdim*-face of the triangulation is contained.

If the triangulation dimension *dim* is at most 4, then you can also
access this simplex through a dimension-specific alias (e.g.,
tetrahedron() in the case *dim* = 3).

Returns:
    the top-dimensional simplex.)doc";

// Docstring regina::python::doc::detail::FaceEmbeddingBase_::vertices
constexpr const char *vertices =
R"doc(Maps vertices (0,...,*subdim*) of the underlying *subdim*-face of the
triangulation to the corresponding vertex numbers of simplex().

If the link of the underlying *subdim*-face is orientable, then this
permutation also maps (*subdim*+1, ..., *dim*) to the remaining vertex
numbers of simplex() in a manner that preserves orientation as you
walk through the many different FaceEmbedding objects for the same
underlying *subdim*-face.

This routine returns the same permutation as
``simplex().faceMapping<subdim>(face())`` (and this routine is faster
if you already have a FaceEmbedding). See Simplex<dim>::faceMapping()
for details.

Returns:
    a mapping from the vertices of the underlying *subdim*-face to the
    corresponding vertices of simplex().)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

