/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::polynomialProduct
inline constexpr const char polynomialProduct[] =
R"doc(Multiplies two polynomials using the specified algorithm.

Both the input and output polynomials will be given as arrays of
consecutive coefficients (in order from lowest to highest degree
terms). The corresponding exponents are not specified (and do not need
to be); if the input exponents are ``a0,...,a1`` and ``b0,...,b1``,
then the output exponents will be ``(a0+b0),...,(a1+b1)``.

This routine is provided mainly for timing, testing and diagnostics.
If you just wish to multiply two polynomials, you should use the usual
polynomial product operators (e.g., ``x = y * z``): this way Regina
will choose the most suitable algorithm for you.

The Karatsuba algorithm is not available for all polynomials: it
requires the two polynomials to have comparable degree spans (i.e.,
one polynomial cannot have significantly more coefficients than the
other. At present, this means (roughly) that the shorter polynomial
should be more than half the length of the longer polynomial. The
precise constraints are subject to change in future versions of
Regina, and so if you are forcing Karatsuba multiplication then it is
strongly recommended that you wrap this in a try/catch block.

As a special case, if either of the two input polynomials has just one
or zero coefficients then the given algorithm will be ignored, and
this routine will use scalar multiplication instead.

Python:
    The coefficient type *T* will be taken to be Integer. Each input
    polynomial should each be presented as a Python list of integer
    coefficients, and the output polynomial will be presented the same
    way. Since Python does not support C++ templates, you should pass
    the algorithm at runtime as a third argument: that is,
    ``product(lhs, rhs, algorithm)``.

Exception ``InvalidArgument``:
    The *algorithm* argument specified Karatsuba multiplication, but
    the two given polynomials do not have comparable degree spans. See
    above for further explanation.

Template parameter ``algorithm``:
    the polynomial multiplication algorithm to use.

Template parameter ``T``:
    the coefficient type.

Parameter ``lhs``:
    the coefficients of the first polynomial to multiply.

Parameter ``rhs``:
    the coefficients of the second polynomial to multiply.

Returns:
    the coefficients of the product of both polynomials.)doc";

struct PolynomialProductAlgorithm {

// Docstring regina::python::doc::PolynomialProductAlgorithm::Classic
static constexpr const char Classic[] =
R"doc(The classic "schoolbook" algorithm involving two nested loops over the
polynomial coefficients. To multiply two polynomials with degree span
*n*, this requires computing ``O(n^2)`` individual products of
coefficients.)doc";

// Docstring regina::python::doc::PolynomialProductAlgorithm::Default
static constexpr const char Default[] =
R"doc(The default algorithm. Here Regina will choose whichever algorithm it
thinks (rightly or wrongly) is most appropriate.)doc";

// Docstring regina::python::doc::PolynomialProductAlgorithm::Karatsuba
static constexpr const char Karatsuba[] =
R"doc(Karatsuba's divide-and-conquer algorithm. To multiply two polynomials
with the same degree span *n*, this requires computing ``O(n^(log₂ 3))
≃ O(n^1.585)`` individual products of coefficients.

Karatsuba's algorithm is asymptotically better than the classic
algorithm, but it carries significant overhead and so will typically
be slower for small polynomials.)doc";

// Docstring regina::python::doc::PolynomialProductAlgorithm::__class
static constexpr const char __class[] = R"doc(Represents different algorithms for multiplying polynomials.)doc";

}; // struct PolynomialProductAlgorithm

namespace detail {

// Docstring regina::python::doc::detail::productBest
inline constexpr const char productBest[] =
R"doc(Multiplies two ranges of coefficients using whatever polynomial
multiplication algorithm this routine deems best. This should be the
main entry point for polynomial types when implementing their own
product operations such as ``x * y``, ``x *= y``, or ``x.addProduct(y,
z)``.

The input consists of two C-style arrays *lhs* and *rhs*, each given
by a starting pointer and the number of coefficients. The output will
consist of ``lhsLen + rhsLen - 1`` coefficients, which will likewise
be written to the given C-style array *dest*. Whether the output
coefficients are written via assignment or addition will depend on the
template parameter *operation*.

Each range of input coefficients is given in order from lowest degree
to highest degree, and may include zeroes at the endpoints. Likewise,
the output coefficients will be written in order from lowest degree to
highest degree, and may include zeroes at the endpoints.

Regarding scratch space: this algorithm may or may not need scratch
space (this depends on whether Karatsuba multiplication is used).
However, it manages this perfectly well itself: if it ever discovers
that scratch space is required then it will create a single block of
scratch space for the entire multiplication process, and will pass
this down through to any recursive calls to productKaratsuba() and
productBest().

Precondition:
    Both *lhsLen* and *rhsLen* are strictly positive.

Precondition:
    If *scratch* is non-null, then it should point to a C-style array
    of size at least ``2(max-1 + log_2(max-1))``, where *max* denotes
    the larger of *lhsLen* and *rhsLen*.

Template parameter ``T``:
    the coefficient type.

Template parameter ``operation``:
    indicates whether the output coefficients should be written via
    assignment (``dest[i] = result``), addition (``dest[i] +=
    result``), or whichever is fastest (which is only appropriate when
    every cofficient in *dest* has previously been initialised to
    zero).

Template parameter ``moveable``:
    ``True`` if we are allowed to move data out of the input
    coefficients, or ``False`` if the input coefficients should be
    read-only.

Parameter ``dest``:
    the C-style array in which to write the output coefficients.

Parameter ``lhs``:
    the C-style array holding the first range of input coefficients.

Parameter ``lhsLen``:
    the total number of coefficients in the first input range.

Parameter ``rhs``:
    the C-style array holding the second range of input coefficients.

Parameter ``rhsLen``:
    the total number of coefficients in the second input range.

Parameter ``scratch``:
    pre-allocated scratch space, or ``None`` if the product algorithm
    should create its own scratch space if/when required. The scratch
    space does not need to be initialised in any particular way.)doc";

// Docstring regina::python::doc::detail::productClassic
inline constexpr const char productClassic[] =
R"doc(Multiplies two ranges of coefficients using the classical polynomial
multiplication algorithm.

This involves scanning through both input ranges in a pair of nested
loops, and so to multiply two polynomials with degree span *n*
requires computing ``O(n^2)`` individual products of coefficients.

See productBest() for details of how the input and output ranges work.

Precondition:
    Both *lhsLen* and *rhsLen* are strictly positive.

Template parameter ``T``:
    the coefficient type.

Template parameter ``operation``:
    indicates whether the output coefficients should be written via
    assignment (``dest[i] = result``), addition (``dest[i] +=
    result``), or whichever is fastest (which is only appropriate when
    every cofficient in *dest* has previously been initialised to
    zero).

Parameter ``dest``:
    the C-style array in which to write the output coefficients.

Parameter ``lhs``:
    the C-style array holding the first range of input coefficients.

Parameter ``lhsLen``:
    the total number of coefficients in the first input range.

Parameter ``rhs``:
    the C-style array holding the second range of input coefficients.

Parameter ``rhsLen``:
    the total number of coefficients in the second input range.)doc";

// Docstring regina::python::doc::detail::productKaratsuba
inline constexpr const char productKaratsuba[] =
R"doc(Multiplies two ranges of coefficients using Karatsuba's polynomial
multiplication algorithm.

Karatsuba's algorithm is a divide-and-conquer algorithm: to multiply
two polynomials with the same degree span *n*, it requires computing
``O(n^(log₂ 3)) ≃ O(n^1.585)`` individual products of coefficients.

See productBest() for details of how the input and output ranges work.

Regarding scratch space: this algorithm _will_ need to create its own
scratch space if you do not provide your own. However, it manages this
perfectly well itself: the outermost call to productKaratsuba() will
create a single block of scratch space for the entire multiplication
process, and will pass this down through to its recursive calls to
productBest() and productKaratsuba().

Precondition:
    Both *lhsLen* and *rhsLen* are strictly positive.

Precondition:
    The input range lengths are not too unbalanced. Specifically, if
    we let *minLen* and *maxLen* denote the smaller and larger of
    *lhsLen* and *rhsLen*, then ``2 * minLen > max + 1``.

Precondition:
    If *scratch* is non-null, then it should point to a C-style array
    of size at least ``2(max-1 + log_2(max-1))``, where *max* denotes
    the larger of *lhsLen* and *rhsLen*.

Template parameter ``T``:
    the coefficient type.

Template parameter ``operation``:
    indicates whether the output coefficients should be written via
    assignment (``dest[i] = result``), addition (``dest[i] +=
    result``), or whichever is fastest (which is only appropriate when
    every cofficient in *dest* has previously been initialised to
    zero).

Template parameter ``moveable``:
    ``True`` if we are allowed to move data out of the input
    coefficients, or ``False`` if the input coefficients should be
    read-only.

Parameter ``dest``:
    the C-style array in which to write the output coefficients.

Parameter ``lhs``:
    the C-style array holding the first range of input coefficients.

Parameter ``lhsLen``:
    the total number of coefficients in the first input range.

Parameter ``rhs``:
    the C-style array holding the second range of input coefficients.

Parameter ``rhsLen``:
    the total number of coefficients in the second input range.

Parameter ``scratch``:
    pre-allocated scratch space, or ``None`` if the product algorithm
    should create its own scratch space. The scratch space does not
    need to be initialised in any particular way.)doc";

struct SetOrAdd {

// Docstring regina::python::doc::detail::SetOrAdd::Add
static constexpr const char Add[] =
R"doc(Indicates that the result should be added to the destination object.
This is used (for example) by ``x.addProduct(y, z)``.)doc";

// Docstring regina::python::doc::detail::SetOrAdd::Either
static constexpr const char Either[] =
R"doc(Indicates that the result may be _either_ assigned to or added to the
destination object, whichever is fastest. This is typically used when
assignment is required, but the destination object is known to have
been initialised to zero.)doc";

// Docstring regina::python::doc::detail::SetOrAdd::Set
static constexpr const char Set[] =
R"doc(Indicates that the result should be assigned to the destination
object. This is used (for example) by ``x = y * z``.)doc";

// Docstring regina::python::doc::detail::SetOrAdd::__class
static constexpr const char __class[] =
R"doc(Indicate whether the result of some computation should be added to or
assigned to a destination object. Regina uses this internally when
working with polynomial coefficients (in particular, for implementing
various polynomial product algorithms).)doc";

}; // struct SetOrAdd

} // namespace detail

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

