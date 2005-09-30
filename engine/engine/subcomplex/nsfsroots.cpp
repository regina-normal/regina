
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
    NSFSRootMobiusChain rootMobNor1("/", false);
    NSFSRootMobiusChain rootMobNor2("//", false);
    NSFSRootMobiusChain rootMobNor3("/\\", false);
    NSFSRootMobiusChain rootMobNor4("Z", false);
    NSFSRootMobiusChain rootMobNor5("X", false);
    NSFSRootMobiusChain rootMobNor6("///", false);
    NSFSRootMobiusChain rootMobNor7("//\\", false);
    NSFSRootMobiusChain rootMobNor8("/\\/", false);
    NSFSRootMobiusChain rootMobNor9("/J/", false);
    NSFSRootMobiusChain rootMobNor10("/L/", false);
    NSFSRootMobiusChain rootMobNor11("Z/", false);
    NSFSRootMobiusChain rootMobNor12("Z\\", false);
    NSFSRootMobiusChain rootMobNor13("X/", false);
    NSFSRootMobiusChain rootMobNor14("X\\", false);
    NSFSRootMobiusChain rootMobOr1("/", true);
    NSFSRootMobiusChain rootMobOr2("//", true);
    NSFSRootMobiusChain rootMobOr3("/\\", true);
    NSFSRootMobiusChain rootMobOr4("Z", true);
    NSFSRootMobiusChain rootMobOr5("X", true);
    NSFSRootMobiusChain rootMobOr6("///", true);
    NSFSRootMobiusChain rootMobOr7("//\\", true);
    NSFSRootMobiusChain rootMobOr8("/\\/", true);
    NSFSRootMobiusChain rootMobOr9("/J/", true);
    NSFSRootMobiusChain rootMobOr10("/L/", true);
    NSFSRootMobiusChain rootMobOr11("Z/", true);
    NSFSRootMobiusChain rootMobOr12("X/", true);
    NSFSRootMobiusChain rootMobOr13("X\\", true);
    NSFSRootReflectorChain rootRef1(1);
    NSFSRootReflectorChain rootRef2(2);
    NSFSRootReflectorChain rootRef3(3);
    NSFSRootTriPrism rootPrism;
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
    if ((ans = hunt(tri, rootMobNor1)))
        return ans;
    if ((ans = hunt(tri, rootMobNor2)))
        return ans;
    if ((ans = hunt(tri, rootMobNor3)))
        return ans;
    if ((ans = hunt(tri, rootMobNor4)))
        return ans;
    if ((ans = hunt(tri, rootMobNor5)))
        return ans;
    if ((ans = hunt(tri, rootMobNor6)))
        return ans;
    if ((ans = hunt(tri, rootMobNor7)))
        return ans;
    if ((ans = hunt(tri, rootMobNor8)))
        return ans;
    if ((ans = hunt(tri, rootMobNor9)))
        return ans;
    if ((ans = hunt(tri, rootMobNor10)))
        return ans;
    if ((ans = hunt(tri, rootMobNor11)))
        return ans;
    if ((ans = hunt(tri, rootMobNor12)))
        return ans;
    if ((ans = hunt(tri, rootMobNor13)))
        return ans;
    if ((ans = hunt(tri, rootMobNor14)))
        return ans;
    if ((ans = hunt(tri, rootRef1)))
        return ans;
    if ((ans = hunt(tri, rootRef2)))
        return ans;
    if ((ans = hunt(tri, rootRef3)))
        return ans;
    if ((ans = hunt(tri, rootPrism)))
        return ans;
    if ((ans = hunt(tri, rootMobOr1)))
        return ans;
    if ((ans = hunt(tri, rootMobOr2)))
        return ans;
    if ((ans = hunt(tri, rootMobOr3)))
        return ans;
    if ((ans = hunt(tri, rootMobOr4)))
        return ans;
    if ((ans = hunt(tri, rootMobOr5)))
        return ans;
    if ((ans = hunt(tri, rootMobOr6)))
        return ans;
    if ((ans = hunt(tri, rootMobOr7)))
        return ans;
    if ((ans = hunt(tri, rootMobOr8)))
        return ans;
    if ((ans = hunt(tri, rootMobOr9)))
        return ans;
    if ((ans = hunt(tri, rootMobOr10)))
        return ans;
    if ((ans = hunt(tri, rootMobOr11)))
        return ans;
    if ((ans = hunt(tri, rootMobOr12)))
        return ans;
    if ((ans = hunt(tri, rootMobOr13)))
        return ans;

    return 0;
}

NSFSRootMobiusChain::NSFSRootMobiusChain(const char* spec, bool orbl) :
        NSFSRoot(countSockets(spec)), or_(orbl) {
    spec_ = strdup(spec);

    NTetrahedron** t = new NTetrahedron*[nSockets_ * 3];
    unsigned i;
    for (i = 0; i < nSockets_ * 3; i++)
        t[i] = new NTetrahedron();

    unsigned chunks = strlen(spec);
    NSFSAnnulus* left = new NSFSAnnulus[chunks];
    NSFSAnnulus* right = new NSFSAnnulus[chunks];

    /**
     * Left and right sides are annuli of the form:
     *
     *         *--->---*
     *         |0  2 / |
     *         |    / 1|
     *  Front  |   /   |  Back
     *         |1 /    |
     *         | / 2  0|
     *         *--->---*
     */

    unsigned pos = 0;
    for (i = 0; i < chunks; i++) {
        switch (spec[i]) {
            case '/':
            case '\\':
                t[pos]->joinTo(0, t[pos + 1], NPerm(1, 2));
                t[pos]->joinTo(1, t[pos + 2], NPerm(1, 3));
                t[pos + 1]->joinTo(1, t[pos + 2], NPerm(0, 2));

                left[i].tet[0] = t[pos];
                left[i].tet[1] = t[pos + 2];
                right[i].tet[0] = t[pos + 1];
                right[i].tet[1] = t[pos + 2];
                if (spec[i] == '/') {
                    left[i].roles[0] = NPerm(0, 1, 3, 2);
                    left[i].roles[1] = NPerm(1, 3, 0, 2);
                    right[i].roles[0] = NPerm(1, 0, 3, 2);
                    right[i].roles[1] = NPerm(1, 3, 2, 0);
                } else {
                    left[i].roles[0] = NPerm(1, 0, 3, 2);
                    left[i].roles[1] = NPerm(3, 1, 0, 2);
                    right[i].roles[0] = NPerm(0, 1, 3, 2);
                    right[i].roles[1] = NPerm(3, 1, 2, 0);
                }

                pos += 3;
                break;
            case 'J':
            case 'L':
                t[pos]->joinTo(0, t[pos + 1], NPerm(1, 2));
                t[pos]->joinTo(1, t[pos + 2], NPerm(1, 3));
                t[pos + 1]->joinTo(1, t[pos + 2], NPerm(0, 2));

                left[i].tet[0] = t[pos + 2];
                left[i].tet[1] = t[pos];
                right[i].tet[0] = t[pos + 2];
                right[i].tet[1] = t[pos + 1];
                if (spec[i] == 'J') {
                    left[i].roles[0] = NPerm(3, 1, 0, 2);
                    left[i].roles[1] = NPerm(1, 0, 3, 2);
                    right[i].roles[0] = NPerm(3, 1, 2, 0);
                    right[i].roles[1] = NPerm(0, 1, 3, 2);
                } else {
                    left[i].roles[0] = NPerm(1, 3, 0, 2);
                    left[i].roles[1] = NPerm(0, 1, 3, 2);
                    right[i].roles[0] = NPerm(1, 3, 2, 0);
                    right[i].roles[1] = NPerm(1, 0, 3, 2);
                }

                pos += 3;
                break;
            case 'Z':
            case 'S':
                t[pos]->joinTo(0, t[pos + 2], NPerm(0, 3));
                t[pos + 2]->joinTo(0, t[pos + 1], NPerm(1, 2));
                t[pos + 1]->joinTo(2, t[pos], NPerm(0, 1));
                t[pos + 3]->joinTo(0, t[pos + 5], NPerm(0, 3));
                t[pos + 5]->joinTo(0, t[pos + 4], NPerm(1, 2));
                t[pos + 4]->joinTo(2, t[pos + 3], NPerm(0, 1));
                t[pos]->joinTo(1, t[pos + 5], NPerm(0, 2, 3, 1));
                t[pos + 2]->joinTo(1, t[pos + 4], NPerm(2, 1, 3, 0));
                t[pos + 3]->joinTo(1, t[pos + 2], NPerm(0, 2, 3, 1));
                t[pos + 5]->joinTo(1, t[pos + 1], NPerm(2, 1, 3, 0));

                left[i].tet[0] = t[pos];
                left[i].tet[1] = t[pos + 3];
                right[i].tet[0] = t[pos + 1];
                right[i].tet[1] = t[pos + 4];
                if (spec[i] == 'Z') {
                    left[i].roles[0] = NPerm(2, 3);
                    left[i].roles[1] = NPerm(1, 0, 3, 2);
                    right[i].roles[0] = NPerm(1, 0, 3, 2);
                    right[i].roles[1] = NPerm(2, 3);
                } else {
                    left[i].roles[0] = NPerm(1, 0, 3, 2);
                    left[i].roles[1] = NPerm(2, 3);
                    right[i].roles[0] = NPerm(2, 3);
                    right[i].roles[1] = NPerm(1, 0, 3, 2);
                }

                pos += 6;
                break;
            case 'X':
            case 'x':
                t[pos]->joinTo(1, t[pos + 3], NPerm());
                t[pos + 3]->joinTo(0, t[pos + 2], NPerm(0, 1));
                t[pos + 2]->joinTo(0, t[pos + 4], NPerm());
                t[pos + 4]->joinTo(1, t[pos], NPerm(0, 1));
                t[pos + 3]->joinTo(2, t[pos + 5], NPerm());
                t[pos + 4]->joinTo(2, t[pos + 5], NPerm(2, 3));
                t[pos]->joinTo(2, t[pos + 2], NPerm());
                t[pos + 5]->joinTo(0, t[pos + 5], NPerm(0, 1));
                t[pos + 4]->joinTo(3, t[pos + 1], NPerm(2, 3, 1, 0));
                t[pos + 3]->joinTo(3, t[pos + 1], NPerm(2, 3, 0, 1));

                if (spec[i] == 'X') {
                    left[i].tet[0] = t[pos];
                    left[i].tet[1] = t[pos + 5];
                    right[i].tet[0] = t[pos + 2];
                    right[i].tet[1] = t[pos + 5];
                    left[i].roles[0] = NPerm(2, 3);
                    left[i].roles[1] = NPerm(3, 2, 0, 1);
                    right[i].roles[0] = NPerm(2, 3);
                    right[i].roles[1] = NPerm(3, 2, 1, 0);
                } else {
                    left[i].tet[0] = t[pos + 5];
                    left[i].tet[1] = t[pos];
                    right[i].tet[0] = t[pos + 5];
                    right[i].tet[1] = t[pos + 2];
                    left[i].roles[0] = NPerm(2, 3, 0, 1);
                    left[i].roles[1] = NPerm(1, 0, 3, 2);
                    right[i].roles[0] = NPerm(2, 3, 1, 0);
                    right[i].roles[1] = NPerm(1, 0, 3, 2);
                }

                pos += 6;
                break;
            default:
                std::cerr << "ERROR: Bad NSFSRootMobiusChain specification.  "
                    "Expect a crash any time now." << std::endl;
        }
    }

    for (i = 0; i < chunks - 1; i++) {
        right[i].tet[0]->joinTo(right[i].roles[0][3], left[i + 1].tet[0],
            left[i + 1].roles[0] * right[i].roles[0].inverse());
        right[i].tet[1]->joinTo(right[i].roles[1][3], left[i + 1].tet[1],
            left[i + 1].roles[1] * right[i].roles[1].inverse());
    }
    if (or_) {
        right[chunks - 1].tet[0]->joinTo(right[chunks - 1].roles[0][3],
            left[0].tet[1], left[0].roles[1] *
            right[chunks - 1].roles[0].inverse());
        right[chunks - 1].tet[1]->joinTo(right[chunks - 1].roles[1][3],
            left[0].tet[0], left[0].roles[0] *
            right[chunks - 1].roles[1].inverse());
    } else {
        right[chunks - 1].tet[0]->joinTo(right[chunks - 1].roles[0][3],
            left[0].tet[1], left[0].roles[1] * NPerm(0, 1) *
            right[chunks - 1].roles[0].inverse());
        right[chunks - 1].tet[1]->joinTo(right[chunks - 1].roles[1][3],
            left[0].tet[0], left[0].roles[0] * NPerm(0, 1) *
            right[chunks - 1].roles[1].inverse());
    }

    for (i = 0; i < nSockets_ * 3; i++)
        root_.addTetrahedron(t[i]);

    // Almost all socket roles are identity permutations.

    // Make sure we go through the sockets in order as they appear
    // around the boundary.
    unsigned s = 0;
    for (pos = 0, i = 0; i < chunks; i++)
        switch(spec[i]) {
            case '/':
            case '\\':
                socket_[s].tet[0] = t[pos];
                socket_[s].tet[1] = t[pos + 1];
                socketOrient_[s] = (spec[i] == '/');
                s++;
                pos += 3;
                break;
            case 'J':
            case 'L':
                pos += 3;
                break;
            case 'Z':
            case 'S':
                socket_[s].tet[0] = t[pos];
                socket_[s].tet[1] = t[pos + 1];
                socketOrient_[s] = (spec[i] == 'Z');
                s++;
                pos += 6;
                break;
            case 'X':
                socket_[s].tet[0] = t[pos];
                socket_[s].tet[1] = t[pos + 1];
                socket_[s + 1].tet[0] = t[pos + 1];
                socket_[s + 1].tet[1] = t[pos + 2];
                socket_[s + 1].roles[0] = NPerm(2, 3);
                socketOrient_[s] = true;
                socketOrient_[s + 1] = false;
                s += 2;
                pos += 6;
                break;
            case 'x':
                pos += 6;
                break;
        }
    for (pos = 0, i = 0; i < chunks; i++)
        switch(spec[i]) {
            case '/':
            case '\\':
                pos += 3;
                break;
            case 'J':
            case 'L':
                socket_[s].tet[0] = t[pos];
                socket_[s].tet[1] = t[pos + 1];
                if (or_)
                    socketOrient_[s] = (spec[i] == 'L');
                else
                    socketOrient_[s] = (spec[i] == 'J');
                s++;
                pos += 3;
                break;
            case 'Z':
            case 'S':
                socket_[s].tet[0] = t[pos + 3];
                socket_[s].tet[1] = t[pos + 4];
                if (or_)
                    socketOrient_[s] = (spec[i] == 'S');
                else
                    socketOrient_[s] = (spec[i] == 'Z');
                s++;
                pos += 6;
                break;
            case 'X':
                pos += 6;
                break;
            case 'x':
                socket_[s].tet[0] = t[pos];
                socket_[s].tet[1] = t[pos + 1];
                socket_[s + 1].tet[0] = t[pos + 1];
                socket_[s + 1].tet[1] = t[pos + 2];
                socket_[s + 1].roles[0] = NPerm(2, 3);
                socketOrient_[s] = ! or_;
                socketOrient_[s + 1] = ! socketOrient_[s];
                s += 2;
                pos += 6;
                break;
        }

    delete[] t;
    delete[] left;
    delete[] right;
}

NSFSRootMobiusChain::~NSFSRootMobiusChain() {
    free(spec_);
}

NSFSpace* NSFSRootMobiusChain::createSFS() const {
    if (or_) {
        return new NSFSpace(NSFSpace::n2, 1, 0, 0);
    } else {
        NSFSpace* ans = new NSFSpace(NSFSpace::n1, 1, 0, 0);
        ans->insertFibre(1, 1);
        return ans;
    }
}

std::ostream& NSFSRootMobiusChain::writeName(std::ostream& out) const {
    if (or_)
        return out << "M~(" << spec_ << ')';
    else
        return out << "M(" << spec_ << ')';
}

std::ostream& NSFSRootMobiusChain::writeTeXName(std::ostream& out) const {
    if (or_)
        return out << "\\tilde{M}_\\mathtt{" << spec_ << '}';
    else
        return out << "M_\\mathtt{" << spec_ << '}';
}

void NSFSRootMobiusChain::writeTextLong(std::ostream& out) const {
    out << "SFS root Mobius chain: ";
    writeName(out);
}

unsigned NSFSRootMobiusChain::countSockets(const char* spec) {
    unsigned ans = 0;
    for (; *spec; spec++) {
        switch(*spec) {
            case '/':
            case '\\':
            case 'J':
            case 'L':
                ans++; break;
            case 'Z':
            case 'S':
            case 'X':
            case 'x':
                ans += 2; break;

            // Default is to ignore invalid characters.
        }
    }
    return ans;
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

NSFSRootTriPrism::NSFSRootTriPrism() : NSFSRoot(3) {
    NTetrahedron* a = new NTetrahedron();
    NTetrahedron* b = new NTetrahedron();
    NTetrahedron* c = new NTetrahedron();

    a->joinTo(0, b, NPerm(0, 2, 3, 1));
    b->joinTo(1, c, NPerm(3, 0, 2, 1));
    c->joinTo(1, a, NPerm(2, 1, 0, 3));

    root_.addTetrahedron(a);
    root_.addTetrahedron(b);
    root_.addTetrahedron(c);

    socket_[0].tet[0] = a;
    socket_[0].roles[0] = NPerm(2, 3);
    socket_[0].tet[1] = b;
    socketOrient_[0] = true;
    socket_[1].tet[0] = b;
    socket_[1].roles[0] = NPerm(1, 0, 3, 2);
    socket_[1].tet[1] = c;
    socket_[1].roles[1] = NPerm(2, 3);
    socketOrient_[1] = true;
    socket_[2].tet[0] = c;
    socket_[2].tet[1] = a;
    socketOrient_[2] = false;
}

NSFSpace* NSFSRootTriPrism::createSFS() const {
    return new NSFSpace(NSFSpace::o1, 0, 0, 0);
}

std::ostream& NSFSRootTriPrism::writeName(std::ostream& out) const {
    return out << "Tri";
}

std::ostream& NSFSRootTriPrism::writeTeXName(std::ostream& out) const {
    return out << "\\Delta";
}

void NSFSRootTriPrism::writeTextLong(std::ostream& out) const {
    out << "SFS root triangular prism";
}

} // namespace regina
