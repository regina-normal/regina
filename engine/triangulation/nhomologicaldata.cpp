
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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
#include "triangulation/nhomologicaldata.h"
#include <list>
#include <iostream>
#include <sstream>
#include <cmath>

namespace regina {


void NHomologicalData::writeTextShort(std::ostream& out) const {
    if (mHomology0) {
        out<<"H_0(M) = ";
        mHomology0->writeTextShort(out);
        out<<" ";
    }
    if (mHomology1) {
        out<<"H_1(M) = ";
        mHomology1->writeTextShort(out);
        out<<" ";
    }
    if (mHomology2) {
        out<<"H_2(M) = ";
        mHomology2->writeTextShort(out);
        out<<" ";
    }
    if (mHomology3) {
        out<<"H_3(M) = ";
        mHomology3->writeTextShort(out);
        out<<" ";
    }

    if (bHomology0) {
        out<<"H_0(BM) = ";
        bHomology0->writeTextShort(out);
        out<<" ";
    }
    if (bHomology1) {
        out<<"H_1(BM) = ";
        bHomology1->writeTextShort(out);
        out<<" ";
    }
    if (bHomology2) {
        out<<"H_2(BM) = ";
        bHomology2->writeTextShort(out);
        out<<" ";
    }

    if (bmMap0) {
        out<<"H_0(BM) --> H_0(M) = ";
        bmMap0->writeTextShort(out);
        out<<" ";
    }
    if (bmMap1) {
        out<<"H_1(BM) --> H_1(M) = ";
        bmMap1->writeTextShort(out);
        out<<" ";
    }
    if (bmMap2) {
        out<<"H_2(BM) --> H_2(M) = ";
        bmMap2->writeTextShort(out);
        out<<" ";
    }

    if (dmTomMap1) {
        out<<"PD map = ";
        dmTomMap1->writeTextShort(out);
        out<<" ";
    }
    if (torsionFormComputed) {
        out<<"Torsion subgroup rank vector: "<<torsionRankString<<" ";
        out<<"Torsion sigma vector: "<<torsionSigmaString<<" ";
        out<<"Torsion Legendre symbol vector: "<<torsionLegendreString<<" ";
        out<<"Embedability comment: "<<embeddabilityString<<" ";
    }

}

void NHomologicalData::computeccIndexing() {
    // Only do this if we haven't already done it.
    if (ccIndexingComputed)
        return;

    // Off we go...

    unsigned long i=0;
    unsigned long j=0;

    for (NTriangulation::VertexIterator vit = tri->getVertices().begin();
            vit != tri->getVertices().end(); vit++) {
        if (!((*vit)->isIdeal())) sNIV.push_back(i);
        i++;
    } // sNIV

    for (NTriangulation::EdgeIterator eit = tri->getEdges().begin();
            eit != tri->getEdges().end(); eit++) {
        for (i=0;i<2;i++) {
            if ((*eit)->getVertex(i)->isIdeal()) sIEOE.push_back(2*j+i);
        }
        j++;
    }
    j=0; // sIEOE

    for (NTriangulation::FaceIterator fit = tri->getFaces().begin();
            fit != tri->getFaces().end(); fit++) {
        for (i=0;i<3;i++) {
            if ((*fit)->getVertex(i)->isIdeal()) sIEEOF.push_back(3*j+i);
        }
        j++;
    }
    j=0; // sIEEOF

    for (NTriangulation::TetrahedronIterator tit =
            tri->getTetrahedra().begin();
            tit != tri->getTetrahedra().end(); tit++) {
        for (i=0;i<4;i++)  {
            if ((*tit)->getVertex(i)->isIdeal()) {
                sIEFOT.push_back(4*j+i);
            }
        }
        j++;
    }
    j=0;// sIEFOT

    for (NTriangulation::VertexIterator vit = tri->getVertices().begin();
            vit != tri->getVertices().end(); vit++) // dNINBV
    {if ((!((*vit)->isIdeal())) &&
            (!((*vit)->isBoundary()))) dNINBV.push_back(j);
        j++;
    } j=0;
    for (NTriangulation::EdgeIterator eit = tri->getEdges().begin();
            eit != tri->getEdges().end(); eit++) {
        if (!((*eit)->isBoundary()))
            dNBE.push_back(j);
        j++;
    }
    j=0; // dNBE
    for (NTriangulation::FaceIterator fit = tri->getFaces().begin();
            fit != tri->getFaces().end(); fit++) {
        if (!((*fit)->isBoundary()))        dNBF.push_back(j);
        j++;
    }
    i=0; // dNBF

    for (NTriangulation::VertexIterator vit = tri->getVertices().begin();
            vit != tri->getVertices().end(); vit++) // sBNIV
    {if ( (!((*vit)->isIdeal())) &&
            ((*vit)->isBoundary())) sBNIV.push_back(i);
        i++;
    } i=0;
    for (NTriangulation::EdgeIterator eit = tri->getEdges().begin();
            eit != tri->getEdges().end(); eit++) // sBNIE
    {if ((*eit)->isBoundary()) sBNIE.push_back(i);
        i++;
    } i=0;
    for (NTriangulation::FaceIterator fit = tri->getFaces().begin();
            fit != tri->getFaces().end(); fit++) // sBNIF
    {if ((*fit)->isBoundary()) sBNIF.push_back(i);
        i++;
    }

    ccIndexingComputed = true;

    // standard (0..3)-cells:
    numStandardCells[0] = sNIV.size() + sIEOE.size();
    numStandardCells[1] = tri->getNumberOfEdges() + sIEEOF.size();
    numStandardCells[2] = tri->getNumberOfFaces() + sIEFOT.size();
    numStandardCells[3] = tri->getNumberOfTetrahedra();

    // dual (0..3)-cells:
    numDualCells[0] = tri->getNumberOfTetrahedra();
    numDualCells[1] = dNBF.size();
    numDualCells[2] = dNBE.size();
    numDualCells[3] = dNINBV.size();

    // boundary (0..2)-cells:
    numBdryCells[0] = sBNIV.size() + sIEOE.size();
    numBdryCells[1] = sBNIE.size() + sIEEOF.size();
    numBdryCells[2] = sBNIF.size() + sIEFOT.size();
}

void NHomologicalData::computeChainComplexes() {
    // Only do this if we haven't already done it.
    if (chainComplexesComputed)
        return;

    // Off we go...

    if (!ccIndexingComputed) computeccIndexing();

    chainComplexesComputed = true;

    // need to convert this so that it does not use tri
    B0 = new NMatrixInt(1, numDualCells[0]);
    B1 = new NMatrixInt(numDualCells[0], numDualCells[1]);
    B2 = new NMatrixInt(numDualCells[1], numDualCells[2]);
    B3 = new NMatrixInt(numDualCells[2], numDualCells[3]);
    B4 = new NMatrixInt(numDualCells[3], 1);

    A0 = new NMatrixInt(1, numStandardCells[0]);
    A1 = new NMatrixInt(numStandardCells[0], numStandardCells[1]);
    A2 = new NMatrixInt(numStandardCells[1], numStandardCells[2]);
    A3 = new NMatrixInt(numStandardCells[2], numStandardCells[3]);
    A4 = new NMatrixInt(numStandardCells[3], 1);

    H1map = new NMatrixInt(numStandardCells[1], numDualCells[1]);

    Bd0 = new NMatrixInt(1, numBdryCells[0]);
    Bd1 = new NMatrixInt(numBdryCells[0], numBdryCells[1]);
    Bd2 = new NMatrixInt(numBdryCells[1], numBdryCells[2]);
    Bd3 = new NMatrixInt(numBdryCells[2], 1);

    B0Incl = new NMatrixInt(numStandardCells[0], numBdryCells[0]);
    B1Incl = new NMatrixInt(numStandardCells[1], numBdryCells[1]);
    B2Incl = new NMatrixInt(numStandardCells[2], numBdryCells[2]);

    long int temp;
    unsigned long i,j;

    NPerm p1,p2;

    // This fills out matrix A1
    for (i=0;i<tri->getNumberOfEdges();i++) {
        // these are the standard edges
        temp=sNIV.index(tri->getVertexIndex(tri->getEdge(i)->getVertex(0)));
        (A1->entry( ((temp==(-1)) ?
            (sNIV.size()+sIEOE.index(2*i)) : temp ), i))-=1;
        temp=sNIV.index(tri->getVertexIndex(tri->getEdge(i)->getVertex(1)));
        (A1->entry( ((temp==(-1)) ?
            (sNIV.size()+sIEOE.index(2*i+1)) : temp), i))+=1;
    } // ok

    for (i=0;i<sIEEOF.size();i++) { // these are the ideal edges...
        // sIEEOF[i] /3 is the face index, and sIEEOF[i] % 3 tells us
        // the vertex of this face
        p1=tri->getFace(sIEEOF[i]/3)->getEdgeMapping( (sIEEOF[i] + 1) % 3);
        if (p1.sign()==1) {
            A1->entry(sNIV.size() + sIEOE.index(2*(
                tri->getEdgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )+1), tri->getNumberOfEdges()+i)-=1;
        } else {
            A1->entry(sNIV.size() + sIEOE.index(2*(
                tri->getEdgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )) , tri->getNumberOfEdges()+i)-=1;
        }
        p1=tri->getFace(sIEEOF[i]/3)->getEdgeMapping( (sIEEOF[i] + 2) % 3);
        if (p1.sign()==1) {
            A1->entry(sNIV.size() + sIEOE.index(2*(
                tri->getEdgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )) , tri->getNumberOfEdges()+i)+=1;
        } else {
            A1->entry(sNIV.size() + sIEOE.index(2*(
                tri->getEdgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )+1) , tri->getNumberOfEdges()+i)+=1;
        }
    }
    // that handles matrix A1.

    // start filling out A2...
    for (i=0;i<tri->getNumberOfFaces();i++) {
        // put boundary edges into A2..
        for (j=0;j<6;j++) {
            // run through the 6 possible boundary edges of the face
            // the first 3 are standard, the last three are the ideal
            // edges (if they exist)
            if ( (j/3) == 0) {
                p1=tri->getFace(i)->getEdgeMapping(j % 3);
                A2->entry( tri->getEdgeIndex(
                    tri->getFace(i)->getEdge(j % 3)) ,i) +=
                    ( (p1.sign()==1) ? +1 : -1 );
            } else {
                // check face i vertex j % 3 is ideal
                if (tri->getFace(i)->getVertex(j % 3)->isIdeal())
                    A2->entry( tri->getNumberOfEdges() +
                        sIEEOF.index((3*i) + (j % 3)), i) += 1;
            }
        }
    }

    for (i=0;i<sIEFOT.size();i++) {
        // boundary edges from ideal faces of tetrahedra.
        // sIEFOT[i] /4 is the tetrahedron number
        // sIEFOT[i] % 4 is the vertex number for this tetrahedron
        // tetrahedra[ sIEFOT[i]/4 ].getFace(sIEFOT[i] + 1,2,3 % 4)
        // are the respective faces
        // tetrahedra[ sIEFOT[i]/4 ].getFaceMapping(sIEFOT[i] + 1,2,3 % 4)
        // gives the perm
        // faces().index( tetrahedra[sIEFOT[i]/4].getFace(
        // sIEFOT[i] + 1,2,3 % 4) is therefore the face number, and
        // tetrahedra[ sIEFOT[i]/4 ].getFaceMapping(
        // sIEFOT[i] + 1,2,3 % 4)^{-1} applied to sIEFOT[i] % 4 is the
        // vertex of this face.
        for (j=1;j<4;j++) {
            p1=tri->getTetrahedron( sIEFOT[i]/4 )->getFaceMapping(
                (sIEFOT[i] + j) % 4);
            A2->entry( tri->getNumberOfEdges() + sIEEOF.index(
                3*tri->getFaceIndex(tri->getTetrahedron(
                sIEFOT[i]/4 )->getFace( (sIEFOT[i] + j) % 4)) +
                p1.preImageOf(sIEFOT[i] % 4) ) ,
                tri->getNumberOfFaces()+i ) += ( (p1.sign()==1 ? -1 : 1 ) );
        }
    }
    // end A2

    // start A3
    for (i=0;i<tri->getNumberOfTetrahedra();i++) {
        for (j=0;j<4;j++) {
            // first go through standard faces 0 through 3
            p1=tri->getTetrahedron(i)->getFaceMapping(j);
            A3->entry( tri->getFaceIndex(
                tri->getTetrahedron(i)->getFace(j) ), i) +=
                ( (p1.sign()==1) ? 1 : -1 );
            // then ideal faces 0 through 3, if they exist
            if (tri->getTetrahedron(i)->getVertex(j)->isIdeal()==1) {
                // this part is in error.
                A3->entry( tri->getNumberOfFaces() +
                    sIEFOT.index((4*i) + j), i) += 1;
            }
        }
    }
    // end A3


    // start B1: for each dual edge == non-boundary face,
    //              find the tetrahedra that bound it
    for (i=0;i<dNBF.size();i++) {
        B1->entry( tri->getTetrahedronIndex(
            tri->getFace(dNBF[i])->getEmbedding(1).getTetrahedron() ),i)+=1;
        B1->entry( tri->getTetrahedronIndex(
            tri->getFace(dNBF[i])->getEmbedding(0).getTetrahedron() ),i)-=1;
    }
    // end B1

    // start B2: for each dual face == non-boundary edge,
    // find dual edges it bounds == link of tetrahedra that contain it
    for (i=0;i<dNBE.size();i++) {
        const std::deque<NEdgeEmbedding>& edgeque(
            tri->getEdge(dNBE[i])->getEmbeddings());
        for (j=0;j<edgeque.size();j++) {
            p1=edgeque[j].getVertices();
            // the face of the tetrahedron corresponding to vertex 2 is
            // what we want to orient... but we need to decide on its
            // orientation.  For that we check to see if this face's
            // getEmbedding(0).getTetrahedron() is the current tet, and
            // getEmbedding(0).getFace() is this current face p1[2]...

            B2->entry( dNBF.index( tri->getFaceIndex(
                edgeque[j].getTetrahedron()->getFace(p1[2]) ) ) ,i)+=
                    ( ( edgeque[j].getTetrahedron() ==
                        edgeque[j].getTetrahedron()->getFace(
                            p1[2] )->getEmbedding( 0 ).getTetrahedron() &&
                        edgeque[j].getTetrahedron()->getFace(
                            p1[2] )->getEmbedding( 0 ).getFace() == p1[2] )
                              ? 1 : -1);
        }
    }
    // end B2

    NIndexedArray<long int> unorientedlist; // this should be the list of
                                            // unoriented tetrahedra
    // together with marked vertices, stored as
    // 4*tetindex + vertex no.
    NIndexedArray<long int> orig_uol;
    NIndexedArray<long int> edge_adjacency;

    std::vector<int> tetor;
    long int ind1;
    long int ind2;
    int k;

    NEdgeEmbedding tempe;

    // start B3: for each dual tetrahedron==nonboundary vertex,
    //           find the corresp edges==non-boundary boundary faces

    for (i=0;i<dNINBV.size();i++) {
        // dNINBV[i] is the vertices.index() of this vertex.
        const std::vector<NVertexEmbedding>& vtetlist(
            tri->getVertex(dNINBV[i])->getEmbeddings());
        tetor.resize(vtetlist.size(),0);
        unorientedlist.resize(0);

        for (j=0;j<vtetlist.size();j++) { // here is the first problem.
            unorientedlist.push_back(
                4*tri->getTetrahedronIndex( vtetlist[j].getTetrahedron() ) +
                vtetlist[j].getVertex() );
        }
        orig_uol=unorientedlist;

        // need to set up a local orientation for the tangent
        // bundle at the vertex so that we can compare with the
        // normal orientations of the edges incident. This normal
        // orientation will have the form of a sign +-1 for each
        // NVertexEmbedding in the list vtetlist. Our orientation convention
        // will be chosen so that vtetlist[0] is positively oriented,
        // ie: tetor[0]==1 always.

        tetor[0]=1;
        unorientedlist.erase( 4*tri->getTetrahedronIndex(
            vtetlist[0].getTetrahedron()) + vtetlist[0].getVertex() );

        while (!unorientedlist.empty())
          for (j=0;j<vtetlist.size();j++)
            // go through all oriented tetrahedra and orient
            // the adjacent tetrahedra
            {
                ind1=orig_uol[j];

                if ( unorientedlist.index( ind1 ) == (-1) ) {
                    // this tetrahedron has been oriented check to see
                    // if any of the adjacent
                    // tetrahedra are unoriented, and if so, orient them.
                    for (k=0;k<4;k++) {
                        if (k== (ind1 % 4)) k++;
                        p1=vtetlist[j].getTetrahedron() ->
                            getAdjacentTetrahedronGluing(k);
                        ind2=4*tri->getTetrahedronIndex(
                            vtetlist[j].getTetrahedron() ->
                                getAdjacentTetrahedron(k) ) + p1[ind1 % 4];

                        if (unorientedlist.index( ind2 )  != (-1) ) {
                            // we have an adjacent unoriented tetrahedron.
                            // we orient it and erase from unorientedlist.
                            tetor[ orig_uol.index(ind2) ] =
                                (-1)*tetor[j]*p1.sign();
                            unorientedlist.erase( ind2 );
                        }
                    }
                }
            }

        // now a local orientation is set up and can compute the boundary.
        // to do this, it seems best to compile a list of incident edges
        // which contains their endpoint data and sign.
        // the list will be an NIndexedArray<long int> edge_adjacency,
        // data will be stored as
        // 4*(edge index) + 2*(endpt index) + sign stored as 0 or 1.
        edge_adjacency.resize(0);

        for (j=0;j<vtetlist.size();j++)
            for (k=0;k<6;k++) {
                ind2=vtetlist[j].getTetrahedron()->getEdgeMapping(k).
                    preImageOf( vtetlist[j].getVertex() );
                if ( ind2<2 ) {
                    // edge k of tetrahedron j, moreover we know that
                    // the vertex of the edge corresponds to ind2
                    tempe=NEdgeEmbedding( vtetlist[j].getTetrahedron(), k );
                    // the corresp orientation coming from our local
                    // orientation
                    // plus orienting the edge out of vertex k % 2...

                    p1=tempe.getVertices();
                    if ( ind2 == 1 ) p1=p1*(NPerm(0,1));
                    // now p1 sends 0 to point corresp to v, 1 to point
                    // corresp to end of edge.
                    // if p1.sign() == tetor[j] then sign = +1 otherwise -1.

                    ind1=4*tri->getEdgeIndex(
                        vtetlist[j].getTetrahedron()->getEdge(k) )
                        + 2*ind2 + (p1.sign() == tetor[j] ? 1 : 0);

                    if (edge_adjacency.index(ind1) == (-1) )
                        edge_adjacency.push_back(ind1);
                }
            }

        for (j=0;j<edge_adjacency.size();j++) {
            B3->entry( dNBE.index(edge_adjacency[j]/4) , i) +=
                ( ( (edge_adjacency[j] % 2)==0 ) ? 1 : -1 );
        }
    }
    // end B3

    // proceed to fill out H1map
    // the algorithm will proceed in 2 steps.

    // step 1) fix once and for all a map from dual 0-cells to regular
    // 0-cells, the only condition this map needs to satisfy is that the
    // regular 0-cell associated to a dual 0-cell must be contained in
    // the same ideal simplex.

    std::vector<unsigned long> zeroCellMap(tri->getNumberOfTetrahedra());
    // zeroCellMap[i] describes the vertex of tetrahedra[i] that the dual
    // 0-cell is sent to. It will be stored as
    // 4*(vertex number 0,1,2,3) + 0,1,2,3 (equal to prev. number if
    // non-ideal

    for (i=0; i<zeroCellMap.size(); i++) {
        // cycle through the vertices, take the first non-ideal one if
        // it exists.
        j=0;
        while ( j<4 && tri->getTetrahedron(i)->getVertex(j)->isIdeal())
            j++;
        if (j<4) zeroCellMap[i]=4*j+j;
        else zeroCellMap[i]=1;
    }

    // step 2) fill out the matrix. each dual 1-cell corresponds to a
    // face of the ideal triangulation. the map of 0-cells has already
    // been chosen so for the map of 1-cells simply choose any path
    // from the first 0-cell to the 2nd 0-cell with the condition that
    // the path stays inside the two ideal simplicies and only crosses
    // the face corresponding to the dual 1-cell once. (and no other faces).

    for (j=0; j<H1map->columns(); j++) // H1map.columns()==dNBF.size() 
        // while H1map.rows() is edges.size()+sIEEOF.size()
    {
        // now we have to decide where dual edge j == ideal triangulation
        // face j is sent.

        unsigned tet0FaceIndex = tri->getFace(dNBF[j])->
            getEmbedding(0).getFace(); 
        unsigned tet1FaceIndex = tri->getFace(dNBF[j])->
            getEmbedding(1).getFace(); 

        unsigned vert0Num = zeroCellMap[tri->getTetrahedronIndex(
            tri->getFace(dNBF[j]) -> getEmbedding(0).getTetrahedron() )]/4;
            // vertex number of start vertex in tet0
        unsigned vert1Num = zeroCellMap[tri->getTetrahedronIndex(
            tri->getFace(dNBF[j]) -> getEmbedding(1).getTetrahedron() )]/4;
            // vertex number of end vertex in tet1.
        unsigned vert0id = zeroCellMap[tri->getTetrahedronIndex(
            tri->getFace(dNBF[j]) -> getEmbedding(0).getTetrahedron() )]%4;
            // not equal to vert0Num if and only if vert0 is ideal.
        unsigned vert1id = zeroCellMap[tri->getTetrahedronIndex(
            tri->getFace(dNBF[j]) -> getEmbedding(1).getTetrahedron() )]%4;
            // not equal to vert1Num if and only if vert1 is ideal.
        NPerm P1 = tri->getFace(dNBF[j])->getEmbedding(0).getVertices();
        NPerm P2 = tri->getFace(dNBF[j])->getEmbedding(1).getVertices();
        NPerm P3;
        NPerm P0to1 = P2 * ( P1.inverse() ); 
        // the permutation from the start simplex vertices
        // to the end simplex.

        bool stage0nec = false;
        unsigned long stage0edgeNum;
        bool stage0posOr;
        unsigned stage0choice; // this indicates the vertex of the simplex
        // that our chosen edge
        // with its induced orientation ends...

        if (vert0Num == tet0FaceIndex)  // stage 0
        {
            stage0nec = true;

            if (vert0Num == vert0id) {
                stage0choice = (tet0FaceIndex + 1) % 4;
            } // not ideal
            else {
                stage0choice = vert0id;
            } // ideal

            stage0edgeNum = tri->getEdgeIndex(tri->getFace(dNBF[j]) ->
                getEmbedding(0).getTetrahedron() ->
                getEdge( edgeNumber[vert0Num][stage0choice] ));
            stage0posOr = ( tri->getFace(dNBF[j]) ->
                getEmbedding(0).getTetrahedron()->getEdgeMapping(
                edgeNumber[vert0Num][stage0choice])[1] == stage0choice ) ?
                true : false ;
        }


        bool stage4nec = false; // stage 4
        unsigned long stage4edgeNum;
        bool stage4posOr;
        unsigned stage4choice;

        if (vert1Num == tet1FaceIndex) {
            stage4nec = true;

            if (vert1Num == vert1id) // the non-ideal case.
            {
                stage4choice = (tet1FaceIndex + 1) % 4;
            } // duh, this is all wrong.
            else {
                stage4choice = vert1id;
            }

            stage4edgeNum = tri->getEdgeIndex(tri->getFace(dNBF[j]) ->
                getEmbedding(1).getTetrahedron() ->
                getEdge( edgeNumber[vert1Num][stage4choice] ));
            stage4posOr = ( tri->getFace(dNBF[j]) ->
                getEmbedding(1).getTetrahedron()->getEdgeMapping(
                edgeNumber[vert1Num][stage4choice])[1] == vert1Num ) ?
                true : false ;
        }

        // decide if stages 1 and 3 are neccessary...
        bool stage1nec = false;         // stage 1
        unsigned stage1v, stage1vi;
        unsigned long stage1edgeNum;
        bool stage1posOr;
        unsigned stage1FaceToUse;

        if (stage0nec && tri->getFace(dNBF[j]) ->
                getEmbedding(0).getTetrahedron() ->
                getVertex(stage0choice)->isIdeal() ) {
            stage1v = stage0choice;
            stage1vi = vert0Num;
            stage1nec=true;
        } else
            if ((!stage0nec) && (vert0Num != vert0id) &&
                    (vert0id == tet0FaceIndex)) {
                stage1v = vert0Num;
                stage1vi = vert0id;
                stage1nec = true;
            }
        if (stage1nec) { // we need to decide which face to use...
            stage1FaceToUse = tri->getFace(dNBF[j]) ->
                getEmbedding(0).getTetrahedron()->getEdgeMapping(
                edgeNumber[stage1v][tet0FaceIndex] )[2];
            P3 = tri->getFace(dNBF[j])->getEmbedding(0).getTetrahedron()->
                getFaceMapping(stage1FaceToUse);
            stage1edgeNum = tri->getNumberOfEdges() + sIEEOF.index(
                3*(tri->getFaceIndex(tri->getFace(dNBF[j]) ->
                getEmbedding(0).getTetrahedron() ->
                getFace(stage1FaceToUse))) + P3.preImageOf(stage1v) );
            stage1posOr = ( ( P3[(P3.preImageOf(stage1v)+1) % 3] !=
                stage1vi ) ? true : false );
        }
        bool stage3nec = false;
        unsigned stage3v, stage3vi;
        unsigned long stage3edgeNum;
        bool stage3posOr;
        unsigned stage3FaceToUse;

        if (stage4nec && tri->getFace(dNBF[j]) ->
                getEmbedding(1).getTetrahedron() ->
                getVertex(stage4choice)->isIdeal() ) { // ideal case
            stage3v = stage4choice;
            stage3vi = vert1Num;
            stage3nec=true;
        } else
            if ((!stage4nec) && (vert1Num != vert1id) &&
                    (vert1id == tet1FaceIndex)) { // non-ideal case
                stage3v = vert1Num;
                stage3vi = vert1id;
                stage3nec = true;
            }
        if (stage3nec) { // we need to decide which face to use...
            stage3FaceToUse = tri->getFace(dNBF[j]) ->
                getEmbedding(1).getTetrahedron()->getEdgeMapping(
                edgeNumber[stage3v][tet1FaceIndex] )[2];
            P3 = tri->getFace(dNBF[j])->getEmbedding(1).getTetrahedron()->
                getFaceMapping(stage3FaceToUse);
            stage3edgeNum = tri->getNumberOfEdges() + sIEEOF.index(
                3*(tri->getFaceIndex(tri->getFace(dNBF[j]) ->
                getEmbedding(1).getTetrahedron() ->
                getFace(stage3FaceToUse))) + P3.preImageOf(stage3v) );
            stage3posOr = ( ( P3[(P3.preImageOf(stage3v)+1) % 3] ==
                stage3vi ) ? true : false );
        }

        unsigned stage2startdata, stage2enddata;
        // 3*vertex number(0,1,2) + another vertex number (0,1,2)
        // these are the same indicates the vertex is non-ideal
        // these are different indicates the vertex is ideal and dir
        // of relevant point..

        if (stage1nec) // set up stage2startdata
        {
            stage2startdata = 3*P1.preImageOf( stage1v ) +
                P1.preImageOf((tri->getFace(dNBF[j]) ->
                getEmbedding(0).getTetrahedron() ->
                getEdgeMapping( edgeNumber[stage1v][stage1vi] ))[3] );
        } else {
            // we have to deal with 2 possibilities a) stage 0 was called
            // and it jumped here, so it is not an ideal vertex.
            // b) neither stage 0 or 1 was called and this may or may
            // not be an ideal vertex
            if (stage0nec) { // this is the non-ideal situation
                stage2startdata = 3*P1.preImageOf( stage0choice ) +
                        ((P1.preImageOf( stage0choice )+1) % 3);
            } else {
                // this is the starting point... back to using vert0 info...
                if (vert0Num != vert0id)
                    stage2startdata = 3*P1.preImageOf( vert0Num ) +
                        P1.preImageOf( vert0id );
                else
                    stage2startdata = 3*P1.preImageOf( vert0Num ) +
                        ((P1.preImageOf( vert0Num ) + 1) % 3);
            }
        }

        if (stage3nec) // set up stage2enddata
        {
            stage2enddata = 3*P2.preImageOf( stage3v ) +
                P2.preImageOf((tri->getFace(dNBF[j]) ->
                getEmbedding(1).getTetrahedron() ->
                getEdgeMapping( edgeNumber[stage3v][stage3vi] ))[3] );
        } else {
            if (stage4nec) { // this is the non-ideal situation
                stage2enddata = 3*P2.preImageOf( stage4choice ) +
                        ((P2.preImageOf( stage4choice ) + 1) % 3);
            } else {
                // this is the starting point... back to using vert1 info...
                if (vert1Num != vert1id)
                    stage2enddata = 3*P2.preImageOf( vert1Num ) +
                        P2.preImageOf( vert1id );
                else
                    stage2enddata = 3*P2.preImageOf( vert1Num ) +
                        ((P2.preImageOf( vert1Num ) + 1) % 3);
            }
        }
        // now cycle through pairs of adjacent vertices on the face
        // and check to see if the corresponding edge is required...

        unsigned currV = stage2startdata;
        unsigned prevV = stage2startdata;
        if (stage2startdata != stage2enddata)
            while (currV != stage2enddata) {
                // first, increment currV -- this is a number from the
                // set { 1, 2, 3, 5, 6, 7 } describing an ideal vertex
                // of the face is triadic 3*vert num + direction...
                switch (currV) {
                case 1:
                    currV = 3;
                    break;
                case 2:
                    currV = 1;
                    break;
                case 3:
                    currV=5;
                    break;
                case 5:
                    currV = 7;
                    break;
                case 6:
                    currV = 2;
                    break;
                case 7:
                    currV=6;
                    break;
                }
                // main alg here.
                if (( currV/3  == prevV/3 ) && (tri->getFace(dNBF[j])->
                        getVertex(currV/3)->isIdeal()) )  // ideal edge
                {
                    H1map->entry( tri->getNumberOfEdges() +
                        sIEEOF.index(3*dNBF[j] + (currV/3)) , j ) += 1;
                }
                if ( currV/3  != prevV/3 ) // regular edge
                {
                    H1map->entry( tri->getEdgeIndex(tri->getFace(dNBF[j])->
                        getEdge( ((currV/3) + 1) % 3 )), j ) +=
                        ( (tri->getFace(dNBF[j]) -> getEdgeMapping(
                        ((currV/3) + 1) % 3)[1] == currV/3) ? +1 : -1);
                }
                // move prevV to be equal to currV.
                prevV = currV;
            }
        // now we fill out the matrix.
        if (stage0nec)
            H1map->entry( stage0edgeNum, j ) += ( stage0posOr ? 1 : -1 );
        if (stage1nec)
            H1map->entry( stage1edgeNum, j ) += ( stage1posOr ? 1 : -1 );

        if (stage3nec)
            H1map->entry( stage3edgeNum, j ) += ( stage3posOr ? 1 : -1 );
        if (stage4nec)
            H1map->entry( stage4edgeNum, j ) += ( stage4posOr ? 1 : -1 );

    }

    // This fills out matrix Bd1: rows==sBNIV.size()+sIEOE.size(),
    // cols==sBNIE.size()+sIEEOF.size()
    for (i=0;i<sBNIE.size();i++) { // these are the standard boundary edges
        // temp == -1 when the boundary edge end is ideal.
        temp=sBNIV.index(tri->getVertexIndex(tri->getEdge(sBNIE[i])->
            getVertex(0)));
        (Bd1->entry( ((temp==(-1)) ? (sBNIV.size()+2*i) : temp ), i))-=1;
        temp=sBNIV.index(tri->getVertexIndex(tri->getEdge(sBNIE[i])->
            getVertex(1)));
        (Bd1->entry( ((temp==(-1)) ? (sBNIV.size()+2*i+1) : temp), i))+=1;
    } // ok

    for (i=0;i<sIEEOF.size();i++) { // these are the ideal edges...
        // sIEEOF[i] /3 is the face index, and sIEEOF[i] % 3 tells us
        // the vertex of this face
        p1=tri->getFace(sIEEOF[i]/3)->getEdgeMapping( (sIEEOF[i] + 1) % 3);
        if (p1.sign()==1) {
            Bd1->entry(sBNIV.size() + sIEOE.index(2*(
                tri->getEdgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )+1), sBNIE.size()+i)-=1;
        } else {
            Bd1->entry(sBNIV.size() + sIEOE.index(2*(
                tri->getEdgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )) , sBNIE.size()+i)-=1;
        }
        p1=tri->getFace(sIEEOF[i]/3)->getEdgeMapping( (sIEEOF[i] + 2) % 3);
        if (p1.sign()==1) {
            Bd1->entry(sBNIV.size() + sIEOE.index(2*(
                tri->getEdgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )) , sBNIE.size()+i)+=1;
        } else {
            Bd1->entry(sBNIV.size() + sIEOE.index(2*(
                tri->getEdgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )+1) , sBNIE.size()+i)+=1;
        }
    }
    // that handles matrix Bd1.

    // start filling out Bd2: rows==sBNIE.size()+sIEEOF.size(),
    // cols==sBNIF.size()+sIEFOT.size()
    for (i=0;i<sBNIF.size();i++) // boundary non-ideal faces...
    { // put boundary edges into Bd2..
        for (j=0;j<6;j++) {
            // run through the 6 possible boundary edges of the face
            // the first 3 are standard, the last three are the ideal
            // edges (if they exist)
            if ( (j/3) == 0) {
                p1=tri->getFace(sBNIF[i])->getEdgeMapping(j % 3);
                Bd2->entry( sBNIE.index( tri->getEdgeIndex(tri->getFace(
                    sBNIF[i])->getEdge(j % 3)) ) ,i) +=
                    ( (p1.sign()==1) ? +1 : -1 );
            } else {
                // check face i vertex j % 3 is ideal
                if (tri->getFace(sBNIF[i])->getVertex(j % 3)->isIdeal())
                    Bd2->entry( sBNIF.size() + sIEEOF.index(
                        (3*i) + (j % 3)), i) += 1;
            }
        }
    }

    for (i=0;i<sIEFOT.size();i++) // ideal faces...
    { // boundary edges from ideal faces of tetrahedra.
        // sIEFOT[i] /4 is the tetrahedron number
        // sIEFOT[i] % 4 is the vertex number for this tetrahedron
        // tetrahedra[ sIEFOT[i]/4 ].getFace(sIEFOT[i] + 1,2,3 % 4) are
        // the respective faces
        // tetrahedra[ sIEFOT[i]/4 ].getFaceMapping(sIEFOT[i] + 1,2,3 % 4)
        // gives the perm
        // faces().index( tetrahedra[sIEFOT[i]/4].getFace(
        //     sIEFOT[i] + 1,2,3 % 4) is therefore the
        // face number, and tetrahedra[ sIEFOT[i]/4 ].getFaceMapping(
        //     sIEFOT[i] + 1,2,3 % 4)^{-1}
        // applied to sIEFOT[i] % 4 is the vertex of this face.
        for (j=1;j<4;j++) {
            p1=tri->getTetrahedron( sIEFOT[i]/4 )->getFaceMapping(
                (sIEFOT[i] + j) % 4);
            Bd2->entry( sBNIE.size() + sIEEOF.index(3*tri->getFaceIndex(
                tri->getTetrahedron(sIEFOT[i]/4 )->getFace(
                (sIEFOT[i] + j) % 4)) + p1.preImageOf(sIEFOT[i] % 4) ) ,
                sBNIF.size()+i ) += ( (p1.sign()==1 ? -1 : 1 ) );
        }
    }
    // end Bd2

    // fill out b0Incl
    // boundary 0-cells:
    for (i=0;i<B0Incl->columns();i++)
        B0Incl->entry( ( ( i < sBNIV.size()) ? sNIV.index(sBNIV[i]) :
                sNIV.size() + i - sBNIV.size() ) ,i)+=1;
    // fill out b1Incl
    for (i=0;i<B1Incl->columns();i++)
        // each boundary edge corresponds to a triangulation edge
        B1Incl->entry( ( ( i < sBNIE.size() ) ? sBNIE[i] :
                tri->getNumberOfEdges() + i - sBNIE.size() ) ,i)+=1;
    // fill out b2Incl
    for (i=0;i<B2Incl->columns();i++)
        B2Incl->entry( ( ( i < sBNIF.size() ) ? sBNIF[i] :
                tri->getNumberOfFaces() + i - sBNIF.size() ) ,i)+=1;
}

const NMarkedAbelianGroup& NHomologicalData::getMH(unsigned q) {
    if (q==0) {
        if (!mHomology0) {
            computeChainComplexes();
            mHomology0 = new NMarkedAbelianGroup(*A0,*A1);
        }
        return *mHomology0;
    } else if (q==1) {
        if (!mHomology1) {
            computeChainComplexes();
            mHomology1 = new NMarkedAbelianGroup(*A1,*A2);
        }
        return *mHomology1;
    } else if (q==2) {
        if (!mHomology2) {
            computeChainComplexes();
            mHomology2 = new NMarkedAbelianGroup(*A2,*A3);
        }
        return *mHomology2;
    } else {
        // Assume q == 3.  This will at least avoid a crash if q lies
        // outside the required range.
        if (!mHomology3) {
            computeChainComplexes();
            mHomology3 = new NMarkedAbelianGroup(*A3,*A4);
        }
        return *mHomology3;
    }
    // the A's should probably be redone as an array of pointers...
}

const NMarkedAbelianGroup& NHomologicalData::getBMH(unsigned q) {
    if (q==0) {
        if (!bHomology0) {
            computeChainComplexes();
            bHomology0 = new NMarkedAbelianGroup(*Bd0,*Bd1);
        }
        return *bHomology0;
    } else if (q==1) {
        if (!bHomology1) {
            computeChainComplexes();
            bHomology1 = new NMarkedAbelianGroup(*Bd1,*Bd2);
        }
        return *bHomology1;
    } else {
        // Assume q == 2.  This will at least avoid a crash if q lies
        // outside the required range.
        if (!bHomology2) {
            computeChainComplexes();
            bHomology2 = new NMarkedAbelianGroup(*Bd2,*Bd3);
        }
        return *bHomology2;
    }
}

const NMarkedAbelianGroup& NHomologicalData::getDMH(unsigned q) {
    if (q==0) {
        if (!dmHomology0) {
            computeChainComplexes();
            dmHomology0 = new NMarkedAbelianGroup(*B0,*B1);
        }
        return *dmHomology0;
    } else if (q==1) {
        if (!dmHomology1) {
            computeChainComplexes();
            dmHomology1 = new NMarkedAbelianGroup(*B1,*B2);
        }
        return *dmHomology1;
    } else if (q==2) {
        if (!dmHomology2) {
            computeChainComplexes();
            dmHomology2 = new NMarkedAbelianGroup(*B2,*B3);
        }
        return *dmHomology2;
    } else {
        // Assume q == 3.  This will at least avoid a crash if q lies
        // outside the required range.
        if (!dmHomology3) {
            computeChainComplexes();
            dmHomology3 = new NMarkedAbelianGroup(*B3,*B4);
        }
        return *dmHomology3;
    }
}

void NHomologicalData::computeHomology() {
    computeChainComplexes();
    if (!mHomology0)
        mHomology0 = new NMarkedAbelianGroup(*A0,*A1);
    if (!mHomology1)
        mHomology1 = new NMarkedAbelianGroup(*A1,*A2);
    if (!mHomology2)
        mHomology2 = new NMarkedAbelianGroup(*A2,*A3);
    if (!mHomology3)
        mHomology3 = new NMarkedAbelianGroup(*A3,*A4);
}

void NHomologicalData::computeBHomology() {
    computeChainComplexes();
    if (!bHomology0)
        bHomology0 = new NMarkedAbelianGroup(*Bd0,*Bd1);
    if (!bHomology1)
        bHomology1 = new NMarkedAbelianGroup(*Bd1,*Bd2);
    if (!bHomology2)
        bHomology2 = new NMarkedAbelianGroup(*Bd2,*Bd3);
}

void NHomologicalData::computeDHomology() {
    computeChainComplexes();
    if (!dmHomology0)
        dmHomology0 = new NMarkedAbelianGroup(*B0,*B1);
    if (!dmHomology1)
        dmHomology1 = new NMarkedAbelianGroup(*B1,*B2);
    if (!dmHomology2)
        dmHomology2 = new NMarkedAbelianGroup(*B2,*B3);
    if (!dmHomology3)
        dmHomology3 = new NMarkedAbelianGroup(*B3,*B4);
}

const NHomMarkedAbelianGroup& NHomologicalData::getH1cellap() {
    if (!dmTomMap1) {
        computeHomology();
        computeDHomology();
        dmTomMap1 = new NHomMarkedAbelianGroup(
            *dmHomology1, *mHomology1, *H1map );
    }
    return (*dmTomMap1);
}

const NHomMarkedAbelianGroup& NHomologicalData::getBMmapH(unsigned q) {
    if (q==0) {
        if (!bmMap0) {
            computeHomology();
            computeBHomology();
            bmMap0 = new NHomMarkedAbelianGroup(
                *bHomology0, *mHomology0, *B0Incl );
        }
        return *bmMap0;
    } else if (q==1) {
        if (!bmMap1) {
            computeHomology();
            computeBHomology();
            bmMap1 = new NHomMarkedAbelianGroup(
                *bHomology1, *mHomology1, *B1Incl );
        }
        return *bmMap1;
    } else {
        // Assume q == 2.  This will at least avoid a crash if q lies
        // outside the required range.
        if (!bmMap2) {
            computeHomology();
            computeBHomology();
            bmMap2 = new NHomMarkedAbelianGroup(
                *bHomology2, *mHomology2, *B2Incl );
        }
        return *bmMap2;
    }
}

void NHomologicalData::computeBIncl() {
    computeHomology();
    computeBHomology();
    if (!bmMap0)
        bmMap0 = new NHomMarkedAbelianGroup(*bHomology0, *mHomology0, *B0Incl);
    if (!bmMap1)
        bmMap1 = new NHomMarkedAbelianGroup(*bHomology1, *mHomology1, *B1Incl);
    if (!bmMap2)
        bmMap2 = new NHomMarkedAbelianGroup(*bHomology2, *mHomology2, *B2Incl);
}


void NHomologicalData::computeTorsionLinkingForm() {
    // Only do this if we haven't done it already.
    if (torsionFormComputed)
        return;

    // dual h1 --> standard h1 isomorphism:
    const NHomMarkedAbelianGroup& h1CellAp(getH1cellap());
    // min number of torsion gens:
    unsigned long niv(dmHomology1->getNumberOfInvariantFactors());
    // for holding prime decompositions.:
    std::vector<std::pair<NLargeInteger, unsigned long> > tFac;

    NLargeInteger tI;


    // step 1: go through H1 of the manifold, take prime power decomposition
    //            of each summand.  building primePowerH1Torsion vector and
    //            pTorsionH1Mat matrix...
    //            also, we need to find the 2-chains bounding2c
    //            boundary(bounding2c[i]) = orderinh1(pvList[i])*pvList[i]

    std::vector< NLargeInteger > tV; // temporary vector for holding dual
                                     // cc vectors.

    std::vector<NLargeInteger> ppList; // prime power list
    std::vector< std::pair<NLargeInteger, unsigned long> >
        pPrList; // proper prime power list.
    std::vector< std::vector<NLargeInteger> > pvList; // list of vectors
    // the above two lists will have the same length. for each i,
    // pvList[i] will be a vector in the dual h1 homology chain complex, and
    // ppList[i] will be its order.

    unsigned long i, j, k, l;

    for (i=0; i<niv; i++) {
        tI = dmHomology1->getInvariantFactor(i);
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
            ppList.push_back( fac1 ); // record the order...
            // now the corresponding vector...
            // this will have to be fac1i * vector corresponding to
            // getInvariantFactor(i).
            tV = dmHomology1->getTorRep(i);

            for (k=0; k<tV.size(); k++) tV[k]=fac1i*fac2*tV[k];

            pvList.push_back(tV);
        }
    }

    // step1a: construct (2 2 4) (3 3 9 27) ... indexing of ppList, pvList, etc.
    // the indexing will be as a list of pairs
    // < prime, vector< pair< power, index> > >
    // Use a list because we are continually inserting items in the middle.
    std::list< std::pair< NLargeInteger, std::vector<
        std::pair<unsigned long, unsigned long> > > > indexing;
    // indexing[i] is the i-th prime in increasing order, the first bit is
    // the prime, the 2nd bit is the vector list of powers, the power is an
    // unsigned long, and its respective index in ppList and pvList is the
    // 2nd bit...
    std::list< std::pair< NLargeInteger, std::vector<
        std::pair<unsigned long, unsigned long> > > >::iterator it1;
    std::list< std::pair< NLargeInteger, std::vector<
        std::pair<unsigned long, unsigned long> > > >::iterator il1;

    std::vector< std::pair<unsigned long, unsigned long> >::iterator it2;
    std::vector< std::pair<unsigned long, unsigned long> >::iterator il2;
    std::pair< NLargeInteger, std::vector<
        std::pair<unsigned long, unsigned long> > > dummyv;

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

    // step 2: construct dual vectors
    //           for every pvList vector, find corresponding standard vector.


    NMatrixInt standardBasis( numStandardCells[1], pvList.size() );
    NMatrixInt dualtostandard(h1CellAp.getDefiningMatrix());

    for (i=0; i<standardBasis.rows(); i++)
        for (j=0; j<standardBasis.columns(); j++)
            for (k=0; k<dualtostandard.columns(); k++)
                standardBasis.entry(i,j) +=
                    dualtostandard.entry(i,k)*pvList[j][k];

    // step 3: construct bounding classes
    //           the j-th column of standardBasis, when multiplied by
    //           ppList[j] bounds, so find a chain with that boundary and
    //           put its info in a matrix.

    NMatrixInt ON(mHomology1->getON());
    NMatrixInt R(ON.columns(),ON.columns());
    NMatrixInt Ri(ON.columns(),ON.columns());
    NMatrixInt C(ON.rows(),ON.rows());
    NMatrixInt Ci(ON.rows(),ON.rows());

    smithNormalForm(ON, R, Ri, C, Ci);
    // boundingMat=R*(divide by ON diag, rescale(C*areboundariesM))
    //                                             ---- stepa -----
    //                  ---------------- stepb ---
    //               ----stepc----
    // first I guess we need to determine rank of ON?
    NMatrixInt areboundariesM( standardBasis );

    for (i=0; i<standardBasis.rows(); i++)
        for (j=0; j<standardBasis.columns(); j++)
            areboundariesM.entry(i,j) *= ppList[j];

    NMatrixInt stepa( areboundariesM.rows(), areboundariesM.columns() );
    for (i=0; i<standardBasis.rows(); i++)
        for (j=0; j<standardBasis.columns(); j++)
            for (k=0; k<C.columns(); k++)
                stepa.entry(i,j) += C.entry(i,k)*areboundariesM.entry(k,j);

    unsigned long rankON=0;
    for (i=0; ((i<ON.rows()) && (i<ON.columns())); i++)
        if (ON.entry(i,i) != NLargeInteger::zero) rankON++;

    NMatrixInt stepb( R.columns(), stepa.columns() );

    for (i=0; i<rankON; i++)
        for (j=0; j<stepb.columns(); j++)
            stepb.entry(i,j) = stepa.entry(i,j).divByExact(ON.entry(i,i));

    NMatrixInt boundingMat( stepb.rows(), stepb.columns() );

    for (i=0; i<stepb.rows(); i++)
        for (j=0; j<stepb.columns(); j++)
            for (k=0; k<R.columns(); k++)
                boundingMat.entry(i,j) += R.entry(i,k)*stepb.entry(k,j);

    // step 4: intersect, construct matrix.

    NMatrixRing<NRational> torsionLinkingFormPresentationMat(
        pvList.size(), pvList.size() );

    NLargeInteger tN,tD,tR;

    for (i=0; i<pvList.size(); i++)
        for (j=0; j<pvList.size(); j++) {
            for (k=0; k<dNBF.size(); k++) {
                // we need to determine the sign of the intersection of
                // boundingMat.entry(k,i) * pvList[j][k]
                // the denominator will be ppList[i]
                //
                // Computing the orientation of the intersection, ingredients:
                //
                // tetrahedra[?]->orientation() is +1 or -1 depending on if the
                //   natural orientation agrees with the manifolds one or not.
                //
                // dual orientation of face points into some tetrahedron given
                //  by face[?]->getEmbedding(0) is an NFaceEmbedding
                //     face[]->getEmbedding(0).getTetrahedron() tet pointer
                //     face[]->getEmbedding(0).getVertices() is an NPerm
                //
                // faces[dNBF[k]] is the face pointer of the dual 1-cell

                // boundingMat is vectors in standard 2-complex so it has
                // the same dimension as the standard 2-cells + ideal 2-cells,
                // standard ones coming first.
                // pvList is vectors in dual 1-cells
                torsionLinkingFormPresentationMat.entry(i,j) +=
                    NRational(
                        boundingMat.entry(dNBF[k],i)*pvList[j][k]*
                        NLargeInteger(
                            tri->getFace(dNBF[k])->getEmbedding(0).
                                getTetrahedron()->orientation()*
                            tri->getFace(dNBF[k])->getEmbedding(0).
                                getVertices().sign() ), ppList[i] );
            }
            tN=torsionLinkingFormPresentationMat.entry(i,j).getNumerator();
            tD=torsionLinkingFormPresentationMat.entry(i,j).getDenominator();
            tN.divisionAlg(tD,tR);
            tN = tR.gcd(tD);
            tR.divByExact(tN);
            tD.divByExact(tN);
            torsionLinkingFormPresentationMat.entry(i,j)=NRational(tR,tD);
        }

    // Compute indexing.size() just once, since for std::list this might be a slow
    // operation.
    unsigned long indexingSize = indexing.size();

    h1PrimePowerDecomp.resize(indexingSize);
    linkingFormPD.resize(indexingSize);
    for (i=0, it1 = indexing.begin(); it1 != indexing.end(); i++, it1++) {
        h1PrimePowerDecomp[i].second.resize(it1->second.size());
        h1PrimePowerDecomp[i].first = it1->first;

        for (j=0; j<it1->second.size(); j++)
            h1PrimePowerDecomp[i].second[j] = it1->second[j].first;

        linkingFormPD[i] = new NMatrixRing<NRational>(it1->second.size(),
                it1->second.size() );
        for (j=0; j<it1->second.size(); j++)
            for (k=0; k<it1->second.size(); k++)
                linkingFormPD[i]->entry(j,k) =
                    torsionLinkingFormPresentationMat.entry(
                        it1->second[j].second,
                        it1->second[k].second
                    );
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

    torRankV.resize(indexingSize);
    // std::vector< std::pair< NLargeInteger,
    //     std::vector< unsigned long > > > torRankV(indexing.size());
    // vector which lists the primes and the number of each power...
    for (i=0, it1 = indexing.begin(); it1 != indexing.end(); i++, it1++) {
        torRankV[i].first = it1->first;
        torRankV[i].second.resize(
            it1->second[it1->second.size()-1].first, 0);
        for (j=0; j<it1->second.size(); j++) { 
        // indexing[i].second[j] is a pair (order, index) where the order k
        // indicates one copy of p^k where p==indexing[i].first.
            torRankV[i].second[it1->second[j].first-1]++;
        }
    }


    // step 2: KK 2-torsion invariant (need to implement)
    //           *what is a smart way to implement the sigma invariant?*
    //           I guess it should be of the form std::vector< int >
    //           since it is only holding the reps 0,1,2,3,4,5,6,7 and inf.
    //           inf we can represent by -1 or something? or we could use
    //           and NLargeInteger instead.
    // decide on if there is 2-torsion...
    NLargeInteger twoPow;
    static const NRational pi( NRational(
                NLargeInteger("314159265358979323846264338327950288"),
                NLargeInteger("100000000000000000000000000000000000") ));
    std::vector< NLargeInteger > groupV;
    bool notatend;
    NRational tSum;

    unsigned long incind;
    bool incrun;
    long double tLD;
    long double xlD, ylD;

    std::vector< NLargeInteger > ProperPrimePower;

    if (h1PrimePowerDecomp.size() > 0)
        if (h1PrimePowerDecomp[0].first == NLargeInteger(2)) { 
            // there is 2-torsion. now we put together the sigma vector
            // twoTorSigmaV
            // first initialize the length of twoTorSigmaV
            twoTorSigmaV.resize(torRankV[0].second.size());

            groupV.resize(h1PrimePowerDecomp[0].second.size(),
                NLargeInteger("0") );

            ProperPrimePower.resize( h1PrimePowerDecomp[0].second.size() );
            for (i=0; i<ProperPrimePower.size(); i++) {
                ProperPrimePower[i] = NLargeInteger(2);
                ProperPrimePower[i].raiseToPower(h1PrimePowerDecomp[0].
                    second[i] );
            }

            for (i=0; i<twoTorSigmaV.size(); i++) {
                // now we construct the sum over the group of
                // e^{ 2^{i+1} pi i form(x,x) } where x is a group element
                // torRankV[0].second[0] through torRankV.second[size()-1]
                // are the number of copies of
                // Z_2 ... through Z_{2^size()}
                // we need a loop that evaluates form(x,x) for all x in
                // this group...
                // the idea will be to have a start vector (0,0,...,0) and then
                // increment it until at the end vector.  For this purpose it
                // makes more sense to use the
                // std::vector< std::pair< NLargeInteger,
                //     std::vector<unsigned long> > > h1PrimePowerDecomp;
                // as it's easier to work with.
                // h1PrimePowerDecomp[0].first == 2
                // so we just need to cycle through
                // h1PrimePowerDecomp[0].second which is an increasing list
                // of the powers of 2, ie: 2^i...

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
                        groupV[incind] = (groupV[incind] + NLargeInteger::one)
                            % ProperPrimePower[incind];
                        if (groupV[incind] == NLargeInteger::zero) {
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
                    twoTorSigmaV[i] = NLargeInteger::infinity;
                } else { // now we need to determine the sigma angle...
                    // since it's all integer multiples of 2pi/8, we just
                    // need to check for
                    // xld==0, yld<>0, yld==0, xld<>0 and xld/yld=pm1
                    if ( fabs(xlD) < 0.001*fabs(ylD) ) {
                        if (ylD > 0.0) twoTorSigmaV[i]=2;
                        else twoTorSigmaV[i]=6;
                    } else
                        if ( fabs(ylD) < 0.001*fabs(xlD) ) {
                            if (xlD > 0.0) twoTorSigmaV[i]=0L;
                            else twoTorSigmaV[i]=4;
                        } else
                            if (xlD/ylD > 0.0 ) {
                                if (xlD > 0.0) twoTorSigmaV[i]=1;
                                else twoTorSigmaV[i]=5;
                            } else {
                                if (xlD > 0.0) twoTorSigmaV[i]=7;
                                else twoTorSigmaV[i]=3;
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
    if (torRankV.size() > 0)
        if (torRankV[0].first == NLargeInteger(2))
            starti=1;
    // this ensures we skip the 2-torsion
    std::vector<int> tempa;
    unsigned long curri;

    NMatrixInt *tempM;

    for (i=starti; i<torRankV.size(); i++) // for each prime
    {
        tempa.resize(0);
        curri=0;

        // now we cut out the appropriate section of linkingFormPD[i]
        // std::vector< std::pair< NLargeInteger,
        //     std::vector< unsigned long > > > torRankV(indexing.size());
        // starting at curri ending at torRankV[i].second[j]

        for (j=0; j<torRankV[i].second.size(); j++)
        // dimensions of p^{j+1} subspace
        {
            // initialize a torRankV[i].second[j] square matrix.
            tempM=new NMatrixInt(torRankV[i].second[j], torRankV[i].second[j]);

            // tempM will be the torRankV[i].second[j] square submatrix
            // starting at curri, multiplied by tI == p^j
            tI = torRankV[i].first;
            tI.raiseToPower(j+1);

            for (k=0; k<torRankV[i].second[j]; k++)
                for (l=0; l<torRankV[i].second[j]; l++)
                    tempM->entry(k,l) = (NRational(tI)*linkingFormPD[i]->
                        entry(k+curri,l+curri)).getNumerator();

            tempa.push_back( tempM->det().legendre(torRankV[i].first) );
            // legendre symbol, compute and append to tempa
            // compute determinant.

            // delete the temp matrix.
            // if (tempM != 0)
            delete tempM;

            // increment curri
            curri = curri + torRankV[i].second[j]; // crashes here.
        }
        oddTorLegSymV.push_back( make_pair( torRankV[i].first , tempa) );
    }

    // step 4: kk test for: split, hyperbolic, and the embeddability
    //           2^k-torsion condition.

    torsionLinkingFormIsSplit=true;
    torsionLinkingFormIsHyperbolic=true;

    starti=0;
    if (torRankV.size() > 0)
        if (torRankV[0].first == NLargeInteger(2))
            starti=1;

    for (i=0; i<torRankV.size(); i++)
        for (j=0; j<torRankV[i].second.size(); j++)
            if ( (torRankV[i].second[j] % 2) != 0 )
                torsionLinkingFormIsSplit=false;
    if (torsionLinkingFormIsSplit) {
        for (i=0; i<oddTorLegSymV.size(); i++)
            for (j=0; j<oddTorLegSymV[i].second.size(); j++) {
                if ( ( (NLargeInteger(torRankV[i+starti].second[j])*
                        (torRankV[i+starti].first -
                        NLargeInteger::one))/NLargeInteger(4) ) %
                        NLargeInteger(2) == NLargeInteger::zero ) {
                    if (oddTorLegSymV[i].second[j] != 1)
                        torsionLinkingFormIsSplit=false;
                } // does this know how to deal with .second[j]==0??
                else {
                    if (oddTorLegSymV[i].second[j] == 1)
                        torsionLinkingFormIsSplit=false;
                }
            }
    }
    if (starti==1) // have 2-torsion
    { // all the sigmas need to be 0 or inf.
        for (i=0; i<twoTorSigmaV.size(); i++)
            if ( (twoTorSigmaV[i]!=NLargeInteger::zero) &&
                    (twoTorSigmaV[i]!=NLargeInteger::infinity) )
                torsionLinkingFormIsSplit=false;
    }

    if (torsionLinkingFormIsSplit==false) torsionLinkingFormIsHyperbolic=false;

    if ( (torsionLinkingFormIsSplit) && (starti==1) ) {
        torsionLinkingFormIsHyperbolic = true;
        for (i=0; i<twoTorSigmaV.size(); i++)
            if (twoTorSigmaV[i]!=NLargeInteger::zero)
                torsionLinkingFormIsHyperbolic=false;
    }

    NRational tRat;

    torsionLinkingFormSatisfiesKKtwoTorCondition=true;
    if (starti==1) { // for each k need to compute 2^{k-1}*form(x,x) on all
        // elements of order 2^k, check to see if it is zero.
        // so this is not yet quite implemented, yet....
        // std::vector< std::pair< NLargeInteger,
        //     std::vector<unsigned long> > > h1PrimePowerDecomp;
        // stored as list { (2, (1, 1, 2)), (3, (1, 2, 2, 3)), (5, (1, 1, 2)) }
        //std::vector< NMatrixRing<NRational>* > linkingFormPD;
        for (i=0; i<h1PrimePowerDecomp[0].second.size(); i++) {
            // run down diagonal of linkingFormPD[0], for each (i,i) entry
            // multiply it by 2^{h1PrimePowerDecomp[0].second[i]-1} check if
            // congruent to zero. if not, trigger flag.
            tI = NLargeInteger("2");
            tI.raiseToPower(h1PrimePowerDecomp[0].second[i]-1);
            tRat = NRational(tI) * linkingFormPD[0]->entry(i,i);
            tN = tRat.getNumerator();
            tD = tRat.getDenominator();
            tN.divisionAlg(tD,tR);
            if (tR != NLargeInteger::zero)
                torsionLinkingFormSatisfiesKKtwoTorCondition=false;
        }

    }

    torsionRankString.assign("");
    if (torRankV.size()==0) torsionRankString.append("no torsion");
    else for (i=0; i<torRankV.size(); i++) {
            torsionRankString.append(torRankV[i].first.stringValue());
            torsionRankString.append("(");
            for (j=0; j<torRankV[i].second.size(); j++) {
                torsionRankString.append(
                    NLargeInteger(torRankV[i].second[j]).stringValue() );
                if (j < torRankV[i].second.size()-1)
                    torsionRankString.append(" ");
            }
            torsionRankString.append(")");
            if (i<(torRankV.size()-1))
                torsionRankString.append(" ");
    }

    if (tri->isOrientable()) {
        torsionSigmaString.assign("");
        if (twoTorSigmaV.size()==0) torsionSigmaString.append("no 2-torsion");
        else for (i=0; i<twoTorSigmaV.size(); i++) {
            torsionSigmaString.append(twoTorSigmaV[i].stringValue());
            if (i<(twoTorSigmaV.size()-1)) torsionSigmaString.append(" ");
            }
        }
    else torsionSigmaString.assign("manifold is non-orientable");

    if (tri->isOrientable()) {
        torsionLegendreString.assign("");
        if (oddTorLegSymV.size()==0)
            torsionLegendreString.append("no odd p-torsion");
        else for (i=0; i<oddTorLegSymV.size(); i++) {
            torsionLegendreString.append(oddTorLegSymV[i].first.stringValue());
            torsionLegendreString.append("(");
            for (j=0; j<oddTorLegSymV[i].second.size(); j++) {
                torsionLegendreString.append( NLargeInteger(
                    oddTorLegSymV[i].second[j]).stringValue());
                if (j<oddTorLegSymV[i].second.size()-1)
                    torsionLegendreString.append(" ");
            }
            torsionLegendreString.append(")");
            if (i<(oddTorLegSymV.size()-1))
                torsionLegendreString.append(" ");
            }
        }
    else
        torsionLegendreString.append("manifold is non-orientable");

    embeddabilityString.assign("");
    if (tri->isOrientable()) 
      { // orientable
        if (getBMH(0).isTrivial()) 
        { // no boundary : orientable
            if (torRankV.size()==0) 
            { // no torsion : no boundary, orientable
                if (tri->knowsThreeSphere() && tri->isThreeSphere())
                    embeddabilityString = "This manifold is S^3.";
                else if (getDMH(1).isTrivial())
                    embeddabilityString = "Manifold is a homology 3-sphere.";
                else
                    embeddabilityString = "No information.";
            } // no torsion : no boundary, orientable 
            else 
            {// torsion : no boundary, orientable
                if (!torsionLinkingFormSatisfiesKKtwoTorCondition)
                    embeddabilityString =
                        "This manifold, once-punctured, "
                        "does not embed in a homology 4-sphere.";
                else if (!torsionLinkingFormIsHyperbolic)
                    embeddabilityString =
                        "Does not embed in homology 4-sphere.";
                else
                    embeddabilityString = "The torsion linking form is "
                        "of hyperbolic type.";
                if (getDMH(1).getRank()==0)
                    embeddabilityString += "  Manifold is a rational "
                        "homology sphere.";
            } // torsion : no boundary, orientable
        } // no boundary : orientable
        else 
        { // boundary : orientable
            if (torRankV.size()==0) 
                {
                // orientable with boundary, no torsion. We have no tests
                // so far for checking if it embeds in a homology 4-sphere
                // unless we implement the Kojima alexander polynomials.
                // H1 map check... boundary map has full rank iff embeds in
                // rational homology 3-sph
                // boundary map epic iff embeds in homology 3-sphere
                 if (getBMmapH(1).isEpic())
                    {
                    embeddabilityString =
                        "Embeds in a homology 3-sphere as a ";
                    if (getBMH(1).getRank() == 2*getBMH(0).getRank())
                        {
                        if (getBMH(0).getRank()==1)
                            embeddabilityString += "knot complement.";
                        else
                            embeddabilityString += "link complement.";
                        }
                    else
                        embeddabilityString += "graph complement.";
                    }
                 else if (getBMmapH(1).getCoKernel().getRank()==0)
                    {
                    embeddabilityString =
                        "Embeds in a rational homology 3-sphere as a ";
                    if (getBMH(1).getRank() == 2*getBMH(0).getRank() )
                        {
                        if (getBMH(0).getRank()==1)
                            embeddabilityString += "knot complement.";
                        else
                            embeddabilityString += "link complement.";
                        }
                    else
                        embeddabilityString += "graph complement.";
                    } 
                 else
                    embeddabilityString =
                        "Does not embed in a rational homology 3-sphere.";
                 } // no torsion : boundary, orientable
            else
                { // torsion : boundary, orientable
                if (!torsionLinkingFormSatisfiesKKtwoTorCondition)
                 { // two tor condition not satisfied
                 if (getBMmapH(1).isEpic())
                   embeddabilityString =
                        "Embeds in homology 3-sphere "
                        "but not homology 4-sphere.";
                 else if (getBMmapH(1).getCoKernel().getRank()==0)
                   embeddabilityString =
                        "Embeds in rational homology 3-sphere but not "
                        "homology 4-sphere.";
                 else 
                    embeddabilityString =
                        "Does not embed in homology 3-sphere, "
                        "nor homology 4-sphere.";
                 }
                else
                 { // KK twotor condition satisfied...
                 if (getBMmapH(1).isEpic())
                   embeddabilityString =
                        "Embeds in homology 3-sphere.  "
                        "KK 2-tor condition satisfied.";
                 else if (getBMmapH(1).getCoKernel().getRank()==0)
                   embeddabilityString =
                        "Embeds in rational homology 3-sphere.  "
                        "KK 2-tor condition satisfied.";
                 else 
                    embeddabilityString =
                        "Does not embed in homology 3-sphere.  "
                        "KK 2-tor condition satisfied.";
                 }
                } // torsion : boundary, orientable
        } // boundary : orientable 
     } // end orientable 
     else 
     { // triangulation is NOT orientable, therefore can not embed
       // in any rational homology 3-sphere.  So we look at the
       // orientation cover...
       NTriangulation orTri(*tri);
       orTri.makeDoubleCover();
       NHomologicalData covHomol(orTri);
        // break up into two cases, boundary and no boundary...
        if (covHomol.getBMH(0).isTrivial())
         { // no boundary
          if (covHomol.formIsHyperbolic())
            {
            embeddabilityString = "Orientation cover has hyperbolic"
                                  " torsion linking form.";
            }
          else
            {
            embeddabilityString = "Does not embed in homology 4-sphere.";
            }
         }
        else
         {// boundary
          if (covHomol.formSatKK())
            {
            embeddabilityString = "Orientation cover satisfies"
                                      " KK 2-torsion condition.";
            }
          else
            {
            embeddabilityString = "Does not embed in homology 4-sphere.";
            }
         }
     }

    torsionFormComputed = true;
} // end computeTorsionLinkingForm()


bool NHomologicalData::formIsHyperbolic() {
    // TODO: this is not minimal effort!
    // minimal effort approach: for each invariant factor check
    // corresp. getTorsionRank is even. ONLY if all these tests
    // pass, then computeTorsionLinkingForm();
    if (torsionFormComputed)
        return torsionLinkingFormIsHyperbolic;

    unsigned long nif=tri->getHomologyH1().getNumberOfInvariantFactors();
    if (nif == 0)
        return true;

    if ((nif % 2) != 0)
        return false;

    // check invariant factors agree in pairs, if so call
    // computeTorsionLinkingForm
    for (unsigned long i=0;i<(nif/2);i++) {
        if (tri->getHomologyH1().getInvariantFactor(2*i) <
                tri->getHomologyH1().getInvariantFactor((2*i)+1))
            return false;
    }

    computeTorsionLinkingForm();
    return torsionLinkingFormIsHyperbolic;
}


} // namespace regina



