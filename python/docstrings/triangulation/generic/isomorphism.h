/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Isomorphism
constexpr const char *Isomorphism =
R"doc(Represents a combinatorial isomorphism from one *dim*-manifold
triangulation into another.

In essence, a combinatorial isomorphism from triangulation *T* to
triangulation *U* is a one-to-one map from the simplices of *T* to the
simplices of *U* that allows relabelling of both the simplices and
their facets (or equivalently, their vertices), and that preserves
gluings across adjacent simplices.

More precisely: An isomorphism consists of (i) a one-to-one map *f*
from the simplices of *T* to the simplices of *U*, and (ii) for each
simplex *S* of *T*, a permutation *f*_{S} of the facets (0,...,*dim*)
of *S*, for which the following condition holds:

* If facet *k* of simplex *S* and facet *k*' of simplex *S*' are
  identified in *T*, then facet *f*_{S}(*k*) of *f*(S) and facet
  *f*_{S'}(*k*') of *f*(S') are identified in *U*. Moreover, their
  gluing is consistent with the facet/vertex permutations; that is,
  there is a commutative square involving the gluing maps in *T* and
  *U* and the permutations *f*_{S} and *f*_{S'}.

Isomorphisms can be _boundary complete_ or _boundary incomplete_. A
boundary complete isomorphism satisfies the additional condition:

* If facet *x* is a boundary facet of *T* then facet *f*(x) is a
  boundary facet of *U*.

A boundary complete isomorphism thus indicates that a copy of
triangulation *T* is present as an entire component (or components) of
*U*, whereas a boundary incomplete isomorphism represents an embedding
of a copy of triangulation *T* as a subcomplex of some possibly larger
component (or components) of *U*.

Note that for all types of isomorphism, triangulation *U* is allowed
to contain more simplices than triangulation *T*.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

Python:
    Python does not support templates. Instead this class can be used
    by appending the dimension as a suffix (e.g., Isomorphism2 and
    Isomorphism3 for dimensions 2 and 3).

Template parameter ``dim``:
    the dimension of the triangulations that this isomorphism class
    works with. This must be between 2 and 15 inclusive.)doc";

namespace Isomorphism_ {

// Docstring regina::python::doc::Isomorphism_::__array
constexpr const char *__array =
R"doc(Determines the image of the given source simplex facet under this
isomorphism. This operator returns by value: it cannot be used to
alter the isomorphism.

Parameter ``source``:
    the given source simplex facet; this must be one of the (*dim* +
    1) facets of one of the size() simplices in the source
    triangulation.

Returns:
    the image of the source simplex facet under this isomorphism.)doc";

// Docstring regina::python::doc::Isomorphism_::__call
constexpr const char *__call =
R"doc(Applies this isomorphism to the given triangulation, and returns the
result as a new triangulation.

An isomorphism represents a combinatorial map from a triangulation *T*
to a triangulation *U*. This routine treats the given triangulation as
the domain *T*, and returns the corresponding range *U*. The given
triangulation *T* is not modified in any way.

In more detail: A new triangulation *U* is returned, so that this
isomorphism represents a one-to-one, onto and boundary complete
isomorphism from *T* to *U*. That is, *T* and *U* will be
combinatorially isomorphic triangulations, and this isomorphism
describes the mapping from the simplices of *T* and their facets to
the simplices of *U* and their facets.

Precondition:
    The simplex images are precisely 0,1,...,size()-1 in some order
    (i.e., this isomorphism does not represent a mapping from a
    smaller triangulation into a larger triangulation).

Exception ``InvalidArgument``:
    The number of top-dimensional simplices in the given triangulation
    is not equal to size() for this isomorphism.

Parameter ``tri``:
    the triangulation to which this isomorphism should be applied.

Returns:
    the new isomorphic triangulation.)doc";

// Docstring regina::python::doc::Isomorphism_::__call_2
constexpr const char *__call_2 =
R"doc(Returns the image of the given facet-of-simplex under this
isomorphism.

Specifically:

* If ``f.simp`` is in the range 0,1,...,size()-1 inclusive (i.e., *f*
  denotes a facet of an actual top-dimensional simplex), then this
  routine will return an object denoting facet ``facetPerm(f.facet)``
  of simplex ``simpImage(f.simp)``.

* If ``f.simp`` is negative (i.e., *f* takes a before-the-start
  value), or if ``f.simp`` is at least size() (i.e., *f* takes a
  boundary or past-the-end value), then this routine will return *f*
  unchanged (but see the precondition below).

Precondition:
    If this isomorphism maps a smaller triangulation into a larger
    triangulation (in particular, if the simplex images under this
    isomorphism are not just some reordering of 0,1,...,size()-1),
    then *f* must not denote a boundary or past-the-end value. This is
    because a boundary or past-the-end value is encoded by using a
    past-the-end value of ``FacetSpec::simp``. If this isomorphism
    maps into a larger triangulation then this past-the-end simplex
    number would need to change, but the isomorphism does not actually
    know what the new value of ``FacetSpec::simp`` should be.

Parameter ``f``:
    the facet-of-simplex which should be transformed by this
    isomorphism.

Returns:
    the image of *f* under this isomorphism.)doc";

// Docstring regina::python::doc::Isomorphism_::__call_3
constexpr const char *__call_3 =
R"doc(Applies this isomorphism to the given facet pairing, and returns the
result as a new facet pairing.

Although the Isomorphism class was designed to represent mappings
between isomorphic triangulations, it can just as well describe
mappings between isomorphic facet pairings. In particular, if *iso*
represents this isomorphism and if *p* were the facet pairing of some
triangulation *tri*, then ``iso(p)`` would be the facet pairing for
the triangulation ``iso(tri)``. Of course, this routine works directly
with the facet pairing, and does not actually construct any
triangulations at all.

This routine behaves correctly even if some facets of *p* are
unmatched (i.e., if *p* models a triangulation with boundary facets).

Precondition:
    The simplex images are precisely 0,1,...,size()-1 in some order
    (i.e., this isomorphism does not represent a mapping from a
    smaller triangulation into a larger triangulation).

Exception ``InvalidArgument``:
    The number of top-dimensional simplices described by the given
    facet pairing is not equal to size() for this isomorphism.

Parameter ``p``:
    the facet pairing to which this isomorphism should be applied.

Returns:
    the new isomorphic facet pairing.)doc";

// Docstring regina::python::doc::Isomorphism_::__copy
constexpr const char *__copy =
R"doc(Creates a copy of the given isomorphism.

Parameter ``src``:
    the isomorphism to copy.)doc";

// Docstring regina::python::doc::Isomorphism_::__eq
constexpr const char *__eq =
R"doc(Determines whether this and the given isomorphism are identical.

Two isomorphisms are considered _identical_ if they act on the same
number of top-dimensional simplices, and all destination simplex
numbers and facet permutations are the same for both isomorphisms.

In particular it is only the simplex, facet and vertex _labels_ that
matter: an isomorphism does not refer to a specific triangulation, and
there is no sense in which the two isomorphisms need to act on the
same triangulations and/or point to the same destination Simplex
objects.

It is safe to compare isomorphisms of different sizes (in which case
this routine will return ``False``).

Parameter ``other``:
    the isomorphism to compare with this.

Returns:
    ``True`` if and only if this and the given isomorphism are
    identical.)doc";

// Docstring regina::python::doc::Isomorphism_::__inc
constexpr const char *__inc =
R"doc(A preincrement operator that changes this to be the next isomorphism
in an iteration through all possible isomorphisms of this size.

The order of iteration is lexicographical, by the sequence of simplex
images and then by the sequence of facet permutations. Facet
permutations, in turn, are ordered by their indices in the array
Perm<dim>::Sn.

In particular, the identity isomorphism is the first in such an
iteration. If this isomorphism is the _last_ in such an iteration,
then this operator will "wrap around" and set this to the identity.

Precondition:
    The class Perm<dim+1> supports the preincrement operator;
    currently this means that *dim* must be at most 6.

Python:
    This routine is named inc() since Python does not support the
    increment operator. Unlike other Regina classes, here inc() wraps
    the preincrement operator (not the postincrement operator), since
    the postincrement operator is significantly more expensive. To
    avoid confusion, the python inc() function returns ``None`` (not
    this isomorphism).

Returns:
    a reference to this isomorphism after the increment.)doc";

// Docstring regina::python::doc::Isomorphism_::__init
constexpr const char *__init =
R"doc(Creates a new isomorphism with no initialisation. The images of the
simplices and their vertices must be explicitly set using simpImage()
and facetPerm().

Python:
    For Python users, the images of the simplices and their vertices
    must be set using setSimpImage() and setFacetPerm() instead.

Parameter ``nSimplices``:
    the number of simplices in the source triangulation associated
    with this isomorphism. This is allowed to be zero.)doc";

// Docstring regina::python::doc::Isomorphism_::__mul
constexpr const char *__mul =
R"doc(Returns the composition of this isomorphism with the given
isomorphism.

This follows the same order convention as Regina's permutation
classes: the composition ``a * b`` first applies the right-hand
isomorphism *b*, and then the left-hand isomorphism *a*.

Precondition:
    The source triangulation for this isomorphism (the left-hand side)
    is at least as large as the destination triangulation for *rhs*
    (the right-hand side). In other words, the maximum value of
    ``rhs.simpImage(i)`` over all *i* must be less than
    ``this->size()``.

Returns:
    the composition of both isomorphisms.)doc";

// Docstring regina::python::doc::Isomorphism_::__mul_2
constexpr const char *__mul_2 =
R"doc(Returns the composition of this isomorphism with the given
isomorphism.

This follows the same order convention as Regina's permutation
classes: the composition ``a * b`` first applies the right-hand
isomorphism *b*, and then the left-hand isomorphism *a*.

Precondition:
    The source triangulation for this isomorphism (the left-hand side)
    is at least as large as the destination triangulation for *rhs*
    (the right-hand side). In other words, the maximum value of
    ``rhs.simpImage(i)`` over all *i* must be less than
    ``this->size()``.

Returns:
    the composition of both isomorphisms.)doc";

// Docstring regina::python::doc::Isomorphism_::__ne
constexpr const char *__ne =
R"doc(Determines whether this and the given isomorphism are not identical.

Two isomorphisms are considered _identical_ if they act on the same
number of top-dimensional simplices, and all destination simplex
numbers and facet permutations are the same for both isomorphisms.

In particular it is only the simplex, facet and vertex _labels_ that
matter: an isomorphism does not refer to a specific triangulation, and
there is no sense in which the two isomorphisms need to act on the
same triangulations and/or point to the same destination Simplex
objects.

It is safe to compare isomorphisms of different sizes (in which case
this routine will return ``True``).

Parameter ``other``:
    the isomorphism to compare with this.

Returns:
    ``True`` if and only if this and the given isomorphism are not
    identical.)doc";

// Docstring regina::python::doc::Isomorphism_::apply
constexpr const char *apply =
R"doc(Deprecated routine that applies this isomorphism to the given
triangulation, and returns the result as a new triangulation.

.. deprecated::
    If this isomorphism is *iso*, then this routine is equivalent to
    calling ``iso(tri)``. See the bracket operator for further
    details.

Precondition:
    The simplex images are precisely 0,1,...,size()-1 in some order
    (i.e., this isomorphism does not represent a mapping from a
    smaller triangulation into a larger triangulation).

Exception ``InvalidArgument``:
    The number of top-dimensional simplices in the given triangulation
    is not equal to size() for this isomorphism.

Parameter ``tri``:
    the triangulation to which this isomorphism should be applied.

Returns:
    the new isomorphic triangulation.)doc";

// Docstring regina::python::doc::Isomorphism_::applyInPlace
constexpr const char *applyInPlace =
R"doc(Deprecated routine that applies this isomorphism to the given
triangulation, modifying the given triangulation directly.

.. deprecated::
    If this isomorphism is *iso*, then this routine is equivalent to
    calling ``tri = iso(tri)``. See the bracket operator for further
    details.

Precondition:
    The simplex images are precisely 0,1,...,size()-1 in some order
    (i.e., this isomorphism does not represent a mapping from a
    smaller triangulation into a larger triangulation).

Exception ``InvalidArgument``:
    The number of top-dimensional simplices in the given triangulation
    is not equal to size() for this isomorphism.

Parameter ``tri``:
    the triangulation to which this isomorphism should be applied.)doc";

// Docstring regina::python::doc::Isomorphism_::facetPerm
constexpr const char *facetPerm =
R"doc(Determines the permutation that is applied to the (*dim* + 1) facets
of the given source simplex under this isomorphism. Facet *i* of
source simplex *sourceSimp* will be mapped to face
``facetPerm(sourceSimp)[i]`` of simplex ``simpImage(sourceSimp)``.

If the dimension *dim* is 2 or 3, then you can also access this
permutation through the dimension-specific alias edgePerm() or
facePerm() respectively.

Parameter ``sourceSimp``:
    the index of the source simplex containing the original (*dim* +
    1) facets; this must be between 0 and ``size()-1`` inclusive.

Returns:
    the permutation applied to the facets of the source simplex.)doc";

// Docstring regina::python::doc::Isomorphism_::global_swap
constexpr const char *global_swap =
R"doc(Swaps the contents of the given isomorphisms.

This global routine simply calls Isomorphism<dim>::swap(); it is
provided so that Isomorphism<dim> meets the C++ Swappable
requirements.

Parameter ``a``:
    the first isomorphism whose contents should be swapped.

Parameter ``b``:
    the second isomorphism whose contents should be swapped.)doc";

// Docstring regina::python::doc::Isomorphism_::identity
constexpr const char *identity =
R"doc(Returns the identity isomorphism for the given number of simplices.
This isomorphism sends every simplex and every vertex to itself.

Parameter ``nSimplices``:
    the number of simplices that the new isomorphism should operate
    upon.

Returns:
    the identity isomorphism.)doc";

// Docstring regina::python::doc::Isomorphism_::inverse
constexpr const char *inverse =
R"doc(Returns the inverse of this isomorphism.

Precondition:
    The destination triangulation has precisely the same number of
    simplices as the source triangulation. In other words, there are
    no "gaps" in the simplex images: the values ``simpImage(0)``, ...,
    ``simpImage(size()-1)`` must be a permutation of 0, ...,
    ``size()-1``.

Returns:
    the inverse isomorphism.)doc";

// Docstring regina::python::doc::Isomorphism_::isIdentity
constexpr const char *isIdentity =
R"doc(Determines whether or not this is an identity isomorphism.

In an identity isomorphism, each simplex image is itself, and within
each simplex the facet/vertex permutation is the identity permutation.

Returns:
    ``True`` if this is an identity isomorphism, or ``False``
    otherwise.)doc";

// Docstring regina::python::doc::Isomorphism_::random
constexpr const char *random =
R"doc(Returns a random isomorphism for the given number of simplices. This
isomorphism will reorder simplices 0 to ``nSimplices-1`` in a random
fashion, and for each simplex a random permutation of its (*dim* + 1)
vertices will be selected.

All possible isomorphisms for the given number of simplices are
equally likely.

This routine is thread-safe, and uses RandomEngine for its random
number generation.

Parameter ``nSimplices``:
    the number of simplices that the new isomorphism should operate
    upon.

Parameter ``even``:
    if ``True``, then every simplex will have its vertices permuted
    with an even permutation. This means that, if the random
    isomorphism is applied to an oriented triangulation, it will
    preserve the orientation.

Returns:
    the new random isomorphism.)doc";

// Docstring regina::python::doc::Isomorphism_::setFacetPerm
constexpr const char *setFacetPerm =
R"doc(Python-only routine that sets the permutation that is applied to the
(*dim* + 1) facets of the given source simplex under this isomorphism.
Facet *i* of source simplex *sourceSimp* will be mapped to facet
``perm[i]`` of simplex ``simpImage(sourceSimp)``.

If the dimension *dim* is 2 or 3, then you can also set this
permutation through the dimension-specific alias setEdgePerm() or
setFacePerm() respectively.

Parameter ``sourceSimp``:
    the index of the source simplex containing the original (*dim* +
    1) facets; this must be between 0 and ``size()-1`` inclusive.

Parameter ``perm``:
    the new permutation that should be applied to the facets of the
    source simplex.)doc";

// Docstring regina::python::doc::Isomorphism_::setSimpImage
constexpr const char *setSimpImage =
R"doc(Python-only routine that sets the image of the given source simplex to
the given value under this isomorphism.

If the dimension *dim* is 2, 3 or 4, then you can also set this image
through the dimension-specific alias setTriImage(), setTetImage() or
setPentImage() respectively.

Simplex images are stored using type ``ssize_t``, not ``size_t``, and
so you can safely use the special value -1 as a marker for an image
that is unknown or not yet initialised.

Parameter ``sourceSimp``:
    the index of the source simplex; this must be between 0 and
    ``size()-1`` inclusive.

Parameter ``image``:
    the index of the new destination simplex that the source simplex
    should map to.)doc";

// Docstring regina::python::doc::Isomorphism_::simpImage
constexpr const char *simpImage =
R"doc(Determines the image of the given source simplex under this
isomorphism.

If the dimension *dim* is 2, 3 or 4, then you can also access this
image through the dimension-specific alias triImage(), tetImage() or
pentImage() respectively.

Parameter ``sourceSimp``:
    the index of the source simplex; this must be between 0 and
    ``size()-1`` inclusive.

Returns:
    the index of the destination simplex that the source simplex maps
    to.)doc";

// Docstring regina::python::doc::Isomorphism_::size
constexpr const char *size =
R"doc(Returns the number of simplices in the source triangulation associated
with this isomorphism. Note that this is always less than or equal to
the number of simplices in the destination triangulation.

Python:
    This is also used to implement the Python special method
    __len__().

Returns:
    the number of simplices in the source triangulation.)doc";

// Docstring regina::python::doc::Isomorphism_::swap
constexpr const char *swap =
R"doc(Swaps the contents of this and the given isomorphism.

It does not matter if this and the given isomorphism use different
numbers of simplices; if so then they will be adjusted accordingly.

Parameter ``other``:
    the isomorphism whose contents are to be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

