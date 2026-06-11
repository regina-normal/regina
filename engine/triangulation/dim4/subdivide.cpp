
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <vector>
#include <map>
#include "triangulation/dim4.h"

namespace regina {

namespace {
    // Supporting material for truncateIdeal().

    // We list the types of pentachora:
    // (1) Original pentachoron, i.e. no ideal vertices.
    // else pen has an ideal vertex. Pen coords.
    // (2) Cone on standard tetrahedron, at pent barycentre. Using ambient pent
    //     coords.
    // (3) Cone on ideal tetrahedron, at pent barycentre. Using ambient pent
    //     coords.
    // Remaining are cone at pent barycentre of subdivided tetrahedron, ON:
    // (4) Cone on std tri at tet barycentre, using tet coords
    // (5) Cone on ideal tri at tet barycentre, using tet coords
    // else
    // (6) cone at tet barycentre of subdivided triangle with ideal vertices,
    //     using tri coords.
    enum class SubDivType { OP, CT, CiT, CCt, CCit, CCdt };
    //                      (1) (2) (3)  (4)  (5)   (6)

    struct SubDivNot { // used for types (a) and (b) via tetFlag.
        SubDivType penType;
        size_t penIdx;
        int tetIdx; // needed for all but (1).
        int triIdx; // needed for (4), (5), (6).
        int vtxIdx; // to specify which triangle vertex for (6).

        SubDivNot(const SubDivNot&) = default;

        SubDivNot(SubDivType PT, size_t PI, int TI=0, int tI=0, int VI=0) {
            penType = PT;
            penIdx = PI;
            tetIdx = TI;
            triIdx = tI;
            vtxIdx = VI;
        }

        bool operator<(const SubDivNot &other) const {
            if (penIdx < other.penIdx) return true;
            if (penIdx > other.penIdx) return false;
            if (penType < other.penType) return true;
            if (penType > other.penType) return false;
            // same penType here.
            if (penType == SubDivType::OP) return false;
            if (tetIdx < other.tetIdx) return true;
            if (tetIdx > other.tetIdx) return false;
            if (penType == SubDivType::CT || penType == SubDivType::CiT)
                return false;
            // done with (1), (2), (3)
            if (triIdx < other.triIdx) return true;
            if (triIdx > other.triIdx) return false;
            if (penType == SubDivType::CCt || penType == SubDivType::CCit)
                return false;
            // only CCdt (6) left
            if (vtxIdx < other.vtxIdx) return true;
            if (vtxIdx > other.vtxIdx) return false;
            return false;
        }
    };

    inline bool shouldTruncate(Vertex<4>* v) {
        return (v->isIdeal() || ! v->isValid());
    }
} // anonymous namespace

bool Triangulation<4>::truncateIdeal() {
    bool idVrts = false;
    for (auto* v : vertices())
        if (shouldTruncate(v)) {
            idVrts = true;
            break;
        }
    if (!idVrts)
        return false;

    // Although we don't necessarily subdivide every pentachoron,
    // the algorithm is messy enough that for now we just enforce no locks
    // at all (much like we do in 3-D).  Perhaps we can be more refined
    // about when we throw exceptions in some future release of Regina.
    if (hasLocks())
        throw LockViolation("An attempt was made to subdivide a "
            "triangulation with one or more locked pentachora or tetrahedra");

    // * * * Create new triangulation * * *
    Triangulation<4> newTri;

    // * * * Create the pentachora for the new triangulation * * *
    std::map<SubDivNot, Pentachoron<4>*> newPens; // this will index them.
    for (size_t i=0; i<size(); i++) {
        const Pentachoron<4>* aPen( pentachoron(i) ); // ambient pent
        bool pIv(false);   // check if has ideal vertices
        for (int j=0; j<5; j++)
            if (shouldTruncate(aPen->vertex(j)))
                pIv = true;
        if (!pIv) {
            newPens.insert({ SubDivNot(SubDivType::OP, i),
                newTri.newSimplexRaw() });
            continue;
        }
        for (int j=0; j<5; j++) { // tet / pen vtx loop
            // CiT check
            if (shouldTruncate(aPen->vertex(j)))
                newPens.insert({ SubDivNot(SubDivType::CiT, i, j),
                    newTri.newSimplexRaw() });
            // CT check
            bool TIv = false; // tet across from j has ideal vertex?
            for (int k=1; k<5; k++)
                if (shouldTruncate(aPen->vertex( (j+k) % 5)))
                    TIv = true;
            if (!TIv) {
                newPens.insert({ SubDivNot(SubDivType::CT, i, j),
                    newTri.newSimplexRaw() });
                continue;
            }
            // we're in situation 4, 5, or 6.
            const Tetrahedron<4>* aTet = aPen->tetrahedron(j);
            for (int k=0; k<4; k++) {
                if (shouldTruncate(aTet->vertex(k)))
                    newPens.insert({ SubDivNot( SubDivType::CCit, i, j, k ),
                       newTri.newSimplexRaw() }); // CCit
                newPens.insert({ SubDivNot( SubDivType::CCt, i, j, k ),
                    newTri.newSimplexRaw() }); // CCt
                bool tIv = false; // check if remaining triangle has ideal vtx
                for (int l=1; l<4; l++)
                    if (shouldTruncate(aTet->vertex( (k+l) % 4)))
                        tIv = true;
                if (!tIv)
                    continue;
                // the only way we can get here is the triangle has ideal
                // vertices. So we have to subdivide canonically.
                const Triangle<4>* aTri = aTet->triangle(k);
                for (int l=0; l<3; l++)
                    if (shouldTruncate(aTri->vertex(l)) )
                        newPens.insert({ SubDivNot( SubDivType::CCdt,
                            i, j, k, l ), newTri.newSimplexRaw() });
            } // end k loop
        } // end j loop
    } // end i loop

    //                         * * Create the Gluings. * *
    //  * * * gluings corresponding to non-boundary tets in original tri * * *
    for (size_t i=0; i<countTetrahedra(); i++)
        if (!tetrahedron(i)->isBoundary()) {
            // check if has ideal vertices
            const Tetrahedron<4>* aTet( tetrahedron(i) );
            const TetrahedronEmbedding<4> tEmb0( aTet->embedding(0) );
            const TetrahedronEmbedding<4> tEmb1( aTet->embedding(1) );

            bool TIv(false);
            for (int j=0; j<4; j++)
                if (shouldTruncate(aTet->vertex(j))) {
                    TIv=true;
                    break;
                }
            if (!TIv) { // decide between OP (1) and CT (2) for these
                SubDivNot p0( SubDivType::OP, tEmb0.pentachoron()->index() );
                SubDivNot p1( SubDivType::OP, tEmb1.pentachoron()->index() );
                if (shouldTruncate(tEmb0.pentachoron()->vertex(
                        tEmb0.tetrahedron()))) {
                    p0.penType = SubDivType::CT;
                    p0.tetIdx = aTet->embedding(0).tetrahedron();
                }
                if (shouldTruncate(tEmb1.pentachoron()->vertex(
                        tEmb1.tetrahedron()))) {
                    p1.penType = SubDivType::CT;
                    p1.tetIdx = aTet->embedding(1).tetrahedron();
                }
                newPens[ p0 ]->joinRaw(tEmb0.tetrahedron(), newPens[p1],
                    tEmb0.pentachoron()->adjacentGluing(tEmb0.tetrahedron()));
                continue;
            }
            // tet has ideal vertices, so it consists of cones on (perhaps
            // subdivided) triangles, so we're we're gluing (4) tet coords (5)
            // tc also or (6) tri coords.
            for (int j=0; j<4; j++) {
                bool tIV(false);   // check if tri across from vertex j has
                for (int k=1; k<4; k++) // ideal vertices
                    if (shouldTruncate(aTet->vertex( (j+k) % 4 ))) {
                        tIV=true;
                        break;
                    }
                {
                    SubDivNot p0(SubDivType::CCt, tEmb0.pentachoron()->index());
                    SubDivNot p1(SubDivType::CCt, tEmb1.pentachoron()->index());
                    p0.tetIdx = tEmb0.tetrahedron();
                    p1.tetIdx = tEmb1.tetrahedron();
                    p0.triIdx = j;
                    p1.triIdx = j;
                    newPens[ p0 ]->joinRaw( 4, newPens[ p1 ], Perm<5>() );
                    if (!tIV)
                        continue;

                    const Triangle<4>* aTri(aTet->triangle(j) );
                    // now the type (6) CCdt.
                    p0.penType = SubDivType::CCdt;
                    p1.penType = SubDivType::CCdt;
                    for (int k=0; k<3; k++)
                        if (shouldTruncate(aTri->vertex(k))) {
                            p0.vtxIdx = k;
                            p1.vtxIdx = k; // the tri with no ideal vertices.
                            newPens[ p0 ]->joinRaw(4, newPens[p1], {});
                        }
                }
                if (shouldTruncate(aTet->vertex(j))) { // we have a CCit
                    SubDivNot p0(SubDivType::CCit, tEmb0.pentachoron()->index());
                    SubDivNot p1(SubDivType::CCit, tEmb1.pentachoron()->index());
                    p0.tetIdx = tEmb0.tetrahedron();
                    p1.tetIdx = tEmb1.tetrahedron();
                    p0.triIdx = j;
                    p1.triIdx = j;
                    newPens[ p0 ]->joinRaw( 4, newPens[ p1 ], {});
                }
            } // end loop through tet vertices
        } // end look through tets.

    // * * * gluings corresponding to subdivision of individual pentachora * * *
    for (size_t i=0; i<size(); i++) {
        const Pentachoron<4>* aPen( pentachoron(i) );
        bool pIv(false);
        for (int j=0; j<5; j++)
            if (shouldTruncate(aPen->vertex(j)))
                pIv = true;
        if (!pIv)
            continue; // nothing to do!
        // step 1: all the gluings corresponding to triangle subdivisions, i.e.
        //    all  objects of type (6) CCdt and (4) CCt if on a common pen,
        //    tet and triangle.
        for (int j=0; j<5; j++) {
            const Tetrahedron<4>* aTet( aPen->tetrahedron( j ) );
            for (int k=0; k<4; k++) {
                SubDivNot p0( SubDivType::OP, i, j, k ); // will have to
                SubDivNot p1( SubDivType::OP, i, j, k ); // modify later
                const Triangle<4>* aTri( aTet->triangle(k) );
                bool tidV(false);
                for (int l=0; l<3; l++)
                    if (shouldTruncate(aTri->vertex(l))) {
                        tidV = true;
                        break;
                    }
                if (!tidV)
                    continue;
                // the triangle has ideal vertices, so there's something to do.
                // gluing pattern CCdt 0 -- CCdt 2 -- CCt -- CCdt 1, if not
                // ideal just erase the CCt uses tet coords.  So we need
                // the tri inclusion.
                Perm<5> triInc( aTet->triangleMapping(k) );
                if (shouldTruncate(aTri->vertex(1))) { // glue CCdt to CCt.
                    p0.penType = SubDivType::CCdt;
                    p1.penType = SubDivType::CCt;
                    p0.vtxIdx = 1;
                    newPens[ p0 ]->joinRaw(1, newPens[ p1 ],
                        { triInc[0], triInc[2], triInc[1], triInc[3], triInc[4] });
                }
                if (shouldTruncate(aTri->vertex(2))) { // glue this CCdt to CCt.
                    p0.penType = SubDivType::CCdt;
                    p1.penType = SubDivType::CCt;
                    p0.vtxIdx  = 2;
                    newPens[ p0 ]->joinRaw( 2, newPens[ p1 ],
                        { triInc[0], triInc[2], triInc[1], triInc[3], triInc[4] });
                }
                if (shouldTruncate(aTri->vertex(0))
                        && shouldTruncate(aTri->vertex(2))) { // glue 0 to 2
                    p0.penType = SubDivType::CCdt;
                    p1.penType = SubDivType::CCdt;
                    p0.vtxIdx = 2;
                    p1.vtxIdx = 0;
                    newPens[ p0 ]->joinRaw(1, newPens[p1], { 0, 2, 1, 3, 4 });
                }
                if (shouldTruncate(aTri->vertex(0))
                        && ! shouldTruncate(aTri->vertex(2))) {
                    p0.penType = SubDivType::CCdt; // glue 0 CCdt to CCt
                    p1.penType = SubDivType::CCt;
                    p0.vtxIdx = 0;
                    p1.vtxIdx = 0;
                    newPens[ p0 ]->joinRaw( 2, newPens[ p1 ],
                        { triInc[0], triInc[2], triInc[1], triInc[3], triInc[4] });
                }
            }
        }

        // step 2: glue the types (6) CCdt, (4) CCt and (5) CCit if on common
        //  tet but not on common triangular face of a tet. One gluing for
        //  every edge of the tet, and for every ideal edge of a triangle in
        //  the tet, if it exists.
        for (int j=0; j<5; j++) {
            const Tetrahedron<4>* aTet( aPen->tetrahedron(j) );
            bool tIv(false);
            for (int k=0; k<4; k++)
                if (shouldTruncate(aTet->vertex(k))) {
                    tIv=true;
                    break;
                }
            if (!tIv)
                continue;
            // first, we run through the tets of this pentachoron, and check if
            // it has ideal vertices.  If not, jump to next step.
            // (a) glue the CCit's (tet coords) to the CCdt's and CCts
            //    (tri and tet coords).
            for (int k=0; k<4; k++)
                if (shouldTruncate(aTet->vertex(k))) {
                    SubDivNot p0( SubDivType::CCdt, i, j );    // will have to
                    SubDivNot p1( SubDivType::CCit, i, j, k ); // modify later
                    for (int l=1; l<4; l++) {
                        p0.triIdx = (k+l) % 4;
                        Perm<5> triInc( aTet->triangleMapping( (k+l) % 4 ) );
                        p0.vtxIdx = triInc.pre( k );
                        // figure out gluing map, would seem to depend on
                        // p1.vtxIdx non-trivially.
                        // p0.vtxIdx= 0, 20    0->triInc[1] 1->p0.triIdx 2->triInc[2] 3->k 4->4
                        //            1, 12    0->p0.triIdx 1->triInc[0] 2->triInc[2] 3->k 4->4
                        //            2, 12    0->p0.triIdx 1->triInc[1] 2->triInc[0] 3->k 4->4
                        int A( (p0.vtxIdx==0) ? triInc[1] : p0.triIdx );
                        int B( (p0.vtxIdx==0) ? p0.triIdx : (p0.vtxIdx==1) ?
                            triInc[0] : triInc[1] );
                        int C( (p0.vtxIdx==2) ? triInc[0] : triInc[2] );
                        newPens[ p0 ]->joinRaw( (p0.vtxIdx==0) ? 1 : 0,
                            newPens[ p1 ], Perm<5>( A, B, C, k, 4 ) );
                    }
                }
            // (b) glue the CCdt and CCt's appropriately across tetrahedral
            for (int k=0; k<6; k++) { // edges.
                // recall aTet is the ambient tet, as we're in the j loop.
                SubDivNot p0( SubDivType::OP, i, j ); // will have to
                SubDivNot p1( SubDivType::OP, i, j ); // modify later
                Perm<5> eMap( aTet->edgeMapping(k) ); // eMap[0] eMap[1] edge
                // endpts eMap[2], eMap[3] tri indices.
                Perm<5> triInc2( aTet->triangleMapping(
                                    eMap[2] ) ); // natural inclusion to tet.
                Perm<5> triInc3( aTet->triangleMapping( eMap[3] ) ); // also.
                p0.triIdx = eMap[2];
                p1.triIdx = eMap[3];
                Perm<5> incPerm0, incPerm1; // relating the pent facets to the
                int glueT(0); // ambient pent. glue across this tet in
                // pen p0. for tri2 we are gluing a CCt only if either:
                //  (1) triInc2.pre( eMap[3] )==0 or
                //  (2) triInc2[1] non-ideal
                //  (3) *both* triInc2[2] and triInc2[0] nonideal
                if (eMap[3]==triInc2[0] ) { // id vtx 0 in tri2 adj
                    glueT=triInc2[0];
                    p0.penType = SubDivType::CCt;
                } else if (! shouldTruncate(aTet->vertex( triInc2[1] )) &&
                           eMap[3]==triInc2[2] ) { // id vtx 1 in tri2 adj
                    glueT=triInc2[2];
                    p0.penType = SubDivType::CCt;
                } else // id vtx
                    if (! shouldTruncate(aTet->vertex( triInc2[0] )) &&
                            ! shouldTruncate(aTet->vertex( triInc2[2] )) &&
                            eMap[3]==triInc2[1] ) {
                        glueT=triInc2[1];
                        p0.penType = SubDivType::CCt;
                    } else // now the CCdt
                        if (shouldTruncate(aTet->vertex( triInc2[1] )) &&
                                eMap[3]==triInc2[2] ) { // CCdt vtx 1
                            glueT=2;
                            p0.penType = SubDivType::CCdt;
                            incPerm0 = triInc2;
                            p0.vtxIdx = 1;
                        } else if ( shouldTruncate(aTet->vertex( triInc2[2] )) &&
                                  ! shouldTruncate(aTet->vertex( triInc2[0] )) &&
                                    eMap[3]==triInc2[1] ) {
                            glueT=1;
                            p0.penType = SubDivType::CCdt;
                            incPerm0 = triInc2;
                            p0.vtxIdx = 2;
                        } else if ( shouldTruncate(aTet->vertex( triInc2[0] )) &&
                                    eMap[3]==triInc2[1] ) {
                            glueT=0;
                            p0.penType = SubDivType::CCdt;
                            incPerm0 = Perm<5>(triInc2[1],triInc2[2],triInc2[0],
                                triInc2[3], triInc2[4] );
                        } else {
                            std::cout<<"Triangulation<4>::truncateIdeal()"<<
                                        " Error 1."<<std::endl;
                            exit(1);
                        }
                if (eMap[2]==triInc3[0]) {
                    // repeating the above for the other side.
                    p1.penType = SubDivType::CCt;
                } else if (! shouldTruncate(aTet->vertex( triInc3[1] )) &&
                           eMap[2]==triInc3[2] ) { // id vtx 1 in tri2 adj
                    p1.penType = SubDivType::CCt;
                } else // id vtx
                    if (! shouldTruncate(aTet->vertex( triInc3[0] )) &&
                            ! shouldTruncate(aTet->vertex( triInc3[2] )) &&
                            eMap[2]==triInc3[1] ) {
                        p1.penType = SubDivType::CCt;
                    } else // now the CCdt
                        if (shouldTruncate(aTet->vertex( triInc3[1] )) &&
                                eMap[2]==triInc3[2] ) { // CCdt vtx 1
                            p1.penType = SubDivType::CCdt;
                            incPerm1 = triInc3;
                            p1.vtxIdx = 1;
                        } else if ( shouldTruncate(aTet->vertex( triInc3[2] )) &&
                                  ! shouldTruncate(aTet->vertex( triInc3[0] )) &&
                                    eMap[2]==triInc3[1] ) {
                            p1.penType = SubDivType::CCdt;
                            incPerm1 = triInc3;
                            p1.vtxIdx = 2;
                        } else if ( shouldTruncate(aTet->vertex( triInc3[0] )) &&
                                    eMap[2]==triInc3[1] ) {
                            p1.penType = SubDivType::CCdt;
                            incPerm1 = Perm<5>(triInc3[1],triInc3[2],triInc3[0],
                                              triInc3[3], triInc3[4] );
                        } else {
                            std::cout<<"Triangulation<4>::truncateIdeal()"<<
                                        " Error 2."<<std::endl;
                            exit(1);
                        }
                newPens[ p0 ]->joinRaw( glueT, newPens[ p1 ],
                   incPerm1.inverse()*Perm<5>(eMap[2], eMap[3])*incPerm0 );
            }
        }

        // step 3: glue types in common pen but not in common tet facet.
        // we have an ideal vertex, so this pen is subdivided at the barycentre.
        for (int j=0; j<5; j++)
            if (shouldTruncate(aPen->vertex(j))) {
                // **all** CiT type (4) gluings done
                SubDivNot p0( SubDivType::CiT, i, j ); // uses pen coords
                SubDivNot p1( SubDivType::CCit, i );   // uses tet coords.
                for (int k=1; k<5; k++) { // gluing for tet j+k % 5.
                    Perm<5> tetInc( aPen->tetrahedronMapping( (j+k) % 5 ) );
                    p1.tetIdx = (j+k) % 5;
                    p1.triIdx = tetInc.pre( j ); // the ideal triangle
                    newPens[ p0 ]->joinRaw( (j+k)%5, newPens[ p1 ],
                        tetInc.inverse()*Perm<5>(j, (j+k)%5) );
                }
            }
        // we need to run through the triangles (equiv edges) of the pentachoron
        // and perform all of the appropriate gluings between tetrahedra.
        // step (a) check if it has ideal vertices.  If not, we are done!

        for (int j=0; j<10; j++) {
            Perm<5> triInc( aPen->triangleMapping( j ) );
            const Triangle<4>* aTri( aPen->triangle( j ) );
            // triInc[0 1 2] are the triangle vertices, [3 4] the vertices of the
            //  opposite edge. So we are gluing all the facets of tet's 3 and 4 which
            //  are incident to the triangle 012.
            Perm<5> tet0inc( aPen->tetrahedronMapping( triInc[3] ) );
            Perm<5> tet1inc( aPen->tetrahedronMapping( triInc[4] ) );
            int tri0idx( tet0inc.pre( triInc[4] ) );
            int tri1idx( tet1inc.pre( triInc[3] ) );
            // check if the triangle has any ideal vertices
            bool tIv(false);
            for (int k=0; k<3; k++)
                if (shouldTruncate(aTri->vertex(k))) {
                    tIv=true;
                    break;
                }
            // if tIv true, both sides are type CCt.
            bool s0it(false), s1it(false); // check if adjacent tets have ideal
            for (int k=0; k<4; k++) { // vertices
                if (shouldTruncate(aPen->vertex(tet0inc[k]))) s0it=true;
                if (shouldTruncate(aPen->vertex(tet1inc[k]))) s1it=true;
            }
            SubDivNot p0( s0it ? SubDivType::CCt : SubDivType::CT, i, triInc[3],
                s0it ? tri0idx : 0 );
            SubDivNot p1( s1it ? SubDivType::CCt : SubDivType::CT, i, triInc[4],
                s1it ? tri1idx : 0 );
            newPens[ p0 ]->joinRaw( s0it ? tri0idx : triInc[4] , newPens[ p1 ],
                       (s1it ? tet1inc : Perm<5>()).inverse()*Perm<5>(triInc[3],
                                      triInc[4])*(s0it ? tet0inc : Perm<5>()));
            if (!tIv) continue;
            for (int k=0; k<3; k++)
                if (shouldTruncate(aTri->vertex(k))) {
                    // CCdt uses triangle coordinates, which are fixed.
                    p0.penType = SubDivType::CCdt;
                    p1.penType = SubDivType::CCdt;
                    p0.vtxIdx  = k;
                    p1.vtxIdx  = k;
                    newPens[ p0 ]->joinRaw( 3, newPens[ p1 ], Perm<5>() );
                }
        } // j loop -- edges
    }

    // The call to swap() will trigger a change event and snapshot.
    swap(newTri);
    return true;
}

namespace {
    Perm<5> originalFacetToPrismBottom(int facet) {
        switch (facet) {
            case 1:
                return Perm<5>(1, 0, 2, 3, 4);
            case 2:
                return Perm<5>(1, 2, 0, 3, 4);
            case 3:
                return Perm<5>(1, 2, 3, 0, 4);
            case 4:
                return Perm<5>(1, 2, 3, 4, 0);
            default:
                return Perm<5>();
        }
    }

    Perm<5> prismBottomToAdjacentFacet(int facet, Perm<5> oldGluing) {
        return oldGluing * originalFacetToPrismBottom(facet).inverse();
    }

    int verticalFacet(int prismSimplex, int omittedBaseVertex) {
        if (prismSimplex < omittedBaseVertex)
            return omittedBaseVertex + 1;
        if (prismSimplex > omittedBaseVertex)
            return omittedBaseVertex;
        return -1;
    }
} // anonymous namespace

void Triangulation<4>::puncture(Tetrahedron<4>* location) {
    // If no tetrahedron is passed, then we avoid ever having to compute the
    // skeleton (since the skeleton will be destroyed after this operation
    // anyway). Therefore we keep the location of the puncture as a
    // (pentachoron, facet) pair:
    Pentachoron<4>* pent;
    int facet;

    if (location) {
        if (std::addressof(location->triangulation()) != this)
            throw InvalidArgument("puncture(): the given location is not "
                "within this triangulation");
        const auto& emb = location->front();
        pent = emb.pentachoron();
        facet = emb.tetrahedron();
    } else {
        if (simplices_.empty())
            throw InvalidArgument("puncture(): the triangulation is empty");
        // The default location:
        pent = simplices_.front();
        facet = 0;
    }

    // Is there a lock that we need to preserve?
    bool lock = pent->isFacetLocked(facet);

    // Note: we use the "raw" routines (joinRaw, newSimplexRaw, etc.),
    // mainly since we want to manage facet locks manually. This means that
    // the ChangeAndClearSpan here is vital.
    ChangeAndClearSpan<> span(*this);

    // We will attach a pair of tetrahedral prisms to the given facet of pent.
    // We will join the triangular-prism walls of the two prisms together, and
    // one tetrahedral end from each will join to form the new S^3 boundary.
    Pentachoron<4>* prism[2][4];

    // Create the new pentachora in an order that ensures that the two
    // tetrahedra of the new S^3 boundary will appear as the second-last and
    // last pentachora.
    for (int j = 0; j < 4; ++j)
        for (int i = 0; i < 2; ++i)
            prism[i][j] = newSimplexRaw();

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
            prism[i][j]->joinRaw(j + 1, prism[i][j + 1], Perm<5>());

    for (int r = 0; r < 4; ++r)
        for (int j = 0; j < 4; ++j) {
            int f = verticalFacet(j, r);
            if (f >= 0)
                prism[0][j]->joinRaw(f, prism[1][j], Perm<5>());
        }

    Pentachoron<4>* adj = pent->adjacentPentachoron(facet);
    if (adj) {
        Perm<5> gluing = pent->adjacentGluing(facet);
        pent->unjoinRaw(facet);
        prism[1][0]->joinRaw(0, adj,
            prismBottomToAdjacentFacet(facet, gluing));
    }

    pent->joinRaw(facet, prism[0][0],
        originalFacetToPrismBottom(facet));

    // Move the tetrahedron lock, if there was one.
    // If adj is non-null, its lock is already in place; we just need to
    // fix pent (move the lock from pent:facet to the far side of the prism).
    // If adj is null (so the tetrahedron is boundary), this same code will
    // push the lock out to the new boundary tetrahedron as expected.
    if (lock) {
        pent->unlockFacetRaw(facet);
        prism[1][0]->lockFacetRaw(0);
    }
}

void Triangulation<4>::connectedSumWith(const Triangulation<4>& other) {
    if (other.simplices_.empty())
        return;
    if (simplices_.empty()) {
        insertTriangulation(other);
        return;
    }

    // From here we can assume that each triangulation contains at least
    // one pentachoron.

    // Note: This PacketChangeSpan is essential, since we use "raw" routines
    // (joinRaw, etc.) further down below - this is so we can manage facet
    // locks manually. A basic PacketChangeSpan is enough: we do not need to
    // take a snapshot or clear properties, since (a) that will be managed
    // already by insertTriangulation() and puncture(), and (b) we will not
    // compute any fresh properties that need clearing after that.
    PacketChangeSpan span(*this);

    // Insert the other triangulation *before* puncturing this, so that
    // things work in the case where we sum a triangulation with itself.
    size_t n = simplices_.size();
    insertTriangulation(other);

    // Make the puncture in this triangulation and record the resulting new
    // boundary tetrahedra. Note: the default location for puncture() is
    // pentachoron 0, facet 0, which means the puncture comes from the
    // original (this) triangulation, not the inserted copy of other.
    puncture();
    Pentachoron<4>* bdryThis[2] = {
        simplices_[simplices_.size() - 2],
        simplices_[simplices_.size() - 1]
    };

    // Make the corresponding puncture in the inserted triangulation and
    // record its new boundary tetrahedra. We use facet 0 of pentachoron n,
    // which is the first pentachoron in the inserted copy of other.
    puncture(simplices_[n]->tetrahedron(0));
    Pentachoron<4>* bdryOther[2] = {
        simplices_[simplices_.size() - 2],
        simplices_[simplices_.size() - 1]
    };

    // Both punctures create the same two-tetrahedron triangulation of S^3.
    // Glue corresponding boundary tetrahedra using the same boundary map.
    bdryThis[0]->joinRaw(4, bdryOther[0], Perm<5>());
    bdryThis[1]->joinRaw(4, bdryOther[1], Perm<5>());
}

} // namespace regina
