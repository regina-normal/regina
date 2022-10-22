/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


namespace FacetPairing_ {

// Docstring regina::python::doc::FacetPairing_::global_swap
constexpr const char *global_swap =
R"doc(Swaps the contents of the given facet pairings.

This global routine simply calls FacetPairing<dim>::swap(); it is
provided so that FacetPairing<dim> meets the C++ Swappable
requirements.

Parameter ``a``:
    the first facet pairing whose contents should be swapped.

Parameter ``b``:
    the second facet pairing whose contents should be swapped.)doc";

}

namespace detail {

// Docstring regina::python::doc::detail::FacetPairingBase
constexpr const char *FacetPairingBase =
R"doc(Provides core functionality for facet pairings (that is, dual graphs)
of *dim*-dimensional triangulations.

Such a facet pairing is represented by the class FacetPairing<dim>,
which uses this as a base class. End users should not need to refer to
FacetPairingBase directly.

See the FacetPairing class notes for further information.

Both this class and the "end user" class FacetPairing<dim> implement
C++ move semantics, and FacetPairing<dim> also adheres to the C++
Swappable requirement. These classes are designed to avoid deep copies
wherever possible, even when passing or returning objects by value.

Python:
    This base class is not present, but the "end user" class
    FacetPairing<dim> is.

Template parameter ``dim``:
    the dimension of the triangulation. This must be between 2 and 15
    inclusive.)doc";

}

namespace detail::FacetPairingBase_ {

// Docstring regina::python::doc::detail::FacetPairingBase_::__array
constexpr const char *__array =
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

// Docstring regina::python::doc::detail::FacetPairingBase_::__copy
constexpr const char *__copy =
R"doc(Creates a new copy of the given facet pairing.

Parameter ``src``:
    the facet pairing to clone.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::__eq
constexpr const char *__eq =
R"doc(Determines if this and the given facet pairing are identical.

Parameter ``other``:
    the facet pairing to compare with this.

Returns:
    ``True`` if and only if this and the given facet pairing are
    identical.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::__init
constexpr const char *__init =
R"doc(Creates the facet pairing of given triangulation. This is the facet
pairing that describes how the facets of simplices in the given
triangulation are joined together, as described in the class notes.

Precondition:
    The given triangulation is not empty.

Parameter ``tri``:
    the triangulation whose facet pairing should be constructed.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::__init_2
constexpr const char *__init_2 =
R"doc(Reads a new facet pairing from the given input stream. This routine
reads data in the format written by textRep().

This routine will skip any initial whitespace in the given input
stream. Once it finds its first non-whitespace character, it will read
the _entire_ line from the input stream and expect that line to
containin the text representation of a facet pairing.

Exception ``InvalidInput``:
    The data found in the input stream is invalid, incomplete, or
    incorrectly formatted.

Parameter ``in``:
    the input stream from which to read.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::__ne
constexpr const char *__ne =
R"doc(Determines if this and the given facet pairing are not identical.

Parameter ``other``:
    the facet pairing to compare with this.

Returns:
    ``True`` if and only if this and the given facet pairing are not
    identical.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::canonical
constexpr const char *canonical =
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

// Docstring regina::python::doc::detail::FacetPairingBase_::canonicalAll
constexpr const char *canonicalAll =
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

// Docstring regina::python::doc::detail::FacetPairingBase_::dest
constexpr const char *dest =
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

// Docstring regina::python::doc::detail::FacetPairingBase_::dest_2
constexpr const char *dest_2 =
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

// Docstring regina::python::doc::detail::FacetPairingBase_::divideConnected
constexpr const char *divideConnected =
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
    the best possible cut as described above, or no value if no such
    cut exists.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::dot
constexpr const char *dot =
R"doc(Returns a Graphviz DOT representation of the graph that describes this
facet pairing.

This routine simply returns the output of writeDot() as a string,
instead of dumping it to an output stream.

All arguments are the same as for writeDot(); see the writeDot() notes
for further details.

Returns:
    the output of writeDot(), as outlined above.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::dotHeader
constexpr const char *dotHeader =
R"doc(Returns header information for a Graphviz DOT file that will describe
the graphs for one or more facet pairings.

This routine simply returns the output of writeDotHeader() as a
string, instead of dumping it to an output stream.

All arguments are the same as for writeDotHeader(); see the
writeDotHeader() notes for further details.

Returns:
    the output of writeDotHeader(), as outlined above.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::findAllPairings
constexpr const char *findAllPairings =
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
*action* (which must be a function or some other callable object).

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

* *action* must return ``void``.

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
    a function (or other callable object) to call for each facet
    pairing that is found.

Parameter ``args``:
    any additional arguments that should be passed to *action*,
    following the initial facet pairing argument and the optional
    automorphism argument.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::findAutomorphisms
constexpr const char *findAutomorphisms =
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

// Docstring regina::python::doc::detail::FacetPairingBase_::fromTextRep
constexpr const char *fromTextRep =
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

// Docstring regina::python::doc::detail::FacetPairingBase_::isCanonical
constexpr const char *isCanonical =
R"doc(Determines whether this facet pairing is in canonical form.

See the FacetPairing class notes for more information on isomorphisms,
automorphisms and canonical form.

Precondition:
    This facet pairing is connected, i.e., it is possible to reach any
    simplex from any other simplex via a series of matched facet
    pairs.

Returns:
    ``True`` if and only if this facet pairing is in canonical form.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::isClosed
constexpr const char *isClosed =
R"doc(Determines whether this facet pairing is closed. A closed facet
pairing has no unmatched facets.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::isConnected
constexpr const char *isConnected =
R"doc(Determines whether this facet pairing is connected.

A facet pairing is _connected_ if it is possible to reach any simplex
from any other simplex via a series of matched facet pairs.

For this purpose, the empty facet pairing is considered to be
connected.

Returns:
    ``True`` if and only if this pairing is connected.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::isUnmatched
constexpr const char *isUnmatched =
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

// Docstring regina::python::doc::detail::FacetPairingBase_::isUnmatched_2
constexpr const char *isUnmatched_2 =
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

// Docstring regina::python::doc::detail::FacetPairingBase_::size
constexpr const char *size =
R"doc(Returns the number of simplices whose facets are described by this
facet pairing.

Python:
    This is also used to implement the Python special method
    __len__().

Returns:
    the number of simplices under consideration.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::swap
constexpr const char *swap =
R"doc(Swaps the contents of this and the given facet pairing.

Parameter ``other``:
    the facet pairing whose contents are to be swapped with this.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::textRep
constexpr const char *textRep =
R"doc(Returns a text-based representation that can be used to reconstruct
this facet pairing. This reconstruction is done through the routine
fromTextRep().

The text produced is not particularly readable; for a human-readable
text representation, see routine str() instead.

The string returned will contain no newlines.

Returns:
    a text-based representation of this facet pairing.)doc";

// Docstring regina::python::doc::detail::FacetPairingBase_::toTextRep
constexpr const char *toTextRep =
R"doc(Deprecated routine that returns a text-based representation that can
be used to reconstruct this facet pairing.

.. deprecated::
    This routine has been renamed to textRep(). See the textRep()
    documentation for further details.

Returns:
    a text-based representation of this facet pairing.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

