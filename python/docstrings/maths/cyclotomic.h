/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Cyclotomic
static const char *Cyclotomic =
R"doc(Represents an element of a cyclotomic field.

The cyclotomic field of order *n* extends the rationals with a
primitive *n*th root of unity. This is isomorphic to the polynomial
field ``ℚ[x]/Φ_n``, where ``Φ_n`` is the *n*th cyclotomic polynomial.

Using this isomorphism, each element of the cyclotomic field can be
uniquely represented as a rational polynomial of degree strictly less
than ``deg(Φ_n) = φ(n)``, where ``φ`` denotes Euler's totient
function. This class stores field elements using such a polynomial
representation, and does *not* store complex numbers directly. If you
require the complex value of a field element (as a floating point
approximation), you can call evaluate().

Each object of this class stores both the value of the field element
and the order *n* of the underlying field. This means that you can
freely work with elements of different fields simultaneously, though
of course most operations (such as addition, multplication and so on)
require all operands to belong to the same field.

This class requires that the order *n* is strictly positive.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

Although this class makes use of global data in its implementation,
all of its methods are thread-safe.)doc";

// Docstring regina::python::doc::__add
static const char *__add =
R"doc(Adds the two given cyclotomic field elements.

Precondition:
    Both arguments belong to the same cyclotomic field.

Parameter ``lhs``:
    the first field element to add.

Parameter ``rhs``:
    the second field element to add.

Returns:
    the sum of both field elements.)doc";

// Docstring regina::python::doc::__add_2
static const char *__add_2 =
R"doc(Adds the two given cyclotomic field elements.

Precondition:
    Both arguments belong to the same cyclotomic field.

Parameter ``lhs``:
    the first field element to add.

Parameter ``rhs``:
    the second field element to add.

Returns:
    the sum of both field elements.)doc";

// Docstring regina::python::doc::__add_3
static const char *__add_3 =
R"doc(Adds the two given cyclotomic field elements.

Precondition:
    Both arguments belong to the same cyclotomic field.

Parameter ``lhs``:
    the first field element to add.

Parameter ``rhs``:
    the second field element to add.

Returns:
    the sum of both field elements.)doc";

// Docstring regina::python::doc::__add_4
static const char *__add_4 =
R"doc(Adds the two given cyclotomic field elements.

Precondition:
    Both arguments belong to the same cyclotomic field.

Parameter ``lhs``:
    the first field element to add.

Parameter ``rhs``:
    the second field element to add.

Returns:
    the sum of both field elements.)doc";

// Docstring regina::python::doc::__div
static const char *__div =
R"doc(Divides the given field element by the given rational.

Precondition:
    The argument *scalar* is non-zero.

Parameter ``elt``:
    the field element to divide by the given rational.

Parameter ``scalar``:
    the rational to divide by.

Returns:
    the quotient of the given field element by the given rational.)doc";

// Docstring regina::python::doc::__div_2
static const char *__div_2 =
R"doc(Divides the two given cyclotomic field elements.

Precondition:
    The second argument *rhs* is non-zero.

Precondition:
    Both arguments belong to the same cyclotomic field.

Parameter ``lhs``:
    the field element to divide by *rhs*.

Parameter ``rhs``:
    the field element to divide *lhs* by.

Returns:
    the result of dividing *lhs* by *rhs*.)doc";

// Docstring regina::python::doc::__mul
static const char *__mul =
R"doc(Multiplies the given field element by the given rational.

Parameter ``elt``:
    the field element to multiply by.

Parameter ``scalar``:
    the rational to multiply by.

Returns:
    the product of the given field element and rational.)doc";

// Docstring regina::python::doc::__mul_2
static const char *__mul_2 =
R"doc(Multiplies the given field element by the given rational.

Parameter ``scalar``:
    the rational to multiply by.

Parameter ``elt``:
    the field element to multiply by.

Returns:
    the product of the given field element and rational.)doc";

// Docstring regina::python::doc::__mul_3
static const char *__mul_3 =
R"doc(Multiplies the two given cyclotomic field elements.

Precondition:
    Both arguments belong to the same cyclotomic field.

Parameter ``lhs``:
    the first field element to multiply.

Parameter ``rhs``:
    the second field element to multiply.

Returns:
    the product of both field elements.)doc";

// Docstring regina::python::doc::__sub
static const char *__sub =
R"doc(Returns the negative of the given field element.

Parameter ``arg``:
    the field element to negate.

Returns:
    the negative of *arg*.)doc";

// Docstring regina::python::doc::__sub_2
static const char *__sub_2 =
R"doc(Subtracts the two given cyclotomic field elements.

Precondition:
    Both arguments belong to the same cyclotomic field.

Parameter ``lhs``:
    the field element to subtract from.

Parameter ``rhs``:
    the field element to subtract.

Returns:
    the first field element minus the second.)doc";

// Docstring regina::python::doc::__sub_3
static const char *__sub_3 =
R"doc(Subtracts the two given cyclotomic field elements.

Precondition:
    Both arguments belong to the same cyclotomic field.

Parameter ``lhs``:
    the field element to subtract from.

Parameter ``rhs``:
    the field element to subtract.

Returns:
    the first field element minus the second.)doc";

// Docstring regina::python::doc::__sub_4
static const char *__sub_4 =
R"doc(Subtracts the two given cyclotomic field elements.

Precondition:
    Both arguments belong to the same cyclotomic field.

Parameter ``lhs``:
    the field element to subtract from.

Parameter ``rhs``:
    the field element to subtract.

Returns:
    the first field element minus the second.)doc";

// Docstring regina::python::doc::__sub_5
static const char *__sub_5 =
R"doc(Subtracts the two given cyclotomic field elements.

Precondition:
    Both arguments belong to the same cyclotomic field.

Parameter ``lhs``:
    the field element to subtract from.

Parameter ``rhs``:
    the field element to subtract.

Returns:
    the first field element minus the second.)doc";

namespace Cyclotomic_ {

// Docstring regina::python::doc::Cyclotomic_::__array
static const char *__array =
R"doc(Returns an individual rational coefficient of the polynomial
representation of this field element.

The polynomial representation expresses this field element as a member
of ``ℚ[x]/Φ_n``, using a rational polynomial of degree strictly less
than ``deg(Φ_n) = φ(n)``; that is, strictly less than the value
returned by degree(). See the Cyclotomic class notes for further
details.

In particular, for a field element *e*, the operator ``e[i]`` will
return the coefficient of ``x^i`` in this polynomial representation.

This is a constant (read-only) routine; note that there is a non-
constant (read-write) variant of this routine also.

Parameter ``exp``:
    indicates which coefficient to return; this must be between 0 and
    degree()-1 inclusive.

Returns:
    a constant reference to the corresponding rational coefficient.)doc";

// Docstring regina::python::doc::Cyclotomic_::__array_2
static const char *__array_2 =
R"doc(Offers access to an individual rational coefficient of the polynomial
representation of this field element.

The polynomial representation expresses this field element as a member
of ``ℚ[x]/Φ_n``, using a rational polynomial of degree strictly less
than ``deg(Φ_n) = φ(n)``; that is, strictly less than the value
returned by degree(). See the Cyclotomic class notes for further
details.

In particular, for a field element *e*, the operator ``e[i]`` will
give access to the coefficient of ``x^i`` in this polynomial
representation.

This routine returns a non-constant reference: you can use this to
directly edit the coefficients (and therefore the value of the field
element). Note that there is also a constant (read-only) variant of
this routine.

Parameter ``exp``:
    indicates which coefficient to access; this must be between 0 and
    degree()-1 inclusive.

Returns:
    a reference to the corresponding rational coefficient.)doc";

// Docstring regina::python::doc::Cyclotomic_::__copy
static const char *__copy =
R"doc(Creates a copy of the given field element, within the same cyclotomic
field.

This constructor induces a deep copy of *value*.

Parameter ``value``:
    the field element to copy.)doc";

// Docstring regina::python::doc::Cyclotomic_::__default
static const char *__default =
R"doc(Creates an uninitialised field element.

This element must be initialised using either init() or the assignment
operator before it can be used.

The underlying cyclotomic field is not yet known; this will also be
specified during the call to init() or the assignment operator.)doc";

// Docstring regina::python::doc::Cyclotomic_::__eq
static const char *__eq =
R"doc(Tests whether or not this and the given argument are the same element
of the same cyclotomic field.

If this and *rhs* have different underlying fields then this test will
always return ``False``, even if they take the same numerical value
when evaluated as complex numbers.

If either this or *rhs* have not been initialised (typically because
they were created using the default constructor), then this comparison
will return ``False``. If *both* field elements have not been
initialised, then this comparison will return ``True``.

Parameter ``rhs``:
    the value to compare with this.

Returns:
    ``True`` if and only if this and *rhs* are the same element of the
    same cyclotomic field.)doc";

// Docstring regina::python::doc::Cyclotomic_::__iadd
static const char *__iadd =
R"doc(Adds the given field element to this.

Precondition:
    The argument *other* belongs to the same cyclotomic field as this.

Parameter ``other``:
    the field element to add to this.

Returns:
    a reference to this field element.)doc";

// Docstring regina::python::doc::Cyclotomic_::__idiv
static const char *__idiv =
R"doc(Divides this field element by the given rational.

This has the effect of dividing the polynomial representation by a
scalar constant.

Precondition:
    The given rational is non-zero.

Parameter ``scalar``:
    the rational to divide this by.

Returns:
    a reference to this field element.)doc";

// Docstring regina::python::doc::Cyclotomic_::__idiv_2
static const char *__idiv_2 =
R"doc(Divides this by the given field element.

Precondition:
    The argument *other* is non-zero.

Precondition:
    The argument *other* belongs to the same cyclotomic field as this.

Parameter ``other``:
    the field element to divide this by.

Returns:
    a reference to this field element.)doc";

// Docstring regina::python::doc::Cyclotomic_::__imul
static const char *__imul =
R"doc(Multiplies this field element by the given rational.

This has the effect of multiplying the polynomial representation by a
scalar constant.

Parameter ``scalar``:
    the rational to multiply this by.

Returns:
    a reference to this field element.)doc";

// Docstring regina::python::doc::Cyclotomic_::__imul_2
static const char *__imul_2 =
R"doc(Multiplies this by the given field element.

Precondition:
    The argument *other* belongs to the same cyclotomic field as this.

Parameter ``other``:
    the field element to multiply this by.

Returns:
    a reference to this field element.)doc";

// Docstring regina::python::doc::Cyclotomic_::__init
static const char *__init =
R"doc(Creates the zero element of the given cyclotomic field.

Parameter ``field``:
    the order of the underlying cyclotomic field; this must be
    strictly positive.)doc";

// Docstring regina::python::doc::Cyclotomic_::__init_2
static const char *__init_2 =
R"doc(Creates the given integer element within the given cyclotomic field.

The polynomial representation of this element will simply be an
integer constant.

Parameter ``field``:
    the order of the underlying cyclotomic field; this must be
    strictly positive.

Parameter ``value``:
    the value of this element; that is, the integer constant.)doc";

// Docstring regina::python::doc::Cyclotomic_::__init_3
static const char *__init_3 =
R"doc(Creates the given rational element within the given cyclotomic field.

The polynomial representation of this element will simply be a
rational constant.

Parameter ``field``:
    the order of the underlying cyclotomic field; this must be
    strictly positive.

Parameter ``value``:
    the value of this element; that is, the rational constant.)doc";

// Docstring regina::python::doc::Cyclotomic_::__init_4
static const char *__init_4 =
R"doc(Creates a new field element from the given sequence of coefficients.
The coefficients should describe the field element's polynomial
representation, and should be given in order from the constant
coefficient upwards. See operator[] for details on what this
polynomial representation means.

There should be at most ``deg(Φ_n) = φ(n)`` coefficients in the list,
where *n* is the given order of the underlying field; any missing
coefficients are assumed to be zero. In particular, an empty sequence
is allowed (and represents the zero field element).

Precondition:
    Rationals can be assigned values from dereferenced iterators of
    type *iterator*.

Python:
    Instead of a pair of iterators, this routine takes a python list
    of coefficients.

Parameter ``field``:
    the order of the underlying cyclotomic field; this must be
    strictly positive.

Parameter ``begin``:
    the beginning of a sequence of at most ``φ(n)`` coefficients, as
    described above.

Parameter ``end``:
    a past-the-end iterator indicating the end of the sequence of
    coefficients.)doc";

// Docstring regina::python::doc::Cyclotomic_::__isub
static const char *__isub =
R"doc(Subtracts the given field element from this.

Precondition:
    The argument *other* belongs to the same cyclotomic field as this.

Parameter ``other``:
    the field element to subtract from this.

Returns:
    a reference to this field element.)doc";

// Docstring regina::python::doc::Cyclotomic_::__ne
static const char *__ne =
R"doc(Tests whether or not this and the given argument are the same element
of the same cyclotomic field.

If this and *rhs* have different underlying fields then this test will
always return ``True`` (indicating that the elements are not equal),
even if they take the same numerical value when evaluated as complex
numbers.

If either this or *rhs* have not been initialised (typically because
they were created using the default constructor), then this comparison
will return ``True``. If *both* field elements have not been
initialised, then this comparison will return ``False``.

Parameter ``rhs``:
    the value to compare with this.

Returns:
    ``False`` if this and *rhs* are the same element of the same
    cyclotomic field, or ``True`` if they are not.)doc";

// Docstring regina::python::doc::Cyclotomic_::cyclotomic
static const char *cyclotomic =
R"doc(Returns the *n*th cyclotomic polynomial ``Φ_n``.

Cyclotomic polynomials are cached after they are computed, and so
after the first call to ``cyclotomic(n)``, all subsequent calls with
the same value of *n* will be essentially instantaneous.

Although it queries and manipulates a global cache, this routine is
thread-safe.

Precondition:
    The given integer *n* must be strictly positive.

Python:
    Since Python exposes the class Polynomial<Rational> but not
    Polynomial<Integer>, this routine will convert the result to a
    Polynomial<Rational> (and will therefore return by value, not by
    reference).

Parameter ``n``:
    indicates which cyclotomic polynomial to return.

Returns:
    the cyclotomic polynomial ``Φ_n``.)doc";

// Docstring regina::python::doc::Cyclotomic_::degree
static const char *degree =
R"doc(Returns the degree of the polynomial that defines the underlying
cyclotomic field.

This is the degree of the cyclotomic polynomial ``Φ_n``, and also the
value of Euler's totient function ``φ(n)``, where *n* is the order of
the field as returned by field().

A value of zero indicates that this field element has not yet been
initialised (for instance, it was created using the default
constructor).

Python:
    This is also used to implement the Python special method
    __len__().

Returns:
    the degree of the polynomial that defines the underlying field.)doc";

// Docstring regina::python::doc::Cyclotomic_::evaluate
static const char *evaluate =
R"doc(Returns the value of this cyclotomic field element as a complex
number.

The evaluation depends upon *which* primitive root of unity is used to
build the underlying cyclotomic field of order *n*. This ambiguity is
resolved as follows.

Suppose the polynomial representation of this field element in
``ℚ[x]/Φ_n`` (as described in the Cyclotomic class notes) is ``f(x)``.
Then the evaluation of this field element will be ``f(ρ)``, where *ρ*
is the ``n``th root of unity ``ρ = exp(2πi × k/n)``, and where *k* is
the argument *whichRoot* as passed to this routine.

Precondition:
    The argument *whichRoot* is coprime to *n* (the order of the
    underlying cyclotomic field).

Precondition:
    This field element has been initialised (either through a non-
    default constructor, an assignment operator, or by calling
    init()).

.. warning::
    This routine uses floating point arithmetic, and so the value that
    it returns is subject to the usual floating point error.

Parameter ``whichRoot``:
    indicates which root of unity will be used to convert the
    polynomial representation of this field element into a complex
    number.

Returns:
    a floating-point approximation of this cyclotomic field element as
    a complex number.)doc";

// Docstring regina::python::doc::Cyclotomic_::field
static const char *field =
R"doc(Returns the order *n* of the underlying cyclotomic field to which this
element belongs.

A value of zero indicates that this field element has not yet been
initialised (for instance, it was created using the default
constructor).

Returns:
    the order of the underlying cyclotomic field.)doc";

// Docstring regina::python::doc::Cyclotomic_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given field elements.

This global routine simply calls Cyclotomic::swap(); it is provided so
that Cyclotomic meets the C++ Swappable requirements.

Parameter ``a``:
    the first field element whose contents should be swapped.

Parameter ``b``:
    the second field element whose contents should be swapped.)doc";

// Docstring regina::python::doc::Cyclotomic_::init
static const char *init =
R"doc(Initialises this to be the zero element of the given cyclotomic field.

This is safe even if this element was previously initialised as an
element of a *different* field - all prior information about this
field element will be safely discarded.

Parameter ``field``:
    the order of the cyclotomic field to which this field element will
    now belong; this must be strictly positive.)doc";

// Docstring regina::python::doc::Cyclotomic_::inverse
static const char *inverse =
R"doc(Returns the inverse of this field element. This field element is not
changed.

Precondition:
    This field element has already been initialised (and so it already
    has specified an underlying cyclotomic field).

Precondition:
    This field element is non-zero.)doc";

// Docstring regina::python::doc::Cyclotomic_::invert
static const char *invert =
R"doc(Inverts this field element. This field element is changed directly.

Precondition:
    This field element has already been initialised (and so it already
    has specified an underlying cyclotomic field).

Precondition:
    This field element is non-zero.)doc";

// Docstring regina::python::doc::Cyclotomic_::negate
static const char *negate = R"doc(Negates this field element. This field element is changed directly.)doc";

// Docstring regina::python::doc::Cyclotomic_::polynomial
static const char *polynomial =
R"doc(Returns the full polynomial representation of this field element.

The polynomial representation expresses this field element as a member
of ``ℚ[x]/Φ_n``, using a rational polynomial of degree strictly less
than ``deg(Φ_n) = φ(n)``; that is, strictly less than the value
returned by degree(). See the Cyclotomic class notes for further
details.

Precondition:
    This field element has been initialised (either through a non-
    default constructor, an assignment operator, or by calling
    init()).

Returns:
    the full polynomial representation of this field element.)doc";

// Docstring regina::python::doc::Cyclotomic_::str
static const char *str =
R"doc(Returns this field element as a human-readable string, using the given
variable name instead of ``x``.

The field element will be written using its rational polynomial
representation. The underlying field will *not* be indicated in the
output, since this is often already understood. If required, it can be
accessed by calling ``c.field()``.

.. note::
    There is also the usual variant of str() which takes no arguments;
    that variant is inherited from the Output class.

Parameter ``variable``:
    the symbol to use for the polynomial variable. This may be
    ``None``, in which case the default variable ``x`` will be used.

Returns:
    this field element as a human-readable string.)doc";

// Docstring regina::python::doc::Cyclotomic_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given field element. This is a fast
(constant time) operation.

This and the given field element do not need to belong to the same
cyclotomic field, and indeed one or both of them may be uninitialised.
The underlying fields (if different) will be swapped accordingly.

Parameter ``other``:
    the field element whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Cyclotomic_::utf8
static const char *utf8 =
R"doc(Returns this field element as a human-readable string using unicode
characters, using the given variable name instead of ``x``.

The field element will be written using its rational polynomial
representation. The underlying field will *not* be indicated in the
output, since this is often already understood. If required, it can be
accessed by calling ``c.field()``.

This is similar to the output from str(), except that it uses unicode
characters to make the output more pleasant to read. In particular, it
makes use of superscript digits for exponents.

The string is encoded in UTF-8.

.. note::
    There is also the usual variant of utf8() which takes no
    arguments; that variant is inherited from the Output class.

Parameter ``variable``:
    the symbol to use for the polynomial variable. This may be
    ``None``, in which case the default variable ``x`` will be used.

Returns:
    this field element as a unicode-enabled human-readable string.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

