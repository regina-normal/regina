/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct Bitmask {

// Docstring regina::python::doc::Bitmask::__class
static constexpr const char __class[] =
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

// Docstring regina::python::doc::Bitmask::__cmp
static constexpr const char __cmp[] =
R"doc(Compares two bitmasks under the subset relation.

Here the bitmask *x* is considered less than *y* if the bits that are
set in *x* form a strict subset of the bits that are set in *y*. In
other words: ``x ≠ y``, and every bit that is set in *x* is also set
in *y*.

This operator generates all of the usual comparison operators,
including ``<``, ``<=``, ``>``, and ``>=``.

Precondition:
    This and the given bitmask have the same length.

.. note::
    This does not compare bitmasks lexicographically. For
    lexicographical comparison, use lessThan() instead.

Python:
    This spaceship operator ``x <=> y`` is not available, but the
    other comparison operators that it generates _are_ available.

Parameter ``rhs``:
    the bitmask to compare against this.

Returns:
    the result of the subset comparison between this and the given
    bitmask.)doc";

// Docstring regina::python::doc::Bitmask::__copy
static constexpr const char __copy[] =
R"doc(Creates a clone of the given bitmask.

It is fine if the given bitmask is invalid (but in this case, the new
bitmask will be invalid also). Invalid bitmasks must be assigned a
length using reset(size_t) or the assignment operator.)doc";

// Docstring regina::python::doc::Bitmask::__default
static constexpr const char __default[] =
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

// Docstring regina::python::doc::Bitmask::__eq
static constexpr const char __eq[] =
R"doc(Determines whether this and the given bitmask are identical.

.. warning::
    As explain in the class notes, bitmasks do not store their exact
    length; instead the length is rounded up to the next "raw unit of
    storage". This means that two bitmasks that were initialised with
    different lengths may still be considered equal if the two lengths
    round up to the same value _and_ the extra bits in the longer
    bitmask are all ``False``.

Returns:
    ``True`` if and only if this and the given bitmask are identical.)doc";

// Docstring regina::python::doc::Bitmask::__iand
static constexpr const char __iand[] =
R"doc(Sets this to the intersection of this and the given bitmask. Every bit
that is unset in *other* will be unset in this bitmask.

Precondition:
    This and the given bitmask have the same length.

Parameter ``other``:
    the bitmask to intersect with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask::__init
static constexpr const char __init[] =
R"doc(Creates a new bitmask of the given length with all bits set to
``False``.

Parameter ``length``:
    the number of bits stored in this bitmask.)doc";

// Docstring regina::python::doc::Bitmask::__ior
static constexpr const char __ior[] =
R"doc(Sets this to the union of this and the given bitmask. Every bit that
is set in *other* will be set in this bitmask.

Precondition:
    This and the given bitmask have the same length.

Parameter ``other``:
    the bitmask to union with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask::__isub
static constexpr const char __isub[] =
R"doc(Sets this to the set difference of this and the given bitmask. Every
bit that is set in *other* will be cleared in this bitmask.

Precondition:
    This and the given bitmask have the same length.

Parameter ``other``:
    the bitmask to XOR with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask::__ixor
static constexpr const char __ixor[] =
R"doc(Sets this to the exclusive disjunction (XOR) of this and the given
bitmask. Every bit that is set in *other* will be flipped in this
bitmask.

Precondition:
    This and the given bitmask have the same length.

Parameter ``other``:
    the bitmask to XOR with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask::atMostOneBit
static constexpr const char atMostOneBit[] =
R"doc(Determines whether at most one bit is set to ``True`` in this bitmask.

If this bitmask is entirely ``False`` or if only one bit is set to
``True``, then this routine will return ``True``. Otherwise this
routine will return ``False``.

Returns:
    ``True`` if and only if at most one bit is set to ``True``.)doc";

// Docstring regina::python::doc::Bitmask::bits
static constexpr const char bits[] =
R"doc(Returns the number of bits currently set to ``True`` in this bitmask.

Returns:
    the number of ``True`` bits.)doc";

// Docstring regina::python::doc::Bitmask::containsIntn
static constexpr const char containsIntn[] =
R"doc(Determines whether this bitmask contains the intersection of the two
given bitmasks.

For this routine to return ``True``, every bit that is set in _both_
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

// Docstring regina::python::doc::Bitmask::firstBit
static constexpr const char firstBit[] =
R"doc(Returns the index of the first ``True`` bit in this bitmask, or -1 if
there are no ``True`` bits.

Returns:
    the index of the first ``True`` bit.)doc";

// Docstring regina::python::doc::Bitmask::flip
static constexpr const char flip[] =
R"doc(Negates every bit in this bitmask. All ``True`` bits will be set to
``False`` and vice versa.

.. warning::
    Because this class may increase the bitmask length (rounding up to
    the next unit of storage), flip() may set additional ``True`` bits
    in the "dead space" between the intended length and the actual
    length. This may cause unexpected results for routines such as
    subset testing, bit counting and so on. Be careful!)doc";

// Docstring regina::python::doc::Bitmask::get
static constexpr const char get[] =
R"doc(Returns the value of the given bit of this bitmask.

Parameter ``index``:
    indicates which bit to query; this must be at least zero and
    strictly less than the length of this bitmask.

Returns:
    the value of the (*index*)th bit.)doc";

// Docstring regina::python::doc::Bitmask::global_swap
static constexpr const char global_swap[] =
R"doc(Swaps the contents of the two given bitmasks.

This global routine simply calls Bitmask::swap(); it is provided so
that Bitmask meets the C++ Swappable requirements.

Parameter ``a``:
    the first bitmask whose contents should be swapped.

Parameter ``b``:
    the second bitmask whose contents should be swapped.)doc";

// Docstring regina::python::doc::Bitmask::inUnion
static constexpr const char inUnion[] =
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

// Docstring regina::python::doc::Bitmask::lastBit
static constexpr const char lastBit[] =
R"doc(Returns the index of the last ``True`` bit in this bitmask, or -1 if
there are no ``True`` bits.

Returns:
    the index of the last ``True`` bit.)doc";

// Docstring regina::python::doc::Bitmask::lessThan
static constexpr const char lessThan[] =
R"doc(Deprecated routine that determines whether this bitmask appears
strictly before the given bitmask when bitmasks are sorted in "reverse
lexicographical" order. Here the bit at index 0 is least significant,
and the bit at index *length*-1 is most significant.

.. deprecated::
    Instead use ``numericalComp(other)``, which has a clearer name and
    which returns a three-way comparison.

Precondition:
    This and the given bitmask have the same length.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this is "reverse lexicographically"
    strictly smaller than the given bitmask.)doc";

// Docstring regina::python::doc::Bitmask::numericalComp
static constexpr const char numericalComp[] =
R"doc(Compares this against the given bitmask numerically, treating the
bitmask as an unsigned integer written in binary. This is essentially
a "reverse lexicographical" ordering: the bit at index 0 is least
significant, and the bit at index *length*-1 is most significant.

Precondition:
    This and the given bitmask have the same length.

.. warning::
    We do not use < for this ordering, since the comparison operators
    (``<``, ``≤``, ``>``, ``≥``) work with the subset relation
    instead.

Python:
    Since Python does not support ``std::strong_ordering``, this
    routine is called ``numericalLessThan()`` instead, and it returns
    a boolean according to whether the comparison orders this bitmask
    before *other*.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    the result of the numerical comparison between this and the given
    bitmask.)doc";

// Docstring regina::python::doc::Bitmask::reset
static constexpr const char reset[] =
R"doc(Sets all bits of this bitmask to ``False``.

.. warning::
    The length of this bitmask must already have been initialised. In
    particular, if the default constructor was used, you must call the
    one-argument reset(size_t) before you can use this routine.)doc";

// Docstring regina::python::doc::Bitmask::reset_2
static constexpr const char reset_2[] =
R"doc(Resizes this bitmask to the given length and sets all bits to
``False``.

This routine can be used to change the length (number of bits) of the
bitmask if desired.

Parameter ``length``:
    the number of bits to store in this bitmask.)doc";

// Docstring regina::python::doc::Bitmask::set
static constexpr const char set[] =
R"doc(Sets the given bit of this bitmask to the given value.

Parameter ``index``:
    indicates which bit to set; this must be at least zero and
    strictly less than the length of this bitmask.

Parameter ``value``:
    the value that will be assigned to the (*index*)th bit.)doc";

// Docstring regina::python::doc::Bitmask::set_2
static constexpr const char set_2[] =
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
    The list of indices described by these iterators is in _sorted_
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

// Docstring regina::python::doc::Bitmask::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given bitmask.

Parameter ``other``:
    the bitmask whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Bitmask::truncate
static constexpr const char truncate[] =
R"doc(Leaves the first *numBits* bits of this bitmask intact, but sets all
subsequent bits to ``False``. In other words, this routine "truncates"
this bitmask to the given number of bits.

This routine does not change the _length_ of this bitmask (as passed
to the contructor or to reset()).

Precondition:
    *numBits* is at most the length of this bitmask.

Parameter ``numBits``:
    the number of bits that will _not_ be cleared.)doc";

}; // struct Bitmask

struct Bitmask1 {

// Docstring regina::python::doc::Bitmask1::__class
static constexpr const char __class[] =
R"doc(A small but extremely fast bitmask class that can store up to ``8 *
sizeof(T)`` true-or-false bits.

This bitmask packs all of the bits together into a single variable of
type *T*. This means that operations on bitmasks are extremely fast,
because all of the bits can be processed at once.

The downside of course is that the number of bits that can be stored
is limited to ``8 * sizeof(T)``, where *T* is some native unsigned C++
integer type.

For another extremely fast bitmask class that can store twice as many
bits, see Bitmask2. For a bitmask class that can store arbitrarily
many bits, see Bitmask.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Python:
    Python does not support templates, and so instead Regina's python
    interface offers the classes Bitmask8, Bitmask16, Bitmask32,
    Bitmask64, Bitmask128, and (if the machine supports 128-bit
    integers) Bitmask256. Each of these will be an optimised bitmask
    class that can hold the corresponding number of bits, and is
    guaranteed to be an instance of either the C++ ``Bitmask1<T>``
    class (where possible) or the C++ ``Bitmask2<T>`` template class
    (if necessary).)doc";

// Docstring regina::python::doc::Bitmask1::__cmp
static constexpr const char __cmp[] =
R"doc(Compares two bitmasks under the subset relation.

Here the bitmask *x* is considered less than *y* if the bits that are
set in *x* form a strict subset of the bits that are set in *y*. In
other words: ``x ≠ y``, and every bit that is set in *x* is also set
in *y*.

This operator generates all of the usual comparison operators,
including ``<``, ``<=``, ``>``, and ``>=``.

.. note::
    This does not compare bitmasks lexicographically. For
    lexicographical comparison, use lessThan() instead.

Python:
    This spaceship operator ``x <=> y`` is not available, but the
    other comparison operators that it generates _are_ available.

Parameter ``rhs``:
    the bitmask to compare against this.

Returns:
    the result of the subset comparison between this and the given
    bitmask.)doc";

// Docstring regina::python::doc::Bitmask1::__copy
static constexpr const char __copy[] = R"doc(Creates a clone of the given bitmask.)doc";

// Docstring regina::python::doc::Bitmask1::__default
static constexpr const char __default[] = R"doc(Creates a new bitmask with all bits set to ``False``.)doc";

// Docstring regina::python::doc::Bitmask1::__eq
static constexpr const char __eq[] =
R"doc(Determines whether this and the given bitmask are identical.

Returns:
    ``True`` if and only if this and the given bitmask are identical.)doc";

// Docstring regina::python::doc::Bitmask1::__iand
static constexpr const char __iand[] =
R"doc(Sets this to the intersection of this and the given bitmask. Every bit
that is unset in *other* will be unset in this bitmask.

Parameter ``other``:
    the bitmask to intersect with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask1::__init
static constexpr const char __init[] =
R"doc(Creates a new bitmask with all bits set to ``False``.

The integer argument is merely for compatibility with the Bitmask
constructor, and will be ignored.

.. warning::
    This is _not_ a constructor that initialises the bitmask to a
    given pattern.)doc";

// Docstring regina::python::doc::Bitmask1::__ior
static constexpr const char __ior[] =
R"doc(Sets this to the union of this and the given bitmask. Every bit that
is set in *other* will be set in this bitmask.

Parameter ``other``:
    the bitmask to union with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask1::__isub
static constexpr const char __isub[] =
R"doc(Sets this to the set difference of this and the given bitmask. Every
bit that is set in *other* will be cleared in this bitmask.

Parameter ``other``:
    the bitmask to XOR with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask1::__ixor
static constexpr const char __ixor[] =
R"doc(Sets this to the exclusive disjunction (XOR) of this and the given
bitmask. Every bit that is set in *other* will be flipped in this
bitmask.

Parameter ``other``:
    the bitmask to XOR with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask1::atMostOneBit
static constexpr const char atMostOneBit[] =
R"doc(Determines whether at most one bit is set to ``True`` in this bitmask.

If this bitmask is entirely ``False`` or if only one bit is set to
``True``, then this routine will return ``True``. Otherwise this
routine will return ``False``.

Returns:
    ``True`` if and only if at most one bit is set to ``True``.)doc";

// Docstring regina::python::doc::Bitmask1::bits
static constexpr const char bits[] =
R"doc(Returns the number of bits currently set to ``True`` in this bitmask.

Returns:
    the number of ``True`` bits.)doc";

// Docstring regina::python::doc::Bitmask1::containsIntn
static constexpr const char containsIntn[] =
R"doc(Determines whether this bitmask contains the intersection of the two
given bitmasks.

For this routine to return ``True``, every bit that is set in _both_
*x* and *y* must be set in this bitmask also.

Parameter ``x``:
    the first bitmask used to form the intersection.

Parameter ``y``:
    the first bitmask used to form the intersection.

Returns:
    ``True`` if and only if this bitmask entirely contains the
    intersection of *x* and *y*.)doc";

// Docstring regina::python::doc::Bitmask1::firstBit
static constexpr const char firstBit[] =
R"doc(Returns the index of the first ``True`` bit in this bitmask, or -1 if
there are no ``True`` bits.

Returns:
    the index of the first ``True`` bit.)doc";

// Docstring regina::python::doc::Bitmask1::flip
static constexpr const char flip[] =
R"doc(Negates every bit in this bitmask. All ``True`` bits will be set to
``False`` and vice versa.

Unlike the more generic Bitmask, this optimised bitmask class does not
store a length. This means that all ``8 * sizeof(T)`` possible bits
will be negated.)doc";

// Docstring regina::python::doc::Bitmask1::get
static constexpr const char get[] =
R"doc(Returns the value of the given bit of this bitmask.

Parameter ``index``:
    indicates which bit to query; this must be between 0 and ``(8 *
    sizeof(T) - 1)`` inclusive.

Returns:
    the value of the (*index*)th bit.)doc";

// Docstring regina::python::doc::Bitmask1::inUnion
static constexpr const char inUnion[] =
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

// Docstring regina::python::doc::Bitmask1::lastBit
static constexpr const char lastBit[] =
R"doc(Returns the index of the last ``True`` bit in this bitmask, or -1 if
there are no ``True`` bits.

Returns:
    the index of the last ``True`` bit.)doc";

// Docstring regina::python::doc::Bitmask1::lessThan
static constexpr const char lessThan[] =
R"doc(Deprecated routine that determines whether this bitmask appears
strictly before the given bitmask when bitmasks are sorted in "reverse
lexicographical" order. Here the bit at index 0 is least significant,
and the bit at index ``length-1`` is most significant.

.. deprecated::
    Instead use ``numericalComp(other)``, which has a clearer name and
    which returns a three-way comparison.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this is "reverse lexicographically"
    strictly smaller than the given bitmask.)doc";

// Docstring regina::python::doc::Bitmask1::numericalComp
static constexpr const char numericalComp[] =
R"doc(Compares this against the given bitmask numerically, treating the
bitmask as an unsigned integer written in binary. This is essentially
a "reverse lexicographical" ordering: the bit at index 0 is least
significant, and the bit at index *length*-1 is most significant.

.. warning::
    We do not use < for this ordering, since the comparison operators
    (``<``, ``≤``, ``>``, ``≥``) work with the subset relation
    instead.

Python:
    Since Python does not support ``std::strong_ordering``, this
    routine is called ``numericalLessThan()`` instead, and it returns
    a boolean according to whether the comparison orders this bitmask
    before *other*.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    the result of the numerical comparison between this and the given
    bitmask.)doc";

// Docstring regina::python::doc::Bitmask1::reset
static constexpr const char reset[] = R"doc(Sets all bits of this bitmask to ``False``.)doc";

// Docstring regina::python::doc::Bitmask1::reset_2
static constexpr const char reset_2[] =
R"doc(Sets all bits of this bitmask to ``False``.

The integer argument is merely for compatibility with
Bitmask::reset(size_t), and will be ignored.)doc";

// Docstring regina::python::doc::Bitmask1::set
static constexpr const char set[] =
R"doc(Sets the given bit of this bitmask to the given value.

Parameter ``index``:
    indicates which bit to set; this must be between 0 and ``(8 *
    sizeof(T) - 1)`` inclusive.

Parameter ``value``:
    the value that will be assigned to the (*index*)th bit.)doc";

// Docstring regina::python::doc::Bitmask1::set_2
static constexpr const char set_2[] =
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
    The list of indices described by these iterators is in _sorted_
    order. This is to allow optimisations for larger bitmask types.

Precondition:
    All indices in the given list are between 0 and ``(8 * sizeof(T) -
    1)`` inclusive.

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

// Docstring regina::python::doc::Bitmask1::truncate
static constexpr const char truncate[] =
R"doc(Leaves the first *numBits* bits of this bitmask intact, but sets all
subsequent bits to ``False``. In other words, this routine "truncates"
this bitmask to the given number of bits.

This routine does not change the _length_ of this bitmask (as passed
to the contructor or to reset()).

Parameter ``numBits``:
    the number of bits that will _not_ be cleared.)doc";

}; // struct Bitmask1

struct Bitmask2 {

// Docstring regina::python::doc::Bitmask2::__class
static constexpr const char __class[] =
R"doc(A small but extremely fast bitmask class that can store up to ``16 *
sizeof(T)`` true-or-false bits.

This bitmask packs all of the bits together into two variables of type
*T*. This means that operations on entire bitmasks are extremely fast,
because all of the bits can be processed in just two "native"
operations.

The downside of course is that the number of bits that can be stored
is limited to ``16 * sizeof(T)``, where *T* is some native unsigned
C++ integer type.

For an even faster bitmask class that can only store half as many
bits, see Bitmask1. For a bitmask class that can store arbitrarily
many bits, see Bitmask.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Python:
    Python does not support templates, and so instead Regina's python
    interface offers the classes Bitmask8, Bitmask16, Bitmask32,
    Bitmask64, Bitmask128, and (if the machine supports 128-bit
    integers) Bitmask256. Each of these will be an optimised bitmask
    class that can hold the corresponding number of bits, and is
    guaranteed to be an instance of either the C++ ``Bitmask1<T>``
    class (where possible) or the C++ ``Bitmask2<T>`` template class
    (if necessary).)doc";

// Docstring regina::python::doc::Bitmask2::__cmp
static constexpr const char __cmp[] =
R"doc(Compares two bitmasks under the subset relation.

Here the bitmask *x* is considered less than *y* if the bits that are
set in *x* form a strict subset of the bits that are set in *y*. In
other words: ``x ≠ y``, and every bit that is set in *x* is also set
in *y*.

This operator generates all of the usual comparison operators,
including ``<``, ``<=``, ``>``, and ``>=``.

.. note::
    This does not compare bitmasks lexicographically. For
    lexicographical comparison, use lessThan() instead.

Python:
    This spaceship operator ``x <=> y`` is not available, but the
    other comparison operators that it generates _are_ available.

Parameter ``rhs``:
    the bitmask to compare against this.

Returns:
    the result of the subset comparison between this and the given
    bitmask.)doc";

// Docstring regina::python::doc::Bitmask2::__copy
static constexpr const char __copy[] = R"doc(Creates a clone of the given bitmask.)doc";

// Docstring regina::python::doc::Bitmask2::__default
static constexpr const char __default[] = R"doc(Creates a new bitmask with all bits set to ``False``.)doc";

// Docstring regina::python::doc::Bitmask2::__eq
static constexpr const char __eq[] =
R"doc(Determines whether this and the given bitmask are identical.

Returns:
    ``True`` if and only if this and the given bitmask are identical.)doc";

// Docstring regina::python::doc::Bitmask2::__iand
static constexpr const char __iand[] =
R"doc(Sets this to the intersection of this and the given bitmask. Every bit
that is unset in *other* will be unset in this bitmask.

Parameter ``other``:
    the bitmask to intersect with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask2::__init
static constexpr const char __init[] =
R"doc(Creates a new bitmask with all bits set to ``False``.

The integer argument is merely for compatibility with the Bitmask
constructor, and will be ignored.

.. warning::
    This is _not_ a constructor that initialises the bitmask to a
    given pattern.)doc";

// Docstring regina::python::doc::Bitmask2::__ior
static constexpr const char __ior[] =
R"doc(Sets this to the union of this and the given bitmask. Every bit that
is set in *other* will be set in this bitmask.

Parameter ``other``:
    the bitmask to union with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask2::__isub
static constexpr const char __isub[] =
R"doc(Sets this to the set difference of this and the given bitmask. Every
bit that is set in *other* will be cleared in this bitmask.

Parameter ``other``:
    the bitmask to XOR with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask2::__ixor
static constexpr const char __ixor[] =
R"doc(Sets this to the exclusive disjunction (XOR) of this and the given
bitmask. Every bit that is set in *other* will be flipped in this
bitmask.

Parameter ``other``:
    the bitmask to XOR with this.

Returns:
    a reference to this bitmask.)doc";

// Docstring regina::python::doc::Bitmask2::atMostOneBit
static constexpr const char atMostOneBit[] =
R"doc(Determines whether at most one bit is set to ``True`` in this bitmask.

If this bitmask is entirely ``False`` or if only one bit is set to
``True``, then this routine will return ``True``. Otherwise this
routine will return ``False``.

Returns:
    ``True`` if and only if at most one bit is set to ``True``.)doc";

// Docstring regina::python::doc::Bitmask2::bits
static constexpr const char bits[] =
R"doc(Returns the number of bits currently set to ``True`` in this bitmask.

Returns:
    the number of ``True`` bits.)doc";

// Docstring regina::python::doc::Bitmask2::containsIntn
static constexpr const char containsIntn[] =
R"doc(Determines whether this bitmask contains the intersection of the two
given bitmasks.

For this routine to return ``True``, every bit that is set in _both_
*x* and *y* must be set in this bitmask also.

Parameter ``x``:
    the first bitmask used to form the intersection.

Parameter ``y``:
    the first bitmask used to form the intersection.

Returns:
    ``True`` if and only if this bitmask entirely contains the
    intersection of *x* and *y*.)doc";

// Docstring regina::python::doc::Bitmask2::firstBit
static constexpr const char firstBit[] =
R"doc(Returns the index of the first ``True`` bit in this bitmask, or -1 if
there are no ``True`` bits.

Returns:
    the index of the first ``True`` bit.)doc";

// Docstring regina::python::doc::Bitmask2::flip
static constexpr const char flip[] =
R"doc(Negates every bit in this bitmask. All ``True`` bits will be set to
``False`` and vice versa.

Unlike the more generic Bitmask, this optimised bitmask class does not
store a length. This means that all ``16 * sizeof(T)`` possible bits
will be negated.)doc";

// Docstring regina::python::doc::Bitmask2::get
static constexpr const char get[] =
R"doc(Returns the value of the given bit of this bitmask.

Parameter ``index``:
    indicates which bit to query; this must be between 0 and ``(16 *
    sizeof(T) - 1)`` inclusive.

Returns:
    the value of the (*index*)th bit.)doc";

// Docstring regina::python::doc::Bitmask2::inUnion
static constexpr const char inUnion[] =
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

// Docstring regina::python::doc::Bitmask2::lastBit
static constexpr const char lastBit[] =
R"doc(Returns the index of the last ``True`` bit in this bitmask, or -1 if
there are no ``True`` bits.

Returns:
    the index of the last ``True`` bit.)doc";

// Docstring regina::python::doc::Bitmask2::lessThan
static constexpr const char lessThan[] =
R"doc(Deprecated routine that determines whether this bitmask appears
strictly before the given bitmask when bitmasks are sorted in "reverse
lexicographical" order. Here the bit at index 0 is least significant,
and the bit at index ``length-1`` is most significant.

.. deprecated::
    Instead use ``numericalComp(other)``, which has a clearer name and
    which returns a three-way comparison.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    ``True`` if and only if this is "reverse lexicographically"
    strictly smaller than the given bitmask.)doc";

// Docstring regina::python::doc::Bitmask2::numericalComp
static constexpr const char numericalComp[] =
R"doc(Compares this against the given bitmask numerically, treating the
bitmask as an unsigned integer written in binary. This is essentially
a "reverse lexicographical" ordering: the bit at index 0 is least
significant, and the bit at index *length*-1 is most significant.

.. warning::
    We do not use < for this ordering, since the comparison operators
    (``<``, ``≤``, ``>``, ``≥``) work with the subset relation
    instead.

Python:
    Since Python does not support ``std::strong_ordering``, this
    routine is called ``numericalLessThan()`` instead, and it returns
    a boolean according to whether the comparison orders this bitmask
    before *other*.

Parameter ``other``:
    the bitmask to compare against this.

Returns:
    the result of the numerical comparison between this and the given
    bitmask.)doc";

// Docstring regina::python::doc::Bitmask2::reset
static constexpr const char reset[] = R"doc(Sets all bits of this bitmask to ``False``.)doc";

// Docstring regina::python::doc::Bitmask2::reset_2
static constexpr const char reset_2[] =
R"doc(Sets all bits of this bitmask to ``False``.

The integer argument is merely for compatibility with
Bitmask::reset(size_t), and will be ignored.)doc";

// Docstring regina::python::doc::Bitmask2::set
static constexpr const char set[] =
R"doc(Sets the given bit of this bitmask to the given value.

Parameter ``index``:
    indicates which bit to set; this must be between 0 and ``(16 *
    sizeof(T) - 1)`` inclusive.

Parameter ``value``:
    the value that will be assigned to the (*index*)th bit.)doc";

// Docstring regina::python::doc::Bitmask2::set_2
static constexpr const char set_2[] =
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
    The list of indices described by these iterators is in _sorted_
    order. This is to allow optimisations for larger bitmask types.

Precondition:
    All indices in the given list are between 0 and ``(16 * sizeof(T)
    - 1)`` inclusive.

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

// Docstring regina::python::doc::Bitmask2::truncate
static constexpr const char truncate[] =
R"doc(Leaves the first *numBits* bits of this bitmask intact, but sets all
subsequent bits to ``False``. In other words, this routine "truncates"
this bitmask to the given number of bits.

This routine does not change the _length_ of this bitmask (as passed
to the contructor or to reset()).

Parameter ``numBits``:
    the number of bits that will _not_ be cleared.)doc";

}; // struct Bitmask2

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

