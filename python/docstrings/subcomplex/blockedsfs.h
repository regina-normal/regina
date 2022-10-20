/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::BlockedSFS
static const char *BlockedSFS =
R"doc(Represents a blocked Seifert fibred space (possibly with boundary).
This is a particular type of triangulation of a Seifert fibred space,
where this triangulation is formed from a single saturated region. A
saturated region is in turn formed from saturated blocks by
identifying their boundary annuli in pairs; see the SatRegion class
for details.

Note that the routines writeName() and writeTeXName() do *not* offer
enough information to uniquely identify the triangulation, since this
essentially requires a 2-dimensional assembling of saturated blocks.
For full detail, writeTextLong() may be used instead.

The optional StandardTriangulation routine manifold() is implemented
for this class, but homology() is not.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value. Note,
however, that the only way to create objects of this class (aside from
copying or moving) is via the static member function recognise().)doc";

namespace BlockedSFS_ {

// Docstring regina::python::doc::BlockedSFS_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given structure. This will induce a deep
copy of *src*.

Parameter ``src``:
    the structure to copy.)doc";

// Docstring regina::python::doc::BlockedSFS_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
type of blocked Seifert fibred space.

Specifically, two structures will compare as equal if and only if both
structures are formed from the same combinatorial presentation of a
saturated region (as returned by the SatRegion comparison operators).

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
    same type of blocked Seifert fibred space.)doc";

// Docstring regina::python::doc::BlockedSFS_::__ne
static const char *__ne =
R"doc(Determines whether this and the given structure do not represent the
same type of blocked Seifert fibred space.

Specifically, two structures will compare as equal if and only if both
structures are formed from the same combinatorial presentation of a
saturated region (as returned by the SatRegion comparison operators).

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
    represent the same type of blocked Seifert fibred space.)doc";

// Docstring regina::python::doc::BlockedSFS_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls BlockedSFS::swap(); it is provided so
that BlockedSFS meets the C++ Swappable requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::BlockedSFS_::isPluggedIBundle
static const char *isPluggedIBundle =
R"doc(Determines whether this triangulation is a plugged thin I-bundle or a
plugged thick I-bundle. These structures are described in "Structures
of small closed non-orientable 3-manifold triangulations", Benjamin A.
Burton, J. Knot Theory Ramifications 16 (2007), 545--574.

Returns:
    the name of the plugged thin/thick I-bundle, if the triangulation
    is of this form, or no value if it is not.)doc";

// Docstring regina::python::doc::BlockedSFS_::recognise
static const char *recognise =
R"doc(Determines if the given triangulation is a blocked Seifert fibred
space.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``tri``:
    the triangulation to examine.

Returns:
    a structure containing details of the blocked Seifert fibred
    space, or ``None`` if the given triangulation is not a blocked
    Seifert fibred space.)doc";

// Docstring regina::python::doc::BlockedSFS_::region
static const char *region =
R"doc(Returns details of the single saturated region that fills this
triangulation.

Returns:
    the single saturated region.)doc";

// Docstring regina::python::doc::BlockedSFS_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

