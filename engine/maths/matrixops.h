
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#ifndef __REGINA_MATRIXOPS_H
#ifndef __DOXYGEN
#define __REGINA_MATRIXOPS_H
#endif

/*! \file maths/matrixops.h
 *  \brief Provides various complex matrix calculations.
 *  \todo \featurelong Add a routine to find the rank of an integer
 *  matrix; use this to show the rank of the matching equations.
 */

#include "regina-core.h"
#include "maths/matrix.h"
#include <vector>

namespace regina {

/**
 * Transforms the given integer matrix into Smith normal form.
 * Note that the given matrix need not be square and need not be of full
 * rank.
 *
 * Reading down the diagonal, the final Smith normal form will have a
 * series of non-negative, non-decreasing invariant factors followed by
 * zeroes.  "Invariant factor" refers to the convention that the <i>i</i>th
 * term divides the (<i>i</i>+1)th term, and so they are unique.
 *
 * The algorithm used is due to Hafner and McCurley (1991).
 * It does not use modular arithmetic to control the intermediate
 * coefficient explosion.
 *
 * \param matrix the matrix to transform.
 *
 * \ingroup maths
 */
void smithNormalForm(MatrixInt& matrix);

/**
 * A Smith normal form algorithm that also returns change of basis matrices.
 *
 * This is a modification of the one-argument smithNormalForm(MatrixInt&).
 * As well as converting the given matrix \a matrix into Smith normal form,
 * it also returns the appropriate change-of-basis matrices corresponding
 * to all the row and column operations that were performed.
 *
 * The only input argument is \a matrix.  The four remaining arguments
 * (the change of basis matrices) will be refilled.
 * All five arguments are used to return information as follows.
 *
 * Let \a M be the initial value of \a matrix, and let \a S be the Smith
 * normal form of \a M.  After this routine exits:
 *
 * - The argument \a matrix will contain the Smith normal form \a S;
 * - `colSpaceBasis * M * rowSpaceBasis = S`;
 * - `colSpaceBasisInv * S * rowSpaceBasisInv = M`;
 * - `colSpaceBasis * colSpaceBasisInv` and
 *   `rowSpaceBasis * rowSpaceBasisInv` are both identity matrices.
 *
 * Thus, one obtains the Smith normal form of the original matrix by multiplying
 * on the left by ColSpaceBasis and on the right by RowSpaceBasis.
 *
 * The matrices \a rowSpaceBasis and \a rowSpaceBasisInv that are passed
 * may be of any size, or they may even be uninitialised; upon return they
 * will both be square with side length matrix.columns().
 * Likewise, the matrices \a colSpaceBasis and \a colSpaceBasisInv that are
 * passed may be of any size or may be uninitialised; upon return they
 * will both be square with side length matrix.rows().
 *
 * \param matrix the original matrix to put into Smith Normal Form (this
 * need not be square).  When the algorithm terminates, this matrix _is_
 * in its Smith Normal Form.
 * \param rowSpaceBasis used to return a change of basis matrix (see
 * above for details).
 * \param rowSpaceBasisInv used to return the inverse of \a rowSpaceBasis.
 * \param colSpaceBasis used to return a change of basis matrix (see
 * above for details).
 * \param colSpaceBasisInv used to return the inverse of \a colSpaceBasis.
 *
 * \ingroup maths
 */
void smithNormalForm(MatrixInt& matrix,
        MatrixInt& rowSpaceBasis, MatrixInt& rowSpaceBasisInv,
        MatrixInt& colSpaceBasis, MatrixInt& colSpaceBasisInv);

/**
 * An alternative Smith normal form algorithm that also returns change of
 * basis matrices.  This routine may be preferable for extremely large
 * matrices.  This is a variant of Hafner-McCurley and Havas-Holt-Rees's
 * description of pivoting methods.
 *
 * The only input argument is \a matrix.  The four remaining arguments
 * (the change of basis matrices) will be refilled.
 * All five arguments are used to return information as follows.
 *
 * Let \a M be the initial value of \a matrix, and let \a S be the Smith
 * normal form of \a M.  After this routine exits:
 *
 * - The argument \a matrix will contain the Smith normal form \a S;
 * - `colSpaceBasis * M * rowSpaceBasis = S`;
 * - `colSpaceBasisInv * S * rowSpaceBasisInv = M`;
 * - `colSpaceBasis * colSpaceBasisInv` and
 *   `rowSpaceBasis * rowSpaceBasisInv` are both identity matrices.
 *
 * Thus, one obtains the Smith normal form the original matrix by multiplying
 * on the left by ColSpaceBasis and on the right by RowSpaceBasis.
 *
 * The matrices \a rowSpaceBasis and \a rowSpaceBasisInv that are passed
 * may be of any size, or they may even be uninitialised; upon return they
 * will both be square with side length matrix.columns().
 * Likewise, the matrices \a colSpaceBasis and \a colSpaceBasisInv that are
 * passed may be of any size or may be uninitialised; upon return they
 * will both be square with side length matrix.rows().
 *
 * \param matrix the original matrix to put into Smith Normal Form (this
 * need not be square).  When the algorithm terminates, this matrix _is_
 * in its Smith Normal Form.
 * \param rowSpaceBasis used to return a change of basis matrix (see
 * above for details).
 * \param rowSpaceBasisInv used to return the inverse of \a rowSpaceBasis.
 * \param colSpaceBasis used to return a change of basis matrix (see
 * above for details).
 * \param colSpaceBasisInv used to return the inverse of \a colSpaceBasis.
 *
 * \author Ryan Budney
 *
 * \ingroup maths
 */
void metricalSmithNormalForm(MatrixInt& matrix,
        MatrixInt& rowSpaceBasis, MatrixInt& rowSpaceBasisInv,
        MatrixInt& colSpaceBasis, MatrixInt& colSpaceBasisInv);

/**
 * Find a basis for the row space of the given matrix.
 *
 * This routine will rearrange the rows of the given matrix so that the
 * first \a rank rows form a basis for the row space (where \a rank is
 * the rank of the matrix).  The rank itself will be returned.  No other
 * changes will be made to the matrix aside from swapping rows.
 *
 * Although this routine takes an integer matrix (and only uses integer
 * operations), we consider the row space to be over the _rationals_.
 * That is, although we never divide, we act as though we could if we
 * wanted to.
 *
 * \param matrix the matrix to examine and rearrange.
 * \return the rank of the given matrix.
 *
 * \ingroup maths
 */
size_t rowBasis(MatrixInt& matrix);

/**
 * Finds a basis for the row space of the given matrix, as well as an
 * "incremental" basis for its orthogonal complement.
 *
 * This routine takes an (\a r by \a c) matrix \a input, as well as a
 * square (\a c by \a c) matrix \a complement, and does the following:
 *
 * - The rows of \a input are rearranged so that the first \a rank rows form
 *   a basis for the row space (where \a rank is the rank of the matrix).
 *   No other changes are made to this matrix aside from swapping rows.
 *
 * - The matrix \a complement is re-filled (any previous contents are
 *   thrown away) so that, for any \a i between 0 and \a rank-1 inclusive,
 *   the final (\a c - \a i) rows of \a complement form a basis for the
 *   orthogonal complement of the first \a i rows of the rearranged \a input.
 *
 * - The rank of the matrix \a input is returned from this routine.
 *
 * This routine can help with larger procedures that need to build up a row
 * space and simultaneously cut down the complement one dimension at a time.
 *
 * Although this routine takes integer matrices (and only uses integer
 * operations), we consider all bases to be over the _rationals_.
 * That is, although we never divide, we act as though we could if we
 * wanted to.
 *
 * \pre The matrix \a complement is a square matrix, whose size is equal
 * to the number of columns in \a input.
 *
 * \exception InvalidArgument The matrix \a complement is not square with side
 * length equal to `input.columns()`.
 *
 * \param input the input matrix whose row space we will describe; this
 * matrix will be changed (though only by swapping rows).
 * \param complement the square matrix that will be re-filled with the
 * "incremental" basis for the orthogonal complement of \a input.
 * \return the rank of the given matrix \a input.
 *
 * \ingroup maths
 */
size_t rowBasisAndOrthComp(MatrixInt& input, MatrixInt& complement);

/**
 * Transforms a given matrix into column echelon form with respect to a
 * collection of rows.  The transformation will perform only column operations.
 *
 * Given the matrix \a M and the list \a rowList of rows from \a M, this
 * algorithm puts \a M in column echelon form with respect to the rows
 * in \a rowList.  The only purpose of \a rowList is to clarify and/or
 * weaken precisely what is meant by "column echelon form"; all rows of
 * \a M are affected by the resulting column operations that take place.
 *
 * This routine also returns the corresponding change of coordinate
 * matrices \a R and \a Ri:
 *
 * - The matrix \a R will have precisely the same column operations applied to
 *   it as the matrix \a M.  The matrix \a Ri will have the inverse _row_
 *   operations applied to it, thereby maintaining a constant value of the
 *   product `R * Ri` as the algorithm runs.
 *
 * - In particular, if \a R and \a Ri are passed into this routine as
 *   square identity matrices, then after the reduction is complete we will
 *   have `original_M * R = final_M` and `final_M * Ri = original_M`.
 *
 * Our convention is that a matrix is in column echelon form if:
 *
 * - each column is either zero or there is a first non-zero entry which
 *   is positive (but see the note regarding \a rowList below);
 *
 * - moving from the leftmost column to the rightmost column, the rows
 *   containing the first non-zero entries for these columns have strictly
 *   increasing indices in \a rowList;
 *
 * - given a first non-zero column entry, in that row all the elements to
 *   the left are smaller and non-negative (all elements to the right are
 *   already zero by the previous condition);
 *
 * - all the zero columns are on the right hand side of the matrix.
 *
 * By a "zero column" here we simply mean "zero for every row in \a
 * rowList".  Likewise, by "first non-zero entry" we mean "first row in
 * \a rowList with a non-zero entry".
 *
 * In a pinch, you can also use this routine to compute the inverse of an
 * invertible square matrix.
 *
 * If you just wish to reduce the matrix, you do not care about the order of
 * rows, and you do not want the change-of-basis matrices, then you should
 * call MatrixInt::columnEchelonForm() instead, which is simpler but also more
 * streamlined.
 *
 * \pre If \a n is the number of _columns_ in \a M, then \a R has precisely
 * \a n columns also, and \a Ri has precisely \a n rows.
 *
 * \exception InvalidArgument Either `R.columns() ≠ M.columns()`, and/or
 * `Ri.rows() ≠ M.columns()`.
 *
 * \param M the matrix to reduce.
 * \param R used to return the row-reduction matrix, as described above.
 * \param Ri used to return the inverse of \a R.
 * \param rowList the rows to pay attention to.  This list must contain
 * distinct integers, all between 0 and M.rows()-1 inclusive (though it
 * need not contain _all_ of these integers).  The integers may appear in
 * any order (though changing the order will change the resulting column
 * echelon form).  For a "classical" column echelon form, this would be the
 * list of all rows: `0,...,(M.rows()-1)`.
 *
 * \author Ryan Budney
 *
 * \ingroup maths
 */
void columnEchelonForm(MatrixInt &M, MatrixInt &R, MatrixInt &Ri,
        const std::vector<size_t> &rowList);

/**
 * Given a homomorphism from Z^n to Z^k and a sublattice of Z^k,
 * compute the preimage of this sublattice under this homomorphism.
 *
 * The homomorphism from Z^n to Z^k is described by the given
 * \a k by \a n matrix \a hom.  The sublattice is of the form
 * `(p1 Z) * (p2 Z) * ... * (pk Z)`, where the non-negative integers
 * \a p1, ..., \a pk are passed in the given list \a sublattice.
 *
 * An equivalent problem is to consider \a hom to be a homomorphism
 * from Z^n to Z_p1 + ... + Z_pk; this routine then finds the kernel
 * of this homomorphism.
 *
 * The preimage of the sublattice (equivalently, the kernel described
 * above) is some rank \a n lattice in Z^n.  This algorithm finds and
 * returns a basis for the lattice.
 *
 * \exception InvalidArgument The length of \a sublattice is different from
 * the number of rows of \a hom.  Note that the _contents_ of \a sublattice
 * (specifically, the signs of the integers it contains) are not checked.
 *
 * \param hom the matrix representing the homomorphism from Z^n to Z^k;
 * this must be a \a k by \a n matrix.
 * \param sublattice a list of length \a k describing the sublattice of Z^k;
 * the elements of this list must be the non-negative integers
 * \a p1, ..., \a pk as described above.
 * \return a new matrix whose columns are a basis for the preimage lattice.
 * This matrix will have precisely \a n rows.
 *
 * \author Ryan Budney
 *
 * \ingroup maths
 */
MatrixInt preImageOfLattice(const MatrixInt& hom,
        const std::vector<Integer>& sublattice);

/**
 * Given an automorphism of an abelian group,
 * this procedure computes the inverse automorphism.
 *
 * The abelian group is of the form `Z_p1 + Z_p2 + ... + Z_pn`.
 * The input is an n-by-n matrix \a A which represents a lift of the
 * automorphism to just some n-by-n matrix.  Specifically, you have a little
 * commutative diagram with `Z^n --A--> Z^n` covering the automorphism
 * of `Z_p1 + Z_p2 + ... + Z_pn`, where the maps down are the direct
 * sum of the standard quotients `Z --> Z_pi`.  So if you want this
 * procedure to give you meaningful output, \a A must be a lift of a genuine
 * automorphism of `Z_p1 + ... + Z_pn`.
 *
 * \pre The list p1, p2, ..., pn is a list of invariant factors,
 * which means that p1|p2, ..., p{n-1}|pn.
 *
 * \exception InvalidArgument Either \a input is not a square matrix, and/or
 * the length of \a invF is different from the side length of \a input.
 * Note that the _contents_ of \a invF (specifically, the divisibility
 * property) are not checked.
 *
 * \param input the n-by-n matrix \a A, which must be a lift of a genuine
 * automorphism as described above.
 * \param invF the list p1, p2, ..., pn.
 * \return the inverse automorphism, also described as an n-by-n matrix
 * as per the discussion above.
 *
 * \author Ryan Budney
 *
 * \ingroup maths
 */
MatrixInt torsionAutInverse(const MatrixInt& input,
    const std::vector<Integer> &invF);

} // namespace regina

#endif

