/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::columnEchelonForm
static const char *columnEchelonForm =
R"doc(Transforms a given matrix into column echelon form with respect to a
collection of rows. The transformation will perform only column
operations.

Given the matrix *M* and the list *rowList* of rows from *M*, this
algorithm puts *M* in column echelon form with respect to the rows in
*rowList*. The only purpose of *rowList* is to clarify and/or weaken
precisely what is meant by "column echelon form"; all rows of *M* are
affected by the resulting column operations that take place.

This routine also returns the corresponding change of coordinate
matrices *R* and *Ri:*

* The matrix *R* will have precisely the same column operations
  applied to it as the matrix *M*. The matrix *Ri* will have the
  inverse _row_ operations applied to it, thereby maintaining a
  constant value of the product ``R * Ri`` as the algorithm runs.

* In particular, if *R* and *Ri* are passed into this routine as
  square identity matrices, then after the reduction is complete we
  will have ``original_M * R = final_M`` and ``final_M * Ri =
  original_M``.

Our convention is that a matrix is in column echelon form if:

* each column is either zero or there is a first non-zero entry which
  is positive (but see the note regarding *rowList* below);

* moving from the leftmost column to the rightmost column, the rows
  containing the first non-zero entries for these columns have
  strictly increasing indices in *rowList*;

* given a first non-zero column entry, in that row all the elements to
  the left are smaller and non-negative (all elements to the right are
  already zero by the previous condition);

* all the zero columns are on the right hand side of the matrix.

By a "zero column" here we simply mean "zero for every row in
*rowList*". Likewise, by "first non-zero entry" we mean "first row in
*rowList* with a non-zero entry".

In a pinch, you can also use this routine to compute the inverse of an
invertible square matrix.

If you just wish to reduce the matrix, you do not care about the order
of rows, and you do not want the change-of-basis matrices, then you
should call MatrixInt::columnEchelonForm() instead, which is simpler
but also more streamlined.

Precondition:
    If *n* is the number of _columns_ in *M*, then *R* has precisely
    *n* columns also, and *Ri* has precisely *n* rows.

Exception ``InvalidArgument``:
    Either ``R.columns() ≠ M.columns()``, and/or ``Ri.rows() ≠
    M.columns()``.

Parameter ``M``:
    the matrix to reduce.

Parameter ``R``:
    used to return the row-reduction matrix, as described above.

Parameter ``Ri``:
    used to return the inverse of *R*.

Parameter ``rowList``:
    the rows to pay attention to. This list must contain distinct
    integers, all between 0 and M.rows()-1 inclusive (though it need
    not contain _all_ of these integers). The integers may appear in
    any order (though changing the order will change the resulting
    column echelon form). For a "classical" column echelon form, this
    would be the list of all rows: ``0,...,(M.rows()-1)``.

Author:
    Ryan Budney)doc";

// Docstring regina::python::doc::metricalSmithNormalForm
static const char *metricalSmithNormalForm =
R"doc(An alternative Smith normal form algorithm that also returns change of
basis matrices. This routine may be preferable for extremely large
matrices. This is a variant of Hafner-McCurley and Havas-Holt-Rees's
description of pivoting methods.

The only input argument is *matrix*. The four remaining arguments (the
change of basis matrices) will be refilled. All five arguments are
used to return information as follows.

Let *M* be the initial value of *matrix*, and let *S* be the Smith
normal form of *M*. After this routine exits:

* The argument *matrix* will contain the Smith normal form *S*;

* ``colSpaceBasis * M * rowSpaceBasis = S``;

* ``colSpaceBasisInv * S * rowSpaceBasisInv = M``;

* ``colSpaceBasis * colSpaceBasisInv`` and ``rowSpaceBasis *
  rowSpaceBasisInv`` are both identity matrices.

Thus, one obtains the Smith normal form the original matrix by
multiplying on the left by ColSpaceBasis and on the right by
RowSpaceBasis.

The matrices *rowSpaceBasis* and *rowSpaceBasisInv* that are passed
may be of any size, or they may even be uninitialised; upon return
they will both be square with side length matrix.columns(). Likewise,
the matrices *colSpaceBasis* and *colSpaceBasisInv* that are passed
may be of any size or may be uninitialised; upon return they will both
be square with side length matrix.rows().

Parameter ``matrix``:
    the original matrix to put into Smith Normal Form (this need not
    be square). When the algorithm terminates, this matrix _is_ in its
    Smith Normal Form.

Parameter ``rowSpaceBasis``:
    used to return a change of basis matrix (see above for details).

Parameter ``rowSpaceBasisInv``:
    used to return the inverse of *rowSpaceBasis*.

Parameter ``colSpaceBasis``:
    used to return a change of basis matrix (see above for details).

Parameter ``colSpaceBasisInv``:
    used to return the inverse of *colSpaceBasis*.

Author:
    Ryan Budney)doc";

// Docstring regina::python::doc::preImageOfLattice
static const char *preImageOfLattice =
R"doc(Given a homomorphism from Z^n to Z^k and a sublattice of Z^k, compute
the preimage of this sublattice under this homomorphism.

The homomorphism from Z^n to Z^k is described by the given *k* by *n*
matrix *hom*. The sublattice is of the form ``(p1 Z) * (p2 Z) * ... *
(pk Z)``, where the non-negative integers *p1*, ..., *pk* are passed
in the given list *sublattice*.

An equivalent problem is to consider *hom* to be a homomorphism from
Z^n to Z_p1 + ... + Z_pk; this routine then finds the kernel of this
homomorphism.

The preimage of the sublattice (equivalently, the kernel described
above) is some rank *n* lattice in Z^n. This algorithm finds and
returns a basis for the lattice.

Exception ``InvalidArgument``:
    The length of *sublattice* is different from the number of rows of
    *hom*. Note that the _contents_ of *sublattice* (specifically, the
    signs of the integers it contains) are not checked.

Parameter ``hom``:
    the matrix representing the homomorphism from Z^n to Z^k; this
    must be a *k* by *n* matrix.

Parameter ``sublattice``:
    a list of length *k* describing the sublattice of Z^k; the
    elements of this list must be the non-negative integers *p1*, ...,
    *pk* as described above.

Returns:
    a new matrix whose columns are a basis for the preimage lattice.
    This matrix will have precisely *n* rows.

Author:
    Ryan Budney)doc";

// Docstring regina::python::doc::rowBasis
static const char *rowBasis =
R"doc(Find a basis for the row space of the given matrix.

This routine will rearrange the rows of the given matrix so that the
first *rank* rows form a basis for the row space (where *rank* is the
rank of the matrix). The rank itself will be returned. No other
changes will be made to the matrix aside from swapping rows.

Although this routine takes an integer matrix (and only uses integer
operations), we consider the row space to be over the _rationals_.
That is, although we never divide, we act as though we could if we
wanted to.

Parameter ``matrix``:
    the matrix to examine and rearrange.

Returns:
    the rank of the given matrix.)doc";

// Docstring regina::python::doc::rowBasisAndOrthComp
static const char *rowBasisAndOrthComp =
R"doc(Finds a basis for the row space of the given matrix, as well as an
"incremental" basis for its orthogonal complement.

This routine takes an (*r* by *c*) matrix *input*, as well as a square
(*c* by *c*) matrix *complement*, and does the following:

* The rows of *input* are rearranged so that the first *rank* rows
  form a basis for the row space (where *rank* is the rank of the
  matrix). No other changes are made to this matrix aside from
  swapping rows.

* The matrix *complement* is re-filled (any previous contents are
  thrown away) so that, for any *i* between 0 and *rank*-1 inclusive,
  the final (*c* - *i*) rows of *complement* form a basis for the
  orthogonal complement of the first *i* rows of the rearranged
  *input*.

* The rank of the matrix *input* is returned from this routine.

This routine can help with larger procedures that need to build up a
row space and simultaneously cut down the complement one dimension at
a time.

Although this routine takes integer matrices (and only uses integer
operations), we consider all bases to be over the _rationals_. That
is, although we never divide, we act as though we could if we wanted
to.

Precondition:
    The matrix *complement* is a square matrix, whose size is equal to
    the number of columns in *input*.

Exception ``InvalidArgument``:
    The matrix *complement* is not square with side length equal to
    ``input.columns()``.

Parameter ``input``:
    the input matrix whose row space we will describe; this matrix
    will be changed (though only by swapping rows).

Parameter ``complement``:
    the square matrix that will be re-filled with the "incremental"
    basis for the orthogonal complement of *input*.

Returns:
    the rank of the given matrix *input*.)doc";

// Docstring regina::python::doc::smithNormalForm
static const char *smithNormalForm =
R"doc(Transforms the given integer matrix into Smith normal form. Note that
the given matrix need not be square and need not be of full rank.

Reading down the diagonal, the final Smith normal form will have a
series of non-negative, non-decreasing invariant factors followed by
zeroes. "Invariant factor" refers to the convention that the *i*th
term divides the (*i*+1)th term, and so they are unique.

The algorithm used is due to Hafner and McCurley (1991). It does not
use modular arithmetic to control the intermediate coefficient
explosion.

Parameter ``matrix``:
    the matrix to transform.)doc";

// Docstring regina::python::doc::smithNormalForm_2
static const char *smithNormalForm_2 =
R"doc(A Smith normal form algorithm that also returns change of basis
matrices.

This is a modification of the one-argument
smithNormalForm(MatrixInt&). As well as converting the given matrix
*matrix* into Smith normal form, it also returns the appropriate
change-of-basis matrices corresponding to all the row and column
operations that were performed.

The only input argument is *matrix*. The four remaining arguments (the
change of basis matrices) will be refilled. All five arguments are
used to return information as follows.

Let *M* be the initial value of *matrix*, and let *S* be the Smith
normal form of *M*. After this routine exits:

* The argument *matrix* will contain the Smith normal form *S*;

* ``colSpaceBasis * M * rowSpaceBasis = S``;

* ``colSpaceBasisInv * S * rowSpaceBasisInv = M``;

* ``colSpaceBasis * colSpaceBasisInv`` and ``rowSpaceBasis *
  rowSpaceBasisInv`` are both identity matrices.

Thus, one obtains the Smith normal form of the original matrix by
multiplying on the left by ColSpaceBasis and on the right by
RowSpaceBasis.

The matrices *rowSpaceBasis* and *rowSpaceBasisInv* that are passed
may be of any size, or they may even be uninitialised; upon return
they will both be square with side length matrix.columns(). Likewise,
the matrices *colSpaceBasis* and *colSpaceBasisInv* that are passed
may be of any size or may be uninitialised; upon return they will both
be square with side length matrix.rows().

Parameter ``matrix``:
    the original matrix to put into Smith Normal Form (this need not
    be square). When the algorithm terminates, this matrix _is_ in its
    Smith Normal Form.

Parameter ``rowSpaceBasis``:
    used to return a change of basis matrix (see above for details).

Parameter ``rowSpaceBasisInv``:
    used to return the inverse of *rowSpaceBasis*.

Parameter ``colSpaceBasis``:
    used to return a change of basis matrix (see above for details).

Parameter ``colSpaceBasisInv``:
    used to return the inverse of *colSpaceBasis*.)doc";

// Docstring regina::python::doc::torsionAutInverse
static const char *torsionAutInverse =
R"doc(Given an automorphism of an abelian group, this procedure computes the
inverse automorphism.

The abelian group is of the form ``Z_p1 + Z_p2 + ... + Z_pn``. The
input is an n-by-n matrix *A* which represents a lift of the
automorphism to just some n-by-n matrix. Specifically, you have a
little commutative diagram with ``Z^n --A--> Z^n`` covering the
automorphism of ``Z_p1 + Z_p2 + ... + Z_pn``, where the maps down are
the direct sum of the standard quotients ``Z --> Z_pi``. So if you
want this procedure to give you meaningful output, *A* must be a lift
of a genuine automorphism of ``Z_p1 + ... + Z_pn``.

Precondition:
    The list p1, p2, ..., pn is a list of invariant factors, which
    means that p1|p2, ..., p{n-1}|pn.

Exception ``InvalidArgument``:
    Either *input* is not a square matrix, and/or the length of *invF*
    is different from the side length of *input*. Note that the
    _contents_ of *invF* (specifically, the divisibility property) are
    not checked.

Parameter ``input``:
    the n-by-n matrix *A*, which must be a lift of a genuine
    automorphism as described above.

Parameter ``invF``:
    the list p1, p2, ..., pn.

Returns:
    the inverse automorphism, also described as an n-by-n matrix as
    per the discussion above.

Author:
    Ryan Budney)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

