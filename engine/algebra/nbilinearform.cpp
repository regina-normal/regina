
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
			     const NMarkedAbelianGroup &range,   const NSparseGridRing< NLargeInteger > &pairing) : 
ShareableObject(), reducedPairing(NULL), unreducedPairing(NULL), lDomain(ldomain), rDomain(rdomain), Range(range)
{
 unreducedPairing = new NSparseGridRing< NLargeInteger > (pairing);
 // now we construct the reducedPairing
 reducedPairing = new NSparseGridRing< NLargeInteger > (3);

 for (unsigned long i=0; i<ldomain.minNumberOfGenerators(); i++)
  { std::vector< NLargeInteger > lv(ldomain.ccRep(i));
   for (unsigned long j=0; j<rdomain.minNumberOfGenerators(); j++)
    { std::vector< NLargeInteger > rv(rdomain.ccRep(j));
      std::vector< NLargeInteger > evalcc( range.getRankCC(), NLargeInteger::zero ); // pre SNF rep

      // sum_{ii, jj, k} lv[ii] * rv[jj] * pairing[ii,jj,k] e_k
      // is reducedPairing[i,j,k], record if non-zero.  
      std::map< NMultiIndex< unsigned long >, NLargeInteger* >::const_iterator I;
      for (I=pairing.getGrid().begin(); I!=pairing.getGrid().end(); I++)
	 evalcc[ I->first.entry(2) ] += lv[ I->first.entry(0) ] * rv[ I->first.entry(1) ] * (*(I->second));
//if (!range.isCycle(evalcc)) std::cout<<"!"; // helpful for debugging not-well-thought-out bilinear forms
      std::vector< NLargeInteger > evalsnf( range.snfRep( evalcc ) );

      NMultiIndex< unsigned long > J(3); J[0] = i; J[1]=j;
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

const std::map< NMultiIndex< unsigned long >, NLargeInteger* > & NBilinearForm::unreducedMap() const
{ return unreducedPairing->getGrid(); }

const std::map< NMultiIndex< unsigned long >, NLargeInteger* > & NBilinearForm::reducedMap() const
{ return reducedPairing->getGrid(); }


std::vector<NLargeInteger> NBilinearForm::evalCC(std::vector<NLargeInteger> &lcc, std::vector<NLargeInteger> &rcc) const
{
 std::vector<NLargeInteger> retval(Range.getRankCC(), NLargeInteger::zero);
 static const std::vector<NLargeInteger> nullVec(0);
 if ( (lcc.size() != lDomain.getRankCC()) || (rcc.size() != rDomain.getRankCC()) ) return nullVec;
 // good to go.

 std::map< NMultiIndex< unsigned long >, NLargeInteger* >::const_iterator J;
 for (J = unreducedPairing->getGrid().begin(); J!=unreducedPairing->getGrid().end(); J++)
   retval[ J->first.entry(2) ] += lcc[J->first.entry(0)]*rcc[J->first.entry(1)]*(*J->second);
 // now find an NMarkedAbelianGroup with the right presentation for Range such that mat makes sense as a map..

 return retval;
}


long int NBilinearForm::signature() const 
{
 if (!isSymmetric()) return 0; 
 if (!Range.isIsomorphicTo(NMarkedAbelianGroup(1, NLargeInteger::zero))) return 0;
 // ldomain == rdomain, form symmetric, range == Z.
 // so reducedpairing is nxnx1 -- think of it as a matrix M, computed Det(tI-M)
 NMatrixRing< NSVPolynomialRing< NLargeInteger > > cM( lDomain.getRank(), rDomain.getRank() );
 // iterate through reducedPairing, insert into cM
 std::map< NMultiIndex< unsigned long >, NLargeInteger* >::const_iterator i;
 for (i = reducedPairing->getGrid().begin(); i!=reducedPairing->getGrid().end(); i++)
  { 
  if ( (i->first.entry(0) >= lDomain.getNumberOfInvariantFactors()) &&
       (i->first.entry(1) >= rDomain.getNumberOfInvariantFactors()) )
  cM.entry( i->first.entry(0) - lDomain.getNumberOfInvariantFactors() , 
            i->first.entry(1) - rDomain.getNumberOfInvariantFactors() ) = 
            NSVPolynomialRing< NLargeInteger >(-(*i->second), 0); }
 // add t down diagonal
 for (unsigned long j=0; j<cM.rows(); j++) cM.entry(j,j) += NSVPolynomialRing< NLargeInteger >::pvar;
 // grab an adjoint, get its defining matrix, compute char poly, use Descartes
 // to get number of pos - neg roots. 
 NSVPolynomialRing< NLargeInteger > charPoly(cM.det());
 return charPoly.descartesNo();
}



std::vector< NLargeInteger > NBilinearForm::oddKKvec() const
{
// assumes ldomain == rdomain, form symmetric, range cyclic of order the
// order of the torsion subgroup of ldomain and rdomain. 
std::vector<NLargeInteger> retval;
// TODO

return retval;
}



std::vector< NLargeInteger > NBilinearForm::twoKKvec() const
{
// assumes ldomain == rdomain, form symmetric, range cyclic of order the
// order of the torsion subgroup of ldomain and rdomain. 
std::vector<NLargeInteger> retval;
// TODO
return retval;
}



bool NBilinearForm::isHyperbolic() const
{
// go through oddKKvec and twoKKvec and check... import from NHomologicalData
// TODO
return true;
}

NMarkedAbelianGroup NBilinearForm::image() const
{ 
 // lets compute the image based off of the reducedpairing. 
 NMarkedAbelianGroup dom( lDomain.minNumberOfGenerators()*rDomain.minNumberOfGenerators(), NLargeInteger::zero );
 NMatrixInt mat( Range.minNumberOfGenerators(), dom.minNumberOfGenerators() );
 // fill mat -- we'll do this with an iterator through reducedPairing
 std::map< NMultiIndex< unsigned long >, NLargeInteger* >::const_iterator J;
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
std::map< NMultiIndex< unsigned long >, NLargeInteger* >::const_iterator J;

for (J = reducedPairing->getGrid().begin(); J!=reducedPairing->getGrid().end(); J++)
  { 
  NMultiIndex< unsigned long > x(3);
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
std::map< NMultiIndex< unsigned long >, NLargeInteger* >::const_iterator J;

for (J = reducedPairing->getGrid().begin(); J!=reducedPairing->getGrid().end(); J++)
  { 
  NMultiIndex< unsigned long > x(3);
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
NSparseGridRing< NLargeInteger > newPairing(3);
// 0th index is lDomain SNF coord, 1st index rDomain SNF coord, 3rd index Range SNF coord
std::map< NMultiIndex< unsigned long >, NLargeInteger* >::const_iterator J;

for (unsigned long i=0; i<f.getDomain().getRankCC(); i++)
 for (J = unreducedPairing->getGrid().begin(); J!=unreducedPairing->getGrid().end(); J++)
  { 
  // newPairing[ i,J[1],J[2] ] += f.getDefiningMatrix.entry( J[0], i ) * unreducedPairing[ J ]
  NMultiIndex< unsigned long > x(3);
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
NSparseGridRing< NLargeInteger > newPairing(3);
std::map< NMultiIndex< unsigned long >, NLargeInteger* >::const_iterator J;

for (unsigned long i=0; i<f.getDomain().getRankCC(); i++)
 for (J = unreducedPairing->getGrid().begin(); J!=unreducedPairing->getGrid().end(); J++)
  { 
  // newPairing[ ?,i,? ] += f.getDefiningMatrix.entry( ?, i ) * unreducedPairing[ ?, ?, ? ]
  NMultiIndex< unsigned long > x(3);
  x[0]=J->first.entry(0); x[1] = i; x[2]=J->first.entry(2);
  newPairing.incEntry( x, f.getDefiningMatrix().entry( J->first.entry(1), i ) * (*J->second) );
  }

return NBilinearForm( lDomain, f.getDomain(), Range, newPairing );
}



NBilinearForm NBilinearForm::postCompose(const NHomMarkedAbelianGroup &f) const
{
NSparseGridRing< NLargeInteger > newPairing(3);

std::map< NMultiIndex< unsigned long >, NLargeInteger* >::const_iterator J;

for (J = unreducedPairing->getGrid().begin(); J!=unreducedPairing->getGrid().end(); J++)
 for (unsigned long i=0; i<f.getRange().getRankCC(); i++)
  { 
  // newPairing[ ?,?,i ] += f.getDefiningMatrix.entry( i, ? ) * unreducedPairing[ ?, ?, ? ]
  NMultiIndex< unsigned long > x(3);
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
 std::map< NMultiIndex< unsigned long >, NLargeInteger* >::const_iterator I;
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
 std::map< NMultiIndex< unsigned long >, NLargeInteger* >::const_iterator I;
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
out<<"\nrp: "; reducedPairing->writeTextShort(out); out<<" / ";
out<<"urp: "; unreducedPairing->writeTextShort(out); out<<"\n";

if (reducedPairing->getGrid().size() == 0) out<<" zero"; else
 {
 if (isSymmetric()) out<<" symmetric"; 
 if (isAntiSymmetric()) out<<" anti-symmetric";
 out<<" image == "; image().writeTextShort(out);
 }
}


void computeTorsionLinkingFormInvariants(const NBilinearForm &intP, 
	std::vector< std::pair< NLargeInteger, std::vector< unsigned long > > > &ppVec, // almost same except this counts
        std::vector< std::pair< NLargeInteger, std::vector< unsigned long > > > &ppList,// this lists 
        std::vector<unsigned long> &ttVec, 
        std::vector< std::pair< unsigned long, std::vector< int > > > &ptVec, 
        std::vector< NMatrixRing<NRational>* > &linkingFormPD ) 
{
    NLargeInteger tN,tD,tR;
    // min number of torsion gens:
    unsigned long niv(intP.ldomain().getNumberOfInvariantFactors());
    // for holding prime decompositions.:
    std::vector<std::pair<NLargeInteger, unsigned long> > tFac;
    NLargeInteger tI;

    // step 1: go through H1 of the manifold, take prime power decomposition
    //            of each summand.  building primePowerH1Torsion vector and
    //            pTorsionH1Mat matrix...
    //            also, we need to find the 2-chains bounding2c
    //            boundary(bounding2c[i]) = orderinh1(pvList[i])*pvList[i]
    std::vector< NLargeInteger > tV; // temporary vector for holding dual cc vectors.

    std::vector< std::pair<NLargeInteger, unsigned long> > pPrList; // proper prime power list.
    std::vector< NLargeInteger > PPList;
    std::vector< std::vector<NLargeInteger> > pvList; // list of vectors
    // the above two lists will have the same length. for each i,
    // pvList[i] will be a vector in the dual h1 homology chain complex, and
    // ppList[i] will be its order.

    unsigned long i, j, k, l;

    for (i=0; i<niv; i++) {
        tI = intP.ldomain().getInvariantFactor(i);
        tFac = NPrimes::primePowerDecomp(tI);

        for (j=0; j<tFac.size(); j++) {
            pPrList.push_back(tFac[j]);
            NLargeInteger fac1, fac2, fac1i, fac2i;
            fac1 = tFac[j].first;
            fac1.raiseToPower(tFac[j].second);
            fac2 = tI;
            fac2.divByExact(fac1);
            // fac2's should be the denominators in the matrix...
            fac2.gcdWithCoeffs( fac1, fac1i, fac2i ); // fac2i unneccessary..
            // fac1i is the inverse of fac1 mod fac2
            PPList.push_back( fac1 ); // record the order...
            // now the corresponding vector...
            // this will have to be fac1i * vector corresponding to
            // getInvariantFactor(i).
            tV = intP.ldomain().getTorsionRep(i);
            for (k=0; k<tV.size(); k++) tV[k]=fac1i*fac2*tV[k];
            pvList.push_back(tV);
        }
    }

    // step1a: construct (2 2 4) (3 3 9 27) ... indexing of ppList, pvList, etc.
    // the indexing will be as a list of pairs
    // < prime, vector< pair< power, index> > >
    // Use a list because we are continually inserting items in the middle.
    typedef std::vector<std::pair<unsigned long, unsigned long> > IndexingPowerVector;
    typedef std::pair<NLargeInteger, IndexingPowerVector> IndexingPrimePair;
    typedef std::list<IndexingPrimePair> IndexingList;
    IndexingList indexing;
    // indexing[i] is the i-th prime in increasing order, the first bit is
    // the prime, the 2nd bit is the vector list of powers, the power is an
    // unsigned long, and its respective index in ppList and pvList is the
    // 2nd bit...
    IndexingList::iterator it1, il1;
    IndexingPowerVector::iterator it2, il2;
    IndexingPrimePair dummyv;

    for (i=0; i<pPrList.size(); i++) { 
        // for each entry in pPrList, find its appropriate position in indexing.
        // so this means comparing pPrList[i].first with all elts
        // indexing[j].first and stopping at first >= comparison.

        it1 = indexing.begin(); 
        // now run up p until we either get to the end, or
        // pPrList[i].first >= it1->first
        il1 = indexing.end(); 
        // the idea is that this while loop will terminate with il1 pointing
        // to the right insertion location.
        while ( it1 != indexing.end() ) {
            if (pPrList[i].first <= it1->first) {
                il1 = it1;
                it1 = indexing.end();
            }
            if (it1 != indexing.end()) it1++;
        }
        // now do the same for the power... but we have to make a decision
        // on whether to grow the
        // indexing or not... we grow the indexing iff il1 == indexing.end() or
        //         (pPrList[i].first > il1->first)
        if (il1 == indexing.end()) {
            dummyv.first = pPrList[i].first;
            dummyv.second.resize(1);
            dummyv.second[0] = std::make_pair( pPrList[i].second, i );
            indexing.insert( il1, dummyv );
        } else
            if (pPrList[i].first < il1->first) {
                dummyv.first = pPrList[i].first;
                dummyv.second.resize(1);
                dummyv.second[0] = std::make_pair( pPrList[i].second, i );
                indexing.insert( il1, dummyv );
            } else {
                // NOW we know this prime is already in the list, so we do
                // the same search for the power...
                it2 = il1->second.begin();
                il2 = il1->second.end();
                while ( it2 != il1->second.end() ) {
                    // it2->first is the power, it2->second is the index.
                    if (pPrList[i].second <= it2->first) {
                        il2 = it2;
                        it2 = il1->second.end();
                    }
                    if (it2 != il1->second.end()) it2++;
                }
                il1->second.insert(il2, std::make_pair( pPrList[i].second, i ));
            }
    }

    // Compute indexing.size() just once, since for std::list this might be
    // a slow operation.
    unsigned long indexingSize = indexing.size();
    ppList.resize(indexingSize);
    linkingFormPD.resize(indexingSize);

    // let's find the denominator of all our NRationals in our linking form matrices...
    NLargeInteger DenOm( intP.range().getInvariantFactor(0) );
 
    for (i=0, it1 = indexing.begin(); it1 != indexing.end(); i++, it1++) {
        ppList[i].second.resize(it1->second.size());
        ppList[i].first = it1->first;

        for (j=0; j<it1->second.size(); j++)
            ppList[i].second[j] = it1->second[j].first;

        linkingFormPD[i] = new NMatrixRing<NRational>(it1->second.size(), it1->second.size() );
        for (j=0; j<it1->second.size(); j++)
            for (k=0; k<it1->second.size(); k++)
                {
                // this routine is currently slower than it should be because of the new
	        // sparse implementation in nbilinearform -- if this is a problem it should eventually
	        // be given a proper going-over as the slow-down is only due to inefficient coding
	        // on my part (Budney) -- it's fast enough for my purposes so I'll stop mucking with it.
                const NSparseGridRing< NLargeInteger >* presMap( intP.reducedSparseGrid() );
		const NLargeInteger* nliP( presMap->getEntry( NMultiIndex< unsigned long >( it1->second[j].second, it1->second[k].second, 0 ) ) );
		if (nliP) linkingFormPD[i]->entry(j,k) = NRational( *nliP, DenOm );
                }
    }

    // now we should implement the classification of these forms
    // due to Seifert, Wall, Burger, Kawauchi, Kojima, Deloup:
    // this will have 3 parts, first the rank vector will be a list
    // n1 Z_p1^k1 + ... + nj Z_pj^kj which will be in lexicographically
    // increasing order: first the p?'s then the k?'s.
    // the 2nd part will be the 2-torsion sigma-vector:
    // sigma_k for k=1,2,3,... these are fractions 0/8, ..., 7/8 or infinity.
    // the 3rd part will be the odd p-torsion Legendre symbol data
    // this will be in lexicographical increasing order, first
    // by the prime, then by k \chi_p^k k=1,2,3,...

    // CLASSIFICATION

    // step 1: rank vectors (done)
    //
    // this will be a std::vector< std::pair< NLargeInteger,
    //                                        std::vector< unsigned long > > >
    // rankv[i].first is the prime, and rankv[i].second is the vector which
    // lists the ranks
    // ie: if rankv[i].first==3 then rankv[i].second=(0,1,0,2,0,1) means that
    // there are no copies of Z_3, one copy of Z_9, no copies of Z_27 but two
    // copies of Z_{3^4}, etc.

    // std::vector< std::pair< NLargeInteger,
    //     std::vector<std::pair<unsigned long, unsigned long> > > > indexing;
    //                         prime        , list of (exponents, index)

    ppVec.resize(indexingSize);
    // std::vector< std::pair< NLargeInteger,
    //     std::vector< unsigned long > > > ppVec(indexing.size());
    // vector which lists the primes and the number of each power...
    for (i=0, it1 = indexing.begin(); it1 != indexing.end(); i++, it1++) {
        ppVec[i].first = it1->first;
        ppVec[i].second.resize(
            it1->second[it1->second.size()-1].first, 0);
        for (j=0; j<it1->second.size(); j++) { 
        // indexing[i].second[j] is a pair (order, index) where the order k
        // indicates one copy of p^k where p==indexing[i].first.
            ppVec[i].second[it1->second[j].first-1]++;
        }
    }

    // step 2: KK 2-torsion invariant
    // decide on if there is 2-torsion...
    NLargeInteger twoPow;
    static const NRational pi = NRational(
                NLargeInteger("314159265358979323846264338327950288"),
                NLargeInteger("100000000000000000000000000000000000") );
    std::vector< NLargeInteger > groupV;
    bool notatend;
    NRational tSum;

    unsigned long incind;
    bool incrun;
    long double tLD;
    long double xlD, ylD;

    std::vector< NLargeInteger > ProperPrimePower;

    if (ppList.size() > 0)
        if (ppList[0].first == NLargeInteger(2)) { 
            // there is 2-torsion. now we put together the sigma vector
            // twoTorSigmaV
            // first initialize the length of twoTorSigmaV
            ttVec.resize(ppVec[0].second.size());

            groupV.resize(ppList[0].second.size(),
                NLargeInteger("0") );

            ProperPrimePower.resize( ppList[0].second.size() );
            for (i=0; i<ProperPrimePower.size(); i++) {
                ProperPrimePower[i] = NLargeInteger(2);
                ProperPrimePower[i].raiseToPower(ppList[0].
                    second[i] );
            }

            for (i=0; i<ttVec.size(); i++) {
                // now we construct the sum over the group of
                // e^{ 2^{j+1} pi i form(x,x) } where x is a group element
                // ppVec[0].second[0] through ppVec.second[size()-1]
                // are the number of copies of
                // Z_2 ... through Z_{2^size()}
                // we need a loop that evaluates form(x,x) for all x in
                // this group...
                // the idea will be to have a start vector (0,0,...,0) and then
                // increment it until at the end vector.  For this purpose it
                // makes more sense to use the
                // std::vector< std::pair< NLargeInteger,
                //     std::vector<unsigned long> > > ppList;
                // as it's easier to work with.
                // ppList[0].first == 2
                // so we just need to cycle through
                // ppList[0].second which is an increasing list
                // of the powers of 2, ie: 2^j...

                twoPow = NLargeInteger(2);
                twoPow.raiseToPower(i+1);

                xlD=0.0;
                ylD=0.0;

                // now start the sum through the group.
                notatend=true;
                while (notatend) {
                    // compute twoPow * pi * form(x,x), reduce mod 1 then
                    // call doubleApprox()
                    // first we evaluate the form(x,x) for x==groupV.
                    // the form is linkingformPD[0]
                    tSum=NRational::zero;
                    for (j=0; j<linkingFormPD[0]->rows(); j++)
                        for (k=0; k<linkingFormPD[0]->columns();
                                k++)
                            tSum += NRational(groupV[j]*groupV[k])*
                                linkingFormPD[0]->entry(j,k);

                    // reduce mod 1, then turn into a long double and
                    // evaluate cos, sin
                    tN = tSum.getNumerator();
                    tD = tSum.getDenominator();
                    tN.divisionAlg(tD,tR);
                    tSum = NRational(twoPow) * pi * NRational( tR, tD );
                    tLD = tSum.doubleApprox();
                    // we ignore `inrange' parameter as the number is reduced
                    // mod 1, so either way it is
                    // returning essentially the correct number.
                    xlD = xlD + cos(tLD);
                    ylD = ylD + sin(tLD);
                    // increment the groupV
                    incind=0;
                    incrun=true; // tells while loop to increment at incind

                    while (incrun) {
			groupV[incind] += NLargeInteger::one;
			groupV[incind] %= ProperPrimePower[incind];
                        if (groupV[incind] == 0) {
                            incind++;
                        } else {
                            incrun=false;
                        }
                        if ( (incind == groupV.size()) && (incrun) ) {
                            incrun=false;
                            notatend=false;
                        }
                    }

                }
                // this sum is either zero or a multiple of e^{2pi i sigma /8}
                // and we now we need to determine if (xlD,ylD) is 0 or
                // nonzero with some sigma*2pi/8 angle...
                if ( (xlD*xlD)+(ylD*ylD)<0.0000001 ) // this we accept as zero.
                {
                    ttVec[i] = 8;
                } else { // now we need to determine the sigma angle...
                    // since it's all integer multiples of 2pi/8, we just
                    // need to check for
                    // xld==0, yld<>0, yld==0, xld<>0 and xld/yld=pm1
                    if ( fabs(xlD) < 0.001*fabs(ylD) ) {
                        if (ylD > 0.0) ttVec[i]=2;
                        else ttVec[i]=6;
                    } else
                        if ( fabs(ylD) < 0.001*fabs(xlD) ) {
                            if (xlD > 0.0) ttVec[i]=0;
                            else ttVec[i]=4;
                        } else
                            if (xlD/ylD > 0.0 ) {
                                if (xlD > 0.0) ttVec[i]=1;
                                else ttVec[i]=5;
                            } else {
                                if (xlD > 0.0) ttVec[i]=7;
                                else ttVec[i]=3;
                            }
                }
            }
        }

    // step 3: Seifert odd p-torsion legendre symbol invariant (done)
    //           to do this I need to add a determinant to NMatrixRing class
    //           this invariant will be expressed as a
    //           std::vector< std::pair< NLargeInteger, std::vector< int > > >
    //           storing the odd prime, list of Legendre symbols -1, 0, 1.
    //           one for each quotient up to p^k where k is the largest order of
    //           p in the torsion subgroup.

    unsigned long starti=0;

    if (ppVec.size() > 0)
        if (ppVec[0].first == NLargeInteger(2))
            starti=1;
    // this ensures we skip the 2-torsion
    std::vector<int> tempa;
    unsigned long curri;

    for (i=starti; i<ppVec.size(); i++) // for each prime
    {
        tempa.resize(0);
        curri=0;

        // now we cut out the appropriate section of linkingFormPD[i]
        // std::vector< std::pair< NLargeInteger,
        //     std::vector< unsigned long > > > ppVec(indexing.size());
        // starting at curri ending at ppVec[i].second[j]

        for (j=0; j<ppVec[i].second.size(); j++)
        // dimensions of p^{j+1} subspace
        {
            // initialize a ppVec[i].second[j] square matrix.
            NMatrixInt tempM(ppVec[i].second[j], ppVec[i].second[j]);

            // tempM will be the ppVec[i].second[j] square submatrix
            // starting at curri, multiplied by tI == p^j
            tI = ppVec[i].first;
            tI.raiseToPower(j+1);

            for (k=0; k<ppVec[i].second[j]; k++)
                for (l=0; l<ppVec[i].second[j]; l++)
                    tempM.entry(k,l) = (NRational(tI)*linkingFormPD[i]->
                        entry(k+curri,l+curri)).getNumerator();

            tempa.push_back( tempM.det().legendre(ppVec[i].first) );
            // legendre symbol, compute and append to tempa
            // compute determinant.

            // increment curri
            curri = curri + ppVec[i].second[j]; 
        }
        ptVec.push_back( make_pair( ppVec[i].first.longValue() , tempa) );
    }
} // end computeTorsionLinkingForm()


void readTeaLeavesTLF(const std::vector< std::pair< NLargeInteger, std::vector< unsigned long > > > &ppVec,
        const std::vector< std::pair< NLargeInteger, std::vector< unsigned long > > > &ppList,
        const std::vector<unsigned long> &ttVec, 
        const std::vector< std::pair< unsigned long, std::vector< int > > > &ptVec, 
        const std::vector< NMatrixRing<NRational>* > &linkingFormPD, 
        bool orientable, 
        bool &torsionLinkingFormIsSplit, bool &torsionLinkingFormIsHyperbolic, 
        std::string &torsionRankString,     std::string &torsionSigmaString,     
        std::string &torsionLegendreString )
{
    unsigned long starti, i, j; 
    NLargeInteger tN, tD, tR, tI;

    // step 4: kk test for: split, hyperbolic, and the embeddability
    //           2^k-torsion condition.

    torsionLinkingFormIsSplit=true;
    torsionLinkingFormIsHyperbolic=true;

    starti=0;
    if (ppVec.size() > 0)
        if (ppVec[0].first == NLargeInteger(2))
            starti=1;

    for (i=0; i<ppVec.size(); i++)
        for (j=0; j<ppVec[i].second.size(); j++)
            if ( (ppVec[i].second[j] % 2) != 0 )
                torsionLinkingFormIsSplit=false;
    if (torsionLinkingFormIsSplit) {
        for (i=0; i<ptVec.size(); i++)
            for (j=0; j<ptVec[i].second.size(); j++) {
                if ( ( (NLargeInteger(ppVec[i+starti].second[j])*
                        (ppVec[i+starti].first -
                        NLargeInteger::one))/NLargeInteger(4) ) %
                        NLargeInteger(2) == 0 ) {
                    if (ptVec[i].second[j] != 1)
                        torsionLinkingFormIsSplit=false;
                } // does this know how to deal with .second[j]==0??
                else {
                    if (ptVec[i].second[j] == 1)
                        torsionLinkingFormIsSplit=false;
                }
            }
    }
    if (starti==1) // have 2-torsion
    { // all the sigmas need to be 0 or inf.
        for (i=0; i<ttVec.size(); i++)
            if ( (ttVec[i]!=0) && (ttVec[i]!=8) )
                torsionLinkingFormIsSplit=false;
    }

    if (torsionLinkingFormIsSplit==false) torsionLinkingFormIsHyperbolic=false;

    if ( (torsionLinkingFormIsSplit) && (starti==1) ) {
        torsionLinkingFormIsHyperbolic = true;
        for (i=0; i<ttVec.size(); i++)
            if (ttVec[i]!=0)
                torsionLinkingFormIsHyperbolic=false;
    }

    NRational tRat;

    bool torsionLinkingFormSatisfiesKKtwoTorCondition=true;
    if (starti==1) { // for each k need to compute 2^{k-1}*form(x,x) on all
        // elements of order 2^k, check to see if it is zero.
        // so this is not yet quite implemented, yet....
        // std::vector< std::pair< NLargeInteger,
        //     std::vector<unsigned long> > > ppList;
        // stored as list { (2, (1, 1, 2)), (3, (1, 2, 2, 3)), (5, (1, 1, 2)) }
        //std::vector< NMatrixRing<NRational>* > linkingFormPD;
        for (i=0; i<ppList[0].second.size(); i++) {
            // run down diagonal of linkingFormPD[0], for each (i,i) entry
            // multiply it by 2^{ppList[0].second[i]-1} check if
            // congruent to zero. if not, trigger flag.
            tI = NLargeInteger("2");
            tI.raiseToPower(ppList[0].second[i]-1);
            tRat = NRational(tI) * linkingFormPD[0]->entry(i,i);
            tN = tRat.getNumerator();
            tD = tRat.getDenominator();
            tN.divisionAlg(tD,tR);
            if (tR != 0)
                torsionLinkingFormSatisfiesKKtwoTorCondition=false;
        }

    }

    torsionRankString.assign("");
    if (ppVec.size()==0) torsionRankString.append("no torsion");
    else for (i=0; i<ppVec.size(); i++) {
            torsionRankString.append(ppVec[i].first.stringValue());
            torsionRankString.append("(");
            for (j=0; j<ppVec[i].second.size(); j++) {
                torsionRankString.append(
                    NLargeInteger(ppVec[i].second[j]).stringValue() );
                if (j < ppVec[i].second.size()-1)
                    torsionRankString.append(" ");
            }
            torsionRankString.append(")");
            if (i<(ppVec.size()-1))
                torsionRankString.append(" ");
    }

    if (orientable) {
        torsionSigmaString.assign("");
        if (ttVec.size()==0) torsionSigmaString.append("no 2-torsion");
        else for (i=0; i<ttVec.size(); i++) {
            std::stringstream ss; ss << ttVec[i]; 
            std::string tempS; ss >> tempS;
            torsionSigmaString.append(tempS); 
            if (i<(ttVec.size()-1)) torsionSigmaString.append(" ");
            }
        }
    else torsionSigmaString.assign("manifold is non-orientable");

    if (orientable) {
        torsionLegendreString.assign("");
        if (ptVec.size()==0)
            torsionLegendreString.append("no odd p-torsion");
        else for (i=0; i<ptVec.size(); i++) {
            std::stringstream ss; ss << ptVec[i].first; 
	    std::string tempS; ss >> tempS;
            torsionLegendreString.append(tempS);           
            torsionLegendreString.append("(");
            for (j=0; j<ptVec[i].second.size(); j++) {
                torsionLegendreString.append( NLargeInteger(
                    ptVec[i].second[j]).stringValue());
                if (j<ptVec[i].second.size()-1)
                    torsionLegendreString.append(" ");
            }
            torsionLegendreString.append(")");
            if (i<(ptVec.size()-1))
                torsionLegendreString.append(" ");
            }
        }
    else
        torsionLegendreString.append("manifold is non-orientable");
} // end readTeaLeavesTLF


} // namespace regina


