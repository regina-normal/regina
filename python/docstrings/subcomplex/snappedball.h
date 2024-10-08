/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::SnappedBall
static const char *SnappedBall =
R"doc(Represents a snapped 3-ball in a triangulation. A snapped 3-ball is a
single tetrahedron with two faces glued to each other to form a 3-ball
with a two triangle boundary.

All optional StandardTriangulation routines are implemented for this
class.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
StandardTriangulation subclasses. Note that the only way to create
these objects (aside from copying or moving) is via the static member
function recognise().)doc";

namespace SnappedBall_ {

// Docstring regina::python::doc::SnappedBall_::__copy
static const char *__copy = R"doc(Creates a new copy of the given structure.)doc";

// Docstring regina::python::doc::SnappedBall_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object represent the same
specific presentation of a snapped 3-ball.

Unlike the parameterised subclasses of StandardTriangulation, this
SnappedBall subclass represents a fixed structure, and so its
comparisons test not for the _structure_ but the precise _location_ of
this structure within the enclosing triangulation.

Specifically, two snapped 3-balls will compare as equal if and only if
each uses the same numbered tetrahedron, and has the same numbered
equator edge within that tetrahedron. That is, the tetrahedra returned
by tetrahedron() must have equal indices within the triangulation, and
the edge numbers returned by equatorEdge() must be equal. In
particular, it is still meaningful to compare snapped 3-balls within
different triangulations.

Parameter ``other``:
    the snapped 3-ball to compare with this.

Returns:
    ``True`` if and only if this and the given object represent the
    same specific presentation of a snapped 3-ball.)doc";

// Docstring regina::python::doc::SnappedBall_::boundaryFace
static const char *boundaryFace =
R"doc(Returns one of the two tetrahedron faces that forms the boundary of
this snapped ball.

You are guaranteed that index 0 will return a smaller face number than
index 1.

Parameter ``index``:
    specifies which of the two boundary faces to return; this must be
    either 0 or 1.

Returns:
    the corresponding face number in the tetrahedron.)doc";

// Docstring regina::python::doc::SnappedBall_::equatorEdge
static const char *equatorEdge =
R"doc(Returns the edge that forms the equator of the boundary sphere of this
ball.

Returns:
    the corresponding edge number in the tetrahedron.)doc";

// Docstring regina::python::doc::SnappedBall_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls SnappedBall::swap(); it is provided
so that SnappedBall meets the C++ Swappable requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::SnappedBall_::internalEdge
static const char *internalEdge =
R"doc(Returns the edge internal to this snapped ball.

Returns:
    the corresponding edge number in the tetrahedron.)doc";

// Docstring regina::python::doc::SnappedBall_::internalFace
static const char *internalFace =
R"doc(Returns one of the two tetrahedron faces internal to this snapped
ball.

You are guaranteed that index 0 will return a smaller face number than
index 1.

Parameter ``index``:
    specifies which of the two internal faces to return; this must be
    either 0 or 1.

Returns:
    the corresponding face number in the tetrahedron.)doc";

// Docstring regina::python::doc::SnappedBall_::recognise
static const char *recognise =
R"doc(Determines if the given tetrahedron forms a snapped 3-ball within a
triangulation. The ball need not be the entire triangulation; the
boundary triangles may be glued to something else (or to each other).

Note that the two boundary triangles of the snapped 3-ball need not be
boundary triangles within the overall triangulation, i.e., they may be
identified with each other or with triangles of other tetrahedra.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``tet``:
    the tetrahedron to examine as a potential 3-ball.

Returns:
    a structure containing details of the snapped 3-ball, or ``None``
    if the given tetrahedron is not a snapped 3-ball.)doc";

// Docstring regina::python::doc::SnappedBall_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::SnappedBall_::tetrahedron
static const char *tetrahedron =
R"doc(Returns the tetrahedron that forms this snapped ball.

Returns:
    the tetrahedron.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

