/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Polynomial
static const char *Polynomial =
R"doc(Represents a single-variable polynomial with coefficients of type *T*.
All exponents in the polynomial must be non-negative (so you can
represent ``2+3x`` but not ``1+1/x``).

The type *T* must represent a ring with no zero divisors. In
particular, it must:

* support basic arithmetic operations;

* support assignments of the form ``x = int`` and tests of the form
  ``x == int`` and ``x < int``;

* have a default constructor that assigns an explicit value of zero.

This means that Regina's numerical types such as Integer and Rational
are supported, but native data types such as int and long are not
(since they have no zero-initialising default constructor).

The underlying storage method for this class is dense (i.e., all
coefficients are explicitly stored, including zero coefficients).

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

Python:
    In Python, the class Polynomial refers to the specific template
    class Polynomial<Rational>.)doc";

// Docstring regina::python::doc::__add
static const char *__add =
R"doc(Adds the two given polynomials.

This operator ``+`` is sometimes faster than using ``+=``, since it
has more flexibility to avoid an internal deep copy.

Parameter ``lhs``:
    the first polynomial to add.

Parameter ``rhs``:
    the second polynomial to add.

Returns:
    the sum of both polynomials.)doc";

// Docstring regina::python::doc::__add_2
static const char *__add_2 =
R"doc(Adds the two given polynomials.

This operator ``+`` is sometimes faster than using ``+=``, since it
has more flexibility to avoid an internal deep copy.

Parameter ``lhs``:
    the first polynomial to add.

Parameter ``rhs``:
    the second polynomial to add.

Returns:
    the sum of both polynomials.)doc";

// Docstring regina::python::doc::__add_3
static const char *__add_3 =
R"doc(Adds the two given polynomials.

This operator ``+`` is sometimes faster than using ``+=``, since it
has more flexibility to avoid an internal deep copy.

Parameter ``lhs``:
    the first polynomial to add.

Parameter ``rhs``:
    the second polynomial to add.

Returns:
    the sum of both polynomials.)doc";

// Docstring regina::python::doc::__add_4
static const char *__add_4 =
R"doc(Adds the two given polynomials.

This operator ``+`` is sometimes faster than using ``+=``, since it
has more flexibility to avoid an internal deep copy.

Parameter ``lhs``:
    the first polynomial to add.

Parameter ``rhs``:
    the second polynomial to add.

Returns:
    the sum of both polynomials.)doc";

// Docstring regina::python::doc::__div
static const char *__div =
R"doc(Divides the given polynomial by the given scalar constant.

This uses the division operator /= for the coefficient type *T*.

The scalar is simply of type *T*; we use the identical type
Polynomial<T>::Coefficient here to assist with C++ template type
matching.

Precondition:
    The argument *scalar* is non-zero.

Parameter ``poly``:
    the polynomial to divide by the given scalar.

Parameter ``scalar``:
    the scalar factor to divide by.

Returns:
    the quotient of the given polynomial by the given scalar.)doc";

// Docstring regina::python::doc::__div_2
static const char *__div_2 =
R"doc(Divides the two given polynomials.

More precisely: suppose there exist polynomials *q* and *r* with
coefficients of type *T* for which ``lhs = q.rhs + r``, and where *r*
has smaller degree than *rhs*. Then we call *q* the _quotient_, and
*r* the _remainder_.

This routine returns the quotient *q*, and discards the remainder. If
you need to keep the remainder also, then call
Polynomial::divisionAlg() instead.

Coefficients are divided using the operator /= on type *T*.

If your coefficient type *T* is not a field (e.g., if *T* is Integer),
you must be sure to know in advance that the quotient exists (see the
precondition below). Otherwise the behaviour of this routine is
undefined.

Precondition:
    The second polynomial *rhs* is non-zero.

Precondition:
    The quotient as defined above exists. If *T* is a field type
    (e.g., if *T* is Rational) then this is true automatically. If not
    (e.g., if *T* is Integer) then this requires some prior knowledge
    about the arguments.

Parameter ``lhs``:
    the polynomial to divide by *rhs*.

Parameter ``rhs``:
    the polynomial that we will divide *lhs* by.

Returns:
    the quotient, as described above.)doc";

// Docstring regina::python::doc::__mul
static const char *__mul =
R"doc(Multiplies the given polynomial by the given scalar constant.

The scalar is simply of type *T*; we use the identical type
Polynomial<T>::Coefficient here to assist with C++ template type
matching.

Parameter ``poly``:
    the polynomial to multiply by.

Parameter ``scalar``:
    the scalar to multiply by.

Returns:
    the product of the given polynomial and scalar.)doc";

// Docstring regina::python::doc::__mul_2
static const char *__mul_2 =
R"doc(Multiplies the given polynomial by the given scalar constant.

The scalar is simply of type *T*; we use the identical type
Polynomial<T>::Coefficient here to assist with C++ template type
matching.

Parameter ``scalar``:
    the scalar to multiply by.

Parameter ``poly``:
    the polynomial to multiply by.

Returns:
    the product of the given polynomial and scalar.)doc";

// Docstring regina::python::doc::__mul_3
static const char *__mul_3 =
R"doc(Multiplies the two given polynomials.

Parameter ``lhs``:
    the first polynomial to multiply.

Parameter ``rhs``:
    the second polynomial to multiply.

Returns:
    the product of both polynomials.)doc";

// Docstring regina::python::doc::__sub
static const char *__sub =
R"doc(Returns the negative of the given polynomial.

Parameter ``arg``:
    the polynomial to negate.

Returns:
    the negative of *arg*.)doc";

// Docstring regina::python::doc::__sub_2
static const char *__sub_2 =
R"doc(Subtracts the two given polynomials.

This operator ``-`` is sometimes faster than using ``-=``, since it
has more flexibility to avoid an internal deep copy.

Parameter ``lhs``:
    the polynomial to sutract *rhs* from.

Parameter ``rhs``:
    the polynomial to subtract from *lhs*.

Returns:
    the difference of the two given polynomials.)doc";

// Docstring regina::python::doc::__sub_3
static const char *__sub_3 =
R"doc(Subtracts the two given polynomials.

This operator ``-`` is sometimes faster than using ``-=``, since it
has more flexibility to avoid an internal deep copy.

Parameter ``lhs``:
    the polynomial to sutract *rhs* from.

Parameter ``rhs``:
    the polynomial to subtract from *lhs*.

Returns:
    the difference of the two given polynomials.)doc";

// Docstring regina::python::doc::__sub_4
static const char *__sub_4 =
R"doc(Subtracts the two given polynomials.

This operator ``-`` is sometimes faster than using ``-=``, since it
has more flexibility to avoid an internal deep copy.

Parameter ``lhs``:
    the polynomial to sutract *rhs* from.

Parameter ``rhs``:
    the polynomial to subtract from *lhs*.

Returns:
    the difference of the two given polynomials.)doc";

// Docstring regina::python::doc::__sub_5
static const char *__sub_5 =
R"doc(Subtracts the two given polynomials.

This operator ``-`` is sometimes faster than using ``-=``, since it
has more flexibility to avoid an internal deep copy.

Parameter ``lhs``:
    the polynomial to sutract *rhs* from.

Parameter ``rhs``:
    the polynomial to subtract from *lhs*.

Returns:
    the difference of the two given polynomials.)doc";

namespace Polynomial_ {

// Docstring regina::python::doc::Polynomial_::__array
static const char *__array =
R"doc(Returns the given coefficient of this polynomial.

Python:
    Python users can also use this operator to _set_ cofficients; that
    is, you can write ``poly[exp] = value``. However, when _getting_ a
    coefficient this operator will return by value (to enforce
    constness), which means for example you cannot write something
    like ``poly[exp].negate()``.

Parameter ``exp``:
    the exponent of the term whose coefficient should be returned.
    This must be between 0 and degree() inclusive.

Returns:
    the coefficient of the given term.)doc";

// Docstring regina::python::doc::Polynomial_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given polynomial.

This constructor induces a deep copy of *value*.

A note for developers: even though this routine is identical to the
templated copy constructor, it must be declared and implemented
separately. Otherwise the compiler might create its own (incorrect)
copy constructor automatically.

Parameter ``value``:
    the polynomial to clone.)doc";

// Docstring regina::python::doc::Polynomial_::__default
static const char *__default = R"doc(Creates the zero polynomial.)doc";

// Docstring regina::python::doc::Polynomial_::__eq
static const char *__eq =
R"doc(Tests whether this and the given polynomial are equal.

Parameter ``rhs``:
    the polynomial to compare with this.

Returns:
    ``True`` if and only if this and the given polynomial are equal.)doc";

// Docstring regina::python::doc::Polynomial_::__iadd
static const char *__iadd =
R"doc(Adds the given polynomial to this.

The given polynomial need not have the same degree as this. Note that
the degree of this polynomial might change as a result of this
operation.

.. warning::
    This routine may trigger a deep copy (currently this happens when
    *other* has higher degree than this). Consider using the binary
    ``+`` operator instead, which is better able to avoid this deep
    copy where possible.

Parameter ``other``:
    the polynomial to add to this.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Polynomial_::__idiv
static const char *__idiv =
R"doc(Divides this polynomial by the given constant.

This uses the division operator /= for the coefficient type *T*.

Precondition:
    The argument *scalar* is non-zero.

Parameter ``scalar``:
    the scalar factor to divide by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Polynomial_::__idiv_2
static const char *__idiv_2 =
R"doc(Divides this by the given polynomial.

More precisely: suppose there exist polynomials *q* and *r* with
coefficients of type *T* for which ``this = q.other + r``, and where
*r* has smaller degree than *other*. Then we call *q* the _quotient_,
and *r* the _remainder_.

This routine replaces this polynomial with the quotient *q*, and
discards the remainder. If you need to keep the remainder also, then
call divisionAlg() instead.

Coefficients are divided using the operator /= on type *T*.

If your coefficient type *T* is not a field (e.g., if *T* is Integer),
you must be sure to know in advance that the quotient exists (see the
precondition below). Otherwise the behaviour of this routine is
undefined.

Precondition:
    The given polynomial is not the zero polynomial.

Precondition:
    The quotient as defined above exists. If *T* is a field type
    (e.g., if *T* is Rational) then this is true automatically. If not
    (e.g., if *T* is Integer) then this requires some prior knowledge
    about the arguments.

Parameter ``other``:
    the polynomial to divide this by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Polynomial_::__imul
static const char *__imul =
R"doc(Multiplies this polynomial by the given constant.

Parameter ``scalar``:
    the scalar factor to multiply by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Polynomial_::__imul_2
static const char *__imul_2 =
R"doc(Multiplies this by the given polynomial.

Parameter ``other``:
    the polynomial to multiply this by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Polynomial_::__init
static const char *__init =
R"doc(Creates the polynomial ``x^d`` for the given degree *d*.

Parameter ``degree``:
    the degree of the new polynomial.)doc";

// Docstring regina::python::doc::Polynomial_::__init_2
static const char *__init_2 =
R"doc(Creates a new polynomial from the given sequence of coefficients. The
coefficients should be given in order from the constant coefficient to
the leading coefficient.

There is no problem if the leading coefficient (i.e., the last
coefficient in the sequence) is zero. An empty sequence will be
treated as the zero polynomial.

This constructor induces a deep copy of the given range.

Precondition:
    Objects of type *T* can be assigned values from dereferenced
    iterators of type *iterator*.

Python:
    Instead of a pair of iterators, this routine takes a python list
    of coefficients.

Parameter ``begin``:
    the beginning of the sequence of coefficients.

Parameter ``end``:
    a past-the-end iterator indicating the end of the sequence of
    coefficients.)doc";

// Docstring regina::python::doc::Polynomial_::__isub
static const char *__isub =
R"doc(Subtracts the given polynomial from this.

The given polynomial need not have the same degree as this. Note that
the degree of this polynomial might change as a result of this
operation.

Parameter ``other``:
    the polynomial to subtract from this.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Polynomial_::__ne
static const char *__ne =
R"doc(Tests whether this and the given polynomial are not equal.

Parameter ``rhs``:
    the polynomial to compare with this.

Returns:
    ``True`` if and only if this and the given polynomial are not
    equal.)doc";

// Docstring regina::python::doc::Polynomial_::degree
static const char *degree =
R"doc(Returns the degree of this polynomial. This is the largest exponent
with a non-zero coefficient.

For the purposes of this class, the zero polynomial is considered to
have degree zero.

Returns:
    the degree of this polynomial.)doc";

// Docstring regina::python::doc::Polynomial_::divisionAlg
static const char *divisionAlg =
R"doc(Divides this by the given divisor, and returns both the quotient and
the remainder.

More precisely: suppose there exist polynomials *q* and *r* with
coefficients of type *T* for which ``this = q.divisor + r``, and where
*r* has smaller degree than *divisor*. Then this routine returns the
pair (*q*, *r*); that is, the _quotient_ and the _remainder_.

If you do not need the remainder (e.g., if you know in advance that
*divisor* divides into this polynomial exactly), then you can use the
division operator /= instead, which will be a little faster.

If your coefficient type *T* is not a field (e.g., if *T* is Integer),
you must be sure to know in advance that the quotient exists (see the
precondition below). Otherwise the behaviour of this routine is
undefined.

Coefficients are divided using the operator /= on type *T*.

Precondition:
    The given divisor is not the zero polynomial.

Precondition:
    The quotient as defined above exists. If *T* is a field type
    (e.g., if *T* is Rational) then this is true automatically. If not
    (e.g., if *T* is Integer) then this requires some prior knowledge
    about the arguments.

Parameter ``divisor``:
    the polynomial to divide this by.

Returns:
    a pair holding the quotient and remainder, as described above.)doc";

// Docstring regina::python::doc::Polynomial_::gcdWithCoeffs
static const char *gcdWithCoeffs =
R"doc(Calculates the greatest common divisor of this and the given
polynomial, and finds a linear combination of these polynomials that
gives this gcd.

The greatest common divisor will be a monic polynomial. The
polynomials returned in *u* and *v* will satisfy ``u⋅this + v⋅other =
gcd``.

As a special case, gcd(0,0) is considered to be zero.

Precondition:
    The coefficient type *T* represents a field. In particular,
    Rational is supported but Integer is not.

Parameter ``other``:
    the polynomial whose greatest common divisor with this polynomial
    we should compute.

Parameter ``gcd``:
    a polynomial whose contents will be destroyed and replaced with
    the greatest common divisor *d*, as described above.

Parameter ``u``:
    a polynomial whose contents will be destroyed and replaced with
    *u*, as described above.

Parameter ``v``:
    a polynomial whose contents will be destroyed and replaced with
    *v*, as described above.)doc";

// Docstring regina::python::doc::Polynomial_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given polynomials.

This global routine simply calls Polynomial<T>::swap(); it is provided
so that Polynomial<T> meets the C++ Swappable requirements.

Parameter ``a``:
    the first polynomial whose contents should be swapped.

Parameter ``b``:
    the second polynomial whose contents should be swapped.)doc";

// Docstring regina::python::doc::Polynomial_::init
static const char *init = R"doc(Sets this to become the zero polynomial.)doc";

// Docstring regina::python::doc::Polynomial_::init_2
static const char *init_2 =
R"doc(Sets this to become the polynomial ``x^d`` for the given degree *d*.

Parameter ``degree``:
    the new degree of this polynomial.)doc";

// Docstring regina::python::doc::Polynomial_::init_3
static const char *init_3 =
R"doc(Sets this to become the polynomial described by the given sequence of
coefficients. The coefficients should appear in order from the
constant coefficient to the leading coefficient.

There is no problem if the leading coefficient (i.e., the last
coefficient in the sequence) is zero. An empty sequence will be
treated as the zero polynomial.

This routine induces a deep copy of the given range.

Precondition:
    Objects of type *T* can be assigned values from dereferenced
    iterators of type *iterator*.

Python:
    Instead of a pair of iterators, this routine takes a python list
    of coefficients.

Parameter ``begin``:
    the beginning of the sequence of coefficients.

Parameter ``end``:
    a past-the-end iterator indicating the end of the sequence of
    coefficients.)doc";

// Docstring regina::python::doc::Polynomial_::isMonic
static const char *isMonic =
R"doc(Returns whether this polynomial is monic. A _monic_ polynomial is a
non-zero polynomial whose leading coefficient is one.

Returns:
    ``True`` if and only if this is monic.)doc";

// Docstring regina::python::doc::Polynomial_::isZero
static const char *isZero =
R"doc(Returns whether this is the zero polynomial.

Returns:
    ``True`` if and only if this is the zero polynomial.)doc";

// Docstring regina::python::doc::Polynomial_::leading
static const char *leading =
R"doc(Returns the leading coefficient of this polynomial. If this is the
zero polynomial, then the leading coefficient will be zero.

Python:
    This routine returns by value, not by reference, since constness
    is important here and Python cannot enforce it otherwise.

Returns:
    the leading coefficient of this polynomial.)doc";

// Docstring regina::python::doc::Polynomial_::negate
static const char *negate = R"doc(Negates this polynomial. This field element is changed directly.)doc";

// Docstring regina::python::doc::Polynomial_::set
static const char *set =
R"doc(Changes the given coefficient of this polynomial.

It is fine to set the leading coefficient to zero, though note that
degree() will now return a smaller value as a result.

It is also fine to set a coefficient whose exponent is larger than the
current degree; this time degree() will now return a larger value
(unless the given coefficient is zero). Such an operation is
expensive, however, since it will require deallocating and
reallocating the full list of coefficients.

Python:
    This set() routine is available, but you can also set coefficients
    directly using syntax of the form ``p[exp] = value``.

Parameter ``exp``:
    the exponent of the term whose coefficient should be changed.

Parameter ``value``:
    the new value of this coefficient.)doc";

// Docstring regina::python::doc::Polynomial_::str
static const char *str =
R"doc(Returns this polynomial as a human-readable string, using the given
variable name instead of ``x``.

.. note::
    There is also the usual variant of str() which takes no arguments;
    that variant is inherited from the Output class.

Parameter ``variable``:
    the symbol to use for the variable in this polynomial. This may be
    ``None``, in which case the default variable ``x`` will be used.

Returns:
    this polynomial as a human-readable string.)doc";

// Docstring regina::python::doc::Polynomial_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given polynomial. This is a fast
(constant time) operation.

This and the given polynomial do not need to have the same degree.

Parameter ``other``:
    the polynomial whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Polynomial_::utf8
static const char *utf8 =
R"doc(Returns this polynomial as a human-readable string using unicode
characters, using the given variable name instead of ``x``.

This is similar to the output from str(), except that it uses unicode
characters to make the output more pleasant to read. In particular, it
makes use of superscript digits for exponents.

The string is encoded in UTF-8.

.. note::
    There is also the usual variant of utf8() which takes no
    arguments; that variant is inherited from the Output class.

Parameter ``variable``:
    the symbol to use for the variable in this polynomial. This may be
    ``None``, in which case the default variable ``x`` will be used.

Returns:
    this polynomial as a unicode-enabled human-readable string.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

