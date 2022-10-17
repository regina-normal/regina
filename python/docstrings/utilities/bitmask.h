/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Bitmask
static const char *Bitmask =
R"doc(A bitmask that can store arbitrarily many true-or-false bits.

This bitmask packs the bits together, so that (unlike an array of
bools) many bits can be stored in a single byte. As a result,
operations on this class are fast because the CPU can work on many
bits simultaneously.

Nevertheless, this class still has overhead because the bits must be
allocated on the heap, and because every operation requires looping
through the individual bytes. For reasonably small bitmasks, see the
highly optimised Bitmask1 and Bitmask2 classes instead.

Once a bitmask is created, the only way its length (the number of
bits) can be changed is by calling reset(size_t).

The length of the bitmask is not actually stored in this structure.
This means that, upon construction (or reset), the length will be
automatically rounded up to the next "raw unit of storage".

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

.. warning::
    Because this class may increase the length of the bitmask
    (rounding up to the next unit of storage), bitwise computations
    may not give the results that you expect. In particular, flip()
    may set additional ``True`` bits in the "dead space" between the
    intended length and the actual length, and this may have a flow-on
    effect for other operations (such as subset testing, bit counting
    and so on). Be careful!)doc";

// Docstring regina::python::doc::Bitmask1
static const char *Bitmask1 =
R"doc(A small but extremely fast bitmask class that can store up to 8 *
sizeof(*T*) true-or-false bits.

This bitmask packs all of the bits together into a single variable of
type *T*. This means that operations on bitmasks are extremely fast,
because all of the bits can be processed at once.

The downside of course is that the number of bits that can be stored
is limited to 8 * sizeof(*T*), where *T* must be a native unsigned
integer type (such as unsigned char, unsigned int, or unsigned long
long).

For another extremely fast bitmask class that can store twice as many
bits, see Bitmask2. For a bitmask class that can store arbitrarily
many bits, see Bitmask.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Precondition:
    Type *T* is an unsigned integral numeric type.

Python:
    Python does not support templates, and so instead Regina's python
    interface offers the classes Bitmask8, Bitmask16, Bitmask32,
    Bitmask64, Bitmask128, and (if the machine supports 128-bit
    integers) Bitmask256. Each of these will be an optimised bitmask
    class that can hold the corresponding number of bits, and is
    guaranteed to be an instance of either the C++ Bitmask1<T> class
    (where possible) or the C++ Bitmask2<T,U> template class (if
    necessary).)doc";

// Docstring regina::python::doc::Bitmask2
static const char *Bitmask2 =
R"doc(A small but extremely fast bitmask class that can store up to 8 *
sizeof(*T*) + 8 * sizeof(*U*) true-or-false bits.

This bitmask packs all of the bits together into a single variable of
type *T* and a single variable of type *U*. This means that operations
on entire bitmasks are extremely fast, because all of the bits can be
processed in just two "native" operations.

The downside of course is that the number of bits that can be stored
is limited to 8 * sizeof(*T*) + 8 * sizeof(*U*), where *T* and *U*
must be native unsigned integer types (such as unsigned char, unsigned
int, or unsigned long long).

For an even faster bitmask class that can only store half as many
bits, see Bitmask1. For a bitmask class that can store arbitrarily
many bits, see Bitmask.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Precondition:
    Types *T* and *U* are unsigned integral numeric types.

Python:
    Python does not support templates, and so instead Regina's python
    interface offers the classes Bitmask8, Bitmask16, Bitmask32,
    Bitmask64, Bitmask128, and (if the machine supports 128-bit
    integers) Bitmask256. Each of these will be an optimised bitmask
    class that can hold the corresponding number of bits, and is
    guaranteed to be an instance of either the C++ Bitmask1<T> class
    (where possible) or the C++ Bitmask2<T,U> template class (if
    necessary).)doc";

namespace Bitmask1_ {

// Docstring regina::python::doc::Bitmask1_::__copy
static const char *__copy =
R"doc(Creates a clone of the given bitmask.

Parameter ``src``:
    the bitmask to clone.)doc";

// Docstring regina::python::doc::Bitmask1_::__default
static const char *__default = R"doc(Creates a new bitmask with all bits set to ``False``.)doc";

// Docstring regina::python::doc::Bitmask1_::__eq
static const char *__eq =
R"doc(Determines whether this and the given bitmask are identical.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this and the given bitmask are identical.)doc";

// Docstring regina::python::doc::Bitmask1_::__iand
static const char *__iand =
R"doc(Sets this to the intersection of this and the given bitmask. Every bit
that is unset in *other* will be unset in this bitmask.

Parameter ``other``:
    the bitmask to intersect with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask1_::__init
static const char *__init =
R"doc(Creates a new bitmask with all bits set to ``False``.

The integer argument is merely for compatibility with the Bitmask
constructor, and will be ignored.

.. warning::
    This is *not* a constructor that initialises the bitmask to a
    given pattern.)doc";

// Docstring regina::python::doc::Bitmask1_::__ior
static const char *__ior =
R"doc(Sets this to the union of this and the given bitmask. Every bit that
is set in *other* will be set in this bitmask.

Parameter ``other``:
    the bitmask to union with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask1_::__isub
static const char *__isub =
R"doc(Sets this to the set difference of this and the given bitmask. Every
bit that is set in *other* will be cleared in this bitmask.

Parameter ``other``:
    the bitmask to XOR with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask1_::__ixor
static const char *__ixor =
R"doc(Sets this to the exclusive disjunction (XOR) of this and the given
bitmask. Every bit that is set in *other* will be flipped in this
bitmask.

Parameter ``other``:
    the bitmask to XOR with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask1_::__le
static const char *__le =
R"doc(Determines whether this bitmask is entirely contained within the given
bitmask.

For this routine to return ``True``, every bit that is set in this
bitmask must also be set in the given bitmask.

.. warning::
    This operation does not compare bitmasks lexicographically;
    moreover, it only describes a partial order, not a total order. To
    compare bitmasks lexicographically, use lessThan() instead.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this bitmask is entirely contained within
    the given bitmask.)doc";

// Docstring regina::python::doc::Bitmask1_::__ne
static const char *__ne =
R"doc(Determines whether this and the given bitmask are different.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this and the given bitmask are different.)doc";

// Docstring regina::python::doc::Bitmask1_::atMostOneBit
static const char *atMostOneBit =
R"doc(Determines whether at most one bit is set to ``True`` in this bitmask.

If this bitmask is entirely ``False`` or if only one bit is set to
``True``, then this routine will return ``True``. Otherwise this
routine will return ``False``.

Returns:
    ``True`` if and only if at most one bit is set to ``True``.)doc";

// Docstring regina::python::doc::Bitmask1_::bits
static const char *bits =
R"doc(Returns the number of bits currently set to ``True`` in this bitmask.

Returns:
    the number of ``True`` bits.)doc";

// Docstring regina::python::doc::Bitmask1_::containsIntn
static const char *containsIntn =
R"doc(Determines whether this bitmask contains the intersection of the two
given bitmasks.

For this routine to return ``True``, every bit that is set in *both*
*x* and *y* must be set in this bitmask also.

Parameter ``x``:
    the first bitmask used to form the intersection.

Parameter ``y``:
    the first bitmask used to form the intersection.

Returns:
    ``True`` if and only if this bitmask entirely contains the
    intersection of *x* and *y*.)doc";

// Docstring regina::python::doc::Bitmask1_::firstBit
static const char *firstBit =
R"doc(Returns the index of the first ``True`` bit in this bitmask, or -1 if
there are no ``True`` bits.

Returns:
    the index of the first ``True`` bit.)doc";

// Docstring regina::python::doc::Bitmask1_::flip
static const char *flip =
R"doc(Negates every bit in this bitmask. All ``True`` bits will be set to
``False`` and vice versa.

Unlike the more generic Bitmask, this optimised bitmask class does not
store a length. This means that all 8 * sizeof(*T*) possible bits will
be negated.)doc";

// Docstring regina::python::doc::Bitmask1_::get
static const char *get =
R"doc(Returns the value of the given bit of this bitmask.

Parameter ``index``:
    indicates which bit to query; this must be between 0 and (8 *
    sizeof(*T*) - 1) inclusive.

Returns:
    the value of the (*index*)th bit.)doc";

// Docstring regina::python::doc::Bitmask1_::inUnion
static const char *inUnion =
R"doc(Determines whether this bitmask is entirely contained within the union
of the two given bitmasks.

For this routine to return ``True``, every bit that is set in this
bitmask must also be set in either *x* or *y*.

Parameter ``x``:
    the first bitmask used to form the union.

Parameter ``y``:
    the first bitmask used to form the union.

Returns:
    ``True`` if and only if this bitmask is entirely contained within
    the union of *x* and *y*.)doc";

// Docstring regina::python::doc::Bitmask1_::lastBit
static const char *lastBit =
R"doc(Returns the index of the last ``True`` bit in this bitmask, or -1 if
there are no ``True`` bits.

Returns:
    the index of the last ``True`` bit.)doc";

// Docstring regina::python::doc::Bitmask1_::lessThan
static const char *lessThan =
R"doc(Determines whether this bitmask appears strictly before the given
bitmask when bitmasks are sorted in lexicographical order. Here the
bit at index 0 is least significant, and the bit at index *length*-1
is most significant.

.. warning::
    We do not use < for this operation, since <= represents the subset
    operation.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this is lexicographically strictly smaller
    than the given bitmask.)doc";

// Docstring regina::python::doc::Bitmask1_::reset
static const char *reset = R"doc(Sets all bits of this bitmask to ``False``.)doc";

// Docstring regina::python::doc::Bitmask1_::reset_2
static const char *reset_2 =
R"doc(Sets all bits of this bitmask to ``False``.

The integer argument is merely for compatibility with
Bitmask::reset(size_t), and will be ignored.)doc";

// Docstring regina::python::doc::Bitmask1_::set
static const char *set =
R"doc(Sets the given bit of this bitmask to the given value.

Parameter ``index``:
    indicates which bit to set; this must be between 0 and (8 *
    sizeof(*T*) - 1) inclusive.

Parameter ``value``:
    the value that will be assigned to the (*index*)th bit.)doc";

// Docstring regina::python::doc::Bitmask1_::set_2
static const char *set_2 =
R"doc(Sets all bits in the given sorted list to the given value.

This is a convenience routine for setting many bits at once. The
indices of the bits to set should be sorted and stored in some
container, such as a std::set or a C-style array. This routine takes
iterators over this container, and sets the bits at the corresponding
indices to the given value.

For example, the following code would set bits 3, 5 and 6 to ``true:``

```
std::vector<unsigned> indices;
indices.push(3); indices.push(5); indices.push(6);
bitmask.set(indices.begin(), indices.end(), true);
```

Likewise, the following code would set bits 1, 4 and 7 to ``false:``

```
unsigned indices[3] = { 1, 4, 7 };
bitmask.set(indices, indices + 3, false);
```

All other bits of this bitmask are unaffected by this routine.

Precondition:
    *ForwardIterator* is a forward iterator type that iterates over
    integer values.

Precondition:
    The list of indices described by these iterators is in *sorted*
    order. This is to allow optimisations for larger bitmask types.

Precondition:
    All indices in the given list are between 0 and (8 * sizeof(*T*) -
    1) inclusive.

Python:
    Instead of a pair of iterators, you should pass a Python list
    (which, as described above, must be a sorted list of indices).

Parameter ``indexBegin``:
    the beginning of the iterator range containing the sorted indices
    of the bits to set.

Parameter ``indexEnd``:
    the end of the iterator range containing the sorted indices of the
    bits to set.

Parameter ``value``:
    the value that will be assigned to each of the corresponding bits.)doc";

// Docstring regina::python::doc::Bitmask1_::truncate
static const char *truncate =
R"doc(Leaves the first *numBits* bits of this bitmask intact, but sets all
subsequent bits to ``False``. In other words, this routine "truncates"
this bitmask to the given number of bits.

This routine does not change the *length* of this bitmask (as passed
to the contructor or to reset()).

Parameter ``numBits``:
    the number of bits that will *not* be cleared.)doc";

}

namespace Bitmask2_ {

// Docstring regina::python::doc::Bitmask2_::__copy
static const char *__copy =
R"doc(Creates a clone of the given bitmask.

Parameter ``src``:
    the bitmask to clone.)doc";

// Docstring regina::python::doc::Bitmask2_::__default
static const char *__default = R"doc(Creates a new bitmask with all bits set to ``False``.)doc";

// Docstring regina::python::doc::Bitmask2_::__eq
static const char *__eq =
R"doc(Determines whether this and the given bitmask are identical.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this and the given bitmask are identical.)doc";

// Docstring regina::python::doc::Bitmask2_::__iand
static const char *__iand =
R"doc(Sets this to the intersection of this and the given bitmask. Every bit
that is unset in *other* will be unset in this bitmask.

Parameter ``other``:
    the bitmask to intersect with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask2_::__init
static const char *__init =
R"doc(Creates a new bitmask with all bits set to ``False``.

The integer argument is merely for compatibility with the Bitmask
constructor, and will be ignored.

.. warning::
    This is *not* a constructor that initialises the bitmask to a
    given pattern.)doc";

// Docstring regina::python::doc::Bitmask2_::__ior
static const char *__ior =
R"doc(Sets this to the union of this and the given bitmask. Every bit that
is set in *other* will be set in this bitmask.

Parameter ``other``:
    the bitmask to union with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask2_::__isub
static const char *__isub =
R"doc(Sets this to the set difference of this and the given bitmask. Every
bit that is set in *other* will be cleared in this bitmask.

Parameter ``other``:
    the bitmask to XOR with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask2_::__ixor
static const char *__ixor =
R"doc(Sets this to the exclusive disjunction (XOR) of this and the given
bitmask. Every bit that is set in *other* will be flipped in this
bitmask.

Parameter ``other``:
    the bitmask to XOR with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask2_::__le
static const char *__le =
R"doc(Determines whether this bitmask is entirely contained within the given
bitmask.

For this routine to return ``True``, every bit that is set in this
bitmask must also be set in the given bitmask.

.. warning::
    This operation does not compare bitmasks lexicographically;
    moreover, it only describes a partial order, not a total order. To
    compare bitmasks lexicographically, use lessThan() instead.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this bitmask is entirely contained within
    the given bitmask.)doc";

// Docstring regina::python::doc::Bitmask2_::__ne
static const char *__ne =
R"doc(Determines whether this and the given bitmask are different.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this and the given bitmask are different.)doc";

// Docstring regina::python::doc::Bitmask2_::atMostOneBit
static const char *atMostOneBit =
R"doc(Determines whether at most one bit is set to ``True`` in this bitmask.

If this bitmask is entirely ``False`` or if only one bit is set to
``True``, then this routine will return ``True``. Otherwise this
routine will return ``False``.

Returns:
    ``True`` if and only if at most one bit is set to ``True``.)doc";

// Docstring regina::python::doc::Bitmask2_::bits
static const char *bits =
R"doc(Returns the number of bits currently set to ``True`` in this bitmask.

Returns:
    the number of ``True`` bits.)doc";

// Docstring regina::python::doc::Bitmask2_::containsIntn
static const char *containsIntn =
R"doc(Determines whether this bitmask contains the intersection of the two
given bitmasks.

For this routine to return ``True``, every bit that is set in *both*
*x* and *y* must be set in this bitmask also.

Parameter ``x``:
    the first bitmask used to form the intersection.

Parameter ``y``:
    the first bitmask used to form the intersection.

Returns:
    ``True`` if and only if this bitmask entirely contains the
    intersection of *x* and *y*.)doc";

// Docstring regina::python::doc::Bitmask2_::firstBit
static const char *firstBit =
R"doc(Returns the index of the first ``True`` bit in this bitmask, or -1 if
there are no ``True`` bits.

Returns:
    the index of the first ``True`` bit.)doc";

// Docstring regina::python::doc::Bitmask2_::flip
static const char *flip =
R"doc(Negates every bit in this bitmask. All ``True`` bits will be set to
``False`` and vice versa.

Unlike the more generic Bitmask, this optimised bitmask class does not
store a length. This means that all 8 * sizeof(*T*) + 8 * sizeof(*U*)
possible bits will be negated.)doc";

// Docstring regina::python::doc::Bitmask2_::get
static const char *get =
R"doc(Returns the value of the given bit of this bitmask.

Parameter ``index``:
    indicates which bit to query; this must be between 0 and (8 *
    sizeof(*T*) + 8 * sizeof(*U*) - 1) inclusive.

Returns:
    the value of the (*index*)th bit.)doc";

// Docstring regina::python::doc::Bitmask2_::inUnion
static const char *inUnion =
R"doc(Determines whether this bitmask is entirely contained within the union
of the two given bitmasks.

For this routine to return ``True``, every bit that is set in this
bitmask must also be set in either *x* or *y*.

Parameter ``x``:
    the first bitmask used to form the union.

Parameter ``y``:
    the first bitmask used to form the union.

Returns:
    ``True`` if and only if this bitmask is entirely contained within
    the union of *x* and *y*.)doc";

// Docstring regina::python::doc::Bitmask2_::lastBit
static const char *lastBit =
R"doc(Returns the index of the last ``True`` bit in this bitmask, or -1 if
there are no ``True`` bits.

Returns:
    the index of the last ``True`` bit.)doc";

// Docstring regina::python::doc::Bitmask2_::lessThan
static const char *lessThan =
R"doc(Determines whether this bitmask appears strictly before the given
bitmask when bitmasks are sorted in lexicographical order. Here the
bit at index 0 is least significant, and the bit at index *length*-1
is most significant.

.. warning::
    We do not use < for this operation, since <= represents the subset
    operation.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this is lexicographically strictly smaller
    than the given bitmask.)doc";

// Docstring regina::python::doc::Bitmask2_::reset
static const char *reset = R"doc(Sets all bits of this bitmask to ``False``.)doc";

// Docstring regina::python::doc::Bitmask2_::reset_2
static const char *reset_2 =
R"doc(Sets all bits of this bitmask to ``False``.

The integer argument is merely for compatibility with
Bitmask::reset(size_t), and will be ignored.)doc";

// Docstring regina::python::doc::Bitmask2_::set
static const char *set =
R"doc(Sets the given bit of this bitmask to the given value.

Parameter ``index``:
    indicates which bit to set; this must be between 0 and (8 *
    sizeof(*T*) + 8 * sizeof(*U*) - 1) inclusive.

Parameter ``value``:
    the value that will be assigned to the (*index*)th bit.)doc";

// Docstring regina::python::doc::Bitmask2_::set_2
static const char *set_2 =
R"doc(Sets all bits in the given sorted list to the given value.

This is a convenience routine for setting many bits at once. The
indices of the bits to set should be sorted and stored in some
container, such as a std::set or a C-style array. This routine takes
iterators over this container, and sets the bits at the corresponding
indices to the given value.

For example, the following code would set bits 3, 5 and 6 to ``true:``

```
std::vector<unsigned> indices;
indices.push(3); indices.push(5); indices.push(6);
bitmask.set(indices.begin(), indices.end(), true);
```

Likewise, the following code would set bits 1, 4 and 7 to ``false:``

```
unsigned indices[3] = { 1, 4, 7 };
bitmask.set(indices, indices + 3, false);
```

All other bits of this bitmask are unaffected by this routine.

Precondition:
    *ForwardIterator* is a forward iterator type that iterates over
    integer values.

Precondition:
    The list of indices described by these iterators is in *sorted*
    order. This is to allow optimisations for larger bitmask types.

Precondition:
    All indices in the given list are between 0 and (8 * sizeof(*T*) +
    8 * sizeof(*U*) - 1) inclusive.

Python:
    Instead of a pair of iterators, you should pass a Python list
    (which, as described above, must be a sorted list of indices).

Parameter ``indexBegin``:
    the beginning of the iterator range containing the sorted indices
    of the bits to set.

Parameter ``indexEnd``:
    the end of the iterator range containing the sorted indices of the
    bits to set.

Parameter ``value``:
    the value that will be assigned to each of the corresponding bits.)doc";

// Docstring regina::python::doc::Bitmask2_::truncate
static const char *truncate =
R"doc(Leaves the first *numBits* bits of this bitmask intact, but sets all
subsequent bits to ``False``. In other words, this routine "truncates"
this bitmask to the given number of bits.

This routine does not change the *length* of this bitmask (as passed
to the contructor or to reset()).

Parameter ``numBits``:
    the number of bits that will *not* be cleared.)doc";

}

namespace Bitmask_ {

// Docstring regina::python::doc::Bitmask_::__copy
static const char *__copy =
R"doc(Creates a clone of the given bitmask.

It is fine if the given bitmask is invalid (but in this case, the new
bitmask will be invalid also). Invalid bitmasks must be assigned a
length using reset(size_t) or the assignment operator.

Parameter ``src``:
    the bitmask to clone.)doc";

// Docstring regina::python::doc::Bitmask_::__default
static const char *__default =
R"doc(Creates a new invalid bitmask. You must call the one-argument
reset(size_t) or use the assignment operator to give the bitmask a
length before it can be used.

Use of this default constructor is discouraged. The only reason it
exists is to support arrays and containers of bitmasks, where the
bitmasks must be created in bulk and then individually assigned
lengths.

.. warning::
    No other routines can be used with this bitmask until it has been
    assigned a length via reset(size_t) or the assignment operator. As
    the single exception, the class destructor is safe to use even if
    a bitmask has never been initialised.)doc";

// Docstring regina::python::doc::Bitmask_::__eq
static const char *__eq =
R"doc(Determines whether this and the given bitmask are identical.

.. warning::
    As explain in the class notes, bitmasks do not store their exact
    length; instead the length is rounded up to the next "raw unit of
    storage". This means that two bitmasks that were initialised with
    different lengths may still be considered equal if the two lengths
    round up to the same value *and* the extra bits in the longer
    bitmask are all ``False``.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this and the given bitmask are identical.)doc";

// Docstring regina::python::doc::Bitmask_::__iand
static const char *__iand =
R"doc(Sets this to the intersection of this and the given bitmask. Every bit
that is unset in *other* will be unset in this bitmask.

Precondition:
    This and the given bitmask have the same length.

Parameter ``other``:
    the bitmask to intersect with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask_::__init
static const char *__init =
R"doc(Creates a new bitmask of the given length with all bits set to
``False``.

Parameter ``length``:
    the number of bits stored in this bitmask; this must be at least
    one.)doc";

// Docstring regina::python::doc::Bitmask_::__ior
static const char *__ior =
R"doc(Sets this to the union of this and the given bitmask. Every bit that
is set in *other* will be set in this bitmask.

Precondition:
    This and the given bitmask have the same length.

Parameter ``other``:
    the bitmask to union with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask_::__isub
static const char *__isub =
R"doc(Sets this to the set difference of this and the given bitmask. Every
bit that is set in *other* will be cleared in this bitmask.

Precondition:
    This and the given bitmask have the same length.

Parameter ``other``:
    the bitmask to XOR with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask_::__ixor
static const char *__ixor =
R"doc(Sets this to the exclusive disjunction (XOR) of this and the given
bitmask. Every bit that is set in *other* will be flipped in this
bitmask.

Precondition:
    This and the given bitmask have the same length.

Parameter ``other``:
    the bitmask to XOR with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask_::__le
static const char *__le =
R"doc(Determines whether this bitmask is entirely contained within the given
bitmask.

For this routine to return ``True``, every bit that is set in this
bitmask must also be set in the given bitmask.

Precondition:
    This and the given bitmask have the same length.

.. warning::
    This operation does not compare bitmasks lexicographically;
    moreover, it only describes a partial order, not a total order. To
    compare bitmasks lexicographically, use lessThan() instead.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this bitmask is entirely contained within
    the given bitmask.)doc";

// Docstring regina::python::doc::Bitmask_::__ne
static const char *__ne =
R"doc(Determines whether this and the given bitmask are different.

.. warning::
    As explain in the class notes, bitmasks do not store their exact
    length; instead the length is rounded up to the next "raw unit of
    storage". This means that two bitmasks that were initialised with
    different lengths may still be considered equal if the two lengths
    round up to the same value *and* the extra bits in the longer
    bitmask are all ``False``.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this and the given bitmask are different.)doc";

// Docstring regina::python::doc::Bitmask_::atMostOneBit
static const char *atMostOneBit =
R"doc(Determines whether at most one bit is set to ``True`` in this bitmask.

If this bitmask is entirely ``False`` or if only one bit is set to
``True``, then this routine will return ``True``. Otherwise this
routine will return ``False``.

Returns:
    ``True`` if and only if at most one bit is set to ``True``.)doc";

// Docstring regina::python::doc::Bitmask_::bits
static const char *bits =
R"doc(Returns the number of bits currently set to ``True`` in this bitmask.

Returns:
    the number of ``True`` bits.)doc";

// Docstring regina::python::doc::Bitmask_::containsIntn
static const char *containsIntn =
R"doc(Determines whether this bitmask contains the intersection of the two
given bitmasks.

For this routine to return ``True``, every bit that is set in *both*
*x* and *y* must be set in this bitmask also.

Precondition:
    Both *x* and *y* are the same length as this bitmask.

Parameter ``x``:
    the first bitmask used to form the intersection.

Parameter ``y``:
    the first bitmask used to form the intersection.

Returns:
    ``True`` if and only if this bitmask entirely contains the
    intersection of *x* and *y*.)doc";

// Docstring regina::python::doc::Bitmask_::firstBit
static const char *firstBit =
R"doc(Returns the index of the first ``True`` bit in this bitmask, or -1 if
there are no ``True`` bits.

Returns:
    the index of the first ``True`` bit.)doc";

// Docstring regina::python::doc::Bitmask_::flip
static const char *flip =
R"doc(Negates every bit in this bitmask. All ``True`` bits will be set to
``False`` and vice versa.

.. warning::
    Because this class may increase the bitmask length (rounding up to
    the next unit of storage), flip() may set additional ``True`` bits
    in the "dead space" between the intended length and the actual
    length. This may cause unexpected results for routines such as
    subset testing, bit counting and so on. Be careful!)doc";

// Docstring regina::python::doc::Bitmask_::get
static const char *get =
R"doc(Returns the value of the given bit of this bitmask.

Parameter ``index``:
    indicates which bit to query; this must be at least zero and
    strictly less than the length of this bitmask.

Returns:
    the value of the (*index*)th bit.)doc";

// Docstring regina::python::doc::Bitmask_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given bitmasks.

This global routine simply calls Bitmask::swap(); it is provided so
that Bitmask meets the C++ Swappable requirements.

Parameter ``a``:
    the first bitmask whose contents should be swapped.

Parameter ``b``:
    the second bitmask whose contents should be swapped.)doc";

// Docstring regina::python::doc::Bitmask_::inUnion
static const char *inUnion =
R"doc(Determines whether this bitmask is entirely contained within the union
of the two given bitmasks.

For this routine to return ``True``, every bit that is set in this
bitmask must also be set in either *x* or *y*.

Precondition:
    Both *x* and *y* are the same length as this bitmask.

Parameter ``x``:
    the first bitmask used to form the union.

Parameter ``y``:
    the first bitmask used to form the union.

Returns:
    ``True`` if and only if this bitmask is entirely contained within
    the union of *x* and *y*.)doc";

// Docstring regina::python::doc::Bitmask_::lastBit
static const char *lastBit =
R"doc(Returns the index of the last ``True`` bit in this bitmask, or -1 if
there are no ``True`` bits.

Returns:
    the index of the last ``True`` bit.)doc";

// Docstring regina::python::doc::Bitmask_::lessThan
static const char *lessThan =
R"doc(Determines whether this bitmask appears strictly before the given
bitmask when bitmasks are sorted in lexicographical order. Here the
bit at index 0 is least significant, and the bit at index *length*-1
is most significant.

Precondition:
    This and the given bitmask have the same length.

.. warning::
    We do not use < for this operation, since <= represents the subset
    operation.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this is lexicographically strictly smaller
    than the given bitmask.)doc";

// Docstring regina::python::doc::Bitmask_::reset
static const char *reset =
R"doc(Sets all bits of this bitmask to ``False``.

.. warning::
    The length of this bitmask must already have been initialised. In
    particular, if the default constructor was used, you must call the
    one-argument reset(size_t) before you can use this routine.)doc";

// Docstring regina::python::doc::Bitmask_::reset_2
static const char *reset_2 =
R"doc(Resizes this bitmask to the given length and sets all bits to
``False``.

This routine can be used to change the length (number of bits) of the
bitmask if desired.

Parameter ``length``:
    the number of bits to store in this bitmask; this must be at least
    one.)doc";

// Docstring regina::python::doc::Bitmask_::set
static const char *set =
R"doc(Sets the given bit of this bitmask to the given value.

Parameter ``index``:
    indicates which bit to set; this must be at least zero and
    strictly less than the length of this bitmask.

Parameter ``value``:
    the value that will be assigned to the (*index*)th bit.)doc";

// Docstring regina::python::doc::Bitmask_::set_2
static const char *set_2 =
R"doc(Sets all bits in the given sorted list to the given value.

This is a convenience routine for setting many bits at once. The
indices of the bits to set should be sorted and stored in some
container, such as a std::set or a C-style array. This routine takes
iterators over this container, and sets the bits at the corresponding
indices to the given value.

For example, the following code would set bits 3, 5 and 6 to ``true:``

```
std::vector<unsigned> indices;
indices.push(3); indices.push(5); indices.push(6);
bitmask.set(indices.begin(), indices.end(), true);
```

Likewise, the following code would set bits 1, 4 and 7 to ``false:``

```
unsigned indices[3] = { 1, 4, 7 };
bitmask.set(indices, indices + 3, false);
```

All other bits of this bitmask are unaffected by this routine.

Precondition:
    *ForwardIterator* is a forward iterator type that iterates over
    integer values.

Precondition:
    The list of indices described by these iterators is in *sorted*
    order. This is to allow optimisations for larger bitmask types.

Precondition:
    All indices in the given list are at least zero and strictly less
    than the length of this bitmask.

Python:
    Instead of a pair of iterators, you should pass a Python list
    (which, as described above, must be a sorted list of indices).

Parameter ``indexBegin``:
    the beginning of the iterator range containing the sorted indices
    of the bits to set.

Parameter ``indexEnd``:
    the end of the iterator range containing the sorted indices of the
    bits to set.

Parameter ``value``:
    the value that will be assigned to each of the corresponding bits.)doc";

// Docstring regina::python::doc::Bitmask_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given bitmask.

Parameter ``other``:
    the bitmask whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Bitmask_::truncate
static const char *truncate =
R"doc(Leaves the first *numBits* bits of this bitmask intact, but sets all
subsequent bits to ``False``. In other words, this routine "truncates"
this bitmask to the given number of bits.

This routine does not change the *length* of this bitmask (as passed
to the contructor or to reset()).

Precondition:
    *numBits* is at most the length of this bitmask.

Parameter ``numBits``:
    the number of bits that will *not* be cleared.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

