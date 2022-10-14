/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::TypeTrie
static const char *TypeTrie =
R"doc(A trie that stores a set of type vectors of a fixed length.

This class forms part of the tree traversal algorithm for enumerating
vertex normal surfaces, as described in "A tree traversal algorithm
for decision problems in knot theory and 3-manifold topology", Burton
and Ozlen, Algorithmica 65:4 (2013), pp. 772-801.

A type vector is a sequence of digits, each between 0 and *nTypes*-1
inclusive. Type vectors are represented as arrays of characters: these
are not strings, but simply sequences of one-byte integers. In
particular, you cannot print them (since they use raw integer values,
not ASCII digits). The length of a type vector must be passed
alongside it (i.e., there is no special terminating character).

A type vector *v* is said to *dominate* *u* if, for each position *i*,
either v[i] == u[i] or else u[i] == 0. So, for instance, (1,0,2,3)
dominates (1,0,2,0), which in turn dominates (1,0,0,0). Domination is
a partial order, not a total order: for instance, neither of (1,0,2,0)
or (1,0,3,0) dominates the other.

We assume that all type vectors used in this trie have the same
length. This is important, since we optimise the implementation by
ignoring trailing zeroes, which means that this trie cannot
distinguish between a vector *v* and the same vector with additional
zeroes appended to its end.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value. However, be
aware that the cost of moving is linear in the template parameter
*nTypes* (which, as noted below, is usually very small).

Precondition:
    *nTypes* is between 1 and 256 inclusive. The typical value for
    *nTypes* for normal surface enumeration is either 4 or 7
    (depending upon whether we are supporting almost normal surfaces).

Python:
    This is available only for the template parameters *nTypes* = 4
    and 7, under the names TypeTrie4 and TypeTrie7 respectively.)doc";

namespace TypeTrie_ {

// Docstring regina::python::doc::TypeTrie_::__eq
static const char *__eq =
R"doc(Determines whether this and the given trie store exactly the same type
vectors.

Parameter ``other``:
    the trie to compare with this.

Returns:
    ``True`` if and only if both tries store the same type vectors.)doc";

// Docstring regina::python::doc::TypeTrie_::__init
static const char *__init = R"doc(Creates an empty trie.)doc";

// Docstring regina::python::doc::TypeTrie_::__init_2
static const char *__init_2 =
R"doc(Creates a new copy of the given trie. This will induce a deep copy of
*src*.

Parameter ``src``:
    the trie to copy.)doc";

// Docstring regina::python::doc::TypeTrie_::__ne
static const char *__ne =
R"doc(Determines whether this and the given trie do not store exactly the
same type vectors.

Parameter ``other``:
    the trie to compare with this.

Returns:
    ``True`` if and only if both tries do not store the same type
    vectors.)doc";

// Docstring regina::python::doc::TypeTrie_::clear
static const char *clear = R"doc(Resets this to the empty trie.)doc";

// Docstring regina::python::doc::TypeTrie_::dominates
static const char *dominates =
R"doc(Determines whether the given type vector dominates any vector in this
trie.

Precondition:
    The given length *len* is non-zero, and is fixed throughout the
    life of this trie; that is, it is the same every time insert() or
    dominates() is called.

Python:
    Instead of the arguments *entry* and *len*, you should pass a
    single argument which is a python sequence of length *len*. This
    list should be a type vector, and each list element should be
    between 0 and (*nTypes* - 1) inclusive.

Parameter ``vec``:
    the type vector to test.

Parameter ``len``:
    the number of elements in the given type vector.

Returns:
    ``True`` if and only if *vec* dominates some type vector stored in
    this trie.)doc";

// Docstring regina::python::doc::TypeTrie_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given tries.

Parameter ``a``:
    the first trie whose contents should be swapped.

Parameter ``b``:
    the second trie whose contents should be swapped.)doc";

// Docstring regina::python::doc::TypeTrie_::insert
static const char *insert =
R"doc(Inserts the given type vector into this trie.

Precondition:
    The given length *len* is non-zero, and is fixed throughout the
    life of this trie; that is, it is the same every time insert() or
    dominates() is called.

Python:
    Instead of the arguments *entry* and *len*, you should pass a
    single argument which is a python sequence of length *len*. This
    list should be a type vector, and each list element should be
    between 0 and (*nTypes* - 1) inclusive.

Parameter ``entry``:
    the type vector to insert.

Parameter ``len``:
    the number of elements in the given type vector.)doc";

// Docstring regina::python::doc::TypeTrie_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given trie.

Parameter ``other``:
    the trie whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

