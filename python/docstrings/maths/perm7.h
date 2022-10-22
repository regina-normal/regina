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
R"doc(Represents a permutation of {0,1,2,3,4,5,6}. This is a specialisation
of the generic Perm template: it is highly optimised, and also offers
some additional functionality. Amongst other things, this permutation
class is used to specify how simplices of a 6-dimensional
triangulation are glued together.

As with all Perm template classes, these objects are small enough to
pass by value and swap with std::swap(), with no need for any
specialised move operations or swap functions.

Each permutation has an internal code, which is a single native
integer that is sufficient to reconstruct the permutation. Thus the
internal code may be a useful means for passing permutation objects to
and from the engine. For Perm<7>, the internal permutation codes have
changed as of Regina 7.0:

* _First-generation_ codes were used internally in Regina 6.0.1 and
  earlier. These are _image packs_: integers whose lowest three bits
  represent the image of 0, whose next lowest three bits represent the
  image of 1, and so on. The routines permCode1(), setPermCode1(),
  fromPermCode1() and isPermCode1() continue to work with first-
  generation codes for backward compatibility. Likewise, the XML data
  file format continues to use first-generation codes to describe
  gluings between 6-simplices.

* _Second-generation_ codes are used internally in Regina 7.0 and
  above. These codes are integers between 0 and 5039 inclusive,
  representing the index of the permutation in the array Perm<7>::S7.
  The routines permCode2(), setPermCode2(), fromPermCode2() and
  isPermCode2() work with second-generation codes.

It is highly recommended that, if you need to work with permutation
codes at all, you use second-generation codes where possible. This is
because the first-generation routines incur additional overhead in
converting back and forth between the second-generation codes (which
are used internally by Perm<7>).

To use this class, simply include the main permutation header
maths/perm.h.

Python:
    Since Python does not support templates, this class is made
    available under the name Perm7.)doc";

namespace Perm_ {

// Docstring regina::python::doc::Perm_::OrderedS7Lookup
static const char *OrderedS7Lookup = R"doc(A lightweight array-like object used to implement Perm<7>::orderedS7.)doc";

// Docstring regina::python::doc::Perm_::S7Index
static const char *S7Index =
R"doc(Returns the index of this permutation in the Perm<7>::S7 array.

This is a dimension-specific alias for SnIndex(). In general, for
every *n* there will be a member function Perm<n>::SnIndex(); however,
these numerical aliases Perm<2>::S2Index(), ..., Perm<7>::S7Index()
are only available for small *n*.

See Sn for further information on how these permutations are indexed.

Returns:
    the index *i* for which this permutation is equal to
    Perm<7>::S7[i]. This will be between 0 and 5039 inclusive.)doc";

// Docstring regina::python::doc::Perm_::S7Lookup
static const char *S7Lookup = R"doc(A lightweight array-like object used to implement Perm<7>::S7.)doc";

// Docstring regina::python::doc::Perm_::SnIndex
static const char *SnIndex =
R"doc(Returns the index of this permutation in the Perm<7>::Sn array.

See Sn for further information on how these permutations are indexed.

Returns:
    the index *i* for which this permutation is equal to
    Perm<7>::Sn[i]. This will be between 0 and 5039 inclusive.)doc";

// Docstring regina::python::doc::Perm_::__array
static const char *__array =
R"doc(Determines the image of the given integer under this permutation.

Parameter ``source``:
    the integer whose image we wish to find. This should be between 0
    and 6 inclusive.

Returns:
    the image of *source*.)doc";

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
and only if both permutations have the same images for 0, 1, 2, 3, 4,
5 and 6.

Parameter ``other``:
    the permutation with which to compare this.

Returns:
    ``True`` if and only if this and the given permutation are equal.)doc";

// Docstring regina::python::doc::Perm_::__inc
static const char *__inc =
R"doc(A postincrement operator that changes this to be the next permutation
in the array Perm<7>::Sn. If this is the last such permutation then
this will wrap around to become the first permutation in Perm<7>::Sn,
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
    *a* and *b* are in {0,1,2,3,4,5,6}.

Parameter ``a``:
    the element to switch with *b*.

Parameter ``b``:
    the element to switch with *a*.)doc";

// Docstring regina::python::doc::Perm_::__init_2
static const char *__init_2 =
R"doc(Creates a permutation mapping (0,1,2,3,4,5,6) to
(*a*,*b*,*c*,*d*,*e*,*f*,*g*) respectively.

Precondition:
    {*a*,*b*,*c*,*d*,*e*,*f*,*g*} = {0,1,2,3,4,5,6}.

Parameter ``a``:
    the desired image of 0.

Parameter ``b``:
    the desired image of 1.

Parameter ``c``:
    the desired image of 2.

Parameter ``d``:
    the desired image of 3.

Parameter ``e``:
    the desired image of 4.

Parameter ``f``:
    the desired image of 5.

Parameter ``g``:
    the desired image of 6.)doc";

// Docstring regina::python::doc::Perm_::__init_3
static const char *__init_3 =
R"doc(Creates a permutation mapping *i* to *image*[i] for each *i* =
0,1,2,3,4,5,6.

Precondition:
    The elements of *image* are 0, 1, 2, 3, 4, 5 and 6 in some order.

Parameter ``image``:
    the array of images.)doc";

// Docstring regina::python::doc::Perm_::__init_4
static const char *__init_4 =
R"doc(Creates a permutation mapping (*a0*,*b0*,*c0*,*d0*,*e0*,*f0*,*g0*) to
(*a1*,*b1*,*c1*,*d1*,*e1*,*f1*,*g1*) respectively.

Precondition:
    {*a0*,*b0*,*c0*,*d0*,*e0*,*f0*,*g0*} =
    {*a1*,*b1*,*c1*,*d1*,*e1*,*f1*,*g1*} = {0,1,2,3,4,5,6}.

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

Parameter ``f0``:
    the desired preimage of *f1*.

Parameter ``g0``:
    the desired preimage of *g1*.

Parameter ``a1``:
    the desired image of *a0*.

Parameter ``b1``:
    the desired image of *b0*.

Parameter ``c1``:
    the desired image of *c0*.

Parameter ``d1``:
    the desired image of *d0*.

Parameter ``e1``:
    the desired image of *e0*.

Parameter ``f1``:
    the desired image of *f0*.

Parameter ``g1``:
    the desired image of *g0*.)doc";

// Docstring regina::python::doc::Perm_::__lt
static const char *__lt =
R"doc(Determines if this appears earlier than the given permutation in the
array Perm<7>::Sn.

Note that this is _not_ the same ordering of permutations as the
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
will be *p*∘*q*, and will satisfy ``(p*q)[x] == p[q[x]]``.

For permutations of five and fewer objects, composition is extremely
fast because it uses hard-coded lookup tables. However, for Perm<7>
these tables would grow too large, and so instead this routine
involves significant computational overhead.

If you are going to make significant use of the Perm<7> class, you
should instead:

* call precompute() to precompute a full 5040-by-5040 lookup table in
  advance (this will consume roughly 50MB of memory); and then

* call cachedComp() instead of the * operator to compute your
  compositions.

Parameter ``q``:
    the permutation to compose this with.

Returns:
    the composition of both permutations.)doc";

// Docstring regina::python::doc::Perm_::__ne
static const char *__ne =
R"doc(Determines if this differs from the given permutation. This is true if
and only if the two permutations have different images for at least
one of 0, 1, 2, 3, 4, 5 or 6.

Parameter ``other``:
    the permutation with which to compare this.

Returns:
    ``True`` if and only if this and the given permutation differ.)doc";

// Docstring regina::python::doc::Perm_::cachedComp
static const char *cachedComp =
R"doc(Returns the composition of this and the given permutation, using fast
precomputed lookup tables.

The advantage of this routine is speed: calling cachedComp() is a
single table lookup, whereas the * operator requires significant
computational overhead.

The disadvantages of this routine are that (1) you must remember to
call precompute() in advance, and (2) the resulting lookup table will
consume roughly 50MB of memory for the lifetime of your program.

The permutation that is returned is the same as you would obtain by
calling ``(*this) * q``.

Precondition:
    You _must_ have called the routine precompute() at least once in
    the lifetime of this program before using cachedComp(). Otherwise
    this routine will almost certainly crash your program.

Parameter ``q``:
    the permutation to compose this with.

Returns:
    the composition of both permutations.)doc";

// Docstring regina::python::doc::Perm_::cachedComp_2
static const char *cachedComp_2 =
R"doc(Returns the composition of this and the given two permutations, using
fast precomputed lookup tables.

The advantage of this routine is speed: calling cachedComp() with two
arguments requires just two table lookups, whereas using the *
operator twice involves significant computational overhead.

The disadvantages of this routine are that (1) you must remember to
call precompute() in advance, and (2) the resulting lookup tables will
consume roughly 50MB of memory for the lifetime of your program.

The permutation that is returned is the same as you would obtain by
calling ``(*this) * q * r``.

Precondition:
    You _must_ have called the routine precompute() at least once in
    the lifetime of this program before using cachedComp(). Otherwise
    this routine will almost certainly crash your program.

Parameter ``q``:
    the first permutation to compose this with.

Parameter ``r``:
    the second permutation to compose this with.

Returns:
    the composition of both permutations.)doc";

// Docstring regina::python::doc::Perm_::cachedOrder
static const char *cachedOrder =
R"doc(Returns the order of this permutation, using fast precomputed lookup
tables.

In other words; this routine returns the smallest positive integer *k*
for which the *k*th power of this permutation is the identity.

The advantage of this routine is speed: calling cachedOrder() removes
most of the computational overhead required by order().

The disadvantages of this routine are that (1) you must remember to
call precompute() in advance, and (2) the resulting lookup tables will
consume roughly 50MB of memory for the lifetime of your program. Note
that these are the same lookup tables used by cachedComp() and
cachedPow(), so if you are already using cachedComp() or cachedPow()
then there is no extra cost for using this routine also.

The permutation that is returned is the same as you would obtain by
calling order().

Precondition:
    You _must_ have called the routine precompute() at least once in
    the lifetime of this program before using cachedOrder(). Otherwise
    this routine will almost certainly crash your program.

Returns:
    the order of this permutation.)doc";

// Docstring regina::python::doc::Perm_::cachedPow
static const char *cachedPow =
R"doc(Computes the given power of this permutation, using fast precomputed
lookup tables.

This routine runs in constant time.

The advantage of this routine is speed: calling cachedPow() removes
most of the significant computational overhead required by pow().

The disadvantages of this routine are that (1) you must remember to
call precompute() in advance, and (2) the resulting lookup tables will
consume roughly 50MB of memory for the lifetime of your program. Note
that these are the same lookup tables used by cachedComp() and
cachedOrder(), so if you are already using cachedComp() or
cachedOrder() then there is no extra cost for using this routine also.

The permutation that is returned is the same as you would obtain by
calling pow(exp).

Precondition:
    You _must_ have called the routine precompute() at least once in
    the lifetime of this program before using cachedPow(). Otherwise
    this routine will almost certainly crash your program.

Parameter ``exp``:
    the exponent; this may be positive, zero or negative.

Returns:
    this permutation raised to the power of *exp*.)doc";

// Docstring regina::python::doc::Perm_::clear
static const char *clear =
R"doc(Resets the images of all integers from *from* onwards to the identity
map.

Specifically, for each *i* in the range *from*,...,6, this routine
will ensure that ``image[i] == i``. The images of 0,1,...,*from*-1
will not be altered.

Precondition:
    The images of *from*,...,6 are exactly *from*,...,6, but possibly
    in a different order.

Parameter ``from``:
    the first integer whose image should be reset. This must be
    between 0 and 7 inclusive.)doc";

// Docstring regina::python::doc::Perm_::compareWith
static const char *compareWith =
R"doc(Lexicographically compares the images of (0,1,2,3,4,5,6) under this
and the given permutation.

Note that this does _not_ yield the same ordering of permutations as
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
R"doc(Restricts a *k*-element permutation to a 7-element permutation, where
*k* > 7.

The resulting permutation will map 0,...,6 to their respective images
under *p*, and will ignore the "unused" images *p*[7],...,*p*[*k*-1].

Precondition:
    The given permutation maps 0,...,6 to 0,...,6 in some order.

Template parameter ``k``:
    the number of elements for the input permutation; this must be
    strictly greater than 7.

Parameter ``p``:
    a permutation on *k* elements.

Returns:
    the same permutation restricted to a permutation on 7 elements.)doc";

// Docstring regina::python::doc::Perm_::extend
static const char *extend =
R"doc(Extends a *k*-element permutation to a 7-element permutation, where 2
≤ *k* < 7.

The resulting permutation will map 0,...,*k*-1 to their respective
images under *p*, and will map the "unused" elements *k*,...,6 to
themselves.

Template parameter ``k``:
    the number of elements for the input permutation; this must be 2,
    3, 4, 5 or 6.

Parameter ``p``:
    a permutation on *k* elements.

Returns:
    the same permutation expressed as a permutation on seven elements.)doc";

// Docstring regina::python::doc::Perm_::fromImagePack
static const char *fromImagePack =
R"doc(Creates a permutation from the given image pack.

See the class notes for more information on image packs, and how they
are used to build the old first-generation permutation codes.

For Perm<7>, this routine is identical to fromPermCode1().

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
    This routine will incur additional overhead, since Perm<7> now
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
internally by the Perm<7> class.

Precondition:
    the given code is a valid second-generation permutation code; see
    isPermCode2() for details.

Parameter ``code``:
    the second-generation code for the new permutation.

Returns:
    the permutation represented by the given code.)doc";

// Docstring regina::python::doc::Perm_::imagePack
static const char *imagePack =
R"doc(Returns the image pack that represents this permutation.

See the class notes for more information on image packs, and how they
are used to build the old first-generation permutation codes.

For Perm<7>, this routine is identical to permCode1().

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
permutation has the smallest index in the array Perm<7>::Sn.

See Sn for further information on how permutations are indexed.

This routine is extremely fast for Perm<7>, since it essentially uses
a hard-coded lookup table.

Returns:
    ``True`` if and only if this permutation is minimal in its
    conjugacy class.)doc";

// Docstring regina::python::doc::Perm_::isIdentity
static const char *isIdentity =
R"doc(Determines if this is the identity permutation. This is true if and
only if each of 0, 1, 2, 3, 4, 5 and 6 is mapped to itself.

Returns:
    ``True`` if and only if this is the identity permutation.)doc";

// Docstring regina::python::doc::Perm_::isImagePack
static const char *isImagePack =
R"doc(Determines whether the given argument is the image pack of some
7-element permutation.

See the class notes for more information on image packs, and how they
are used to build the old first-generation permutation codes.

For Perm<7>, this routine is identical to isPermCode1().

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
    This routine will incur additional overhead, since Perm<7> now
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
internally by the Perm<7> class.

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

Unlike the smaller permutation classes, Perm<7>::order() does not use
precomputed tables; instead it computes orders on the fly. If you need
your order computation to be faster, you can instead:

* call precompute() to precompute a full table of orders in advance
  (though this will also compute the much larger 5040-by-5040 table of
  products, which consumes roughly 50MB of memory); and then

* call cachedOrder() instead of order(), which will now become a fast
  table lookup.

Returns:
    the order of this permutation.)doc";

// Docstring regina::python::doc::Perm_::orderedS7Index
static const char *orderedS7Index =
R"doc(Returns the lexicographical index of this permutation. This will be
the index of this permutation in the Perm<7>::orderedSn array.

This is a dimension-specific alias for orderedSnIndex(). In general,
for every *n* there will be a member function
Perm<n>::orderedSnIndex(); however, these numerical aliases
Perm<2>::orderedS2Index(), ..., Perm<7>::orderedS7Index() are only
available for small *n*.

See orderedSn for further information on lexicographical ordering.

Returns:
    the lexicographical index of this permutation. This will be
    between 0 and 5039 inclusive.)doc";

// Docstring regina::python::doc::Perm_::orderedSnIndex
static const char *orderedSnIndex =
R"doc(Returns the lexicographical index of this permutation. This will be
the index of this permutation in the Perm<7>::orderedSn array.

See orderedSn for further information on lexicographical ordering.

Returns:
    the lexicographical index of this permutation. This will be
    between 0 and 5039 inclusive.)doc";

// Docstring regina::python::doc::Perm_::permCode1
static const char *permCode1 =
R"doc(Returns the first-generation code representing this permutation. This
code is sufficient to reproduce the entire permutation.

The code returned will be a valid first-generation permutation code as
determined by isPermCode1().

.. warning::
    This routine will incur additional overhead, since Perm<7> now
    uses second-generation codes internally. See the class notes and
    the routine isPermCode2() for details.

Returns:
    the first-generation permutation code.)doc";

// Docstring regina::python::doc::Perm_::permCode2
static const char *permCode2 =
R"doc(Returns the second-generation code representing this permutation. This
code is sufficient to reproduce the entire permutation.

The code returned will be a valid second-generation permutation code
as determined by isPermCode2().

Second-generation codes are fast to work with, since they are used
internally by the Perm<7> class.

Returns:
    the second-generation permutation code.)doc";

// Docstring regina::python::doc::Perm_::pow
static const char *pow =
R"doc(Computes the given power of this permutation.

This routine runs in constant time.

For Perm<7>, this routine makes use of the composition operator *,
which involves significant computational overhead. If you need your
powers to be fast, you can instead:

* call precompute() to precompute a full 5040-by-5040 product table in
  advance (this will consume roughly 50MB of memory); and then

* call cachedPow() instead of pow() to make full use of this table,
  which will remove most of the overhead from this routine.

Parameter ``exp``:
    the exponent; this may be positive, zero or negative.

Returns:
    this permutation raised to the power of *exp*.)doc";

// Docstring regina::python::doc::Perm_::pre
static const char *pre =
R"doc(Determines the preimage of the given integer under this permutation.

Parameter ``image``:
    the integer whose preimage we wish to find. This should be between
    0 and 6 inclusive.

Returns:
    the preimage of *image*.)doc";

// Docstring regina::python::doc::Perm_::precompute
static const char *precompute =
R"doc(Performs the precomputation necessary for using the optimised
cachedComp(), cachedPow() and cachedOrder() routines.

This _must_ be called before calling any of cachedComp(), cachedPow()
or cachedOrder().

This only needs to be done once in the lifetime of the program. If you
do try to call precompute() a second time then it will do nothing and
return immediately.

This routine is thread-safe.)doc";

// Docstring regina::python::doc::Perm_::rand
static const char *rand =
R"doc(Returns a random permutation on seven elements. All permutations are
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

Here _reverse_ means that we reverse the images of 0,...,6. In other
words, if permutation *q* is the reverse of *p*, then ``p[i] == q[6 -
i]`` for all *i*.)doc";

// Docstring regina::python::doc::Perm_::rot
static const char *rot =
R"doc(Returns the *i*th rotation. This maps *k* to *k* + *i* (mod 7) for all
*k*.

Parameter ``i``:
    the image of 0; this must be between 0 and 6 inclusive.

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
    This routine will incur additional overhead, since Perm<7> now
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
internally by the Perm<7> class.

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
representation will consist of seven adjacent digits representing the
images of 0, 1, 2, 3, 4, 5 and 6 respectively. An example of a string
representation is ``3045261``.

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
    The given string is not a tight encoding of a 7-element
    permutation.

Parameter ``enc``:
    the tight encoding for a 7-element permutation.

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
    the length of the prefix required; this must be between 0 and 7
    inclusive.

Returns:
    the corresponding prefix of the string representation of this
    permutation.)doc";

}

namespace Perm_::OrderedS7Lookup_ {

// Docstring regina::python::doc::Perm_::OrderedS7Lookup_::__array
static const char *__array =
R"doc(Returns the permutation at the given index in the array orderedS7. See
Perm<7>::orderedS7 for details.

This operation is extremely fast (and constant time).

Parameter ``index``:
    an index between 0 and 5039 inclusive.

Returns:
    the corresponding permutation in orderedS7.)doc";

// Docstring regina::python::doc::Perm_::OrderedS7Lookup_::size
static const char *size =
R"doc(Returns the number of permutations in the array orderedS7.

Returns:
    the size of this array.)doc";

}

namespace Perm_::S7Lookup_ {

// Docstring regina::python::doc::Perm_::S7Lookup_::__array
static const char *__array =
R"doc(Returns the permutation at the given index in the array S7. See
Perm<7>::S7 for details.

This operation is extremely fast (and constant time).

Parameter ``index``:
    an index between 0 and 5039 inclusive.

Returns:
    the corresponding permutation in S7.)doc";

// Docstring regina::python::doc::Perm_::S7Lookup_::size
static const char *size =
R"doc(Returns the number of permutations in the array S7.

Returns:
    the size of this array.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

