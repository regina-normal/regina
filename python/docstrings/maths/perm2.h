/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Perm
static const char *Perm =
R"doc(Represents a permutation of {0,1}. This is a specialisation of the
generic Perm template: it is highly optimised, but also somewhat
trivial (since there are only two possible permutations). It is
provided simply to optimise the general Perm<n> template for this
trivial case.

As with all Perm template classes, these objects are small enough to
pass by value and swap with std::swap(), with no need for any
specialised move operations or swap functions. Moreover, Perm<2> in
particular is extremely fast to work with.

Each permutation has an internal code, which is a single native
integer that is sufficient to reconstruct the permutation. Thus the
internal code may be a useful means for passing permutation objects to
and from the engine. For Perm<2>, the internal code is 0 for the
identity permutation, or 1 for the (unique) non-identity permutation.
This is consistent with the second-generation codes used in classes
Perm<4>,...,Perm<7>.

To use this class, simply include the main permutation header
maths/perm.h.

.. warning::
    Every permutation class Perm<n> provides a transposition (i.e.,
    pair swap) constructor ``Perm<n>(a,b).`` In addition, the
    specialised classes Perm<3>, Perm<4> and Perm<5> provide "list of
    images" constructors ``Perm<3>(a,b,c)``, ``Perm<4>(a,b,c,d)`` and
    ``Perm<5>(a,b,c,d,e)``. For Perm<2>, these two constructors would
    be indistinguishable (since both would take two integer
    arguments). Here Perm<2> takes an approach that is consistent with
    the generic Perm<n> class: ``Perm<2>(a,b)`` is interpreted as the
    _transposition_ of *a* and *b*. In particular, ``Perm(0,1)`` is
    _not_ the identity permutation.

Python:
    Since Python does not support templates, this class is made
    available under the name Perm2.)doc";

namespace Perm_ {

// Docstring regina::python::doc::Perm_::S1Lookup
static const char *S1Lookup = R"doc(A lightweight array-like object used to implement Perm<2>::S1.)doc";

// Docstring regina::python::doc::Perm_::S2Index
static const char *S2Index =
R"doc(Returns the index of this permutation in the Perm<2>::S2 array.

This is a dimension-specific alias for SnIndex(). In general, for
every *n* there will be a member function Perm<n>::SnIndex(); however,
these numerical aliases Perm<2>::S2Index(), ..., Perm<7>::S7Index()
are only available for small *n*.

See Sn for further information on how these permutations are indexed.

Returns:
    the index *i* for which this permutation is equal to
    Perm<2>::S2[i]. This will be 0 or 1.)doc";

// Docstring regina::python::doc::Perm_::S2Lookup
static const char *S2Lookup = R"doc(A lightweight array-like object used to implement Perm<2>::S2.)doc";

// Docstring regina::python::doc::Perm_::SnIndex
static const char *SnIndex =
R"doc(Returns the index of this permutation in the Perm<2>::Sn array.

See Sn for further information on how these permutations are indexed.

Returns:
    the index *i* for which this permutation is equal to
    Perm<2>::Sn[i]. This will be 0 or 1.)doc";

// Docstring regina::python::doc::Perm_::__array
static const char *__array =
R"doc(Determines the image of the given integer under this permutation.

Parameter ``source``:
    the integer whose image we wish to find. This should be 0 or 1.

Returns:
    the image of *source*.)doc";

// Docstring regina::python::doc::Perm_::__cmp
static const char *__cmp =
R"doc(Compares two permutations according to which appears earlier in the
array Perm<2>::Sn.

For the special case of permutations on two elements, this ordering is
consistent with the ordering implied by compareWith() (but beware: for
other permutation classes this is not true). Also, like all
permutation classes, this ordering is consistent with the ordering
implied by the ++ operators.

This generates all of the usual comparison operators, including ``<``,
``<=``, ``>``, and ``>=``.

Python:
    This spaceship operator ``x <=> y`` is not available, but the
    other comparison operators that it generates _are_ available.

Returns:
    The result that indicates which permutation appears earlier in
    *Sn*.)doc";

// Docstring regina::python::doc::Perm_::__copy
static const char *__copy =
R"doc(Creates a permutation that is a clone of the given permutation.

Parameter ``cloneMe``:
    the permutation to clone.)doc";

// Docstring regina::python::doc::Perm_::__default
static const char *__default = R"doc(Creates the identity permutation.)doc";

// Docstring regina::python::doc::Perm_::__eq
static const char *__eq =
R"doc(Determines if this is equal to the given permutation. This is true if
and only if both permutations have the same images for 0 and 1.

Returns:
    ``True`` if and only if this and the given permutation are equal.)doc";

// Docstring regina::python::doc::Perm_::__inc
static const char *__inc =
R"doc(A postincrement operator that changes this to be the next permutation
in the array Perm<2>::Sn. If this is the last such permutation then
this will wrap around to become the first permutation in Perm<2>::Sn,
which is the identity.

Python:
    This routine is named inc() since python does not support the
    increment operator.

Returns:
    a copy of this permutation before the increment took place.)doc";

// Docstring regina::python::doc::Perm_::__init
static const char *__init =
R"doc(Creates the transposition of *a* and *b*. Note that *a* and *b* need
not be distinct.

Precondition:
    *a* and *b* are in {0,1}.

Parameter ``a``:
    the element to switch with *b*.

Parameter ``b``:
    the element to switch with *a*.)doc";

// Docstring regina::python::doc::Perm_::__init_2
static const char *__init_2 =
R"doc(Creates a permutation mapping *i* to *image*[i] for each *i* = 0,1.

Precondition:
    The elements of *image* are 0 and 1 in some order.

Parameter ``image``:
    the array of images.)doc";

// Docstring regina::python::doc::Perm_::__mul
static const char *__mul =
R"doc(Returns the composition of this permutation with the given
permutation. If this permutation is *p*, the resulting permutation
will be *p*∘*q*, and will satisfy ``(p*q)[x] == p[q[x]]``.

Parameter ``q``:
    the permutation with which to compose this.

Returns:
    the composition of both permutations.)doc";

// Docstring regina::python::doc::Perm_::cachedComp
static const char *cachedComp =
R"doc(An alias for the composition operator, provided to assist with writing
generic code.

This specialised Perm<2> class does not use precomputation for its
optimisations. The only point of having cachedComp() in Perm<2> is to
make it easier to write generic code that works with Perm<n> for any
*n*.

* If you know you are only working with Perm<2>, you should just use
  the composition operator instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedComp(). (For Perm<2>, which does not use precomputation
  for its optimisations, precompute() does nothing.)

The permutation that is returned is the same as you would obtain by
calling ``(*this) * q``.

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedComp(). For Perm<2>,
    precompute() does nothing; however, for other Perm<n> classes a
    failure to do this will almost certainly crash your program.

Parameter ``q``:
    the permutation to compose this with.

Returns:
    the composition of both permutations.)doc";

// Docstring regina::python::doc::Perm_::cachedComp_2
static const char *cachedComp_2 =
R"doc(Deprecated alias for using the composition operator twice, provided to
assist with writing generic code.

The permutation that is returned is the same as you would obtain by
calling ``(*this) * q * r``.

.. deprecated::
    The three-way cachedComp() was originally written to support
    conjugation. If you are indeed conjugating, then call
    cachedConjugate() instead; otherwise just call the two-way
    cachedComp() twice.

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedComp(). For Perm<2>,
    precompute() does nothing; however, for other Perm<n> classes a
    failure to do this will almost certainly crash your program.

Parameter ``q``:
    the first permutation to compose this with.

Parameter ``r``:
    the second permutation to compose this with.

Returns:
    the composition of both permutations.)doc";

// Docstring regina::python::doc::Perm_::cachedConjugate
static const char *cachedConjugate =
R"doc(An alias for conjugate(), provided to assist with writing generic
code.

This specialised Perm<2> class does not use precomputation for its
optimisations. The only point of having cachedConjugate() in Perm<2>
is to make it easier to write generic code that works with Perm<n> for
any *n*.

* If you know you are only working with Perm<2>, you should just call
  conjugate() instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedConjugate(). (For Perm<2>, which does not use
  precomputation for its optimisations, precompute() does nothing.)

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedConjugate(). For Perm<6>,
    precompute() does nothing; however, for other Perm<n> classes a
    failure to do this will almost certainly crash your program.

Parameter ``q``:
    the permutation to conjugate this by.

Returns:
    the conjugate of this permutation by *q*.)doc";

// Docstring regina::python::doc::Perm_::cachedInverse
static const char *cachedInverse =
R"doc(An alias for inverse(), provided to assist with writing generic code.

This specialised Perm<2> class does not use precomputation for its
optimisations. The only point of having cachedInverse() in Perm<2> is
to make it easier to write generic code that works with Perm<n> for
any *n*.

* If you know you are only working with Perm<2>, you should just call
  inverse() instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedInverse(). (For Perm<2>, which does not use
  precomputation for its optimisations, precompute() does nothing.)

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedInverse(). For Perm<2>,
    precompute() does nothing; however, for other Perm<n> classes a
    failure to do this will almost certainly crash your program.

Returns:
    the inverse of this permutation.)doc";

// Docstring regina::python::doc::Perm_::cachedOrder
static const char *cachedOrder =
R"doc(An alias for order(), provided to assist with writing generic code.

This specialised Perm<2> class does not use precomputation for its
optimisations. The only point of having cachedOrder() in Perm<2> is to
make it easier to write generic code that works with Perm<n> for any
*n*.

* If you know you are only working with Perm<2>, you should just call
  order() instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedOrder(). (For Perm<2>, which does not use precomputation
  for its optimisations, precompute() does nothing.)

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedOrder(). For Perm<2>,
    precompute() does nothing; however, for other Perm<n> classes a
    failure to do this will almost certainly crash your program.

Returns:
    the order of this permutation.)doc";

// Docstring regina::python::doc::Perm_::cachedPow
static const char *cachedPow =
R"doc(An alias for pow(), provided to assist with writing generic code.

This specialised Perm<2> class does not use precomputation for its
optimisations. The only point of having cachedPow() in Perm<2> is to
make it easier to write generic code that works with Perm<n> for any
*n*.

* If you know you are only working with Perm<2>, you should just call
  pow() instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedPow(). (For Perm<2>, which does not use precomputation
  for its optimisations, precompute() does nothing.)

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedPow(). For Perm<2>,
    precompute() does nothing; however, for other Perm<n> classes a
    failure to do this will almost certainly crash your program.

Parameter ``exp``:
    the exponent; this may be positive, zero or negative.

Returns:
    this permutation raised to the power of *exp*.)doc";

// Docstring regina::python::doc::Perm_::clear
static const char *clear =
R"doc(Resets the images of all integers from *from* onwards to the identity
map.

Specifically, for each *i* in the range *from*,...,1, this routine
will ensure that ``image[i] == i``. The images of 0,1,...,*from*-1
will not be altered.

Precondition:
    The images of *from*,...,1 are exactly *from*,...,1, but possibly
    in a different order.

Parameter ``from``:
    the first integer whose image should be reset. This must be
    between 0 and 2 inclusive.)doc";

// Docstring regina::python::doc::Perm_::compareWith
static const char *compareWith =
R"doc(Lexicographically compares the images of (0,1) under this and the
given permutation.

Parameter ``other``:
    the permutation with which to compare this.

Returns:
    -1 if this permutation produces a smaller image, 0 if the
    permutations are equal and 1 if this permutation produces a
    greater image.)doc";

// Docstring regina::python::doc::Perm_::conjugate
static const char *conjugate =
R"doc(Computes the conjugate of this permutation by *q*.

Specifically, calling ``p.conjugate(q)`` is equivalent to computing
``q * p * q.inverse()``. The resulting permutation will have the same
cycle structure as *p*, but with the cycle elements translated
according to *q*.

Parameter ``q``:
    the permutation to conjugate this by.

Returns:
    the conjugate of this permutation by *q*.)doc";

// Docstring regina::python::doc::Perm_::contract
static const char *contract =
R"doc(Restricts a *k*-element permutation to an 2-element permutation, where
*k* > 2.

The resulting permutation will map 0,1 to their respective images
under *p*, and will ignore the "unused" images *p*[2],...,*p*[*k*-1].

Precondition:
    The given permutation maps 0,1 to 0,1 in some order.

Template parameter ``k``:
    the number of elements for the input permutation; this must be
    strictly greater than 2.

Parameter ``p``:
    a permutation on *k* elements.

Returns:
    the same permutation restricted to a permutation on 2 elements.)doc";

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

// Docstring regina::python::doc::Perm_::hash
static const char *hash =
R"doc(Hashes this permutation to a non-negative integer, allowing it to be
used for keys in hash tables.

The implementation currently returns the internal permutation code
(which for Perm<2> will always fit within a ``size_t``). This
implementation (and therefore the specific hash values obtained) is
subject to change in future versions of Regina.

Python:
    For Python users, this function uses the standard Python name
    __hash__(). This allows permutations to be used as keys in Python
    dictionaries and sets.

Returns:
    The integer hash of this permutation.)doc";

// Docstring regina::python::doc::Perm_::inverse
static const char *inverse =
R"doc(Finds the inverse of this permutation.

Returns:
    the inverse of this permutation.)doc";

// Docstring regina::python::doc::Perm_::isConjugacyMinimal
static const char *isConjugacyMinimal =
R"doc(Is this permutation minimal in its conjugacy class?

Here "minimal" means that, amongst all its conjugates, this
permutation has the smallest index in the array Perm<2>::Sn.

See Sn for further information on how permutations are indexed.

This routine is extremely fast for Perm<2>, since the answer is always
``True``.

Returns:
    ``True`` if and only if this permutation is minimal in its
    conjugacy class.)doc";

// Docstring regina::python::doc::Perm_::isIdentity
static const char *isIdentity =
R"doc(Determines if this is the identity permutation. This is true if and
only if each of 0 and 1 is mapped to itself.

Returns:
    ``True`` if and only if this is the identity permutation.)doc";

// Docstring regina::python::doc::Perm_::isPermCode
static const char *isPermCode =
R"doc(Determines whether the given integer is a valid internal permutation
code. Valid permutation codes can be passed to setPermCode() or
fromPermCode(), and are returned by permCode().

Parameter ``code``:
    the permutation code to test.

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

// Docstring regina::python::doc::Perm_::orderedS2Index
static const char *orderedS2Index =
R"doc(Returns the lexicographical index of this permutation. This will be
the index of this permutation in the Perm<2>::orderedSn array.

This is a dimension-specific alias for orderedSnIndex(). In general,
for every *n* there will be a member function
Perm<n>::orderedSnIndex(); however, these numerical aliases
Perm<2>::orderedS2Index(), ..., Perm<7>::orderedS7Index() are only
available for small *n*.

See orderedSn for further information on lexicographical ordering.

Returns:
    the lexicographical index of this permutation. This will be 0 or
    1.)doc";

// Docstring regina::python::doc::Perm_::orderedSnIndex
static const char *orderedSnIndex =
R"doc(Returns the lexicographical index of this permutation. This will be
the index of this permutation in the Perm<2>::orderedSn array.

See orderedSn for further information on lexicographical ordering.

Returns:
    the lexicographical index of this permutation. This will be 0 or
    1.)doc";

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
    the integer whose preimage we wish to find. This should be 0 or 1.

Returns:
    the preimage of *image*.)doc";

// Docstring regina::python::doc::Perm_::precompute
static const char *precompute =
R"doc(A do-nothing routine that assists with writing generic code.

This specialised Perm<2> class does not use precomputation for its
optimisations, and so this precompute() function does nothing. The
only point of having precompute() in Perm<2> is to make it easier to
write generic code that works with Perm<n> for any *n*.

* If you know you are only working with Perm<2>, you do not need to
  call this function at all.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using any of the optimised ``cachedXXX()`` functions, such as
  cachedComp(), cachedInverse(), and so on.

All Perm<n>::precompute() routines are thread-safe, and are harmless
if called multiple times (since any call after the first will do
nothing).)doc";

// Docstring regina::python::doc::Perm_::rand
static const char *rand =
R"doc(Returns a random permutation on two elements. All permutations are
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
    even (which means, for a permutation on two elements, the
    resulting permutation must be the identity).

Returns:
    a random permutation.)doc";

// Docstring regina::python::doc::Perm_::reverse
static const char *reverse =
R"doc(Finds the reverse of this permutation.

Here _reverse_ means that we reverse the images of 0 and 1. In other
words, if permutation *q* is the reverse of *p*, then ``p[i] == q[1 -
i]`` for all *i*.)doc";

// Docstring regina::python::doc::Perm_::rot
static const char *rot =
R"doc(Returns the *i*th rotation. This maps *k* to *k* + *i* (mod 2) for all
*k*.

Parameter ``i``:
    the image of 0; this must be 0 or 1.

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
representation will consist of two adjacent digits representing the
images of 0 and 1 respectively. An example of a string representation
is ``10``.

Returns:
    a string representation of this permutation.)doc";

// Docstring regina::python::doc::Perm_::tightDecoding
static const char *tightDecoding =
R"doc(Reconstructs a permutation from its given tight encoding. See the page
on tight encodings for details.

The tight encoding will be given as a string. If this string contains
leading whitespace or any trailing characters at all (including
trailing whitespace), then it will be treated as an invalid encoding
(i.e., this routine will throw an exception).

Tight encodings are fast to work with for small permutation classes
(*n* ≤ 7), but slower for larger permutation classes (8 ≤ *n* ≤ 16).
See tightEncoding() for further details.

Exception ``InvalidArgument``:
    The given string is not a tight encoding of a 2-element
    permutation.

Parameter ``enc``:
    the tight encoding for a 2-element permutation.

Returns:
    the permutation represented by the given tight encoding.)doc";

// Docstring regina::python::doc::Perm_::tightEncoding
static const char *tightEncoding =
R"doc(Returns the tight encoding of this permutation. See the page on tight
encodings for details.

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
    the length of the prefix required; this must be between 0 and 2
    inclusive.

Returns:
    the corresponding prefix of the string representation of this
    permutation.)doc";

}

namespace Perm_::S1Lookup_ {

// Docstring regina::python::doc::Perm_::S1Lookup_::__array
static const char *__array =
R"doc(Returns the permutation at the given index in the array S1. See
Perm<2>::S1 for details.

This operation is extremely fast (and constant time).

Parameter ``index``:
    an index; the only allowed value is 0.

Returns:
    the corresponding permutation in S1.)doc";

// Docstring regina::python::doc::Perm_::S1Lookup_::size
static const char *size =
R"doc(Returns the number of permutations in the array S1.

Returns:
    the size of this array.)doc";

}

namespace Perm_::S2Lookup_ {

// Docstring regina::python::doc::Perm_::S2Lookup_::__array
static const char *__array =
R"doc(Returns the permutation at the given index in the array S2. See
Perm<2>::S2 for details.

This operation is extremely fast (and constant time).

Parameter ``index``:
    an index between 0 and 1 inclusive.

Returns:
    the corresponding permutation in S2.)doc";

// Docstring regina::python::doc::Perm_::S2Lookup_::size
static const char *size =
R"doc(Returns the number of permutations in the array S2.

Returns:
    the size of this array.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

