/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::HyperCoords
static const char *HyperCoords =
R"doc(Represents different coordinate systems that can be used for
enumerating and/or displaying normal hypersurfaces within 4-manifold
triangulations.

IDs 0-9999 are reserved for future use by Regina. If you are extending
Regina to include your own coordinate system, you should choose an ID
>= 10000.)doc";

// Docstring regina::python::doc::HyperEncoding
static const char *HyperEncoding =
R"doc(Indicates precisely how a normal hypersurface within a 4-manifold
triangulation is encoded by an integer vector.

Normal hypersurfaces do not always store their internal vectors in the
same coordinate system that was used to enumerate the surfaces, and
indeed an isolated hypersurface does not know *how* it was originally
created.

Therefore each normal hypersurface keeps a small amount of data,
represented by this class, so that it knows how to interpret its
internal integer vector. This data also remembers properties of the
enumeration process that can be used for optimisations (e.g., the
knowledge that, even if the vector stores tetrahedron coordinates, the
surface cannot contain any vertex linking components).

Encodings have the important property that any rational multiple of a
normal hypersurface *s* can always be stored using the same encoding
as is used for *s*. (In particular, taking a rational multiple will
not invalidate any of the property flags in the encoding.)

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.)doc";

// Docstring regina::python::doc::HyperInfo
static const char *HyperInfo =
R"doc(A class used to query general information about different normal
hypersurface coordinate systems.

This class has become much simpler and more stripped-down since Regina
7.0. Much of the functionality that was available in this class in
Regina 6.0.1 and earlier can now be accessed through the new
HyperEncoding class.)doc";

namespace HyperCoords_ {

// Docstring regina::python::doc::HyperCoords_::HS_EDGE_WEIGHT
static const char *HS_EDGE_WEIGHT =
R"doc(Represents edge weight coordinates for normal hypersurfaces.

This coordinate system is for display only: Regina can view
hypersurfaces in this coordinate system, but it cannot use it to
enumerate or create hypersurfaces.)doc";

// Docstring regina::python::doc::HyperCoords_::HS_PRISM
static const char *HS_PRISM =
R"doc(Represents prism coordinates for normal hypersurfaces.

Regina can both enumerate and view hypersurfaces in this coordinate
system.)doc";

// Docstring regina::python::doc::HyperCoords_::HS_STANDARD
static const char *HS_STANDARD =
R"doc(Represents standard tetrahedron-prism coordinates for normal
hypersurfaces.

Regina can both enumerate and view hypersurfaces in this coordinate
system.)doc";

}

namespace HyperEncoding_ {

// Docstring regina::python::doc::HyperEncoding_::HyperEncoding
static const char *HyperEncoding =
R"doc(Returns an encoding that precisely describes the given normal
hypersurface coordinate system. This is the encoding that you would
use with a "pen and paper" enumeration of hypersurfaces in the given
coordinate system, with no further conversion, optimisation, or post-
processing.

Note that, when Regina enumerates hypersurfaces in the given
coordinate system, it might choose to post-process the resulting
vectors to use a different encoding. (In the 3-dimensional world, for
example, normal surfaces enumerated in quad coordinates are post-
processed and stored in standard coordinates.)

If *coords* is not one of the coordinate systems that Regina can use
to enumerate normal hypersurfaces, then the resulting encoding will be
invalid, and valid() will return ``False``.

Parameter ``coords``:
    one of Regina's normal hypersurface coordinate systems.)doc";

// Docstring regina::python::doc::HyperEncoding_::HyperEncoding_2
static const char *HyperEncoding_2 = R"doc(Creates a new copy of the given encoding.)doc";

// Docstring regina::python::doc::HyperEncoding_::__add
static const char *__add =
R"doc(Returns an encoding that could hold the sum of hypersurfaces that use
this and the given encoding.

More precisely, the encoding that is returned is the "simplest"
possible encoding that is capable of holding the sum of two normal
hypersurfaces that use this and the given encoding respectively.

Parameter ``rhs``:
    the encoding to combine with this.

Returns:
    the "sum" of this and the given encoding, as defined above.)doc";

// Docstring regina::python::doc::HyperEncoding_::__eq
static const char *__eq =
R"doc(Determines whether this and the given encoding are identical.

Parameter ``other``:
    the encoding to compare with this.

Returns:
    ``True`` if and only if both encodings are identical.)doc";

// Docstring regina::python::doc::HyperEncoding_::__ne
static const char *__ne =
R"doc(Determines whether this and the given encoding are different.

Parameter ``other``:
    the encoding to compare with this.

Returns:
    ``True`` if and only if both encodings are different.)doc";

// Docstring regina::python::doc::HyperEncoding_::block
static const char *block =
R"doc(Returns the number of coordinates stored for each pentachoron.

Returns:
    The number of coordinates per pentachoron.)doc";

// Docstring regina::python::doc::HyperEncoding_::couldBeNonCompact
static const char *couldBeNonCompact =
R"doc(Returns whether it is possible for a normal hypersurface using this
encoding to be non-compact. Here "non-compact" refers to a surface
with infinitely many normal pieces. See
NormalHypersurface::isCompact() for details.

This is typically information that is inherited from whoever
enumerated or created the hypersurface that uses this encoding.

If this returns ``True``, it does not mean that the hypersurface does
actually contain infinitely many normal pieces; it simply means that
the user will need to test this themselves. If this returns ``False``,
however, it is guaranteed that the hypersurface is compact, with no
further testing required.

Returns:
    ``True`` if it is possible that the hypersurface might be non-
    compact.)doc";

// Docstring regina::python::doc::HyperEncoding_::couldBeVertexLink
static const char *couldBeVertexLink =
R"doc(Returns whether it is possible for a normal hypersurface using this
encoding to include one or more vertex linking components.

This is typically information that is inherited from whoever
enumerated or created the hypersurface that uses this encoding.

If this returns ``True``, it does not mean that the hypersurface does
actually contain vertex linking components; it simply means that the
user will need to test this themselves. If this returns ``False``,
however, it is guaranteed that the hypersurface does *not* contain any
vertex linking components, with no further testing required.

Returns:
    ``True`` if it is possible that the hypersurface might contain one
    or more vertex linking components.)doc";

// Docstring regina::python::doc::HyperEncoding_::fromIntValue
static const char *fromIntValue =
R"doc(Reconstructs an encoding from an integer value.

This is a partner routine to intValue(): for any encoding *enc*, the
encoding ``fromIntValue(enc.intValue())`` is always equal to *enc*.

The main purpose of intValue() and fromIntValue() is to support
reading and writing hypersurfaces to/from Regina's XML file format.

Parameter ``value``:
    an integer that represents an encoding, typically obtained by
    calling intValue().

Returns:
    the corresponding encoding.)doc";

// Docstring regina::python::doc::HyperEncoding_::intValue
static const char *intValue =
R"doc(Exports this encoding as an integer.

The exact value of the integer is meant to be opaque, in the sense
that it can only be used with fromIntValue(). End users are not meant
to pull it apart and analyse it (this is what the other query routines
in this class are for).

The main purpose of intValue() and fromIntValue() is to support
reading and writing hypersurfaces to/from Regina's XML file format.

It is guaranteed that 0 will never be the integer value of a
(correctly constructed) encoding.

Returns:
    an integer that represents this encoding.)doc";

// Docstring regina::python::doc::HyperEncoding_::storesTetrahedra
static const char *storesTetrahedra =
R"doc(Returns whether this encoding explicitly stores tetrahedron
coordinates.

Returns:
    ``True`` if tetrahedron coordinates are stored.)doc";

// Docstring regina::python::doc::HyperEncoding_::valid
static const char *valid =
R"doc(Returns whether this describes a vector encoding of normal
hypersurfaces.

The only case where this would be ``False`` is if you create an
encoding using either (1) a coordinate system that Regina does not use
for enumeration; or (2) another invalid encoding.

Returns:
    ``True`` if and only if this is a valid encoding.)doc";

// Docstring regina::python::doc::HyperEncoding_::withTetrahedra
static const char *withTetrahedra =
R"doc(Returns an extension of this encoding that explicitly stores
tetrahedron coordinates.

All other properties of the encoding will be copied across without
changes (including "theoretical" properties such as whether the
surface could have vertex linking components, or whether it could be
non-compact).

If this encoding already stores tetrahedron coordinates, then the
result will be identical to this.

Returns:
    an extension of this encoding that stores tetrahedron coordinates.)doc";

}

namespace HyperInfo_ {

// Docstring regina::python::doc::HyperInfo_::name
static const char *name =
R"doc(Returns the human-readable name of the given coordinate system.

The first letter of the returned string will be upper-case, and all
subsequent letters will be lower-case.

This routine is guaranteed to return a non-null string, even if
*coordSystem* is not one of the HyperCoords enum values.

Parameter ``coordSystem``:
    the coordinate system being queried; this may be any of the
    HyperCoords enumeration values.

Returns:
    the name of the given coordinate system.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

