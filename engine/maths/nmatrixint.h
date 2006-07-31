
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

#ifndef __NMATRIXINT_H
#ifndef __DOXYGEN
#define __NMATRIXINT_H
#endif

/*! \file nmatrixint.h
 *  \brief Deals with matrices of arbitrary precision integers.
 */

#include "shareableobject.h"
#include "utilities/nmpi.h"
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
 * \ifacespython Most inherited member functions are implemented.
 * Exceptions are noted in the documentation for each individual member
 * function.
 */
class NMatrixInt : public NMatrixRing<NLargeInteger>, public ShareableObject {
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
	 * Computes the determinant. Returns 0 if matrix is not square.
	 * What should it return for a 0 by 0 matrix? probably 0 makes the most sense.
	 *
	 * @author Ryan Budney
	 */
	NLargeInteger det() const;

        virtual void writeTextShort(std::ostream& out) const;
        virtual void writeTextLong(std::ostream& out) const;
};

// Inline functions for NMatrixInt

inline NMatrixInt::NMatrixInt(unsigned long rows, unsigned long cols) :
        NMatrixRing<NLargeInteger>(rows, cols), ShareableObject() {
}
inline NMatrixInt::NMatrixInt(const NMatrixInt& cloneMe) :
        NMatrixRing<NLargeInteger>(cloneMe), ShareableObject() {
}

inline void NMatrixInt::writeTextShort(std::ostream& out) const {
    out << nRows << " x " << nCols << " integer matrix";
}
inline void NMatrixInt::writeTextLong(std::ostream& out) const {
    writeMatrix(out);
}

/*@}*/

} // namespace regina

#endif

