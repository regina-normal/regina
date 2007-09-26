
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
 * @param matrix the matrix to transform.
 */
void smithNormalForm(NMatrixInt& matrix);

/**
 * These are intended to be internal routines, although they could be used for
 * other purposes. RBMOD_smithNormalForm is a modification of the
 * smithNormalForm algorithm, it returns the appropriate change-of-basis
 * matrices corresponding to all the row and column operations performed in
 * the process of constructing the smithNormalForm.  RBADD_columnEchelonForm
 * puts a matrix into reduced column echelon form with respect to a certain
 * submatrix specified by rowList. This is used in the RBADD_preImageOfLattice
 * algorithm, which computes the kernel of a homomorphism
 * Z^n --> Z_p1 + Z_p2 + ... + Z_pk specified by a matrix.
 *
 * @param matrix is the original m x n matrix to put into Smith Normal Form.
 * When the algorithm terminates, it *is* in its Smith Normal Form.
 * @param RowSpaceBasis change of basis matrix
 * @param RowSpaceBasisInv inverse of RowSpaceBasis
 * @param ColSpaceBasis change of basis matrix
 * @param ColSpaceBasisInv invase of ColSpaceBasis
 * when the algorithm terminates, the original matrix is equal to
 * ColSpaceBasisInv * matrix * RowSpaceBasisInv.
 * thus, one obtains matrix from the old matrix by multiplying on the
 * left by ColSpaceBasis and on the right by RowSpaceBasis.
 *
 * \author Ryan Budney
 */
void RBMOD_smithNormalForm(NMatrixInt& matrix,
        NMatrixInt& RowSpaceBasis, NMatrixInt& RowSpaceBasisInv,
        NMatrixInt& ColSpaceBasis, NMatrixInt& ColSpaceBasisInv);

/**
 * Modification of RBMOD_smithNormalForm to only put the matrix in Column
 * Echelon form with respect to a collection of rows.
 *
 * @param M matrix to reduce
 * @param R row-reduction matrix
 * @param Ri the inverse of R
 * @param rowList the rows to pay attention to.
 *
 * \author Ryan Budney
 */
void RBADD_columnEchelonForm(NMatrixInt &M, NMatrixInt &R, NMatrixInt &Ri,
        const std::vector<unsigned> &rowList);

/**
 * Given a homomorphism (hom) from Z^n to Z^m and a sublattice of Z^m
 * represented by L, this procedure computes the preimage of L under hom.
 *
 * @param hom the matrix representing the homomorphism from Z^n to Z^m
 * @param L the sublattice of Z^m
 * @return a matrix whose columns are a basis for the preimage lattice.
 *
 * \author Ryan Budney
 */
NMatrixInt RBADD_preImageOfLattice(NMatrixInt& hom,
        std::vector<NLargeInteger>& L);

/*@}*/

} // namespace regina

#endif

