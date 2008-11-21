
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "surfaces/nnormalsurface.h"
#include "surfaces/ndisc.h"

namespace regina {

std::ostream& operator << (std::ostream& out, const NDiscSpec& spec) {
    out << '(' << spec.tetIndex << ", " << spec.type << ", "
        << spec.number << ')';
    return out;
}

bool numberDiscsAwayFromVertex(int discType, int vertex) {
    if (discType < 4)
        return (vertex == discType);
    return (vertex == 0 || vertex ==
        vertexSplitPartner[(discType - 1) % 3][0]);
}

bool discOrientationFollowsEdge(int discType, int vertex, int edgeStart,
        int edgeEnd) {
    NPerm forwards(vertex, edgeStart, edgeEnd, 6-vertex-edgeStart-edgeEnd);
    NPerm reverse(vertex, edgeEnd, edgeStart, 6-vertex-edgeStart-edgeEnd);

    if (discType < 4) {
        for (int i = 0; i < 3; i++) {
            if (triDiscArcs(discType, i) == forwards)
                return true;
            if (triDiscArcs(discType, i) == reverse)
                return false;
        }
    }
    else if (discType < 7) {
        for (int i = 0; i < 4; i++) {
            if (quadDiscArcs(discType - 4, i) == forwards)
                return true;
            if (quadDiscArcs(discType - 4, i) == reverse)
                return false;
        }
    }
    else {
        for (int i = 0; i < 8; i++) {
            if (octDiscArcs(discType - 7, i) == forwards)
                return true;
            if (octDiscArcs(discType - 7, i) == reverse)
                return false;
        }
    }
    return false;
}

NDiscSetTet::NDiscSetTet(const NNormalSurface& surface,
        unsigned long tetIndex) {
    int i;
    for (i=0; i<4; i++)
        internalNDiscs[i] = surface.getTriangleCoord(tetIndex, i).longValue();
    for (i=4; i<7; i++)
        internalNDiscs[i] = surface.getQuadCoord(tetIndex, i - 4).longValue();
    for (i=7; i<10; i++)
        internalNDiscs[i] = surface.getOctCoord(tetIndex, i - 7).longValue();
}

unsigned long NDiscSetTet::arcFromDisc(int /* arcFace */, int arcVertex,
        int discType, unsigned long discNumber) const {
    // Is it a triangle?
    if (discType < 4)
        return discNumber;

    // It's a quad or an octagon.
    // Note that there is at most one octagonal or quad type present
    // (since the surface must be embedded), so this must be it.
    if (arcVertex == 0 || arcVertex ==
            vertexSplitPartner[(discType - 1) % 3][0])
        return internalNDiscs[arcVertex] + discNumber;
    else
        return internalNDiscs[arcVertex] + internalNDiscs[discType]
            - discNumber - 1;
}

void NDiscSetTet::discFromArc(int arcFace, int arcVertex,
        unsigned long arcNumber,
        int& discType, unsigned long& discNumber) const {
    // Is it a triangle?
    if (arcNumber < internalNDiscs[arcVertex]) {
        discType = arcVertex;
        discNumber = arcNumber;
        return;
    }

    // It's a quad or an octagon.
    // Note that there is at most one octagonal or quad type present
    // (since the surface must be embedded), so this must be it.
    if (internalNDiscs[vertexSplit[arcVertex][arcFace] + 4] > 0)
        discType = vertexSplit[arcVertex][arcFace] + 4;
    else if (internalNDiscs[vertexSplitMeeting[arcVertex][arcFace][0] + 7] > 0)
        discType = vertexSplitMeeting[arcVertex][arcFace][0] + 7;
    else
        discType = vertexSplitMeeting[arcVertex][arcFace][1] + 7;

    if (arcVertex == 0 || arcVertex ==
            vertexSplitPartner[(discType - 1) % 3][0])
        discNumber = arcNumber - internalNDiscs[arcVertex];
    else
        discNumber = internalNDiscs[discType] -
            (arcNumber - internalNDiscs[arcVertex]) - 1;
}

NDiscSetSurface::NDiscSetSurface(const NNormalSurface& surface, bool) :
        triangulation(surface.getTriangulation()) {
    unsigned long tot = triangulation->getNumberOfTetrahedra();
    if (tot == 0)
        discSets = 0;
    else
        discSets = new NDiscSetTet*[tot];
}

NDiscSetSurface::NDiscSetSurface(const NNormalSurface& surface) :
        triangulation(surface.getTriangulation()) {
    unsigned long tot = triangulation->getNumberOfTetrahedra();
    if (tot == 0)
        discSets = 0;
    else {
        discSets = new NDiscSetTet*[tot];
        for (unsigned long index = 0; index < tot; index++)
            discSets[index] = new NDiscSetTet(surface, index);
    }
}

NDiscSetSurface::~NDiscSetSurface() {
    if (discSets) {
        unsigned long tot = nTets();
        for (unsigned long index = 0; index < tot; index++)
            delete discSets[index];
        delete[] discSets;
    }
}

NDiscSpec* NDiscSetSurface::adjacentDisc(const NDiscSpec& disc,
        NPerm arc, NPerm& adjArc) const {
    NTetrahedron* tet = triangulation->getTetrahedron(disc.tetIndex);
    int arcFace = arc[3];
    if (tet->adjacentTetrahedron(arcFace) == 0)
        return 0;

    NDiscSpec* ans = new NDiscSpec;
    ans->tetIndex = triangulation->tetrahedronIndex(
        tet->adjacentTetrahedron(arcFace));
    adjArc = tet->adjacentGluing(arcFace) * arc;

    unsigned long arcNumber = discSets[disc.tetIndex]->arcFromDisc(
        arcFace, arc[0], disc.type, disc.number);
    discSets[ans->tetIndex]->discFromArc(adjArc[3], adjArc[0], arcNumber,
        ans->type, ans->number);

    return ans;
}

void NDiscSpecIterator::makeValid() {
    while (current.number ==
            internalDiscSet->nDiscs(current.tetIndex, current.type)) {
        current.number = 0;
        current.type++;
        if (current.type == 10) {
            current.type = 0;
            current.tetIndex++;
            if (current.tetIndex == internalDiscSet->nTets())
                break;
        }
    }
}

} // namespace regina

