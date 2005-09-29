
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

#include <cstring>
#include "manifold/nsfs.h"
#include "subcomplex/nsfsroots.h"

namespace regina {

namespace {
    NSFSRootMobiusChain rootMob1("/");
    NSFSRootMobiusChain rootMob2("//");
    NSFSRootMobiusChain rootMob3("/\\");
    NSFSRootMobiusChain rootMob4("///");
    NSFSRootMobiusChain rootMob5("//\\");
    NSFSRootMobiusChain rootMob6("/J/");
    NSFSRootMobiusChain rootMob7("/L/");
    NSFSRootReflectorChain rootRef1(1);
    NSFSRootReflectorChain rootRef2(2);
    NSFSRootReflectorChain rootRef3(3);
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
    if ((ans = hunt(tri, rootMob1)))
        return ans;
    if ((ans = hunt(tri, rootMob2)))
        return ans;
    if ((ans = hunt(tri, rootMob3)))
        return ans;
    if ((ans = hunt(tri, rootMob4)))
        return ans;
    if ((ans = hunt(tri, rootMob5)))
        return ans;
    if ((ans = hunt(tri, rootMob6)))
        return ans;
    if ((ans = hunt(tri, rootMob7)))
        return ans;
    if ((ans = hunt(tri, rootRef1)))
        return ans;
    if ((ans = hunt(tri, rootRef2)))
        return ans;
    if ((ans = hunt(tri, rootRef3)))
        return ans;
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

    socket_[0].tet[0] = root_.getTetrahedron(1);
    socket_[0].tet[1] = root_.getTetrahedron(0);
    socket_[1].tet[0] = root_.getTetrahedron(0);
    socket_[1].tet[1] = root_.getTetrahedron(2);

    socket_[1].roles[0] = NPerm(2, 3);

    socketOrient_[0] = false;
    socketOrient_[1] = true;
}

NSFSpace* NSFSRootT_5_1::createSFS() const {
    NSFSpace* ans = new NSFSpace(NSFSpace::n1, 1, 0, 0);
    ans->insertFibre(1, 1);
    return ans;
}

std::ostream& NSFSRootT_5_1::writeName(std::ostream& out) const {
    return out << "T~5^1";
}

std::ostream& NSFSRootT_5_1::writeTeXName(std::ostream& out) const {
    return out << "\\tilde{T}_5^1";
}

NSFSRootMobiusChain::NSFSRootMobiusChain(const char* spec) :
        NSFSRoot(strlen(spec)) {
    spec_ = strdup(spec);

    NTetrahedron** t = new NTetrahedron*[nSockets_ * 3];
    unsigned i;
    for (i = 0; i < nSockets_ * 3; i++)
        t[i] = new NTetrahedron();

    NSFSAnnulus* left = new NSFSAnnulus[nSockets_];
    NSFSAnnulus* right = new NSFSAnnulus[nSockets_];

    for (i = 0; i < nSockets_; i++) {
        t[3 * i]->joinTo(0, t[3 * i + 1], NPerm(1, 2));
        t[3 * i]->joinTo(1, t[3 * i + 2], NPerm(1, 3));
        t[3 * i + 1]->joinTo(1, t[3 * i + 2], NPerm(0, 2));

        switch (spec[i]) {
            case '/':
                left[i].tet[0] = t[3 * i];
                left[i].tet[1] = t[3 * i + 2];
                left[i].roles[0] = NPerm(0, 3, 1, 2);
                left[i].roles[1] = NPerm(1, 0, 3, 2);
                right[i].tet[0] = t[3 * i + 1];
                right[i].tet[1] = t[3 * i + 2];
                right[i].roles[0] = NPerm(1, 3, 0, 2);
                right[i].roles[1] = NPerm(1, 2, 3, 0);
                break;
            case '\\':
                left[i].tet[0] = t[3 * i];
                left[i].tet[1] = t[3 * i + 2];
                left[i].roles[0] = NPerm(1, 3, 0, 2);
                left[i].roles[1] = NPerm(3, 0, 1, 2);
                right[i].tet[0] = t[3 * i + 1];
                right[i].tet[1] = t[3 * i + 2];
                right[i].roles[0] = NPerm(0, 3, 1, 2);
                right[i].roles[1] = NPerm(3, 2, 1, 0);
                break;
            case 'J':
                left[i].tet[0] = t[3 * i + 2];
                left[i].tet[1] = t[3 * i];
                left[i].roles[0] = NPerm(3, 0, 1, 2);
                left[i].roles[1] = NPerm(1, 3, 0, 2);
                right[i].tet[0] = t[3 * i + 2];
                right[i].tet[1] = t[3 * i + 1];
                right[i].roles[0] = NPerm(3, 2, 1, 0);
                right[i].roles[1] = NPerm(0, 3, 1, 2);
                break;
            case 'L':
                left[i].tet[0] = t[3 * i + 2];
                left[i].tet[1] = t[3 * i];
                left[i].roles[0] = NPerm(1, 0, 3, 2);
                left[i].roles[1] = NPerm(0, 3, 1, 2);
                right[i].tet[0] = t[3 * i + 2];
                right[i].tet[1] = t[3 * i + 1];
                right[i].roles[0] = NPerm(1, 2, 3, 0);
                right[i].roles[1] = NPerm(1, 3, 0, 2);
                break;
            default:
                std::cerr << "ERROR: Bad NSFSRootMobiusChain specification.  "
                    "Expect a crash any time now." << std::endl;
        }
    }

    for (i = 0; i < nSockets_ - 1; i++) {
        right[i].tet[0]->joinTo(right[i].roles[0][3], left[i + 1].tet[0],
            left[i + 1].roles[0] * right[i].roles[0].inverse());
        right[i].tet[1]->joinTo(right[i].roles[1][3], left[i + 1].tet[1],
            left[i + 1].roles[1] * right[i].roles[1].inverse());
    }
    right[nSockets_ - 1].tet[0]->joinTo(right[nSockets_ - 1].roles[0][3],
        left[0].tet[1], left[0].roles[1] * NPerm(0, 2) *
        right[nSockets_ - 1].roles[0].inverse());
    right[nSockets_ - 1].tet[1]->joinTo(right[nSockets_ - 1].roles[1][3],
        left[0].tet[0], left[0].roles[0] * NPerm(0, 2) *
        right[nSockets_ - 1].roles[1].inverse());

    for (i = 0; i < nSockets_ * 3; i++)
        root_.addTetrahedron(t[i]);

    // All socket roles are identity permutations.

    // Make sure we go through the sockets in order as they appear
    // around the boundary.
    unsigned s = 0;
    for (i = 0; i < nSockets_; i++)
        if (spec[i] == '/' || spec[i] == '\\') {
            socket_[s].tet[0] = t[3 * i];
            socket_[s].tet[1] = t[3 * i + 1];
            socketOrient_[s] = (s == '/');
            s++;
        }
    for (i = 0; i < nSockets_; i++)
        if (spec[i] == 'L' || spec[i] == 'J') {
            socket_[s].tet[0] = t[3 * i];
            socket_[s].tet[1] = t[3 * i + 1];
            socketOrient_[s] = (s == 'J');
            s++;
        }

    delete[] t;
    delete[] left;
    delete[] right;
}

NSFSRootMobiusChain::~NSFSRootMobiusChain() {
    free(spec_);
}

NSFSpace* NSFSRootMobiusChain::createSFS() const {
    NSFSpace* ans = new NSFSpace(NSFSpace::n1, 1, 0, 0);
    ans->insertFibre(1, 1);
    return ans;
}

std::ostream& NSFSRootMobiusChain::writeName(std::ostream& out) const {
    return out << "M(" << spec_ << ')';
}

std::ostream& NSFSRootMobiusChain::writeTeXName(std::ostream& out) const {
    return out << "M_\\mathtt{" << spec_ << '}';
}

void NSFSRootMobiusChain::writeTextLong(std::ostream& out) const {
    out << "SFS root Mobius chain: ";
    writeName(out);
}

NSFSRootReflectorChain::NSFSRootReflectorChain(unsigned length) :
        NSFSRoot(length) {
    NTetrahedron** t = new NTetrahedron*[length * 3];
    unsigned i;
    for (i = 0; i < length * 3; i++)
        t[i] = new NTetrahedron();

    for (i = 0; i < length; i++) {
        t[3 * i]->joinTo(0, t[3 * i + 2], NPerm(0, 1, 2, 3));
        t[3 * i + 1]->joinTo(0, t[3 * i + 2], NPerm(3, 2, 1, 0));
        t[3 * i]->joinTo(1, t[3 * i + 2], NPerm(3, 2, 0, 1));
        t[3 * i + 1]->joinTo(1, t[3 * i + 2], NPerm(0, 1, 3, 2));
    }

    for (i = 0; i < length - 1; i++)
        t[3 * i + 1]->joinTo(2, t[3 * i + 3], NPerm(0, 1));
    t[3 * length - 2]->joinTo(2, t[0], NPerm(0, 1));

    for (i = 0; i < length * 3; i++)
        root_.addTetrahedron(t[i]);

    // All socket roles are identity permutations.

    for (i = 0; i < length; i++) {
        socket_[i].tet[0] = t[3 * i];
        socket_[i].tet[1] = t[3 * i + 1];
        socketOrient_[i] = true;
    }

    delete[] t;
}

NSFSpace* NSFSRootReflectorChain::createSFS() const {
    return new NSFSpace(NSFSpace::o1, 0, 0, 1);
}

std::ostream& NSFSRootReflectorChain::writeName(std::ostream& out) const {
    return out << 'R' << length();
}

std::ostream& NSFSRootReflectorChain::writeTeXName(std::ostream& out) const {
    return out << "R_{" << length() << '}';
}

void NSFSRootReflectorChain::writeTextLong(std::ostream& out) const {
    out << "SFS root reflector chain: ";
    writeName(out);
}

} // namespace regina
