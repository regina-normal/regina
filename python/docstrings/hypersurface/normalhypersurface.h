/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::NormalHypersurface
static const char *NormalHypersurface =
R"doc(Represents a single normal hypersurface in a 4-manifold triangulation.

The normal hypersurface is described internally by an integer vector
(discussed in more detail below). Since different hypersurfaces may
use different vector encodings, you should not rely on the raw vector
entries unless absolutely necessary. Instead, the query routines such
as tetrahedra(), prisms(), edgeWeight() and so on are independent of
the underlying vector encoding being used.

Note that non-compact hypersurfaces (surfaces with infinitely many
pieces) are allowed; in these cases, the corresponding coordinate
lookup routines will return LargeInteger::infinity where appropriate.

Since Regina 7.0, you can modify or even destroy the original
triangulation that was used to create this normal hypersurface. If you
do, then this normal hypersurface will automatically make a private
copy of the original triangulation as an ongoing reference. Different
normal hypersurfaces can all share the same private copy, so this is
not an expensive process.

Internally, a normal hypersurface is represented by a
Vector<LargeInteger> (possibly using a different coordinate system
from the one in which the hypersurfaces were originally enumerated).
This contains a block of coordinates for each pentachoron, in order
from the first pentachoron to the last. Each block begins with five
tetrahedron coordinates (always), followed by ten prisms coordinates
(always) - unlike the 3-dimensional world, there are currently no
optional coordinates that might or might not be stored (though this
could change in future versions of Regina). Therefore the vector that
is stored will always have length 15*n*, where *n* is the number of
pentachora in the underlying triangulation.

When adding support for a new coordinate system:

* The file hypercoords.h must be updated. This includes a new enum
  value for HyperCoords, a new case for the HyperEncoding constructor,
  and new cases for the functions in HyperInfo. Do not forget to
  update the python bindings for HyperCoords also.

* The global routines makeEmbeddedConstraints() and
  makeMatchingEquations() should be updated to incorporate the new
  coordinate system.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace NormalHypersurface_ {

// Docstring regina::python::doc::NormalHypersurface_::__add
static const char *__add =
R"doc(Returns the sum of this and the given hypersurface. This will combine
all tetrahedra and/or prisms from both surfaces.

The two hypersurfaces do not need to use the same coordinate system
and/or internal vector encodings. Moreover, the resulting hypersurface
might well use an encoding different from both of these, or even a
hybrid encoding that does not come from one of Regina's ready-made
coordinate systems.

Precondition:
    Both this and the given normal hypersurface use the same
    underlying triangulation.

Parameter ``rhs``:
    the hypersurface to sum with this.

Returns:
    the sum of both normal hypersurfaces.)doc";

// Docstring regina::python::doc::NormalHypersurface_::__eq
static const char *__eq =
R"doc(Determines whether this and the given hypersurface in fact represent
the same normal hypersurface.

Specifically, this routine examines (or computes) the number of normal
pieces of each type, and returns ``True`` if and only if these counts
are the same for both hypersurfaces.

It does not matter what vector encodings the two hypersurfaces use. In
particular, it does not matter if this and the given hypersurface use
different encodings, or if one but not the other supports non-compact
hypersurfaces.

This routine is safe to call even if this and the given hypersurface
do not belong to the same triangulation:

* If the two triangulations have the same size, then this routine will
  test whether this hypersurface, if transplanted into the other
  triangulation using the same pentachoron numbering and the same
  normal piece types, would be the same as *other*.

* If the two triangulations have different sizes, then this routine
  will return ``False``.

Parameter ``other``:
    the hypersurface to be compared with this hypersurface.

Returns:
    ``True`` if both hypersurfaces represent the same normal
    hypersurface, or ``False`` if not.)doc";

// Docstring regina::python::doc::NormalHypersurface_::__imul
static const char *__imul =
R"doc(Converts this hypersurface into the given integer multiple of itself.

The internal vector encoding used by this hypersurface will not
change.

Parameter ``coeff``:
    the coefficient to multiply this hypersurface by; this must be
    non-negative.

Returns:
    a reference to this hypersurface.)doc";

// Docstring regina::python::doc::NormalHypersurface_::__init
static const char *__init = R"doc(Creates a new copy of the given normal hypersurface.)doc";

// Docstring regina::python::doc::NormalHypersurface_::__init_2
static const char *__init_2 =
R"doc(Creates a new copy of the given normal hypersurface, but relocated to
the given triangulation.

A snapshot will be taken of the given triangulation as it appears
right now. You may change or even delete the triangulation later on;
if so, then this normal hypersurface will still refer to the frozen
snapshot that was taken at the time of construction.

Precondition:
    The given triangulation is either the same as, or is
    combinatorially identical to, the triangulation in which *src*
    resides.

Parameter ``src``:
    the normal hypersurface to copy.

Parameter ``triangulation``:
    the triangulation in which this new hypersurface will reside.)doc";

// Docstring regina::python::doc::NormalHypersurface_::__init_3
static const char *__init_3 =
R"doc(Creates a new normal hypersurface inside the given triangulation with
the given coordinate vector, using the given vector encoding.

There is no guarantee that this hypersurface will keep the given
encoding: NormalHypersurface will sometimes convert the vector to use
a different encoding for its own internal storage.

Despite what is said in the class notes, it is okay if the given
vector encoding does not include tetrahedron coordinates. (If this is
the case, the vector will be converted automatically.)

A snapshot will be taken of the given triangulation as it appears
right now. You may change or even delete the triangulation later on;
if so, then this normal hypersurface will still refer to the frozen
snapshot that was taken at the time of construction.

Precondition:
    The given coordinate vector does indeed represent a normal
    hypersurface inside the given triangulation, using the given
    encoding. This will not be checked!

Python:
    Instead of a Vector<LargeInteger>, you may (if you prefer) pass a
    Python list of integers.

Parameter ``triang``:
    the triangulation in which this normal hypersurface resides.

Parameter ``enc``:
    indicates precisely how the given vector encodes a normal
    hypersurface.

Parameter ``vector``:
    a vector containing the coordinates of the normal hypersurface.)doc";

// Docstring regina::python::doc::NormalHypersurface_::__init_4
static const char *__init_4 =
R"doc(Creates a new normal hypersurface inside the given triangulation with
the given coordinate vector, using the given coordinate system.

It is assumed that this hypersurface uses the vector encoding
described by ``HyperEncoding(coords)``. Be careful with this if you
are extracting the vector from some other normal hypersurface, since
Regina may internally convert to use a different encoding from
whatever was used during enumeration and/or read from file. In the
same spirit, there is no guarantee that this hypersurface will use
``HyperEncoding(coords)`` as its internal encoding method.

Despite what is said in the class notes, it is okay if the given
coordinate system does not include tetrahedron coordinates. (If this
is the case, the vector will be converted automatically.)

A snapshot will be taken of the given triangulation as it appears
right now. You may change or even delete the triangulation later on;
if so, then this normal hypersurface will still refer to the frozen
snapshot that was taken at the time of construction.

Precondition:
    The given coordinate vector does indeed represent a normal
    hypersurface inside the given triangulation, using the encoding
    ``HyperEncoding(coords)``. This will not be checked!

Python:
    Instead of a Vector<LargeInteger>, you may (if you prefer) pass a
    Python list of integers.

Parameter ``triang``:
    the triangulation in which this normal hypersurface resides.

Parameter ``coords``:
    the coordinate system from which the vector encoding will be
    deduced.

Parameter ``vector``:
    a vector containing the coordinates of the normal hypersurface.)doc";

// Docstring regina::python::doc::NormalHypersurface_::__lt
static const char *__lt =
R"doc(Imposes a total order on all normal hypersurfaces.

This order is not mathematically meaningful; it is merely provided for
scenarios where you need to be able to sort hypersurfaces (e.g., when
using them as keys in a map).

The order *is* well-defined, and will be preserved across copy/move
operations, different program executions, and different platforms
(since it is defined purely in terms of the normal coordinates, and
does not use transient properties such as locations in memory).

This operation is consistent with the equality test. In particular, it
does not matter whether the two hypersurfaces belong to different
triangulations, or use different encodings, or if one but not the
other supports non-compact hypersurfaces. See the equality test
operator==() for further details.

Parameter ``other``:
    the hypersurface to be compared with this hypersurface.

Returns:
    ``True`` if and only if this appears before the given hypersurface
    in the total order.)doc";

// Docstring regina::python::doc::NormalHypersurface_::__mul
static const char *__mul =
R"doc(Returns the given integer multiple of this hypersurface.

The resulting hypersurface will use the same internal vector encoding
as this hypersurface.

Parameter ``coeff``:
    the coefficient to multiply this hypersurface by; this must be
    non-negative.

Returns:
    the resulting multiple of this hypersurface.)doc";

// Docstring regina::python::doc::NormalHypersurface_::__ne
static const char *__ne =
R"doc(Determines whether this and the given hypersurface represent different
normal hypersurfaces.

Specifically, this routine examines (or computes) the number of normal
pieces of each type, and returns ``True`` if and only if these counts
are not the same for both hypersurfaces.

It does not matter what vector encodings the two hypersurfaces use. In
particular, it does not matter if this and the given hypersurface use
different encodings, or if one but not the other supports non-compact
hypersurfaces.

This routine is safe to call even if this and the given hypersurface
do not belong to the same triangulation:

* If the two triangulations have the same size, then this routine will
  test whether this hypersurface, if transplanted into the other
  triangulation using the same pentachoron numbering and the same
  normal piece types, would be different from *other*.

* If the two triangulations have different sizes, then this routine
  will return ``True``.

Parameter ``other``:
    the hypersurface to be compared with this hypersurface.

Returns:
    ``True`` if both hypersurfaces represent different normal
    hypersurface, or ``False`` if not.)doc";

// Docstring regina::python::doc::NormalHypersurface_::doubleHypersurface
static const char *doubleHypersurface =
R"doc(Deprecated routine that returns the double of this hypersurface.

.. deprecated::
    Normal hypersurfaces can now be multiplied by integer constants.
    In particular, this routine has exactly the same effect as
    multiplying the hypersurface by 2.

Returns:
    the double of this normal hypersurface.)doc";

// Docstring regina::python::doc::NormalHypersurface_::edgeWeight
static const char *edgeWeight =
R"doc(Returns the number of times this normal hypersurface crosses the given
edge.

Parameter ``edgeIndex``:
    the index in the triangulation of the edge in which we are
    interested; this should be between 0 and
    Triangulation<4>::countEdges()-1 inclusive.

Returns:
    the number of times this normal hypersurface crosses the given
    edge.)doc";

// Docstring regina::python::doc::NormalHypersurface_::embedded
static const char *embedded =
R"doc(Determines whether this hypersurface is embedded. This is true if and
only if the surface contains no conflicting prism types.

Returns:
    ``True`` if and only if this hypersurface is embedded.)doc";

// Docstring regina::python::doc::NormalHypersurface_::encoding
static const char *encoding =
R"doc(Returns the specific integer vector encoding that this hypersurface
uses internally. This is the encoding that should be used to interpret
vector().

Note that this might differ from the encoding originally passed to the
class constructor.

Returns:
    the internal vector encoding.)doc";

// Docstring regina::python::doc::NormalHypersurface_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given normal hypersurfaces. This is a fast
(constant time) operation.

This global routine simply calls NormalHypersurface::swap(); it is
provided so that NormalHypersurface meets the C++ Swappable
requirements.

Parameter ``a``:
    the first normal hypersurface whose contents should be swapped.

Parameter ``b``:
    the second normal hypersurface whose contents should be swapped.)doc";

// Docstring regina::python::doc::NormalHypersurface_::hasRealBoundary
static const char *hasRealBoundary =
R"doc(Determines if this hypersurface has any real boundary, that is,
whether it meets any boundary tetrahedra of the triangulation.

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately.

Returns:
    ``True`` if and only if this hypersurface has real boundary.)doc";

// Docstring regina::python::doc::NormalHypersurface_::homology
static const char *homology =
R"doc(Returns the first homology group of this hypersurface.

There is an important caveat regarding invalid 4-manifold
triangulations. If the underlying triangulation has edge links that
are not spheres, then it is possible that this normal hypersurface is
not a compact 3-manifold. In such a case, this routine will compute
homology in the same way as Triangulation<3>::homology() - that is, by
effectively truncating ideal points of the hypersurface (which may
arise where the hypersurface meets an invalid edge).

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately.

Precondition:
    This normal hypersurface is compact and embedded.

.. warning::
    This routine explicitly builds the normal pieces, and so may run
    out of memory if the normal coordinates are extremely large.

Returns:
    the first homology group.)doc";

// Docstring regina::python::doc::NormalHypersurface_::isCompact
static const char *isCompact =
R"doc(Determines if this normal hypersurface is compact (has finitely many
pieces).

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately.

Returns:
    ``True`` if and only if this normal hypersurface is compact.)doc";

// Docstring regina::python::doc::NormalHypersurface_::isConnected
static const char *isConnected =
R"doc(Returns whether or not this hypersurface is connected.

For our purposes, the empty hypersurface is considered to be
connected.

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately.

Precondition:
    This normal hypersurface is compact and embedded.

.. warning::
    This routine explicitly builds the normal pieces, and so may run
    out of memory if the normal coordinates are extremely large.

Returns:
    ``True`` if this hypersurface is connected, or ``False`` if this
    hypersurface is disconnected.)doc";

// Docstring regina::python::doc::NormalHypersurface_::isEmpty
static const char *isEmpty =
R"doc(Determines if this normal hypersurface is empty (has no pieces
whatsoever).)doc";

// Docstring regina::python::doc::NormalHypersurface_::isNormalEdgeLink
static const char *isNormalEdgeLink =
R"doc(Determines whether or not a positive rational multiple of this
hypersurface is the normalised link of a single edge.

Here the phrase *normalised* link of an edge *e* means the frontier of
a regular neighbourhood of *e*, converted into a normal hypersurface
by expanding away from the edge using some basic normalisation moves.
It could be that there is no normalisation required at all (in which
case it is also a *thin* edge link). However, it could be that the
normalisation process expands the hypersurface far away from the edge
itself, or changes its topology, or disconnects the hypersurface, or
even normalises it away to an empty hypersurface.

In particular, this test behaves differently from isThinEdgeLink(),
which tests for thin edge links only (where no additional
normalisation is required). If you are only interested in thin edge
links, then you should call isThinEdgeLink(), which has much less
overhead.

A hypersurface (or its positive rational multiple) could be the
normalised link of many edges. The return value will be a pair (*v*,
*thin*), where:

* *v* is a vector containing all such edges. This will begin with the
  edges for which this hypersurface is a thin link, followed by the
  edges where normalisation was required; within each category the
  edges will be ordered by their index within the triangulation.

* *thin* is either 0 or 1, indicating how many edges this hypersurface
  is a thin link for. This uses an unsigned type, since it will often
  be compared to ``v.size()``.

If no positive rational multiple of this hypersurface is the
normalised link of any edge, then *link* will be 0 and *v* will be the
empty vector.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    a vector containing the edge(s) linked by a positive rational
    multiple of this hypersurface and an integer indicating how many
    of these links are thin, as described above.)doc";

// Docstring regina::python::doc::NormalHypersurface_::isNormalTetrahedronLink
static const char *isNormalTetrahedronLink =
R"doc(Determines whether or not a positive rational multiple of this
hypersurface is the normalised link of a single tetrahedron.

Here the phrase *normalised* link of a tetrahedron *t* means the
frontier of a regular neighbourhood of *t*, converted into a normal
hypersurface by expanding away from the tetrahedron using some basic
normalisation moves. It could be that there is no normalisation
required at all (in which case it is also a *thin* tetrahedron link).
However, it could be that the normalisation process expands the
hypersurface far away from the tetrahedron itself, or changes its
topology, or disconnects the hypersurface, or even normalises it away
to an empty hypersurface.

In particular, this test behaves differently from
isThinTetrahedronLink(), which tests for thin tetrahedron links only
(where no additional normalisation is required). Unlike the tests for
edge links, the routines isThinTetrahedronLink() and
isNormalTetrahedronLink() use essentially the same implementation (so
testing for only thin links may be a little faster, but not by much).

A hypersurface (or its positive rational multiple) could be the
normalised link of many tetrahedra. The return value will be a pair
(*v*, *thin*), where:

* *v* is a vector containing all such tetrahedra. This will begin with
  the tetrahedra for which this hypersurface is a thin link, followed
  by the tetrahedra where normalisation was required; within each
  category the tetrahedra will be ordered by their index within the
  triangulation.

* *thin* is either 0, 1 or 2, indicating how many tetrahedra this
  hypersurface is a thin link for. This uses an unsigned type, since
  it will often be compared to ``v.size()``.

If no positive rational multiple of this hypersurface is the
normalised link of any tetrahedron, then *link* will be 0 and *v* will
be the empty vector.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    a vector containing the tetrahedra linked by a positive rational
    multiple of this hypersurface and an integer indicating how many
    of these links are thin, as described above.)doc";

// Docstring regina::python::doc::NormalHypersurface_::isNormalTriangleLink
static const char *isNormalTriangleLink =
R"doc(Determines whether or not a positive rational multiple of this
hypersurface is the normalised link of a single triangle.

Here the phrase *normalised* link of a triangle *t* means the frontier
of a regular neighbourhood of *t*, converted into a normal
hypersurface by expanding away from the triangle using some basic
normalisation moves. It could be that there is no normalisation
required at all (in which case it is also a *thin* triangle link).
However, it could be that the normalisation process expands the
hypersurface far away from the triangle itself, or changes its
topology, or disconnects the hypersurface, or even normalises it away
to an empty hypersurface.

In particular, this test behaves differently from
isThinTriangleLink(), which tests for thin triangle links only (where
no additional normalisation is required). Unlike the tests for edge
links, the routines isThinTriangleLink() and isNormalTriangleLink()
use essentially the same implementation (so testing for only thin
links may be a little faster, but not by much).

A hypersurface (or its positive rational multiple) could be the
normalised link of many triangles. The return value will be a pair
(*v*, *thin*), where:

* *v* is a vector containing all such triangles. This will begin with
  the triangles for which this hypersurface is a thin link, followed
  by the triangles where normalisation was required; within each
  category the triangles will be ordered by their index within the
  triangulation.

* *thin* is either 0, 1 or 2, indicating how many triangles this
  hypersurface is a thin link for. This uses an unsigned type, since
  it will often be compared to ``v.size()``.

If no positive rational multiple of this hypersurface is the
normalised link of any triangle, then *link* will be 0 and *v* will be
the empty vector.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    a vector containing the triangle(s) linked by a positive rational
    multiple of this hypersurface and an integer indicating how many
    of these links are thin, as described above.)doc";

// Docstring regina::python::doc::NormalHypersurface_::isOrientable
static const char *isOrientable =
R"doc(Returns whether or not this hypersurface is orientable.

For our purposes, the empty hypersurface is considered to be
orientable.

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately.

Precondition:
    This normal hypersurface is compact and embedded.

.. warning::
    This routine explicitly builds the normal pieces, and so may run
    out of memory if the normal coordinates are extremely large.

Returns:
    ``True`` if this hypersurface is orientable, or ``False`` if this
    hypersurface is non-orientable.)doc";

// Docstring regina::python::doc::NormalHypersurface_::isThinEdgeLink
static const char *isThinEdgeLink =
R"doc(Determines whether or not a positive rational multiple of this
hypersurface is the thin link of a single edge.

Here a *thin* edge link is a normal hypersurface which appears
naturally as the frontier of a regular neighbourhood of an edge, with
no need for any further normalisation.

This behaves differently from isNormalEdgeLink(), which tests for a
*normalised* edge link (which could end up far away from the edge, or
could be normalised into a hypersurface with different topology, or
could even be normalised away to nothing). Although isNormalEdgeLink()
will also indicate thin edge links, this test has significantly less
overhead (and so should be faster).

A hypersurface (or its positive rational multiple) can be the thin
edge link of at most one edge.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    the edge linked by a positive rational multiple of this
    hypersurface, or ``null`` if this hypersurface is not a multiple
    of a single thin edge link.)doc";

// Docstring regina::python::doc::NormalHypersurface_::isThinTetrahedronLink
static const char *isThinTetrahedronLink =
R"doc(Determines whether or not a positive rational multiple of this
hypersurface is the thin link of a single tetrahedron.

Here a *thin* tetrahedron link is a normal hypersurface which appears
naturally as the frontier of a regular neighbourhood of a tetrahedron,
with no need for any further normalisation.

This behaves differently from isNormalTetrahedronLink(), which tests
for a *normalised* tetrahedron link (which could end up far away from
the tetrahedron, or could be normalised into a hypersurface with
different topology, or could even be normalised away to nothing).
Unlike the tests for edge links, the routines isThinTetrahedronLink()
and isNormalTetrahedronLink() use essentially the same implementation
(so testing for only thin links may be a little faster, but not by
much).

A hypersurface (or its positive rational multiple) can be the thin
link of at most two tetrahedra. If there are indeed two different
tetrahedra *t1* and *t2* for which a multiple of this hypersurface can
be expressed as the thin tetrahedron link, then the pair (*t1*, *t2*)
will be returned. If there is only one such tetrahedron *t*, then the
pair (*t*, ``null``) will be returned. If no positive rational
multiple of this hypersurface is the thin link of any tetrahedron,
then the pair (``null``, ``null``) will be returned.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    a pair containing the tetrahedra linked by a positive rational
    multiple of this hypersurface, as described above.)doc";

// Docstring regina::python::doc::NormalHypersurface_::isThinTriangleLink
static const char *isThinTriangleLink =
R"doc(Determines whether or not a positive rational multiple of this
hypersurface is the thin link of a single triangle.

Here a *thin* triangle link is a normal hypersurface which appears
naturally as the frontier of a regular neighbourhood of a triangle,
with no need for any further normalisation.

This behaves differently from isNormalTriangleLink(), which tests for
a *normalised* triangle link (which could end up far away from the
triangle, or could be normalised into a hypersurface with different
topology, or could even be normalised away to nothing). Unlike the
tests for edge links, the routines isThinTriangleLink() and
isNormalTriangleLink() use essentially the same implementation (so
testing for only thin links may be a little faster, but not by much).

A hypersurface (or its positive rational multiple) can be the thin
triangle link of at most two triangles. If there are indeed two
different triangles *t1* and *t2* for which a multiple of this
hypersurface can be expressed as the thin triangle link, then the pair
(*t1*, *t2*) will be returned. If there is only one such triangle *t*,
then the pair (*t*, ``null``) will be returned. If no positive
rational multiple of this hypersurface is the thin link of any
triangle, then the pair (``null``, ``null``) will be returned.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    a pair containing the triangle(s) linked by a positive rational
    multiple of this hypersurface, as described above.)doc";

// Docstring regina::python::doc::NormalHypersurface_::isTwoSided
static const char *isTwoSided =
R"doc(Returns whether or not this hypersurface is two-sided.

For our purposes, the empty hypersurface is considered to be two-
sided.

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately.

Precondition:
    This normal hypersurface is compact and embedded.

.. warning::
    This routine explicitly builds the normal pieces, and so may run
    out of memory if the normal coordinates are extremely large.

Returns:
    ``True`` if this hypersurface is two-sided, or ``False`` if this
    hypersurface is one-sided.)doc";

// Docstring regina::python::doc::NormalHypersurface_::isVertexLink
static const char *isVertexLink =
R"doc(Determines whether or not a positive rational multiple of this
hypersurface is the link of a single vertex.

This behaves differently from isVertexLinking(), which will also
detect a union of several different vertex links. In contrast, this
routine will only identify the link of a *single* vertex (or a
multiple of such a link).

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    the vertex linked by a positive rational multiple of this
    hypersurface, or ``null`` if this hypersurface is not a multiple
    of a single vertex link.)doc";

// Docstring regina::python::doc::NormalHypersurface_::isVertexLinking
static const char *isVertexLinking =
R"doc(Determines whether or not this hypersurface is vertex linking. A
*vertex linking* hypersurface contains only tetrahedra.

This behaves differently from isVertexLink(), which only detects the
link of a single vertex (or a multiple of such a link). In contrast,
this routine will also detect the union of several *different* vertex
links.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    ``True`` if and only if this hypersurface is vertex linking.)doc";

// Docstring regina::python::doc::NormalHypersurface_::locallyCompatible
static const char *locallyCompatible =
R"doc(Determines whether this and the given hypersurface are locally
compatible. Local compatibility means that, within each individual
pentachoron of the triangulation, it is possible to arrange the normal
discs of both hypersurfaces so that none intersect.

This is a local constraint, not a global constraint. That is, we do
not insist that we can avoid intersections within all pentachora
*simultaneously*.

Local compatibility can be formulated in terms of normal piece types.
Two normal hypersurfaces are locally compatible if and only if they
together use at most two prism piece types per pentachoron; moreover,
if there *are* two prism piece types within a single pentachoron then
these prism types are non-intersecting.

If one of the two hypersurfaces breaks the local compatibility
constraints on its own (for instance, it contains two conflicting
prism types within the same pentachoron), then this routine will
return ``False`` regardless of what the other hypersurface contains.

Precondition:
    Both this and the given normal hypersurface live within the same
    4-manifold triangulation.

Parameter ``other``:
    the other hypersurface to test for local compatibility with this
    hypersurface.

Returns:
    ``True`` if the two hypersurfaces are locally compatible, or
    ``False`` if they are not.)doc";

// Docstring regina::python::doc::NormalHypersurface_::name
static const char *name =
R"doc(Returns the name associated with this normal hypersurface. Names are
optional and need not be unique. The default name for a hypersurface
is the empty string.

Returns:
    the name of associated with this hypersurface.)doc";

// Docstring regina::python::doc::NormalHypersurface_::prisms
static const char *prisms =
R"doc(Returns the number of prism pieces of the given type in this normal
hypersurface. A prism piece type is identified by specifying a
pentachoron and an edge of that pentachoron; prisms of this type will
then separate edge *i* of the pentachoron from triangle *i* of the
pentachoron.

Parameter ``pentIndex``:
    the index in the triangulation of the pentachoron in which the
    requested prisms reside; this should be between 0 and
    Triangulation<4>::size()-1 inclusive.

Parameter ``prismType``:
    specifies the edge of the given pentachoron that this prism
    separates from the opposite triangle; this should be between 0 and
    9 inclusive.

Returns:
    the number of prism pieces of the given type.)doc";

// Docstring regina::python::doc::NormalHypersurface_::reconstructTetrahedra
static const char *reconstructTetrahedra =
R"doc(Reconstructs the tetrahedron coordinates in the given integer vector.

The given vector must represent a normal hypersurface within the given
triangulation, using the given vector encoding.

* If the given encoding does not already store tetrahedron
  coordinates, then the vector will be modified directly to use a new
  encoding that does, and this new encoding will be returned.

* If the given encoding does already store tetrahedra, then this
  routine will do nothing and immediately return *enc*.

Parameter ``tri``:
    the triangulation in which the normal hypersurface lives.

Parameter ``vector``:
    an integer vector that encodes a normal hypersurface within *tri*;
    this will be modified directly.

Parameter ``enc``:
    the encoding used by the given integer vector.

Returns:
    the new encoding used by the modified *vector*.)doc";

// Docstring regina::python::doc::NormalHypersurface_::scaleDown
static const char *scaleDown =
R"doc(Converts this hypersurface into its smallest positive rational
multiple with integer coordinates.

Note that the scaling factor will be independent of which internal
vector encoding is used. This is essentially because integer prism
coordinates (which are stored in every encoding) are enough to
guarantee integer tetrahedron coordinates (which might or might not be
stored).

Returns:
    the integer by which the original hypersurface was divided (i.e.,
    the gcd of all normal coordinates in the original hypersurface).
    This will always be strictly positive.)doc";

// Docstring regina::python::doc::NormalHypersurface_::setName
static const char *setName =
R"doc(Sets the name associated with this normal hypersurface. Names are
optional and need not be unique. The default name for a hypersurface
is the empty string.

Parameter ``name``:
    the new name to associate with this hypersurface.)doc";

// Docstring regina::python::doc::NormalHypersurface_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given normal hypersurface. This is
a fast (constant time) operation.

This and the given normal hypersurface do not need to live in the same
underlying triangulation, and they do not need to have the same length
vectors or use the same normal coordinate system - if any of these
properties differs then the two hypersurfaces will be adjusted
accordingly.

Parameter ``other``:
    the normal hypersurface whose contents should be swapped with
    this.)doc";

// Docstring regina::python::doc::NormalHypersurface_::tetrahedra
static const char *tetrahedra =
R"doc(Returns the number of tetrahedron pieces of the given type in this
normal hypersurface. A tetrahedron piece type is identified by
specifying a pentachoron and a vertex of that pentachoron that the
tetrahedron surrounds.

Parameter ``pentIndex``:
    the index in the triangulation of the pentachoron in which the
    requested pieces reside; this should be between 0 and
    Triangulation<4>::size()-1 inclusive.

Parameter ``vertex``:
    the vertex of the given pentachoron around which the requested
    pieces lie; this should be between 0 and 4 inclusive.

Returns:
    the number of tetrahedron pieces of the given type.)doc";

// Docstring regina::python::doc::NormalHypersurface_::triangulate
static const char *triangulate =
R"doc(Returns a 3-manifold triangulation describing this normal
hypersurface.

The triangulation will be simplified via
Triangulation<3>::intelligentSimplify(), which means that the
tetrahedra of the final triangulation are not likely to correspond to
any particular tetrahedron/prism pieces of this normal hypersurface.

Precondition:
    This normal hypersurface is compact and embedded.

Returns:
    a triangulation of this normal hypersurface.)doc";

// Docstring regina::python::doc::NormalHypersurface_::triangulation
static const char *triangulation =
R"doc(Returns the triangulation in which this normal hypersurface resides.

This will be a snapshot frozen in time of the triangulation that was
originally passed to the NormalHypersurface constructor.

This will return a correct result even if the original triangulation
has since been modified or destroyed. However, in order to ensure this
behaviour, it is possible that at different points in time this
function may return references to different C++ objects.

The rules for using the triangulation() reference are:

* Do not keep the resulting reference as a long-term reference or
  pointer of your own, since in time you may find yourself referring
  to the wrong object (see above). Just call this function again.

* You must respect the read-only nature of the result (i.e., you must
  not cast the constness away). The snapshotting process detects
  modifications, and modifying the frozen snapshot may result in an
  exception being thrown.

Returns:
    a reference to the underlying triangulation.)doc";

// Docstring regina::python::doc::NormalHypersurface_::vector
static const char *vector =
R"doc(Gives read-only access to the integer vector that Regina uses
internally to represent this hypersurface.

Note that this vector might not use the same coordinate system in
which the hypersurfaces were originally enumerated. (For example, this
vector will always include tetrahedron coordinates, even if the
surfaces were originally enumerated in prism coordinates.) You can
call encoding() to find out precisley how the coordinates of this
vector should be interpreted.

See the NormalHypersurface class notes for information on how this
vector is structured.

.. note::
    If you wish to access the numbers of tetrahedra, prisms and so on,
    you should use the functions tetrahedra(), prisms(), etc., which
    do not require any knowledge of the internal vector encoding that
    this hypersurface uses.

Returns:
    the underlying integer vector.)doc";

// Docstring regina::python::doc::NormalHypersurface_::writeXMLData
static const char *writeXMLData =
R"doc(Writes a chunk of XML containing this normal hypersurface and all of
its properties. This routine will be called from within
NormalHypersurfaces::writeXMLPacketData().

Python:
    The argument *out* should be an open Python file object.

Parameter ``out``:
    the output stream to which the XML should be written.

Parameter ``format``:
    indicates which of Regina's XML file formats to write.

Parameter ``list``:
    the enclosing normal hypersurface list. Currently this is only
    relevant when writing to the older REGINA_XML_GEN_2 format; it
    will be ignored (and may be ``null``) for newer file formats.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

