
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

#ifndef __NMATRIXINT_H
#ifndef __DOXYGEN
#define __NMATRIXINT_H
#endif

/*! \file nmatrixint.h
 *  \brief Deals with matrices of arbitrary precision integers.
 */

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "shareableobject.h"
    #include "nmpi.h"
    #include "nmatrix.h"
#else
    #include "engine/shareableobject.h"
    #include "engine/utilities/nmpi.h"
    #include "engine/maths/nmatrix.h"
#endif

/**
 * Represents a matrix of arbitrary precision integers.
 * Calculations will be exact no matter how large the integers become.
 *
 * \ifaces Most inherited member functions are implemented.
 * Exceptions are noted in the documentation for each individual member
 * function documentation; specifically see the notes for the entry() routines.
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

        virtual void writeTextShort(ostream& out) const;
        virtual void writeTextLong(ostream& out) const;
};

// Inline functions for NMatrixInt

inline NMatrixInt::NMatrixInt(unsigned long rows, unsigned long cols) :
        NMatrixRing<NLargeInteger>(rows, cols) {
}
inline NMatrixInt::NMatrixInt(const NMatrixInt& cloneMe) :
        NMatrixRing<NLargeInteger>(cloneMe) {
}

inline void NMatrixInt::writeTextShort(ostream& out) const {
    out << nRows << " x " << nCols << " integer matrix";
}
inline void NMatrixInt::writeTextLong(ostream& out) const {
    writeMatrix(out);
}

#endif

