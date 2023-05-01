/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::FacetPairing
static const char *FacetPairing =
R"doc(Represents the dual graph of a 3-manifold triangulation.

This is a specialisation of the generic FacetPairing class template;
see the generic FacetPairing documentation for an overview of how the
facet pairing classes work. In Python, you can read this generic
documentation by looking at a higher dimension: try
``help(FacetPairing5)``.

This 3-dimensional specialisation contains some extra functionality.
In particular, it provides routines for finding informative subgraphs
within the dual graph.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace FacetPairing_ {

// Docstring regina::python::doc::FacetPairing_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given face pairing.

Parameter ``src``:
    the face pairing to clone.)doc";

// Docstring regina::python::doc::FacetPairing_::__init
static const char *__init =
R"doc(Creates the face pairing of the given 3-manifold triangulation. This
describes how the tetrahedron faces of the given triangulation are
joined together in pairs.

Calling ``FacetPairing<3>(tri)`` is equivalent to calling
``tri.pairing()``.

Precondition:
    The given triangulation is not empty.

Parameter ``tri``:
    the triangulation whose face pairing should be constructed.)doc";

// Docstring regina::python::doc::FacetPairing_::followChain
static const char *followChain =
R"doc(Follows a chain as far as possible from the given point.

A chain is the underlying face pairing for a layered chain;
specifically it involves one tetrahedron joined to a second along two
faces, the remaining two faces of the second tetrahedron joined to a
third and so on. A chain can involve as few as one tetrahedron or as
many as we like. Note that the remaining two faces of the first
tetrahedron and the remaining two faces of the final tetrahedron
remain unaccounted for by this structure.

This routine begins with two faces of a given tetrahedron, described
by parameters *tet* and *faces*. If these two faces are both joined to
some different tetrahedron, parameter *tet* will be changed to this
new tetrahedron and parameter *faces* will be changed to the remaining
faces of this new tetrahedron (i.e., the two faces that were not
joined to the original faces of the original tetrahedron).

This procedure is repeated as far as possible until either the two
faces in question join to two different tetrahedra, the two faces join
to each other, or at least one of the two faces is unmatched.

Thus, given one end of a chain, this procedure can be used to follow
the face pairings through to the other end of the chain.

.. warning::
    You must be sure when calling this routine that you are not inside
    a chain that loops back onto itself! If the face pairing forms a
    large loop with each tetrahedron joined by two faces to the next,
    this routine will cycle around the loop forever and never return.

Parameter ``tet``:
    the index in the underlying triangulation of the tetrahedron to
    begin at. This parameter will be modified directly by this routine
    as a way of returning the results.

Parameter ``faces``:
    the pair of face numbers in the given tetrahedron at which we
    begin. This parameter will also be modified directly by this
    routine as a way of returning results.)doc";

// Docstring regina::python::doc::FacetPairing_::hasBrokenDoubleEndedChain
static const char *hasBrokenDoubleEndedChain =
R"doc(Determines whether this face pairing contains a broken double-ended
chain.

A chain involves a sequence of tetrahedra, each joined to the next
along two faces, and is described in detail in the documentation for
followChain().

A one-ended chain is a chain in which the first tetrahedron is also
joined to itself along one face (i.e., the underlying face pairing for
a layered solid torus). A double-ended chain is a chain in which the
first tetrahedron is joined to itself along one face and the final
tetrahedron is also joined to itself along one face (i.e., the
underlying face pairing for a layered lens space).

A broken double-ended chain consists of two one-ended chains (using
distinct sets of tetrahedra) joined together along one face. The
remaining two faces (one from each chain) that were unaccounted for by
the individual one-ended chains remain unaccounted for by this broken
double-ended chain.

In this routine we are interested specifically in finding a broken
double-ended chain that is not a part of a complete double-ended
chain, i.e., the final two unaccounted faces are not joined together.

A face pairing containing a broken double-ended chain cannot model a
closed minimal irreducible P²-irreducible 3-manifold triangulation on
more than two tetrahedra. See "Face pairing graphs and 3-manifold
enumeration", Benjamin A. Burton, J. Knot Theory Ramifications 13
(2004), 1057--1101.

Returns:
    ``True`` if and only if this face pairing contains a broken
    double-ended chain that is not part of a complete double-ended
    chain.)doc";

// Docstring regina::python::doc::FacetPairing_::hasDoubleSquare
static const char *hasDoubleSquare =
R"doc(Determines whether this face pairing contains a double-edged square.

A double-edged square involves four distinct tetrahedra that meet each
other as follows. Two pairs of tetrahedra are joined along two pairs
of faces each. Then each tetrahedron is joined along a single face to
one tetrahedron of the other pair. The four tetrahedron faces not yet
joined to anything (one from each tetrahedron) remain unaccounted for
by this structure.

Returns:
    ``True`` if and only if this face pairing contains a double-edged
    square.)doc";

// Docstring regina::python::doc::FacetPairing_::hasDoubleStar
static const char *hasDoubleStar =
R"doc(Determines whether this face pairing contains a double-edged star.

A double-edged star involves two tetrahedra that are adjacent along
two separate pairs of faces, where the four remaining faces of these
tetrahedra are joined to four entirely new tetrahedra (so that none of
the six tetrahedra described in this structure are the same).

Returns:
    ``True`` if and only if this face pairing contains a double-edged
    star.)doc";

// Docstring regina::python::doc::FacetPairing_::hasOneEndedChainWithDoubleHandle
static const char *hasOneEndedChainWithDoubleHandle =
R"doc(Determines whether this face pairing contains a one-ended chain with a
double handle.

A chain involves a sequence of tetrahedra, each joined to the next
along two faces, and is described in detail in the documentation for
followChain().

A one-ended chain is a chain in which the first tetrahedron is also
joined to itself along one face (i.e., the underlying face pairing for
a layered solid torus).

A one-ended chain with a double handle begins with a one-ended chain.
The two faces that are unaccounted for by this one-ended chain must be
joined to two different tetrahedra, and these two tetrahedra must be
joined to each other along two faces. The remaining two faces of these
two tetrahedra remain unaccounted for by this structure.

A face pairing containing a one-ended chain with a double handle
cannot model a closed minimal irreducible P²-irreducible 3-manifold
triangulation on more than two tetrahedra. See "Face pairing graphs
and 3-manifold enumeration", Benjamin A. Burton, J. Knot Theory
Ramifications 13 (2004), 1057--1101.

Returns:
    ``True`` if and only if this face pairing contains a one-ended
    chain with a double handle.)doc";

// Docstring regina::python::doc::FacetPairing_::hasOneEndedChainWithStrayBigon
static const char *hasOneEndedChainWithStrayBigon =
R"doc(Determines whether this face pairing contains a one-ended chain with a
stray bigon.

A chain involves a sequence of tetrahedra, each joined to the next
along two faces, and is described in detail in the documentation for
followChain().

A one-ended chain is a chain in which the first tetrahedron is also
joined to itself along one face (i.e., the underlying face pairing for
a layered solid torus).

A one-ended chain with a stray bigon describes the following
structure. We begin with a one-ended chain. Two new tetrahedra are
added; these are joined to each other along two pairs of faces, and
one of the new tetrahedra is joined to the end of the one-ended chain.
We then ensure that:

* This configuration is not part of a longer one-ended chain that
  encompasses all of the aforementioned tetrahedra;

* There is no extra tetrahedron that is joined to both the two new
  tetrahedra and the end of the chain;

* There is no extra tetrahedron that is joined to the end of the chain
  along one face and the far new tetrahedron along two additional
  faces (where by "the far new tetrahedron" we mean the new
  tetrahedron that was not originally joined to the chain).

Aside from these constraints, the remaining four tetrahedron faces
(two faces of the far new tetrahedron, one face of the other new
tetrahedron, and one face at the end of the chain) remain unaccounted
for by this structure.

A face pairing containing a structure of this type cannot model a
closed minimal irreducible P²-irreducible 3-manifold triangulation on
more than two tetrahedra. See "Enumeration of non-orientable
3-manifolds using face-pairing graphs and union-find", Benjamin A.
Burton, Discrete Comput. Geom. 38 (2007), no. 3, 527--571.

Returns:
    ``True`` if and only if this face pairing contains a one-ended
    chain with a stray bigon.)doc";

// Docstring regina::python::doc::FacetPairing_::hasSingleStar
static const char *hasSingleStar =
R"doc(Determines whether this face pairing contains a single-edged star.

A single-edged star involves two tetrahedra that are adjacent along a
single face, where the six remaining faces of these tetrahedra are
joined to six entirely new tetrahedra (so that none of the eight
tetrahedra described in this structure are the same).

Returns:
    ``True`` if and only if this face pairing contains a single-edged
    star.)doc";

// Docstring regina::python::doc::FacetPairing_::hasTripleEdge
static const char *hasTripleEdge =
R"doc(Determines whether this face pairing contains a triple edge. A triple
edge is where two different tetrahedra are joined along three of their
faces.

A face pairing containing a triple edge cannot model a closed minimal
irreducible P²-irreducible 3-manifold triangulation on more than two
tetrahedra. See "Face pairing graphs and 3-manifold enumeration",
Benjamin A. Burton, J. Knot Theory Ramifications 13 (2004), 1057--
1101.

Returns:
    ``True`` if and only if this face pairing contains a triple edge.)doc";

// Docstring regina::python::doc::FacetPairing_::hasTripleOneEndedChain
static const char *hasTripleOneEndedChain =
R"doc(Determines whether this face pairing contains a triple one-ended
chain.

A chain involves a sequence of tetrahedra, each joined to the next
along two faces, and is described in detail in the documentation for
followChain().

A one-ended chain is a chain in which the first tetrahedron is also
joined to itself along one face (i.e., the underlying face pairing for
a layered solid torus).

A triple one-ended chain is created from three one-ended chains as
follows. Two new tetrahedra are added, and each one-ended chain is
joined to each of the new tetrahedra along a single face. The
remaining two faces (one from each of the new tetrahedra) remain
unaccounted for by this structure.

A face pairing containing a triple one-ended chain cannot model a
closed minimal irreducible P²-irreducible 3-manifold triangulation on
more than two tetrahedra. See "Enumeration of non-orientable
3-manifolds using face-pairing graphs and union-find", Benjamin A.
Burton, Discrete Comput. Geom. 38 (2007), no. 3, 527--571.

Returns:
    ``True`` if and only if this face pairing contains a triple one-
    ended chain.)doc";

// Docstring regina::python::doc::FacetPairing_::hasWedgedDoubleEndedChain
static const char *hasWedgedDoubleEndedChain =
R"doc(Determines whether this face pairing contains a wedged double-ended
chain.

A chain involves a sequence of tetrahedra, each joined to the next
along two faces, and is described in detail in the documentation for
followChain().

A one-ended chain is a chain in which the first tetrahedron is also
joined to itself along one face (i.e., the underlying face pairing for
a layered solid torus). A double-ended chain is a chain in which the
first tetrahedron is joined to itself along one face and the final
tetrahedron is also joined to itself along one face (i.e., the
underlying face pairing for a layered lens space).

A wedged double-ended chain is created from two one-ended chains as
follows. Two new tetrahedra are added, and each one-ended chain is
joined to each of the new tetrahedra along a single face. In addition,
the two new tetrahedra are joined to each other along a single face.
The remaining two faces (one from each of the new tetrahedra) remain
unaccounted for by this structure.

An alternative way of viewing a wedged double-ended chain is as an
ordinary double-ended chain, where one of the internal tetrahedra is
removed and replaced with a pair of tetrahedra joined to each other.
Whereas the original tetrahedron met its two neighbouring tetrahedra
along two faces each (giving a total of four face identifications),
the two new tetrahedra now meet each of the two neighbouring
tetrahedra along a single face each (again giving four face
identifications).

Note that if this alternate construction is used to replace one of the
end tetrahedra of the double-ended chain (not an internal
tetrahedron), the resulting structure will either be a triple edge or
a one-ended chain with a double handle (according to whether the
original chain has zero or positive length). See hasTripleEdge() and
hasOneEndedChainWithDoubleHandle() for further details.

A face pairing containing a wedged double-ended chain cannot model a
closed minimal irreducible P²-irreducible 3-manifold triangulation on
more than two tetrahedra. See "Enumeration of non-orientable
3-manifolds using face-pairing graphs and union-find", Benjamin A.
Burton, Discrete Comput. Geom. 38 (2007), no. 3, 527--571.

Returns:
    ``True`` if and only if this face pairing contains a wedged
    double-ended chain.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

