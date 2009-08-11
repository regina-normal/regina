
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

#include "algebra/nbilinearform.h"
#include "maths/nsparsegrid.h"
#include "maths/nlargeinteger.h"
#include "shareableobject.h"

#include <map>
//#include <iostream>
//#include <sstream>

namespace regina {

NBilinearForm::NBilinearForm(const NMarkedAbelianGroup &ldomain, const NMarkedAbelianGroup &rdomain,
			     const NMarkedAbelianGroup &range,   const NSparseGrid< NLargeInteger > &pairing) : 
ShareableObject(), reducedPairing(NULL), unreducedPairing(NULL), lDomain(ldomain), rDomain(rdomain), Range(range)
{
unreducedPairing = new NSparseGrid< NLargeInteger > (pairing);
// now we construct the reducedPairing
reducedPairing = new NSparseGrid< NLargeInteger > (3);

for (unsigned long i=0; i<ldomain.minNumberOfGenerators(); i++)
 {
 std::vector< NLargeInteger > lv(ldomain.ccRep(i));
 for (unsigned long j=0; j<rdomain.minNumberOfGenerators(); j++)
  {
  std::vector< NLargeInteger > rv(rdomain.ccRep(j));

  std::vector< NLargeInteger > evalcc( range.getRankCC(), NLargeInteger::zero ); // pre SNF rep
  // sum_{ii, jj, k} lv[ii] * rv[jj] * pairing[ii,jj,k] e_k
  // is reducedPairing[i,j,k], record if non-zero.  
  std::map< NMultiIndex, NLargeInteger* >::const_iterator I;
  for (I=pairing.getGrid().begin(); I!=pairing.getGrid().end(); I++)
	{
	 evalcc[ I->first.entry(2) ] += lv[ I->first.entry(0) ] * rv[ I->first.entry(1) ] * (*(I->second));
	}
  std::vector< NLargeInteger > evalsnf( range.snfRep( evalcc ) );
  for (unsigned long k=0; k<evalsnf.size(); k++) if (evalsnf[k] != 0) 
	{
	 NMultiIndex J(3);
	 J[0]=i; J[1]=j; J[2]=k;
	 reducedPairing->setEntry( J, evalsnf[k] );
	}
  }
 }
}

NBilinearForm::NBilinearForm(const NBilinearForm& cloneMe) : 
reducedPairing(clonePtr(cloneMe.reducedPairing)), unreducedPairing(clonePtr(cloneMe.unreducedPairing)),
lDomain(cloneMe.lDomain), rDomain(cloneMe.rDomain), Range(cloneMe.Range)
{}

NBilinearForm::~NBilinearForm()
{
if (reducedPairing) delete reducedPairing;
if (unreducedPairing) delete unreducedPairing;
}

NBilinearForm& NBilinearForm::operator = (const NBilinearForm& cloneMe)
{
if (reducedPairing) delete reducedPairing;
if (unreducedPairing) delete unreducedPairing;
reducedPairing = clonePtr(cloneMe.reducedPairing);
unreducedPairing = clonePtr(cloneMe.unreducedPairing);
}

const std::map< NMultiIndex, NLargeInteger* > & NBilinearForm::unreducedMap() const
{ return unreducedPairing->getGrid(); }

const std::map< NMultiIndex, NLargeInteger* > & NBilinearForm::reducedMap() const
{ return reducedPairing->getGrid(); }

unsigned long NBilinearForm::signature() const
{
// assumes ldomain == rdomain, form symmetric, range == Z. 
// grab an adjoint, get its defining matrix, compute char poly, use Descartes
// to get number of pos - neg roots. 

}

std::vector< unsigned long > NBilinearForm::oddKKvec() const
{
// assumes ldomain == rdomain, form symmetric, range cyclic of order the
// order of the torsion subgroup of ldomain and rdomain. 

}

std::vector< unsigned long > NBilinearForm::twoKKvec() const
{
// assumes ldomain == rdomain, form symmetric, range cyclic of order the
// order of the torsion subgroup of ldomain and rdomain. 
}

bool NBilinearForm::isHyperbolic() const
{
// go through oddKKvec and twoKKvec and check... import from NHomologicalData

}

NHomMarkedAbelianGroup NBilinearForm::leftAdjoint() const
{
// best to define using the unreducedPairing
// maybe that's a long-term thing.  Let's do it with the reducedPairing
// for now as it's easy to write out a presentation for Hom(B,C) in this
// case.
	/**
	 * Given a map A \otimes B --> C, there are two adjoints, 
	 *  1) A --> Hom(B,C) the "left" adjoint and
         *  2) B --> Hom(A,C) the "right adjoint.  This computes (1). 
	 */
// step 1: construct Hom(B,C)
// matrix M will be 1 -by- blah
// matrix N will be blah -by- num rels
// so we compute the vector of torsion thinggies...

// step 2: find matrix A --> Hom(B,C)
// step 3: return the adjoint

}

NHomMarkedAbelianGroup NBilinearForm::rightAdjoint() const
{
// best to define using the unreducedPairing

}

void NBilinearForm::writeTextShort(std::ostream& out) const
{
out<<"Bilinear form: [";
lDomain.writeTextShort(out);
out<<" x ";
rDomain.writeTextShort(out);
out<<" --> ";
Range.writeTextShort(out);
out<<"]";
// todo
}

} // namespace regina


