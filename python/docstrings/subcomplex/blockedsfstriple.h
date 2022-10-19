/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::BlockedSFSTriple
static const char *BlockedSFSTriple =
R"doc(Represents a blocked sequence of three Seifert fibred spaces joined
along connecting tori. This is a particular type of triangulation of a
graph manifold, formed from three saturated regions whose various
torus boundaries are identified as described below. Optional layerings
may be placed between torus boundaries to allow for more interesting
relationships between the respective boundary curves of each region.
For more detail on saturated regions and their constituent saturated
blocks, see the SatRegion class; for more detail on layerings, see the
Layering class.

The three saturated regions must be joined together as illustrated
below. Each large box represents a saturated region, and the small
tunnels show where the region boundaries are joined (possibly via
layerings).

```
    /----------------\   /------------------\   /----------------\
    |                |   |                  |   |                |
    |  End region 0   ---   Central region   ---   End region 1  |
    |                 ---                    ---                 |
    |                |   |                  |   |                |
    \----------------/   \------------------/   \----------------/
```

Each of the end regions must have precisely one boundary component
formed from just one saturated annulus. The central region may have
two boundary components formed from one saturated annulus each.
Alternatively, it may have one boundary formed from two saturated
annuli, where this boundary is pinched together so that each annulus
becomes a two-sided torus joined to one of the end regions. None of
the boundary components (or the two-sided tori discussed above) may be
twisted (i.e., they must be tori, not Klein bottles).

The ways in which the various region boundaries are identified are
specified by 2-by-2 matrices, which express curves representing the
fibres and base orbifold of each end region in terms of the central
region (see the page on notation for Seifert fibred spaces for
terminology).

Specifically, consider the matrix *M* that describes the joining of
the central region and the first end region (marked in the diagram
above as end region 0). Suppose that *f* and *o* are directed curves
on the central region boundary and *f0* and *o0* are directed curves
on the first end region boundary, where *f* and *f0* represent the
fibres of each region and *o* and *o0* represent the base orbifolds.
Then the boundaries are joined according to the following relation:

```
    [f0]       [f ]
    [  ] = M * [  ]
    [o0]       [o ]
```

Likewise, let *M*' be the matrix describing how the central region and
the second end region (marked in the diagram as end region 1) are
joined. Let *f*' and *o*' be directed curves on the other central
region boundary and *f1* and *o1* be directed curves on the second end
region boundary, where *f*' and *f1* represent fibres and *o* and *o1*
represent the base orbifolds. Then the boundaries are joined according
to the relation:

```
    [f1]        [f']
    [  ] = M' * [  ]
    [o1]        [o']
```

If a layering is present between two regions, then the corresponding
boundary curves are not identified directly. In this case, the
relevant matrix *M* or *M*' shows how the layering relates the curves
on each region boundary.

Note that the routines writeName() and writeTeXName() do *not* offer
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

namespace BlockedSFSTriple_ {

// Docstring regina::python::doc::BlockedSFSTriple_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given structure. This will induce a deep
copy of *src*.

Parameter ``src``:
    the structure to copy.)doc";

// Docstring regina::python::doc::BlockedSFSTriple_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
type of blocked sequence of three Seifert fibred spaces.

Specifically, two structures will compare as equal if and only if both
structures are formed from the same triple of combinatorial
presentations of saturated regions (as returned by the SatRegion
comparison operators), presented in the same order, and with their
torus boundaries joined using the same pair of 2-by-2 matrices.

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
    same type of blocked sequence of three Seifert fibred spaces.)doc";

// Docstring regina::python::doc::BlockedSFSTriple_::__ne
static const char *__ne =
R"doc(Determines whether this and the given structure do not represent the
same type of blocked sequence of three Seifert fibred spaces.

Specifically, two structures will compare as equal if and only if both
structures are formed from the same triple of combinatorial
presentations of saturated regions (as returned by the SatRegion
comparison operators), presented in the same order, and with their
torus boundaries joined using the same pair of 2-by-2 matrices.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass is
more specific than combinatorial isomorphism, since this test does not
account for the many symmetries in a blocked Seifert fibred space).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure do not
    represent the same type of blocked sequence of three Seifert
    fibred spaces.)doc";

// Docstring regina::python::doc::BlockedSFSTriple_::centre
static const char *centre =
R"doc(Returns details of the central saturated region, as described in the
class notes above. This is the saturated region with two boundary
annuli, each of which is joined to one of the end regions.

Returns:
    details of the central region.)doc";

// Docstring regina::python::doc::BlockedSFSTriple_::end
static const char *end =
R"doc(Returns details of the requested end region, as described in the class
notes above. The end regions are the two saturated regions with one
boundary annulus each, which are both joined to the central region.

Parameter ``which``:
    0 if the first end region should be returned (marked as end region
    0 in the class notes), or 1 if the second end region should be
    returned (marked as end region 1 in the class notes).

Returns:
    details of the requested end region.)doc";

// Docstring regina::python::doc::BlockedSFSTriple_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls BlockedSFSTriple::swap(); it is
provided so that BlockedSFSTriple meets the C++ Swappable
requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::BlockedSFSTriple_::matchingReln
static const char *matchingReln =
R"doc(Returns the matrix describing how the given end region is joined to
the central region. Note that if a layering is placed between the two
respective region boundaries, then any changes to the boundary
relationships caused by the layering are included in this matrix.

See the class notes above for precise information on how each matrix
is presented.

Parameter ``which``:
    0 if the matrix returned should describe how the central region is
    joined to the first end region (marked end region 0 in the class
    notes), or 1 if the matrix returned should describe how the
    central region is joined to the second end region (marked end
    region 1 in the class notes).

Returns:
    the matrix describing how the requested region boundaries are
    joined.)doc";

// Docstring regina::python::doc::BlockedSFSTriple_::recognise
static const char *recognise =
R"doc(Determines if the given triangulation is a blocked sequence of three
Seifert fibred spaces, as described in the class notes above.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``tri``:
    the triangulation to examine.

Returns:
    a structure containing details of the blocked triple, or ``null``
    if the given triangulation is not of this form.)doc";

// Docstring regina::python::doc::BlockedSFSTriple_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

