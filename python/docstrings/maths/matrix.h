/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::Matrix
static const char *Matrix =
R"doc(Represents a matrix of elements of the given type *T*.

As of Regina 5.96, the old subclasses of Matrix have now been merged
into a single Matrix class. The additional member functions that the
old subclasses MatrixRing and MatrixIntDomain used to provide are now
part of Matrix, and are enabled or disabled according to the Matrix
template parameters.

It is generally safe to just use the type Matrix<T>, since the
``ring`` argument has a sensible default. At present, ``ring``
defaults to ``True`` (thereby enabling member functions designed for
matrices over rings) when *T* is one of the following types:

* native C++ integer types (i.e., where std::is_integral_v<T> is
  ``True`` and *T* is not bool);

* native C++ floating-point types (i.e., where
  std::is_floating_point_v<T> is ``True``); or

* Regina's own types Integer, LargeInteger, NativeInteger<...>, and
  Rational.

Other types may be added to this list in future versions of Regina.

There are several requirements for the underlying type *T*. For all
matrix types:

* *T* must have a default constructor and an assignment operator.

* An element *t* of type *T* must be writable to an output stream
  using the standard stream operator ``<<``.

If *ring* is ``True``, then in addition to this:

* *T* must support binary operators ``+``, ``-`` and ``*``, and unary
  operators ``+=``, ``-=`` and ``*=``.

* *T* must be able to be constructed or assigned to from the integers
  0 and 1 (representing the additive and multiplicative identities in
  the ring respectively). Likewise, *T* must be able to be tested for
  equality or inequality against 0 or 1 also.

In particular, all of Regina's integer and rational types (Integer,
LargeInteger, NativeInteger<...> and Rational) satisfy all of these
requirements, and will set *ring* to ``True`` by default.

The header maths/matrixops.h contains several other algorithms that
work with the specific class Matrix<Integer>.

This class implements C++ move semantics and adheres to the C++
Swappable requirement. It is designed to avoid deep copies wherever
possible, even when passing or returning objects by value.

Python:
    Only the specific types Matrix<Integer>, Matrix<bool> and
    Matrix<double> are available, under the names MatrixInt,
    MatrixBool and MatrixReal respectively.

Template parameter ``T``:
    the type of each individual matrix element.

Template parameter ``ring``:
    ``True`` if we should enable member functions that only work when
    T represents an element of a ring. This has a sensible default;
    see above in the class documentation for details.)doc";

namespace Matrix_ {

// Docstring regina::python::doc::Matrix_::__copy
static const char *__copy =
R"doc(Creates a new matrix that is a clone of the given matrix.

This constructor induces a deep copy of *src*.

This routine is safe to call even if *src* is uninitialised (in which
case this matrix will become uninitialised also).

Parameter ``src``:
    the matrix to clone.)doc";

// Docstring regina::python::doc::Matrix_::__eq
static const char *__eq =
R"doc(Determines whether this and the given matrix are identical.

Two matrices are identical if and only if (i) their dimensions are the
same, and (ii) the corresponding elements of each matrix are equal.

Note that this routine can happily deal with two matrices of different
dimensions (in which case it will always return ``False``).

This routine returns ``True`` if and only if the inequality operator
(!=) returns ``False``.

Precondition:
    The type *T* provides an equality operator (==).

Parameter ``other``:
    the matrix to compare with this.

Returns:
    ``True`` if the matrices are equal as described above, or
    ``False`` otherwise.)doc";

// Docstring regina::python::doc::Matrix_::__init
static const char *__init =
R"doc(Creates a new square matrix of the given size. Both the number of rows
and the number of columns will be set to *size*.

All entries will be initialised using their default constructors. In
particular, this means that for Regina's own integer classes (Integer,
LargeInteger and NativeInteger), all entries will be initialised to
zero.

.. warning::
    If *T* is a native C++ integer type (such as ``int`` or ``long``),
    then the matrix elements will not be initialised to any particular
    value.

Precondition:
    The given size is strictly positive.

Parameter ``size``:
    the number of rows and columns in the new matrix.)doc";

// Docstring regina::python::doc::Matrix_::__init_2
static const char *__init_2 =
R"doc(Creates a new matrix of the given size.

All entries will be initialised using their default constructors. In
particular, this means that for Regina's own integer classes (Integer,
LargeInteger and NativeInteger), all entries will be initialised to
zero.

.. warning::
    If *T* is a native C++ integer type (such as ``int`` or ``long``),
    then the matrix elements will not be initialised to any particular
    value.

Precondition:
    The given number of rows and columns are both strictly positive.

Parameter ``rows``:
    the number of rows in the new matrix.

Parameter ``cols``:
    the number of columns in the new matrix.)doc";

// Docstring regina::python::doc::Matrix_::__init_3
static const char *__init_3 =
R"doc(Creates a new matrix containing the given hard-coded entries. This
constructor can be used (for example) to create hard-coded examples
directly in C++ code.

Each element of the initialiser list *data* describes a single row of
the matrix.

Precondition:
    The list *data* is non-empty (i.e., the number of rows is
    positive), and each of its elements is non-empty (i.e., the number
    of columns is positive).

Precondition:
    All elements of *data* (representing the rows of the matrix) are
    lists of the same size.

Python:
    The argument *data* should be a Python list of Python lists.

Parameter ``data``:
    the rows of the matrix, each given as a list of elements.)doc";

// Docstring regina::python::doc::Matrix_::__mul
static const char *__mul =
R"doc(Multiplies this by the given matrix, and returns the result. This
matrix is not changed.

The two matrices being multiplied may use different underlying types
(e.g., you can multiply a matrix of LargeInteger objects with a matrix
of native C++ long integers). The type of object that is stored in the
resulting matrix will be deduced accordingly (specifically, it will be
the type obtained by multiplying objects of types *T* and *U* using
the binary multiplication operator).

Precondition:
    The template argument *ring* is ``True``.

Precondition:
    The number of columns in this matrix equals the number of rows in
    the given matrix.

Parameter ``other``:
    the other matrix to multiply this matrix by.

Returns:
    the product matrix ``this * other``.)doc";

// Docstring regina::python::doc::Matrix_::__mul_2
static const char *__mul_2 =
R"doc(Multiplies this matrix by the given vector, and returns the result.
The given vector is treated as a column vector.

The matrix and vector may use different underlying types (e.g., you
can multiply a matrix of LargeInteger objects with a vector of native
C++ long integers). The type of object that is stored in the resulting
vector will be deduced accordingly (specifically, it will be the type
obtained by multiplying objects of types *T* and *U* using the binary
multiplication operator).

Precondition:
    The template argument *ring* is ``True``.

Precondition:
    The length of the given vector is precisely the number of columns
    in this matrix.

Parameter ``other``:
    the vector to multiply this matrix by.

Returns:
    the product ``this * other``, which will be a vector whose length
    is the number of rows in this matrix.)doc";

// Docstring regina::python::doc::Matrix_::__ne
static const char *__ne =
R"doc(Determines whether this and the given matrix are different.

Two matrices are different if either (i) their dimensions differ, or
(ii) the corresponding elements of each matrix differ in at least one
location.

Note that this routine can happily deal with two matrices of different
dimensions (in which case it will always return ``True``).

This routine returns ``True`` if and only if the equality operator
(==) returns ``False``.

Precondition:
    The type *T* provides an equality operator (==).

Parameter ``other``:
    the matrix to compare with this.

Returns:
    ``True`` if the matrices are different as described above, or
    ``False`` otherwise.)doc";

// Docstring regina::python::doc::Matrix_::addCol
static const char *addCol =
R"doc(Adds the given source column to the given destination column.

.. warning::
    If you only wish to add a portion of a column, be careful: you
    cannot just pass the usual *fromRow* argument, since this will be
    interpreted as a coefficient to be used with the other version of
    addCol() that adds _several_ copies of the source column. Instead
    you will need to call addColFrom().

Precondition:
    The template argument *ring* is ``True``.

Precondition:
    The two given columns are distinct and between 0 and columns()-1
    inclusive.

Parameter ``source``:
    the columns to add.

Parameter ``dest``:
    the column that will be added to.)doc";

// Docstring regina::python::doc::Matrix_::addCol_2
static const char *addCol_2 =
R"doc(Adds the given number of copies of the given source column to the
given destination column.

Note that *copies* is passed by value in case it is an element of the
row to be changed.

If the optional argument *fromRow* is passed, then the operation will
only be performed for the elements from that row down to the bottom of
the column (inclusive).

Precondition:
    The template argument *ring* is ``True``.

Precondition:
    The two given columns are distinct and between 0 and columns()-1
    inclusive.

Precondition:
    If passed, *fromRow* is between 0 and rows() -1 inclusive.

Parameter ``source``:
    the columns to add.

Parameter ``dest``:
    the column that will be added to.

Parameter ``copies``:
    the number of copies of *source* to add to *dest*.

Parameter ``fromRow``:
    the starting point in the column from which the operation will be
    performed.)doc";

// Docstring regina::python::doc::Matrix_::addColFrom
static const char *addColFrom =
R"doc(Adds a portion of the given source column to the given destination
column.

This is similar to addCol(), except that the operation will only be
performed for the elements from the row *fromRow* down to the bottom
of the column (inclusive).

Precondition:
    The template argument *ring* is ``True``.

Precondition:
    The two given columns are distinct and between 0 and columns()-1
    inclusive.

Precondition:
    If passed, *fromRow* is between 0 and rows() -1 inclusive.

Parameter ``source``:
    the columns to add.

Parameter ``dest``:
    the column that will be added to.

Parameter ``fromRow``:
    the starting point in the column from which the operation will be
    performed.)doc";

// Docstring regina::python::doc::Matrix_::addRow
static const char *addRow =
R"doc(Adds the given source row to the given destination row.

Precondition:
    The template argument *ring* is ``True``.

Precondition:
    The two given rows are distinct and between 0 and rows()-1
    inclusive.

.. warning::
    If you only wish to add a portion of a row, be careful: you cannot
    just pass the usual *fromCol* argument, since this will be
    interpreted as a coefficient to be used with the other version of
    addRow() that adds _several_ copies of the source row. Instead you
    will need to call addRowFrom().

Parameter ``source``:
    the row to add.

Parameter ``dest``:
    the row that will be added to.)doc";

// Docstring regina::python::doc::Matrix_::addRow_2
static const char *addRow_2 =
R"doc(Adds the given number of copies of the given source row to the given
destination row.

Note that *copies* is passed by value in case it is an element of the
row to be changed.

If the optional argument *fromCol* is passed, then the operation will
only be performed for the elements from that column to the rightmost
end of the row (inclusive).

Precondition:
    The template argument *ring* is ``True``.

Precondition:
    The two given rows are distinct and between 0 and rows()-1
    inclusive.

Precondition:
    If passed, *fromCol* is between 0 and columns() -1 inclusive.

Parameter ``source``:
    the row to add.

Parameter ``dest``:
    the row that will be added to.

Parameter ``copies``:
    the number of copies of *source* to add to *dest*.

Parameter ``fromCol``:
    the starting point in the row from which the operation will be
    performed.)doc";

// Docstring regina::python::doc::Matrix_::addRowFrom
static const char *addRowFrom =
R"doc(Adds a portion of the given source row to the given destination row.

This is similar to addRow(), except that the operation will only be
performed for the elements from the column *fromCol* to the rightmost
end of the row (inclusive).

Precondition:
    The template argument *ring* is ``True``.

Precondition:
    The two given rows are distinct and between 0 and rows()-1
    inclusive.

Precondition:
    If passed, *fromCol* is between 0 and columns() -1 inclusive.

Parameter ``source``:
    the row to add.

Parameter ``dest``:
    the row that will be added to.

Parameter ``fromCol``:
    the starting point in the row from which the operation will be
    performed.)doc";

// Docstring regina::python::doc::Matrix_::columnEchelonForm
static const char *columnEchelonForm =
R"doc(Transforms this matrix into column echelon form. The transformation
will perform only column operations.

This is simpler than the global routine regina::columnEchelonForm():
it does not return the change of basis matrices, and it processes all
rows in order from left to right (instead of passing a custom row
list).

Our convention is that a matrix is in column echelon form if:

* each column is either zero or there is a first non-zero entry which
  is positive;

* moving from the left column to the right, these first non-zero
  entries have strictly increasing row indices;

* for each first non-zero column entry, in that row all the elements
  to the left are smaller and non-negative (and all elements to the
  right are already zero by the previous condition);

* all the zero columns are at the right hand end of the matrix.

Precondition:
    Type *T* is one of Regina's own integer classes (Integer,
    LargeInteger, or NativeIntgeger).

Returns:
    the rank of this matrix, i.e., the number of non-zero columns
    remaining.)doc";

// Docstring regina::python::doc::Matrix_::columns
static const char *columns =
R"doc(Returns the number of columns in this matrix.

Returns:
    the number of columns.)doc";

// Docstring regina::python::doc::Matrix_::combCols
static const char *combCols =
R"doc(Rewrites two columns as linear combinations of those two columns.

Specifically, if *C1* and *C2* are the original values of columns
*col1* and *col2* respectively, then:

* Column *col1* will become ``coeff11 * C1 + coeff12 * C2``;

* Column *col2* will become ``coeff21 * C1 + coeff22 * C2``.

The four coefficients are passed by value, in case they are elements
of the columns to be changed.

If the optional argument *fromRow* is passed, then the operation will
only be performed for the elements from that column down to the bottom
of each column (inclusive).

Precondition:
    The template argument *ring* is ``True``.

Precondition:
    The two given columns are distinct and between 0 and columns()-1
    inclusive.

Precondition:
    If passed, *fromCol* is between 0 and columns() -1 inclusive.

Parameter ``col1``:
    the first column to operate on.

Parameter ``col2``:
    the second column to operate on.

Parameter ``coeff11``:
    the coefficient of column *col1* to use when rewriting column
    *col1*.

Parameter ``coeff12``:
    the coefficient of column *col2* to use when rewriting column
    *col1*.

Parameter ``coeff21``:
    the coefficient of column *col1* to use when rewriting column
    *col2*.

Parameter ``coeff22``:
    the coefficient of column *col2* to use when rewriting column
    *col2*.

Parameter ``fromRow``:
    the starting point in the columns from which the operation will be
    performed.)doc";

// Docstring regina::python::doc::Matrix_::combRows
static const char *combRows =
R"doc(Rewrites two rows as linear combinations of those two rows.

Specifically, if *R1* and *R2* are the original values of rows *row1*
and *row2* respectively, then:

* Row *row1* will become ``coeff11 * R1 + coeff12 * R2``;

* Row *row2* will become ``coeff21 * R1 + coeff22 * R2``.

The four coefficients are passed by value, in case they are elements
of the rows to be changed.

If the optional argument *fromCol* is passed, then the operation will
only be performed for the elements from that column to the rightmost
end of each row (inclusive).

Precondition:
    The template argument *ring* is ``True``.

Precondition:
    The two given rows are distinct and between 0 and rows()-1
    inclusive.

Precondition:
    If passed, *fromCol* is between 0 and columns() -1 inclusive.

Parameter ``row1``:
    the first row to operate on.

Parameter ``row2``:
    the second row to operate on.

Parameter ``coeff11``:
    the coefficient of row *row1* to use when rewriting row *row1*.

Parameter ``coeff12``:
    the coefficient of row *row2* to use when rewriting row *row1*.

Parameter ``coeff21``:
    the coefficient of row *row1* to use when rewriting row *row2*.

Parameter ``coeff22``:
    the coefficient of row *row2* to use when rewriting row *row2*.

Parameter ``fromCol``:
    the starting point in the rows from which the operation will be
    performed.)doc";

// Docstring regina::python::doc::Matrix_::det
static const char *det =
R"doc(Evaluates the determinant of the matrix.

This algorithm has quartic complexity, and uses the dynamic
programming approach of Mahajan and Vinay. For further details, see
Meena Mahajan and V. Vinay, "Determinant: Combinatorics, algorithms,
and complexity", Chicago J. Theor. Comput. Sci., Vol. 1997, Article 5.

Although the Matrix class does not formally support empty matrices, if
this _is_ found to be a 0-by-0 matrix then the determinant returned
will be 1.

Precondition:
    The template argument *ring* is ``True``.

Precondition:
    This is a square matrix.

Exception ``FailedPrecondition``:
    This matrix is not square.

Returns:
    the determinant of this matrix.)doc";

// Docstring regina::python::doc::Matrix_::divColExact
static const char *divColExact =
R"doc(Divides all elements of the given column by the given integer. This
can only be used when the given integer divides into all column
elements exactly (with no remainder). For the Integer class, this may
be much faster than ordinary division.

Precondition:
    Type *T* is one of Regina's own integer classes (Integer,
    LargeInteger, or NativeIntgeger).

Precondition:
    The argument *divBy* is neither zero nor infinity, and none of the
    elements of the given column are infinity.

Precondition:
    The argument *divBy* divides exactly into every element of the
    given column (i.e., it leaves no remainder).

Precondition:
    The given column number is between 0 and columns()-1 inclusive.

Parameter ``col``:
    the index of the column whose elements should be divided by
    *divBy*.

Parameter ``divBy``:
    the integer to divide each column element by.)doc";

// Docstring regina::python::doc::Matrix_::divRowExact
static const char *divRowExact =
R"doc(Divides all elements of the given row by the given integer. This can
only be used when the given integer divides into all row elements
exactly (with no remainder). For the Integer class, this may be much
faster than ordinary division.

Precondition:
    Type *T* is one of Regina's own integer classes (Integer,
    LargeInteger, or NativeIntgeger).

Precondition:
    The argument *divBy* is neither zero nor infinity, and none of the
    elements of the given row are infinity.

Precondition:
    The argument *divBy* divides exactly into every element of the
    given row (i.e., it leaves no remainder).

Precondition:
    The given row number is between 0 and rows()-1 inclusive.

Parameter ``row``:
    the index of the row whose elements should be divided by *divBy*.

Parameter ``divBy``:
    the integer to divide each row element by.)doc";

// Docstring regina::python::doc::Matrix_::entry
static const char *entry =
R"doc(Returns a read-write reference to the entry at the given row and
column. Rows and columns are numbered beginning at zero.

Python:
    In general, to assign values to matrix elements you should use the
    Python-only set() routine. This entry() routine does give read-
    write access to matrix elements in Python, but it does not allow
    them to be set using the assignment operator. In other words, code
    such as ``matrix.entry(r, c).negate()`` will work, but
    ``matrix.entry(r, c) = value`` will not; instead you will need to
    call ``matrix.set(r, c, value)``.

Parameter ``row``:
    the row of the desired entry; this must be between 0 and rows()-1
    inclusive.

Parameter ``column``:
    the column of the desired entry; this must be between 0 and
    columns()-1 inclusive.

Returns:
    a reference to the entry in the given row and column.)doc";

// Docstring regina::python::doc::Matrix_::entry_2
static const char *entry_2 =
R"doc(Returns a read-only reference to the entry at the given row and
column. Rows and columns are numbered beginning at zero.

Parameter ``row``:
    the row of the desired entry; this must be between 0 and rows()-1
    inclusive.

Parameter ``column``:
    the column of the desired entry; this must be between 0 and
    columns()-1 inclusive.

Returns:
    a reference to the entry in the given row and column.)doc";

// Docstring regina::python::doc::Matrix_::fill
static const char *fill =
R"doc(Sets every entry in the matrix to the given value.

Parameter ``value``:
    the value to assign to each entry.)doc";

// Docstring regina::python::doc::Matrix_::gcdCol
static const char *gcdCol =
R"doc(Computes the greatest common divisor of all elements of the given
column. The value returned is guaranteed to be non-negative.

Precondition:
    Type *T* is one of Regina's own integer classes (Integer,
    LargeInteger, or NativeIntgeger).

Precondition:
    The given column number is between 0 and columns()-1 inclusive.

Parameter ``col``:
    the index of the column whose gcd should be computed.

Returns:
    the greatest common divisor of all elements of this column.)doc";

// Docstring regina::python::doc::Matrix_::gcdRow
static const char *gcdRow =
R"doc(Computes the greatest common divisor of all elements of the given row.
The value returned is guaranteed to be non-negative.

Precondition:
    Type *T* is one of Regina's own integer classes (Integer,
    LargeInteger, or NativeIntgeger).

Precondition:
    The given row number is between 0 and rows()-1 inclusive.

Parameter ``row``:
    the index of the row whose gcd should be computed.

Returns:
    the greatest common divisor of all elements of this row.)doc";

// Docstring regina::python::doc::Matrix_::global_swap
static const char *global_swap =
R"doc(Swaps the contents of the given matrices.

This global routine simply calls Matrix<T>::swap(); it is provided so
that Matrix<T> meets the C++ Swappable requirements.

Parameter ``a``:
    the first matrix whose contents should be swapped.

Parameter ``b``:
    the second matrix whose contents should be swapped.)doc";

// Docstring regina::python::doc::Matrix_::identity
static const char *identity =
R"doc(Returns an identity matrix of the given size. The matrix returned will
have *size* rows and *size* columns.

Precondition:
    The template argument *ring* is ``True``.

Parameter ``size``:
    the number of rows and columns of the matrix to build.

Returns:
    an identity matrix of the given size.)doc";

// Docstring regina::python::doc::Matrix_::initialise
static const char *initialise =
R"doc(Deprecated function that sets every entry in the matrix to the given
value.

.. deprecated::
    This routine has been renamed to fill(), to make it clear that it
    has nothing to do with initialised versus uninitialised matrices.

Parameter ``value``:
    the value to assign to each entry.)doc";

// Docstring regina::python::doc::Matrix_::initialised
static const char *initialised =
R"doc(Determines whether this matrix is initialised or uninitialised.

The only ways for a matrix to be _uninitialised_ are:

* it was created using the default constructor, and has not yet been
  initialised using the assignment operator;

* it was the result of assignment or copy construction from some other
  uninitialised matrix.

Returns:
    ``True`` if this matrix is initialised, or ``False`` if it is
    uninitialised.)doc";

// Docstring regina::python::doc::Matrix_::isIdentity
static const char *isIdentity =
R"doc(Determines whether this matrix is a square identity matrix.

If this matrix is square, isIdentity() will return ``True`` if and
only if the matrix has ones in the main diagonal and zeroes everywhere
else.

If this matrix is not square, isIdentity() will always return
``False`` (even if makeIdentity() was called earlier).

Precondition:
    The template argument *ring* is ``True``.

Returns:
    ``True`` if and only if this is a square identity matrix.)doc";

// Docstring regina::python::doc::Matrix_::isZero
static const char *isZero =
R"doc(Determines whether this is the zero matrix.

Precondition:
    The template argument *ring* is ``True``.

Returns:
    ``True`` if and only if all entries in the matrix are zero.)doc";

// Docstring regina::python::doc::Matrix_::makeIdentity
static const char *makeIdentity =
R"doc(Turns this matrix into an identity matrix. This matrix need not be
square; after this routine it will have ``entry(r,c)`` equal to 1 if
``r == c`` and 0 otherwise.

Precondition:
    The template argument *ring* is ``True``.)doc";

// Docstring regina::python::doc::Matrix_::multCol
static const char *multCol =
R"doc(Multiplies the given column by the given factor.

Note that *factor* is passed by value in case it is an element of the
row to be changed.

If the optional argument *fromRow* is passed, then the operation will
only be performed for the elements from that row down to the bottom of
the column (inclusive).

Precondition:
    The template argument *ring* is ``True``.

Precondition:
    The given column is between 0 and columns()-1 inclusive.

Precondition:
    If passed, *fromRow* is between 0 and rows() -1 inclusive.

Parameter ``column``:
    the column to work with.

Parameter ``factor``:
    the factor by which to multiply the given column.

Parameter ``fromRow``:
    the starting point in the column from which the operation will be
    performed.)doc";

// Docstring regina::python::doc::Matrix_::multRow
static const char *multRow =
R"doc(Multiplies the given row by the given factor.

Note that *factor* is passed by value in case it is an element of the
row to be changed.

If the optional argument *fromCol* is passed, then the operation will
only be performed for the elements from that column to the rightmost
end of the row (inclusive).

Precondition:
    The template argument *ring* is ``True``.

Precondition:
    The given row is between 0 and rows()-1 inclusive.

Precondition:
    If passed, *fromCol* is between 0 and columns() -1 inclusive.

Parameter ``row``:
    the row to work with.

Parameter ``factor``:
    the factor by which to multiply the given row.

Parameter ``fromCol``:
    the starting point in the row from which the operation will be
    performed.)doc";

// Docstring regina::python::doc::Matrix_::negateCol
static const char *negateCol =
R"doc(Negates all elements in the given column.

Precondition:
    Type *T* is one of Regina's own integer classes (Integer,
    LargeInteger, or NativeIntgeger).

Precondition:
    The given column number is between 0 and columns()-1 inclusive.

Parameter ``col``:
    the index of the column whose elements should be negated.)doc";

// Docstring regina::python::doc::Matrix_::negateRow
static const char *negateRow =
R"doc(Negates all elements in the given row.

Precondition:
    Type *T* is one of Regina's own integer classes (Integer,
    LargeInteger, or NativeIntgeger).

Precondition:
    The given row number is between 0 and rows()-1 inclusive.

Parameter ``row``:
    the index of the row whose elements should be negated.)doc";

// Docstring regina::python::doc::Matrix_::rank
static const char *rank =
R"doc(A non-destructive routine that returns the rank of this matrix whilst
preserving the contents of the matrix.

Normally, a rank computation would involve modifying the matrix
directly (e.g., by converting it to row echelon form). In contrast,
this routine will leave the matrix unchanged. The cost is an extra
deep copy in the implementation.

If your matrix is disposable (i.e., you will never need to use it
again), then it is faster to use the rvalue reference version of this
routine, which will avoid the extra overhead of the deep copy. To do
this, replace ``matrix.rank()`` with ``std::move(matrix).rank()``.

Precondition:
    Type *T* is one of Regina's own integer classes (Integer,
    LargeInteger, or NativeIntgeger).

Python:
    Only the const version of rank() (i.e., this version) is available
    for Python users.

Returns:
    the rank of this matrix.)doc";

// Docstring regina::python::doc::Matrix_::reduceCol
static const char *reduceCol =
R"doc(Reduces the given column by dividing all its elements by their
greatest common divisor. It is guaranteed that, if the column is
changed at all, it will be divided by a _positive_ integer.

Precondition:
    Type *T* is one of Regina's own integer classes (Integer,
    LargeInteger, or NativeIntgeger).

Precondition:
    The given column number is between 0 and columns()-1 inclusive.

Parameter ``col``:
    the index of the column to reduce.)doc";

// Docstring regina::python::doc::Matrix_::reduceRow
static const char *reduceRow =
R"doc(Reduces the given row by dividing all its elements by their greatest
common divisor. It is guaranteed that, if the row is changed at all,
it will be divided by a _positive_ integer.

Precondition:
    Type *T* is one of Regina's own integer classes (Integer,
    LargeInteger, or NativeIntgeger).

Precondition:
    The given row number is between 0 and rows()-1 inclusive.

Parameter ``row``:
    the index of the row to reduce.)doc";

// Docstring regina::python::doc::Matrix_::rowEchelonForm
static const char *rowEchelonForm =
R"doc(Transforms this matrix into row echelon form. The transformation will
perform only row operations.

This is simpler than the global routine regina::columnEchelonForm():
it does not return the change of basis matrices, and it processes all
columns in order from left to right (instead of passing a custom
column list).

Our convention is that a matrix is in row echelon form if:

* each row is either zero or there is a first non-zero entry which is
  positive;

* moving from the top row to the bottom, these first non-zero entries
  have strictly increasing column indices;

* for each first non-zero row entry, in that column all the elements
  above are smaller and non-negative (and all elements below are
  already zero by the previous condition);

* all the zero rows are at the bottom of the matrix.

Precondition:
    Type *T* is one of Regina's own integer classes (Integer,
    LargeInteger, or NativeIntgeger).

Returns:
    the rank of this matrix, i.e., the number of non-zero rows
    remaining.)doc";

// Docstring regina::python::doc::Matrix_::rows
static const char *rows =
R"doc(Returns the number of rows in this matrix.

Returns:
    the number of rows.)doc";

// Docstring regina::python::doc::Matrix_::set
static const char *set =
R"doc(Python-only routine that sets the entry at the given row and column.
Rows and columns are numbered beginning at zero.

Python:
    In general, to assign values to matrix elements you should use the
    syntax ``matrix.set(row, column, value)``. The entry() routine
    does give read-write access to matrix elements in Python, but it
    does not allow them to be set using the assignment operator. In
    other words, code such as ``matrix.entry(r, c).negate()`` will
    work, but ``matrix.entry(r, c) = value`` will not.

Parameter ``row``:
    the row of the entry to set; this must be between 0 and rows()-1
    inclusive.

Parameter ``column``:
    the column of the entry to set; this must be between 0 and
    columns()-1 inclusive.

Parameter ``value``:
    the new entry to place in the given row and column.)doc";

// Docstring regina::python::doc::Matrix_::swap
static const char *swap =
R"doc(Swaps the contents of this and the given matrix.

Parameter ``other``:
    the matrix whose contents are to be swapped with this.)doc";

// Docstring regina::python::doc::Matrix_::swapCols
static const char *swapCols =
R"doc(Swaps the elements of the two given columns in the matrix.

This operation is linear time (unlike swapping rows, which is constant
time).

If the optional argument *fromRow* is passed, then the operation will
only be performed for the elements from that row down to the bottom of
each column (inclusive).

Precondition:
    The two given columns are between 0 and columns()-1 inclusive.

Precondition:
    If passed, *fromRow* is between 0 and rows() -1 inclusive.

Parameter ``first``:
    the first column to swap.

Parameter ``second``:
    the second column to swap.

Parameter ``fromRow``:
    the starting point in each column from which the operation will be
    performed.)doc";

// Docstring regina::python::doc::Matrix_::swapRows
static const char *swapRows =
R"doc(Swaps the elements of the two given rows in the matrix.

This operation is constant time (unlike swapping columns, which is
linear time).

Unlike swapCols(), this operation does not take a *fromCol* argument.
This is because swapping rows is already as fast possible (internally,
just a single pointer swap), and so iterating along only part of the
row would slow the routine down considerably.

Precondition:
    The two given rows are between 0 and rows()-1 inclusive.

Parameter ``first``:
    the first row to swap.

Parameter ``second``:
    the second row to swap.)doc";

// Docstring regina::python::doc::Matrix_::transpose
static const char *transpose =
R"doc(Returns the transpose of this matrix. This matrix is not changed.

Returns:
    the transpose.)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

