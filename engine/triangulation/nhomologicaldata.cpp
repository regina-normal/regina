
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
#include "triangulation/nhomologicaldata.h"
#include <list>
#include <iostream>
#include <sstream>
#include <cmath>

namespace regina {


void NHomologicalData::writeTextShort(std::ostream& out) const {
    if (mHomology0.get()) {
        out<<"H_0(M) = ";
        mHomology0->writeTextShort(out);
        out<<" ";
    }
    if (mHomology1.get()) {
        out<<"H_1(M) = ";
        mHomology1->writeTextShort(out);
        out<<" ";
    }
    if (mHomology2.get()) {
        out<<"H_2(M) = ";
        mHomology2->writeTextShort(out);
        out<<" ";
    }
    if (mHomology3.get()) {
        out<<"H_3(M) = ";
        mHomology3->writeTextShort(out);
        out<<" ";
    }

    if (bHomology0.get()) {
        out<<"H_0(BM) = ";
        bHomology0->writeTextShort(out);
        out<<" ";
    }
    if (bHomology1.get()) {
        out<<"H_1(BM) = ";
        bHomology1->writeTextShort(out);
        out<<" ";
    }
    if (bHomology2.get()) {
        out<<"H_2(BM) = ";
        bHomology2->writeTextShort(out);
        out<<" ";
    }

    if (bmMap0.get()) {
        out<<"H_0(BM) --> H_0(M) = ";
        bmMap0->writeTextShort(out);
        out<<" ";
    }
    if (bmMap1.get()) {
        out<<"H_1(BM) --> H_1(M) = ";
        bmMap1->writeTextShort(out);
        out<<" ";
    }
    if (bmMap2.get()) {
        out<<"H_2(BM) --> H_2(M) = ";
        bmMap2->writeTextShort(out);
        out<<" ";
    }

    if (dmTomMap1.get()) {
        out<<"PD map = ";
        dmTomMap1->writeTextShort(out);
        out<<" ";
    }
    if (torsionFormComputed) {
        out<<"Torsion form rank vector: "<<torsionRankString<<" ";
        out<<"Torsion sigma vector: "<<torsionSigmaString<<" ";
        out<<"Torsion Legendre symbol vector: "<<torsionLegendreString<<" ";
    }
    if (! embeddabilityString.empty()) {
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

    // number of boundary cells that are ideal ends
    numIdBdryCells[0] = sIEOE.size(); 
    numIdBdryCells[1] = sIEEOF.size();
    numIdBdryCells[2] = sIEFOT.size();

    // number of cells in the mixed decomposition
    numMixCells[0] = numStandardCells[0] + numDualCells[0] + numDualCells[1] + numDualCells[2];
    numMixCells[1] = 2*tri->getNumberOfEdges() + 3*tri->getNumberOfFaces() + numIdBdryCells[1] +
		  4*tri->getNumberOfTetrahedra();
    numMixCells[2] = numIdBdryCells[2] + 3*tri->getNumberOfFaces() + 6*tri->getNumberOfTetrahedra();
    numMixCells[3] = 4*tri->getNumberOfTetrahedra();
}

void NHomologicalData::computeChainComplexes() {
    // Only do this if we haven't already done it.
    if (chainComplexesComputed)
        return;

    if (!ccIndexingComputed) computeccIndexing();

    chainComplexesComputed = true;

    B0.reset(new NMatrixInt(1, numDualCells[0]));
    B1.reset(new NMatrixInt(numDualCells[0], numDualCells[1]));
    B2.reset(new NMatrixInt(numDualCells[1], numDualCells[2]));
    B3.reset(new NMatrixInt(numDualCells[2], numDualCells[3]));
    B4.reset(new NMatrixInt(numDualCells[3], 1));

    A0.reset(new NMatrixInt(1, numStandardCells[0]));
    A1.reset(new NMatrixInt(numStandardCells[0], numStandardCells[1]));
    A2.reset(new NMatrixInt(numStandardCells[1], numStandardCells[2]));
    A3.reset(new NMatrixInt(numStandardCells[2], numStandardCells[3]));
    A4.reset(new NMatrixInt(numStandardCells[3], 1));

    H1map.reset(new NMatrixInt(numStandardCells[1], numDualCells[1]));

    Bd0.reset(new NMatrixInt(1, numBdryCells[0]));
    Bd1.reset(new NMatrixInt(numBdryCells[0], numBdryCells[1]));
    Bd2.reset(new NMatrixInt(numBdryCells[1], numBdryCells[2]));
    Bd3.reset(new NMatrixInt(numBdryCells[2], 1));

    B0Incl.reset(new NMatrixInt(numStandardCells[0], numBdryCells[0]));
    B1Incl.reset(new NMatrixInt(numStandardCells[1], numBdryCells[1]));
    B2Incl.reset(new NMatrixInt(numStandardCells[2], numBdryCells[2]));

    long int temp;
    unsigned long i,j;

    NPerm4 p1,p2;

    // This fills out matrix A1
    for (i=0;i<tri->getNumberOfEdges();i++) {
        // these are the standard edges
        temp=sNIV.index(tri->vertexIndex(tri->getEdge(i)->getVertex(0)));
        (A1->entry( ((temp==(-1)) ?
            (sNIV.size()+sIEOE.index(2*i)) : temp ), i))-=1;
        temp=sNIV.index(tri->vertexIndex(tri->getEdge(i)->getVertex(1)));
        (A1->entry( ((temp==(-1)) ?
            (sNIV.size()+sIEOE.index(2*i+1)) : temp), i))+=1;
    } // ok

    for (i=0;i<sIEEOF.size();i++) { // these are the ideal edges...
        // sIEEOF[i] /3 is the face index, and sIEEOF[i] % 3 tells us
        // the vertex of this face
        p1=tri->getFace(sIEEOF[i]/3)->getEdgeMapping( (sIEEOF[i] + 1) % 3);
        if (p1.sign()==1) {
            A1->entry(sNIV.size() + sIEOE.index(2*(
                tri->edgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )+1), tri->getNumberOfEdges()+i)-=1;
        } else {
            A1->entry(sNIV.size() + sIEOE.index(2*(
                tri->edgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )) , tri->getNumberOfEdges()+i)-=1;
        }
        p1=tri->getFace(sIEEOF[i]/3)->getEdgeMapping( (sIEEOF[i] + 2) % 3);
        if (p1.sign()==1) {
            A1->entry(sNIV.size() + sIEOE.index(2*(
                tri->edgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )) , tri->getNumberOfEdges()+i)+=1;
        } else {
            A1->entry(sNIV.size() + sIEOE.index(2*(
                tri->edgeIndex((tri->getFace(sIEEOF[i]/3))->
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
                A2->entry( tri->edgeIndex(
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
                3*tri->faceIndex(tri->getTetrahedron(
                sIEFOT[i]/4 )->getFace( (sIEFOT[i] + j) % 4)) +
                p1.preImageOf(sIEFOT[i] % 4) ) ,
                tri->getNumberOfFaces()+i ) -= p1.sign();
        }
    }
    // end A2

    // start A3
    for (i=0;i<tri->getNumberOfTetrahedra();i++) {
        for (j=0;j<4;j++) {
            // first go through standard faces 0 through 3
            p1=tri->getTetrahedron(i)->getFaceMapping(j);
            A3->entry( tri->faceIndex(
                tri->getTetrahedron(i)->getFace(j) ), i) += p1.sign();
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
        B1->entry( tri->tetrahedronIndex(
            tri->getFace(dNBF[i])->getEmbedding(1).getTetrahedron() ),i)+=1;
        B1->entry( tri->tetrahedronIndex(
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

            B2->entry( dNBF.index( tri->faceIndex(
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

    long int ind1;
    long int ind2;
    int k;

    NEdgeEmbedding tempe;

    // start B3: for each dual tetrahedron==nonboundary vertex,
    //           find the corresp edges==non-boundary boundary faces
    // problem: this ad-hoc orientation is inaccessible elsewhere.  So it's time to use
    //          regina's accessible one, via regina::NVertexEmbedding::getVertices()

    for (i=0;i<dNINBV.size();i++) {
        // dNINBV[i] is the vertices.index() of this vertex.
        const std::vector<NVertexEmbedding>& vtetlist(
            tri->getVertex(dNINBV[i])->getEmbeddings());

        // It seems best to compile a list of incident edges
        // which contains their endpoint data and sign.
        // the list will be an NIndexedArray<long int> edge_adjacency,
        // data will be stored as
        // 4*(edge index) + 2*(endpt index) + sign stored as 0 or 1.
        NIndexedArray<long int> edge_adjacency;
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
                    if ( ind2 == 1 ) p1=p1*(NPerm4(0,1));
                    // now p1 sends 0 to point corresp to v, 1 to point
                    // corresp to end of edge.
                    // if p1.sign() == tetor[j] then sign = +1 otherwise -1.

                    ind1=4*tri->edgeIndex(
                        vtetlist[j].getTetrahedron()->getEdge(k) )
                        + 2*ind2 + (p1.sign() == vtetlist[j].getVertices().sign() ? 1 : 0);  

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

        unsigned vert0Num = zeroCellMap[tri->tetrahedronIndex(
            tri->getFace(dNBF[j]) -> getEmbedding(0).getTetrahedron() )]/4;
            // vertex number of start vertex in tet0
        unsigned vert1Num = zeroCellMap[tri->tetrahedronIndex(
            tri->getFace(dNBF[j]) -> getEmbedding(1).getTetrahedron() )]/4;
            // vertex number of end vertex in tet1.
        unsigned vert0id = zeroCellMap[tri->tetrahedronIndex(
            tri->getFace(dNBF[j]) -> getEmbedding(0).getTetrahedron() )]%4;
            // not equal to vert0Num if and only if vert0 is ideal.
        unsigned vert1id = zeroCellMap[tri->tetrahedronIndex(
            tri->getFace(dNBF[j]) -> getEmbedding(1).getTetrahedron() )]%4;
            // not equal to vert1Num if and only if vert1 is ideal.
        NPerm4 P1 = tri->getFace(dNBF[j])->getEmbedding(0).getVertices();
        NPerm4 P2 = tri->getFace(dNBF[j])->getEmbedding(1).getVertices();
        NPerm4 P3;
        NPerm4 P0to1 = P2 * ( P1.inverse() ); 
        // the permutation from the start simplex vertices
        // to the end simplex.

        bool stage0nec = false;
        unsigned long stage0edgeNum = 0;
        bool stage0posOr = false;
        unsigned stage0choice = 0; // this indicates the vertex of the simplex
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

            stage0edgeNum = tri->edgeIndex(tri->getFace(dNBF[j]) ->
                getEmbedding(0).getTetrahedron() ->
                getEdge( NEdge::edgeNumber[vert0Num][stage0choice] ));
            stage0posOr = ( static_cast<unsigned>(tri->getFace(dNBF[j]) ->
                getEmbedding(0).getTetrahedron()->getEdgeMapping(
                NEdge::edgeNumber[vert0Num][stage0choice])[1]) == stage0choice) ?
                true : false ;
        }


        bool stage4nec = false; // stage 4
        unsigned long stage4edgeNum = 0;
        bool stage4posOr = false;
        unsigned stage4choice = 0;

        if (vert1Num == tet1FaceIndex) {
            stage4nec = true;

            if (vert1Num == vert1id) // the non-ideal case.
            {
                stage4choice = (tet1FaceIndex + 1) % 4;
            } // duh, this is all wrong.
            else {
                stage4choice = vert1id;
            }

            stage4edgeNum = tri->edgeIndex(tri->getFace(dNBF[j]) ->
                getEmbedding(1).getTetrahedron() ->
                getEdge( NEdge::edgeNumber[vert1Num][stage4choice] ));
            stage4posOr = ( static_cast<unsigned>(tri->getFace(dNBF[j]) ->
                getEmbedding(1).getTetrahedron()->getEdgeMapping(
                NEdge::edgeNumber[vert1Num][stage4choice])[1]) == vert1Num ) ?
                true : false ;
        }

        // decide if stages 1 and 3 are neccessary...
        bool stage1nec = false;         // stage 1
        unsigned stage1v = 0;
        unsigned stage1vi = 0;
        unsigned long stage1edgeNum = 0;
        bool stage1posOr = false;
        unsigned stage1FaceToUse = 0;

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
                NEdge::edgeNumber[stage1v][tet0FaceIndex] )[2];
            P3 = tri->getFace(dNBF[j])->getEmbedding(0).getTetrahedron()->
                getFaceMapping(stage1FaceToUse);
            stage1edgeNum = tri->getNumberOfEdges() + sIEEOF.index(
                3*(tri->faceIndex(tri->getFace(dNBF[j]) ->
                getEmbedding(0).getTetrahedron() ->
                getFace(stage1FaceToUse))) + P3.preImageOf(stage1v) );
            stage1posOr = ( ( static_cast<unsigned>(
                P3[(P3.preImageOf(stage1v)+1) % 3]) !=
                stage1vi ) ? true : false );
        }
        bool stage3nec = false;
        unsigned stage3v = 0;
        unsigned stage3vi = 0;
        unsigned long stage3edgeNum = 0;
        bool stage3posOr = false;
        unsigned stage3FaceToUse = 0;

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
                NEdge::edgeNumber[stage3v][tet1FaceIndex] )[2];
            P3 = tri->getFace(dNBF[j])->getEmbedding(1).getTetrahedron()->
                getFaceMapping(stage3FaceToUse);
            stage3edgeNum = tri->getNumberOfEdges() + sIEEOF.index(
                3*(tri->faceIndex(tri->getFace(dNBF[j]) ->
                getEmbedding(1).getTetrahedron() ->
                getFace(stage3FaceToUse))) + P3.preImageOf(stage3v) );
            stage3posOr = ( ( static_cast<unsigned>(
                P3[(P3.preImageOf(stage3v)+1) % 3]) ==
                stage3vi ) ? true : false );
        }

        unsigned stage2startdata = 0;
        unsigned stage2enddata = 0;
        // 3*vertex number(0,1,2) + another vertex number (0,1,2)
        // these are the same indicates the vertex is non-ideal
        // these are different indicates the vertex is ideal and dir
        // of relevant point..

        if (stage1nec) // set up stage2startdata
        {
            stage2startdata = 3*P1.preImageOf( stage1v ) +
                P1.preImageOf((tri->getFace(dNBF[j]) ->
                getEmbedding(0).getTetrahedron() ->
                getEdgeMapping( NEdge::edgeNumber[stage1v][stage1vi] ))[3] );
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
                getEdgeMapping( NEdge::edgeNumber[stage3v][stage3vi] ))[3] );
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
                    H1map->entry(tri->edgeIndex(tri->getFace(dNBF[j])->getEdge(((currV/3) + 1) % 3 )), j) 
                      += ( 
                           ( 
                             static_cast<unsigned>( 
                              tri->getFace(dNBF[j])->getEdgeMapping(((currV/3) + 1) % 3)[1]
                                                  ) == currV/3
                           ) 
                         ? +1 : -1 );
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
        temp=sBNIV.index(tri->vertexIndex(tri->getEdge(sBNIE[i])->
            getVertex(0)));
        (Bd1->entry( ((temp==(-1)) ? (sBNIV.size()+2*i) : temp ), i))-=1;
        temp=sBNIV.index(tri->vertexIndex(tri->getEdge(sBNIE[i])->
            getVertex(1)));
        (Bd1->entry( ((temp==(-1)) ? (sBNIV.size()+2*i+1) : temp), i))+=1;
    } // ok

    for (i=0;i<sIEEOF.size();i++) { // these are the ideal edges...
        // sIEEOF[i] /3 is the face index, and sIEEOF[i] % 3 tells us
        // the vertex of this face
        p1=tri->getFace(sIEEOF[i]/3)->getEdgeMapping( (sIEEOF[i] + 1) % 3);
        if (p1.sign()==1) {
            Bd1->entry(sBNIV.size() + sIEOE.index(2*(
                tri->edgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )+1), sBNIE.size()+i)-=1;
        } else {
            Bd1->entry(sBNIV.size() + sIEOE.index(2*(
                tri->edgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )) , sBNIE.size()+i)-=1;
        }
        p1=tri->getFace(sIEEOF[i]/3)->getEdgeMapping( (sIEEOF[i] + 2) % 3);
        if (p1.sign()==1) {
            Bd1->entry(sBNIV.size() + sIEOE.index(2*(
                tri->edgeIndex((tri->getFace(sIEEOF[i]/3))->
                getEdge(p1[2])) )) , sBNIE.size()+i)+=1;
        } else {
            Bd1->entry(sBNIV.size() + sIEOE.index(2*(
                tri->edgeIndex((tri->getFace(sIEEOF[i]/3))->
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
                Bd2->entry( sBNIE.index( tri->edgeIndex(tri->getFace(
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
            Bd2->entry( sBNIE.size() + sIEEOF.index(3*tri->faceIndex(
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

const NMarkedAbelianGroup& NHomologicalData::getHomology(unsigned q) {
    if (q==0) {
        if (!mHomology0.get()) {
            computeChainComplexes();
            mHomology0.reset(new NMarkedAbelianGroup(*A0,*A1));
        }
        return *mHomology0;
    } else if (q==1) {
        if (!mHomology1.get()) {
            computeChainComplexes();
            mHomology1.reset(new NMarkedAbelianGroup(*A1,*A2));
        }
        return *mHomology1;
    } else if (q==2) {
        if (!mHomology2.get()) {
            computeChainComplexes();
            mHomology2.reset(new NMarkedAbelianGroup(*A2,*A3));
        }
        return *mHomology2;
    } else {
        // Assume q == 3.  This will at least avoid a crash if q lies
        // outside the required range.
        if (!mHomology3.get()) {
            computeChainComplexes();
            mHomology3.reset(new NMarkedAbelianGroup(*A3,*A4));
        }
        return *mHomology3;
    }
}

const NMarkedAbelianGroup& NHomologicalData::getBdryHomology(unsigned q) {
    computeChainComplexes();
    if (q==0) {
        if (!bHomology0.get()) bHomology0.reset(new NMarkedAbelianGroup(*Bd0,*Bd1));
        return *bHomology0;
    } else if (q==1) {
        if (!bHomology1.get()) bHomology1.reset(new NMarkedAbelianGroup(*Bd1,*Bd2));
        return *bHomology1;
    } else {
        // Assume q == 2.  This will at least avoid a crash if q lies
        // outside the required range.
        if (!bHomology2.get()) bHomology2.reset(new NMarkedAbelianGroup(*Bd2,*Bd3));
        return *bHomology2;
    }
}

const NMarkedAbelianGroup& NHomologicalData::getDualHomology(unsigned q) {
    computeChainComplexes();
    if (q==0) {
        if (!dmHomology0.get()) dmHomology0.reset(new NMarkedAbelianGroup(*B0,*B1));
        return *dmHomology0;
    } else if (q==1) {
        if (!dmHomology1.get()) dmHomology1.reset(new NMarkedAbelianGroup(*B1,*B2));
        return *dmHomology1;
    } else if (q==2) {
        if (!dmHomology2.get()) dmHomology2.reset(new NMarkedAbelianGroup(*B2,*B3));
        return *dmHomology2;
    } else {
        // Assume q == 3.  This will at least avoid a crash if q lies
        // outside the required range.
        if (!dmHomology3.get()) dmHomology3.reset(new NMarkedAbelianGroup(*B3,*B4));
        return *dmHomology3;
    }
}

const NMarkedAbelianGroup& NHomologicalData::getMixedHomology(unsigned q)
{
  if (!M0.get()) computeBaryCC();
  if (q==0) {
    if (!mH0.get()) mH0.reset(new NMarkedAbelianGroup(*M0, *M1));
    return *mH0;
  } else if (q==1) {
    if (!mH1.get()) mH1.reset(new NMarkedAbelianGroup(*M1, *M2));
    return *mH1;
  } else if (q==2) {
    if (!mH2.get()) mH2.reset(new NMarkedAbelianGroup(*M2, *M3));
    return *mH2;  
  } else {
    if (!mH3.get()) mH3.reset(new NMarkedAbelianGroup(*M3, *M4));
    return *mH3;
  }
}


// this is dual cellular -> standard
const NHomMarkedAbelianGroup& NHomologicalData::getH1CellAp() {
    if (!dmTomMap1.get()) {
        computeChainComplexes();
        if (!dmHomology1.get())
         dmHomology1.reset(new NMarkedAbelianGroup(*B1,*B2));
        if (!mHomology1.get())
         mHomology1.reset(new NMarkedAbelianGroup(*A1,*A2));
        dmTomMap1.reset(new NHomMarkedAbelianGroup(
            *dmHomology1, *mHomology1, *H1map ));
    }
    return (*dmTomMap1);
}

const NHomMarkedAbelianGroup& NHomologicalData::getStandardToMixedHom(unsigned q)
{ 
  computeBaryCC();
  computeChainComplexes();
  if (q==0) {
    // step 1, make sure relevant homology groups have been computed
        if (!mHomology0.get()) mHomology0.reset(new NMarkedAbelianGroup(*A0,*A1));
        if (!mH0.get()) mH0.reset(new NMarkedAbelianGroup(*M0,*M1));
        if (!SMHom0.get()) SMHom0.reset(new NHomMarkedAbelianGroup( *mHomology0, *mH0, *AM0 ) );
	return *SMHom0; 
  } else if (q==1) {
        if (!mHomology1.get()) mHomology1.reset(new NMarkedAbelianGroup(*A1,*A2));
        if (!mH1.get()) mH1.reset(new NMarkedAbelianGroup(*M1,*M2));
        if (!SMHom1.get()) SMHom1.reset(new NHomMarkedAbelianGroup( *mHomology1, *mH1, *AM1 ) );
	return *SMHom1; 
  } else if (q==2) {
        if (!mHomology2.get()) mHomology2.reset(new NMarkedAbelianGroup(*A2,*A3));
        if (!mH2.get()) mH2.reset(new NMarkedAbelianGroup(*M2,*M3));
        if (!SMHom2.get()) SMHom2.reset(new NHomMarkedAbelianGroup( *mHomology2, *mH2, *AM2 ) );
	return *SMHom2; 
  } else {
        if (!mHomology3.get()) mHomology3.reset(new NMarkedAbelianGroup(*A3,*A4));
        if (!mH3.get()) mH3.reset(new NMarkedAbelianGroup(*M3,*M4));
        if (!SMHom3.get()) SMHom3.reset(new NHomMarkedAbelianGroup( *mHomology3, *mH3, *AM3 ) );
	return *SMHom3; 
  }
}


const NHomMarkedAbelianGroup& NHomologicalData::getDualToMixedHom(unsigned q)
{
  computeBaryCC();
  computeChainComplexes();
  if (q==0) {
        if (!dmHomology0.get()) dmHomology0.reset(new NMarkedAbelianGroup(*B0,*B1));
        if (!mH0.get()) mH0.reset(new NMarkedAbelianGroup(*M0,*M1));
        if (!DMHom0.get()) DMHom0.reset(new NHomMarkedAbelianGroup( *dmHomology0, *mH0, *BM0 ) );
	return *DMHom0; 
  } else if (q==1) {
        if (!dmHomology1.get()) dmHomology1.reset(new NMarkedAbelianGroup(*B1,*B2));
        if (!mH1.get()) mH1.reset(new NMarkedAbelianGroup(*M1,*M2));
        if (!DMHom1.get()) DMHom1.reset(new NHomMarkedAbelianGroup( *dmHomology1, *mH1, *BM1 ) );
	return *DMHom1; 
  } else if (q==2) {
        if (!dmHomology2.get()) dmHomology2.reset(new NMarkedAbelianGroup(*B2,*B3));
        if (!mH2.get()) mH2.reset(new NMarkedAbelianGroup(*M2,*M3));
        if (!DMHom2.get()) DMHom2.reset(new NHomMarkedAbelianGroup( *dmHomology2, *mH2, *BM2 ) );
	return *DMHom2; 
  } else {
        if (!dmHomology3.get()) dmHomology3.reset(new NMarkedAbelianGroup(*B3,*B4));
        if (!mH3.get()) mH3.reset(new NMarkedAbelianGroup(*M3,*M4));
        if (!DMHom3.get()) DMHom3.reset(new NHomMarkedAbelianGroup( *dmHomology3, *mH3, *BM3 ) );
	return *DMHom3; 
  }
}

const NHomMarkedAbelianGroup& NHomologicalData::getBdryHomologyMap(unsigned q) {
    computeChainComplexes();
    if (q==0) {
        if (!bHomology0.get()) bHomology0.reset(new NMarkedAbelianGroup(*Bd0,*Bd1));
        if (!mHomology0.get()) mHomology0.reset(new NMarkedAbelianGroup(*A0,*A1));
        if (!bmMap0.get()) bmMap0.reset(new NHomMarkedAbelianGroup( *bHomology0, *mHomology0, *B0Incl ));
        return *bmMap0;
    } else if (q==1) {
        if (!bHomology1.get()) bHomology1.reset(new NMarkedAbelianGroup(*Bd1,*Bd2));
        if (!mHomology1.get()) mHomology1.reset(new NMarkedAbelianGroup(*A1,*A2));
        if (!bmMap1.get()) bmMap1.reset(new NHomMarkedAbelianGroup( *bHomology1, *mHomology1, *B1Incl ));
        return *bmMap1;
    } else {
        // Assume q == 2.  This will at least avoid a crash if q lies
        // outside the required range.
        if (!bHomology2.get()) bHomology2.reset(new NMarkedAbelianGroup(*Bd2,*Bd3));
        if (!mHomology2.get()) mHomology2.reset(new NMarkedAbelianGroup(*A2,*A3));
        if (!bmMap2.get()) bmMap2.reset(new NHomMarkedAbelianGroup( *bHomology2, *mHomology2, *B2Incl ));
        return *bmMap2;
    }
}

void NHomologicalData::computeTorsionLinkingForm() {
    // Only do this if we haven't done it already.
    if (torsionFormComputed)
        return;

    // dual h1 --> standard h1 isomorphism:
    const NHomMarkedAbelianGroup& h1CellAp(getH1CellAp());
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
            tV = dmHomology1->getTorsionRep(i);

            for (k=0; k<tV.size(); k++) tV[k]=fac1i*fac2*tV[k];

            pvList.push_back(tV);
        }
    }

    // step1a: construct (2 2 4) (3 3 9 27) ... indexing of ppList, pvList, etc.
    // the indexing will be as a list of pairs
    // < prime, vector< pair< power, index> > >
    // Use a list because we are continually inserting items in the middle.
    typedef std::vector<std::pair<unsigned long, unsigned long> >
        IndexingPowerVector;
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

    // step 2: construct dual vectors
    //           for every pvList vector, find corresponding standard vector.
    NMatrixInt standardBasis( numStandardCells[1], pvList.size() );
    const NMatrixInt& dualtostandard(h1CellAp.getDefiningMatrix());

    for (i=0; i<standardBasis.rows(); i++)
        for (j=0; j<standardBasis.columns(); j++)
            for (k=0; k<dualtostandard.columns(); k++)
                standardBasis.entry(i,j) +=
                    dualtostandard.entry(i,k)*pvList[j][k];

    // step 3: construct bounding classes
    //           the j-th column of standardBasis, when multiplied by
    //           ppList[j] bounds, so find a chain with that boundary and
    //           put its info in a matrix.

    NMatrixInt ON(mHomology1->getN());
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

    // Compute indexing.size() just once, since for std::list this might be
    // a slow operation.
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
            NMatrixInt tempM(torRankV[i].second[j], torRankV[i].second[j]);

            // tempM will be the torRankV[i].second[j] square submatrix
            // starting at curri, multiplied by tI == p^j
            tI = torRankV[i].first;
            tI.raiseToPower(j+1);

            for (k=0; k<torRankV[i].second[j]; k++)
                for (l=0; l<torRankV[i].second[j]; l++)
                    tempM.entry(k,l) = (NRational(tI)*linkingFormPD[i]->
                        entry(k+curri,l+curri)).getNumerator();

            tempa.push_back( tempM.det().legendre(torRankV[i].first) );
            // legendre symbol, compute and append to tempa
            // compute determinant.

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

    torsionFormComputed = true;
} // end computeTorsionLinkingForm()

void NHomologicalData::computeEmbeddabilityString() {
    // Only do this if we haven't done it already.
    if (! embeddabilityString.empty())
        return;

    if (tri->getNumberOfTetrahedra() == 0)
      {
        // special-case the empty triangulation
        embeddabilityString = "Manifold is empty.";
      }
    else if (tri->isOrientable())
      { // orientable -- we need the torsion linking form
        computeTorsionLinkingForm();

        if (getBdryHomology(0).isTrivial()) 
        { // no boundary : orientable
            if (torRankV.size()==0) 
            { // no torsion : no boundary, orientable
                if (tri->knowsThreeSphere() && tri->isThreeSphere())
                    embeddabilityString = "This manifold is S^3.";
                else if (getDualHomology(1).isTrivial())
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
                if (getDualHomology(1).getRank()==0)
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
                 if (getBdryHomologyMap(1).isEpic())
                    {
                    embeddabilityString =
                        "Embeds in a homology 3-sphere as a ";
                    if (getBdryHomology(1).getRank() ==
                            2*getBdryHomology(0).getRank())
                        {
                        if (getBdryHomology(0).getRank()==1)
                            embeddabilityString += "knot complement.";
                        else
                            embeddabilityString += "link complement.";
                        }
                    else
                        {
                        if (getBdryHomology(1).getRank() == 0)
                            embeddabilityString += "ball complement.";
                        else
                            embeddabilityString += "graph complement.";
                        }
                    }
                 else if (getBdryHomologyMap(1).getCokernel().getRank()==0)
                    {
                    embeddabilityString =
                        "Embeds in a rational homology 3-sphere as a ";
                    if (getBdryHomology(1).getRank() ==
                            2*getBdryHomology(0).getRank() )
                        {
                        if (getBdryHomology(0).getRank()==1)
                            embeddabilityString += "knot complement.";
                        else
                            embeddabilityString += "link complement.";
                        }
                    else
                        {
                        if (getBdryHomology(1).getRank() == 0)
                            embeddabilityString += "ball complement.";
                        else
                            embeddabilityString += "graph complement.";
                        }
                    } 
                 else
                    embeddabilityString =
                        "Does not embed in a rational homology 3-sphere.";
                 } // no torsion : boundary, orientable
            else
                { // torsion : boundary, orientable
                if (!torsionLinkingFormSatisfiesKKtwoTorCondition)
                 { // two tor condition not satisfied
                 if (getBdryHomologyMap(1).isEpic())
                   embeddabilityString =
                        "Embeds in homology 3-sphere "
                        "but not homology 4-sphere.";
                 else if (getBdryHomologyMap(1).getCokernel().getRank()==0)
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
                 if (getBdryHomologyMap(1).isEpic())
                   embeddabilityString =
                        "Embeds in homology 3-sphere.  "
                        "KK 2-tor condition satisfied.";
                 else if (getBdryHomologyMap(1).getCokernel().getRank()==0)
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
        if (covHomol.getBdryHomology(0).isTrivial())
         { // no boundary
          if (covHomol.formIsHyperbolic())
            embeddabilityString = "Orientation cover has hyperbolic"
                                  " torsion linking form.";
          else
            embeddabilityString = "Does not embed in homology 4-sphere.";
         }
        else
         {// boundary
          if (covHomol.formSatKK())
            embeddabilityString = "Orientation cover satisfies"
                                      " KK 2-torsion condition.";
          else
            embeddabilityString = "Does not embed in homology 4-sphere.";
         }
     }
} // end computeEmbeddabilityString()


bool NHomologicalData::formIsHyperbolic() {
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



/** Compute the chain complexes from the point of view of the barycentric CW-decomposition.
     this will also compute all the relevant maps to the simplicial homology of the
     triangulation and the CW-homology of the dual polyhedral decomposition. **/
// Note: currently this does not produce a chain complex.  It appears there's an orientation 
//       inconsistency. 
void NHomologicalData::computeBaryCC()
{
 // only continue if this has not been called before.
 if (M0.get()) return;
 computeChainComplexes();
 // first things first, set up chain complexes, mimick NHomologicalData::computeChainComplexes()
 //  as everything in here will be a special case of there...

 // Setting up the chain complexes.  Implicitly, all the indexing will be by 
 //  1st pure T^i, then pure P^j, then T^i \cap P^j in lexicographical order, 
 //  then ideal stuff in same order...

 unsigned long v1 = sNIV.size(); 
 unsigned long v2 = numStandardCells[0]; 
 unsigned long v3 = v2 + tri->getNumberOfEdges();
 unsigned long v4 = v3 + tri->getNumberOfFaces();

 // In the mixed cell decomp, there are 0-cells for all T^0, P^0, the ends of ideal edges, 
 // 	and intersections T^1 \cap P^2, T^2 \cap P^1
 M0.reset(new NMatrixInt(1, numMixCells[0])); // C0 -> 0
 // There are 1-cells for all T^1, P^1, ends of ideal faces, and T^2 \cap P^2
 M1.reset(new NMatrixInt(numMixCells[0], numMixCells[1])); // C1 -> C0
 // There are 2-cells for all T^2, P^2, and ends of ideal tets. 
 M2.reset(new NMatrixInt(numMixCells[1], numMixCells[2])); // C2 -> C1
 // There are four 3-cells for each tetrahedron. 
 M3.reset(new NMatrixInt(numMixCells[2], numMixCells[3])); // C3 -> C2
 // no 4-cells. 
 M4.reset(new NMatrixInt(numMixCells[3], 1)); // 0 -> C3

 // M0 == 0 already
 long int temp;
 NPerm4 p1, p2;

 // This fills out matrix M1 in 4 parts.
 //  1st part: standard edge boundaries
 for (unsigned long i=0; i<tri->getNumberOfEdges(); i++)
  { // M1.entry(?, i)
    // each standard edge divided in 2, orientation induced from standard edge. First half
    temp=sNIV.index(tri->vertexIndex(tri->getEdge(i)->getVertex(0))); // temp == -1 if ideal
    (M1->entry( ((temp==(-1)) ? (v1+sIEOE.index(2*i)) : temp), 2*i)) -= 1;
    M1->entry( v2 + i, 2*i) += 1;
     // 2nd half
    temp=sNIV.index(tri->vertexIndex(tri->getEdge(i)->getVertex(1))); // temp == -1 if ideal
    (M1->entry( ((temp==(-1)) ? (v1+sIEOE.index(2*i+1)) : temp), 2*i+1)) += 1;
    M1->entry( v2 + i, 2*i+1) -= 1;
  }
 // ideal edges, oriented by face
 unsigned long tc=2*tri->getNumberOfEdges();
 for (unsigned long i=0; i<numIdBdryCells[1]; i++)
  { // M1.entry(?, tc + i)
        // sIEEOF[i] /3 is the face index, and sIEEOF[i] % 3 tells us the vertex of this face

        p1=tri->getFace(sIEEOF[i]/3)->getEdgeMapping( (sIEEOF[i] + 1) % 3);
        int ei=tri->edgeIndex((tri->getFace(sIEEOF[i]/3))-> getEdge(p1[2]));
        if (p1.sign()==1) {
	                   M1->entry(v1 + sIEOE.index(2*ei+1), tc+i)-=1;
        } else {
		           M1->entry(v1 + sIEOE.index(2*ei) , tc+i)-=1;
        }

        p1=tri->getFace(sIEEOF[i]/3)->getEdgeMapping( (sIEEOF[i] + 2) % 3);
        ei = tri->edgeIndex((tri->getFace(sIEEOF[i]/3))->getEdge(p1[2]));
        if (p1.sign()==1) {
                            M1->entry(v1 + sIEOE.index(2*ei) , tc+i)+=1;
        } else {
                            M1->entry(v1 + sIEOE.index(2*ei+1) , tc+i)+=1;
        }
  }
 // the 3 internal face edges
 tc += numIdBdryCells[1];
 for (unsigned long i=0; i<3*tri->getNumberOfFaces(); i++)
  { // M1.entry(?, tc + i)
    M1->entry( v2 + tri->edgeIndex( tri->getFace( i/3 )->getEdge( i % 3 ) ), tc + i ) += 1;
    M1->entry( v3 + (i/3), tc + i ) -= 1;
  }
 // the 4 internal tet edges    
 tc += 3*tri->getNumberOfFaces();
 for (unsigned long i=0; i<4*tri->getNumberOfTetrahedra(); i++)
  { // M1.entry(?, tc + i)
    M1->entry( v3 + tri->faceIndex( tri->getTetrahedron( i/4 )->getFace( i % 4 ) ), tc+i ) += 1;
    M1->entry( v4 + (i/4), tc + i ) -= 1;
  }
  // that handles matrix M1.

  unsigned long w1 = 2*tri->getNumberOfEdges();
  unsigned long w2 = w1 + numIdBdryCells[1];
  unsigned long w3 = w2 + 3*tri->getNumberOfFaces();
  tc=0;

  // this fills out M2 in 3 parts
  // part 1 standard faces
  for (unsigned long i=0; i<3*tri->getNumberOfFaces(); i++)
   { // M2.entry(?, tc+i) -- the 2-cells from 1->3 pachner on standard faces
     M2->entry( w2 + 3*(i/3) + ((i+1) % 3), tc + i ) += 1; 
     M2->entry( w2 + 3*(i/3) + ((i+2) % 3), tc + i ) -= 1; 

     int j=tri->getFace( i/3 )->getEdgeMapping( (i+1) % 3 ).sign();
     M2->entry( 2*tri->edgeIndex(tri->getFace( i/3 )->getEdge( (i+1) % 3 )) + ((j==1)? 1 : 0), tc + i ) += j;

     j=tri->getFace( i/3 )->getEdgeMapping( (i+2) % 3 ).sign();
     M2->entry( 2*tri->edgeIndex(tri->getFace( i/3 )->getEdge( (i+2) % 3 )) + ((j==1)? 0 : 1), tc + i ) += j;

     if ( tri->getFace( i/3 )->getVertex( i%3 )->isIdeal() )
        M2->entry( w1 + sIEEOF.index(i), tc + i ) += 1;
   }
  tc += 3*tri->getNumberOfFaces();
  // part 2 ideal ends of tetrahedra
  for (unsigned long i=0; i<numIdBdryCells[2]; i++)
   { // M2.entry(?, tc+i)
     for (unsigned long j=1; j<4; j++)
     { // face mappings of things that have the relevant ideal bdry
      p1=tri->getTetrahedron( sIEFOT[i]/4 )->getFaceMapping((sIEFOT[i] + j) % 4);
      int fi = tri->faceIndex(tri->getTetrahedron(sIEFOT[i]/4 )->getFace( (sIEFOT[i] + j) % 4));
      M2->entry( w1 + sIEEOF.index(3*fi + p1.preImageOf(sIEFOT[i] % 4) ), tc + i ) -= p1.sign();
      }
   }

  tc += numIdBdryCells[2];
  // part 3 the 6 2-dimensional dual polyhedral bits in a tetrahedron
  for (unsigned long i=0; i<6*tri->getNumberOfTetrahedra(); i++)
   { // M2.entry(?, tc+i)
     // one term for every edge of a tetrahedron. i/6 is tet index, i%6 is edge index
     // we're orienting the faces via getEdgeMapping perm, which we modify to be in A_4
     //  to me at the moment. So we orient the square from tet edge to p1[2] to barycentre to p1[3].
     // the trouble almost certainly has to be here.
     p1 = tri->getTetrahedron( i/6 ) -> getEdgeMapping( i%6 );
     if (p1.sign()!=1) p1 = p1 * NPerm4(2,3); // getEdgeMapping not always in A_4
     p2 = tri->getTetrahedron( i/6 ) -> getFaceMapping( p1[3] );

     // two boundary edges in face
     // near one is in face
     M2->entry( w2 + 3*tri->faceIndex( tri->getTetrahedron( i/6 ) -> getFace( p1[3] )) + 
	p2.preImageOf(p1[2]), tc+i ) -= 1;
     // far one
     p2 = tri->getTetrahedron( i/6 ) -> getFaceMapping( p1[2] );
 
     M2->entry( w2 + 3*tri->faceIndex( tri->getTetrahedron( i/6 ) -> getFace( p1[2] )) + 
	p2.preImageOf(p1[3]), tc+i ) += 1;

     // two boundary edges in tet interior.
     // near one  
     M2->entry( w3 + 4*(i/6) + p1[3], tc+i ) -= 1; // ok
     // far one
     M2->entry( w3 + 4*(i/6) + p1[2], tc+i ) += 1;
   }
  // done M2

  tc = 0;
  unsigned long q1 = 3*tri->getNumberOfFaces();
  unsigned long q2 = q1 + numIdBdryCells[2];
  // this fills out M3 in 1 part
  for (unsigned long i=0; i<4*tri->getNumberOfTetrahedra(); i++)
   { // 3 parts to deal with, first the 3 faces adjacent to the corner
     // I'm giving the boundary the inner-pointing normal orientation
     p1 = tri->getTetrahedron( i/4 )->getFaceMapping( (i+1) % 4 );
     M3->entry( 3*tri->faceIndex( tri->getTetrahedron( i/4 )->getFace( (i+1) % 4) ) + 
	p1.preImageOf( i % 4 ), i ) += p1.sign();

     p1 = tri->getTetrahedron( i/4 )->getFaceMapping( (i+2) % 4 );
     M3->entry( 3*tri->faceIndex( tri->getTetrahedron( i/4 )->getFace( (i+2) % 4) ) + 
	p1.preImageOf( i % 4 ), i ) += p1.sign();

     p1 = tri->getTetrahedron( i/4 )->getFaceMapping( (i+3) % 4 );
     M3->entry( 3*tri->faceIndex( tri->getTetrahedron( i/4 )->getFace( (i+3) % 4) ) + 
	p1.preImageOf( i % 4 ), i ) += p1.sign();
     // the 3 faces opposite the corner
     int en = edgeNumber[i % 4][(i+1) % 4];
     p1 = tri->getTetrahedron( i/4 )->getEdgeMapping( en ); // 0 1
     M3->entry( q2 + 6*(i/4) + en, i) -= ((p1[0] == (i % 4)) ? 1 : -1);

     en = edgeNumber[i % 4][(i+2) % 4];
     p1 = tri->getTetrahedron( i/4 )->getEdgeMapping( en ); // 0 2
     M3->entry( q2 + 6*(i/4) + en, i) -= ((p1[0] == (i % 4)) ? 1 : -1);

     en = edgeNumber[i % 4][(i+3) % 4];
     p1 = tri->getTetrahedron( i/4 )->getEdgeMapping( en ); // 0 3
     M3->entry( q2 + 6*(i/4) + en, i) -= ((p1[0] == (i % 4)) ? 1 : -1);

     // and the ideal face, if there is one.
     if (tri->getTetrahedron( i/4 )->getVertex( i%4 )->isIdeal())
	M3->entry( q1 + sIEFOT.index(i), i) += 1; // we're now using the inner
	 // orientation convention to  agree w/ the other chain complexes.
   }
  // done M3

  // M4 is always zero.

  // now set up all the chain maps to the (more) efficient homology groups.
   AM0.reset(new NMatrixInt(numMixCells[0], numStandardCells[0])); // AC0 -> MC0
   AM1.reset(new NMatrixInt(numMixCells[1], numStandardCells[1])); // AC1 -> MC1
   AM2.reset(new NMatrixInt(numMixCells[2], numStandardCells[2])); // AC2 -> MC2
   AM3.reset(new NMatrixInt(numMixCells[3], numStandardCells[3])); // AC3 -> MC3

   BM0.reset(new NMatrixInt(numMixCells[0], numDualCells[0])); // BC0 -> MC0
   BM1.reset(new NMatrixInt(numMixCells[1], numDualCells[1])); // BC1 -> MC1
   BM2.reset(new NMatrixInt(numMixCells[2], numDualCells[2])); // BC2 -> MC2
   BM3.reset(new NMatrixInt(numMixCells[3], numDualCells[3])); // BC3 -> MC3

   // The chain maps describing the homomorphisms standard homology -> mixed cellular
   for (unsigned long i=0; i<v2; i++) AM0->entry(i,i) = 1; //the 0-cells
   for (unsigned long i=0; i<w1; i++)  // the standard 1-cells 
	AM1->entry(i,i/2) = 1;
   for (unsigned long i=0; i<numIdBdryCells[1];i++) // the ideal 1-cells
	AM1->entry(w1+i, w1/2 + i) = 1;
   for (unsigned long i=0; i<q1; i++) // standard 2-cells
	AM2->entry(i, i/3) = 1;
   for (unsigned long i=0; i<numIdBdryCells[2];i++) // ideal 2-cells
 	AM2->entry(q1 + i, q1/3 + i) = 1;
   for (unsigned long i=0; i<4*tri->getNumberOfTetrahedra(); i++)
	AM3->entry(i, i/4) = 1;

   // the chain maps describing thehomomorphisms dual homology -> mixed cellular
   for (unsigned long i=0; i<numDualCells[0]; i++) // 0-cells
	BM0->entry(v4+i, i) = 1;
   for (unsigned long i=0; i<numDualCells[1]; i++) // 1-cells
	{ // each such dual cell comes from a face, whose relative orientationis given by
	  // getEmbedding(0 to 1).  So we need to figure out the respective two tetrahedra
	  // and the vertices corresponding to the faces. Oh, boundary faces don't count so
	  // there'll be some reindexing using dNBF
	BM1->entry( w3 + 4*tri->getTetrahedronIndex(tri->getFace(dNBF[i])->getEmbedding(0).getTetrahedron())+
	   tri->getFace(dNBF[i])->getEmbedding(0).getFace(), i ) += 1;
	BM1->entry( w3 + 4*tri->getTetrahedronIndex(tri->getFace(dNBF[i])->getEmbedding(1).getTetrahedron())+
           tri->getFace(dNBF[i])->getEmbedding(1).getFace(), i ) -= 1;
	}
   for (unsigned long i=0; i<numDualCells[2]; i++) // 2-cells
	for (unsigned long j=0; j<tri->getEdge(dNBE[i])->getNumberOfEmbeddings(); j++)
	 BM2->entry( q2 + 6*tri->getTetrahedronIndex(tri->getEdge(dNBE[i])->getEmbedding(j).getTetrahedron())+
	   tri->getEdge(dNBE[i])->getEmbedding(j).getEdge(), i) += 
		tri->getEdge(dNBE[i])->getEmbedding(j).getVertices().sign() ;

   for (unsigned long i=0; i<numDualCells[3]; i++) // 3-cells
	for (unsigned long j=0; j<tri->getVertex(dNINBV[i])->getNumberOfEmbeddings(); j++)
	  BM3->entry( 4*tri->getTetrahedronIndex(tri->getVertex(dNINBV[i])->getEmbedding(j).getTetrahedron())+
 			tri->getVertex(dNINBV[i])->getEmbedding(j).getVertex(), i) += 
			tri->getVertex(dNINBV[i])->getEmbedding(j).getVertices().sign();

  // done
}

NMarkedAbelianGroup NHomologicalData::imgH2form(unsigned long p)
{
// there's less work to do if p==0 so we'll start assuming that
//  basically all we have to do is compose the two isomorphisms between the
//  standard and dual cellular homologies at the H_2-level, then pair being 
//  careful about the simplex's orientation in the manifold

//if (p==0)
// {

	unsigned long k=getDualHomology(2).getRank();
if ((k==0) || (!tri->isOrientable())) return NMarkedAbelianGroup( 0, NLargeInteger::zero );

	NMarkedAbelianGroup freeGens( k*k, NLargeInteger::zero );
        NMatrixInt h2pairing( numDualCells[1], k*k );

	NHomMarkedAbelianGroup MtD1( getDualToMixedHom(1).inverseHom() );
	NHomMarkedAbelianGroup DtS2( getStandardToMixedHom(2).inverseHom()*getDualToMixedHom(2) );

	for (unsigned long i=0; i<k; i++) for (unsigned long j=0; j<k; j++)
	{ // a(i) in CC coords is getDualHomology(2).getFreeRep(i)
	  std::vector<NLargeInteger> aiDH(getDualHomology(2).getFreeRep(i));
          // b(j) in CC coords is getDualHomology(2).getFreeRep(j)
	  std::vector<NLargeInteger> bjDH(getDualHomology(2).getFreeRep(j));
          // B(j) in CC coords is DtS2.evalCC( b(j) )
	  std::vector<NLargeInteger> bjSH(DtS2.evalCC(bjDH));
	  // compute pairing a(i) and B(j), first in mixed homology coords
	  std::vector<NLargeInteger> pijMH(numMixCells[1], NLargeInteger::zero);
	  // ... needs to be filled out now...
	  // run through the list of appropriate mixed 1-cells 3*numfaces...
          // for each one, find corresponding dual 2-cell and standard 2-cell
	  for (unsigned long l=0; l<3*tri->getNumberOfFaces(); l++)
	   {
	    pijMH[2*tri->getNumberOfEdges()+numIdBdryCells[1]+l] = 
	         bjSH[l/3] * aiDH[ dNBE.index(tri->edgeIndex(tri->getFace(l/3)->getEdge(l % 3))) ] *
		 tri->getFace(l/3)->getEdgeMapping(l % 3).sign() *
		 tri->getFace(l/3)->getEdge(l % 3)->getEmbedding(0).getVertices().sign() *
	         tri->getFace(l/3)->getEdge(l % 3)->getEmbedding(0).getTetrahedron()->orientation();
	   // intersection count is indexed by faces.  for each face find the quantity of Bj
	   // and we look "in" to one of the adjacent tetrahedra, I guess the 0th one should be fine
	   // as it doesn't matter which one we choose, and the 1st one might not exist if we're on
	   // a standard boundary component.

	   // now we choose an orientation convention for the intersection.  There will be
	   // (-1)^simplex orientation * (-1)^does the corresp. edge give the correct orientation of the face
	   // the convention is you take 3 vectors v1 v2 v3 positively orienting the space, 
	   // the 1st in the intersection, v2 orienting the intersection, 
	   //                              v1 v2 orienting the dual cycle
           //	                          v2 v3 orienting the face
	   // 				  v1 v2 v3 local orientation
	   //              s(v1) = s(v1 v2) s(v1 v3) s(v1 v2 v3) convention. 
 	   //                      s(v1 v2) = +1 by our conventions.
           //                      s(v2 v3) = relative orientation of edge in face
           //                   s(v1 v2 v3) = edgeembedding.sign() * simplex orientation
           //			             does not matter which simplex you choose so
           //                                 might as well coose the 0th on the list.
	   }
	  std::vector<NLargeInteger> pijDH(MtD1.evalCC(pijMH));	  

	  for (unsigned long l=0; l<h2pairing.rows(); l++)
		  h2pairing.entry(l, (k*i)+j) = pijDH[l];
	} 
	// construct homomorphism from Z^{k^2} --> H_1 where k is the number of
	// free generators of H_2 all in dual homology coordinates.  
	// So an element of Z^{k^2} is a pair a,b where a and b
	// represent H_2 classes.  Convert b to a standard class, find pairing, convert
	// it to a dual class.  Request image of this NHomMarkedAbelianGroup, return. 
	NHomMarkedAbelianGroup ontoImg( freeGens, getDualHomology(1), h2pairing);
	return ontoImg.getImage();
// }
//else 
// { // implement once NMarkedAbelianGroup and NHomMarkedAbelianGroup have mod-p coefficients.
// }
/*
With coefficients in Z_p, H^1(M;Z_p) is isomorphic to H_2(M;Z_p) by poincare duality, and
this is the direct sum of ker L_p on H_1(M;Z) and coker L_p on H_2(M;Z).   So with mod-p
coefficients,  there are four types of pairings to compute:

1) ker L_p H_1(M;Z) \otimes ker L_p H_1(M;Z)     ---> H_1(M;Z_p)

2) ker L_p H_1(M;Z) \otimes coker L_p H_2(M;Z)   ---> H_1(M;Z_p)

3) coker L_p H_2(M;Z) \otimes ker L_p H_1(M;Z)   ---> H_1(M;Z_p)

4) coker L_p H_2(M;Z) \otimes coker L_p H_2(M;Z) ---> H_1(M;Z_p)

By the anti-symmetry of the pairing 2 and 3 have the same image so that leaves us with only
3 pairings to compute. 

4) is simply the mod-p reduction of the integral H_2-pairing. 

2) is obtained by taking [x], [y], writing px=\partial A, and computing the intersection A\cap y

1) is obtained by taking [x], [y], writing px=\partial A, py=\partial B, computing the intersection A \cap B. 
   
So everything here can be done in integral coefficients except the last step of determining the subgroup
of H_1(M;Z_p). So we'll have to compute H_1(M;Z_p) seperately, and cook up the homomorphism. Not so bad.

Add routines to get rank p-torsion subgroup, i-th generator of is in CC coords and SNF coords
Add routine to get num generators of G/pG and reps of i-th generators in CC and SNF coords. 

Then construct H_1(M;Z_p) in mixed and dual coordinates and mixed->dual iso for Z_p, and
construct image of pairing in H_1(M;Z_p). 

Continuing the thought, NMarkedAbelianGroup can add isBoundary() routine and
writeAsBoundary() routine.
*/

}

bool NHomologicalData::verifyChainComplexes()
{
bool retval = true;
computeChainComplexes(); 
computeBaryCC();
for (unsigned long i=0; i<4; i++) if (!getDualToMixedHom(i).isCycleMap()) retval = false;
for (unsigned long i=0; i<4; i++) if (!getStandardToMixedHom(i).isCycleMap()) retval = false;
if (!getH1CellAp().isCycleMap()) retval = false;
for (unsigned long i=0; i<3; i++) if (!getBdryHomologyMap(i).isCycleMap()) retval = false;
if (!getDualToMixedHom(2).isChainMap(getDualToMixedHom(1))) retval = false;
if (!getDualToMixedHom(3).isChainMap(getDualToMixedHom(2))) retval = false;
if (!getStandardToMixedHom(2).isChainMap(getStandardToMixedHom(1))) retval = false;
if (!getStandardToMixedHom(3).isChainMap(getStandardToMixedHom(2))) retval = false;
return retval;
}

bool NHomologicalData::verifyCoordinateIsomorphisms()
{
bool retval = true;
for (unsigned long i=0; i<4; i++) if (!getDualToMixedHom(i).isIso()) retval = false;
for (unsigned long i=0; i<4; i++) if (!getStandardToMixedHom(i).isIso()) retval = false;
for (unsigned long i=0; i<4; i++) if (!getDualToMixedHom(i).isIso()) retval = false;
if (! (getDualToMixedHom(1).inverseHom()*getStandardToMixedHom(1)*getH1CellAp()).isIdentity() ) retval = false;
return retval;
}


bool NHomologicalData::findSpin() {
	bool retval=true;
	// todo!

	return retval;
}



} // namespace regina



