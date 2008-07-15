
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#ifndef __MATRIXOPS_H
#ifndef __DOXYGEN
#define __MATRIXOPS_H
#endif

/*! \file matrixops.h
 *  \brief Provides various complex matrix calculations.
 *  \todo \featurelong Add a routine to find the rank of an integer
 *  matrix; use this to show the rank of the matching equations.
 */

#include "maths/nmatrixint.h"
#include <vector>

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Transforms the given integer matrix into Smith normal form.
 * Note that the given matrix need not be square and need not be of full
 * rank.
 *
 * Reading down the diagonal, the final Smith normal form will have a
 * series of non-negative, non-decreasing invariant factors followed by
 * zeroes.
 *
 * The algorithm used is due to Hafner and McCurley (1991).
 * It does not use modular arithmetic to control the intermediate
 * coefficient explosion.
 *
 * \testpart
 *
 * @param matrix the matrix to transform.
 */
void smithNormalForm(NMatrixInt& matrix);

/**
 * A Smith normal form algorithm that also returns change of basis matrices.
 *
 * This is a modification of the one-argument smithNormalForm(NMatrixInt&).
 * As well as converting the given matrix \a matrix into Smith normal form,
 * it also returns the appropriate change-of-basis matrices corresponding
 * to all the row and column operations that were performed.
 *
 * The only input argument is \a matrix.  The four remaining arguments
 * (the change of basis matrices) will be refilled, though they must be
 * constructed with the correct dimensions as seen in the preconditions
 * below.  All five arguments are used to return information as follows.
 *
 * Let \a M be the initial value of \a matrix, and let \a S be the Smith
 * normal form of \a M.  After this routine exits:
 *
 * - The argument \a matrix will contain the Smith normal form \a S;
 * - <tt>colSpaceBasis * M * rowSpaceBasis = S</tt>;
 * - <tt>colSpaceBasisInv * S * rowSpaceBasisInv = M</tt>;
 * - <tt>colSpaceBasis * colSpaceBasisInv</tt> and
 *   <tt>rowSpaceBasis * rowSpaceBasisInv</tt> are both identity matrices.
 *
 * Thus, one obtains the Smith normal form the original matrix by multiplying
 * on the left by ColSpaceBasis and on the right by RowSpaceBasis.
 *
 * \pre The matrices \a rowSpaceBasis and \a rowSpaceBasisInv that are
 * passed are square, with side length matrix.columns().
 * \pre The matrices \a colSpaceBasis and \a colSpaceBasisInv that are
 * passed are square, with side length matrix.rows().
 *
 * \testpart
 *
 * @param matrix the original matrix to put into Smith Normal Form (this
 * need not be square).  When the algorithm terminates, this matrix \e is
 * in its Smith Normal Form.
 * @param rowSpaceBasis used to return a change of basis matrix (see
 * above for details).
 * @param rowSpaceBasisInv used to return the inverse of \a rowSpaceBasis.
 * @param colSpaceBasis used to return a change of basis matrix (see
 * above for details).
 * @param colSpaceBasisInv used to return the inverse of \a colSpaceBasis.
 *
 * \author Ryan Budney
 */
void smithNormalForm(NMatrixInt& matrix,
        NMatrixInt& rowSpaceBasis, NMatrixInt& rowSpaceBasisInv,
        NMatrixInt& colSpaceBasis, NMatrixInt& colSpaceBasisInv);

/**
 * Find a basis for the row space of the given matrix.
 *
 * This routine will rearrange the rows of the given matrix so that the
 * first \a rank rows form a basis of the row space.  The rank itself
 * will be returned.  No other changes will be made to the matrix aside
 * from swapping rows.
 *
 * Although this routine takes an integer matrix (and only uses integer
 * operations), we consider the row space to be over the \e rationals.
 * That is, although we never divide, we act as though we could if we
 * wanted to.
 *
 * @param the matrix to examine and rearrange.
 * @return the rank of the given matrix.
 */
unsigned rowBasis(NMatrixInt& matrix);

/**
 * Transforms a given matrix into column echelon form with respect to a
 * collection of rows.
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
 * - If \a R and \a Ri are passed as identity matrices, the returned
 *   matrices will be such that <tt>original_M * R = final_M</tt> and
 *   <tt>final_M * Ri = original_M</tt> (and of course <tt>final_M</tt> is
 *   in column echelon form with respect to the given row list).
 * - If \a R and \a Ri are already non-trivial coordinate transformations,
 *   they are modified appropriately by the algorithm.
 *
 * Our convention is that a matrix is in column echelon form if:
 *
 * -# each column is either zero or there is a first non-zero entry which
 *    is positive (but see the note regarding \a rowList below);
 * -# moving from the leftmost column to the rightmost column, the rows
 *    containing the first non-zero entries for these columns have strictly
 *    increasing indices in \a rowList;
 * -# given a first non-zero column entry, in that row all the elements to
 *    the left are smaller and non-negative (all elements to the right are
 *    already zero by the previous condition);
 * -# all the zero columns are on the right hand side of the matrix.
 *
 * By a "zero column" here we simply mean "zero for every row in \a
 * rowList".  Likewise, by "first non-zero entry" we mean "first row in
 * \a rowList with a non-zero entry".
 *
 * \pre Both \a R and \a Ri are square matrices with side length M.columns(),
 * and these matrices are inverses of each other.
 *
 * \ifacespython The argument \a rowList should be supplied as a python list.
 *
 * @param M the matrix to reduce.
 * @param R used to return the row-reduction matrix, as described above.
 * @param Ri used to return the inverse of \a R.
 * @param rowList the rows to pay attention to.  This list must contain
 * distinct integers, all between 0 and M.rows()-1 inclusive.  The
 * integers may appear in any order (though changing the order will
 * change the resulting column echelon form).
 *
 * \author Ryan Budney
 */
void columnEchelonForm(NMatrixInt &M, NMatrixInt &R, NMatrixInt &Ri,
        const std::vector<unsigned> &rowList);

/**
 * Given a homomorphism from Z^n to Z^k and a sublattice of Z^k,
 * compute the preimage of this sublattice under this homomorphism.
 *
 * The homomorphism from Z^n to Z^k is described by the given
 * \a k by \a n matrix \a hom.  The sublattice is of the form
 * <tt>(p1 Z) * (p2 Z) * ... * (pk Z)</tt>, where the non-negative integers
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
 * \ifacespython The argument \a sublattice should be supplied as a python list.
 *
 * @param hom the matrix representing the homomorphism from Z^n to Z^k;
 * this must be a \a k by \a n matrix.
 * @param sublattice a list of length \a k describing the sublattice of Z^k;
 * the elements of this list must be the non-negative integers
 * \a p1, ..., \a pk as described above.
 * @return a new matrix whose columns are a basis for the preimage lattice.
 * This matrix will have precisely \a n rows.
 *
 * \author Ryan Budney
 */
std::auto_ptr<NMatrixInt> preImageOfLattice(const NMatrixInt& hom,
        const std::vector<NLargeInteger>& sublattice);

/*@}*/

} // namespace regina

#endif

