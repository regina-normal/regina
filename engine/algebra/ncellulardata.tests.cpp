
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

#include "maths/matrixops.h"
#include "maths/nprimes.h"
#include "algebra/ncellulardata.h"

#include <map>
#include <list>
#include <cmath>

#include <iostream>
#include <sstream>

namespace regina {

// for iteratoring through homology_coordinate_system
template <class Enum>
Enum & enum_increment(Enum & value, Enum begin, Enum end)
{ return value = (value == end) ? begin : Enum(value + 1); }

NCellularData::homology_coordinate_system & operator++ (NCellularData::homology_coordinate_system & cs)
{ return enum_increment(cs, NCellularData::first_coord, NCellularData::last_implemented_coord); }

// need to fix. 
bool NCellularData::chainComplexesVerified() const
{
unsigned long aDim( (tri4 != NULL) ? 4 : 3 );
for (homology_coordinate_system i=first_coord; i != last_implemented_coord; ++i) 
 {
  unsigned long maxDim( ((i == STD_BDRY_coord) || (i == MIX_BDRY_coord) || (i == DUAL_BDRY_coord)) ?
                        aDim - 1 : aDim );
  for (unsigned long j=1; j<maxDim; j++)
  {
   const NMatrixInt* A = integerChainComplex( ChainComplexLocator( j, i) );
   const NMatrixInt* B = integerChainComplex( ChainComplexLocator( j+1, i) );
   std::auto_ptr< NMatrixRing<NLargeInteger> > prod = (*A)*(*B);
   for (unsigned long j=0; j<prod->rows(); j++) for (unsigned long k=0; k<prod->columns(); k++)
	if (prod->entry(j,k) != 0) return false; 
  }
 }
return true;
}

bool NCellularData::chainMapsVerified() const
{
// verify mCC[i]*smCM[i] == smCM[i-1]*sCC[i]
/*for (unsigned long i=1; i<smCM.size(); i++) if (smCM[i] && smCM[i-1] && mCC[i] && sCC[i])
 {
  if ( (mCC[i]->columns() != smCM[i]->rows()) || (smCM[i-1]->columns() != sCC[i]->rows()) ) return false;
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod1 = (*mCC[i])*(*smCM[i]);
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod2 = (*smCM[i-1])*(*sCC[i]);
  if ( (*prod1) != (*prod2) ) return false; 
 }
// verify mCC[i]*dmCM[i] == dmCM[i-1]*dCC[i]
for (unsigned long i=1; i<dmCM.size(); i++) if (dmCM[i] && dmCM[i-1] && mCC[i] && dCC[i])
 {
  if ( (mCC[i]->columns() != dmCM[i]->rows()) || (dmCM[i-1]->columns() != dCC[i]->rows()) ) return false;
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod1 = (*mCC[i])*(*dmCM[i]);
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod2 = (*dmCM[i-1])*(*dCC[i]);
  if ( (*prod1) != (*prod2) ) return false; 
 }
// verify srCC[i]*strCM[i] == strCM[i-1]*sCC[i]
for (unsigned long i=1; i<strCM.size(); i++) if (strCM[i] && strCM[i-1] && sCC[i] && srCC[i])
 { // srCC 
  if ( (srCC[i]->columns() != strCM[i]->rows()) || (strCM[i-1]->columns() != sCC[i]->rows()) ) return false;
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod1 = (*srCC[i])*(*strCM[i]);
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod2 = (*strCM[i-1])*(*sCC[i]);
  if ( (*prod1) != (*prod2) ) return false;
 }
// verify sCC[i]*sbiCM[i] == sbiCM[i-1]*sbCC[i]
for (unsigned long i=1; i<sbiCM.size(); i++) if (sbiCM[i] && sbiCM[i-1] && sCC[i] && sbCC[i])
 {
  if ( (sCC[i]->columns() != sbiCM[i]->rows()) || (sbiCM[i-1]->columns() != sbCC[i]->rows()) ) return false;
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod1 = (*sCC[i])*(*sbiCM[i]);
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod2 = (*sbiCM[i-1])*(*sbCC[i]);
  if ( (*prod1) != (*prod2) ) return false; 
 }
// verify sbCC[i]*schCM[i] == (-1)*schCM[i-1]*srCC[i+1]
for (unsigned long i=1; i<schCM.size(); i++) if (schCM[i] && schCM[i-1] && srCC[i+1] && sbCC[i])
 { 
  if ( (sbCC[i]->columns() != schCM[i]->rows()) || (schCM[i-1]->columns() != srCC[i+1]->rows()) ) return false;
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod1 = (*sbCC[i])*(*schCM[i]);
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod2 = (*schCM[i-1])*(*srCC[i+1]);
  for (unsigned long j=0; j<prod1->rows(); j++) for (unsigned long k=0; k<prod1->columns(); k++)
	if (prod1->entry(j,k) + prod2->entry(j,k) != 0) return false; 
 }*/ // TODO! fix this...
return true;
}

bool NCellularData::coordinateIsomorphismsVerified(variance_type var, unsigned long coef) const
{
unsigned long aDim = ( tri3 ? 3 : 4 );
// dual to mixed, homology
for (unsigned long i=0; i<=aDim; i++)
 {
  GroupLocator dom(i, var, (var == coVariant ? STD_coord : MIX_coord), coef);
  GroupLocator ran(i, var, (var == coVariant ? MIX_coord : STD_coord), coef);
  if (!homGroup( HomLocator(dom, ran) )->isIsomorphism()) return false;
 }
// standard to mixed, homology
for (unsigned long i=0; i<=aDim; i++)
 {
  GroupLocator dom(i, var, (var == coVariant ? DUAL_coord : MIX_coord), coef);
  GroupLocator ran(i, var, (var == coVariant ? MIX_coord : DUAL_coord), coef);
  if (!homGroup( HomLocator(dom, ran) )->isIsomorphism()) return false;
 }
return true;
}

bool NCellularData::homologyLESVerified(variance_type var, unsigned long coef) const
{
unsigned long aDim = ( tri3 ? 3 : 4 );
unsigned long flag = 0;
// exactness at H_i M:                   H_i (\partial M) --> H_i M --> H_i(M,\partial M),       i == 0, ..., aDim-1
for (unsigned long i=0; i<aDim; i++) //  H^i (\partial M) <-- H^i M <-- H^i(M,\partial M)
 {
  GroupLocator middleG( i, var, STD_coord, coef );
  GroupLocator rightG( i, var, (var == coVariant ? STD_REL_BDRY_coord : STD_BDRY_coord), coef);
  GroupLocator leftG( i, var, (var == coVariant ? STD_BDRY_coord : STD_REL_BDRY_coord), coef);
  HomLocator secondMapLoc( middleG, rightG );
  HomLocator firstMapLoc( leftG, middleG );
  const NHomMarkedAbelianGroup secondMap(*homGroup(secondMapLoc));
  const NHomMarkedAbelianGroup firstMap(*homGroup(firstMapLoc));
  if (!(secondMap*firstMap).isZero()) flag=1;
  if (!(secondMap.getKernel().isIsomorphicTo( firstMap.getImage() ) ) ) flag = 2; 
  if ( (i==0) && (var==coVariant) ) if (!secondMap.isEpic()) flag = 3;  // rightmost term in LES, covariant case
  if ( (i==0) && (var==contraVariant) ) if (!firstMap.isMonic()) flag = 4; // leftmost in LES, contravariant case
 } 

// exactness at H_i(\partial M):          H_i(M,\partial M) --> H_{i-1} \partial M --> H_{i-1} M, i == 1, ..., aDim
for (unsigned long i=1; i<=aDim; i++) //  H^i(M,\partial M) <-- H^{i-1} \partial M <-- H^{i-1} M
 {
  GroupLocator middleG( i-1, var, STD_BDRY_coord, coef );
  GroupLocator rightG( (var == coVariant ? i-1 : i), var, (var == coVariant ? STD_coord : STD_REL_BDRY_coord), coef );
  GroupLocator leftG( (var == coVariant ? i : i-1), var, (var == coVariant ? STD_REL_BDRY_coord : STD_coord), coef );
  HomLocator secondMapLoc( middleG, rightG );
  HomLocator firstMapLoc( leftG, middleG );
  const NHomMarkedAbelianGroup secondMap(*homGroup(secondMapLoc));
  const NHomMarkedAbelianGroup firstMap(*homGroup(firstMapLoc));
  if (!(secondMap*firstMap).isZero()) flag=5;
  if (!(secondMap.getKernel().isIsomorphicTo( firstMap.getImage() ) ) ) flag=6;
 } 

// exactness at H_i(M, \partial M):       H_i M --> H_i(M, \partial M) --> H_{i-1} \partial M     i == 1, ..., aDim
for (unsigned long i=1; i<=aDim; i++) //  H^i M <-- H^i(M, \partial M) <-- H^{i-1} \partial M
 {
  GroupLocator middleG( i, var, STD_REL_BDRY_coord, coef );
  GroupLocator rightG( (var == coVariant ? i-1 : i), var, (var == coVariant ? STD_BDRY_coord : STD_coord), coef );
  GroupLocator leftG( (var == coVariant ? i : i-1), var, (var == coVariant ? STD_coord : STD_BDRY_coord), coef );
  HomLocator secondMapLoc( middleG, rightG );
  HomLocator firstMapLoc( leftG, middleG );
  const NHomMarkedAbelianGroup secondMap(*homGroup(secondMapLoc));
  const NHomMarkedAbelianGroup firstMap(*homGroup(firstMapLoc));
  if (!(secondMap*firstMap).isZero()) flag=7;
  if (!(secondMap.getKernel().isIsomorphicTo( firstMap.getImage() ) ) ) flag=8;
  if ( (i == aDim) && (var == coVariant) ) if (!firstMap.isMonic()) flag=9; // leftmost term in LES, coVariant case
  if ( (i == aDim) && (var == contraVariant) ) if (!secondMap.isEpic()) flag=10; // rightmost term, contravariant case
 } 

return (flag==0);
}

bool NCellularData::poincareDualityVerified() const
{ // at present this only checks to see if H_i(M) and H^{n-i}(M, boundary) are isomorphic.  Later
  // I'll add the appropriate map and this will check that that map is an isomorphism. 
unsigned long aDim = 3;
unsigned long coeff = 0;
if (tri4) { aDim = 4; if (!tri4->isOrientable()) coeff = 2; }
 else { if (!tri3->isOrientable()) coeff = 2; }

for (unsigned long i=0; i <= aDim; i++)
 {
  GroupLocator homoL( i, coVariant, DUAL_coord, coeff );
  GroupLocator cohomoL( aDim-i, contraVariant, STD_REL_BDRY_coord, coeff);
  const NHomMarkedAbelianGroup duality( *homGroup( HomLocator(homoL, cohomoL) ) );
  if (!duality.isIsomorphism()) return false;
 }

for (unsigned long i=0; i <= aDim; i++)
 {
  GroupLocator cohomoL( i, contraVariant, DUAL_coord, coeff );
  GroupLocator homoL( aDim-i, coVariant, STD_REL_BDRY_coord, coeff);
  const NHomMarkedAbelianGroup duality( *homGroup( HomLocator(cohomoL, homoL) ) );
  if (!duality.isIsomorphism()) return false;
 }

return true;
}

bool NCellularData::intersectionFormsVerified() const
{ 
bool retval=true;
unsigned long aDim = 3;
unsigned long coeff = 0;
if (tri4) { aDim = 4; if (!tri4->isOrientable()) coeff = 2; }
 else { if (!tri3->isOrientable()) coeff = 2; }
// for a n-manifold we need to check if (dual)H_1 x (std_rel_bdry)H_2 --> Z
//  has leftadj (dual)H_i --> Hom(H_{n-i}, Z) ker is torsion subgroup, and should be onto
//
for (unsigned long i=1; i<=(aDim/2); i++)
 {
  GroupLocator LDom( i, coVariant, DUAL_coord, coeff );
  GroupLocator RDom( aDim-i, coVariant, STD_REL_BDRY_coord, coeff );
  FormLocator intFloc( intersectionForm, LDom, RDom );
  const NBilinearForm intF( *bilinearForm( intFloc ) );   
  NHomMarkedAbelianGroup lHom(intF.leftAdjoint());
  if (!lHom.isEpic()) retval=false;
  NMarkedAbelianGroup ker(lHom.getKernel()); 
  // todo: add test to check kernel is the torsion subgroup
  //       in non-orientable case ker is trivial.
  if (coeff == 2) { if (!ker.isTrivial()) retval=false; }
  else 
   {
    if (ker.getRank()!= 0) { retval=false; }
    if (ker.getNumberOfInvariantFactors() == intF.ldomain().getNumberOfInvariantFactors())
     {
      for (unsigned long j=0; j<ker.getNumberOfInvariantFactors(); j++)
 	{
	 if (ker.getInvariantFactor(j) !=intF.ldomain().getInvariantFactor(j))
		{ retval = false; }
	}
     }
    else { retval = false;  }
   }
 }
return retval;
}



} // namespace regina


