
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

#include "algebra/nabeliangroup.h"
#include "manifold/nlensspace.h"
#include "manifold/nsfs.h"
#include "triangulation/nedge.h"
#include "triangulation/ncomponent.h"
#include "triangulation/ntetrahedron.h"
#include "subcomplex/nlayeredloop.h"

namespace regina {

NLayeredLoop* NLayeredLoop::clone() const {
    NLayeredLoop* ans = new NLayeredLoop();
    ans->length = length;
    ans->hinge[0] = hinge[0];
    ans->hinge[1] = hinge[1];
    return ans;
}

NManifold* NLayeredLoop::getManifold() const {
    if (hinge[1]) {
        // Not twisted.
        return new NLensSpace(length, 1);
    } else {
        // Twisted.
        NSFSpace* ans = new NSFSpace();
        ans->insertFibre(2, -1);
        ans->insertFibre(2, 1);
        ans->insertFibre(length, 1);
        ans->reduce();
        return ans;
    }
}

NLayeredLoop* NLayeredLoop::isLayeredLoop(const NComponent* comp) {
    // Basic property check.
    if ((! comp->isClosed()) || (! comp->isOrientable()))
        return 0;

    unsigned long nTet = comp->getNumberOfTetrahedra();
    if (nTet == 0)
        return 0;
    unsigned long nVertices = comp->getNumberOfVertices();
    if (nVertices > 2)
        return 0;
    bool twisted = (nVertices == 1);

    // We have at least 1 tetrahedron and precisely 1 or 2 vertices.
    // The component is closed and orientable (and connected, since it's
    // a component).

    // Pick our base tetrahedron.
    NTetrahedron* base = comp->getTetrahedron(0);

    NTetrahedron* tet = base;
    NTetrahedron* next;
    int baseTop0, baseTop1, baseBottom0, baseBottom1;
    int top0, top1, bottom0, bottom1;
    int adjTop0 = 0, adjTop1 = 0, adjBottom0 = 0, adjBottom1 = 0;
    int hinge0, hinge1;
    NPerm p;
    bool ok;

    // Declare 0 to be a top face; find its partner.
    baseTop0 = 0;
    next = base->adjacentTetrahedron(0);
    for (baseTop1 = 1; baseTop1 < 4; baseTop1++) {
        if (base->adjacentTetrahedron(baseTop1) != next)
            continue;

        // Find the vertex joined to baseTop0 by a hinge.
        for (baseBottom0 = 1; baseBottom0 < 4; baseBottom0++) {
            if (baseBottom0 == baseTop1)
                continue;
            baseBottom1 = 6 - baseBottom0 - baseTop0 - baseTop1;

            // Some basic property checks.
            if (base->adjacentTetrahedron(baseBottom0) !=
                    base->adjacentTetrahedron(baseBottom1))
                continue;

            hinge0 = edgeNumber[baseTop0][baseBottom0];
            hinge1 = edgeNumber[baseTop1][baseBottom1];
            if (twisted) {
                if (base->getEdge(hinge0) != base->getEdge(hinge1))
                    continue;
                if (base->getEdge(hinge0)->getNumberOfEmbeddings() != 2 * nTet)
                    continue;
            } else {
                if (base->getEdge(hinge0)->getNumberOfEmbeddings() != nTet)
                    continue;
                if (base->getEdge(hinge1)->getNumberOfEmbeddings() != nTet)
                    continue;
            }

            top0 = baseTop0; top1 = baseTop1;
            bottom0 = baseBottom0; bottom1 = baseBottom1;

            // Follow the gluings up.
            ok = true;
            while (true) {
                // Already set: tet, next, topi, bottomi.
                
                // Check that both steps up lead to the same tetrahedron.
                // Note that this check has already been done for the first
                // iteration of this loop; never mind, no big loss.
                if (tet->adjacentTetrahedron(top0) !=
                        tet->adjacentTetrahedron(top1)) {
                    ok = false;
                    break;
                }

                // Check that the corresponding gluings are correct.
                p = tet->adjacentGluing(top0);
                adjTop0 = p[bottom0];
                adjTop1 = p[top1];
                adjBottom0 = p[top0];
                adjBottom1 = p[bottom1];

                p = tet->adjacentGluing(top1);
                // Note that only three of the four comparisons are needed.
                if (adjTop0 != p[top0] || adjTop1 != p[bottom1] ||
                        adjBottom0 != p[bottom0]) {
                    ok = false;
                    break;
                }

                // If we've finished the loop, exit at this point so we
                // can check that it all glued up correctly.
                if (next == base)
                    break;

                // We haven't finished the loop, so the next
                // tetrahedron should be different from this one.
                if (next == tet) {
                    ok = false;
                    break;
                }

                // Move to the next tetrahedron.
                top0 = adjTop0; top1 = adjTop1;
                bottom0 = adjBottom0; bottom1 = adjBottom1;
                tet = next;
                next = tet->adjacentTetrahedron(top0);
            }

            if (ok) {
                // Make sure the final gluing wraps everything up
                // correctly.
                if (twisted) {
                    if (adjTop0 != baseTop1 || adjTop1 != baseTop0 ||
                            adjBottom0 != baseBottom1)
                        continue;
                } else {
                    if (adjTop0 != baseTop0 || adjTop1 != baseTop1 ||
                            adjBottom0 != baseBottom0)
                        continue;
                }

                // We have a solution!
                NLayeredLoop* ans = new NLayeredLoop();
                ans->length = nTet;
                ans->hinge[0] = base->getEdge(hinge0);
                ans->hinge[1] = (twisted ? 0 : base->getEdge(hinge1));
                return ans;
            }
        }
    }

    // Nothing found.
    return 0;
}

NAbelianGroup* NLayeredLoop::getHomologyH1() const {
    NAbelianGroup* ans = new NAbelianGroup();
    if (hinge[1]) {
        // Untwisted.
        if (length > 1)
            ans->addTorsionElement(length);
    } else {
        // Twisted.
        if (length % 2 == 0)
            ans->addTorsionElement(2, 2);
        else
            ans->addTorsionElement(4);
    }
    return ans;
}

} // namespace regina

