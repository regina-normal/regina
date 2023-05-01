/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SnapPeaCensusTri
static const char *SnapPeaCensusTri =
R"doc(Represents a 3-manifold triangulation from the SnapPea cusped census.

The SnapPea cusped census is the census of cusped hyperbolic
3-manifolds formed from up to seven tetrahedra. This census was
tabulated by Callahan, Hildebrand and Weeks, and is shipped with
SnapPea 3.0d3 (and also with Regina).

.. note::
    The modern cusped hyperbolic census now extends to nine
    tetrahedra, and indeed the 9-tetrahedron database is accessible
    through the Census lookup routines. However, for the time being,
    the scope of these SnapPeaCensusManifold and SnapPeaCensusTri
    classes is restricted to the original Callahan-Hildebrand-Weeks
    7-tetrahedron census only.

The census is split into five different sections according to number
of tetrahedra and orientability. Each of these sections corresponds to
one of the section constants defined in this class.

For further details regarding the SnapPea census, see "A census of
cusped hyperbolic 3-manifolds", Patrick J. Callahan, Martin V.
Hildebrand and Jeffrey R. Weeks, Math. Comp. 68 (1999), no. 225, pp.
321--332.

Note that this class is closely tied to SnapPeaCensusManifold. In
particular, the section constants defined in SnapPeaCensusManifold and
SnapPeaCensusTri are identical, and so may be freely mixed.
Furthermore, the section and index parameters of a SnapPeaCensusTri
are identical to those of its corresponding SnapPeaCensusManifold.

All of the optional StandardTriangulation routines are implemented for
this class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
StandardTriangulation subclasses. Note that the only way to create
these objects (aside from copying or moving) is via the static member
function recognise().)doc";

namespace SnapPeaCensusTri_ {

// Docstring regina::python::doc::SnapPeaCensusTri_::__copy
static const char *__copy = R"doc(Creates a new copy of the given structure.)doc";

// Docstring regina::python::doc::SnapPeaCensusTri_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
triangulation from the SnapPea census.

The SnapPea census manifolds ``x101`` and ``x103`` are homeomorphic;
however, the corresponding triangulations represented by
SnapPeaCensusTri will compare as different (since this class describes
the specific triangulation, not the underlying manifold).

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass means
they describe isomorphic structures).

Parameter ``compare``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent the
    same SnapPea census triangulation.)doc";

// Docstring regina::python::doc::SnapPeaCensusTri_::__ne
static const char *__ne =
R"doc(Determines whether this and the given structure represent different
triangulations from the SnapPea census.

The SnapPea census manifolds ``x101`` and ``x103`` are homeomorphic;
however, the corresponding triangulations represented by
SnapPeaCensusTri will compare as different (since this class describes
the specific triangulation, not the underlying manifold).

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass means
they describe isomorphic structures).

Parameter ``compare``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent
    different SnapPea census triangulations.)doc";

// Docstring regina::python::doc::SnapPeaCensusTri_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls SnapPeaCensusTri::swap(); it is
provided so that SnapPeaCensusTri meets the C++ Swappable
requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::SnapPeaCensusTri_::index
static const char *index =
R"doc(Returns the index of this triangulation within its particular section
of the SnapPea census. Note that indices for each section begin
counting at zero.

Returns:
    the index of this triangulation within its section.)doc";

// Docstring regina::python::doc::SnapPeaCensusTri_::recognise
static const char *recognise =
R"doc(Determines if the given triangulation component is one of the smallest
SnapPea census triangulations.

This routine is able to recognise a small selection of triangulations
from the beginning of the SnapPea census, by way of hard-coding their
structures and properties. Most triangulations from the census however
will not be recognised by this routine.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``comp``:
    the triangulation component to examine.

Returns:
    a structure representing the small SnapPea census triangulation,
    or ``None`` if the given component is not one of the few SnapPea
    census triangulations recognised by this routine.)doc";

// Docstring regina::python::doc::SnapPeaCensusTri_::section
static const char *section =
R"doc(Returns the section of the SnapPea census to which this triangulation
belongs. This will be one of the section constants defined in this
class.

Returns:
    the section of the SnapPea census.)doc";

// Docstring regina::python::doc::SnapPeaCensusTri_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

