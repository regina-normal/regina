/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::TriSolidTorus
static const char *TriSolidTorus =
R"doc(Represents a three-tetrahedron triangular solid torus in a
triangulation. A three-tetrahedron triangular solid torus is a three-
tetrahedron triangular prism with its two ends identified.

The resulting triangular solid torus will have all edges as boundary
edges. Three of these will be _axis edges_ (parallel to the axis of
the solid torus). Between the axis edges will be three annuli, each
with two internal edges. One of these internal edges will meet all
three tetrahedra (the *major* edge) and one of these internal edges
will only meet two of the tetrahedra (the *minor* edge).

Assume the axis of the layered solid torus is oriented. The three
major edges together form a loop on the boundary torus. This loop can
be oriented to run around the solid torus in the same direction as the
axis; this then induces an orientation on the boundary of a meridinal
disc. Thus, using an axis edge as longitude, the three major edges
will together form a (1,1) curve on the boundary torus.

We can now orient the minor edges so they also run around the solid
torus in the same direction as the axis, together forming a (2, -1)
curve on the boundary torus.

Finally, the three tetrahedra can be numbered 0, 1 and 2 in an order
that follows the axis, and the annuli can be numbered 0, 1 and 2 in an
order that follows the meridinal disc boundary so that annulus *i*
does not use any faces from tetrahedron *i*.

Note that all three tetrahedra in the triangular solid torus must be
distinct.

All optional StandardTriangulation routines are implemented for this
class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
StandardTriangulation subclasses. Note that the only way to create
these objects (aside from copying or moving) is via the static member
function recognise().)doc";

namespace TriSolidTorus_ {

// Docstring regina::python::doc::TriSolidTorus_::__copy
static const char *__copy = R"doc(Creates a new copy of the given structure.)doc";

// Docstring regina::python::doc::TriSolidTorus_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object represent the same
specific presentation of a triangular solid torus.

Unlike the parameterised subclasses of StandardTriangulation, this
TriSolidTorus subclass represents a fixed structure, and so its
comparisons test not for the _structure_ but the precise _location_ of
this structure within the enclosing triangulation.

Specifically, two triangular solid tori will compare as equal if and
only if each uses the same three numbered tetrahedra, in the same
order, and with the same vertex roles. That is, the corresponding
permutations returned by vertexRoles() must be equal, and the
corresponding tetrahedra returned by tetrahedron() must have equal
indices within the triangulation. In particular, it is still
meaningful to compare triangular solid tori within different
triangulations.

Parameter ``other``:
    the triangular solid torus to compare with this.

Returns:
    ``True`` if and only if this and the given object represent the
    same specific presentation of a triangular solid torus.)doc";

// Docstring regina::python::doc::TriSolidTorus_::areAnnuliLinkedAxis
static const char *areAnnuliLinkedAxis =
R"doc(Determines whether the two given annuli are linked in a particular
fashion by a layered chain.

In this scenario, one of the given annuli meets both faces of the top
tetrahedron and the other annulus meets both faces of the bottom
tetrahedron of the layered chain.

To be identified by this routine, the layered chain (described by
LayeredChain) must be attached as follows.

The two diagonals of the layered chain (between the two top faces and
between the two bottom faces) should correspond to the two directed
major edges of the two annuli, with the major edges both pointing from
top hinge edge to bottom hinge edge. The other boundary edges of the
layered chain that are not hinge edges should correspond to the two
directed minor edges of the two annuli, with the minor edges both
pointing from bottom hinge edge to top hinge edge. The hinge edges
themselves should correspond to the axis edges of the triangular solid
torus (this correspondence is determined by the previous
identifications; the axis edge between the two annuli will be
identified to both of the others in reverse).

Parameter ``otherAnnulus``:
    the annulus on the solid torus boundary *not* to be examined; this
    must be 0, 1 or 2.

Returns:
    the number of tetrahedra in the layered chain if the two annuli
    are linked as described, or 0 otherwise.)doc";

// Docstring regina::python::doc::TriSolidTorus_::areAnnuliLinkedMajor
static const char *areAnnuliLinkedMajor =
R"doc(Determines whether the two given annuli are linked in a particular
fashion by a layered chain.

In this scenario, both of the given annuli meet one face of the top
tetrahedron and one face of the bottom tetrahedron of the layered
chain.

To be identified by this routine, the layered chain (described by
LayeredChain) must be attached as follows. The two directed major
edges of the two annuli should correspond to the two hinge edges of
the layered chain (with both hinge edges pointing in the same
direction around the solid torus formed by the layered chain). The two
directed diagonals of the layered chain (between the two top faces and
between the two bottom faces, each pointing in the opposite direction
to the hinge edges around the solid torus formed by the layered chain)
should be identified and must correspond to the (identified) two
directed minor edges of the two annuli. The remaining boundary edges
of the layered chain should correspond to the axis edges of the
triangular solid torus (this correspondence is determined by the
previous identifications).

Parameter ``otherAnnulus``:
    the annulus on the solid torus boundary *not* to be examined; this
    must be 0, 1 or 2.

Returns:
    the number of tetrahedra in the layered chain if the two annuli
    are linked as described, or 0 otherwise.)doc";

// Docstring regina::python::doc::TriSolidTorus_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls TriSolidTorus::swap(); it is provided
so that TriSolidTorus meets the C++ Swappable requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::TriSolidTorus_::isAnnulusSelfIdentified
static const char *isAnnulusSelfIdentified =
R"doc(Determines whether the two triangles of the requested annulus are
glued to each other.

If the two triangles are glued, this routine will return a permutation
describing how the vertex roles are glued to each other. This will
describe directly how axis edges, major edges and minor edges map to
each other without having to worry about the specific assignment of
tetrahedron vertex numbers. For a discussion of vertex roles, see
vertexRoles().

Note that annulus ``index`` uses faces from tetrahedra ``index+1`` and
``index+2``. The gluing permutation that maps vertices of tetrahedron
``index+1`` to vertices of tetrahedron ``index+2`` will be
``vertexRoles(index+2) * roleMap * vertexRoles(index+1).inverse()``.

Parameter ``index``:
    specifies which annulus on the solid torus boundary to examine;
    this must be 0, 1 or 2.

Returns:
    a permutation that describes the gluing of vertex roles, or
    ``nullopt`` if the two triangles of the requested annulus are not
    glued together.)doc";

// Docstring regina::python::doc::TriSolidTorus_::recognise
static const char *recognise =
R"doc(Determines if the given tetrahedron forms part of a three-tetrahedron
triangular solid torus with its vertices playing the given roles in
the solid torus.

Note that the six boundary triangles of the triangular solid torus
need not be boundary triangles within the overall triangulation, i.e.,
they may be identified with each other or with faces of other
tetrahedra.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``tet``:
    the tetrahedron to examine.

Parameter ``useVertexRoles``:
    a permutation describing the role each tetrahedron vertex must
    play in the solid torus; this must be in the same format as the
    permutation returned by vertexRoles().

Returns:
    a structure containing details of the solid torus with the given
    tetrahedron as tetrahedron 0, or ``None`` if the given tetrahedron
    is not part of a triangular solid torus with the given vertex
    roles.)doc";

// Docstring regina::python::doc::TriSolidTorus_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::TriSolidTorus_::tetrahedron
static const char *tetrahedron =
R"doc(Returns the requested tetrahedron in this solid torus. See the general
class notes for further details.

Parameter ``index``:
    specifies which tetrahedron in the solid torus to return; this
    must be 0, 1 or 2.

Returns:
    the requested tetrahedron.)doc";

// Docstring regina::python::doc::TriSolidTorus_::vertexRoles
static const char *vertexRoles =
R"doc(Returns a permutation represeting the role that each vertex of the
requested tetrahedron plays in the solid torus. The permutation
returned (call this ``p``) maps 0, 1, 2 and 3 to the four vertices of
tetrahedron *index* so that the edge from ``p[0]`` to ``p[3]`` is an
oriented axis edge, and the path from vertices ``p[0]`` to ``p[1]`` to
``p[2]`` to ``p[3]`` follows the three oriented major edges. In
particular, the major edge for annulus *index* will run from vertices
``p[1]`` to ``p[2]``. Edges ``p[0]`` to ``p[2]`` and ``p[1]`` to
``p[3]`` will both be oriented minor edges.

Note that annulus ``index+1`` uses face ``p[1]`` of the requested
tetrahedron and annulus ``index+2`` uses face ``p[2]`` of the
requested tetrahedron. Both annuli use the axis edge ``p[0]`` to
``p[3]``, and each annulus uses one other major edge and one other
minor edge so that (according to homology) the axis edge equals the
major edge plus the minor edge.

See the general class notes for further details.

Parameter ``index``:
    specifies which tetrahedron in the solid torus to examine; this
    must be 0, 1 or 2.

Returns:
    a permutation representing the roles of the vertices of the
    requested tetrahedron.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

