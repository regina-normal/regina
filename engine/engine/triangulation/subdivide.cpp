
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "triangulation/ntriangulation.h"
#include "utilities/memutils.h"

// The indices of the new tetrahedra
static int tetIndex[4][4][4] = {
    { {-1,-1,-1,-1}, {-1,-1,0,1}, {-1,2,-1,3}, {-1,4,5,-1} },
    { {-1,-1,6,7}, {-1,-1,-1,-1}, {8,-1,-1,9}, {10,-1,11,-1} },
    { {-1,12,-1,13}, {14,-1,-1,15}, {-1,-1,-1,-1}, {16,17,-1,-1} },
    { {-1,18,19,-1}, {20,-1,21,-1}, {22,23,-1,-1}, {-1,-1,-1,-1} } };


void NTriangulation::barycentricSubdivision() {
    // Written by Dave Letscher  11/2/00

    unsigned long nOldTet = tetrahedra.size();
    NTetrahedron** newTet = new (NTetrahedron*)[nOldTet * 24];
    NTetrahedron* oldTet;
    NPerm p;
    unsigned long tet;
    int face, edge, corner, other;
    for (tet=0; tet<24*nOldTet; tet++)
        newTet[tet] = new NTetrahedron();

    // Do all of the internal gluings
    for (tet=0; tet<nOldTet; tet++) {
        for (face=0; face<4; face++)
            for (edge=0; edge<4; edge++)
                if (edge != face)
                    for (corner=0; corner<4; corner++)
                        if ( (face != corner) && (edge != corner) ) {
                            other = 6-face-edge-corner;

                            // Glue to the tetrahedron on the same face and
                            // on the same edge
                            newTet[24*tet+tetIndex[face][edge][corner]]->
                                joinTo(corner,
                                newTet[24*tet+tetIndex[face][edge][other]],
                                NPerm(corner,other) );
                        
                            // Glue to the tetrahedron on the same face and
                            // at the same corner
                            newTet[24*tet+tetIndex[face][edge][corner]]->
                                joinTo(other,
                                newTet[24*tet+tetIndex[face][other][corner]],
                                NPerm(edge,other) );
                        
                            // Glue to the tetrahedron on the adjacent face
                            // sharing an edge and a vertex
                            newTet[24*tet+tetIndex[face][edge][corner]]->
                                joinTo(edge,
                                newTet[24*tet+tetIndex[edge][face][corner]],
                                NPerm(face, edge) );

                            // Glue to the new tetrahedron across an existing
                            // face
                            oldTet = getTetrahedron(tet);
                            if (oldTet->getAdjacentTetrahedron(face)) {
                                p = oldTet->getAdjacentTetrahedronGluing(face);
                                newTet[24*tet+tetIndex[face][edge][corner]]->
                                    joinTo(face, newTet[24*getTetrahedronIndex(
                                    oldTet->getAdjacentTetrahedron(face))+
                                    tetIndex[p[face]][p[edge]][p[corner]] ],
                                    NPerm(p) );
                             }
                        }
    }



    // Delete the existing tetrahedra and put in the new ones.
    removeAllTetrahedra();
    for (tet=0; tet<24*nOldTet; tet++)
        addTetrahedron(newTet[tet]);

    delete[] newTet;
}

bool NTriangulation::idealToFinite(bool forceDivision) {
    // The call to isValid() ensures the skeleton has been calculated.
    if (isValid() && ! isIdeal())
        if (! forceDivision)
            return false;
    
    int i,j,k,l;
    int numOldTet = tetrahedra.size();
    
    NTetrahedron **newTet = new (NTetrahedron *)[32*numOldTet];
    for (i=0; i<32*numOldTet; i++)
        newTet[i] = new NTetrahedron();
    
    int tip[numOldTet][4];
    int interior[numOldTet][4];
    int edge[numOldTet][4][4];
    int vertex[numOldTet][4][4];
    
    l = 0;
    for (i=0; i<numOldTet; i++) {
        for (j=0; j<4; j++) {
            tip[i][j] = l++;            
            interior[i][j] = l++;
            
            for (k=0; k<4; k++)
                if (j != k) {
                    edge[i][j][k] = l++;                    
                    vertex[i][j][k] = l++;
                }
        }
    }
        
    // First glue all of the tetrahedra inside the same
    // old tetrahedron together.
    for (i=0; i<numOldTet; i++) {
        // Glue the tip tetrahedra to the others.
        for (j=0; j<4; j++)
            newTet[tip[i][j]]->joinTo(j, newTet[interior[i][j]], NPerm());        
        
        // Glue the interior tetrahedra to the others.
        for (j=0; j<4; j++) {
            for (k=0; k<4; k++)
                if (j != k) {
                    newTet[interior[i][j]]->joinTo(k,
                        newTet[vertex[i][k][j]], NPerm());
                }
        }
        
        // Glue the edge tetrahedra to the others.
        for (j=0; j<4; j++)
            for (k=0; k<4; k++)
                if (j != k) {
                    newTet[edge[i][j][k]]->joinTo(j,
                        newTet[edge[i][k][j]], NPerm(j,k));
                    
                    for (l=0; l<4; l++)
                        if ( (l != j) && (l != k) )
                            newTet[edge[i][j][k]]->joinTo(l,
                                newTet[vertex[i][j][l]], NPerm(k,l));
                }
    }

    // Now deal with the gluings between the pieces inside adjacent tetrahedra.
    NTetrahedron *ot;
    int oppTet;
    NPerm p;
    for (i=0; i<numOldTet; i++) {
        ot = getTetrahedron(i);
        for (j=0; j<4; j++)
            if (ot->getAdjacentTetrahedron(j)) {
                 oppTet = getTetrahedronIndex(ot->getAdjacentTetrahedron(j));
                 p = ot->getAdjacentTetrahedronGluing(j);
                 
                 // First deal with the tip tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                          newTet[tip[i][k]]->joinTo(j,
                              newTet[tip[oppTet][p[k]]], p);
                 
                 // Next the edge tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                         newTet[edge[i][j][k]]->joinTo(k,
                             newTet[edge[oppTet][p[j]][p[k]]], p);
                 
                 // Finally, the vertex tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                         newTet[vertex[i][j][k]]->joinTo(k,
                             newTet[vertex[oppTet][p[j]][p[k]]], p);
                         
            }
    }

    removeAllTetrahedra();    
    
    for (i=0; i<32*numOldTet; i++)
        addTetrahedron(newTet[i]);
    
    gluingsHaveChanged();
    
    // Remove the tetrahedra that meet any of the non-standard or
    // ideal vertices.
    // First we make a list of the tetrahedra.
    std::hash_set<NTetrahedron*, HashPointer> tetList;
    std::vector<NVertexEmbedding>::const_iterator vembit;
    for (VertexIterator vIter = vertices.begin();
            vIter != vertices.end(); vIter++)
        if ((*vIter)->isIdeal() || ! (*vIter)->isStandard())
            for (vembit = (*vIter)->getEmbeddings().begin();
                    vembit != (*vIter)->getEmbeddings().end(); vembit++)
                tetList.insert((*vembit).getTetrahedron());
    
    // Now remove the tetrahedra.
    // For each tetrahedron, remove it and delete it.
    for_each(tetList.begin(), tetList.end(),
        compose1(FuncDelete<NTetrahedron>(),
        std::bind1st(std::mem_fun(&NTriangulation::removeTetrahedron), this)));

    gluingsHaveChanged();
    return true;
}
