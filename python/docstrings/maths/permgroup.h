/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::NamedPermGroup
static const char *NamedPermGroup =
R"doc(Constants that represent particular well-known classes of permutation
groups.

These constants are intended to be used with permutation groups on *n*
elements for arbitrary *n*. (In particular, you can pass them to the
PermGroup<n> constructor.))doc";

// Docstring regina::python::doc::PermGroup
static const char *PermGroup =
R"doc(Represents a group of permutations on *n* elements. This is a subgroup
of the symmetric group ``S_n``.

Groups are stored internally using Sims tables (see Knuth volume 4A
for a description of how these work); these are called _stabiliser
chains_ in many places. This storage mechanism means that, even though
a permutation group could have size factorial in *n*, the storage
space required is only quadratic in *n*.

PermGroup objects are, in their current implementation, entirely
stack-based. This means they cannot support fast move or swap
operations. However, since their size is quadratic in *n*, copy
operations involve significantly more overhead than (for example) just
copying a Perm object (which just holds a single machine-native
integer). This decision is a deliberate trade-off between speed versus
space; the implication for end users is that you should be economical
about copying PermGroup objects, and work with them in-place where
possible.

Python:
    Python does not support templates. In Python, the "vanilla" non-
    cached variants ``Perm<n>`` are available under the names
    PermGroup2, PermGroup3, ..., PermGroup16, and the cached variants
    ``Perm<n, true>`` are available under the names PermGroup2_Cached,
    PermGroup3_Cached, ..., PermGroup16_Cached.

Template parameter ``n``:
    the number of objects being permuted. This must be between 2 and
    16 inclusive.

Template parameter ``cached``:
    ``True`` if we should use precomputation-assisted routines such as
    Perm<n>::cachedComp() and Perm<n>::cachedInverse(), or ``False``
    (the default) if we should just use the composition operator,
    inverse(), and so on. If this argument is ``True``, you _must_
    have called Perm<n>::precompute() at least once in the lifetime of
    the program before using this class.)doc";

namespace NamedPermGroup_ {

// Docstring regina::python::doc::NamedPermGroup_::PERM_GROUP_ALTERNATING
static const char *PERM_GROUP_ALTERNATING =
R"doc(Represents the alternating group on *n* elements, containing all
``n!/2`` even permutations.)doc";

// Docstring regina::python::doc::NamedPermGroup_::PERM_GROUP_SYMMETRIC
static const char *PERM_GROUP_SYMMETRIC =
R"doc(Represents the symmetric group on *n* elements, containing all ``n!``
possible permutations.)doc";

// Docstring regina::python::doc::NamedPermGroup_::PERM_GROUP_TRIVIAL
static const char *PERM_GROUP_TRIVIAL =
R"doc(Represents the trivial group on *n* elements, containing only the
identity permutation.)doc";

}

namespace PermGroup_ {

// Docstring regina::python::doc::PermGroup_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given group.

Parameter ``src``:
    the group to copy.)doc";

// Docstring regina::python::doc::PermGroup_::__default
static const char *__default =
R"doc(Constructs the trivial group, containing only the identity
permutation.)doc";

// Docstring regina::python::doc::PermGroup_::__eq
static const char *__eq =
R"doc(Indicates whether this and the given group are identical.

This does _not_ test group isomorphism, and it does _not_ test whether
the two groups use the same internal representation. Instead it tests
_membership_; that is, whether or not the two groups contain precisely
the same set of permutations.

As a result, this test is not trivial. It _is_ small polynomial time
in *n*, but it is not as fast as (for example) directly comparing the
internal representations.

Parameter ``other``:
    the group to compare this with.

Returns:
    ``True`` if and only if this and the given group contain the same
    permutations.)doc";

// Docstring regina::python::doc::PermGroup_::__init
static const char *__init =
R"doc(Construct the given well-known permutation group. This constructor can
(for example) be used to easily construct the symmetric or alternating
group on *n* elements.

Parameter ``group``:
    indicates which well-known permutation group to construct.)doc";

// Docstring regina::python::doc::PermGroup_::__init_2
static const char *__init_2 =
R"doc(Constructs the symmetric group ``S_k``, formed from all permutations
of 1,...,*k*. The elements (*k* + 1),...,*n* will remain fixed under
all permutations in this group.

The size of this group will be ``k!``.

Parameter ``k``:
    indicates how many elements should be permuted; this must be
    between 0 and *n* inclusive.)doc";

// Docstring regina::python::doc::PermGroup_::__init_3
static const char *__init_3 =
R"doc(Generates the subgroup of all elements in the given group that pass
the given membership test.

Specifically, this generates the subgroup of all permutations *p* in
*parent* for which ``test(p, args...)`` returns ``True``.

The argument *test* should be a function or some other callable
object. It must return a boolean, and its first argument should be a
permutation (either by value as type ``Perm<n>``, or by const
reference as type ``const Perm<n>&``). If there are any additional
arguments supplied in the list *args*, these will be forwarded through
as additional arguments to *test*.

Note that *test* will not necessarily be called for _all_ permutations
in *parent*, since this routine will deduce some subgroup members
using the standard subgroup properties (e.g., closure and inverse). It
is, however, guaranteed that the _only_ permutations passed to *test*
will be permutations that are already known to belong to *parent*.

Precondition:
    The given membership test does actually define a subgroup (that
    is, it behaves appropriately with respect to identity, inverse and
    closure).

Python:
    This constructor is available in Python, and the *test* argument
    may be a pure Python function. However, its form is more
    restricted: *test* must take exactly one argument (the
    permutation), and the *args* argument to this constructor is not
    present.

Parameter ``parent``:
    the "starting" group of all permutations under consideration.

Parameter ``test``:
    a function (or other callable object) that determines which
    permutations in *parent* become members of this subgroup.

Parameter ``args``:
    any additional arguments that should be passed to *test*,
    following the initial permutation argument.)doc";

// Docstring regina::python::doc::PermGroup_::__iter__
static const char *__iter__ =
R"doc(Returns a Python iterator over the elements of this group.

The order of iteration is arbitrary, and may change in future releases
of Regina.

Returns:
    an iterator over the elements of this group.)doc";

// Docstring regina::python::doc::PermGroup_::__ne
static const char *__ne =
R"doc(Indicates whether this and the given group are different.

This does _not_ test group isomorphism, and it does _not_ test whether
the two groups use the same internal representation. Instead it tests
_membership_; that is, whether or not the two groups contain precisely
the same set of permutations.

As a result, this test is not trivial. It _is_ small polynomial time
in *n*, but it is not as fast as (for example) directly comparing the
internal representations.

Parameter ``other``:
    the group to compare this with.

Returns:
    ``True`` if and only if there is some permutation that belongs to
    one group but not the other.)doc";

// Docstring regina::python::doc::PermGroup_::centraliser
static const char *centraliser =
R"doc(Returns the group of all permutations that fix the minimal
representative of the given conjugacy class under conjugation.

Specifically, if *r* is the minimal representative of the given class
as returned by ``conj.rep()``, then this routine constructs the
subgroup of all permutations *p* for which ``p.inverse() * r * p ==
r``.

.. warning::
    While "most" such centraliser groups are small, they _could_ get
    very large. For example, if *conj* represents the identity
    permutation, then the centraliser will be all of S_n. For *n* ≥ 5,
    it can be show that the next-worst case is where *conj* represents
    a single pair swap, in which case the centraliser has size
    ``2⋅(n-2)!``.

Precondition:
    *conj* is not the past-the-end conjugacy class.

Returns:
    the group of all permutations that leave rep() fixed under
    conjugation.)doc";

// Docstring regina::python::doc::PermGroup_::contains
static const char *contains =
R"doc(Determines whether the given permutation belongs to this group.

Regardless of the size of this group, the running time for this
routine is small polynomial in *n*.

Parameter ``p``:
    the permutation whose membership we wish to test.

Returns:
    ``True`` if and only if *p* belongs to this group.)doc";

// Docstring regina::python::doc::PermGroup_::iterator
static const char *iterator =
R"doc(The iterator type for this group.

Unlike most iterator types, the dereference operator for this iterator
type returns by value (not by reference). This is because the
individual permutations in a group are generated (not stored), based
upon an internal group representation that is typically _much_ smaller
than the group itself.

Both *iterator* and *const_iterator* are the same type, since a
PermGroup only offers read-only access to its group members.)doc";

// Docstring regina::python::doc::PermGroup_::size
static const char *size =
R"doc(Returns the total number of elements in this group.

Returns:
    the size of this group.)doc";

}

namespace PermGroup_::iterator_ {

// Docstring regina::python::doc::PermGroup_::iterator_::__eq
static const char *__eq =
R"doc(Compares this with the given iterator for equality.

To be considered equal, two iterators must be pointing to the same
permutation within the same group. (The second condition means that
the underlying PermGroup pointers must be the same - it is not enough
to have two distinct PermGorup objects with identical contents.)

Two past-the-end iterators will always be considered equal.

Parameter ``rhs``:
    the iterator to compare this with.

Returns:
    ``True`` if the iterators point to the same permutation, or
    ``False`` if they do not.)doc";

// Docstring regina::python::doc::PermGroup_::iterator_::__ne
static const char *__ne =
R"doc(Compares this with the given iterator for inequality.

To be considered equal, two iterators must be pointing to the same
permutation within the same group. (The second condition means that
the underlying PermGroup pointers must be the same - it is not enough
to have two distinct PermGorup objects with identical contents.)

Two past-the-end iterators will always be considered equal.

Parameter ``rhs``:
    the iterator to compare this with.

Returns:
    ``False`` if the iterators point to the same permutation, or
    ``True`` if they do not.)doc";

// Docstring regina::python::doc::PermGroup_::iterator_::__next__
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

