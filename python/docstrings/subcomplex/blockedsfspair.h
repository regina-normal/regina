/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::BlockedSFSPair
static const char *BlockedSFSPair =
R"doc(Represents a blocked pair of Seifert fibred spaces joined along a
single connecting torus. This is a particular type of triangulation of
a graph manifold, formed from two saturated regions whose torus
boundaries are identified. An optional layering may be placed between
the two torus boundaries to allow for a more interesting relationship
between the boundary curves of each region. For more detail on
saturated regions and their constituent saturated blocks, see the
SatRegion class; for more detail on layerings, see the Layering class.

Each of the two saturated regions must have precisely one boundary
component formed from just one saturated annulus, and this boundary
may not be twisted (i.e., it must be a torus, not a Klein bottle). The
way in which the boundaries from each region are identified is
specified by a 2-by-2 matrix *M*, which expresses curves representing
the fibres and base orbifold of the second region in terms of the
first (see the page on notation for Seifert fibred spaces for
terminology).

More specifically, suppose that *f0* and *o0* are directed curves on
the first region boundary and *f1* and *o1* are directed curves on the
second region boundary, where *f0* and *f1* represent the fibres of
each region and *o0* and *o1* represent the base orbifolds. Then the
boundaries are joined according to the following relation:

```
    [f1]       [f0]
    [  ] = M * [  ]
    [o1]       [o0]
```

If a layering is present between the two boundaries, then the boundary
curves are not identified directly. In this case, the matrix *M* shows
how the layering relates the curves on each region boundary.

Note that the routines writeName() and writeTeXName() do _not_ offer
enough information to uniquely identify the triangulation, since this
essentially requires 2-dimensional assemblings of saturated blocks.
For full details, writeTextLong() may be used instead.

The optional StandardTriangulation routine manifold() is implemented
for this class, but homology() is not.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value. Note,
however, that the only way to create objects of this class (aside from
copying or moving) is via the static member function recognise().)doc";

namespace BlockedSFSPair_ {

// Docstring regina::python::doc::BlockedSFSPair_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given structure. This will induce a deep
copy of *src*.

Parameter ``src``:
    the structure to copy.)doc";

// Docstring regina::python::doc::BlockedSFSPair_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
type of blocked pair of Seifert fibred spaces.

Specifically, two structures will compare as equal if and only if both
structures are formed from the same pair of combinatorial
presentations of saturated regions (as returned by the SatRegion
comparison operators), presented in the same order, and with their
torus boundaries joined using the same 2-by-2 matrix.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass is
more specific than combinatorial isomorphism, since this test does not
account for the many symmetries in a blocked Seifert fibred space).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent the
    same type of blocked pair of Seifert fibred spaces.)doc";

// Docstring regina::python::doc::BlockedSFSPair_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls BlockedSFSPair::swap(); it is
provided so that BlockedSFSPair meets the C++ Swappable requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::BlockedSFSPair_::matchingReln
static const char *matchingReln =
R"doc(Returns the matrix describing how the two saturated region boundaries
are joined. Note that if a layering is placed between the two region
boundaries, then any changes to the boundary relationships caused by
the layering are included in this matrix.

See the class notes above for precise information on how this matrix
is presented.

Returns:
    the matrix describing how the region boundaries are joined.)doc";

// Docstring regina::python::doc::BlockedSFSPair_::recognise
static const char *recognise =
R"doc(Determines if the given triangulation is a blocked pair of Seifert
fibred spaces, as described by this class.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``tri``:
    the triangulation to examine.

Returns:
    a structure containing details of the blocked pair, or ``None`` if
    the given triangulation is not of this form.)doc";

// Docstring regina::python::doc::BlockedSFSPair_::region
static const char *region =
R"doc(Returns details of one of the two bounded saturated regions that form
this triangulation. See the class notes above for further information.

Parameter ``which``:
    0 if the first region should be returned, or 1 if the second
    region should be returned.

Returns:
    details of the requested saturated region.)doc";

// Docstring regina::python::doc::BlockedSFSPair_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

