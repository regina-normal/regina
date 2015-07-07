
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

#include <cstring>
#include "census/partialcensusdb.h"
#include "census/ngluingpermsearcher.h"

namespace regina {

PartialTriangulationData::PartialTriangulationData(const OneStepSearcher *s,
        int size) {
    if (size < 0)
        nTets = s->size();
    else
        nTets = size;
//    nVertexClasses = s->nVertexClasses;
//    nEdgeClasses = s->nEdgeClasses;
//    vertexState = new OneStepSearcher::TetVertexState[nTets * 4];
//    std::memcpy(vertexState, s->vertexState, sizeof(vertexState));
//    vertexStateChanged = new int[nTets * 8];
//    std::memcpy(vertexStateChanged, s->vertexStateChanged, sizeof(vertexStateChanged));
//    edgeState = new OneStepSearcher::TetEdgeState[nTets * 6];
//    std::memcpy(edgeState, s->edgeState, sizeof(edgeState));
//    edgeStateChanged = new int[nTets * 8];
//    std::memcpy(edgeStateChanged, s->edgeStateChanged, sizeof(edgeStateChanged));
    permIndices_ = new int[nTets * 4];
    std::memcpy(permIndices_, s->permIndices_, 4*nTets*sizeof(int));
//    if (s->orientableOnly_) {
//        orientation = new int[nTets];
//        std::memcpy(orientation, s->orientation, sizeof(orientation));
//    } else {
//        orientation = NULL;
//    }
}

PartialTriangulationData::~PartialTriangulationData() {
//    delete[] vertexState;
//    delete[] vertexStateChanged;
//    delete[] edgeState;
//    delete[] edgeStateChanged;
    delete[] permIndices_;
//    if (orientation)
//        delete[] orientation;
}

} // namespace regine
