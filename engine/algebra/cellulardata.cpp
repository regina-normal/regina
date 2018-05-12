
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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
#include "maths/primes.h"
#include "algebra/cellulardata.h"
#include "maths/partition.h"

#include <map>
#include <list>
#include <cmath>

namespace regina {

void correctRelOrMat( MatrixInt &CM, unsigned long domdim, 
    const Triangulation<3>* tri3, const Triangulation<4>* tri4,  
    const std::vector< std::vector<unsigned long> > &dcIx ); 
// forward reference, see ncellulardata_init.cpp for details

const AbelianGroup* CellularData::unmarkedGroup( 
                        const GroupLocator &g_desc) const
{
 unsigned long aDim = ( (tri4 != NULL) ? 4 : 3 );
 unsigned long topDim = ( (g_desc.hcs==STD_BDRY_coord) || 
    (g_desc.hcs==DUAL_BDRY_coord) || (g_desc.hcs==MIX_BDRY_coord) ) ?
     aDim-1 : aDim; // the highest dimensional cell for this cell complex.
 std::map< GroupLocator, AbelianGroup* >::const_iterator p;
 p = abelianGroups.find(g_desc);
 if (p != abelianGroups.end()) return (p->second);
 // we know there's no group matching g_desc in markedAbelianGroups. 
 // so is the request out-of-bounds? 

 if (g_desc.dim > topDim) return NULL;
 // chain complex of the form A -- ccN --> B -- ccM --> C, compute ccN and ccM
 const MatrixInt* ccN = integerChainComplex( 
                         ChainComplexLocator( g_desc.dim+1, g_desc.hcs ) );
 const MatrixInt* ccM = integerChainComplex( 
                         ChainComplexLocator( g_desc.dim, g_desc.hcs ) );
 const MatrixInt* tempMat(NULL);
 if ( g_desc.dim == 0 ) 
  {
   tempMat = new MatrixInt( 0, cellCount( 
                             ChainComplexLocator( 0, g_desc.hcs) ) );
   ccM = tempMat;
  }
 if ( g_desc.dim == topDim )
  {
   tempMat = new MatrixInt( cellCount( 
                             ChainComplexLocator( topDim, g_desc.hcs ) ), 0 );
   ccN = tempMat;
  } 
 // let's construct the group, this will be it, eventually.
 AbelianGroup* mgptr(NULL);

 if (g_desc.var == coVariant) // homology requested
  {
   if (g_desc.cof == 0) mgptr = new AbelianGroup( *ccM, *ccN );
    else mgptr = new AbelianGroup( *ccM, *ccN, Integer(g_desc.cof) );

   std::map< GroupLocator, AbelianGroup* > *mabgptr =  
 	const_cast< std::map< GroupLocator, AbelianGroup* > *> (&abelianGroups);
   mabgptr->insert(std::pair<GroupLocator,AbelianGroup*>(g_desc,mgptr)); 
  }
 else // cohomology requested
  { 
   MatrixInt ccMt( ccN->columns(), ccN->rows() );
   for (unsigned long i=0; i<ccMt.rows(); i++) 
    for (unsigned long j=0; j<ccMt.columns(); j++)
     ccMt.entry(i,j) = ccN->entry(j,i);
   MatrixInt ccNt( ccM->columns(), ccM->rows() );
   for (unsigned long i=0; i<ccNt.rows(); i++) 
    for (unsigned long j=0; j<ccNt.columns(); j++)
     ccNt.entry(i,j) = ccM->entry(j,i);
   if (g_desc.cof == 0) mgptr = new AbelianGroup( ccMt, ccNt );
    else mgptr = new AbelianGroup( ccMt, ccNt, Integer(g_desc.cof) ); 

   std::map< GroupLocator, AbelianGroup* > *mabgptr = 
	const_cast< std::map< GroupLocator, AbelianGroup* > *> (&abelianGroups);
   mabgptr->insert(std::pair<GroupLocator,AbelianGroup*>(g_desc,mgptr)); 
  }
 // clean up
 if (tempMat != NULL) delete tempMat; 
 return mgptr;
}

// todo add an aDim and ensure request is with dimension bounds
const MarkedAbelianGroup* CellularData::markedGroup( 
                            const GroupLocator &g_desc) const
{
 unsigned long aDim = ( (tri4 != NULL) ? 4 : 3 );
 unsigned long topDim = ( (g_desc.hcs==STD_BDRY_coord) || 
   (g_desc.hcs==DUAL_BDRY_coord) || (g_desc.hcs==MIX_BDRY_coord) ) ?
           aDim-1 : aDim; // the highest dimensional cell for this cell complex.
 std::map< GroupLocator, MarkedAbelianGroup* >::const_iterator p;
 p = markedAbelianGroups.find(g_desc);
 if (p != markedAbelianGroups.end()) return (p->second);
 // we know there's no group matching g_desc in markedAbelianGroups. 
 // so is the request out-of-bounds? 

 if (g_desc.dim > topDim) return NULL;
 // chain complex of the form A -- ccN --> B -- ccM --> C, compute ccN and ccM
 const MatrixInt* ccN = integerChainComplex( 
                          ChainComplexLocator( g_desc.dim+1, g_desc.hcs ) );
 const MatrixInt* ccM = integerChainComplex( 
                          ChainComplexLocator( g_desc.dim, g_desc.hcs ) );
 const MatrixInt* tempMat(NULL);
 if ( g_desc.dim == 0 ) 
  {
   tempMat = new MatrixInt( 0, cellCount( 
                             ChainComplexLocator( 0, g_desc.hcs) ) );
   ccM = tempMat;
  }
 if ( g_desc.dim == topDim )
  {
   tempMat = new MatrixInt( cellCount( 
                             ChainComplexLocator( topDim, g_desc.hcs ) ), 0 );
   ccN = tempMat;
  } 
 // let's construct the group, this will be it, eventually.
 MarkedAbelianGroup* mgptr(NULL);

 if (g_desc.var == coVariant) // homology requested
  {
   if (g_desc.cof == 0) mgptr = new MarkedAbelianGroup( *ccM, *ccN );
    else mgptr = new MarkedAbelianGroup( *ccM, *ccN, 
                        Integer(g_desc.cof) );

   std::map< GroupLocator, MarkedAbelianGroup* > *mabgptr =  
 	const_cast< std::map< GroupLocator, MarkedAbelianGroup* > *> 
        (&markedAbelianGroups);
   mabgptr->insert(std::pair<GroupLocator,MarkedAbelianGroup*>(g_desc,mgptr)); 
  }
 else // cohomology requested
  { 
   MatrixInt ccMt( ccN->columns(), ccN->rows() );
   for (unsigned long i=0; i<ccMt.rows(); i++) 
    for (unsigned long j=0; j<ccMt.columns(); j++)
     ccMt.entry(i,j) = ccN->entry(j,i);
   MatrixInt ccNt( ccM->columns(), ccM->rows() );
   for (unsigned long i=0; i<ccNt.rows(); i++) 
    for (unsigned long j=0; j<ccNt.columns(); j++)
     ccNt.entry(i,j) = ccM->entry(j,i);
   if (g_desc.cof == 0) mgptr = new MarkedAbelianGroup( ccMt, ccNt );
    else mgptr = new MarkedAbelianGroup( ccMt, ccNt, 
        Integer(g_desc.cof) ); 

   std::map< GroupLocator, MarkedAbelianGroup* > *mabgptr = 
	const_cast< std::map< GroupLocator, MarkedAbelianGroup* > *> 
        (&markedAbelianGroups);

   mabgptr->insert(std::pair<GroupLocator,MarkedAbelianGroup*>(g_desc,mgptr)); 
  }
 // clean up

 if (tempMat != NULL) delete tempMat; 
 return mgptr;
}

// TODO: coefficient LES maps like Bockstein
const HomMarkedAbelianGroup* CellularData::homGroup( 
                                const HomLocator &h_desc) const
{
 std::map< HomLocator, HomMarkedAbelianGroup* >::const_iterator p;
 p = homMarkedAbelianGroups.find(h_desc);
 if (p != homMarkedAbelianGroups.end()) return (p->second);
 // there's no group matching g_desc in markedAbelianGroups, so we make one.
 unsigned long aDim = ( tri3 ? 3 : 4 ); // ambient dimension
 // out-of-bounds request
 if ( (h_desc.domain.dim > aDim) || (h_desc.range.dim > aDim) ) return NULL;
 // inappropriate change of coefficients request
 if ( h_desc.range.cof != 0 ) 
  if ( h_desc.domain.cof % h_desc.range.cof != 0 ) return NULL;
 // leave unallocated if we can't make sense of the request.
 MatrixInt* CM(NULL); 

 if (h_desc.domain.var == h_desc.range.var) // variance-preserving map requested
  { // check if they want a pure change-of-coefficients map 
   if ( (h_desc.domain.dim == h_desc.range.dim) && 
        (h_desc.domain.hcs == h_desc.range.hcs) )
    {
      unsigned long ccdim = markedGroup( h_desc.domain )->rankCC();
      CM = new MatrixInt(ccdim, ccdim); CM->makeIdentity(); 
    }

   // check if they want a subdivision-induced map, co-variant
   if ( ( (h_desc.domain.hcs == STD_coord) || 
          (h_desc.domain.hcs == DUAL_coord) ) && 
          (h_desc.domain.var == coVariant) && 
          (h_desc.range.hcs == MIX_coord) )
         CM = ( h_desc.domain.hcs == STD_coord ? 
          clonePtr(smCM[h_desc.domain.dim]) :
	      clonePtr(dmCM[h_desc.domain.dim]) );
   if ( ( (h_desc.range.hcs == STD_coord) || 
          (h_desc.range.hcs == DUAL_coord) ) && // contravariant 
          (h_desc.domain.var == contraVariant) && 
          (h_desc.domain.hcs == MIX_coord) )
     {
	  const MatrixInt* tCMp( h_desc.range.hcs == STD_coord ? 
      smCM[h_desc.domain.dim] : dmCM[h_desc.domain.dim] );
      CM = new MatrixInt( tCMp->columns(), tCMp->rows() );
      for (unsigned long i=0; i<CM->rows(); i++) 
       for (unsigned long j=0; j<CM->columns(); j++)
	    CM->entry( i, j ) = tCMp->entry( j, i );
     }

   // check if they want a map from the homology LES of the pair (M, \partial M) 
   if ( h_desc.domain.var == coVariant )
    { // \partial M --> M
     if ( (h_desc.domain.hcs == STD_BDRY_coord) && 
          (h_desc.range.hcs == STD_coord) &&
          (h_desc.domain.dim == h_desc.range.dim) && 
          (h_desc.domain.dim < aDim) )
	 CM = clonePtr( sbiCM[h_desc.domain.dim] ); 
     // M --> (M, \partial M)
     else if ( (h_desc.domain.hcs == STD_coord) && 
               (h_desc.range.hcs == STD_REL_BDRY_coord) &&
               (h_desc.domain.dim == h_desc.range.dim) )
	 CM = clonePtr( strCM[h_desc.domain.dim] );
     // (M, \partial M) --> \partial M
     else if ( (h_desc.domain.hcs == STD_REL_BDRY_coord) && 
               (h_desc.range.hcs == STD_BDRY_coord) &&
               (h_desc.domain.dim == h_desc.range.dim+1) && 
               (h_desc.range.dim < aDim) )
	CM = clonePtr( schCM[h_desc.range.dim] );
   }
   else
    { // \partial M <-- M
     if ( (h_desc.domain.hcs == STD_coord) && 
          (h_desc.range.hcs == STD_BDRY_coord) &&
          (h_desc.domain.dim == h_desc.range.dim) && 
          (h_desc.range.dim < aDim) )
       {	 
        CM = new MatrixInt( sbiCM[h_desc.domain.dim]->columns(), 
                             sbiCM[h_desc.domain.dim]->rows() );
        for (unsigned long i=0; i<CM->rows(); i++) 
         for (unsigned long j=0; j<CM->columns(); j++)
	      CM->entry( i, j ) = sbiCM[h_desc.domain.dim]->entry( j, i );
       }
      // M <-- (M, \partial M)
      else if ( (h_desc.domain.hcs == STD_REL_BDRY_coord) && 
                (h_desc.range.hcs == STD_coord) &&
           (h_desc.domain.dim == h_desc.range.dim) )
       {	 
        CM = new MatrixInt( strCM[h_desc.domain.dim]->columns(), 
                             strCM[h_desc.domain.dim]->rows() );
        for (unsigned long i=0; i<CM->rows(); i++) 
         for (unsigned long j=0; j<CM->columns(); j++)
	      CM->entry( i, j ) = strCM[h_desc.domain.dim]->entry( j, i );
       }
      // (M, \partial M) <-- \partial M
      else if ( (h_desc.domain.hcs == STD_BDRY_coord) && 
                (h_desc.range.hcs == STD_REL_BDRY_coord) &&
                (h_desc.domain.dim+1 == h_desc.range.dim) && 
                (h_desc.domain.dim < aDim) )
       {	 
        CM = new MatrixInt( schCM[h_desc.domain.dim]->columns(), 
                             schCM[h_desc.domain.dim]->rows() );
        for (unsigned long i=0; i<CM->rows(); i++) 
         for (unsigned long j=0; j<CM->columns(); j++)
	      CM->entry( i, j ) = schCM[h_desc.domain.dim]->entry( j, i );
       }
    } 
 }
 else
 { // variance-reversing map requested
   // record if manifold orientable or not
   bool orientable = false;
   if ( tri3 ) if ( tri3->isOrientable() ) orientable = true;
   if ( tri4 ) if ( tri4->isOrientable() ) orientable = true;
   if ( (!orientable) && (h_desc.domain.cof != 2) && 
        (h_desc.range.cof != 2) ) return NULL;
 
   // check if they want Poincare Duality
   if ( (h_desc.domain.var == coVariant) && (h_desc.domain.hcs == DUAL_coord) &&
        (h_desc.range.hcs == STD_REL_BDRY_coord) && 
        (h_desc.domain.dim + h_desc.range.dim == aDim) )
   { 
    CM = new MatrixInt( numRelativeCells[ h_desc.range.dim ], 
                         numDualCells[ h_desc.domain.dim ] );
    CM->makeIdentity(); // correct if orientable. 
    if (orientable) correctRelOrMat( *CM, h_desc.domain.dim, tri3, tri4, dcIx );
   } else 
   if ( (h_desc.domain.var == contraVariant) && 
        (h_desc.domain.hcs == DUAL_coord) &&
        (h_desc.range.hcs == STD_REL_BDRY_coord) && 
        (h_desc.domain.dim + h_desc.range.dim == aDim) )
   { 
    CM = new MatrixInt( numRelativeCells[ h_desc.range.dim ], 
                         numDualCells[ h_desc.domain.dim ] );
    CM->makeIdentity(); // correct if orientable. 
    if (orientable) correctRelOrMat( *CM, h_desc.domain.dim, tri3, tri4, dcIx );   
   }
 }

 HomMarkedAbelianGroup* hmgptr(NULL);

 if ( CM ) // make sure we have the domain and range, then we're happy.
 {
  //  ensure we have domain and range
  const MarkedAbelianGroup* dom = markedGroup( h_desc.domain );
  const MarkedAbelianGroup* ran = markedGroup( h_desc.range );

  if ( dom && ran )
   {
    hmgptr = new HomMarkedAbelianGroup( *dom, *ran, *CM );
    std::map< HomLocator, HomMarkedAbelianGroup* > *hmabgptr = 
     const_cast< std::map< HomLocator, HomMarkedAbelianGroup* > *> 
      (&homMarkedAbelianGroups);
    hmabgptr->insert(std::pair<HomLocator,HomMarkedAbelianGroup*>
        (h_desc,hmgptr)); 
    delete CM;
    return hmgptr;
   }
 }
 if ( CM ) delete CM;

 // didn't find what was requested
 return NULL;
}

SVPolynomialRing< Integer > CellularData::poincarePolynomial() const
{
 SVPolynomialRing< Integer > retval;
 unsigned long aDim( tri3 ? 3 : 4 );
 for (unsigned long i=0; i<=aDim; i++) retval += 
  SVPolynomialRing< Integer >( Integer( unmarkedGroup( 
                 GroupLocator(i, coVariant, DUAL_coord, 0))->rank() ), i );
return retval;
}


/**
 *  Computes various bilinear forms associated to the homology of the manifold:
 *
 *  1) Homology-Cohomology pairing <.,.>  
 *          ie: H_i(M;R) x H^i(M;R) --> R  where R is the coefficients
 *  2) Intersection product   ie: H_i(M;R) x H_j(M;R) --> H_{(i+j)-n}(M;R)
 *  3) Torsion linking form   ie: H_i(M;Z) x H_j(M;Z) --> H_{(i+j)-(n-1)}(M;Q/Z)
 *  4) cup products           ie: H^i(M;R) x H^j(M;R) --> H^{i+j}(M;R)
 *
 *  What has been implemented:
 *
 *  ALL of (1) and (2). 
 *
 *  3) 3-dimensional torsion linking form
 *
 *  4) nothing
 *
 *  TODO:
 *
 *  (2) for new coordinate systems MIX_BDRY_coord, MIX_REL_BDRY_coord, 
 *                                 DUAL_BDRY_coord, DUAL_REL_BDRY_coord
 *
 *  3) aDim == 4:  (2,1)->0, (1,2)->0
 *
 *  4) all -- implement via Poincare duality once (2) complete
 */
const BilinearForm* CellularData::bilinearForm( 
                        const FormLocator &f_desc ) const
{
 unsigned long aDim( tri3 ? 3 : 4 );
 BilinearForm* bfptr(NULL);

 std::map< FormLocator, BilinearForm* >::const_iterator p;
 p = bilinearForms.find(f_desc);
 if (p != bilinearForms.end()) return (p->second);
 // there's no form matching f_desc in bilinearForms, so we make one.

 // case 1: homology-cohomology pairing
 if ( ( f_desc.ft == evaluationForm ) &&
      ( f_desc.ldomain.dim == f_desc.rdomain.dim ) && 
      ( f_desc.ldomain.var != f_desc.rdomain.var ) &&
      ( f_desc.ldomain.cof == f_desc.rdomain.cof ) && 
      ( f_desc.ldomain.hcs == f_desc.rdomain.hcs ) ) 
  { // homology-cohomology pairing definable.
   const MarkedAbelianGroup* lDom( markedGroup(f_desc.ldomain) );
   const MarkedAbelianGroup* rDom( markedGroup(f_desc.rdomain) );
   MarkedAbelianGroup rAng( 1, f_desc.rdomain.cof );

   NSparseGridRing< Integer > intM(3); 
   NMultiIndex< unsigned long > x(3); 

   for (unsigned long i=0; i<lDom->rankCC(); i++)
    { x[0] = i; x[1] = i; x[2] = 0;
      intM.setEntry( x, 1 ); } 

   bfptr = new BilinearForm( *lDom, *rDom, rAng, intM );
   std::map< FormLocator, BilinearForm* > *mbfptr = 
      const_cast< std::map< FormLocator, BilinearForm* > *> (&bilinearForms);
   mbfptr->insert( std::pair<FormLocator, BilinearForm*>(f_desc, bfptr) );
   return bfptr; 
  } // end case 1

 // case 2: intersection products i+j >= n == aDim
 if ( ( f_desc.ft == intersectionForm ) &&
      ( f_desc.ldomain.var == coVariant ) && 
      ( f_desc.rdomain.var == coVariant) &&
      ( f_desc.ldomain.dim + f_desc.rdomain.dim >= aDim ) &&
      ( (f_desc.ldomain.dim + f_desc.rdomain.dim) - aDim < aDim - 1 ) &&
      ( f_desc.ldomain.dim > 0) && ( f_desc.rdomain.dim > 0 ) &&
      ( f_desc.ldomain.cof == f_desc.rdomain.cof ) &&
      ( f_desc.ldomain.hcs == DUAL_coord ) && 
      ( f_desc.rdomain.hcs == STD_REL_BDRY_coord ) )
  {// check its orientable if R != Z_2
   if ( (f_desc.ldomain.cof != 2) && 
        ( tri3 ? !tri3->isOrientable() : !tri4->isOrientable() ) ) return NULL;
   const MarkedAbelianGroup* lDom( markedGroup(f_desc.ldomain) );
   const MarkedAbelianGroup* rDom( markedGroup(f_desc.rdomain) );
   const MarkedAbelianGroup* rAng( markedGroup( 
         GroupLocator( (f_desc.ldomain.dim + f_desc.rdomain.dim) - aDim,
					coVariant, MIX_coord, f_desc.ldomain.cof ) ) );
   NSparseGridRing< Integer > intM(3); 
   
   if (aDim == 3) // aDim==3  
    {
      // (dual)H_2 x (std_rel)H_2 --> (mix)H_1
     if ( (f_desc.ldomain.dim == 2) && (f_desc.rdomain.dim == 2) )
       for (unsigned long i=0; i<numRelativeCells[2]; i++)
        { // each STD_REL_BDRY cell has <= 3 boundary 1-cells, 
          // each one corresponds to a DUAL cell...
         const Face<3,2>* fac( tri3->triangle( rIx[2][i] ) ); 
         const Face<3,1>* edg(NULL);
         const Simplex<3>* tet( fac->embedding(1).tetrahedron() );
         for (unsigned long j=0; j<3; j++)
	  {
	   edg = fac->edge(j); if (!edg->isBoundary())
	    { // intM[ J, i, 2*numNonIdealCells[2] + 3*i+j ] += whatever.  
          // For orientation we need to compare normal orientation 
          // of these edges to product normal orientations
          unsigned long J( lower_bound( dcIx[2].begin(), dcIx[2].end(), 
           edg->index() ) - dcIx[2].begin() );
	     NMultiIndex< unsigned long > x(3); x[0] = J; 
         x[1] = i; x[2] = 2*numNonIdealCells[1] + 3*rIx[2][i] + j;
	     // fac->edgeMapping(j)[0] and [1] are the vertices of the edge 
         // in the face, so we apply facinc to that, 
         // then get the corresp edge number
	     Perm<4> facinc( fac->embedding(1).vertices() );
	     Perm<4> edginc( tet->edgeMapping( 
           Face<3,1>::edgeNumber[facinc[(j+1)%3]][facinc[(j+2)%3]] ) );
         // edginc[2,3] give orientation of part of dual 2-cell in this tet...
	     // normalize edginc to ambient orientation
	     if (tet->orientation() != edginc.sign()) edginc = edginc * Perm<4>(0,1);
	     int inoutor = ((tet->orientation() == facinc.sign()) ? 1 : -1);
	     Perm<4> dualor( facinc[j], edginc[0], edginc[1], facinc[3]);           
             intM.setEntry( x, dualor.sign()*inoutor*tet->orientation() );
	    } } }
      // (dual)H_2 x (std_rel)H_1 --> (mix)H_0
     if ( (f_desc.ldomain.dim == 2) && (f_desc.rdomain.dim == 1) )
       for (unsigned long i=0; i<numRelativeCells[1]; i++)
        {
         const Face<3,1>* edg( tri3->edge( rIx[1][i] ) ); 
         const Simplex<3>* tet( edg->embedding(0).tetrahedron() );
         unsigned long J( lower_bound( dcIx[2].begin(), dcIx[2].end(), 
          rIx[1][i] ) - dcIx[2].begin() );
         NMultiIndex< unsigned long > x(3); x[0] = J; x[1] = i; 
         x[2] = numNonIdealCells[0] + i;
         Perm<4> edginc( edg->embedding(0).vertices() );
         intM.setEntry( x, edginc.sign()*tet->orientation() ); 
        }
      // (dual)H_1 x (std_rel)H_2 --> (mix)H_0 
     if ( (f_desc.ldomain.dim == 1) && (f_desc.rdomain.dim == 2) )
       for (unsigned long i=0; i<numRelativeCells[2]; i++)
        {
         const Face<3,2>* fac( tri3->triangle( rIx[2][i] ) ); 
         const Simplex<3>* tet( fac->embedding(0).tetrahedron() );
         unsigned long J( lower_bound( dcIx[1].begin(), dcIx[1].end(), 
          rIx[2][i] ) - dcIx[1].begin() );
         NMultiIndex< unsigned long > x(3); x[0] = J; 
         x[1] = i; x[2] = numNonIdealCells[0] + numNonIdealCells[1] + i;
         Perm<4> facinc( fac->embedding(0).vertices() );
         intM.setEntry( x, facinc.sign()*tet->orientation() ); 
        }

     bfptr = new BilinearForm( *lDom, *rDom, *rAng, intM );
     std::map< FormLocator, BilinearForm* > *mbfptr = 
      const_cast< std::map< FormLocator, BilinearForm* > *> (&bilinearForms);
     mbfptr->insert( std::pair<FormLocator, BilinearForm*>(f_desc, bfptr) );
     return bfptr; 
    } // end aDim == 3
   
   if ( aDim == 4 ) // aDim==4 
    { // H_2 x H_2 --> H_0
     if ( (f_desc.ldomain.dim == 2) && (f_desc.rdomain.dim == 2) ) 
       for (unsigned long i=0; i<numRelativeCells[2]; i++)
        {
         const Face<4,2>* fac( tri4->triangle( rIx[2][i] ) );
         const Simplex<4>* pen( fac->embedding(0).pentachoron() );
         Perm<5> facinc( fac->embedding(0).vertices() );
         unsigned long J( lower_bound( dcIx[2].begin(), dcIx[2].end(), 
          rIx[2][i] ) - dcIx[2].begin() );
         NMultiIndex< unsigned long > x(3); x[0] = J; 
         x[1] = i; x[2] = numNonIdealCells[0] + numNonIdealCells[1] + i;
         intM.setEntry( x, facinc.sign() * pen->orientation() );
        }
     // (dual)H_1 x (std_rel_bdry)H_3 --> H_0
     if ( (f_desc.ldomain.dim == 1) && (f_desc.rdomain.dim == 3) )
       for (unsigned long i=0; i<numRelativeCells[3]; i++)
        {
         const Face<4,3>* tet( tri4->tetrahedron( rIx[3][i] ) );
         const Simplex<4>* pen( tet->embedding(0).pentachoron() );
         Perm<5> tetinc( tet->embedding(0).vertices() );
         unsigned long J( lower_bound( dcIx[1].begin(), dcIx[1].end(), 
          rIx[3][i] ) - dcIx[1].begin() );
         NMultiIndex< unsigned long > x(3); x[0] = J; 
         x[1] = i; x[2] = numNonIdealCells[0] + numNonIdealCells[1] + 
          numNonIdealCells[2] + i;
         intM.setEntry( x, tetinc.sign() * pen->orientation() );
        }
     // (dual)H_3 x (std_rel_bdry)H_1 --> H_0
     if ( (f_desc.ldomain.dim == 3) && (f_desc.rdomain.dim == 1) )
      for (unsigned long i=0; i<numRelativeCells[1]; i++)
        {
         const Face<4,1>* edg( tri4->edge( rIx[1][i] ) );
         const Simplex<4>* pen( edg->embedding(0).pentachoron() );
         Perm<5> edginc( edg->embedding(0).vertices() );
         unsigned long J( lower_bound( dcIx[3].begin(), dcIx[3].end(), 
          rIx[1][i] ) - dcIx[3].begin() );
         NMultiIndex< unsigned long > x(3); x[0] = J; 
         x[1] = i; x[2] = numNonIdealCells[0] + numNonIdealCells[1] + 
          numNonIdealCells[2] + i;
         intM.setEntry( x, edginc.sign() * pen->orientation() );
        }
     // (dual)H_2 x (std_rel_bdry)H_3 --> H_1
     if ( (f_desc.ldomain.dim == 2) && (f_desc.rdomain.dim == 3) )
       for (unsigned long i=0; i<numRelativeCells[3]; i++)
        { // each STD_REL_BDRY cell has <= 3 boundary 1-cells, 
          // each one corresponds to a DUAL cell...
         const Face<4,3>* tet( tri4->tetrahedron( rIx[3][i] ) ); 
         const Face<4,2>* fac(NULL);
         const Simplex<4>* pen( tet->embedding(1).pentachoron() );
         Perm<5> tetinc( tet->embedding(1).vertices() );
         for (unsigned long j=0; j<4; j++)
	  {
	   fac = tet->triangle(j); if (!fac->isBoundary())
	    { // intM[ J, i, 2*numNonIdealCells[1] + 3*numNonIdealCells[2] + 4*i+j ]
          //  += whatever for orientation we need to compare normal 
          // orientation of intersection to product normal orientations
             unsigned long J( lower_bound( dcIx[2].begin(), dcIx[2].end(), 
                              fac->index() ) - dcIx[2].begin() );
	     NMultiIndex< unsigned long > x(3); 
         x[0] = J; x[1] = i; 
         x[2] = 2*numNonIdealCells[1] + 3*numNonIdealCells[2] + 4*rIx[3][i] + j;
	     Perm<5> facinc( pen->triangleMapping( 
         Face<4,2>::triangleNumber[tetinc[(j+1)%4]][tetinc[(j+2)%4]]
          [tetinc[(j+3)%4]] ) ); 
         // adjust for coherent oriented normal fibres
	     if (facinc.sign() != pen->orientation()) facinc=facinc*Perm<5>(0,1);
	     int inoutor = ( (tetinc.sign() == pen->orientation()) ? 1 : -1 );	     
         // the intersection is the edge from the centre of tet to the 
         // centre of fac so the intersection edge + normal orientation 
         // is represented by tetinc[j],  facinc[0,1,2], tetnum in pen
	     Perm<5> dualor( tetinc[j], facinc[0], facinc[1], facinc[2],  
             tet->embedding(1).tetrahedron() );           
         intM.setEntry( x, dualor.sign()*pen->orientation()*inoutor ); 
     } } }
     // (dual)H_3 x (std_red_bdry)H_2 --> H_1
     if ( (f_desc.ldomain.dim == 3) && (f_desc.rdomain.dim == 2) )
       for (unsigned long i=0; i<numRelativeCells[2]; i++)
        { // each STD_REL_BDRY cell has <= 3 boundary 1-cells, 
          // each one corresponds to a DUAL cell...
         const Face<4,2>* fac( tri4->triangle( rIx[2][i] ) ); 
         const Face<4,1>* edg(NULL);
         const Simplex<4>* pen( fac->embedding(0).pentachoron() );
         Perm<5> facinc( fac->embedding(0).vertices() );
         for (unsigned long j=0; j<3; j++)
	  {
	   edg = fac->edge(j); if (!edg->isBoundary())
	    { // intM[ J, i, 2*numNonIdealCells[1] + 3*numNonIdealCells[2] + 
          // 4*i+j ] += whatever for orientation we need to compare normal 
          // orientation of intersection to product normal orientations
             unsigned long J( lower_bound( dcIx[3].begin(), dcIx[3].end(), 
                              edg->index() ) - dcIx[3].begin() );
	     NMultiIndex< unsigned long > x(3); 
         x[0] = J; x[1] = i; x[2] = 2*numNonIdealCells[1] + 3*rIx[2][i] + j; 
	     Perm<5> edginc( pen->edgeMapping( 
           Face<4,1>::edgeNumber[facinc[(j+1)%3]][facinc[(j+2)%3]] ) ); 
	     // adjust for coherent oriented normal fibres
	     if (facinc.sign() != pen->orientation()) facinc = facinc*Perm<5>(3,4);
	     if (edginc.sign() != pen->orientation()) edginc = edginc*Perm<5>(0,1);
	     // the intersection is the edge from the centre of fac to the centre 
         // of edg. so the intersection edge + normal orientation is 
         // represented by facinc[j], edginc[0,1], facinc[3,4]

	     Perm<5> dualor( facinc[j], edginc[0], edginc[1], facinc[3],  facinc[4]); 
         intM.setEntry( x, dualor.sign()*pen->orientation() );
     } } }

     // (dual)H_3 x (std_rel_bdry)H_3 --> H_2
     if ( (f_desc.ldomain.dim == 3) && (f_desc.rdomain.dim == 3) )
       for (unsigned long i=0; i<numRelativeCells[3]; i++)
        { // each STD_REL_BDRY cell has <= 3 boundary 1-cells, each one 
          // corresponds to a DUAL cell...
         const Face<4,3>* tet( tri4->tetrahedron( rIx[3][i] ) ); 
         const Face<4,1>* edg(NULL);
         const Simplex<4>* pen( tet->embedding(1).pentachoron() );
         Perm<5> tetinc( tet->embedding(1).vertices() ); 
         // [0,1,2,3]->tet in pen, 4->tet num in pen.
         for (unsigned long j=0; j<6; j++)
	  {
	   edg = tet->edge(j); if (!edg->isBoundary())
	    { // intM[ J, i, 2*numNonIdealCells[1] + 3*numNonIdealCells[2] + 4*i+j ]
          //  += whatever.  for orientation we need to compare normal 
          // orientation of intersection to product normal orientations
         unsigned long J( lower_bound( dcIx[3].begin(), dcIx[3].end(), 
                          edg->index() ) - dcIx[3].begin() );
	     NMultiIndex< unsigned long > x(3); x[0] = J; x[1] = i; 
         x[2] = 3*numNonIdealCells[2] + 6*i + j;
	     Perm<5> edgintet( tet->edgeMapping( j ) ); 
         // [0,1] --> verts in tet, 4->4. 
         Perm<5> ordual2cell( tetinc * edgintet ); 
         // [0,1] --> verts in pen, 4->tet in pen
	     Perm<5> edginc( pen->edgeMapping( 
                       Face<4,1>::edgeNumber[ordual2cell[0]][ordual2cell[1]] ) );
	     // adjust for coherent oriented normal fibres
	     if (edginc.sign() != pen->orientation()) edginc = edginc*Perm<5>(0,1);
	     int inoutor = ( (tetinc.sign() == pen->orientation()) ? 1 : -1 );	     
         // intersection is the mixed 2-cell corresp to dual 2-cell in tet, 
         // dual to edge j. combined orientation Perm<5> consists of 
         //  tetinc[ Face<3,1>::ordering[j][2], Face<3,1>::ordering[j][3] ], 
	     //  edginc[0], edginc[1], tet->embedding(1).tetrahedron()

	     Perm<5> dualor( ordual2cell[2], ordual2cell[3], edginc[0], edginc[1], 
			    tet->embedding(1).tetrahedron() ); 
         intM.setEntry( x, dualor.sign()*pen->orientation()*inoutor );
     } } }

     bfptr = new BilinearForm( *lDom, *rDom, *rAng, intM );
     std::map< FormLocator, BilinearForm* > *mbfptr = 
      const_cast< std::map< FormLocator, BilinearForm* > *> (&bilinearForms);
     mbfptr->insert( std::pair<FormLocator, BilinearForm*>(f_desc, bfptr) );
     return bfptr; 
    } // end aDim == 4
  } // end DUAL x STD_REL_BDRY intersectionform loop

 // convienience intersection product pairings
 if ( ( f_desc.ft == intersectionForm ) && 
      ( f_desc.ldomain.var == coVariant ) && 
      ( f_desc.rdomain.var == coVariant ) && 
      ( f_desc.ldomain.dim + f_desc.rdomain.dim >= aDim ) &&       
      ( (f_desc.ldomain.dim + f_desc.rdomain.dim) - aDim < aDim - 1 ) &&
      ( f_desc.ldomain.dim > 0) && ( f_desc.rdomain.dim > 0 ) && 
      ( f_desc.ldomain.cof == f_desc.rdomain.cof ) &&
      ( f_desc.ldomain.hcs == DUAL_coord ) && 
      ( f_desc.rdomain.hcs == DUAL_coord ) )
  { // convienience pairing -- the DUAL x DUAL --> MIX pairing
   GroupLocator dc( f_desc.rdomain.dim, coVariant, DUAL_coord,         
       f_desc.rdomain.cof );
   GroupLocator mc( f_desc.rdomain.dim, coVariant, MIX_coord, 
       f_desc.rdomain.cof );
   GroupLocator sc( f_desc.rdomain.dim, coVariant, STD_coord,          
       f_desc.rdomain.cof );
   GroupLocator sb( f_desc.rdomain.dim, coVariant, STD_REL_BDRY_coord, 
       f_desc.rdomain.cof );
   const HomMarkedAbelianGroup* sc_sb(homGroup( HomLocator( sc, sb ) ) );
   const HomMarkedAbelianGroup* sc_mc(homGroup( HomLocator( sc, mc ) ) );
   const HomMarkedAbelianGroup* dc_mc(homGroup( HomLocator( dc, mc ) ) );
   std::unique_ptr<HomMarkedAbelianGroup> f( 
     (*sc_sb) * *(*(sc_mc->inverseHom()) * (*dc_mc)) );
   FormLocator prim(f_desc); prim.rdomain.hcs = STD_REL_BDRY_coord;
   bfptr = new BilinearForm( bilinearForm(prim)->rCompose(*f) );
   std::map< FormLocator, BilinearForm* > *mbfptr = 
    const_cast< std::map< FormLocator, BilinearForm* > *> (&bilinearForms);
   mbfptr->insert( std::pair<FormLocator, BilinearForm*>(f_desc, bfptr) );
   return bfptr; 
  }
 if ( ( f_desc.ft == intersectionForm ) && 
      ( f_desc.ldomain.var == coVariant ) && 
      ( f_desc.rdomain.var == coVariant ) && 
      ( f_desc.ldomain.dim + f_desc.rdomain.dim >= aDim ) &&
      ( (f_desc.ldomain.dim + f_desc.rdomain.dim) - aDim < aDim - 1 ) &&
      ( f_desc.ldomain.dim > 0) && ( f_desc.rdomain.dim > 0 ) && 
      ( f_desc.ldomain.cof == f_desc.rdomain.cof ) &&
      ( f_desc.ldomain.hcs == STD_coord ) && 
      ( f_desc.rdomain.hcs == STD_REL_BDRY_coord) )
  { // convienience pairing -- the STD x STD_REL_BDRY --> MIX pairing
   GroupLocator dc( f_desc.rdomain.dim, coVariant, DUAL_coord,         
    f_desc.rdomain.cof );
   GroupLocator mc( f_desc.rdomain.dim, coVariant, MIX_coord,          
    f_desc.rdomain.cof );
   GroupLocator sc( f_desc.rdomain.dim, coVariant, STD_coord,          
    f_desc.rdomain.cof );
   const HomMarkedAbelianGroup* sc_mc(homGroup( HomLocator( sc, mc ) ) );
   const HomMarkedAbelianGroup* dc_mc(homGroup( HomLocator( dc, mc ) ) );
   std::unique_ptr<HomMarkedAbelianGroup> f( (*dc_mc->inverseHom()) * (*sc_mc) );
   FormLocator prim(f_desc); prim.ldomain.hcs = DUAL_coord;
   bfptr = new BilinearForm( bilinearForm(prim)->lCompose(*f) );
   std::map< FormLocator, BilinearForm* > *mbfptr = 
    const_cast< std::map< FormLocator, BilinearForm* > *> (&bilinearForms);
   mbfptr->insert( std::pair<FormLocator, BilinearForm*>(f_desc, bfptr) );
   return bfptr; 
  }

 // case 3: torsion linking forms
 if ( ( f_desc.ft == torsionlinkingForm) && 
      ( f_desc.ldomain.var == coVariant ) && 
      ( f_desc.rdomain.var == coVariant) && 
      ( f_desc.ldomain.dim + f_desc.rdomain.dim + 1 == aDim ) && 
      ( f_desc.ldomain.dim > 0) &&       
      ( f_desc.rdomain.dim > 0 ) && ( f_desc.ldomain.cof == 0 ) && 
      ( f_desc.rdomain.cof == 0 ) && 
      ( f_desc.ldomain.hcs == DUAL_coord ) && 
      ( f_desc.rdomain.hcs == STD_REL_BDRY_coord) )
  { // TLF step 1: construct range, ldomain and rdomain.  Make range Z_n 
    // where n=gcd of the order of the torsion subgroups of ldomain and rdomain
    // ie: n == gcd(a,b) a, b largest inv. facs ldomain,rdomain.
   GroupLocator ldd( f_desc.ldomain.dim, f_desc.ldomain.var, 
     f_desc.ldomain.hcs, f_desc.ldomain.cof );
   GroupLocator rdd( f_desc.rdomain.dim, f_desc.rdomain.var, 
     f_desc.rdomain.hcs, f_desc.rdomain.cof );
   const MarkedAbelianGroup* ld(markedGroup(ldd));     
   const MarkedAbelianGroup* rd(markedGroup(rdd));
   // now we build ldomain and rdomain
   std::unique_ptr<MarkedAbelianGroup> ldomain = 
    ld->torsionSubgroup(); // triv pres torsion subgroups
   std::unique_ptr<MarkedAbelianGroup> rdomain = rd->torsionSubgroup(); // ...

   Integer N(Integer::one);
   if ( !ldomain->isTrivial() && !rdomain->isTrivial() ) 
      N=ld->invariantFactor( ld->countInvariantFactors()-1 ).gcd(
        rd->invariantFactor( rd->countInvariantFactors()-1 ) );
   MarkedAbelianGroup range( 1, N ); 
   // Z_N with triv pres 0 --> Z --N--> Z --> Z_N --> 0
   NSparseGridRing< Integer > intM(3); 

   // TLF step 2: dimension-specific constructions
   if (aDim == 3)
    {
     for (unsigned long i=0; i<ld->countInvariantFactors(); i++)
 	  for (unsigned long j=0; j<rd->countInvariantFactors(); j++)
	   {
	   // take ccRep(j), multiply by order rd->invariantFactor(j), 
       //  apply writeAsBoundary, 
       std::vector< Integer > rFac( rd->torsionRep(j) );
       for (unsigned long k=0; k<rFac.size(); k++) 
        rFac[k]*=rd->invariantFactor(j);
       std::vector< Integer > std_rel_bdry_2vec( 
        rd->writeAsBoundary( rFac ) );
       std::vector< Integer > dual_1vec( ld->torsionRep(i) );
	   // intersect with ld->invariantFactor(i)
	   Integer sum(Integer::zero);
       for (unsigned long k=0; k<dual_1vec.size(); k++)
        {
         const Face<3,2>* fac( tri3->triangle( rIx[2][k] ) ); 
         // shouldn't this be k? previously i
         const Simplex<3>* tet( fac->embedding(0).tetrahedron() );
         Perm<4> facinc( fac->embedding(0).vertices() );
         sum += std_rel_bdry_2vec[k]*dual_1vec[k]*
                facinc.sign()*tet->orientation(); 
         // orientation convention...
        }
       // rescale sum, check if relevant, append to intM if so...
       sum *= (N / rd->invariantFactor(j));
       sum %= N; if (sum < Integer::zero) sum += N;
       NMultiIndex< unsigned long > x(3); x[0] = i; x[1] = j; x[2] = 0; 
       if (sum != Integer::zero) intM.setEntry( x, sum );
	   } 
     }
    
    if ( (aDim == 4) && (f_desc.ldomain.dim == 2) )
     {
	for (unsigned long i=0; i<ld->countInvariantFactors(); i++)
 	 for (unsigned long j=0; j<rd->countInvariantFactors(); j++)
	  {
	   // take ccRep(j), multiply by order rd->invariantFactor(j), 
       //  apply writeAsBoundary, 
       std::vector< Integer > rFac( rd->torsionRep(j) );
       for (unsigned long k=0; k<rFac.size(); k++) 
        rFac[k]*=rd->invariantFactor(j);
       std::vector< Integer > std_rel_bdry_2vec( 
                                       rd->writeAsBoundary( rFac ) );
       std::vector< Integer > dual_1vec( ld->torsionRep(i) );
	   // intersect with ld->invariantFactor(i)
	   Integer sum(Integer::zero);
           for (unsigned long k=0; k<dual_1vec.size(); k++)
            {
             const Face<4,2>* fac( tri4->triangle( rIx[2][i] ) ); 
             const Simplex<4>* pen( 
                fac->embedding(0).pentachoron() );
             Perm<5> facinc( fac->embedding(0).vertices() );
             sum += std_rel_bdry_2vec[k]*dual_1vec[k]*
                    facinc.sign()*pen->orientation(); 
             // orientation convention...
            }
           // rescale sum, check if relevant, append to intM if so...
           sum *= (N / rd->invariantFactor(j));
           sum %= N; if (sum < Integer::zero) sum += N;
           NMultiIndex< unsigned long > x(3); x[0] = i; x[1] = j; x[2] = 0; 
           if (sum != Integer::zero) intM.setEntry( x, sum );
	  }
     } 
    if ( (aDim == 4) && (f_desc.ldomain.dim == 1) )
     {
 	for (unsigned long i=0; i<ld->countInvariantFactors(); i++)
 	 for (unsigned long j=0; j<rd->countInvariantFactors(); j++)
	  {
	   // take ccRep(j), multiply by order rd->invariantFactor(j), 
       // apply writeAsBoundary, 
       std::vector< Integer > rFac( rd->torsionRep(j) );
       for (unsigned long k=0; k<rFac.size(); k++) 
            rFac[k]*=rd->invariantFactor(j);
       std::vector< Integer > std_rel_bdry_2vec( 
            rd->writeAsBoundary( rFac ) );
       std::vector< Integer > dual_1vec( ld->torsionRep(i) );
	   // intersect with ld->invariantFactor(i)
	   Integer sum(Integer::zero);
       for (unsigned long k=0; k<dual_1vec.size(); k++)
        {
         const Face<4,3>* tet( tri4->tetrahedron( rIx[1][i] ) ); 
         const Simplex<4>* pen( tet->embedding(1).pentachoron() );
         Perm<5> tetinc( tet->embedding(1).vertices() );
         sum += std_rel_bdry_2vec[k]*dual_1vec[k]*
                tetinc.sign()*pen->orientation(); 
         // orientation convention...
        }
       // rescale sum, check if relevant, append to intM if so...
       sum *= (N / rd->invariantFactor(j));
       sum %= N; if (sum < Integer::zero) sum += N;
       NMultiIndex< unsigned long > x(3); x[0] = i; x[1] = j; x[2] = 0; 
       if (sum != Integer::zero) intM.setEntry( x, sum );	  }
    } 
    bfptr = new BilinearForm( *ldomain, *rdomain, range, intM );
    std::map< FormLocator, BilinearForm* > *fptr = 
      const_cast< std::map< FormLocator, BilinearForm* > *> (&bilinearForms);
    fptr->insert( std::pair<FormLocator, BilinearForm*>(f_desc, bfptr) );
    return bfptr; 
  }

 // convienience torsion linking pairings
 if ( ( f_desc.ft == torsionlinkingForm ) && 
      ( f_desc.ldomain.var == coVariant ) && 
      ( f_desc.rdomain.var == coVariant ) && 
      ( f_desc.ldomain.dim + f_desc.rdomain.dim + 1 == aDim ) &&
      ( f_desc.ldomain.dim > 0) && ( f_desc.rdomain.dim > 0 ) &&
      ( f_desc.ldomain.cof == 0 ) && ( f_desc.rdomain.cof == 0 ) &&
      ( f_desc.ldomain.hcs == DUAL_coord ) && 
      ( f_desc.rdomain.hcs == DUAL_coord) )
  { // convienience pairing -- the DUAL x DUAL pairing
    // the natural pairing is in DUAL x STD_REL_BDRY coords, 
    // so we provide the change of coordinates...
    GroupLocator dc( f_desc.rdomain.dim, coVariant, DUAL_coord,
     f_desc.rdomain.cof );
    GroupLocator mc( f_desc.rdomain.dim, coVariant, MIX_coord,
     f_desc.rdomain.cof );
    GroupLocator sc( f_desc.rdomain.dim, coVariant, STD_coord,
     f_desc.rdomain.cof );
    GroupLocator sb( f_desc.rdomain.dim, coVariant, STD_REL_BDRY_coord,
     f_desc.rdomain.cof );
    const HomMarkedAbelianGroup* sc_sb(homGroup( HomLocator( sc, sb ) ) );
    const HomMarkedAbelianGroup* sc_mc(homGroup( HomLocator( sc, mc ) ) );
    const HomMarkedAbelianGroup* dc_mc(homGroup( HomLocator( dc, mc ) ) );
    std::unique_ptr<MarkedAbelianGroup> rtrivG(
      dc_mc->domain().torsionSubgroup() );
    // trivially presented dual torsion subgroup 
    std::unique_ptr<HomMarkedAbelianGroup> rinc(
     dc_mc->domain().torsionInclusion() );
    // triv pres dual torsion inclusion     
    std::unique_ptr<MarkedAbelianGroup> ltrivG(
     sc_sb->range().torsionSubgroup() );
    // triv pres std torsion subgroup

    MatrixInt lMap( sc_sb->range().countInvariantFactors(), 
      sc_sb->range().rankCC() );    
    for (unsigned long j=0; j<lMap.columns(); j++)
    	{
         std::vector<Integer> jtor( sc_sb->range().snfRep( 
            sc_sb->range().cycleProjection( j ) ) );
         for (unsigned long i=0; i<lMap.rows(); i++)
          lMap.entry( i, j ) = jtor[i];
    	}
    HomMarkedAbelianGroup lproj( sc_sb->range(), *ltrivG, lMap );
    // map std-rel-bdry to std coord

    std::unique_ptr<HomMarkedAbelianGroup> f(
      lproj * *((*sc_sb) * *((*sc_mc->inverseHom()) * *((*dc_mc) * (*rinc) )))); 
    // dual -> std_rel_bdry
    FormLocator prim(f_desc); prim.rdomain.hcs = STD_REL_BDRY_coord;

    bfptr = new BilinearForm( bilinearForm(prim)->rCompose(*f) ); 
    std::map< FormLocator, BilinearForm* > *mbfptr =             
     const_cast< std::map< FormLocator, BilinearForm* > *> (&bilinearForms);
    mbfptr->insert( std::pair<FormLocator, BilinearForm*>(f_desc, bfptr) );
     return bfptr; 
  }
 if ( ( f_desc.ft == torsionlinkingForm ) && 
      ( f_desc.ldomain.var == coVariant ) && 
      ( f_desc.rdomain.var == coVariant) && 
      ( f_desc.ldomain.dim + f_desc.rdomain.dim + 1 == aDim ) &&
      ( f_desc.ldomain.dim > 0) && ( f_desc.rdomain.dim > 0 ) &&
      ( f_desc.ldomain.cof == 0 ) && ( f_desc.rdomain.cof == 0 ) &&
      ( f_desc.ldomain.hcs == STD_coord ) && 
      ( f_desc.rdomain.hcs == STD_coord) )
  { // convienience pairing -- the STD x STD pairing
    // natural pairing is DUAL x STD_REL_BDRY coords, so we need
    //  DUAL -> STD and STD -> STD_REL_BDRY maps
    GroupLocator dc( f_desc.rdomain.dim, coVariant, DUAL_coord,
      f_desc.rdomain.cof );
    GroupLocator mc( f_desc.rdomain.dim, coVariant, MIX_coord,
      f_desc.rdomain.cof );
    GroupLocator sc( f_desc.rdomain.dim, coVariant, STD_coord,
      f_desc.rdomain.cof );
    GroupLocator sb( f_desc.rdomain.dim, coVariant, STD_REL_BDRY_coord,
      f_desc.rdomain.cof );
    std::unique_ptr<HomMarkedAbelianGroup> sc_sb(
        homGroup( HomLocator( sc, sb ) )->torsionSubgroup() ); 
    std::unique_ptr<HomMarkedAbelianGroup> sc_mc(
        homGroup( HomLocator( sc, mc ) )->torsionSubgroup() );
    std::unique_ptr<HomMarkedAbelianGroup> dc_mc(
        homGroup( HomLocator( dc, mc ) )->torsionSubgroup() );
    std::unique_ptr<HomMarkedAbelianGroup> fl( 
        (*dc_mc->inverseHom())*(*sc_mc) ); // STD -> DUAL
      FormLocator prim(f_desc); prim.ldomain.hcs = DUAL_coord; 
    prim.rdomain.hcs = STD_REL_BDRY_coord;
    bfptr = new BilinearForm( 
        (bilinearForm(prim)->lCompose(*fl)).rCompose(*sc_sb) ); 

    std::map< FormLocator, BilinearForm* > *mbfptr = 
     const_cast< std::map< FormLocator, BilinearForm* > *> (&bilinearForms);
    mbfptr->insert( std::pair<FormLocator, BilinearForm*>(f_desc, bfptr) );
    return bfptr; 
  }

 // case 4: cup products
 //         a) std_rel_bdry x dual
 //         b) std_rel_bdry x std_rel_bdry
 //         c) std_rel_bdry x std
 //         d) std x std
 //         e) dual x dual
 if ( ( f_desc.ft == cupproductForm ) &&
      ( f_desc.ldomain.var == contraVariant ) && 
      ( f_desc.rdomain.var == contraVariant ) &&
      ( f_desc.ldomain.dim + f_desc.rdomain.dim <= aDim ) &&
      ( f_desc.ldomain.dim > 0 ) && ( f_desc.rdomain.dim > 0 )&&
      ( f_desc.ldomain.cof == f_desc.rdomain.cof ) &&
      ( f_desc.ldomain.hcs == DUAL_coord ) && 
      ( f_desc.rdomain.hcs == STD_REL_BDRY_coord) )
  {// TODO 
  }
 // dual pairing DUAL x STD_REL_BDRY
 // dual cp1     ( f_desc.ldomain.hcs == DUAL_coord ) && 
 //              (f_desc.rdomain.hcs == DUAL_coord) )
 // dual cp2    ( f_desc.ldomain.hcs == STD_coord ) && 
 //             ( f_desc.rdomain.hcs == STD_REL_BDRY_coord) )
 // PD maps (dual)H_k --> (std_rel_bdry)H^{n-k} and
 //         (dual)H^k --> (std_rel_bdry)H_{n-k}
 // oh, to get H^i M x H^j M --> H^{i+j} M we need dual_boundary and 
 // dual_rel_bdry coords, and all the relevant maps, and 
 // dual_rel_bdry --> std poincare duality map... a bunch more work. 
 return NULL;
}

const GroupPresentation* CellularData::groupPresentation( 
        const GroupPresLocator &g_desc ) const
{
 reloop_loop:

 // various bail triggers
 if ( (g_desc.sub_man==ideal_boundary) &&    
      (g_desc.component_index >= numIdealBdryComps) ) return NULL;
 if ( (g_desc.sub_man==standard_boundary) && 
      (g_desc.component_index >= numStdBdryComps) )   return NULL;

 std::map< GroupPresLocator, GroupPresentation* >::const_iterator p;
 p = groupPresentations.find(g_desc);
 if (p != groupPresentations.end()) return (p->second);
 else 
  { 
   buildFundGrpPres(); // ensure it's computed, and look it up.
   goto reloop_loop;
  }
 // return NULL if an invalid request
 return NULL;
}

const HomGroupPresentation* CellularData::homGroupPresentation( 
      const HomGroupPresLocator &h_desc ) const
{
 reloop_loop:
 std::map< HomGroupPresLocator, HomGroupPresentation* >::const_iterator p;

 // various bail triggers
 if ( (h_desc.inclusion_sub_man==ideal_boundary) &&    
      (h_desc.subman_component_index >= numIdealBdryComps) ) return NULL;
 if ( (h_desc.inclusion_sub_man==standard_boundary) && 
      (h_desc.subman_component_index >= numStdBdryComps) )   return NULL;

 p = homGroupPresentations.find(h_desc);
 if (p != homGroupPresentations.end()) return (p->second);
 else 
  { 
   buildFundGrpPres(); // ensure it's computed, and look it up.
   goto reloop_loop;
  }
 // return NULL if an invalid request
 return NULL;
}

unsigned long CellularData::components( submanifold_type ctype ) const
{
 if (ctype == whole_manifold) return 1;
 if (ctype == standard_boundary) return stdBdryPi1Gen.size();
 if (ctype == ideal_boundary) return idBdryPi1Gen.size();
 return 0; // it will only get here if the input request is invalid.
} 

unsigned long CellularData::cellCount( 
  const ChainComplexLocator &coord_system) const
{ // TODO all the new coordinate systems
 if ( (coord_system.dim > 4) && tri4 ) return 0; 
 if ( (coord_system.dim > 3) && tri3 ) return 0; // out of bounds check
 if (coord_system.hcs == STD_coord) return numStandardCells[coord_system.dim]; 
 if (coord_system.hcs == DUAL_coord) return numDualCells[coord_system.dim]; 
 if (coord_system.hcs == MIX_coord) return numMixCells[coord_system.dim];
 if (coord_system.hcs == MIX_REL_BDRY_coord) 
  return numMixRelCells[coord_system.dim]; 
 if (coord_system.hcs == STD_REL_BDRY_coord) 
  return numRelativeCells[coord_system.dim]; 
 if (coord_system.hcs == DUAL_REL_BDRY_coord) 
  return numDualRelCells[coord_system.dim]; 
 if ( (coord_system.dim > 3) && tri4 ) return 0;
 if ( (coord_system.dim > 2) && tri3 ) return 0;
 if (coord_system.hcs == STD_BDRY_coord) 
  return numStandardBdryCells[coord_system.dim]; 
 if (coord_system.hcs == MIX_BDRY_coord) 
  return numMixBdryCells[coord_system.dim]; 
 if (coord_system.hcs == DUAL_BDRY_coord) 
  return numDualBdryCells[coord_system.dim]; 
 return 0; // it will only get here if hcs is out of bounds. 
}

long int CellularData::eulerChar() const
{ return numDualCells[0]-numDualCells[1]+numDualCells[2]-
         numDualCells[3]+numDualCells[4]; }

long int CellularData::signature() const
{
 if (tri3) return 0; 
 if (!tri4->isOrientable()) return 0;
 const BilinearForm* b = bilinearForm( 
  FormLocator( intersectionForm, GroupLocator(2, coVariant, DUAL_coord, 0), 
                                 GroupLocator(2, coVariant, DUAL_coord, 0) ) );
 return b->zFormSignature();
}


const MatrixInt* CellularData::integerChainComplex( 
      const ChainComplexLocator &c_desc ) const
{
 std::map< ChainComplexLocator, MatrixInt* >::const_iterator p;
 ChainComplexLocator range_desc(c_desc); range_desc.dim--;
 // various bail triggers
 p = integerChainComplexes.find(c_desc);
 if (p != integerChainComplexes.end()) return (p->second);
 else 
  { 
   ccCollectionType::const_iterator q;
   q = genCC.find(c_desc); 
   if (q == genCC.end()) return NULL; // invalid request
   // q->second is our NSparseGrid< coverFacetData > ccMapType; 
   CellularData::ccMapType thisCC( *q->second ); 
   // build matrix. 
   MatrixInt* buildMat( NULL ); 
   buildMat = new MatrixInt( cellCount(range_desc), cellCount(c_desc) );
   // build entries
   std::map< NMultiIndex< unsigned long >, coverFacetData* >::const_iterator ci;
   for (ci = thisCC.getGrid().begin(); ci!=thisCC.getGrid().end(); ci++)
     buildMat->entry( ci->second->cellNo,  ci->first.entry(0) ) += 
         ci->second->sig;
   // insert
   std::map< ChainComplexLocator, MatrixInt* > *Mptr = 
   const_cast< std::map< ChainComplexLocator, MatrixInt* > *> 
     (&integerChainComplexes);
   Mptr->insert( std::pair< ChainComplexLocator, MatrixInt* > 
     ( c_desc, buildMat ) );
   return buildMat; 
  }
 // return NULL if an invalid request
 return NULL;
}

const MatrixInt* CellularData::integerChainMap( 
   const ChainMapLocator &m_desc ) const
{
 std::map< ChainMapLocator, MatrixInt* >::const_iterator p;
 p = integerChainMaps.find(m_desc);
 if (p != integerChainMaps.end()) return (p->second);
 else 
  { 
   // Look up the appropriate chain complex in genCC
   cmCollectionType::const_iterator q;
   q = genCM.find(m_desc); 
    if (q == genCM.end()) return NULL; // invalid request
   // q->second is our NSparseGrid< coverFacetData > ccMapType; 
   CellularData::ccMapType thisCM( *q->second ); 
   // build matrix. 
   MatrixInt* buildMat( NULL ); 
   buildMat = new MatrixInt( cellCount(m_desc.range), 
                              cellCount(m_desc.domain) );
   // build entries
   std::map< NMultiIndex< unsigned long >, coverFacetData* >::const_iterator ci;
   for (ci = thisCM.getGrid().begin(); ci!=thisCM.getGrid().end(); ci++)
     buildMat->entry( ci->second->cellNo,  ci->first.entry(0) ) += 
      ci->second->sig;
   // insert
   std::map< ChainMapLocator, MatrixInt* > *Mptr = 
    const_cast< std::map< ChainMapLocator, MatrixInt* > *> 
     (&integerChainMaps);
   Mptr->insert( std::pair< ChainMapLocator, MatrixInt* > 
     ( m_desc, buildMat ) );
   return buildMat; 
  }
 // return NULL if an invalid request
 return NULL;
}

unsigned long num_less_than(const std::set<unsigned long> &thelist, 
                            const unsigned long &obj); // forward declaration.

// Eventually this should return the appropriate map from the (a_desc)-stage 
//  of the chain complex for the Alexander module.  The current algorithm is 
//  a hack, but until all the appropriate `wordles' are computed in
//  ncellulardata.init.cc.cpp, this is all that's available. 
//
// At present this algorithm collapses the maximal tree in the 1-skeleton 
// of the dual CW-decomposition.  Assumes triangulation is connected. 
// So the 1->0 chain map is just a [t^a-1, ..., t^p-1] type of matrix.  
const MatrixRing< SVPolynomialRing< Integer > >* 
 CellularData::alexanderChainComplex( 
     const ChainComplexLocator &a_desc ) const
{ 
 std::map< ChainComplexLocator, MatrixRing< 
            SVPolynomialRing< Integer > >* >::const_iterator p;
 ChainComplexLocator range_desc(a_desc); range_desc.dim--;
 // reasons for giving up.
 p = alexanderChainComplexes.find(a_desc);
 if (p != alexanderChainComplexes.end()) return (p->second);
 ccCollectionType::const_iterator q; q = genCC.find(a_desc);
 if (q == genCC.end()) return NULL; // that chain complex does not exist!
 if (a_desc.hcs != DUAL_coord) return NULL; 
 if ((a_desc.dim > 2) || (a_desc.dim<1)) return NULL; 
 // get to work, no reason to give up. 
 std::set< unsigned long > maxTreedcIx; 
 // dcIx index vs. nicIx[dim-1] indices of max tree maxTreeStd
 for (std::set<unsigned long>::const_iterator i=maxTreeStd.begin(); 
                                              i!=maxTreeStd.end(); i++)
  { // this is a local def.
   if (tri3) if (!tri3->triangle( nicIx[2][*i] )->isBoundary()) 
              maxTreedcIx.insert( dcIxLookup( tri3->triangle( nicIx[2][*i] ) ) );
   if (tri4) if (!tri4->tetrahedron( nicIx[3][*i] )->isBoundary()) 
                 maxTreedcIx.insert( 
                   dcIxLookup( tri4->tetrahedron( nicIx[3][*i] ) ) );
  }
  // this is a sparse grid of coverFacetData from the chain complex.
  CellularData::ccMapType thisCC( *q->second ); 
  // fundamental group and its abelianization. 
  const GroupPresentation* pi1( groupPresentation( 
                                GroupPresLocator( whole_manifold, 0 ) ) );
  std::unique_ptr<MarkedAbelianGroup> pi1Ab( pi1->markedAbelianisation() );
  // We adapt the chain complex because of the reduced number of 0 and 1-cells, 
  // since we're using a maximal tree in dual 1-skeleton to reduce the cell 
  // structure. this is an ad-hoc solution since we don't have good 
  // algorithms implemented to compute homology of chain complexes over a 
  // single-variable Laurent polynomial ring implemented.
  MatrixRing<SVPolynomialRing< Integer > >* buildMat( NULL ); 
  unsigned long ranDim; unsigned long domDim; 
   // lower rank of domain by one for every element of the maximal tree
  if (a_desc.dim==1) { ranDim = 1;  
                       domDim = cellCount(a_desc) - maxTreedcIx.size(); }
   // lower rank of range by one for every element of the maximal tree
  if (a_desc.dim==2) { ranDim = cellCount(range_desc) - maxTreedcIx.size(); 
                       domDim = cellCount(a_desc); }
  buildMat = new MatrixRing<SVPolynomialRing< Integer > >
        ( ranDim, domDim );

  // build entries
  std::map< NMultiIndex< unsigned long >, coverFacetData* >::const_iterator ci;

  for (ci = thisCC.getGrid().begin(); ci!=thisCC.getGrid().end(); ci++)
    {
     // ci is pointing to the boundary facets of cells in the chain complex, 
     // the cells being in DUAL_coord. it's possible ci could be emanating 
     // from or pointing to something from the maximal tree.  
     //  If this is the case, we can safely ignore it. 
    if ( (a_desc.dim==1) && ( maxTreedcIx.find( ci->first.entry(0) ) != 
        maxTreedcIx.end() ) ) continue;
    if ( (a_desc.dim==2) && ( maxTreedcIx.find( ci->second->cellNo ) != 
        maxTreedcIx.end() ) ) continue; 

    // now we take the cover facet data pointed to by ci and convert it to 
    // something in the chain complex for the abelianization of pi1. 
    std::vector<Integer> ccI( pi1->countGenerators() );  
    // to put into pi1Ab
    for (unsigned long i=0; i<ci->second->trans.countTerms(); i++)
      ccI[ci->second->trans.term(i).generator] += 
        ci->second->trans.term(i).exponent;
    signed long levelOfCell ( 
        pi1Ab->snfRep(ccI)[pi1Ab->countInvariantFactors()].longValue() );
    // level of cell is the index i, for the covering space trans t^i
    unsigned long cR, cC;  
    // so we need to chuck the 1-cell if it is not in the dual 1-skeleton, 
    //  and if it *is* in maxTreeStd

    if (a_desc.dim==1) { cR=0; cC=ci->first.entry(0) - 
        num_less_than(maxTreedcIx, ci->first.entry(0)); }
    if (a_desc.dim==2) { cR=ci->second->cellNo - 
        num_less_than(maxTreedcIx, ci->second->cellNo);
                         cC = ci->first.entry(0); }
    buildMat->entry( cR,  cC ) += SVPolynomialRing< Integer >( 
        Integer(ci->second->sig), levelOfCell ); 
    }
   // insert
  std::map< ChainComplexLocator, 
            MatrixRing<SVPolynomialRing< Integer > >* > *Mptr = 
  const_cast< std::map< ChainComplexLocator, 
              MatrixRing<SVPolynomialRing< Integer > >* > *>
    (&alexanderChainComplexes);
  Mptr->insert( std::pair< ChainComplexLocator, 
              MatrixRing<SVPolynomialRing< Integer > >* > 
    ( a_desc, buildMat ) );
  return buildMat; 
}

namespace {
/*
 *  Given integers n != 0 and m, this routine computes d and r so that
 *   m = dn + r with 0 <= r < |n|
 */
void signedLongDivAlg( signed long n, signed long m, 
                       signed long &d, signed long &r )
{ d = m/n; r = m-d*n; if (r<0) { r += abs(n); d += ( (n>0) ? -1 : 1 ); } }

} // end anonymous namespace

std::unique_ptr< MatrixRing< SVPolynomialRing< Integer > > > 
  CellularData::alexanderPresentationMatrix() const
{
 const MatrixRing<SVPolynomialRing< Integer > >* 
  M( alexanderChainComplex( ChainComplexLocator(1, CellularData::DUAL_coord)));
 const MatrixRing<SVPolynomialRing< Integer > >* 
  N( alexanderChainComplex( ChainComplexLocator(2, CellularData::DUAL_coord)));
 MatrixRing<SVPolynomialRing< Integer > > workM(*M);  
 MatrixRing<SVPolynomialRing< Integer > > 
    rowOpMat(M->columns(), M->columns());
 MatrixRing<SVPolynomialRing< Integer > > workN(*N);  
 MatrixRing<SVPolynomialRing< Integer > > 
    rowOpInvMat(M->columns(), M->columns());

 rowOpMat.makeIdentity(); rowOpInvMat.makeIdentity();
 // the single row of M consists of elements of the form t^n-1 for n an 
 // integer.  In particular entries can be zero although there must be 
 // one non-zero entry.  Column reducing this matrix amounts to the GCD 
 // algorithm on the exponents.
 unsigned long pivotCol;
 signed long smallestNZdeg;
 find_small_degree:  // look for smallest non-zero degree elt, 
 // set index of column to pivotCol
 pivotCol=0; smallestNZdeg=0;

 for (unsigned long i=0; i<workM.columns();i++)
  { if ( (workM.entry(0,i).degree() != 0) && 
       ( ( abs(workM.entry(0,i).degree()) < abs(smallestNZdeg)) || 
         (smallestNZdeg==0) ) )
    {     pivotCol = i;  smallestNZdeg = workM.entry(0,i).degree(); } 
  }

 bool nonZeroFlag(false);
 for (unsigned long i=0; i<M->columns(); i++)
  if ( (workM.entry(0,i).degree() != 0) && (i!=pivotCol) ) 
  // use pivotCol to reduce i
  {
   // so we compute the division alg on the two degrees, 
   // and use that to subtract a multiple of one from the other
   signed long d, r; 
   signedLongDivAlg( workM.entry(0,pivotCol).degree(), 
                     workM.entry(0,i).degree(), d, r);
   // t^m-1 = SVPolynomialRing< Integer >(n,m,d)*(t^n-1) + t^r-1  
   SVPolynomialRing< Integer > Fac( SVPolynomialRing< Integer >
         ( workM.entry(0,pivotCol).degree(), workM.entry(0,i).degree(), d) );
   workM.entry(0,i) = SVPolynomialRing< Integer >( 
         Integer::one, r ) - SVPolynomialRing< Integer >::one;
   // now do corresponding row op on workN, ie subtract NSVP(n,m,d) of 
   // the pivot row from the ith row
   workN.addRow( i, pivotCol, Fac );
   // if entry (0,i) nonzero after reduction, set nonZeroFlag
   if (!workM.entry(0,i).isZero()) nonZeroFlag = true;
  }
 if (nonZeroFlag) goto find_small_degree;
 // okay, all entries except pivotCol are killed, so pivotCol in workM 
 // must be t^{\pm}-1. 
 std::unique_ptr< MatrixRing< SVPolynomialRing< Integer > > > retval( 
   new MatrixRing< SVPolynomialRing< Integer > >
    (N->rows()-1,N->columns()) );
 for (unsigned long i=0; i<retval->rows(); i++) 
  for (unsigned long j=0; j<retval->columns(); j++)
   retval->entry( i, j ) = workN.entry( (i<pivotCol) ? i : i+1, j ); 

 return retval;
}

// compute the Alexander ideal of the Alexander module. 
std::unique_ptr< std::list< SVPolynomialRing< Integer > > > 
    CellularData::alexanderIdeal() const
{
 std::unique_ptr< MatrixRing< SVPolynomialRing< Integer > > > 
    aPM(alexanderPresentationMatrix());
 std::list< SVPolynomialRing< Integer > > alexIdeal;
 // degenerate matrices first.
 if (aPM->rows()==0) 
    alexIdeal.push_back( SVPolynomialRing< Integer >::one );
 else if (aPM->columns()==0) 
    alexIdeal.push_back( SVPolynomialRing< Integer >::zero );
 else
  {
  // in general aPM might be wider than it is tall
  unsigned long colToErase = aPM->columns() - aPM->rows();
  // choose numbers 0 <= a1 < ... < ak <=aPM.columns() to erase, 
  //  then take the determinant of that submatrix. 
  // let's use NPartition to iterate through all such. 
  NPartition skipCols( aPM->columns(), colToErase );
  while ( !skipCols.atEnd() )
   {
    MatrixRing< SVPolynomialRing< Integer > > 
        sqSubMat( aPM->rows(), aPM->rows() );
    unsigned long delta=0;
    for (unsigned long j=0; j<sqSubMat.columns(); j++)
     {
      while (skipCols.partition().get(j+delta)) delta++;     
      for (unsigned long i=0; i<sqSubMat.rows(); i++) 
        sqSubMat.entry(i,j) = aPM->entry( i, j+delta );
     }

    SVPolynomialRing< Integer > TEMP(sqSubMat.det());
    if (!TEMP.isZero()) alexIdeal.push_back( TEMP );
    ++skipCols; // next!
   }
  }
 // consider reducing the ideal before returning it.
 reduceIdeal(alexIdeal);

 // clean up the polynomials a little, if possible. 
 std::list< SVPolynomialRing< Integer > >::iterator it;
 for (it = alexIdeal.begin(); it!=alexIdeal.end(); it++)
   { prettifyPolynomial(*it); }

 return std::unique_ptr< std::list< SVPolynomialRing< Integer > > >
        (new std::list< SVPolynomialRing< Integer > >(alexIdeal));
}

namespace {

std::string embeddabilityString(const Triangulation<3>* tri, 
    const CellularData* cdat, const BilinearForm* tlf); // forward ref

}

std::string CellularData::stringInfo( const StringRequest &s_desc ) const
{ //TODO - this routine isn't complete yet. 
 std::string retval("Invalid request");
 const BilinearForm* torForm(NULL);
 if ( ( (s_desc == TORFORM_powerdecomp) || 
        (s_desc == TORFORM_sigmastring) || 
        (s_desc == TORFORM_legendresymbol) || 
        (s_desc == TORFORM_tests) || 
        (s_desc == TORFORM_embinfo) ) &&
        (tri3 != NULL) &&
        (tri3->isOrientable() ) &&
        (tri3->isConnected() ) )
  {
   torForm = bilinearForm( FormLocator( torsionlinkingForm, 
              GroupLocator( 1, coVariant, DUAL_coord, 0 ), 
              GroupLocator( 1, coVariant, DUAL_coord, 0 ) ) );
  }
 if (torForm != NULL) 
 {
 if (s_desc == TORFORM_powerdecomp)
  { retval = torForm->kkTorRank(); }
 else if (s_desc == TORFORM_sigmastring)
  { retval = torForm->kkTorSigma(); }
 else if (s_desc == TORFORM_legendresymbol)
  { retval = torForm->kkTorLegendre(); }
 else if (s_desc == TORFORM_tests)
  { } // TODO ?? what is this again ??
 else if (s_desc == TORFORM_embinfo)
  { retval = embeddabilityString( tri3, this, torForm ); } }

 return retval;
}

namespace {
// this routine computes a string describing embeddability of the manifold 
// into S^4, it assumes cdat is derived from tri, and tlf is the torsion 
// linking form for cdat.
std::string embeddabilityString(const Triangulation<3>* tri, 
    const CellularData* cdat,  const BilinearForm* tlf) {
    // Only do this if we haven't done it already.
    std::string retval;
    unsigned long totbcomp( cdat->components(CellularData::standard_boundary) + 
                            cdat->components(CellularData::ideal_boundary) );

    if (tri->countTetrahedra() == 0)
      return std::string("Manifold is empty.");
    
   const MarkedAbelianGroup* homol( cdat->markedGroup( 
        CellularData::GroupLocator(1, CellularData::coVariant, 
                                       CellularData::STD_coord, 0 )
        ) );


    if (tri->isOrientable())
      { // orientable -- we need the torsion linking form

        if (totbcomp==0) 
        { // no boundary : orientable
            if (homol->countInvariantFactors()==0) 
            { // no torsion : no boundary, orientable
                if (tri->knowsThreeSphere() && tri->isThreeSphere())
                    retval = "This manifold is S^3.";
                else if (homol->isTrivial())
                    retval = "Manifold is a homology 3-sphere.";
                else
                    retval = "No information.";
            } // no torsion : no boundary, orientable 
            else 
            {// torsion : no boundary, orientable
                if (!tlf->kkTwoTor())
                    retval =
                        "This manifold, once-punctured, "
                        "does not embed in a homology 4-sphere.";
                else if (!tlf->kkIsHyperbolic())
                    retval =
                        "Does not embed in homology 4-sphere.";
                else
                    retval = "The torsion linking form is "
                        "of hyperbolic type.";
                if (homol->rank()==0)
                    retval += "  Manifold is a rational "
                        "homology sphere.";
            } // torsion : no boundary, orientable
        } // no boundary : orientable
        else 
        { // boundary : orientable
         const HomMarkedAbelianGroup bhomolMap( *cdat->homGroup( 
          CellularData::HomLocator( CellularData::GroupLocator( 1,
           CellularData::coVariant, CellularData::STD_BDRY_coord, 0),
           CellularData::GroupLocator( 1, CellularData::coVariant,
           CellularData::STD_coord, 0) ) ) );

         const MarkedAbelianGroup bhomol( *cdat->markedGroup( 
          CellularData::GroupLocator( 1, CellularData::coVariant,
           CellularData::STD_BDRY_coord, 0) ) );

         if (homol->countInvariantFactors()==0)   
          {
           // orientable with boundary, no torsion. We have no tests
           // so far for checking if it embeds in a homology 4-sphere
           // unless we implement the Kojima alexander polynomials.
           // H1 map check... boundary map has full rank iff embeds in
           // rational homology 3-sph
           // boundary map epic iff embeds in homology 3-sphere
           if (bhomolMap.isEpic())
            {
             retval = "Embeds in a homology 3-sphere as a ";
             if (bhomol.rank() == 2*totbcomp)
              {
               if (totbcomp==1) retval += "knot complement.";
               else retval += "link complement.";
              }
             else
              {
               if (bhomol.rank() == 0) retval += "ball complement.";
               else retval += "graph complement.";
              }
            }
           else if (bhomolMap.cokernel().rank()==0)
            {
             retval = "Embeds in a rational homology 3-sphere as a ";
             if (bhomol.rank() == 2*totbcomp )
              {
               if (totbcomp==1) retval += "knot complement.";
               else retval += "link complement.";
              }
             else
              {
               if (bhomol.rank() == 0) retval += "ball complement.";
               else retval += "graph complement.";
              }
            } 
           else
             retval = "Does not embed in a rational homology 3-sphere.";
          } // no torsion : boundary, orientable
         else
          { // torsion : boundary, orientable
           if (!tlf->kkTwoTor())
            { // two tor condition not satisfied
             if (bhomolMap.isEpic())
              retval = "Embeds in homology 3-sphere "
                       "but not homology 4-sphere.";
             else if (bhomolMap.cokernel().rank()==0)
               retval = "Embeds in rational homology 3-sphere but not "
                        "homology 4-sphere.";
             else retval = "Does not embed in homology 3-sphere, "
                           "nor homology 4-sphere.";
            }
           else
            { // KK twotor condition satisfied...
             if (bhomolMap.isEpic())
              retval = "Embeds in homology 3-sphere.  "
                       "KK 2-tor condition satisfied.";
             else if (bhomolMap.cokernel().rank()==0)
              retval = "Embeds in rational homology 3-sphere.  "
                       "KK 2-tor condition satisfied.";
             else 
              retval = "Does not embed in homology 3-sphere.  "
                       "KK 2-tor condition satisfied.";
            }
          } // torsion : boundary, orientable
        } // boundary : orientable 
     } // end orientable 
     else 
     { // triangulation is NOT orientable, therefore can not embed
       // in any rational homology 3-sphere.  So we look at the
       // orientation cover...
       Triangulation<3> orTri(*tri);
       orTri.makeDoubleCover();
       CellularData covHomol( orTri );
       const BilinearForm* covForm( covHomol.bilinearForm( 
         CellularData::FormLocator(
         CellularData::torsionlinkingForm, 
         CellularData::GroupLocator( 1, CellularData::coVariant, 
            CellularData::STD_coord, 0),
         CellularData::GroupLocator( 1, CellularData::coVariant, 
            CellularData::STD_coord, 0) ) ) );

       // break up into two cases, boundary and no boundary...
       if (covHomol.components(CellularData::standard_boundary)+
           covHomol.components(CellularData::ideal_boundary)==0)
         { // no boundary
          if (covForm->kkIsHyperbolic())
          retval = "Orientation cover has hyperbolic"
                   " torsion linking form.";
          else
            retval = "Does not embed in homology 4-sphere.";
         }
        else
         {// boundary
          if (covForm->kkTwoTor())
            retval = "Orientation cover satisfies"
                     " KK 2-torsion condition.";
          else
            retval = "Does not embed in homology 4-sphere.";
         }
     }
  return retval;
} // end embeddabilityString()

} // end unnamed namespace

bool CellularData::boolInfo( const BoolRequest &b_desc) const
{ // TODO something!
  return true;
}
// eventually we can have a tribool routine as well...

std::vector< bool > CellularData::stiefel_whitney( unsigned long i ) const
{
 if (i!=1) return std::vector<bool>();
 std::vector<bool> retval; 
 retval.resize( cellCount( ChainComplexLocator( 1, DUAL_coord ) ) );
 // numDualCells[1] or cellCount( ChainComplexLocator( 1, DUAL_coord ) )
 const Simplex<3>* tet; const Simplex<4>* pen;
 unsigned long idx;
 for (unsigned long i=0; i<retval.size(); i++)
  {
   if (tri3 != NULL) 
    { tet = tri3->triangle(dcIx[1][i])->embedding(0).tetrahedron();
      idx = tri3->triangle(dcIx[1][i])->embedding(0).face();
      retval[i] = (tet->adjacentGluing(idx).sign() > 0) ? true : false; }
   else 
    { pen = tri4->tetrahedron(dcIx[1][i])->embedding(0).pentachoron();
      idx = tri4->tetrahedron(dcIx[1][i])->embedding(0).tetrahedron();
      retval[i] = (pen->adjacentGluing(idx).sign() > 0) ? true : false; }
  } // true indicates preserving the orientations given by characteristic maps.
 return retval;
}

// The Hurewicz map where the i-th column is the DUAL_coord CC listing
// of the i-th generator of the fundamental group, computed in DUAL_coord. 

MatrixInt CellularData::hurewicz_map_H1() const
{
 // The idea to compute this is to take the endpoints of the i-th generator.
 // In the 4-dimensional case they're in either the dual 1-skeleton or the
 // refined version where they can be in the dual 1-skeleton of the boundary. 
 // We have to close this to a loop with the unique arc in the maximal forest,
 // and then push that into the chain complex for DUAL_coord (which lacks the
 // edges in the boundary, etc... ). 
 // TODO
 return MatrixInt(0,0);
}

std::vector< bool > CellularData::PI1_stiefel_whitney1() const
{ 
 std::vector<bool> SW_H1( stiefel_whitney( 1 ) );
 // given generator gi of the fundamental group, we need to compute its
 // H1 representative. 

 // TODO: finish! For this we'll need to know the minimal arcs in the
 //  maximal forest.  This exists somewhere in the code, I think... just
 //  need to figure out where. 
 /*  maxTreeStd    stores the edges dual to co-dimension 1 simplices, 
  *                indexed by nicIx[n-1]
  *  maxTreeStB    boundary edges dual to co-dimension 2 boundary faces, 
  *                indexed by bcIx[n-2]
  *  maxTreeIdB    boundary edges dual to co-dimension 2 ideal boundary faces,
  *                indexed by icIx[n-2]
  *  maxTreeSttIdB edges connecting top-dimensional simplices barycentres 
  *                to boundary cd1 barycentres, indexed by icIx[n-1]
  */
 return std::vector<bool>();
}



} // namespace regina


