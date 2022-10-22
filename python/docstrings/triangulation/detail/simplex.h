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

// Docstring regina::python::doc::detail::SimplexBase
constexpr const char *SimplexBase =
R"doc(Helper class that provides core functionality for a top-dimensional
simplex in a *dim*-manifold triangulation.

Each top-dimensional simplex is represented by the class Simplex<dim>,
which uses this as a base class. End users should not need to refer to
SimplexBase directly.

See the Simplex template class notes for further information,
including details of how the vertices and facets of each simplex are
numbered.

Neither this class nor the "end user" class Simplex<dim> support value
semantics: they cannot be copied, swapped, or manually constructed.
Their memory is managed by the Triangulation class, and their
locations in memory define them. See Simplex<dim> for further details.

Python:
    This base class is not present, but the "end user" class
    Simplex<dim> is.

Template parameter ``dim``:
    the dimension of the underlying triangulation. This must be
    between 2 and 15 inclusive.)doc";

}

namespace detail::SimplexBase_ {

// Docstring regina::python::doc::detail::SimplexBase_::adjacentFacet
constexpr const char *adjacentFacet =
R"doc(If the given facet of this simplex is glued to facet *f* of some
adjacent simplex, then this routine returns the adjacent facet number
*f*.

The return value from this routine is identical to
adjacentGluing(*facet*)[*facet*].

Precondition:
    The given facet of this simplex has some adjacent simplex
    (possibly this one) glued to it. In other words,
    adjacentSimplex(*facet*) is not ``None``.

Parameter ``facet``:
    the facet of this simplex that we are examining. This must be
    between 0 and *dim* inclusive.

Returns:
    the corresponding facet number of the adjacent simplex that is
    glued to the given facet of this simplex.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::adjacentGluing
constexpr const char *adjacentGluing =
R"doc(Returns a permutation that indicates precisely how this simplex is
glued to the adjacent simplex across the given facet.

In detail: suppose that the given facet of this simplex is glued to an
adjacent simplex *A*. Then this gluing induces a mapping from the
vertices of this simplex to the vertices of *A*. We can express this
mapping in the form of a permutation *p*, where:

* for any *v* ≠ *facet*, the gluing identifies vertex *v* of this
  simplex with vertex *p*[*v*] of simplex *A*;

* *p*[*facet*] indicates the facet of *A* that is on the other side of
  the gluing (i.e., the facet of *A* that is glued to the given facet
  of this simplex).

Precondition:
    The given facet of this simplex has some adjacent simplex
    (possibly this one) glued to it. In other words,
    adjacentSimplex(*facet*) is not ``None``.

Parameter ``facet``:
    the facet of this simplex that we are examining. This must be
    between 0 and *dim* inclusive.

Returns:
    a permutation that maps the vertices of this simplex to the
    vertices of the adjacent simplex, as described above.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::adjacentSimplex
constexpr const char *adjacentSimplex =
R"doc(Returns the adjacent simplex that is glued to the given facet of this
simplex. If there is no adjacent simplex (i.e., the given facet lies
on the triangulation boundary), then this routine will return
``None``.

Parameter ``facet``:
    the facet of this simplex to examine; this must be between 0 and
    *dim* inclusive.

Returns:
    the adjacent simplex glued to the given facet, or ``None`` if the
    given facet lies on the boundary.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::component
constexpr const char *component =
R"doc(Returns the connected component of the triangulation to which this
simplex belongs.

Returns:
    the component containing this simplex.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::description
constexpr const char *description =
R"doc(Returns the description associated with this simplex.

Returns:
    the description of this simplex, or the empty string if no
    description is stored.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::edge
constexpr const char *edge =
R"doc(A dimension-specific alias for face<1>().

This alias is available for all dimensions *dim*.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::edge_2
constexpr const char *edge_2 =
R"doc(Returns the edge of this simplex that connects the two given vertices
of this simplex.

This is a convenience routine to avoid more cumbersome calls to
Edge<dim>::faceNumber(). In dimensions 3 and 4 (where the array
Edge<dim>::edgeNumber is defined), this routine is identical to
calling ``edge(Edge<dim>::edgeNumber[i][j])``.

Parameter ``i``:
    the vertex of this simplex that forms one endpoint of the edge;
    this must be between 0 and *dim* inclusive.

Parameter ``j``:
    the vertex of this simplex that forms the other endpoint of the
    edge; this must be between 0 and *dim* inclusive, and must also be
    different from *i*.

Returns:
    the edge of this simplex that connects vertices *i* and *j* of
    this simplex.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::edgeMapping
constexpr const char *edgeMapping =
R"doc(A dimension-specific alias for faceMapping<1>().

This alias is available for all dimensions *dim*.

See faceMapping() for further information.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::face
constexpr const char *face =
R"doc(Returns the *subdim*-face of the underlying triangulation that appears
as the given *subdim*-face of this simplex.

See FaceNumbering<dim, subdim> for the conventions of how
*subdim*-faces are numbered within a *dim*-simplex.

Python:
    Python does not support templates. Instead, Python users should
    call this function in the form ``face(subdim, face)``; that is,
    the template parameter *subdim* becomes the first argument of the
    function.

Parameter ``face``:
    the *subdim*-face of this simplex to examine. This should be
    between 0 and (*dim*+1 choose *subdim*+1)-1 inclusive.

Returns:
    the corresponding *subdim*-face of the triangulation.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::faceMapping
constexpr const char *faceMapping =
R"doc(Examines the given *subdim*-face of this simplex, and returns the
mapping between the underlying *subdim*-face of the triangulation and
the individual vertices of this simplex.

Specifically:

* Suppose several *subdim*-faces of several top-dimensional simplices
  are identified within the overall triangulation. Then we call this a
  single "*subdim*-face of the triangulation", and arbitrarily label
  its vertices (0, ..., *subdim*).

* Now let *F* denote the *subdim*-face of the triangulation that
  corresponds to *subdim*-face number *face* of this simplex. Then
  this routine returns a map from vertices (0, ..., *subdim*) of *F*
  to the corresponding vertex numbers of this simplex.

* In particular, if this routine returns the permutation *p*, then the
  images *p*[0,...,*subdim*] will be some permutation of the vertices
  Face<dim, subdim>::ordering[0,...,*subdim*].

* If *F* also appears as face number *k* in some other simplex *s*,
  then for each *i* in the range 0 ≤ *i* ≤ *subdim*, vertex ``p[i]``
  of this simplex will be identified with vertex
  ``s.faceMapping(k)[i]`` of simplex *s*.

If the link of the underlying *subdim*-face is orientable, then this
permutation maps the remaining numbers (*subdim*+1, ..., *dim*) to the
remaining vertex numbers of this simplex in a manner that preserves
orientation as you walk through the many different simplices that
contain the same underlying *subdim*-face. Specifically:

* The images of (*subdim*+1, ..., *dim*) under this permutation imply
  an orientation for the (*dim* - *subdim* - 1)-face opposite *F* in
  this simplex. These orientations will be consistent for all
  simplices containing *F*.

* For faces of codimension two (e.g., edges in a 3-manifold
  triangulation), this orientation condition is even stronger. Here
  the link of the face *F* must be a path (for a boundary face) or a
  cycle (for an internal face). In each simplex we can form a directed
  edge from the image of *dim*-1 to the image of *dim* under this
  permutation, and together these directed edges form a directed path
  or cycle that follows the link of the face *F*. Moreover, an
  iteration through the corresponding FaceEmbedding<dim, subdim>
  objects in order from ``F.begin()`` to ``F.end()``, will follow this
  directed path in order from start to end. (In the case where the
  link of *F* is a cycle, the start point in the list of FaceEmbedding
  objects will be arbitrary.)

Note that, even if the link is orientable, there are still arbitrary
decisions to be made for the images of (*subdim*+1, ..., *dim*), since
there will always be (*dim*-*subdim*)!/2 possible mappings that yield
the correct orientation.

If this simplex (and therefore the face *F*) belongs to an
_orientable_ component of the triangulation, then there will also be
connections between faceMapping() and the orientations of the top-
dimensional simplices (as returned by orientation()):

* If *subdim* is less than (*dim* - 1), then the sign of the
  permutation returned by faceMapping() will always be equal to the
  orientation of this simplex.

* If *subdim* is equal to (*dim* - 1), then the face *F* can only
  belong to either one or two top-dimensional simplices; let *s0* and
  *s1* be the simplices corresponding to ``F.embedding(0)`` and (if it
  exists) ``F.embedding(1)`` respectively. Then in the simplex *s0*,
  the sign of the faceMapping() permutation will match the orientation
  of *s0*, and in *s1* (if it exists), the sign of the faceMapping()
  permutation will be negative the orientation of *s1*.

.. note::
    This routine returns the same permutation as FaceEmbedding<dim,
    subdim>::vertices(), in the context of the FaceEmbedding<dim,
    subdim> object that refers to *subdim*-face number *face* of this
    simplex.

Python:
    Python does not support templates. Instead, Python users should
    call this function in the form ``faceMapping(subdim, face)``; that
    is, the template parameter *subdim* becomes the first argument of
    the function.

Parameter ``face``:
    the *subdim*-face of this simplex to examine. This should be
    between 0 and (*dim*+1 choose *subdim*+1)-1 inclusive.

Returns:
    a mapping from the vertices of the underlying *subdim*-face of the
    triangulation to the vertices of this simplex.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::facetInMaximalForest
constexpr const char *facetInMaximalForest =
R"doc(Determines whether the given facet of this simplex belongs to the
maximal forest that has been chosen for the dual 1-skeleton of the
underlying triangulation.

When the skeletal structure of a triangulation is first computed, a
maximal forest in the dual 1-skeleton of the triangulation is also
constructed. Each dual edge in this maximal forest represents a
(*dim*-1)-face of the (primal) triangulation.

This maximal forest will remain fixed until the triangulation changes,
at which point it will be recomputed (as will all other skeletal
objects, such as connected components and so on). There is no
guarantee that, when it is recomputed, the maximal forest will use the
same dual edges as before.

This routine identifies which (*dim*-1)-faces of the triangulation
belong to the dual forest. Because it lives in the Simplex class, this
routine can even be used for those dimensions that do not have
explicit classes for (*dim*-1)-faces of the triangulation.

If the skeleton has already been computed, then this routine is very
fast (since it just returns a precomputed answer).

Parameter ``facet``:
    the facet of this simplex that we are examining. This must be
    between 0 and *dim* inclusive.

Returns:
    ``True`` if and only if the given facet of this simplex
    corresponds to a dual edge in the maximal forest chosen for the
    dual 1-skeleton.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::hasBoundary
constexpr const char *hasBoundary =
R"doc(Determines if this simplex has any facets that lie on the
triangulation boundary. In other words, this routine determines
whether any facet of this simplex is not currently glued to an
adjacent simplex.

Returns:
    ``True`` if and only if this simplex has any boundary facets.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::index
constexpr const char *index =
R"doc(Returns the index of this simplex in the underlying triangulation.

The index will be an integer between 0 and
``triangulation().size()-1`` inclusive.

Note that indexing may change when a simplex is added to or removed
from the underlying triangulation.

Returns:
    the index of this simplex.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::isolate
constexpr const char *isolate =
R"doc(Unglues this simplex from any adjacent simplices. As a result, every
facet of this simplex will become a boundary facet, and this simplex
will form its own separate component of the underlying triangulation.

If there were any adjacent simplices to begin with, these will be
updated automatically.

This routine is safe to call even if there are no adjacent simplices
(in which case it will do nothing).)doc";

// Docstring regina::python::doc::detail::SimplexBase_::join
constexpr const char *join =
R"doc(Joins the given facet of this simplex to some facet of another
simplex. The other simplex will be updated automatically (i.e., you
only need to call join() from one side of the gluing).

You may join a facet of this simplex to some different facet of the
same simplex (i.e., you may pass *you* == ``this``), though you cannot
join a facet to itself.

Precondition:
    This and the given simplex belong to the same triangulation.

Precondition:
    The given facet of this simplex is not currently glued to
    anything.

Precondition:
    The corresponding facet of the other simplex (i.e., facet
    *gluing*[*myFacet*] of *you*) is likewise not currently glued to
    anything.

Precondition:
    We are not attempting to glue a facet to itself (i.e., we do not
    have both *you* == ``this`` and *gluing*[*myFacet*] == *myFacet*).

Exception ``InvalidArgument``:
    At least one of the conditions above fails; that is, either the
    two simplices being joined belong to different triangulations, or
    one of the two facets being joined is already joined to something,
    or you are trying to join the same facet of the same simplex to
    itself.

Parameter ``myFacet``:
    the facet of this simplex that will be glued to the given simplex
    *you*. This facet number must be between 0 and *dim* inclusive.

Parameter ``you``:
    the other simplex that will be glued to the given facet of this
    simplex.

Parameter ``gluing``:
    a permutation that describes how the vertices of this simplex will
    map to the vertices of *you* across the new gluing. This
    permutation should be in the form described by adjacentGluing().)doc";

// Docstring regina::python::doc::detail::SimplexBase_::orientation
constexpr const char *orientation =
R"doc(Returns the orientation of this simplex in the *dim*-dimensional
triangulation.

The orientation of each top-dimensional simplex is always +1 or -1. In
an orientable component of a triangulation, adjacent simplices have
the same orientations if one could be transposed onto the other
without reflection, and they have opposite orientations if a
reflection would be required. In a non-orientable component,
orientations are arbitrary (but they will still all be +1 or -1).

In each component, the top-dimensional simplex with smallest index
will always have orientation +1. In particular, simplex 0 will always
have orientation +1.

Returns:
    +1 or -1 according to the orientation of this simplex.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::pentachoron
constexpr const char *pentachoron =
R"doc(A dimension-specific alias for face<4>().

This alias is available for dimensions *dim* ≥ 5.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::pentachoronMapping
constexpr const char *pentachoronMapping =
R"doc(A dimension-specific alias for faceMapping<4>().

This alias is available for dimensions *dim* ≥ 5.

See faceMapping() for further information.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::setDescription
constexpr const char *setDescription =
R"doc(Sets the description associated with this simplex.

This may be any text whatsoever; typically it is intended to be human-
readable. Descriptions do not need to be unique.

To remove an existing description, you can simply set the description
to the empty string.

Parameter ``desc``:
    the new description to assign to this simplex.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::tetrahedron
constexpr const char *tetrahedron =
R"doc(A dimension-specific alias for face<3>().

This alias is available for dimensions *dim* ≥ 4.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::tetrahedronMapping
constexpr const char *tetrahedronMapping =
R"doc(A dimension-specific alias for faceMapping<3>().

This alias is available for dimensions *dim* ≥ 4.

See faceMapping() for further information.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::triangle
constexpr const char *triangle =
R"doc(A dimension-specific alias for face<2>().

This alias is available for dimensions *dim* ≥ 3.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::triangleMapping
constexpr const char *triangleMapping =
R"doc(A dimension-specific alias for faceMapping<2>().

This alias is available for dimensions *dim* ≥ 3.

See faceMapping() for further information.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::triangulation
constexpr const char *triangulation =
R"doc(Returns the triangulation to which this simplex belongs.

Returns:
    a reference to the triangulation containing this simplex.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::unjoin
constexpr const char *unjoin =
R"doc(Unglues the given facet of this simplex from whatever it is joined to.
As a result, the given facet of this simplex will become a boundary
facet.

If there was an adjacent simplex to begin with, then this other
simplex will be updated automatically (i.e., you only need to call
unjoin() from one side of the gluing).

This routine is safe to call even if the given facet is already a
boundary facet (in which case it will do nothing).

Parameter ``myFacet``:
    the facet of this simplex whose gluing we will undo. This should
    be between 0 and *dim* inclusive.

Returns:
    the simplex that was originally glued to the given facet of this
    simplex, or ``None`` if this was already a boundary facet.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::vertex
constexpr const char *vertex =
R"doc(A dimension-specific alias for face<0>().

This alias is available for all dimensions *dim*.

See face() for further information.)doc";

// Docstring regina::python::doc::detail::SimplexBase_::vertexMapping
constexpr const char *vertexMapping =
R"doc(A dimension-specific alias for faceMapping<0>().

This alias is available for all dimensions *dim*.

See faceMapping() for further information.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

