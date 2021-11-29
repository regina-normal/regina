
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

// We use M_PI from <cmath>, which seems to cause problems under windows.
// The fix seems to be to include <cmath> before anything else, *and*
// to set _USE_MATH_DEFINES beforehand.
#if defined(_WIN32)
  #define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "maths/matrixops.h"
#include "maths/primes.h"
#include "triangulation/homologicaldata.h"
#include <list>
#include <iostream>
#include <sstream>

namespace regina {


void HomologicalData::swap(HomologicalData& other) noexcept {
    // OMFGBBQ.
    tri_.swap(other.tri_);
    mHomology0_.swap(other.mHomology0_);
    mHomology1_.swap(other.mHomology1_);
    mHomology2_.swap(other.mHomology2_);
    mHomology3_.swap(other.mHomology3_);
    bHomology0_.swap(other.bHomology0_);
    bHomology1_.swap(other.bHomology1_);
    bHomology2_.swap(other.bHomology2_);
    bmMap0_.swap(other.bmMap0_);
    bmMap1_.swap(other.bmMap1_);
    bmMap2_.swap(other.bmMap2_);
    dmHomology0_.swap(other.dmHomology0_);
    dmHomology1_.swap(other.dmHomology1_);
    dmHomology2_.swap(other.dmHomology2_);
    dmHomology3_.swap(other.dmHomology3_);
    dmTomMap1_.swap(other.dmTomMap1_);

    std::swap(ccIndexingComputed_, other.ccIndexingComputed_);
    std::swap_ranges(numStandardCells, numStandardCells + 4,
        other.numStandardCells);
    std::swap_ranges(numDualCells, numDualCells + 4, other.numDualCells);
    std::swap_ranges(numBdryCells, numBdryCells + 3, other.numBdryCells);
    sNIV.swap(other.sNIV);
    sIEOE.swap(other.sIEOE);
    sIEEOF.swap(other.sIEEOF);
    sIEFOT.swap(other.sIEFOT);
    dNINBV.swap(other.dNINBV);
    dNBE.swap(other.dNBE);
    dNBF.swap(other.dNBF);
    sBNIV.swap(other.sBNIV);
    sBNIE.swap(other.sBNIE);
    sBNIF.swap(other.sBNIF);

    std::swap(chainComplexesComputed, other.chainComplexesComputed);
    A0_.swap(other.A0_);
    A1_.swap(other.A1_);
    A2_.swap(other.A2_);
    A3_.swap(other.A3_);
    A4_.swap(other.A4_);
    B0_.swap(other.B0_);
    B1_.swap(other.B1_);
    B2_.swap(other.B2_);
    B3_.swap(other.B3_);
    B4_.swap(other.B4_);
    Bd0_.swap(other.Bd0_);
    Bd1_.swap(other.Bd1_);
    Bd2_.swap(other.Bd2_);
    Bd3_.swap(other.Bd3_);
    B0Incl_.swap(other.B0Incl_);
    B1Incl_.swap(other.B1Incl_);
    B2Incl_.swap(other.B2Incl_);
    H1map_.swap(other.H1map_);

    std::swap(torsionFormComputed, other.torsionFormComputed);
    h1PrimePowerDecomp.swap(other.h1PrimePowerDecomp);
    linkingFormPD.swap(other.linkingFormPD);
    std::swap(torsionLinkingFormIsHyperbolic,
        other.torsionLinkingFormIsHyperbolic);
    std::swap(torsionLinkingFormIsSplit, other.torsionLinkingFormIsSplit);
    std::swap(torsionLinkingFormSatisfiesKKtwoTorCondition,
        other.torsionLinkingFormSatisfiesKKtwoTorCondition);
    torRankV.swap(other.torRankV);
    twoTorSigmaV.swap(other.twoTorSigmaV);
    oddTorLegSymV.swap(other.oddTorLegSymV);
    torsionRankString.swap(other.torsionRankString);
    torsionSigmaString.swap(other.torsionSigmaString);
    torsionLegendreString.swap(other.torsionLegendreString);
    embeddabilityString.swap(other.embeddabilityString);
}

void HomologicalData::writeTextShort(std::ostream& out) const {
    if (mHomology0_) {
        out<<"H_0(M) = ";
        mHomology0_->writeTextShort(out);
        out<<" ";
    }
    if (mHomology1_) {
        out<<"H_1(M) = ";
        mHomology1_->writeTextShort(out);
        out<<" ";
    }
    if (mHomology2_) {
        out<<"H_2(M) = ";
        mHomology2_->writeTextShort(out);
        out<<" ";
    }
    if (mHomology3_) {
        out<<"H_3(M) = ";
        mHomology3_->writeTextShort(out);
        out<<" ";
    }

    if (bHomology0_) {
        out<<"H_0(BM) = ";
        bHomology0_->writeTextShort(out);
        out<<" ";
    }
    if (bHomology1_) {
        out<<"H_1(BM) = ";
        bHomology1_->writeTextShort(out);
        out<<" ";
    }
    if (bHomology2_) {
        out<<"H_2(BM) = ";
        bHomology2_->writeTextShort(out);
        out<<" ";
    }

    if (bmMap0_) {
        out<<"H_0(BM) --> H_0(M) = ";
        bmMap0_->writeTextShort(out);
        out<<" ";
    }
    if (bmMap1_) {
        out<<"H_1(BM) --> H_1(M) = ";
        bmMap1_->writeTextShort(out);
        out<<" ";
    }
    if (bmMap2_) {
        out<<"H_2(BM) --> H_2(M) = ";
        bmMap2_->writeTextShort(out);
        out<<" ";
    }

    if (dmTomMap1_) {
        out<<"PD map = ";
        dmTomMap1_->writeTextShort(out);
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

void HomologicalData::computeccIndexing() {
    // Only do this if we haven't already done it.
    if (ccIndexingComputed_)
        return;

    // Dereference the SnapshotRef just once.
    const Triangulation<3>& tri(*tri_);

    // Off we go...

    unsigned long i=0;
    for (Vertex<3>* v : tri.vertices()) {
        if (!(v->isIdeal()))
            sNIV.push_back(i);
        i++;
    } // sNIV

    unsigned long j=0;
    for (Edge<3>* e : tri.edges()) {
        for (i=0;i<2;i++) {
            if (e->vertex(i)->isIdeal())
                sIEOE.push_back(2*j+i);
        }
        j++;
    }

    j=0; // sIEOE
    for (Triangle<3>* t : tri.triangles()) {
        for (i=0;i<3;i++) {
            if (t->vertex(i)->isIdeal())
                sIEEOF.push_back(3*j+i);
        }
        j++;
    }

    j=0; // sIEEOF
    for (Tetrahedron<3>* tet : tri.tetrahedra()) {
        for (i=0;i<4;i++)  {
            if (tet->vertex(i)->isIdeal()) {
                sIEFOT.push_back(4*j+i);
            }
        }
        j++;
    }

    j=0;// sIEFOT
    for (Vertex<3>* v : tri.vertices()) { // dNINBV
        if ((!(v->isIdeal())) && (!(v->isBoundary())))
            dNINBV.push_back(j);
        j++;
    }
    j=0;
    for (Edge<3>* e : tri.edges()) {
        if (!(e->isBoundary()))
            dNBE.push_back(j);
        j++;
    }
    j=0; // dNBE
    for (Triangle<3>* t : tri.triangles()) {
        if (!(t->isBoundary()))
            dNBF.push_back(j);
        j++;
    }

    i=0;
    for (Vertex<3>* v : tri.vertices()) { // sBNIV
        if ( (!(v->isIdeal())) && (v->isBoundary()))
            sBNIV.push_back(i);
        i++;
    }
    i=0;
    for (Edge<3>* e : tri.edges()) { // sBNIE
        if (e->isBoundary())
            sBNIE.push_back(i);
        i++;
    }
    i=0;
    for (Triangle<3>* t : tri.triangles()) { // sBNIF
        if (t->isBoundary())
            sBNIF.push_back(i);
        i++;
    }

    ccIndexingComputed_ = true;

    // standard (0..3)-cells:
    numStandardCells[0] = sNIV.size() + sIEOE.size();
    numStandardCells[1] = tri.countEdges() + sIEEOF.size();
    numStandardCells[2] = tri.countTriangles() + sIEFOT.size();
    numStandardCells[3] = tri.size();

    // dual (0..3)-cells:
    numDualCells[0] = tri.size();
    numDualCells[1] = dNBF.size();
    numDualCells[2] = dNBE.size();
    numDualCells[3] = dNINBV.size();

    // boundary (0..2)-cells:
    numBdryCells[0] = sBNIV.size() + sIEOE.size();
    numBdryCells[1] = sBNIE.size() + sIEEOF.size();
    numBdryCells[2] = sBNIF.size() + sIEFOT.size();
}

void HomologicalData::computeChainComplexes() {
    // Only do this if we haven't already done it.
    if (chainComplexesComputed)
        return;

    // Dereference the SnapshotRef just once.
    const Triangulation<3>& tri(*tri_);

    // Off we go...

    if (!ccIndexingComputed_) computeccIndexing();

    chainComplexesComputed = true;

    // need to convert this so that it does not use tri
    B0_ = MatrixInt(1, numDualCells[0]);
    B1_ = MatrixInt(numDualCells[0], numDualCells[1]);
    B2_ = MatrixInt(numDualCells[1], numDualCells[2]);
    B3_ = MatrixInt(numDualCells[2], numDualCells[3]);
    B4_ = MatrixInt(numDualCells[3], 1);

    A0_ = MatrixInt(1, numStandardCells[0]);
    A1_ = MatrixInt(numStandardCells[0], numStandardCells[1]);
    A2_ = MatrixInt(numStandardCells[1], numStandardCells[2]);
    A3_ = MatrixInt(numStandardCells[2], numStandardCells[3]);
    A4_ = MatrixInt(numStandardCells[3], 1);

    H1map_ = MatrixInt(numStandardCells[1], numDualCells[1]);

    Bd0_ = MatrixInt(1, numBdryCells[0]);
    Bd1_ = MatrixInt(numBdryCells[0], numBdryCells[1]);
    Bd2_ = MatrixInt(numBdryCells[1], numBdryCells[2]);
    Bd3_ = MatrixInt(numBdryCells[2], 1);

    B0Incl_ = MatrixInt(numStandardCells[0], numBdryCells[0]);
    B1Incl_ = MatrixInt(numStandardCells[1], numBdryCells[1]);
    B2Incl_ = MatrixInt(numStandardCells[2], numBdryCells[2]);

    long int temp;
    unsigned long i,j;

    Perm<4> p1;

    // This fills out matrix A1
    for (i=0;i<tri.countEdges();i++) {
        // these are the standard edges
        temp=sNIV.index(tri.edge(i)->vertex(0)->index());
        (A1_->entry( ((temp==(-1)) ?
            (sNIV.size()+sIEOE.index(2*i)) : temp ), i))-=1;
        temp=sNIV.index(tri.edge(i)->vertex(1)->index());
        (A1_->entry( ((temp==(-1)) ?
            (sNIV.size()+sIEOE.index(2*i+1)) : temp), i))+=1;
    } // ok

    for (i=0;i<sIEEOF.size();i++) { // these are the ideal edges...
        // sIEEOF[i] /3 is the triangle index, and sIEEOF[i] % 3 tells us
        // the vertex of this triangle
        p1=tri.triangle(sIEEOF[i]/3)->edgeMapping( (sIEEOF[i] + 1) % 3);
        if (p1.sign()==1) {
            A1_->entry(sNIV.size() + sIEOE.index(2*(
                tri.triangle(sIEEOF[i]/3)->
                edge(p1[2])->index() )+1), tri.countEdges()+i)-=1;
        } else {
            A1_->entry(sNIV.size() + sIEOE.index(2*(
                tri.triangle(sIEEOF[i]/3)->
                edge(p1[2])->index() )) , tri.countEdges()+i)-=1;
        }
        p1=tri.triangle(sIEEOF[i]/3)->edgeMapping( (sIEEOF[i] + 2) % 3);
        if (p1.sign()==1) {
            A1_->entry(sNIV.size() + sIEOE.index(2*(
                tri.triangle(sIEEOF[i]/3)->
                edge(p1[2])->index() )) , tri.countEdges()+i)+=1;
        } else {
            A1_->entry(sNIV.size() + sIEOE.index(2*(
                tri.triangle(sIEEOF[i]/3)->
                edge(p1[2])->index() )+1) , tri.countEdges()+i)+=1;
        }
    }
    // that handles matrix A1.

    // start filling out A2...
    for (i=0;i<tri.countTriangles();i++) {
        // put boundary edges into A2..
        for (j=0;j<6;j++) {
            // run through the 6 possible boundary edges of the triangle
            // the first 3 are standard, the last three are the ideal
            // edges (if they exist)
            if ( (j/3) == 0) {
                p1=tri.triangle(i)->edgeMapping(j % 3);
                A2_->entry(
                    tri.triangle(i)->edge(j % 3)->index() ,i) +=
                    ( (p1.sign()==1) ? +1 : -1 );
            } else {
                // check triangle i vertex j % 3 is ideal
                if (tri.triangle(i)->vertex(j % 3)->isIdeal())
                    A2_->entry( tri.countEdges() +
                        sIEEOF.index((3*i) + (j % 3)), i) += 1;
            }
        }
    }

    for (i=0;i<sIEFOT.size();i++) {
        // boundary edges from ideal faces of tetrahedra.
        // sIEFOT[i] /4 is the tetrahedron number
        // sIEFOT[i] % 4 is the vertex number for this tetrahedron
        // tetrahedra[ sIEFOT[i]/4 ].triangle(sIEFOT[i] + 1,2,3 % 4)
        // are the respective faces
        // tetrahedra[ sIEFOT[i]/4 ].triangleMapping(sIEFOT[i] + 1,2,3 % 4)
        // gives the perm
        // triangles().index( tetrahedra[sIEFOT[i]/4].triangle(
        // sIEFOT[i] + 1,2,3 % 4) is therefore the triangle number, and
        // tetrahedra[ sIEFOT[i]/4 ].triangleMapping(
        // sIEFOT[i] + 1,2,3 % 4)^{-1} applied to sIEFOT[i] % 4 is the
        // vertex of this triangle.
        for (j=1;j<4;j++) {
            p1=tri.tetrahedron( sIEFOT[i]/4 )->triangleMapping(
                (sIEFOT[i] + j) % 4);
            A2_->entry( tri.countEdges() + sIEEOF.index(
                3*tri.tetrahedron(
                sIEFOT[i]/4 )->triangle( (sIEFOT[i] + j) % 4)->index() +
                p1.pre(sIEFOT[i] % 4) ) ,
                tri.countTriangles()+i ) += ( (p1.sign()==1 ? -1 : 1 ) );
        }
    }
    // end A2

    // start A3
    for (i=0;i<tri.size();i++) {
        for (j=0;j<4;j++) {
            // first go through standard faces 0 through 3
            p1=tri.tetrahedron(i)->triangleMapping(j);
            A3_->entry( tri.tetrahedron(i)->triangle(j)->index(), i) +=
                ( (p1.sign()==1) ? 1 : -1 );
            // then ideal faces 0 through 3, if they exist
            if (tri.tetrahedron(i)->vertex(j)->isIdeal()==1) {
                // this part is in error.
                A3_->entry( tri.countTriangles() +
                    sIEFOT.index((4*i) + j), i) += 1;
            }
        }
    }
    // end A3


    // start B1: for each dual edge == non-boundary triangle,
    //              find the tetrahedra that bound it
    for (i=0;i<dNBF.size();i++) {
        B1_->entry(
            tri.triangle(dNBF[i])->embedding(1).tetrahedron()->index(), i)+=1;
        B1_->entry(
            tri.triangle(dNBF[i])->embedding(0).tetrahedron()->index(), i)-=1;
    }
    // end B1

    // start B2: for each dual triangle == non-boundary edge,
    // find dual edges it bounds == link of tetrahedra that contain it
    for (i=0;i<dNBE.size();i++) {
        for (auto& emb : *tri.edge(dNBE[i])) {
            p1=emb.vertices();
            // the face of the tetrahedron corresponding to vertex 2 is
            // what we want to orient... but we need to decide on its
            // orientation.  For that we check to see if this face's
            // embedding(0).tetrahedron() is the current tet, and
            // embedding(0).triangle() is this current face p1[2]...

            B2_->entry( dNBF.index(
                emb.tetrahedron()->triangle(p1[2])->index() ) ,i)+=
                    ( ( emb.tetrahedron() ==
                        emb.tetrahedron()->triangle(
                            p1[2] )->embedding( 0 ).tetrahedron() &&
                        emb.tetrahedron()->triangle(
                            p1[2] )->embedding( 0 ).triangle() == p1[2] )
                              ? 1 : -1);
        }
    }
    // end B2

    std::vector<int> tetor;
    long int ind1;
    long int ind2;
    int k;

    // start B3: for each dual tetrahedron==nonboundary vertex,
    //           find the corresp edges==non-boundary boundary triangles

    for (i=0;i<dNINBV.size();i++) {
        // dNINBV[i] is the vertices.index() of this vertex.
        Vertex<3>* vtet = tri.vertex(dNINBV[i]);
        tetor.resize(vtet->degree(),0);

        std::vector<std::pair<long, bool> > unorientedlist;
        // This should be the list of unoriented tetrahedra, together
        // with marked vertices.
        // Indices into the vector are 4*tetindex + vertex no.
        // Values are (index into vtet's list of embeddings, already oriented).
        unorientedlist.resize(4 * tri.size());

        for (j=0;j<vtet->degree();j++) {
            // unoriented list stores the tetrahedra adjacent to the vertex
            // plus the vertex index in that tetrahedra's coords
            unorientedlist[
                4*vtet->embedding(j).tetrahedron()->index() +
                vtet->embedding(j).vertex() ] = std::make_pair(j, false);
        }

        // need to set up a local orientation for the tangent
        // bundle at the vertex so that we can compare with the
        // normal orientations of the edges incident. This normal
        // orientation will have the form of a sign +-1 for each
        // VertexEmbedding<3> in vtet's embedding list. Our orientation convention
        // will be chosen so that vtet->front() is positively oriented,
        // ie: tetor[0]==1 always.

        tetor[0]=1;
        unorientedlist[ 4*vtet->front().tetrahedron()->index() +
            vtet->front().vertex() ].second = true;

        size_t stillToOrient = vtet->degree() - 1;
        while (stillToOrient > 0)
          for (j=0;j<vtet->degree();j++)
            // go through all oriented tetrahedra and orient
            // the adjacent tetrahedra
            {
                ind1 = 4*vtet->embedding(j).tetrahedron()->index() +
                    vtet->embedding(j).vertex();

                if ( unorientedlist[ ind1 ].second ) {
                    // this tetrahedron has been oriented check to see
                    // if any of the adjacent
                    // tetrahedra are unoriented, and if so, orient them.
                    for (k=0;k<4;k++) {
                        if (k!= (ind1 % 4))
                        {
                            p1=vtet->embedding(j).tetrahedron() ->
                                adjacentGluing(k);
                            ind2=4*vtet->embedding(j).tetrahedron() ->
                                adjacentTetrahedron(k)->index() + p1[ind1 % 4];
                            if (! unorientedlist[ ind2 ].second )
                            {
                                // we have an adjacent unoriented tetrahedron.
                                // we orient it.
                                tetor[ unorientedlist[ind2].first ] =
                                   (-1)*tetor[j]*p1.sign();
                                unorientedlist[ ind2 ].second = true;
                                --stillToOrient;
                            }
                        }
                    }
                }
            }

        // now a local orientation is set up and can compute the boundary.
        // to do this, it seems best to compile a list of incident edges
        // which contains their endpoint data and sign.
        // the list will be a std::set<long> edge_adjacency,
        // data will be stored as
        // 4*(edge index) + 2*(endpt index) + sign stored as 0 or 1.
        std::set<long> edge_adjacency;

        for (j=0;j<vtet->degree();j++)
            for (k=0;k<6;k++) {
                ind2=vtet->embedding(j).tetrahedron()->edgeMapping(k).
                    pre( vtet->embedding(j).vertex() );
                if ( ind2<2 ) {
                    // edge k of tetrahedron j, moreover we know that
                    // the vertex of the edge corresponds to ind2
                    //
                    // the corresp orientation coming from our local
                    // orientation
                    // plus orienting the edge out of vertex k % 2...

                    p1=vtet->embedding(j).tetrahedron()->edgeMapping(k);

                    if ( ind2 == 1 ) p1=p1*(Perm<4>(0,1));
                    // now p1 sends 0 to point corresp to v, 1 to point
                    // corresp to end of edge.
                    // if p1.sign() == tetor[j] then sign = +1 otherwise -1.

                    ind1=4*vtet->embedding(j).tetrahedron()->edge(k)->index()
                        + 2*ind2 + (p1.sign() == tetor[j] ? 1 : 0);

                    // Insertion in std::set is harmless if the key
                    // already exists.
                    edge_adjacency.insert(ind1);
                }
            }

        std::set<long>::const_iterator it;
        for (it = edge_adjacency.begin(); it != edge_adjacency.end(); ++it) {
            B3_->entry( dNBE.index((*it)/4) , i) +=
                ( ( ((*it) % 2)==0 ) ? 1 : -1 );
        }
    }
    // end B3

    // proceed to fill out H1map
    // the algorithm will proceed in 2 steps.

    // step 1) fix once and for all a map from dual 0-cells to regular
    // 0-cells, the only condition this map needs to satisfy is that the
    // regular 0-cell associated to a dual 0-cell must be contained in
    // the same ideal simplex.

    std::vector<unsigned long> zeroCellMap(tri.size());
    // zeroCellMap[i] describes the vertex of tetrahedra[i] that the dual
    // 0-cell is sent to. It will be stored as
    // 4*(vertex number 0,1,2,3) + 0,1,2,3 (equal to prev. number if
    // non-ideal

    for (i=0; i<zeroCellMap.size(); i++) {
        // cycle through the vertices, take the first non-ideal one if
        // it exists.
        j=0;
        while ( j<4 && tri.tetrahedron(i)->vertex(j)->isIdeal())
            j++;
        if (j<4) zeroCellMap[i]=4*j+j;
        else zeroCellMap[i]=1;
    }

    // step 2) fill out the matrix. each dual 1-cell corresponds to a
    // triangular face of the ideal triangulation. the map of 0-cells has
    // already been chosen so for the map of 1-cells simply choose any path
    // from the first 0-cell to the 2nd 0-cell with the condition that
    // the path stays inside the two ideal simplicies and only crosses
    // the triangle corresponding to the dual 1-cell once. (and no other
    // triangles).

    for (j=0; j<H1map_->columns(); j++) // H1map_.columns()==dNBF.size() 
        // while H1map_.rows() is edges.size()+sIEEOF.size()
    {
        // now we have to decide where dual edge j == ideal triangulation
        // triangle j is sent.

        unsigned tet0TriIndex = tri.triangle(dNBF[j])->embedding(0).triangle(); 
        unsigned tet1TriIndex = tri.triangle(dNBF[j])->embedding(1).triangle(); 

        unsigned vert0Num = zeroCellMap[
            tri.triangle(dNBF[j]) -> embedding(0).tetrahedron()->index()]/4;
            // vertex number of start vertex in tet0
        unsigned vert1Num = zeroCellMap[
            tri.triangle(dNBF[j]) -> embedding(1).tetrahedron()->index()]/4;
            // vertex number of end vertex in tet1.
        unsigned vert0id = zeroCellMap[
            tri.triangle(dNBF[j]) -> embedding(0).tetrahedron()->index()]%4;
            // not equal to vert0Num if and only if vert0 is ideal.
        unsigned vert1id = zeroCellMap[
            tri.triangle(dNBF[j]) -> embedding(1).tetrahedron()->index()]%4;
            // not equal to vert1Num if and only if vert1 is ideal.
        Perm<4> P1 = tri.triangle(dNBF[j])->embedding(0).vertices();
        Perm<4> P2 = tri.triangle(dNBF[j])->embedding(1).vertices();
        Perm<4> P3;
        // the permutation from the start simplex vertices
        // to the end simplex.

        bool stage0nec = false;
        unsigned long stage0edgeNum = 0;
        bool stage0posOr = false;
        unsigned stage0choice = 0; // this indicates the vertex of the simplex
        // that our chosen edge
        // with its induced orientation ends...

        if (vert0Num == tet0TriIndex)  // stage 0
        {
            stage0nec = true;

            if (vert0Num == vert0id) {
                stage0choice = (tet0TriIndex + 1) % 4;
            } // not ideal
            else {
                stage0choice = vert0id;
            } // ideal

            stage0edgeNum = tri.triangle(dNBF[j]) ->
                embedding(0).tetrahedron() ->
                edge( Edge<3>::edgeNumber[vert0Num][stage0choice] )->index();
            stage0posOr = ( static_cast<unsigned>(tri.triangle(dNBF[j]) ->
                embedding(0).tetrahedron()->edgeMapping(
                Edge<3>::edgeNumber[vert0Num][stage0choice])[1]) == stage0choice) ?
                true : false ;
        }


        bool stage4nec = false; // stage 4
        unsigned long stage4edgeNum = 0;
        bool stage4posOr = false;
        unsigned stage4choice = 0;

        if (vert1Num == tet1TriIndex) {
            stage4nec = true;

            if (vert1Num == vert1id) // the non-ideal case.
            {
                stage4choice = (tet1TriIndex + 1) % 4;
            } // duh, this is all wrong.
            else {
                stage4choice = vert1id;
            }

            stage4edgeNum = tri.triangle(dNBF[j]) ->
                embedding(1).tetrahedron() ->
                edge( Edge<3>::edgeNumber[vert1Num][stage4choice] )->index();
            stage4posOr = ( static_cast<unsigned>(tri.triangle(dNBF[j]) ->
                embedding(1).tetrahedron()->edgeMapping(
                Edge<3>::edgeNumber[vert1Num][stage4choice])[1]) == vert1Num ) ?
                true : false ;
        }

        // decide if stages 1 and 3 are neccessary...
        bool stage1nec = false;         // stage 1
        unsigned stage1v = 0;
        unsigned stage1vi = 0;
        unsigned long stage1edgeNum = 0;
        bool stage1posOr = false;
        unsigned stage1TriToUse = 0;

        if (stage0nec && tri.triangle(dNBF[j]) ->
                embedding(0).tetrahedron() ->
                vertex(stage0choice)->isIdeal() ) {
            stage1v = stage0choice;
            stage1vi = vert0Num;
            stage1nec=true;
        } else
            if ((!stage0nec) && (vert0Num != vert0id) &&
                    (vert0id == tet0TriIndex)) {
                stage1v = vert0Num;
                stage1vi = vert0id;
                stage1nec = true;
            }
        if (stage1nec) { // we need to decide which triangle to use...
            stage1TriToUse = tri.triangle(dNBF[j]) ->
                embedding(0).tetrahedron()->edgeMapping(
                Edge<3>::edgeNumber[stage1v][tet0TriIndex] )[2];
            P3 = tri.triangle(dNBF[j])->embedding(0).tetrahedron()->
                triangleMapping(stage1TriToUse);
            stage1edgeNum = tri.countEdges() + sIEEOF.index(
                3*tri.triangle(dNBF[j]) ->
                embedding(0).tetrahedron() ->
                triangle(stage1TriToUse)->index() + P3.pre(stage1v) );
            stage1posOr = ( ( static_cast<unsigned>(
                P3[(P3.pre(stage1v)+1) % 3]) != stage1vi ) ? true : false );
        }
        bool stage3nec = false;
        unsigned stage3v = 0;
        unsigned stage3vi = 0;
        unsigned long stage3edgeNum = 0;
        bool stage3posOr = false;
        unsigned stage3TriToUse = 0;

        if (stage4nec && tri.triangle(dNBF[j]) ->
                embedding(1).tetrahedron() ->
                vertex(stage4choice)->isIdeal() ) { // ideal case
            stage3v = stage4choice;
            stage3vi = vert1Num;
            stage3nec=true;
        } else
            if ((!stage4nec) && (vert1Num != vert1id) &&
                    (vert1id == tet1TriIndex)) { // non-ideal case
                stage3v = vert1Num;
                stage3vi = vert1id;
                stage3nec = true;
            }
        if (stage3nec) { // we need to decide which triangle to use...
            stage3TriToUse = tri.triangle(dNBF[j]) ->
                embedding(1).tetrahedron()->edgeMapping(
                Edge<3>::edgeNumber[stage3v][tet1TriIndex] )[2];
            P3 = tri.triangle(dNBF[j])->embedding(1).tetrahedron()->
                triangleMapping(stage3TriToUse);
            stage3edgeNum = tri.countEdges() + sIEEOF.index(
                3*tri.triangle(dNBF[j]) ->
                embedding(1).tetrahedron() ->
                triangle(stage3TriToUse)->index() + P3.pre(stage3v) );
            stage3posOr = ( ( static_cast<unsigned>(
                P3[(P3.pre(stage3v)+1) % 3]) == stage3vi ) ? true : false );
        }

        unsigned stage2startdata = 0;
        unsigned stage2enddata = 0;
        // 3*vertex number(0,1,2) + another vertex number (0,1,2)
        // these are the same indicates the vertex is non-ideal
        // these are different indicates the vertex is ideal and dir
        // of relevant point..

        if (stage1nec) // set up stage2startdata
        {
            stage2startdata = 3*P1.pre( stage1v ) +
                P1.pre((tri.triangle(dNBF[j]) ->
                embedding(0).tetrahedron() ->
                edgeMapping( Edge<3>::edgeNumber[stage1v][stage1vi] ))[3] );
        } else {
            // we have to deal with 2 possibilities a) stage 0 was called
            // and it jumped here, so it is not an ideal vertex.
            // b) neither stage 0 or 1 was called and this may or may
            // not be an ideal vertex
            if (stage0nec) { // this is the non-ideal situation
                stage2startdata = 3*P1.pre( stage0choice ) +
                        ((P1.pre( stage0choice )+1) % 3);
            } else {
                // this is the starting point... back to using vert0 info...
                if (vert0Num != vert0id)
                    stage2startdata = 3*P1.pre( vert0Num ) + P1.pre( vert0id );
                else
                    stage2startdata = 3*P1.pre( vert0Num ) +
                        ((P1.pre( vert0Num ) + 1) % 3);
            }
        }

        if (stage3nec) // set up stage2enddata
        {
            stage2enddata = 3*P2.pre( stage3v ) +
                P2.pre((tri.triangle(dNBF[j]) ->
                embedding(1).tetrahedron() ->
                edgeMapping( Edge<3>::edgeNumber[stage3v][stage3vi] ))[3] );
        } else {
            if (stage4nec) { // this is the non-ideal situation
                stage2enddata = 3*P2.pre( stage4choice ) +
                        ((P2.pre( stage4choice ) + 1) % 3);
            } else {
                // this is the starting point... back to using vert1 info...
                if (vert1Num != vert1id)
                    stage2enddata = 3*P2.pre( vert1Num ) + P2.pre( vert1id );
                else
                    stage2enddata = 3*P2.pre( vert1Num ) +
                        ((P2.pre( vert1Num ) + 1) % 3);
            }
        }
        // now cycle through pairs of adjacent vertices on the triangle
        // and check to see if the corresponding edge is required...

        unsigned currV = stage2startdata;
        unsigned prevV = stage2startdata;
        if (stage2startdata != stage2enddata)
            while (currV != stage2enddata) {
                // first, increment currV -- this is a number from the
                // set { 1, 2, 3, 5, 6, 7 } describing an ideal vertex
                // of the triangle is triadic 3*vert num + direction...
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
                if (( currV/3  == prevV/3 ) && (tri.triangle(dNBF[j])->
                        vertex(currV/3)->isIdeal()) )  // ideal edge
                {
                    H1map_->entry( tri.countEdges() +
                        sIEEOF.index(3*dNBF[j] + (currV/3)) , j ) += 1;
                }
                if ( currV/3  != prevV/3 ) // regular edge
                {
                    H1map_->entry(tri.triangle(dNBF[j])->edge(((currV/3) + 1) % 3 )->index(), j) 
                      += ( 
                           ( 
                             static_cast<unsigned>( 
                              tri.triangle(dNBF[j])->edgeMapping(((currV/3) + 1) % 3)[1]
                                                  ) == currV/3
                           ) 
                         ? +1 : -1 );
                }
                // move prevV to be equal to currV.
                prevV = currV;
            }
        // now we fill out the matrix.
        if (stage0nec)
            H1map_->entry( stage0edgeNum, j ) += ( stage0posOr ? 1 : -1 );
        if (stage1nec)
            H1map_->entry( stage1edgeNum, j ) += ( stage1posOr ? 1 : -1 );

        if (stage3nec)
            H1map_->entry( stage3edgeNum, j ) += ( stage3posOr ? 1 : -1 );
        if (stage4nec)
            H1map_->entry( stage4edgeNum, j ) += ( stage4posOr ? 1 : -1 );

    }

    // This fills out matrix Bd1: rows==sBNIV.size()+sIEOE.size(),
    // cols==sBNIE.size()+sIEEOF.size()
    for (i=0;i<sBNIE.size();i++) { // these are the standard boundary edges
        // temp == -1 when the boundary edge end is ideal.
        temp=sBNIV.index(tri.edge(sBNIE[i])->vertex(0)->index());
        (Bd1_->entry( ((temp==(-1)) ? (sBNIV.size()+2*i) : temp ), i))-=1;
        temp=sBNIV.index(tri.edge(sBNIE[i])->vertex(1)->index());
        (Bd1_->entry( ((temp==(-1)) ? (sBNIV.size()+2*i+1) : temp), i))+=1;
    } // ok

    for (i=0;i<sIEEOF.size();i++) { // these are the ideal edges...
        // sIEEOF[i] /3 is the triangle index, and sIEEOF[i] % 3 tells us
        // the vertex of this triangle
        p1=tri.triangle(sIEEOF[i]/3)->edgeMapping( (sIEEOF[i] + 1) % 3);
        if (p1.sign()==1) {
            Bd1_->entry(sBNIV.size() + sIEOE.index(2*(
                tri.triangle(sIEEOF[i]/3)->
                edge(p1[2])->index() )+1), sBNIE.size()+i)-=1;
        } else {
            Bd1_->entry(sBNIV.size() + sIEOE.index(2*(
                tri.triangle(sIEEOF[i]/3)->
                edge(p1[2])->index() )) , sBNIE.size()+i)-=1;
        }
        p1=tri.triangle(sIEEOF[i]/3)->edgeMapping( (sIEEOF[i] + 2) % 3);
        if (p1.sign()==1) {
            Bd1_->entry(sBNIV.size() + sIEOE.index(2*(
                tri.triangle(sIEEOF[i]/3)->
                edge(p1[2])->index() )) , sBNIE.size()+i)+=1;
        } else {
            Bd1_->entry(sBNIV.size() + sIEOE.index(2*(
                tri.triangle(sIEEOF[i]/3)->
                edge(p1[2])->index() )+1) , sBNIE.size()+i)+=1;
        }
    }
    // that handles matrix Bd1.

    // start filling out Bd2: rows==sBNIE.size()+sIEEOF.size(),
    // cols==sBNIF.size()+sIEFOT.size()
    for (i=0;i<sBNIF.size();i++) // boundary non-ideal triangles...
    { // put boundary edges into Bd2..
        for (j=0;j<6;j++) {
            // run through the 6 possible boundary edges of the triangle
            // the first 3 are standard, the last three are the ideal
            // edges (if they exist)
            if ( (j/3) == 0) {
                p1=tri.triangle(sBNIF[i])->edgeMapping(j % 3);
                Bd2_->entry( sBNIE.index( tri.triangle(
                    sBNIF[i])->edge(j % 3)->index() ) ,i) +=
                    ( (p1.sign()==1) ? +1 : -1 );
            } else {
                // check triangle i vertex j % 3 is ideal
                if (tri.triangle(sBNIF[i])->vertex(j % 3)->isIdeal())
                    Bd2_->entry( sBNIF.size() + sIEEOF.index(
                        (3*i) + (j % 3)), i) += 1;
            }
        }
    }

    for (i=0;i<sIEFOT.size();i++) // ideal triangles...
    { // boundary edges from ideal faces of tetrahedra.
        // sIEFOT[i] /4 is the tetrahedron number
        // sIEFOT[i] % 4 is the vertex number for this tetrahedron
        // tetrahedra[ sIEFOT[i]/4 ].triangle(sIEFOT[i] + 1,2,3 % 4) are
        // the respective faces
        // tetrahedra[ sIEFOT[i]/4 ].triangleMapping(sIEFOT[i] + 1,2,3 % 4)
        // gives the perm
        // triangles().index( tetrahedra[sIEFOT[i]/4].triangle(
        //     sIEFOT[i] + 1,2,3 % 4) is therefore the
        // triangle number, and tetrahedra[ sIEFOT[i]/4 ].triangleMapping(
        //     sIEFOT[i] + 1,2,3 % 4)^{-1}
        // applied to sIEFOT[i] % 4 is the vertex of this triangle.
        for (j=1;j<4;j++) {
            p1=tri.tetrahedron( sIEFOT[i]/4 )->triangleMapping(
                (sIEFOT[i] + j) % 4);
            Bd2_->entry( sBNIE.size() + sIEEOF.index(3*
                tri.tetrahedron(sIEFOT[i]/4 )->triangle(
                (sIEFOT[i] + j) % 4)->index() + p1.pre(sIEFOT[i] % 4) ) ,
                sBNIF.size()+i ) += ( (p1.sign()==1 ? -1 : 1 ) );
        }
    }
    // end Bd2

    // fill out b0Incl
    // boundary 0-cells:
    for (i=0;i<B0Incl_->columns();i++)
        B0Incl_->entry( ( ( i < sBNIV.size()) ? sNIV.index(sBNIV[i]) :
                sNIV.size() + i - sBNIV.size() ) ,i)+=1;
    // fill out b1Incl
    for (i=0;i<B1Incl_->columns();i++)
        // each boundary edge corresponds to a triangulation edge
        B1Incl_->entry( ( ( i < sBNIE.size() ) ? sBNIE[i] :
                tri.countEdges() + i - sBNIE.size() ) ,i)+=1;
    // fill out b2Incl
    for (i=0;i<B2Incl_->columns();i++)
        B2Incl_->entry( ( ( i < sBNIF.size() ) ? sBNIF[i] :
                tri.countTriangles() + i - sBNIF.size() ) ,i)+=1;
}

const MarkedAbelianGroup& HomologicalData::homology(unsigned q) {
    if (q==0) {
        if (!mHomology0_) {
            computeChainComplexes();
            mHomology0_ = MarkedAbelianGroup(*A0_,*A1_);
        }
        return *mHomology0_;
    } else if (q==1) {
        if (!mHomology1_) {
            computeChainComplexes();
            mHomology1_ = MarkedAbelianGroup(*A1_,*A2_);
        }
        return *mHomology1_;
    } else if (q==2) {
        if (!mHomology2_) {
            computeChainComplexes();
            mHomology2_ = MarkedAbelianGroup(*A2_,*A3_);
        }
        return *mHomology2_;
    } else {
        // Assume q == 3.  This will at least avoid a crash if q lies
        // outside the required range.
        if (!mHomology3_) {
            computeChainComplexes();
            mHomology3_ = MarkedAbelianGroup(*A3_,*A4_);
        }
        return *mHomology3_;
    }
    // the A's should probably be redone as an array of pointers...
}

const MarkedAbelianGroup& HomologicalData::bdryHomology(unsigned q) {
    if (q==0) {
        if (!bHomology0_) {
            computeChainComplexes();
            bHomology0_ = MarkedAbelianGroup(*Bd0_,*Bd1_);
        }
        return *bHomology0_;
    } else if (q==1) {
        if (!bHomology1_) {
            computeChainComplexes();
            bHomology1_ = MarkedAbelianGroup(*Bd1_,*Bd2_);
        }
        return *bHomology1_;
    } else {
        // Assume q == 2.  This will at least avoid a crash if q lies
        // outside the required range.
        if (!bHomology2_) {
            computeChainComplexes();
            bHomology2_ = MarkedAbelianGroup(*Bd2_,*Bd3_);
        }
        return *bHomology2_;
    }
}

const MarkedAbelianGroup& HomologicalData::dualHomology(unsigned q) {
    if (q==0) {
        if (!dmHomology0_) {
            computeChainComplexes();
            dmHomology0_ = MarkedAbelianGroup(*B0_,*B1_);
        }
        return *dmHomology0_;
    } else if (q==1) {
        if (!dmHomology1_) {
            computeChainComplexes();
            dmHomology1_ = MarkedAbelianGroup(*B1_,*B2_);
        }
        return *dmHomology1_;
    } else if (q==2) {
        if (!dmHomology2_) {
            computeChainComplexes();
            dmHomology2_ = MarkedAbelianGroup(*B2_,*B3_);
        }
        return *dmHomology2_;
    } else {
        // Assume q == 3.  This will at least avoid a crash if q lies
        // outside the required range.
        if (!dmHomology3_) {
            computeChainComplexes();
            dmHomology3_ = MarkedAbelianGroup(*B3_,*B4_);
        }
        return *dmHomology3_;
    }
}

void HomologicalData::computeHomology() {
    computeChainComplexes();
    if (!mHomology0_)
        mHomology0_ = MarkedAbelianGroup(*A0_,*A1_);
    if (!mHomology1_)
        mHomology1_ = MarkedAbelianGroup(*A1_,*A2_);
    if (!mHomology2_)
        mHomology2_ = MarkedAbelianGroup(*A2_,*A3_);
    if (!mHomology3_)
        mHomology3_ = MarkedAbelianGroup(*A3_,*A4_);
}

void HomologicalData::computeBHomology() {
    computeChainComplexes();
    if (!bHomology0_)
        bHomology0_ = MarkedAbelianGroup(*Bd0_,*Bd1_);
    if (!bHomology1_)
        bHomology1_ = MarkedAbelianGroup(*Bd1_,*Bd2_);
    if (!bHomology2_)
        bHomology2_ = MarkedAbelianGroup(*Bd2_,*Bd3_);
}

void HomologicalData::computeDHomology() {
    computeChainComplexes();
    if (!dmHomology0_)
        dmHomology0_ = MarkedAbelianGroup(*B0_,*B1_);
    if (!dmHomology1_)
        dmHomology1_ = MarkedAbelianGroup(*B1_,*B2_);
    if (!dmHomology2_)
        dmHomology2_ = MarkedAbelianGroup(*B2_,*B3_);
    if (!dmHomology3_)
        dmHomology3_ = MarkedAbelianGroup(*B3_,*B4_);
}

const HomMarkedAbelianGroup& HomologicalData::h1CellAp() {
    if (!dmTomMap1_) {
        computeHomology();
        computeDHomology();
        dmTomMap1_ = HomMarkedAbelianGroup(
            *dmHomology1_, *mHomology1_, *H1map_ );
    }
    return *dmTomMap1_;
}

const HomMarkedAbelianGroup& HomologicalData::bdryHomologyMap(unsigned q) {
    if (q==0) {
        if (!bmMap0_) {
            computeHomology();
            computeBHomology();
            bmMap0_ = HomMarkedAbelianGroup(
                *bHomology0_, *mHomology0_, *B0Incl_ );
        }
        return *bmMap0_;
    } else if (q==1) {
        if (!bmMap1_) {
            computeHomology();
            computeBHomology();
            bmMap1_ = HomMarkedAbelianGroup(
                *bHomology1_, *mHomology1_, *B1Incl_ );
        }
        return *bmMap1_;
    } else {
        // Assume q == 2.  This will at least avoid a crash if q lies
        // outside the required range.
        if (!bmMap2_) {
            computeHomology();
            computeBHomology();
            bmMap2_ = HomMarkedAbelianGroup(
                *bHomology2_, *mHomology2_, *B2Incl_ );
        }
        return *bmMap2_;
    }
}

void HomologicalData::computeBIncl() {
    computeHomology();
    computeBHomology();
    if (!bmMap0_)
        bmMap0_ = HomMarkedAbelianGroup(
            *bHomology0_, *mHomology0_, *B0Incl_);
    if (!bmMap1_)
        bmMap1_ = HomMarkedAbelianGroup(
            *bHomology1_, *mHomology1_, *B1Incl_);
    if (!bmMap2_)
        bmMap2_ = HomMarkedAbelianGroup(
            *bHomology2_, *mHomology2_, *B2Incl_);
}


void HomologicalData::computeTorsionLinkingForm() {
    // Only do this if we haven't done it already.
    if (torsionFormComputed)
        return;

    // dual h1 --> standard h1 isomorphism:
    const HomMarkedAbelianGroup& h1CellApComputed(h1CellAp());
    // min number of torsion gens:
    unsigned long niv(dmHomology1_->countInvariantFactors());
    // for holding prime decompositions.:
    std::vector<std::pair<Integer, unsigned long> > tFac;

    Integer tI;


    // step 1: go through H1 of the manifold, take prime power decomposition
    //            of each summand.  building primePowerH1Torsion vector and
    //            pTorsionH1Mat matrix...
    //            also, we need to find the 2-chains bounding2c
    //            boundary(bounding2c[i]) = orderinh1(pvList[i])*pvList[i]

    std::vector<Integer> ppList; // prime power list
    std::vector< std::pair<Integer, unsigned long> >
        pPrList; // proper prime power list.
    std::vector< Vector<Integer> > pvList; // list of vectors
    // the above two lists will have the same length. for each i,
    // pvList[i] will be a vector in the dual h1 homology chain complex, and
    // ppList[i] will be its order.

    unsigned long i, j, k, l;

    for (i=0; i<niv; i++) {
        tI = dmHomology1_->invariantFactor(i);
        tFac = Primes::primePowerDecomp(tI);

        for (j=0; j<tFac.size(); j++) {
            pPrList.push_back(tFac[j]);
            Integer fac1, fac2, fac1i, fac2i;

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
            // invariantFactor(i).
            Vector<Integer> tV = dmHomology1_->torsionRep(i);

            for (k=0; k<tV.size(); k++) tV[k]=fac1i*fac2*tV[k];

            pvList.push_back(tV);
        }
    }

    // step1a: construct (2 2 4) (3 3 9 27) ... indexing of ppList, pvList, etc.
    // the indexing will be as a list of pairs
    // < prime, vector< pair< power, index> > >
    // Use a list because we are continually inserting items in the middle.
    std::list< std::pair< Integer,
        std::vector<std::pair<unsigned long, unsigned long> > > > indexing;
    // indexing[i] is the i-th prime in increasing order, the first bit is
    // the prime, the 2nd bit is the vector list of powers, the power is an
    // unsigned long, and its respective index in ppList and pvList is the
    // 2nd bit...

    for (i=0; i<pPrList.size(); i++) { 
        // for each entry in pPrList, find its appropriate position in indexing.
        // so this means comparing pPrList[i].first with all elts
        // indexing[j].first and stopping at first >= comparison.

        auto it1 = indexing.begin(); 
        // now run up p until we either get to the end, or
        // pPrList[i].first >= it1->first
        auto il1 = indexing.end(); 
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
            indexing.insert( il1,
                { pPrList[i].first, { { pPrList[i].second, i } } });
        } else
            if (pPrList[i].first < il1->first) {
                indexing.insert( il1,
                    { pPrList[i].first, { { pPrList[i].second, i } } });
            } else {
                // NOW we know this prime is already in the list, so we do
                // the same search for the power...
                auto it2 = il1->second.begin();
                auto il2 = il1->second.end();
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


    MatrixInt standardBasis( numStandardCells[1], pvList.size() );
    const MatrixInt& dualtostandard(h1CellApComputed.definingMatrix());

    for (i=0; i<standardBasis.rows(); i++)
        for (j=0; j<standardBasis.columns(); j++)
            for (k=0; k<dualtostandard.columns(); k++)
                standardBasis.entry(i,j) +=
                    dualtostandard.entry(i,k)*pvList[j][k];

    // step 3: construct bounding classes
    //           the j-th column of standardBasis, when multiplied by
    //           ppList[j] bounds, so find a chain with that boundary and
    //           put its info in a matrix.

    MatrixInt ON(mHomology1_->N());
    MatrixInt R, Ri, C, Ci;
    smithNormalForm(ON, R, Ri, C, Ci);
    // boundingMat=R*(divide by ON diag, rescale(C*areboundariesM))
    //                                             ---- stepa -----
    //                  ---------------- stepb ---
    //               ----stepc----
    // first I guess we need to determine rank of ON?
    MatrixInt areboundariesM( standardBasis );

    for (i=0; i<standardBasis.rows(); i++)
        for (j=0; j<standardBasis.columns(); j++)
            areboundariesM.entry(i,j) *= ppList[j];

    MatrixInt stepa( areboundariesM.rows(), areboundariesM.columns() );
    for (i=0; i<standardBasis.rows(); i++)
        for (j=0; j<standardBasis.columns(); j++)
            for (k=0; k<C.columns(); k++)
                stepa.entry(i,j) += C.entry(i,k)*areboundariesM.entry(k,j);

    unsigned long rankON=0;
    for (i=0; ((i<ON.rows()) && (i<ON.columns())); i++)
        if (ON.entry(i,i) != 0) rankON++;

    MatrixInt stepb( R.columns(), stepa.columns() );

    for (i=0; i<rankON; i++)
        for (j=0; j<stepb.columns(); j++)
            stepb.entry(i,j) = stepa.entry(i,j).divByExact(ON.entry(i,i));

    MatrixInt boundingMat( stepb.rows(), stepb.columns() );

    for (i=0; i<stepb.rows(); i++)
        for (j=0; j<stepb.columns(); j++)
            for (k=0; k<R.columns(); k++)
                boundingMat.entry(i,j) += R.entry(i,k)*stepb.entry(k,j);

    // step 4: intersect, construct matrix.

    Matrix<Rational> torsionLinkingFormPresentationMat(
        pvList.size(), pvList.size() );

    // Dereference the SnapshotRef just once.
    const Triangulation<3>& tri(*tri_);

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
                // dual orientation of triangle points into some tetrahedron
                //  given by triangle[?]->embedding(0) is a TriangleEmbedding<3>
                //     triangle[]->embedding(0).tetrahedron() tet pointer
                //     triangle[]->embedding(0).vertices() is a Perm
                //
                // triangles[dNBF[k]] is the triangle pointer of the dual 1-cell

                // boundingMat is vectors in standard 2-complex so it has
                // the same dimension as the standard 2-cells + ideal 2-cells,
                // standard ones coming first.
                // pvList is vectors in dual 1-cells
                torsionLinkingFormPresentationMat.entry(i,j) +=
                    Rational(
                        boundingMat.entry(dNBF[k],i)*pvList[j][k]*
                        Integer(
                            tri.triangle(dNBF[k])->embedding(0).
                                tetrahedron()->orientation()*
                            tri.triangle(dNBF[k])->embedding(0).
                                vertices().sign() ), ppList[i] );
            }
            Integer tN=torsionLinkingFormPresentationMat.entry(i,j).numerator();
            Integer tD=torsionLinkingFormPresentationMat.entry(i,j).denominator();
            auto [tQ, tR] = tN.divisionAlg(tD);
            tN = tR.gcd(tD);
            tR.divByExact(tN);
            tD.divByExact(tN);
            torsionLinkingFormPresentationMat.entry(i,j)=Rational(tR,tD);
        }

    // Compute indexing.size() just once, since for std::list this might be
    // a slow operation.
    unsigned long indexingSize = indexing.size();
    decltype(indexing.begin()) it1;

    h1PrimePowerDecomp.resize(indexingSize);
    linkingFormPD.resize(indexingSize);
    for (i=0, it1 = indexing.begin(); it1 != indexing.end(); i++, it1++) {
        h1PrimePowerDecomp[i].second.resize(it1->second.size());
        h1PrimePowerDecomp[i].first = it1->first;

        for (j=0; j<it1->second.size(); j++)
            h1PrimePowerDecomp[i].second[j] = it1->second[j].first;

        linkingFormPD[i] = Matrix<Rational>(it1->second.size(),
                it1->second.size() );
        for (j=0; j<it1->second.size(); j++)
            for (k=0; k<it1->second.size(); k++)
                linkingFormPD[i].entry(j,k) =
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
    // this will be a std::vector< std::pair< Integer,
    //                                        std::vector< unsigned long > > >
    // rankv[i].first is the prime, and rankv[i].second is the vector which
    // lists the ranks
    // ie: if rankv[i].first==3 then rankv[i].second=(0,1,0,2,0,1) means that
    // there are no copies of Z_3, one copy of Z_9, no copies of Z_27 but two
    // copies of Z_{3^4}, etc.

    // std::vector< std::pair< Integer,
    //     std::vector<std::pair<unsigned long, unsigned long> > > > indexing;
    //                         prime        , list of (exponents, index)

    torRankV.resize(indexingSize);
    // std::vector< std::pair< Integer,
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
    //           and LargeInteger instead.
    // decide on if there is 2-torsion...
    Integer twoPow;
    std::vector< Integer > groupV;
    bool notatend;

    unsigned long incind;
    bool incrun;

    std::vector< Integer > ProperPrimePower;

    if (h1PrimePowerDecomp.size() > 0)
        if (h1PrimePowerDecomp[0].first == Integer(2)) { 
            // there is 2-torsion. now we put together the sigma vector
            // twoTorSigmaV
            // first initialize the length of twoTorSigmaV
            twoTorSigmaV.resize(torRankV[0].second.size());

            groupV.resize(h1PrimePowerDecomp[0].second.size(), Integer(0));

            ProperPrimePower.resize( h1PrimePowerDecomp[0].second.size() );
            for (i=0; i<ProperPrimePower.size(); i++) {
                ProperPrimePower[i] = Integer(2);
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
                // std::vector< std::pair< Integer,
                //     std::vector<unsigned long> > > h1PrimePowerDecomp;
                // as it's easier to work with.
                // h1PrimePowerDecomp[0].first == 2
                // so we just need to cycle through
                // h1PrimePowerDecomp[0].second which is an increasing list
                // of the powers of 2, ie: 2^i...

                twoPow = 2;
                twoPow.raiseToPower(i+1);

                long double xlD=0.0;
                long double ylD=0.0;

                // now start the sum through the group.
                notatend=true;
                while (notatend) {
                    // compute twoPow * pi * form(x,x), reduce mod 1 then
                    // call doubleApprox()
                    // first we evaluate the form(x,x) for x==groupV.
                    // the form is linkingformPD[0]
                    Rational tSum; // init to 0
                    for (j=0; j<linkingFormPD[0].rows(); j++)
                        for (k=0; k<linkingFormPD[0].columns();
                                k++)
                            tSum += Rational(groupV[j]*groupV[k])*
                                linkingFormPD[0].entry(j,k);

                    // reduce mod 1, then turn into a long double and
                    // evaluate cos, sin
                    Integer tN = tSum.numerator();
                    Integer tD = tSum.denominator();
                    auto [tQ, tR] = tN.divisionAlg(tD);
                    long double tLD =
                        (Rational(tR, tD) * twoPow).doubleApprox() * M_PI;
                    // we ignore `inrange' parameter as the number is reduced
                    // mod 1, so either way it is
                    // returning essentially the correct number.
                    xlD = xlD + cos(tLD);
                    ylD = ylD + sin(tLD);
                    // increment the groupV
                    incind=0;
                    incrun=true; // tells while loop to increment at incind

                    while (incrun) {
                        groupV[incind] = (groupV[incind] + 1)
                            % ProperPrimePower[incind];
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
                    twoTorSigmaV[i] = LargeInteger::infinity;
                } else { // now we need to determine the sigma angle...
                    // since it's all integer multiples of 2pi/8, we just
                    // need to check for
                    // xld==0, yld<>0, yld==0, xld<>0 and xld/yld=pm1
                    if ( std::abs(xlD) < 0.001*std::abs(ylD) ) {
                        if (ylD > 0.0) twoTorSigmaV[i]=2;
                        else twoTorSigmaV[i]=6;
                    } else
                        if ( std::abs(ylD) < 0.001*std::abs(xlD) ) {
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
    //           to do this I need to add a determinant to Matrix class
    //           this invariant will be expressed as a
    //           std::vector< std::pair< Integer, std::vector< int > > >
    //           storing the odd prime, list of Legendre symbols -1, 0, 1.
    //           one for each quotient up to p^k where k is the largest order of
    //           p in the torsion subgroup.

    unsigned long starti=0;
    if (torRankV.size() > 0)
        if (torRankV[0].first == Integer(2))
            starti=1;
    // this ensures we skip the 2-torsion
    std::vector<int> tempa;
    unsigned long curri;

    for (i=starti; i<torRankV.size(); i++) // for each prime
    {
        tempa.resize(0);
        curri=0;

        // now we cut out the appropriate section of linkingFormPD[i]
        // std::vector< std::pair< Integer,
        //     std::vector< unsigned long > > > torRankV(indexing.size());
        // starting at curri ending at torRankV[i].second[j]

        for (j=0; j<torRankV[i].second.size(); j++)
        // dimensions of p^{j+1} subspace
        {
            // initialize a torRankV[i].second[j] square matrix.
            MatrixInt tempM(torRankV[i].second[j], torRankV[i].second[j]);

            // tempM will be the torRankV[i].second[j] square submatrix
            // starting at curri, multiplied by tI == p^j
            tI = torRankV[i].first;
            tI.raiseToPower(j+1);

            for (k=0; k<torRankV[i].second[j]; k++)
                for (l=0; l<torRankV[i].second[j]; l++)
                    tempM.entry(k,l) = (Rational(tI)*linkingFormPD[i].
                        entry(k+curri,l+curri)).numerator();

            tempa.push_back( tempM.det().legendre(torRankV[i].first) );
            // legendre symbol, compute and append to tempa
            // compute determinant.

            // increment curri
            curri = curri + torRankV[i].second[j]; // crashes here.
        }
        oddTorLegSymV.emplace_back( torRankV[i].first , tempa );
    }

    // step 4: kk test for: split, hyperbolic, and the embeddability
    //           2^k-torsion condition.

    torsionLinkingFormIsSplit=true;
    torsionLinkingFormIsHyperbolic=true;

    starti=0;
    if (torRankV.size() > 0)
        if (torRankV[0].first == Integer(2))
            starti=1;

    for (i=0; i<torRankV.size(); i++)
        for (j=0; j<torRankV[i].second.size(); j++)
            if ( (torRankV[i].second[j] % 2) != 0 )
                torsionLinkingFormIsSplit=false;
    if (torsionLinkingFormIsSplit) {
        for (i=0; i<oddTorLegSymV.size(); i++)
            for (j=0; j<oddTorLegSymV[i].second.size(); j++) {
                if ( ( (Integer(torRankV[i+starti].second[j])*
                        (torRankV[i+starti].first - 1)) / 4 ) % 2 == 0 ) {
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
            if ( (twoTorSigmaV[i]!=0) &&
                    (twoTorSigmaV[i]!=LargeInteger::infinity) )
                torsionLinkingFormIsSplit=false;
    }

    if (torsionLinkingFormIsSplit==false) torsionLinkingFormIsHyperbolic=false;

    if ( (torsionLinkingFormIsSplit) && (starti==1) ) {
        torsionLinkingFormIsHyperbolic = true;
        for (i=0; i<twoTorSigmaV.size(); i++)
            if (twoTorSigmaV[i]!=0)
                torsionLinkingFormIsHyperbolic=false;
    }

    Rational tRat;

    torsionLinkingFormSatisfiesKKtwoTorCondition=true;
    if (starti==1) { // for each k need to compute 2^{k-1}*form(x,x) on all
        // elements of order 2^k, check to see if it is zero.
        // so this is not yet quite implemented, yet....
        // std::vector< std::pair< Integer,
        //     std::vector<unsigned long> > > h1PrimePowerDecomp;
        // stored as list { (2, (1, 1, 2)), (3, (1, 2, 2, 3)), (5, (1, 1, 2)) }
        //std::vector< Matrix<Rational> > linkingFormPD;
        for (i=0; i<h1PrimePowerDecomp[0].second.size(); i++) {
            // run down diagonal of linkingFormPD[0], for each (i,i) entry
            // multiply it by 2^{h1PrimePowerDecomp[0].second[i]-1} check if
            // congruent to zero. if not, trigger flag.
            tI = Integer(2);
            tI.raiseToPower(h1PrimePowerDecomp[0].second[i]-1);
            tRat = Rational(tI) * linkingFormPD[0].entry(i,i);
            Integer tN = tRat.numerator();
            Integer tD = tRat.denominator();
            auto [tQ, tR] = tN.divisionAlg(tD);
            if (tR != 0)
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
                    LargeInteger(torRankV[i].second[j]).stringValue() );
                if (j < torRankV[i].second.size()-1)
                    torsionRankString.append(" ");
            }
            torsionRankString.append(")");
            if (i<(torRankV.size()-1))
                torsionRankString.append(" ");
    }

    if (tri.isOrientable()) {
        torsionSigmaString.assign("");
        if (twoTorSigmaV.size()==0) torsionSigmaString.append("no 2-torsion");
        else for (i=0; i<twoTorSigmaV.size(); i++) {
            torsionSigmaString.append(twoTorSigmaV[i].stringValue());
            if (i<(twoTorSigmaV.size()-1)) torsionSigmaString.append(" ");
            }
        }
    else torsionSigmaString.assign("manifold is non-orientable");

    if (tri.isOrientable()) {
        torsionLegendreString.assign("");
        if (oddTorLegSymV.size()==0)
            torsionLegendreString.append("no odd p-torsion");
        else for (i=0; i<oddTorLegSymV.size(); i++) {
            torsionLegendreString.append(oddTorLegSymV[i].first.stringValue());
            torsionLegendreString.append("(");
            for (j=0; j<oddTorLegSymV[i].second.size(); j++) {
                torsionLegendreString.append( Integer(
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

void HomologicalData::computeEmbeddabilityString() {
    // Only do this if we haven't done it already.
    if (! embeddabilityString.empty())
        return;

    // Dereference the SnapshotRef just once.
    const Triangulation<3>& tri(*tri_);

    if (tri.isEmpty())
      {
        // special-case the empty triangulation
        embeddabilityString = "Manifold is empty.";
      }
    else if (tri.isOrientable())
      { // orientable -- we need the torsion linking form
        computeTorsionLinkingForm();

        if (bdryHomology(0).isTrivial()) 
        { // no boundary : orientable
            if (torRankV.size()==0) 
            { // no torsion : no boundary, orientable
                if (tri.knowsSphere() && tri.isSphere())
                    embeddabilityString = "This manifold is S^3.";
                else if (dualHomology(1).isTrivial())
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
                if (dualHomology(1).rank()==0)
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
                 if (bdryHomologyMap(1).isEpic())
                    {
                    embeddabilityString =
                        "Embeds in a homology 3-sphere as a ";
                    if (bdryHomology(1).rank() ==
                            2*bdryHomology(0).rank())
                        {
                        if (bdryHomology(0).rank()==1)
                            embeddabilityString += "knot complement.";
                        else
                            embeddabilityString += "link complement.";
                        }
                    else
                        {
                        if (bdryHomology(1).rank() == 0)
                            embeddabilityString += "ball complement.";
                        else
                            embeddabilityString += "graph complement.";
                        }
                    }
                 else if (bdryHomologyMap(1).cokernel().rank()==0)
                    {
                    embeddabilityString =
                        "Embeds in a rational homology 3-sphere as a ";
                    if (bdryHomology(1).rank() ==
                            2*bdryHomology(0).rank() )
                        {
                        if (bdryHomology(0).rank()==1)
                            embeddabilityString += "knot complement.";
                        else
                            embeddabilityString += "link complement.";
                        }
                    else
                        {
                        if (bdryHomology(1).rank() == 0)
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
                 if (bdryHomologyMap(1).isEpic())
                   embeddabilityString =
                        "Embeds in homology 3-sphere "
                        "but not homology 4-sphere.";
                 else if (bdryHomologyMap(1).cokernel().rank()==0)
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
                 if (bdryHomologyMap(1).isEpic())
                   embeddabilityString =
                        "Embeds in homology 3-sphere.  "
                        "KK 2-tor condition satisfied.";
                 else if (bdryHomologyMap(1).cokernel().rank()==0)
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
       Triangulation<3> orTri(tri, false);
       orTri.makeDoubleCover();
       HomologicalData covHomol(orTri);
        // break up into two cases, boundary and no boundary...
        if (covHomol.bdryHomology(0).isTrivial())
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


bool HomologicalData::formIsHyperbolic() {
    if (torsionFormComputed)
        return torsionLinkingFormIsHyperbolic;

    // Dereference the SnapshotRef just once.
    const Triangulation<3>& tri(*tri_);

    unsigned long nif=tri.homology().countInvariantFactors();
    if (nif == 0)
        return true;

    if ((nif % 2) != 0)
        return false;

    // check invariant factors agree in pairs, if so call
    // computeTorsionLinkingForm
    for (unsigned long i=0;i<(nif/2);i++) {
        if (tri.homology().invariantFactor(2*i) <
                tri.homology().invariantFactor((2*i)+1))
            return false;
    }

    computeTorsionLinkingForm();
    return torsionLinkingFormIsHyperbolic;
}


} // namespace regina



