
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

#endif

