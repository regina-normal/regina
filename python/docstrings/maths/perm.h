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
R"doc(Represents a permutation of {0,1,...,*n*-1}. Amongst other things,
such permutations are used to describe simplex gluings in
(*n*-1)-manifold triangulations.

Perm objects are small enough to pass by value and swap with
std::swap(), with no need to use references, specialised move
operations or custom swap functions. The trade-off is that, for this
to be possible, the Perm template class can only work with *n* ≤ 16.

Each permutation has an internal code, which is a single native
integer that is sufficient to reconstruct the entire permutation. Thus
the internal code may be a useful means for passing permutation
objects to and from the engine. These codes are constructed as
follows:

* For 8 ≤ *n* ≤ 16, the code is an _image pack_: essentially a packed
  array that holds the images of 0,...,*n*-1 in a single native
  integer type. More precisely, this is an unsigned integer of type
  *ImagePack*, whose lowest *imageBits* bits represent the image of 0,
  whose next lowest *imageBits* bits represent the image of 1, and so
  on. This scheme is consistent with the old first-generation codes
  for *n* = 4,...,7, which are still supported but no longer used
  internally.

* For *n* ≤ 7, the code is an index into a hard-coded list of all
  possible permutations; more precisely, an index into the symmetric
  group Perm<n>::Sn. The ordering of Perm<n>::Sn is "almost
  lexicographic", in that we swap some pairs of indices (2*k*, 2*k*+1)
  to ensure that the even permutations are precisely those with even
  indices.

For *n* = 2,...,5 (which appear throughout 2-, 3- and 4-manifold
triangulations), this template is specialised: the code is highly
optimised and also offers some extra functionality. For *n* = 6,7,
this template is again specialised and highly optimised, and it offers
some extra functionality but not as much as Perm<5> and below. For *n*
≥ 8, this template is generic and most operations require more time
(in particular, there are no harded-coded lookup tables).

Python:
    Python does not support templates. For each *n* = 2,...,16, this
    class is available in Python under the corresponding name Perm2,
    Perm3, ..., Perm16.

Template parameter ``n``:
    the number of objects being permuted. This must be between 2 and
    16 inclusive.)doc";

// Docstring regina::python::doc::PermClass
static const char *PermClass =
R"doc(Represents a conjugacy class of permutations on *n* elements.

Each such conjugacy class correpsonds to an unordered partition of
*n*, describing the cycle structure of the permutations in the
conjugacy class.

There is a special "past-the-end" conjugacy class, which effectively
holds no value; the only way you can obtain such a class is to iterate
through *all* classes using the increment operators. You can test for
a past-the-end value by casting to ``bool``.

PermClass objects are, in their current implementation, entirely
stack-based. This means they cannot support fast move or swap
operations. However, their size is linear in *n*, so while copy
operations are not enormously expensive, they are not as cheap as
copying a Perm object (which just holds a single machine-native
integer). This decision is a deliberate trade-off between speed versus
space; the implication for end users is that you should be economical
about copying PermClass objects, and work with them in-place where
possible.

Python:
    Python does not support templates. For each *n* = 2,...,16, this
    class is available in Python under the corresponding name
    PermClass2, PermClass3, ..., PermClass16.

Template parameter ``n``:
    the number of objects being permuted. This must be between 2 and
    16 inclusive.)doc";

// Docstring regina::python::doc::PermCodeType
static const char *PermCodeType =
R"doc(Represents the different kinds of internal permutation codes that are
used in Regina's various Perm<n> template classes. See the Perm<n>
class notes for more information on exactly how these codes are
constructed. The class constant Perm<n>::codeType indicates which type
of code is used for which *n*.)doc";

// Docstring regina::python::doc::digit
static const char *digit =
R"doc(Returns the character used to express the integer *i* in a
permutation.

* For *i* = 0,...,9, this will be the usual digit representing *i*.

* For *i* ≥ 10, this will be a lower-case letter. In particular, for
  *i* = 10,...,15, this will be the usual hexadecimal digit
  representing *i*.

* At present, this routine only supports integers *i* < 36.

Parameter ``i``:
    the integer to represent; this must be between 0 and 35 inclusive.

Returns:
    the single character used to represent *i*.)doc";

// Docstring regina::python::doc::factorial
static const char *factorial =
R"doc(Returns the factorial of *n*.

Parameter ``n``:
    any non-negative integer; this must be at most 20 (since otherwise
    the factorial will overflow).

Returns:
    the factorial of *n*..)doc";

namespace PermClass_ {

// Docstring regina::python::doc::PermClass_::__as_bool
static const char *__as_bool =
R"doc(Determines whether this represents a genuine conjugacy class, or
whether it represents the special past-the-end value.

See the class notes and the increment operators for further discussion
on what past-the-end represents.

Returns:
    ``False`` if this is the past-the-end conjugacy class, or ``True``
    otherwise.)doc";

// Docstring regina::python::doc::PermClass_::__copy
static const char *__copy =
R"doc(Creates a copy of the given conjugacy class.

Parameter ``src``:
    the conjugacy class to clone.)doc";

// Docstring regina::python::doc::PermClass_::__default
static const char *__default =
R"doc(Creates the conjugacy class for the identity permutation.

The corresponding cycle structure (i.e., partition of *n*) is *n*
cycles, each of length 1.)doc";

// Docstring regina::python::doc::PermClass_::__eq
static const char *__eq =
R"doc(Determines whether this and the given object describe the same
conjugacy class.

Two past-the-end conjugacy classes will be treated as equal.

Parameter ``other``:
    the conjugacy class to compare with this.

Returns:
    ``True`` if and only if this and the given conjugacy class are
    identical.)doc";

// Docstring regina::python::doc::PermClass_::__inc
static const char *__inc =
R"doc(A postincrement operator that changes this to become the next
conjugacy class in a lexicographical enumeration.

Specifically, the increment operators work through conjugacy classes
in lexicographical order, where each class is represented by its
sequence of cycle lengths (given in increasing order). Thus the
conjugacy class ``11...1`` (representing the identity permutation)
will come first, and the conjugacy class ``n`` (representing a single
long cycle) will come last.

If there are no more conjugacy classes (i.e., this represents a single
cycle of length *n*), then this object will be set to the special
past-the-end value (which can be tested by casting to a ``bool``).

Python:
    This routine is named inc() since python does not support the
    increment operator.

Returns:
    a copy of this conjugacy class before the increment took place.)doc";

// Docstring regina::python::doc::PermClass_::__ne
static const char *__ne =
R"doc(Determines whether this and the given object describe different
conjugacy classes.

Two past-the-end conjugacy classes will be treated as equal.

Parameter ``other``:
    the conjugacy class to compare with this.

Returns:
    ``True`` if and only if this and the given conjugacy class are
    different.)doc";

// Docstring regina::python::doc::PermClass_::countCycles
static const char *countCycles =
R"doc(Returns the number of cycles in this conjugacy class.

Recall from the class notes that a conjugacy class identifies the
cycle structure of a permutation. This routine simply returns the
number of cycles for this conjugacy class. Fixed points are also
counted (they are considered to be cycles of length 1).

Returns:
    the number of cycles.)doc";

// Docstring regina::python::doc::PermClass_::cycle
static const char *cycle =
R"doc(Returns the length of the given cycle in this conjugacy class.

Recall from the class notes that a conjugacy class identifies the
cycle structure of a permutation. If the cycle lengths are listed in
order from shortest to longest, then this routine returns the
*which*th cycle length in this sequence. Fixed points are included in
this sequence (they are considered to be cycles of length 1, and will
appear at the beginning of the sequence).

For example, for a conjugacy class in ``PermClass<9>`` with cycle
lengths ``(1,1,3,4)``, calling ``cycle(2)`` will return 3.

Parameter ``which``:
    indicates which cycle length to return; this must be between 0 and
    countCycles()-1 inclusive.

Returns:
    the requested cycle length.)doc";

// Docstring regina::python::doc::PermClass_::isIdentity
static const char *isIdentity =
R"doc(Determines whether this is the conjugacy class for the identity
permutation.

Returns:
    ``True`` if and only if this conjugacy class represents the
    identity permutation.)doc";

// Docstring regina::python::doc::PermClass_::rep
static const char *rep =
R"doc(Returns the minimal representative of this conjugacy class.

The _minimal representative_ is the permutation belonging to this
class with the smallest index in Perm<n>::Sn.

Because all permutations in the same class must have the same sign, it
follows that this is also the permutation in this class with the
smallest index in Perm<n>::orderedSn.

Precondition:
    This is not the past-the-end conjugacy class.

Returns:
    the minimal representative of this conjugacy class.)doc";

// Docstring regina::python::doc::PermClass_::str
static const char *str =
R"doc(Returns a string representation of this conjugacy class.

At present, the string representation consists of a sequence of digits
indicating the cycle lengths (where cycle lengths 10,11,...,16 use
``a,b,...,g`` for their respective digits).

If this is the past-the-end conjugacy class, a suitable string will be
returned.

Returns:
    a string representation of this conjugacy class.)doc";

}

namespace PermCodeType_ {

// Docstring regina::python::doc::PermCodeType_::PERM_CODE_IMAGES
static const char *PERM_CODE_IMAGES =
R"doc(This is a permutation code that packs the images of 0,...,*n*-1 into a
single native integer using a handful of bits per image. Such codes
are easier to manipulate on an element-by-element basis.

Codes of this type can always be queried using Perm<n>::permCode(),
and permutations can be recreated from them using
Perm<n>::fromPermCode().)doc";

// Docstring regina::python::doc::PermCodeType_::PERM_CODE_INDEX
static const char *PERM_CODE_INDEX =
R"doc(This is a permutation code that stores the index into the full
permutation group *S_n*. Such codes typically require fewer bytes and
are packed together, making them ideal for working with lookup tables.

Codes of this type can be queried using Perm<n>::SnIndex(), and
permutations can be recreated from them by indexing into Perm<n>::Sn.

.. warning::
    The routines Perm<n>::permCode() and Perm<n>::fromPermCode() will
    still be present, but in some classes (e.g., Perm<4> and Perm<5>),
    these are legacy routines that refer to different types of codes.)doc";

}

namespace Perm_ {

// Docstring regina::python::doc::Perm_::OrderedSnLookup
static const char *OrderedSnLookup = R"doc(A lightweight array-like object used to implement Perm<n>::orderedSn.)doc";

// Docstring regina::python::doc::Perm_::SnIndex
static const char *SnIndex =
R"doc(Returns the index of this permutation in the Perm<n>::Sn array.

See Sn for further information on how these permutations are indexed.

Returns:
    the index *i* for which this permutation is equal to
    Perm<n>::Sn[i]. This will be between 0 and *n*!-1 inclusive.)doc";

// Docstring regina::python::doc::Perm_::SnLookup
static const char *SnLookup = R"doc(A lightweight array-like object used to implement Perm<n>::Sn.)doc";

// Docstring regina::python::doc::Perm_::__array
static const char *__array =
R"doc(Determines the image of the given integer under this permutation.

Parameter ``source``:
    the integer whose image we wish to find. This should be between 0
    and *n*-1 inclusive.

Returns:
    the image of *source*.)doc";

// Docstring regina::python::doc::Perm_::__copy
static const char *__copy = R"doc(Creates a permutation that is a clone of the given permutation.)doc";

// Docstring regina::python::doc::Perm_::__default
static const char *__default = R"doc(Creates the identity permutation.)doc";

// Docstring regina::python::doc::Perm_::__eq
static const char *__eq =
R"doc(Determines if this is equal to the given permutation. This is true if
and only if both permutations have the same images for all 0 ≤ *i* <
*n*.

Parameter ``other``:
    the permutation with which to compare this.

Returns:
    ``True`` if and only if this and the given permutation are equal.)doc";

// Docstring regina::python::doc::Perm_::__inc
static const char *__inc =
R"doc(A postincrement operator that changes this to be the next permutation
in the array Perm<n>::Sn. If this is the last such permutation then
this will wrap around to become the first permutation in Perm<n>::Sn,
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
    0 ≤ *a*,*b* < *n*.

Parameter ``a``:
    the element to switch with *b*.

Parameter ``b``:
    the element to switch with *a*.)doc";

// Docstring regina::python::doc::Perm_::__init_2
static const char *__init_2 =
R"doc(Creates a permutation mapping *i* to *image*[*i*] for each 0 ≤ *i* <
*n*.

Precondition:
    The elements of *image* are 0,...,*n*-1 in some order.

Parameter ``image``:
    the array of images.)doc";

// Docstring regina::python::doc::Perm_::__lt
static const char *__lt =
R"doc(Determines if this appears earlier than the given permutation in the
array Perm<n>::Sn.

Note that this is _not_ the same ordering of permutations as the
ordering implied by compareWith(). This is, however, consistent with
the ordering implied by the ++ operators.

Unlike the smaller permutation classes that use *Sn* indices as
internal permutation codes, for this generic Perm class the ordering
defined here is _slower_ to compute than compareWith(). It is
recommended that, unless you specifically need to align your ordering
with *Sn* indices, you either (i) use compareWith() for
lexicographical ordering (which is a little faster), or else (ii) just
compare permutation codes if you are happy with an arbitrary ordering
(which will be _much_ faster).

Parameter ``rhs``:
    the permutation to compare this against.

Returns:
    ``True`` if and only if this appears before *rhs* in *Sn*.)doc";

// Docstring regina::python::doc::Perm_::__mul
static const char *__mul =
R"doc(Returns the composition of this permutation with the given
permutation. If this permutation is *p*, the resulting permutation
will be *p*∘*q*, and will satisfy ``(p*q)[x] == p[q[x]]``.

Parameter ``q``:
    the permutation to compose this with.

Returns:
    the composition of both permutations.)doc";

// Docstring regina::python::doc::Perm_::__ne
static const char *__ne =
R"doc(Determines if this differs from the given permutation. This is true if
and only if the two permutations have different images for some 0 ≤
*i* < *n*.

Parameter ``other``:
    the permutation with which to compare this.

Returns:
    ``True`` if and only if this and the given permutation differ.)doc";

// Docstring regina::python::doc::Perm_::cachedComp
static const char *cachedComp =
R"doc(An alias for the composition operator, provided to assist with writing
generic code.

This generic Perm<n> class does not use precomputation to compute
compositions. The only point of having cachedComp() in this generic
Perm<n> class is to make it easier to write generic code that works
with Perm<n> for any *n*.

* If you know you are only working with the generic Perm<n>, you
  should just use the composition operator instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedComp().

The permutation that is returned is the same as you would obtain by
calling ``(*this) * q``.

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedComp(). For generic Perm<n>,
    precompute() does not affect compositions; however, for other
    Perm<n> classes a failure to do this will almost certainly crash
    your program.

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
    of this program before calling cachedComp(). For generic Perm<n>,
    precompute() does not affect compositions; however, for other
    Perm<n> classes a failure to do this will almost certainly crash
    your program.

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

This generic Perm<n> class does not use precomputation to compute
conjugates. The only point of having cachedConjugate() in this generic
Perm<n> class is to make it easier to write generic code that works
with Perm<n> for any *n*.

* If you know you are only working with the generic Perm<n>, you
  should just call conjugate() instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedConjugate().

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedConjugate(). For generic
    Perm<n>, precompute() does not affect conjugate computations;
    however, for other Perm<n> classes a failure to do this will
    almost certainly crash your program.

Parameter ``q``:
    the permutation to conjugate this by.

Returns:
    the conjugate of this permutation by *q*.)doc";

// Docstring regina::python::doc::Perm_::cachedInverse
static const char *cachedInverse =
R"doc(Finds the inverse of this permutation, optimised using precomputed
"partial lookup tables".

The advantage of this routine is speed: calling cachedInverse()
involves two table lookups and some simple arithmetic to combine the
results, whereas inverse() requires time linear in *n*.

The disadvantages of this routine are that (1) you must remember to
call precompute() in advance, and (2) the precomputed lookup tables
will consume additional memory for the lifetime of your program. See
precompute() for details on just how much memory these tables will
consume for each *n*.

The permutation that is returned is the same as you would obtain by
calling inverse().

Precondition:
    You _must_ have called the routine precompute() at least once in
    the lifetime of the program before using cachedInverse().
    Otherwise this routine will almost certainly crash your program.

Returns:
    the inverse of this permutation.)doc";

// Docstring regina::python::doc::Perm_::cachedOrder
static const char *cachedOrder =
R"doc(An alias for order(), provided to assist with writing generic code.

This specialised Perm<n> class does not use precomputation to compute
orders. The only point of having cachedOrder() in this generic Perm<n>
class is to make it easier to write generic code that works with
Perm<n> for any *n*.

* If you know you are only working with the generic Perm<n>, you
  should just call order() instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedOrder().

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedOrder(). For generic Perm<n>,
    precompute() does not affect order computations; however, for
    other Perm<n> classes a failure to do this will almost certainly
    crash your program.

Returns:
    the order of this permutation.)doc";

// Docstring regina::python::doc::Perm_::cachedPow
static const char *cachedPow =
R"doc(An alias for pow(), provided to assist with writing generic code.

This specialised Perm<n> class does not use precomputation to compute
powers. The only point of having cachedPow() in this generic Perm<n>
class is to make it easier to write generic code that works with
Perm<n> for any *n*.

* If you know you are only working with the generic Perm<n>, you
  should just call pow() instead.

* If you are writing generic code, you _must_ remember to call
  precompute() at least once in the lifetime of this program before
  using cachedPow().

Precondition:
    You _must_ have called precompute() at least once in the lifetime
    of this program before calling cachedPow(). For generic Perm<n>,
    precompute() does not affect powers; however, for other Perm<n>
    classes a failure to do this will almost certainly crash your
    program.

Parameter ``exp``:
    the exponent; this may be positive, zero or negative.

Returns:
    this permutation raised to the power of *exp*.)doc";

// Docstring regina::python::doc::Perm_::clear
static const char *clear =
R"doc(Resets the images of all integers from *from* onwards to the identity
map.

Specifically, for each *i* in the range *from*,...,*n*-1, this routine
will ensure that ``image[i] == i``. The images of 0,1,...,*from*-1
will not be altered.

Precondition:
    The images of *from*,...,*n*-1 are exactly *from*,...,*n*-1, but
    possibly in a different order.

Parameter ``from``:
    the first integer whose image should be reset. This must be
    between 0 and *n* inclusive.)doc";

// Docstring regina::python::doc::Perm_::compareWith
static const char *compareWith =
R"doc(Lexicographically compares the images of (0,1,...,*n*-1) under this
and the given permutation.

Parameter ``other``:
    the permutation with which to compare this.

Returns:
    -1 if this permutation produces a smaller image, 0 if the
    permutations are equal, and 1 if this permutation produces a
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
R"doc(Restricts a *k*-element permutation to an *n*-element permutation,
where *k* > *n*.

The resulting permutation will map 0,...,*n*-1 to their respective
images under *p*, and will ignore the "unused" images
*p*[*n*],...,*p*[*k*-1].

Precondition:
    The given permutation maps 0,...,*n*-1 to 0,...,*n*-1 in some
    order.

Template parameter ``k``:
    the number of elements for the input permutation; this must be
    strictly greater than *n*.

Parameter ``p``:
    a permutation on *k* elements.

Returns:
    the same permutation restricted to a permutation on *n* elements.)doc";

// Docstring regina::python::doc::Perm_::extend
static const char *extend =
R"doc(Extends a *k*-element permutation to an *n*-element permutation, where
2 ≤ *k* < *n*.

The resulting permutation will map 0,...,*k*-1 to their respective
images under *p*, and will map the "unused" elements *k*,...,*n*-1 to
themselves.

Template parameter ``k``:
    the number of elements for the input permutation; this must be at
    least 2, and strictly less than *n*.

Parameter ``p``:
    a permutation on *k* elements.

Returns:
    the same permutation expressed as a permutation on *n* elements.)doc";

// Docstring regina::python::doc::Perm_::fromImagePack
static const char *fromImagePack =
R"doc(Creates a permutation from the given image pack.

See the class notes for more information on image packs, and how they
are used to build permutation codes.

For *n* ≥ 8, this routine is identical to fromPermCode().

Precondition:
    The argument *pack* is a valid image pack; see isImagePack() for
    details.

Parameter ``pack``:
    an image pack that describes a permutation.

Returns:
    the permutation represented by the given image pack.)doc";

// Docstring regina::python::doc::Perm_::fromPermCode
static const char *fromPermCode =
R"doc(Creates a permutation from the given internal code.

Precondition:
    the given code is a valid permutation code; see isPermCode() for
    details.

Parameter ``code``:
    the internal code for the new permutation.

Returns:
    the permutation reprsented by the given internal code.)doc";

// Docstring regina::python::doc::Perm_::imagePack
static const char *imagePack =
R"doc(Returns the image pack that represents this permutation.

See the class notes for more information on image packs, and how they
are used to build permutation codes.

For *n* ≥ 8, this routine is identical to permCode().

Returns:
    the image pack for this permutation.)doc";

// Docstring regina::python::doc::Perm_::inverse
static const char *inverse =
R"doc(Finds the inverse of this permutation.

For permutations of seven and fewer objects, inversion is extremely
fast because it uses hard-coded lookup tables. However, for this
generic Perm<n> class, inversion cannot use these lookup tables (for
multiple reasons), and so inverse() takes time linear in *n*.

If you are going to make significant use of the generic Perm<n> class
for some particular value of *n*, you should instead:

* call precompute() to precompute some "partial lookup tables" in
  advance (see precompute() for details on how much memory they will
  consume); and then

* call cachedInverse() instead of inverse() to compute your inverses.

Returns:
    the inverse of this permutation.)doc";

// Docstring regina::python::doc::Perm_::isConjugacyMinimal
static const char *isConjugacyMinimal =
R"doc(Is this permutation minimal in its conjugacy class?

Here "minimal" means that, amongst all its conjugates, this
permutation has the smallest index in the array Perm<n>::Sn.

See Sn for further information on how permutations are indexed.

Returns:
    ``True`` if and only if this permutation is minimal in its
    conjugacy class.)doc";

// Docstring regina::python::doc::Perm_::isIdentity
static const char *isIdentity =
R"doc(Determines if this is the identity permutation. This is true if and
only if every integer 0 ≤ *i* < *n* is mapped to itself.

Returns:
    ``True`` if and only if this is the identity permutation.)doc";

// Docstring regina::python::doc::Perm_::isImagePack
static const char *isImagePack =
R"doc(Determines whether the given argument is the image pack of some
*n*-element permutation.

See the class notes for more information on image packs, and how they
are used to build permutation codes.

For *n* ≥ 8, this routine is identical to isPermCode().

Parameter ``pack``:
    the candidate image pack to test.

Returns:
    ``True`` if and only if *pack* is a valid image pack.)doc";

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

Note that the largest possible order for the largest supported *n*
(*n* = 16) is 140. See OEIS sequence A000793 for details.

Returns:
    the order of this permutation.)doc";

// Docstring regina::python::doc::Perm_::orderedSnIndex
static const char *orderedSnIndex =
R"doc(Returns the lexicographical index of this permutation. This will be
the index of this permutation in the Perm<n>::orderedSn array.

See orderedSn for further information on lexicographical ordering.

Returns:
    the lexicographical index of this permutation. This will be
    between 0 and *n*!-1 inclusive.)doc";

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

This routine runs in time linear in *n* (in particular, the running
time does not depend upon the given exponent).

Parameter ``exp``:
    the exponent; this may be positive, zero or negative.

Returns:
    this permutation raised to the power of *exp*.)doc";

// Docstring regina::python::doc::Perm_::pre
static const char *pre =
R"doc(Determines the preimage of the given integer under this permutation.

Parameter ``image``:
    the integer whose preimage we wish to find. This should be between
    0 and *n*-1 inclusive.

Returns:
    the preimage of *image*.)doc";

// Docstring regina::python::doc::Perm_::precompute
static const char *precompute =
R"doc(Performs the precomputation necessary for using the optimised
cachedInverse() routine.

This _must_ be called before calling cachedInverse().

This only needs to be done once in the lifetime of the program. If you
do try to call precompute() a second time then it will do nothing and
return immediately.

The precomputed tables will consume roughly:

* 33 kB for *n* = 8;

* 8.9 MB for *n* = 9;

* 17 MB for *n* = 10;

* 143 MB for *n* = 11;

* 268 MB for *n* = 12;

* 2.3 GB for *n* = 13;

* 4.3 GB for *n* = 14;

* 37 GB for *n* = 15;

* 69 GB for *n* = 16.

In particular, a 32-bit machine will not be able to store these tables
for *n* ≥ 13, a 24-bit machine will not be these tables for *n* ≥ 9,
and a 16-bit machine will not be able to store these tables for any
*n* ≥ 8.

Precondition:
    There is enough memory available to store the precomputed tables;
    see above for the estimated space requirements.

Exception ``FailedPrecondition``:
    There was not enough memory to available to store the precomputed
    tables.

This routine is thread-safe.)doc";

// Docstring regina::python::doc::Perm_::rand
static const char *rand =
R"doc(Returns a random permutation on *n* elements. All permutations are
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

Here _reverse_ means that we reverse the images of 0,...,*n*-1. In
other words, if permutation *q* is the reverse of *p*, then ``p[i] ==
q[n - 1 - i]`` for all *i*.)doc";

// Docstring regina::python::doc::Perm_::rot
static const char *rot =
R"doc(Returns the *i*th rotation. This maps *k* to *k* + *i* (mod *n*) for
all *k*.

Parameter ``i``:
    the image of 0; this must be between 0 and *n*-1 inclusive.

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
representation will consist of *n* adjacent digits representing the
images of 0,...,*n*-1 respectively. If *n* > 10, then lower-case
hexadecimal digits will be used.

An example of a string representation for *n* = 5 is ``30421``.

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
    The given string is not a tight encoding of an *n*-element
    permutation.

Parameter ``enc``:
    the tight encoding for an *n*-element permutation.

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
    the length of the prefix required; this must be between 0 and *n*
    inclusive.

Returns:
    the corresponding prefix of the string representation of this
    permutation.)doc";

}

namespace Perm_::OrderedSnLookup_ {

// Docstring regina::python::doc::Perm_::OrderedSnLookup_::__array
static const char *__array =
R"doc(Returns the permutation at the given index in the array orderedSn. See
Perm<n>::orderedSn for details.

For *n* ≤ 7, this operator is very fast (and constant time). However,
for *n* ≥ 8 it is not constant time; the current implementation is
quadratic in *n*.

Parameter ``index``:
    an index between 0 and *n*!-1 inclusive.

Returns:
    the corresponding permutation in orderedSn.)doc";

// Docstring regina::python::doc::Perm_::OrderedSnLookup_::size
static const char *size =
R"doc(Returns the number of permutations in the array orderedSn.

Returns:
    the size of this array.)doc";

}

namespace Perm_::SnLookup_ {

// Docstring regina::python::doc::Perm_::SnLookup_::__array
static const char *__array =
R"doc(Returns the permutation at the given index in the array Sn. See
Perm<n>::Sn for details.

For *n* ≤ 7, this operator is very fast (and constant time). However,
for *n* ≥ 8 it is not constant time; the current implementation is
quadratic in *n*.

Parameter ``index``:
    an index between 0 and *n*!-1 inclusive.

Returns:
    the corresponding permutation in Sn.)doc";

// Docstring regina::python::doc::Perm_::SnLookup_::size
static const char *size =
R"doc(Returns the number of permutations in the array Sn.

Returns:
    the size of this array.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

