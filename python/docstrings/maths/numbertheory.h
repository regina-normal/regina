/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::gcd
static const char *gcd =
R"doc(Deprecated routine that calculates the greatest common divisor of two
signed integers. This routine is not recursive.

Although the arguments may be negative, the result is guaranteed to be
non-negative. As a special case, gcd(0,0) is considered to be zero.

.. deprecated::
    Simply use std::gcd(), which was introduced with C++17.

Parameter ``a``:
    one of the two integers to work with.

Parameter ``b``:
    the other integer with which to work.

Returns:
    the greatest common divisor of *a* and *b*.)doc";

// Docstring regina::python::doc::gcdWithCoeffs
static const char *gcdWithCoeffs =
R"doc(Calculates the greatest common divisor of two given integers and finds
the smallest coefficients with which these integers combine to give
their gcd. This routine is not recursive.

Note that the given integers need not be non-negative. However, the
gcd returned is guaranteed to be non-negative. As a special case,
gcd(0,0) is considered to be zero.

If *d* is the gcd of *a* and *b*, then this routine returns the tuple
(*d*, *u*, *v*), where *u* and *v* are coefficients for which:

* ``u⋅a + v⋅b = d``;

* ``-abs(a)/d < v⋅sign(b) ≤ 0``; and

* ``1 ≤ u⋅sign(a) ≤ abs(b)/d``.

In the special case where one of the given integers is zero, the
corresponding coefficient will also be zero and the other coefficient
will be 1 or -1 so that ``u⋅a + v⋅b = d`` still holds. If both given
integers are zero, both of the coefficients will be set to zero.

Parameter ``a``:
    the first integer to compute the gcd of.

Parameter ``b``:
    the second integer to compute the gcd of.

Returns:
    a tuple containing: the greatest common divisor of *a* and *b*;
    the final coefficient of *a*; and the final coefficient of *b*.)doc";

// Docstring regina::python::doc::lcm
static const char *lcm =
R"doc(Deprecated routine that calculates the lowest common multiple of two
signed integers. Although the arguments may be negative, the result is
guaranteed to be non-negative.

If either of the arguments is zero, the return value will also be
zero.

Regarding possible overflow: This routine does not create any
temporary integers that are larger than the final LCM.

.. deprecated::
    Simply use std::lcm(), which was introduced with C++17.

Parameter ``a``:
    one of the two integers to work with.

Parameter ``b``:
    the other integer with which to work.

Returns:
    the lowest common multiple of *a* and *b*.)doc";

// Docstring regina::python::doc::modularInverse
static const char *modularInverse =
R"doc(Calculates the multiplicative inverse of one integer modulo another.
The inverse returned will be between 0 and *n*-1 inclusive.

Precondition:
    *n* and *k* are both strictly positive;

Precondition:
    *n* and *k* have no common factors.

Parameter ``n``:
    the modular base in which to work.

Parameter ``k``:
    the number whose multiplicative inverse should be found.

Returns:
    the inverse *v* for which ``k * v == 1 (mod n)``.)doc";

// Docstring regina::python::doc::reducedMod
static const char *reducedMod =
R"doc(Reduces *k* modulo *modBase* to give the smallest possible absolute
value. For instance, ``reducedMod(4,10) = 4`` but ``reducedMod(6,10) =
-4``. In the case of a tie, the positive solution is taken.

Precondition:
    *modBase* is strictly positive.

Parameter ``k``:
    the number to reduce modulo *modBase*.

Parameter ``modBase``:
    the modular base in which to work.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

