
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#ifndef __NMATRIXINT_H
#ifndef __DOXYGEN
#define __NMATRIXINT_H
#endif

/*! \file maths/nmatrixint.h
 *  \brief Deals with matrices of arbitrary precision integers.
 */

#include "regina-core.h"
#include "maths/ninteger.h"
#include "maths/nmatrix.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * Represents a matrix of arbitrary precision integers.
 * Calculations will be exact no matter how large the integers become.
 *
 * Note that many important functions (such as entry()) are inherited
 * from the superclasses NMatrix and NMatrixRing, and are not documented
 * again here.  Many other algorithms that work on the NMatrixInt class
 * are available in the maths/matrixops.h file. 
 *
 * \ifacespython Most inherited member functions are implemented.
 * Exceptions are noted in the documentation for each individual member
 * function.
 */
class REGINA_API NMatrixInt : public NMatrixRing<NLargeInteger> {
    public:
        /**
         * Creates a new matrix of the given size.
         * All entries will be initialised to zero.
         *
         * \pre The given number of rows and columns are
         * both strictly positive.
         *
         * @param rows the number of rows in the new matrix.
         * @param cols the number of columns in the new matrix.
         */
        NMatrixInt(unsigned long rows, unsigned long cols);
        /**
         * Creates a new matrix that is a clone of the given matrix.
         *
         * @param cloneMe the matrix to clone.
         */
        NMatrixInt(const NMatrixInt& cloneMe);

        /**
         * Divides all elements of the given row by the given integer.
         * This can only be used when the given integer divides into all
         * row elements exactly (with no remainder), and is much faster
         * than ordinary division.
         *
         * \pre The argument \a divBy is neither zero nor infinity, and
         * none of the elements of the given row are infinity.
         * \pre The argument \a divBy divides exactly into every element
         * of the given row (i.e., it leaves no remainder).
         * \pre The given row number is between 0 and rows()-1 inclusive.
         *
         * @param row the index of the row whose elements should be
         * divided by \a divBy.
         * @param divBy the integer to divide each row element by.
         */
        void divRowExact(unsigned long row, const NLargeInteger& divBy) {
            for (NLargeInteger* x = this->data[row];
                    x != this->data[row] + nCols; ++x)
                x->divByExact(divBy);
        }

        /**
         * Divides all elements of the given column by the given integer.
         * This can only be used when the given integer divides into all
         * column elements exactly (with no remainder), and is much faster
         * than ordinary division.
         *
         * \pre The argument \a divBy is neither zero nor infinity, and
         * none of the elements of the given column are infinity.
         * \pre The argument \a divBy divides exactly into every element
         * of the given column (i.e., it leaves no remainder).
         * \pre The given column number is between 0 and columns()-1 inclusive.
         *
         * @param col the index of the column whose elements should be
         * divided by \a divBy.
         * @param divBy the integer to divide each column element by.
         */
        void divColExact(unsigned long col, const NLargeInteger& divBy) {
            for (NLargeInteger** row = this->data; row != this->data + nRows;
                    ++row)
                (*row)[col].divByExact(divBy);
        }

        /**
         * Computes the greatest common divisor of all elements of the
         * given row.  The value returned is guaranteed to be non-negative.
         *
         * \pre The given row number is between 0 and rows()-1 inclusive.
         *
         * @param row the index of the row whose gcd should be computed.
         * @return the greatest common divisor of all elements of this row.
         */
        NLargeInteger gcdRow(unsigned long row) {
            NLargeInteger* x = this->data[row];

            NLargeInteger gcd = *x++;
            while (x != this->data[row] + nCols && gcd != 1 && gcd != -1)
                gcd = gcd.gcd(*x++);

            if (gcd < 0)
                gcd.negate();
            return gcd;
        }

        /**
         * Computes the greatest common divisor of all elements of the
         * given column.  The value returned is guaranteed to be non-negative.
         *
         * \pre The given column number is between 0 and columns()-1 inclusive.
         *
         * @param col the index of the column whose gcd should be computed.
         * @return the greatest common divisor of all elements of this column.
         */
        NLargeInteger gcdCol(unsigned long col) {
            NLargeInteger** row = this->data;

            NLargeInteger gcd = (*row++)[col];
            while (row != this->data + nRows && gcd != 1 && gcd != -1)
                gcd = gcd.gcd((*row++)[col]);

            if (gcd < 0)
                gcd.negate();
            return gcd;
        }

        /**
         * Reduces the given row by dividing all its elements by their
         * greatest common divisor.  It is guaranteed that, if the row is
         * changed at all, it will be divided by a \e positive integer.
         *
         * \pre The given row number is between 0 and rows()-1 inclusive.
         *
         * @param row the index of the row to reduce.
         */
        void reduceRow(unsigned long row) {
            NLargeInteger gcd = gcdRow(row);
            if (gcd != NLargeInteger::zero && gcd != NLargeInteger::one)
                divRowExact(row, gcd);
        }

        /**
         * Reduces the given column by dividing all its elements by their
         * greatest common divisor.  It is guaranteed that, if the column is
         * changed at all, it will be divided by a \e positive integer.
         *
         * \pre The given column number is between 0 and columns()-1 inclusive.
         *
         * @param col the index of the column to reduce.
         */
        void reduceCol(unsigned long col) {
            NLargeInteger gcd = gcdCol(col);
            if (gcd != NLargeInteger::zero && gcd != NLargeInteger::one)
                divColExact(col, gcd);
        }
};

// Inline functions for NMatrixInt

inline NMatrixInt::NMatrixInt(unsigned long rows, unsigned long cols) :
        NMatrixRing<NLargeInteger>(rows, cols) {
}
inline NMatrixInt::NMatrixInt(const NMatrixInt& cloneMe) :
        NMatrixRing<NLargeInteger>(cloneMe) {
}

/*@}*/

} // namespace regina

#endif

