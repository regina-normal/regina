
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

#include "manifold/nsfs.h"
#include "subcomplex/npluggedsfs.h"

namespace regina {

namespace {
    NSFSRootT_5_1 rootT_5_1;
}

NSFSTree* NSFSTree::isSFSTree(NTriangulation* tri) {
    // Basic property checks.
    if (! tri->isClosed())
        return 0;
    if (tri->getNumberOfVertices() > 1)
        return 0;
    if (tri->getNumberOfComponents() > 1)
        return 0;

    // Our triangulation is closed with one vertex and one component.

    // Hunt for the root.
    // TODO: Run through the list.
    NSFSTree* ans;
    if ((ans = hunt(tri, rootT_5_1)))
        return ans;

    return 0;
}

NSFSRootT_5_1::NSFSRootT_5_1() : NSFSRoot(2) {
    const int adj[6][4] = {
        { 3, 4, -1, -1},
        { 3, 4, 5, -1},
        { 3, 4, 5, -1},
        { 2, 1, 5, 0},
        { 2, 1, 5, 0},
        { 4, 3, 2, 1}
    };

    const int glu[6][4][4] = {
        { { 3, 2, 0, 1 }, { 2, 3, 0, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
        { { 1, 0, 2, 3 }, { 0, 1, 2, 3 }, { 0, 1, 3, 2 }, { 0, 0, 0, 0 } },
        { { 0, 1, 2, 3 }, { 1, 0, 2, 3 }, { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
        { { 0, 1, 2, 3 }, { 1, 0, 2, 3 }, { 3, 2, 1, 0 }, { 2, 3, 1, 0 } },
        { { 1, 0, 2, 3 }, { 0, 1, 2, 3 }, { 3, 2, 0, 1 }, { 2, 3, 0, 1 } },
        { { 2, 3, 1, 0 }, { 3, 2, 1, 0 }, { 0, 1, 2, 3 }, { 0, 1, 3, 2 } }
    };

    root_.insertConstruction(6, adj, glu);

    socket_[0]->tet[0] = root_.getTetrahedron(1);
    socket_[0]->tet[1] = root_.getTetrahedron(0);
    socket_[1]->tet[0] = root_.getTetrahedron(0);
    socket_[1]->tet[1] = root_.getTetrahedron(2);

    socket_[1]->roles[0] = NPerm(2, 3);

    socketOrient_[0] = false;
    socketOrient_[1] = true;
}

NSFSSpace* NSFSRootT_5_1::createSFS() {
    NSFSpace* ans = NSFSpace(NSFSpace::n1, 1, 0, 0);
    ans->insertFibre(1, 1);
    return ans;
}

std::ostream& NSFSRootT_5_1::writeName(std::ostream& out) const {
    return out << "T~5^1";
}

std::ostream& NSFSRootT_5_1::writeTeXName(std::ostream& out) const {
    return out << "\\tilde{T}_5^1";
}

} // namespace regina
