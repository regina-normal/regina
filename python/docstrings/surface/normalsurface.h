/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::NormalSurface
static const char *NormalSurface =
R"doc(Represents a single normal surface in a 3-manifold triangulation.

The normal surface is described internally by an integer vector
(discussed in more detail below). Since different surfaces may use
different vector encodings, you should not rely on the raw vector
entries unless absolutely necessary. Instead, the query routines such
as triangles(), quads(), edgeWeight() and so on are independent of the
underlying vector encoding being used.

Note that non-compact surfaces (surfaces with infinitely many discs,
such as spun-normal surfaces) are allowed; in these cases, the
corresponding lookup routines (such as triangles()) will return
LargeInteger::infinity where appropriate.

Since Regina 7.0, you can modify or even destroy the original
triangulation that was used to create this normal surface. If you do,
then this normal surface will automatically make a private copy of the
original triangulation as an ongoing reference. Different normal
surfaces (and angle structures) can all share the same private copy,
so this is not an expensive process.

Internally, a normal surface is represented by a Vector<LargeInteger>
(possibly using a different coordinate system from the one in which
the surfaces were originally enumerated). This contains a block of
coordinates for each tetrahedron, in order from the first tetrahedron
to the last. Each block begins with four triangle coordinates
(always), then three quadrilateral coordinates (always), and finally
three octagon coordinates (only for some coordinate systems).
Therefore the vector that is stored will always have length 7*n* or
10*n* where *n* is the number of tetrahedra in the underlying
triangulation.

When adding support for a new coordinate system:

* The file normalcoords.h must be updated. This includes a new enum
  value for NormalCoords, a new case for the NormalEncoding
  constructor, and new cases for the functions in NormalInfo. Do not
  forget to update the python bindings for NormalCoords also.

* The global routines makeEmbeddedConstraints() and
  makeMatchingEquations() should be updated to incorporate the new
  coordinate system.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace NormalSurface_ {

// Docstring regina::python::doc::NormalSurface_::__add
static const char *__add =
R"doc(Returns the sum of this and the given surface. This will combine all
triangles, quadrilaterals and/or octagons from both surfaces.

The two surfaces do not need to use the same coordinate system and/or
internal vector encodings. Moreover, the resulting surface might well
use an encoding different from both of these, or even a hybrid
encoding that does not come from one of Regina's ready-made coordinate
systems.

Precondition:
    Both this and the given normal surface use the same underlying
    triangulation.

Parameter ``rhs``:
    the surface to sum with this.

Returns:
    the sum of both normal surfaces.)doc";

// Docstring regina::python::doc::NormalSurface_::__copy
static const char *__copy = R"doc(Creates a new copy of the given normal surface.)doc";

// Docstring regina::python::doc::NormalSurface_::__eq
static const char *__eq =
R"doc(Determines whether this and the given surface in fact represent the
same normal (or almost normal) surface.

Specifically, this routine examines (or computes) the number of normal
or almost normal discs of each type, and returns ``True`` if and only
if these counts are the same for both surfaces.

It does not matter what vector encodings the two surfaces use. In
particular, it does not matter if the two surfaces use different
encodings, or if one but not the other supports almost normal and/or
spun-normal surfaces.

This routine is safe to call even if this and the given surface do not
belong to the same triangulation:

* If the two triangulations have the same size, then this routine will
  test whether this surface, if transplanted into the other
  triangulation using the same tetrahedron numbering and the same
  normal disc types, would be the same as *other*.

* If the two triangulations have different sizes, then this routine
  will return ``False``.

Parameter ``other``:
    the surface to be compared with this surface.

Returns:
    ``True`` if both surfaces represent the same normal or almost
    normal surface, or ``False`` if not.)doc";

// Docstring regina::python::doc::NormalSurface_::__imul
static const char *__imul =
R"doc(Converts this surface into the given integer multiple of itself.

The internal vector encoding used by this surface will not change.

Parameter ``coeff``:
    the coefficient to multiply this surface by; this must be non-
    negative.

Returns:
    a reference to this surface.)doc";

// Docstring regina::python::doc::NormalSurface_::__init
static const char *__init =
R"doc(Creates a new copy of the given normal surface, but relocated to the
given triangulation.

A snapshot will be taken of the given triangulation as it appears
right now. You may change or even delete the triangulation later on;
if so, then this normal surface will still refer to the frozen
snapshot that was taken at the time of construction.

Precondition:
    The given triangulation is either the same as, or is
    combinatorially identical to, the triangulation in which *src*
    resides.

Parameter ``src``:
    the normal surface to copy.

Parameter ``triangulation``:
    the triangulation in which this new surface will reside.)doc";

// Docstring regina::python::doc::NormalSurface_::__init_2
static const char *__init_2 =
R"doc(Create the empty surface within the given triangulation.

All normal coordinates will be zero.

Parameter ``triang``:
    the triangulation in which this normal surface resides.)doc";

// Docstring regina::python::doc::NormalSurface_::__init_3
static const char *__init_3 =
R"doc(Creates a new normal surface inside the given triangulation with the
given coordinate vector, using the given vector encoding.

There is no guarantee that this surface will keep the given encoding:
NormalSurface will sometimes convert the vector to use a different
encoding for its own internal storage.

Despite what is said in the class notes, it is okay if the given
vector encoding does not include triangle coordinates. (If this is the
case, the vector will be converted automatically.)

A snapshot will be taken of the given triangulation as it appears
right now. You may change or even delete the triangulation later on;
if so, then this normal surface will still refer to the frozen
snapshot that was taken at the time of construction.

Precondition:
    The given coordinate vector does indeed represent a normal surface
    inside the given triangulation, using the given encoding. This
    will not be checked!

Python:
    The supported types for the template parameter *U* are
    regina::Integer and regina::LargeInteger. You may also, if you
    prefer, pass *vector* as a Python list of integers.

Template parameter ``U``:
    the type of object held by the given vector. It must be possible
    to assign an object of type *U* to a regina::LargeInteger.

Parameter ``triang``:
    the triangulation in which this normal surface resides.

Parameter ``enc``:
    indicates precisely how the given vector encodes a normal surface.

Parameter ``vector``:
    a vector containing the coordinates of the normal surface.)doc";

// Docstring regina::python::doc::NormalSurface_::__init_4
static const char *__init_4 =
R"doc(Creates a new normal surface inside the given triangulation with the
given coordinate vector, using the given coordinate system.

It is assumed that this surface uses the vector encoding described by
``NormalEncoding(coords)``. Be careful with this if you are extracting
the vector from some other normal surface, since Regina may internally
convert to use a different encoding from whatever was used during
enumeration and/or read from file. In the same spirit, there is no
guarantee that this surface will use ``NormalEncoding(coords)`` as its
internal encoding method.

Despite what is said in the class notes, it is okay if the given
coordinate system does not include triangle coordinates. (If this is
the case, the vector will be converted automatically.)

A snapshot will be taken of the given triangulation as it appears
right now. You may change or even delete the triangulation later on;
if so, then this normal surface will still refer to the frozen
snapshot that was taken at the time of construction.

Precondition:
    The given coordinate vector does indeed represent a normal surface
    inside the given triangulation, using the encoding
    ``NormalEncoding(coords)``. This will not be checked!

Python:
    The supported types for the template parameter *U* are
    regina::Integer and regina::LargeInteger. You may also, if you
    prefer, pass *vector* as a Python list of integers.

Template parameter ``U``:
    the type of object held by the given vector. It must be possible
    to assign an object of type *U* to a regina::LargeInteger.

Parameter ``triang``:
    the triangulation in which this normal surface resides.

Parameter ``coords``:
    the coordinate system from which the vector encoding will be
    deduced.

Parameter ``vector``:
    a vector containing the coordinates of the normal surface.)doc";

// Docstring regina::python::doc::NormalSurface_::__lt
static const char *__lt =
R"doc(Imposes a total order on all normal and almost normal surfaces.

This order is not mathematically meaningful; it is merely provided for
scenarios where you need to be able to sort surfaces (e.g., when using
them as keys in a map).

The order _is_ well-defined, and will be preserved across copy/move
operations, different program executions, and different platforms
(since it is defined purely in terms of the normal coordinates, and
does not use transient properties such as locations in memory).

This operation is consistent with the equality test. In particular, it
does not matter whether the two surfaces belong to different
triangulations, or use different encodings, or if one but not the
other supports non-compact or almost normal surfaces. See the equality
test operator==() for further details.

Parameter ``other``:
    the surface to be compared with this surface.

Returns:
    ``True`` if and only if this appears before the given surface in
    the total order.)doc";

// Docstring regina::python::doc::NormalSurface_::__mul
static const char *__mul =
R"doc(Returns the given integer multiple of this surface.

The resulting surface will use the same internal vector encoding as
this surface.

Parameter ``coeff``:
    the coefficient to multiply this surface by; this must be non-
    negative.

Returns:
    the resulting multiple of this surface.)doc";

// Docstring regina::python::doc::NormalSurface_::arcs
static const char *arcs =
R"doc(Returns the number of arcs in which this normal surface intersects the
given triangle in the given direction.

Parameter ``triIndex``:
    the index in the triangulation of the triangle in which we are
    interested; this should be between 0 and
    Triangulation<3>::countTriangles()-1 inclusive.

Parameter ``triVertex``:
    the vertex of the triangle (0, 1 or 2) around which the arcs of
    intersection that we are interested in lie; only these arcs will
    be counted.

Returns:
    the number of times this normal surface intersect the given
    triangle with the given arc type.)doc";

// Docstring regina::python::doc::NormalSurface_::boundaryIntersections
static const char *boundaryIntersections =
R"doc(Computes the information about the boundary slopes of this surface at
each cusp of the triangulation. This is for use with spun-normal
surfaces (since for closed surfaces all boundary slopes are zero).

This routine is only available for use with SnapPea triangulations,
since it needs to know the specific meridian and longitude on each
cusp. These meridians and longitudes are _only_ available through the
SnapPea kernel, since Regina does not use or store peripheral curves
for its own Triangulation<3> class. Therefore:

* If the underlying triangulation (as returned by triangulation()) is
  not of the subclass SnapPeaTriangulation, this routine will throw an
  exception (see below).

* In particular, this _will_ happen if you have edited or deleted the
  original triangulation that was used to construct this normal
  surface. This is because such a modification will trigger an
  internal deep copy of the original, and this will only copy Regina's
  native Triangulation<3> data.

All cusps are treated as complete. That is, any Dehn fillings stored
in the SnapPea triangulation will be ignored.

The results are returned in a matrix with *V* rows and two columns,
where *V* is the number of vertices in the triangulation. If row *i*
of the matrix contains the integers *M* and *L*, this indicates that
at the *i*th cusp, the boundary curves have algebraic intersection
number *M* with the meridian and *L* with the longitude. Equivalently,
the boundary curves pass *L* times around the meridian and *-M* times
around the longitude. The rational boundary slope is therefore
``-L/M``, and there are ``gcd(L,M)`` boundary curves with this slope.

The orientations of the boundary curves of a spun-normal surface are
chosen so that _if_ meridian and longitude are a positive basis as
vieved from the cusp, then as one travels along an oriented boundary
curve, the spun-normal surface spirals into the cusp to one's right
and down into the manifold to one's left.

If the triangulation contains more than one vertex, the rows in the
resulting matrix are ordered by cusp index (as stored by SnapPea). You
can call SnapPeaTriangulation::cuspVertex() to map these to Regina's
vertex indices if needed.

Precondition:
    As noted above, the underlying triangulation must be a
    SnapPeaTriangulation; this will be checked, and this routine will
    throw an exception if this requirement is not met.

Precondition:
    At present, Regina can only compute boundary slopes if the
    triangulation is oriented, if every vertex link in the
    triangulation is a torus, and if the underlying coordinate system
    is for normal surfaces only (not almost normal surfaces). These
    conditions will likewise be checked, and this routine will throw
    an exception if they are not met.

Exception ``SnapPeaIsNull``:
    This is a null SnapPea triangulation.

Exception ``FailedPrecondition``:
    One or more of the preconditions listed above was not met.

Author:
    William Pettersson and Stephan Tillmann

Returns:
    a matrix with *number_of_vertices* rows and two columns as
    described above.)doc";

// Docstring regina::python::doc::NormalSurface_::components
static const char *components =
R"doc(Splits this surface into connected components.

A list of connected components will be returned. These components will
always be encoded using standard (tri-quad or tri-quad-oct)
coordinates, regardless of the internal vector encoding that is used
by this surface.

Precondition:
    This normal surface is embedded (not singular or immersed).

Precondition:
    This normal surface is compact (has finitely many discs).

.. warning::
    This routine explicitly builds the normal discs, and so may run
    out of memory if the normal coordinates are extremely large.

Returns:
    the list of connected components.)doc";

// Docstring regina::python::doc::NormalSurface_::couldBeAlmostNormal
static const char *couldBeAlmostNormal =
R"doc(Indicates whether the internal vector encoding for this surface
supports almost normal surfaces.

If this routine returns ``True``, it does not mean that the surface
actually _contains_ one or more octagons; you should use normal() to
test for that. This routine simply queries a basic property of the
vector encoding that is being used, and this property is often
inherited from whatever coordinate system was used to perform the
normal surface enumeration.

On the other hand, if this routine returns ``False``, it is a
guarantee that this surface is normal.

Returns:
    ``True`` if the internal encoding supports almost normal surfaces.)doc";

// Docstring regina::python::doc::NormalSurface_::couldBeNonCompact
static const char *couldBeNonCompact =
R"doc(Indicates whether the internal vector encoding for this surface
supports non-compact surfaces. Non-compact surfaces are surfaces that
contain infinitely many discs (i.e., spun-normal surfaces).

If this routine returns ``True``, it does not mean that the surface
actually _is_ non-compact; you should use isCompact() to test for
that. This routine simply queries a basic property of the vector
encoding that is being used, and this property is often inherited from
whatever coordinate system was used to perform the normal surface
enumeration.

On the other hand, if this routine returns ``False``, it is a
guarantee that this surface is compact.

Returns:
    ``True`` if the internal encoding supports almost normal surfaces.)doc";

// Docstring regina::python::doc::NormalSurface_::countBoundaries
static const char *countBoundaries =
R"doc(Returns the number of disjoint boundary curves on this surface.

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately.

Precondition:
    This normal surface is embedded (not singular or immersed).

Precondition:
    This normal surface is compact (has finitely many discs).

.. warning::
    This routine explicitly builds the normal arcs on the boundary. If
    the normal coordinates are extremely large, (in particular, of a
    similar order of magnitude as the largest possible long integer),
    then the behaviour of this routine is undefined.

Author:
    Alex He

Returns:
    the number of disjoint boundary curves.)doc";

// Docstring regina::python::doc::NormalSurface_::crush
static const char *crush =
R"doc(Crushes this surface to a point in the underlying triangulation and
returns the result as a new triangulation. The original triangulation
is not changed.

Crushing the surface will produce a number of tetrahedra, triangular
pillows and/or footballs. The pillows and footballs will then be
flattened to triangles and edges respectively (resulting in the
possible changes mentioned below) to produce a proper triangulation.

Note that the new triangulation will have at most the same number of
tetrahedra as the old triangulation, and will have strictly fewer
tetrahedra if this surface is not vertex linking.

The act of flattening pillows and footballs as described above can
lead to unintended topological side-effects, beyond the effects of
merely cutting along this surface and identifying the new boundary
surface(s) to points. Examples of these unintended side-effects can
include connected sum decompositions, removal of 3-spheres and small
Lens spaces and so on; a full list of possible changes is beyond the
scope of this API documentation.

This routine will ignore any locks on tetrahedra and/or triangles of
the original triangulation (and of course the original triangulation
will be left safely unchanged). The triangulation that is returned
will not have any locks at all.

.. warning::
    This routine can have unintended topological side-effects, as
    described above.

.. warning::
    In exceptional cases with non-orientable 3-manifolds, these side-
    effects might lead to invalid edges (edges whose midpoints are
    projective plane cusps).

Precondition:
    This normal surface is compact and embedded.

Precondition:
    This normal surface contains no octagonal discs.

Returns:
    the resulting crushed triangulation.)doc";

// Docstring regina::python::doc::NormalSurface_::cutAlong
static const char *cutAlong =
R"doc(Cuts the underlying triangulation along this surface and returns the
result as a new triangulation. The original triangulation is not
changed.

Note that, unlike crushing a surface to a point, this operation will
not change the topology of the underlying 3-manifold beyond simply
slicing along this surface.

As of Regina 7.1, this routine can happily cut along _almost_ normal
surfaces as well as normal surfaces. That is, it can now handle
octagons, including cases with multiple octagons in the same
tetrahedron and/or octagons in multiple tetrahedra.

This routine will ignore any locks on tetrahedra and/or triangles of
the original triangulation (and of course the original triangulation
will be left safely unchanged). The triangulation that is returned
will not have any locks at all.

.. warning::
    The number of tetrahedra in the new triangulation can be _very_
    large.

Precondition:
    This normal surface is compact and embedded.

Returns:
    the resulting cut-open triangulation.)doc";

// Docstring regina::python::doc::NormalSurface_::disjoint
static const char *disjoint =
R"doc(Determines whether this and the given surface can be placed within the
surrounding triangulation so that they do not intersect anywhere at
all, without changing either normal isotopy class.

This is a global constraint, and therefore gives a stronger test than
locallyCompatible(). However, this global constraint is also much
slower to test; the running time is proportional to the total number
of normal discs in both surfaces.

Precondition:
    Both this and the given normal surface live within the same
    3-manifold triangulation.

Precondition:
    Both this and the given surface are compact (have finitely many
    discs), embedded, non-empty and connected.

.. warning::
    This routine is slow, since it performs a depth-first search over
    the entire set of normal discs.

Parameter ``other``:
    the other surface to test alongside this surface for potential
    intersections.

Returns:
    ``True`` if both surfaces can be embedded without intersecting
    anywhere, or ``False`` if this and the given surface are forced to
    intersect at some point.)doc";

// Docstring regina::python::doc::NormalSurface_::doubleSurface
static const char *doubleSurface =
R"doc(Deprecated routine that returns the double of this surface.

.. deprecated::
    Normal surfaces can now be multiplied by integer constants. In
    particular, this routine has exactly the same effect as
    multiplying the surface by 2.

Returns:
    the double of this normal surface.)doc";

// Docstring regina::python::doc::NormalSurface_::edgeWeight
static const char *edgeWeight =
R"doc(Returns the number of times this normal surface crosses the given
edge.

Parameter ``edgeIndex``:
    the index in the triangulation of the edge in which we are
    interested; this should be between 0 and
    Triangulation<3>::countEdges()-1 inclusive.

Returns:
    the number of times this normal surface crosses the given edge.)doc";

// Docstring regina::python::doc::NormalSurface_::embedded
static const char *embedded =
R"doc(Determines whether this surface is embedded. This is true if and only
if the surface contains no conflicting quadrilateral and/or octagon
types.

Returns:
    ``True`` if and only if this surface is embedded.)doc";

// Docstring regina::python::doc::NormalSurface_::encoding
static const char *encoding =
R"doc(Returns the specific integer vector encoding that this surface uses
internally. This is the encoding that should be used to interpret
vector().

Note that this might differ from the encoding originally passed to the
class constructor.

Returns:
    the internal vector encoding.)doc";

// Docstring regina::python::doc::NormalSurface_::eulerChar
static const char *eulerChar =
R"doc(Returns the Euler characteristic of this surface.

For properly embedded surfaces, this is of course just the ordinary
Euler characteristic of the surface.

For immersed or singular surfaces, the situation is more complex since
Regina does not know how many branch points there are (if any).
Regina's approach is to compute everything locally, assuming that the
surface is an immersion. This means that eulerChar() will report the
correct result for an immersed surface, but for singular surfaces it
will report a _larger_ number than it should since it essentially
counts each branch point as multiple vertices.

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately.

Precondition:
    This normal surface is compact (has finitely many discs).

Returns:
    the Euler characteristic.)doc";

// Docstring regina::python::doc::NormalSurface_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given normal surfaces. This is a fast
(constant time) operation.

This global routine simply calls NormalSurface::swap(); it is provided
so that NormalSurface meets the C++ Swappable requirements.

Parameter ``a``:
    the first normal surface whose contents should be swapped.

Parameter ``b``:
    the second normal surface whose contents should be swapped.)doc";

// Docstring regina::python::doc::NormalSurface_::hasMultipleOctDiscs
static const char *hasMultipleOctDiscs =
R"doc(Determines if this normal surface has more than one octagonal disc. It
may be assumed that at most one octagonal disc _type_ exists in this
surface. This routine will return ``True`` if an octagonal type does
exist and its coordinate is greater than one.

Precondition:
    At most one octagonal disc _type_ exists in this surface.

Returns:
    ``True`` if and only if there is an octagonal disc type present
    and its coordinate is greater than one.)doc";

// Docstring regina::python::doc::NormalSurface_::hasRealBoundary
static const char *hasRealBoundary =
R"doc(Determines if this surface has any real boundary, that is, whether it
meets any boundary triangles of the triangulation.

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately.

Returns:
    ``True`` if and only if this surface has real boundary.)doc";

// Docstring regina::python::doc::NormalSurface_::isCentral
static const char *isCentral =
R"doc(Determines whether or not this surface is a central surface. A
*central* surface is a compact surface containing at most one normal
or almost normal disc per tetrahedron. If this surface is central, the
number of tetrahedra that it meets (i.e., the number of discs in the
surface) will be returned.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    the number of tetrahedra that this surface meets if it is a
    central surface, or 0 if it is not a central surface.)doc";

// Docstring regina::python::doc::NormalSurface_::isCompact
static const char *isCompact =
R"doc(Determines if this normal surface is compact (has finitely many
discs).

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately.

Returns:
    ``True`` if and only if this normal surface is compact.)doc";

// Docstring regina::python::doc::NormalSurface_::isCompressingDisc
static const char *isCompressingDisc =
R"doc(Determines whether this surface represents a compressing disc in the
underlying 3-manifold.

Let this surface be *D* and let the underlying 3-manifold be *M* with
boundary *B*. To be a compressing disc, *D* must be a properly
embedded disc in *M*, and the boundary of *D* must not bound a disc in
*B*.

The implementation of this routine is somewhat inefficient at present,
since it cuts along the disc, retriangulates and then examines the
resulting boundary components.

Precondition:
    This normal surface is compact and embedded.

Precondition:
    This normal surface contains no octagonal discs.

.. warning::
    This routine might cut along the surface and retriangulate, and so
    may run out of memory if the normal coordinates are extremely
    large.

Parameter ``knownConnected``:
    ``True`` if this normal surface is already known to be connected
    (for instance, if it came from an enumeration of vertex normal
    surfaces), or ``False`` if we should not assume any such
    information about this surface.

Returns:
    ``True`` if this surface is a compressing disc, or ``False`` if
    this surface is not a compressing disc.)doc";

// Docstring regina::python::doc::NormalSurface_::isConnected
static const char *isConnected =
R"doc(Returns whether or not this surface is connected.

For our purposes, the empty surface is considered to be connected.

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately.

Precondition:
    This normal surface is embedded (not singular or immersed).

Precondition:
    This normal surface is compact (has finitely many discs).

.. warning::
    This routine explicitly builds the normal discs, and so may run
    out of memory if the normal coordinates are extremely large.

Returns:
    ``True`` if this surface is connected, or ``False`` if this
    surface is disconnected.)doc";

// Docstring regina::python::doc::NormalSurface_::isEmpty
static const char *isEmpty = R"doc(Determines if this normal surface is empty (has no discs whatsoever).)doc";

// Docstring regina::python::doc::NormalSurface_::isIncompressible
static const char *isIncompressible =
R"doc(Determines whether this is an incompressible surface within the
surrounding 3-manifold. At present, this routine is only implemented
for surfaces embedded within _closed_ and _irreducible_ 3-manifold
triangulations.

Let *D* be some disc embedded in the underlying 3-manifold, and let
*B* be the boundary of *D*. We call *D* a _compressing disc_ for this
surface if (i) the intersection of *D* with this surface is the
boundary *B*, and (ii) although *B* bounds a disc within the
3-manifold, it does not bound a disc within this surface.

We declare this surface to be _incompressible_ if there are no such
compressing discs. For our purposes, spheres are never considered
incompressible (so if this surface is a sphere then this routine will
always return ``False``).

This test is designed exclusively for two-sided surfaces. If this
surface is one-sided, the incompressibility test will be run on its
two-sided double cover.

.. warning::
    This routine may in some circumstances be extremely slow. This is
    because the underlying algorithm cuts along this surface,
    retriangulates (possibly using a very large number of tetrahedra),
    and then searches for a normal compressing disc in each component
    of the cut-open triangulation.

Precondition:
    The underlying triangulation is valid and closed, and represents
    an irreducible 3-manifold.

Precondition:
    This normal surface is compact, embedded and connected, and
    contains no octagonal discs.

Returns:
    ``True`` if this surface is incompressible, or ``False`` if this
    surface is not incompressible (or if it is a sphere).)doc";

// Docstring regina::python::doc::NormalSurface_::isNormalEdgeLink
static const char *isNormalEdgeLink =
R"doc(Determines whether or not a positive rational multiple of this surface
is the normalised link of a single edge.

Here the phrase _normalised_ link of an edge *e* means the frontier of
a regular neighbourhood of *e*, converted into a normal surface by
expanding away from the edge using the normalisation process. It could
be that there is no normalisation required at all (in which case it is
also a _thin_ edge link). However, it could be that the normalisation
process expands the surface far away from the edge itself, or changes
its topology, or disconnects the surface, or even normalises it away
to an empty surface.

In particular, this test behaves differently from isThinEdgeLink(),
which tests for thin edge links only (where no additional
normalisation is required). If you are only interested in thin edge
links, then you should call isThinEdgeLink(), which has much less
overhead.

A surface (or its positive rational multiple) could be the normalised
link of many edges. The return value will be a pair (*v*, *thin*),
where:

* *v* is a vector containing all such edges. This will begin with the
  edges for which this surface is a thin link, followed by the edges
  where normalisation was required; within each category the edges
  will be ordered by their index within the triangulation.

* *thin* is either 0, 1 or 2, indicating how many edges this surface
  is a thin link for. This uses an unsigned type, since it will often
  be compared to ``v.size()``.

If no positive rational multiple of this surface is the normalised
link of any edge, then *link* will be 0 and *v* will be the empty
vector.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    a vector containing the edge(s) linked by a positive rational
    multiple of this surface and an integer indicating how many of
    these links are thin, as described above.)doc";

// Docstring regina::python::doc::NormalSurface_::isNormalTriangleLink
static const char *isNormalTriangleLink =
R"doc(Determines whether or not a positive rational multiple of this surface
is the normalised link of a single triangle.

Here the phrase _normalised_ link of a triangle *t* means the frontier
of a regular neighbourhood of *t*, converted into a normal surface by
expanding away from the triangle using the normalisation process. It
could be that there is no normalisation required at all (in which case
it is also a _thin_ triangle link). However, it could be that the
normalisation process expands the surface far away from the triangle
itself, or changes its topology, or disconnects the surface, or even
normalises it away to an empty surface.

In particular, this test behaves differently from
isThinTriangleLink(), which tests for thin triangle links only (where
no additional normalisation is required). Unlike the tests for edge
links, the routines isThinTriangleLink() and isNormalTriangleLink()
use essentially the same implementation (so testing for only thin
links may be a little faster, but not by much).

A surface (or its positive rational multiple) could be the normalised
link of many triangles. The return value will be a pair (*v*, *thin*),
where:

* *v* is a vector containing all such triangles. This will begin with
  the triangles for which this surface is a thin link, followed by the
  triangles where normalisation was required; within each category the
  triangles will be ordered by their index within the triangulation.

* *thin* is either 0, 1 or 2, indicating how many triangles this
  surface is a thin link for. This uses an unsigned type, since it
  will often be compared to ``v.size()``.

If no positive rational multiple of this surface is the normalised
link of any triangle, then *link* will be 0 and *v* will be the empty
vector.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    a vector containing the triangle(s) linked by a positive rational
    multiple of this surface and an integer indicating how many of
    these links are thin, as described above.)doc";

// Docstring regina::python::doc::NormalSurface_::isOrientable
static const char *isOrientable =
R"doc(Returns whether or not this surface is orientable.

For our purposes, the empty surface is considered to be orientable.

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately.

Precondition:
    This normal surface is embedded (not singular or immersed).

Precondition:
    This normal surface is compact (has finitely many discs).

.. warning::
    This routine explicitly builds the normal discs, and so may run
    out of memory if the normal coordinates are extremely large.

Returns:
    ``True`` if this surface is orientable, or ``False`` if this
    surface is non-orientable.)doc";

// Docstring regina::python::doc::NormalSurface_::isSplitting
static const char *isSplitting =
R"doc(Determines whether or not this surface is a splitting surface. A
*splitting* surface is a compact surface containing precisely one quad
per tetrahedron and no other normal (or almost normal) discs.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    ``True`` if and only if this is a splitting surface.)doc";

// Docstring regina::python::doc::NormalSurface_::isThinEdgeLink
static const char *isThinEdgeLink =
R"doc(Determines whether or not a positive rational multiple of this surface
is the thin link of a single edge.

Here a _thin_ edge link is a normal surface which appears naturally as
the frontier of a regular neighbourhood of an edge, with no need for
any further normalisation.

This behaves differently from isNormalEdgeLink(), which tests for a
_normalised_ edge link (which could end up far away from the edge, or
could be normalised into a surface with different topology, or could
even be normalised away to nothing). Although isNormalEdgeLink() will
also indicate thin edge links, this test has significantly less
overhead (and so should be faster).

A surface (or its positive rational multiple) can be the _thin_ edge
link of at most two edges. If there are indeed two different edges
*e1* and *e2* for which a multiple of this surface can be expressed as
the thin edge link, then the pair (*e1*, *e2*) will be returned. If
there is only one such edge *e*, then the pair (*e*, ``None``) will be
returned. If no positive rational multiple of this surface is the thin
link of any edge, then the pair (``None``, ``None``) will be returned.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    a pair containing the edge(s) linked by a positive rational
    multiple of this surface, as described above.)doc";

// Docstring regina::python::doc::NormalSurface_::isThinTriangleLink
static const char *isThinTriangleLink =
R"doc(Determines whether or not a positive rational multiple of this surface
is the thin link of a single triangle.

Here a _thin_ triangle link is a normal surface which appears
naturally as the frontier of a regular neighbourhood of a triangle,
with no need for any further normalisation.

This behaves differently from isNormalTriangleLink(), which tests for
a _normalised_ triangle link (which could end up far away from the
triangle, or could be normalised into a surface with different
topology, or could even be normalised away to nothing). Unlike the
tests for edge links, the routines isThinTriangleLink() and
isNormalTriangleLink() use essentially the same implementation (so
testing for only thin links may be a little faster, but not by much).

A surface (or its positive rational multiple) can be the _thin_ link
of at most two triangles. If there are indeed two different triangles
*t1* and *t2* for which a multiple of this surface can be expressed as
the thin triangle link, then the pair (*t1*, *t2*) will be returned.
If there is only one such triangle *t*, then the pair (*t*, ``None``)
will be returned. If no positive rational multiple of this surface is
the thin link of any triangle, then the pair (``None``, ``None``) will
be returned.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    a pair containing the triangle(s) linked by a positive rational
    multiple of this surface, as described above.)doc";

// Docstring regina::python::doc::NormalSurface_::isTwoSided
static const char *isTwoSided =
R"doc(Returns whether or not this surface is two-sided.

For our purposes, the empty surface is considered to be two-sided.

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately.

Precondition:
    This normal surface is embedded (not singular or immersed).

Precondition:
    This normal surface is compact (has finitely many discs).

.. warning::
    This routine explicitly builds the normal discs, and so may run
    out of memory if the normal coordinates are extremely large.

Returns:
    ``True`` if this surface is two-sided, or ``False`` if this
    surface is one-sided.)doc";

// Docstring regina::python::doc::NormalSurface_::isVertexLink
static const char *isVertexLink =
R"doc(Determines whether or not a positive rational multiple of this surface
is the link of a single vertex.

This behaves differently from isVertexLinking(), which will also
detect a union of several different vertex links. In contrast, this
routine will only identify the link of a _single_ vertex (or a
multiple of such a link).

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    the vertex linked by a positive rational multiple of this surface,
    or ``None`` if this surface is not a multiple of a single vertex
    link.)doc";

// Docstring regina::python::doc::NormalSurface_::isVertexLinking
static const char *isVertexLinking =
R"doc(Determines whether or not this surface is vertex linking. A _vertex
linking_ surface contains only triangles.

This behaves differently from isVertexLink(), which only detects the
link of a single vertex (or a multiple of such a link). In contrast,
this routine will also detect the union of several _different_ vertex
links.

Note that the results of this routine are not cached. Thus the results
will be reevaluated every time this routine is called.

Returns:
    ``True`` if and only if this surface is vertex linking.)doc";

// Docstring regina::python::doc::NormalSurface_::locallyCompatible
static const char *locallyCompatible =
R"doc(Determines whether this and the given surface are locally compatible.
Local compatibility means that, within each individual tetrahedron of
the triangulation, it is possible to arrange the normal discs of both
surfaces so that none intersect.

This is a local constraint, not a global constraint. That is, we do
not insist that we can avoid intersections within all tetrahedra
_simultaneously_. To test the global constraint, see the (much slower)
routine disjoint() instead.

Local compatibility can be formulated in terms of normal disc types.
Two normal (or almost normal) surfaces are locally compatible if and
only if they together have at most one quadrilateral or octagonal disc
type per tetrahedron.

Note again that this is a local constraint only. In particular, for
almost normal surfaces, it does _not_ insist that there is at most one
octagonal disc type anywhere within the triangulation.

If one of the two surfaces breaks the local compatibility constraints
on its own (for instance, it contains two different quadrilateral disc
types within the same tetrahedron), then this routine will return
``False`` regardless of what the other surface contains.

Precondition:
    Both this and the given normal surface live within the same
    3-manifold triangulation.

Parameter ``other``:
    the other surface to test for local compatibility with this
    surface.

Returns:
    ``True`` if the two surfaces are locally compatible, or ``False``
    if they are not.)doc";

// Docstring regina::python::doc::NormalSurface_::name
static const char *name =
R"doc(Returns the name associated with this normal surface. Names are
optional and need not be unique. The default name for a surface is the
empty string.

Returns:
    the name of associated with this surface.)doc";

// Docstring regina::python::doc::NormalSurface_::normal
static const char *normal =
R"doc(Determines whether this surface contains only triangle and/or
quadrilateral discs. This is to distinguish normal surfaces from more
general surfaces such as almost normal surfaces (which also contain
octagonal pieces).

Even if the underlying vector encoding supports other disc types (such
as octagons), this routine will still return ``True`` if this
particular surface does not use them. This is in contrast to the
routine NormalSurfaces::allowsAlmostNormal(), which only examines the
underlying coordinate system.

Returns:
    ``True`` if and only if this surface contains only triangles
    and/or quadrilaterals.)doc";

// Docstring regina::python::doc::NormalSurface_::octPosition
static const char *octPosition =
R"doc(Determines the first coordinate position at which this surface has a
non-zero octagonal coordinate. In other words, if this routine returns
the disc type *t*, then the octagonal coordinate returned by
octs(t.tetIndex, t.type) is non-zero. Here DiscType::type represents
an octagon type within a tetrahedron, and takes values between 0 and 2
inclusive.

If this surface does not contain any octagons, this routine returns a
null disc type instead.

This routine caches its results, which means that once it has been
called for a particular surface, subsequent calls return the answer
immediately. Moreover, if the underlying coordinate system does not
support almost normal surfaces, then even the first call is fast (it
returns a null disc type immediately).

Returns:
    the position of the first non-zero octagonal coordinate, or a null
    disc type if there is no such coordinate.)doc";

// Docstring regina::python::doc::NormalSurface_::octs
static const char *octs =
R"doc(Returns the number of octagonal discs of the given type in this normal
surface.

In each tetrahedron, there are three types of octagons, defined by how
they separate the four tetrahedron vertices into two pairs. Octagon
type *i* (for *i* = 0, 1 or 2) is defined to intersect edges *i* and
(5-*i*) of the tetrahedron twice each, and to intersect the remaining
edges once each. This means:

* type 0 separates vertices 0,1 of the tetrahedron from vertices 2,3;

* type 1 separates vertices 0,2 of the tetrahedron from vertices 1,3;

* type 2 separates vertices 0,3 of the tetrahedron from vertices 1,2.

Parameter ``tetIndex``:
    the index in the triangulation of the tetrahedron in which the
    requested octagons reside; this should be between 0 and
    Triangulation<3>::size()-1 inclusive.

Parameter ``octType``:
    the type of this octagon in the given tetrahedron; this should be
    0, 1 or 2, as described above.

Returns:
    the number of octagonal discs of the given type.)doc";

// Docstring regina::python::doc::NormalSurface_::quads
static const char *quads =
R"doc(Returns the number of quadrilateral discs of the given type in this
normal surface.

In each tetrahedron, there are three types of quadrilaterals, defined
by how they separate the four tetrahedron vertices into two pairs.
Quadrilateral type *i* (for *i* = 0, 1 or 2) is defined to separate
edge *i* of the tetrahedron from edge (5-*i*). That is:

* type 0 separates vertices 0,1 of the tetrahedron from vertices 2,3;

* type 1 separates vertices 0,2 of the tetrahedron from vertices 1,3;

* type 2 separates vertices 0,3 of the tetrahedron from vertices 1,2.

Parameter ``tetIndex``:
    the index in the triangulation of the tetrahedron in which the
    requested quadrilaterals reside; this should be between 0 and
    Triangulation<3>::size()-1 inclusive.

Parameter ``quadType``:
    the type of this quadrilateral in the given tetrahedron; this
    should be 0, 1 or 2, as described above.

Returns:
    the number of quadrilateral discs of the given type.)doc";

// Docstring regina::python::doc::NormalSurface_::reconstructTriangles
static const char *reconstructTriangles =
R"doc(Reconstructs the triangle coordinates in the given integer vector.

The given vector must represent a normal surface within the given
triangulation, using the given vector encoding.

* If the given encoding does not already store triangle coordinates,
  then the vector will be modified directly to use a new encoding that
  does, and this new encoding will be returned.

* If the given encoding does already store triangles, then this
  routine will do nothing and immediately return *enc*.

Parameter ``tri``:
    the triangulation in which the normal surface lives.

Parameter ``vector``:
    an integer vector that encodes a normal (or almost normal) surface
    within *tri*; this will be modified directly.

Parameter ``enc``:
    the encoding used by the given integer vector.

Returns:
    the new encoding used by the modified *vector*.)doc";

// Docstring regina::python::doc::NormalSurface_::removeOcts
static const char *removeOcts =
R"doc(Returns an isotopic normal surface that contains no octagons, placed
within a homeomorphic (but possibly different) triangulation.

Specifically: this routine returns a surface *s* within a
triangulation *t*, where *t* is homeomorphic to the triangulation
containing this surface, and where *s* is a normal (not almost normal)
surface isotopic to this. Only the surface *s* is returned; you can
access *t* by calling ``s.triangulation()``.

If this surface is already normal (i.e., it does not contain any
octagons), then the surface returned will simply be a copy of this
surface (but possibly using a different vector encoding), living
within the same triangulation.

If this surface does contain octagons, then the triangulation *t* will
be obtained from the original by replacing each tetrahedron containing
octagons with _three_ tetrahedra (essentially performing a 0-2 move).
Each octagon can then be subdivided into a quadrilateral and four
triangles. If the original triangulation is oriented, then the new
triangulation *t* will preserve this orientation.

In all cases, the surface that is returned will use a vector encoding
that does not store octagons.

Note: this routine can happily cope with multiple octagons in the same
tetrahedron, and/or multiple tetrahedra containing octagons.

This routine ignores tetrahedron locks entirely. If there are octagons
present and the triangulation does change, then the new triangulation
will have no tetrahedron or triangle locks at all. In particular, such
locks will not stop this routine from subdividing those tetrahedra
that contain octagons. Of course, the original triangulation will
remain untouched, with all of its locks intact.

Precondition:
    This surface is embedded.

Returns:
    an isotopic normal (not almost normal) surface *s*, as described
    above.)doc";

// Docstring regina::python::doc::NormalSurface_::scaleDown
static const char *scaleDown =
R"doc(Converts this surface into its smallest positive rational multiple
with integer coordinates.

Note that the scaling factor will be independent of which internal
vector encoding is used. This is essentially because integer quad
coordinates (which are stored in every encoding) and integer octagon
coordinates (which are stored in every almost normal encoding) are
enough to guarantee integer triangle coordinates (which might or might
not be stored).

Returns:
    the integer by which the original surface was divided (i.e., the
    gcd of all normal coordinates in the original surface). This will
    always be strictly positive.)doc";

// Docstring regina::python::doc::NormalSurface_::setName
static const char *setName =
R"doc(Sets the name associated with this normal surface. Names are optional
and need not be unique. The default name for a surface is the empty
string.

Parameter ``name``:
    the new name to associate with this surface.)doc";

// Docstring regina::python::doc::NormalSurface_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given normal surface. This is a
fast (constant time) operation.

This and the given normal surface do not need to live in the same
underlying triangulation, and they do not need to have the same length
vectors or use the same normal coordinate system - if any of these
properties differs then the two surfaces will be adjusted accordingly.

Parameter ``other``:
    the normal surface whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::NormalSurface_::triangles
static const char *triangles =
R"doc(Returns the number of triangular discs of the given type in this
normal surface. A triangular disc type is identified by specifying a
tetrahedron and a vertex of that tetrahedron that the triangle
surrounds.

Parameter ``tetIndex``:
    the index in the triangulation of the tetrahedron in which the
    requested triangles reside; this should be between 0 and
    Triangulation<3>::size()-1 inclusive.

Parameter ``vertex``:
    the vertex of the given tetrahedron around which the requested
    triangles lie; this should be between 0 and 3 inclusive.

Returns:
    the number of triangular discs of the given type.)doc";

// Docstring regina::python::doc::NormalSurface_::triangulation
static const char *triangulation =
R"doc(Returns the triangulation in which this normal surface resides.

This will be a snapshot frozen in time of the triangulation that was
originally passed to the NormalSurface constructor.

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

// Docstring regina::python::doc::NormalSurface_::vector
static const char *vector =
R"doc(Gives read-only access to the integer vector that Regina uses
internally to represent this surface.

Note that this vector might not use the same coordinate system in
which the surfaces were originally enumerated. (For example, this
vector will always include triangle coordinates, even if the surfaces
were originally enumerated in quad or quad-oct coordinates.) You can
call encoding() to find out precisley how the coordinates of this
vector should be interpreted.

See the NormalSurface class notes for information on how this vector
is structured.

.. note::
    If you wish to access the numbers of triangles, quads and so on,
    you should use the functions triangles(), quads(), etc., which do
    not require any knowledge of the internal vector encoding that
    this surface uses.

Returns:
    the underlying integer vector.)doc";

// Docstring regina::python::doc::NormalSurface_::writeXMLData
static const char *writeXMLData =
R"doc(Writes a chunk of XML containing this normal surface and all of its
properties. This routine will be called from within
NormalSurfaces::writeXMLPacketData().

Python:
    The argument *out* should be an open Python file object.

Parameter ``out``:
    the output stream to which the XML should be written.

Parameter ``format``:
    indicates which of Regina's XML file formats to write.

Parameter ``list``:
    the enclosing normal surface list. Currently this is only relevant
    when writing to the older FileFormat::XmlGen2 format; it will be
    ignored (and may be ``None``) for newer file formats.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

