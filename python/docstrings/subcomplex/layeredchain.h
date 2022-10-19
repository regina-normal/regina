/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::LayeredChain
static const char *LayeredChain =
R"doc(Represents a layered chain in a triangulation.

A layered chain is a set of *n* tetrahedra glued to each other by
layerings. For each tetrahedron, select two top faces, two bottom
faces and two hinge edges, so that the top faces are adjacent, the
bottom faces are adjacent, the hinge edges are opposite and each hinge
meets both a top and a bottom face. The tetrahedron can thus be
thought of as a fattened square with the top and bottom faces above
and below the square respectively, and the hinges as the top and
bottom edges of the square. The left and right edges of the square are
identified to form an annulus.

For each *i*, the top faces of tetrahedron *i* are glued to the bottom
faces of tetrahedron *i*+1. This is done by layering the upper
tetrahedron upon the annulus formed by the top faces of the lower
tetrahedron. The layering should be done over the left or right edge
of the lower square (note that these two edges are actually
identified). The top hinges of each tetrahedron should be identified,
as should the bottom hinges.

The bottom faces of the first tetrahedron and the top faces of the
last tetrahedron form the boundary of the layered chain. If there is
more than one tetrahedron, the layered chain forms a solid torus with
two vertices whose axis is parallel to each hinge edge.

The *index* of the layered chain is the number of tetrahedra it
contains. A layered chain must contain at least one tetrahedron.

Note that for the purposes of manifold() and homology(), a layered
chain containing only one tetrahedron will be considered as a
standalone tetrahedron that forms a 3-ball (and not a solid torus).

All optional StandardTriangulation routines are implemented for this
class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
StandardTriangulation subclasses.)doc";

namespace LayeredChain_ {

// Docstring regina::python::doc::LayeredChain_::__copy
static const char *__copy = R"doc(Creates a new copy of this structure.)doc";

// Docstring regina::python::doc::LayeredChain_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
type of layered chain.

Specifically, two layered chains will compare as equal if and only if
they have the same index (i.e., the same number of tetrahedra).

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass means
they describe isomorphic structures).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent the
    same type of layered chain.)doc";

// Docstring regina::python::doc::LayeredChain_::__init
static const char *__init =
R"doc(Creates a new layered chain containing only the given tetrahedron.
This new layered chain will have index 1, but may be extended using
extendAbove(), extendBelow() or extendMaximal().

Parameter ``tet``:
    the tetrahedron that will make up this layered chain.

Parameter ``vertexRoles``:
    a permutation describing the role each tetrahedron vertex must
    play in the layered chain; this must be in the same format as the
    permutation returned by bottomVertexRoles() and topVertexRoles().)doc";

// Docstring regina::python::doc::LayeredChain_::__ne
static const char *__ne =
R"doc(Determines whether this and the given structure represent different
types of layered chain.

Specifically, two layered chains will compare as equal if and only if
they have the same index (i.e., the same number of tetrahedra).

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass means
they describe isomorphic structures).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent
    different types of layered chain.)doc";

// Docstring regina::python::doc::LayeredChain_::bottom
static const char *bottom =
R"doc(Returns the bottom tetrahedron of this layered chain.

Returns:
    the bottom tetrahedron.)doc";

// Docstring regina::python::doc::LayeredChain_::bottomVertexRoles
static const char *bottomVertexRoles =
R"doc(Returns a permutation represeting the role that each vertex of the
bottom tetrahedron plays in the layered chain. The permutation
returned (call this ``p``) maps 0, 1, 2 and 3 to the four vertices of
the bottom tetrahedron so that the edge from ``p[0]`` to ``p[1]`` is
the top hinge, the edge from ``p[2]`` to ``p[3]`` is the bottom hinge,
faces ``p[1]`` and ``p[2]`` are the (boundary) bottom faces and faces
``p[0]`` and ``p[3]`` are the top faces.

See the general class notes for further details.

Returns:
    a permutation representing the roles of the vertices of the bottom
    tetrahedron.)doc";

// Docstring regina::python::doc::LayeredChain_::extendAbove
static const char *extendAbove =
R"doc(Checks to see whether this layered chain can be extended to include
the tetrahedron above the top tetrahedron (and still remain a layered
chain). If so, this layered chain will be modified accordingly (note
that its index will be increased by one and its top tetrahedron will
change).

Returns:
    ``True`` if and only if this layered chain was extended.)doc";

// Docstring regina::python::doc::LayeredChain_::extendBelow
static const char *extendBelow =
R"doc(Checks to see whether this layered chain can be extended to include
the tetrahedron below the bottom tetrahedron (and still remain a
layered chain). If so, this layered chain will be modified accordingly
(note that its index will be increased by one and its bottom
tetrahedron will change).

Returns:
    ``True`` if and only if this layered chain was extended.)doc";

// Docstring regina::python::doc::LayeredChain_::extendMaximal
static const char *extendMaximal =
R"doc(Extends this layered chain to a maximal length layered chain. Both
extendAbove() and extendBelow() will be used until this layered chain
can be extended no further.

Returns:
    ``True`` if and only if this layered chain was extended.)doc";

// Docstring regina::python::doc::LayeredChain_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls LayeredChain::swap(); it is provided
so that LayeredChain meets the C++ Swappable requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::LayeredChain_::index
static const char *index =
R"doc(Returns the number of tetrahedra in this layered chain.

Returns:
    the number of tetrahedra.)doc";

// Docstring regina::python::doc::LayeredChain_::invert
static const char *invert =
R"doc(Inverts this layered chain so the upper hinge becomes the lower and
vice versa. The top and bottom tetrahedra will remain the top and
bottom tetrahedra respectively.

Note that this operation will cause the hinge edges to point in the
opposite direction around the solid torus formed by this layered
chain.

Note that only the representation of the chain is altered; the
underlying triangulation is not changed.)doc";

// Docstring regina::python::doc::LayeredChain_::reverse
static const char *reverse =
R"doc(Reverses this layered chain so the top tetrahedron becomes the bottom
and vice versa. The upper and lower hinges will remain the upper and
lower hinges respectively.

Note that this operation will cause the hinge edges to point in the
opposite direction around the solid torus formed by this layered
chain.

Note that only the representation of the chain is altered; the
underlying triangulation is not changed.)doc";

// Docstring regina::python::doc::LayeredChain_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::LayeredChain_::top
static const char *top =
R"doc(Returns the top tetrahedron of this layered chain.

Returns:
    the top tetrahedron.)doc";

// Docstring regina::python::doc::LayeredChain_::topVertexRoles
static const char *topVertexRoles =
R"doc(Returns a permutation represeting the role that each vertex of the top
tetrahedron plays in the layered chain. The permutation returned (call
this ``p``) maps 0, 1, 2 and 3 to the four vertices of the top
tetrahedron so that the edge from ``p[0]`` to ``p[1]`` is the top
hinge, the edge from ``p[2]`` to ``p[3]`` is the bottom hinge, faces
``p[1]`` and ``p[2]`` are the bottom faces and faces ``p[0]`` and
``p[3]`` are the (boundary) top faces.

See the general class notes for further details.

Returns:
    a permutation representing the roles of the vertices of the top
    tetrahedron.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

