/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Qitmask1
static const char *Qitmask1 =
R"doc(A small but extremely fast "base 4 bitmask" class that can store up to
8 * sizeof(*T*) "qits", each equal to 0, 1, 2 or 3.

This qitmask packs all of the qits together into two variables of type
*T*. This means that operations on qitmasks are extremely fast,
because all of the qits can be processed in just a few native CPU
operations.

The downside of course is that the number of qits that can be stored
is limited to 8 * sizeof(*T*), where *T* must be a native unsigned
integer type (such as unsigned char, unsigned int, or unsigned long
long).

For another extremely fast qitmask class that can store twice as many
qits, see Qitmask2. At present there is no qitmask class in Regina
that can store arbitrarily many qits.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Precondition:
    Type *T* is an unsigned integral numeric type.

Python:
    Python does not support templates, and so instead Regina's python
    interface offers the classes Qitmask8, Qitmask16, Qitmask32,
    Qitmask64, Qitmask128, and (if the machine supports 128-bit
    integers) Qitmask256. Each of these will be an optimised qitmask
    class that can hold the corresponding number of bits, and is
    guaranteed to be an instance of either the C++ Qitmask1<T> class
    (where possible) or the C++ Qitmask2<T,U> template class (if
    necessary).)doc";

// Docstring regina::python::doc::Qitmask1_2
static const char *Qitmask1_2 =
R"doc(A small but extremely fast "base 4 bitmask" class that can store up to
8 * sizeof(*T*) "qits", each equal to 0, 1, 2 or 3.

This qitmask packs all of the qits together into two variables of type
*T*. This means that operations on qitmasks are extremely fast,
because all of the qits can be processed in just a few native CPU
operations.

The downside of course is that the number of qits that can be stored
is limited to 8 * sizeof(*T*), where *T* must be a native unsigned
integer type (such as unsigned char, unsigned int, or unsigned long
long).

For another extremely fast qitmask class that can store twice as many
qits, see Qitmask2. At present there is no qitmask class in Regina
that can store arbitrarily many qits.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Precondition:
    Type *T* is an unsigned integral numeric type.

Python:
    Python does not support templates, and so instead Regina's python
    interface offers the classes Qitmask8, Qitmask16, Qitmask32,
    Qitmask64, Qitmask128, and (if the machine supports 128-bit
    integers) Qitmask256. Each of these will be an optimised qitmask
    class that can hold the corresponding number of bits, and is
    guaranteed to be an instance of either the C++ Qitmask1<T> class
    (where possible) or the C++ Qitmask2<T,U> template class (if
    necessary).)doc";

// Docstring regina::python::doc::Qitmask2
static const char *Qitmask2 =
R"doc(A small but extremely fast "base 4 bitmask" class that can store up to
8 * sizeof(*T*) + 8 * sizeof(*U*) "qits", each equal to 0, 1, 2 or 3.

This qitmask packs all of the qits together into two variables of type
*T* and two variables of type *U*. This means that operations on
entire qitmasks are extremely fast, because all of the qits can be
processed in just a few native CPU operations.

The downside of course is that the number of qits that can be stored
is limited to 8 * sizeof(*T*) + 8 * sizeof(*U*), where *T* and *U*
must be native unsigned integer types (such as unsigned char, unsigned
int, or unsigned long long).

For an even faster qitmask class that can only store half as many
qits, see Qitmask1. At present there is no qitmask class in Regina
that can store arbitrarily many qits.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Precondition:
    Types *T* and *U* are unsigned integral numeric types.

Python:
    Python does not support templates, and so instead Regina's python
    interface offers the classes Qitmask8, Qitmask16, Qitmask32,
    Qitmask64, Qitmask128, and (if the machine supports 128-bit
    integers) Qitmask256. Each of these will be an optimised qitmask
    class that can hold the corresponding number of bits, and is
    guaranteed to be an instance of either the C++ Qitmask1<T> class
    (where possible) or the C++ Qitmask2<T,U> template class (if
    necessary).)doc";

// Docstring regina::python::doc::Qitmask2_2
static const char *Qitmask2_2 =
R"doc(A small but extremely fast "base 4 bitmask" class that can store up to
8 * sizeof(*T*) + 8 * sizeof(*U*) "qits", each equal to 0, 1, 2 or 3.

This qitmask packs all of the qits together into two variables of type
*T* and two variables of type *U*. This means that operations on
entire qitmasks are extremely fast, because all of the qits can be
processed in just a few native CPU operations.

The downside of course is that the number of qits that can be stored
is limited to 8 * sizeof(*T*) + 8 * sizeof(*U*), where *T* and *U*
must be native unsigned integer types (such as unsigned char, unsigned
int, or unsigned long long).

For an even faster qitmask class that can only store half as many
qits, see Qitmask1. At present there is no qitmask class in Regina
that can store arbitrarily many qits.

These objects are small enough to pass by value and swap with
std::swap(), with no need for any specialised move operations or swap
functions.

Precondition:
    Types *T* and *U* are unsigned integral numeric types.

Python:
    Python does not support templates, and so instead Regina's python
    interface offers the classes Qitmask8, Qitmask16, Qitmask32,
    Qitmask64, Qitmask128, and (if the machine supports 128-bit
    integers) Qitmask256. Each of these will be an optimised qitmask
    class that can hold the corresponding number of bits, and is
    guaranteed to be an instance of either the C++ Qitmask1<T> class
    (where possible) or the C++ Qitmask2<T,U> template class (if
    necessary).)doc";

namespace Qitmask1_ {

// Docstring regina::python::doc::Qitmask1_::Qitmask1
static const char *Qitmask1 = R"doc(Creates a new qitmask with all qits set to 0.)doc";

// Docstring regina::python::doc::Qitmask1_::Qitmask1_2
static const char *Qitmask1_2 =
R"doc(Creates a clone of the given qitmask.

Parameter ``cloneMe``:
    the qitmask to clone.)doc";

// Docstring regina::python::doc::Qitmask1_::__eq
static const char *__eq =
R"doc(Determines whether this and the given qitmask are identical.

Parameter ``other``:
    the qitmask to compare against this.

Returns:
    ``True`` if and only if this and the given qitmask are identical.)doc";

// Docstring regina::python::doc::Qitmask1_::__iadd
static const char *__iadd =
R"doc(Sets this to the sum of this and the given qitmask. Each pair of qits
is added modulo 4 (so, for instance, 2 + 3 = 1).

Parameter ``rhs``:
    the qitmask to add to this.

Returns:
    a reference to this qitmask.)doc";

// Docstring regina::python::doc::Qitmask1_::__isub
static const char *__isub =
R"doc(Sets this to the difference of this and the given qitmask. Each pair
of qits is subtracted modulo 4 (so, for instance, 1 - 3 = 2).

Parameter ``rhs``:
    the qitmask to subtract from this.

Returns:
    a reference to this qitmask.)doc";

// Docstring regina::python::doc::Qitmask1_::__ne
static const char *__ne =
R"doc(Determines whether this and the given qitmask are different.

Parameter ``other``:
    the qitmask to compare against this.

Returns:
    ``True`` if and only if this and the given qitmask are different.)doc";

// Docstring regina::python::doc::Qitmask1_::empty
static const char *empty =
R"doc(Determines whether this qitmask contains all zeroes.

Returns:
    ``True`` if every qit is zero, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Qitmask1_::get
static const char *get =
R"doc(Returns the value of the given qit in this qitmask.

Parameter ``index``:
    indicates which qit to query; this must be between 0 and (8 *
    sizeof(*T*) - 1) inclusive.

Returns:
    the value of the (*index*)th qit; this will be either 0, 1, 2 or
    3.)doc";

// Docstring regina::python::doc::Qitmask1_::has3
static const char *has3 =
R"doc(Determines whether this qitmask contains at least one qit with value
3.

Returns:
    ``True`` if at least one qit is 3, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Qitmask1_::hasNonZeroMatch
static const char *hasNonZeroMatch =
R"doc(Determines whether there is some index at which both this and the
given qitmask both have non-zero qits. That is, there is some index
*i* for which ``get(i)`` and ``other.get(i)`` are both non-zero.

Note that these two qits do not need to be equal; they just both need
to be non-zero. Note also that this only needs to happen at one index;
there may be *other* indices at which the qit is zero in one qitmask
but not the other.

Parameter ``other``:
    the qitmask to compare with this.

Returns:
    ``True`` if there is some index at which this and *other* both
    have non-zero qits, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Qitmask1_::nonEmpty
static const char *nonEmpty =
R"doc(Determines whether this qitmask contains at least one non-zero qit.

Returns:
    ``True`` if at least one qit is non-zero, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Qitmask1_::reset
static const char *reset = R"doc(Sets all qits of this qitmask to 0.)doc";

// Docstring regina::python::doc::Qitmask1_::set
static const char *set =
R"doc(Sets the given qit of this qitmask to the given value.

Parameter ``index``:
    indicates which qit to set; this must be between 0 and (8 *
    sizeof(*T*) - 1) inclusive.

Parameter ``value``:
    the value that will be assigned to the (*index*)th qit; this must
    be 0, 1, 2 or 3.)doc";

}

namespace Qitmask2_ {

// Docstring regina::python::doc::Qitmask2_::Qitmask2
static const char *Qitmask2 = R"doc(Creates a new qitmask with all qits set to 0.)doc";

// Docstring regina::python::doc::Qitmask2_::Qitmask2_2
static const char *Qitmask2_2 =
R"doc(Creates a clone of the given qitmask.

Parameter ``cloneMe``:
    the qitmask to clone.)doc";

// Docstring regina::python::doc::Qitmask2_::__eq
static const char *__eq =
R"doc(Determines whether this and the given qitmask are identical.

Parameter ``other``:
    the qitmask to compare against this.

Returns:
    ``True`` if and only if this and the given qitmask are identical.)doc";

// Docstring regina::python::doc::Qitmask2_::__iadd
static const char *__iadd =
R"doc(Sets this to the sum of this and the given qitmask. Each pair of qits
is added modulo 4 (so, for instance, 2 + 3 = 1).

Parameter ``rhs``:
    the qitmask to add to this.

Returns:
    a reference to this qitmask.)doc";

// Docstring regina::python::doc::Qitmask2_::__isub
static const char *__isub =
R"doc(Sets this to the difference of this and the given qitmask. Each pair
of qits is subtracted modulo 4 (so, for instance, 1 - 3 = 2).

Parameter ``rhs``:
    the qitmask to subtract from this.

Returns:
    a reference to this qitmask.)doc";

// Docstring regina::python::doc::Qitmask2_::__ne
static const char *__ne =
R"doc(Determines whether this and the given qitmask are different.

Parameter ``other``:
    the qitmask to compare against this.

Returns:
    ``True`` if and only if this and the given qitmask are different.)doc";

// Docstring regina::python::doc::Qitmask2_::empty
static const char *empty =
R"doc(Determines whether this qitmask contains all zeroes.

Returns:
    ``True`` if every qit is zero, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Qitmask2_::get
static const char *get =
R"doc(Returns the value of the given qit in this qitmask.

Parameter ``index``:
    indicates which qit to query; this must be between 0 and (8 *
    sizeof(*T*) + 8 * sizeof(*U*) - 1) inclusive.

Returns:
    the value of the (*index*)th qit; this will be either 0, 1, 2 or
    3.)doc";

// Docstring regina::python::doc::Qitmask2_::has3
static const char *has3 =
R"doc(Determines whether this qitmask contains at least one qit with value
3.

Returns:
    ``True`` if at least one qit is 3, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Qitmask2_::hasNonZeroMatch
static const char *hasNonZeroMatch =
R"doc(Determines whether there is some index at which both this and the
given qitmask both have non-zero qits. That is, there is some index
*i* for which ``get(i)`` and ``other.get(i)`` are both non-zero.

Note that these two qits do not need to be equal; they just both need
to be non-zero. Note also that this only needs to happen at one index;
there may be *other* indices at which the qit is zero in one qitmask
but not the other.

Parameter ``other``:
    the qitmask to compare with this.

Returns:
    ``True`` if there is some index at which this and *other* both
    have non-zero qits, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Qitmask2_::nonEmpty
static const char *nonEmpty =
R"doc(Determines whether this qitmask contains at least one non-zero qit.

Returns:
    ``True`` if at least one qit is non-zero, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Qitmask2_::reset
static const char *reset = R"doc(Sets all qits of this qitmask to 0.)doc";

// Docstring regina::python::doc::Qitmask2_::set
static const char *set =
R"doc(Sets the given qit of this qitmask to the given value.

Parameter ``index``:
    indicates which qit to set; this must be between 0 and (8 *
    sizeof(*T*) + 8 * sizeof(*U*) - 1) inclusive.

Parameter ``value``:
    the value that will be assigned to the (*index*)th qit; this must
    be 0, 1, 2 or 3.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

