
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

#include "algebra/nfibring.h"
#include "algebra/nmarkedabeliangroup.h"
#include "algebra/ncellulardata.h"

#include "surfaces/nnormalsurface.h"
#include "surfaces/nsstandard.h"
#include "hypersurface/nnormalhypersurface.h"
#include "hypersurface/nhsstandard.h"

namespace regina {

NMapToS1::NMapToS1( const Dim2Triangulation* tri )
{ 
 tri2=new Dim2Triangulation(*tri); tri3=NULL; tri4=NULL; 
 builddimNinc();
}

NMapToS1::NMapToS1( const NTriangulation* tri )
{ 
 tri2=NULL; tri3=new NTriangulation(*tri); tri4=NULL; 
 if (tri3->isIdeal()) { tri3->idealToFinite(); tri3->intelligentSimplify(); }
 builddimNinc();
}

NMapToS1::NMapToS1( const Dim4Triangulation* tri )
{ 
 tri2=NULL; tri3=NULL; tri4=new Dim4Triangulation(*tri); 
 if (tri4->isIdeal()) { tri4->idealToFinite(); tri4->intelligentSimplify(); }
 builddimNinc();
}

void NMapToS1::builddimNinc()
{ // this routine is used to build and rebuild the dim?inc object, so 
  // we need to erase the previous data if it exists.
 if ( tri2 && !dim2inc.empty() ) dim2inc.clear();
 if ( tri3 && !dim3inc.empty() ) dim3inc.clear();
 if ( tri4 && !dim4inc.empty() ) dim4inc.clear();

 if ( (tri2) ) 
  for (unsigned long i=0; i<tri2->getNumberOfEdges(); i++)
  for (unsigned long j=0; j<2; j++)
   dim2inc[ tri2->getEdge(i)->getVertex(j) ].push_back( 
    std::pair< Dim2Edge*, unsigned long >(tri2->getEdge(i),j) );

 if ( (tri3) ) 
  for (unsigned long i=0; i<tri3->getNumberOfEdges(); i++)
  for (unsigned long j=0; j<2; j++)
   dim3inc[ tri3->getEdge(i)->getVertex(j) ].push_back( 
    std::pair< NEdge*, unsigned long >(tri3->getEdge(i), j) );

 if ( (tri4) ) 
  for (unsigned long i=0; i<tri4->getNumberOfEdges(); i++)
  for (unsigned long j=0; j<2; j++)
   dim4inc[ tri4->getEdge(i)->getVertex(j) ].push_back( 
    std::pair<Dim4Edge*, unsigned long >(tri4->getEdge(i),j) );
}


NMapToS1::~NMapToS1()
{
 if (tri2) delete tri2;
 if (tri3) delete tri3;
 if (tri4) delete tri4;
}

/**
 * This routine verifies whether or not a 1-dimensional cochain with 
 * rational coefficients is a cohomologous to a primitive cochain with
 * integer coefficients, i.e. a class suitable for the fiberability
 * test. It assumes you are using STD_coord from NCellularData, i.e. 
 * a vector whose entries correspond to the indexing of the edges of
 * the underlying triangulation.
 */
bool NMapToS1::verifyPrimitiveH1( const std::vector<NRational> &cocy ) const
{
 unsigned long numVrt( tri2 ? tri2->getNumberOfVertices() :
                       tri3 ? tri3->getNumberOfVertices() :
                       tri4->getNumberOfVertices() );
 unsigned long numEdg( tri2 ? tri2->getNumberOfEdges() : 
                       tri3 ? tri3->getNumberOfEdges() :
                       tri4->getNumberOfEdges() );
 unsigned long numTri( tri2 ? tri2->getNumberOfTriangles() :
                       tri3 ? tri3->getNumberOfTriangles() :
                       tri4->getNumberOfTriangles() );

 // step 1: check it correct length and non-zero on all edges
 if ( cocy.size() != numEdg ) return false; 

 for (unsigned long i=0; i<numEdg; i++)
  if ( cocy[i] == NRational::zero ) return false;

 // step 2: check is cocycle
 for (unsigned long i=0; i<numTri; i++)
  {
   NRational SUM(0);
   for (unsigned long j=0; j<3; j++)
    {
      if (tri2) 
       SUM += cocy[tri2->edgeIndex( tri2->getTriangle(i)->getEdge(j) )]*
              tri2->getTriangle(i)->getEdgeMapping(j).sign();
      else if (tri3) 
       SUM += cocy[tri3->edgeIndex( tri3->getTriangle(i)->getEdge(j) )]*
              tri3->getTriangle(i)->getEdgeMapping(j).sign();
      else
       SUM += cocy[tri4->edgeIndex( tri4->getTriangle(i)->getEdge(j) )]*
              tri4->getTriangle(i)->getEdgeMapping(j).sign();
    }
   if (SUM!=NRational::zero) return false;
  }

 // step 3: check primitive
 NMatrixInt C1tC0( numVrt, numEdg ); // homology chain complex
 NMatrixInt C2tC1( numEdg, numTri ); // C2 --> C1
 for (unsigned long i=0; i<numTri; i++)
  for (unsigned long j=0; j<3; j++) // fill C2tC1
   {
    C2tC1.entry( tri2 ? tri2->edgeIndex( tri2->getTriangle(i)->getEdge(j) ) :
                 tri3 ? tri3->edgeIndex( tri3->getTriangle(i)->getEdge(j) ) :
                        tri4->edgeIndex( tri4->getTriangle(i)->getEdge(j) ), i ) 
    += (tri2 ? tri2->getTriangle(i)->getEdgeMapping(j).sign() :
        tri3 ? tri3->getTriangle(i)->getEdgeMapping(j).sign() :
               tri4->getTriangle(i)->getEdgeMapping(j).sign() ); 
   } 

 for (unsigned long i=0; i<numEdg; i++)
  for (unsigned long j=0; j<2; j++) // fill C1tC0
   {
    C1tC0.entry( tri2 ? tri2->vertexIndex( tri2->getEdge(i)->getVertex(j) ) :
                 tri3 ? tri3->vertexIndex( tri3->getEdge(i)->getVertex(j) ) :
                        tri4->vertexIndex( tri4->getEdge(i)->getVertex(j) ), i )
    += ( (j==1) ? 1 : -1 );
   }
 NMarkedAbelianGroup H1( C1tC0, C2tC1 );
 #ifdef DEBUG
 if (!H1.isChainComplex()) { std::cout<<"CC Err!"<<std::endl; exit(1); }
 if (H1.getRank()==0) { std::cout<<"H1 has no rank!"<<std::endl; exit(1); }
 #endif

 std::vector< NLargeInteger > evalCC( H1.getRank() );
 for (unsigned long i=0; i<evalCC.size(); i++)
  {
   NRational evalTemp(0);
   std::vector<NLargeInteger> ccrep( H1.getFreeRep(i) );
   for (unsigned long j=0; j<ccrep.size(); j++)
    evalTemp += NRational(ccrep[j])*cocy[j];
   if (evalTemp.getDenominator()!=NLargeInteger::one) return false;
   evalCC[i] = evalTemp.getNumerator();
  } // entry i is i-th free rep dot with cocy
 //  Check the GCDs of these evaluations are 1 or -1. 
 for (unsigned long i=1; i<evalCC.size(); i++)
    evalCC[0] = evalCC[0].gcd(evalCC[i]);
 if ( (evalCC[0]==NLargeInteger::one) || (evalCC[0]== -NLargeInteger::one) )
  return true;

 return false;
}



/**
 * This routine verifies whether or not a primitive an integral-primitive
 * H1 cochain (with rational coefficients) is induced from a locally-trivial
 * fibre bundle from the manifold to the circle.  The value of the cochain
 * on a 1-cell is seen as the `winding number' around the circle. This amounts
 * to triangulating the level-sets and checking they are manifolds. 
 *
 * Assumes the input cocycle is primitive, i.e. passes the verifyPrimitiveH1
 * test. This condition forces the map to be a submersion provided level
 * sets are PL-submanifolds. 
 */
bool NMapToS1::verifySimpleS1Bundle( const std::vector<NRational> &cocy,
                                 std::vector< unsigned long > **diag_vec) const
{
 bool retval(true);

 if (diag_vec!=NULL) (*diag_vec) = new std::vector< unsigned long >(0);
 if (tri2)
  {
   if (diag_vec!=NULL) (*diag_vec)->reserve( tri2->getNumberOfVertices() );
   for (unsigned long i=0; i<tri2->getNumberOfVertices(); i++)
    {
     const Dim2Vertex* vtx( tri2->getVertex(i) );
     const std::deque<Dim2VertexEmbedding>& vEmb( vtx->getEmbeddings() );
     unsigned long zCount(0); // number of points in link level set. 
     for (std::deque<Dim2VertexEmbedding>::const_iterator ve=vEmb.begin();
          ve!=vEmb.end(); ve++)
     {
       const Dim2Triangle* tri( ve->getTriangle() );
       NPerm3 vInc( ve->getVertices() );
       const Dim2Edge* edg0( tri->getEdge( vInc[1] ) );
       const Dim2Edge* edg1( tri->getEdge( vInc[2] ) );
       NPerm3 e0Inc( tri->getEdgeMapping( vInc[1] ) );
       NPerm3 e1Inc( tri->getEdgeMapping( vInc[2] ) );
       if ( NRational( (e0Inc[0]==vInc[0]) ? 1 : -1 )*
            cocy[tri2->edgeIndex(edg0)]*
            NRational( (e1Inc[0]==vInc[0]) ? 1 : -1 )*
            cocy[tri2->edgeIndex(edg1)] < 0 )
         zCount++;
     }
     // the diagnostic vector
     if (diag_vec!=NULL) (*diag_vec)->push_back( zCount );
      
     if ( ( (zCount != 2) && (!vtx->isBoundary()) ) ||
          ( (zCount != 1) && (vtx->isBoundary()) ) ) 
      { retval = false; if (diag_vec==NULL) return false; }
    }
  }
 else if (tri3)
  {
   if (diag_vec!=NULL) (*diag_vec)->reserve( 2*tri3->getNumberOfVertices() );

   for (unsigned long i=0; i<tri3->getNumberOfVertices(); i++)
    { // triangulate the level-set in the link of vertex i
     const NVertex* vtx( tri3->getVertex(i) );
     const std::vector<NVertexEmbedding>& vEmb( vtx->getEmbeddings() );
     // edgeMap[ed] == i means this edge of the level-set is in vEmb[i]. 
     // the level-set separates one vertex from two in the triangle opposite
     // vtx. ovIdx[ed] == j means vEmb[i][j] is that single vertex.  
     std::map< Dim1Edge*, unsigned long > edgeMap; 
     std::map< Dim1Edge*, unsigned long > ovIdx; 

     // will be the level-set in the of vtx in its vertex link.
     Dim1Triangulation levelSet; 
     for (std::vector<NVertexEmbedding>::const_iterator j=vEmb.begin(); 
          j!=vEmb.end(); j++)
      { // create the edges of levelSet
       const NTetrahedron* tet( j->getTetrahedron() );       
       NPerm4 vInc( j->getVertices() );
       std::set<unsigned long> pvtx,nvtx; // indices of rel pos/neg vertices
       // compare cocy on  vInc[0] vs. vInc[k] k=1,2,3
       for (unsigned long k=1; k<4; k++)
        { // edge vInc[0] to vInc[1], vInc[2,3] are complementary
         unsigned long eNum( NEdge::edgeNumber[vInc[0]][vInc[k]] );
         NPerm4 eInc( tet->getEdgeMapping(eNum) );
         const NEdge* edg( tet->getEdge(eNum) ); 
         // so we need to know the edge direction to properly compute the 
         // relative value, so compute the
         if ( (eInc[0]==vInc[0]) ? (cocy[tri3->edgeIndex( edg )] > 0) :
                                   (cocy[tri3->edgeIndex( edg )] < 0) )
          pvtx.insert(k); else nvtx.insert(k);
        }
       if ( (pvtx.size()==1) || (nvtx.size()==1) )
        { // we have an edge!
          Dim1Edge* newEdg( levelSet.newEdge() );
          edgeMap.insert( std::pair< Dim1Edge*, unsigned long >
                  (newEdg, j-vEmb.begin() ) );
          ovIdx.insert( std::pair< Dim1Edge*, unsigned long >
                  (newEdg, (pvtx.size()==1) ? *pvtx.begin() : *nvtx.begin() ) );
        }
      } // end vertex link loop, levelSet edges exist now.

     // *** Gluings ***
     for (std::set< Dim1Edge* >::iterator j=levelSet.allEdges().begin(); 
          j!=levelSet.allEdges().end(); j++) // levelSet gluings.
      {
       NVertexEmbedding vEmb( vtx->getEmbedding( edgeMap[*j] ) );
       const NTetrahedron* aTet( vEmb.getTetrahedron() );
       NPerm4 vInc( vEmb.getVertices() );
       std::set< unsigned long > aSet; aSet.insert(1); aSet.insert(2); 
       aSet.insert(3); aSet.erase( ovIdx[*j] );
       for (unsigned long k=0; k<2; k++)
        {// we order the edge by how it corresponds to the tetrahedron facets,

         // vtx 0 corresponds to aSet.begin(), vtx 1 is aSet.begin()++. 
         unsigned long facet( *aSet.begin() ); aSet.erase( facet );
         if ((*j)->vtx[k] != NULL) continue; // previously glued, so skip.
         // we should also skip if we are crossing the boundary!
         if (aTet->getTriangle(vInc[facet])->isBoundary()) continue;
         // build adjacent embedding and edge.
         const NTetrahedron* oTet( aTet->adjacentTetrahedron( vInc[facet] ) );
         unsigned long oVNum( aTet->adjacentGluing( vInc[facet] )[vInc[0]] );
         // the pair (oTet, ovNum) is an NVertexEmbedding, so we look up
         //  its index.
         unsigned long tEdg; // index of adjacent embedding
         NVertexEmbedding adjvEmb; Dim1Edge* adjEdg(NULL); unsigned long adjIDX;
         for (std::map< Dim1Edge*, unsigned long >::iterator EI=edgeMap.begin(); 
                EI!=edgeMap.end(); EI++)
          { // NVertex* vtx
           if ((vtx->getEmbedding(EI->second).getTetrahedron()==oTet) &&
               (vtx->getEmbedding(EI->second).getVertex()==oVNum))
            { 
             // edgeMap[ed] == i means this edge of the level-set is in vEmb[i]. 
             // the level-set separates one vertex from two in the triangle
             // opposite vtx. ovIdx[ed] == j means vEmb[i][j] is that 
             // single vertex.  
             adjvEmb = vtx->getEmbedding(EI->second); // embedding
             tEdg = EI->second; // vertex embedding index.
             adjEdg = EI->first; // edge pointer
             adjIDX = ovIdx[adjEdg]; // index in {1,2,3} (before vtx embedding)
             break;                  // of the separated edge in the linking tri
            }
          }
         #ifdef DEBUG
         if (adjEdg==NULL) { 
          std::cout<<"verifySimpleS1Bundle: Reverse-lookup of edge failed."
                   <<std::endl; exit(1); }
         #endif
         NPerm4 oInc( adjvEmb.getVertices() );
         std::set< unsigned long > cSet; cSet.insert(1); cSet.insert(2); 
         cSet.insert(3); cSet.erase( adjIDX ); // cSet is for edge orientation 

         #ifdef DEBUG
         if (cSet.size() != 2) std::cout<<"verifySimpleS1Bundle: Set error!\n"; 
         if (adjEdg->vtx[(aTet->adjacentGluing( vInc[facet] )[vInc[facet]]==
             oInc[*cSet.begin()]) ? 0 : 1]!=NULL)
          { std::cout<<"verifySimpleS1Bundle: double gluing of adjEdg"<<
            " requested."<<std::endl; exit(1); }
         #endif

         levelSet.joinEdges( *j, k, adjEdg, 
          (aTet->adjacentGluing( vInc[facet] )[vInc[facet]]
           == oInc[*cSet.begin()]) ? 0 : 1); 

        } // end k loop
      } // end the build gluings loop / end levelSet gluings

     std::pair< unsigned long, unsigned long > 
               comps( levelSet.componentTypes() );

     if (diag_vec != NULL)
      {
       (*diag_vec)->push_back( comps.first );
       (*diag_vec)->push_back( comps.second );
      }

     if ( (vtx->isBoundary() && ( (comps.first!=0) || (comps.second!=1) ) ) ||
          (!vtx->isBoundary() && ( (comps.first!=1) || (comps.second!=0) ) ) )
      { retval = false; if (diag_vec==NULL) return false; }
    } // end loop i for vertices.
  }
 else // tri4
  {
  /* For 4-manifolds one needs to parse the vector as a lexicographical 
   *  ordering of a vector. 
   *
   * The i-th vector will describe the level-set link of the i-th vertex.  
   *
   *  The 0th entry of the i-th vector will be the number of components.  
   *
   *  The 2j+1-th entry will be
   *  the genus of that component, and the 2j+2-th entry will be the number of
   *  boundary circles in that component.   */
   if (diag_vec!=NULL) (*diag_vec)->reserve( tri4->getNumberOfVertices() );

   for (unsigned long i=0; i<tri4->getNumberOfVertices(); i++)
    {
     const Dim4Vertex* vtx( tri4->getVertex(i) );
     //   NTriangulation* buildLinkDetail(bool labels = true,
     //       Dim4Isomorphism** inclusion = 0) const;
     Dim4Isomorphism** vInc( new Dim4Isomorphism* );
     NTriangulation* vLink( vtx->buildLinkDetail(true, vInc) );
     // step 1: declare the NNormalSurfaceVector and fill it
     NNormalSurfaceVector* 
       NSV( NNormalSurfaceVectorStandard::makeZeroVector( vLink ) );
     for (unsigned long j=0; j<vLink->getNumberOfTetrahedra(); j++)
      {
       const NTetrahedron* aTet( vLink->getTetrahedron(j) );
       const Dim4Pentachoron* aPen( tri4->getPentachoron( (*vInc)->pentImage(j) ) );
       NPerm5 tetInc( (*vInc)->facetPerm(j) );
       // now we need to look at all the edges of aPen between vertices
       // tetInc[4] and tetInc[k] k=0,1,2,3
       std::set< unsigned long > pVtx, mVtx;
       for (unsigned long k=0; k<4; k++)
        { // get the edge, and determine its orientation.  
          unsigned long eNum( Dim4Edge::edgeNumber[tetInc[k]][tetInc[4]] );
          Dim4Edge* edg( aPen->getEdge(eNum) );
          NPerm5 eInc( aPen->getEdgeMapping( eNum ) );
          if ( (eInc[0]==tetInc[4]) ? (cocy[tri4->edgeIndex( edg )] > 0) :
                                      (cocy[tri4->edgeIndex( edg )] < 0) )
           pVtx.insert(k); else mVtx.insert(k);
        } // end k loop
       if ( (pVtx.size()==1) || (mVtx.size()==1) ) // tri facet
        { 
         NSV->setElement(
           7*j+( (pVtx.size()==1) ? *pVtx.begin() : *mVtx.begin() ), 1 ); 
        }
       else if ( (pVtx.size()==2) ) // quad facet
        { // quad type?
         std::vector<unsigned long> vrts;
         for (std::set<unsigned long>::iterator X=pVtx.begin(); X!=pVtx.end(); X++)
          vrts.push_back(*X);
         NSV->setElement(
           7*j+4+vertexSplit[vrts[0]][vrts[1]], 1 );
        }
      } // end tet loop j

     // step 2: declare the NNormalSurface using (1).
     NNormalSurface NSurf( vLink, NSV );
     Dim2Triangulation* levelSet( NSurf.triangulate() );

     if (diag_vec!=NULL) { 
        (*diag_vec)->reserve( (*diag_vec)->size() + 
                                       levelSet->getNumberOfComponents() );
        (*(*diag_vec))[i] = levelSet->getNumberOfComponents();

        // run through components, and push_back genus, bdry comps...
        for (unsigned long j=0; j<levelSet->getNumberOfComponents(); j++)
         {
          (*diag_vec)->push_back( 
           ( 2 - (levelSet->getComponent(j)->getEulerChar() +
           levelSet->getComponent(j)->getNumberOfBoundaryComponents() ) ) / 2 );
          (*diag_vec)->push_back(
            levelSet->getComponent(j)->getNumberOfBoundaryComponents() );
         }
        } // end diag_vec filling

     //  first step, number of components.
     if (levelSet->getNumberOfComponents()!=1) 
       { retval = false; if (diag_vec==NULL) return false; }
     if (vtx->isBoundary())
      { // check levelset is D^2
       if ( (levelSet->getNumberOfBoundaryComponents()!=1) || 
            (levelSet->getEulerChar() != 1) ) 
       { retval = false; if (diag_vec==NULL) return false; }
      }
     else
      { // check levelset is S^2
       if (levelSet->getNumberOfBoundaryComponents()!=0)
        { retval = false; if (diag_vec==NULL) return false; }
       if (levelSet->getEulerChar() != 2)
        { retval = false; if (diag_vec==NULL) return false; }
      }
     // done, clean up
     delete levelSet;     // delete NSV; Ben says NSurf takes ownership. 
     delete (*vInc);      delete (vInc); 
     delete vLink;
    }// end vertex loop i
  } // end tri4 case

 #ifdef DEBUG
 if (diag_vec != NULL) {
  std::cout<<"NMapToS1::verifySimpleS1Bundle() ERROR. "<<
             "Should be unreachable code."<<std::endl;
  exit(1); }
 #endif

 return retval;
}

namespace { // unnamed namespace for class intended to be private  
 // to NMapToS1::triangulateFibre
    struct EDGEID {
      unsigned long triIdx; // which triangle is it in?
      unsigned long linking;// which vertex does it link?
      unsigned long level;  // which level is it? 
      EDGEID( unsigned long TI, unsigned long LK, unsigned long LV )
       { triIdx = TI; linking = LK; level = LV; }
      bool operator<(const EDGEID &other) const
       { if (triIdx < other.triIdx)   return true;   
         if (triIdx > other.triIdx)   return false;
         if (linking < other.linking) return true; 
         if (linking > other.linking) return false; 
         if (level < other.level)     return true;     
         if (level > other.level)     return false;
         return false; }
    };
} // end unnamed namespace


void NMapToS1::triangulateFibre( const std::vector<NRational> &cocy, 
        Dim1Triangulation** TRI1, 
        Dim2Triangulation** TRI2, 
        NTriangulation** TRI3 )
{
 // step 1: let's compute the image in Q/Z of all the vertex values.  This will
 //  allow us to compute the fiber at any mid-point between these values. 
 //  we store the mid-points in a std::vector< NRational >.  We initialize this
 //  by taking the map with vertex 0 being mapped to 0. 
 unsigned long numVrts( tri2 ? tri2->getNumberOfVertices() :
                               tri3 ? tri3->getNumberOfVertices() :
                               tri4->getNumberOfVertices() ); 
 std::map< unsigned long, NRational > vtxVal;
 // let's define vtxVals by vtxVal[0] = 0, and then the rest by crawling via a
 //  maximal tree in the 1-skeleton.  Maybe store vtxVal as a 
 //  map< unsigned long, NRational > for purpose of definition? 
 //  we can store a local std::set< vertex* > for the vertices we haven't 
 //  explored. 
 std::set< unsigned long > expVrts;
 expVrts.insert(0);
 while (expVrts.size() != 0)
  { // explore *expVrts.begin(), remove, add any adjacent vertices to the list
    //  and fill-out vtxVal in the process
   unsigned long vrtIdx( *expVrts.begin() );   expVrts.erase( vrtIdx ); 
   // find all edges out of vrtIdx
   std::list< std::pair< Dim2Edge*, unsigned long > > dim2Map;
   std::list< std::pair< NEdge*,    unsigned long > > dim3Map;
   std::list< std::pair< Dim4Edge*, unsigned long > > dim4Map;
   if (tri2)      dim2Map = dim2inc[ tri2->getVertex(vrtIdx) ];
   else if (tri3) dim3Map = dim3inc[ tri3->getVertex(vrtIdx) ];
   else           dim4Map = dim4inc[ tri4->getVertex(vrtIdx) ];

   if (tri2) for (std::list< std::pair< Dim2Edge*, unsigned long > >::iterator 
     i=dim2Map.begin(); i!=dim2Map.end(); i++)
    { // i->first is the edge, and i->second is the end corresponding 
      //  to vertex vrtIdx. 
      unsigned long endpt( i->second ? 0 : 1 );
      if (vtxVal.find( tri2->vertexIndex( i->first->getVertex( endpt ) ) )
           == vtxVal.end() )
       { // unexplored!
         expVrts.insert( tri2->vertexIndex( i->first->getVertex( endpt ) ) );
         vtxVal[ tri2->vertexIndex( i->first->getVertex( endpt ) ) ] = 
          vtxVal[ tri2->vertexIndex( i->first->getVertex( i->second ) ) ] +
          ( endpt ? cocy[ tri2->edgeIndex( i->first ) ] :
                   -cocy[ tri2->edgeIndex( i->first ) ] ); 
       }
    } else 
   if (tri3) for (std::list< std::pair< NEdge*, unsigned long > >::iterator 
     i=dim3Map.begin(); i!=dim3Map.end(); i++)
    { // i->first is the edge, and i->second is the end corresponding 
      // to vertex vrtIdx. 
      unsigned long endpt( i->second ? 0 : 1 );
      if (vtxVal.find( tri3->vertexIndex( i->first->getVertex( endpt ) ) )
          == vtxVal.end() )
       { // unexplored!
         expVrts.insert( tri3->vertexIndex( i->first->getVertex( endpt ) ) );
         vtxVal[ tri3->vertexIndex( i->first->getVertex( endpt ) ) ] = 
          vtxVal[ tri3->vertexIndex( i->first->getVertex( i->second ) ) ] +
          ( endpt ? cocy[ tri3->edgeIndex( i->first ) ] :
                   -cocy[ tri3->edgeIndex( i->first ) ] ); 
       }
    } else // dim4 below 
    for (std::list< std::pair< Dim4Edge*, unsigned long > >::iterator 
     i=dim4Map.begin(); i!=dim4Map.end(); i++)
    { // i->first is the edge, and i->second is the end corresponding to 
      //  vertex vrtIdx. 
      unsigned long endpt( i->second ? 0 : 1 );
      if (vtxVal.find( tri4->vertexIndex( i->first->getVertex( endpt ) ) )
          == vtxVal.end() )
       { // unexplored!
         expVrts.insert( tri4->vertexIndex( i->first->getVertex( endpt ) ) );
         vtxVal[ tri4->vertexIndex( i->first->getVertex( endpt ) ) ] = 
          vtxVal[ tri4->vertexIndex( i->first->getVertex( i->second ) ) ] +
          ( endpt ? cocy[ tri4->edgeIndex( i->first ) ] :
                   -cocy[ tri4->edgeIndex( i->first ) ] ); 
       }
    } 
  } // end vtxVal definition

 // let's do a sanity check that vtxVal is defined on all the vertices (and no
 // more) and define midPts at the same time. At the same time we should reduce
 // modulo 1 the vtxVals...   oh, we should turn the vtxVals into a set!  
 // as they're not ordered, and their may be duplicates. 
 std::set< NRational > setVal;
 unsigned long pIdx(0);
 for (std::map< unsigned long, NRational >::iterator i=vtxVal.begin(); 
      i!=vtxVal.end(); i++)
  { 
    #ifdef DEBUG
    if (i!=vtxVal.begin()) if (i->first != pIdx+1) {
     std::cout<<"NMapToS1::triangulateFibre() indexing error 1."<<std::endl;
     exit(1); }
    #endif
    pIdx = i->first;
    NLargeInteger num( i->second.getNumerator() );
    NLargeInteger den( i->second.getDenominator() );
    num %= den;
    if (num < 0) num += den;
    setVal.insert( NRational(num, den) );
  }
 #ifdef DEBUG
 if (pIdx + 1 != numVrts) {
     std::cout<<"NMapToS1::triangulateFibre() indexing error 2."<<std::endl;
     exit(1); }
 #endif

 std::set< NRational > midPts;
 NRational prevVal(0,1);
 for (std::set< NRational >::iterator i=setVal.begin(); i!=setVal.end(); i++)
  {
   std::set< NRational >::iterator j=i; j++;
   if (j!=setVal.end()) midPts.insert( ((*i)+(*j))*NRational(1,2) ); 
    else midPts.insert( ( (*i) + 1 )*NRational(1,2) );
  }

 // step 2: the image of an n-simplex in R is divided into n intervals
 //  [min==vtx0, vtx1] [vtx1, vtx2] ... [vtxn-1, vtxn==max].  Lets build
 // this list for all the simplices.  To compute this we have to start with
 // the vtx with all up arrows, and take the smallest up arrow, then the
 // smallest up arrow from that, etc.  This can be done recursively. 
 std::vector< std::vector< NRational > > simpInt( 
   tri2 ? tri2->getNumberOfSimplices() : tri3 ? tri3->getNumberOfSimplices() :
          tri4->getNumberOfSimplices() ); // vtx0 through vtxn for each simplex.
 std::vector< std::vector< unsigned long > > simpIntVrt( simpInt.size() );
 // this lists the vertices of the simplices, in their relative ordering under
 // the lift to R.  

 for (unsigned long i=0; i<simpInt.size(); i++)
  { // build intervals using a map< NRational, unsigned long > object.
   std::map< NRational, unsigned long > sortDelta;
   // step 1: run through the vertices and find the minimum.
   unsigned long minVTX; // index of minimal vertex in simplex i
   for (minVTX=0; minVTX<(tri2 ? 3 : tri3 ? 4 : 5); minVTX++)
    {// decide of in this is the minimum, if so, break. 
     if (tri2) 
      { // 3 possibilities for min, vtx 0 1 or 2.
       if ( ( cocy[ tri2->edgeIndex( tri2->getSimplex(i)->
                        getEdge( (minVTX + 1) % 3 ) ) ]
            * NRational( (tri2->getSimplex(i)->
                getEdgeMapping( (minVTX + 1) % 3)[0]==minVTX) ? 1 : -1, 1) > 0 )
         && ( cocy[ tri2->edgeIndex( tri2->getSimplex(i)->
                        getEdge( (minVTX + 2) % 3 ) ) ]
            * NRational( (tri2->getSimplex(i)->getEdgeMapping( 
                (minVTX + 2) % 3)[0]==minVTX) ? 1 : -1, 1) > 0 ) )
         break;
      }
     else 
      { // the tri3 or tri4 case
       unsigned long updircount(0);
       for (unsigned long j=0; j<(tri3 ? 4 : 5); j++)
       {
        if (j==minVTX) continue;
        // get the edge between minVTX and j, get its edgeMapping, and eval cocy
        unsigned long eNum( tri3 ? edgeNumber[minVTX][j] : 
                                   Dim4Edge::edgeNumber[minVTX][j] );
        if (tri3) if ( cocy[ tri3->edgeIndex( tri3->getSimplex(i)->
                        getEdge( eNum ) ) ]
           * NRational( (tri3->getSimplex(i)->getEdgeMapping( eNum )[0]==
                        minVTX ) ? 1 : -1 ) > 0 )
          updircount++;
        if (tri4) if ( cocy[ tri4->edgeIndex( tri4->getSimplex(i)->
                getEdge( eNum ) ) ]
           * NRational( (tri4->getSimplex(i)->getEdgeMapping( eNum )[0]==
                minVTX ) ? 1 : -1 ) > 0 )
           updircount++;
       }
       if (updircount == (tri3 ? 3 : 4)) break;
      } // end tri3 or tri4 branch
    } // end minVTX loop
   #ifdef DEBUG
   if (minVTX==(tri2 ? 3 : tri3 ? 4 : 5)) 
    { std::cout<<"minVTX out of bounds."<<std::endl; exit(1); }
   #endif
   // step 2: build simpInt[i]. Let's do this by building a 
   //  map< NRational, unsigned long > which goes from the lifted values of 
   //   the vertices to the vertex index in the simplex. 
   unsigned long minVtxIdx( 
        tri2 ? tri2->vertexIndex( tri2->getSimplex(i)->getVertex(minVTX) ) :
        tri3 ? tri3->vertexIndex( tri3->getSimplex(i)->getVertex(minVTX) ) :
               tri4->vertexIndex( tri4->getSimplex(i)->getVertex(minVTX) ) ); 
   std::map< NRational, unsigned long > liftSimpVtxVal;
   // we initialize at liftSimpVtxVal -- the lifts of vtxVal to the simplex 
   // that allows for continuous extension of the lift over the entire simplex. 
   liftSimpVtxVal[ vtxVal[ minVtxIdx ] ] = minVTX;
   if (tri2)
    {
     // put in the 2 vals for the edges from minVTX to minVTX+1 % 3 
     //  and minVTX+2 % 3
     NRational V1( cocy[ tri2->edgeIndex( tri2->getSimplex(i)->
                       getEdge( (minVTX+2) % 3 ) ) ] );
     if ( (tri2->getSimplex(i)->getEdgeMapping( (minVTX+2) % 3 )[0]==minVTX) ? 
          (V1 > 0) : (V1 < 0) )
      liftSimpVtxVal[ vtxVal[ minVtxIdx ] + V1.abs() ] = (minVTX+1) % 3;
     #ifdef DEBUG
     else { std::cout<<"NMapToS1::triangulateFibre() lift error 1."<<
                       std::endl; exit(1); }
     #endif

     NRational V2( cocy[ tri2->edgeIndex( tri2->getSimplex(i)->
                        getEdge( (minVTX+1) % 3 ) ) ] );
     if ( (tri2->getSimplex(i)->getEdgeMapping( (minVTX+1) % 3 )[0]==minVTX) ?
          (V2 > 0) : (V2 < 0) )
      liftSimpVtxVal[ vtxVal[ minVtxIdx ] + V2.abs() ] = (minVTX+2) % 3;
     #ifdef DEBUG
     else { std::cout<<"NMapToS1::triangulateFibre() lift error 2."<<
                       std::endl; exit(1); }
     #endif
    }
   else for (unsigned long j=0; j<(tri3 ? 4 : 5); j++)
    {
     if (minVTX==j) continue;
     unsigned long eNum( tri3 ? edgeNumber[ minVTX ][ j ] : 
                      Dim4Edge::edgeNumber[ minVTX ][ j ] );
     NRational V1( cocy[ tri3 ? 
        tri3->edgeIndex( tri3->getSimplex(i)->getEdge(eNum) ) :
        tri4->edgeIndex( tri4->getSimplex(i)->getEdge(eNum) ) ] );
     if (tri3) { if ( (tri3->getSimplex(i)->getEdgeMapping( eNum )[0]==minVTX) ?
                  (V1 > 0) : (V1 < 0) )
      liftSimpVtxVal[ vtxVal[ minVtxIdx ] + V1.abs() ] = j;
      else { std::cout<<"NMapToS1::triangulateFibre() lift error 3."<<
                        std::endl; exit(1); } 
      }
     else { if ( (tri4->getSimplex(i)->getEdgeMapping( eNum )[0]==minVTX) ?
                  (V1 > 0) : (V1 < 0) )
      liftSimpVtxVal[ vtxVal[ minVtxIdx ] + V1.abs() ] = j;
      else { std::cout<<"NMapToS1::triangulateFibre() lift error 4."<<
                        std::endl; exit(1); } 
      }
    }
   // now liftSimpVtxVal defined!
   // std::map< NRational, unsigned long > liftSimpVtxVal;
   // quick check that there are (tri2 ? 3 : tri3 ? 4 : 5) values 
   #ifdef DEBUG
   if (liftSimpVtxVal.size() != (tri2 ? 3 : tri3 ? 4 : 5) )
    { std::cout<<"NMapToS1::triangulateFibre() lift error 5."<<
                 std::endl; exit(1); }
   #endif
   std::vector< NRational > vecLift; 
   std::vector< unsigned long > vrtLift; 
   vecLift.reserve( tri2 ? 3 : tri3 ? 4 : 5 );
   vrtLift.reserve( tri2 ? 3 : tri3 ? 4 : 5 );
   for (std::map< NRational, unsigned long >::iterator j=liftSimpVtxVal.begin();
        j!=liftSimpVtxVal.end(); j++)
    {
     vecLift.push_back( j->first );
     vrtLift.push_back( j->second );
    }
   simpInt[i] = vecLift;
   simpIntVrt[i] = vrtLift;
  } // i loop simpInt
 // TODO: low-priority.  Sometimes we don't appear to be choosing optimal lifts.
 //  For example, the single-tet triangulated S^1 x D^2 gets a lift with values
 //  { 1,2,3,4 }.  We should probably be smarter and choose the { 0,1,2,3 } lift
 //  instead.  But this is small potatoes as it will likely never result in much
 //  time savings.

 // NOTE: this choice here is arbitrary, and we should allow varying it to check
 //  for robustness, eventually to put into the test suite. 
 NRational LVL( *midPts.begin() ); // the level we compute the level-set at.

 // the number of solutions in an interval [simpInt[i][j], simpInt[i][j+1]]
 // at level LVL is the difference of the two q_{j+1} - q_j where
 // q_j*den + r_j = num for the rational number simpInt[i][j] - LVL = num/den 
 // we can use divisionAlg() to compute this.  We will put these solution sets
 // in a vector of length the dimension of the simplex. 
 std::vector< std::vector<unsigned long> > normCount( simpInt.size() );
 unsigned long DIM( tri2 ? 2 : tri3 ? 3 : 4 ); // simpInt.size() ==
 for (unsigned long i=0; i<simpInt.size(); i++) //       num simplices
  { // this is the set-up normCount loop
   NRational Diff[DIM+1];
   for (unsigned long j=0; j<(DIM+1); j++)
    Diff[j] = simpInt[i][j] - LVL;
   NInteger REM;
   normCount[i].resize(DIM);
   for (unsigned long j=0; j<DIM; j++)
    normCount[i][j] = ( 
     (Diff[j+1].getNumerator().divisionAlg( Diff[j+1].getDenominator(), REM ) -
      Diff[j].getNumerator().divisionAlg( Diff[j].getDenominator(), REM ) ).
        longValue() );
  } 

 if (tri2)
  { // dim2, we'll have to do by hand. dim3 and dim4 use normal surfaces.
    // each simplex has two halfs [min, vtx], [vtx, max] corresponding to 
    // normCount[i][0, 1].  Let's start by creating the Dim1Triangulation, 
    // and a map to keep track of the edges. 
    #ifdef DEBUG
    if (TRI1==NULL) { std::cout<<"NMapToS1::triangulateFibre called with "<<
                                 "a null pointer (2D)."<<std::endl;
                      exit(1); }
    #endif
    (*TRI1) = new Dim1Triangulation();
    // cast simpIntVrt into vector of NPerm3's. 
    std::vector< NPerm3 > simpIntPerm( simpIntVrt.size() );
    for (unsigned long i=0; i<simpIntPerm.size(); i++)
     simpIntPerm[i] = NPerm3( simpIntVrt[i][0], simpIntVrt[i][1], 
                              simpIntVrt[i][2] );
    
    // let's create the edges
    std::map< Dim1Edge*, EDGEID > edIdx; // keeps track of edges. 
    std::map< EDGEID, Dim1Edge* > edIdxR;
    for (unsigned long i=0; i<tri2->getNumberOfSimplices(); i++)
     for (unsigned long j=0; j<2; j++)
      for (unsigned long k=0; k<normCount[i][j]; k++)
       {
        Dim1Edge* nEdg( (*TRI1)->newEdge() );
        edIdx.insert( std::pair< Dim1Edge*, EDGEID >( nEdg, 
         EDGEID(i, (j==0) ? simpIntPerm[i][0] : simpIntPerm[i][2], k) ) ); 
        edIdxR.insert( std::pair< EDGEID, Dim1Edge* >(
         EDGEID(i, (j==0) ? simpIntPerm[i][0] : simpIntPerm[i][2], k), nEdg ) );
       }

    // *** Gluings ***

    // Orient the intervals from the short side of the triangle
    //  to the "long" side -- with the extra vertex. 
    // simpIntVrt[i][1] is the middle vertex index.  0 to 1 is the long edge.
    // I suppose we can keep track via pair< ulong, ulong >, indicating the 
    // simplex index and 2*layer + 0 or 1 depending on if in top of bottom
    // let's perform the gluings.  I guess run through all the non-boundary 
    // edges... seems like a good way to go
    for (unsigned long i=0; i<tri2->getNumberOfEdges(); i++)
     {
      const Dim2Edge* edg( tri2->getEdge(i) );
      if (edg->isBoundary()) continue;
      // now look at the two inclusions and assemble the gluing. 
      unsigned long tri0idx( tri2->triangleIndex( 
                edg->getEmbedding(0).getTriangle() ) );
      unsigned long vtx0( edg->getEmbedding(0).getEdge() );
      unsigned long opp0idx( (vtx0==simpIntPerm[tri0idx][1]) ? vtx0 :
                             (vtx0==simpIntPerm[tri0idx][0]) ? 
                            simpIntPerm[tri0idx][2] : simpIntPerm[tri0idx][0] );
      NPerm3 e0inc( edg->getEmbedding(0).getVertices() );

      unsigned long tri1idx( tri2->triangleIndex( 
                edg->getEmbedding(1).getTriangle() ) );
      unsigned long vtx1( edg->getEmbedding(1).getEdge() );
      unsigned long opp1idx( (vtx1==simpIntPerm[tri1idx][1]) ? vtx1 :
                             (vtx1==simpIntPerm[tri1idx][0]) ? 
                            simpIntPerm[tri1idx][2] : simpIntPerm[tri1idx][0] );
      NPerm3 e1inc( edg->getEmbedding(1).getVertices() );

      // now we have to perform the gluings... first of all, how many edges are
      // there being glued?  Decide if its all edges in the triangle, or 
      // only half.
      bool glueboth0( (vtx0 == simpIntPerm[ tri0idx ][1]) );
      bool glueboth1( (vtx1 == simpIntPerm[ tri1idx ][1]) );
      // let's do a quick check to see the numbers add up on both sides. 
      unsigned long count0[3] = { normCount[ tri0idx ][0], 
        normCount[ tri0idx ][0]+normCount[ tri0idx ][1], 
        normCount[ tri0idx ][1] };
      unsigned long count1[3] = { normCount[ tri1idx ][0], 
        normCount[ tri1idx ][0]+normCount[ tri1idx ][1], 
        normCount[ tri1idx ][1] };

      #ifdef DEBUG
      if ( count0[ simpIntPerm[ tri0idx ].preImageOf( opp0idx ) ] !=
           count1[ simpIntPerm[ tri1idx ].preImageOf( opp1idx ) ] )
       { std::cout<<"NMapToS1::triangulateFibre() matching equations error "<<
          count0[ simpIntPerm[ tri0idx ].preImageOf( opp0idx ) ]<<" != "<<
          count1[ simpIntPerm[ tri1idx ].preImageOf( opp1idx ) ]<<std::endl; 
         std::cout<<"Gluing triangle "<<tri0idx<<" (edge "<<vtx0<<
                    ") to triangle "<<tri1idx<<" (edge "<<vtx1<<")"<<std::endl; 
         exit(1); } 
      #endif
      // gluings
      for (unsigned long j=0; j<count0[simpIntPerm[ tri0idx ].preImageOf( opp0idx )]; j++)
       {
        // perform edge gluings appropriately..
        EDGEID EID0( tri0idx, (!glueboth0) ? opp0idx : 
           (j<count0[0]) ? simpIntPerm[tri0idx][0] : simpIntPerm[tri0idx][2],
           (!glueboth0) ? j : (j<count0[0]) ? j : j-count0[0] );
        EDGEID EID1( tri1idx, (!glueboth1) ? opp1idx : 
           (j<count1[0]) ? simpIntPerm[tri1idx][0] : simpIntPerm[tri1idx][2],
           (!glueboth1) ? j : (j<count1[0]) ? j : j-count1[0] );
        #ifdef DEBUG
        if (edIdxR.find(EID0)==edIdxR.end()) { std::cout<<"NMapToS1::"<<
                "triangulateFibre() EID0 not found."; exit(1); }
        if (edIdxR.find(EID1)==edIdxR.end()) { std::cout<<"NMapToS1::"<<
                "triangulateFibre() EID1 not found."; exit(1); }
        #endif
        (*TRI1)->joinEdges( edIdxR[EID0], glueboth0 ? 0 : 1, 
                            edIdxR[EID1], glueboth1 ? 0 : 1 );        
       }
     }
   return;
  }
 else if (tri3)
  { // dim3, use NNormalSurface
    // [min, vtx1] [vtx1,vtx2] [vtx2,max]
    //  tri         quad         tri 
    #ifdef DEBUG
    if (TRI2==NULL) { std::cout<<"NMapToS1::triangulateFibre called with a"<<
                                 " null pointer (3D)."<<std::endl;
                      exit(1); }
    #endif
    NNormalSurfaceVector* 
       NSV( NNormalSurfaceVectorStandard::makeZeroVector( tri3 ) );
    // 7*tetindex + vtx for triangle types
    // 7*tetindex + 4 + vertexSplit[][] for quad types.
    for (unsigned long i=0; i<tri3->getNumberOfSimplices(); i++)
     {
      NSV->setElement( 7*i + simpIntVrt[i][0], normCount[i][0] );
      NSV->setElement( 7*i + 4 + vertexSplit[simpIntVrt[i][0]]
                [simpIntVrt[i][1]], normCount[i][1] );
      NSV->setElement( 7*i + simpIntVrt[i][3], normCount[i][2] );
     }
    NNormalSurface NSurf( tri3, NSV );
    (*TRI2) = NSurf.triangulate();
  }
 else
  { // dim4, use NNormalHyperSurface
    // [min, vtx1] [vtx1, vtx2] [vtx2,vtx3] [vtx3,max]
    //  tri          prism         prism      tri
    #ifdef DEBUG
    if (TRI3==NULL) { std::cout<<"NMapToS1::triangulateFibre called with a"<<
                                 " null pointer (4D)."<<std::endl;
                      exit(1); }
    #endif

    NNormalHypersurfaceVector* 
       NSV( NNormalHypersurfaceVectorStandard::makeZeroVector( tri4 ) );

    for (unsigned long i=0; i<tri4->getNumberOfSimplices(); i++)
     { // 15*pent index + vertexnum for tets
       // 15*pent index + 5 + prismtype via Dim4Edge::edgeNumber[5][5].
       NSV->setElement( 15*i + simpIntVrt[i][0], normCount[i][0] );
       NSV->setElement( 15*i + 5 + Dim4Edge::edgeNumber[simpIntVrt[i][0]]
            [simpIntVrt[i][1]], normCount[i][1] );
       NSV->setElement( 15*i + 5 + Dim4Edge::edgeNumber[simpIntVrt[i][3]]
            [simpIntVrt[i][4]], normCount[i][2] );
       NSV->setElement( 15*i + simpIntVrt[i][4], normCount[i][3] );
     } 

    NNormalHypersurface NSurf( tri4, NSV );
    (*TRI3) = NSurf.triangulate();
    return;
  }
 // perhaps put a check here to ensure routine never gets to here. 
 return;
}

// The current evolution of this routine involves the short and long term.
//
//  TODO: storing vertex links (in 4-manifold case) in the NMapToS1
//   class to avoid repeatedly rebuilding them.  How much time does it save?
//
//  LONG TERM: once we have a flexible triangulation we can try more intelligent
//   perturbations of the cocycle, in an attempt to designularize the level
//   set vertex links.  We now have diagnostics/return values in 
//   verifySimpleS1Bundle.  Let's start to use them dynamically. 
//
bool NMapToS1::findS1Bundle(findS1BundleAbortReason &FSBAR, 
      std::vector<NRational> &COCY)
{
 FSBAR = FSBAR_success; // temporary status holder for now.

 unsigned long numVrt( tri2 ? tri2->getNumberOfVertices() :
                       tri3 ? tri3->getNumberOfVertices() :
                              tri4->getNumberOfVertices() );
 unsigned long numEdg( tri2 ? tri2->getNumberOfEdges() : 
                       tri3 ? tri3->getNumberOfEdges() :
                       tri4->getNumberOfEdges() );
 unsigned long numTri( tri2 ? tri2->getNumberOfTriangles() :
                       tri3 ? tri3->getNumberOfTriangles() :
                              tri4->getNumberOfTriangles() );
std::cout<<"vrts: "<<numVrt<<" edges: "<<numEdg<<" tris: "<<numTri<<"\n";

 NCellularData* cDat;
 if      (tri3) cDat = new NCellularData( *tri3 );
 else if (tri4) cDat = new NCellularData( *tri4 );
 else 
  { // TODO: eventually add a 2-manifold algorithm.
   FSBAR = FSBAR_invalidinput;
   return false;
  } // long-term it would be best to add a 2-manifolds constructor to
    // NCellularData, and make ncellulardata class even more light-weight, 
    // so that it only builds partial chain complexes. 

 const NMarkedAbelianGroup* H1( 
    cDat->markedGroup( NCellularData::GroupLocator(1, 
     NCellularData::contraVariant, NCellularData::STD_coord, 0) ) );

 // presently quit if H1 rank isn't just 1. Eventually we might want to
 //  consider a more elaborate search. 
 if (H1->getRank() != 1) { delete cDat; FSBAR = FSBAR_h1rank; return false; } 
 // 1) find the cochain rep for the H^1 generator
 std::vector< NLargeInteger > ccGen( H1->getFreeRep(0) );

std::cout<<"ccGen (1): ";
for (unsigned long i=0; i<ccGen.size(); i++)
{
 if (tri3) std::cout<<( (tri3->getEdge(i)->getVertex(0)==tri3->getEdge(i)->getVertex(1)) ? 
    "\033[1;31m" : "\033[1;37m" )<<ccGen[i]<<"\033[0m ";
 if (tri4) std::cout<<( (tri4->getEdge(i)->getVertex(0)==tri4->getEdge(i)->getVertex(1)) ? 
    "\033[1;31m" : "\033[1;37m" )<<ccGen[i]<<"\033[0m ";
}
std::cout<<"\n";

 // 2) build table of the bad edges
 std::set< unsigned long > badEdgeIdx;
  for (unsigned long i=0; i<numEdg; i++)
  {
   if (tri2 ? tri2->getEdge(i)->getVertex(0) == tri2->getEdge(i)->getVertex(1) :
       tri3 ? tri3->getEdge(i)->getVertex(0) == tri3->getEdge(i)->getVertex(1) :
              tri4->getEdge(i)->getVertex(0) == tri4->getEdge(i)->getVertex(1) )
   if (ccGen[i] == NLargeInteger::zero)
    badEdgeIdx.insert(i);
  }

 // Special case: if there are no bad edges, let's cast ccGen into a cocycle
 // and check it. 
std::cout<<"badEdgeIdx: ";
for (std::set< unsigned long >::const_iterator i = badEdgeIdx.begin(); i!= badEdgeIdx.end(); i++)
 std::cout<<(*i)<<" ";
std::cout<<"\n";

 if (badEdgeIdx.empty())
  {
   std::vector< NRational > cocy( ccGen.size() );
   for (unsigned long i=0; i<ccGen.size(); i++)
    cocy[i] = NRational( ccGen[i], NLargeInteger::one );

   if ( verifyPrimitiveH1( cocy ) ) if (verifySimpleS1Bundle(cocy)) 
    { COCY=cocy; FSBAR = FSBAR_success; return true; }
  }

 if (tri2) { FSBAR = FSBAR_invalidinput; delete cDat; return false; }
 // TODO: perhaps adapt this in the 2-dimensional case, but perhaps that's
 //  a waste of time?  We might want to make an inductive algorithm, in that
 //  case the 2-dimensional situation will be important.

 // Step (a) cast badEdgeIdx appropriately and call divideEdges. 
 delete cDat;  cDat=NULL;
 if (tri3)
  {
   std::set< const NEdge* > splitEdges;
   for (std::set< unsigned long >::iterator i=badEdgeIdx.begin(); i!=badEdgeIdx.end(); i++)
    splitEdges.insert( tri3->getEdge(*i) );
// TODO: fix   tri3->divideEdges(splitEdges);   
  }
 else
  {
   std::set< const Dim4Edge* > splitEdges;
   for (std::set< unsigned long >::iterator i=badEdgeIdx.begin(); i!=badEdgeIdx.end(); i++)
    splitEdges.insert( tri4->getEdge(*i) );
// TODO: fix   tri4->divideEdges(splitEdges);   
  }
 // we should perhaps consider this preamble to be something to go into a
 // conditionTriangulation routine. 
std::cout<<"After divideEdges, triangulation has "<<(tri3 ? tri3->getNumberOfSimplices() : 
        tri4->getNumberOfSimplices())<<" simplices and ";
// okay this is a problem.  A 2-knot exterior with 6 pens after idealtofinite and intelligentsimplify
// has 72 pens, but then it has only 1 vertex with 11 bad edges.  After divideOnEdges we have
// 2688 simplices...  Ouch.  
std::cout<<(tri3 ? tri3->getNumberOfVertices() : tri4->getNumberOfVertices())<<" vertices ";
std::cout<<(tri3 ? tri3->getNumberOfEdges() : tri4->getNumberOfEdges())<<" edges ";
std::cout<<(tri3 ? tri3->getNumberOfTriangles() : tri4->getNumberOfTriangles())<<" triangles"<<std::endl;
 // Step (b) If we can find appropriate edges to collapse, let's try to do that.
 //  the idea will be to look through pairs of edges, check to see if they 
 //  start and end at the same places (or reverse), if so, check if the loop
 //  is homologically trivial.  If the first edge has no such 2nd edge, then we
 //  can crush it... I think.   There might be special cases so we should 
 //  consider possibly backtracking / undoing the move if it leads to bad 
 //  business.  TODO
 
 // we'll make this some kind of while loop.  While didsomething
 // TODO: perhaps consider a collapse edge routine that does not need homological data
 //  to make decisions.  First collapse all edges that do not have a complementary edge
 //  forming a 2-edge closed loop. 
 bool hFlag(false); // we've given up on homology-less computations when true
 bool didSomething(true);
 while (didSomething)
  {
   didSomething = false;
   if (cDat) { delete cDat; cDat = NULL; }
   if      (hFlag && tri3) cDat = new NCellularData( *tri3 );
   else if (hFlag && tri4) cDat = new NCellularData( *tri4 );
   if (hFlag) {
   H1 = cDat->markedGroup( NCellularData::GroupLocator(1, 
        NCellularData::contraVariant, NCellularData::STD_coord, 0) );
   ccGen = H1->getFreeRep(0);

   for (unsigned long i=0;i<ccGen.size();i++) {
     std::cout<<( (tri4->getEdge(i)->getVertex(0)==tri4->getEdge(i)->getVertex(1)) ? 
     "\033[1;31m" : "\033[1;37m" )<<ccGen[i]<<"\033[0m ";
     }
    std::cout<<"\n";
    } // end hFlag branch

   numVrt = ( tri2 ? tri2->getNumberOfVertices() :
              tri3 ? tri3->getNumberOfVertices() :
                     tri4->getNumberOfVertices() );
   numEdg = ( tri2 ? tri2->getNumberOfEdges() : 
              tri3 ? tri3->getNumberOfEdges() :
                     tri4->getNumberOfEdges() );
   numTri = ( tri2 ? tri2->getNumberOfTriangles() :
              tri3 ? tri3->getNumberOfTriangles() :
                     tri4->getNumberOfTriangles() );
std::cout<<"collapseEdge loop: numVrt "<<numVrt<<" numEdg "<<numEdg<<" numTri "<<numTri
 <<" simp: "<<(tri3 ? tri3->getNumberOfTetrahedra() : tri4->getNumberOfPentachora())<<std::endl;
if (hFlag) std::cout<<"Checking for null 1-edge loops.\n";
if (hFlag) for (unsigned long i=0; i<numEdg; i++)
 if ( (tri3 ? tri3->getEdge(i)->getVertex(0)==tri3->getEdge(i)->getVertex(1) :
              tri4->getEdge(i)->getVertex(0)==tri4->getEdge(i)->getVertex(1)) && (ccGen[i]==0) )
 { std::cout<<"Edge "<<i<<" is closed and H1-trivial.   Impossible?"<<std::endl; exit(1); }
// TODO: the problem appears to be that after the divide edges routine, we still have
//  1-edge closed loops that are trivial in H1.  Why is that?? Probably I made a mistake
//  implementing divideEdges...

   for (unsigned long i=0; i<numEdg; i++)
    {
     // if we find a collapsible edge, try collapsing it in another copy of
     // the triangulation. Ben verified that if there are no 2-edge closed 
     // loops to begin with, there will not be any 1-edge closed loops in
     // the end. So we look for a 2nd edge that has the same endpoints as this
     // edge, but so that the associated closed loop is null.  If we find such
     // a thing, we abort.  If no such 2nd edge exists, we collapse. 
     bool foundBadSecondEdge(false);
     for (unsigned long j=0; j<numEdg; j++)
      {
       if (i==j) continue;
       // edge i and j connect vertex to vertex
       if ( (tri3 ? ( (tri3->getEdge(i)->getVertex(0) == tri3->getEdge(j)->getVertex(0)) &&
                      (tri3->getEdge(i)->getVertex(1) == tri3->getEdge(j)->getVertex(1)) ) : false ) ||
            (tri4 ? ( (tri4->getEdge(i)->getVertex(0) == tri4->getEdge(j)->getVertex(0)) &&
                      (tri4->getEdge(i)->getVertex(1) == tri4->getEdge(j)->getVertex(1)) ) : false ) )
        { // two distinct edges form closed loop, unoriented.
         if ( ( (hFlag) && (ccGen[i] - ccGen[j] == 0) ) || (!hFlag) ) foundBadSecondEdge=true;
        } else
       if ( (tri3 ? ( (tri3->getEdge(i)->getVertex(0) == tri3->getEdge(j)->getVertex(1)) &&
                      (tri3->getEdge(i)->getVertex(1) == tri3->getEdge(j)->getVertex(0)) ) : false ) ||
            (tri4 ? ( (tri4->getEdge(i)->getVertex(0) == tri4->getEdge(j)->getVertex(1)) &&
                      (tri4->getEdge(i)->getVertex(1) == tri4->getEdge(j)->getVertex(0)) ) : false ) )
        { // two distinct edges form closed oriented loop
         if ( ( (hFlag) && (ccGen[i] + ccGen[j] == 0) ) || (!hFlag) ) foundBadSecondEdge=true;
        }
       if (foundBadSecondEdge) break;
      } // end j loop
     if (foundBadSecondEdge) continue; // jump to next i.
     // no such edge exists, so we collapse edge i.
     bool edgeCollapsePerformed;
     if (tri3) edgeCollapsePerformed = 
        tri3->collapseEdge( tri3->getEdge(i), true, true ); 
     else if (tri4) edgeCollapsePerformed = 
        tri4->collapseEdge( tri4->getEdge(i), true, true );
if (edgeCollapsePerformed) std::cout<<"Collapse on edge "<<i<<"\n";
     // loop back to start of while loop if edge was collapsed.
     if (edgeCollapsePerformed) { didSomething = true; break; }
    } // end i loop
   if ( (!hFlag) && (didSomething==false) ) { 
     std::cout<<"hFlag turned on!\n"; 
     hFlag = true; didSomething = true; 
     }
  } // end while (didSomething)

if (tri4) if (tri4->getNumberOfPentachora() > 1000) exit(1);

 builddimNinc();
std::cout<<"ccGen (2): ";
for (unsigned long i=0; i<ccGen.size(); i++)
 std::cout<<ccGen[i]<<" ";
std::cout<<"\n";
   std::vector< NRational > cocy( ccGen.size() );

 // TODO: try ccGen for cocy, if no zero entries in cocycle. 

 // Step (c) find the generators of the image of the C^0 --> C^1 map, the transpose
 //    of the boundary map C_1 --> C_0. We can use H1->getN() for this. 
 //    Technically this is the same as the dim?inc data, but more usable. 

 // Step (d) Use feedback from the vertex level-set links to modify the
 //   potential fibering cocycle. 

 NMatrixInt C0C1Map( H1->getN() );
 delete cDat; // we don't need this anymore

 if (numVrt==1) 
    { FSBAR = FSBAR_onevtx; 
      #ifdef DEBUG    
      std::cout<<"vrts "<<numVrt<<" edg "<<numEdg<<" tris "<<numTri<<"\n";
      std::cout<<"NMapToS1::findS1Bundle routine should never go here."<<
                 std::endl; 
      exit(1); 
      #endif
      return false;
    }

 // if not, and if numVrt > 1, try perturbations.

 // TODO: invent a new reason to quit the search.  If we can find an edge where
 //  no cocycle modification can change the edges value

 // 3) explore the space of perturbations of our generator, relevant to the 
 //    bundle construction. Eventually we should make this a routine that 
 //    considers the level-set vertex links and attempts moves that 
 //    desingularize.  Right now let's start with single perturbations and 
 //    see where it gets us. 
 std::vector< unsigned long > **diag_vec( new std::vector< unsigned long >* );

 for (unsigned long k=0; k<5; k++)
  { // let's make several attempts
   for (unsigned long j=0; j<cocy.size(); j++)
    cocy[j] = NRational( ccGen[j], NLargeInteger(1) );
   for (unsigned long i=0; i<numVrt; i++)
    { // random number in [2,15]
     NLargeInteger randDen( NLargeInteger(14).randomBoundedByThis() + 2 ); 
     NLargeInteger randNum( randDen.randomBoundedByThis() );
     for (unsigned long j=0; j<cocy.size(); j++)
      cocy[j] += NRational(randNum,randDen)*NRational( C0C1Map.entry(j,i), 
        NLargeInteger(1) );
    }
   if ( verifyPrimitiveH1( cocy ) ) 
    {
     if ( verifySimpleS1Bundle( cocy, diag_vec ) ) 
      { 
       delete(*diag_vec);
       delete(diag_vec);
       COCY=cocy; FSBAR = FSBAR_success; return true; 
      } 
     // TODO: output diag_vec
std::cout<<"Level-set links: ";
     unsigned long vNum(0); // current vertex
     unsigned long vChangeIdx(0); // change-of-vertex index
     for (unsigned long i=0; i<(*diag_vec)->size(); i++)
      {
       if (i==vChangeIdx) {
        if (i!=0) vNum++;
        vChangeIdx += 2*(*(*diag_vec))[i]+1;
        std::cout<<"vtx "<<vNum<<" ";
        std::cout<<"C"<<(*(*diag_vec))[i]<<":";
        i++;
       }
      std::cout<<"g"<<(*(*diag_vec))[i];
      i++;
      std::cout<<"b"<<(*(*diag_vec))[i];
      }
  /*  For 4-manifolds one needs to
   *  parse the vector as a lexicographical ordering of a vector. The i-th vector
   *  will describe the level-set link of the i-th vertex.  The 0th entry of the
   *  i-th vector will be the number of components.  The 2j+1-th entry will be
   *  the genus of that component, and the 2j+2-th entry will be the number of
   *  boundary circles in that component.  It is the end-user's responsibility
   *  to deallocate diag_vec.  Note that the routine might be slightly slower
   *  if you pass an allocated diag_vec argument, as without this argument the
   *  algorithm fails at the first non-sphere vertex level-set link.  With an
   *  allocated diag_vec, the homeomorphism type of all vertex level-set links
   *  is determined.  */
     delete(*diag_vec);
    }
   else {
    std::cout<<"Non-primitive. cocy ";
    for (unsigned long i=0; i<cocy.size(); i++)
     std::cout<<((i!=0) ? "," : "(")<<cocy[i];
    std::cout<<")\n";
    }
  }
// bool verifySimpleS1Bundle( const std::vector<NRational> &cocy,
//                       std::vector< unsigned long > **diag_vec=NULL) const;

 FSBAR = FSBAR_other;
 return false;
}



// Dim1Triangulation code below
Dim1Triangulation::Dim1Triangulation() {}

Dim1Triangulation::~Dim1Triangulation()
{
 for (std::set< Dim1Edge* >::iterator i=edges.begin(); i!=edges.end(); i++)
  delete (*i);
}

Dim1Edge* Dim1Triangulation::newEdge(Dim1Edge* join0, unsigned f0,
                                     Dim1Edge* join1, unsigned f1 )
{ 
    if (join0!=NULL) if ( (f0==0) ? join0->vtx[0] != NULL :
                                    join0->vtx[1] != NULL ) return NULL;
    if (join1!=NULL) if ( (f1==0) ? join1->vtx[0] != NULL :
                                    join1->vtx[1] != NULL ) return NULL;
    // the above aborts if the gluing contradicts a previous gluing.
    Dim1Edge* nedg( new Dim1Edge );
    edges.insert( nedg );
    nedg->vtx[0] = join0;
    if (join0!=NULL) { if (f0==0) join0->vtx[0]=nedg; else join0->vtx[1]=nedg; }
    nedg->vtx[1] = join1;
    if (join1!=NULL) { if (f1==0) join1->vtx[0]=nedg; else join1->vtx[1]=nedg; }

    return nedg;
}

bool Dim1Triangulation::joinEdges( Dim1Edge* edg0, unsigned vtx0, 
                                   Dim1Edge* edg1, unsigned vtx1 )
{
 if ( (edg0->vtx[vtx0] != NULL) && (edg0->vtx[vtx0] != edg1) ) return false;
 if ( (edg1->vtx[vtx1] != NULL) && (edg1->vtx[vtx1] != edg0) ) return false;
 edg0->vtx[vtx0] = edg1; edg1->vtx[vtx1] = edg0;
 return true;
}

std::pair< unsigned long, unsigned long > 
    Dim1Triangulation::componentTypes() const
{
 std::pair<unsigned long, unsigned long > retval; // circles, intervals.
 std::set< Dim1Edge* > eCopy( edges );
 while (!eCopy.empty())
  {
   Dim1Edge* seedp( *eCopy.begin() );
   std::set< Dim1Edge* > todo; todo.insert(seedp);
   // let's sweep out a path, see if it closes or hits an end. 
   // if it hits an end, we know we have an interval.  If not, it is a circle.
   bool foundEndPt(false);
   while (!todo.empty())
    {
     Dim1Edge* newPOP( *todo.begin() );
     bool LRvec[2] = { false, false }; 

     for (unsigned long i=0; i<2; i++) { 
      if ( newPOP->vtx[i] == NULL ) foundEndPt = true;
      else if ( eCopy.find( newPOP->vtx[i] )!=eCopy.end() )
       LRvec[i] = true; // unexplored and valid across i
     }

     for (unsigned long i=0; i<2; i++) if (LRvec[i]) 
      if (newPOP->vtx[i] != newPOP) todo.insert(newPOP->vtx[i]);

     todo.erase( newPOP );
     eCopy.erase( newPOP );
    } // end todo loop
   // decide on component type
   if (foundEndPt) retval.second++; else retval.first++;
  }
 return retval;
}

std::set< Dim1Edge* >& Dim1Triangulation::allEdges()
{ return edges; }



} // end namespace regina
