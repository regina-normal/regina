
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "algebra/nquaternionicinteger.h"
#include "algebra/nfield25.h"
#include "maths/nrational.h"
#include <iostream>

namespace regina {

/**
 * Assignment of quaternions.
 */
NQuaternionicInteger NQuaternionicInteger::operator = (const NQuaternionicInteger& other)
 {
 Rc = other.Rc; Ic = other.Ic; Jc = other.Jc; Kc = other.Kc;
 return (*this);
 }

/**
 * Addition of quaternions.
 */
NQuaternionicInteger NQuaternionicInteger::operator + (const NQuaternionicInteger& other) const
{
 NQuaternionicInteger temp(Rc + other.Rc, Ic + other.Ic, Jc + other.Jc, Kc + other.Kc);
 return temp;
}

/**
 * Multiplication of quaternions.
 */
NQuaternionicInteger NQuaternionicInteger::operator * (const NQuaternionicInteger& other) const
{
  NField25 tRc, tIc, tJc, tKc;

  tRc = Rc*other.Rc - Ic*other.Ic - Jc*other.Jc - Kc*other.Kc;
  tIc = Rc*other.Ic + Ic*other.Rc + Jc*other.Kc - Kc*other.Jc;
  tJc = Rc*other.Jc - Ic*other.Kc + Jc*other.Rc + Kc*other.Ic;
  tKc = Rc*other.Kc + Ic*other.Jc - Jc*other.Ic + Kc*other.Rc;

  return NQuaternionicInteger(tRc,tIc,tJc,tKc); 
}

/**
 * Multiplicative inverse
 */
NQuaternionicInteger NQuaternionicInteger::inverse() const
{
  NQuaternionicInteger temp( *this );  
  temp.Ic.negate();
  temp.Jc.negate();
  temp.Kc.negate();
  return temp;
}

void NQuaternionicInteger::writeTextShort(std::ostream& out) const 
{
 // eventually should write it in a normalized format? 
 // or maybe only do that for TeX output?

 bool ws=false; // written something	
 if ( (Ic != NRational::zero) || (Jc != NRational::zero) || (Kc != NRational::zero) ) 
  {
  if (Rc != NRational::zero) 
     { 
     Rc.writeTextShort(out); ws=true; 
     }
  if (Ic != NRational::zero) 
     { 
      if (ws) 
         { 
          out<<"+"; Ic.writeTextShort(out); out<<"i"; 
         }
       else 
         { 
           Ic.writeTextShort(out); out<<"i"; ws=true; 
         }  
      }
  if (Jc != NRational::zero) 
      { 
        if (ws) 
            {
              out<<"+"; Jc.writeTextShort(out); out<<"j"; 
            }
	else 
            { 
              Jc.writeTextShort(out); out<<"j"; ws=true; 
            }  
      }
  if (Kc != NRational::zero) 
      { 
        if (ws) 
          { 
            out<<"+"; Kc.writeTextShort(out); out<<"k"; 
          }
      }
  }
 else
  {
  Rc.writeTextShort(out);  
  }
}



} // namespace regina

