/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


namespace Perm_ {

// Docstring regina::python::doc::Perm_::Perm
static const char *Perm = R"doc(Creates the identity permutation.)doc";

// Docstring regina::python::doc::Perm_::Perm_2
static const char *Perm_2 =
R"doc(Creates the transposition of *a* and *b*. Note that *a* and *b* need
not be distinct.

Precondition:
    *a* and *b* are in {0,1,2}.

Parameter ``a``:
    the element to switch with *b*.

Parameter ``b``:
    the element to switch with *a*.)doc";

// Docstring regina::python::doc::Perm_::Perm_3
static const char *Perm_3 =
R"doc(Creates a permutation mapping (0,1,2) to (*a*,*b*,*c*) respectively.

Precondition:
    {*a*,*b*,*c*} = {0,1,2}.

Parameter ``a``:
    the desired image of 0.

Parameter ``b``:
    the desired image of 1.

Parameter ``c``:
    the desired image of 2.)doc";

// Docstring regina::python::doc::Perm_::Perm_4
static const char *Perm_4 =
R"doc(Creates a permutation mapping *i* to *image*[i] for each *i* = 0,1,2.

Precondition:
    The elements of *image* are 0, 1 and 2 in some order.

Parameter ``image``:
    the array of images.)doc";

// Docstring regina::python::doc::Perm_::Perm_5
static const char *Perm_5 =
R"doc(Creates a permutation mapping (*a0*,*b0*,*c0*) to (*a1*,*b1*,*c1*)
respectively.

Precondition:
    {*a0*,*b0*,*c0*} = {*a1*,*b1*,*c1*} = {0,1,2}.

Parameter ``a0``:
    the desired preimage of *a1*.

Parameter ``b0``:
    the desired preimage of *b1*.

Parameter ``c0``:
    the desired preimage of *c1*.

Parameter ``a1``:
    the desired image of *a0*.

Parameter ``b1``:
    the desired image of *b0*.

Parameter ``c1``:
    the desired image of *c0*.)doc";

// Docstring regina::python::doc::Perm_::Perm_6
static const char *Perm_6 =
R"doc(Creates a permutation that is a clone of the given permutation.

Parameter ``cloneMe``:
    the permutation to clone.)doc";

// Docstring regina::python::doc::Perm_::S3Index
static const char *S3Index =
R"doc(Returns the index of this permutation in the Perm<3>::S3 array.

This is a dimension-specific alias for SnIndex(). In general, for
every *n* there will be a member function Perm<n>::SnIndex(); however,
these numerical aliases Perm<2>::S2Index(), ..., Perm<5>::S5Index()
are only available for small *n*.

See Sn for further information on how these permutations are indexed.

Returns:
    the index *i* for which this permutation is equal to
    Perm<3>::S3[i]. This will be between 0 and 5 inclusive.)doc";

// Docstring regina::python::doc::Perm_::SnIndex
static const char *SnIndex =
R"doc(Returns the index of this permutation in the Perm<3>::Sn array.

See Sn for further information on how these permutations are indexed.

Returns:
    the index *i* for which this permutation is equal to
    Perm<3>::Sn[i]. This will be between 0 and 5 inclusive.)doc";

// Docstring regina::python::doc::Perm_::__array
static const char *__array =
R"doc(Determines the image of the given integer under this permutation.

Parameter ``source``:
    the integer whose image we wish to find. This should be between 0
    and 2 inclusive.

Returns:
    the image of *source*.)doc";

// Docstring regina::python::doc::Perm_::__eq
static const char *__eq =
R"doc(Determines if this is equal to the given permutation. This is true if
and only if both permutations have the same images for 0, 1 and 2.

Parameter ``other``:
    the permutation with which to compare this.

Returns:
    ``True`` if and only if this and the given permutation are equal.)doc";

// Docstring regina::python::doc::Perm_::__inc
static const char *__inc =
R"doc(A postincrement operator that changes this to be the next permutation
in the array Perm<3>::Sn. If this is the last such permutation then
this will wrap around to become the first permutation in Perm<3>::Sn,
which is the identity.

Python:
    This routine is named inc() since python does not support the
    increment operator.

Returns:
    a copy of this permutation before the increment took place.)doc";

// Docstring regina::python::doc::Perm_::__lt
static const char *__lt =
R"doc(Determines if this appears earlier than the given permutation in the
array Perm<3>::Sn.

Note that this is *not* the same ordering of permutations as the
ordering implied by compareWith(). This is, however, consistent with
the ordering implied by the ++ operators, and this order is also
faster to compute than compareWith().

Parameter ``rhs``:
    the permutation to compare this against.

Returns:
    ``True`` if and only if this appears before *rhs* in *Sn*.)doc";

// Docstring regina::python::doc::Perm_::__mul
static const char *__mul =
R"doc(Returns the composition of this permutation with the given
permutation. If this permutation is *p*, the resulting permutation
will be *p o q*, satisfying ``(p*q)[x] == p[q[x]]``.

Parameter ``q``:
    the permutation with which to compose this.

Returns:
    the composition of both permutations.)doc";

// Docstring regina::python::doc::Perm_::__ne
static const char *__ne =
R"doc(Determines if this differs from the given permutation. This is true if
and only if the two permutations have different images for at least
one of 0, 1 or 2.

Parameter ``other``:
    the permutation with which to compare this.

Returns:
    ``True`` if and only if this and the given permutation differ.)doc";

// Docstring regina::python::doc::Perm_::clear
static const char *clear =
R"doc(Resets the images of all integers from *from* onwards to the identity
map.

Specifically, for each *i* in the range *from*,...,2, this routine
will ensure that ``image[i] == i``. The images of 0,1,...,*from*-1
will not be altered.

Precondition:
    The images of *from*,...,2 are exactly *from*,...,2, but possibly
    in a different order.

Parameter ``from``:
    the first integer whose image should be reset. This must be
    between 0 and 3 inclusive.)doc";

// Docstring regina::python::doc::Perm_::compareWith
static const char *compareWith =
R"doc(Lexicographically compares the images of (0,1,2) under this and the
given permutation.

Note that this does *not* yield the same ordering of permutations as
used by the less-than and increment operators. Moreover, compareWith()
is slower than the less-than operator to compute.

Parameter ``other``:
    the permutation with which to compare this.

Returns:
    -1 if this permutation produces a smaller image, 0 if the
    permutations are equal and 1 if this permutation produces a
    greater image.)doc";

// Docstring regina::python::doc::Perm_::contract
static const char *contract =
R"doc(Restricts a *k*-element permutation to an 3-element permutation, where
*k* > 3.

The resulting permutation will map 0,1,2 to their respective images
under *p*, and will ignore the "unused" images *p*[3],...,*p*[*k*-1].

Precondition:
    The given permutation maps 0,1,2 to 0,1,2 in some order.

Template parameter ``k``:
    the number of elements for the input permutation; this must be
    strictly greater than 3.

Parameter ``p``:
    a permutation on *k* elements.

Returns:
    the same permutation restricted to a permutation on 3 elements.)doc";

// Docstring regina::python::doc::Perm_::extend
static const char *extend =
R"doc(Extends a *k*-element permutation to an 3-element permutation. where 2
≤ *k* < 3. The only possible value of *k* is 2, but this routine is
kept as a template function for consistency with the other classes'
Perm<n>::extend() routines.

The resulting permutation will map 0,1 to their respective images
under *p*, and will map the "unused" element 3 to itself.

Template parameter ``k``:
    the number of elements for the input permutation; this must be
    exactly 2.

Parameter ``p``:
    a permutation on two elements.

Returns:
    the same permutation expressed as a permutation on three elements.)doc";

// Docstring regina::python::doc::Perm_::fromPermCode
static const char *fromPermCode =
R"doc(Creates a permutation from the given internal code.

Precondition:
    the given code is a valid permutation code; see isPermCode() for
    details.

Parameter ``code``:
    the internal code for the new permutation.

Returns:
    the permutation represented by the given internal code.)doc";

// Docstring regina::python::doc::Perm_::inverse
static const char *inverse =
R"doc(Finds the inverse of this permutation.

Returns:
    the inverse of this permutation.)doc";

// Docstring regina::python::doc::Perm_::isConjugacyMinimal
static const char *isConjugacyMinimal =
R"doc(Is this permutation minimal in its conjugacy class?

Here "minimal" means that, amongst all its conjugates, this
permutation has the smallest index in the array Perm<3>::Sn.

See Sn for further information on how permutations are indexed.

This routine is extremely fast for Perm<3>, since it essentially uses
a hard-coded lookup table.

Returns:
    ``True`` if and only if this permutation is minimal in its
    conjugacy class.)doc";

// Docstring regina::python::doc::Perm_::isIdentity
static const char *isIdentity =
R"doc(Determines if this is the identity permutation. This is true if and
only if each of 0, 1 and 2 is mapped to itself.

Returns:
    ``True`` if and only if this is the identity permutation.)doc";

// Docstring regina::python::doc::Perm_::isPermCode
static const char *isPermCode =
R"doc(Determines whether the given integer is a valid internal permutation
code. Valid permutation codes can be passed to setPermCode() or
fromPermCode(), and are returned by permCode().

Returns:
    ``True`` if and only if the given code is a valid internal
    permutation code.)doc";

// Docstring regina::python::doc::Perm_::order
static const char *order =
R"doc(Returns the order of this permutation.

In other words; this routine returns the smallest positive integer *k*
for which the *k*th power of this permutation is the identity.

Returns:
    the order of this permutation.)doc";

// Docstring regina::python::doc::Perm_::orderedS3Index
static const char *orderedS3Index =
R"doc(Returns the lexicographical index of this permutation. This will be
the index of this permutation in the Perm<3>::orderedSn array.

This is a dimension-specific alias for orderedSnIndex(). In general,
for every *n* there will be a member function
Perm<n>::orderedSnIndex(); however, these numerical aliases
Perm<2>::orderedS2Index(), ..., Perm<5>::orderedS5Index() are only
available for small *n*.

See orderedSn for further information on lexicographical ordering.

Returns:
    the lexicographical index of this permutation. This will be
    between 0 and 5 inclusive.)doc";

// Docstring regina::python::doc::Perm_::orderedSnIndex
static const char *orderedSnIndex =
R"doc(Returns the lexicographical index of this permutation. This will be
the index of this permutation in the Perm<3>::orderedSn array.

See orderedSn for further information on lexicographical ordering.

Returns:
    the lexicographical index of this permutation. This will be
    between 0 and 5 inclusive.)doc";

// Docstring regina::python::doc::Perm_::permCode
static const char *permCode =
R"doc(Returns the internal code representing this permutation. Note that the
internal code is sufficient to reproduce the entire permutation.

The code returned will be a valid permutation code as determined by
isPermCode().

Returns:
    the internal code.)doc";

// Docstring regina::python::doc::Perm_::pow
static const char *pow =
R"doc(Computes the given power of this permutation.

This routine runs in constant time.

Parameter ``exp``:
    the exponent; this may be positive, zero or negative.

Returns:
    this permutation raised to the power of *exp*.)doc";

// Docstring regina::python::doc::Perm_::pre
static const char *pre =
R"doc(Determines the preimage of the given integer under this permutation.

Parameter ``image``:
    the integer whose preimage we wish to find. This should be between
    0 and 2 inclusive.

Returns:
    the preimage of *image*.)doc";

// Docstring regina::python::doc::Perm_::rand
static const char *rand =
R"doc(Returns a random permutation on three elements. All permutations are
returned with equal probability.

This routine is thread-safe, and uses RandomEngine for its random
number generation.

.. warning::
    This routine is expensive, since it locks and unlocks the mutex
    protecting Regina's global uniform random bit generator. If you
    are calling this many times in quick succession, consider creating
    a single RandomEngine object yourself and then calling
    ``rand(randomEngine.engine(), even)``.

Parameter ``even``:
    if ``True``, then the resulting permutation is guaranteed to be
    even (and again all even permutations are returned with equal
    probability).

Returns:
    a random permutation.)doc";

// Docstring regina::python::doc::Perm_::reverse
static const char *reverse =
R"doc(Finds the reverse of this permutation.

Here *reverse* means that we reverse the images of 0,1,2. In other
words, if permutation *q* is the reverse of *p*, then ``p[i] == q[2 -
i]`` for all *i*.)doc";

// Docstring regina::python::doc::Perm_::rot
static const char *rot =
R"doc(Returns the *i*th rotation. This maps *k* to *k*&nbsp;+&nbsp;*i* (mod
3) for all *k*.

Parameter ``i``:
    the image of 0; this must be between 0 and 2 inclusive.

Returns:
    the *i*th rotation.)doc";

// Docstring regina::python::doc::Perm_::setPermCode
static const char *setPermCode =
R"doc(Sets this permutation to that represented by the given internal code.

Precondition:
    the given code is a valid permutation code; see isPermCode() for
    details.

Parameter ``code``:
    the internal code that will determine the new value of this
    permutation.)doc";

// Docstring regina::python::doc::Perm_::sign
static const char *sign =
R"doc(Determines the sign of this permutation.

Returns:
    1 if this permutation is even, or -1 if this permutation is odd.)doc";

// Docstring regina::python::doc::Perm_::str
static const char *str =
R"doc(Returns a string representation of this permutation. The
representation will consist of three adjacent digits representing the
images of 0, 1 and 2 respectively. An example of a string
representation is ``120``.

Returns:
    a string representation of this permutation.)doc";

// Docstring regina::python::doc::Perm_::tightDecoding
static const char *tightDecoding =
R"doc(Reconstructs a permutation from its given tight encoding. See the page
on tight "tight encodings" for details.

The tight encoding will be given as a string. If this string contains
leading whitespace or any trailing characters at all (including
trailing whitespace), then it will be treated as an invalid encoding
(i.e., this routine will throw an exception).

Tight encodings are fast to work with for small permutation classes
(*n* ≤ 7), but slower for larger permutation classes (8 ≤ *n* ≤ 16).
See tightEncoding() for further details.

Exception ``InvalidArgument``:
    The given string is not a tight encoding of a 3-element
    permutation.

Parameter ``enc``:
    the tight encoding for a 3-element permutation.

Returns:
    the permutation represented by the given tight encoding.)doc";

// Docstring regina::python::doc::Perm_::tightEncoding
static const char *tightEncoding =
R"doc(Returns the tight encoding of this permutation. See the page on tight
"tight encodings" for details.

For all permutation classes Perm<n>, the tight encoding is based on
the index into the full permutation group *S_n*. For smaller
permutation classes (*n* ≤ 7), such encodings are very fast to work
with since the *S_n* index is used as the internal permutation code.
For larger permutation classes however (8 ≤ *n* ≤ 16), the *S_n* index
requires some non-trivial work to compute.

Returns:
    the resulting encoded string.)doc";

// Docstring regina::python::doc::Perm_::trunc
static const char *trunc =
R"doc(Returns a prefix of the string representation of this permutation,
containing only the images of the first *len* integers.

Parameter ``len``:
    the length of the prefix required; this must be between 0 and 3
    inclusive.

Returns:
    the corresponding prefix of the string representation of this
    permutation.)doc";

// Docstring regina::python::doc::Perm_::trunc2
static const char *trunc2 =
R"doc(Returns a string representation of this permutation with only the
images of 0 and 1. The resulting string will therefore have length
two.

Returns:
    a truncated string representation of this permutation.)doc";

}

namespace Perm_::OrderedS3Lookup_ {

// Docstring regina::python::doc::Perm_::OrderedS3Lookup_::__array
static const char *__array =
R"doc(Returns the permutation at the given index in the array orderedS3. See
Perm<3>::orderedS3 for details.

This operation is extremely fast (and constant time).

Parameter ``index``:
    an index between 0 and 5 inclusive.

Returns:
    the corresponding permutation in orderedS3.)doc";

}

namespace Perm_::S2Lookup_ {

// Docstring regina::python::doc::Perm_::S2Lookup_::__array
static const char *__array =
R"doc(Returns the permutation at the given index in the array S2. See
Perm<3>::S2 for details.

This operation is extremely fast (and constant time).

Parameter ``index``:
    an index between 0 and 1 inclusive.

Returns:
    the corresponding permutation in S2.)doc";

}

namespace Perm_::S3Lookup_ {

// Docstring regina::python::doc::Perm_::S3Lookup_::__array
static const char *__array =
R"doc(Returns the permutation at the given index in the array S3. See
Perm<3>::S3 for details.

This operation is extremely fast (and constant time).

Parameter ``index``:
    an index between 0 and 5 inclusive.

Returns:
    the corresponding permutation in S3.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

