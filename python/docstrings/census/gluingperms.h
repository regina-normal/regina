/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::GluingPerms
constexpr const char *GluingPerms =
R"doc(Represents a specific set of gluing permutations to complement a
particular pairwise matching of simplex facets. Given a pairwise
matching of facets (as described by class FacetPairing<dim>), each
facet that is matched with some other facet will have an associated
permutation of (*dim* + 1) elements.

If a facet is matched with some other facet, the two associated
permutations in this set will be inverses. If a facet is left
deliberately unmatched, it will have no associated permutation in this
set.

Such a set of permutations models part of the structure of a
triangulation, in which each simplex facet that is glued to another
facet has a corresponding gluing permutation (and the matched facet
has the inverse gluing permutation).

The main purpose of this class is to provide a way of quickly
describing and manipulating the combinatorial structure of a
triangulation without the overhead of a full-blown Triangulation<dim>
object. In particular, this class was designed for use in census
enumeration, which involves rapid construction and editing of these
permutations. To convert this into a full Triangulation<dim> object,
you can call triangulate().

Internally, this class stores each permutation as an index into
Perm<dim+1>::Sn_1 (that is, an index into the permutation group
*S_dim*). Importantly, you can only *set* permutations using these
indices, via the non-const permIndex() function - access to the
permutations themselves is read-only. You can use indexToGluing() and
gluingToIndex() to convert between indicex and permutations, or you
can use the read-only member function perm() which returns the
permutation directly. Note that this correspondence is non-trivial,
and depends on the details of the underlying facet pairing.

This class also supports a special "not yet chosen" permutation,
represented using the permutation index -1. Attempting to convert this
to a real permutation using perm() or indexToGluing() is not allowed,
and will result in undefined behaviour.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

Template parameter ``dim``:
    the dimension of the underlying triangulation that is being
    modelled. This must be between 2 and 15 inclusive.)doc";

namespace GluingPerms_ {

// Docstring regina::python::doc::GluingPerms_::__eq
constexpr const char *__eq =
R"doc(Determines if this and the given gluing permutation set are identical.

To be identical, the two sets must use identical facet pairings and
all of their corresponding permutations must be the same.

Parameter ``other``:
    the gluing permutation set to compare with this.

Returns:
    ``True`` if and only if this and the given set are identical.)doc";

// Docstring regina::python::doc::GluingPerms_::__init
constexpr const char *__init =
R"doc(Creates a new permutation set. All internal permutations will be
marked as not yet chosen, and any attempt to query them before setting
them to explicit values will result in undefined behaviour. The
corresponding permutation indices will be set to -1.

This constructor induces a deep copy of the given facet pairing.

Precondition:
    The given facet pairing is connected, i.e., it is possible to
    reach any simplex from any other simplex via a series of matched
    facet pairs.

Precondition:
    The given facet pairing is in canonical form as described by
    FacetPairing::isCanonical(). Note that all facet pairings
    constructed by FacetPairing::findAllPairings() are of this form.

Parameter ``pairing``:
    the specific pairing of simplex facets that this permutation set
    will complement.)doc";

// Docstring regina::python::doc::GluingPerms_::__init_2
constexpr const char *__init_2 =
R"doc(Creates a new permutation set. All internal permutations will be
marked as not yet chosen, and any attempt to query them before setting
them to explicit values will result in undefined behaviour. The
corresponding permutation indices will be set to -1.

This constructor moves the contents of the given facet pairing into
this object. After it returns, *pairing* will no longer be usable.

Precondition:
    The given facet pairing is connected, i.e., it is possible to
    reach any simplex from any other simplex via a series of matched
    facet pairs.

Precondition:
    The given facet pairing is in canonical form as described by
    FacetPairing::isCanonical(). Note that all facet pairings
    constructed by FacetPairing::findAllPairings() are of this form.

Parameter ``pairing``:
    the specific pairing of simplex facets that this permutation set
    will complement.)doc";

// Docstring regina::python::doc::GluingPerms_::__init_3
constexpr const char *__init_3 =
R"doc(Creates a new copy of the given gluing permutation set.

Parameter ``src``:
    the gluing permutations to copy.)doc";

// Docstring regina::python::doc::GluingPerms_::__ne
constexpr const char *__ne =
R"doc(Determines if this and the given gluing permutation set are not
identical.

To be identical, the two sets must use identical facet pairings and
all of their corresponding permutations must be the same.

Parameter ``other``:
    the gluing permutation set to compare with this.

Returns:
    ``True`` if and only if this and the given set are not identical.)doc";

// Docstring regina::python::doc::GluingPerms_::data
constexpr const char *data =
R"doc(Returns all internal data in a plain text format. This gluing
permutation sert can be recreated from this text data by calling
fromData().

This routine may be useful for transferring objects from one processor
to another.

This routine returns the same information that dumpData() writes.

.. warning::
    The data format is liable to change between Regina releases. Data
    in this format should be used on a short-term temporary basis
    only.

Returns:
    all of this object's internal data in plain text format.)doc";

// Docstring regina::python::doc::GluingPerms_::fromData
constexpr const char *fromData =
R"doc(Reads a new set of gluing permutations from the given string. This
routine reads data in the format written by data().

.. warning::
    The data format is liable to change between Regina releases. Data
    in this format should be used on a short-term temporary basis
    only.

Exception ``InvalidArgument``:
    The data found in the given string is invalid, incomplete, or
    incorrectly formatted.

Parameter ``data``:
    the data from which to reconstruct a gluing permutation set.

Returns:
    the reconstructed gluing permutation set.)doc";

// Docstring regina::python::doc::GluingPerms_::global_swap
constexpr const char *global_swap =
R"doc(Swaps the contents of the given gluing permutation sets.

This global routine simply calls GluingPerms<dim>::swap(); it is
provided so that GluingPerms<dim> meets the C++ Swappable
requirements.

Parameter ``a``:
    the first matrix whose contents should be swapped.

Parameter ``b``:
    the second matrix whose contents should be swapped.)doc";

// Docstring regina::python::doc::GluingPerms_::gluingToIndex
constexpr const char *gluingToIndex =
R"doc(Returns the index into array Perm<dim+1>::Sn_1 corresponding to the
given gluing permutation from the given facet to its partner. This
need not be the index into Perm<dim+1>::Sn_1 that is currently stored
for the given facet.

Indices into array Perm<dim+1>::Sn_1 are stored internally in the
array *permIndices_*. Full gluing permutations on the other hand are
used in constructing triangulations.

Precondition:
    The given simplex facet has a partner according to the underlying
    facet pairing, i.e., is not a boundary facet.

Precondition:
    If the given simplex facet and its partner are facets *x* and *y*
    of their respective simplices, then the given gluing permutation
    maps *x* to *y*.

Parameter ``source``:
    the simplex facet under investigation.

Parameter ``gluing``:
    a possible gluing permutation from the given simplex facet to its
    partner according to the underlying facet pairing.

Returns:
    the index into Perm<dim+1>::Sn_1 corresponding to the given gluing
    permutation; this will be between 0 and *dim*!-1 inclusive.)doc";

// Docstring regina::python::doc::GluingPerms_::gluingToIndex_2
constexpr const char *gluingToIndex_2 =
R"doc(Returns the index into array Perm<dim+1>::Sn_1 corresponding to the
given gluing permutation from the given facet to its partner. This
need not be the index into Perm<dim+1>::Sn_1 that is currently stored
for the given facet.

Indices into array Perm<dim+1>::Sn_1 are stored internally in the
array *permIndices_*. Full gluing permutations on the other hand are
used in constructing triangulations.

Precondition:
    The given simplex facet has a partner according to the underlying
    facet pairing, i.e., is not a boundary facet.

Precondition:
    If the given simplex facet and its partner are facets *x* and *y*
    of their respective simplices, then the given gluing permutation
    maps *x* to *y*.

Parameter ``simp``:
    the simplex under investigation; this must be strictly less than
    the total number of simplices under consideration.

Parameter ``facet``:
    the facet of the given simplex under investigation; this must be
    between 0 and *dim* inclusive.

Parameter ``gluing``:
    a possible gluing permutation from the given simplex facet to its
    partner according to the underlying facet pairing.

Returns:
    the index into Perm<dim+1>::Sn_1 corresponding to the given gluing
    permutation; this will be between 0 and *dim*!-1 inclusive.)doc";

// Docstring regina::python::doc::GluingPerms_::indexToGluing
constexpr const char *indexToGluing =
R"doc(Returns the gluing permutation from the given facet to its partner
that corresponds to the given index into array Perm<dim+1>::Sn_1. This
index into Perm<dim+1>::Sn_1 need not be the index that is currently
stored for the given facet.

Indices into array Perm<dim+1>::Sn_1 are stored internally in the
array *permIndices_*. Full gluing permutations on the other hand are
used in constructing triangulations.

If the given simplex facet and its partner according to the underlying
facet pairing are facets *x* and *y* of their respective simplices,
then the resulting gluing permutation will map *x* to *y*.

Precondition:
    The given simplex facet has a partner according to the underlying
    facet pairing, i.e., is not a boundary facet.

Parameter ``source``:
    the simplex facet under investigation.

Parameter ``index``:
    an index into Perm<dim+1>::Sn_1; this must be between 0 and
    *dim*!-1 inclusive.

Returns:
    the gluing permutation corresponding to the given index into
    Perm<dim+1>::Sn_1.)doc";

// Docstring regina::python::doc::GluingPerms_::indexToGluing_2
constexpr const char *indexToGluing_2 =
R"doc(Returns the gluing permutation from the given facet to its partner
that corresponds to the given index into array Perm<dim+1>::Sn_1. This
index into Perm<dim+1>::Sn_1 need not be the index that is currently
stored for the given facet.

Indices into array Perm<dim+1>::Sn_1 are stored internally in the
array *permIndices_*. Full gluing permutations on the other hand are
used in constructing triangulations.

If the given simplex facet and its partner according to the underlying
facet pairing are facets *x* and *y* of their respective simplices,
then the resulting gluing permutation will map *x* to *y*.

Precondition:
    The given simplex facet has a partner according to the underlying
    facet pairing, i.e., is not a boundary facet.

Parameter ``simp``:
    the simplex under investigation; this must be strictly less than
    the total number of simplices under consideration.

Parameter ``facet``:
    the facet of the given simplex under investigation; this must be
    between 0 and *dim* inclusive.

Parameter ``index``:
    an index into Perm<dim+1>::Sn_1; this must be between 0 and
    *dim*!-1 inclusive.

Returns:
    the gluing permutation corresponding to the given index into
    Perm<dim+1>::Sn_1.)doc";

// Docstring regina::python::doc::GluingPerms_::pairing
constexpr const char *pairing =
R"doc(Returns the specific pairing of simplex facets that this set of gluing
permutations complements.

Returns:
    the corresponding simplex facet pairing.)doc";

// Docstring regina::python::doc::GluingPerms_::perm
constexpr const char *perm =
R"doc(Returns the gluing permutation associated with the given simplex
facet.

Precondition:
    The given facet is actually paired with some other facet in the
    underlying pairwise matching (see routine pairing()).

Precondition:
    The given facet is a real simplex facet (not boundary, before-the-
    start or past-the-end).

Precondition:
    The requested permutation is not the special "not yet chosen"
    value (i.e., the internal index that is stored for it is not -1).

Parameter ``source``:
    the simplex facet under investigation.

Returns:
    the associated gluing permutation.)doc";

// Docstring regina::python::doc::GluingPerms_::perm_2
constexpr const char *perm_2 =
R"doc(Returns the gluing permutation associated with the given simplex
facet.

Precondition:
    The given facet is actually paired with some other facet in the
    underlying pairwise matching (see routine pairing()).

Precondition:
    The requested permutation is not the special "not yet chosen"
    value (i.e., the internal index that is stored for it is not -1).

Parameter ``simp``:
    the simplex under investigation (this must be strictly less than
    the total number of simplices under consideration).

Parameter ``facet``:
    the facet of the given simplex under investigation (between 0 and
    *dim* inclusive).

Returns:
    the associated gluing permutation.)doc";

// Docstring regina::python::doc::GluingPerms_::permIndex
constexpr const char *permIndex =
R"doc(Returns the index into array Perm<dim+1>::Sn_1 describing how the the
given facet is joined to its partner.

Note that this is not the *S_n* index of the gluing permutation on
(*dim* + 1) elements, but rather the index of a permutation on just
*dim* elements. You can use indexToGluing() and gluingToIndex() to
convert between these indices and gluing permutations, or you can call
perm() to read the gluing permutation directly.

As described in the class notes, this index could be a real
permutation index between 0 and (dim!)-1 inclusive, or it could be the
special value -1 indicating that the permutation has not yet been
chosen.

Precondition:
    The given facet is a real simplex facet (not boundary, before-the-
    start or past-the-end).

Parameter ``source``:
    the simplex facet under investigation.

Returns:
    a reference to the corresponding array index.)doc";

// Docstring regina::python::doc::GluingPerms_::permIndex_2
constexpr const char *permIndex_2 =
R"doc(Returns the index into array Perm<dim+1>::Sn_1 describing how the the
given facet is joined to its partner.

Note that this is not the *S_n* index of the gluing permutation on
(*dim* + 1) elements, but rather the index of a permutation on just
*dim* elements. You can use indexToGluing() and gluingToIndex() to
convert between these indices and gluing permutations, or you can call
perm() to read the gluing permutation directly.

As described in the class notes, this index could be a real
permutation index between 0 and (dim!)-1 inclusive, or it could be the
special value -1 indicating that the permutation has not yet been
chosen.

Parameter ``simp``:
    the simplex under investigation (this must be strictly less than
    the total number of simplices under consideration).

Parameter ``facet``:
    the facet of the given simplex under investigation (between 0 and
    *dim* inclusive).

Returns:
    a reference to the corresponding array index.)doc";

// Docstring regina::python::doc::GluingPerms_::permIndex_3
constexpr const char *permIndex_3 =
R"doc(Offers write access to the index into array Perm<dim+1>::Sn_1
describing how the the given facet is joined to its partner.

Note that this is not the *S_n* index of the gluing permutation on
(*dim* + 1) elements, but rather the index of a permutation on just
*dim* elements. You can use indexToGluing() and gluingToIndex() to
convert between these indices and gluing permutations, or you can call
perm() for read-only access to the gluing permutation directly.

As described in the class notes, this index can be a real permutation
index between 0 and (dim!)-1 inclusive, or it may be the special value
-1 indicating that the permutation has not yet been chosen.

Precondition:
    The given facet is a real simplex facet (not boundary, before-the-
    start or past-the-end).

Python:
    Python users can only access the read-only version of this
    function that returns by value: you cannot use permIndex() to edit
    the gluing permutations. As an alternative however, you can call
    ``setPermIndex(source, index)`` instead.

Parameter ``source``:
    the simplex facet under investigation.

Returns:
    a reference to the corresponding array index.)doc";

// Docstring regina::python::doc::GluingPerms_::permIndex_4
constexpr const char *permIndex_4 =
R"doc(Offers write access to the index into array Perm<dim+1>::Sn_1
describing how the the given facet is joined to its partner.

Note that this is not the *S_n* index of the gluing permutation on
(*dim* + 1) elements, but rather the index of a permutation on just
*dim* elements. You can use indexToGluing() and gluingToIndex() to
convert between these indices and gluing permutations, or you can call
perm() for read-only access to the gluing permutation directly.

As described in the class notes, this index can be a real permutation
index between 0 and (dim!)-1 inclusive, or it may be the special value
-1 indicating that the permutation has not yet been chosen.

Python:
    Python users can only access the read-only version of this
    function that returns by value: you cannot use permIndex() to edit
    the gluing permutations. As an alternative however, you can call
    ``setPermIndex(simp, facet, index)`` instead.

Parameter ``simp``:
    the simplex under investigation (this must be strictly less than
    the total number of simplices under consideration).

Parameter ``facet``:
    the facet of the given simplex under investigation (between 0 and
    *dim* inclusive).

Returns:
    a reference to the corresponding array index.)doc";

// Docstring regina::python::doc::GluingPerms_::size
constexpr const char *size =
R"doc(Returns the total number of simplices under consideration.

Returns:
    the number of simplices under consideration.)doc";

// Docstring regina::python::doc::GluingPerms_::swap
constexpr const char *swap =
R"doc(Swaps the contents of this and the given gluing permutation set.

Parameter ``other``:
    the set whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::GluingPerms_::triangulate
constexpr const char *triangulate =
R"doc(Returns the triangulation modelled by this set of gluing permutations
and the associated simplex facet pairing.

Each matched pair of facets and their associated permutations will be
realised as two simplex facets in the triangulation glued together
with the corresponding gluing permutation. Each unmatched facet will
be realised as a boundary facet in the triangulation.

Precondition:
    For every simplex facet in the underlying facet pairing that is
    glued to a partner, the corresponding gluing permutation in this
    set is not the special "not yet chosen" value (i.e., does not have
    internal index -1).

Returns:
    the triangulation modelled by this structure.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

