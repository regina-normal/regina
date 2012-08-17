
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include "dim4/dim4edge.h"
#include "dim4/dim4isomorphism.h"
#include "dim2/dim2triangulation.h"
#include <sstream>

namespace regina {

const int Dim4Edge::edgeNumber[5][5] = {
    { -1, 0, 1, 2, 3 },
    { 0, -1, 4, 5, 6 },
    { 1, 4, -1, 7, 8 },
    { 2, 5, 7, -1, 9 },
    { 3, 6, 8, 9, -1 }};

const int Dim4Edge::edgeVertex[10][2] = {
    { 0, 1 },
    { 0, 2 },
    { 0, 3 },
    { 0, 4 },
    { 1, 2 },
    { 1, 3 },
    { 1, 4 },
    { 2, 3 },
    { 2, 4 },
    { 3, 4 }};

const NPerm5 Dim4Edge::ordering[10] = {
    NPerm5(0, 1, 2, 3, 4),
    NPerm5(0, 2, 1, 3, 4),
    NPerm5(0, 3, 1, 2, 4),
    NPerm5(0, 4, 1, 2, 3),
    NPerm5(1, 2, 0, 3, 4),
    NPerm5(1, 3, 0, 2, 4),
    NPerm5(1, 4, 0, 2, 3),
    NPerm5(2, 3, 0, 1, 4),
    NPerm5(2, 4, 0, 1, 3),
    NPerm5(3, 4, 0, 1, 2)
};

// TODO: have a pass-by-reference Dim4Isomorphism that describes
//       how the link is sitting in the Dim4Triangulation. 
std::auto_ptr< Dim2Triangulation > Dim4Edge::buildLink(Dim4Isomorphism* inc) const
{
    std::auto_ptr< Dim2Triangulation > retval ( new Dim2Triangulation );
    for (unsigned long i=0; i<getNumberOfEmbeddings(); i++)
       {
        Dim2Triangle* tTri( retval->newTriangle() );
        std::stringstream temp;
        Dim4Pentachoron* pen( getEmbedding(i).getPentachoron() );
        temp << pen->getTriangulation()->pentachoronIndex( pen );
        temp << " " << getEmbedding(i).getEdge();
        tTri->setDescription( temp.str() );        
       }     
    for (unsigned long i=0; i<getNumberOfEmbeddings(); i++) {
       const Dim4EdgeEmbedding eEmb( getEmbedding(i) );
       const NPerm5 edgInc( eEmb.getVertices() );

       for (unsigned long j=2; j<5; j++)
            if ( !(eEmb.getPentachoron()->getTetrahedron(edgInc[j])->isBoundary()) && 
               (retval->getTriangle(i)->adjacentTriangle(j-2)==NULL) ) { 
          NPerm5 penGlue( eEmb.getPentachoron()->adjacentGluing(edgInc[j]) );

          const Dim4EdgeEmbedding adjEdjInc( const_cast< Dim4Pentachoron* >
            (eEmb.getPentachoron()->adjacentPentachoron(edgInc[j])), 
             Dim4Edge::edgeNumber[penGlue[edgInc[0]]][penGlue[edgInc[1]]]);
          // lets lookup adjEdjInc in emb_
          unsigned long adjeEmbIndx = find(emb_.begin(), emb_.end(), 
                adjEdjInc) - emb_.begin();
          NPerm5 incTrans( 
            adjEdjInc.getVertices().inverse() * penGlue * edgInc );
          NPerm3 tPerm( incTrans[2]-2, incTrans[3]-2, incTrans[4]-2 );
          // which is the target triangle 
          retval->getTriangle(i)->joinTo(j-2, 
                retval->getTriangle(adjeEmbIndx), tPerm);
        }
     }

    if ( (inc != NULL) ? (inc->getSourcePentachora() == retval->getNumberOfTriangles()) : false )
      for (unsigned long j=0; j<getNumberOfEmbeddings(); j++)
       { // it will be set up so that 0 and 1 go to the edge vertices in the pentachora
         // 2,3,4 will go to the triangle opposite. 
         Dim4Pentachoron* pen( getEmbedding(j).getPentachoron() );
         inc->pentImage(j) = pen->getTriangulation()->pentachoronIndex( pen );
         inc->facetPerm(j) = getEmbedding(j).getVertices();
       }

    return retval;
}


} // namespace regina

