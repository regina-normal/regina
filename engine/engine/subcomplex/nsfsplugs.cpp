
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
#include "subcomplex/nlayeredsolidtorus.h"
#include "subcomplex/nsfsplugs.h"
#include "triangulation/nedge.h"
#include "triangulation/nfacepair.h"
#include "triangulation/ntetrahedron.h"

namespace regina {

NSFSPlug* NSFSPlug::isPlugged(const NSFSAnnulus& socket) {
    // TODO: Flesh out.
    NSFSPlug* ans;

    if ((ans = NSFSPlugMobius::isPlugged(socket)))
        return ans;
    if ((ans = NSFSPlugLST::isPlugged(socket)))
        return ans;

    return 0;
}

NSFSPlug* NSFSPlug::isPlugged(const NSFSAnnulus& socket,
        std::list<NTetrahedron*>& /* avoidTets */) {
    // TODO: Flesh out.
    NSFSPlug* ans;

    if ((ans = NSFSPlugMobius::isPlugged(socket)))
        return ans;
    if ((ans = NSFSPlugLST::isPlugged(socket)))
        return ans;

    return 0;
}

void NSFSPlugMobius::adjustSFS(NSFSpace& sfs, bool reflect) const {
    int p, q;
    if (orientation == 0) {
        p = q = 1;
    } else if (orientation == 1) {
        p = 1; q = -2;
    } else {
        p = 2; q = -1;
    }

    sfs.insertFibre(p, reflect ? -q : q);
}

std::ostream& NSFSPlugMobius::writeName(std::ostream& out) const {
    return out << "m(" << orientation << ')';
}

std::ostream& NSFSPlugMobius::writeTeXName(std::ostream& out) const {
    return out << "m(" << orientation << ')';
}

NSFSPlug* NSFSPlugMobius::isPlugged(const NSFSAnnulus& socket) {
    if (socket.meetsBoundary())
        return 0;
    NSFSAnnulus internal = socket.otherSide();

    // The two faces must be joined together.
    if (! (internal.tet[0] == socket.tet[1] &&
            internal.roles[0][3] == socket.roles[1][3]))
        return 0;

    // Find the 012 <-> 012 gluing.
    NPerm p = socket.roles[1].inverse() * internal.roles[0];

    // See if it's one of the three allowable permutations.
    int orientation;
    if (p == NPerm(0, 2, 1, 3))
        orientation = 0;
    else if (p == NPerm(2, 1, 0, 3))
        orientation = 1;
    else if (p == NPerm(1, 0, 2, 3))
        orientation = 2;
    else
        return 0;

    // It is!
    return new NSFSPlugMobius(internal, orientation);
}

NSFSPlugLST::~NSFSPlugLST() {
    delete lst;
}

void NSFSPlugLST::adjustSFS(NSFSpace& sfs, bool reflect) const {
    long cuts0 = lst->getMeridinalCuts(roles[0]);
    long cuts1 = lst->getMeridinalCuts(roles[1]);
    if (roles[2] != 2)
        cuts1 = -cuts1;

    sfs.insertFibre(cuts0, reflect ? -cuts1 : cuts1);
}

std::ostream& NSFSPlugLST::writeName(std::ostream& out) const {
    long cuts0 = lst->getMeridinalCuts(roles[0]);
    long cuts1 = lst->getMeridinalCuts(roles[1]);
    if (roles[2] != 2)
        cuts1 = -cuts1;

    return out << "t(" << cuts0 << ',' << cuts1 << ')';
}

std::ostream& NSFSPlugLST::writeTeXName(std::ostream& out) const {
    long cuts0 = lst->getMeridinalCuts(roles[0]);
    long cuts1 = lst->getMeridinalCuts(roles[1]);
    if (roles[2] != 2)
        cuts1 = -cuts1;

    return out << "t(" << cuts0 << ',' << cuts1 << ')';
}

NSFSPlug* NSFSPlugLST::isPlugged(const NSFSAnnulus& socket) {
    if (socket.meetsBoundary())
        return 0;
    NSFSAnnulus internal = socket.otherSide();

    // Check that we move to a common new tetrahedron.
    if (internal.tet[0] != internal.tet[1])
        return 0;
    if (internal.tet[0] == socket.tet[0] || internal.tet[0] == socket.tet[1])
        return 0;

    // Do we have a layering?
    NFacePair other =
        NFacePair(internal.roles[0][3], internal.roles[1][3]).complement();

    if (internal.roles[1] !=
           NPerm(internal.roles[0][3], internal.roles[1][3]) *
           NPerm(other.upper(), other.lower()) *
           internal.roles[0])
        return 0;

    // Find the layered solid torus.
    NLayeredSolidTorus* lst = NLayeredSolidTorus::formsLayeredSolidTorusTop(
        internal.tet[0], internal.roles[0][3], internal.roles[1][3]);
    if (! lst)
        return 0;

    // All good!
    return new NSFSPlugLST(internal, lst, NPerm(
        lst->getTopEdgeGroup(
            edgeNumber[internal.roles[0][0]][internal.roles[0][1]]),
        lst->getTopEdgeGroup(
            edgeNumber[internal.roles[0][0]][internal.roles[0][2]]),
        lst->getTopEdgeGroup(
            edgeNumber[internal.roles[0][1]][internal.roles[0][2]]),
        3));
}

} // namespace regina
