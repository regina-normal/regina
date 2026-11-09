/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


struct Laurent {

// Docstring regina::python::doc::Laurent::__add
static constexpr const char __add[] =
R"doc(Adds the two given polynomials.

Parameter ``lhs``:
    the first polynomial to add.

Parameter ``rhs``:
    the second polynomial to add.

Returns:
    the sum of both polynomials.)doc";

// Docstring regina::python::doc::Laurent::__array
static constexpr const char __array[] =
R"doc(Returns the given coefficient of a single-variable Laurent polynomial.

Specifically, for a single-variable polynomial *p*, calling ``p[a]``
will return the coefficient of ``x^a``, where *x* is the polynomial
variable. There are no restrictions on the exponent *a*.

For multivariate polynomials you should use the round bracket (call)
operator instead, since that operator supports multiple arguments
(i.e., a sequence of exponents).

Python:
    Python users should use this square bracket (subscript) operator
    even for multivariate polynomials. Moreover, Python users can also
    use this operator to _set_ cofficients; that is, you can write
    ``poly[exp] = value``. However, when _getting_ a coefficient this
    operator will return by value (to enforce constness), which means
    for example you cannot write something like
    ``poly[exp].negate()``.

Parameter ``exp``:
    the exponent of the term whose coefficient should be returned
    (i.e., the integer *a* in the explanation above).

Returns:
    the requested coefficient.)doc";

// Docstring regina::python::doc::Laurent::__call
static constexpr const char __call[] =
R"doc(Returns the given coefficient of a multivariate Laurent polynomial.

Specifically, for a multivariate polynomial *p*, calling ``p(a,b,…)``
will return the coefficient of ``x^a y^b …``, where ``x,y,…`` are the
polynomial variables. There are no restrictions on the exponents
``a,b,…``.

Python:
    In Python, this is the square bracket (subscript) operator, not
    the round bracket (call) operator; that is, Python users can
    access coefficients through the syntax ``poly[a,b,…]``. Moreover,
    this operator can also _set_ cofficients; that is, you can write
    ``poly[a,b,…] = value``. However, when _getting_ a coefficient
    this operator will return by value (to enforce constness), which
    means for example you cannot write something like
    ``poly[a,b,…].negate()``.

Parameter ``firstExp``:
    the exponent of the first polynomial variable *x* (i.e., the
    integer *a* in the explanation above).

Parameter ``otherExp``:
    the exponents of the remaining ``(vars - 1)`` polynomial variables
    ``y,…`` (i.e., the integers ``b,…`` in the explanation above).

Returns:
    the requested coefficient.)doc";

// Docstring regina::python::doc::Laurent::__class
static constexpr const char __class[] =
R"doc(Represents a Laurent polynomial with coefficients of type *T*. A
Laurent polynomial differs from an ordinary polynomial in that it
allows negative exponents (so, unlike the Polynomial class, you can
represent both ``2+3x`` and ``1+1/x``).

As of Regina 8.0, this class can be configured to represent either
single-variable or multivariate Laurent polynomials, via the optional
template parameter *vars*. In particular, ``Laurent<T, 2>`` replaces
the old type ``Laurent2<T>`` from Regina 7.x, which represents Laurent
polynomials in two variables. In the documentation for this class, we
frequently refer to the individual polynomial variables as ``x,y,…``.

This class uses _dense_ storage: coefficients are stored in contiguous
ranges, including zero coefficients. Like ``std::vector``, Laurent
polynomials allocate additional memory to accommodate future growth,
which means that (for example) appending a term to a single-variable
polynomial with exponent ``maxExp() + 1`` or ``minExp() - 1`` takes
amortised constant time.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value. If a
Laurent object is moved from, it can later be reused by assigning it a
new value or by calling one of the initialisation functions init() or
initExp().

Precondition:
    The coefficient type *T* has the property that, if an object is
    moved from, it can later be reused by assigning it a new value.
    Examples of types with this property include C++ standard library
    types (see C++ 20 standard, section 16.5.5.16), and Regina's
    Integer, LargeInteger and Rational classes.

Python:
    In Python, the classes Laurent, Laurent2, and Laurent3 refer to
    the specific template classes ``Laurent<Integer>``,
    ``Laurent<Integer, 2>``, and ``Laurent<Integer, 3>`` respectively.

Template parameter ``T``:
    the coefficient type. A typical coefficient type would be Integer
    or Rational. Note that native C++ integer types are _not_
    supported (since they have no zero-initialising default
    constructor).

Template parameter ``vars``:
    the number of variables in this type of Laurent polynomial. This
    is optional, and defaults to 1. Currently it is limited to a
    maximum of 3 (this is because a handful of member functions are
    implemented separately for each value of *vars*).)doc";

// Docstring regina::python::doc::Laurent::__cmp
static constexpr const char __cmp[] =
R"doc(Compares this against the given polynomial under a total ordering of
all Laurent polynomials.

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

// Docstring regina::python::doc::Laurent::__copy
static constexpr const char __copy[] =
R"doc(Creates a new copy of the given polynomial.

This constructor induces a deep copy of *value*.

A note for developers: even though this routine is identical to the
templated copy constructor, it must be declared and implemented
separately. Otherwise the compiler might create its own (incorrect)
copy constructor automatically.

Parameter ``value``:
    the polynomial to clone.)doc";

// Docstring regina::python::doc::Laurent::__default
static constexpr const char __default[] = R"doc(Creates the zero polynomial.)doc";

// Docstring regina::python::doc::Laurent::__div
static constexpr const char __div[] =
R"doc(Divides the given polynomial by the given scalar constant.

This uses the division operator ``/=`` for the coefficient type *T*.

The scalar is simply of type *T*; we use the identical type Laurent<T,
vars>::Coefficient here to assist with C++ template type matching.

Precondition:
    The argument *scalar* is non-zero.

Parameter ``poly``:
    the polynomial to divide by the given scalar.

Parameter ``scalar``:
    the scalar factor to divide by.

Returns:
    the quotient of the given polynomial by the given scalar.)doc";

// Docstring regina::python::doc::Laurent::__div_2
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

// Docstring regina::python::doc::Laurent::__eq
static constexpr const char __eq[] =
R"doc(Tests whether this and the given polynomial are equal.

Parameter ``rhs``:
    the polynomial to compare with this.

Returns:
    ``True`` if and only if this and the given polynomial are equal.)doc";

// Docstring regina::python::doc::Laurent::__eq_2
static constexpr const char __eq_2[] =
R"doc(Tests whether this polynomial is equal to the given constant.

Parameter ``constant``:
    the value to compare this polynomial against.

Returns:
    ``True`` if and only if this polynomial is equal to the given
    constant.)doc";

// Docstring regina::python::doc::Laurent::__eq_3
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

// Docstring regina::python::doc::Laurent::__iadd
static constexpr const char __iadd[] =
R"doc(Adds the given polynomial to this.

Parameter ``other``:
    the polynomial to add to this.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent::__idiv
static constexpr const char __idiv[] =
R"doc(Divides this polynomial by the given constant.

This uses the division operator ``/=`` for the coefficient type *T*.

Precondition:
    The argument *scalar* is non-zero.

Parameter ``scalar``:
    the scalar factor to divide by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent::__idiv_2
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

// Docstring regina::python::doc::Laurent::__imul
static constexpr const char __imul[] =
R"doc(Multiplies this polynomial by the given constant.

Parameter ``scalar``:
    the scalar factor to multiply by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent::__imul_2
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

// Docstring regina::python::doc::Laurent::__imul_3
static constexpr const char __imul_3[] =
R"doc(Multiplies this by the given polynomial.

Parameter ``other``:
    the polynomial to multiply this by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent::__init
static constexpr const char __init[] =
R"doc(Creates a new polynomial from a contiguous sequence of coefficients.
This is only relevant for single-variable Laurent polynomials.

The coefficients should be given in order from the smallest exponent
term to the largest. The first coefficient in the sequence will be
associated with the exponent *minExp*.

There is no problem if the first and/or last coefficient in the
sequence is zero. An empty sequence will be treated as the zero
polynomial.

This constructor induces a deep copy of the given range.

The iterator type must be random access because this allows the
implementation to compute the sequence length in constant time.

Python:
    Instead of the iterators *begin* and *end*, this routine takes a
    Python list of coefficients.

Parameter ``minExp``:
    the exponent corresponding to the first coefficient in the
    sequence.

Parameter ``begin``:
    the beginning of the sequence of coefficients.

Parameter ``end``:
    a past-the-end iterator indicating the end of the sequence of
    coefficients.)doc";

// Docstring regina::python::doc::Laurent::__init_2
static constexpr const char __init_2[] =
R"doc(Creates a new polynomial with a single constant term, equal to the
given value.

There is no problem if the given value is zero.

Parameter ``constant``:
    the value of this new constant polynomial.)doc";

// Docstring regina::python::doc::Laurent::__init_3
static constexpr const char __init_3[] =
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

// Docstring regina::python::doc::Laurent::__isub
static constexpr const char __isub[] =
R"doc(Subtracts the given polynomial from this.

Parameter ``other``:
    the polynomial to subtract from this.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Laurent::__mul
static constexpr const char __mul[] =
R"doc(Multiplies the given polynomial by the given scalar constant.

The scalar is simply of type *T*; we use the identical type Laurent<T,
vars>::Coefficient here to assist with C++ template type matching.

Parameter ``poly``:
    the polynomial to multiply by.

Parameter ``scalar``:
    the scalar to multiply by.

Returns:
    the product of the given polynomial and scalar.)doc";

// Docstring regina::python::doc::Laurent::__mul_2
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

// Docstring regina::python::doc::Laurent::__mul_3
static constexpr const char __mul_3[] =
R"doc(Multiplies the given polynomial by the given scalar constant.

The scalar is simply of type *T*; we use the identical type Laurent<T,
vars>::Coefficient here to assist with C++ template type matching.

Parameter ``scalar``:
    the scalar to multiply by.

Parameter ``poly``:
    the polynomial to multiply by.

Returns:
    the product of the given polynomial and scalar.)doc";

// Docstring regina::python::doc::Laurent::__mul_4
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

// Docstring regina::python::doc::Laurent::__mul_5
static constexpr const char __mul_5[] =
R"doc(Multiplies the two given polynomials.

Parameter ``lhs``:
    the first polynomial to multiply.

Parameter ``rhs``:
    the second polynomial to multiply.

Returns:
    the product of both polynomials.)doc";

// Docstring regina::python::doc::Laurent::__sub
static constexpr const char __sub[] =
R"doc(Returns the negative of the given polynomial.

Parameter ``arg``:
    the polynomial to negate.

Returns:
    the negative of *arg*.)doc";

// Docstring regina::python::doc::Laurent::__sub_2
static constexpr const char __sub_2[] =
R"doc(Subtracts the two given polynomials.

Parameter ``lhs``:
    the polynomial to sutract *rhs* from.

Parameter ``rhs``:
    the polynomial to subtract from *lhs*.

Returns:
    the difference of the two given polynomials.)doc";

// Docstring regina::python::doc::Laurent::addProduct
static constexpr const char addProduct[] =
R"doc(Adds the product of the two given polynomials to this. This is a
common operation in (for example) inner products and matrix
multiplication.

Calling ``z.addProduct(x, y)`` is equivalent to, but often faster
than, calling ``z += x * y``.

Parameter ``x``:
    the first polynomial in the product to add to this.

Parameter ``y``:
    the second polynomial in the product to add to this.)doc";

// Docstring regina::python::doc::Laurent::addProduct_2
static constexpr const char addProduct_2[] =
R"doc(Adds the product of the given polynomial with the given scalar to
this.

Calling ``z.addProduct(x, scalar)`` is equivalent to, but often faster
than, calling ``z += x * scalar``.

Parameter ``x``:
    the polynomial in the product to add to this.

Parameter ``scalar``:
    the scalar in the product to add to this.)doc";

// Docstring regina::python::doc::Laurent::allocation
static constexpr const char allocation[] =
R"doc(Indicates the range of terms for which memory is currently allocated.

This is mainly provided for diagnostics and performance analysis; end
users will typically not need to use this routine.

This routine returns a pair ``(base, capacity)``, where *base*
indicates the smallest exponent of the first polynomial variable for
which memory is allocated, and *capacity* indicates the total number
of exponents of the first polynomial variable for which memory is
allocated. This means that the _largest_ exponent of the first
variable for which memory is allocated will be ``base + capacity -
1``.

In the special case of the zero polynomial, it is possible (but not
necessarily true) that no memory is allocated at all. If no memory is
allocated, then this routine will return ``(0, 0)``. If a zero
polynomial _does_ have memory allocated, then this memory will not be
holding any meaningful terms, and so this routine will return a
positive capacity but an arbitrary base.

Returns:
    a pair ``(base, capacity)`` as described above, or ``(0, 0)`` if
    this is the zero polynomial _and_ no memory is allocated at all.)doc";

// Docstring regina::python::doc::Laurent::first
static constexpr const char first[] =
R"doc(Returns an iterator pointing to the first non-zero coefficient of this
polynomial. This can be used as a starting point for forward iteration
(via operator ``++``) through all non-zero coefficients.

Such iterators provide read-only access to the coefficients: to modify
coefficients you will need to call different routines, such as set().

If this is the zero polynomial, then the iterator that is returned
will be non-deferenceable.

See the LaurentCoefficient class notes for further details and
examples on how iteration works for Laurent polynomials.

Returns:
    an iterator pointing to the first non-zero coefficient.)doc";

// Docstring regina::python::doc::Laurent::global_swap
static constexpr const char global_swap[] =
R"doc(Swaps the contents of the given polynomials.

This global routine simply calls Laurent<T, vars>::swap(); it is
provided so that Laurent<T, vars> meets the C++ Swappable
requirements.

Parameter ``a``:
    the first polynomial whose contents should be swapped.

Parameter ``b``:
    the second polynomial whose contents should be swapped.)doc";

// Docstring regina::python::doc::Laurent::init
static constexpr const char init[] = R"doc(Sets this to become the zero polynomial.)doc";

// Docstring regina::python::doc::Laurent::initExp
static constexpr const char initExp[] =
R"doc(Sets this to become the monomial ``x^a y^b …`` for the given sequence
of exponents ``a,b,…``. Here ``x,y,…`` represent the polynomial
variables.

The arguments are separated into *firstExp* vs *otherExp* for
implementation reasons only - to create the monomial ``x^a y^b …`` you
would simply call ``initExp(a, b, …)``.

Parameter ``firstExp``:
    the exponent of the first polynomial variable *x*.

Parameter ``otherExp``:
    the exponents of the remaining ``(vars - 1)`` polynomial variables
    ``y,…``.)doc";

// Docstring regina::python::doc::Laurent::invertX
static constexpr const char invertX[] =
R"doc(Replaces the first polynomial variable ``x`` with ``x^-1``, leaving
all other polynomial variables unchanged. This polynomial is changed
directly.

For single-variable Laurent polynomials, this is equivalent to calling
``scaleUp(-1)``.)doc";

// Docstring regina::python::doc::Laurent::isZero
static constexpr const char isZero[] =
R"doc(Returns whether this is the zero polynomial.

Returns:
    ``True`` if and only if this is the zero polynomial.)doc";

// Docstring regina::python::doc::Laurent::last
static constexpr const char last[] =
R"doc(Returns an iterator pointing to the last non-zero coefficient of this
polynomial. This can be used as a starting point for backward
iteration (via operator ``--``) through all non-zero coefficients.

Such iterators provide read-only access to the coefficients: to modify
coefficients you will need to call different routines, such as set().

If this is the zero polynomial, then the iterator that is returned
will be non-deferenceable.

See the LaurentCoefficient class notes for further details and
examples on how iteration works for Laurent polynomials.

Returns:
    an iterator pointing to the last non-zero coefficient.)doc";

// Docstring regina::python::doc::Laurent::maxExp
static constexpr const char maxExp[] =
R"doc(Returns the largest exponent that appears in this polynomial with a
non-zero coefficient. This is only relevant for single-variable
Laurent polynomials.

If this is the zero polynomial, then this routine returns 0.

Returns:
    the largest exponent.)doc";

// Docstring regina::python::doc::Laurent::minExp
static constexpr const char minExp[] =
R"doc(Returns the smallest exponent that appears in this polynomial with a
non-zero coefficient. This is only relevant for single-variable
Laurent polynomials.

If this is the zero polynomial, then this routine returns 0.

Returns:
    the smallest exponent.)doc";

// Docstring regina::python::doc::Laurent::monomial
static constexpr const char monomial[] =
R"doc(Returns the monomial ``x^a y^b …`` for the given sequence of exponents
``a,b,…``. Here ``x,y,…`` represent the polynomial variables.

The arguments are separated into *firstExp* vs *otherExp* for
implementation reasons only - to create the monomial ``x^a y^b …`` you
would simply call ``monomial(a, b, …)``.

Parameter ``firstExp``:
    the exponent of the first polynomial variable *x*.

Parameter ``otherExp``:
    the exponents of the remaining ``(vars - 1)`` polynomial variables
    ``y,…``.

Returns:
    the requested monomial.)doc";

// Docstring regina::python::doc::Laurent::negate
static constexpr const char negate[] = R"doc(Negates this polynomial. This polynomial is changed directly.)doc";

// Docstring regina::python::doc::Laurent::polynomial
static constexpr const char polynomial[] =
R"doc(Converts a single-variable Laurent polynomial into one of Regina's
Polynomial objects.

This routine returns a pair ``(p, exp)``, where *p* is of type
``Polynomial<T>``, and where this Laurent polynomial is equivalent to
``x^exp p(x)``.

If this polynomial is non-zero, then *exp* will be precisely minExp(),
which means that the resulting polynomial *p* will have a non-zero
constant term. If this polynomial is zero, then both *p* and *exp*
will be zero also.

Returns:
    the pair ``(p, exp)``, as described above.)doc";

// Docstring regina::python::doc::Laurent::reserveRange
static constexpr const char reserveRange[] =
R"doc(Reserves enough memory in the array of terms to span the given range
of exponents in the first polynomial variable. This routine merely
affects the internal storage; it does not change the value of this
polynomial.

This routine is intended for use with single-variable Laurent
polynomials, but it is made available for multivariate polynomials
also.

In particular, for single-variable polynomials, after calling
``reserveRange(fromExp, toExp)`` you can add non-zero coefficients for
any ``x^i`` where ``fromExp ≤ i ≤ toExp`` without needing to
reallocate memory and/or shuffle data around.

Note that the range ``[fromExp, toExp]`` is inclusive at both ends
(unlike iterator ranges, for example).

This routine is safe to call even if ``fromExp > toExp`` (in which
case it will do nothing at all).

Parameter ``fromExp``:
    the smallest exponent for the first polynomial variable whose
    coefficient will have storage reserved.

Parameter ``toExp``:
    the largest exponent for the first polynomial variable whose
    coefficient will have storage reserved.)doc";

// Docstring regina::python::doc::Laurent::scaleDown
static constexpr const char scaleDown[] =
R"doc(Divides all exponents in this polynomial by *k* for some integer *k*.
This is equivalent to replacing the variable *x* of the polynomial
with ``x^(1/k)``. This function is only available for single-variable
Laurent polynomials.

Both positive and negative scaling factors *k* are allowed.

Precondition:
    *k* is non-zero.

Precondition:
    All exponents in this polynomial with non-zero coefficients are
    multiples of *k*.

Exception ``InvalidArgument``:
    The given scaling factor *k* is zero.

Exception ``FailedPrecondition``:
    The scaling factor *k* is non-zero, but some exponent with a non-
    zero coefficient is not a multiple of *k*. Be aware that this
    polynomial might be changed before this exception is thrown.

Parameter ``k``:
    the scaling factor to divide exponents by.)doc";

// Docstring regina::python::doc::Laurent::scaleUp
static constexpr const char scaleUp[] =
R"doc(Multiplies all exponents in this polynomial by *k* for some integer
*k*. This is equivalent to replacing the variable *x* of the
polynomial with ``x^k``. This function is only available for single-
variable Laurent polynomials.

Both positive and negative scaling factors *k* are allowed.

Precondition:
    *k* is non-zero.

Exception ``InvalidArgument``:
    The given scaling factor *k* is zero.

Parameter ``k``:
    the scaling factor to multiply exponents by.)doc";

// Docstring regina::python::doc::Laurent::set
static constexpr const char set[] =
R"doc(Changes the given coefficient of a single-variable Laurent polynomial.

Calling ``poly.set(exp, value)`` is identical to calling the generic
function ``poly.setTo(value, exp)``; the only difference is that the
new value is passed as the last argument instead of the first. See the
setTo() documentation for further details.

The reason Laurent offers both set() and setTo() is that the argument
order for setTo() greatly simplifies the C++ template implementation,
whereas set() is consistent with other classes such as Polynomial and
Bitmask. If you are not sure which routine to use, just use set().

Python:
    This set() routine is available, but you can also set coefficients
    directly using syntax of the form ``p[exp] = value``.

Parameter ``exp``:
    the exponent of the polynomial variable for the coefficient that
    we should set.

Parameter ``value``:
    the new value to assign to the given coefficient.)doc";

// Docstring regina::python::doc::Laurent::set_2
static constexpr const char set_2[] =
R"doc(Changes the given coefficient of a two-variable Laurent polynomial.

Calling ``poly.set(exp0, exp1, value)`` is identical to calling the
generic function ``poly.setTo(value, exp0, exp1)``; the only
difference is that the new value is passed as the last argument
instead of the first. See the setTo() documentation for further
details.

The reason Laurent offers both set() and setTo() is that the argument
order for setTo() greatly simplifies the C++ template implementation,
whereas set() is consistent with other classes such as Polynomial and
Bitmask. If you are not sure which routine to use, just use set().

Python:
    This set() routine is available, but you can also set coefficients
    directly using syntax of the form ``p[exp0, exp1] = value``.

Parameter ``exp0``:
    the exponent of the first polynomial variable for the coefficient
    that we should set.

Parameter ``exp1``:
    the exponent of the second polynomial variable for the coefficient
    that we should set.

Parameter ``value``:
    the new value to assign to the given coefficient.)doc";

// Docstring regina::python::doc::Laurent::set_3
static constexpr const char set_3[] =
R"doc(Changes the given coefficient of a three-variable Laurent polynomial.

Calling ``poly.set(exp0, exp1, exp2, value)`` is identical to calling
the generic function ``poly.setTo(value, exp0, exp1, exp2)``; the only
difference is that the new value is passed as the last argument
instead of the first. See the setTo() documentation for further
details.

The reason Laurent offers both set() and setTo() is that the argument
order for setTo() greatly simplifies the C++ template implementation,
whereas set() is consistent with other classes such as Polynomial and
Bitmask. If you are not sure which routine to use, just use set().

Python:
    This set() routine is available, but you can also set coefficients
    directly using syntax of the form ``p[exp0, exp1, exp2] = value``.

Parameter ``exp0``:
    the exponent of the first polynomial variable for the coefficient
    that we should set.

Parameter ``exp1``:
    the exponent of the second polynomial variable for the coefficient
    that we should set.

Parameter ``exp2``:
    the exponent of the third polynomial variable for the coefficient
    that we should set.

Parameter ``value``:
    the new value to assign to the given coefficient.)doc";

// Docstring regina::python::doc::Laurent::setTo
static constexpr const char setTo[] =
R"doc(Changes the given coefficient of this polynomial.

Specifically, for a polynomial *p*, calling ``p.setTo(value,a,b,…)``
will set the coefficient of ``x^a y^b …`` to *value*, where ``x,y,…``
are the polynomial variables. There are no restrictions on the
exponents ``a,b,…``, and the new coefficient *value* may be zero.

Note that it may be expensive to set a non-zero coefficient outside
the current span of exponents, since this may require allocating more
space and/or moving coefficients around in memory. The Laurent class
does, however, use an exponential growth strategy that attempts to
minimise the cost of this.

The only difference between setTo() and set() is the order of
arguments: in setTo() the value comes first, since this greatly
simplifies the recursive C++ template implementation. If you are not
sure which routine to use, just use set(): if Regina ever moves to a
C++ standard with better handling of template parameter packs then
setTo() may eventually be deprecated and later removed.

Python:
    This setTo() routine is available, but you can also set
    coefficients directly using syntax of the form ``p[a,b,…] =
    value``.

Parameter ``value``:
    the new value to assign to the given coefficient.

Parameter ``firstExp``:
    the exponent of the first polynomial variable *x* for the
    coefficient that we should set.

Parameter ``otherExp``:
    the exponents of the remaining ``(vars - 1)`` polynomial variables
    ``y,…`` for the coefficient that we should set.)doc";

// Docstring regina::python::doc::Laurent::shift
static constexpr const char shift[] =
R"doc(Multiplies this polynomial by ``x^s y^t …`` for the given sequence of
exponent shifts ``s,t,…``. Here ``x,y,…`` represent the polynomial
variables. This polynomial will be changed directly.

The arguments are separated into *firstExp* vs *otherExp* for
implementation reasons only - to multiply by ``x^s y^t …`` you would
simply call ``shift(s, t, …)``.

This operation is very fast (for single-variable polynomials it runs
in small constant time).

Parameter ``firstShift``:
    the power of first polynomial variable *x* to multiply by.

Parameter ``otherShifts``:
    the powers of the remaining ``(vars - 1)`` polynomial variables
    ``y,…`` to multiply by.)doc";

// Docstring regina::python::doc::Laurent::shifted
static constexpr const char shifted[] =
R"doc(Returns the product of this polynomial with ``x^s y^t …`` for the
given sequence of exponent shifts ``s,t,…``. Here ``x,y,…`` represent
the polynomial variables. This polynomial is not changed.

Be aware that the rvalue variant of shifted() is _much_ faster than
this const version (this const variant has the extra cost of a deep
copy, whereas for single-variable polynomials the rvalue variant runs
in small constant time).

Parameter ``shifts``:
    the powers of the polynomial variables ``x,y,…`` to multiply by;
    that is, the sequence of integers ``s,t,…`` as described above.

Returns:
    the product of this with ``x^s y^t …``.)doc";

// Docstring regina::python::doc::Laurent::str
static constexpr const char str[] =
R"doc(Returns this polynomial as a human-readable string, using the given
variable names instead of ``x,y,…``.

.. note::
    There is also the usual variant of str() which takes no arguments;
    that variant is inherited from the Output class.

Parameter ``variables``:
    a sequence of *vars* C-style strings to use for the polynomial
    variables. Any (or all) of these may be ``None``, in which case
    the defaults ``x,y,…`` will be used for those variable(s).

Returns:
    this polynomial as a human-readable string.)doc";

// Docstring regina::python::doc::Laurent::swap
static constexpr const char swap[] =
R"doc(Swaps the contents of this and the given polynomial. This is a fast
(constant time) operation.

This and the given polynomial do not need to use the same ranges of
exponents.

Parameter ``other``:
    the polynomial whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Laurent::utf8
static constexpr const char utf8[] =
R"doc(Returns this polynomial as a human-readable string using unicode
characters, using the given variable names instead of ``x,y,…``.

This is similar to the output from str(), except that it uses unicode
characters to make the output more pleasant to read. In particular, it
makes use of superscript digits for exponents and a wider minus sign.

The string is encoded in UTF-8.

.. note::
    There is also the usual variant of utf8() which takes no
    arguments; that variant is inherited from the Output class.

Parameter ``variables``:
    a sequence of *vars* C-style strings to use for the polynomial
    variables. Any (or all) of these may be ``None``, in which case
    the defaults ``x,y,…`` will be used for those variable(s).

Returns:
    this polynomial as a unicode-enabled human-readable string.)doc";

// Docstring regina::python::doc::Laurent::validate
static constexpr const char validate[] =
R"doc(A diagnostic routine that ensures that the internal representation of
this polynomial is valid.

If Regina is working correctly, this routine should do nothing. If the
internal representation of this polynomial is _not_ valid, this
routine will throw an exception.

This routine is provided for use within Regina's various test suites.
End users should not need to call it at all.

Exception ``ImpossibleScenario``:
    The internal state of this polynomial is invalid.)doc";

}; // struct Laurent

struct LaurentCoefficient {

// Docstring regina::python::doc::LaurentCoefficient::__as_bool
static constexpr const char __as_bool[] =
R"doc(Tests whether this iterator is dereferenceable.

Returns:
    ``True`` if and only if this iterator is dereferenceable.)doc";

// Docstring regina::python::doc::LaurentCoefficient::__class
static constexpr const char __class[] =
R"doc(A bidirectional iterator that steps through the non-zero coefficients
of a Laurent polynomial.

This class works with ``Laurent<T, vars>``. It behaves like a
bidirectional iterator over the non-zero coefficients of a (possibly
multivariate) polynomial, but also offers extra member functions for
identifying which monomial the current cofficient is attached to
(i.e., for identifying the current exponents of the polynomial
variables).

For a Laurent polynomial *p*, you begin iteration by calling either
``p.first()`` or ``p.last()`` (for forward or backward iteration
respectively), which returns a LaurentCoefficient *c*. You can then
dereference *c* to extract the coefficient, call ``c.exp()`` to
identify the current exponents of the polynomial variables, and/or
move the iterator forwards or backwards using the usual operators
``++c`` and ``--c``. To test whether iteration has finished (i.e.,
whether *c* is no longer dereferenceable), you can test whether *c*
evaluates to ``False`` in a boolean context, or you can compare *c* to
a default-constructed ``LaurentCoefficient()`` (which behaves like an
end iterator).

As an example, you could print all of the non-zero terms of a single-
variable Laurent polynomial using the following loop:

```
for (auto c = p.first(); c; ++c) {
    std::cout << "Term: " << *c << " x^" << c.exp() << std::endl;
}
```

LaurentCoefficient objects are small enough to pass by value and swap
with std::swap(), with no need for any specialised move operations or
swap functions.

Python:
    In Python, the classes LaurentCoefficient, LaurentCoefficient2,
    and LaurentCoefficient3 refer to the specific template classes
    ``LaurentCoefficient<Integer, 1>``, ``LaurentCoefficient<Integer,
    2>``, and ``LaurentCoefficient<Integer, 3>`` respectively.

Template parameter ``T``:
    the coefficient type of the corresponding Laurent polynomial.

Template parameter ``vars``:
    the number of variables in the corresponding Laurent polynomial.)doc";

// Docstring regina::python::doc::LaurentCoefficient::__copy
static constexpr const char __copy[] = R"doc(Creates a copy of the given iterator.)doc";

// Docstring regina::python::doc::LaurentCoefficient::__dec
static constexpr const char __dec[] =
R"doc(Predecrement operator. This iterator will step backward to point to
the previous non-zero coefficient of the underlying Laurent
polynomial, or will become non-dereferenceable if there are no prior
non-zero coefficients.

Precondition:
    This iterator is dereferenceable.

Returns:
    a reference to this iterator.)doc";

// Docstring regina::python::doc::LaurentCoefficient::__dec_2
static constexpr const char __dec_2[] =
R"doc(Postdecrement operator. This iterator will step backward to point to
the previous non-zero coefficient of the underlying Laurent
polynomial, or will become non-dereferenceable if there are no prior
non-zero coefficients.

Precondition:
    This iterator is dereferenceable.

Returns:
    a copy of this iterator before it was decremented.)doc";

// Docstring regina::python::doc::LaurentCoefficient::__default
static constexpr const char __default[] =
R"doc(Creates an iterator that is not dereferenceable.

This will compare as equal to any other non-dereferenceable iterator,
and so it can be used as an end iterator in a begin/end pair.)doc";

// Docstring regina::python::doc::LaurentCoefficient::__eq
static constexpr const char __eq[] =
R"doc(Tests whether this and the given iterator point to the same non-zero
coefficient of the same Laurent polynomial.

Iterators that point to coefficients attached to different monomials
within the same polynomial will compare as different, even if both
coefficients take the same numerical value of type *T*.

A non-dereferenceable iterator will compare as equal to any other non-
dereferenceable iterator (even if they came from different Laurent
polynomials, or if they were default constructed and therefore have no
associated polynomial at all).

Parameter ``rhs``:
    the iterator to compare to this.

Returns:
    ``True`` if and only if the iterators are equal as described
    above.)doc";

// Docstring regina::python::doc::LaurentCoefficient::__inc
static constexpr const char __inc[] =
R"doc(Preincrement operator. This iterator will step forward to point to the
next non-zero coefficient of the underlying Laurent polynomial, or
will become non-dereferenceable if there are no more non-zero
coefficients.

Precondition:
    This iterator is dereferenceable.

Returns:
    a reference to this iterator.)doc";

// Docstring regina::python::doc::LaurentCoefficient::__inc_2
static constexpr const char __inc_2[] =
R"doc(Postincrement operator. This iterator will step forward to point to
the next non-zero coefficient of the underlying Laurent polynomial, or
will become non-dereferenceable if there are no more non-zero
coefficients.

Precondition:
    This iterator is dereferenceable.

Returns:
    a copy of this iterator before it was incremented.)doc";

// Docstring regina::python::doc::LaurentCoefficient::__mul
static constexpr const char __mul[] =
R"doc(Returns a read-only reference to the current non-zero coefficient of
the underlying Laurent polynomial.

Precondition:
    This iterator is dereferenceable.

Returns:
    the current non-zero coefficient.)doc";

// Docstring regina::python::doc::LaurentCoefficient::__sub_
static constexpr const char __sub_[] =
R"doc(Gives read-only member access to the current non-zero coefficient of
the underlying Laurent polynomial.

Precondition:
    This iterator is dereferenceable.

Returns:
    member access to the current non-zero coefficient.)doc";

// Docstring regina::python::doc::LaurentCoefficient::constantTerm
static constexpr const char constantTerm[] =
R"doc(Identifies whether the current non-zero coefficient is in fact the
constant term of the underlying Laurent polynomial.

This is true if and only if ``exp<i>() == 0`` for all *i*.

Precondition:
    This iterator is dereferenceable.

Returns:
    ``True`` if and only if this iterator is currently pointing to the
    constant coefficient.)doc";

// Docstring regina::python::doc::LaurentCoefficient::exp
static constexpr const char exp[] =
R"doc(Identifies which exponent of the given polynomial variable corresponds
to the current non-zero coefficient. This allows you to determine
which monomial the current coefficient is attached to.

Precondition:
    This iterator is dereferenceable.

Template parameter ``var``:
    indicates which variable of the polynomial you wish to query.

Returns:
    the current exponent of the given polynomial variable.)doc";

}; // struct LaurentCoefficient

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

