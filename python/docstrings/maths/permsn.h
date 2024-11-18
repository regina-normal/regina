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

Typically you would access this object through the static constant
``Perm<n>::Sn``. There should be no need for end users to refer to
this type directly.

The two main ways in which you can use this object are:

* Array-style lookup, using ``Perm<n>::Sn[i]`` and
  ``Perm<n>::Sn::size()`` (or the usual ``len()`` function in Python);

* Iteration, by iterating over ``Perm<n>::Sn`` in a range-based
  ``for`` loop (or by using the iterators ``Perm<n>::Sn.begin()`` and
  ``Perm<n>::Sn.end()`` directly).

Regarding indices and iteration:

* The permutations with even indices are the even permutations, and
  the permutations with odd indices are the odd permutations. The
  identity permutation will have index 0.

* The order of iteration is the same ordering used by index-based
  lookup. In particular, iteration will start with the identity and
  alternate between even and odd permutations.

* Iterating is typically faster than calling ``Sn[i]`` for each index
  *i* in turn, particularly for larger *n*. See the notes on time
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
typically just use ``Perm<n>::Sn`` instead of creating an object of
this type yourself.

Python:
    Python does not support templates. In Python, the various classes
    ``PermSn<n>`` are available under the names PermSn2, PermSn3, ...,
    PermSn16.

Template parameter ``n``:
    the number of objects being permuted. This must be between 2 and
    16 inclusive.)doc";

namespace PermSn_ {

// Docstring regina::python::doc::PermSn_::__array
static const char *__array =
R"doc(Returns the permutation at the given index in *Sn*.

See the PermSn class notes for further details on how array-like
indexing works over *Sn*.

For *n* ≤ 7, this operator is very fast constant time. However, for
*n* ≥ 8 the current implementation is quadratic in *n*.

Parameter ``index``:
    an index between 0 and ``n!-1`` inclusive.

Returns:
    the corresponding permutation in Sn.)doc";

// Docstring regina::python::doc::PermSn_::__iter__
static const char *__iter__ =
R"doc(Returns a Python iterator over all permutations in *Sn*.

See the PermSn class notes for further details on how iteration works
over *Sn*.

Returns:
    an iterator over all permutations in *Sn*.)doc";

// Docstring regina::python::doc::PermSn_::iterator
static const char *iterator =
R"doc(An iterator over all permutations in *Sn*.

See the PermSn class notes for further details on how iteration works
over *Sn*. In particular:

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
  access iterator (*n* ≤ 7) or a forward iterator (*n* ≥ 8), and you
  would typically access iterators either via ``Perm<n>::Sn.begin()``
  and ``Perm<n>::Sn.end()``, or by using a range-based ``for`` loop
  over ``Perm<n>::Sn``.

* In Python, this class and PermSn together implement the expected
  iterface for Python iterators: you would typically create and use
  iterators by iterating over ``Perm<n>.Sn``, which internally uses
  ``PermSn.__iter__()`` and ``PermSn.iterator.__next__()``.)doc";

// Docstring regina::python::doc::PermSn_::size
static const char *size =
R"doc(Returns the total number of permutations in *Sn*. This is of course
just ``n!``.

Python:
    This is also used to implement the Python special method
    __len__().

Returns:
    the size of *Sn*.)doc";

}

namespace PermSn_::iterator_ {

// Docstring regina::python::doc::PermSn_::iterator_::__default
static const char *__default =
R"doc(Creates a begin iterator.

This iterator will point to the identity permutation; that is, it will
be equal to ``Perm<n>::Sn.begin()``.)doc";

// Docstring regina::python::doc::PermSn_::iterator_::__eq
static const char *__eq =
R"doc(Compares this with the given iterator for equality.

To be considered equal, two iterators must be pointing to the same
permutation in *Sn*.

Two past-the-end iterators will be considered equal.

Parameter ``rhs``:
    the iterator to compare this with.

Returns:
    ``True`` if and only if the two iterators are equal.)doc";

// Docstring regina::python::doc::PermSn_::iterator_::__init
static const char *__init =
R"doc(Creates either a begin or end iterator.

If no arguments are given or if *valid* is ``True``, then this
iterator will point to the identity permutation; that is, it will be
equal to ``Perm<n>::Sn.begin()``.

If *valid* is ``False``, then this iterator will be past-the-end; that
is, it will be equal to ``Perm<n>::Sn.end()``.

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

