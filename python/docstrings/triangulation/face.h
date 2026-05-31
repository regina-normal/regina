/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct Face {

// Docstring regina::python::doc::Face::__class
static constexpr const char __class[] =
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
    the dimension of the underlying triangulation.

Template parameter ``subdim``:
    the dimension of the faces that this class represents.)doc";

}; // struct Face

struct FaceEmbedding {

// Docstring regina::python::doc::FaceEmbedding::__class
static constexpr const char __class[] =
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
    the dimension of the underlying triangulation.

Template parameter ``subdim``:
    the dimension of the faces of the underlying triangulation.)doc";

// Docstring regina::python::doc::FaceEmbedding::__copy
static constexpr const char __copy[] =
R"doc(Creates a new copy of the given object.

Parameter ``cloneMe``:
    the object to copy.)doc";

// Docstring regina::python::doc::FaceEmbedding::__eq
static constexpr const char __eq[] =
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

// Docstring regina::python::doc::FaceEmbedding::__init
static constexpr const char __init[] =
R"doc(Creates a new object containing the given data.

Parameter ``simplex``:
    the top-dimensional simplex in which the underlying *subdim*-face
    of the triangulation is contained.

Parameter ``vertices``:
    a mapping from the vertices of the underlying *subdim*-face of the
    triangulation to the corresponding vertex numbers of *simplex*.
    See vertices() for details of how this permutation should be
    structured.)doc";

// Docstring regina::python::doc::FaceEmbedding::edge
static constexpr const char edge[] =
R"doc(An alias for face() for faces of dimension 1.

See face() for further information.)doc";

// Docstring regina::python::doc::FaceEmbedding::face
static constexpr const char face[] =
R"doc(Returns the corresponding face number of simplex(). This identifies
which face of the top-dimensional simplex simplex() refers to the
underlying *subdim*-face of the triangulation.

If the face dimension *subdim* is at most 4, then you can also access
this face number through a dimension-specific alias (e.g., edge() in
the case *subdim* = 1).

Returns:
    the corresponding face number of the top-dimensional simplex. This
    will be between 0 and (*dim*+1 choose *subdim*+1)-1 inclusive.)doc";

// Docstring regina::python::doc::FaceEmbedding::pentachoron
static constexpr const char pentachoron[] =
R"doc(An alias for face() for faces of dimension 4.

See face() for further information.)doc";

// Docstring regina::python::doc::FaceEmbedding::simplex
static constexpr const char simplex[] =
R"doc(Returns the top-dimensional simplex in which the underlying
*subdim*-face of the triangulation is contained.

If the triangulation dimension *dim* is at most 4, then you can also
access this simplex through a dimension-specific alias (e.g.,
tetrahedron() in the case *dim* = 3).

Returns:
    the top-dimensional simplex.)doc";

// Docstring regina::python::doc::FaceEmbedding::simplex_dim2
static constexpr const char simplex_dim2[] =
R"doc(An alias for simplex() in dimension 2.

See simplex() for further information.)doc";

// Docstring regina::python::doc::FaceEmbedding::simplex_dim3
static constexpr const char simplex_dim3[] =
R"doc(An alias for simplex() in dimension 3.

See simplex() for further information.)doc";

// Docstring regina::python::doc::FaceEmbedding::simplex_dim4
static constexpr const char simplex_dim4[] =
R"doc(An alias for simplex() in dimension 4.

See simplex() for further information.)doc";

// Docstring regina::python::doc::FaceEmbedding::tetrahedron
static constexpr const char tetrahedron[] =
R"doc(An alias for face() for faces of dimension 3.

See face() for further information.)doc";

// Docstring regina::python::doc::FaceEmbedding::triangle
static constexpr const char triangle[] =
R"doc(An alias for face() for faces of dimension 2.

See face() for further information.)doc";

// Docstring regina::python::doc::FaceEmbedding::vertex
static constexpr const char vertex[] =
R"doc(An alias for face() for faces of dimension 0.

See face() for further information.)doc";

// Docstring regina::python::doc::FaceEmbedding::vertices
static constexpr const char vertices[] =
R"doc(Maps vertices (0,...,*subdim*) of the underlying *subdim*-face of the
triangulation to the corresponding vertex numbers of simplex().

If the link of the underlying *subdim*-face is orientable, then this
permutation also maps (*subdim*+1, ..., *dim*) to the remaining vertex
numbers of simplex() in a manner that preserves orientation as you
walk through the many different FaceEmbedding objects for the same
underlying *subdim*-face.

This routine returns the same permutation as
``simplex()->faceMapping<subdim>(face())`` (and this routine is faster
if you already have a FaceEmbedding). See Simplex<dim>::faceMapping()
for details.

.. warning::
    Be aware that if the triangulation changes then the skeleton will
    be recomputed, and there is no guarantee that the new Face objects
    will use the same inherent labelling as the old ones. In
    particular, after the triangulation changes, the FaceEmbedding for
    the same face of the same simplex might return a different
    permutation for ``vertices()``. Likewise, if you keep a copy of an
    old FaceEmbedding ``emb`` and then change the triangulation, the
    connection between ``emb.vertices()`` (which will not be updated)
    and ``emb.simplex()->faceMapping<subdim>(emb.face())`` (which will
    be updated) will be lost.

Returns:
    a mapping from the vertices of the underlying *subdim*-face to the
    corresponding vertices of simplex().)doc";

}; // struct FaceEmbedding

struct TriangleType {

// Docstring regina::python::doc::TriangleType::Cone
static constexpr const char Cone[] =
R"doc(Specifies a triangle with two edges identified to form a cone. The
apex of the cone is not identified with the other two vertices, and
the base of the cone is not identified with the other two edges.)doc";

// Docstring regina::python::doc::TriangleType::DunceHat
static constexpr const char DunceHat[] =
R"doc(Specifies a triangle with all three edges identified, some via
orientation-preserving and some via orientation-reversing gluings.)doc";

// Docstring regina::python::doc::TriangleType::Horn
static constexpr const char Horn[] =
R"doc(Specifies a triangle with two edges identified to form a cone, and
with all three vertices identified. The base of the cone is not
identified with the other two edges.)doc";

// Docstring regina::python::doc::TriangleType::L31
static constexpr const char L31[] =
R"doc(Specifies a triangle with all three edges identified using
orientation-reversing gluings. Note that this forms a spine for the
lens space ``L(3,1)``.)doc";

// Docstring regina::python::doc::TriangleType::Mobius
static constexpr const char Mobius[] =
R"doc(Specifies a triangle with two edges identified to form a Möbius band.
The boundary of the Möbius band is not identified with the other two
edges.)doc";

// Docstring regina::python::doc::TriangleType::Parachute
static constexpr const char Parachute[] =
R"doc(Specifies a triangle with three identified vertices, but no edge
identifications.)doc";

// Docstring regina::python::doc::TriangleType::Scarf
static constexpr const char Scarf[] =
R"doc(Specifies a triangle with two identified vertices, and no other edge
or vertex identifications.)doc";

// Docstring regina::python::doc::TriangleType::Triangle
static constexpr const char Triangle[] = R"doc(Specifies a triangle with no identified vertices or edges.)doc";

// Docstring regina::python::doc::TriangleType::Unknown
static constexpr const char Unknown[] = R"doc(Indicates that the triangle type has not yet been determined.)doc";

// Docstring regina::python::doc::TriangleType::__class
static constexpr const char __class[] =
R"doc(The combinatorial _type_ of a triangle, which indicates how the
vertices and edges of the triangle are identified together. Here the
vertices of the triangle are considered unlabelled (so a relabelling
will not change the combinatorial type).

This is the result of calling ``f.triangleType()``, where *f* is a
2-face within a triangulation of any dimension ≥ 3.)doc";

}; // struct TriangleType

namespace detail {

struct FaceBase {

// Docstring regina::python::doc::detail::FaceBase::__class
static constexpr const char __class[] =
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
    the dimension of the underlying triangulation.

Template parameter ``subdim``:
    the dimension of the faces that this class represents.)doc";

// Docstring regina::python::doc::detail::FaceBase::__iter__
static constexpr const char __iter__[] =
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

// Docstring regina::python::doc::detail::FaceBase::back
static constexpr const char back[] =
R"doc(Returns the last appearance of this face within a top-dimensional
simplex of the underlying triangulation.

This is equivalent to calling ``embedding(degree()-1)``.

In most cases, the ordering of appearances is arbitrary. The exception
is for codimension 2, where the appearances of a face are ordered in a
way that follows the link around the face (which in codimension 2 is
always a path or a cycle). In particular, for a boundary face of
codimension 2, both front() and back() will refer to the two
appearances of this face on the (*dim*-1)-dimensional boundary.

Returns:
    details of the last appearance.)doc";

// Docstring regina::python::doc::detail::FaceBase::boundaryComponent
static constexpr const char boundaryComponent[] =
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

// Docstring regina::python::doc::detail::FaceBase::component
static constexpr const char component[] =
R"doc(Returns the component of the triangulation to which this face belongs.

Returns:
    the component containing this face.)doc";

// Docstring regina::python::doc::detail::FaceBase::degree
static constexpr const char degree[] =
R"doc(Returns the degree of this face. This is the number of different ways
in which the face appears within the various top-dimensional simplices
of the underlying triangulation.

Note that if this face appears multiple times within the same top-
dimensional simplex, then it will be counted multiple times by this
routine.

Returns:
    the degree of this face.)doc";

// Docstring regina::python::doc::detail::FaceBase::edge
static constexpr const char edge[] =
R"doc(A dimension-specific alias for face<1>().

See face() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase::edgeMapping
static constexpr const char edgeMapping[] =
R"doc(A dimension-specific alias for faceMapping<1>().

See faceMapping() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase::embedding
static constexpr const char embedding[] =
R"doc(Returns one of the ways in which this face appears within a top-
dimensional simplex of the underlying triangulation.

For convenience, you can also use begin() and end() to iterate through
all such appearances, or equivalently you can iterate over the
lightweight view returned by embeddings().

In most cases, the ordering of appearances is arbitrary. The exception
is for codimension 2, where these appearances are ordered in a way
that follows the link around the face (which in codimension 2 is
always a path or a cycle).

Parameter ``index``:
    the index of the requested appearance. This must be between 0 and
    degree()-1 inclusive.

Returns:
    details of the requested appearance.)doc";

// Docstring regina::python::doc::detail::FaceBase::embeddings
static constexpr const char embeddings[] =
R"doc(Returns an object that allows iteration through and random access to
all of the ways in which this face appears within a top-dimensional
simplex of the underlying triangulation.

The object that is returned is lightweight, and can be happily copied
by value. The C++ type of the object is subject to change, so C++
users should use ``auto`` (just like this declaration does).

The returned object is guaranteed to be a lightweight view type from
the ``std::ranges`` library, which means it supports range-based
``for`` loops. The elements of the list will be read-only objects of
type ``FaceEmbedding<dim, subdim>``. For example, your code might look
like:

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

// Docstring regina::python::doc::detail::FaceBase::face
static constexpr const char face[] =
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
    the dimension of subface to examine.

Parameter ``face``:
    the *lowerdim*-face of this *subdim*-face to examine. This should
    be between 0 and (*subdim*+1 choose *lowerdim*+1)-1 inclusive.

Returns:
    the corresponding *lowerdim*-face of the triangulation.)doc";

// Docstring regina::python::doc::detail::FaceBase::faceMapping
static constexpr const char faceMapping[] =
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
    the dimension of subface to examine.

Parameter ``face``:
    the *lowerdim*-face of this *subdim*-face to examine. This should
    be between 0 and (*subdim*+1 choose *lowerdim*+1)-1 inclusive.

Returns:
    a mapping from the vertices of the underlying *lowerdim*-face of
    the triangulation to the vertices of this *subdim*-face.)doc";

// Docstring regina::python::doc::detail::FaceBase::formsCone
static constexpr const char formsCone[] =
R"doc(For triangles, determines whether this face is wrapped up to form a
cone, possibly with or without additional identifications between its
vertices and/or edges.

Note that several different triangle types (as returned by
triangleType()) can produce this result. Note also that a triangle can
satisfy both formsMobiusBand() and formsCone().

The reason this routine is non-const is because the triangle type is
cached when first computed.

Returns:
    ``True`` if and only if this triangle forms a cone.)doc";

// Docstring regina::python::doc::detail::FaceBase::formsMobiusBand
static constexpr const char formsMobiusBand[] =
R"doc(For triangles, determines whether this face is wrapped up to form a
Möbius band, possibly with or without additional identifications
between its vertices and/or edges.

Note that several different triangle types (as returned by
triangleType()) can produce this result. Note also that a triangle can
satisfy both formsMobiusBand() and formsCone().

The reason this routine is non-const is because the triangle type is
cached when first computed.

Returns:
    ``True`` if and only if this triangle forms a Mobius band.)doc";

// Docstring regina::python::doc::detail::FaceBase::front
static constexpr const char front[] =
R"doc(Returns the first appearance of this face within a top-dimensional
simplex of the underlying triangulation.

This is equivalent to calling ``*begin()``, or ``embedding(0)``.

In most cases, the ordering of appearances is arbitrary. The exception
is for codimension 2, where the appearances of a face are ordered in a
way that follows the link around the face (which in codimension 2 is
always a path or a cycle). In particular, for a boundary face of
codimension 2, both front() and back() will refer to the two
appearances of this face on the (*dim*-1)-dimensional boundary.

Returns:
    details of the first appearance.)doc";

// Docstring regina::python::doc::detail::FaceBase::hasBadIdentification
static constexpr const char hasBadIdentification[] =
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

// Docstring regina::python::doc::detail::FaceBase::hasBadLink
static constexpr const char hasBadLink[] =
R"doc(Determines if this face does not have an appropriate link. See
condition (2) in the documentation for isValid() for a full
description of what "appropriate" means.

A face whose link is not appropriate will always be marked as invalid.
Note that there are other types of invalid faces also. See isValid()
for a full discussion of what it means for a face to be valid.

This routine is only available when *dim* is one of Regina's standard
dimensions, because testing for bad links in higher dimensions can
require solutions to undecidable problems.

Returns:
    ``True`` if the link of this face is not appropriate (thereby
    making the face invalid), or ``False`` if the link is appropriate.)doc";

// Docstring regina::python::doc::detail::FaceBase::inMaximalForest
static constexpr const char inMaximalForest[] =
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

Returns:
    ``True`` if and only if this (*dim*-1)-face represents a dual edge
    in the maximal forest.)doc";

// Docstring regina::python::doc::detail::FaceBase::index
static constexpr const char index[] =
R"doc(Returns the index of this face within the underlying triangulation.

Returns:
    the index of this face.)doc";

// Docstring regina::python::doc::detail::FaceBase::isBoundary
static constexpr const char isBoundary[] =
R"doc(Determines if this face lies entirely on the boundary of the
triangulation.

For dimensions in which ideal and/or invalid vertices are both
possible and recognised: both ideal and invalid vertices are
considered to be on the boundary.

Returns:
    ``True`` if and only if this face lies on the boundary.)doc";

// Docstring regina::python::doc::detail::FaceBase::isLinkOrientable
static constexpr const char isLinkOrientable[] =
R"doc(Determines if the link of this face is orientable.

This routine is fast: it uses pre-computed information, and does not
need to build a full triangulation of the link.

As of Regina 7.4.1, the orientability of the link will be calculated
correctly even if the face is invalid due to a non-trivial self-
identification.

Returns:
    ``True`` if and only if the link is orientable.)doc";

// Docstring regina::python::doc::detail::FaceBase::isLocked
static constexpr const char isLocked[] =
R"doc(Determines whether this codimension-1-face is locked.

Essentially, locking a face of dimension (*dim*-1) means that the face
must not change. See Simplex<dim>::lockFacet() for full details on how
locks work and what their implications are.

This is equivalent to calling Simplex<dim>::isFacetLocked() from one
of the simplices on either side of this (*dim*-1)-face.

See Triangulation<dim>::hasLocks() for a convenient way to test
whether any top-dimensional simplex and/or (*dim*-1)-face is locked
across an entire triangulation.

Returns:
    ``True`` if and only if this (*dim*-1)-face is locked.)doc";

// Docstring regina::python::doc::detail::FaceBase::isLoop
static constexpr const char isLoop[] =
R"doc(For edges, determines whether this face is a loop. A _loop_ is an edge
whose two endpoints are identified.

Returns:
    ``True`` if and only if this edge is a loop.)doc";

// Docstring regina::python::doc::detail::FaceBase::isValid
static constexpr const char isValid[] =
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

// Docstring regina::python::doc::detail::FaceBase::join
static constexpr const char join[] =
R"doc(For boundary facets, joins this to another boundary facet using the
given gluing.

This is a convenience method that calls Simplex<dim>::join(). Where it
differs is that:

* you directly pass the two `(dim-1)`-faces that need to be joined, as
  opposed to working with `dim`-dimensional simplices;

* the gluing permutation is relative to the inherent labellings of the
  vertices of the (``dim-1``)-faces, _not_ the vertices of the top-
  dimensional simplices.

By "inherent labelling of vertices" of a face *f* we mean the way that
the vertices of *f* are labelled according to
FaceEmbedding::vertices(). This labelling is independent of the vertex
numbers in any top-dimensional simplices that contain *f*.

Precondition:
    This and the given face are distinct boundary facets of the same
    triangulation.

.. warning::
    As soon as the join takes place, both this and the given facet
    will be destroyed (since the skeleton of a triangulation is
    rebuilt whenever the triangulation changes).

Exception ``InvalidArgument``:
    At least one of the preconditions above fails; that is, this and
    the given face are the same, or belong to different
    triangulations, or are not both boundary facets.

Exception ``LockViolation``:
    Either this or the given face is a locked facet. This exception
    will be thrown before any change is made. See Simplex::lockFacet()
    for further details on how facet locks work and what their
    implications are.

Parameter ``you``:
    the other boundary facet that this should be glued to.

Parameter ``gluing``:
    a permutation that describes how the inherent vertices of this
    boundary facet will map to the inherent vertices of the given
    boundary facet across the new gluing.)doc";

// Docstring regina::python::doc::detail::FaceBase::linkingSurface
static constexpr const char linkingSurface[] =
R"doc(Returns the link of this vertex as a normal surface or hypersurface.

Note that vertex linking (hyper)surfaces only ever contain triangles
(in dimension 3) or tetrahedra (in dimension 4), not quadrilaterals or
prisms. Moreover, vertex links are always thin; that is, after
constructing the frontier of a regular neighbourhood of the vertex, no
further normalisation steps are required.

For faces of dimension ≥ 1, linkingSurface() returns a pair: the
linking (hyper)surface, as well as a boolean indicating whether the
link is thin. In contrast, for vertices this function returns just the
linking (hyper)surface, since this will always be thin.

Returns:
    the corresponding vertex linking normal (hyper)surface.)doc";

// Docstring regina::python::doc::detail::FaceBase::linkingSurface_2
static constexpr const char linkingSurface_2[] =
R"doc(Returns the link of this face as a normal surface or hypersurface.

Constructing the link of a face begins with building the frontier of a
regular neighbourhood of the face. If this is already a normal
(hyper)surface, then then link is called _thin_. Otherwise some basic
normalisation steps are performed until the (hyper)surface becomes
normal; note that these normalisation steps could change the topology
of the (hyper)surface, and in some pathological cases could even
reduce it to the empty (hyper)surface.

In dimension 3, normalising an arbitrary embedded surface is
reasonably straightforward. However, in dimension 4, normalising an
arbitrary embedded 3-manifold becomes much messier. Nevertheless, for
_face links_ (our scenario here), normalisation remains a fairly
simple process: essentially, any changes will be limited to operations
analagous to compressions and boundary compressions along discs and
3-balls, as well as removing trivial 4-sphere components.

Returns:
    a pair (*s*, *thin*), where *s* is the face-linking normal
    (hyper)surface, and *thin* is ``True`` if and only if this link is
    thin (i.e., no additional normalisation steps were required).)doc";

// Docstring regina::python::doc::detail::FaceBase::lock
static constexpr const char lock[] =
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

It is safe to call this function even if this face is already locked.)doc";

// Docstring regina::python::doc::detail::FaceBase::pentachoron
static constexpr const char pentachoron[] =
R"doc(A dimension-specific alias for face<4>().

See face() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase::pentachoronMapping
static constexpr const char pentachoronMapping[] =
R"doc(A dimension-specific alias for faceMapping<4>().

See faceMapping() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase::tetrahedron
static constexpr const char tetrahedron[] =
R"doc(A dimension-specific alias for face<3>().

See face() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase::tetrahedronMapping
static constexpr const char tetrahedronMapping[] =
R"doc(A dimension-specific alias for faceMapping<3>().

See faceMapping() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase::triangle
static constexpr const char triangle[] =
R"doc(A dimension-specific alias for face<2>().

See face() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase::triangleMapping
static constexpr const char triangleMapping[] =
R"doc(A dimension-specific alias for faceMapping<2>().

See faceMapping() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase::triangleSubtype
static constexpr const char triangleSubtype[] =
R"doc(For triangles, returns the vertex or edge number in this face that
plays a special role for this triangle's combinatorial type. Note that
only some triangle types have a special vertex or edge. The triangle
type itself is returned by triangleType().

If one or more edges of this triangle are invalid due to bad self-
identifications, then the triangle type might not be well-defined and
so the return value will likewise be undefined.

The reason this routine is non-const is because the triangle type and
subtype are cached when first computed.

Returns:
    the vertex or edge number (0, 1 or 2) that plays a special role,
    or -1 if this triangle's combinatorial type has no special vertex
    or edge.)doc";

// Docstring regina::python::doc::detail::FaceBase::triangleType
static constexpr const char triangleType[] =
R"doc(For triangles, returns the combinatorial type of this face. This will
be one of the eight shapes described by the TriangleType enumeration,
which indicates how the edges and vertices of the triangle are
identified.

If one or more edges of this triangle are invalid due to bad self-
identifications, then the triangle type might not be well-defined and
so the return value will likewise be undefined.

The reason this routine is non-const is because the triangle type and
subtype are cached when first computed.

Returns:
    the combinatorial type of this triangle. This routine will never
    return TriangleType::Unknown.)doc";

// Docstring regina::python::doc::detail::FaceBase::triangulation
static constexpr const char triangulation[] =
R"doc(Returns the triangulation to which this face belongs.

Returns:
    a reference to the triangulation containing this face.)doc";

// Docstring regina::python::doc::detail::FaceBase::unlock
static constexpr const char unlock[] =
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
triangulation.)doc";

// Docstring regina::python::doc::detail::FaceBase::vertex
static constexpr const char vertex[] =
R"doc(A dimension-specific alias for face<0>().

See face() for further information.)doc";

// Docstring regina::python::doc::detail::FaceBase::vertexMapping
static constexpr const char vertexMapping[] =
R"doc(A dimension-specific alias for faceMapping<0>().

See faceMapping() for further information.)doc";

}; // struct FaceBase

} // namespace detail

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

