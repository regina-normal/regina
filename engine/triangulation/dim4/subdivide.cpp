
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include <vector>
#include <map>
#include "triangulation/dim4.h"

namespace regina {

namespace {
    // Supporting material for idealToFinite().

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
    enum subDivType { _OP, _CT, _CiT, _CCt, _CCit, _CCdt };
    //                 (1) (2)   (3)   (4)   (5)     (6)

    struct subDivNot { // used for types (a) and (b) via tetFlag.
        subDivType penType;
        size_t penIdx;
        int tetIdx; // needed for all but (1).
        int triIdx; // needed for (4), (5), (6).
        int vtxIdx; // to specify which triangle vertex for (6).

        subDivNot(const subDivNot&) = default;

        subDivNot(subDivType PT, size_t PI, int TI=0, int tI=0, int VI=0) {
            penType = PT;
            penIdx = PI;
            tetIdx = TI;
            triIdx = tI;
            vtxIdx = VI;
        }

        bool operator<(const subDivNot &other) const {
            if (penIdx < other.penIdx) return true;
            if (penIdx > other.penIdx) return false;
            if (penType < other.penType) return true;
            if (penType > other.penType) return false;
            // same penType here.
            if (penType == _OP) return false;
            if (tetIdx < other.tetIdx) return true;
            if (tetIdx > other.tetIdx) return false;
            if ( (penType == _CT) || (penType == _CiT) ) return false;
            // done with (1), (2), (3)
            if (triIdx < other.triIdx) return true;
            if (triIdx > other.triIdx) return false;
            if ( (penType==_CCt) || (penType==_CCit) ) return false;
            // only _CCdt (6) left
            if (vtxIdx < other.vtxIdx) return true;
            if (vtxIdx > other.vtxIdx) return false;
            return false;
        }
    };

    inline bool shouldTruncate(Vertex<4>* v) {
        return (v->isIdeal() || ! v->isValid());
    }
} // anonymous namespace

bool Triangulation<4>::idealToFinite() {
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
    std::map<subDivNot, Pentachoron<4>*> newPens; // this will index them.
    for (size_t i=0; i<size(); i++) {
        const Pentachoron<4>* aPen( pentachoron(i) ); // ambient pent
        bool pIv(false);   // check if has ideal vertices
        for (int j=0; j<5; j++)
            if (shouldTruncate(aPen->vertex(j)))
                pIv = true;
        if (!pIv) {
            newPens.insert({ subDivNot(_OP, i), newTri.newSimplexRaw() });
            continue;
        }
        for (int j=0; j<5; j++) { // tet / pen vtx loop
            // _CiT check
            if (shouldTruncate(aPen->vertex(j)))
                newPens.insert({ subDivNot(_CiT, i, j), newTri.newSimplexRaw() });
            // _CT check
            bool TIv = false; // tet across from j has ideal vertex?
            for (int k=1; k<5; k++)
                if (shouldTruncate(aPen->vertex( (j+k) % 5)))
                    TIv = true;
            if (!TIv) {
                newPens.insert({ subDivNot(_CT, i, j), newTri.newSimplexRaw() });
                continue;
            }
            // we're in situation 4, 5, or 6.
            const Tetrahedron<4>* aTet = aPen->tetrahedron(j);
            for (int k=0; k<4; k++) {
                if (shouldTruncate(aTet->vertex(k)))
                    newPens.insert({ subDivNot( _CCit, i, j, k ),
                       newTri.newSimplexRaw() }); // CCit
                newPens.insert({ subDivNot( _CCt, i, j, k ),
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
                        newPens.insert({ subDivNot( _CCdt, i, j, k, l ),
                            newTri.newSimplexRaw() });
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
            if (!TIv) { // decide between _OP (1) and _CT (2) for these
                subDivNot p0( _OP, tEmb0.pentachoron()->index() );
                subDivNot p1( _OP, tEmb1.pentachoron()->index() );
                if (shouldTruncate(tEmb0.pentachoron()->vertex(
                        tEmb0.tetrahedron()))) {
                    p0.penType = _CT;
                    p0.tetIdx = aTet->embedding(0).tetrahedron();
                }
                if (shouldTruncate(tEmb1.pentachoron()->vertex(
                        tEmb1.tetrahedron()))) {
                    p1.penType = _CT;
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
                    subDivNot p0( _CCt, tEmb0.pentachoron()->index() );
                    subDivNot p1( _CCt, tEmb1.pentachoron()->index() );
                    p0.tetIdx = tEmb0.tetrahedron();
                    p1.tetIdx = tEmb1.tetrahedron();
                    p0.triIdx = j;
                    p1.triIdx = j;
                    newPens[ p0 ]->joinRaw( 4, newPens[ p1 ], Perm<5>() );
                    if (!tIV)
                        continue;

                    const Triangle<4>* aTri(aTet->triangle(j) );
                    // now the type (6) _CCdt.
                    p0.penType = _CCdt;
                    p1.penType = _CCdt;
                    for (int k=0; k<3; k++)
                        if (shouldTruncate(aTri->vertex(k))) {
                            p0.vtxIdx = k;
                            p1.vtxIdx = k; // the tri with no ideal vertices.
                            newPens[ p0 ]->joinRaw(4, newPens[p1], {});
                        }
                }
                if (shouldTruncate(aTet->vertex(j))) { // we have a _CCit
                    subDivNot p0( _CCit, tEmb0.pentachoron()->index() );
                    subDivNot p1( _CCit, tEmb1.pentachoron()->index() );
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
                subDivNot p0( _OP, i, j, k ); // will have to
                subDivNot p1( _OP, i, j, k ); // modify later
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
                    p0.penType = _CCdt;
                    p1.penType = _CCt;
                    p0.vtxIdx = 1;
                    newPens[ p0 ]->joinRaw(1, newPens[ p1 ],
                        { triInc[0], triInc[2], triInc[1], triInc[3], triInc[4] });
                }
                if (shouldTruncate(aTri->vertex(2))) { // glue this CCdt to CCt.
                    p0.penType = _CCdt;
                    p1.penType = _CCt;
                    p0.vtxIdx  = 2;
                    newPens[ p0 ]->joinRaw( 2, newPens[ p1 ],
                        { triInc[0], triInc[2], triInc[1], triInc[3], triInc[4] });
                }
                if (shouldTruncate(aTri->vertex(0))
                        && shouldTruncate(aTri->vertex(2))) { // glue 0 to 2
                    p0.penType = _CCdt;
                    p1.penType = _CCdt;
                    p0.vtxIdx = 2;
                    p1.vtxIdx = 0;
                    newPens[ p0 ]->joinRaw(1, newPens[p1], { 0, 2, 1, 3, 4 });
                }
                if (shouldTruncate(aTri->vertex(0))
                        && ! shouldTruncate(aTri->vertex(2))) {
                    p0.penType = _CCdt; // glue 0 CCdt to CCt
                    p1.penType = _CCt;
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
                    subDivNot p0( _CCdt, i, j ); // will have to
                    subDivNot p1( _CCit, i, j, k ); // modify later
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
                subDivNot p0( _OP, i, j ); // will have to
                subDivNot p1( _OP, i, j ); // modify later
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
                    p0.penType = _CCt;
                } else if (! shouldTruncate(aTet->vertex( triInc2[1] )) &&
                           eMap[3]==triInc2[2] ) { // id vtx 1 in tri2 adj
                    glueT=triInc2[2];
                    p0.penType = _CCt;
                } else // id vtx
                    if (! shouldTruncate(aTet->vertex( triInc2[0] )) &&
                            ! shouldTruncate(aTet->vertex( triInc2[2] )) &&
                            eMap[3]==triInc2[1] ) {
                        glueT=triInc2[1];
                        p0.penType = _CCt;
                    } else // now the _CCdt
                        if (shouldTruncate(aTet->vertex( triInc2[1] )) &&
                                eMap[3]==triInc2[2] ) { // CCdt vtx 1
                            glueT=2;
                            p0.penType = _CCdt;
                            incPerm0 = triInc2;
                            p0.vtxIdx = 1;
                        } else if ( shouldTruncate(aTet->vertex( triInc2[2] )) &&
                                  ! shouldTruncate(aTet->vertex( triInc2[0] )) &&
                                    eMap[3]==triInc2[1] ) {
                            glueT=1;
                            p0.penType = _CCdt;
                            incPerm0 = triInc2;
                            p0.vtxIdx = 2;
                        } else if ( shouldTruncate(aTet->vertex( triInc2[0] )) &&
                                    eMap[3]==triInc2[1] ) {
                            glueT=0;
                            p0.penType = _CCdt;
                            incPerm0 = Perm<5>(triInc2[1],triInc2[2],triInc2[0],
                                triInc2[3], triInc2[4] );
                        } else {
                            std::cout<<"Triangulation<4>::idealToFinite()"<<
                                        " Error 1."<<std::endl;
                            exit(1);
                        }
                if (eMap[2]==triInc3[0]) { // repeating the above for the other
                    p1.penType = _CCt;     // side.
                } else if (! shouldTruncate(aTet->vertex( triInc3[1] )) &&
                           eMap[2]==triInc3[2] ) { // id vtx 1 in tri2 adj
                    p1.penType = _CCt;
                } else // id vtx
                    if (! shouldTruncate(aTet->vertex( triInc3[0] )) &&
                            ! shouldTruncate(aTet->vertex( triInc3[2] )) &&
                            eMap[2]==triInc3[1] ) {
                        p1.penType = _CCt;
                    } else // now the _CCdt
                        if (shouldTruncate(aTet->vertex( triInc3[1] )) &&
                                eMap[2]==triInc3[2] ) { // CCdt vtx 1
                            p1.penType = _CCdt;
                            incPerm1 = triInc3;
                            p1.vtxIdx = 1;
                        } else if ( shouldTruncate(aTet->vertex( triInc3[2] )) &&
                                  ! shouldTruncate(aTet->vertex( triInc3[0] )) &&
                                    eMap[2]==triInc3[1] ) {
                            p1.penType = _CCdt;
                            incPerm1 = triInc3;
                            p1.vtxIdx = 2;
                        } else if ( shouldTruncate(aTet->vertex( triInc3[0] )) &&
                                    eMap[2]==triInc3[1] ) {
                            p1.penType = _CCdt;
                            incPerm1 = Perm<5>(triInc3[1],triInc3[2],triInc3[0],
                                              triInc3[3], triInc3[4] );
                        } else {
                            std::cout<<"Triangulation<4>::idealToFinite()"<<
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
                subDivNot p0( _CiT, i, j ); // uses pen coords
                subDivNot p1( _CCit, i );   // uses tet coords.
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
            subDivNot p0( s0it ? _CCt : _CT, i, triInc[3], s0it ? tri0idx : 0 );
            subDivNot p1( s1it ? _CCt : _CT, i, triInc[4], s1it ? tri1idx : 0 );
            newPens[ p0 ]->joinRaw( s0it ? tri0idx : triInc[4] , newPens[ p1 ],
                       (s1it ? tet1inc : Perm<5>()).inverse()*Perm<5>(triInc[3],
                                      triInc[4])*(s0it ? tet0inc : Perm<5>()));
            if (!tIv) continue;
            for (int k=0; k<3; k++)
                if (shouldTruncate(aTri->vertex(k))) {
                    // _CCdt uses triangle coordinates, which are fixed.
                    p0.penType =_CCdt;
                    p1.penType = _CCdt;
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

} // namespace regina
