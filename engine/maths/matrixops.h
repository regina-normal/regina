
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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
 * \testpartial
 *
 * @param matrix the matrix to transform.
 */
void smithNormalForm(NMatrixInt& matrix);

/**
 * A Smith normal form algorithm that also returns change of basis matrices.
 *
 * This is a modification of smithNormalForm(NMatrixInt&).  It returns
 * the appropriate change-of-basis matrices corresponding to all the
 * row and column operations performed in the process of constructing the
 * Smith normal form.  In particular,
 * <tt>ColSpaceBasis * original_matrix * RowSpaceBasis = final_matrix</tt>
 * (which is in Smith normal form).
 *
 * When the algorithm terminates, the original matrix is equal to
 * ColSpaceBasisInv * matrix * RowSpaceBasisInv.
 * thus, one obtains matrix from the old matrix by multiplying on the
 * left by ColSpaceBasis and on the right by RowSpaceBasis.
 *
 * The four change of basis matrices will be reset on entry to the
 * function; their original values will be ignored.
 *
 * @param matrix is the original m x n matrix to put into Smith Normal Form.
 * When the algorithm terminates, it *is* in its Smith Normal Form.
 * @param rowSpaceBasis change of basis matrix; this must be square with
 * size matrix.columns().
 * @param rowSpaceBasisInv inverse of RowSpaceBasis; this must also be
 * square with size matrix.columns().
 * @param colSpaceBasis change of basis matrix; this must be square with
 * size matrix.rows().
 * @param colSpaceBasisInv invase of ColSpaceBasis; this must also be
 * square with size matrix.rows().
 *
 * \author Ryan Budney
 */
void smithNormalForm(NMatrixInt& matrix,
        NMatrixInt& rowSpaceBasis, NMatrixInt& rowSpaceBasisInv,
        NMatrixInt& colSpaceBasis, NMatrixInt& colSpaceBasisInv);

/**
 * Puts a given matrix into column echelon form with respect to a
 * collection of rows.
 *
 * Given the matrix \a M and the list \a rowList of rows from \a M, this
 * algorithm puts \a M in column echelon form with respect to the
 * given list of rows.
 *
 * This routine also returns the corresponding change of coordinate
 * matrices \a R and \a Ri:
 *
 * - If \a R and \a Ri are passed as identity matrices, the returned
 *   matrices will be such that <tt>original_M * R = final_M</tt> and
 *   <tt>final_M * Ri = original_M</tt> (and of course <tt>final_M</tt> is
 *   in column echelon form with respect to the given rows).
 * - If \a R and \a Ri are already non-trivial coordinate transformations,
 *   they are modified appropriately by the algorithm.
 *
 * Our convention is that a matrix is in column echelon form if:
 *
 * -# each column is either zero or there is a first non-zero entry which
 *    is positive;
 * -# from left to right, the first non-zero entries have strictly increasing
 *    indices (i.e., row numbers);
 * -# given a first non-zero column entry, in that row all the elements to
 *    the left are smaller and non-negative (all elements to the right are
 *    already zero by the previous condition);
 * -# in a row which has no first non-zero column entry, all elements are zero;
 * -# thus all the zero columns are on the right hand side of the matrix.
 *
 * \pre Both \a R and \a Ri are square matrices with dimensions equal to
 * M.columns(), and these matrices are inverses of each other.
 *
 * @param M matrix to reduce
 * @param R row-reduction matrix
 * @param Ri the inverse of R
 * @param rowList the rows to pay attention to.
 *
 * \author Ryan Budney
 */
void columnEchelonForm(NMatrixInt &M, NMatrixInt &R, NMatrixInt &Ri,
        const std::vector<unsigned> &rowList);

/**
 * Given a homomorphism (hom) from Z^n to Z^m and a sublattice of Z^m
 * represented by L, this procedure computes the preimage of L under hom.
 *
 * Given a homomorphism from Z^n to Z_p1 + ... + Z_pk, the kernel
 * of this homomorphism is some rank-n lattice in Z^n.  This algorithm
 * finds a basis for the lattice.  hom is a k by n matrix representing
 * the homomorphism and L is a k-vector whose entries are p1 through pk.
 * A basis is returned.
 *
 * @param hom the matrix representing the homomorphism from Z^n to Z^m
 * @param L the sublattice of Z^m
 * @return a matrix whose columns are a basis for the preimage lattice.
 *
 * \author Ryan Budney
 */
NMatrixInt preImageOfLattice(const NMatrixInt& hom,
        const std::vector<NLargeInteger>& L);

/*@}*/

} // namespace regina

#endif

