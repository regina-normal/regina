/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Laurent
static const char *Laurent =
R"doc(Represents a single-variable Laurent polynomial with coefficients of
type *T*. A Laurent polynomial differs from an ordinary polynomial in
that it allows negative exponents (so, unlike the Polynomial class,
you can represent both ``2+3x`` and ``1+1/x``).

The type *T* must represent a ring with no zero divisors. In
particular, it must:

* support basic arithmetic operations;

* support assignments of the form ``x = int`` and tests of the form
  ``x == int`` and ``x < int``;

* have a default constructor that assigns an explicit value of zero.

This means that Regina's numerical types such as Integer and Rational
are supported, but native data types such as int and long are not
(since they have no zero-initialising default constructor).

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

The underlying storage method for this class is dense (i.e., all
coefficients are explicitly stored, including zero coefficients).

See also the class Laurent2, which describes Laurent polynomials in
two variables.

Python:
    In Python, the class Laurent refers to the specific template class
    Laurent<Integer>.)doc";

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
Laurent<T>::Coefficient here to assist with C++ template type
matching.

Precondition:
    The argument *scalar* is non-zero.

Parameter ``poly``:
    the polynomial to divide by the given scalar.

Parameter ``scalar``:
    the scalar factor to divide by.

Returns:
    the quotient of the given polynomial by the given scalar.)doc";

// Docstring regina::python::doc::__mul
static const char *__mul =
R"doc(Multiplies the given polynomial by the given scalar constant.

The scalar is simply of type *T*; we use the identical type
Laurent<T>::Coefficient here to assist with C++ template type
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
Laurent<T>::Coefficient here to assist with C++ template type
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

namespace Laurent_ {

// Docstring regina::python::doc::Laurent_::__array
static const char *__array =
R"doc(Returns the given coefficient of this polynomial. There are no
restrictions on the exponent *exp*.

Python:
    Python users can also use this operator to _set_ cofficients; that
    is, you can write ``poly[exp] = value``. However, when _getting_ a
    coefficient this operator will return by value (to enforce
    constness), which means for example you cannot write something
    like ``poly[exp].negate()``.

Parameter ``exp``:
    the exponent of the term whose coefficient should be returned.

Returns:
    the coefficient of the given term.)doc";

// Docstring regina::python::doc::Laurent_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given polynomial.

This constructor induces a deep copy of *value*.

A note for developers: even though this routine is identical to the
templated copy constructor, it must be declared and implemented
separately. Otherwise the compiler might create its own (incorrect)
copy constructor automatically.

Parameter ``value``:
    the polynomial to clone.)doc";

// Docstring regina::python::doc::Laurent_::__default
static const char *__default = R"doc(Creates the zero polynomial.)doc";

// Docstring regina::python::doc::Laurent_::__eq
static const char *__eq =
R"doc(Tests whether this and the given polynomial are equal.

Parameter ``rhs``:
    the polynomial to compare with this.

Returns:
    ``True`` if and only if this and the given polynomial are equal.)doc";

// Docstring regina::python::doc::Laurent_::__ge
static const char *__ge =
R"doc(Compares this against the given polynomial under a total ordering of
all one-variable Laurent polynomials.

The particular total order that Regina uses is not important, and may
change between Regina releases (though such changes should be very
infrequent). The main purpose of this routine is to support algorithms
that require a "canonical" choice of polynomial from amongst many
alternatives.

Parameter ``rhs``:
    the polynomial to compare with this.

Returns:
    ``True`` if and only if this is greater than or equal to the given
    polynomial under the total order that Regina uses.)doc";

// Docstring regina::python::doc::Laurent_::__gt
static const char *__gt =
R"doc(Compares this against the given polynomial under a total ordering of
all one-variable Laurent polynomials.

The particular total order that Regina uses is not important, and may
change between Regina releases (though such changes should be very
infrequent). The main purpose of this routine is to support algorithms
that require a "canonical" choice of polynomial from amongst many
alternatives.

Parameter ``rhs``:
    the polynomial to compare with this.

Returns:
    ``True`` if and only if this is greater than the given polynomial
    under the total order that Regina uses.)doc";

// Docstring regina::python::doc::Laurent_::__iadd
static const char *__iadd =
R"doc(Adds the given polynomial to this.

The given polynomial need not have the same minimum and/or maximum
exponents as this.

.. warning::
    This routine may trigger a deep copy (depending upon the range of
    exponents used in *other*). Consider using the binary ``+``
    operator instead, which is better able to avoid this deep copy
    where possible.

Parameter ``other``:
    the polynomial to add to this.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent_::__idiv
static const char *__idiv =
R"doc(Divides this polynomial by the given constant.

This uses the division operator /= for the coefficient type *T*.

Precondition:
    The argument *scalar* is non-zero.

Parameter ``scalar``:
    the scalar factor to divide by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent_::__imul
static const char *__imul =
R"doc(Multiplies this polynomial by the given constant.

Parameter ``scalar``:
    the scalar factor to multiply by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent_::__imul_2
static const char *__imul_2 =
R"doc(Multiplies this by the given polynomial.

The given polynomial need not have the same minimum and/or maximum
exponents as this.

Parameter ``other``:
    the polynomial to multiply this by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent_::__init
static const char *__init =
R"doc(Creates the polynomial ``x^d`` for the given exponent *d*.

Parameter ``exponent``:
    the exponent to use for the new polynomial.)doc";

// Docstring regina::python::doc::Laurent_::__init_2
static const char *__init_2 =
R"doc(Creates a new polynomial from the given sequence of coefficients.

The coefficients should be given in order from the smallest exponent
term to the largest. The first coefficient in the sequence will be
associated with the exponent *minExp*.

There is no problem if the first and/or last coefficient in the
sequence is zero. An empty sequence will be treated as the zero
polynomial.

This constructor induces a deep copy of the given range.

Precondition:
    Objects of type *T* can be assigned values from dereferenced
    iterators of type *iterator*.

Python:
    Instead of the iterators *begin* and *end*, this routine takes a
    python list of coefficients.

Parameter ``minExp``:
    the exponent corresponding to the first coefficient in the
    sequence.

Parameter ``begin``:
    the beginning of the sequence of coefficients.

Parameter ``end``:
    a past-the-end iterator indicating the end of the sequence of
    coefficients.)doc";

// Docstring regina::python::doc::Laurent_::__isub
static const char *__isub =
R"doc(Subtracts the given polynomial from this.

The given polynomial need not have the same minimum and/or maximum
exponents as this.

Parameter ``other``:
    the polynomial to subtract from this.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent_::__le
static const char *__le =
R"doc(Compares this against the given polynomial under a total ordering of
all one-variable Laurent polynomials.

The particular total order that Regina uses is not important, and may
change between Regina releases (though such changes should be very
infrequent). The main purpose of this routine is to support algorithms
that require a "canonical" choice of polynomial from amongst many
alternatives.

Parameter ``rhs``:
    the polynomial to compare with this.

Returns:
    ``True`` if and only if this is less than or equal to the given
    polynomial under the total order that Regina uses.)doc";

// Docstring regina::python::doc::Laurent_::__lt
static const char *__lt =
R"doc(Compares this against the given polynomial under a total ordering of
all one-variable Laurent polynomials.

The particular total order that Regina uses is not important, and may
change between Regina releases (though such changes should be very
infrequent). The main purpose of this routine is to support algorithms
that require a "canonical" choice of polynomial from amongst many
alternatives.

Parameter ``rhs``:
    the polynomial to compare with this.

Returns:
    ``True`` if and only if this is less than the given polynomial
    under the total order that Regina uses.)doc";

// Docstring regina::python::doc::Laurent_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given polynomials.

This global routine simply calls Laurent<T>::swap(); it is provided so
that Laurent<T> meets the C++ Swappable requirements.

Parameter ``a``:
    the first polynomial whose contents should be swapped.

Parameter ``b``:
    the second polynomial whose contents should be swapped.)doc";

// Docstring regina::python::doc::Laurent_::init
static const char *init = R"doc(Sets this to become the zero polynomial.)doc";

// Docstring regina::python::doc::Laurent_::init_2
static const char *init_2 =
R"doc(Sets this to become the polynomial ``x^d`` for the given exponent *d*.

Parameter ``exponent``:
    the new exponent to use for this polynomial.)doc";

// Docstring regina::python::doc::Laurent_::init_3
static const char *init_3 =
R"doc(Sets this to become the polynomial described by the given sequence of
coefficients.

The coefficients should appear in order from the smallest exponent
term to the largest. The first coefficient in the sequence will be
associated with the exponent *minExp*.

There is no problem if the first and/or last coefficient in the
sequence is zero. An empty sequence will be treated as the zero
polynomial.

This routine induces a deep copy of the given range.

Precondition:
    Objects of type *T* can be assigned values from dereferenced
    iterators of type *iterator*.

Python:
    Instead of the iterators *begin* and *end*, this routine takes a
    python list of coefficients.

Parameter ``minExp``:
    the exponent corresponding to the first coefficient in the
    sequence.

Parameter ``begin``:
    the beginning of the sequence of coefficients.

Parameter ``end``:
    a past-the-end iterator indicating the end of the sequence of
    coefficients.)doc";

// Docstring regina::python::doc::Laurent_::invertX
static const char *invertX =
R"doc(Replaces ``x`` with ``x^-1`` in this polynomial. This polynomial is
changed directly.

Calling this routine is equivalent to calling ``scaleUp(-1)``.)doc";

// Docstring regina::python::doc::Laurent_::isZero
static const char *isZero =
R"doc(Returns whether this is the zero polynomial.

Returns:
    ``True`` if and only if this is the zero polynomial.)doc";

// Docstring regina::python::doc::Laurent_::maxExp
static const char *maxExp =
R"doc(Returns the largest exponent that appears in this polynomial with a
non-zero coefficient. If this is the zero polynomial, then this
routine returns 0.

Returns:
    the largest exponent.)doc";

// Docstring regina::python::doc::Laurent_::minExp
static const char *minExp =
R"doc(Returns the smallest exponent that appears in this polynomial with a
non-zero coefficient. If this is the zero polynomial, then this
routine returns 0.

Returns:
    the smallest exponent.)doc";

// Docstring regina::python::doc::Laurent_::negate
static const char *negate = R"doc(Negates this polynomial. This polynomial is changed directly.)doc";

// Docstring regina::python::doc::Laurent_::scaleDown
static const char *scaleDown =
R"doc(Divides all exponents in this polynomial by *k* for some integer *k*.
This is equivalent to replacing the variable *x* of the polynomial
with *x*^{1/*k*}.

Both positive and negative scaling factors *k* are allowed.

Precondition:
    *k* is non-zero.

Precondition:
    All exponents in this polynomial with non-zero coefficients are
    multiples of *k*.

Parameter ``k``:
    the scaling factor to divide exponents by.)doc";

// Docstring regina::python::doc::Laurent_::scaleUp
static const char *scaleUp =
R"doc(Multiplies all exponents in this polynomial by *k* for some integer
*k*. This is equivalent to replacing the variable *x* of the
polynomial with *x*^{*k*}.

Both positive and negative scaling factors *k* are allowed.

Precondition:
    *k* is non-zero.

Parameter ``k``:
    the scaling factor to multiply exponents by.)doc";

// Docstring regina::python::doc::Laurent_::set
static const char *set =
R"doc(Changes the given coefficient of this polynomial.

There are no restrictions on the exponent *exp*, and the new
coefficient *value* may be zero.

Note, however, that it is expensive to set a non-zero coefficient
whose exponent is larger than maxExp() or smaller than minExp(), since
this will typically require deallocating and reallocating the full
list of coefficients.

In contrast, setting a zero coefficient for the exponent maxExp() or
minExp() is cheap, even though the range of non-zero coefficients
changes as a result.

Python:
    This set() routine is available, but you can also set coefficients
    directly using syntax of the form ``p[exp] = value``.

Parameter ``exp``:
    the exponent of the term whose coefficient should be changed.

Parameter ``value``:
    the new value of this coefficient.)doc";

// Docstring regina::python::doc::Laurent_::shift
static const char *shift =
R"doc(Multiplies this polynomial by ``x^s`` for some integer *s*.

Parameter ``s``:
    the power of *x* to multiply by.)doc";

// Docstring regina::python::doc::Laurent_::str
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

// Docstring regina::python::doc::Laurent_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given polynomial. This is a fast
(constant time) operation.

This and the given polynomial do not need to have the same minimum
and/or maximum exponents.

Parameter ``other``:
    the polynomial whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Laurent_::utf8
static const char *utf8 =
R"doc(Returns this polynomial as a human-readable string using unicode
characters, using the given variable name instead of ``x``.

This is similar to the output from str(), except that it uses unicode
characters to make the output more pleasant to read. In particular, it
makes use of superscript digits for exponents and a wider minus sign.

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

