
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

#include <list>
#include <sstream>

#include "packet/ncontainer.h"
#include "surfaces/nnormalsurface.h"
#include "triangulation/nisomorphism.h"
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
            label << getPacketLabel() << " - Cmpt #" << (whichComp + 1);
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

    if (! primeParent)
        primeParent = this;

    // Make a working copy, simplify and record the initial homology.
    NTriangulation* working = new NTriangulation(*this);
    working->intelligentSimplify();

    unsigned long initZ, initZ2, initZ3;
    {
        const NAbelianGroup& homology = working->getHomologyH1();
        initZ = homology.getRank();
        initZ2 = homology.getTorsionRank(2);
        initZ3 = homology.getTorsionRank(3);
    }

    // Start crushing normal spheres.
    NContainer toProcess;
    toProcess.insertChildLast(working);

    std::list<NTriangulation*> primeComponents;
    unsigned long whichComp = 0;

    NTriangulation* processing;
    NTriangulation* crushed;
    NNormalSurface* sphere;
    while ((processing = static_cast<NTriangulation*>(
            toProcess.getFirstTreeChild()))) {
        // INV: Our triangulation is the connected sum of all the
        // children of toProcess, all the elements of primeComponents
        // and possibly some copies of S2xS1, RP3 and/or L(3,1).

        // Work with the last child.
        processing->makeOrphan();

        // Find a normal 2-sphere to crush.
        sphere = NNormalSurface::findNonTrivialSphere(processing);
        if (sphere) {
            crushed = sphere->crush();
            delete sphere;
            delete processing;

            crushed->intelligentSimplify();

            // Insert each component of the crushed triangulation back
            // into the list to process.
            if (crushed->getNumberOfComponents() == 0)
                delete crushed;
            else if (crushed->getNumberOfComponents() == 1)
                toProcess.insertChildLast(crushed);
            else {
                crushed->splitIntoComponents(&toProcess, false);
                delete crushed;
            }
        } else {
            // We have no non-trivial normal 2-spheres!
            // The triangulation is 0-efficient (and prime).
            // Is it a 3-sphere?
            if (processing->getNumberOfVertices() > 1) {
                // Proposition 5.1 of Jaco & Rubinstein's 0-efficiency
                // paper:  If a closed orientable triangulation T is
                // 0-efficient then either T has one vertex or T is a
                // 3-sphere with precisely two vertices.
                //
                // It follows then that this is a 3-sphere.
                // Toss it away.
                delete sphere;
                delete processing;
            } else {
                // Now we have a one-vertex prime 0-efficient triangulation.
                // We have to look for an almost normal sphere.
                //
                // From the proof of Proposition 5.12 in Jaco & Rubinstein's
                // 0-efficiency paper, we see that we can restrict our
                // search to octagonal almost normal surfaces.
                // Furthermore, from Casson's proof (directly following
                // Proposition 5.12), we see that we can restrict this
                // search further to vertex octagonal almost normal surfaces.
                sphere = NNormalSurface::findVtxOctAlmostNormalSphere(
                    processing);
                if (sphere) {
                    // It's a 3-sphere.  Toss this component away.
                    delete sphere;
                    delete processing;
                } else {
                    // It's a non-trivial prime component!
                    primeComponents.push_back(processing);
                }
            }
        }
    }

    // Run a final homology check and put back our missing S2xS1, RP3
    // and L(3,1) terms.
    unsigned long finalZ = 0, finalZ2 = 0, finalZ3 = 0;
    for (std::list<NTriangulation*>::iterator it = primeComponents.begin();
            it != primeComponents.end(); it++) {
        const NAbelianGroup& homology = (*it)->getHomologyH1();
        finalZ += homology.getRank();
        finalZ2 += homology.getTorsionRank(2);
        finalZ3 += homology.getTorsionRank(3);
    }

    while (finalZ++ < initZ) {
        working = new NTriangulation();
        working->insertLayeredLensSpace(0, 1);
        primeComponents.push_back(working);
    }
    while (finalZ2++ < initZ2) {
        working = new NTriangulation();
        working->insertLayeredLensSpace(2, 1);
        primeComponents.push_back(working);
    }
    while (finalZ3++ < initZ3) {
        working = new NTriangulation();
        working->insertLayeredLensSpace(3, 1);
        primeComponents.push_back(working);
    }

    // All done!
    for (std::list<NTriangulation*>::iterator it = primeComponents.begin();
            it != primeComponents.end(); it++) {
        primeParent->insertChildLast(*it);

        if (setLabels) {
            std::ostringstream label;
            label << getPacketLabel() << " - Summand #" << (whichComp + 1);
            (*it)->setPacketLabel(makeUniqueLabel(label.str()));
        }

        whichComp++;
    }

    return whichComp;
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
    // Better simplify first, which means we need a clone.
    NTriangulation* working = new NTriangulation(*this);
    working->intelligentSimplify();

    if (! working->getHomologyH1().isTrivial()) {
        threeSphere = false;
        delete working;
        return false;
    }

    // Time for some more heavy machinery.  On to normal surfaces.
    NContainer toProcess;
    toProcess.insertChildLast(working);

    NTriangulation* processing;
    NTriangulation* crushed;
    NNormalSurface* sphere;
    while ((processing = static_cast<NTriangulation*>(
            toProcess.getLastTreeChild()))) {
        // INV: Our triangulation is the connected sum of all the
        // children of toProcess.  Each of these children has trivial
        // homology (and therefore we have no S2xS1 / RP3 / L(3,1)
        // summands to worry about).

        // Work with the last child.
        processing->makeOrphan();

        // Find a normal 2-sphere to crush.
        sphere = NNormalSurface::findNonTrivialSphere(processing);
        if (sphere) {
            crushed = sphere->crush();
            delete sphere;
            delete processing;

            crushed->intelligentSimplify();

            // Insert each component of the crushed triangulation in the
            // list to process.
            if (crushed->getNumberOfComponents() == 0)
                delete crushed;
            else if (crushed->getNumberOfComponents() == 1)
                toProcess.insertChildLast(crushed);
            else {
                crushed->splitIntoComponents(&toProcess, false);
                delete crushed;
            }
        } else {
            // We have no non-trivial normal 2-spheres!
            // The triangulation is 0-efficient.
            // We can now test directly whether we have a 3-sphere.
            if (processing->getNumberOfVertices() > 1) {
                // Proposition 5.1 of Jaco & Rubinstein's 0-efficiency
                // paper:  If a closed orientable triangulation T is
                // 0-efficient then either T has one vertex or T is a
                // 3-sphere with precisely two vertices.
                //
                // It follows then that this is a 3-sphere.
                // Toss it away.
                delete sphere;
                delete processing;
            } else {
                // Now we have a one-vertex 0-efficient triangulation.
                // We have to look for an almost normal sphere.
                //
                // From the proof of Proposition 5.12 in Jaco & Rubinstein's
                // 0-efficiency paper, we see that we can restrict our
                // search to octagonal almost normal surfaces.
                // Furthermore, from Casson's proof (directly following
                // Proposition 5.12), we see that we can restrict this
                // search further to vertex octagonal almost normal surfaces.
                sphere = NNormalSurface::findVtxOctAlmostNormalSphere(
                    processing);
                if (sphere) {
                    // It's a 3-sphere.  Toss this component away.
                    delete sphere;
                    delete processing;
                } else {
                    // It's not a 3-sphere.  We're done!
                    threeSphere = false;
                    delete processing;
                    return false;
                }
            }
        }
    }

    // Our triangulation is the connected sum of 0 components!
    threeSphere = true;
    return true;
}

bool NTriangulation::knowsThreeSphere() const {
    if (threeSphere.known())
        return true;

    // Run some very fast prelimiary tests before we give up and say no.
    if (! (isValid() && isClosed() && isOrientable() && isConnected())) {
        threeSphere = false;
        return true;
    }

    // More work is required.
    return false;
}

NPacket* NTriangulation::makeZeroEfficient() {
    // Extract a connected sum decomposition.
    NContainer* connSum = new NContainer();
    connSum->setPacketLabel(getPacketLabel() + " - Decomposition");

    unsigned long ans = connectedSumDecomposition(connSum, true);
    if (ans > 1) {
        // Composite!
        return connSum;
    } else if (ans == 1) {
        // Prime.
        NTriangulation* newTri = dynamic_cast<NTriangulation*>(
            connSum->getLastTreeChild());
        if (! isIsomorphicTo(*newTri).get()) {
            removeAllTetrahedra();
            insertTriangulation(*newTri);
        }
        delete connSum;
        return 0;
    } else {
        // 3-sphere.
        if (getNumberOfTetrahedra() > 1) {
            removeAllTetrahedra();
            insertLayeredLensSpace(1,0);
        }
        delete connSum;
        return 0;
    }
}

} // namespace regina

