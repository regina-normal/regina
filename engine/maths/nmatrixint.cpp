
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

/*! \file nmatrixint.cpp
 *  \brief Functions for integer matrices.
 */

#include "utilities/nmpi.h"
#include "maths/nmatrixint.h"

namespace regina {


/** 
 * Computes the determinant. Returns 0 if matrix is not square.
 * What should it return for a 0 by 0 matrix? probably 0 makes the most sense.
 */
NLargeInteger NMatrixInt::det() const
 {

//std::cout<<"Step 1.\n";std::cout.flush();

 NMatrixInt matrix(*this);
 NLargeInteger retval(one);
 if ( (matrix.rows() == matrix.columns()) && (matrix.rows()>0) )
  {
  // we will compute the determinant by putting the matrix in SNF and
  // keeping track of the number of transpositions used in the process.
  unsigned long currStage = 0;
  bool flag;
  unsigned long i, j;
  NLargeInteger d, u, v, a, b, tmp;

//std::cout<<"Step 2.\n";std::cout.flush();

   while ( currStage < matrix.rows() ) 
        { 

//std::cout<<"currStage == "<<currStage<<"\n";std::cout.flush();
//std::cout<<"retval == "<<retval<<"\n";std::cout.flush();

        // Have we got an empty row?
        flag = true;
        for (i=currStage; i<matrix.rows(); i++)
          if (matrix.entry(currStage, i) != 0) { flag = false; break; }
        if (flag) { retval = zero; break; }// Empty row!

        // Have we got an empty column?
        flag = true;
        for (i=currStage; i<matrix.rows(); i++)
          if (matrix.entry(i, currStage) != 0) { flag = false; break; }
        if (flag) { retval = zero; break; }// Empty column!

        // Get zeros in the current row.
        for (i=currStage+1; i<matrix.rows(); i++) 
         {
          if (matrix.entry(currStage, i) == 0) continue;
          // Put a zero in (currStage, i).
          a = matrix.entry(currStage, currStage);
          b = matrix.entry(currStage, i);
          d = a.gcdWithCoeffs(b, u, v);
          a.divByExact(d);
          b.divByExact(d);
          // Do a modification to columns currStage and i.
          // right multiplication by `elementary' matrix
          // [ u  -b ]
          // [ v   a ] whose determinant is 1. 
          for (j=currStage; j<matrix.rows(); j++) 
            {
            tmp = u * matrix.entry(j, currStage) +
                  v * matrix.entry(j, i);
            matrix.entry(j, i) = a * matrix.entry(j, i) -
                  b * matrix.entry(j, currStage);
            matrix.entry(j, currStage) = tmp;
            }
         }

        // Get zeros in the current column.
        flag = false;
        for (i=currStage+1; i<matrix.rows(); i++) 
         {
         if (matrix.entry(i, currStage) == 0) continue;
         flag = true;
         a = matrix.entry(currStage, currStage);
         b = matrix.entry(i, currStage);
         d = a.gcdWithCoeffs(b, u, v);
         a.divByExact(d);
         b.divByExact(d);
         // Do a modification to rows currStage and i.
         for (j=currStage; j<matrix.rows(); j++) 
          {
           tmp = u * matrix.entry(currStage, j) +
                 v * matrix.entry(i, j);
           matrix.entry(i, j) = a * matrix.entry(i, j) -
                 b * matrix.entry(currStage, j);
           matrix.entry(currStage, j) = tmp;
          }
         }

        if (flag) { continue; }

        // Factor out entry (currStage, currStage).
        retval *= matrix.entry(currStage,currStage);

        currStage++;
       }
      } else retval = zero; // for non-square matrices.
return retval;        
}



} // end namespace regina

