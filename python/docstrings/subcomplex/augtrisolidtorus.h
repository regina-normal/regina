/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::AugTriSolidTorus
static const char *AugTriSolidTorus =
R"doc(Represents an augmented triangular solid torus component of a
triangulation. Such a component is obtained as follows. Begin with a
three-tetrahedron triangular solid torus (as described by
TriSolidTorus). Observe that the three axis edges divide the boundary
into three annuli. Then take one of the following actions.

* To each of these annuli, glue a layered solid torus. Note that the
  degenerate (2,1,1) layered solid torus (i.e., a one-triangle mobius
  strip) is allowed and corresponds to simply gluing the two triangles
  of the annulus together.

* To one of these annuli, glue a layered solid torus as described
  above. Join the other two annuli with a layered chain in either the
  manner described by TriSolidTorus::areAnnuliLinkedMajor() or the
  manner described by TriSolidTorus::areAnnuliLinkedAxis().

It will be assumed that all layered solid tori other than the
degenerate (2,1,1) will have (3,2,1) layered solid tori at their
bases. That is, layered solid tori that begin with the degenerate
(2,1,1) and layer over the boundary of the mobius strip are **not**
considered in this class.

Note that (unless a (1,1,0) layered solid torus is used with the 0
edge glued to an axis edge) the resulting space will be a Seifert
fibred space over the 2-sphere with at most three exceptional fibres.

Of the optional StandardTriangulation routines, manifold() is
implemented for most augmented triangular solid tori and homology() is
not implemented at all.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the other
StandardTriangulation subclasses. Note that the only way to create
these objects (aside from copying or moving) is via the static member
function recognise().)doc";

namespace AugTriSolidTorus_ {

// Docstring regina::python::doc::AugTriSolidTorus_::__copy
static const char *__copy = R"doc(Creates a new copy of this structure.)doc";

// Docstring regina::python::doc::AugTriSolidTorus_::__eq
static const char *__eq =
R"doc(Determines whether this and the given structure represent the same
type of augmented triangular solid torus.

Specifically, two structures will compare as equal if and only if:

* in both structures, the layered solid tori attached to the same
  numbered annuli have the same three integer parameters, and have
  their top level edge groups attached to the annuli in the same
  manner;

* either both structures do not include a layered chain, or else both
  structures do include a layered chain of the same type and length,
  attached to the same numbered annulus.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass is
more specific than combinatorial isomorphism, since this test does not
account for the many symmetries in an augmented triangular solid
torus).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent the
    same type of augmented triangular solid torus.)doc";

// Docstring regina::python::doc::AugTriSolidTorus_::__ne
static const char *__ne =
R"doc(Determines whether this and the given structure represent different
types of augmented triangular solid torus.

Specifically, two structures will compare as equal if and only if:

* in both structures, the layered solid tori attached to the same
  numbered annuli have the same three integer parameters, and have
  their top level edge groups attached to the annuli in the same
  manner;

* either both structures do not include a layered chain, or else both
  structures do include a layered chain of the same type and length,
  attached to the same numbered annulus.

This test follows the general rule for most subclasses of
StandardTriangulation (excluding fixed structures such as SnappedBall
and TriSolidTorus): two objects compare as equal if and only if they
have the same combinatorial parameters (which for this subclass is
more specific than combinatorial isomorphism, since this test does not
account for the many symmetries in an augmented triangular solid
torus).

Parameter ``other``:
    the structure with which this will be compared.

Returns:
    ``True`` if and only if this and the given structure represent
    different types of augmented triangular solid torus.)doc";

// Docstring regina::python::doc::AugTriSolidTorus_::augTorus
static const char *augTorus =
R"doc(Returns the layered solid torus attached to the requested annulus on
the boundary of the core triangular solid torus. If the layered solid
torus is a degenerate (2,1,1) mobius band (i.e., the two triangles of
the corresponding annulus have simply been glued together), then no
value will be returned.

Parameter ``annulus``:
    specifies which annulus to examine; this must be 0, 1 or 2.

Returns:
    the corresponding layered solid torus.)doc";

// Docstring regina::python::doc::AugTriSolidTorus_::chainLength
static const char *chainLength =
R"doc(Returns the number of tetrahedra in the layered chain linking two of
the boundary annuli of the core triangular solid torus. Note that this
count does not include any of the tetrahedra actually belonging to the
triangular solid torus.

Returns:
    the number of tetrahedra in the layered chain, or 0 if there is no
    layered chain linking two boundary annuli.)doc";

// Docstring regina::python::doc::AugTriSolidTorus_::chainType
static const char *chainType =
R"doc(Returns the way in which a layered chain links two of the boundary
annuli of the core triangular solid torus. This will be one of the
chain type constants defined in this class.

Returns:
    the type of layered chain, or *CHAIN_NONE* if there is no layered
    chain linking two boundary annuli.)doc";

// Docstring regina::python::doc::AugTriSolidTorus_::core
static const char *core =
R"doc(Returns the triangular solid torus at the core of this triangulation.

Returns:
    the core triangular solid torus.)doc";

// Docstring regina::python::doc::AugTriSolidTorus_::edgeGroupRoles
static const char *edgeGroupRoles =
R"doc(Returns a permutation describing the role played by each top level
edge group of the layered solid torus glued to the requested annulus
of the core triangular solid torus. See LayeredSolidTorus::topEdge()
for details regarding edge groups.

If the permutation returned is ``p``, edge group ``p[0]`` will be
glued to an axis edge, group ``p[1]`` will be glued to a major edge
and group ``p[2]`` will be glued to a minor edge.

Even if the corresponding layered solid torus is a degenerate (2,1,1)
mobius band (i.e., augTorus() returns ``null``), the concept of edge
groups is still meaningful and this routine will return correct
results.

Parameter ``annulus``:
    specifies which annulus to examine; this must be 0, 1 or 2. It is
    the layered solid torus glued to this annulus whose edge groups
    will be described.

Returns:
    a permutation describing the roles of the corresponding top level
    edge groups.)doc";

// Docstring regina::python::doc::AugTriSolidTorus_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given structures.

This global routine simply calls AugTriSolidTorus::swap(); it is
provided so that AugTriSolidTorus meets the C++ Swappable
requirements.

Parameter ``a``:
    the first structure whose contents should be swapped.

Parameter ``b``:
    the second structure whose contents should be swapped.)doc";

// Docstring regina::python::doc::AugTriSolidTorus_::hasLayeredChain
static const char *hasLayeredChain =
R"doc(Determines whether the core triangular solid torus has two of its
boundary annuli linked by a layered chain as described in the general
class notes.

Returns:
    ``True`` if and only if the layered chain described in the class
    notes is present.)doc";

// Docstring regina::python::doc::AugTriSolidTorus_::recognise
static const char *recognise =
R"doc(Determines if the given triangulation component is an augmented
triangular solid torus.

This function returns by (smart) pointer for consistency with
StandardTriangulation::recognise(), which makes use of the polymorphic
nature of the StandardTriangulation class hierarchy.

Parameter ``comp``:
    the triangulation component to examine.

Returns:
    a structure containing details of the augmented triangular solid
    torus, or ``null`` if the given component is not an augmented
    triangular solid torus.)doc";

// Docstring regina::python::doc::AugTriSolidTorus_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given structure.

Parameter ``other``:
    the structure whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::AugTriSolidTorus_::torusAnnulus
static const char *torusAnnulus =
R"doc(Returns the single boundary annulus of the core triangular solid torus
to which a layered solid torus is attached. This routine is only
meaningful if the other two annuli are linked by a layered chain.

The integer returned will be 0, 1 or 2; see the TriSolidTorus class
notes for how the boundary annuli are numbered.

Returns:
    the single annulus to which the layered solid torus is attached,
    or -1 if there is no layered chain (and thus all three annuli have
    layered solid tori attached).)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

