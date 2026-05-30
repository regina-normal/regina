/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct FacetPairing {

// Docstring regina::python::doc::FacetPairing::__array
static constexpr const char __array[] =
R"doc(Returns the other facet to which the given simplex facet is paired.
This is a convenience operator whose behaviour is identical to that of
dest(const FacetSpec<dim>&).

If the given facet is left deliberately unmatched, the value returned
will be boundary (as returned by FacetSpec<dim>::isBoundary()).

Precondition:
    The given facet is a real simplex facet (not boundary, before-the-
    start or past-the-end).

Python:
    This routine returns by value, not by reference, since Python
    cannot enforce constness otherwise.

Parameter ``source``:
    the facet under investigation.

Returns:
    the other facet to which the given facet is paired.)doc";

// Docstring regina::python::doc::FacetPairing::__class
static constexpr const char __class[] =
R"doc(Represents the dual graph of a *dim*-manifold triangulation; that is,
the pairwise matching of facets of *dim*-dimensional simplices.

Given a fixed number of *dim*-dimensional simplices, each facet of
each simplex is either paired with some other simplex facet (which is
in turn paired with it) or remains unmatched. A simplex facet cannot
be paired with itself.

Such a matching models part of the structure of a *dim*-manifold
triangulation, in which each simplex facet is either glued to some
other simplex facet (which is in turn glued to it) or is an unglued
boundary facet. Note however that a facet pairing does not contain
enough information to fully reconstruct a triangulation, since the
permutations used for each individual gluing are not stored.

Facet pairings are _labelled_, in that the simplices are explicitly
numbered 0,1,..., and the facets of each simplex are explicitly
numbered 0,...,*dim* (just like in a triangulation). Facet pairings do
also come with code to help identify and work with relabellings, via
isomorphisms, automorphisms, and canonical representations. In this
context:

* An _isomorphism_ of a facet pairing means a relabelling of the
  simplices and a relabelling of the (*dim* + 1) facets within each
  simplex; this can be represented by the same class Isomorphism<dim>
  that is used for isomorphisms of triangulations.

* An _automorphism_ of a facet pairing is an isomorphism that, when
  applied, results in an identical facet pairing (i.e., where exactly
  the same pairs of labelled simplex facets are matched together).

* A facet pairing is in _canonical form_ if it is a lexicographically
  minimal representative of its isomorphism class. Here we order facet
  pairings by lexicographical comparison of the sequence
  ``dest(0,0)``, ``dest(0,1)``, ..., ``dest(size()-1, dim)`` (which in
  turn uses the ordering defined by FacetSpec<dim>, where each simplex
  facet is ordered first by simplex number and then by facet number,
  and where the boundary is ordered last).

In dimension 3, this class offers extra functionality compared to
other dimensions. In particular, it offers several extra routines for
finding informative subgraphs within the dual graph.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g., FacetPairing2 and
    FacetPairing3 for dimensions 2 and 3).

Template parameter ``dim``:
    the dimension of the underlying triangulation.)doc";

// Docstring regina::python::doc::FacetPairing::__copy
static constexpr const char __copy[] =
R"doc(Creates a new copy of the given facet pairing.

Parameter ``src``:
    the facet pairing to clone.)doc";

// Docstring regina::python::doc::FacetPairing::__eq
static constexpr const char __eq[] =
R"doc(Determines if this and the given facet pairing are identical.

Parameter ``other``:
    the facet pairing to compare with this.

Returns:
    ``True`` if and only if this and the given facet pairing are
    identical.)doc";

// Docstring regina::python::doc::FacetPairing::__init
static constexpr const char __init[] =
R"doc(Creates the dual graph of the given triangulation. This is the facet
pairing that describes how the facets of top-dimensional simplices in
the given triangulation are joined together, as described in the class
notes.

Calling ``FacetPairing<dim>(tri)`` is equivalent to calling
``tri.pairing()``.

Precondition:
    The given triangulation is not empty.

Parameter ``tri``:
    the triangulation whose facet pairing should be constructed.)doc";

// Docstring regina::python::doc::FacetPairing::canonical
static constexpr const char canonical[] =
R"doc(Returns the canonical form of this facet pairing, along with one
isomorphism that transforms this pairing into canonial form.

Note that, while the canoncial form is uniquely determined, the
isomorphism is not (since the facet pairing could have non-trivial
automorphisms). If you need _all_ such isomorphisms then you should
call canonicalAll() instead.

See the FacetPairing class notes for more information on isomorphisms,
automorphisms and canonical form.

Precondition:
    This facet pairing is connected, i.e., it is possible to reach any
    simplex from any other simplex via a series of matched facet
    pairs.

Returns:
    a pair (*c*, *iso*), where *c* is the canonical form and *iso* is
    one isomorphism that converts this facet pairing into *c*.)doc";

// Docstring regina::python::doc::FacetPairing::canonicalAll
static constexpr const char canonicalAll[] =
R"doc(Returns the canonical form of this facet pairing, along with the list
of all isomorphisms that transform this pairing into canonial form.

Note that the list that is returned will be a left coset of the
automorphism group of this facet pairing, and also a right coset of
the automorphism group of the canonical form.

If you only need one such isomorphism (not all), then you should call
canonical() instead.

See the FacetPairing class notes for more information on isomorphisms,
automorphisms and canonical form.

Precondition:
    This facet pairing is connected, i.e., it is possible to reach any
    simplex from any other simplex via a series of matched facet
    pairs.

Returns:
    a pair (*c*, *isos*), where *c* is the canonical form and *isos*
    is the list of all isomorphisms that convert this facet pairing
    into *c*.)doc";

// Docstring regina::python::doc::FacetPairing::dest
static constexpr const char dest[] =
R"doc(Returns the other facet to which the given simplex facet is paired. If
the given facet is left deliberately unmatched, the value returned
will be boundary (as returned by FacetSpec<dim>::isBoundary()).

Precondition:
    The given facet is a real simplex facet (not boundary, before-the-
    start or past-the-end).

Python:
    This routine returns by value, not by reference, since Python
    cannot enforce constness otherwise.

Parameter ``source``:
    the facet under investigation.

Returns:
    the other facet to which the given facet is paired.)doc";

// Docstring regina::python::doc::FacetPairing::dest_2
static constexpr const char dest_2[] =
R"doc(Returns the other facet to which the given simplex facet is paired. If
the given facet is left deliberately unmatched, the value returned
will be boundary (as returned by FacetSpec<dim>::isBoundary()).

Python:
    This routine returns by value, not by reference, since Python
    cannot enforce constness otherwise.

Parameter ``simp``:
    the simplex under investigation (this must be strictly less than
    the total number of simplices under consideration).

Parameter ``facet``:
    the facet of the given simplex under investigation (between 0 and
    *dim* inclusive).

Returns:
    the other facet to which the given facet is paired.)doc";

// Docstring regina::python::doc::FacetPairing::divideConnected
static constexpr const char divideConnected[] =
R"doc(Returns a cut that divides this facet pairing into two connected
pieces, both of size at least *minSide*.

If solutions exist, then the cut that is returned will have minimum
weight amongst all solutions (i.e., will have the smallest number of
matched simplex facets that cross the two sides of the resulting
partition). If there are still multiple solutions, then the cut that
is returned will have the two pieces with sizes that are as close as
possible to equal. If there are _still_ multiple solutions, then the
choice will be arbitrary.

Note that it is possible that no solution exists (e.g. this could
happen if the matching is a star graph and *minSide* is greater than
1).

.. warning::
    Currently the implementation of this routine is exhaustive, and so
    the running time is exponential in the size of this facet pairing.

Parameter ``minSide``:
    the minimum number of simplices in each of the two connected
    pieces; this must be at least 1.

Returns:
    the best possible cut as described above, or ``None`` if no such
    cut exists.)doc";

// Docstring regina::python::doc::FacetPairing::dot
static constexpr const char dot[] =
R"doc(Returns a Graphviz DOT representation of the graph that describes this
facet pairing.

Every vertex of this graph represents a simplex, and every edge
represents a pair of simplex facets that are joined together. Note
that for a closed triangulation this graph will be entirely (*dim* +
1)-valent; for triangulations with boundary facets, some graph
vertices will have degree *dim* or less.

The graph can either be written as a complete DOT graph, or as a
clustered subgraph within some larger DOT graph (according to whether
the argument *subgraph* is passed as ``False`` or ``True``).

If a complete DOT graph is being written, the output may be used as a
standalone DOT file ready for use with Graphviz.

If a subgraph is being written, the output will contain a single
``subgraph`` section that should be inserted into some larger DOT
file. Note that the output generated by dotHeader() or
writeDotHeader(), followed by one or more subgraphs and then a closing
curly brace will suffice. The subgraph name will begin with the string
``pairing_``.

The argument *prefix* will be prepended to the name of each graph
vertex, and will also be used in the name of the graph or subgraph.
Using unique prefixes becomes important if you are calling dot() or
writeDot() several times to generate several subgraphs for use in a
single DOT file. If the *prefix* argument is null or empty then a
default prefix will be used.

Note that this routine generates undirected graphs, not directed
graphs. The final DOT file should be used with either the *neato* or
*fdp* programs shipped with Graphviz.

If you are simply writing this string to an output stream then you
should call writeDot() instead, which is more efficient.

Parameter ``prefix``:
    a string to prepend to the name of each graph vertex, and to
    include in the graph or subgraph name; see above for details.

Parameter ``subgraph``:
    ``False`` if a complete standalone DOT graph should be output, or
    ``True`` if a clustered subgraph should be output for use in some
    larger DOT file.

Parameter ``labels``:
    indicates whether graph vertices will be labelled with the
    corresponding simplex numbers.

Returns:
    the output of writeDot(), as outlined above.)doc";

// Docstring regina::python::doc::FacetPairing::dotHeader
static constexpr const char dotHeader[] =
R"doc(Returns header information for a Graphviz DOT file that will describe
the graphs for one or more facet pairings. See the dot() documentation
for further information on such graphs.

The output will be in the Graphviz DOT language, and will include
appropriate display settings for graphs, edges and nodes. The opening
brace for a ``graph`` section of the DOT file is included.

This routine may be used with dot() or writeDot() to generate a single
DOT file containing the graphs for several different facet pairings. A
complete DOT file can be produced by calling this routine, then
calling dot() or writeDot() in subgraph mode for each facet pairing,
then outputting a final closing curly brace.

Note that if you require a DOT file containing the graph for only a
single facet pairing, this routine is unnecessary; you may simply call
dot() or writeDot() in full graph mode instead.

This routine is suitable for generating undirected graphs, not
directed graphs. The final DOT file should be used with either the
*neato* or *fdp* programs shipped with Graphviz.

If you are simply writing this string to an output stream then you
should call writeDotHeader() instead, which is more efficient.

Parameter ``graphName``:
    the name of the graph to write in the DOT header. If this is null
    or empty then a default graph name will be used.

Returns:
    the DOT header information, as outlined above.

See also:
    http://www.graphviz.org/)doc";

// Docstring regina::python::doc::FacetPairing::findAllPairings
static constexpr const char findAllPairings[] =
R"doc(Generates all possible facet pairings satisfying the given
constraints. Only connected facet pairings (pairings in which each
simplex can be reached from each other via a series of individual
matched facets) will be produced.

Each facet pairing will be produced precisely once up to isomorphism.
Facet pairings are considered isomorphic if they are related by a
relabelling of the simplices and/or a renumbering of the (*dim* + 1)
facets of each simplex. Each facet pairing that is generated will be a
lexicographically minimal representative of its isomorphism class,
i.e., will be in canonical form as described by isCanonical().

For each facet pairing that is generated, this routine will call
*action* (which must be a function or some other callable type).

* The first argument to *action* must be a const reference to a
  FacetPairing<dim>. This will be the facet pairing that was found. If
  *action* wishes to keep the facet pairing, it should take a deep
  copy (not a reference), since the facet pairing may be changed and
  reused after *action* returns.

* If *action* takes a FacetPairing<dim>::IsoList as its second
  argument (which may be as a reference, and may have const/volatile
  qualifiers), then this will be the list of all automorphisms of the
  facet pairing that was found. This list will be passed by value
  using move semantics. If *action* does not take a second argument,
  or if the second argument is of a different type, then the list of
  automorphisms will not be passed.

* If there are any additional arguments supplied in the list *args*,
  then these will be passed as subsequent arguments to *action*.

* The return value of *action* will be ignored; typically it would
  return ``void``.

Because this class cannot represent an empty facet pairing, if the
argument *nSimplices* is zero then no facet pairings will be generated
at all.

.. warning::
    If you are allowing a large number of boundary facets, then the
    automorphisms groups could be enormous. In this case it is highly
    recommended that your action does _not_ take the list of all
    automorphisms as its second argument, since this will avoid the
    enormous memory cost of storing and passing such a list.

Python:
    This function is available, and *action* may be a pure Python
    function. However, its form is more restricted: *action* must take
    both a facet pairing and its automorphisms (i.e., the
    automorphisms argument is not optional); moreover, it cannot take
    any additional arguments beyond these. As a consequence, the
    additional *args* list is omitted also.

Parameter ``nSimplices``:
    the number of simplices whose facets should be (potentially)
    matched.

Parameter ``boundary``:
    determines whether any facets may be left unmatched. This set
    should contain ``True`` if pairings with at least one unmatched
    facet are to be generated, and should contain ``False`` if
    pairings with no unmatched facets are to be generated.

Parameter ``nBdryFacets``:
    specifies the precise number of facets that should be left
    unmatched. If this parameter is negative, it is ignored and no
    additional restriction is imposed. If parameter *boundary* does
    not contain ``True``, this parameter is likewise ignored. If
    parameter *boundary* does contain true and this parameter is non-
    negative, only pairings with precisely this many unmatched facets
    will be generated. In particular, if this parameter is positive
    then pairings with no unmatched facets will not be produced
    irrespective of whether ``False`` is contained in parameter
    *boundary*. Note that, in order to produce any pairings at all,
    this parameter must be of the same parity as ``nSimplices *
    (dim+1)``, and can be at most ``(dim-1) * nSimplices + 2``.

Parameter ``action``:
    a function (or other callable type) to call for each facet pairing
    that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial facet pairing argument and the optional
    automorphism argument.)doc";

// Docstring regina::python::doc::FacetPairing::findAutomorphisms
static constexpr const char findAutomorphisms[] =
R"doc(Returns the set of all combinatorial automorphisms of this facet
pairing, assuming the pairing is already in canonical form.

See the FacetPairing class notes for more information on isomorphisms,
automorphisms and canonical form.

Precondition:
    This facet pairing is connected, i.e., it is possible to reach any
    simplex from any other simplex via a series of matched facet
    pairs.

Precondition:
    This facet pairing is in canonical form. This is crucial, since
    this routine uses optimisations that can cause unpredictable
    breakages if this facet pairing is not in canonical form.

Returns:
    the list of all automorphisms.)doc";

// Docstring regina::python::doc::FacetPairing::followChain
static constexpr const char followChain[] =
R"doc(Follows a chain in a 3-dimensional facet pairing as far as possible
from the given point.

A chain in a 3-dimensional facet pairing is the underlying facet
pairing for a layered chain; specifically it involves one tetrahedron
joined to a second along two faces, the remaining two faces of the
second tetrahedron joined to a third and so on. A chain can involve as
few as one tetrahedron or as many as we like. Note that the remaining
two faces of the first tetrahedron and the remaining two faces of the
final tetrahedron remain unaccounted for by this structure.

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
    a chain that loops back onto itself! If the facet pairing forms a
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

// Docstring regina::python::doc::FacetPairing::fromTextRep
static constexpr const char fromTextRep[] =
R"doc(Reconstructs a facet pairing from a text-based representation. This
text-based representation must be in the format produced by routine
textRep().

Exception ``InvalidArgument``:
    The given string was not a valid text-based representation of a
    facet pairing on a positive number of simplices.

Parameter ``rep``:
    a text-based representation of a facet pairing, as produced by
    routine textRep().

Returns:
    the corresponding facet pairing.)doc";

// Docstring regina::python::doc::FacetPairing::global_swap
static constexpr const char global_swap[] =
R"doc(Swaps the contents of the given facet pairings.

This global routine simply calls FacetPairing<dim>::swap(); it is
provided so that FacetPairing<dim> meets the C++ Swappable
requirements.

Parameter ``a``:
    the first facet pairing whose contents should be swapped.

Parameter ``b``:
    the second facet pairing whose contents should be swapped.)doc";

// Docstring regina::python::doc::FacetPairing::hasBrokenDoubleEndedChain
static constexpr const char hasBrokenDoubleEndedChain[] =
R"doc(Determines whether this 3-dimensional facet pairing contains a broken
double-ended chain.

A chain involves a sequence of tetrahedra, each joined to the next
along two faces, and is described in detail in the documentation for
followChain().

A one-ended chain is a chain in which the first tetrahedron is also
joined to itself along one face (i.e., the underlying facet pairing
for a layered solid torus). A double-ended chain is a chain in which
the first tetrahedron is joined to itself along one face and the final
tetrahedron is also joined to itself along one face (i.e., the
underlying facet pairing for a layered lens space).

A broken double-ended chain consists of two one-ended chains (using
distinct sets of tetrahedra) joined together along one face. The
remaining two faces (one from each chain) that were unaccounted for by
the individual one-ended chains remain unaccounted for by this broken
double-ended chain.

In this routine we are interested specifically in finding a broken
double-ended chain that is not a part of a complete double-ended
chain, i.e., the final two unaccounted faces are not joined together.

A facet pairing containing a broken double-ended chain cannot model a
closed minimal irreducible P²-irreducible 3-manifold triangulation on
more than two tetrahedra. See "Face pairing graphs and 3-manifold
enumeration", Benjamin A. Burton, J. Knot Theory Ramifications 13
(2004), 1057--1101.

Returns:
    ``True`` if and only if this facet pairing contains a broken
    double-ended chain that is not part of a complete double-ended
    chain.)doc";

// Docstring regina::python::doc::FacetPairing::hasDoubleEdge
static constexpr const char hasDoubleEdge[] =
R"doc(Determines whether the underlying graph for this facet pairing
contains a double edge.

This corresponds to two distinct top-dimensional simplices joined to
each other along at least two of their facets.

Note that this routine does not consider loops in the underlying
graph, only edges between distinct nodes.

This routine is identical to calling ``hasMultiEdge<2>()``.

Returns:
    ``True`` if and only if the underlying graph has a double edge.)doc";

// Docstring regina::python::doc::FacetPairing::hasDoubleSquare
static constexpr const char hasDoubleSquare[] =
R"doc(Determines whether this 3-dimensional facet pairing contains a double-
edged square.

A double-edged square involves four distinct tetrahedra that meet each
other as follows. Two pairs of tetrahedra are joined along two pairs
of faces each. Then each tetrahedron is joined along a single face to
one tetrahedron of the other pair. The four tetrahedron faces not yet
joined to anything (one from each tetrahedron) remain unaccounted for
by this structure.

Returns:
    ``True`` if and only if this facet pairing contains a double-edged
    square.)doc";

// Docstring regina::python::doc::FacetPairing::hasDoubleStar
static constexpr const char hasDoubleStar[] =
R"doc(Determines whether this 3-dimensional facet pairing contains a double-
edged star.

A double-edged star involves two tetrahedra that are adjacent along
two separate pairs of faces, where the four remaining faces of these
tetrahedra are joined to four entirely new tetrahedra (so that none of
the six tetrahedra described in this structure are the same).

Returns:
    ``True`` if and only if this facet pairing contains a double-edged
    star.)doc";

// Docstring regina::python::doc::FacetPairing::hasMultiEdge
static constexpr const char hasMultiEdge[] =
R"doc(Determines whether the underlying graph for this facet pairing
contains an edge of multiplicity *k*, where *k* does not need to be
known until runtime.

An edge of multiplicity *k* corresponds to two distinct top-
dimensional simplices joined to each other along at least *k* of their
facets.

Note that this routine does not consider loops in the underlying
graph, only edges between distinct nodes.

For C++ programmers who know *k* at compile time, it is faster to call
the template function ``hasMultiEdge<k>()`` instead.

For the cases ``k = 2`` and ``k = 3``, you can also access this
functionality via the aliases hasDoubleEdge() and hasTripleEdge().

Exception ``InvalidArgument``:
    The argument *k* is outside the supported range.

Parameter ``k``:
    the multiplicity of edges to search for; this must be between 2
    and ``dim+1`` inclusive.

Returns:
    ``True`` if and only if the underyling graph has an edge of
    multiplicity *k*.)doc";

// Docstring regina::python::doc::FacetPairing::hasOneEndedChainWithDoubleHandle
static constexpr const char hasOneEndedChainWithDoubleHandle[] =
R"doc(Determines whether this 3-dimensional facet pairing contains a one-
ended chain with a double handle.

A chain involves a sequence of tetrahedra, each joined to the next
along two faces, and is described in detail in the documentation for
followChain().

A one-ended chain is a chain in which the first tetrahedron is also
joined to itself along one face (i.e., the underlying facet pairing
for a layered solid torus).

A one-ended chain with a double handle begins with a one-ended chain.
The two faces that are unaccounted for by this one-ended chain must be
joined to two different tetrahedra, and these two tetrahedra must be
joined to each other along two faces. The remaining two faces of these
two tetrahedra remain unaccounted for by this structure.

A facet pairing containing a one-ended chain with a double handle
cannot model a closed minimal irreducible P²-irreducible 3-manifold
triangulation on more than two tetrahedra. See "Face pairing graphs
and 3-manifold enumeration", Benjamin A. Burton, J. Knot Theory
Ramifications 13 (2004), 1057--1101.

Returns:
    ``True`` if and only if this facet pairing contains a one-ended
    chain with a double handle.)doc";

// Docstring regina::python::doc::FacetPairing::hasOneEndedChainWithStrayBigon
static constexpr const char hasOneEndedChainWithStrayBigon[] =
R"doc(Determines whether this 3-dimensional facet pairing contains a one-
ended chain with a stray bigon.

A chain involves a sequence of tetrahedra, each joined to the next
along two faces, and is described in detail in the documentation for
followChain().

A one-ended chain is a chain in which the first tetrahedron is also
joined to itself along one face (i.e., the underlying facet pairing
for a layered solid torus).

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

A facet pairing containing a structure of this type cannot model a
closed minimal irreducible P²-irreducible 3-manifold triangulation on
more than two tetrahedra. See "Enumeration of non-orientable
3-manifolds using face-pairing graphs and union-find", Benjamin A.
Burton, Discrete Comput. Geom. 38 (2007), no. 3, 527--571.

Returns:
    ``True`` if and only if this facet pairing contains a one-ended
    chain with a stray bigon.)doc";

// Docstring regina::python::doc::FacetPairing::hasSingleStar
static constexpr const char hasSingleStar[] =
R"doc(Determines whether this 3-dimensional facet pairing contains a single-
edged star.

A single-edged star involves two tetrahedra that are adjacent along a
single face, where the six remaining faces of these tetrahedra are
joined to six entirely new tetrahedra (so that none of the eight
tetrahedra described in this structure are the same).

Returns:
    ``True`` if and only if this facet pairing contains a single-edged
    star.)doc";

// Docstring regina::python::doc::FacetPairing::hasTripleEdge
static constexpr const char hasTripleEdge[] =
R"doc(Determines whether the underlying graph for this facet pairing
contains a triple edge.

This corresponds to two distinct top-dimensional simplices joined to
each other along at least three of their facets.

Note that this routine does not consider loops in the underlying
graph, only edges between distinct nodes.

In dimension 3, a facet pairing containing a triple edge cannot model
a closed minimal irreducible P²-irreducible 3-manifold triangulation
on more than two tetrahedra. See "Face pairing graphs and 3-manifold
enumeration", Benjamin A. Burton, J. Knot Theory Ramifications 13
(2004), 1057--1101.

This routine is identical to calling ``hasMultiEdge<3>()``.

Returns:
    ``True`` if and only if the underlying graph has a triple edge.)doc";

// Docstring regina::python::doc::FacetPairing::hasTripleOneEndedChain
static constexpr const char hasTripleOneEndedChain[] =
R"doc(Determines whether this 3-dimensional facet pairing contains a triple
one-ended chain.

A chain involves a sequence of tetrahedra, each joined to the next
along two faces, and is described in detail in the documentation for
followChain().

A one-ended chain is a chain in which the first tetrahedron is also
joined to itself along one face (i.e., the underlying facet pairing
for a layered solid torus).

A triple one-ended chain is created from three one-ended chains as
follows. Two new tetrahedra are added, and each one-ended chain is
joined to each of the new tetrahedra along a single face. The
remaining two faces (one from each of the new tetrahedra) remain
unaccounted for by this structure.

A facet pairing containing a triple one-ended chain cannot model a
closed minimal irreducible P²-irreducible 3-manifold triangulation on
more than two tetrahedra. See "Enumeration of non-orientable
3-manifolds using face-pairing graphs and union-find", Benjamin A.
Burton, Discrete Comput. Geom. 38 (2007), no. 3, 527--571.

Returns:
    ``True`` if and only if this facet pairing contains a triple one-
    ended chain.)doc";

// Docstring regina::python::doc::FacetPairing::hasWedgedDoubleEndedChain
static constexpr const char hasWedgedDoubleEndedChain[] =
R"doc(Determines whether this 3-dimensional facet pairing contains a wedged
double-ended chain.

A chain involves a sequence of tetrahedra, each joined to the next
along two faces, and is described in detail in the documentation for
followChain().

A one-ended chain is a chain in which the first tetrahedron is also
joined to itself along one face (i.e., the underlying facet pairing
for a layered solid torus). A double-ended chain is a chain in which
the first tetrahedron is joined to itself along one face and the final
tetrahedron is also joined to itself along one face (i.e., the
underlying facet pairing for a layered lens space).

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

A facet pairing containing a wedged double-ended chain cannot model a
closed minimal irreducible P²-irreducible 3-manifold triangulation on
more than two tetrahedra. See "Enumeration of non-orientable
3-manifolds using face-pairing graphs and union-find", Benjamin A.
Burton, Discrete Comput. Geom. 38 (2007), no. 3, 527--571.

Returns:
    ``True`` if and only if this facet pairing contains a wedged
    double-ended chain.)doc";

// Docstring regina::python::doc::FacetPairing::isCanonical
static constexpr const char isCanonical[] =
R"doc(Determines whether this facet pairing is in canonical form.

See the FacetPairing class notes for more information on isomorphisms,
automorphisms and canonical form.

Precondition:
    This facet pairing is connected, i.e., it is possible to reach any
    simplex from any other simplex via a series of matched facet
    pairs.

Returns:
    ``True`` if and only if this facet pairing is in canonical form.)doc";

// Docstring regina::python::doc::FacetPairing::isClosed
static constexpr const char isClosed[] =
R"doc(Determines whether this facet pairing is closed. A closed facet
pairing has no unmatched facets.)doc";

// Docstring regina::python::doc::FacetPairing::isConnected
static constexpr const char isConnected[] =
R"doc(Determines whether this facet pairing is connected.

A facet pairing is _connected_ if it is possible to reach any simplex
from any other simplex via a series of matched facet pairs.

For this purpose, the empty facet pairing is considered to be
connected.

Returns:
    ``True`` if and only if this pairing is connected.)doc";

// Docstring regina::python::doc::FacetPairing::isUnmatched
static constexpr const char isUnmatched[] =
R"doc(Determines whether the given simplex facet has been left deliberately
unmatched.

Precondition:
    The given facet is a real simplex facet (not boundary, before-the-
    start or past-the-end).

Parameter ``source``:
    the facet under investigation.

Returns:
    ``True`` if the given facet has been left unmatched, or ``False``
    if the given facet is paired with some other facet.)doc";

// Docstring regina::python::doc::FacetPairing::isUnmatched_2
static constexpr const char isUnmatched_2[] =
R"doc(Determines whether the given simplex facet has been left deliberately
unmatched.

Parameter ``simp``:
    the simplex under investigation (this must be strictly less than
    the total number of simplices under consideration).

Parameter ``facet``:
    the facet of the given simplex under investigation (between 0 and
    *dim* inclusive).

Returns:
    ``True`` if the given facet has been left unmatched, or ``False``
    if the given facet is paired with some other facet.)doc";

// Docstring regina::python::doc::FacetPairing::size
static constexpr const char size[] =
R"doc(Returns the number of simplices whose facets are described by this
facet pairing.

Python:
    This is also used to implement the Python special method
    __len__().

Returns:
    the number of simplices under consideration.)doc";

// Docstring regina::python::doc::FacetPairing::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given facet pairing.

Parameter ``other``:
    the facet pairing whose contents are to be swapped with this.)doc";

// Docstring regina::python::doc::FacetPairing::textRep
static constexpr const char textRep[] =
R"doc(Returns a text-based representation that can be used to reconstruct
this facet pairing. This reconstruction is done through the routine
fromTextRep().

The text produced is not particularly readable; for a human-readable
text representation, see routine str() instead.

The string returned will contain no newlines.

Returns:
    a text-based representation of this facet pairing.)doc";

// Docstring regina::python::doc::FacetPairing::toTextRep
static constexpr const char toTextRep[] =
R"doc(Deprecated routine that returns a text-based representation that can
be used to reconstruct this facet pairing.

.. deprecated::
    This routine has been renamed to textRep(). See the textRep()
    documentation for further details.

Returns:
    a text-based representation of this facet pairing.)doc";

}; // struct FacetPairing

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

