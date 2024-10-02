/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Matrix2
static const char *Matrix2 =
R"doc(Represents a 2-by-2 integer matrix. The advantage of using this class
over the larger Matrix class template (e.g., MatrixInt) is that this
class has less overhead.

This class only contains four long integers, and so it may be
considered small enough to pass about by value.

This class supports copying but does not implement separate move
operations, since its internal data is so small that copying is just
as efficient. It implements the C++ Swappable requirement via its own
member and global swap() functions, for consistency with the Matrix
classes.)doc";

// Docstring regina::python::doc::simpler
static const char *simpler =
R"doc(Determines whether the first given matrix is more aesthetically
pleasing than the second. The way in which this judgement is made is
purely aesthetic on the part of the author, and is subject to change
in future versions of Regina.

Parameter ``m1``:
    the first matrix to examine.

Parameter ``m2``:
    the second matrix to examine.

Returns:
    ``True`` if *m1* is deemed to be more pleasing than *m2*, or
    ``False`` if either the matrices are equal or *m2* is more
    pleasing than *m1*.)doc";

// Docstring regina::python::doc::simpler_2
static const char *simpler_2 =
R"doc(Determines whether the first given pair of matrices is more
aesthetically pleasing than the second pair. The way in which this
judgement is made is purely aesthetic on the part of the author, and
is subject to change in future versions of Regina.

Note that pairs are ordered, so the pair (*M*, *N*) may be more (or
perhaps less) pleasing than the pair (*N*, *M*).

Parameter ``pair1first``:
    the first matrix of the first pair to examine.

Parameter ``pair1second``:
    the second matrix of the first pair to examine.

Parameter ``pair2first``:
    the first matrix of the second pair to examine.

Parameter ``pair2second``:
    the second matrix of the second pair to examine.

Returns:
    ``True`` if the first pair is deemed to be more pleasing than the
    second pair, or ``False`` if either the ordered pairs are equal or
    the second pair is more pleasing than the first.)doc";

namespace Matrix2_ {

// Docstring regina::python::doc::Matrix2_::__add
static const char *__add =
R"doc(Calculates the sum of two matrices. Neither this nor the given matrix
is changed.

Parameter ``other``:
    the matrix to add to this.

Returns:
    the sum *this* + *other*.)doc";

// Docstring regina::python::doc::Matrix2_::__array
static const char *__array =
R"doc(Gives read-only access to a single row of this matrix.

This means that the integer in row *r*, column *c* can be accessed as
``myMatrix[r][c]`` (where *r* and *c* are each 0 or 1).

Parameter ``row``:
    the index of the requested row; this must be 0 or 1.

Returns:
    a two-integer array containing the elements of the requested row.)doc";

// Docstring regina::python::doc::Matrix2_::__array_2
static const char *__array_2 =
R"doc(Gives read-write access to a single row of this matrix.

This means that the integer in row *r*, column *c* can be accessed as
``myMatrix[r][c]`` (where *r* and *c* are each 0 or 1). Each such
element may be modified directly.

Parameter ``row``:
    the index of the requested row; this must be 0 or 1.

Returns:
    a reference to the two-integer array containing the elements of
    the requested row.)doc";

// Docstring regina::python::doc::Matrix2_::__copy
static const char *__copy = R"doc(Initialises to a copy of the given matrix.)doc";

// Docstring regina::python::doc::Matrix2_::__default
static const char *__default = R"doc(Initialises to the zero matrix.)doc";

// Docstring regina::python::doc::Matrix2_::__eq
static const char *__eq =
R"doc(Determines if this is equal to the given matrix.

Parameter ``compare``:
    the matrix with which this will be compared.

Returns:
    ``True`` if and only if this matrix is equal to *compare*.)doc";

// Docstring regina::python::doc::Matrix2_::__iadd
static const char *__iadd =
R"doc(Adds the given matrix to this. This matrix is changed to reflect the
result.

Parameter ``other``:
    the matrix to add to this.

Returns:
    a reference to this matrix with its new value.)doc";

// Docstring regina::python::doc::Matrix2_::__imul
static const char *__imul =
R"doc(Multiplies this by the given matrix. This matrix is changed to reflect
the result.

Parameter ``other``:
    the matrix by which this should be multiplied.

Returns:
    a reference to this matrix with its new value.)doc";

// Docstring regina::python::doc::Matrix2_::__imul_2
static const char *__imul_2 =
R"doc(Multiplies this by the given scalar. This matrix is changed to reflect
the result.

Parameter ``scalar``:
    the scalar by which this should be multiplied.

Returns:
    a reference to this matrix with its new value.)doc";

// Docstring regina::python::doc::Matrix2_::__init
static const char *__init =
R"doc(Initialises to the given integer values.

Parameter ``val00``:
    the value to place in row 0, column 0.

Parameter ``val01``:
    the value to place in row 0, column 1.

Parameter ``val10``:
    the value to place in row 1, column 0.

Parameter ``val11``:
    the value to place in row 1, column 1.)doc";

// Docstring regina::python::doc::Matrix2_::__isub
static const char *__isub =
R"doc(Subtracts the given matrix from this. This matrix is changed to
reflect the result.

Parameter ``other``:
    the matrix to subtract from this.

Returns:
    a reference to this matrix with its new value.)doc";

// Docstring regina::python::doc::Matrix2_::__mul
static const char *__mul =
R"doc(Calculates the matrix product of this and the given matrix. Neither
this nor the given matrix is changed.

Parameter ``other``:
    the matrix that this should be multiplied by.

Returns:
    the product *this* * *other*.)doc";

// Docstring regina::python::doc::Matrix2_::__mul_2
static const char *__mul_2 =
R"doc(Calculates the scalar product of this matrix and the given integer.
This matrix is not changed.

Parameter ``scalar``:
    the integer that this matrix should be multiplied by.

Returns:
    the product *this* * *scalar*.)doc";

// Docstring regina::python::doc::Matrix2_::__sub
static const char *__sub =
R"doc(Calculates the difference of two matrices. Neither this nor the given
matrix is changed.

Parameter ``other``:
    the matrix to subtract from this.

Returns:
    the difference *this* - *other*.)doc";

// Docstring regina::python::doc::Matrix2_::__sub_2
static const char *__sub_2 =
R"doc(Determines the negative of this matrix. This matrix is not changed.

Returns:
    the negative of this matrix.)doc";

// Docstring regina::python::doc::Matrix2_::determinant
static const char *determinant =
R"doc(Returns the determinant of this matrix.

Returns:
    the determinant of this matrix.)doc";

// Docstring regina::python::doc::Matrix2_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the two given matrices.

This global routine simply calls Matrix2::swap(); it is provided so
that Matrix2 meets the C++ Swappable requirements.

Parameter ``a``:
    the first matrix whose contents should be swapped.

Parameter ``b``:
    the second matrix whose contents should be swapped.)doc";

// Docstring regina::python::doc::Matrix2_::inverse
static const char *inverse =
R"doc(Calculates the inverse of this matrix. This matrix is not changed.

This routine only works for integer matrices whose determinant is
either +1 or -1.

Returns:
    the inverse of this matrix. If this matrix does not have
    determinant +1 or -1, the zero matrix will be returned instead.)doc";

// Docstring regina::python::doc::Matrix2_::invert
static const char *invert =
R"doc(Inverts this matrix.

This routine only works for integer matrices whose determinant is
either +1 or -1. Otherwise this matrix is left unchanged.

Returns:
    ``True`` if this matrix was successfully inverted (i.e., its
    determinant was +1 or -1), or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Matrix2_::isIdentity
static const char *isIdentity =
R"doc(Determines if this is the 2-by-2 identity matrix.

Returns:
    ``True`` if this is the identity matrix, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Matrix2_::isZero
static const char *isZero =
R"doc(Determines if this is the 2-by-2 zero matrix.

Returns:
    ``True`` if this is the zero matrix, or ``False`` otherwise.)doc";

// Docstring regina::python::doc::Matrix2_::negate
static const char *negate = R"doc(Negates this matrix. This matrix is changed to reflect the result.)doc";

// Docstring regina::python::doc::Matrix2_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given matrix.

Parameter ``other``:
    the matrix whose contents should be swapped with this.)doc";

// Docstring regina::python::doc::Matrix2_::transpose
static const char *transpose =
R"doc(Returns the transpose of this matrix. This matrix is not changed.

Returns:
    the transpose of this matrix.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

