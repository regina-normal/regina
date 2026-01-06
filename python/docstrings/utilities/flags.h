/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Flags
constexpr const char *Flags =
R"doc(A class representing a bitwise combination of flags defined by an
enumeration type.

The enumeration type is given in the template parameter *T*. This
class allows the user to form and test bitwise combinations of the
individual enum values, without losing type safety.

There is usually no need for end users to refer to the type
``Flags<T>`` explicitly by name. If a function takes an argument of
type ``Flags<T>``, then you can pass a single flag of type *T*, or a
bitwise combination of such flags ``(flag1 | flag2)``, or empty braces
``{}`` to indicate no flags at all.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Python:
    Present only for some particular enumeration types *T*, when
    explicitly noted in the corresponding enum documentation. The
    Python name of the flags class will be the enumeration type name
    with the prefix ``Flags_``; for instance, the C++ type
    ``Flags<NormalAlg>`` will appear in Python as ``Flags_NormalAlg``.

Template parameter ``T``:
    the enumeration type holding the individual flags that can be
    combined. This may be a scoped or unscoped enumeration.)doc";

namespace Flags_ {

// Docstring regina::python::doc::Flags_::__as_bool
constexpr const char *__as_bool =
R"doc(Determines whether this flag set is non-empty.

An empty flag set has no bits set at all.

Returns:
    ``True`` if and only if this flag set is non-empty.)doc";

// Docstring regina::python::doc::Flags_::__band
constexpr const char *__band =
R"doc(Returns the bitwise AND of this set and the given flag. This flag set
is not changed.

Parameter ``rhs``:
    the flag to combine with this set.

Returns:
    the combination of this set and the given flag.)doc";

// Docstring regina::python::doc::Flags_::__band_2
constexpr const char *__band_2 =
R"doc(Returns the bitwise AND of this and the given flag set. This flag set
is not changed.

Parameter ``rhs``:
    the flag set to combine with this set.

Returns:
    the combination of this and the given flag set.)doc";

// Docstring regina::python::doc::Flags_::__bor
constexpr const char *__bor =
R"doc(Returns the bitwise OR of this set and the given flag. This flag set
is not changed.

Parameter ``rhs``:
    the flag to combine with this set.

Returns:
    the combination of this set and the given flag.)doc";

// Docstring regina::python::doc::Flags_::__bor_2
constexpr const char *__bor_2 =
R"doc(Returns the bitwise OR of this and the given flag set. This flag set
is not changed.

Parameter ``rhs``:
    the flag set to combine with this set.

Returns:
    the combination of this and the given flag set.)doc";

// Docstring regina::python::doc::Flags_::__bxor
constexpr const char *__bxor =
R"doc(Returns the bitwise XOR of this set and the given flag. This flag set
is not changed.

Parameter ``rhs``:
    the flag to combine with this set.

Returns:
    the combination of this set and the given flag.)doc";

// Docstring regina::python::doc::Flags_::__bxor_2
constexpr const char *__bxor_2 =
R"doc(Returns the bitwise XOR of this and the given flag set. This flag set
is not changed.

Parameter ``rhs``:
    the flag set to combine with this set.

Returns:
    the combination of this and the given flag set.)doc";

// Docstring regina::python::doc::Flags_::__copy
constexpr const char *__copy = R"doc(Creates a clone of the given flag set.)doc";

// Docstring regina::python::doc::Flags_::__default
constexpr const char *__default = R"doc(Creates an empty flag set, with no flags set at all.)doc";

// Docstring regina::python::doc::Flags_::__eq
constexpr const char *__eq =
R"doc(Determines whether this set is precisely equal to the given flag.

Parameter ``rhs``:
    the flag to test this against.

Returns:
    ``True`` if and only if this and the given flag are identical.)doc";

// Docstring regina::python::doc::Flags_::__eq_2
constexpr const char *__eq_2 =
R"doc(Determines whether this set is precisely equal to the given flag set.

Returns:
    ``True`` if and only if this and the given flag set are identical.)doc";

// Docstring regina::python::doc::Flags_::__iand
constexpr const char *__iand =
R"doc(Changes this flag set by taking a bitwise AND with the given flag.

Parameter ``rhs``:
    the flag to combine with this set.

Returns:
    a reference to this flag set.)doc";

// Docstring regina::python::doc::Flags_::__iand_2
constexpr const char *__iand_2 =
R"doc(Changes this flag set by taking a bitwise AND with the given flag set.

Parameter ``rhs``:
    the flag set to combine with this set.

Returns:
    a reference to this flag set.)doc";

// Docstring regina::python::doc::Flags_::__init
constexpr const char *__init =
R"doc(Creates a flag set initialised to the given value.

Parameter ``init``:
    the initial value of this flag set.)doc";

// Docstring regina::python::doc::Flags_::__ior
constexpr const char *__ior =
R"doc(Changes this flag set by taking a bitwise OR with the given flag.

Parameter ``rhs``:
    the flag to combine with this set.

Returns:
    a reference to this flag set.)doc";

// Docstring regina::python::doc::Flags_::__ior_2
constexpr const char *__ior_2 =
R"doc(Changes this flag set by taking a bitwise OR with the given flag set.

Parameter ``rhs``:
    the flag set to combine with this set.

Returns:
    a reference to this flag set.)doc";

// Docstring regina::python::doc::Flags_::__ixor
constexpr const char *__ixor =
R"doc(Changes this flag set by taking a bitwise XOR with the given flag.

Parameter ``rhs``:
    the flag to combine with this set.

Returns:
    a reference to this flag set.)doc";

// Docstring regina::python::doc::Flags_::__ixor_2
constexpr const char *__ixor_2 =
R"doc(Changes this flag set by taking a bitwise XOR with the given flag set.

Parameter ``rhs``:
    the flag set to combine with this set.

Returns:
    a reference to this flag set.)doc";

// Docstring regina::python::doc::Flags_::baseValue
constexpr const char *baseValue =
R"doc(Returns the native C++ integer representation of this set. This is
suitable for file input and/or output.

.. warning::
    This function should not be used widely, since it effectively
    works around inbuilt type safety mechanisms.

Returns:
    the native C++ integer value of this set.)doc";

// Docstring regina::python::doc::Flags_::clear
constexpr const char *clear =
R"doc(Clears all bits from this set that appear in the given flag.

Parameter ``rhs``:
    the flag to clear from this set.)doc";

// Docstring regina::python::doc::Flags_::clear_2
constexpr const char *clear_2 =
R"doc(Clears all bits from this set that appear in the given set.

Parameter ``rhs``:
    identifies the bits to clear from this set.)doc";

// Docstring regina::python::doc::Flags_::ensureOne
constexpr const char *ensureOne =
R"doc(Adjust this set so that exactly one and only one of the two given
flags are included.

If neither flag is present or both flags are present, this set will be
adjusted so that *default_* is present and *other* is not.

Precondition:
    Both *default_* and *other* are each single-bit flags.

Parameter ``default_``:
    the flag that will be set if any adjustments need to be made.

Parameter ``other``:
    the flag that will be cleared if any adjustments need to be made.)doc";

// Docstring regina::python::doc::Flags_::ensureOne_2
constexpr const char *ensureOne_2 =
R"doc(Adjust this set so that exactly one and only one of the three given
flags are included.

If neither flag is present, then *default_* will be used. If multiple
flags are present, then the flag that appears _earlier_ in the
argument list for this routine will be used.

Precondition:
    Each of the given flags is single-bit.

Parameter ``default_``:
    the highest-priority flag.

Parameter ``second``:
    the second-highest-priority flag.

Parameter ``last``:
    the lowest-priority flag.)doc";

// Docstring regina::python::doc::Flags_::ensureOne_3
constexpr const char *ensureOne_3 =
R"doc(Adjust this set so that exactly one and only one of the four given
flags are included.

If neither flag is present, then *default_* will be used. If multiple
flags are present, then the flag that appears _earlier_ in the
argument list for this routine will be used.

Precondition:
    Each of the given flags is single-bit.

Parameter ``default_``:
    the highest-priority flag.

Parameter ``second``:
    the second-highest-priority flag.

Parameter ``third``:
    the third-highest-priority flag.

Parameter ``last``:
    the lowest-priority flag.)doc";

// Docstring regina::python::doc::Flags_::fromBase
constexpr const char *fromBase =
R"doc(Returns the set corresponding to the given native C++ integer value.
This is suitable for file input and/or output.

.. warning::
    This function should not be used widely, since it effectively
    works around inbuilt type safety mechanisms.

Returns:
    the set corresponding to the given native C++ integer value.)doc";

// Docstring regina::python::doc::Flags_::fromInt
constexpr const char *fromInt =
R"doc(Deprecated function that returns the set corresponding to the given
integer value. This is suitable for file input and/or output.

.. deprecated::
    Use fromBase() instead, which makes no assumptions about the
    underlying native C++ integer type.

.. warning::
    This function should not be used widely, since it effectively
    works around inbuilt type safety mechanisms.

Returns:
    the set corresponding to the given integer value.)doc";

// Docstring regina::python::doc::Flags_::has
constexpr const char *has =
R"doc(Returns whether the given flag is set.

This requires _all_ of the bits of the given flag to be set. The test
is equivalent to ``(*this & flag) == flag``.

Parameter ``flag``:
    the flag whose presence will be tested.

Returns:
    ``True`` if and only if all of the bits of the given flag are set.)doc";

// Docstring regina::python::doc::Flags_::has_2
constexpr const char *has_2 =
R"doc(Returns whether all of the flags in the given set are set.

This requires _all_ of the bits of all of the flags in the given set
to be present in this set. The test is equivalent to ``(*this & rhs)
== rhs``.

Parameter ``rhs``:
    the set whose presence will be tested.

Returns:
    ``True`` if and only if all of the bits of the given set are
    present in this set.)doc";

// Docstring regina::python::doc::Flags_::intValue
constexpr const char *intValue =
R"doc(Deprecated function that returns the integer representation of this
set. This is suitable for file input and/or output.

.. deprecated::
    Use baseValue() instead, which makes no assumptions about the
    underlying native C++ integer type.

.. warning::
    This function should not be used widely, since it effectively
    works around inbuilt type safety mechanisms.

Returns:
    the integer value of this set.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

