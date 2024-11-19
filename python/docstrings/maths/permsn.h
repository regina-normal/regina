/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::PermSn
static const char *PermSn =
R"doc(A lightweight array-like object that supports fast lookup and
iteration for permutations on *n* objects.

Typically you would access this object through static constants such
as ``Perm<n>::Sn`` or ``Perm<n>::orderedSn``. There should be no need
for end users to refer to this type directly.

There are two main ways in which you can use this object. For the
examples below, we assume that we are accessing ``Perm<n>::Sn``.

* Array-style lookup. Here you would use accessors such as
  ``Perm<n>::Sn[i]`` and ``Perm<n>::Sn::size()``. Instead of size(),
  you can also use the standard ``len()`` function in Python.

* Iteration. Here you would typically iterate over ``Perm<n>::Sn`` in
  a range-based ``for`` loop, or use begin/end pairs such as
  ``Perm<n>::Sn.begin()`` and ``Perm<n>::Sn.end()``.

Regarding indices and iteration:

* Indices are between 0 and ``(n!-1)`` inclusive, and permutations are
  indexed according to the chosen ordering, i.e., the template
  parameter *order*. In particular: ``Perm<n>::Sn`` uses sign-based
  ordering, beginning with the identity permutation at index 0 and
  alternating between even and odd permutations, whereas
  ``Perm<n>::orderedSn`` uses lexicographical ordering according the
  images of ``0,...,n-1`` under each permutation.

* The order of iteration is the same as the order used for indexing.

* Iterating directly over this object (e.g., iterating over
  ``Perm<n>::Sn``) is typically faster than using array-like access
  for each index in turn (e.g., accessing ``Sn[0]``, ``Sn[1]``, etc.).
  This is particularly true when *n* is larger. See the notes on time
  complexity below.

Regarding time complexity:

* For *n* ≤ 7, iteration steps and index-based lookup are both
  extremely fast constant time. Iterators are random-access (and
  satisfy all of the expected time complexity constraints that come
  with this).

* For *n* ≥ 8, the time for a single iteration step is in linear in
  *n*, and index-based lookup is currently _quadratic_ in *n*.
  Iterators are merely forward iterators, not random access.

Objects of this type contain no data at all, which means they are
trivial to pass by value or swap with std::swap(), and all objects of
this type are essentially identical. As mentioned above, you would
typically just use ``Perm<n>::Sn`` or ``Perm<n>::orderedSn`` instead
of creating an object of this type yourself.

Python:
    Python does not support templates. In Python, the various classes
    ``PermSn<n, PermOrder::Sign>`` are available under the names
    PermSn2_Sign, PermSn3_Sign, ..., PermSn16_Sign, and the various
    classes ``PermSn<n, PermOrder::Lex>`` are available under the
    names PermSn2_Lex, PermSn3_Lex, ..., PermSn16_Lex.

Template parameter ``n``:
    the number of objects being permuted. This must be between 2 and
    16 inclusive.

Template parameter ``order``:
    the way in which we should order permutations for the purposes of
    indexing and iteration.

Template parameter ``codeType``:
    the constant ``Perm<n>::codeType``. You should allow the compiler
    to deduce this and not attempt to set it yourself.)doc";

namespace PermSn_ {

// Docstring regina::python::doc::PermSn_::__array
static const char *__array =
R"doc(Returns the permutation at the given index in *Sn*, according to the
chosen ordering.

See the PermSn class notes for further details on how array-like
indexing works for permutations of *n* objects. In particular, note
that which permutation corresponds to which index will depend upon the
template parameter *order*.

For *n* ≤ 7, this operator is very fast constant time. However, for
*n* ≥ 8 the current implementation is quadratic in *n*.

Parameter ``index``:
    an index between 0 and ``n!-1`` inclusive.

Returns:
    the corresponding permutation.)doc";

// Docstring regina::python::doc::PermSn_::__iter__
static const char *__iter__ =
R"doc(Returns a Python iterator over all permutations of *n* objects.

See the PermSn class notes for further details on how iteration works.

Returns:
    an iterator over all permutations of *n* objects.)doc";

// Docstring regina::python::doc::PermSn_::iterator
static const char *iterator =
R"doc(An iterator over all permutations of *n* objects.

See the PermSn class notes for further details on how iteration works.
In particular:

* For smaller permutations (*n* ≤ 7), these iterators are random-
  access, and all of the expected operations are fast constant time.

* For larger permutations (*n* ≥ 8), these are forward iterators only,
  and a single forward step takes time _linear_ in *n*.

Unlike most iterator types, the dereference operator for this iterator
type returns by value, not by reference. This is because the
individual permutations are generated, not stored.

For most iterator classes, Regina now uses specialisations of
std::iterator_traits to provide access to their associated types
(e.g., value_type). However, this is not possible for PermSn::iterator
since PermSn is templated. Therefore, for PermSn::iterator, we
continue to provide these associated types directly as class members.

Both *iterator* and *const_iterator* are the same type, since PermSn
only offers read-only access to its permutations.

The full interface for the iterator type is not documented here, since
it changes according to both *n* and your programming language:

* In C++, this class implements the full interface for either a random
  access iterator (*n* ≤ 7) or a forward iterator (*n* ≥ 8). Using
  ``Perm<n>::Sn`` as an example, you would typically access iterators
  either via ``Perm<n>::Sn.begin()`` and ``Perm<n>::Sn.end()``, or by
  using a range-based ``for`` loop over ``Perm<n>::Sn``.

* In Python, this class and PermSn together implement the expected
  interface for Python iterators. Using ``Perm4.Sn`` as an example,
  you would typically create and use iterators by iterating over
  ``Perm4.Sn``, which internally uses ``PermSn4_Sign.__iter__()`` and
  ``PermSn4_Sign.iterator.__next__()``.)doc";

// Docstring regina::python::doc::PermSn_::size
static const char *size =
R"doc(Returns the total number of permutations of *n* objects. This is of
course just ``n!``.

Python:
    This is also used to implement the Python special method
    __len__().

Returns:
    the total number of permutations.)doc";

}

namespace PermSn_::iterator_ {

// Docstring regina::python::doc::PermSn_::iterator_::__default
static const char *__default =
R"doc(Creates a begin iterator.

This iterator will point to the identity permutation. So, for example,
if you are iterating over ``Perm<n>::Sn``, then this iterator will be
equal to ``Perm<n>::Sn.begin()``.)doc";

// Docstring regina::python::doc::PermSn_::iterator_::__eq
static const char *__eq =
R"doc(Compares this with the given iterator for equality.

To be considered equal, two iterators must both be dereferencable and
pointing to the same permutation, or must both be past-the-end.

Parameter ``rhs``:
    the iterator to compare this with.

Returns:
    ``True`` if and only if the two iterators are equal.)doc";

// Docstring regina::python::doc::PermSn_::iterator_::__init
static const char *__init =
R"doc(Creates either a begin or end iterator.

If no arguments are given or if *valid* is ``True``, then this
iterator will point to the identity permutation. For example, if you
are iterating over ``Perm<n>::Sn``, then this iterator will be equal
to ``Perm<n>::Sn.begin()``.

If *valid* is ``False``, then this iterator will be past-the-end. For
example, if you are iterating over ``Perm<n>::Sn``, then this iterator
will be equal to ``Perm<n>::Sn.end()``.

Parameter ``valid``:
    ``True`` if this should be a begin iterator, or ``False`` if this
    should be an end iterator.)doc";

// Docstring regina::python::doc::PermSn_::iterator_::__next__
static const char *__next__ =
R"doc(Returns the current permutation and increments this iterator.

Exception ``StopIteration``:
    The iterator is already past-the-end when this function is called.

Returns:
    the permutation that this iterator is pointing to, before the
    increment takes place.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

