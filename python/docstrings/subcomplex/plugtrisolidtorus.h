/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::PlugTriSolidTorus
static const char *PlugTriSolidTorus =
R"doc(Represents a plugged triangular solid torus component of a
triangulation. Such a component is obtained as follows.

Begin with a three-tetrahedron triangular solid torus (as described by
class TriSolidTorus). Observe that the three axis edges divide the
boundary into three annuli.

To each of these annuli a layered chain may be optionally attached. If
present, the chain should be attached so its hinge edges are
identified with the axis edges of the corresonding annulus and its
bottom tetrahedron is layered over either the major edge or minor edge
of the corresponding annulus. The top two triangular faces of the
layered chain should remain free.

Thus we now have three annuli on the boundary, each represented as a
square two of whose (opposite) edges are axis edges of the original
triangular solid torus (and possibly also hinge edges of a layered
chain).

Create a _plug_ by gluing two tetrahedra together along a single
triangle. The six edges that do not run along this common triangle
split the plug boundary into three squares. These three squares must
be glued to the three boundary annuli previously described. Each axis
edge meets two of the annuli; the two corresponding edges of the plug
must be non-adjacent (have no common vertex) on the plug. In this way
each of the six edges of the plug not running along its interior
triangle corresponds to precisely one of the two instances of
precisely one of the three axis edges.

If the axis edges are directed so that they all point the same way
around the triangular solid torus, these axis edges when drawn on the
plug must all point from one common tip of the plug to the other
(where the _tips_ of the plug are the vertices not meeting the
interior triangle). The gluings must also be made so that the
resulting triangulation component is orientable.

Of the optional StandardTriangulation routines, manifold() is
implemented for most plugged triangular solid tori and homology() is
not implemented at all.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
StandardTriangulation subclasses. Note that the only way to create
these objects (aside from copying or moving) is via the static member
function recognise().)doc";

namespace PlugTriSolidTorus_ {

// Docstring regina::python::doc::PlugTriSolidTorus_::__copy
static const char *__copy = R"doc(Creates a new copy of the given structure.)doc";

// Docstring regina::python::doc::PlugTriSolidTorus_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
type of plugged triangular solid torus.

Specifically, two structures will compare as equal if and only if
their equators are of the same (major/minor) type, and the same
numbered annuli either both have no chains attached or both have
chains of the same length attached in the same (major/minor) manner.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass is
more specific than combinatorial isomorphism, since this test does not
account for the many symmetries in a plugged triangular solid torus).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent the
    same type of plugged triangular solid torus.)doc";

// Docstring regina::python::doc::PlugTriSolidTorus_::chain
static const char *chain =
R"doc(Returns the layered chain attached to the requested annulus on the
boundary of the core triangular solid torus. If there is no attached
layered chain, ``None`` will be returned.

Note that the core triangular solid torus will be attached to the
bottom (as opposed to the top) of the layered chain.

Parameter ``annulus``:
    specifies which annulus to examine; this must be 0, 1 or 2.

Returns:
    the corresponding layered chain.)doc";

// Docstring regina::python::doc::PlugTriSolidTorus_::chainType
static const char *chainType =
R"doc(Returns the way in which a layered chain is attached to the requested
annulus on the boundary of the core triangular solid torus. This will
be one of the chain type constants defined in this class.

Parameter ``annulus``:
    specifies which annulus to examine; this must be 0, 1 or 2.

Returns:
    the type of layered chain, or *CHAIN_NONE* if there is no layered
    chain attached to the requested annulus.)doc";

// Docstring regina::python::doc::PlugTriSolidTorus_::core
static const char *core =
R"doc(Returns the triangular solid torus at the core of this triangulation.

Returns:
    the core triangular solid torus.)doc";

// Docstring regina::python::doc::PlugTriSolidTorus_::equatorType
static const char *equatorType =
R"doc(Returns which types of edges form the equator of the plug. In the
absence of layered chains these will either all be major edges or all
be minor edges.

Layered chains complicate matters, but the roles that the major and
minor edges play on the boundary annuli of the triangular solid torus
can be carried up to the annuli at the top of each layered chain; the
edges filling the corresponding major or minor roles will then form
the equator of the plug.

Returns:
    the types of edges that form the equator of the plug; this will be
    one of the equator type constants defined in this class.)doc";

// Docstring regina::python::doc::PlugTriSolidTorus_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls PlugTriSolidTorus::swap(); it is
provided so that PlugTriSolidTorus meets the C++ Swappable
requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::PlugTriSolidTorus_::recognise
static const char *recognise =
R"doc(Determines if the given triangulation component is a plugged
triangular solid torus.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``comp``:
    the triangulation component to examine.

Returns:
    a structure containing details of the plugged triangular solid
    torus, or ``None`` if the given component is not a plugged
    triangular solid torus.)doc";

// Docstring regina::python::doc::PlugTriSolidTorus_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

