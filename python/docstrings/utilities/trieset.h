/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::TrieSet
static const char *TrieSet =
R"doc(A trie-like data structure for storing and retriving sets. This class
is useful when the *elements* of these sets are taken from a fairly
small universe, but where the *number* of sets being stored can be
extremely large.

For simplicity, let the universe consist of the integers
0,...,(*n*-1). Sets are represented as bitmasks of length *n* (using
one of Regina's bitmask types, such as Bitmask, Bitmask1 or Bitmask2).
The *i*th bit of a bitmask indicates whether the integer *i* belongs
to the corresponding set.

To construct an empty trie, simply call the default constructor. To
insert a new set into the trie, call insert() (whose running time is
linear in *n*). You can insert the same set into the trie multiple
times and the trie will record the number of times that it occurs.

Currently the only searching operations are hasSubset() and
hasExtraSuperset(). These operations are slow, but still much faster
than searching through a linear list; see the hasSubset() and
hasExtraSuperset() documentation for details.

The implementation of this data structure uses a binary tree with
depth levels 0,...,*n*, where each node at level *i* represents a
potential length-*i* prefix for a bitmask. So, for instance, the root
node represents the empty prefix, its children represent prefixes 0
and 1, their children represent prefixes 00, 01, 10 and 11, and so on.

Internally, a set is "stored" at the first node whose prefix in fact
describes the entire set. For instance, the bitmask 101100 is stored
at the node corresponding to the prefix 1011, which occurs at level 3
of the tree. Regions of the tree that do not store any sets are never
explicitly constructed in memory.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

namespace TrieSet_ {

// Docstring regina::python::doc::TrieSet_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given collection. This will induce a deep
copy of *src*.

Parameter ``src``:
    the collection of sets to copy.)doc";

// Docstring regina::python::doc::TrieSet_::__default
static const char *__default = R"doc(Constructs an empty collection of sets.)doc";

// Docstring regina::python::doc::TrieSet_::__eq
static const char *__eq =
R"doc(Determines whether this and the given collection store exactly the
same sets.

Parameter ``other``:
    the collection to compare with this.

Returns:
    ``True`` if and only if both collections store the same sets.)doc";

// Docstring regina::python::doc::TrieSet_::__ne
static const char *__ne =
R"doc(Determines whether this and the given collection do not store exactly
the same sets.

Parameter ``other``:
    the collection to compare with this.

Returns:
    ``True`` if and only if both collections do not store the same
    sets.)doc";

// Docstring regina::python::doc::TrieSet_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given collections.

Parameter ``a``:
    the first collection of sets whose contents should be swapped.

Parameter ``b``:
    the second collection of sets whose contents should be swapped.)doc";

// Docstring regina::python::doc::TrieSet_::hasExtraSuperset
static const char *hasExtraSuperset =
R"doc(Performs the particular superset search required by the double
description method.

This routine asks the following question: In this collection of sets,
is there any superset of the argument *subset* *other* than *exc1* or
*exc2*? Here the sets *exc1* and *exc2* are explicitly excluded from
our search. Supersets need not be *proper* supersets (so if an exact
copy of *subset* is found in the tree then this will suffice).

This routine has a slow running time, which in pathological cases can
grow to either ``2^n`` (where *n* is the bitmask length) or the number
of sets stored in this collection, whichever is smaller. However, for
"typical" searches in the context of normal surface enumeration, the
running time is often significantly faster.

Precondition:
    The sets *exc1* and *exc2* are distinct, and each is contained in
    this collection precisely once.

Template parameter ``T``:
    One of Regina's bitmask types, such as Bitmask, Bitmask1 or
    Bitmask2.

Parameter ``subset``:
    the object of the query: we are searching this collection for a
    (non-strict) superset of this argument.

Parameter ``exc1``:
    the first set in the collection to be excluded from this search.

Parameter ``exc2``:
    the second set in the collection to be excluded from this search.

Parameter ``universeSize``:
    the number of elements in the underlying universe (and therefore
    the lowest possible level in the search tree). This must be less
    than or equal to the number of bits that the underlying bitmask
    type *T* can support.

Returns:
    ``True`` if a superset with the required properties was found, or
    ``False`` otherwise.)doc";

// Docstring regina::python::doc::TrieSet_::hasSubset
static const char *hasSubset =
R"doc(Determines whether this collection of sets contains any subset of the
argument *superset*. Subsets need not be *proper* subsets (so if an
exact copy of *superset* is found in the tree then this will suffice).

This routine has a slow running time, which in pathological cases can
grow to either ``2^n`` (where *n* is the bitmask length) or the number
of sets stored in this collection, whichever is smaller. However, for
"typical" searches in the context of normal surface enumeration, the
running time is often significantly faster.

Template parameter ``T``:
    One of Regina's bitmask types, such as Bitmask, Bitmask1 or
    Bitmask2.

Parameter ``superset``:
    the object of the query: we are searching this collection for a
    (non-strict) subset of this argument.

Parameter ``universeSize``:
    the number of elements in the underlying universe (and therefore
    the lowest possible level in the search tree). This must be less
    than or equal to the number of bits that the underlying bitmask
    type *T* can support.

Returns:
    ``True`` if a subset was found, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::TrieSet_::insert
static const char *insert =
R"doc(Insert the given set into this collection. The same set may be insert
into this collection multiple times (and this multiplicity will be
recorded correctly).

Running time for insertion is O(*n*), where *n* is the bitmask length.

Template parameter ``T``:
    One of Regina's bitmask types, such as Bitmask, Bitmask1 or
    Bitmask2.

Parameter ``entry``:
    the new set to insert.)doc";

// Docstring regina::python::doc::TrieSet_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given collection.

Parameter ``other``:
    the collection whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

