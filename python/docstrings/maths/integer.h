/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::InfinityBase
static const char *InfinityBase =
R"doc(An internal base class inherited by LargeInteger, which provides
support for infinity as an allowed value.

End users should not use this class directly.)doc";

// Docstring regina::python::doc::InfinityBase_2
static const char *InfinityBase_2 =
R"doc(An empty internal base class inherited by Integer, which does not
support infinity as an allowed value.)doc";

// Docstring regina::python::doc::IntegerBase
static const char *IntegerBase =
R"doc(Represents an arbitrary precision integer. Calculations are always
guaranteed to be exact, regardless of how large the integers become.

The current implementation uses fast native integer arithmetic
wherever possible, whilst always testing for potential overflow. If a
potential overflow is detected, this class switches to using the GNU
multiple precision arithmetic library (libgmp) instead.

This class takes a single boolean argument *supportInfinity*. If this
is ``True``, then this class will support infinity as an allowed
value. If this is ``False`` (the default), then infinity is not
supported, and any attempt to work with infinity will lead to
undefined behaviour. Supporting infinity is more flexible, but also
comes with a slight performance cost (very roughly estimated at around
10%-20%).

For the purposes of comparison, infinity is considered larger than any
other integer but equal to itself.

All routines in this class, including random number generation, are
thread-safe.

The opportunistic use of native arithmetic where possible was inspired
by the (much more complex and powerful) lazy exact arithmetic in CGAL.
Thanks to Menelaos Karavelas for encouraging me to take another look
at these ideas.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

Python:
    Both variants of this template are available through Python. For
    *supportInfinity* = ``False``, use the name Integer. For
    *supportInfinity* = ``True``, use the name LargeInteger.)doc";

// Docstring regina::python::doc::__add
static const char *__add =
R"doc(Adds the given native integer to the given large integer. If the large
integer is infinite, the result will also be infinity.

Parameter ``lhs``:
    the native integer to add.

Parameter ``rhs``:
    the large integer to add.

Returns:
    the sum *lhs* plus *rhs*.)doc";

// Docstring regina::python::doc::__mul
static const char *__mul =
R"doc(Multiplies the given native integer with the given large integer. If
the large integer is infinite, the result will also be infinity.

Parameter ``lhs``:
    the native integer to multiply.

Parameter ``rhs``:
    the large integer to multiply.

Returns:
    the product *lhs* times *rhs*.)doc";

// Docstring regina::python::doc::tightEncoding
static const char *tightEncoding =
R"doc(Returns the tight encoding of the given arbitrary precision integer.
See the page on tight encodings for details.

This global function does the same thing as the member function
IntegerBase::tightEncoding(). However, this global function is more
efficient if the integer argument is an rvalue reference (since the
const member function induces an extra deep copy).

Parameter ``value``:
    the integer to encode.

Returns:
    the resulting encoded string.)doc";

namespace IntegerBase_ {

// Docstring regina::python::doc::IntegerBase_::__add
static const char *__add =
R"doc(Adds this to the given integer and returns the result. This integer is
not changed.

If either term of the sum is infinite, the result will be infinity.

Parameter ``other``:
    the integer to add to this integer.

Returns:
    the sum *this* plus *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::__add_2
static const char *__add_2 =
R"doc(Adds this to the given integer and returns the result. This integer is
not changed.

If either term of the sum is infinite, the result will be infinity.

Parameter ``other``:
    the integer to add to this integer.

Returns:
    the sum *this* plus *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::__copy
static const char *__copy =
R"doc(Initialises this integer to the given value.

Parameter ``value``:
    the new value of this integer.)doc";

// Docstring regina::python::doc::IntegerBase_::__dec
static const char *__dec =
R"doc(The postdecrement operator. This operator decrements this integer by
one, and returns a copy of the integer _before_ the decrement.

Python:
    This routine is named dec() since python does not support the
    decrement operator.

Returns:
    a copy of this integer before the decrement took place.)doc";

// Docstring regina::python::doc::IntegerBase_::__default
static const char *__default = R"doc(Initialises this integer to zero.)doc";

// Docstring regina::python::doc::IntegerBase_::__div
static const char *__div =
R"doc(Divides this by the given integer and returns the result. The result
will be truncated to an integer, i.e. rounded towards zero. This
integer is not changed.

If *other* is known to divide this integer exactly, divExact() should
be used instead.

Infinity divided by anything will return infinity. Anything finite
divided by infinity will return zero. Anything finite divided by zero
will return infinity.

For a division routine that always rounds down, see divisionAlg().

Precondition:
    If this class does not support infinity, then *other* must be non-
    zero.

Parameter ``other``:
    the integer to divide this by.

Returns:
    the quotient *this* divided by *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::__div_2
static const char *__div_2 =
R"doc(Divides this by the given integer and returns the result. The result
will be truncated to an integer, i.e. rounded towards zero. This
integer is not changed.

If *other* is known to divide this integer exactly, divExact() should
be used instead.

Infinity divided by anything will return infinity. Anything finite
divided by zero will return infinity.

For a division routine that always rounds down, see divisionAlg().

Precondition:
    If this class does not support infinity, then *other* must be non-
    zero.

Parameter ``other``:
    the integer to divide this by.

Returns:
    the quotient *this* divided by *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::__eq
static const char *__eq =
R"doc(Determines if this is equal to the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this and the given integer are equal.)doc";

// Docstring regina::python::doc::IntegerBase_::__eq_2
static const char *__eq_2 =
R"doc(Determines if this is equal to the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this and the given integer are equal.)doc";

// Docstring regina::python::doc::IntegerBase_::__eq_3
static const char *__eq_3 =
R"doc(Determines if this is equal to the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this and the given integer are equal.)doc";

// Docstring regina::python::doc::IntegerBase_::__ge
static const char *__ge =
R"doc(Determines if this is greater than or equal to the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this is greater than or equal to the given
    integer.)doc";

// Docstring regina::python::doc::IntegerBase_::__ge_2
static const char *__ge_2 =
R"doc(Determines if this is greater than or equal to the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this is greater than or equal to the given
    integer.)doc";

// Docstring regina::python::doc::IntegerBase_::__gt
static const char *__gt =
R"doc(Determines if this is greater than the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this is greater than the given integer.)doc";

// Docstring regina::python::doc::IntegerBase_::__gt_2
static const char *__gt_2 =
R"doc(Determines if this is greater than the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this is greater than the given integer.)doc";

// Docstring regina::python::doc::IntegerBase_::__iadd
static const char *__iadd =
R"doc(Adds the given integer to this. This integer is changed to reflect the
result.

If either term of the sum is infinite, the result will be infinity.

Parameter ``other``:
    the integer to add to this integer.

Returns:
    a reference to this integer with its new value.)doc";

// Docstring regina::python::doc::IntegerBase_::__iadd_2
static const char *__iadd_2 =
R"doc(Adds the given integer to this. This integer is changed to reflect the
result.

If either term of the sum is infinite, the result will be infinity.

Parameter ``other``:
    the integer to add to this integer.

Returns:
    a reference to this integer with its new value.)doc";

// Docstring regina::python::doc::IntegerBase_::__idiv
static const char *__idiv =
R"doc(Divides this by the given integer. The result will be truncated to an
integer, i.e. rounded towards zero. This integer is changed to reflect
the result.

If *other* is known to divide this integer exactly, divByExact()
should be used instead.

Infinity divided by anything will return infinity. Anything finite
divided by infinity will return zero. Anything finite divided by zero
will return infinity.

For a division routine that always rounds down, see divisionAlg().

Precondition:
    If this class does not support infinity, then *other* must be non-
    zero.

Parameter ``other``:
    the integer to divide this by.

Returns:
    a reference to this integer with its new value.)doc";

// Docstring regina::python::doc::IntegerBase_::__idiv_2
static const char *__idiv_2 =
R"doc(Divides this by the given integer. The result will be truncated to an
integer, i.e. rounded towards zero. This integer is changed to reflect
the result.

If *other* is known to divide this integer exactly, divByExact()
should be used instead.

Infinity divided by anything will return infinity. Anything finite
divided by zero will return infinity.

For a division routine that always rounds down, see divisionAlg().

Precondition:
    If this class does not support infinity, then *other* must be non-
    zero.

Parameter ``other``:
    the integer to divide this by.

Returns:
    a reference to this integer with its new value.)doc";

// Docstring regina::python::doc::IntegerBase_::__imod
static const char *__imod =
R"doc(Reduces this integer modulo the given integer. If non-zero, the result
will have the same sign as the original value of this integer. This
integer is changed to reflect the result.

For a mod routine that always returns a non-negative remainder, see
divisionAlg().

Precondition:
    *other* is not zero.

Precondition:
    Neither this nor *other* is infinite.

Parameter ``other``:
    the integer modulo which this integer will be reduced.

Returns:
    a reference to this integer with its new value.)doc";

// Docstring regina::python::doc::IntegerBase_::__imod_2
static const char *__imod_2 =
R"doc(Reduces this integer modulo the given integer. If non-zero, the result
will have the same sign as the original value of this integer. This
integer is changed to reflect the result.

For a mod routine that always returns a non-negative remainder, see
divisionAlg().

Precondition:
    *other* is not zero.

Precondition:
    This integer is not infinite.

Parameter ``other``:
    the integer modulo which this integer will be reduced.

Returns:
    a reference to this integer with its new value.)doc";

// Docstring regina::python::doc::IntegerBase_::__imul
static const char *__imul =
R"doc(Multiplies the given integer by this. This integer is changed to
reflect the result.

If either factor of the product is infinite, the result will be
infinity.

Parameter ``other``:
    the integer to multiply with this integer.

Returns:
    a reference to this integer with its new value.)doc";

// Docstring regina::python::doc::IntegerBase_::__imul_2
static const char *__imul_2 =
R"doc(Multiplies the given integer by this. This integer is changed to
reflect the result.

If either factor of the product is infinite, the result will be
infinity.

Parameter ``other``:
    the integer to multiply with this integer.

Returns:
    a reference to this integer with its new value.)doc";

// Docstring regina::python::doc::IntegerBase_::__inc
static const char *__inc =
R"doc(The postincrement operator. This operator increments this integer by
one, and returns a copy of the integer _before_ the increment.

Python:
    This routine is named inc() since python does not support the
    increment operator.

Returns:
    a copy of this integer before the increment took place.)doc";

// Docstring regina::python::doc::IntegerBase_::__init
static const char *__init =
R"doc(Initialises this integer to the given value.

Python:
    In Python, this is the only native-integer constructor available.

Parameter ``value``:
    the new value of this integer.)doc";

// Docstring regina::python::doc::IntegerBase_::__init_2
static const char *__init_2 =
R"doc(Initialises this integer to the given value.

Precondition:
    The given integer is not infinite.

Parameter ``value``:
    the new value of this integer.)doc";

// Docstring regina::python::doc::IntegerBase_::__init_3
static const char *__init_3 =
R"doc(Moves the given integer into this new integer. This is a fast
(constant time) operation.

The integer that is passed (*src*) will no longer be usable.

Precondition:
    The given integer is not infinite.

Parameter ``src``:
    the integer to move.)doc";

// Docstring regina::python::doc::IntegerBase_::__init_4
static const char *__init_4 =
R"doc(Initialises this to the given Python arbitrary-precision integer.

The argument is of the Python type ``long``, which Python uses to
store integers of arbitrary magnitude (much like Regina does with its
Integer and LargeInteger classes).

Parameter ``value``:
    the new value of this integer.)doc";

// Docstring regina::python::doc::IntegerBase_::__init_5
static const char *__init_5 =
R"doc(Initialises this integer to the truncation of the given real number.

Parameter ``value``:
    the real number to be truncated.)doc";

// Docstring regina::python::doc::IntegerBase_::__init_6
static const char *__init_6 =
R"doc(Initialises this integer to the given value which is represented as a
string of digits in a given base.

If not specified, the base defaults to 10. If the given base is zero,
the base will be automatically determined. If the given string begins
with ``0x`` or ``0X``, the base will be assumed to be 16. Otherwise,
if the string begins with ``0``, the base will be assumed to be 8.
Otherwise it will be taken as base 10.

If the template argument *supportInfinity* is ``True``, then any
string beginning with "inf" (after any initial whitesapce) will be
interpreted as infinity.

Whitespace may be present at the beginning or the end of the given
string, and will simply be ignored.

For finer details on how the string parsing works, see strtol() from
the standard C library (on which this method is based).

Precondition:
    The given base is zero, or is between 2 and 36 inclusive.

Precondition:
    The given string represents an integer in the given base, with
    optional whitespace beforehand.

Exception ``InvalidArgument``:
    The given string was not a valid large integer representation.

Parameter ``value``:
    the new value of this integer, represented as a string of digits
    in base *base*.

Parameter ``base``:
    the base in which *value* is given.)doc";

// Docstring regina::python::doc::IntegerBase_::__init_7
static const char *__init_7 =
R"doc(Initialises this integer to the given value which is represented as a
string of digits in a given base.

If not specified, the base defaults to 10. If the given base is zero,
the base will be automatically determined. If the given string begins
with ``0x`` or ``0X``, the base will be assumed to be 16. Otherwise,
if the string begins with ``0``, the base will be assumed to be 8.
Otherwise it will be taken as base 10.

If the template argument *supportInfinity* is ``True``, then any
string beginning with "inf" (after any initial whitesapce) will be
interpreted as infinity.

Whitespace may be present at the beginning or the end of the given
string, and will simply be ignored.

For finer details on how the string parsing works, see strtol() from
the standard C library (on which this method is based).

Precondition:
    The given base is zero, or is between 2 and 36 inclusive.

Precondition:
    The given string represents an integer in the given base, with
    optional whitespace beforehand.

Exception ``InvalidArgument``:
    The given string was not a valid large integer representation.

Parameter ``value``:
    the new value of this integer, represented as a string of digits
    in base *base*.

Parameter ``base``:
    the base in which *value* is given.)doc";

// Docstring regina::python::doc::IntegerBase_::__isub
static const char *__isub =
R"doc(Subtracts the given integer from this. This integer is changed to
reflect the result.

If either term of the difference is infinite, the result will be
infinity.

Parameter ``other``:
    the integer to subtract from this integer.

Returns:
    a reference to this integer with its new value.)doc";

// Docstring regina::python::doc::IntegerBase_::__isub_2
static const char *__isub_2 =
R"doc(Subtracts the given integer from this. This integer is changed to
reflect the result.

If either term of the difference is infinite, the result will be
infinity.

Parameter ``other``:
    the integer to subtract from this integer.

Returns:
    a reference to this integer with its new value.)doc";

// Docstring regina::python::doc::IntegerBase_::__le
static const char *__le =
R"doc(Determines if this is less than or equal to the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this is less than or equal to the given
    integer.)doc";

// Docstring regina::python::doc::IntegerBase_::__le_2
static const char *__le_2 =
R"doc(Determines if this is less than or equal to the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this is less than or equal to the given
    integer.)doc";

// Docstring regina::python::doc::IntegerBase_::__lt
static const char *__lt =
R"doc(Determines if this is less than the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this is less than the given integer.)doc";

// Docstring regina::python::doc::IntegerBase_::__lt_2
static const char *__lt_2 =
R"doc(Determines if this is less than the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this is less than the given integer.)doc";

// Docstring regina::python::doc::IntegerBase_::__mod
static const char *__mod =
R"doc(Determines the remainder when this integer is divided by the given
integer. If non-zero, the result will have the same sign as this
integer. This integer is not changed.

For a division routine that always returns a non-negative remainder,
see divisionAlg().

Precondition:
    *other* is not zero.

Precondition:
    Neither this nor *other* is infinite.

Parameter ``other``:
    the integer to divide this by.

Returns:
    the remainder *this* modulo *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::__mod_2
static const char *__mod_2 =
R"doc(Determines the remainder when this integer is divided by the given
integer. If non-zero, the result will have the same sign as this
integer. This integer is not changed.

For a division routine that always returns a non-negative remainder,
see divisionAlg().

Precondition:
    *other* is not zero.

Precondition:
    This integer is not infinite.

Parameter ``other``:
    the integer to divide this by.

Returns:
    the remainder *this* modulo *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::__mul
static const char *__mul =
R"doc(Multiplies this by the given integer and returns the result. This
integer is not changed.

If either factor of the product is infinite, the result will be
infinity.

Parameter ``other``:
    the integer to multiply by this integer.

Returns:
    the product *this* times *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::__mul_2
static const char *__mul_2 =
R"doc(Multiplies this by the given integer and returns the result. This
integer is not changed.

If either factor of the product is infinite, the result will be
infinity.

Parameter ``other``:
    the integer to multiply by this integer.

Returns:
    the product *this* times *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::__ne
static const char *__ne =
R"doc(Determines if this is not equal to the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this and the given integer are not equal.)doc";

// Docstring regina::python::doc::IntegerBase_::__ne_2
static const char *__ne_2 =
R"doc(Determines if this is not equal to the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this and the given integer are not equal.)doc";

// Docstring regina::python::doc::IntegerBase_::__ne_3
static const char *__ne_3 =
R"doc(Determines if this is not equal to the given integer.

Parameter ``rhs``:
    the integer with which this will be compared.

Returns:
    ``True`` if and only if this and the given integer are not equal.)doc";

// Docstring regina::python::doc::IntegerBase_::__sub
static const char *__sub =
R"doc(Subtracts the given integer from this and returns the result. This
integer is not changed.

If either term of the difference is infinite, the result will be
infinity.

Parameter ``other``:
    the integer to subtract from this integer.

Returns:
    the difference *this* minus *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::__sub_2
static const char *__sub_2 =
R"doc(Subtracts the given integer from this and returns the result. This
integer is not changed.

If either term of the difference is infinite, the result will be
infinity.

Parameter ``other``:
    the integer to subtract from this integer.

Returns:
    the difference *this* minus *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::__sub_3
static const char *__sub_3 =
R"doc(Determines the negative of this integer. This integer is not changed.

Negative infinity will return infinity.

Returns:
    the negative of this integer.)doc";

// Docstring regina::python::doc::IntegerBase_::abs
static const char *abs =
R"doc(Determines the absolute value of this integer. This integer is not
changed.

Returns:
    the absolute value of this integer.)doc";

// Docstring regina::python::doc::IntegerBase_::divByExact
static const char *divByExact =
R"doc(Divides this by the given integer. This can only be used when the
given integer divides into this exactly, and for large integers this
is much faster than ordinary division. This integer is changed to
reflect the result.

Precondition:
    The given integer divides exactly into this integer, i.e. *this*
    divided by *other* is an integer.

Precondition:
    *other* is not zero.

Precondition:
    Neither this nor *other* is infinite.

Parameter ``other``:
    the integer to divide this by.

Returns:
    a reference to this integer with its new value.)doc";

// Docstring regina::python::doc::IntegerBase_::divByExact_2
static const char *divByExact_2 =
R"doc(Divides this by the given integer. This can only be used when the
given integer divides into this exactly, and for large integers this
is much faster than ordinary division. This integer is changed to
reflect the result.

Precondition:
    The given integer divides exactly into this integer, i.e. *this*
    divided by *other* is an integer.

Precondition:
    *other* is not zero.

Precondition:
    This integer is not infinite.

Parameter ``other``:
    the integer to divide this by.

Returns:
    a reference to this integer with its new value.)doc";

// Docstring regina::python::doc::IntegerBase_::divExact
static const char *divExact =
R"doc(Divides this by the given integer and returns the result. This can
only be used when the given integer divides into this exactly, and for
large integers can be much faster than ordinary division. This integer
is not changed.

Precondition:
    The given integer divides exactly into this integer, i.e. *this*
    divided by *other* is an integer.

Precondition:
    *other* is not zero.

Precondition:
    Neither this nor *other* is infinite.

Parameter ``other``:
    the integer to divide this by.

Returns:
    the quotient *this* divided by *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::divExact_2
static const char *divExact_2 =
R"doc(Divides this by the given integer and returns the result. This can
only be used when the given integer divides into this exactly, and for
large integers can be much faster than ordinary division. This integer
is not changed.

Precondition:
    The given integer divides exactly into this integer, i.e. *this*
    divided by *other* is an integer.

Precondition:
    *other* is not zero.

Precondition:
    This integer is not infinite.

Parameter ``other``:
    the integer to divide this by.

Returns:
    the quotient *this* divided by *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::divisionAlg
static const char *divisionAlg =
R"doc(Uses the division algorithm to obtain a quotient and remainder when
dividing by the given integer.

Suppose this integer is *n* and we pass the divisor *d*. The _division
algorithm_ describes the result of dividing *n* by *d*; in particular,
it expresses ``n = qd + r``, where *q* is the quotient and *r* is the
remainder.

The division algorithm is precise about which values of *q* and *r*
are chosen; in particular it chooses the unique *r* in the range ``0 ≤
r < |d|``.

Note that this differs from other division routines in this class, in
that it always rounds to give a non-negative remainder. Thus
(-7).divisionAlg(3) gives quotient -3 and remainder 2, whereas (-7)/3
gives quotient -2 and (-7)%3 gives remainder -1.

In the special case where the given divisor is 0 (not allowed by the
usual division algorithm), this routine selects quotient 0 and
remainder *n*.

Precondition:
    Neither this nor the divisor are infinite.

Parameter ``divisor``:
    the divisor *d*.

Returns:
    the pair (*q*, *r*), where *q* is the quotient and *r* is the
    remainder, as described above.)doc";

// Docstring regina::python::doc::IntegerBase_::gcd
static const char *gcd =
R"doc(Determines the greatest common divisor of this and the given integer.
This integer is not changed.

The result is guaranteed to be non-negative. As a special case,
gcd(0,0) is considered to be zero.

Precondition:
    Neither this integer nor *other* is infinite.

Parameter ``other``:
    the integer whose greatest common divisor with this will be found.

Returns:
    the greatest common divisor of this and the given integer.)doc";

// Docstring regina::python::doc::IntegerBase_::gcdWith
static const char *gcdWith =
R"doc(Sets this integer to be the greatest common divisor of this and the
given integer.

The result is guaranteed to be non-negative. As a special case,
gcd(0,0) is considered to be zero.

Precondition:
    Neither this integer nor *other* is infinite.

Parameter ``other``:
    the integer whose greatest common divisor with this will be found.)doc";

// Docstring regina::python::doc::IntegerBase_::gcdWithCoeffs
static const char *gcdWithCoeffs =
R"doc(Determines the greatest common divisor of this and the given integer
and finds the smallest coefficients with which these integers combine
to give their gcd.

Note that the given integers need not be non-negative. However, the
gcd returned is guaranteed to be non-negative.

If *d* is the gcd of *this* and *other*, then this routine returns the
tuple (*d*, *u*, *v*), where *u* and *v* are coefficients for which:

* ``u*this + v*other = d``;

* ``-abs(this)/d < v*sign(other) ≤ 0``; and

* ``1 ≤ u*sign(this) ≤ abs(other)/d``.

These equations are not satisfied when either of *this* or *other* are
zero, but in this case *u* and *v* will both be 0, 1 or -1, using as
many zeros as possible.

Precondition:
    Neither this integer nor *other* is infinite.

.. note::
    There are two variants of this routine: one returns the
    coefficients *u* and *v* as part of a tuple, and one returns them
    via reference arguments. For now both versions remain supported,
    but there is a long-term plan to eventually phase out the
    reference argument variant (i.e., not this variant).

Parameter ``other``:
    the integer whose greatest common divisor with this will be found.

Returns:
    a tuple containing: the greatest common divisor of *this* and
    *other*; the final coefficient of *this*; and the final
    coefficient of *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::gcdWithCoeffs_2
static const char *gcdWithCoeffs_2 =
R"doc(Determines the greatest common divisor of this and the given integer
and finds the smallest coefficients with which these integers combine
to give their gcd.

Note that the given integers need not be non-negative. However, the
gcd returned is guaranteed to be non-negative.

If *d* is the gcd of *this* and *other*, the values placed into *u*
and *v* will be coefficients for which:

* ``u*this + v*other = d``;

* ``-abs(this)/d < v*sign(other) ≤ 0``; and

* ``1 ≤ u*sign(this) ≤ abs(other)/d``.

These equations are not satisfied when either of *this* or *other* are
zero, but in this case *u* and *v* will both be 0, 1 or -1, using as
many zeros as possible.

Precondition:
    Neither this integer nor *other* is infinite.

.. note::
    There are two variants of this routine: one returns the
    coefficients *u* and *v* as part of a tuple, and one returns them
    via reference arguments. For now both versions remain supported,
    but there is a long-term plan to eventually phase out the
    reference argument variant (i.e., this variant).

Parameter ``other``:
    the integer whose greatest common divisor with this will be found.

Parameter ``u``:
    a variable into which the final coefficient of *this* will be
    placed. Any existing contents of *u* will be overwritten.

Parameter ``v``:
    a variable into which the final coefficient of *other* will be
    placed. Any existing contents of *v* will be overwritten.

Returns:
    the greatest common divisor of *this* and *other*.)doc";

// Docstring regina::python::doc::IntegerBase_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given integers.

This global routine simply calls IntegerBase<supportInfinity>::swap();
it is provided so that IntegerBase<supportInfinity> meets the C++
Swappable requirements.

Parameter ``a``:
    the first integer whose contents should be swapped.

Parameter ``b``:
    the second integer whose contents should be swapped.)doc";

// Docstring regina::python::doc::IntegerBase_::isInfinite
static const char *isInfinite =
R"doc(Returns whether this integer is infinity.

Returns:
    ``True`` if and only if this integer is infinity.)doc";

// Docstring regina::python::doc::IntegerBase_::isNative
static const char *isNative =
R"doc(Returns whether we are currently working with a native C/C++ long, or
whether we have switched to GMP large integer arithmetic for this
integer.

If this integer is infinite, this routine will return ``False``.

Returns:
    ``True`` if and only if we are still using a native C/C++ long.)doc";

// Docstring regina::python::doc::IntegerBase_::isZero
static const char *isZero =
R"doc(Returns whether or not this integer is zero.

This is micro-optimised to be faster than simply testing whether
(*this) == 0.

Returns:
    ``True`` if and only if this integer is zero.)doc";

// Docstring regina::python::doc::IntegerBase_::lcm
static const char *lcm =
R"doc(Determines the lowest common multiple of this and the given integer.
This integer is not changed.

Note that the result might possibly be negative.

Precondition:
    Neither this integer nor *other* is infinite.

Parameter ``other``:
    the integer whose lowest common multiple with this will be found.

Returns:
    the lowest common multiple of this and the given integer.)doc";

// Docstring regina::python::doc::IntegerBase_::lcmWith
static const char *lcmWith =
R"doc(Sets this integer to be the lowest common multiple of this and the
given integer.

Note that the result might possibly be negative.

Precondition:
    Neither this integer nor *other* is infinite.

Parameter ``other``:
    the integer whose lowest common multiple with this will be found.)doc";

// Docstring regina::python::doc::IntegerBase_::legendre
static const char *legendre =
R"doc(Returns the Legendre symbol (*a*/*p*), where *a* is this integer and
*p* is an odd prime.

The Legendre symbol is equal to 0 if this integer is divisible by *p*,
1 if this integer is congruent to a square mod *p* (but not divisible
by *p*), and -1 otherwise.

Precondition:
    The given integer *p* is an odd positive prime.

Precondition:
    This integer is not infinite.

Parameter ``p``:
    the given odd prime.

Returns:
    The Legendre symbol (0, 1 or -1) as described above.

Author:
    Ryan Budney)doc";

// Docstring regina::python::doc::IntegerBase_::longValue
static const char *longValue =
R"doc(Returns the value of this integer as a long.

It is the programmer's reponsibility to ensure that this integer is
within the required range. If this integer is too large or small to
fit into a long, then the result will be undefined.

Note that, assuming the value is within the required range, this
routine will give correct results regardless of whether the underlying
representation is a native or large integer.

Precondition:
    This integer is not infinity.

Returns:
    the value of this integer.)doc";

// Docstring regina::python::doc::IntegerBase_::makeInfinite
static const char *makeInfinite =
R"doc(Sets this integer to be infinity.

If the template parameter *supportInfinity* is ``False``, this routine
safely does nothing.)doc";

// Docstring regina::python::doc::IntegerBase_::makeLarge
static const char *makeLarge =
R"doc(Converts this integer to use a GMP large integer representation,
regardless of whether this is actually necessary. The contents of this
integer will be preserved.

It does not matter which kind of representation this integer is
currently using.

Precondition:
    This integer is not infinite.)doc";

// Docstring regina::python::doc::IntegerBase_::negate
static const char *negate =
R"doc(Negates this integer. This integer is changed to reflect the result.

Negating infinity will result in infinity.)doc";

// Docstring regina::python::doc::IntegerBase_::raiseToPower
static const char *raiseToPower =
R"doc(Raises this integer to the power of the given exponent. This integer
is changed to reflect the result.

Note that 0 to the power of 0 will be 1, infinity to the power of 0
will be 1, and infinity to the power of anything else will be
infinity.

Precondition:
    The given exponent is non-negative.

Parameter ``exp``:
    the power to which this integer will be raised.)doc";

// Docstring regina::python::doc::IntegerBase_::randomBinary
static const char *randomBinary =
R"doc(Generate a pseudo-random integer that is uniformly distributed in the
interval [0,2^n).

The random number generation here does _not_ use Regina's own
RandomEngine class, but instead uses a separate random number
generator provided by GMP.

Parameter ``n``:
    the maximum number of bits in the pseudo-random integer.

Returns:
    a pseudo-random integer.)doc";

// Docstring regina::python::doc::IntegerBase_::randomBoundedByThis
static const char *randomBoundedByThis =
R"doc(Generate a pseudo-random integer that is uniformly distributed in the
interval [0,*this).

The random number generation here does _not_ use Regina's own
RandomEngine class, but instead uses a separate random number
generator provided by GMP.

Precondition:
    This integer is strictly positive.

.. warning::
    Even if this integer is small, this routine is still slow - it
    always goes through the GMP large integer routines so that the
    random number generation algorithm is consistent. If you need a
    fast random number generator and this integer is small, consider
    using the standard rand() function instead.

Returns:
    a pseudo-random integer.)doc";

// Docstring regina::python::doc::IntegerBase_::randomCornerBinary
static const char *randomCornerBinary =
R"doc(Generate a pseudo-random integer that is distributed in the interval
[0,2^n), with a tendency to have long strings of 0s and 1s in its
binary expansion.

The random number generation here does _not_ use Regina's own
RandomEngine class, but instead uses a separate random number
generator provided by GMP.

Parameter ``n``:
    the maximum number of bits in the pseudo-random integer.

Returns:
    a pseudo-random integer.)doc";

// Docstring regina::python::doc::IntegerBase_::safeLongValue
static const char *safeLongValue =
R"doc(Returns the value of this integer as a long, or throws an exception if
this is not possible.

If this integer is within the required range, regardless of whether
the underlying representation is a native or large integer, this
routine will return the correct result.

Exception ``NoSolution``:
    This integer is too large or small to fit into a long.

Returns:
    the value of this integer.)doc";

// Docstring regina::python::doc::IntegerBase_::sign
static const char *sign =
R"doc(Returns the sign of this integer.

In this routine, infinity is considered to have sign +1.

Returns:
    +1, -1 or 0 according to whether this integer is positive,
    negative or zero.)doc";

// Docstring regina::python::doc::IntegerBase_::stringValue
static const char *stringValue =
R"doc(Returns the value of this integer as a string in the given base. If
not specified, the base defaults to 10.

If this integer is infinity, the string returned will be ``inf``.

Precondition:
    The given base is between 2 and 36 inclusive.

Returns:
    the value of this integer as a string.)doc";

// Docstring regina::python::doc::IntegerBase_::swap
static const char *swap =
R"doc(Swaps the values of this and the given integer.

Parameter ``other``:
    the integer whose value will be swapped with this.)doc";

// Docstring regina::python::doc::IntegerBase_::tightDecoding
static const char *tightDecoding =
R"doc(Reconstructs an integer from its given tight encoding. See the page on
tight encodings for details.

The tight encoding will be given as a string. If this string contains
leading whitespace or any trailing characters at all (including
trailing whitespace), then it will be treated as an invalid encoding
(i.e., this routine will throw an exception).

This routine does recognise infinity in the case where
*supportInfinity* is ``True``.

This routine is identical to calling the global template routine
regina::tightDecoding() with this type as the template argument.

Exception ``InvalidArgument``:
    The given string is not a tight encoding of an integer of this
    type.

Parameter ``enc``:
    the tight encoding for an integer.

Returns:
    the integer represented by the given tight encoding.)doc";

// Docstring regina::python::doc::IntegerBase_::tightEncoding
static const char *tightEncoding =
R"doc(Returns the tight encoding of this integer. See the page on tight
encodings for details.

There is also a corresponding global regina::tightEncoding() function,
for better compatibility with native C++ integer types. The global
function is more efficient if the integer argument is an rvalue
reference (since this const member function induces an extra deep
copy).

Returns:
    the resulting encoded string.)doc";

// Docstring regina::python::doc::IntegerBase_::tryReduce
static const char *tryReduce =
R"doc(Converts this integer to use a native C/C++ long representation, if
this is possible. However, if this integer is outside the range of a
C/C++ long, then it will remain as a GMP large integer instead (i.e.,
nothing will change). Whatever happens, the contents of this integer
will be preserved.

It does not matter which kind of representation this integer is
currently using.

Precondition:
    This integer is not infinite.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

