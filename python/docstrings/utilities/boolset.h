/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct BoolSet {

// Docstring regina::python::doc::BoolSet::__band
static constexpr const char __band[] =
R"doc(Returns the intersection of this set with the given set. The result is
a set containing precisely the elements that belong to both original
sets. This set is not changed.

Parameter ``other``:
    the set to intersect with this set.

Returns:
    the intersection of this and the given set.)doc";

// Docstring regina::python::doc::BoolSet::__bnot
static constexpr const char __bnot[] =
R"doc(Returns the complement of this set. The result is a set containing
precisely the elements that this set does not contain. This set is not
changed.

Returns:
    the complement of this set.)doc";

// Docstring regina::python::doc::BoolSet::__bor
static constexpr const char __bor[] =
R"doc(Returns the union of this set with the given set. The result is a set
containing precisely the elements that belong to either of the
original sets. This set is not changed.

Parameter ``other``:
    the set to union with this set.

Returns:
    the union of this and the given set.)doc";

// Docstring regina::python::doc::BoolSet::__bxor
static constexpr const char __bxor[] =
R"doc(Returns the symmetric difference of this set and the given set. The
result is a set containing precisely the elements that belong to one
but not both of the original sets. This set is not changed.

Parameter ``other``:
    the set whose symmetric difference with this set is to be found.

Returns:
    the symmetric difference of this and the given set.)doc";

// Docstring regina::python::doc::BoolSet::__class
static constexpr const char __class[] =
R"doc(A set of booleans. Note that there are only four possible such sets.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.)doc";

// Docstring regina::python::doc::BoolSet::__cmp
static constexpr const char __cmp[] =
R"doc(Compares two sets under the subset relation.

This generates all of the usual comparison operators, including ``<``,
``<=``, ``>``, and ``>=``.

Python:
    This spaceship operator ``x <=> y`` is not available, but the
    other comparison operators that it generates _are_ available.

Parameter ``rhs``:
    the set to compare with this.

Returns:
    the result of the subset comparison between this and the given
    set.)doc";

// Docstring regina::python::doc::BoolSet::__copy
static constexpr const char __copy[] = R"doc(Creates a clone of the given set.)doc";

// Docstring regina::python::doc::BoolSet::__default
static constexpr const char __default[] = R"doc(Creates a new empty set.)doc";

// Docstring regina::python::doc::BoolSet::__eq
static constexpr const char __eq[] =
R"doc(Determines if this set is equal to the given set.

Returns:
    ``True`` if and only if this and the given set are equal.)doc";

// Docstring regina::python::doc::BoolSet::__iand
static constexpr const char __iand[] =
R"doc(Sets this set to be the intersection of this and the given set. The
result is a set containing precisely the elements that belong to both
original sets. Note that this set will be modified.

Parameter ``other``:
    the set to intersect with this set.

Returns:
    a reference to this set.)doc";

// Docstring regina::python::doc::BoolSet::__init
static constexpr const char __init[] =
R"doc(Creates a set containing a single boolean, which is passed as an
argument.

Parameter ``member``:
    the single element to include in this set.)doc";

// Docstring regina::python::doc::BoolSet::__init_2
static constexpr const char __init_2[] =
R"doc(Creates a set specifying whether ``True`` and/or ``False`` should be a
member.

Parameter ``insertTrue``:
    should the new set include the element ``True``?

Parameter ``insertFalse``:
    should the new set include the element ``False``?)doc";

// Docstring regina::python::doc::BoolSet::__ior
static constexpr const char __ior[] =
R"doc(Sets this set to be the union of this and the given set. The result is
a set containing precisely the elements that belong to either of the
original sets. Note that this set will be modified.

Parameter ``other``:
    the set to union with this set.

Returns:
    a reference to this set.)doc";

// Docstring regina::python::doc::BoolSet::__ixor
static constexpr const char __ixor[] =
R"doc(Sets this set to be the symmetric difference of this and the given
set. The result is a set containing precisely the elements that belong
to one but not both of the original sets. Note that this set will be
modified.

Parameter ``other``:
    the set whose symmetric difference with this set is to be found.

Returns:
    a reference to this set.)doc";

// Docstring regina::python::doc::BoolSet::byteCode
static constexpr const char byteCode[] =
R"doc(Returns the byte code representing this boolean set. The byte code is
sufficient to reconstruct the set and is thus a useful means for
passing boolean sets to and from the engine.

The lowest order bit of the byte code is 1 if and only if ``True`` is
in the set. The next lowest order bit is 1 if and only if ``False`` is
in the set. All other bits are 0. Therefore sets {}, {true}, {false}
and {true, false} have byte codes 0, 1, 2 and 3 respectively.

Returns:
    the byte code representing this set.)doc";

// Docstring regina::python::doc::BoolSet::clear
static constexpr const char clear[] =
R"doc(Removes all elements from this set.

In Regina 7.3 and earlier, this routine was called empty(). It has
been renamed to clear(), _without_ a deprecated alias, to avoid
confusion with the more common pattern where empty() queries whether a
container holds any elements at all.)doc";

// Docstring regina::python::doc::BoolSet::contains
static constexpr const char contains[] =
R"doc(Determines if the given boolean is a member of this set.

Parameter ``value``:
    the boolean to search for in this set.

Returns:
    ``True`` if and only if the given boolean is a member of this set.)doc";

// Docstring regina::python::doc::BoolSet::fill
static constexpr const char fill[] =
R"doc(Places both ``True`` and ``False`` into this set if they are not
already present.)doc";

// Docstring regina::python::doc::BoolSet::fromByteCode
static constexpr const char fromByteCode[] =
R"doc(Creates a boolean set from the given byte code. See byteCode() for
more information on byte codes.

Exception ``InvalidArgument``:
    The given argument was not a valid byte code (i.e., it was not 0,
    1, 2 or 3).

Parameter ``code``:
    the byte code from which the new set will be created.)doc";

// Docstring regina::python::doc::BoolSet::fromStringCode
static constexpr const char fromStringCode[] =
R"doc(Creates a boolean set from the given string code. See stringCode() for
more information on string codes.

Both upper-case and lower-case codes (or a mix of the two) are
accepted by this routine.

Exception ``InvalidArgument``:
    The given argument was not a valid string code.

Parameter ``code``:
    the string code from which the new set will be created.)doc";

// Docstring regina::python::doc::BoolSet::full
static constexpr const char full[] =
R"doc(Determines whether this is the full set, containing both ``True`` and
``False``.

Returns:
    ``True`` if and only if this is the full set.)doc";

// Docstring regina::python::doc::BoolSet::hasFalse
static constexpr const char hasFalse[] =
R"doc(Determines if ``False`` is a member of this set.

Returns:
    ``True`` if and only if ``False`` is a member of this set.)doc";

// Docstring regina::python::doc::BoolSet::hasTrue
static constexpr const char hasTrue[] =
R"doc(Determines if ``True`` is a member of this set.

Returns:
    ``True`` if and only if ``True`` is a member of this set.)doc";

// Docstring regina::python::doc::BoolSet::insertFalse
static constexpr const char insertFalse[] = R"doc(Inserts ``False`` into this set if it is not already present.)doc";

// Docstring regina::python::doc::BoolSet::insertTrue
static constexpr const char insertTrue[] = R"doc(Inserts ``True`` into this set if it is not already present.)doc";

// Docstring regina::python::doc::BoolSet::removeFalse
static constexpr const char removeFalse[] = R"doc(Removes ``False`` from this set if it is present.)doc";

// Docstring regina::python::doc::BoolSet::removeTrue
static constexpr const char removeTrue[] = R"doc(Removes ``True`` from this set if it is present.)doc";

// Docstring regina::python::doc::BoolSet::setByteCode
static constexpr const char setByteCode[] =
R"doc(Deprecated routine that sets this to be the boolean set represented by
the given byte code. See byteCode() for more information on byte
codes.

If *code* is not a valid byte code, then this routine will do nothing
and return ``False``.

.. deprecated::
    Instead of ``b.setByteCode(code)``, use ``b =
    BoolSet.fromByteCode(code)``. Note that fromByteCode() will report
    any errors by throwing an exception.

Parameter ``code``:
    the byte code that will determine the new value of this set.

Returns:
    ``True`` if and only if ``code`` is a valid byte code.)doc";

// Docstring regina::python::doc::BoolSet::setStringCode
static constexpr const char setStringCode[] =
R"doc(Deprecated routine that sets this to be the boolean set represented by
the given string code. See stringCode() for more information on string
codes.

Both upper-case and lower-case codes (or a mix of the two) are
accepted by this routine.

If *code* is not a valid string code, then this routine will do
nothing and return ``False``.

.. deprecated::
    Instead of ``b.setStringCode(code)``, use ``b =
    BoolSet.fromStringCode(code)``. Note that fromStringCode() will
    report any errors by throwing an exception.

Parameter ``code``:
    the string code that will determine the new value of this set.

Returns:
    ``True`` if and only if ``code`` is a valid string code.)doc";

// Docstring regina::python::doc::BoolSet::stringCode
static constexpr const char stringCode[] =
R"doc(Returns the string code representing this boolean set. String codes
are a more human-readable alternative to byte codes; in particular,
they are used in XML data files.

Every string code contains precisely two characters (plus a
terminating null). The sets ``{}``, ``{true}``, ``{false}`` and
``{true, false}`` have string codes ``--``, ``T-``, ``-F`` and ``TF``
respectively.

Returns:
    the two-character string code representing this set. Any letters
    in this code will be upper-case.)doc";

}; // struct BoolSet

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

