
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nvectormatrix.h
 *  \brief Deals with vectors that are rows or columns of matrices.
 */

#ifndef __NVECTORMATRIX_H
#ifndef __DOXYGEN
#define __NVECTORMATRIX_H
#endif

#include "maths/nvector.h"
#include "maths/nvectordense.h"
#include "maths/nmatrix.h"

namespace regina {

/**
 * \weakgroup maths
 * @{
 */

/**
 * An exception thrown when a matrix row or column vector is modified.
 * See the NVectorMatrix class notes for why this should never happen.
 */
class NVectorMatrix_Illegal_Modification {
};

/**
 * A vector that corresponds to a row or column of a matrix.
 * Such a vector takes up essentially no storage space, since it merely looks
 * up values in the matrix it references.  Because of this, it should
 * \e never be modified!  The modification routines (such as
 * <tt>=</tt>, <tt>+=</tt> and so on) throw an exception when called.
 * It is recommended to declare any matrix vector as \c const for
 * a safeguard.
 *
 * If the matrix upon which this is vector is based should change, the
 * vector will change accordingly.  If it is destroyed, the vector will
 * become invalid and should not be used.
 *
 * Since a cloned vector might be modified, clone() will
 * return a new NVectorDense instead of a new NVectorMatrix.
 *
 * \pre See the preconditions for NVector<T>.
 *
 * \ifacespython Not present.
 */
template <class T>
class NVectorMatrix : public NVector<T> {
    protected:
        const NMatrix<T>& source;
            /**< The matrix to which this row or column belongs. */
        unsigned rowCol;
            /**< The row or column of the matrix that is this vector. */

    public:
        /**
         * Creates a new vector that refers to the given
         * row or column of the given matrix.
         * The subclass of NVectorMatrix being used will determine if it
         * is a row or a column that we are referencing.
         *
         * \pre The given row or column is between 0 and
         * <tt>matrix.rows() - 1</tt> or <tt>matrix.columns() - 1</tt>
         * respectively (between is inclusive).
         *
         * @param matrix the matrix to which this row or column belongs.
         * @param whichRowCol the row or column of the given matrix that is
         * this vector.
         */
        NVectorMatrix(const NMatrix<T>& matrix, unsigned whichRowCol) :
                source(matrix), rowCol(whichRowCol) {
        }
        virtual NVector<T>* clone() const {
            return new NVectorDense<T>(*this);
        }
        virtual void setElement(unsigned, const T&) {
            throw NVectorMatrix_Illegal_Modification();
        }
        virtual void operator = (const NVector<T>&) {
            throw NVectorMatrix_Illegal_Modification();
        }
        virtual void operator += (const NVector<T>&) {
            throw NVectorMatrix_Illegal_Modification();
        }
        virtual void operator -= (const NVector<T>&) {
            throw NVectorMatrix_Illegal_Modification();
        }
        virtual void operator *= (const T&) {
            throw NVectorMatrix_Illegal_Modification();
        }
        virtual void negate() {
            throw NVectorMatrix_Illegal_Modification();
        }
        virtual void addCopies(const NVector<T>&, const T&) {
            throw NVectorMatrix_Illegal_Modification();
        }
        virtual void subtractCopies(const NVector<T>&, const T&) {
            throw NVectorMatrix_Illegal_Modification();
        }
};

/**
 * A vector that corresponds to a row of a matrix.
 *
 * \pre See the preconditions for NVector<T>.
 *
 * \ifacespython Not present.
 */
template <class T>
class NVectorMatrixRow : public NVectorMatrix<T> {
    public:
        /**
         * Creates a new vector that refers to the given
         * row of the given matrix.
         *
         * \pre The given row is between 0 and
         * <tt>matrix.rows() - 1</tt> inclusive.
         *
         * @param matrix the matrix to which this row belongs.
         * @param whichRow the row of the given matrix that is
         * this vector.
         */
        NVectorMatrixRow(const NMatrix<T>& matrix, unsigned whichRow) :
                NVectorMatrix<T>(matrix, whichRow) {
        }
        virtual unsigned size() const {
            return source.columns();
        }
        virtual const T& operator[](unsigned index) const {
            return source.entry(rowCol, index);
        }
};

/**
 * A vector that corresponds to a column of a matrix.
 *
 * \pre See the preconditions for NVector<T>.
 *
 * \ifacespython Not present.
 */
template <class T>
class NVectorMatrixCol : public NVectorMatrix<T> {
    public:
        /**
         * Creates a new vector that refers to the given
         * column of the given matrix.
         *
         * \pre The given column is between 0 and
         * <tt>matrix.columns() - 1</tt> inclusive.
         *
         * @param matrix the matrix to which this column belongs.
         * @param whichCol the column of the given matrix that is
         * this vector.
         */
        NVectorMatrixCol(const NMatrix<T>& matrix, unsigned whichCol) :
                NVectorMatrix<T>(matrix, whichCol) {
        }
        virtual unsigned size() const {
            return source.rows();
        }
        virtual const T& operator[](unsigned index) const {
            return source.entry(index, rowCol);
        }
};

/*@}*/

} // namespace regina

#endif

