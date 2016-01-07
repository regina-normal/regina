
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

#include "dim2/dim2triangulation.h"
#include "dim4/dim4edge.h"
#include "maths/permconv.h"
#include <sstream>

namespace regina {

const int Face<4, 1>::edgeNumber[5][5] = {
    { -1, 0, 1, 2, 3 },
    { 0, -1, 4, 5, 6 },
    { 1, 4, -1, 7, 8 },
    { 2, 5, 7, -1, 9 },
    { 3, 6, 8, 9, -1 }};

const int Face<4, 1>::edgeVertex[10][2] = {
    { 0, 1 },
    { 0, 2 },
    { 0, 3 },
    { 0, 4 },
    { 1, 2 },
    { 1, 3 },
    { 1, 4 },
    { 2, 3 },
    { 2, 4 },
    { 3, 4 }};

Dim2Triangulation* Face<4, 1>::buildLinkDetail(bool labels,
        Dim4Isomorphism** inclusion) const {
    // Build the triangulation.
    Dim2Triangulation* ans = new Dim2Triangulation();
    NPacket::ChangeEventSpan span(ans);

    if (inclusion)
        *inclusion = new Dim4Isomorphism(degree());

    std::vector<Dim4EdgeEmbedding>::const_iterator it, adjIt;
    Dim2Triangle* tTri;
    NPerm5 perm;
    int i;
    for (it = begin(), i = 0; it != end(); ++it, ++i) {
        tTri = ans->newTriangle();
        if (labels) {
            std::stringstream s;
            s << it->getPentachoron()->markedIndex() <<
                " (" << it->getEdge() << ')';
            tTri->setDescription(s.str());
        }
        if (inclusion) {
            (*inclusion)->pentImage(i) = it->getPentachoron()->markedIndex();

            perm = it->getPentachoron()->getTriangleMapping(it->getEdge());
            if (perm[3] ==
                    it->getPentachoron()->getEdgeMapping(it->getEdge())[0])
                (*inclusion)->facetPerm(i) = perm;
            else
                (*inclusion)->facetPerm(i) = perm * NPerm5(3, 4);
        }
    }

    Dim4Pentachoron *pent, *adj;
    NPerm5 adjGluing;
    int exitTet, e;
    int edgeInLink;
    int adjIndex;
    int adjEdge;
    for (it = begin(), i = 0; it != end(); ++it, ++i) {
        pent = it->getPentachoron();
        e = it->getEdge();

        for (exitTet = 0; exitTet < 5; ++exitTet) {
            if (exitTet == edgeVertex[e][0] || exitTet == edgeVertex[e][1])
                continue;

            adj = pent->adjacentPentachoron(exitTet);
            if (! adj)
                continue;

            edgeInLink = pent->getTriangleMapping(e).preImageOf(exitTet);
            if (ans->getTriangle(i)->adjacentTriangle(edgeInLink)) {
                // We've already made this gluing in the vertex link
                // from the other side.
                continue;
            }

            adjGluing = pent->adjacentGluing(exitTet);
            adjEdge = edgeNumber[adjGluing[edgeVertex[e][0]]]
                                [adjGluing[edgeVertex[e][1]]];

            // TODO: We need to find which *embedding* corresponds to
            // the adjacent pentachoron/edge pair.
            // Currently we do a simple linear scan, which makes the
            // overall link construction quadratic.  This can surely be
            // made linear(ish) with the right data structure and/or algorithm.
            for (adjIt = begin(), adjIndex = 0;
                    adjIt != end(); ++adjIt, ++adjIndex)
                if (adjIt->getPentachoron() == adj &&
                        adjIt->getEdge() == adjEdge)
                    break; // Sets adjIndex to the right value.

            ans->getTriangle(i)->joinTo(edgeInLink, ans->getTriangle(adjIndex),
                perm5to3(adj->getTriangleMapping(adjEdge).inverse() *
                    adjGluing *
                    pent->getTriangleMapping(e)));
        }
    }

    return ans;
}

void Face<4, 1>::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << std::endl;

    out << "Appears as:" << std::endl;
    for (auto& emb : *this)
        out << "  " << emb << std::endl;
}

} // namespace regina

