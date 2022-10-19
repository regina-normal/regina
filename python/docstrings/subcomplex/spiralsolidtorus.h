/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SpiralSolidTorus
static const char *SpiralSolidTorus =
R"doc(Represents a spiralled solid torus in a triangulation.

A spiralled solid torus is created by placing tetrahedra one upon
another in a spiralling fashion to form a giant loop.

For each tetrahedron, label the vertices A, B, C and D. Draw the
tetrahedron so that the vertices form an upward spiral in the order
A-B-C-D, with D directly above A. Face BCD is on the top, face ABC is
on the bottom and faces ABD and ACD are both vertical.

When joining two tetrahedra, face BCD of the lower tetrahedron will be
joined to face ABC of the upper tetrahedron. In this way the
tetrahedra are placed one upon another to form a giant loop (which is
closed up by placing the bottommost tetrahedron above the topmost
tetrahedron in a similar fashion), forming a solid torus overall.

In each tetrahedron, directed edges AB, BC and CD are *major edges*,
directed edges AC and BD are *minor edges* and directed edge AD is an
*axis edge*.

The major edges all combined form a single longitude of the solid
torus. Using this directed longitude, using the directed meridinal
curve ACBA and assuming the spiralled solid torus contains *n*
tetrahedra, the minor edges all combined form a (2, *n*) curve and the
axis edges all combined form a (3, *n*) curve on the torus boundary.

Note that all tetrahedra in the spiralled solid torus must be distinct
and there must be at least one tetrahedron.

Note also that class TriSolidTorus represents a spiralled solid torus
with precisely three tetrahedra. A spiralled solid torus with only one
tetrahedron is in fact a (1,2,3) layered solid torus.

All optional StandardTriangulation routines are implemented for this
class.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value. Note,
however, that the only way to create objects of this class (aside from
copying or moving) is via the static member function recognise().)doc";

namespace SpiralSolidTorus_ {

// Docstring regina::python::doc::SpiralSolidTorus_::__copy
static const char *__copy =
R"doc(Creates a new copy of this structure. This will induce a deep copy of
*src*.

Parameter ``src``:
    the structure to copy.)doc";

// Docstring regina::python::doc::SpiralSolidTorus_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
type of spiralled solid torus.

Specifically, two spiralled solid tori will compare as equal if and
only if they have the same size (i.e., the same number of tetrahedra).

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass means
they describe isomorphic structures).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent the
    same type of spiralled solid torus.)doc";

// Docstring regina::python::doc::SpiralSolidTorus_::__ne
static const char *__ne =
R"doc(Determines whether this and the given structure represent different
types of spiralled solid torus.

Specifically, two spiralled solid tori will compare as equal if and
only if they have the same size (i.e., the same number of tetrahedra).

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass means
they describe isomorphic structures).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent
    different types of spiralled solid torus.)doc";

// Docstring regina::python::doc::SpiralSolidTorus_::cycle
static const char *cycle =
R"doc(Cycles this spiralled solid torus by the given number of tetrahedra.
Tetrahedra *k*, *k*+1, *k*+2 and so on will become tetrahedra 0, 1, 2
and so on respectively. Note that this operation will not change the
vertex roles.

The underlying triangulation is not changed; all that changes is how
this spiralled solid torus is represented.

Parameter ``k``:
    the number of tetrahedra through which we should cycle.)doc";

// Docstring regina::python::doc::SpiralSolidTorus_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls SpiralSolidTorus::swap(); it is
provided so that SpiralSolidTorus meets the C++ Swappable
requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::SpiralSolidTorus_::isCanonical
static const char *isCanonical =
R"doc(Determines whether this spiralled solid torus is in canonical form.
Canonical form is described in detail in the description for
makeCanonical().

Returns:
    ``True`` if and only if this spiralled solid torus is in canonical
    form.)doc";

// Docstring regina::python::doc::SpiralSolidTorus_::makeCanonical
static const char *makeCanonical =
R"doc(Converts this spiralled solid torus into its canonical representation.
The canonical representation of a spiralled solid torus is unique in a
given triangulation.

Tetrahedron 0 in the spiralled solid torus will be the tetrahedron
with the lowest index in the triangulation, and under permutation
``vertexRoles(0)`` the image of 0 will be less than the image of 3.

Returns:
    ``True`` if and only if the representation of this spiralled solid
    torus was actually changed.)doc";

// Docstring regina::python::doc::SpiralSolidTorus_::recognise
static const char *recognise =
R"doc(Determines if the given tetrahedron forms part of a spiralled solid
torus with its vertices playing the given roles in the solid torus.

Note that the boundary triangles of the spiralled solid torus need not
be boundary triangles within the overall triangulation, i.e., they may
be identified with each other or with triangles of other tetrahedra.

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
    tetrahedron as tetrahedron 0, or ``null`` if the given tetrahedron
    is not part of a spiralled solid torus with the given vertex
    roles.)doc";

// Docstring regina::python::doc::SpiralSolidTorus_::reverse
static const char *reverse =
R"doc(Reverses this spiralled solid torus. Tetrahedra 0, 1, 2, ..., size()-1
will become tetrahedra size()-1, ..., 2, 1, 0 respectively. Note that
this operation will change the vertex roles as well.

The underlying triangulation is not changed; all that changes is how
this spiralled solid torus is represented.)doc";

// Docstring regina::python::doc::SpiralSolidTorus_::size
static const char *size =
R"doc(Returns the number of tetrahedra in this spiralled solid torus.

Returns:
    the number of tetrahedra.)doc";

// Docstring regina::python::doc::SpiralSolidTorus_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::SpiralSolidTorus_::tetrahedron
static const char *tetrahedron =
R"doc(Returns the requested tetrahedron in this spiralled solid torus.
Tetrahedra are numbered from 0 to size()-1 inclusive, with tetrahedron
*i*+1 being placed above tetrahedron *i*.

Parameter ``index``:
    specifies which tetrahedron to return; this must be between 0 and
    size()-1 inclusive.

Returns:
    the requested tetrahedron.)doc";

// Docstring regina::python::doc::SpiralSolidTorus_::vertexRoles
static const char *vertexRoles =
R"doc(Returns a permutation represeting the role that each vertex of the
requested tetrahedron plays in the solid torus. The permutation
returned (call this ``p``) maps 0, 1, 2 and 3 to the four vertices of
tetrahedron *index* so that vertices ``p[0]``, ``p[1]``, ``p[2]`` and
``p[3]`` correspond to vertices A, B, C and D respectively as
described in the general class notes.

In particular, the directed edge from vertex ``p[0]`` to ``p[3]`` is
an axis edge, directed edges ``p[0]`` to ``p[2]`` and ``p[1]`` to
``p[3]`` are minor edges and the directed path from vertices ``p[0]``
to ``p[1]`` to ``p[2]`` to ``p[3]`` follows the three major edges.

See the general class notes for further details.

Parameter ``index``:
    specifies which tetrahedron in the solid torus to examine; this
    must be between 0 and size()-1 inclusive.

Returns:
    a permutation representing the roles of the vertices of the
    requested tetrahedron.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

