/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Arrow
static const char *Arrow =
R"doc(Represents a multivariate polynomial of the type used by arrow
polynomials of links.

An arrow polynomial is a polynomial in the "ordinary variable" ``A``
and a finite number of "diagram variables" ``K_1,K_2,...``. The
ordinary variable may appear with any integer exponents, including
negative exponents (as in a Laurent polynomial). The diagram variables
may only appear with non-negative integer exponents (as in an ordinary
polynomial). All of the variables commute, and all of the coefficients
are integers.

This class is implemented as a collection of Laurent polynomials in
``A``, each attached to a different product of diagram variables
``(K_1)^(a_1) (K_2)^(a_2) ...``. We represent each such product of
diagram variables by the sequence of non-negative integers
``a_1,a_2,...``, where the final integer is strictly positive; for the
trivial product with no diagram variables at all we use the empty
sequence. In the notes below we call such a sequence a _diagram
sequence_.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.)doc";

// Docstring regina::python::doc::__add
static const char *__add =
R"doc(Adds the two given polynomials.

Parameter ``lhs``:
    the first polynomial to add.

Parameter ``rhs``:
    the second polynomial to add.

Returns:
    the sum of both polynomials.)doc";

// Docstring regina::python::doc::__add_2
static const char *__add_2 =
R"doc(Adds the two given polynomials.

Parameter ``lhs``:
    the first polynomial to add.

Parameter ``rhs``:
    the second polynomial to add.

Returns:
    the sum of both polynomials.)doc";

// Docstring regina::python::doc::__add_3
static const char *__add_3 =
R"doc(Adds the two given polynomials.

Parameter ``lhs``:
    the first polynomial to add.

Parameter ``rhs``:
    the second polynomial to add.

Returns:
    the sum of both polynomials.)doc";

// Docstring regina::python::doc::__add_4
static const char *__add_4 =
R"doc(Adds the two given polynomials.

Parameter ``lhs``:
    the first polynomial to add.

Parameter ``rhs``:
    the second polynomial to add.

Returns:
    the sum of both polynomials.)doc";

// Docstring regina::python::doc::__mul
static const char *__mul =
R"doc(Multiplies the given polynomial by the given integer constant.

Parameter ``poly``:
    the polynomial to multiply by.

Parameter ``scalar``:
    the scalar factor to multiply by.

Returns:
    the product of the given polynomial and scalar.)doc";

// Docstring regina::python::doc::__mul_2
static const char *__mul_2 =
R"doc(Multiplies the given polynomial by the given integer constant.

Parameter ``scalar``:
    the scalar factor to multiply by.

Parameter ``poly``:
    the polynomial to multiply by.

Returns:
    the product of the given polynomial and scalar.)doc";

// Docstring regina::python::doc::__mul_3
static const char *__mul_3 =
R"doc(Multiplies the given arrow polynomial by the given Laurent polynomial
in ``A``.

Parameter ``arrow``:
    the arrow polynomial to multiply by.

Parameter ``laurent``:
    the Laurent polynomial to multiply by; this will be treated as a
    Laurent polynomial in the ordinary variable ``A``.

Returns:
    the product of the given arrow and Laurent polynomials.)doc";

// Docstring regina::python::doc::__mul_4
static const char *__mul_4 =
R"doc(Multiplies the given arrow polynomial by the given Laurent polynomial
in ``A``.

Parameter ``laurent``:
    the Laurent polynomial to multiply by; this will be treated as a
    Laurent polynomial in the ordinary variable ``A``.

Parameter ``arrow``:
    the arrow polynomial to multiply by.

Returns:
    the product of the given arrow and Laurent polynomials.)doc";

// Docstring regina::python::doc::__mul_5
static const char *__mul_5 =
R"doc(Multiplies the two given polynomials.

Parameter ``lhs``:
    the first polynomial to multiply.

Parameter ``rhs``:
    the second polynomial to multiply.

Returns:
    the product of the two given polynomials.)doc";

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

Parameter ``lhs``:
    the polynomial to sutract *rhs* from.

Parameter ``rhs``:
    the polynomial to subtract from *lhs*.

Returns:
    the difference of the two given polynomials.)doc";

// Docstring regina::python::doc::__sub_3
static const char *__sub_3 =
R"doc(Subtracts the two given polynomials.

Parameter ``lhs``:
    the polynomial to sutract *rhs* from.

Parameter ``rhs``:
    the polynomial to subtract from *lhs*.

Returns:
    the difference of the two given polynomials.)doc";

// Docstring regina::python::doc::__sub_4
static const char *__sub_4 =
R"doc(Subtracts the two given polynomials.

Parameter ``lhs``:
    the polynomial to sutract *rhs* from.

Parameter ``rhs``:
    the polynomial to subtract from *lhs*.

Returns:
    the difference of the two given polynomials.)doc";

// Docstring regina::python::doc::__sub_5
static const char *__sub_5 =
R"doc(Subtracts the two given polynomials.

Parameter ``lhs``:
    the polynomial to sutract *rhs* from.

Parameter ``rhs``:
    the polynomial to subtract from *lhs*.

Returns:
    the difference of the two given polynomials.)doc";

namespace Arrow_ {

// Docstring regina::python::doc::Arrow_::__array
static const char *__array =
R"doc(Returns the Laurent polynomial in ``A`` that is attached to the given
product of diagram variables.

Python:
    The diagram sequence should be presented as a sequence of integer
    arguments; that is, you should write ``arrow[a1, a2, ...]``.
    Moreover, in Python this operator can also _set_ the attached
    Laurent polynomial: you can write ``arrow[a1, a2, ...] = ...``.
    However, when _getting_ a coefficient this operator will return by
    value (to enforce constness), which means for example you cannot
    write something like ``arrow[a1, a2, ...].negate()``.

Exception ``InvalidArgument``:
    The given sequence of integers is non-empty and its last entry is
    zero.

Parameter ``d``:
    a sequence of integers representing some product of diagram
    variables. If this sequence is non-empty, then its last entry
    should be strictly positive.

Returns:
    the Laurent polynomial attached to the given product of diagram
    variables.)doc";

// Docstring regina::python::doc::Arrow_::__cmp
static const char *__cmp =
R"doc(Compares this against the given polynomial under a total ordering of
all arrow polynomials.

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
    The result of the comparison between this and the given
    polynomial.)doc";

// Docstring regina::python::doc::Arrow_::__copy
static const char *__copy =
R"doc(Creates a new copy of the given polynomial.

This constructor induces a deep copy of the given polynomial.)doc";

// Docstring regina::python::doc::Arrow_::__default
static const char *__default = R"doc(Creates the zero polynomial.)doc";

// Docstring regina::python::doc::Arrow_::__eq
static const char *__eq =
R"doc(Tests whether this and the given polynomial are equal.

Parameter ``rhs``:
    the polynomial to compare with this.

Returns:
    ``True`` if and only if this and the given polynomial are equal.)doc";

// Docstring regina::python::doc::Arrow_::__eq_2
static const char *__eq_2 =
R"doc(Tests whether this is equal to the given Laurent polynomial in *A*.

For this to be true, this polynomial must not use any of the diagram
variables ``K_i`` at all.

Parameter ``rhs``:
    the Laurent polynomial in *A* to compare this with.

Returns:
    ``True`` if and only if this and the given Laurent polynomial are
    equal.)doc";

// Docstring regina::python::doc::Arrow_::__iadd
static const char *__iadd =
R"doc(Adds the given polynomial to this.

Parameter ``other``:
    the polynomial to add to this.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Arrow_::__imul
static const char *__imul =
R"doc(Multiplies this polynomial by the given integer constant.

Parameter ``scalar``:
    the scalar factor to multiply by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Arrow_::__imul_2
static const char *__imul_2 =
R"doc(Multiplies this arrow polynomial by the given Laurent polynomial in
``A``.

Parameter ``laurent``:
    the Laurent polynomial to multiply by; this will be treated as a
    Laurent polynomial in the ordinary variable ``A``.

Returns:
    a reference to this arrow polynomial.)doc";

// Docstring regina::python::doc::Arrow_::__imul_3
static const char *__imul_3 =
R"doc(Multiplies this by the given polynomial.

Parameter ``other``:
    the polynomial to multiply this by.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Arrow_::__init
static const char *__init =
R"doc(Creates a new polynomial from the given collection of diagram
sequences and non-zero Laurent polynomials in *A*.

The data should be presented as a collection of pairs of the form
``(seq, laurent)``, where ``seq`` is a diagram sequence and
``laurent`` is the associated Laurent polynomial in ``A``.

The pairs may be given in any order. An empty sequence will be treated
as the zero polynomial.

Unlike the std::initializer_list constructor, zero Laurent polynomials
are allowed (these will be silently ignored), and multiple pairs with
the same diagram sequences are also allowed (these will be summed
together).

This routine is targeted more towards Python users (since in C++ it is
often easier to hard-code arrow polynomials using the
std::initializer_list constructor). As an example, Python users can
create the arrow polynomial ``A^-4 + (A^-6 - A^-10) K_1`` using either
of the expressions:

```
Arrow([([], Laurent(-4, [1])), ([1], Laurent(-10, [-1,0,0,0,1]))])
Arrow([([], (-4, [1])), ([1], (-10, [-1,0,0,0,1]))])
```

Python:
    Instead of the iterators *begin* and *end*, this routine takes a
    python list of pairs ``(seq, laurent)``, where *seq* is a python
    list of integers representing a diagram sequence, and where
    *laurent* is either (i) a Laurent polynomial, or (ii) a pair
    ``(minExp, coefficients)`` which could be used to construct a
    Laurent polynomial. In the latter case, *minExp* would an integer,
    and *coefficients* would be a python list of integers.

Precondition:
    No diagram sequence ends in zero.

Exception ``InvalidArgument``:
    At least one of the given diagram sequences is non-empty and ends
    in zero.

Parameter ``begin``:
    the beginning of the collection of pairs, as outlined above.

Parameter ``end``:
    a past-the-end iterator indicating the end of the collection of
    pairs.)doc";

// Docstring regina::python::doc::Arrow_::__init_2
static const char *__init_2 =
R"doc(Creates the given Laurent polynomial in *A*.

This polynomial will have no diagram variables at all.

Parameter ``laurent``:
    the value of this new polynomial, given as a Laurent polynomial in
    *A*.)doc";

// Docstring regina::python::doc::Arrow_::__isub
static const char *__isub =
R"doc(Subtracts the given polynomial from this.

Parameter ``other``:
    the polynomial to subtract from this.

Returns:
    a reference to this polynomial.)doc";

// Docstring regina::python::doc::Arrow_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given polynomials.

This global routine simply calls Arrow::swap(); it is provided so that
Arrow meets the C++ Swappable requirements.

Parameter ``a``:
    the first polynomial whose contents should be swapped.

Parameter ``b``:
    the second polynomial whose contents should be swapped.)doc";

// Docstring regina::python::doc::Arrow_::init
static const char *init = R"doc(Sets this to become the zero polynomial.)doc";

// Docstring regina::python::doc::Arrow_::initDiagram
static const char *initDiagram =
R"doc(Sets this to become the given product of diagram variables, using a
deep copy.

If *d* is the sequence ``a_1,a_2,...``, then this polynomial will be
set to ``(K_1)^(a_1) (K_2)^(a_2) ...``.

Exception ``InvalidArgument``:
    The given sequence of integers is non-empty and its last entry is
    zero.

Parameter ``d``:
    a sequence of integers representing some product of diagram
    variables. If this sequence is non-empty, then its last entry
    should be strictly positive.)doc";

// Docstring regina::python::doc::Arrow_::invertA
static const char *invertA =
R"doc(Replaces ``A`` with ``A^-1`` in this polynomial. This polynomial is
changed directly.

Calling this routine is equivalent to calling ``scaleUp(-1)``.)doc";

// Docstring regina::python::doc::Arrow_::isZero
static const char *isZero =
R"doc(Returns whether this is the zero polynomial.

Returns:
    ``True`` if and only if this is the zero polynomial.)doc";

// Docstring regina::python::doc::Arrow_::multDiagram
static const char *multDiagram =
R"doc(Multiplies this polynomial by the given diagram variable.

Specifically, if the given index is *i*, then this polynomial will be
multiplied by the diagram variable ``K_i``. Note that this requires
*i* to be strictly positive.

Exception ``InvalidArgument``:
    The given index is zero.

Parameter ``index``:
    the index of the diagram variable to multiply by; this must be
    strictly positive.)doc";

// Docstring regina::python::doc::Arrow_::negate
static const char *negate = R"doc(Negates this polynomial. This polynomial is changed directly.)doc";

// Docstring regina::python::doc::Arrow_::scaleDown
static const char *scaleDown =
R"doc(Divides all exponents in this polynomial by *k* for some integer *k*.
This is equivalent to replacing the variable ``A`` with ``A^(1/k)``.

Both positive and negative scaling factors *k* are allowed.

Precondition:
    *k* is non-zero.

Precondition:
    All exponents of ``A`` that appear in this polynomial with non-
    zero coefficients are multiples of *k*.

Exception ``FailedPrecondition``:
    Either *k* is zero, or some exponent of ``A`` with a non-zero
    coefficient is not a multiple of *k*.

Parameter ``k``:
    the scaling factor to divide exponents by.)doc";

// Docstring regina::python::doc::Arrow_::scaleUp
static const char *scaleUp =
R"doc(Multiplies all exponents of ``A`` in this polynomial by *k* for some
integer *k*. This is equivalent to replacing the variable ``A`` with
``A^k``.

Both positive and negative scaling factors *k* are allowed.

Precondition:
    *k* is non-zero.

Parameter ``k``:
    the scaling factor to multiply exponents by.)doc";

// Docstring regina::python::doc::Arrow_::set
static const char *set =
R"doc(Changes the Laurent polynomial in ``A`` that is attached to the given
product of diagram variables.

The new coefficient is allowed to be zero.

Python:
    The diagram sequence should be presented as a sequence of integer
    arguments; that is: ``arrow.set(a1, a2, ...) = value``. In Python
    (but not C++), you can also set the attached Laurent polynomial
    directly using the syntax ``arrow[a1, a2, ...] = value``.

Exception ``InvalidArgument``:
    The given sequence of integers is non-empty and its last entry is
    zero.

Parameter ``d``:
    a sequence of integers representing some product of diagram
    variables. If this sequence is non-empty, then its last entry
    should be strictly positive.

Parameter ``value``:
    the new Laurent polynomial that should be attached to the given
    product of diagram variables.)doc";

// Docstring regina::python::doc::Arrow_::shift
static const char *shift =
R"doc(Multiplies this polynomial by ``A^s`` for some integer *s*.

Parameter ``s``:
    the power of *A* to multiply by.)doc";

// Docstring regina::python::doc::Arrow_::sumLaurent
static const char *sumLaurent =
R"doc(Returns the sum of all Laurent polynomials in ``A`` that are attached
to each diagram sequence. This is the Laurent polynomial in ``A`` that
would be obtained if we set each diagram variable ``K_i = 1``.

Returns:
    the sum of all attached Laurent polynomials in ``A``.)doc";

// Docstring regina::python::doc::Arrow_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given polynomial. This is a fast
(constant time) operation.

Parameter ``other``:
    the polynomial whose contents should be swapped with this.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

