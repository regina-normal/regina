
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "surfaces/normalsurface.h"
#include "surfaces/disc.h"

namespace regina {

std::ostream& operator << (std::ostream& out, const DiscSpec& spec) {
    out << '(' << spec.tetIndex << ", " << spec.type << ", "
        << spec.number << ')';
    return out;
}

bool numberDiscsAwayFromVertex(int discType, int vertex) {
    if (discType < 4)
        return (vertex == discType);
    return (vertex == 0 || vertex == quadPartner[(discType - 1) % 3][0]);
}

bool discOrientationFollowsEdge(int discType, int vertex, int edgeStart,
        int edgeEnd) {
    Perm<4> forwards(vertex, edgeStart, edgeEnd, 6-vertex-edgeStart-edgeEnd);
    Perm<4> reverse(vertex, edgeEnd, edgeStart, 6-vertex-edgeStart-edgeEnd);

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

DiscSetTet::DiscSetTet(const NormalSurface& surface, size_t tetIndex) {
    int i;
    for (i=0; i<4; i++)
        internalNDiscs[i] = surface.triangles(tetIndex, i).longValue();
    for (i=4; i<7; i++)
        internalNDiscs[i] = surface.quads(tetIndex, i - 4).longValue();
    for (i=7; i<10; i++)
        internalNDiscs[i] = surface.octs(tetIndex, i - 7).longValue();
}

DiscSetTet::DiscSetTet(unsigned long tri0, unsigned long tri1,
        unsigned long tri2, unsigned long tri3,
        unsigned long quad0, unsigned long quad1, unsigned long quad2,
        unsigned long oct0, unsigned long oct1, unsigned long oct2) {
    internalNDiscs[0] = tri0;
    internalNDiscs[1] = tri1;
    internalNDiscs[2] = tri2;
    internalNDiscs[3] = tri3;
    internalNDiscs[4] = quad0;
    internalNDiscs[5] = quad1;
    internalNDiscs[6] = quad2;
    internalNDiscs[7] = oct0;
    internalNDiscs[8] = oct1;
    internalNDiscs[9] = oct2;
}

unsigned long DiscSetTet::arcFromDisc(int /* arcFace */, int arcVertex,
        int discType, unsigned long discNumber) const {
    // Is it a triangle?
    if (discType < 4)
        return discNumber;

    // It's a quad or an octagon.
    // Note that there is at most one octagonal or quad type present
    // (since the surface must be embedded), so this must be it.
    if (arcVertex == 0 || arcVertex == quadPartner[(discType - 1) % 3][0])
        return internalNDiscs[arcVertex] + discNumber;
    else
        return internalNDiscs[arcVertex] + internalNDiscs[discType]
            - discNumber - 1;
}

void DiscSetTet::discFromArc(int arcFace, int arcVertex,
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
    if (internalNDiscs[quadSeparating[arcVertex][arcFace] + 4] > 0)
        discType = quadSeparating[arcVertex][arcFace] + 4;
    else if (internalNDiscs[quadMeeting[arcVertex][arcFace][0] + 7] > 0)
        discType = quadMeeting[arcVertex][arcFace][0] + 7;
    else
        discType = quadMeeting[arcVertex][arcFace][1] + 7;

    if (arcVertex == 0 || arcVertex == quadPartner[(discType - 1) % 3][0])
        discNumber = arcNumber - internalNDiscs[arcVertex];
    else
        discNumber = internalNDiscs[discType] -
            (arcNumber - internalNDiscs[arcVertex]) - 1;
}

DiscSetSurface::DiscSetSurface(const NormalSurface& surface, bool) :
        triangulation(surface.triangulation()) {
    size_t tot = triangulation->size();
    if (tot == 0)
        discSets = 0;
    else
        discSets = new DiscSetTet*[tot];
}

DiscSetSurface::DiscSetSurface(const NormalSurface& surface) :
        triangulation(surface.triangulation()) {
    size_t tot = triangulation->size();
    if (tot == 0)
        discSets = 0;
    else {
        discSets = new DiscSetTet*[tot];
        for (size_t index = 0; index < tot; index++)
            discSets[index] = new DiscSetTet(surface, index);
    }
}

DiscSetSurface::~DiscSetSurface() {
    if (discSets) {
        size_t tot = nTets();
        for (size_t index = 0; index < tot; index++)
            delete discSets[index];
        delete[] discSets;
    }
}

DiscSpec* DiscSetSurface::adjacentDisc(const DiscSpec& disc,
        Perm<4> arc, Perm<4>& adjArc) const {
    const Tetrahedron<3>* tet = triangulation->tetrahedron(disc.tetIndex);
    int arcFace = arc[3];
    if (tet->adjacentTetrahedron(arcFace) == 0)
        return 0;

    DiscSpec* ans = new DiscSpec;
    ans->tetIndex = tet->adjacentTetrahedron(arcFace)->index();
    adjArc = tet->adjacentGluing(arcFace) * arc;

    unsigned long arcNumber = discSets[disc.tetIndex]->arcFromDisc(
        arcFace, arc[0], disc.type, disc.number);
    discSets[ans->tetIndex]->discFromArc(adjArc[3], adjArc[0], arcNumber,
        ans->type, ans->number);

    return ans;
}

void DiscSpecIterator::makeValid() {
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

