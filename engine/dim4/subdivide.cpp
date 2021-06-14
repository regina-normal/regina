
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

#include <vector>
#include <map>

#include "dim4/dim4triangulation.h"
#include "triangulation/nedge.h"

namespace regina {

void Dim4Triangulation::barycentricSubdivision() {
    unsigned long nOldPent = pentachora_.size();
    if (nOldPent == 0)
        return;

    Dim4Triangulation staging;
    ChangeEventSpan span1(&staging);

    Dim4Pentachoron** newPent = new Dim4Pentachoron*[nOldPent * 120];
    Dim4Pentachoron* oldPent;

    // A pentachoron in the subdivision is uniquely defined by the
    // permutation (tet, triangle, edge, vtx, corner) of (0, 1, 2, 3, 4).
    // This is the pentachoron that:
    // - meets the boundary in the tetrahedron opposite vertex "tet";
    // - meets that tetrahedron in the triangle opposite vertex "triangle";
    // - meets that triangle in the edge opposite vertex "edge";
    // - meets that edge in the vertex opposite vertex "vtx";
    // - directly touches vertex "corner".

    unsigned long pent;
    for (pent=0; pent < 120 * nOldPent; ++pent)
        newPent[pent] = staging.newPentachoron();

    // Do all of the internal gluings
    int permIdx;
    NPerm5 perm, glue;
    for (pent=0; pent < nOldPent; ++pent)
        for (permIdx = 0; permIdx < 120; ++permIdx) {
            perm = NPerm5::S5[permIdx];
            // (0, 1, 2, 3, 4) -> (tet, triangle, edge, vtx, corner)

            // Internal gluings within the old pentachoron:
            newPent[120 * pent + permIdx]->joinTo(perm[4],
                newPent[120 * pent + (perm * NPerm5(4, 3)).S5Index()],
                NPerm5(perm[4], perm[3]));

            newPent[120 * pent + permIdx]->joinTo(perm[3],
                newPent[120 * pent + (perm * NPerm5(3, 2)).S5Index()],
                NPerm5(perm[3], perm[2]));

            newPent[120 * pent + permIdx]->joinTo(perm[2],
                newPent[120 * pent + (perm * NPerm5(2, 1)).S5Index()],
                NPerm5(perm[2], perm[1]));

            newPent[120 * pent + permIdx]->joinTo(perm[1],
                newPent[120 * pent + (perm * NPerm5(1, 0)).S5Index()],
                NPerm5(perm[1], perm[0]));

            // Adjacent gluings to the adjacent pentachoron:
            oldPent = getPentachoron(pent);
            if (! oldPent->adjacentPentachoron(perm[0]))
                continue; // This hits a boundary facet.
            if (newPent[120 * pent + permIdx]->adjacentPentachoron(perm[0]))
                continue; // We've already done this gluing from the other side.

            glue = oldPent->adjacentGluing(perm[0]);
            newPent[120 * pent + permIdx]->joinTo(perm[0],
                newPent[120 * pentachoronIndex(
                    oldPent->adjacentPentachoron(perm[0])) +
                    (glue * perm).S5Index()],
                glue);
        }


    // Delete the existing pentachora and put in the new ones.
    ChangeEventSpan span2(this);
    removeAllPentachora();
    swapContents(staging);
    delete[] newPent;
}

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
        unsigned long penIdx;
        unsigned long tetIdx; // needed for all but (1).
        unsigned long triIdx; // needed for (4), (5), (6).
        unsigned long vtxIdx; // to specify which triangle vertex for (6).

        subDivNot( const subDivNot &cloneMe )
        {
            penType = cloneMe.penType;
            penIdx = cloneMe.penIdx;
            tetIdx = cloneMe.tetIdx;
            triIdx = cloneMe.triIdx;
            vtxIdx = cloneMe.vtxIdx;
        }

    // final three options optional.
        subDivNot( subDivType PT, unsigned long PI, unsigned long TI=0,
                   unsigned long tI=0, unsigned long VI=0 )
        {
            penType = PT;
            penIdx = PI;
            tetIdx = TI;
            triIdx = tI;
            vtxIdx = VI;
        }

        bool operator<(const subDivNot &other) const
        {
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

#if 0 // Not used at present.
        std::string label() const
        {
            std::stringstream retval;
            if (penType==_OP)   retval<<"OP."   <<penIdx; //1
            else if (penType==_CT)   retval<<"CT.P"  <<penIdx<<"T"<<tetIdx; //2
            else if (penType==_CiT)  retval<<"CiT.P" <<penIdx<<"v"<<tetIdx; //3
            else if (penType==_CCt)  retval<<"CCt.P" << penIdx<<"T"<<
                tetIdx<<"t"<<triIdx; //4
            else if (penType==_CCit) retval<<"CCit.P"<< penIdx<<"T"<<
                tetIdx<<"v"<<triIdx; //5
            else if (penType==_CCdt) retval<<"CCdt.P"<< penIdx<<"T"<<
                tetIdx<<"t"<<triIdx<<"v"<<vtxIdx; //6
            return retval.str();
        }
#endif
    }; // end def subDivNot

    inline bool shouldTruncate(Dim4Vertex* v) {
        return (v->isIdeal() || ! v->isValid());
    }
} // anonymous namespace

bool Dim4Triangulation::idealToFinite() {
    bool idVrts(false);
    for (unsigned long i=0; i<getNumberOfVertices(); i++)
        if (shouldTruncate(getVertex(i))) {
            idVrts=true;
            break;
        }
    if (!idVrts) return false;
// * * * Create new triangulation * * *
    Dim4Triangulation* newTri( new Dim4Triangulation );
#ifdef DEBUG
    std::cerr << "Performing idealToFinite()\n";
#endif

// * * * Create the pentachora for the new triangulation * * *
    std::map< subDivNot, Dim4Pentachoron* > newPens; // this will index them.
    for (unsigned long i=0; i<getNumberOfPentachora(); i++) {
        const Dim4Pentachoron* aPen( getPentachoron(i) ); // ambient pent
        bool pIv(false);   // check if has ideal vertices
        for (unsigned long j=0; j<5; j++)
            if (shouldTruncate(aPen->getVertex(j)))
                pIv = true;
        if (!pIv) {
            newPens.insert( std::pair< subDivNot, Dim4Pentachoron* >
                            ( subDivNot( _OP, i ), newTri->newPentachoron() ) );
            continue;
        }
        for (unsigned long j=0; j<5; j++) { // tet / pen vtx loop
            // _CiT check
            if (shouldTruncate(aPen->getVertex(j)))
                newPens.insert( std::pair< subDivNot, Dim4Pentachoron* >
                        ( subDivNot( _CiT, i, j ), newTri->newPentachoron() ) );
            // _CT check
            bool TIv(false); // tet across from j has ideal vertex?
            for (unsigned long k=1; k<5; k++)
                if (shouldTruncate(aPen->getVertex( (j+k) % 5)))
                    TIv = true;
            if (!TIv) {
                newPens.insert( std::pair< subDivNot, Dim4Pentachoron* >
                         ( subDivNot( _CT, i, j ), newTri->newPentachoron() ) );
                continue;
            }
            // we're in situation 4, 5, or 6.
            const Dim4Tetrahedron* aTet( aPen->getTetrahedron(j) );
            for (unsigned long k=0; k<4; k++) {
                if (shouldTruncate(aTet->getVertex(k)))
                    newPens.insert( std::pair< subDivNot, Dim4Pentachoron* >
                     ( subDivNot( _CCit, i, j, k ), 
                       newTri->newPentachoron() ) ); // CCit
                newPens.insert( std::pair< subDivNot, Dim4Pentachoron* > // CCt
                     ( subDivNot( _CCt, i, j, k ), 
                       newTri->newPentachoron() ) );
                bool tIv(false); // check if remaining triangle has 
                for (unsigned long l=1; l<4; l++) // ideal vertex or not.
                    if (shouldTruncate(aTet->getVertex( (k+l) % 4))) tIv = true;
                if (!tIv) continue;
                // the only way we can get here is the triangle has ideal
                // vertices. So we have to subdivide canonically.
                const Dim4Triangle* aTri( aTet->getTriangle(k) );
                for (unsigned long l=0; l<3; l++)
                    if (shouldTruncate(aTri->getVertex(l)) )
                        newPens.insert( std::pair< subDivNot, Dim4Pentachoron* >
                         ( subDivNot( _CCdt, i, j, k, l ), 
                           newTri->newPentachoron() ) );
            } // end k loop
        } // end j loop
    } // end i loop

//                             * * Create the Gluings. * *
//      * * * gluings corresponding to non-boundary tets in original tri * * *
    for (unsigned long i=0; i<getNumberOfTetrahedra(); i++)
        if (!getTetrahedron(i)->isBoundary()) {
            // check if has ideal vertices
            const Dim4Tetrahedron* aTet( getTetrahedron(i) );
            const Dim4TetrahedronEmbedding tEmb0( aTet->getEmbedding(0) );
            const Dim4TetrahedronEmbedding tEmb1( aTet->getEmbedding(1) );

            bool TIv(false);
            for (unsigned long j=0; j<4; j++)
                if (shouldTruncate(aTet->getVertex(j))) {
                    TIv=true;
                    break;
                }
            if (!TIv) { // decide between _OP (1) and _CT (2) for these
                subDivNot p0( _OP, pentachoronIndex( tEmb0.getPentachoron() ) );
                subDivNot p1( _OP, pentachoronIndex( tEmb1.getPentachoron() ) );
                if (shouldTruncate(tEmb0.getPentachoron()->getVertex( 
                    tEmb0.getTetrahedron() ))) {
                    p0.penType = _CT;
                    p0.tetIdx = aTet->getEmbedding(0).getTetrahedron();
                }
                if (shouldTruncate(tEmb1.getPentachoron()->getVertex( 
                    tEmb1.getTetrahedron() ))) {
                    p1.penType = _CT;
                    p1.tetIdx = aTet->getEmbedding(1).getTetrahedron();
                }
#ifdef DEBUG // test to check if p0 and p1 exist 
                if (newPens.find(p0)==newPens.end()) 
                 std::cerr<<"idealToFinite (1) p0 DNE";
                if (newPens.find(p1)==newPens.end()) 
                 std::cerr<<"idealToFinite (1) p1 DNE";
                if (newPens[p0]->adjacentPentachoron( tEmb0.getTetrahedron() )
                    !=NULL)
                    std::cerr<<"idealToFinite (1) p0 GLUED";
                if (newPens[p1]->adjacentPentachoron( tEmb1.getTetrahedron() )
                    !=NULL)
                    std::cerr<<"idealToFinite (1) p1 GLUED";
#endif // and if gluings previously set. 
                newPens[ p0 ]->joinTo( tEmb0.getTetrahedron(), newPens[ p1 ],
                 tEmb0.getPentachoron()->adjacentGluing( 
                  tEmb0.getTetrahedron() ) );
                continue;
            }
            // tet has ideal vertices, so it consists of cones on (perhaps
            // subdivided) triangles, so we're we're gluing (4) tet coords (5) 
            // tc also or (6) tri coords.
            for (unsigned long j=0; j<4; j++) {
                bool tIV(false);   // check if tri across from vertex j has
                for (unsigned long k=1; k<4; k++) // ideal vertices
                    if (shouldTruncate(aTet->getVertex( (j+k) % 4 ))) {
                        tIV=true;
                        break;
                    }
                {
                    subDivNot p0( _CCt, pentachoronIndex(
                                      tEmb0.getPentachoron() ) ); 
                    subDivNot p1( _CCt, pentachoronIndex(
                                      tEmb1.getPentachoron() ) ); 
                    p0.tetIdx = tEmb0.getTetrahedron();
                    p1.tetIdx = tEmb1.getTetrahedron();
                    p0.triIdx = j;
                    p1.triIdx = j;
#ifdef DEBUG // test to check if p0 and p1 exist 
                    if (newPens.find(p0)==newPens.end()) 
                        std::cerr<<"idealToFinite (2) p0 DNE";
                    if (newPens.find(p1)==newPens.end()) 
                        std::cerr<<"idealToFinite (2) p1 DNE";
                    if (newPens[p0]->adjacentPentachoron( 4 )!=NULL)
                        std::cerr<<"idealToFinite (2) p0 GLUED";
                    if (newPens[p1]->adjacentPentachoron( 4 )!=NULL)
                        std::cerr<<"idealToFinite (2) p1 GLUED";
#endif // and if gluings previously set. 
                    newPens[ p0 ]->joinTo( 4, newPens[ p1 ], NPerm5() );
                    if (!tIV) continue;

                    const Dim4Triangle* aTri(aTet->getTriangle(j) );
                    // now the type (6) _CCdt.
                    p0.penType = _CCdt;
                    p1.penType = _CCdt;
                    for (unsigned long k=0; k<3; k++)
                        if (shouldTruncate(aTri->getVertex(k))) {
                            p0.vtxIdx = k;
                            p1.vtxIdx = k; // the tri with no ideal vertices.
#ifdef DEBUG // test to check if p0 and p1 exist 
                            if (newPens.find(p0)==newPens.end()) 
                                std::cerr<<"idealToFinite (3) p0 DNE";
                            if (newPens.find(p1)==newPens.end()) 
                                std::cerr<<"idealToFinite (3) p1 DNE";
                            if (newPens[p0]->adjacentPentachoron( 4 )!=NULL)
                                std::cerr<<"idealToFinite (3) p0 GLUED";
                            if (newPens[p1]->adjacentPentachoron( 4 )!=NULL)
                                std::cerr<<"idealToFinite (3) p1 GLUED";
#endif // and if gluings previously set. 
                            newPens[ p0 ]->joinTo( 4, newPens[ p1 ], NPerm5() );
                        }
                }
                if (shouldTruncate(aTet->getVertex(j))) { // we have a _CCit
                    subDivNot p0( _CCit, pentachoronIndex( 
                        tEmb0.getPentachoron() ) );
                    subDivNot p1( _CCit, pentachoronIndex(
                        tEmb1.getPentachoron() ) );
                    p0.tetIdx = tEmb0.getTetrahedron();
                    p1.tetIdx = tEmb1.getTetrahedron();
                    p0.triIdx = j;
                    p1.triIdx = j;
#ifdef DEBUG // test to check if p0 and p1 exist 
                    if (newPens.find(p0)==newPens.end()) 
                        std::cerr<<"idealToFinite (4) p0 DNE";
                    if (newPens.find(p1)==newPens.end()) 
                        std::cerr<<"idealToFinite (4) p1 DNE";
                    if (newPens[p0]->adjacentPentachoron( 4 )!=NULL)
                        std::cerr<<"idealToFinite (4) p0 GLUED";
                    if (newPens[p1]->adjacentPentachoron( 4 )!=NULL)
                        std::cerr<<"idealToFinite (4) p1 GLUED";
#endif // and if gluings previously set. 
                    newPens[ p0 ]->joinTo( 4, newPens[ p1 ], NPerm5() );
                }
            } // end loop through tet vertices
        } // end look through tets.

// * * * gluings corresponding to subdivision of individual pentachora * * *
    for (unsigned long i=0; i<getNumberOfPentachora(); i++) {
        const Dim4Pentachoron* aPen( getPentachoron(i) );
        bool pIv(false);
        for (unsigned long j=0; j<5; j++)
            if (shouldTruncate(aPen->getVertex(j))) pIv = true;
        if (!pIv) continue; // nothing to do!
        // step 1: all the gluings corresponding to triangle subdivisions, i.e.
        //    all  objects of type (6) CCdt and (4) CCt if on a common pen, 
        //    tet and triangle.
        for (unsigned long j=0; j<5; j++) {
            const Dim4Tetrahedron* aTet( aPen->getTetrahedron( j ) );
            for (unsigned long k=0; k<4; k++) {
                subDivNot p0( _OP, i, j, k ); // will have to
                subDivNot p1( _OP, i, j, k ); // modify later
                const Dim4Triangle* aTri( aTet->getTriangle(k) );
                bool tidV(false);
                for (unsigned long l=0; l<3; l++)
                    if (shouldTruncate(aTri->getVertex(l))) {
                        tidV = true;
                        break;
                    }
                if (!tidV) continue;
                // the triangle has ideal vertices, so there's something to do.
                // gluing pattern CCdt 0 -- CCdt 2 -- CCt -- CCdt 1, if not
                // ideal just erase the CCt uses tet coords.  So we need 
                // the tri inclusion.
                NPerm5 triInc( aTet->getTriangleMapping(k) );
                if (shouldTruncate(aTri->getVertex(1))) { // glue CCdt to CCt.
                    p0.penType = _CCdt;
                    p1.penType = _CCt;
                    p0.vtxIdx = 1;
#ifdef DEBUG // test to check if p0 and p1 exist 
                    if (newPens.find(p0)==newPens.end()) 
                        std::cerr<<"idealToFinite (4) p0 DNE";
                    if (newPens.find(p1)==newPens.end()) 
                        std::cerr<<"idealToFinite (4) p1 DNE";
                    if (newPens[p0]->adjacentPentachoron( 1 )!=NULL)
                        std::cerr<<"idealToFinite (4) p0 GLUED";
                    if (newPens[p1]->adjacentPentachoron( triInc[2] )!=NULL)
                        std::cerr<<"idealToFinite (4) p1 GLUED";
#endif // and if gluings previously set. 
                    newPens[ p0 ]->joinTo( 1, newPens[ p1 ],
                     NPerm5(triInc[0], triInc[2], triInc[1], 
                            triInc[3], triInc[4] ) );
                }
                if (shouldTruncate(aTri->getVertex(2))) { // glue this CCdt to CCt.
                    p0.penType = _CCdt;
                    p1.penType = _CCt;
                    p0.vtxIdx  = 2;
#ifdef DEBUG // test to check if p0 and p1 exist 
                    if (newPens.find(p0)==newPens.end()) 
                        std::cerr<<"idealToFinite (5) p0 DNE";
                    if (newPens.find(p1)==newPens.end()) 
                        std::cerr<<"idealToFinite (5) p1 DNE";
                    if (newPens[p0]->adjacentPentachoron( 2 )!=NULL)
                        std::cerr<<"idealToFinite (5) p0 GLUED";
                    if (newPens[p1]->adjacentPentachoron( triInc[1] )!=NULL)
                        std::cerr<<"idealToFinite (5) p1 GLUED";
#endif // and if gluings previously set. 
                    newPens[ p0 ]->joinTo( 2, newPens[ p1 ],
                     NPerm5( triInc[0], triInc[2], triInc[1], 
                             triInc[3], triInc[4] ) );
                }
                if (shouldTruncate(aTri->getVertex(0))
                        && shouldTruncate(aTri->getVertex(2))) { // glue 0 to 2
                    p0.penType = _CCdt;
                    p1.penType = _CCdt;
                    p0.vtxIdx = 2;
                    p1.vtxIdx = 0;
#ifdef DEBUG // test to check if p0 and p1 exist 
                    if (newPens.find(p0)==newPens.end()) 
                        std::cerr<<"idealToFinite (6) p0 DNE";
                    if (newPens.find(p1)==newPens.end()) 
                        std::cerr<<"idealToFinite (6) p1 DNE";
                    if (newPens[p0]->adjacentPentachoron( 1 )!=NULL)
                        std::cerr<<"idealToFinite (6) p0 GLUED";
                    if (newPens[p1]->adjacentPentachoron( 2 )!=NULL)
                        std::cerr<<"idealToFinite (6) p1 GLUED";
#endif // and if gluings previously set. 
                    newPens[ p0 ]->joinTo( 1, newPens[ p1 ], 
                        NPerm5(0, 2, 1, 3, 4) );
                }
                if (shouldTruncate(aTri->getVertex(0))
                        && ! shouldTruncate(aTri->getVertex(2))) { 
                    p0.penType = _CCdt; // glue 0 CCdt to CCt
                    p1.penType = _CCt;
                    p0.vtxIdx = 0;
                    p1.vtxIdx = 0;
#ifdef DEBUG // test to check if p0 and p1 exist 
                    if (newPens.find(p0)==newPens.end()) 
                        std::cerr<<"idealToFinite (7) p0 DNE";
                    if (newPens.find(p1)==newPens.end()) 
                        std::cerr<<"idealToFinite (7) p1 DNE";
                    if (newPens[p0]->adjacentPentachoron( 2 )!=NULL)
                        std::cerr<<"idealToFinite (7) p0 GLUED";
                    if (newPens[p1]->adjacentPentachoron( triInc[1] )!=NULL)
                        std::cerr<<"idealToFinite (7) p1 GLUED";
#endif // and if gluings previously set. 
                    newPens[ p0 ]->joinTo( 2, newPens[ p1 ],
                      NPerm5( triInc[0], triInc[2], triInc[1], 
                              triInc[3], triInc[4] ) );
                }
            }
        }

        // step 2: glue the types (6) CCdt, (4) CCt and (5) CCit if on common
        //  tet but not on common triangular face of a tet. One gluing for 
        //  every edge of the tet, and for every ideal edge of a triangle in 
        //  the tet, if it exists.
        for (unsigned long j=0; j<5; j++) {
            const Dim4Tetrahedron* aTet( aPen->getTetrahedron(j) );
            bool tIv(false);
            for (unsigned long k=0; k<4; k++)
                if (shouldTruncate(aTet->getVertex(k))) {
                    tIv=true;
                    break;
                }
            if (!tIv) continue;
            // first, we run through the tets of this pentachoron, and check if
            // it has ideal vertices.  If not, jump to next step.
            // (a) glue the CCit's (tet coords) to the CCdt's and CCts 
            //    (tri and tet coords).
            for (unsigned long k=0; k<4; k++)
                if (shouldTruncate(aTet->getVertex(k))) {
                    subDivNot p0( _CCdt, i, j ); // will have to
                    subDivNot p1( _CCit, i, j, k ); // modify later
                    for (unsigned long l=1; l<4; l++) {
                        p0.triIdx = (k+l) % 4;
                        const Dim4Triangle* aTri( 
                            aTet->getTriangle( (k+l) % 4 ) );
                        NPerm5 triInc( aTet->getTriangleMapping( (k+l) % 4 ) );
                        p0.vtxIdx = triInc.preImageOf( k );
                        // figure out gluing map, would seem to depend on 
                        // p1.vtxIdx non-trivially.
                        // p0.vtxIdx= 0, 20    0->triInc[1] 1->p0.triIdx 2->triInc[2] 3->k 4->4
                        //            1, 12    0->p0.triIdx 1->triInc[0] 2->triInc[2] 3->k 4->4
                        //            2, 12    0->p0.triIdx 1->triInc[1] 2->triInc[0] 3->k 4->4
                        unsigned long A( (p0.vtxIdx==0) ? triInc[1] : p0.triIdx );
                        unsigned long B( (p0.vtxIdx==0) ? p0.triIdx : (p0.vtxIdx==1) ?
                                         triInc[0] : triInc[1] );
                        unsigned long C( (p0.vtxIdx==2) ? triInc[0] : triInc[2] );
#ifdef DEBUG // test to check if p0 and p1 exist 
                        if (newPens.find(p0)==newPens.end()) 
                            std::cerr<<"idealToFinite (8) p0 DNE";
                        if (newPens.find(p1)==newPens.end()) 
                            std::cerr<<"idealToFinite (8) p1 DNE";
                        if (newPens[p0]->adjacentPentachoron( 
                            (p0.vtxIdx==0) ? 1 : 0 )!=NULL)
                            std::cerr<<"idealToFinite (8) p0 GLUED";
                        if (newPens[p1]->adjacentPentachoron( 
                            (p0.vtxIdx==0) ? B : A )!=NULL)
                            std::cerr<<"idealToFinite (8) p1 GLUED";
#endif // and if gluings previously set. 
                        newPens[ p0 ]->joinTo( (p0.vtxIdx==0) ? 1 : 0, 
                            newPens[ p1 ], NPerm5( A, B, C, k, 4 ) );
                    }
                }
            // (b) glue the CCdt and CCt's appropriately across tetrahedral
            for (unsigned long k=0; k<6; k++) { // edges.
                // recall aTet is the ambient tet, as we're in the j loop.
                subDivNot p0( _OP, i, j ); // will have to
                subDivNot p1( _OP, i, j ); // modify later
                NPerm5 eMap( aTet->getEdgeMapping(k) ); // eMap[0] eMap[1] edge
                // endpts eMap[2], eMap[3] tri indices.
                NPerm5 triInc2( aTet->getTriangleMapping(
                                    eMap[2] ) ); // natural inclusion to tet.
                NPerm5 triInc3( aTet->getTriangleMapping( eMap[3] ) ); // also.
                p0.triIdx = eMap[2];
                p1.triIdx = eMap[3];
                NPerm5 incPerm0, incPerm1; // relating the pent facets to the
                unsigned long glueT(0); // ambient pent. glue across this tet in
                // pen p0. for tri2 we are gluing a CCt only if either:
                //  (1) triInc2.preImageOf( eMap[3] )==0 or
                //  (2) triInc2[1] non-ideal
                //  (3) *both* triInc2[2] and triInc2[0] nonideal
                if (eMap[3]==triInc2[0] ) { // id vtx 0 in tri2 adj
                    glueT=triInc2[0];
                    p0.penType = _CCt;
                } else if (! shouldTruncate(aTet->getVertex( triInc2[1] )) &&
                           eMap[3]==triInc2[2] ) { // id vtx 1 in tri2 adj
                    glueT=triInc2[2];
                    p0.penType = _CCt;
                } else // id vtx
                    if (! shouldTruncate(aTet->getVertex( triInc2[0] )) &&
                            ! shouldTruncate(aTet->getVertex( triInc2[2] )) &&
                            eMap[3]==triInc2[1] ) {
                        glueT=triInc2[1];
                        p0.penType = _CCt;
                    } else // now the _CCdt
                        if (shouldTruncate(aTet->getVertex( triInc2[1] )) &&
                                eMap[3]==triInc2[2] ) { // CCdt vtx 1
                            glueT=2;
                            p0.penType = _CCdt;
                            incPerm0 = triInc2;
                            p0.vtxIdx = 1;
                        } else if ( shouldTruncate(aTet->getVertex( triInc2[2] )) &&
                                  ! shouldTruncate(aTet->getVertex( triInc2[0] )) &&
                                    eMap[3]==triInc2[1] ) {
                            glueT=1;
                            p0.penType = _CCdt;
                            incPerm0 = triInc2;
                            p0.vtxIdx = 2;
                        } else if ( shouldTruncate(aTet->getVertex( triInc2[0] )) &&
                                    eMap[3]==triInc2[1] ) {
                            glueT=0;
                            p0.penType = _CCdt;
                            incPerm0 = NPerm5(triInc2[1],triInc2[2],triInc2[0], 
                                triInc2[3], triInc2[4] );
                        } else {
                            std::cout<<"Dim4Triangulation::idealToFinite()"<<
                                        " Error 1."<<std::endl;
                            exit(1);
                        }
                if (eMap[2]==triInc3[0]) { // repeating the above for the other
                    p1.penType = _CCt;     // side.
                } else if (! shouldTruncate(aTet->getVertex( triInc3[1] )) &&
                           eMap[2]==triInc3[2] ) { // id vtx 1 in tri2 adj
                    p1.penType = _CCt;
                } else // id vtx
                    if (! shouldTruncate(aTet->getVertex( triInc3[0] )) &&
                            ! shouldTruncate(aTet->getVertex( triInc3[2] )) &&
                            eMap[2]==triInc3[1] ) {
                        p1.penType = _CCt;
                    } else // now the _CCdt
                        if (shouldTruncate(aTet->getVertex( triInc3[1] )) &&
                                eMap[2]==triInc3[2] ) { // CCdt vtx 1
                            p1.penType = _CCdt;
                            incPerm1 = triInc3;
                            p1.vtxIdx = 1;
                        } else if ( shouldTruncate(aTet->getVertex( triInc3[2] )) &&
                                  ! shouldTruncate(aTet->getVertex( triInc3[0] )) &&
                                    eMap[2]==triInc3[1] ) {
                            p1.penType = _CCdt;
                            incPerm1 = triInc3;
                            p1.vtxIdx = 2;
                        } else if ( shouldTruncate(aTet->getVertex( triInc3[0] )) &&
                                    eMap[2]==triInc3[1] ) {
                            p1.penType = _CCdt;
                            incPerm1 = NPerm5(triInc3[1],triInc3[2],triInc3[0], 
                                              triInc3[3], triInc3[4] );
                        } else {
                            std::cout<<"Dim4Triangulation::idealToFinite()"<<
                                        " Error 2."<<std::endl;
                            exit(1);
                        }
#ifdef DEBUG // test to check if p0 and p1 exist 
                if (newPens.find(p0)==newPens.end()) 
                    std::cerr<<"idealToFinite (9) p0 DNE";
                if (newPens.find(p1)==newPens.end()) 
                    std::cerr<<"idealToFinite (9) p1 DNE";
                if (newPens[p0]->adjacentPentachoron( glueT )!=NULL)
                    std::cerr<<"idealToFinite (9) p0 GLUED";
                if (newPens[p1]->adjacentPentachoron( 
                    incPerm1.inverse()[NPerm5(eMap[2],eMap[3])
                        [incPerm0[glueT]]] )!=NULL)
                    std::cerr<<"idealToFinite (9) p1 GLUED";
#endif // and if gluings previously set. 
                newPens[ p0 ]->joinTo( glueT, newPens[ p1 ],
                   incPerm1.inverse()*NPerm5(eMap[2], eMap[3])*incPerm0 );
            }
        }

        // step 3: glue types in common pen but not in common tet facet.
        // we have an ideal vertex, so this pen is subdivided at the barycentre.
        for (unsigned long j=0; j<5; j++)
            if (shouldTruncate(aPen->getVertex(j))) {
                // **all** CiT type (4) gluings done
                subDivNot p0( _CiT, i, j ); // uses pen coords
                subDivNot p1( _CCit, i );   // uses tet coords.
                for (unsigned long k=1; k<5; k++) { // gluing for tet j+k % 5.
                    NPerm5 tetInc( aPen->getTetrahedronMapping( (j+k) % 5 ) );
                    const Dim4Tetrahedron* aTet( aPen->getTetrahedron( (j+k) % 5 ) );
                    p1.tetIdx = (j+k) % 5;
                    p1.triIdx = tetInc.preImageOf( j ); // the ideal triangle
#ifdef DEBUG // test to check if p0 and p1 exist 
                    if (newPens.find(p0)==newPens.end()) 
                        std::cerr<<"idealToFinite (10) p0 DNE";
                    if (newPens.find(p1)==newPens.end()) 
                        std::cerr<<"idealToFinite (10) p1 DNE";
                    if (newPens[p0]->adjacentPentachoron( (j+k)%5 )!=NULL)
                        std::cerr<<"idealToFinite (10) p0 GLUED";
                    if (newPens[p1]->adjacentPentachoron(
                            tetInc.inverse()[j] )!=NULL)
                        std::cerr<<"idealToFinite (10) p1 GLUED";
#endif // and if gluings previously set. 
                    newPens[ p0 ]->joinTo( (j+k)%5, newPens[ p1 ], 
                        tetInc.inverse()*NPerm5(j, (j+k)%5) );
                }
            }
        // we need to run through the triangles (equiv edges) of the pentachoron
        // and perform all of the appropriate gluings between tetrahedra.
        // step (a) check if it has ideal vertices.  If not, we are done!

        for (unsigned long j=0; j<10; j++) {
            NPerm5 triInc( aPen->getTriangleMapping( j ) );
            const Dim4Triangle* aTri( aPen->getTriangle( j ) );
            // triInc[0 1 2] are the triangle vertices, [3 4] the vertices of the
            //  opposite edge. So we are gluing all the facets of tet's 3 and 4 which
            //  are incident to the triangle 012.
            NPerm5 tet0inc( aPen->getTetrahedronMapping( triInc[3] ) );
            NPerm5 tet1inc( aPen->getTetrahedronMapping( triInc[4] ) );
            unsigned long tri0idx( tet0inc.preImageOf( triInc[4] ) );
            unsigned long tri1idx( tet1inc.preImageOf( triInc[3] ) );
            const Dim4Tetrahedron* tet0( aPen->getTetrahedron( triInc[3] ) );
            const Dim4Tetrahedron* tet1( aPen->getTetrahedron( triInc[4] ) );
            NPerm5 tri0inc( tet0->getTriangleMapping( tri0idx ) );
            NPerm5 tri1inc( tet1->getTriangleMapping( tri1idx ) );
            // check if the triangle has any ideal vertices
            bool tIv(false);
            for (unsigned long k=0; k<3; k++)
                if (shouldTruncate(aTri->getVertex(k))) {
                    tIv=true;
                    break;
                }
            // if tIv true, both sides are type CCt.
            bool s0it(false), s1it(false); // check if adjacent tets have ideal
            for (unsigned long k=0; k<4; k++) { // vertices
                if (shouldTruncate(aPen->getVertex(tet0inc[k]))) s0it=true;
                if (shouldTruncate(aPen->getVertex(tet1inc[k]))) s1it=true;
            }
            subDivNot p0( s0it ? _CCt : _CT, i, triInc[3], s0it ? tri0idx : 0 );
            subDivNot p1( s1it ? _CCt : _CT, i, triInc[4], s1it ? tri1idx : 0 );
#ifdef DEBUG // test to check if p0 and p1 exist 
            if (newPens.find(p0)==newPens.end()) 
                std::cerr<<"idealToFinite (11) p0 DNE";
            if (newPens.find(p1)==newPens.end()) 
                std::cerr<<"idealToFinite (11) p1 DNE";
            if (newPens[p0]->adjacentPentachoron( 
                s0it ? tri0idx : triInc[4] )!=NULL)
                std::cerr<<"idealToFinite (11) p0 GLUED";
            NPerm5 TP( (s1it ? tet1inc : NPerm5()).inverse()*
              NPerm5(triInc[3], triInc[4])*(s0it ? tet0inc : NPerm5()) );
            if (newPens[p1]->adjacentPentachoron( TP[s0it ? tri0idx : triInc[4]]
                                                )!=NULL)
                std::cerr<<"idealToFinite (11) p1 GLUED";
#endif // and if gluings previously set. 
            newPens[ p0 ]->joinTo( s0it ? tri0idx : triInc[4] , newPens[ p1 ],
                       (s1it ? tet1inc : NPerm5()).inverse()*NPerm5(triInc[3],
                                      triInc[4])*(s0it ? tet0inc : NPerm5()));
            if (!tIv) continue;
            for (unsigned long k=0; k<3; k++)
                if (shouldTruncate(aTri->getVertex(k))) {
                    // _CCdt uses triangle coordinates, which are fixed.
                    p0.penType =_CCdt;
                    p1.penType = _CCdt;
                    p0.vtxIdx  = k;
                    p1.vtxIdx  = k;
#ifdef DEBUG // test to check if p0 and p1 exist 
                    if (newPens.find(p0)==newPens.end()) 
                        std::cerr<<"idealToFinite (12) p0 DNE";
                    if (newPens.find(p1)==newPens.end()) 
                        std::cerr<<"idealToFinite (12) p1 DNE";
                    if (newPens[p0]->adjacentPentachoron( 3 )!=NULL)
                        std::cerr<<"idealToFinite (12) p0 GLUED";
                    if (newPens[p1]->adjacentPentachoron( 3 )!=NULL)
                        std::cerr<<"idealToFinite (12) p1 GLUED";
#endif // and if gluings previously set. 
                    newPens[ p0 ]->joinTo( 3, newPens[ p1 ], NPerm5() );
                }
        } // j loop -- edges
    }
    swapContents( *newTri );
    delete newTri;
    return true;
}

namespace { // unnamed namespace for private structs for divideEdge

// this is intended as a bitmask version of the old simpFacet class. Describes
// a barycentre of a facet of a simplex. aDim is the hard-coded dimension limit
// aDim==5 means 4-simplices are currently the max.
struct bCtr {
 static const unsigned long aDim = 5; // we're at most dealing with 4-simplices.
 unsigned long code; // bitmask describing which vertices this is the
  // barycenter of. 
 bCtr() { code = 0; } // null constructor
 static bCtr byCode(const unsigned long &num) // define by code.
  { bCtr retval; retval.code = num; return retval; }
 bCtr(const bCtr &oth) { code = oth.code; } // copy constructor
 bCtr(const unsigned long &fac0)  { code = (1 << fac0); }
 bCtr(const unsigned long &fac0, const unsigned long &fac1)
  { code = (1 << fac0) | (1 << fac1); }
 bCtr(const unsigned long &fac0, const unsigned long &fac1, 
      const unsigned long &fac2)
  { code = (1 << fac0) | (1 << fac1) | (1 << fac2); }
 bCtr(const unsigned long &fac0, const unsigned long &fac1, 
      const unsigned long &fac2, const unsigned long &fac3)
  { code = (1 << fac0) | (1 << fac1) | (1 << fac2) | (1 << fac3); }
 bCtr(const unsigned long &fac0, const unsigned long &fac1, 
      const unsigned long &fac2, const unsigned long &fac3, const unsigned long &fac4)
  { code = (1 << fac0) | (1 << fac1) | (1 << fac2) | (1 << fac3) | (1 << fac4); }
 bCtr pushForward(const NPerm5 per) const
  { // push forward 
   bCtr retval;
   for (unsigned long i=0; i<aDim; i++) if ( code & (1 << i) )
    retval.code = retval.code | (1 << per[i]);
   return retval;   
  }
 bCtr pullBack(const NPerm5 per) const
  { // pull back
   bCtr retval;
   for (unsigned long i=0; i<aDim; i++) if ( code & (1 << per[i]) )
    retval.code = retval.code | (1 << i);
   return retval;   
  }
 unsigned long dFac() const
  {
   unsigned long retval(0);
   for (unsigned long i=0; i<aDim; i++) if ( code & (1 << i) ) retval++;
   return retval; 
  }
 bool operator<(const bCtr &oth) const
  { // let's try a lexico sort on the dimension of the facets. 
   if (dFac() < oth.dFac()) return true;
   if (dFac() > oth.dFac()) return false;
   if (code < oth.code) return true;
   return false;
   // return code < oth.code; // this is the old sort. 
  }
}; 

// std output << operator
std::ostream& operator<<(std::ostream& out, const bCtr &c) {
 out << "<"; 
 for (unsigned long i=0; i<bCtr::aDim; i++) out<<( ( (1 << i) & c.code) ? "1" : "0" );
 out << ">";
 return out;
}

// to be a replacement for the vtxSet struct.
struct simpSubFac { // a simplicial sub-facet of a simplex. This encodes any
 // sub-simplex of a simplex whose vertices consist of barycentres of the 
 // facets of the original simplex. 
 //
 // it is up to the user to decide what the ambient dimension of the simplex
 // is.  currently dim 4 is a hard-coded max, see aDim in the bCtr struct.
 std::list< bCtr > vtxset;
 // copy constructor
 simpSubFac(const simpSubFac &other) : vtxset( other.vtxset ) {}
 // empty constructor
 simpSubFac() {}
 // the not-subdivided n-dimensional simplex
 simpSubFac(const unsigned long aDim)
  { for (unsigned long i=0; i<=aDim; i++) vtxset.push_back( bCtr(i) ); }
 // check to see if facet fac is part of this simplex
 bool hasFacets( const std::list< bCtr > &fac ) const
  { // both vtxset and fac must be sorted.
   return std::includes( vtxset.begin(), vtxset.end(), 
                         fac.begin(), fac.end() );
  }
 // if facet inF appears, replace it by outF.  Return true if successful.
 bool replaceFacet( const bCtr &inF, const bCtr &outF )
  {
   for (std::list< bCtr >::iterator i=vtxset.begin(); i!=vtxset.end(); i++)
    if ( !((*i) < inF ) && !(inF < (*i)) )
     {
      vtxset.erase(i);
      vtxset.push_back( outF );
      vtxset.sort();
      return true;
     }
   return false;
  }
 // for sorting
 bool operator<(const simpSubFac &other) const
  {
    std::list< bCtr >::const_iterator i=vtxset.begin();
    std::list< bCtr >::const_iterator j=other.vtxset.begin();
    while ( (i!=vtxset.end()) && (j!=other.vtxset.end()) )
     {if ( (*i < *j) ) return true;  if ( (*j < *i) ) return false;
      i++; j++; }
    if ( (i == vtxset.end()) && (j != other.vtxset.end()) ) return true; 
    return false;
  }
 // warning, this routine does not sort -- up to user to do if neccessary.
 void coneOn( const bCtr &conePt )
  {   vtxset.push_back( conePt );  }
 simpSubFac pushForward( const NPerm5 &per ) const
  {
   simpSubFac retval;
   for (std::list< bCtr >::const_iterator i = vtxset.begin(); i!=vtxset.end(); i++)
    retval.vtxset.push_back( i->pushForward(per) ); 
   retval.vtxset.sort();
   return retval;
  }
 simpSubFac pullBack( const NPerm5 &per ) const
  {
   simpSubFac retval;
   for (std::list< bCtr >::const_iterator i = vtxset.begin(); i!=vtxset.end(); i++)
    retval.vtxset.push_back( i->pullBack(per) ); 
   retval.vtxset.sort();
   return retval;
  }
};

// std output << operator
std::ostream& operator<<(std::ostream& out, const simpSubFac &f) {
 out << "{"; 
 for (std::list< bCtr >::const_iterator i=f.vtxset.begin(); i!=f.vtxset.end(); i++) 
  {
   if (i!=f.vtxset.begin()) out<<","; 
   out<<(*i);
  }
 out << "}";
 return out;
}

} // end unnamed namespace

// TODO: let's completely re-do the algorithm.  We'll divide the edges that
//  need to be divided, and then call incidence relations to flesh-out division
//  of higher-dimensional simplices.  Currently it's overly-wasteful and needing
//  to deal with special cases. This way there's two things to check at every
//  stage: the divisions of the lower-dimensional objects and whether or not the
//  lower-dimensional facets are identified. 

// We subdivide inductively from lowest to highest dimension.  We keep track of
//  equivalence classes of vertices of an n-facet by a partition of n+1 points. 
//  Thus an edge can be labelled (1,1) or (2), indicating distinct vertices or
//  glued vertices respectively. 

bool Dim4Triangulation::makeEdgeEndpointsDistinct()
{
  std::vector< std::list< simpSubFac > > penSubDiv( getNumberOfPentachora() );
  // Step 1: Determine what kind of subdivision each pentachoron needs. 
  //   Cases (1,1,1,1,1), (2,1,1,1), (2,2,1), (3,1,1), (3,2), (4,1), (5). 
  //  For this we can have a ?map? from the actual vertex pointers to the
  //   vertex indices in the pentachoron.
  for (unsigned long i=0; i<getNumberOfPentachora(); i++)
   {
    const Dim4Pentachoron* pen( getPentachoron(i) );
    std::map< const Dim4Vertex* , std::set<unsigned long> > vrtCt;
    for (unsigned long j=0; j<5; j++) 
     vrtCt[ pen->getVertex(j) ].insert( j );    
    // Step 2: perform subdivision 

    if (vrtCt.size() == 5) // all distinct, standard pen, no division.
       penSubDiv[i].push_back( simpSubFac(4) );
    else if (vrtCt.size() == 4) // pair glued
      { // split in half.  Two pentachora to add to penSubDiv[i]
        //  both share the 1,1,1 vertices, and the 2 barycentre, one
        //  has one end of the 2, the other has the other. 
        simpSubFac pen1, pen2;
        for (std::map< const Dim4Vertex*, std::set< unsigned long> >::iterator j=vrtCt.begin();
             j!=vrtCt.end(); j++)
         if (j->second.size()==1) { pen1.vtxset.push_back(*(j->second.begin()));
            pen2.vtxset.push_back(*(j->second.begin())); } 
         else { // two element set, find them both. 
          std::set< unsigned long >::iterator k=j->second.begin();
          unsigned long v0( *k ); k++;
          unsigned long v1( *k );
          pen1.vtxset.push_back( v0 ); pen1.vtxset.push_back(bCtr(v0, v1));
          pen2.vtxset.push_back( v1 ); pen1.vtxset.push_back(bCtr(v0, v1));
         }
       penSubDiv[i].push_back( pen1 ); 
       penSubDiv[i].push_back( pen2 );
      }
    else if (vrtCt.size() == 3) // (2,2,1) or (3,1,1)
      { // TODO
      }
    else if (vrtCt.size() == 2) // (3,2) or (4,1)
      { // TODO
      }
    else // all glued.
      { // TODO
      }
   }


  // Step 3: Indexing.

  std::vector< std::map< simpSubFac, unsigned long > > 
        divPIdx( getNumberOfPentachora() );
  std::vector< std::map< std::pair< simpSubFac, bCtr >, unsigned long > > 
        divPvIdx( getNumberOfPentachora() );

  // Step 4: glue all the new pentachora.
  Dim4Triangulation newTri; // we will use swapContents with *this to turn
   // our new triangulation into the ambient triangulation.
  unsigned long count(0); // for building divPIdx
  for (unsigned long i=0; i<getNumberOfPentachora(); i++)
   { // all the gluings inside getSimplex(i), penSubdiv[i].
    const Dim4Pentachoron* pen( getSimplex(i) );
    // index the elements of penSubDiv[] lexicographically.
    for (std::list< simpSubFac >::iterator j=penSubDiv[i].begin(); 
         j!=penSubDiv[i].end(); j++)
     {
      divPIdx[i].insert( std::pair< simpSubFac, unsigned long >( *j, count ) );
      count++; 
      newTri.newSimplex(); // divPIdx is the index of this pentachoron.
      // now index the vertices of this object.
      unsigned long subcount(0);
      for (std::list< bCtr >::iterator k=j->vtxset.begin(); k!=j->vtxset.end(); 
           k++)
       {
        divPvIdx[i].insert( 
         std::pair< std::pair< simpSubFac, bCtr >, unsigned long >
          (std::pair< simpSubFac, bCtr >( *j, *k ), subcount ) );
        subcount++;
       }
     } // end j loop
   } // end i loop -- indexing

  std::vector< std::map< simpSubFac, std::list< 
               std::pair< simpSubFac, bCtr > > > > 
        incidPen(getNumberOfPentachora()); 
  // pents incident to the tetrahedra via the boundary relation, the 2nd element
  // of the pair is the vertex we have to eliminate to get the vtxSet in the 
  // domain of the map.

  // preliminary step to build incidPen
  for (unsigned long i=0; i<getNumberOfPentachora(); i++)
   { 
    for (std::list< simpSubFac >::iterator j=penSubDiv[i].begin(); 
         j!=penSubDiv[i].end(); j++)
     for (unsigned long k=0; k<5; k++)
     {
      simpSubFac delK( *j );// delete k-th element of *j
      for (std::list< bCtr >::iterator l=delK.vtxset.begin(); 
           l!=delK.vtxset.end(); l++)
       if ( divPvIdx[i][std::pair< simpSubFac, bCtr >(*j, *l)] == k )
         {
         bCtr oldL( *l );
         delK.vtxset.erase( l );
         incidPen[i][ delK ].push_back( 
          std::pair< simpSubFac, bCtr >(*j, oldL) );
         break;
         }
#ifdef DEBUG
     if (delK.vtxset.size() != 4) { 
        std::cout<<"subdivide delK size error."<<std::endl; 
        exit(1); 
     }
#endif
     } // end k loop
    // (a) internal gluings
    for (std::map< simpSubFac, 
         std::list< std::pair< simpSubFac, bCtr > > >::iterator 
           j=incidPen[i].begin();
         j!=incidPen[i].end(); j++)
     if (j->second.size()==2) // we have a gluing.  Perform!
     { // pen vtx for first incident object
      simpSubFac pen0vtcs( j->second.front().first ); 
      bCtr pen0ofac( j->second.front().second );
      j->second.pop_front();
       // pen vtx for second incident object
      simpSubFac pen1vtcs( j->second.front().first ); 
      bCtr pen1ofac( j->second.front().second );
      j->second.pop_front();
      unsigned long pen0idx( divPIdx[i][pen0vtcs] );
      unsigned long pen1idx( divPIdx[i][pen1vtcs] );
      std::vector< unsigned long > vrtsIn0, vrtsIn1;
      vrtsIn0.reserve(5); vrtsIn1.reserve(5);
      vrtsIn0.push_back( divPvIdx[i][std::pair< simpSubFac, bCtr >
        (pen0vtcs, pen0ofac) ]);
      vrtsIn1.push_back( divPvIdx[i][std::pair< simpSubFac, bCtr >
        (pen1vtcs, pen1ofac) ]);
      for (std::list< bCtr >::const_iterator k=j->first.vtxset.begin(); 
           k!=j->first.vtxset.end(); k++)
       vrtsIn0.push_back( divPvIdx[i][std::pair< simpSubFac, bCtr >
        (pen0vtcs, *k) ]);
      for (std::list< bCtr >::const_iterator k=j->first.vtxset.begin(); 
           k!=j->first.vtxset.end(); k++)
       vrtsIn1.push_back( divPvIdx[i][std::pair< simpSubFac, bCtr >
        (pen1vtcs, *k) ]);
      NPerm5 glueMap( vrtsIn0[0], vrtsIn1[0], vrtsIn0[1], vrtsIn1[1], 
       vrtsIn0[2], vrtsIn1[2], vrtsIn0[3], vrtsIn1[3], vrtsIn0[4], vrtsIn1[4] );
      newTri.getSimplex( pen0idx )->joinTo( vrtsIn0[0], 
        newTri.getSimplex( pen1idx ), glueMap );
     }
    else
     {
#ifdef DEBUG
      // sanity check that j->second.size()==1
      if (j->second.size()!=1) { 
        std::cout<<"divideEdge() facet count error!"<<std::endl; 
        exit(1); 
      }
#endif
      // check which tetrahedral facet this boundary facet is in...
      simpSubFac pen0vtcs( j->second.front().first ); 
      bCtr pen0ofac( j->second.front().second );
      j->second.pop_front();
#ifdef DEBUG
      if (pen0ofac.code != 31) { 
        std::cout<<"divideEdge() ambient pentachoral gluing across facet "<<
                 <<"that is not <11111>."<<std::endl; 
      exit(1); 
      }
#endif
      unsigned long pen0idx( divPIdx[i][pen0vtcs] );
      // step 1: if already glued, let's skip to the end.
      if (newTri.getSimplex( pen0idx )->adjacentSimplex(  
       divPvIdx[i][std::pair< simpSubFac, bCtr >(pen0vtcs, pen0ofac) ] )!=NULL )
       continue;
      // let's find the ambient tetrahedron. We are in pen i.  j->first is the 
      // simpSubFac of the subtet.  So we'll take the union of the codes and 
      // that should be the vertices of the ambient tet in pen i. 
      unsigned long code(0);
      for (std::list< bCtr >::const_iterator k=j->first.vtxset.begin(); 
           k!=j->first.vtxset.end(); k++)
       code = code | k->code;
      // code is a bitfield with 5 bits, only one should be zero.  
      // Let's figure that out
      unsigned long vidx(0), zcount(0);
      for (unsigned long k=0; k<5; k++) if ( !((1 << k) & code) )
       { zcount++; vidx = k; }
#ifdef DEBUG
      if (zcount != 1) { std::cout<<"zcount error!"<<std::endl; 
         std::cout<<"in apen "<<i<<" pen idx "<<pen0idx<<" gluing across ";
         std::cout<<pen0ofac<<" and tet face consists of "<<j->first<<std::endl;
         exit(1); 
      }
#endif
      // abort if this tetrahedron is a boundary tetrahedron. 
      if (getSimplex(i)->adjacentSimplex(vidx) == NULL) continue;

      // find out what its glued to, and push out subfacet forward
      //       quit if the pen index l>i. 
      unsigned long gPenIdx( 
        simplexIndex( getSimplex(i)->adjacentSimplex(vidx) ) );
      NPerm5 gPenGlue( getSimplex(i)->adjacentGluing(vidx) );
      if (gPenIdx > i) continue; // redundant gluing.
      simpSubFac pushedFac( pen0vtcs.pushForward( gPenGlue ) );
      unsigned long pushedIdx( divPIdx[gPenIdx][pushedFac] ); 
      // the subpentachoron index in newTri
#ifdef DEBUG
      if (newTri.getSimplex( pushedIdx )==NULL)
       { std::cout<<"simplex does not exist!"<<std::endl; exit(1); }
#endif      
      std::vector< unsigned long > vlist; // this is to be the gluing
        // permutation, set up in vector form...
      vlist.reserve(5);

      for (std::list< bCtr >::iterator X=pen0vtcs.vtxset.begin(); 
           X!=pen0vtcs.vtxset.end(); X++)
        vlist.push_back( divPvIdx[gPenIdx][std::pair< simpSubFac, bCtr >
        (pushedFac,X->pushForward(gPenGlue))] );
#ifdef DEBUG
      for (unsigned long x=0; x<5; x++) for (unsigned long y=x+1; y<5; y++)
       if (vlist[x]==vlist[y])
        { std::cout<<"Non permutation error."<<std::endl; exit(1); }
#endif

      newTri.getSimplex( pen0idx )->joinTo( 
        divPvIdx[i][std::pair< simpSubFac, bCtr >(pen0vtcs, pen0ofac)],  
        newTri.getSimplex( pushedIdx ),  
        NPerm5( vlist[0], vlist[1], vlist[2], vlist[3], vlist[4] ) ); 

     } // end facet only has one gluing fork.
   } // end i loop pentachora internal gluings

 swapContents( newTri );
 return true;
} // end divideEdges


} // end namespace regina


