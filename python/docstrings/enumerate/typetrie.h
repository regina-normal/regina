/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct TypeTrie {

// Docstring regina::python::doc::TypeTrie::__class
static constexpr const char __class[] =
R"doc(A trie that stores a set of type vectors of a fixed length.

This class forms part of the tree traversal algorithm for enumerating
vertex normal surfaces, as described in "A tree traversal algorithm
for decision problems in knot theory and 3-manifold topology", Burton
and Ozlen, Algorithmica 65:4 (2013), pp. 772-801.

A type vector is a sequence of integers, each between 0 and
``nTypes-1`` inclusive. The length of a type vector must be passed
alongside it (i.e., there is no special terminating value).

A type vector *v* is said to _dominate_ *u* if, for each position *i*,
either ``v[i] == u[i]`` or else ``u[i] == 0``. So, for instance,
``(1,0,2,3)`` dominates ``(1,0,2,0)``, which in turn dominates
``(1,0,0,0)``. Domination is a partial order, not a total order: for
instance, neither of ``(1,0,2,0)`` or ``(1,0,3,0)`` dominates the
other.

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

Template parameter ``nTypes``:
    specifies the range of possible values for the elements of the
    vectors that are stored. For normal surface enumeration, typical
    values for *nTypes* would be 4 or 7 (depending upon whether we are
    supporting almost normal surfaces).

Python:
    This is available only for the template parameters *nTypes* = 4
    and 7, under the names TypeTrie4 and TypeTrie7 respectively.)doc";

// Docstring regina::python::doc::TypeTrie::__copy
static constexpr const char __copy[] =
R"doc(Creates a new copy of the given trie. This will induce a deep copy of
*src*.

Parameter ``src``:
    the trie to copy.)doc";

// Docstring regina::python::doc::TypeTrie::__default
static constexpr const char __default[] = R"doc(Creates an empty trie.)doc";

// Docstring regina::python::doc::TypeTrie::__eq
static constexpr const char __eq[] =
R"doc(Determines whether this and the given trie store exactly the same type
vectors.

Parameter ``other``:
    the trie to compare with this.

Returns:
    ``True`` if and only if both tries store the same type vectors.)doc";

// Docstring regina::python::doc::TypeTrie::clear
static constexpr const char clear[] = R"doc(Resets this to the empty trie.)doc";

// Docstring regina::python::doc::TypeTrie::dominates
static constexpr const char dominates[] =
R"doc(Determines whether the given type vector dominates any vector in this
trie. The given type vector is specified by an iterator range.

Precondition:
    Each element of the given type vector is between 0 and
    ``nTypes-1`` inclusive.

Precondition:
    The length of the type vector (``endTypes - beginTypes``) is fixed
    throughout the life of this trie; that is, it is the same for
    every call to insert() or dominates().

Python:
    Instead of a pair of iterators, you should pass the type vector as
    a single argument, which is a Python list of integers.

Parameter ``beginTypes``:
    the beginning of the type vector to test.

Parameter ``endTypes``:
    a past-the-end iterator indicating the end of the type vector to
    test.

Returns:
    ``True`` if and only if the given type vector dominates some type
    vector stored in this trie.)doc";

// Docstring regina::python::doc::TypeTrie::global_swap
static constexpr const char global_swap[] =
R"doc(Swaps the contents of the two given tries.

Parameter ``a``:
    the first trie whose contents should be swapped.

Parameter ``b``:
    the second trie whose contents should be swapped.)doc";

// Docstring regina::python::doc::TypeTrie::insert
static constexpr const char insert[] =
R"doc(Inserts the given type vector into this trie. The type vector is
specified by an iterator range.

Precondition:
    Each element of the given type vector is between 0 and
    ``nTypes-1`` inclusive.

Precondition:
    The length of the type vector (``endTypes - beginTypes``) is fixed
    throughout the life of this trie; that is, it is the same for
    every call to insert() or dominates().

Python:
    Instead of a pair of iterators, you should pass the type vector as
    a single argument, which is a Python list of integers.

Parameter ``beginTypes``:
    the beginning of the type vector to insert.

Parameter ``endTypes``:
    a past-the-end iterator indicating the end of the type vector to
    insert.)doc";

// Docstring regina::python::doc::TypeTrie::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given trie.

Parameter ``other``:
    the trie whose contents should be swapped with this.)doc";

}; // struct TypeTrie

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

