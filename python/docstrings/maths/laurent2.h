/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Laurent2
static const char *Laurent2 =
R"doc(Represents a Laurent polynomial in the two variables *x*, *y* with
coefficients of type *T*. A Laurent polynomial differs from an
ordinary polynomial in that it allows negative exponents (so, for
example, you can represent a polynomial such as ``2 + 3x² + y/x -
1/y³``).

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

The underlying storage method for this class is sparse: only the non-
zero coefficients are stored.

See also the class Laurent, which describes Laurent polynomials in
just one variable.

Python:
    In Python, the class Laurent2 refers to the specific template
    class Laurent2<Integer>.)doc";

// Docstring regina::python::doc::__add
static const char *__add =
R"doc(Adds the two given polynomials.

The two polynomials need not have the same range of non-zero
coefficients.

Parameter ``lhs``:
    the first polynomial to add.

Parameter ``rhs``:
    the second polynomial to add.

Returns:
    the sum of both polynomials.)doc";

// Docstring regina::python::doc::__add_2
static const char *__add_2 =
R"doc(Adds the two given polynomials.

The two polynomials need not have the same range of non-zero
coefficients.

Parameter ``lhs``:
    the first polynomial to add.

Parameter ``rhs``:
    the second polynomial to add.

Returns:
    the sum of both polynomials.)doc";

// Docstring regina::python::doc::__add_3
static const char *__add_3 =
R"doc(Adds the two given polynomials.

The two polynomials need not have the same range of non-zero
coefficients.

Parameter ``lhs``:
    the first polynomial to add.

Parameter ``rhs``:
    the second polynomial to add.

Returns:
    the sum of both polynomials.)doc";

// Docstring regina::python::doc::__add_4
static const char *__add_4 =
R"doc(Adds the two given polynomials.

The two polynomials need not have the same range of non-zero
coefficients.

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
Laurent2<T>::Coefficient here to assist with C++ template type
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
Laurent2<T>::Coefficient here to assist with C++ template type
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
Laurent2<T>::Coefficient here to assist with C++ template type
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

The two polynomials need not have the same range of non-zero
coefficients.

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

The two polynomials need not have the same range of non-zero
coefficients.

Parameter ``lhs``:
    the polynomial to subtract from.

Parameter ``rhs``:
    the polynomial to subtract.

Returns:
    the first polynomial minus the second.)doc";

// Docstring regina::python::doc::__sub_3
static const char *__sub_3 =
R"doc(Subtracts the two given polynomials.

The two polynomials need not have the same range of non-zero
coefficients.

Parameter ``lhs``:
    the polynomial to subtract from.

Parameter ``rhs``:
    the polynomial to subtract.

Returns:
    the first polynomial minus the second.)doc";

// Docstring regina::python::doc::__sub_4
static const char *__sub_4 =
R"doc(Subtracts the two given polynomials.

The two polynomials need not have the same range of non-zero
coefficients.

Parameter ``lhs``:
    the polynomial to subtract from.

Parameter ``rhs``:
    the polynomial to subtract.

Returns:
    the first polynomial minus the second.)doc";

// Docstring regina::python::doc::__sub_5
static const char *__sub_5 =
R"doc(Subtracts the two given polynomials.

The two polynomials need not have the same range of non-zero
coefficients.

Parameter ``lhs``:
    the polynomial to subtract from.

Parameter ``rhs``:
    the polynomial to subtract.

Returns:
    the first polynomial minus the second.)doc";

namespace Laurent2_ {

// Docstring regina::python::doc::Laurent2_::__call
static const char *__call =
R"doc(Returns the given coefficient of this polynomial. There are no
restrictions on the exponents *xExp* and *yExp*.

Python:
    In Python, this is the square bracket operator, not the round
    bracket operator; that is, Python users can access coefficients
    through the syntax ``poly[xExp, yExp]``. Moreover, this operator
    can also _set_ cofficients; that is, you can write ``poly[xExp,
    yExp] = value``. However, when _getting_ a coefficient this
    operator will return by value (to enforce constness), which means
    for example you cannot write something like ``poly[xExp,
    yExp].negate()``.

Parameter ``xExp``:
    the exponent attached to *x*.

Parameter ``yExp``:
    the exponent attached to *y*.

Returns:
    the coefficient of the term with the given exponents.)doc";

// Docstring regina::python::doc::Laurent2_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given polynomial.

This constructor induces a deep copy of *value*.

A note for developers: even though this routine is identical to the
templated copy constructor, it must be declared and implemented
separately. Otherwise the compiler might create its own (incorrect)
copy constructor automatically.

Parameter ``value``:
    the polynomial to clone.)doc";

// Docstring regina::python::doc::Laurent2_::__default
static const char *__default = R"doc(Creates the zero polynomial.)doc";

// Docstring regina::python::doc::Laurent2_::__eq
static const char *__eq =
R"doc(Tests whether this and the given polynomial are equal.

Parameter ``rhs``:
    the polynomial to compare with this.

Returns:
    ``True`` if and only if this and the given polynomial are equal.)doc";

// Docstring regina::python::doc::Laurent2_::__ge
static const char *__ge =
R"doc(Compares this against the given polynomial under a total ordering of
all two-variable Laurent polynomials.

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

// Docstring regina::python::doc::Laurent2_::__gt
static const char *__gt =
R"doc(Compares this against the given polynomial under a total ordering of
all two-variable Laurent polynomials.

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

// Docstring regina::python::doc::Laurent2_::__iadd
static const char *__iadd =
R"doc(Adds the given polynomial to this.

This and the given polynomial need not have the same range of non-zero
coefficients.

Parameter ``other``:
    the polynomial to add to this.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent2_::__idiv
static const char *__idiv =
R"doc(Divides this polynomial by the given constant.

This uses the division operator /= for the coefficient type *T*.

Precondition:
    The argument *scalar* is non-zero.

Parameter ``scalar``:
    the scalar factor to divide by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent2_::__imul
static const char *__imul =
R"doc(Multiplies this polynomial by the given constant.

Parameter ``scalar``:
    the scalar factor to multiply by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent2_::__imul_2
static const char *__imul_2 =
R"doc(Multiplies this by the given polynomial.

This and the given polynomial need not have the same range of non-zero
coefficients.

Parameter ``other``:
    the polynomial to multiply this by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent2_::__init
static const char *__init =
R"doc(Creates the polynomial ``x^d y^e`` for the given exponents *d* and
*e*.

Parameter ``xExp``:
    the exponent *d*, which is attached to *x*.

Parameter ``yExp``:
    the exponent *e*, which is attached to *y*.)doc";

// Docstring regina::python::doc::Laurent2_::__init_2
static const char *__init_2 =
R"doc(Creates a copy of the given polynomial with all terms multiplied by
``x^d y^e`` for some integers *d* and *e*.

This constructor induces a deep (and modified) copy of *value*.

Parameter ``toShift``:
    the polynomial to clone and shift.

Parameter ``xShift``:
    the integer *d*, which will be added to all exponents for *x*.

Parameter ``yShift``:
    the integer *e*, which will be added to all exponents for *y*.)doc";

// Docstring regina::python::doc::Laurent2_::__init_3
static const char *__init_3 =
R"doc(Creates a new polynomial from the given collection of coefficients.

The coefficients should be presented as a collection of tuples of the
form (*d*, *e*, *v*), each representing a term of the form ``v x^d
y^e``.

The tuples may be given in any order. An empty sequence will be
treated as the zero polynomial.

Unlike the std::initializer_list constructor, zero coefficients are
allowed (these will be silently ignored), and multiple coefficients
with the same exponents are also allowed (these will be aggregated
using the += operator).

Python:
    Instead of the iterators *begin* and *end*, this routine takes a
    python list of tuples.

Template parameter ``iterator``:
    an iterator type which, when dereferenced, gives a std::tuple of
    the form (*d*, *e*, *v*), where *d* and *e* can be assigned to
    long integers, and where *v* can be assigned to type *T*.

Template parameter ``deref``:
    a dummy argument that should be ignored. This is present to ensure
    that *iterator* can be dereferenced, so that a call such as
    Laurent2(int, int) falls through to the (long, long) constructor,
    and not this iterator-based constructor instead.

Parameter ``begin``:
    the beginning of the set of coefficients, as outlined above.

Parameter ``end``:
    a past-the-end iterator indicating the end of the set of
    coefficients.)doc";

// Docstring regina::python::doc::Laurent2_::__isub
static const char *__isub =
R"doc(Subtracts the given polynomial from this.

This and the given polynomial need not have the same range of non-zero
coefficients.

Parameter ``other``:
    the polynomial to subtract from this.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent2_::__le
static const char *__le =
R"doc(Compares this against the given polynomial under a total ordering of
all two-variable Laurent polynomials.

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

// Docstring regina::python::doc::Laurent2_::__lt
static const char *__lt =
R"doc(Compares this against the given polynomial under a total ordering of
all two-variable Laurent polynomials.

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

// Docstring regina::python::doc::Laurent2_::__ne
static const char *__ne =
R"doc(Tests whether this and the given polynomial are not equal.

Parameter ``rhs``:
    the polynomial to compare with this.

Returns:
    ``True`` if and only if this and the given polynomial are not
    equal.)doc";

// Docstring regina::python::doc::Laurent2_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given polynomials.

This global routine simply calls Laurent2<T>::swap(); it is provided
so that Laurent2<T> meets the C++ Swappable requirements.

Parameter ``a``:
    the first polynomial whose contents should be swapped.

Parameter ``b``:
    the second polynomial whose contents should be swapped.)doc";

// Docstring regina::python::doc::Laurent2_::init
static const char *init = R"doc(Sets this to become the zero polynomial.)doc";

// Docstring regina::python::doc::Laurent2_::init_2
static const char *init_2 =
R"doc(Sets this to become the polynomial ``x^d y^e`` for the given exponents
*d* and *e*.

Parameter ``xExp``:
    the new exponent *d*, which is attached to *x*.

Parameter ``yExp``:
    the new exponent *e*, which is attached to *y*.)doc";

// Docstring regina::python::doc::Laurent2_::invertX
static const char *invertX =
R"doc(Replaces ``x`` with ``x^-1`` in this polynomial. This polynomial is
changed directly.)doc";

// Docstring regina::python::doc::Laurent2_::invertY
static const char *invertY =
R"doc(Replaces ``y`` with ``y^-1`` in this polynomial. This polynomial is
changed directly.)doc";

// Docstring regina::python::doc::Laurent2_::isZero
static const char *isZero =
R"doc(Returns whether this is the zero polynomial.

Returns:
    ``True`` if and only if this is the zero polynomial.)doc";

// Docstring regina::python::doc::Laurent2_::negate
static const char *negate = R"doc(Negates this polynomial. This polynomial is changed directly.)doc";

// Docstring regina::python::doc::Laurent2_::set
static const char *set =
R"doc(Changes the given coefficient of this polynomial.

There are no restrictions on the exponents *xExp* and *yExp*, and the
new coefficient *value* may be zero.

Moreover, the underlying data structures ensure that this operation is
cheap regardless of the exponents involved.

Python:
    This set() routine is available, but you can also set coefficients
    directly using syntax of the form ``p[xExp, yExp] = value``.

Parameter ``xExp``:
    the exponent attached to *x*.

Parameter ``yExp``:
    the exponent attached to *y*.

Parameter ``value``:
    the new value of the corresponding coefficient.)doc";

// Docstring regina::python::doc::Laurent2_::str
static const char *str =
R"doc(Returns this polynomial as a human-readable string, using the given
variable names instead of ``x`` and ``y``.

.. note::
    There is also the usual variant of str() which takes no arguments;
    that variant is inherited from the Output class.

Parameter ``varX``:
    the symbol to use for the variable *x*. This may be ``None``, in
    which case the default symbol ``x`` will be used.

Parameter ``varY``:
    the symbol to use for the variable *y*. This may be ``None``, in
    which case the default symbol ``y`` will be used.

Returns:
    this polynomial as a human-readable string.)doc";

// Docstring regina::python::doc::Laurent2_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given polynomial. This is a fast
(constant time) operation.

This and the given polynomial need not have the same range of non-zero
coefficients.

Parameter ``other``:
    the polynomial whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Laurent2_::utf8
static const char *utf8 =
R"doc(Returns this polynomial as a human-readable string using unicode
characters, using the given variable names instead of ``x`` and ``y``.

This is similar to the output from str(), except that it uses unicode
characters to make the output more pleasant to read. In particular, it
makes use of superscript digits for exponents and a wider minus sign.

The string is encoded in UTF-8.

.. note::
    There is also the usual variant of utf8() which takes no
    arguments; that variant is inherited from the Output class.

Parameter ``varX``:
    the symbol to use for the variable *x*. This may be ``None``, in
    which case the default symbol ``x`` will be used.

Parameter ``varY``:
    the symbol to use for the variable *y*. This may be ``None``, in
    which case the default symbol ``y`` will be used.

Returns:
    this polynomial as a unicode-enabled human-readable string.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

