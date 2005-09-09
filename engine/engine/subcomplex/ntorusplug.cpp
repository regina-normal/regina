
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "subcomplex/nlayeredsolidtorus.h"
#include "subcomplex/ntorusplug.h"
#include "triangulation/nedge.h"
#include "triangulation/nfacepair.h"
#include "triangulation/ntetrahedron.h"

namespace regina {

NTorusPlug* NTorusPlug::isPlugged(
        NTetrahedron* externalBdry1, NPerm externalVertices1,
        NTetrahedron* externalBdry2, NPerm externalVertices2) {
    NTorusPlug* ans;

    if ((ans = NTorusPlugMobius::isPlugged(externalBdry1, externalVertices1,
            externalBdry2, externalVertices2)))
        return ans;
    if ((ans = NTorusPlugLST::isPlugged(externalBdry1, externalVertices1,
            externalBdry2, externalVertices2)))
        return ans;
    if ((ans = NTorusPlugReflector::isPlugged(externalBdry1, externalVertices1,
            externalBdry2, externalVertices2)))
        return ans;
    if ((ans = NTorusPlugCrosscap::isPlugged(externalBdry1, externalVertices1,
            externalBdry2, externalVertices2)))
        return ans;

    std::list<NTetrahedron*> avoidTets;
    avoidTets.push_back(externalBdry1);
    avoidTets.push_back(externalBdry2);

    if ((ans = NTorusPlugDouble::isPlugged(externalBdry1, externalVertices1,
            externalBdry2, externalVertices2, avoidTets)))
        return ans;

    return 0;
}

NTorusPlug* NTorusPlug::isPlugged(
        NTetrahedron* externalBdry1, NPerm externalVertices1,
        NTetrahedron* externalBdry2, NPerm externalVertices2,
        std::list<NTetrahedron*>& avoidTets) {
    NTorusPlug* ans;

    if ((ans = NTorusPlugMobius::isPlugged(externalBdry1, externalVertices1,
            externalBdry2, externalVertices2)))
        return ans;
    if ((ans = NTorusPlugLST::isPlugged(externalBdry1, externalVertices1,
            externalBdry2, externalVertices2)))
        return ans;
    if ((ans = NTorusPlugReflector::isPlugged(externalBdry1, externalVertices1,
            externalBdry2, externalVertices2)))
        return ans;
    if ((ans = NTorusPlugCrosscap::isPlugged(externalBdry1, externalVertices1,
            externalBdry2, externalVertices2)))
        return ans;

    if ((ans = NTorusPlugDouble::isPlugged(externalBdry1, externalVertices1,
            externalBdry2, externalVertices2, avoidTets)))
        return ans;

    return 0;
}

bool NTorusPlug::isBad(NTetrahedron* t,
        const std::list<NTetrahedron*>& avoidTets) {
    std::list<NTetrahedron*>::const_iterator it;
    for (it = avoidTets.begin(); it != avoidTets.end(); it++)
        if (*it == t)
            return true;
    return false;
}

NTemporaryFibre NTorusPlugMobius::plugFibre() const {
    if (orientation == 0)
        return NTemporaryFibre(1, 1);
    else if (orientation == 1)
        return NTemporaryFibre(1, -2);
    else
        return NTemporaryFibre(2, -1);
}

std::ostream& NTorusPlugMobius::writeName(std::ostream& out) const {
    return out << "m(" << orientation << ')';
}

std::ostream& NTorusPlugMobius::writeTeXName(std::ostream& out) const {
    return out << "m(" << orientation << ')';
}

void NTorusPlugMobius::writeTextLong(std::ostream& out) const {
    out << "Mobius torus plug: ";
    writeName(out);
}

NTorusPlug* NTorusPlugMobius::isPlugged(
        NTetrahedron* externalBdry1, NPerm externalVertices1,
        NTetrahedron* externalBdry2, NPerm externalVertices2) {
    NTetrahedron* internalBdry1 = externalBdry1->getAdjacentTetrahedron(
        externalVertices1[3]);
    NTetrahedron* internalBdry2 = externalBdry2->getAdjacentTetrahedron(
        externalVertices2[3]);
    NPerm internalVertices1 = externalBdry1->getAdjacentTetrahedronGluing(
        externalVertices1[3]) * externalVertices1;
    NPerm internalVertices2 = externalBdry2->getAdjacentTetrahedronGluing(
        externalVertices2[3]) * externalVertices2;

    // The two faces must be joined together.
    if (! (internalBdry1 == externalBdry2 &&
            internalVertices1[3] == externalVertices2[3]))
        return 0;

    // Find the 012 <-> 012 gluing.
    NPerm p = externalVertices2.inverse() * internalVertices1;

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

    // It is!  Put together our response.
    NTorusPlugMobius* ans = new NTorusPlugMobius(internalBdry1,
        internalVertices1, internalBdry2, internalVertices2);
    ans->orientation = orientation;
    return ans;
}

NTorusPlugLST::~NTorusPlugLST() {
    if (lst)
        delete lst;
}

NTemporaryFibre NTorusPlugLST::plugFibre() const {
    long cuts0 = lst->getMeridinalCuts(roles[0]);
    long cuts1 = lst->getMeridinalCuts(roles[1]);
    if (roles[2] != 2)
        cuts1 = -cuts1;

    return NTemporaryFibre(cuts0, cuts1);
}

std::ostream& NTorusPlugLST::writeName(std::ostream& out) const {
    long cuts0 = lst->getMeridinalCuts(roles[0]);
    long cuts1 = lst->getMeridinalCuts(roles[1]);
    if (roles[2] != 2)
        cuts1 = -cuts1;

    return out << "t(" << cuts0 << ", " << cuts1 << ')';
}

std::ostream& NTorusPlugLST::writeTeXName(std::ostream& out) const {
    long cuts0 = lst->getMeridinalCuts(roles[0]);
    long cuts1 = lst->getMeridinalCuts(roles[1]);
    if (roles[2] != 2)
        cuts1 = -cuts1;

    // TODO: No dollars!  Perhaps make this permanent across all classes.
    return out << "t(" << cuts0 << ", " << cuts1 << ')';
}

void NTorusPlugLST::writeTextLong(std::ostream& out) const {
    out << "LST torus plug: ";
    writeName(out);
}

NTorusPlug* NTorusPlugLST::isPlugged(
        NTetrahedron* externalBdry1, NPerm externalVertices1,
        NTetrahedron* externalBdry2, NPerm externalVertices2) {
    NTetrahedron* internalBdry1 = externalBdry1->getAdjacentTetrahedron(
        externalVertices1[3]);
    NTetrahedron* internalBdry2 = externalBdry2->getAdjacentTetrahedron(
        externalVertices2[3]);
    NPerm internalVertices1 = externalBdry1->getAdjacentTetrahedronGluing(
        externalVertices1[3]) * externalVertices1;
    NPerm internalVertices2 = externalBdry2->getAdjacentTetrahedronGluing(
        externalVertices2[3]) * externalVertices2;

    // Check that we move to a common new tetrahedron.
    if (! (internalBdry1 && internalBdry1 == internalBdry2))
        return 0;
    if (internalBdry1 == externalBdry1 || internalBdry1 == externalBdry2)
        return 0;

    // Do we have a layering?
    NFacePair other =
        NFacePair(internalVertices1[3], internalVertices2[3]).complement();

    if (internalVertices2 !=
           NPerm(internalVertices1[3], internalVertices2[3]) *
           NPerm(other.upper(), other.lower()) *
           internalVertices1)
        return 0;

    // Find the layered solid torus.
    NLayeredSolidTorus* lst = NLayeredSolidTorus::formsLayeredSolidTorusTop(
        internalBdry1, internalVertices1[3], internalVertices2[3]);
    if (! lst)
        return 0;

    // All good!
    NTorusPlugLST* ans = new NTorusPlugLST(internalBdry1, internalVertices1,
        internalBdry2, internalVertices2);
    ans->lst = lst;
    ans->roles = NPerm(
        lst->getTopEdgeGroup(
            edgeNumber[internalVertices1[0]][internalVertices1[1]]),
        lst->getTopEdgeGroup(
            edgeNumber[internalVertices1[0]][internalVertices1[2]]),
        lst->getTopEdgeGroup(
            edgeNumber[internalVertices1[1]][internalVertices1[2]]),
        3);

    return ans;
}

NTorusPlugDouble::~NTorusPlugDouble() {
    if (plug[0])
        delete plug[0];
    if (plug[1])
        delete plug[1];
}

void NTorusPlugDouble::insertPlugFibres(std::list<NTemporaryFibre>& results)
        const {
    // TODO
}

std::ostream& NTorusPlugDouble::writeName(std::ostream& out) const {
    if (layering == 1)
        out << "d-(";
    else
        out << "dx(";
    plug[0]->writeName(out);
    out << ", ";
    plug[1]->writeName(out);
    return out << ')';
}

std::ostream& NTorusPlugDouble::writeTeXName(std::ostream& out) const {
    if (layering == 1)
        out << "d^-(";
    else
        out << "d^\\times(";
    plug[0]->writeTeXName(out);
    out << ", ";
    plug[1]->writeTeXName(out);
    return out << ')';
}

void NTorusPlugDouble::writeTextLong(std::ostream& out) const {
    out << "Double adaptor torus plug: ";
    writeName(out);
}

NTorusPlug* NTorusPlugDouble::isPlugged(
        NTetrahedron* externalBdry1, NPerm externalVertices1,
        NTetrahedron* externalBdry2, NPerm externalVertices2,
        std::list<NTetrahedron*>& avoidTets) {
    NTetrahedron* internalBdry1 = externalBdry1->getAdjacentTetrahedron(
        externalVertices1[3]);
    NTetrahedron* internalBdry2 = externalBdry2->getAdjacentTetrahedron(
        externalVertices2[3]);
    NPerm internalVertices1 = externalBdry1->getAdjacentTetrahedronGluing(
        externalVertices1[3]) * externalVertices1;
    NPerm internalVertices2 = externalBdry2->getAdjacentTetrahedronGluing(
        externalVertices2[3]) * externalVertices2;

    // Check that we move to two separate new tetrahedra.
    if (! (internalBdry1 && internalBdry2))
        return 0;
    if (internalBdry1 == internalBdry2)
        return 0;
    if (internalBdry1 == externalBdry1 || internalBdry1 == externalBdry2 ||
            internalBdry2 == externalBdry1 || internalBdry2 == externalBdry2)
        return 0;

    if (isBad(internalBdry1, avoidTets) || isBad(internalBdry2, avoidTets))
        return 0;

    // We insist on a vertical partition for now.

    // Test for a layering over the horizontal edge.
    if (internalBdry1->getAdjacentTetrahedron(internalVertices1[0]) ==
            internalBdry2) {
        // Make sure the two new tetrahedra are glued the right way.
        if (internalBdry1->getAdjacentFace(internalVertices1[0]) !=
                internalVertices2[0])
            return 0;
        if (internalVertices1 != internalBdry2->getAdjacentTetrahedronGluing(
                internalVertices2[0]) * internalVertices2 * NPerm(1, 2))
            return 0;

        // Now look for the additional layered tetrahedron.
        NTetrahedron* next = internalBdry1->getAdjacentTetrahedron(
            internalVertices1[1]);
        if (! next)
            return 0;
        if (next != internalBdry2->getAdjacentTetrahedron(
                internalVertices2[1]))
            return 0;
        if (next == internalBdry1 || next == internalBdry2 ||
                next == externalBdry1 || next == externalBdry2)
            return 0;
        if (isBad(next, avoidTets))
            return 0;

        // Check that it is in fact layered correctly.
        NPerm cross1 =
            internalBdry1->getAdjacentTetrahedronGluing(internalVertices1[1]) *
            internalVertices1 * NPerm(1, 3, 0, 2);
        NPerm cross2 =
            internalBdry2->getAdjacentTetrahedronGluing(internalVertices2[1]) *
            internalVertices2 * NPerm(3, 1, 2, 0);
        if (cross1 != cross2)
            return 0;

        // So far so good!  Now look for the new plugs.
        avoidTets.push_back(internalBdry1);
        avoidTets.push_back(internalBdry2);
        avoidTets.push_back(next);

        NTorusPlug *plug0 = 0, *plug1 = 0;
        plug0 = NTorusPlug::isPlugged(
            internalBdry1, internalVertices1 * NPerm(1, 0, 3, 2),
            next, cross1,
            avoidTets);
        if (plug0)
            plug1 = NTorusPlug::isPlugged(
                next, cross2 * NPerm(0, 1, 3, 2),
                internalBdry2, internalVertices2 * NPerm(0, 1, 3, 2),
                avoidTets);

        avoidTets.pop_back();
        avoidTets.pop_back();
        avoidTets.pop_back();

        if (plug0 && plug1) {
            // Complete!
            NTorusPlugDouble* ans = new NTorusPlugDouble(
                internalBdry1, internalVertices1,
                internalBdry2, internalVertices2);
            ans->plug[0] = plug0;
            ans->plug[1] = plug1;
            ans->partition = 0;
            ans->layering = 1;
            return ans;
        } else {
            // Couldn't fill in the two new plugs.  Backtrack.
            if (plug0)
                delete plug0;
            if (plug1)
                delete plug1;
            return 0;
        }
    }

    // Test for a layering over the diagonal edge.
    if (internalBdry1->getAdjacentTetrahedron(internalVertices1[1]) ==
            internalBdry2) {
        // Make sure the two new tetrahedra are glued the right way.
        if (internalBdry1->getAdjacentFace(internalVertices1[1]) !=
                internalVertices2[1])
            return 0;
        if (internalVertices1 != internalBdry2->getAdjacentTetrahedronGluing(
                internalVertices2[1]) * internalVertices2 * NPerm(0, 2))
            return 0;

        // Now look for the additional layered tetrahedron.
        NTetrahedron* next = internalBdry1->getAdjacentTetrahedron(
            internalVertices1[0]);
        if (! next)
            return 0;
        if (next != internalBdry2->getAdjacentTetrahedron(
                internalVertices2[0]))
            return 0;
        if (next == internalBdry1 || next == internalBdry2 ||
                next == externalBdry1 || next == externalBdry2)
            return 0;
        if (isBad(next, avoidTets))
            return 0;

        // Check that it is in fact layered correctly.
        NPerm cross1 =
            internalBdry1->getAdjacentTetrahedronGluing(internalVertices1[0]) *
            internalVertices1 * NPerm(3, 0, 1, 2);
        NPerm cross2 =
            internalBdry2->getAdjacentTetrahedronGluing(internalVertices2[0]) *
            internalVertices2 * NPerm(0, 3, 2, 1);
        if (cross1 != cross2)
            return 0;

        // So far so good!  Now look for the new plugs.
        avoidTets.push_back(internalBdry1);
        avoidTets.push_back(internalBdry2);
        avoidTets.push_back(next);

        NTorusPlug *plug0 = 0, *plug1 = 0;
        plug0 = NTorusPlug::isPlugged(
            internalBdry1, internalVertices1 * NPerm(0, 1, 3, 2),
            next, cross1 * NPerm(1, 0, 2, 3),
            avoidTets);
        if (plug0)
            plug1 = NTorusPlug::isPlugged(
                next, cross2 * NPerm(0, 1, 3, 2),
                internalBdry2, internalVertices2 * NPerm(0, 1, 3, 2),
                avoidTets);

        avoidTets.pop_back();
        avoidTets.pop_back();
        avoidTets.pop_back();

        if (plug0 && plug1) {
            // Complete!
            NTorusPlugDouble* ans = new NTorusPlugDouble(
                internalBdry1, internalVertices1,
                internalBdry2, internalVertices2);
            ans->plug[0] = plug0;
            ans->plug[1] = plug1;
            ans->partition = 0;
            ans->layering = 2;
            return ans;
        } else {
            // Couldn't fill in the two new plugs.  Backtrack.
            if (plug0)
                delete plug0;
            if (plug1)
                delete plug1;
            return 0;
        }
    }

    // Nothing else to try.
    return 0;
}

std::ostream& NTorusPlugReflector::writeName(std::ostream& out) const {
    return out << "r";
}

std::ostream& NTorusPlugReflector::writeTeXName(std::ostream& out) const {
    return out << "r";
}

void NTorusPlugReflector::writeTextLong(std::ostream& out) const {
    out << "Reflector boundary plug";
}

NTorusPlug* NTorusPlugReflector::isPlugged(
        NTetrahedron* externalBdry1, NPerm externalVertices1,
        NTetrahedron* externalBdry2, NPerm externalVertices2) {
    NTetrahedron* internalBdry1 = externalBdry1->getAdjacentTetrahedron(
        externalVertices1[3]);
    NTetrahedron* internalBdry2 = externalBdry2->getAdjacentTetrahedron(
        externalVertices2[3]);
    NPerm internalVertices1 = externalBdry1->getAdjacentTetrahedronGluing(
        externalVertices1[3]) * externalVertices1;
    NPerm internalVertices2 = externalBdry2->getAdjacentTetrahedronGluing(
        externalVertices2[3]) * externalVertices2;

    // Do we have two new tetrahedra?
    if (! (internalBdry1 && internalBdry2))
        return 0;
    if (internalBdry1 == externalBdry1 || internalBdry1 == externalBdry2 ||
            internalBdry2 == externalBdry1 || internalBdry2 == externalBdry2)
        return 0;

    // Are they joined together the right way?
    if (internalBdry1->getAdjacentTetrahedron(internalVertices1[2]) !=
            internalBdry2)
        return 0;
    if (internalVertices2 !=
            internalBdry1->getAdjacentTetrahedronGluing(internalVertices1[2]) *
            internalVertices1 * NPerm(0, 1))
        return 0;

    // Look for the final tetrahedron.
    NTetrahedron* final = internalBdry1->getAdjacentTetrahedron(
        internalVertices1[0]);
    if (final != internalBdry1->getAdjacentTetrahedron(internalVertices1[1]))
        return 0;
    if (final != internalBdry2->getAdjacentTetrahedron(internalVertices2[0]))
        return 0;
    if (final != internalBdry2->getAdjacentTetrahedron(internalVertices2[1]))
        return 0;

    // And verify that the gluings are consistent.
    NPerm cross = internalBdry1->getAdjacentTetrahedronGluing(
        internalVertices1[0]) * internalVertices1;
    if (cross != internalBdry1->getAdjacentTetrahedronGluing(
            internalVertices1[1]) * internalVertices1 * NPerm(2, 3, 1, 0))
        return 0;
    if (cross != internalBdry2->getAdjacentTetrahedronGluing(
            internalVertices2[0]) * internalVertices2 * NPerm(3, 2, 1, 0))
        return 0;
    if (cross != internalBdry2->getAdjacentTetrahedronGluing(
            internalVertices2[1]) * internalVertices2 * NPerm(0, 1, 3, 2))
        return 0;

    // All good!
    NTorusPlugReflector* ans = new NTorusPlugReflector(
        internalBdry1, internalVertices1, internalBdry2, internalVertices2);
    return ans;
}

std::ostream& NTorusPlugCrosscap::writeName(std::ostream& out) const {
    if (reversing)
        return out << (twist ? "c^'" : "c^");
    else
        return out << (twist ? "c'" : "c");
}

std::ostream& NTorusPlugCrosscap::writeTeXName(std::ostream& out) const {
    if (reversing)
        return out << (twist ? "c^\\wedge'" : "c^\\wedge");
    else
        return out << (twist ? "c'" : "c");
}

void NTorusPlugCrosscap::writeTextLong(std::ostream& out) const {
    out << "Crosscap plug: ";
    writeName(out);
}

NTorusPlug* NTorusPlugCrosscap::isPlugged(
        NTetrahedron* externalBdry1, NPerm externalVertices1,
        NTetrahedron* externalBdry2, NPerm externalVertices2) {
    NTetrahedron* internalBdry1 = externalBdry1->getAdjacentTetrahedron(
        externalVertices1[3]);
    NTetrahedron* internalBdry2 = externalBdry2->getAdjacentTetrahedron(
        externalVertices2[3]);
    NPerm internalVertices1 = externalBdry1->getAdjacentTetrahedronGluing(
        externalVertices1[3]) * externalVertices1;
    NPerm internalVertices2 = externalBdry2->getAdjacentTetrahedronGluing(
        externalVertices2[3]) * externalVertices2;

    // Do we have two new tetrahedra?
    if (! (internalBdry1 && internalBdry2))
        return 0;
    if (internalBdry1 == externalBdry1 || internalBdry1 == externalBdry2 ||
            internalBdry2 == externalBdry1 || internalBdry2 == externalBdry2)
        return 0;

    // Are they joined together the right way?
    if (internalBdry1->getAdjacentTetrahedron(internalVertices1[0]) ==
            internalBdry2 &&
            internalVertices2 ==
            internalBdry1->getAdjacentTetrahedronGluing(internalVertices1[0]) *
            internalVertices1 * NPerm(1, 2)) {

        // Look for the final tetrahedron.
        NTetrahedron* final = internalBdry1->getAdjacentTetrahedron(
            internalVertices1[1]);
        if (final != internalBdry1->getAdjacentTetrahedron(
                internalVertices1[2]))
            return 0;
        if (final != internalBdry2->getAdjacentTetrahedron(
                internalVertices2[1]))
            return 0;
        if (final != internalBdry2->getAdjacentTetrahedron(
                internalVertices2[2]))
            return 0;

        // And verify that the gluings are consistent.
        NPerm cross = internalBdry1->getAdjacentTetrahedronGluing(
            internalVertices1[1]) * internalVertices1;
        if (cross != internalBdry2->getAdjacentTetrahedronGluing(
                internalVertices2[1]) * internalVertices2 * NPerm(2, 3, 0, 1))
            return 0;

        if (cross == internalBdry1->getAdjacentTetrahedronGluing(
                    internalVertices1[2]) * internalVertices1 *
                    NPerm(2, 0, 3, 1) &&
                cross == internalBdry2->getAdjacentTetrahedronGluing(
                    internalVertices2[2]) * internalVertices2 *
                    NPerm(3, 1, 2, 0)) {
            NTorusPlugCrosscap* ans = new NTorusPlugCrosscap(
                internalBdry1, internalVertices1,
                internalBdry2, internalVertices2);
            ans->reversing = false;
            ans->twist = true;
            return ans;
        } else if (cross == internalBdry1->getAdjacentTetrahedronGluing(
                    internalVertices1[2]) * internalVertices1 *
                    NPerm(2, 1, 3, 0) &&
                cross == internalBdry2->getAdjacentTetrahedronGluing(
                    internalVertices2[2]) * internalVertices2 *
                    NPerm(3, 0, 2, 1)) {
            NTorusPlugCrosscap* ans = new NTorusPlugCrosscap(
                internalBdry1, internalVertices1,
                internalBdry2, internalVertices2);
            ans->reversing = true;
            ans->twist = false;
            return ans;
        }
    } else if (internalBdry1->getAdjacentTetrahedron(internalVertices1[1]) ==
            internalBdry2 &&
            internalVertices2 ==
            internalBdry1->getAdjacentTetrahedronGluing(internalVertices1[1]) *
            internalVertices1 * NPerm(0, 2)) {

        // Look for the final tetrahedron.
        NTetrahedron* final = internalBdry1->getAdjacentTetrahedron(
            internalVertices1[0]);
        if (final != internalBdry1->getAdjacentTetrahedron(
                internalVertices1[2]))
            return 0;
        if (final != internalBdry2->getAdjacentTetrahedron(
                internalVertices2[0]))
            return 0;
        if (final != internalBdry2->getAdjacentTetrahedron(
                internalVertices2[2]))
            return 0;

        // And verify that the gluings are consistent.
        NPerm cross = internalBdry1->getAdjacentTetrahedronGluing(
            internalVertices1[0]) * internalVertices1;
        if (cross != internalBdry2->getAdjacentTetrahedronGluing(
                internalVertices2[0]) * internalVertices2 * NPerm(3, 2, 1, 0))
            return 0;

        if (cross == internalBdry1->getAdjacentTetrahedronGluing(
                    internalVertices1[2]) * internalVertices1 *
                    NPerm(1, 2, 3, 0) &&
                cross == internalBdry2->getAdjacentTetrahedronGluing(
                    internalVertices2[2]) * internalVertices2 *
                    NPerm(0, 3, 2, 1)) {
            NTorusPlugCrosscap* ans = new NTorusPlugCrosscap(
                internalBdry1, internalVertices1,
                internalBdry2, internalVertices2);
            ans->reversing = false;
            ans->twist = false;
            return ans;
        } else if (cross == internalBdry1->getAdjacentTetrahedronGluing(
                    internalVertices1[2]) * internalVertices1 *
                    NPerm(0, 2, 3, 1) &&
                cross == internalBdry2->getAdjacentTetrahedronGluing(
                    internalVertices2[2]) * internalVertices2 *
                    NPerm(1, 3, 2, 0)) {
            NTorusPlugCrosscap* ans = new NTorusPlugCrosscap(
                internalBdry1, internalVertices1,
                internalBdry2, internalVertices2);
            ans->reversing = true;
            ans->twist = true;
            return ans;
        }
    }

    // Nothing found.
    return 0;
}

} // namespace regina
