/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Rational
static const char *Rational =
R"doc(Represents an arbitrary precision rational number. Calculations with
Rational objects will be exact.

Infinity (1/0) and undefined (0/0) are catered for. (-1/0) is
considered the same as (1/0), and is represented as (1/0). Any
operation involving (0/0) will return (0/0).

Since infinity is the same as negative infinity, both infinity plus
infinity and infinity minus infinity will return infinity. Infinity
divided by infinity returns undefined, as does infinity times zero.

For the purposes of ordering, undefined is the smallest rational and
infinity is the largest. Undefined is always equal to itself, and
infinity is always equal to itself.

When performing computations on rationals, the results will always be
stored in lowest terms (i.e., with relatively prime numerator and
denominator), and with a non-negative denominator. However, when
constructing a rational number from scratch (e.g., by supplying the
numerator and denominator separately), it is your responsibility to
ensure that the rational is in lowest terms.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::swap
static const char *swap =
R"doc(Swaps the contents of the given rationals.

This global routine simply calls Rational::swap(); it is provided so
that Rational meets the C++ Swappable requirements.

Parameter ``a``:
    the first rational whose contents should be swapped.

Parameter ``b``:
    the second rational whose contents should be swapped.)doc";

namespace Rational_ {

// Docstring regina::python::doc::Rational_::Rational
static const char *Rational = R"doc(Initialises to 0/1.)doc";

// Docstring regina::python::doc::Rational_::Rational_2
static const char *Rational_2 =
R"doc(Initialises to the given rational value.

Parameter ``value``:
    the new rational value of this rational.)doc";

// Docstring regina::python::doc::Rational_::Rational_3
static const char *Rational_3 =
R"doc(Initialises to the given integer value. The given integer may be
infinite.

Parameter ``value``:
    the new integer value of this rational.)doc";

// Docstring regina::python::doc::Rational_::Rational_4
static const char *Rational_4 =
R"doc(Initialises to the given integer value.

Parameter ``value``:
    the new integer value of this rational.)doc";

// Docstring regina::python::doc::Rational_::Rational_5
static const char *Rational_5 =
R"doc(Initialises to *newNum*/*newDen*.

Precondition:
    gcd(*newNum*, *newDen*) = 1 or *newDen* = 0.

Precondition:
    *newDen* is non-negative.

Precondition:
    Neither of the given integers is infinite.

.. warning::
    Failing to meet the preconditions above can result in misleading
    or even undefined behaviour. As an example, Rational(4,4) (which
    breaks the gcd requirement) is considered different from
    Rational(1,1) (a valid rational), which is different again from
    Rational(-1,-1) (which breaks the non-negativity requirement).

Parameter ``newNum``:
    the new numerator.

Parameter ``newDen``:
    the new denominator.)doc";

// Docstring regina::python::doc::Rational_::Rational_6
static const char *Rational_6 =
R"doc(Initialises to *newNum*/*newDen*.

Precondition:
    gcd(*newNum*, *newDen*) = 1 or *newDen* = 0.

Precondition:
    *newDen* is non-negative.

.. warning::
    Failing to meet the preconditions above can result in misleading
    or even undefined behaviour. As an example, Rational(4,4) (which
    breaks the gcd requirement) is considered different from
    Rational(1,1) (a valid rational), which is different again from
    Rational(-1,-1) (which breaks the non-negativity requirement).

Parameter ``newNum``:
    the new numerator.

Parameter ``newDen``:
    the new denominator.)doc";

// Docstring regina::python::doc::Rational_::__add
static const char *__add =
R"doc(Calculates the sum of two rationals. This rational is not changed.

Parameter ``r``:
    the rational to add to this.

Returns:
    the sum *this* + *r*.)doc";

// Docstring regina::python::doc::Rational_::__div
static const char *__div =
R"doc(Calculates the ratio of two rationals. This rational is not changed.

Parameter ``r``:
    the rational to divide this by.

Returns:
    the ratio *this* / *r*.)doc";

// Docstring regina::python::doc::Rational_::__eq
static const char *__eq =
R"doc(Determines if this is equal to the given rational.

Parameter ``compare``:
    the rational with which this will be compared.

Returns:
    ``True`` if and only if this rational is equal to *compare*.)doc";

// Docstring regina::python::doc::Rational_::__ge
static const char *__ge =
R"doc(Determines if this is greater than or equal to the given rational.

Parameter ``compare``:
    the rational with which this will be compared.

Returns:
    ``True`` if and only if this rational is greater than or equal to
    *compare*.)doc";

// Docstring regina::python::doc::Rational_::__gt
static const char *__gt =
R"doc(Determines if this is greater than the given rational.

Parameter ``compare``:
    the rational with which this will be compared.

Returns:
    ``True`` if and only if this rational is greater than *compare*.)doc";

// Docstring regina::python::doc::Rational_::__iadd
static const char *__iadd =
R"doc(Adds the given rational to this. This rational is changed to reflect
the result.

Parameter ``other``:
    the rational to add to this.

Returns:
    a reference to this rational with its new value.)doc";

// Docstring regina::python::doc::Rational_::__idiv
static const char *__idiv =
R"doc(Divides this by the given rational. This rational is changed to
reflect the result.

Parameter ``other``:
    the rational to divide this by.

Returns:
    a reference to this rational with its new value.)doc";

// Docstring regina::python::doc::Rational_::__imul
static const char *__imul =
R"doc(Multiplies the given rational by this. This rational is changed to
reflect the result.

Parameter ``other``:
    the rational to multiply by this.

Returns:
    a reference to this rational with its new value.)doc";

// Docstring regina::python::doc::Rational_::__isub
static const char *__isub =
R"doc(Subtracts the given rational from this. This rational is changed to
reflect the result.

Parameter ``other``:
    the rational to subtract from this.

Returns:
    a reference to this rational with its new value.)doc";

// Docstring regina::python::doc::Rational_::__le
static const char *__le =
R"doc(Determines if this is less than or equal to the given rational.

Parameter ``compare``:
    the rational with which this will be compared.

Returns:
    ``True`` if and only if this rational is less than or equal to
    *compare*.)doc";

// Docstring regina::python::doc::Rational_::__lt
static const char *__lt =
R"doc(Determines if this is less than the given rational.

Parameter ``compare``:
    the rational with which this will be compared.

Returns:
    ``True`` if and only if this rational is less than *compare*.)doc";

// Docstring regina::python::doc::Rational_::__mul
static const char *__mul =
R"doc(Calculates the product of two rationals. This rational is not changed.

Parameter ``r``:
    the rational with which to multiply this.

Returns:
    the product *this* * *r*.)doc";

// Docstring regina::python::doc::Rational_::__ne
static const char *__ne =
R"doc(Determines if this is not equal to the given rational.

Parameter ``compare``:
    the rational with which this will be compared.

Returns:
    ``True`` if and only if this rational is not equal to *compare*.)doc";

// Docstring regina::python::doc::Rational_::__sub
static const char *__sub =
R"doc(Calculates the difference of two rationals. This rational is not
changed.

Parameter ``r``:
    the rational to subtract from this.

Returns:
    the difference *this* - *r*.)doc";

// Docstring regina::python::doc::Rational_::__sub_2
static const char *__sub_2 =
R"doc(Determines the negative of this rational. This rational is not
changed.

Returns:
    the negative of this rational.)doc";

// Docstring regina::python::doc::Rational_::abs
static const char *abs =
R"doc(Determines the absolute value of this rational. This rational is not
changed.

Returns:
    the absolute value of this rational.)doc";

// Docstring regina::python::doc::Rational_::denominator
static const char *denominator =
R"doc(Returns the denominator of this rational. Note that rationals are
always stored in lowest terms with non-negative denominator. Infinity
will be stored as 1/0.

Returns:
    the denominator.)doc";

// Docstring regina::python::doc::Rational_::doubleApprox
static const char *doubleApprox =
R"doc(Attempts to convert this rational to a real number.

If this rational can be approximated by a double (specifically, if it
lies within double's allowable range) then a such an approximation is
returned. Otherwise this routine will throw an exception.

Note that "lies with double's allowable range" is machine-dependent,
and may vary between different installations. Infinity and undefined
are always considered out of range. Otherwise a rational is out of
range if its absolute value is finite but too large (e.g., 10^10000)
or non-zero but too small (e.g., 10^-10000).

Exception ``UnsolvedCase``:
    This rational lies outside double's allowable range.

Returns:
    the double approximation to this rational.

Author:
    Ryan Budney, B.B.)doc";

// Docstring regina::python::doc::Rational_::inverse
static const char *inverse =
R"doc(Calculates the inverse of this rational. This rational is not changed.

Returns:
    the inverse 1 / *this*.)doc";

// Docstring regina::python::doc::Rational_::invert
static const char *invert = R"doc(Inverts this rational. This rational is changed to reflect the result.)doc";

// Docstring regina::python::doc::Rational_::negate
static const char *negate = R"doc(Negates this rational. This rational is changed to reflect the result.)doc";

// Docstring regina::python::doc::Rational_::numerator
static const char *numerator =
R"doc(Returns the numerator of this rational. Note that rationals are always
stored in lowest terms with non-negative denominator. Infinity will be
stored as 1/0.

Returns:
    the numerator.)doc";

// Docstring regina::python::doc::Rational_::swap
static const char *swap =
R"doc(Swaps the values of this and the given rational.

Parameter ``other``:
    the rational whose value will be swapped with this.)doc";

// Docstring regina::python::doc::Rational_::tex
static const char *tex =
R"doc(Returns this rational as written using TeX formatting. No leading or
trailing dollar signs will be included.

Returns:
    this rational as written using TeX formatting.

Author:
    Ryan Budney)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

