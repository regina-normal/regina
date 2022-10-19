/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::LayeredSolidTorus
static const char *LayeredSolidTorus =
R"doc(Represents a layered solid torus in a triangulation. A layered solid
torus must contain at least one tetrahedron.

Note that this class **only** represents layered solid tori with a
(3,2,1) at their base. Thus triangulations that begin with a
degenerate (2,1,1) mobius strip and layer over the mobius strip
boundary (including the minimal (1,1,0) triangulation) are not
described by this class.

All optional StandardTriangulation routines are implemented for this
class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
StandardTriangulation subclasses. Note that the only way to create
these objects (aside from copying or moving) is via the static member
function recognise().)doc";

namespace LayeredSolidTorus_ {

// Docstring regina::python::doc::LayeredSolidTorus_::__copy
static const char *__copy = R"doc(Creates a new copy of this structure.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object represent the same type
of layered solid torus.

Specifically, two layered solid tori will compare as equal if and only
if each has the same ordered triple of integer parameters (describing
how many times the three top-level edge groups cut the meridinal
disc).

Note that it is possible for two non-isomorphic layered solid tori to
compare as equal, since these integer parameters do not detect the
presence of redundant layerings (i.e., consecutive layerings that
topologically cancel each other out).

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass, as
noted above, is weaker than combinatorial isomorphism).

Parameter ``other``:
    the layered solid torus to compare with this.

Returns:
    ``True`` if and only if this and the given object represent the
    same type of layered solid torus.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::__ne
static const char *__ne =
R"doc(Determines whether this and the given object do not represent the same
type of layered solid torus.

Specifically, two layered solid tori will compare as equal if and only
if each has the same ordered triple of integer parameters (describing
how many times the three top-level edge groups cut the meridinal
disc).

Note that it is possible for two non-isomorphic layered solid tori to
compare as equal, since these integer parameters do not detect the
presence of redundant layerings (i.e., consecutive layerings that
topologically cancel each other out).

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass, as
noted above, is weaker than combinatorial isomorphism).

Parameter ``other``:
    the layered solid torus to compare with this.

Returns:
    ``True`` if and only if this and the given object do not represent
    the same type of layered solid torus.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::base
static const char *base =
R"doc(Returns the tetrahedron that is glued to itself at the base of this
layered solid torus.

Returns:
    the base tetrahedron.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::baseEdge
static const char *baseEdge =
R"doc(Returns the requested edge of the base tetrahedron belonging to the
given group. The layering identifies the six edges of the base
tetrahedron into a group of three, a group of two and a single
unidentified edge; these are referred to as groups 3, 2 and 1
respectively.

Note that ``baseEdgeGroup(baseEdge(group, index)) == group`` for all
values of ``group`` and ``index``.

Edges ``baseEdge(2,0)`` and ``baseEdge(3,0)`` will both belong to face
``baseFace(0)``. Edges ``baseEdge(2,1)`` and ``baseEdge(3,2)`` will
both belong to face ``baseFace(1)``.

Parameter ``group``:
    the group that the requested edge should belong to; this must be
    1, 2 or 3.

Parameter ``index``:
    the index within the given group of the requested edge; this must
    be between 0 and *group*-1 inclusive. Note that in group 3 the
    edge at index 1 is adjacent to both the edges at indexes 0 and 2.

Returns:
    the edge number in the base tetrahedron of the requested edge;
    this will be between 0 and 5 inclusive.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::baseEdgeGroup
static const char *baseEdgeGroup =
R"doc(Returns the group that the given edge of the base tetrahedron belongs
to. See baseEdge() for further details about groups.

Note that ``baseEdgeGroup(baseEdge(group, index)) == group`` for all
values of ``group`` and ``index``.

Parameter ``edge``:
    the edge number in the base tetrahedron of the given edge; this
    must be between 0 and 5 inclusive.

Returns:
    the group to which the given edge belongs; this will be 1, 2 or 3.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::baseFace
static const char *baseFace =
R"doc(Returns one of the two faces of the base tetrahedron that are glued to
each other.

Parameter ``index``:
    specifies which of the two faces to return; this must be 0 or 1.

Returns:
    the requested face number in the base tetrahedron; this will be
    between 0 and 3 inclusive.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::flatten
static const char *flatten =
R"doc(Flattens this layered solid torus to a Mobius band. A new modified
triangulation is returned; the original triangulation that contains
this layered solid torus will be left unchanged.

Note that there are three different ways in which this layered solid
torus can be flattened, corresponding to the three different edges of
the boundary torus that could become the boundary edge of the new
Mobius band.

Parameter ``mobiusBandBdry``:
    the edge group on the boundary of this layered solid torus that
    will become the boundary of the new Mobius band (the remaining
    edge groups will become internal edges of the new Mobius band).
    This must be 0, 1 or 2. See topEdge() for further details about
    edge groups.

Returns:
    a new triangulation in which this layered solid torus has been
    flattened to a Mobius band.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls LayeredSolidTorus::swap(); it is
provided so that LayeredSolidTorus meets the C++ Swappable
requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::meridinalCuts
static const char *meridinalCuts =
R"doc(Returns the number of times the meridinal disc of the torus cuts the
top level tetrahedron edges in the given group. See topEdge() for
further details about groups.

Parameter ``group``:
    the given edge group; this must be 0, 1 or 2.

Returns:
    the number of times the meridinal disc cuts the edges in the given
    group.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::recognise
static const char *recognise =
R"doc(Determines if the given triangulation component forms a layered solid
torus in its entirity.

Note that, unlike recogniseFromBase(), this routine tests for a
component that is a layered solid torus with no additional tetrahedra
or gluings. That is, the two boundary triangles of the layered solid
torus must in fact be boundary triangles of the component.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``comp``:
    the triangulation component to examine.

Returns:
    a structure containing details of the layered solid torus, or
    ``null`` if the given component is not a layered solid torus.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::recogniseFromBase
static const char *recogniseFromBase =
R"doc(Determines if the given tetrahedron forms the base of a layered solid
torus within a triangulation. The torus need not be the entire
triangulation; the top level tetrahedron of the torus may be glued to
something else (or to itself).

Note that the base tetrahedron of a layered solid torus is the
tetrahedron furthest from the boundary of the torus, i.e. the
tetrahedron glued to itself with a twist.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``tet``:
    the tetrahedron to examine as a potential base.

Returns:
    a structure containing details of the layered solid torus, or
    ``null`` if the given tetrahedron is not the base of a layered
    solid torus.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::recogniseFromTop
static const char *recogniseFromTop =
R"doc(Determines if the given tetrahedron forms the top level tetrahedron of
a layered solid torus, with the two given faces of this tetrahedron
representing the boundary of the layered solid torus.

Note that the two given faces need not be boundary triangles in the
overall triangulation. That is, the layered solid torus may be a
subcomplex of some larger triangulation. For example, the two given
faces may be joined to some other tetrahedra outside the layered solid
torus or they may be joined to each other. In fact, they may even
extend this smaller layered solid torus to a larger layered solid
torus.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``tet``:
    the tetrahedron to examine as a potential top level of a layered
    solid torus.

Parameter ``topFace1``:
    the face number of the given tetrahedron that should represent the
    first boundary triangle of the layered solid torus. This should be
    between 0 and 3 inclusive.

Parameter ``topFace2``:
    the face number of the given tetrahedron that should represent the
    second boundary triangle of the layered solid torus. This should
    be between 0 and 3 inclusive, and should not be equal to
    *topFace1*.

Returns:
    a structure containing details of the layered solid torus, or
    ``null`` if the given tetrahedron with its two faces do not form
    the top level of a layered solid torus.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::size
static const char *size =
R"doc(Returns the number of tetrahedra in this layered solid torus.

Returns:
    the number of tetrahedra.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::topEdge
static const char *topEdge =
R"doc(Returns the requested edge of the top level tetrahedron belonging to
the given group. The layering reduces five of the top level
tetrahedron edges to three boundary edges of the solid torus; this
divides the five initial edges into groups of size two, two and one.

Group 0 represents the boundary edge that the meridinal disc cuts
fewest times. Group 2 represents the boundary edge that the meridinal
disc cuts most times. Group 1 is in the middle.

Note that ``topEdgeGroup(topEdge(group, index)) == group`` for all
values of ``group`` and ``index`` that actually correspond to an edge.

Edges ``topEdge(group, 0)`` will all belong to face ``topFace(0)``.
Edges ``topEdge(group, 1)`` (if they exist) will all belong to face
``topFace(1)``.

Parameter ``group``:
    the group that the requested edge should belong to; this must be
    0, 1 or 2.

Parameter ``index``:
    the index within the given group of the requested edge; this must
    be 0 or 1. Note that one of the groups only contains one
    tetrahedron edge, in which case this edge will be stored at index
    0.

Returns:
    the edge number in the top level tetrahedron of the requested edge
    (between 0 and 5 inclusive), or -1 if there is no such edge (only
    possible if the given group was the group of size one and the
    given index was 1).)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::topEdgeGroup
static const char *topEdgeGroup =
R"doc(Returns the group that the given edge of the top level tetrahedron
belongs to. See topEdge() for further details about groups.

Note that ``topEdgeGroup(topEdge(group, index)) == group`` for all
values of ``group`` and ``index`` that actually correspond to an edge.

Parameter ``edge``:
    the edge number in the top level tetrahedron of the given edge;
    this must be between 0 and 5 inclusive.

Returns:
    the group to which the given edge belongs (0, 1 or 2), or -1 if
    this edge does not belong to any group (only possible if this is
    the unique edge in the top tetrahedron not on the torus boundary).)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::topFace
static const char *topFace =
R"doc(Returns one of the two faces of the top level tetrahedron that form
the boundary of this layered solid torus.

Parameter ``index``:
    specifies which of the two faces to return; this must be 0 or 1.

Returns:
    the requested face number in the top level tetrahedron; this will
    be between 0 and 3 inclusive.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::topLevel
static const char *topLevel =
R"doc(Returns the top level tetrahedron in this layered solid torus. This is
the tetrahedron that would be on the boundary of the torus if the
torus were the entire manifold.

Returns:
    the top level tetrahedron.)doc";

// Docstring regina::python::doc::LayeredSolidTorus_::transform
static const char *transform =
R"doc(Adjusts the details of this layered solid torus according to the given
isomorphism between triangulations.

The given isomorphism must describe a mapping from *originalTri* to
*newTri*, and this layered solid torus must currently refer to
tetrahedra in *originalTri*. After this routine is called this
structure will instead refer to the corresponding tetrahedra in
*newTri* (with changes in vertex/face numbering also accounted for).

Precondition:
    This layered solid torus currently refers to tetrahedra in
    *originalTri*, and *iso* describes a mapping from *originalTri* to
    *newTri*.

Parameter ``originalTri``:
    the triangulation currently referenced by this layered solid
    torus.

Parameter ``iso``:
    the mapping from *originalTri* to *newTri*.

Parameter ``newTri``:
    the triangulation to be referenced by the updated layered solid
    torus.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

