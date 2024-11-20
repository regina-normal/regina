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
R"doc(Represents a permutation of {0,1,2,3,4}. This is a specialisation of
the generic Perm template: it is highly optimised, and also offers
some additional functionality. Amongst other things, this permutation
class is used to specify how simplices of a 4-manifold triangulation
are glued together.

As with all Perm template classes, these objects are small enough to
pass by value and swap with std::swap(), with no need for any
specialised move operations or swap functions.

Each permutation has an internal code, which is a single native
integer that is sufficient to reconstruct the permutation. Thus the
internal code may be a useful means for passing permutation objects to
and from the engine. For Perm<5>, the internal permutation codes have
changed as of Regina 7.0:

* _First-generation_ codes were used internally in Regina 6.0.1 and
  earlier. These are _image packs_: integers whose lowest three bits
  represent the image of 0, whose next lowest three bits represent the
  image of 1, and so on. The routines permCode1(), setPermCode1(),
  fromPermCode1() and isPermCode1() continue to work with first-
  generation codes for backward compatibility. Likewise, the XML data
  file format continues to use first-generation codes to describe
  pentachoron gluings.

* _Second-generation_ codes are used internally in Regina 7.0 and
  above. These codes are integers between 0 and 119 inclusive,
  representing the index of the permutation in the array Perm<5>::Sn.
  The routines permCode2(), setPermCode2(), fromPermCode2() and
  isPermCode2() work with second-generation codes.

It is highly recommended that, if you need to work with permutation
codes at all, you use second-generation codes where possible. This is
because the first-generation routines incur additional overhead in
converting back and forth between the second-generation codes (which
are used internally by Perm<5>).

You can iterate through all permutations using a range-based ``for``
loop over *Sn*, and this will be extremely fast in both C++ and
Python:

```
for (auto p : Perm<5>::Sn) { ... }
```

This behaviour does not generalise to the large permutation classes
Perm<n> with *n* ≥ 8, which are not as tightly optimised: such range-
based ``for`` loops are still supported for *n* ≥ 8 but will be
significantly slower in Python than in C++. See the generic Perm class
notes for further details.

To use this class, simply include the main permutation header
maths/perm.h.

Python:
    Since Python does not support templates, this class is made
    available under the name Perm5.)doc";

namespace Perm_ {

// Docstring regina::python::doc::Perm_::S5Index
static const char *S5Index =
R"doc(Returns the index of this permutation in the Perm<5>::Sn array.

This is a dimension-specific alias for SnIndex(). In general, for
every *n* there will be a member function Perm<n>::SnIndex(); however,
these numerical aliases Perm<2>::S2Index(), ..., Perm<7>::S7Index()
are only available for small *n*.

See Sn for further information on how these permutations are indexed.

Returns:
    the index *i* for which this permutation is equal to
    Perm<5>::Sn[i]. This will be between 0 and 119 inclusive.)doc";

// Docstring regina::python::doc::Perm_::SnIndex
static const char *SnIndex =
R"doc(Returns the index of this permutation in the Perm<5>::Sn array.

See Sn for further information on how these permutations are indexed.

Returns:
    the index *i* for which this permutation is equal to
    Perm<5>::Sn[i]. This will be between 0 and 119 inclusive.)doc";

// Docstring regina::python::doc::Perm_::__array
static const char *__array =
R"doc(Determines the image of the given integer under this permutation.

Parameter ``source``:
    the integer whose image we wish to find. This should be between 0
    and 4 inclusive.

Returns:
    the image of *source*.)doc";

// Docstring regina::python::doc::Perm_::__cmp
static const char *__cmp =
R"doc(Compares two permutations according to which appears earlier in the
array Perm<5>::Sn.

Note that this is _not_ the same ordering of permutations as the
ordering implied by compareWith(). This ordering is, however,
consistent with the ordering implied by the ++ operators, and this
ordering is also faster to compute than compareWith().

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
and only if both permutations have the same images for 0, 1, 2, 3 and
4.

Returns:
    ``True`` if and only if this and the given permutation are equal.)doc";

// Docstring regina::python::doc::Perm_::__inc
static const char *__inc =
R"doc(A postincrement operator that changes this to be the next permutation
in the array Perm<5>::Sn. If this is the last such permutation then
this will wrap around to become the first permutation in Perm<5>::Sn,
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
    *a* and *b* are in {0,1,2,3,4}.

Parameter ``a``:
    the element to switch with *b*.

Parameter ``b``:
    the element to switch with *a*.)doc";

// Docstring regina::python::doc::Perm_::__init_2
static const char *__init_2 =
R"doc(Creates a permutation mapping (0,1,2,3,4) to (*a*,*b*,*c*,*d*,*e*)
respectively.

Precondition:
    {*a*,*b*,*c*,*d*,*e*} = {0,1,2,3,4}.

Parameter ``a``:
    the desired image of 0.

Parameter ``b``:
    the desired image of 1.

Parameter ``c``:
    the desired image of 2.

Parameter ``d``:
    the desired image of 3.

Parameter ``e``:
    the desired image of 4.)doc";

// Docstring regina::python::doc::Perm_::__init_3
static const char *__init_3 =
R"doc(Creates a permutation mapping *i* to *image*[i] for each *i* =
0,1,2,3,4.

Precondition:
    The elements of *image* are 0, 1, 2, 3 and 4 in some order.

Parameter ``image``:
    the array of images.)doc";

// Docstring regina::python::doc::Perm_::__init_4
static const char *__init_4 =
R"doc(Creates a permutation mapping (*a0*,*b0*,*c0*,*d0*,*e0*) to
(*a1*,*b1*,*c1*,*d1*,*e1*) respectively.

Precondition:
    {*a0*,*b0*,*c0*,*d0*,*e0*} = {*a1*,*b1*,*c1*,*d1*,*e1*} =
    {0,1,2,3,4}.

Parameter ``a0``:
    the desired preimage of *a1*.

Parameter ``b0``:
    the desired preimage of *b1*.

Parameter ``c0``:
    the desired preimage of *c1*.

Parameter ``d0``:
    the desired preimage of *d1*.

Parameter ``e0``:
    the desired preimage of *e1*.

Parameter ``a1``:
    the desired image of *a0*.

Parameter ``b1``:
    the desired image of *b0*.

Parameter ``c1``:
    the desired image of *c0*.

Parameter ``d1``:
    the desired image of *d0*.

Parameter ``e1``:
    the desired image of *e0*.)doc";

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

This specialised Perm<5> class does not use precomputation for its
optimisations. The only point of having cachedComp() in Perm<5> is to
make it easier to write generic code that works with Perm<n> for any
*n*.

* If you know you are only working with Perm<5>, you should just use
  the composition operator instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedComp(). (For Perm<5>, which does not use precomputation
  for its optimisations, precompute() does nothing.)

The permutation that is returned is the same as you would obtain by
calling ``(*this) * q``.

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedComp(). For Perm<5>,
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
    of this program before calling cachedComp(). For Perm<5>,
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

This specialised Perm<5> class does not use precomputation for its
optimisations. The only point of having cachedConjugate() in Perm<5>
is to make it easier to write generic code that works with Perm<n> for
any *n*.

* If you know you are only working with Perm<5>, you should just call
  conjugate() instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedConjugate(). (For Perm<5>, which does not use
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

This specialised Perm<5> class does not use precomputation for its
optimisations. The only point of having cachedInverse() in Perm<5> is
to make it easier to write generic code that works with Perm<n> for
any *n*.

* If you know you are only working with Perm<5>, you should just call
  inverse() instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedInverse(). (For Perm<5>, which does not use
  precomputation for its optimisations, precompute() does nothing.)

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedInverse(). For Perm<5>,
    precompute() does nothing; however, for other Perm<n> classes a
    failure to do this will almost certainly crash your program.

Returns:
    the inverse of this permutation.)doc";

// Docstring regina::python::doc::Perm_::cachedOrder
static const char *cachedOrder =
R"doc(An alias for order(), provided to assist with writing generic code.

This specialised Perm<5> class does not use precomputation for its
optimisations. The only point of having cachedOrder() in Perm<5> is to
make it easier to write generic code that works with Perm<n> for any
*n*.

* If you know you are only working with Perm<5>, you should just call
  order() instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedOrder(). (For Perm<5>, which does not use precomputation
  for its optimisations, precompute() does nothing.)

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedOrder(). For Perm<5>,
    precompute() does nothing; however, for other Perm<n> classes a
    failure to do this will almost certainly crash your program.

Returns:
    the order of this permutation.)doc";

// Docstring regina::python::doc::Perm_::cachedPow
static const char *cachedPow =
R"doc(An alias for pow(), provided to assist with writing generic code.

This specialised Perm<5> class does not use precomputation for its
optimisations. The only point of having cachedPow() in Perm<5> is to
make it easier to write generic code that works with Perm<n> for any
*n*.

* If you know you are only working with Perm<5>, you should just call
  pow() instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedPow(). (For Perm<5>, which does not use precomputation
  for its optimisations, precompute() does nothing.)

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedPow(). For Perm<5>,
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

Specifically, for each *i* in the range *from*,...,4, this routine
will ensure that ``image[i] == i``. The images of 0,1,...,*from*-1
will not be altered.

Precondition:
    The images of *from*,...,4 are exactly *from*,...,4, but possibly
    in a different order.

Parameter ``from``:
    the first integer whose image should be reset. This must be
    between 0 and 5 inclusive.)doc";

// Docstring regina::python::doc::Perm_::compareWith
static const char *compareWith =
R"doc(Lexicographically compares the images of (0,1,2,3,4) under this and
the given permutation.

Note that this does _not_ yield the same ordering of permutations as
used by the less-than and increment operators. Moreover, compareWith()
is slower than the less-than operator to compute.

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
R"doc(Restricts a *k*-element permutation to an 5-element permutation, where
*k* > 5.

The resulting permutation will map 0,...,4 to their respective images
under *p*, and will ignore the "unused" images *p*[5],...,*p*[*k*-1].

Precondition:
    The given permutation maps 0,...,4 to 0,...,4 in some order.

Template parameter ``k``:
    the number of elements for the input permutation; this must be
    strictly greater than 5.

Parameter ``p``:
    a permutation on *k* elements.

Returns:
    the same permutation restricted to a permutation on 5 elements.)doc";

// Docstring regina::python::doc::Perm_::extend
static const char *extend =
R"doc(Extends a *k*-element permutation to a 5-element permutation, where 2
≤ *k* < 5.

The resulting permutation will map 0,...,*k*-1 to their respective
images under *p*, and will map the "unused" elements *k*,...,4 to
themselves.

Template parameter ``k``:
    the number of elements for the input permutation; this must be 2,
    3 or 4.

Parameter ``p``:
    a permutation on *k* elements.

Returns:
    the same permutation expressed as a permutation on five elements.)doc";

// Docstring regina::python::doc::Perm_::fromImagePack
static const char *fromImagePack =
R"doc(Creates a permutation from the given image pack.

See the class notes for more information on image packs, and how they
are used to build the old first-generation permutation codes.

For Perm<5>, this routine is identical to fromPermCode1().

Precondition:
    The argument *pack* is a valid image pack; see isImagePack() for
    details.

Parameter ``pack``:
    an image pack that describes a permutation.

Returns:
    the permutation represented by the given image pack.)doc";

// Docstring regina::python::doc::Perm_::fromPermCode1
static const char *fromPermCode1 =
R"doc(Creates a permutation from the given first-generation permutation
code.

Precondition:
    the given code is a valid first-generation permutation code; see
    isPermCode1() for details.

.. warning::
    This routine will incur additional overhead, since Perm<5> now
    uses second-generation codes internally. See the class notes and
    the routine fromPermCode2() for details.

Parameter ``code``:
    the first-generation code for the new permutation.

Returns:
    the permutation represented by the given code.)doc";

// Docstring regina::python::doc::Perm_::fromPermCode2
static const char *fromPermCode2 =
R"doc(Creates a permutation from the given second-generation permutation
code.

Second-generation codes are fast to work with, since they are used
internally by the Perm<5> class.

Precondition:
    the given code is a valid second-generation permutation code; see
    isPermCode2() for details.

Parameter ``code``:
    the second-generation code for the new permutation.

Returns:
    the permutation represented by the given code.)doc";

// Docstring regina::python::doc::Perm_::hash
static const char *hash =
R"doc(Hashes this permutation to a non-negative integer, allowing it to be
used for keys in hash tables.

The implementation currently returns the internal permutation code
(which for Perm<5> will always fit within a ``size_t``). This
implementation (and therefore the specific hash values obtained) is
subject to change in future versions of Regina.

Python:
    For Python users, this function uses the standard Python name
    __hash__(). This allows permutations to be used as keys in Python
    dictionaries and sets.

Returns:
    The integer hash of this permutation.)doc";

// Docstring regina::python::doc::Perm_::imagePack
static const char *imagePack =
R"doc(Returns the image pack that represents this permutation.

See the class notes for more information on image packs, and how they
are used to build the old first-generation permutation codes.

For Perm<5>, this routine is identical to permCode1().

Returns:
    the image pack for this permutation.)doc";

// Docstring regina::python::doc::Perm_::inverse
static const char *inverse =
R"doc(Finds the inverse of this permutation.

Returns:
    the inverse of this permutation.)doc";

// Docstring regina::python::doc::Perm_::isConjugacyMinimal
static const char *isConjugacyMinimal =
R"doc(Is this permutation minimal in its conjugacy class?

Here "minimal" means that, amongst all its conjugates, this
permutation has the smallest index in the array Perm<5>::Sn.

See Sn for further information on how permutations are indexed.

This routine is extremely fast for Perm<5>, since it essentially uses
a hard-coded lookup table.

Returns:
    ``True`` if and only if this permutation is minimal in its
    conjugacy class.)doc";

// Docstring regina::python::doc::Perm_::isIdentity
static const char *isIdentity =
R"doc(Determines if this is the identity permutation. This is true if and
only if each of 0, 1, 2, 3 and 4 is mapped to itself.

Returns:
    ``True`` if and only if this is the identity permutation.)doc";

// Docstring regina::python::doc::Perm_::isImagePack
static const char *isImagePack =
R"doc(Determines whether the given argument is the image pack of some
5-element permutation.

See the class notes for more information on image packs, and how they
are used to build the old first-generation permutation codes.

For Perm<5>, this routine is identical to isPermCode1().

Parameter ``pack``:
    the candidate image pack to test.

Returns:
    ``True`` if and only if *pack* is a valid image pack.)doc";

// Docstring regina::python::doc::Perm_::isPermCode1
static const char *isPermCode1 =
R"doc(Determines whether the given character is a valid first-generation
permutation code. Valid first-generation codes can be passed to
setPermCode1() or fromPermCode1(), and are returned by permCode1().

.. warning::
    This routine will incur additional overhead, since Perm<5> now
    uses second-generation codes internally. See the class notes and
    the routine isPermCode2() for details.

Parameter ``code``:
    the permutation code to test.

Returns:
    ``True`` if and only if the given code is a valid first-generation
    permutation code.)doc";

// Docstring regina::python::doc::Perm_::isPermCode2
static const char *isPermCode2 =
R"doc(Determines whether the given character is a valid second-generation
permutation code. Valid second-generation codes can be passed to
setPermCode2() or fromPermCode2(), and are returned by permCode2().

Second-generation codes are fast to work with, since they are used
internally by the Perm<5> class.

Parameter ``code``:
    the permutation code to test.

Returns:
    ``True`` if and only if the given code is a valid second-
    generation permutation code.)doc";

// Docstring regina::python::doc::Perm_::order
static const char *order =
R"doc(Returns the order of this permutation.

In other words; this routine returns the smallest positive integer *k*
for which the *k*th power of this permutation is the identity.

Returns:
    the order of this permutation.)doc";

// Docstring regina::python::doc::Perm_::orderedS5Index
static const char *orderedS5Index =
R"doc(Returns the lexicographical index of this permutation. This will be
the index of this permutation in the Perm<5>::orderedSn array.

This is a dimension-specific alias for orderedSnIndex(). In general,
for every *n* there will be a member function
Perm<n>::orderedSnIndex(); however, these numerical aliases
Perm<2>::orderedS2Index(), ..., Perm<7>::orderedS7Index() are only
available for small *n*.

See orderedSn for further information on lexicographical ordering.

Returns:
    the lexicographical index of this permutation. This will be
    between 0 and 119 inclusive.)doc";

// Docstring regina::python::doc::Perm_::orderedSnIndex
static const char *orderedSnIndex =
R"doc(Returns the lexicographical index of this permutation. This will be
the index of this permutation in the Perm<5>::orderedSn array.

See orderedSn for further information on lexicographical ordering.

Returns:
    the lexicographical index of this permutation. This will be
    between 0 and 119 inclusive.)doc";

// Docstring regina::python::doc::Perm_::permCode1
static const char *permCode1 =
R"doc(Returns the first-generation code representing this permutation. This
code is sufficient to reproduce the entire permutation.

The code returned will be a valid first-generation permutation code as
determined by isPermCode1().

.. warning::
    This routine will incur additional overhead, since Perm<5> now
    uses second-generation codes internally. See the class notes and
    the routine permCode2() for details.

Returns:
    the first-generation permutation code.)doc";

// Docstring regina::python::doc::Perm_::permCode2
static const char *permCode2 =
R"doc(Returns the second-generation code representing this permutation. This
code is sufficient to reproduce the entire permutation.

The code returned will be a valid second-generation permutation code
as determined by isPermCode2().

Second-generation codes are fast to work with, since they are used
internally by the Perm<5> class.

Returns:
    the second-generation permutation code.)doc";

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
    0 and 4 inclusive.

Returns:
    the preimage of *image*.)doc";

// Docstring regina::python::doc::Perm_::precompute
static const char *precompute =
R"doc(A do-nothing routine that assists with writing generic code.

This specialised Perm<5> class does not use precomputation for its
optimisations, and so this precompute() function does nothing. The
only point of having precompute() in Perm<5> is to make it easier to
write generic code that works with Perm<n> for any *n*.

* If you know you are only working with Perm<5>, you do not need to
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
R"doc(Returns a random permutation on five elements. All permutations are
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

Here _reverse_ means that we reverse the images of 0,...,4. In other
words, if permutation *q* is the reverse of *p*, then ``p[i] == q[4 -
i]`` for all *i*.)doc";

// Docstring regina::python::doc::Perm_::rot
static const char *rot =
R"doc(Returns the *i*th rotation. This maps *k* to *k* + *i* (mod 5) for all
*k*.

Parameter ``i``:
    the image of 0; this must be between 0 and 4 inclusive.

Returns:
    the *i*th rotation.)doc";

// Docstring regina::python::doc::Perm_::setPermCode1
static const char *setPermCode1 =
R"doc(Sets this permutation to that represented by the given first-
generation permutation code.

Precondition:
    the given code is a valid first-generation permutation code; see
    isPermCode1() for details.

.. warning::
    This routine will incur additional overhead, since Perm<5> now
    uses second-generation codes internally. See the class notes and
    the routine setPermCode2() for details.

Parameter ``code``:
    the first-generation code that will determine the new value of
    this permutation.)doc";

// Docstring regina::python::doc::Perm_::setPermCode2
static const char *setPermCode2 =
R"doc(Sets this permutation to that represented by the given second-
generation permutation code.

Second-generation codes are fast to work with, since they are used
internally by the Perm<5> class.

Precondition:
    the given code is a valid second-generation permutation code; see
    isPermCode2() for details.

Parameter ``code``:
    the second-generation code that will determine the new value of
    this permutation.)doc";

// Docstring regina::python::doc::Perm_::sign
static const char *sign =
R"doc(Determines the sign of this permutation.

Returns:
    1 if this permutation is even, or -1 if this permutation is odd.)doc";

// Docstring regina::python::doc::Perm_::str
static const char *str =
R"doc(Returns a string representation of this permutation. The
representation will consist of five adjacent digits representing the
images of 0, 1, 2, 3 and 4 respectively. An example of a string
representation is ``30421``.

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
    The given string is not a tight encoding of a 5-element
    permutation.

Parameter ``enc``:
    the tight encoding for a 5-element permutation.

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
    the length of the prefix required; this must be between 0 and 5
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

// Docstring regina::python::doc::Perm_::trunc3
static const char *trunc3 =
R"doc(Returns a string representation of this permutation with only the
images of 0, 1 and 2. The resulting string will therefore have length
three.

Returns:
    a truncated string representation of this permutation.)doc";

// Docstring regina::python::doc::Perm_::trunc4
static const char *trunc4 =
R"doc(Returns a string representation of this permutation with only the
images of 0, 1, 2 and 3. The resulting string will therefore have
length four.

Returns:
    a truncated string representation of this permutation.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

