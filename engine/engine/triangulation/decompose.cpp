
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

#include <sstream>

#include "packet/ncontainer.h"
#include "triangulation/ntriangulation.h"

namespace regina {

unsigned long NTriangulation::splitIntoComponents(NPacket* componentParent,
        bool setLabels) {
    if (! componentParent)
        componentParent = this;

    // We clone the triangulation, create new empty component
    // triangulations and then sort the tetrahedra into the new
    // components.

    unsigned long nTets = tetrahedra.size();

    // Begin by cloning the triangulation.
    NTetrahedron** newTets = new NTetrahedron*[tetrahedra.size()];
    NTetrahedron *tet, *adjTet;
    unsigned long tetPos, adjPos;
    NPerm adjPerm;
    int face;

    for (tetPos = 0; tetPos < nTets; tetPos++)
        newTets[tetPos] = new NTetrahedron(
            tetrahedra[tetPos]->getDescription());

    for (tetPos = 0; tetPos < nTets; tetPos++) {
        tet = tetrahedra[tetPos];
        for (face = 0; face < 4; face++) {
            adjTet = tet->getAdjacentTetrahedron(face);
            if (adjTet) {
                adjPos = tetrahedra.index(adjTet);
                adjPerm = tet->getAdjacentTetrahedronGluing(face);
                if (adjPos > tetPos ||
                        (adjPos == tetPos && adjPerm[face] > face))
                    newTets[tetPos]->joinTo(face, newTets[adjPos], adjPerm);
            }
        }
    }

    // Now create the new component triangulations.
    NTriangulation** newTris = new NTriangulation*[components.size()];
    unsigned long whichComp = 0;

    for (ComponentIterator it = components.begin(); it != components.end();
            it++) {
        newTris[whichComp] = new NTriangulation();
        componentParent->insertChildLast(newTris[whichComp]);

        if (setLabels) {
            std::ostringstream label;
            label << getPacketLabel() << " - #" << (whichComp + 1);
            newTris[whichComp]->setPacketLabel(makeUniqueLabel(label.str()));
        }

        whichComp++;
    }

    // At this point whichComp == components.size().

    // Sort the new tetrahedra into component triangulations.
    // Note that component index lookup is faster than tetrahedron index
    // lookup.
    for (tetPos = 0; tetPos < nTets; tetPos++)
        newTris[components.index(tetrahedra[tetPos]->getComponent())]->
            addTetrahedron(newTets[tetPos]);

    // And clean up.
    delete[] newTets;
    delete[] newTris;

    return whichComp;
}

unsigned long NTriangulation::connectedSumDecomposition(NPacket* primeParent,
        bool setLabels) {
    // Precondition checks.
    if (! (isValid() && isClosed() && isOrientable() && isConnected()))
        return 0;

    // TODO
    return 0;
}

bool NTriangulation::isThreeSphere() const {
    if (threeSphere.known())
        return threeSphere.value();

    // Basic property checks.
    if (! (isValid() && isClosed() && isOrientable() && isConnected())) {
        threeSphere = false;
        return false;
    }

    // Check homology.
    // Better simply first, which means we need a clone.
    NTriangulation working(*this);
    working.intelligentSimplify();

    if (! working.getHomologyH1().isTrivial()) {
        threeSphere = false;
        return false;
    }

    // Time for some more heavy machinery.  On to normal surfaces.
    threeSphere = (working.connectedSumDecomposition() == 0);
    return threeSphere.value();
}

/*
void NTriangulation::crushAllNormalSpheres() {
    NContainer working;
    splitIntoComponents(&working, false);

    NTriangulation* child;
    while ((child = static_cast<NTriangulation*>(working.getFirstTreeChild()))) {
        // INV: All children of working are connected.

        // Find a 2-sphere, crush, split into components and destroy.
    }
}
*/

} // namespace regina

