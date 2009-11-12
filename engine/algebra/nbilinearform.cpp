
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
  { std::vector< NLargeInteger > lv(ldomain.ccRep(i));
   for (unsigned long j=0; j<rdomain.minNumberOfGenerators(); j++)
    { std::vector< NLargeInteger > rv(rdomain.ccRep(j));
      std::vector< NLargeInteger > evalcc( range.getRankCC(), NLargeInteger::zero ); // pre SNF rep

      // sum_{ii, jj, k} lv[ii] * rv[jj] * pairing[ii,jj,k] e_k
      // is reducedPairing[i,j,k], record if non-zero.  
      std::map< NMultiIndex, NLargeInteger* >::const_iterator I;
      for (I=pairing.getGrid().begin(); I!=pairing.getGrid().end(); I++)
	 evalcc[ I->first.entry(2) ] += lv[ I->first.entry(0) ] * rv[ I->first.entry(1) ] * (*(I->second));
//if (!range.isCycle(evalcc)) std::cout<<"!"; // helpful for debugging not-well-thought-out bilinear forms
      std::vector< NLargeInteger > evalsnf( range.snfRep( evalcc ) );

      NMultiIndex J(3); J[0] = i; J[1]=j;
      for (J[2]=0; J[2]<evalsnf.size(); J[2]++) reducedPairing->setEntry( J, evalsnf[J[2]] );
    }
  }
}

NBilinearForm::NBilinearForm(const NBilinearForm& cloneMe) : ShareableObject(),
reducedPairing(clonePtr(cloneMe.reducedPairing)), unreducedPairing(clonePtr(cloneMe.unreducedPairing)),
lDomain(cloneMe.lDomain), rDomain(cloneMe.rDomain), Range(cloneMe.Range) {}

NBilinearForm::~NBilinearForm()
{ if (reducedPairing) delete reducedPairing;
  if (unreducedPairing) delete unreducedPairing; }

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



long int NBilinearForm::signature() const 
{
 if (!isSymmetric()) return 0; 
 if (!Range.isIsomorphicTo(NMarkedAbelianGroup(1, NLargeInteger::zero))) return 0;
 // ldomain == rdomain, form symmetric, range == Z.
 // so reducedpairing is nxnx1 -- think of it as a matrix M, computed Det(tI-M)
 NMatrixRing< NSVPolynomialRing > cM( lDomain.getRank(), rDomain.getRank() );
 // iterate through reducedPairing, insert into cM
 std::map< NMultiIndex, NLargeInteger* >::const_iterator i;
 for (i = reducedPairing->getGrid().begin(); i!=reducedPairing->getGrid().end(); i++)
  { 
  if ( (i->first.entry(0) >= lDomain.getNumberOfInvariantFactors()) &&
       (i->first.entry(1) >= rDomain.getNumberOfInvariantFactors()) )
  cM.entry( i->first.entry(0) - lDomain.getNumberOfInvariantFactors() , 
            i->first.entry(1) - rDomain.getNumberOfInvariantFactors() ) = 
            NSVPolynomialRing(-(*i->second), 0); }
 // add t down diagonal
 for (unsigned long j=0; j<cM.rows(); j++) cM.entry(j,j) += NSVPolynomialRing::pvar;
 // grab an adjoint, get its defining matrix, compute char poly, use Descartes
 // to get number of pos - neg roots. 
 NSVPolynomialRing charPoly(cM.det());
 return charPoly.descartesNo();
}



std::vector< NLargeInteger > NBilinearForm::oddKKvec() const
{
// assumes ldomain == rdomain, form symmetric, range cyclic of order the
// order of the torsion subgroup of ldomain and rdomain. 
std::vector<NLargeInteger> retval;
// todo

return retval;
}



std::vector< NLargeInteger > NBilinearForm::twoKKvec() const
{
// assumes ldomain == rdomain, form symmetric, range cyclic of order the
// order of the torsion subgroup of ldomain and rdomain. 
std::vector<NLargeInteger> retval;
// todo
return retval;
}



bool NBilinearForm::isHyperbolic() const
{
// go through oddKKvec and twoKKvec and check... import from NHomologicalData
// todo!
return true;
}

NMarkedAbelianGroup NBilinearForm::image() const
{ 
 // lets compute the image based off of the reducedpairing. 
 NMarkedAbelianGroup dom( lDomain.minNumberOfGenerators()*rDomain.minNumberOfGenerators(), NLargeInteger::zero );
 NMatrixInt mat( Range.minNumberOfGenerators(), dom.minNumberOfGenerators() );
 // fill mat -- we'll do this with an iterator through reducedPairing
 std::map< NMultiIndex, NLargeInteger* >::const_iterator J;
 for (J = reducedPairing->getGrid().begin(); J!=reducedPairing->getGrid().end(); J++)
   mat.entry( J->first.entry(2), J->first.entry(0)*rDomain.minNumberOfGenerators() + J->first.entry(1) ) = (*J->second);
 // now find an NMarkedAbelianGroup with the right presentation for Range such that mat makes sense as a map..
 NMatrixInt zeroM(1, Range.minNumberOfGenerators() );
 NMatrixInt redN( Range.minNumberOfGenerators(), Range.getNumberOfInvariantFactors() );
 for (unsigned long i=0; i<Range.getNumberOfInvariantFactors(); i++)
  redN.entry(i,i) = Range.getInvariantFactor(i);
 NMarkedAbelianGroup modRange( zeroM, redN );
 NHomMarkedAbelianGroup hom( dom, modRange, mat );
 return hom.getImage();
}


bool NBilinearForm::isSymmetric() const
{
if (!lDomain.equalTo(rDomain)) return 0;
// now we check symmetry.... we'll use the reduced matrix for this...
std::map< NMultiIndex, NLargeInteger* >::const_iterator J;

for (J = reducedPairing->getGrid().begin(); J!=reducedPairing->getGrid().end(); J++)
  { 
  NMultiIndex x(3);
  x[0]=J->first.entry(1); x[1]=J->first.entry(0); x[2]=J->first.entry(2);
  const NLargeInteger* t( reducedPairing->getEntry(x));
  if (!t) return false;
  if ( (*J->second) != (*t) ) return false;
  }

return true;
}



bool NBilinearForm::isAntiSymmetric() const
{
if (!lDomain.equalTo(rDomain)) return 0;
// now we check symmetry.... we'll use the reduced matrix for this...
std::map< NMultiIndex, NLargeInteger* >::const_iterator J;

for (J = reducedPairing->getGrid().begin(); J!=reducedPairing->getGrid().end(); J++)
  { 
  NMultiIndex x(3);
  x[0]=J->first.entry(1); x[1]=J->first.entry(0); x[2]=J->first.entry(2);
  const NLargeInteger* t( reducedPairing->getEntry(x));
  if (!t) return false;
  if ( x[2] < lDomain.getNumberOfInvariantFactors() ) 
   { if ( ((*J->second) + (*t)) % lDomain.getInvariantFactor(x[2]) != NLargeInteger::zero ) return false; }
  else if ( (*J->second) + (*t) != NLargeInteger::zero ) return false;
  }
return true;
}
	

/* p(ei,ej)=sum_k p^k_ij, f(ei)=sum_j f^j_i e_j
 * p' = p( f x I ), p'^k_ij = sum_l f^l_i p^k_lj
 */
NBilinearForm NBilinearForm::lCompose(const NHomMarkedAbelianGroup &f) const
{
// we need to compute the new unreducedPairing and pass it to an NBilinearForm constructor
NSparseGrid< NLargeInteger > newPairing(3);
// 0th index is lDomain SNF coord, 1st index rDomain SNF coord, 3rd index Range SNF coord
std::map< NMultiIndex, NLargeInteger* >::const_iterator J;

for (unsigned long i=0; i<f.getDomain().getRankCC(); i++)
 for (J = unreducedPairing->getGrid().begin(); J!=unreducedPairing->getGrid().end(); J++)
  { 
  // newPairing[ i,J[1],J[2] ] += f.getDefiningMatrix.entry( J[0], i ) * unreducedPairing[ J ]
  NMultiIndex x(3);
  x[0]=i; x[1]=J->first.entry(1); x[2]=J->first.entry(2);
  newPairing.incEntry( x, f.getDefiningMatrix().entry( J->first.entry(0), i ) * (*J->second) );
  }

return NBilinearForm( f.getDomain(), rDomain, Range, newPairing );
}


/* p(ei,ej)=sum_k p^k_ij, f(ei)=sum_j f^j_i e_j
 * p' = p( I x f ), p'^k_ij = sum_l f^l_j p^k_il
 */
NBilinearForm NBilinearForm::rCompose(const NHomMarkedAbelianGroup &f) const
{
// we need to compute the new unreducedPairing and pass it to an NBilinearForm constructor
NSparseGrid< NLargeInteger > newPairing(3);
std::map< NMultiIndex, NLargeInteger* >::const_iterator J;

for (unsigned long i=0; i<f.getDomain().getRankCC(); i++)
 for (J = unreducedPairing->getGrid().begin(); J!=unreducedPairing->getGrid().end(); J++)
  { 
  // newPairing[ ?,i,? ] += f.getDefiningMatrix.entry( ?, i ) * unreducedPairing[ ?, ?, ? ]
  NMultiIndex x(3);
  x[0]=J->first.entry(0); x[1] = i; x[2]=J->first.entry(2);
  newPairing.incEntry( x, f.getDefiningMatrix().entry( J->first.entry(1), i ) * (*J->second) );
  }

return NBilinearForm( lDomain, f.getDomain(), Range, newPairing );
}



NBilinearForm NBilinearForm::postCompose(const NHomMarkedAbelianGroup &f) const
{
NSparseGrid< NLargeInteger > newPairing(3);

std::map< NMultiIndex, NLargeInteger* >::const_iterator J;

for (J = unreducedPairing->getGrid().begin(); J!=unreducedPairing->getGrid().end(); J++)
 for (unsigned long i=0; i<f.getRange().getRankCC(); i++)
  { 
  // newPairing[ ?,?,i ] += f.getDefiningMatrix.entry( i, ? ) * unreducedPairing[ ?, ?, ? ]
  NMultiIndex x(3);
  x[0]=J->first.entry(0); x[1] = J->first.entry(1); x[2]=i;
  newPairing.incEntry( x, f.getDefiningMatrix().entry( i, J->first.entry(2) ) * (*J->second)  );
  }

return NBilinearForm( lDomain, rDomain, f.getRange(), newPairing );
}


// A x B --> C   turned into   A --> Hom(B,C)
NHomMarkedAbelianGroup NBilinearForm::leftAdjoint() const
{ 
 NMatrixInt M( 1, rDomain.minNumberOfGenerators()*Range.minNumberOfGenerators() );
 NMatrixInt N( rDomain.minNumberOfGenerators()*Range.minNumberOfGenerators(),  // Hom(B,C) presentation
               rDomain.minNumberOfGenerators()*Range.minNumberOfGenerators() ); // matrix

 for (unsigned long i=0; i<rDomain.minNumberOfGenerators(); i++) 
  for (unsigned long j=0; j<Range.minNumberOfGenerators(); j++)
   { 
     unsigned long k(i*Range.minNumberOfGenerators() + j);
     if ( i < rDomain.getNumberOfInvariantFactors() )
      { 
        if ( j < Range.getNumberOfInvariantFactors() ) 
         N.entry( k, k ) = rDomain.getInvariantFactor( i ).gcd( Range.getInvariantFactor( j ) );
        else N.entry( k, k ) = NLargeInteger::one;
      } 
     else
      { 
         if ( j < Range.getNumberOfInvariantFactors() ) 
          N.entry( k, k ) = Range.getInvariantFactor( j );
	 else N.entry( k, k ) = NLargeInteger::zero;
      } 
   }
 NMarkedAbelianGroup HOM(M,N);

 // step 2: find matrix A --> Hom(B,C)
 NMatrixInt adjmat( rDomain.minNumberOfGenerators()*Range.minNumberOfGenerators(), lDomain.minNumberOfGenerators() );
 std::map< NMultiIndex, NLargeInteger* >::const_iterator I;
 for (I=reducedPairing->getGrid().begin(); I!=reducedPairing->getGrid().end(); I++)
        {
        if ( ( I->first.entry(2) < Range.getNumberOfInvariantFactors() ) && 
             ( I->first.entry(1) < rDomain.getNumberOfInvariantFactors() ) )
         {
	  NLargeInteger P( rDomain.getInvariantFactor(I->first.entry(1)) );
	  NLargeInteger Q( Range.getInvariantFactor(I->first.entry(2)) );
	  NLargeInteger divBy( Q.divExact( P.gcd(Q) ) ); 
          adjmat.entry( I->first.entry(1)*Range.minNumberOfGenerators() + I->first.entry(2), I->first.entry(0) ) = 
           I->second->divExact( divBy ); // okay
         } 
	else
	 {
	  adjmat.entry( I->first.entry(1)*Range.minNumberOfGenerators() + I->first.entry(2), I->first.entry(0) ) = 
           (*I->second); // okay
	 }
        }

 // step 3: return the adjoint
 //         for this we need the SNF presentation of lDomain. 

 NMatrixInt lM( 1, lDomain.minNumberOfGenerators() );
 NMatrixInt lN( lDomain.minNumberOfGenerators(), lDomain.minNumberOfGenerators() );

 for (unsigned long i=0; i<lDomain.getNumberOfInvariantFactors(); i++)
	lN.entry(i,i) = lDomain.getInvariantFactor(i);

 NMarkedAbelianGroup simpleLdomain( lM, lN );

 return NHomMarkedAbelianGroup( simpleLdomain, HOM, adjmat );
}


// A x B --> C   turned into   B --> Hom(A,C)
NHomMarkedAbelianGroup NBilinearForm::rightAdjoint() const
{
 NMatrixInt M(1, lDomain.minNumberOfGenerators()*Range.minNumberOfGenerators() );
 NMatrixInt N( lDomain.minNumberOfGenerators()*Range.minNumberOfGenerators(), 
               lDomain.minNumberOfGenerators()*Range.minNumberOfGenerators() );

 for (unsigned long i=0; i<lDomain.minNumberOfGenerators(); i++) 
  for (unsigned long j=0; j<Range.minNumberOfGenerators(); j++)
   {
    unsigned long k(i*Range.minNumberOfGenerators() + j);
    if ( i < lDomain.getNumberOfInvariantFactors() )
     { 
      if ( j < Range.getNumberOfInvariantFactors() ) 
       N.entry( k, k ) = lDomain.getInvariantFactor( i ).gcd( Range.getInvariantFactor( j ) );
      else N.entry( k, k ) = NLargeInteger::one; 
     } 
    else
     { 
      if ( j < Range.getNumberOfInvariantFactors() ) 
       N.entry( k, k ) = Range.getInvariantFactor( j );
      else N.entry( k, k ) = NLargeInteger::zero; 
     } 
   }
 NMarkedAbelianGroup HOM(M,N);

 // step 2: find matrix B --> Hom(A,C)
 NMatrixInt adjmat( lDomain.minNumberOfGenerators()*Range.minNumberOfGenerators(), rDomain.minNumberOfGenerators() );
 std::map< NMultiIndex, NLargeInteger* >::const_iterator I;
 for (I=reducedPairing->getGrid().begin(); I!=reducedPairing->getGrid().end(); I++)
	{
	 if ( ( I->first.entry(2) < Range.getNumberOfInvariantFactors() ) &&
              ( I->first.entry(0) < lDomain.getNumberOfInvariantFactors() ) )
	  {
	   NLargeInteger P( lDomain.getInvariantFactor( I->first.entry(0) ) );
	   NLargeInteger Q( Range.getInvariantFactor( I->first.entry(2) ) );
	   NLargeInteger divBy( Q.divExact( P.gcd(Q) ) );
           adjmat.entry( I->first.entry(0)*Range.minNumberOfGenerators() + I->first.entry(2), I->first.entry(1) )
	    = I->second->divExact( divBy );
	  }
	 else
	  {
	   adjmat.entry( I->first.entry(0)*Range.minNumberOfGenerators() + I->first.entry(2), I->first.entry(1) )
	    = (*I->second);
	  }
	}
 // step 3: return the adjoint
 //	    simplified SNF pres of rDomain
 NMatrixInt rM( 1, rDomain.minNumberOfGenerators() );
 NMatrixInt rN( rDomain.minNumberOfGenerators(), rDomain.minNumberOfGenerators() );

 for (unsigned long i=0; i<rDomain.getNumberOfInvariantFactors(); i++)
	rN.entry(i,i) = rDomain.getInvariantFactor(i);

 NMarkedAbelianGroup simpleRdomain( rM, rN );

 return NHomMarkedAbelianGroup( simpleRdomain, HOM, adjmat );
}



void NBilinearForm::writeTextShort(std::ostream& out) const
{
out<<"Bilinear form: [";
lDomain.writeTextShort(out); out<<" x ";
rDomain.writeTextShort(out); out<<" --> ";
Range.writeTextShort(out);   out<<"]";
}

void NBilinearForm::writeTextLong(std::ostream& out) const
{
writeTextShort(out);

if (reducedPairing->getGrid().size() == 0) out<<" zero"; else
 {
//out<<"urp: "; unreducedPairing->writeTextShort(out);
out<<" rp: "; reducedPairing->writeTextShort(out);
 if (isSymmetric()) out<<" symmetric"; 
 if (isAntiSymmetric()) out<<" anti-symmetric";
 out<<" image == "; image().writeTextShort(out);
 }
}


} // namespace regina


