/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::NormalCoords
static const char *NormalCoords =
R"doc(Represents different coordinate systems that can be used for
enumerating and/or displaying normal surfaces.

IDs 0-9999 are reserved for future use by Regina. If you are extending
Regina to include your own coordinate system, you should choose an ID
≥ 10000.)doc";

// Docstring regina::python::doc::NormalEncoding
static const char *NormalEncoding =
R"doc(Indicates precisely how a normal surface is encoded by an integer
vector.

Normal surfaces do not always store their internal vectors in the same
coordinate system that was used to enumerate the surfaces, and indeed
an isolated surface does not know _how_ it was originally created.

Therefore each normal surface keeps a small amount of data,
represented by this class, so that it knows how to interpret its
internal integer vector. This data also remembers properties of the
enumeration process that can be used for optimisations (e.g., the
knowledge that, even if the vector stores triangle coordinates, the
surface cannot contain any vertex linking components).

For convenience, there is also a special encoding that identifies an
angle structure vector; this can be created via
``NormalEncoding(NormalCoords::Angle)``, and can be recognised via
storesAngles(). However, like NormalCoords::Angle itself, this special
angle structure encoding does _not_ represent a normal surface, cannot
be combined with other encodings, and must not be used with any of
Regina's routines unless the documentation explicitly allows it.
Specifically, any code that accepts a NormalEncoding argument may
silently assume that the encoding is _not_ the special angle structure
encoding, unless the documentation explicitly says otherwise.

Encodings have the important property that any rational multiple of a
normal surface *s* can always be stored using the same encoding as is
used for *s*. (In particular, taking a rational multiple will not
invalidate any of the property flags in the encoding.)

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.)doc";

// Docstring regina::python::doc::NormalInfo
static const char *NormalInfo =
R"doc(A class used to query general information about different normal
coordinate systems.

This class has become much simpler and more stripped-down since Regina
7.0. Much of the functionality that was available in this class in
Regina 6.0.1 and earlier can now be accessed through the new
NormalEncoding class.)doc";

namespace NormalCoords_ {

// Docstring regina::python::doc::NormalCoords_::AlmostNormal
static const char *AlmostNormal =
R"doc(Represents standard triangle-quadrilateral-octagon coordinates for
octagonal almost normal surfaces.

Regina can both enumerate and view surfaces in this coordinate system.)doc";

// Docstring regina::python::doc::NormalCoords_::Angle
static const char *Angle =
R"doc(Represents angle structure coordinates.

This coordinate system is _not_ for use with normal surfaces: it
cannot be used either to display them or enumerate them. Instead it is
for use with angle structures on triangulations. Because the
combinatorics and linear algebra of angle strutures are tightly
related to those of normal surfaces, we include NormalCoords::Angle
here so that angle structure routines can make use of some of Regina's
existing normal surface machinery.

For a triangulation with *n* tetrahedra, this system has 3*n*+1
coordinates. The first 3*n* are analogous to quadrilateral coordinates
(specifically, for each quadrilateral type *Q*, the corresponding
angle structure coordinate represents the pair of angles in the same
tetrahedron that *Q* does not meet). The final coordinate is a scaling
coordinate, used to projectivise the angle structure polytope so that
it becomes a polyhedral cone that is invariant under (positive)
scaling. If the final scaling coordinate is *s*, then a rational value
of *x* in any other coordinate position should be interpreted as the
angle (*x*/*s*)π.

Precondition:
    This coordinate system must not be used with any of Regina's
    routines unless they explicitly declare that NormalCoords::Angle
    is allowed.)doc";

// Docstring regina::python::doc::NormalCoords_::Arc
static const char *Arc =
R"doc(Represents triangle arc coordinates for normal surfaces.

This coordinate system is for display only: Regina can view surfaces
in this coordinate system, but it cannot use it to enumerate or create
surfaces.)doc";

// Docstring regina::python::doc::NormalCoords_::Edge
static const char *Edge =
R"doc(Represents edge weight coordinates for normal surfaces.

This coordinate system is for display only: Regina can view surfaces
in this coordinate system, but it cannot use it to enumerate or create
surfaces.)doc";

// Docstring regina::python::doc::NormalCoords_::LegacyAlmostNormal
static const char *LegacyAlmostNormal =
R"doc(Indicates that a list of almost normal surfaces was created using
Regina 4.5.1 or earlier, where surfaces with more than one octagon of
the same type were stripped out of the final solution set. As of
Regina 4.6 such surfaces are now included in the solution set, since
we need them if we wish to enumerate _all_ almost normal surfaces (not
just the _vertex_ almost normal surfaces).

Regina cannot enumerate or view surfaces in this coordinate system. It
is only used for reading legacy data files. If you have a list that
uses this system, you can just view the surfaces in standard almost
normal coordinates (NormalCoords::AlmostNormal).)doc";

// Docstring regina::python::doc::NormalCoords_::Quad
static const char *Quad =
R"doc(Represents quadrilateral coordinates for normal surfaces. For details,
see "Normal surface Q-theory", Jeffrey L. Tollefson, Pacific J. Math.
183 (1998), no. 2, 359--374.

Regina can both enumerate and view surfaces in this coordinate system.)doc";

// Docstring regina::python::doc::NormalCoords_::QuadClosed
static const char *QuadClosed =
R"doc(Represents quadrilateral coordinates in ideal triangulations for
enumerating closed surfaces only (thus excluding spun-normal
surfaces). The coordinates themselves are identical to quadrilateral
coordinates, as described by NormalCoords::Quad; however, the
enumeration procedure introduces additional constraints. The resulting
solution space is the space Q_0 as described in "Computing closed
essential surfaces in knot complements", by Burton, Coward and
Tillmann, in SCG ’13: Proceedings of the 29th Annual Symposium on
Computational Geometry, ACM, 2013, pp. 405–414.

Note that, if a vertex surface in quad coordinates is closed, it will
always be a vertex surface in this system of "closed quad
coordinates". However, the converse is not true: a vertex surface in
closed quad coordinates need not be a vertex in "plain" quad
coordinates.

Regina can enumerate surfaces in this coordinate system, but it is not
for viewing. You can just view the surfaces in quad coordinates
(NormalCoords::Quad) instead.

Precondition:
    Regina can only create matching equations in this coordinate
    system for a limited class of triangulations. Currently, such
    triangulations _must_ be oriented and ideal, with precisely one
    torus cusp and no other boundary components or internal vertices.
    These conditions will be checked when building the matching
    equations, and Regina will throw an InvalidArgument exception if
    they are not met.

Precondition:
    SnapPea must be able to work with the underlying triangulation,
    without retriangulating. This should follow from the previous
    constraints; however, it will also be checked when building the
    matching equations, and Regina will throw an UnsolvedCase
    exception if this requirement is not met.)doc";

// Docstring regina::python::doc::NormalCoords_::QuadOct
static const char *QuadOct =
R"doc(Represents quadrilateral-octagon coordinates for octagonal almost
normal surfaces. For details, see "Quadrilateral-octagon coordinates
for almost normal surfaces", Benjamin A. Burton, Experiment. Math. 19
(2010), 285-315.

Regina can both enumerate and view surfaces in this coordinate system.)doc";

// Docstring regina::python::doc::NormalCoords_::QuadOctClosed
static const char *QuadOctClosed =
R"doc(Represents quadrilateral-octagon coordinates in ideal triangulations
for enumerating closed surfaces only (thus excluding spun-almost
normal surfaces). The coordinates themselves are identical to
quadrilateral-octagon coordinates, as described by
NormalCoords::QuadOct; however, the enumeration procedure introduces
additional constraints.

Note that, if a vertex surface in quad-oct coordinates is closed, it
will always be a vertex surface in this system of "closed quad-oct
coordinates". However, the converse is not true: a vertex surface in
closed quad-oct coordinates need not be a vertex in "plain" quad-oct
coordinates.

Regina can enumerate surfaces in this coordinate system, but it is not
for viewing. You can just view the surfaces in quad-oct coordinates
(NormalCoords::QuadOct) instead.

Precondition:
    Regina can only create matching equations in this coordinate
    system for a limited class of triangulations. Currently, such
    triangulations _must_ be oriented and ideal, with precisely one
    torus cusp and no other boundary components or internal vertices.
    These conditions will be checked when building the matching
    equations, and Regina will throw an InvalidArgument exception if
    they are not met.

Precondition:
    SnapPea must be able to work with the underlying triangulation,
    without retriangulating. This should follow from the previous
    constraints; however, it will also be checked when building the
    matching equations, and Regina will throw an UnsolvedCase
    exception if this requirement is not met.)doc";

// Docstring regina::python::doc::NormalCoords_::Standard
static const char *Standard =
R"doc(Represents standard triangle-quadrilateral coordinates for normal
surfaces.

Regina can both enumerate and view surfaces in this coordinate system.)doc";

}

namespace NormalEncoding_ {

// Docstring regina::python::doc::NormalEncoding_::__add
static const char *__add =
R"doc(Returns an encoding that could hold the sum of surfaces that use this
and the given encoding.

More precisely, the encoding that is returned is the "simplest"
possible encoding that is capable of holding the sum of two surfaces
that use this and the given encoding respectively.

Precondition:
    Neither this encoding nor *rhs* is the special angle structure
    encoding (see the class notes for details).

Parameter ``rhs``:
    the encoding to combine with this.

Returns:
    the "sum" of this and the given encoding, as defined above.)doc";

// Docstring regina::python::doc::NormalEncoding_::__copy
static const char *__copy = R"doc(Creates a new copy of the given encoding.)doc";

// Docstring regina::python::doc::NormalEncoding_::__eq
static const char *__eq =
R"doc(Determines whether this and the given encoding are identical.

Parameter ``other``:
    the encoding to compare with this.

Returns:
    ``True`` if and only if both encodings are identical.)doc";

// Docstring regina::python::doc::NormalEncoding_::__init
static const char *__init =
R"doc(Returns an encoding that precisely describes the given normal or
almost normal coordinate system. This is the encoding that you would
use with a "pen and paper" enumeration of surfaces in the given
coordinate system, with no further conversion, optimisation, or post-
processing.

Note that, when Regina enumerates surfaces in the given coordinate
system, it might choose to post-process the resulting vectors to use a
different encoding. For example, when enumerating surfaces in quad or
quad-oct coordinates, Regina computes and stores triangle coordinates
also, and so for its own _internal_ choice of encoding,
storesTriangles() will return ``True``. In contrast, if you simply
create a ``NormalEncoding(NormalCoords::Quad)``, then the resulting
encoding will have storesTriangles() return ``False``.

If *coords* is not one of the coordinate systems that Regina can use
to enumerate or reconstruct surfaces (or NormalCoords::Angle, as
discussed below), then the resulting encoding will be invalid, and
valid() will return ``False``. (Here "reconstruct" refers to the
special case of NormalCoords::LegacyAlmostNormal, which is used only
when reading surfaces from very old data files).

As a special case, you may pass NormalCoords::Angle to this
constructor; however, the resulting encoding does not represent a
normal surface and must not be used anywhere in Regina unless the
documentation explicitly allows it. See the class notes for further
details.

Parameter ``coords``:
    one of Regina's normal or almost normal coordinate systems.)doc";

// Docstring regina::python::doc::NormalEncoding_::__ne
static const char *__ne =
R"doc(Determines whether this and the given encoding are different.

Parameter ``other``:
    the encoding to compare with this.

Returns:
    ``True`` if and only if both encodings are different.)doc";

// Docstring regina::python::doc::NormalEncoding_::block
static const char *block =
R"doc(Returns the number of coordinates stored for each tetrahedron.

Returns:
    The number of coordinates per tetrahedron.)doc";

// Docstring regina::python::doc::NormalEncoding_::couldBeNonCompact
static const char *couldBeNonCompact =
R"doc(Returns whether it is possible for a surface using this encoding to be
non-compact. Here "non-compact" refers to a surface with infinitely
many discs, such as a spun-normal surface. See
NormalSurface::isCompact() for details.

This is typically information that is inherited from whoever
enumerated or created the normal surface that uses this encoding.

If this returns ``True``, it does not mean that the surface does
actually contain infinitely many discs; it simply means that the user
will need to test this themselves. If this returns ``False``, however,
it is guaranteed that the surface is compact, with no further testing
required.

For the special angle structure encoding (described in the class
notes), this routine will return ``False``.

Returns:
    ``True`` if it is possible that the surface might be non-compact.)doc";

// Docstring regina::python::doc::NormalEncoding_::couldBeVertexLink
static const char *couldBeVertexLink =
R"doc(Returns whether it is possible for a surface using this encoding to
include one or more vertex linking components.

This is typically information that is inherited from whoever
enumerated or created the normal surface that uses this encoding.

If this returns ``True``, it does not mean that the surface does
actually contain vertex linking components; it simply means that the
user will need to test this themselves. If this returns ``False``,
however, it is guaranteed that the surface does _not_ contain any
vertex linking components, with no further testing required.

For the special angle structure encoding (described in the class
notes), this routine will return ``False``.

Returns:
    ``True`` if it is possible that the surface might contain one or
    more vertex linking components.)doc";

// Docstring regina::python::doc::NormalEncoding_::empty
static const char *empty =
R"doc(Returns an encoding that is suitable for representing the empty
surface, whose normal coordinates are all zero.

Returns:
    a suitable encoding for the empty surface.)doc";

// Docstring regina::python::doc::NormalEncoding_::fromIntValue
static const char *fromIntValue =
R"doc(Reconstructs an encoding from an integer value.

This is a partner routine to intValue(): for any encoding *enc*, the
encoding ``fromIntValue(enc.intValue())`` is always equal to *enc*.

The main purpose of intValue() and fromIntValue() is to support
reading and writing surfaces to/from Regina's XML file format.

Parameter ``value``:
    an integer that represents an encoding, typically obtained by
    calling intValue().

Returns:
    the corresponding encoding.)doc";

// Docstring regina::python::doc::NormalEncoding_::intValue
static const char *intValue =
R"doc(Exports this encoding as an integer.

The exact value of the integer is meant to be opaque, in the sense
that it can only be used with fromIntValue(). End users are not meant
to pull it apart and analyse it (this is what the other query routines
in this class are for).

The main purpose of intValue() and fromIntValue() is to support
reading and writing surfaces to/from Regina's XML file format.

It is guaranteed that 0 will never be the integer value of a
(correctly constructed) encoding.

Returns:
    an integer that represents this encoding.)doc";

// Docstring regina::python::doc::NormalEncoding_::storesAngles
static const char *storesAngles =
R"doc(Identifies whether this is the special angle structure encoding.

This routine is used to recognise the "special case" encoding
``NormalEncoding(NormalCoords::Angle)``. Such an encoding does not
represent a normal surface, and cannot be used anywhere in Regina
unless explicitly allowed in the documentation. See the class notes
for further details.)doc";

// Docstring regina::python::doc::NormalEncoding_::storesOctagons
static const char *storesOctagons =
R"doc(Returns whether this encoding explicitly stores octagon coordinates.

For the special angle structure encoding (described in the class
notes), this routine will return ``False``.

Returns:
    ``True`` if octagon coordinates are stored.)doc";

// Docstring regina::python::doc::NormalEncoding_::storesTriangles
static const char *storesTriangles =
R"doc(Returns whether this encoding explicitly stores triangle coordinates.

For the special angle structure encoding (described in the class
notes), this routine will return ``False``.

Returns:
    ``True`` if triangle coordinates are stored.)doc";

// Docstring regina::python::doc::NormalEncoding_::valid
static const char *valid =
R"doc(Returns whether this describes a vector encoding of normal or almost
normal surfaces.

The only case where this would be ``False`` is if you create an
encoding using either (1) a coordinate system that Regina does not use
for enumeration or reconstruction; or (2) another invalid encoding.

For the special angle structure encoding (described in the class
notes), this routine will return ``True``.

Returns:
    ``True`` if and only if this is a valid encoding.)doc";

// Docstring regina::python::doc::NormalEncoding_::withOctagons
static const char *withOctagons =
R"doc(Returns an extension of this encoding that explicitly stores octagon
coordinates.

All other properties of the encoding will be copied across without
changes (including "theoretical" properties such as whether the
surface could have vertex linking components, or whether it could be
spun-normal).

If this encoding already stores octagon coordinates, then the result
will be identical to this.

Precondition:
    This is not the special angle structure encoding (see the class
    notes for details).

Returns:
    an extension of this encoding that stores octagon coordinates.)doc";

// Docstring regina::python::doc::NormalEncoding_::withTriangles
static const char *withTriangles =
R"doc(Returns an extension of this encoding that explicitly stores triangle
coordinates.

All other properties of the encoding will be copied across without
changes (including "theoretical" properties such as whether the
surface could have vertex linking components, or whether it could be
spun-normal).

If this encoding already stores triangle coordinates, then the result
will be identical to this.

Precondition:
    This is not the special angle structure encoding (see the class
    notes for details).

Returns:
    an extension of this encoding that stores triangle coordinates.)doc";

// Docstring regina::python::doc::NormalEncoding_::withoutOctagons
static const char *withoutOctagons =
R"doc(Returns a restricted version of this encoding that does not store
octagon coordinates.

All other properties of the encoding will be copied across without
changes (including "theoretical" properties such as whether the
surface could have vertex linking components, or whether it could be
spun-normal).

If this encoding already does not store octagon coordinates, then the
result will be identical to this.

Precondition:
    This is not the special angle structure encoding (see the class
    notes for details).

Returns:
    a restriction of this encoding that does not store octagon
    coordinates.)doc";

// Docstring regina::python::doc::NormalEncoding_::withoutTriangles
static const char *withoutTriangles =
R"doc(Returns a restricted version of this encoding that does not store
triangle coordinates.

Most other properties of the encoding will be copied across without
changes (including "theoretical" properties such as whether the
surface could be spun-normal). However, since triangle coordinates
will be excluded, the "could contain vertex linking components"
property will be explicitly set to ``False``.

If this encoding already does not store triangle coordinates, then the
result will be identical to this.

Precondition:
    This is not the special angle structure encoding (see the class
    notes for details).

Returns:
    a restriction of this encoding that does not store triangle
    coordinates.)doc";

}

namespace NormalInfo_ {

// Docstring regina::python::doc::NormalInfo_::name
static const char *name =
R"doc(Returns the human-readable name of the given coordinate system.

The first letter of the returned string will be upper-case, and all
subsequent letters will be lower-case.

This routine is guaranteed to return a non-null string, even if
*coordSystem* is not one of the NormalCoords enum values.

Parameter ``coordSystem``:
    the coordinate system being queried; this may be any of the
    NormalCoords enumeration values.

Returns:
    the name of the given coordinate system.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

