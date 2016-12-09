
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <list>
#include <sstream>

#include "enumerate/treetraversal.h"
#include "packet/container.h"
#include "subcomplex/snappedball.h"
#include "surfaces/normalsurface.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"

namespace regina {

long Triangulation<3>::connectedSumDecomposition(Packet* primeParent,
        bool setLabels) {
    // Precondition checks.
    if (! (isValid() && isClosed() && isConnected()))
        return 0;

    if (! primeParent)
        primeParent = this;

    bool initOrientable = isOrientable();

    // Make a working copy, simplify and record the initial homology.
    Triangulation<3>* working = new Triangulation<3>(*this, false);
    working->intelligentSimplify();

    unsigned long initZ, initZ2, initZ3;
    {
        const AbelianGroup& homology = working->homology();
        initZ = homology.rank();
        initZ2 = homology.torsionRank(2);
        initZ3 = homology.torsionRank(3);
    }

    // Start crushing normal spheres.
    Container toProcess;
    toProcess.insertChildLast(working);

    std::list<Triangulation<3>*> primeComponents;
    unsigned long whichComp = 0;

    Triangulation<3>* processing;
    Triangulation<3>* crushed;
    NormalSurface* sphere;
    while ((processing = static_cast<Triangulation<3>*>(
            toProcess.firstChild()))) {
        // INV: Our triangulation is the connected sum of all the
        // children of toProcess, all the elements of primeComponents
        // and possibly some copies of S2xS1, S2x~S1, RP3, and/or L(3,1).

        // Work with the last child.
        processing->makeOrphan();

        // Find a normal 2-sphere to crush.
        sphere = processing->hasNonTrivialSphereOrDisc();
        if (sphere) {
            crushed = sphere->crush();
            delete sphere;
            delete processing;

            if (! crushed->isValid()) {
                // We must have had an embedded two-sided projective plane.
                // Abort.

                delete crushed;

                std::list<Triangulation<3>*>::iterator it;
                for (it = primeComponents.begin();
                        it != primeComponents.end(); ++it)
                    delete *it;

                // All children of toProcess will be deleted automatically
                // with toProcess itself (which is on the stack).

                return -1;
            }

            crushed->intelligentSimplify();

            // Insert each component of the crushed triangulation back
            // into the list to process.
            if (crushed->countComponents() == 0)
                delete crushed;
            else if (crushed->countComponents() == 1)
                toProcess.insertChildLast(crushed);
            else {
                crushed->splitIntoComponents(&toProcess, false);
                delete crushed;
            }
        } else {
            // We have no non-trivial normal 2-spheres!
            // The triangulation is 0-efficient (and prime).
            // Is it a 3-sphere?
            if (! processing->isOrientable()) {
                // Definitely not a sphere.
                primeComponents.push_back(processing);
            } else {
                // Orientable, and so possibly a sphere.  Test this precisely.
                if (processing->countVertices() > 1) {
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
                    // Now we have a closed orientable one-vertex 0-efficient
                    // triangulation.
                    // We have to look for an almost normal sphere.
                    //
                    // From the proof of Proposition 5.12 in Jaco & Rubinstein's
                    // 0-efficiency paper, we see that we can restrict our
                    // search to octagonal almost normal surfaces.
                    // Furthermore, from the result in the quadrilateral-octagon
                    // coordinates paper, we can restrict this search further
                    // to vertex octagonal almost normal surfaces in
                    // quadrilateral-octagonal space.
                    sphere = processing->hasOctagonalAlmostNormalSphere();
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
    }

    // Run a final homology check and put back our missing S2xS1, S2x~S1,
    // RP3 and L(3,1) terms.
    unsigned long finalZ = 0, finalZ2 = 0, finalZ3 = 0;
    for (std::list<Triangulation<3>*>::iterator it = primeComponents.begin();
            it != primeComponents.end(); it++) {
        const AbelianGroup& homology = (*it)->homology();
        finalZ += homology.rank();
        finalZ2 += homology.torsionRank(2);
        finalZ3 += homology.torsionRank(3);
    }

    while (finalZ++ < initZ) {
        working = new Triangulation<3>();
        if (initOrientable) {
            // Build S2 x S1.
            working->insertLayeredLensSpace(0, 1);
        } else {
            // Build S2 x~ S1.
            Tetrahedron<3>* t0 = working->newTetrahedron();
            Tetrahedron<3>* t1 = working->newTetrahedron();
            t0->join(0, t1, Perm<4>(0, 1, 3, 2));
            t0->join(1, t1, Perm<4>(0, 1, 3, 2));
            t0->join(2, t1, Perm<4>(1, 3, 2, 0));
            t0->join(3, t1, Perm<4>(2, 0, 1, 3));
        }
        primeComponents.push_back(working);
        irreducible_ = false; // Implied by the S2xS1 or S2x~S1 summand.
        zeroEfficient_ = false; // Implied by the S2xS1 or S2x~S1 summand.
    }
    while (finalZ2++ < initZ2) {
        working = new Triangulation<3>();
        working->insertLayeredLensSpace(2, 1);
        primeComponents.push_back(working);
    }
    while (finalZ3++ < initZ3) {
        working = new Triangulation<3>();
        working->insertLayeredLensSpace(3, 1);
        primeComponents.push_back(working);
    }

    // All done!
    for (std::list<Triangulation<3>*>::iterator it = primeComponents.begin();
            it != primeComponents.end(); it++) {
        primeParent->insertChildLast(*it);

        if (setLabels) {
            std::ostringstream label;
            label << "Summand #" << (whichComp + 1);
            (*it)->setLabel(adornedLabel(label.str()));
        }

        whichComp++;
    }

    // Set irreducibility while we're at it.
    if (whichComp > 1) {
        threeSphere_ = false;
        irreducible_ = false;
        zeroEfficient_ = false;
    } else if (whichComp == 1) {
        threeSphere_ = false;
        if (! irreducible_.known()) {
            // If our manifold is S2xS1 or S2x~S1 then it is *not* irreducible;
            // however, in this case we will have already set irreducible
            // to false when putting back the S2xS1 or S2x~S1 summands above
            // (and therefore irreducible.known() will be true).
            irreducible_ = true;
        }
    } else if (whichComp == 0) {
        threeSphere_ = true;
        irreducible_ = true;
        haken_ = false;
    }

    return whichComp;
}

bool Triangulation<3>::isThreeSphere() const {
    if (threeSphere_.known())
        return threeSphere_.value();

    // Basic property checks.
    if (! (isValid() && isClosed() && isOrientable() && isConnected() &&
            (! isEmpty()))) {
        threeSphere_ = false;
        return false;
    }

    // Check homology and fundamental group.
    // Better simplify first, which means we need a clone.
    Triangulation<3>* working = new Triangulation<3>(*this, false);
    working->intelligentSimplify();

    // The Poincare conjecture!
    if (working->fundamentalGroup().countGenerators() == 0) {
        threeSphere_ = true;
        delete working;

        // Some other things that come for free:
        irreducible_ = true;
        haken_ = false;

        return true;
    }

    // We could still have a trivial group but not know it.
    // At least we can at least check homology precisely.
    if (! working->homology().isTrivial()) {
        threeSphere_ = false;
        delete working;
        return false;
    }

    // Time for some more heavy machinery.  On to normal surfaces.
    Container toProcess;
    toProcess.insertChildLast(working);

    Triangulation<3>* processing;
    Triangulation<3>* crushed;
    NormalSurface* sphere;
    while ((processing = static_cast<Triangulation<3>*>(toProcess.lastChild()))) {
        // INV: Our triangulation is the connected sum of all the
        // children of toProcess.  Each of these children has trivial
        // homology (and therefore we have no S2xS1 / RP3 / L(3,1)
        // summands to worry about).

        // Work with the last child.
        processing->makeOrphan();

        // Find a normal 2-sphere to crush.
        sphere = processing->hasNonTrivialSphereOrDisc();
        if (sphere) {
            crushed = sphere->crush();
            delete sphere;
            delete processing;

            crushed->intelligentSimplify();

            // Insert each component of the crushed triangulation in the
            // list to process.
            if (crushed->countComponents() == 0)
                delete crushed;
            else if (crushed->countComponents() == 1)
                toProcess.insertChildLast(crushed);
            else {
                crushed->splitIntoComponents(&toProcess, false);
                delete crushed;
            }
        } else {
            // We have no non-trivial normal 2-spheres!
            // The triangulation is 0-efficient.
            // We can now test directly whether we have a 3-sphere.
            if (processing->countVertices() > 1) {
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
                // Now we have a closed orientable one-vertex 0-efficient
                // triangulation.
                // We have to look for an almost normal sphere.
                //
                // From the proof of Proposition 5.12 in Jaco & Rubinstein's
                // 0-efficiency paper, we see that we can restrict our
                // search to octagonal almost normal surfaces.
                // Furthermore, from the result in the quadrilateral-octagon
                // coordinates paper, we can restrict this search further
                // to vertex octagonal almost normal surfaces in
                // quadrilateral-octagonal space.
                sphere = processing->hasOctagonalAlmostNormalSphere();
                if (sphere) {
                    // It's a 3-sphere.  Toss this component away.
                    delete sphere;
                    delete processing;
                } else {
                    // It's not a 3-sphere.  We're done!
                    threeSphere_ = false;
                    delete processing;
                    return false;
                }
            }
        }
    }

    // Our triangulation is the connected sum of 0 components!
    threeSphere_ = true;

    // Some other things that we get for free:
    irreducible_ = true;
    haken_ = false;

    return true;
}

bool Triangulation<3>::knowsThreeSphere() const {
    if (threeSphere_.known())
        return true;

    // Run some very fast prelimiary tests before we give up and say no.
    if (! (isValid() && isClosed() && isOrientable() && isConnected())) {
        threeSphere_ = false;
        return true;
    }

    // More work is required.
    return false;
}

bool Triangulation<3>::isBall() const {
    if (threeBall_.known())
        return threeBall_.value();

    // Basic property checks.
    if (! (isValid() && hasBoundaryTriangles() && isOrientable() && isConnected()
            && countBoundaryComponents() == 1
            && boundaryComponents().front()->eulerChar() == 2)) {
        threeBall_ = false;
        return false;
    }

    // Pass straight to isThreeSphere (which in turn will check faster things
    // like homology before pulling out the big guns).
    //
    // Cone the boundary to a point (i.e., fill it with a ball), then
    // call isThreeSphere() on the resulting closed triangulation.

    Triangulation<3> working(*this, false);
    working.intelligentSimplify();
    working.finiteToIdeal();

    // Simplify again in case our coning was inefficient.
    working.intelligentSimplify();

    threeBall_ = working.isThreeSphere();
    return threeBall_.value();
}

bool Triangulation<3>::knowsBall() const {
    if (threeBall_.known())
        return true;

    // Run some very fast prelimiary tests before we give up and say no.
    if (! (isValid() && hasBoundaryTriangles() && isOrientable() && isConnected()
            && countBoundaryComponents() == 1
            && boundaryComponents().front()->eulerChar() == 2)) {
        threeBall_ = false;
        return true;
    }

    // More work is required.
    return false;
}

bool Triangulation<3>::isSolidTorus() const {
    if (solidTorus_.known())
        return solidTorus_.value();

    // Basic property checks.
    if (! (isValid() && isOrientable() && isConnected() &&
            countBoundaryComponents() == 1 &&
            boundaryComponents().front()->eulerChar() == 0 &&
            boundaryComponents().front()->isOrientable()))
        return (solidTorus_ = false);

    // If it's ideal, make it a triangulation with real boundary.
    // If it's not ideal, clone it anyway so we can modify it.
    Triangulation<3>* working = new Triangulation<3>(*this, false);
    working->intelligentSimplify();
    if (working->isIdeal()) {
        working->idealToFinite();
        working->intelligentSimplify();
    }

    // Check homology.
    if (! (working->homology().isZ())) {
        delete working;
        return (solidTorus_ = false);
    }

    // So:
    // We are valid, orientable, compact and connected, with H1 = Z.
    // There is exactly one boundary component, and this is a torus.

    // Note that the homology results imply that this is not a connected
    // sum of something with S2xS1 (otherwise we would have two Z terms
    // in the homology: one from the torus boundary and one from the S2xS1).
    // This observation simplifies the crushing cases later on.

    // Pull out the big guns: normal surface time.
    NormalSurface* s;
    Triangulation<3>* crushed;
    Packet* p;
    Triangulation<3>* comp;
    while (true) {
        // INVARIANT: working is homeomorphic to our original manifold.
        if (working->countVertices() > 1) {
            // Try *really* hard to get to a 1-vertex triangulation,
            // since this will make hasNonTrivialSphereOrDisc() much
            // faster (it will be able to use linear programming).
            working->intelligentSimplify();
            if (working->countVertices() > 1) {
                working->barycentricSubdivision();
                working->intelligentSimplify();
                working->intelligentSimplify();
            }
        }

        // Find a non-trivial normal disc or sphere.
        s = working->hasNonTrivialSphereOrDisc();
        if (! s) {
            // No non-trivial normal disc.  This cannot be a solid torus.
            delete working;
            return (solidTorus_ = false);
        }

        // Crush it and see what happens.
        // Given what we know about the manifold so far, the only things
        // that can happen during crushing are:
        // - undo connected sum decompositions;
        // - cut along properly embedded discs;
        // - gain and/or lose 3-balls and/or 3-spheres.
        crushed = s->crush();
        delete s;
        delete working;
        working = 0;

        crushed->intelligentSimplify();
        crushed->splitIntoComponents(0, false);
        for (p = crushed->firstChild(); p; p = p->nextSibling()) {
            // Examine each connected component after crushing.
            comp = static_cast<Triangulation<3>*>(p);
            if (comp->isClosed()) {
                // A closed piece.
                // Must be a 3-sphere, or else we didn't have a solid torus.
                if (! comp->isThreeSphere()) {
                    delete crushed;
                    return (solidTorus_ = false);
                }
            } else if (comp->countBoundaryComponents() > 1) {
                // Multiple boundaries on the same component.
                // This should never happen, since it implies there was
                // an S2xS1 summand.
                std::cerr << "ERROR: S2xS1 summand detected in "
                    "isSolidTorus() that should not exist." << std::endl;

                // At any rate, it means we did not have a solid torus.
                delete crushed;
                return (solidTorus_ = false);
            } else if (comp->boundaryComponent(0)->eulerChar() == 2) {
                // A component with sphere boundary.
                // Must be a 3-ball, or else we didn't have a solid torus.
                if (! comp->isBall()) {
                    delete crushed;
                    return (solidTorus_ = false);
                }
            } else {
                // The only other possibility is a component with torus
                // boundary.  We should only see at most one of these.
                //
                // Unless some other non-trivial component was split off
                // (i.e., a non-ball and/or non-sphere that will be
                // detected separately in the tests above), this
                // component must be identical to our original manifold.
                if (working) {
                    std::cerr << "ERROR: Multiple torus boundary "
                        "components detected in isSolidTorus(), which "
                        "should not be possible." << std::endl;
                }
                working = comp;
            }
        }

        if (! working) {
            // We have reduced everything down to balls and spheres.
            // The only way this can happen is if we had a solid torus
            // (and we crushed and/or cut along a compressing disc
            // during the crushing operation).
            delete crushed;
            return (solidTorus_ = true);
        }

        // We have the original manifold in working, but this time with
        // fewer tetrahedra.  Around we go again.
        working->makeOrphan();
        delete crushed;
    }
}

bool Triangulation<3>::knowsSolidTorus() const {
    if (solidTorus_.known())
        return true;

    // Run some very fast prelimiary tests before we give up and say no.
    if (! (isValid() && isOrientable() && isConnected())) {
        solidTorus_ = false;
        return true;
    }

    if (countBoundaryComponents() != 1) {
        solidTorus_ = false;
        return true;
    }

    if (boundaryComponents().front()->eulerChar() != 0 ||
            (! boundaryComponents().front()->isOrientable())) {
        solidTorus_ = false;
        return true;
    }

    // More work is required.
    return false;
}

Packet* Triangulation<3>::makeZeroEfficient() {
    // Extract a connected sum decomposition.
    Container* connSum = new Container();
    connSum->setLabel(adornedLabel("Decomposition"));

    long ans = connectedSumDecomposition(connSum, true);
    if (ans > 1) {
        // Composite!
        return connSum;
    } else if (ans == 1) {
        // Prime.
        Triangulation<3>* newTri = dynamic_cast<Triangulation<3>*>(
            connSum->lastChild());
        if (! isIsomorphicTo(*newTri).get()) {
            removeAllTetrahedra();
            insertTriangulation(*newTri);
        }
        delete connSum;
        return 0;
    } else {
        // 3-sphere.
        if (size() > 1) {
            removeAllTetrahedra();
            insertLayeredLensSpace(1,0);
        }
        delete connSum;
        return 0;
    }
}

bool Triangulation<3>::isIrreducible() const {
    if (irreducible_.known())
        return irreducible_.value();

    // Precondition checks.
    if (! (isValid() && isClosed() && isOrientable() && isConnected()))
        return 0;

    // We will essentially carry out a connected sum decomposition, but
    // instead of keeping prime summands we will just count them and
    // throw them away.
    unsigned long summands = 0;

    // Make a working copy, simplify and record the initial homology.
    Triangulation<3>* working = new Triangulation<3>(*this, false);
    working->intelligentSimplify();

    unsigned long Z, Z2, Z3;
    {
        const AbelianGroup& homology = working->homology();
        Z = homology.rank();
        Z2 = homology.torsionRank(2);
        Z3 = homology.torsionRank(3);
    }

    // Start crushing normal spheres.
    Container toProcess;
    toProcess.insertChildLast(working);

    Triangulation<3>* processing;
    Triangulation<3>* crushed;
    NormalSurface* sphere;
    while ((processing = static_cast<Triangulation<3>*>(
            toProcess.firstChild()))) {
        // INV: Our triangulation is the connected sum of all the
        // children of toProcess, all the prime components that we threw away,
        // and possibly some copies of S2xS1, RP3 and/or L(3,1).

        // Work with the last child.
        processing->makeOrphan();

        // Find a normal 2-sphere to crush.
        sphere = processing->hasNonTrivialSphereOrDisc();
        if (sphere) {
            crushed = sphere->crush();
            delete sphere;
            delete processing;

            crushed->intelligentSimplify();

            // Insert each component of the crushed triangulation back
            // into the list to process.
            if (crushed->countComponents() == 0)
                delete crushed;
            else if (crushed->countComponents() == 1)
                toProcess.insertChildLast(crushed);
            else {
                crushed->splitIntoComponents(&toProcess, false);
                delete crushed;
            }
        } else {
            // We have no non-trivial normal 2-spheres!
            // The triangulation is 0-efficient (and prime).
            // Is it a 3-sphere?
            if (processing->countVertices() > 1) {
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
                // Now we have a closed orientable one-vertex 0-efficient
                // triangulation.
                // We have to look for an almost normal sphere.
                //
                // From the proof of Proposition 5.12 in Jaco & Rubinstein's
                // 0-efficiency paper, we see that we can restrict our
                // search to octagonal almost normal surfaces.
                // Furthermore, from the result in the quadrilateral-octagon
                // coordinates paper, we can restrict this search further
                // to vertex octagonal almost normal surfaces in
                // quadrilateral-octagonal space.
                sphere = processing->hasOctagonalAlmostNormalSphere();
                if (sphere) {
                    // It's a 3-sphere.  Toss this component away.
                    delete sphere;
                    delete processing;
                } else {
                    // It's a non-trivial prime component!
                    // Note that this will never be an S2xS1 summand;
                    // those get crushed away entirely (we account for
                    // them later).
                    if (summands > 0) {
                        // We have found more than one prime component.
                        threeSphere_ = false; // Implied by reducibility.
                        zeroEfficient_ = false; // Implied by reducibility.
                        delete processing;
                        return (irreducible_ = false);
                    }
                    ++summands;

                    // Note which parts of our initial homology we have
                    // now accounted for.
                    const AbelianGroup& h1 = processing->homology();
                    Z -= h1.rank();
                    Z2 -= h1.torsionRank(2);
                    Z3 -= h1.torsionRank(3);

                    // Toss away our prime summand and keep going.
                    delete processing;
                }
            }
        }
    }

    // Run a final homology check: were there any additional S2xS1, RP3
    // or L(3,1) terms?
    if (Z > 0) {
        // There were S2xS1 summands that were crushed away.
        // The manifold must be reducible.
        threeSphere_ = false; // Implied by reducibility.
        zeroEfficient_ = false; // Implied by reducibility.
        return (irreducible_ = false);
    }
    if (summands + Z2 + Z3 > 1) {
        // At least two summands were found and/or crushed away: the
        // manifold must be composite.
        threeSphere_ = false; // Implied by reducibility.
        zeroEfficient_ = false; // Implied by reducibility.
        return (irreducible_ = false);
    }

    // There are no S2xS1 summands, and the manifold is prime.
    return (irreducible_ = true);
}

bool Triangulation<3>::knowsIrreducible() const {
    return irreducible_.known();
}

bool Triangulation<3>::hasCompressingDisc() const {
    if (compressingDisc_.known())
        return compressingDisc_.value();

    // Some sanity checks; also enforce preconditions.
    if (! hasBoundaryTriangles())
        return (compressingDisc_ = false);
    if ((! isValid()) || isIdeal())
        return (compressingDisc_ = false);

    long minBdryEuler = 2;
    for (auto bc : boundaryComponents()) {
        if (bc->eulerChar() < minBdryEuler)
            minBdryEuler = bc->eulerChar();
    }
    if (minBdryEuler == 2)
        return (compressingDisc_ = false);

    // Off we go.
    // Work with a simplified triangulation.
    Triangulation<3>* use = new Triangulation<3>(*this, false);
    use->intelligentSimplify();

    // Try for a fast answer first.
    if (use->hasSimpleCompressingDisc()) {
        delete use;
        return (compressingDisc_ = true);
    }

    // Nope.  Decide whether we can use the fast linear programming
    // machinery or whether we need to do a full vertex surface enumeration.
    if (use->isOrientable() && use->countBoundaryComponents() == 1) {
        NormalSurface* ans;
        Triangulation<3>* crush;
        Triangulation<3>* comp;
        unsigned nComp;

        while (true) {
            use->intelligentSimplify();

            if (use->countVertices() > 1) {
                // Try harder.
                use->barycentricSubdivision();
                use->intelligentSimplify();
                if (use->countVertices() > 1) {
                    // Fall back to a full vertex enumeration.
                    // This mirrors the code for non-orientable
                    // triangulations; see that later block for details.
                    NormalSurfaces* q = NormalSurfaces::enumerate(
                        use, NS_STANDARD);

                    unsigned long nSurfaces = q->size();
                    for (unsigned long i = 0; i < nSurfaces; ++i) {
                        if (q->surface(i)->isCompressingDisc(true)) {
                            delete use;
                            return (compressingDisc_ = true);
                        }
                    }

                    // No compressing discs!
                    delete use;
                    return (compressingDisc_ = false);
                }
            }

            TreeSingleSoln<LPConstraintEuler> search(use, NS_STANDARD);
            if (! search.find()) {
                // No compressing discs!
                delete use;
                return (compressingDisc_ = false);
            }

            ans = search.buildSurface();
            crush = ans->crush();
            delete ans;
            delete use;

            nComp = crush->splitIntoComponents();
            comp = static_cast<Triangulation<3>*>(crush->firstChild());
            while (comp) {
                if (comp->countBoundaryComponents() == 1 &&
                        comp->boundaryComponent(0)->eulerChar()
                        == minBdryEuler) {
                    // This must be our original manifold.
                    comp->makeOrphan();
                    break;
                }

                comp = static_cast<Triangulation<3>*>(comp->nextSibling());
            }

            delete crush;

            if (! comp) {
                // We must have compressed.
                return (compressingDisc_ = true);
            }

            // Around we go again, but this time with a smaller triangulation.
            use = comp;
        }
    } else {
        // Sigh.  Enumerate all vertex normal surfaces.
        //
        // Hum, are we allowed to do this in quad space?  Jaco and Tollefson
        // use standard coordinates.  Jaco, Letscher and Rubinstein mention
        // quad space, but don't give details (which I'd prefer to see).
        // Leave it in standard coordinates for now.
        NormalSurfaces* q = NormalSurfaces::enumerate(use, NS_STANDARD);

        // Run through all vertex surfaces looking for a compressing disc.
        unsigned long nSurfaces = q->size();
        for (unsigned long i = 0; i < nSurfaces; ++i) {
            // Use the fact that all vertex normal surfaces are connected.
            if (q->surface(i)->isCompressingDisc(true)) {
                delete use;
                return (compressingDisc_ = true);
            }
        }

        // No compressing discs!
        delete use;
        return (compressingDisc_ = false);
    }
}

bool Triangulation<3>::knowsCompressingDisc() const {
    if (compressingDisc_.known())
        return true;

    // Quickly check for non-spherical boundary components before we give up.
    for (auto bc : boundaryComponents())
        if (bc->eulerChar() < 2)
            return false;

    // All boundary components are 2-spheres.
    compressingDisc_ = false;
    return true;
}

bool Triangulation<3>::hasSimpleCompressingDisc() const {
    // Some sanity checks; also enforce preconditions.
    if (! hasBoundaryTriangles())
        return false;
    if ((! isValid()) || isIdeal())
        return false;

    // Off we go.
    // Work with a simplified triangulation.
    Triangulation<3> use(*this, false);
    use.intelligentSimplify();

    // Check to see whether any component is a one-tetrahedron solid torus.
    for (ComponentIterator cit = use.components().begin();
            cit != use.components().end(); ++cit)
        if ((*cit)->size() == 1 &&
                (*cit)->countTriangles() == 3 &&
                (*cit)->countVertices() == 1) {
            // Because we know the triangulation is valid, this rules out
            // all one-tetrahedron triangulations except for LST(1,2,3).
            return (compressingDisc_ = true);
        }

    // Open up as many boundary triangles as possible (to make it easier to
    // find simple compressing discs).
    TriangleIterator fit;
    bool opened = true;
    while (opened) {
        opened = false;
        for (fit = use.triangles().begin(); fit != use.triangles().end(); ++fit)
            if (use.openBook(*fit, true, true)) {
                opened = true;
                break;
            }
    }

    // How many boundary spheres do we currently have?
    // This is important because we test whether a disc is a compressing
    // disc by cutting along it and looking for any *new* boundary
    // spheres that might result.
    unsigned long origSphereCount = 0;
    for (auto bc : use.boundaryComponents())
        if (bc->eulerChar() == 2)
            ++origSphereCount;

    // Look for a single internal triangle surrounded by three boundary edges.
    // It doesn't matter whether the edges and/or vertices are distinct.
    Edge<3> *e0, *e1, *e2;
    unsigned long newSphereCount;
    for (fit = use.triangles().begin(); fit != use.triangles().end(); ++fit) {
        if ((*fit)->isBoundary())
            continue;

        e0 = (*fit)->edge(0);
        e1 = (*fit)->edge(1);
        e2 = (*fit)->edge(2);
        if (! (e0->isBoundary() && e1->isBoundary() && e2->isBoundary()))
            continue;

        // This could be a compressing disc.
        // Cut along the triangle to be sure.
        const TriangleEmbedding<3>& emb = (*fit)->front();

        Triangulation<3> cut(use, false);
        cut.tetrahedron(emb.tetrahedron()->markedIndex())->unjoin(
            emb.triangle());

        // If we don't see a new boundary component, the disc boundary is
        // non-separating in the manifold boundary and is therefore a
        // non-trivial curve.
        if (cut.countBoundaryComponents() == use.countBoundaryComponents())
            return (compressingDisc_ = true);

        newSphereCount = 0;
        for (auto bc : cut.boundaryComponents())
            if (bc->eulerChar() == 2)
                ++newSphereCount;

        // Was the boundary of the disc non-trivial?
        if (newSphereCount == origSphereCount)
            return (compressingDisc_ = true);
    }

    // Look for a tetrahedron with two faces folded together, giving a
    // degree-one edge on the inside and a boundary edge on the outside.
    // The boundary edge on the outside will surround a disc that cuts
    // right through the tetrahedron.
    TetrahedronIterator tit;
    SnappedBall* ball;
    for (tit = use.simplices_.begin(); tit != use.simplices_.end(); ++tit) {
        ball = SnappedBall::formsSnappedBall(*tit);
        if (! ball)
            continue;

        int equator = ball->equatorEdge();
        if (! (*tit)->edge(equator)->isBoundary()) {
            delete ball;
            continue;
        }

        // This could be a compressing disc.
        // Cut through the tetrahedron to be sure.
        // We do this by removing the tetrahedron, and then plugging
        // both holes on either side of the disc with new copies of the
        // tetrahedron.
        int upper = ball->boundaryFace(0);
        delete ball;

        Tetrahedron<3>* adj = (*tit)->adjacentTetrahedron(upper);
        if (! adj) {
            // The disc is trivial.
            continue;
        }

        Triangulation<3> cut(use, false);
        cut.tetrahedron((*tit)->markedIndex())->unjoin(upper);
        Tetrahedron<3>* tet = cut.newTetrahedron();
        tet->join(Edge<3>::edgeVertex[equator][0], tet, Perm<4>(
            Edge<3>::edgeVertex[equator][0], Edge<3>::edgeVertex[equator][1]));
        tet->join(upper, cut.tetrahedron(adj->markedIndex()),
            (*tit)->adjacentGluing(upper));

        // If we don't see a new boundary component, the disc boundary is
        // non-separating in the manifold boundary and is therefore a
        // non-trivial curve.
        if (cut.countBoundaryComponents() == use.countBoundaryComponents())
            return (compressingDisc_ = true);

        newSphereCount = 0;
        for (auto bc : cut.boundaryComponents())
            if (bc->eulerChar() == 2)
                ++newSphereCount;

        // Was the boundary of the disc non-trivial?
        if (newSphereCount == origSphereCount)
            return (compressingDisc_ = true);
    }

    // Nothing found.
    return false;
}

namespace {
    /**
     * Used to sort candidate incompressible surfaces by Euler characteristic.
     * Surfaces with smaller genus (i.e., larger Euler characteristic)
     * are to be processed first.
     */
    struct SurfaceID {
        long index;
            /**< Which surface in the list are we referring to? */
        long euler;
            /**< What is its Euler characteristic? */

        inline bool operator < (const SurfaceID& rhs) const {
            return (euler > rhs.euler ||
                (euler == rhs.euler && index < rhs.index));
        }
    };
}

bool Triangulation<3>::isHaken() const {
    if (haken_.known())
        return haken_.value();

    // Check basic preconditions.
    if (! (isValid() && isOrientable() && isClosed() && isConnected()))
        return false;

    // Irreducibility is not a precondition, but we promise to return
    // false immediately if the triangulation is not irreducible.
    // Do not set the property in this situation.
    if (! isIrreducible())
        return false;

    // Okay: we are closed, connected, orientable and irreducible.
    // Move to a copy of this triangulation, which we can mess with.
    Triangulation<3> t(*this, false);
    t.intelligentSimplify();

    // First check for an easy answer via homology:
    if (t.homology().rank() > 0) {
        threeSphere_ = false; // Implied by Hakenness.
        return (haken_ = true);
    }

    // Enumerate vertex normal surfaces in quad coordinates.
    // std::cout << "Enumerating surfaces..." << std::endl;
    NormalSurfaces* list = NormalSurfaces::enumerate(&t, NS_QUAD);

    // Run through each surface, one at a time.
    // Sort them first however, so we process the (easier) smaller genus
    // surfaces first.
    SurfaceID* id = new SurfaceID[list->size()];
    unsigned i;
    for (i = 0; i < list->size(); ++i) {
        id[i].index = i;
        id[i].euler = list->surface(i)->eulerChar().longValue();
    }
    std::sort(id, id + list->size());

    for (unsigned i = 0; i < list->size(); ++i) {
        // std::cout << "Testing surface " << i << "..." << std::endl;
        if (list->surface(id[i].index)->isIncompressible()) {
            delete[] id;
            threeSphere_ = false; // Implied by Hakenness.
            return (haken_ = true);
        }
    }

    delete[] id;
    return (haken_ = false);
}

bool Triangulation<3>::knowsHaken() const {
    return haken_.known();
}

} // namespace regina

