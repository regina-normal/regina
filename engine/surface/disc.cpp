
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include "surface/normalsurface.h"
#include "surface/disc.h"

namespace regina {

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
            if (triDiscArcs[discType][i] == forwards)
                return true;
            if (triDiscArcs[discType][i] == reverse)
                return false;
        }
    }
    else if (discType < 7) {
        for (int i = 0; i < 4; i++) {
            if (quadDiscArcs[discType - 4][i] == forwards)
                return true;
            if (quadDiscArcs[discType - 4][i] == reverse)
                return false;
        }
    }
    else {
        for (int i = 0; i < 8; i++) {
            if (octDiscArcs[discType - 7][i] == forwards)
                return true;
            if (octDiscArcs[discType - 7][i] == reverse)
                return false;
        }
    }
    return false;
}

DiscSetTet::DiscSetTet(const NormalSurface& surface, size_t tetIndex) {
    // Note: These calls to safeValue() could throw an IntegerOverflow.
    int i;
    for (i=0; i<4; i++)
        discs_[i] = surface.triangles(tetIndex, i).safeValue<size_t>();
    for (i=4; i<7; i++)
        discs_[i] = surface.quads(tetIndex, i - 4).safeValue<size_t>();
    for (i=7; i<10; i++)
        discs_[i] = surface.octs(tetIndex, i - 7).safeValue<size_t>();
}

size_t DiscSetTet::arcFromDisc(int /* arcFace */, int arcVertex,
        int discType, size_t discNumber) const {
    // Is it a triangle?
    if (discType < 4)
        return discNumber;

    // It's a quad or an octagon.
    // Note that there is at most one octagonal or quad type present
    // (since the surface must be embedded), so this must be it.
    if (arcVertex == 0 || arcVertex == quadPartner[(discType - 1) % 3][0])
        return discs_[arcVertex] + discNumber;
    else
        return discs_[arcVertex] + discs_[discType]
            - discNumber - 1;
}

std::pair<int, size_t> DiscSetTet::discFromArc(int arcFace, int arcVertex,
        size_t arcNumber) const {
    // Is it a triangle?
    if (arcNumber < discs_[arcVertex])
        return { arcVertex, arcNumber };

    // It's a quad or an octagon.
    // Note that there is at most one octagonal or quad type present
    // (since the surface must be embedded), so this must be it.
    int discType;
    if (discs_[quadSeparating[arcVertex][arcFace] + 4] > 0)
        discType = quadSeparating[arcVertex][arcFace] + 4;
    else if (discs_[quadMeeting[arcVertex][arcFace][0] + 7] > 0)
        discType = quadMeeting[arcVertex][arcFace][0] + 7;
    else
        discType = quadMeeting[arcVertex][arcFace][1] + 7;

    size_t discNumber;
    if (arcVertex == 0 || arcVertex == quadPartner[(discType - 1) % 3][0])
        discNumber = arcNumber - discs_[arcVertex];
    else
        discNumber = discs_[discType] - (arcNumber - discs_[arcVertex]) - 1;

    return { discType, discNumber };
}

} // namespace regina

