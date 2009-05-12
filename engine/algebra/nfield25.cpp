
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


#include "maths/nrational.h"
#include "algebra/nfield25.h"

namespace regina {

const NRational NField25::c2(2);
const NRational NField25::c10(10);
const NRational NField25::c5(5);

/**
 * Multiplicative inverse
 */
NField25 NField25::inverse() const
{
 NField25 temp;
 temp.a= a*a*a-c2*a*b*b+c2*c10*b*c*d-c10*a*d*d-c5*a*c*c; 
 temp.b= c2*b*b*b-b*a*a+c10*a*c*d-c10*b*d*d-c5*b*c*c; 
 temp.c= c2*c2*a*b*d-c2*b*b*c-a*a*c+c5*c*c*c-c10*c*d*d; 
 temp.d= c2*a*b*c-c2*b*b*d-a*a*d+c10*d*d*d-c5*c*c*d;
 NRational K;
 K=a*a*a*a+c2*c2*b*b*b*b+c5*c5*c*c*c*c+c10*c10*d*d*d*d
	-c2*c2*a*a*b*b-c10*a*a*c*c-c2*c10*a*a*d*d-c2*c10*b*b*c*c-c2*c2*c10*b*b*d*d-c10*c10*c*c*d*d
	+c2*c2*c2*c10*a*b*c*d;
 temp = temp * (K.inverse());
 return temp;
}

/**
 * the other negate
 */
NField25 operator - (const NField25 &o)
{
NField25 retval(o);
retval.a.negate(); retval.b.negate(); retval.c.negate(); retval.d.negate();
return retval;
}



/**
 * Writes the element of the field in human-readable form, base field not mentioned.
 *
 * @param out the stream to write to.
 */
void NField25::writeTextShort(std::ostream& out) const
{
 bool ws=false; // written something yet?

 if (a != NRational::zero) { out<<a; ws=true; }

 if (b != NRational::zero) 
  { 
   if ( (ws) && (b > NRational::zero) ) out<<"+";
   out<<b;
   out<<"t";
   ws = true;
  }

 if (c != NRational::zero) 
  { 
   if ( (ws) && (c > NRational::zero) ) out<<"+";
   out<<c;
   out<<"f";
   ws = true;
  }

 if (d != NRational::zero) 
  { 
   if ( (ws) && (d > NRational::zero) ) out<<"+";
   out<<d;
   out<<"T";
   ws = true;
  }

 if ( (ws==false) && (nnzt()==0) ) out<<"0";
}

/**
 * writes TeX output
 */
void NField25::writeTeX(std::ostream &out) const
{
 bool ws=false; // written something yet?

 if (a != NRational::zero) { a.writeTeX(out); ws=true; }

 if (b != NRational::zero) 
  { 
   if ( (ws) && (b > NRational::zero) ) out<<"+";
   b.writeTeX(out);
   out<<"\\sqrt{2}";
   ws = true;
  }

 if (c != NRational::zero) 
  { 
   if ( (ws) && (c > NRational::zero) ) out<<"+";
   c.writeTeX(out);
   out<<"\\sqrt{5}";
   ws = true;
  }

 if (d != NRational::zero) 
  { 
   if ( (ws) && (d > NRational::zero) ) out<<"+";
   d.writeTeX(out);
   out<<"\\sqrt{10}";
   ws = true;
  }

 if ( (ws==false) && (nnzt()==0) ) out<<"0";
}

bool NField25::requires_padding() const
{
	bool retval=false;
	if (nnzt()==1)
	 {
	  if (a > NRational::zero) retval=true;
	  if (b > NRational::zero) retval=true;
	  if (c > NRational::zero) retval=true;
	  if (d > NRational::zero) retval=true;
	 }
	else
	 {
	  if (nnzt()>1) retval=true;
	 }
	return retval;
}


std::ostream& operator << (std::ostream& out, const NField25& dat)
{
	dat.writeTextShort(out);
	return out;
}








} // namespace regina


