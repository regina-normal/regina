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

Facet pairings are *labelled*, in that the simplices are explicitly
numbered 0,1,..., and the facets of each simplex are explicitly
numbered 0,...,*dim* (just like in a triangulation). Facet pairings do
also come with code to help identify and work with relabellings, via
isomorphisms, automorphisms, and canonical representations. In this
context:

* An *isomorphism* of a facet pairing means a relabelling of the
  simplices and a relabelling of the (*dim* + 1) facets within each
  simplex; this can be represented by the same class Isomorphism<dim>
  that is used for isomorphisms of triangulations.

* An *automorphism* of a facet pairing is an isomorphism that, when
  applied, results in an identical facet pairing (i.e., where exactly
  the same pairs of labelled simplex facets are matched together).

* A facet pairing is in *canonical form* if it is a lexicographically
  minimal representative of its isomorphism class. Here we order facet
  pairings by lexicographical comparison of the sequence
  ``dest(0,0)``, ``dest(0,1)``, ..., ``dest(size()-1,*dim*)`` (which
  in turn uses the ordering defined by FacetSpec<dim>, where each
  simplex facet is ordered first by simplex number and then by facet
  number, and where the boundary is ordered last).

For dimension 3, this FacetPairing class template is specialised and
offers more functionality. In order to use this specialised class, you
will need to include the corresponding header
triangulation/facetpairing3.h.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g., FacetPairing2 and
    FacetPairing3 for dimensions 2 and 3).

Template parameter ``dim``:
    the dimension of the underlying triangulation. This must be
    between 2 and 15 inclusive.

\headerfile triangulation/generic.h)doc";

namespace FacetPairing_ {

// Docstring regina::python::doc::FacetPairing_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given facet pairing.

Parameter ``src``:
    the facet pairing to clone.)doc";

// Docstring regina::python::doc::FacetPairing_::__init
static const char *__init =
R"doc(Creates the dual graph of the given triangulation. This is the facet
pairing that describes how the facets of simplices in the given
triangulation are joined together, as described in the class notes.

Calling ``FacetPairing<dim>(tri)`` is equivalent to calling
``tri.pairing()``.

Precondition:
    The given triangulation is not empty.

Parameter ``tri``:
    the triangulation whose facet pairing should be constructed.)doc";

// Docstring regina::python::doc::FacetPairing_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given facet pairings.

This global routine simply calls FacetPairing<dim>::swap(); it is
provided so that FacetPairing<dim> meets the C++ Swappable
requirements.

Parameter ``a``:
    the first facet pairing whose contents should be swapped.

Parameter ``b``:
    the second facet pairing whose contents should be swapped.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

