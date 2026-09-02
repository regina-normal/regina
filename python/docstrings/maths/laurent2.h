/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct Laurent2 {

// Docstring regina::python::doc::Laurent2::__add
static constexpr const char __add[] =
R"doc(Adds the two given polynomials.

The two polynomials need not have the same range of non-zero
coefficients.

Parameter ``lhs``:
    the first polynomial to add.

Parameter ``rhs``:
    the second polynomial to add.

Returns:
    the sum of both polynomials.)doc";

// Docstring regina::python::doc::Laurent2::__call
static constexpr const char __call[] =
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

// Docstring regina::python::doc::Laurent2::__class
static constexpr const char __class[] =
R"doc(Represents a Laurent polynomial in the two variables *x*, *y* with
coefficients of type *T*. A Laurent polynomial differs from an
ordinary polynomial in that it allows negative exponents (so, for
example, you can represent a polynomial such as ``2 + 3x² + y/x -
1/y³``).

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

The underlying storage method for this class is sparse: only the non-
zero coefficients are stored.

See also the class Laurent, which describes Laurent polynomials in
just one variable.

Python:
    In Python, the class Laurent2 refers to the specific template
    class ``Laurent2<Integer>``.

Template parameter ``T``:
    the coefficient type. A typical coefficient type would be Integer
    or Rational. Note that native C++ integer types are _not_
    supported (since they have no zero-initialising default
    constructor).)doc";

// Docstring regina::python::doc::Laurent2::__cmp
static constexpr const char __cmp[] =
R"doc(Compares this against the given polynomial under a total ordering of
all two-variable Laurent polynomials.

The particular total order that Regina uses is not important, and may
change between Regina releases (though such changes should be very
infrequent). The main purpose of this routine is to support algorithms
that require a "canonical" choice of polynomial from amongst many
alternatives.

This routine generates all of the usual comparison operators,
including ``<``, ``<=``, ``>``, and ``>=``.

Python:
    This spaceship operator ``x <=> y`` is not available, but the
    other comparison operators that it generates _are_ available.

Parameter ``rhs``:
    the polynomial to compare with this.

Returns:
    the result of the comparison between this and the given
    polynomial.)doc";

// Docstring regina::python::doc::Laurent2::__copy
static constexpr const char __copy[] =
R"doc(Creates a new copy of the given polynomial.

This constructor induces a deep copy of *value*.

A note for developers: even though this routine is identical to the
templated copy constructor, it must be declared and implemented
separately. Otherwise the compiler might create its own (incorrect)
copy constructor automatically.

Parameter ``value``:
    the polynomial to clone.)doc";

// Docstring regina::python::doc::Laurent2::__default
static constexpr const char __default[] = R"doc(Creates the zero polynomial.)doc";

// Docstring regina::python::doc::Laurent2::__div
static constexpr const char __div[] =
R"doc(Divides the given polynomial by the given scalar constant.

This uses the division operator ``/=`` for the coefficient type *T*.

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

// Docstring regina::python::doc::Laurent2::__div_2
static constexpr const char __div_2[] =
R"doc(Divides the given polynomial by the given scalar constant.

This uses the division operator ``/=`` for the coefficient type *T*.

The extent to which this operator can handle large native integer
types without overflow will depend on how well such integer types are
supported by the coefficient type *T*.

Precondition:
    The argument *scalar* is non-zero.

Python:
    It is assumed that the type *IntType* is ``long``.

Parameter ``poly``:
    the polynomial to divide by the given scalar.

Parameter ``scalar``:
    the scalar factor to divide by.

Returns:
    the quotient of the given polynomial by the given scalar.)doc";

// Docstring regina::python::doc::Laurent2::__eq
static constexpr const char __eq[] =
R"doc(Tests whether this and the given polynomial are equal.

Parameter ``rhs``:
    the polynomial to compare with this.

Returns:
    ``True`` if and only if this and the given polynomial are equal.)doc";

// Docstring regina::python::doc::Laurent2::__eq_2
static constexpr const char __eq_2[] =
R"doc(Tests whether this polynomial is equal to the given constant.

Parameter ``constant``:
    the value to compare this polynomial against.

Returns:
    ``True`` if and only if this polynomial is equal to the given
    constant.)doc";

// Docstring regina::python::doc::Laurent2::__eq_3
static constexpr const char __eq_3[] =
R"doc(Tests whether this polynomial is equal to the given integer constant.

The extent to which this operator can handle large native integer
types without overflow will depend on how well such integer types are
supported by the coefficient type *T*.

Python:
    It is assumed that the type *IntType* is ``long``.

Parameter ``constant``:
    the integer to compare this polynomial against.

Returns:
    ``True`` if and only if this polynomial is equal to the given
    integer constant.)doc";

// Docstring regina::python::doc::Laurent2::__iadd
static constexpr const char __iadd[] =
R"doc(Adds the given polynomial to this.

This and the given polynomial need not have the same range of non-zero
coefficients.

Parameter ``other``:
    the polynomial to add to this.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent2::__idiv
static constexpr const char __idiv[] =
R"doc(Divides this polynomial by the given constant.

This uses the division operator ``/=`` for the coefficient type *T*.

Precondition:
    The argument *scalar* is non-zero.

Parameter ``scalar``:
    the scalar factor to divide by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent2::__idiv_2
static constexpr const char __idiv_2[] =
R"doc(Divides this polynomial by the given integer constant.

This uses the division operator ``/=`` for the coefficient type *T*.

The extent to which this operator can handle large native integer
types without overflow will depend on how well such integer types are
supported by the coefficient type *T*.

Precondition:
    The argument *scalar* is non-zero.

Python:
    It is assumed that the type *IntType* is ``long``.

Parameter ``scalar``:
    the scalar factor to divide by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent2::__imul
static constexpr const char __imul[] =
R"doc(Multiplies this polynomial by the given constant.

Parameter ``scalar``:
    the scalar factor to multiply by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent2::__imul_2
static constexpr const char __imul_2[] =
R"doc(Multiplies this polynomial by the given integer constant.

The extent to which this operator can handle large native integer
types without overflow will depend on how well such integer types are
supported by the coefficient type *T*.

Python:
    It is assumed that the type *IntType* is ``long``.

Parameter ``scalar``:
    the scalar factor to multiply by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent2::__imul_3
static constexpr const char __imul_3[] =
R"doc(Multiplies this by the given polynomial.

This and the given polynomial need not have the same range of non-zero
coefficients.

Parameter ``other``:
    the polynomial to multiply this by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent2::__init
static constexpr const char __init[] =
R"doc(Creates a copy of the given polynomial with all terms multiplied by
``x^d y^e`` for some integers *d* and *e*.

This constructor induces a deep (and modified) copy of *value*.

Parameter ``toShift``:
    the polynomial to clone and shift.

Parameter ``xShift``:
    the integer *d*, which will be added to all exponents for *x*.

Parameter ``yShift``:
    the integer *e*, which will be added to all exponents for *y*.)doc";

// Docstring regina::python::doc::Laurent2::__init_2
static constexpr const char __init_2[] =
R"doc(Creates a new polynomial from the given collection of coefficients.

The coefficients should be presented as a collection of tuples of the
form ``(d, e, v)``, each representing a term of the form ``v x^d
y^e``.

The tuples may be given in any order. An empty sequence will be
treated as the zero polynomial.

Unlike the std::initializer_list constructor, zero coefficients are
allowed (these will be silently ignored), and multiple coefficients
with the same exponents are also allowed (these will be aggregated
using the += operator).

Python:
    Instead of the iterators *begin* and *end*, this routine takes a
    Python list of tuples.

Parameter ``begin``:
    the beginning of the set of coefficients, as outlined above.

Parameter ``end``:
    a past-the-end iterator indicating the end of the set of
    coefficients.)doc";

// Docstring regina::python::doc::Laurent2::__init_3
static constexpr const char __init_3[] =
R"doc(Creates a new polynomial with a single constant term, equal to the
given value.

There is no problem if the given value is zero.

Parameter ``constant``:
    the value of this new constant polynomial.)doc";

// Docstring regina::python::doc::Laurent2::__init_4
static constexpr const char __init_4[] =
R"doc(Creates a new polynomial with a single integer constant term.

There is no problem if the given constant is zero (though you can also
construct the zero polynomial by passing no arguments at all).

The extent to which this constructor can handle large native integer
types without overflow will depend on how well such integer types are
supported by the coefficient type *T*.

Python:
    It is assumed that the type *IntType* is ``long``.

Parameter ``constant``:
    the value of this new polynomial.)doc";

// Docstring regina::python::doc::Laurent2::__init_5
static constexpr const char __init_5[] =
R"doc(Creates a new two-variable Laurent polynomial from a one-variable
Laurent polynomial.

Specifically, this will become the two-variable polynomial
``poly(x^xExp y^yExp)``.

Parameter ``poly``:
    the one-variable polynomial into which we substitute ``x^xExp
    y^yExp``.

Parameter ``xExp``:
    the power of *x* to substitute into *poly*.

Parameter ``yExp``:
    the power of *y* to substitute into *poly*.)doc";

// Docstring regina::python::doc::Laurent2::__isub
static constexpr const char __isub[] =
R"doc(Subtracts the given polynomial from this.

This and the given polynomial need not have the same range of non-zero
coefficients.

Parameter ``other``:
    the polynomial to subtract from this.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent2::__iter__
static constexpr const char __iter__[] =
R"doc(Returns a Python iterator that provides read-only access to all non-
zero coefficients and their corresponding exponent pairs. If this is
the zero polynomial then this iterator range will be empty.

To enforce read-only access, Python iterators will return coefficients
and exponents by value, not by reference. If you wish to modify
coefficients or exponents then you will need to call different
routines, such as set().

Iterators in Laurent2 work differently from single-variable polynomial
classes such as Polynomial and Laurent:

* they run through only the _non-zero_ coefficients of the polynomial,
  ignoring any zero coefficients;

* each value returned by an iterator includes not only a coefficent,
  but also its corresponding exponent pair.

Specifically, each value returned by an iterator will be of the form
``((s, t), c)``, where round brackets indicate pairs (stored via
Python tuples). This value describes a single term in the polynomial
of the form ``c x^s y^t``.

The order of iteration will be lexicographic in the pair ``(s, t)``
above; that is, by increasing *x* exponent and then (in the case of
ties) by increasing *y* exponent.

Returns:
    an iterator over all coefficients and their corresponding exponent
    pairs.)doc";

// Docstring regina::python::doc::Laurent2::__mul
static constexpr const char __mul[] =
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

// Docstring regina::python::doc::Laurent2::__mul_2
static constexpr const char __mul_2[] =
R"doc(Multiplies the given polynomial by the given scalar constant.

The extent to which this operator can handle large native integer
types without overflow will depend on how well such integer types are
supported by the coefficient type *T*.

Python:
    It is assumed that the type *IntType* is ``long``.

Parameter ``poly``:
    the polynomial to multiply by.

Parameter ``scalar``:
    the scalar to multiply by.

Returns:
    the product of the given polynomial and scalar.)doc";

// Docstring regina::python::doc::Laurent2::__mul_3
static constexpr const char __mul_3[] =
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

// Docstring regina::python::doc::Laurent2::__mul_4
static constexpr const char __mul_4[] =
R"doc(Multiplies the given polynomial by the given scalar constant.

The extent to which this operator can handle large native integer
types without overflow will depend on how well such integer types are
supported by the coefficient type *T*.

Python:
    It is assumed that the type *IntType* is ``long``.

Parameter ``scalar``:
    the scalar to multiply by.

Parameter ``poly``:
    the polynomial to multiply by.

Returns:
    the product of the given polynomial and scalar.)doc";

// Docstring regina::python::doc::Laurent2::__mul_5
static constexpr const char __mul_5[] =
R"doc(Multiplies the two given polynomials.

The two polynomials need not have the same range of non-zero
coefficients.

Parameter ``lhs``:
    the first polynomial to multiply.

Parameter ``rhs``:
    the second polynomial to multiply.

Returns:
    the product of both polynomials.)doc";

// Docstring regina::python::doc::Laurent2::__sub
static constexpr const char __sub[] =
R"doc(Returns the negative of the given polynomial.

Parameter ``arg``:
    the polynomial to negate.

Returns:
    the negative of *arg*.)doc";

// Docstring regina::python::doc::Laurent2::__sub_2
static constexpr const char __sub_2[] =
R"doc(Subtracts the two given polynomials.

The two polynomials need not have the same range of non-zero
coefficients.

Parameter ``lhs``:
    the polynomial to subtract from.

Parameter ``rhs``:
    the polynomial to subtract.

Returns:
    the first polynomial minus the second.)doc";

// Docstring regina::python::doc::Laurent2::global_swap
static constexpr const char global_swap[] =
R"doc(Swaps the contents of the given polynomials.

This global routine simply calls Laurent2<T>::swap(); it is provided
so that Laurent2<T> meets the C++ Swappable requirements.

Parameter ``a``:
    the first polynomial whose contents should be swapped.

Parameter ``b``:
    the second polynomial whose contents should be swapped.)doc";

// Docstring regina::python::doc::Laurent2::init
static constexpr const char init[] = R"doc(Sets this to become the zero polynomial.)doc";

// Docstring regina::python::doc::Laurent2::initExp
static constexpr const char initExp[] =
R"doc(Sets this to become the polynomial ``x^d y^e`` for the given exponents
*d* and *e*.

Parameter ``xExp``:
    the new exponent *d*, which is attached to *x*.

Parameter ``yExp``:
    the new exponent *e*, which is attached to *y*.)doc";

// Docstring regina::python::doc::Laurent2::invertX
static constexpr const char invertX[] =
R"doc(Replaces ``x`` with ``x^-1`` in this polynomial. This polynomial is
changed directly.)doc";

// Docstring regina::python::doc::Laurent2::invertY
static constexpr const char invertY[] =
R"doc(Replaces ``y`` with ``y^-1`` in this polynomial. This polynomial is
changed directly.)doc";

// Docstring regina::python::doc::Laurent2::isZero
static constexpr const char isZero[] =
R"doc(Returns whether this is the zero polynomial.

Returns:
    ``True`` if and only if this is the zero polynomial.)doc";

// Docstring regina::python::doc::Laurent2::negate
static constexpr const char negate[] = R"doc(Negates this polynomial. This polynomial is changed directly.)doc";

// Docstring regina::python::doc::Laurent2::set
static constexpr const char set[] =
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

// Docstring regina::python::doc::Laurent2::shift
static constexpr const char shift[] =
R"doc(Multiplies this polynomial by ``x^s y^t`` for some integers *s* and
*t*. This polynomial will be changed directly.

Parameter ``s``:
    the power of *x* to multiply by.

Parameter ``t``:
    the power of *y* to multiply by.)doc";

// Docstring regina::python::doc::Laurent2::shifted
static constexpr const char shifted[] =
R"doc(Returns the product of this polynomial with ``x^s y^t`` for some
integers *s* and *t*. This polynomial will not be changed.

Parameter ``s``:
    the power of *x* to multiply by.

Parameter ``t``:
    the power of *y* to multiply by.

Returns:
    the product of this with ``x^s y^t``.)doc";

// Docstring regina::python::doc::Laurent2::str
static constexpr const char str[] =
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

// Docstring regina::python::doc::Laurent2::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given polynomial. This is a fast
(constant time) operation.

This and the given polynomial need not have the same range of non-zero
coefficients.

Parameter ``other``:
    the polynomial whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Laurent2::utf8
static constexpr const char utf8[] =
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

}; // struct Laurent2

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

