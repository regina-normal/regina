
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "surface/normalsurface.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"

namespace regina {

std::vector<Triangulation<3>> Triangulation<3>::summands() const {
    // Precondition checks.
    if (! (isValid() && isClosed() && isConnected()))
        return { };

    // Do we already know the answer?
    if (prop_.threeSphere_.has_value() && *prop_.threeSphere_)
        return { };

    bool initOrientable = isOrientable();

    std::stack<Triangulation<3>> toProcess;

    // Make a working copy (throwing away any locks), simplify, and record
    // the initial homology.
    Triangulation<3>& start = toProcess.emplace(*this, false, false);
    start.intelligentSimplify();

    unsigned long initZ, initZ2, initZ3;
    {
        AbelianGroup homology = start.homology();
        initZ = homology.rank();
        initZ2 = homology.torsionRank(2);
        initZ3 = homology.torsionRank(3);
    }

    // Start crushing normal spheres.
    std::vector<Triangulation<3>> primeComponents;

    while (! toProcess.empty()) {
        // INV: Our triangulation is the connected sum of all the
        // children of toProcess, all the elements of primeComponents
        // and possibly some copies of S2xS1, S2x~S1, RP3, and/or L(3,1).

        // Note: top cannot be const, because we might std::move() it.
        Triangulation<3>& top = toProcess.top();

        // Find a normal 2-sphere to crush.
        std::optional<NormalSurface> sphere = top.nonTrivialSphereOrDisc();
        if (sphere) {
            Triangulation<3> crushed = sphere->crush();
            sphere.reset(); // to avoid a deep copy of top when we pop
            toProcess.pop();

            if (! crushed.isValid()) {
                // We must have had an embedded two-sided projective plane.
                // Abort.
                //
                // All elements of toProcess will be deleted automatically
                // with toProcess itself (which is on the stack).
                throw regina::UnsolvedCase("Found an embedded two-sided "
                    "projective plane");
            }

            crushed.intelligentSimplify();

            // Insert each component of the crushed triangulation back
            // into the list to process.
            if (! crushed.isEmpty()) {
                if (crushed.isConnected())
                    toProcess.push(std::move(crushed));
                else {
                    for (auto& comp : crushed.triangulateComponents())
                        toProcess.push(std::move(comp));
                }
            }
        } else {
            // We have no non-trivial normal 2-spheres!
            // The triangulation is 0-efficient (and prime).
            // Is it a 3-sphere?
            if (! top.isOrientable()) {
                // Definitely not a sphere.
                primeComponents.push_back(std::move(top));
            } else {
                // Orientable, and so possibly a sphere.  Test this precisely.
                if (top.countVertices() > 1) {
                    // Proposition 5.1 of Jaco & Rubinstein's 0-efficiency
                    // paper:  If a closed orientable triangulation T is
                    // 0-efficient then either T has one vertex or T is a
                    // 3-sphere with precisely two vertices.
                    //
                    // It follows then that this is a 3-sphere.
                    // Toss it away.
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
                    if (top.octagonalAlmostNormalSphere()) {
                        // It's a 3-sphere.  Toss this component away.
                    } else {
                        // It's a non-trivial prime component!
                        primeComponents.push_back(std::move(top));
                    }
                }
            }
            toProcess.pop();
        }
    }

    // Run a final homology check and put back our missing S2xS1, S2x~S1,
    // RP3 and L(3,1) terms.
    unsigned long finalZ = 0, finalZ2 = 0, finalZ3 = 0;
    for (const Triangulation<3>& c : primeComponents) {
        AbelianGroup homology = c.homology();
        finalZ += homology.rank();
        finalZ2 += homology.torsionRank(2);
        finalZ3 += homology.torsionRank(3);
    }

    while (finalZ++ < initZ) {
        auto [t0, t1] = primeComponents.emplace_back().newTetrahedra<2>();
        if (initOrientable) {
            // Build S2 x S1.
            t0->join(0, t1, Perm<4>(2, 3, 0, 1));
            t0->join(1, t1, Perm<4>(2, 3, 0, 1));
            t0->join(3, t0, Perm<4>(3, 0, 1, 2));
            t1->join(0, t1, Perm<4>(1, 2, 3, 0));
        } else {
            // Build S2 x~ S1.
            t0->join(0, t1, Perm<4>(0, 1, 3, 2));
            t0->join(1, t1, Perm<4>(0, 1, 3, 2));
            t0->join(2, t1, Perm<4>(1, 3, 2, 0));
            t0->join(3, t1, Perm<4>(2, 0, 1, 3));
        }
        prop_.irreducible_ = false; // Implied by the S2xS1 or S2x~S1 summand.
        prop_.zeroEfficient_ = false; // Implied by the S2xS1 or S2x~S1 summand.
    }
    while (finalZ2++ < initZ2) {
        auto [t0, t1] = primeComponents.emplace_back().newTetrahedra<2>();
        t0->join(0, t1, Perm<4>(0, 3));
        t0->join(1, t1, Perm<4>(1, 2));
        t0->join(3, t0, Perm<4>(2, 3));
        t1->join(0, t1, Perm<4>(1, 2, 3, 0));
        prop_.zeroEfficient_ = false; // Implied by the RP3 summand.
    }
    while (finalZ3++ < initZ3) {
        auto [t0, t1] = primeComponents.emplace_back().newTetrahedra<2>();
        t0->join(0, t1, Perm<4>(2, 3, 0, 1));
        t0->join(1, t1, Perm<4>(2, 3, 0, 1));
        t0->join(3, t0, Perm<4>(1, 3, 0, 2));
        t1->join(0, t1, Perm<4>(1, 2, 3, 0));
    }

    // All done!

    // Set irreducibility while we're at it.
    if (primeComponents.size() > 1) {
        prop_.threeSphere_ = false;
        prop_.irreducible_ = false;
        prop_.zeroEfficient_ = false;
    } else if (primeComponents.size() == 1) {
        prop_.threeSphere_ = false;
        if (! prop_.irreducible_.has_value()) {
            // If our manifold is S2xS1 or S2x~S1 then it is *not* irreducible;
            // however, in this case we will have already set irreducible
            // to false when putting back the S2xS1 or S2x~S1 summands above
            // (and therefore irreducible.known() will be true).
            prop_.irreducible_ = true;
        }
    } else if (primeComponents.size() == 0) {
        prop_.threeSphere_ = true;
        prop_.irreducible_ = true;
        prop_.haken_ = false;
    }

    return primeComponents;
}

bool Triangulation<3>::isSphere() const {
    if (prop_.threeSphere_.has_value())
        return *prop_.threeSphere_;

    // Basic property checks.
    if (! (isValid() && isClosed() && isOrientable() && isConnected() &&
            (! isEmpty()))) {
        return *(prop_.threeSphere_ = false);
    }

    std::stack<Triangulation<3>> toProcess;;

    // Check homology and fundamental group.
    // Better simplify first, which means we need a clone.
    Triangulation<3>& start = toProcess.emplace(*this, false, false);
    start.intelligentSimplify();

    // The Poincare conjecture!
    if (start.group().countGenerators() == 0) {
        prop_.threeSphere_ = true;

        // Some other things that come for free:
        prop_.irreducible_ = true;
        prop_.haken_ = false;

        return true;
    }

    // We could still have a trivial group but not know it.
    // At least we can at least check homology precisely.
    if (! start.homology().isTrivial()) {
        return *(prop_.threeSphere_ = false);
    }

    // Time for some more heavy machinery.  On to normal surfaces.
    while (! toProcess.empty()) {
        // INV: Our triangulation is the connected sum of all the
        // children of toProcess.  Each of these children has trivial
        // homology (and therefore we have no S2xS1 / RP3 / L(3,1)
        // summands to worry about).

        const Triangulation<3>& top = toProcess.top();

        // Find a normal 2-sphere to crush.
        std::optional<NormalSurface> sphere = top.nonTrivialSphereOrDisc();
        if (sphere) {
            Triangulation<3> crushed = sphere->crush();
            sphere.reset(); // to avoid a deep copy of top when we pop
            toProcess.pop();

            crushed.intelligentSimplify();

            // Insert each component of the crushed triangulation in the
            // list to process.
            if (! crushed.isEmpty()) {
                if (crushed.isConnected())
                    toProcess.push(std::move(crushed));
                else {
                    for (auto& comp : crushed.triangulateComponents())
                        toProcess.push(std::move(comp));
                }
            }
        } else {
            // We have no non-trivial normal 2-spheres!
            // The triangulation is 0-efficient.
            // We can now test directly whether we have a 3-sphere.
            if (top.countVertices() > 1) {
                // Proposition 5.1 of Jaco & Rubinstein's 0-efficiency
                // paper:  If a closed orientable triangulation T is
                // 0-efficient then either T has one vertex or T is a
                // 3-sphere with precisely two vertices.
                //
                // It follows then that this is a 3-sphere.
                // Toss it away.
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
                if (top.octagonalAlmostNormalSphere()) {
                    // It's a 3-sphere.  Toss this component away.
                } else {
                    // It's not a 3-sphere.  We're done!
                    return *(prop_.threeSphere_ = false);
                }
            }
            toProcess.pop();
        }
    }

    // Our triangulation is the connected sum of 0 components!
    prop_.threeSphere_ = true;

    // Some other things that we get for free:
    prop_.irreducible_ = true;
    prop_.haken_ = false;

    return true;
}

bool Triangulation<3>::knowsSphere() const {
    if (prop_.threeSphere_.has_value())
        return true;

    // Run some very fast prelimiary tests before we give up and say no.
    if (! (isValid() && isClosed() && isOrientable() && isConnected())) {
        prop_.threeSphere_ = false;
        return true;
    }

    // More work is required.
    return false;
}

bool Triangulation<3>::isBall() const {
    if (prop_.handlebody_.has_value())
        return (*prop_.handlebody_ == 0);

    // Basic property checks.
    if (! (isValid() && isOrientable() && isConnected() &&
            countBoundaryComponents() == 1)) {
        // This cannot be a handlebody at all.
        prop_.handlebody_ = -1;
        return false;
    }
    if (boundaryComponents().front()->eulerChar() != 2) {
        // It might be a handlebody, but it's not a 3-ball.
        return false;
    }

    // Pass straight to isSphere (which in turn will check faster things
    // like homology before pulling out the big guns).
    //
    // Cone the boundary to a point (i.e., fill it with a ball), then
    // call isSphere() on the resulting closed triangulation.

    Triangulation<3> working(*this, false, false);
    working.intelligentSimplify();
    working.finiteToIdeal();

    // Simplify again in case our coning was inefficient.
    working.intelligentSimplify();

    if (working.isSphere()) {
        prop_.handlebody_ = 0;
        return true;
    } else {
        // From what we know above, if it's not a 3-ball then it's not a
        // handlebody of any genus.
        prop_.handlebody_ = -1;
        return false;
    }
}

bool Triangulation<3>::knowsBall() const {
    if (prop_.handlebody_.has_value())
        return true;

    // Run some very fast prelimiary tests before we give up and say no.
    if (! (isValid() && isOrientable() && isConnected() &&
            countBoundaryComponents() == 1)) {
        // This cannot be a handlebody at all.
        prop_.handlebody_ = -1;
        return true;
    }
    if (boundaryComponents().front()->eulerChar() != 2) {
        // It might be a handlebody, but it's not a 3-ball.
        // Although we are not caching the result, we still return true
        // because isBall() will nevertheless give a very fast "no" answer.
        return true;
    }

    // More work is required.
    return false;
}

bool Triangulation<3>::isSolidTorus() const {
    if (prop_.handlebody_.has_value())
        return (*prop_.handlebody_ == 1);

    // Basic property checks.
    if (! (isValid() && isOrientable() && isConnected() &&
            countBoundaryComponents() == 1)) {
        // This cannot be a handlebody at all.
        prop_.handlebody_ = -1;
        return false;
    }
    if (boundaryComponents().front()->eulerChar() != 0) {
        // It might be a handlebody, but it's not a solid torus.
        return false;
    }

    // From here on, if we ever deduce this is not a solid torus, then
    // we know it cannot be an orientable handlebody of *any* genus at all.

    // If it's ideal, make it a triangulation with real boundary.
    // If it's not ideal, clone it anyway so we can modify it.
    Triangulation<3> working(*this, false, false);
    working.intelligentSimplify();
    if (working.isIdeal()) {
        working.idealToFinite();
        working.intelligentSimplify();
    }

    // Check homology.
    if (! (working.homology().isZ())) {
        prop_.handlebody_ = -1;
        return false;
    }

    // So:
    // We are valid, orientable, compact and connected, with H1 = Z.
    // There is exactly one boundary component, and this is a torus.

    // Note that the homology results imply that this is not a connected
    // sum of something with S2xS1 (otherwise we would have two Z terms
    // in the homology: one from the torus boundary and one from the S2xS1).
    // This observation simplifies the crushing cases later on.

    // Pull out the big guns: normal surface time.
    while (true) {
        // INVARIANT: working is homeomorphic to our original manifold.
        if (working.countVertices() > 1) {
            // We really want a 1-vertex triangulation, since this will make
            // nonTrivialSphereOrDisc() much faster (it will be able to use
            // linear programming).
            working.minimiseVertices();
        }

        // Find a non-trivial normal disc or sphere.
        std::optional<NormalSurface> s = working.nonTrivialSphereOrDisc();
        if (! s) {
            // No non-trivial normal disc.  This cannot be a solid torus.
            prop_.handlebody_ = -1;
            return false;
        }

        // Crush it and see what happens.
        // Given what we know about the manifold so far, the only things
        // that can happen during crushing are:
        // - undo connected sum decompositions;
        // - cut along properly embedded discs;
        // - gain and/or lose 3-balls and/or 3-spheres.
        Triangulation<3> crushed = s->crush();
        s.reset(); // to avoid any automatic deep copy of the triangulation


        crushed.intelligentSimplify();

        bool found = false;
        for (Triangulation<3>& comp : crushed.triangulateComponents()) {
            // Examine each connected component after crushing.
            if (comp.isClosed()) {
                // A closed piece.
                // Must be a 3-sphere, or else we didn't have a solid torus.
                if (! comp.isSphere()) {
                    prop_.handlebody_ = -1;
                    return false;
                }
            } else if (comp.countBoundaryComponents() > 1) {
                // Multiple boundaries on the same component.
                // This should never happen, since it implies there was
                // an S2xS1 summand.
                std::cerr << "ERROR: S2xS1 summand detected in "
                    "isSolidTorus() that should not exist." << std::endl;

                // At any rate, it means we did not have a solid torus.
                prop_.handlebody_ = -1;
                return false;
            } else if (comp.boundaryComponent(0)->eulerChar() == 2) {
                // A component with sphere boundary.
                // Must be a 3-ball, or else we didn't have a solid torus.
                if (! comp.isBall()) {
                    prop_.handlebody_ = -1;
                    return false;
                }
            } else {
                // The only other possibility is a component with torus
                // boundary.  We should only see at most one of these.
                //
                // Unless some other non-trivial component was split off
                // (i.e., a non-ball and/or non-sphere that will be
                // detected separately in the tests above), this
                // component must be identical to our original manifold.
                if (found) {
                    std::cerr << "ERROR: Multiple torus boundary "
                        "components detected in isSolidTorus(), which "
                        "should not be possible." << std::endl;
                }
                working = std::move(comp);
                found = true;
            }
        }

        if (! found) {
            // We have reduced everything down to balls and spheres.
            // The only way this can happen is if we had a solid torus
            // (and we crushed and/or cut along a compressing disc
            // during the crushing operation).
            prop_.handlebody_ = 1;
            return true;
        }

        // We have the original manifold in working, but this time with
        // fewer tetrahedra.  Around we go again.
    }
}

bool Triangulation<3>::knowsSolidTorus() const {
    if (prop_.handlebody_.has_value())
        return true;

    // Run some very fast preliminary tests before we give up and say no.
    if (! (isValid() && isOrientable() && isConnected() &&
            countBoundaryComponents() == 1)) {
        // This cannot be a handlebody at all.
        prop_.handlebody_ = -1;
        return true;
    }
    if (boundaryComponents().front()->eulerChar() != 0) {
        // It might be a handlebody, but it's not a solid torus.
        // Although we are not caching the result, we still return true
        // because isSolidTorus() will nevertheless give a very fast "no"
        // answer.
        return true;
    }

    // More work is required.
    return false;
}

ssize_t Triangulation<3>::recogniseHandlebody() const {
    if ( prop_.handlebody_.has_value() ) {
        return *prop_.handlebody_;
    }

    // Basic property checks.
    // Note: an orientable 3-manifold implies that the boundary is also
    // orientable (so we do not need to test that separately).
    if (! (isValid() && isOrientable() && isConnected() &&
            countBoundaryComponents() == 1)) {
        // This cannot be a handlebody at all.
        return *(prop_.handlebody_ = -1);
    }

    // Determine the genus.
    // We can immediately check whether this is a 3-ball or a solid torus.
    size_t genus = ( 2 - boundaryComponents().front()->eulerChar() ) / 2;
    if ( genus == 0 ) {
        if ( isBall() ) {
            // A 3-ball never has a compressing disc.
            prop_.compressingDisc_ = false;
            return *(prop_.handlebody_ = 0);
        } else {
            return *(prop_.handlebody_ = -1);
        }
    }
    else if ( genus == 1 ) {
        if ( isSolidTorus() ) {
            // A solid torus always has a compressing disc.
            prop_.compressingDisc_ = true;
            return *(prop_.handlebody_ = 1);
        } else {
            return *(prop_.handlebody_ = -1);
        }
    }

    // We now know that if this is indeed an orientable handlebody, then it
    // must have genus at least 2.
    // Check that homology matches the genus.
    if ( not ( homology().isFree(genus) ) ) {
        return *(prop_.handlebody_ = -1);
    }

    // If it's ideal, make it a triangulation with real boundary.
    // If it's not ideal, clone it anyway so we can modify it.
    std::stack<Triangulation<3>> toProcess;
    Triangulation<3>& start = toProcess.emplace(*this, false, false);
    start.intelligentSimplify();
    if ( start.isIdeal() ) {
        start.idealToFinite();
        start.intelligentSimplify();
    }

    // So:
    // We are valid, orientable, compact and connected. There is exactly one
    // boundary component, and this is a genus-g torus, where g >= 2. We also
    // have H1 = gZ.
    //
    // Since "half of the boundary homology lives and the other half dies"
    // (see Lemma 3.5 of Hatcher's "Notes on Basic 3-Manifold Topology"), we
    // know that our manifold has g Z terms from the genus-g torus boundary.
    // Therefore, our manifold cannot be a connected sum of something with
    // S2xS1, as this would require additional Z terms in the homology.
    // This observation simplifies the crushing cases later on.
    //
    // Hatcher's notes are available at:
    //      https://pi.math.cornell.edu/~hatcher/3M/3Mdownloads.html
    //
    // Anyway, it's time to pull out normal surfaces.
    while ( not toProcess.empty() ) {
        // INVARIANT: Our original triangulation is an orientable handlebody
        // if and only if every triangulation in toProcess is an orientable
        // handlebody with positive genus.
        Triangulation<3>& top = toProcess.top();
        if ( top.countVertices() > 1 ) {
            // We really want a 1-vertex triangulation, since this will make
            // nonTrivialSphereOrDisc() much faster (it will be able to use
            // linear programming).
            top.minimiseVertices();
        }

        // Find a non-trivial normal disc or sphere.
        std::optional<NormalSurface> s = top.nonTrivialSphereOrDisc();
        if ( not s ) {
            // No non-trivial normal disc. This cannot be an orientable
            // handlebody.
            return *(prop_.handlebody_ = -1);
        }

        // Crush it and see what happens.
        // Given what we know about the manifold so far, the only things that
        // can happen during crushing are:
        // - undoing connected sums;
        // - cutting along properly embedded discs;
        // - gaining and/or losing 3-balls and/or 3-spheres.
        //
        // Thus, if we started with a handlebody, then crushing can only
        // result in pieces of the following types:
        // - handlebodies;
        // - 3-spheres;
        // - 3-balls.
        Triangulation<3> crushed = s->crush();
        s.reset(); // to avoid a deep copy of top when we pop
        toProcess.pop();

        crushed.intelligentSimplify();

        for ( Triangulation<3>& comp : crushed.triangulateComponents() ) {
            // Examine each connected component after crushing.
            if ( comp.isClosed() ) {
                // A closed piece.
                // Must be a 3-sphere, or else we didn't have an orientable
                // handlebody.
                if ( not comp.isSphere() ) {
                    return *(prop_.handlebody_ = -1);
                }
            } else if ( comp.countBoundaryComponents() > 1 ) {
                // Multiple boundaries on the same component.
                // CLAIM. Given what we know about the homology, this can
                //      never happen.
                // A very rough sketch of a proof for this claim is given at
                // the end of this for loop
                // TODO Throw a proper exception.
                std::cerr << "ERROR: S2xS1 summand detected in "
                    "recogniseHandlebody() that should not exist." << std::endl;

                // At any rate, it means we did not have an orientable
                // handlebody.
                return *(prop_.handlebody_ = -1);
            } else if ( comp.boundaryComponent(0)->eulerChar() == 2 ) {
                // A component with sphere boundary.
                // Must be a 3-ball, or else we didn't have a solid torus.
                if ( not comp.isBall() ) {
                    return *(prop_.handlebody_ = -1);
                }
            } else {
                // The only other possibility is a component whose boundary
                // has positive genus. We need to check that this component
                // is an orientable handlebody, so insert it back into the
                // list to process.
                toProcess.push( std::move(comp) );
            }
        }
        // PROOF OF CLAIM.
        // ==================================================================
        // Suppose crushing produced a component with multiple boundaries.
        // We must have cut some piece P along a properly embedded disc D
        // such that:
        // ---> the boundary of D separates the boundary of P into two pieces
        //      A and B; and
        // ---> D itself is non-separating, so there exists a closed curve c
        //      in P that meets D exactly once.
        // Our goal is to show that this, in combination with what we know
        // about the homology of P, leads to a contradiction.
        //
        // We first consider the case where P is the original triangulation.
        // Let g and h denote the genus of A and B, respectively, so that the
        // boundary of P has genus g+h. At the beginning of the algorithm, we
        // verified that the homology of P is (g+h)Z. With this in mind, take
        // the union of A and D, and push this slightly off the boundary to
        // obtain a closed genus-g surface S in P that meets the closed curve
        // c exactly once. Let X' be a small neighbourhood of the union of S
        // and c, and let Y' be the space given by removing the interior of X
        // from P. The intersection of X and Y is a surface R of genus 2g.
        // Let N be a thickening of R, let X be the union of X' and N, and
        // let Y be the union of Y' and N; this gives subspaces X and Y whose
        // interiors cover P, and the intersection of these two subspaces is
        // exactly N. Thus, letting G = H1(X)+H1(Y), we have the following
        // exact sequence (the Mayer-Vietoris sequence for the pair of
        // subspaces X and Y):
        //                        p       q
        //      ... ----> H1(N) ----> G ----> H1(P) ----> ...
        // We make the following homological observations:
        // ---> H1(N) = (4g)Z, because N retracts to a surface of genus 2g.
        // ---> H1(X) = (2g+1)Z, because X retracts to the union of S and c.
        // ---> H1(Y) has rank at least 2g+(g+h) because it has two boundary
        //      components, one of which has genus 2g, while the other has
        //      genus g+h.
        // ---> Thus, G has rank at least (4g+1)+(g+h)
        // ---> H1(P) = (g+h)Z, by assumption.
        // Notice that im(q) must be a free abelian group (of rank at most
        // g+h), which implies that G is isomorphic to ker(q)+im(q). Thus,
        // ker(q) must have rank at least 4g+1. By exactness, im(p) must also
        // have rank at least 4g+1, which is impossible since H1(N) only has
        // rank 4g.
        //
        // It remains to consider the case where P was obtained by some
        // sequence of crushing operations. Topologically, we know that P
        // was obtained from the original triangulation by cutting along
        // properly embedded discs. This implies that the homology of P is
        // kZ, where k is the genus of the boundary of P (one way to check
        // that cutting along discs does indeed preserve this property of the
        // homology is to again use Mayer-Vietoris sequences). Thus, by the
        // same argument as above, it is impossible for P to contain an
        // embedded disc with the properties of D.
        // ==================================================================

        // If we survived to this point, then we still haven't conclusively
        // determined whether we started with an orientable handlebody.
        // However, we have made progress: by crushing a non-trivial normal
        // surface, we guarantee that we have reduced the total number of
        // tetrahedra in the list to process, so we won't keep reaching this
        // point forever. This means that we must eventually terminate via
        // one of the following pathways:
        // ---> We detect a component that cannot occur if we started with an
        //      orientable handlebody.
        // ---> The list to process becomes empty. This can only happen if
        //      everything fell apart into 3-spheres and 3-balls, which
        //      certifies that we started with an orientable handlebody.
    }

    // The list to process became empty, so we must have started with an
    // orientable handlebody (of genus at least 2).
    // By the way, we know that this always has a compressing disc.
    prop_.compressingDisc_ = true;
    return *(prop_.handlebody_ = genus);
}

bool Triangulation<3>::knowsHandlebody() const {
    if (prop_.handlebody_.has_value()) {
        return true;
    }

    // Run some very fast preliminary tests before we give up and say no.
    // Note: an orientable 3-manifold implies that the boundary is also
    // orientable (so we do not need to test that separately).
    if (! (isValid() && isOrientable() && isConnected() &&
            countBoundaryComponents() == 1)) {
        // This cannot be a handlebody at all.
        prop_.handlebody_ = -1;
        return true;
    }

    // More work is required.
    return false;
}

bool Triangulation<3>::isTxI() const {
    // This call to knowsTxI checks basic things including validity and also
    // the number topology of the boundary components.
    if (knowsTxI())
        return *prop_.TxI_;

    Triangulation<3> working(*this, false, false);
    working.intelligentSimplify();
    working.idealToFinite();
    working.intelligentSimplify();

    // If it's not a homology T2xI, we're done.
    if ((! working.homology().isFree(2)) || (! working.homologyRel().isZ())) {
        return *(prop_.TxI_ = false);
    }

    // At this point we should already have boundary components with
    // one vertex each.
    // But out of an abundance of caution, we ensure this is the case.
    working.minimiseBoundary();

    // We have a homology T2xI with a pair of two-triangle boundaries.
    // So we should move on to the meat of the algorithm, testing Dehn fillings.

    // First we collect a boundary edge, which lets us fetch the two triangles
    // on either side along with permutations that show how they glue together.
    BoundaryComponent<3>* bc = working.boundaryComponents().front();
    Edge<3>* e = bc->edge(0);
    const FaceEmbedding<3, 1>& front = e->embedding(0);
    const FaceEmbedding<3, 1>& back = e->embedding(e->degree() - 1);
    Tetrahedron<3>* t0 = front.tetrahedron();
    Tetrahedron<3>* t1 = back.tetrahedron();
    Perm<4> p0 = front.vertices();
    Perm<4> p1 = back.vertices();
    p1 = p1 * Perm<4>(1, 0, 3, 2);

    // Now p0, p1 map {0,1,2} to the vertices of t0, t1 in a symmetric way.
    // Each boundary edge is the image of (i,j) on t0 and (j,i) on t1 for
    // distinct i,j in {0,1,2}.
    //
    // To do the three fillings, we fold the two triangles together in each
    // of the three possible ways (each of which involves swapping one of the
    // pairs 01, 12, 20 in the preimage of permutations p0, p1).
    for (int i = 0; i < 3; ++i) {
        t0->join(p0[3], t1, p1 * Perm<4>(i, (i+1) % 3) * p0.inverse());
        if (! working.isSolidTorus())
            return *(prop_.TxI_ = false);
        t0->unjoin(p0[3]);
    }

    // All three fillings give a solid torus!
    return *(prop_.TxI_ = true);
}

bool Triangulation<3>::knowsTxI() const {
    if (prop_.TxI_.has_value())
        return true;

    // Run some very fast prelimiary tests before we give up and say no.
    if (! (isValid() && isOrientable() && isConnected())) {
        prop_.TxI_ = false;
        return true;
    }

    if (countBoundaryComponents() != 2) {
        prop_.TxI_ = false;
        return true;
    }

    if (boundaryComponents().front()->eulerChar() != 0 ||
            (! boundaryComponents().front()->isOrientable()) ||
            boundaryComponents().back()->eulerChar() != 0 ||
            (! boundaryComponents().back()->isOrientable())) {
        prop_.TxI_ = false;
        return true;
    }

    // More work is required.
    return false;
}

bool Triangulation<3>::isIrreducible() const {
    if (prop_.irreducible_.has_value())
        return *prop_.irreducible_;

    // Precondition checks.
    if (! (isValid() && isClosed() && isOrientable() && isConnected()))
        return false;

    // We will essentially carry out a connected sum decomposition, but
    // instead of keeping prime summands we will just count them and
    // throw them away.
    unsigned long summands = 0;

    std::stack<Triangulation<3>> toProcess;

    // Make a working copy, simplify and record the initial homology.
    Triangulation<3>& start = toProcess.emplace(*this, false, false);
    start.intelligentSimplify();

    unsigned long Z, Z2, Z3;
    {
        AbelianGroup homology = start.homology();
        Z = homology.rank();
        Z2 = homology.torsionRank(2);
        Z3 = homology.torsionRank(3);
    }

    // Start crushing normal spheres.
    while (! toProcess.empty()) {
        // INV: Our triangulation is the connected sum of all the
        // children of toProcess, all the prime components that we threw away,
        // and possibly some copies of S2xS1, RP3 and/or L(3,1).

        // Work with the last child.
        const Triangulation<3>& top = toProcess.top();

        // Find a normal 2-sphere to crush.
        std::optional<NormalSurface> sphere = top.nonTrivialSphereOrDisc();
        if (sphere) {
            Triangulation<3> crushed = sphere->crush();
            sphere.reset(); // to avoid a deep copy of top when we pop
            toProcess.pop();

            crushed.intelligentSimplify();

            // Insert each component of the crushed triangulation back
            // into the list to process.
            if (! crushed.isEmpty()) {
                if (crushed.isConnected())
                    toProcess.push(std::move(crushed));
                else {
                    for (auto& comp : crushed.triangulateComponents())
                        toProcess.push(std::move(comp));
                }
            }
        } else {
            // We have no non-trivial normal 2-spheres!
            // The triangulation is 0-efficient (and prime).
            // Is it a 3-sphere?
            if (top.countVertices() > 1) {
                // Proposition 5.1 of Jaco & Rubinstein's 0-efficiency
                // paper:  If a closed orientable triangulation T is
                // 0-efficient then either T has one vertex or T is a
                // 3-sphere with precisely two vertices.
                //
                // It follows then that this is a 3-sphere.
                // Toss it away.
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
                if (top.octagonalAlmostNormalSphere()) {
                    // It's a 3-sphere.  Toss this component away.
                } else {
                    // It's a non-trivial prime component!
                    // Note that this will never be an S2xS1 summand;
                    // those get crushed away entirely (we account for
                    // them later).
                    if (summands > 0) {
                        // We have found more than one prime component.
                        prop_.threeSphere_ = false; // Implied by reducibility.
                        prop_.zeroEfficient_ = false; // Implied by reducibility.
                        return *(prop_.irreducible_ = false);
                    }
                    ++summands;

                    // Note which parts of our initial homology we have
                    // now accounted for.
                    AbelianGroup h1 = top.homology();
                    Z -= h1.rank();
                    Z2 -= h1.torsionRank(2);
                    Z3 -= h1.torsionRank(3);

                    // Toss away our prime summand and keep going.
                }
            }
            toProcess.pop();
        }
    }

    // Run a final homology check: were there any additional S2xS1, RP3
    // or L(3,1) terms?
    if (Z > 0) {
        // There were S2xS1 summands that were crushed away.
        // The manifold must be reducible.
        prop_.threeSphere_ = false; // Implied by reducibility.
        prop_.zeroEfficient_ = false; // Implied by reducibility.
        return *(prop_.irreducible_ = false);
    }
    if (summands + Z2 + Z3 > 1) {
        // At least two summands were found and/or crushed away: the
        // manifold must be composite.
        prop_.threeSphere_ = false; // Implied by reducibility.
        prop_.zeroEfficient_ = false; // Implied by reducibility.
        return *(prop_.irreducible_ = false);
    }

    // There are no S2xS1 summands, and the manifold is prime.
    return *(prop_.irreducible_ = true);
}

bool Triangulation<3>::knowsIrreducible() const {
    return prop_.irreducible_.has_value();
}

bool Triangulation<3>::hasCompressingDisc() const {
    if (prop_.compressingDisc_.has_value())
        return *prop_.compressingDisc_;

    // Some sanity checks; also enforce preconditions.
    if (! hasBoundaryTriangles())
        return *(prop_.compressingDisc_ = false);
    if ((! isValid()) || isIdeal())
        return *(prop_.compressingDisc_ = false);

    long minBdryEuler = 2;
    for (auto bc : boundaryComponents()) {
        if (bc->eulerChar() < minBdryEuler)
            minBdryEuler = bc->eulerChar();
    }
    if (minBdryEuler == 2)
        return *(prop_.compressingDisc_ = false);

    // Off we go.
    // Work with a simplified triangulation.
    // We keep this as a pointer because we will be switching and changing
    // triangulations.
    Triangulation<3> use(*this, false, false);
    use.intelligentSimplify();

    // Try for a fast answer first.
    if (use.hasSimpleCompressingDisc())
        return *(prop_.compressingDisc_ = true);

    // Nope.  Decide whether we can use the fast linear programming
    // machinery or whether we need to do a full vertex surface enumeration.
    if (use.isOrientable() && use.countBoundaryComponents() == 1) {
        while (true) {
            use.intelligentSimplify();

            if (use.countVertices() > 1) {
                // Try harder.
                use.minimiseVertices();
                if (use.countVertices() > 1) {
                    // We could still be stuck with more than one vertex
                    // (for example, if the original triangulation were
                    // disconnected.
                    //
                    // Fall back to a full vertex enumeration.
                    // This mirrors the code for non-orientable
                    // triangulations; see that later block for details.
                    NormalSurfaces q(use, NormalCoords::Standard);

                    for (const NormalSurface& s : q) {
                        if (s.isCompressingDisc(true))
                            return *(prop_.compressingDisc_ = true);
                    }

                    // No compressing discs!
                    return *(prop_.compressingDisc_ = false);
                }
            }

            std::vector<Triangulation<3>> pieces;
            {
                // Isolate the scope of the normal surface machinery,
                // so that there are no outstanding snapshots of the
                // original triangulation that might want to take a deep
                // copy when we modify the triangulation later on.
                TreeSingleSoln<LPConstraintEulerPositive> search(use,
                    NormalCoords::Standard);
                if (! search.find()) {
                    // No compressing discs!
                    return *(prop_.compressingDisc_ = false);
                }
                pieces = search.buildSurface().crush().triangulateComponents();
            }

            bool found = false;
            for (auto& comp : pieces) {
                if (comp.countBoundaryComponents() == 1 &&
                        comp.boundaryComponent(0)->eulerChar()
                        == minBdryEuler) {
                    // This must be our original manifold.
                    // Start again, but this time with a smaller triangulation.
                    use = std::move(comp);
                    found = true;
                    break;
                }
            }

            if (! found) {
                // We must have compressed.
                return *(prop_.compressingDisc_ = true);
            }

            // Around we go again, but this time with a smaller triangulation.
        }
    } else {
        // Sigh.  Enumerate all vertex normal surfaces.
        //
        // Hum, are we allowed to do this in quad space?  Jaco and Tollefson
        // use standard coordinates.  Jaco, Letscher and Rubinstein mention
        // quad space, but don't give details (which I'd prefer to see).
        // Leave it in standard coordinates for now.
        NormalSurfaces q(use, NormalCoords::Standard);

        // Run through all vertex surfaces looking for a compressing disc.
        for (const NormalSurface& s : q) {
            // Use the fact that all vertex normal surfaces are connected.
            if (s.isCompressingDisc(true))
                return *(prop_.compressingDisc_ = true);
        }

        // No compressing discs!
        return *(prop_.compressingDisc_ = false);
    }
}

bool Triangulation<3>::knowsCompressingDisc() const {
    if (prop_.compressingDisc_.has_value())
        return true;

    // Quickly check for non-spherical boundary components before we give up.
    for (auto bc : boundaryComponents())
        if (bc->eulerChar() < 2)
            return false;

    // All boundary components are 2-spheres.
    prop_.compressingDisc_ = false;
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
    Triangulation<3> use(*this, false, false);
    use.intelligentSimplify();

    // Check to see whether any component is a one-tetrahedron solid torus.
    for (Component<3>* c : use.components())
        if (c->size() == 1 && c->countTriangles() == 3 &&
                c->countVertices() == 1) {
            // Because we know the triangulation is valid, this rules out
            // all one-tetrahedron triangulations except for LST(1,2,3).
            return *(prop_.compressingDisc_ = true);
        }

    // Open up as many boundary triangles as possible (to make it easier to
    // find simple compressing discs).
    bool opened = true;
    while (opened) {
        opened = false;
        for (Triangle<3>* t : use.triangles())
            if (use.openBook(t, true, true)) {
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
    for (Triangle<3>* t : use.triangles()) {
        if (t->isBoundary())
            continue;

        e0 = t->edge(0);
        e1 = t->edge(1);
        e2 = t->edge(2);
        if (! (e0->isBoundary() && e1->isBoundary() && e2->isBoundary()))
            continue;

        // This could be a compressing disc.
        // Cut along the triangle to be sure.
        const TriangleEmbedding<3>& emb = t->front();

        Triangulation<3> cut(use, false, false);
        cut.tetrahedron(emb.tetrahedron()->markedIndex())->unjoin(
            emb.triangle());

        // If we don't see a new boundary component, the disc boundary is
        // non-separating in the manifold boundary and is therefore a
        // non-trivial curve.
        if (cut.countBoundaryComponents() == use.countBoundaryComponents())
            return *(prop_.compressingDisc_ = true);

        newSphereCount = 0;
        for (auto bc : cut.boundaryComponents())
            if (bc->eulerChar() == 2)
                ++newSphereCount;

        // Was the boundary of the disc non-trivial?
        if (newSphereCount == origSphereCount)
            return *(prop_.compressingDisc_ = true);
    }

    // Look for a tetrahedron with two faces folded together, giving a
    // degree-one edge on the inside and a boundary edge on the outside.
    // The boundary edge on the outside will surround a disc that cuts
    // right through the tetrahedron.
    for (Tetrahedron<3>* t : use.simplices_) {
        auto ball = SnappedBall::recognise(t);
        if (! ball)
            continue;

        int equator = ball->equatorEdge();
        if (! t->edge(equator)->isBoundary())
            continue;

        // This could be a compressing disc.
        // Cut through the tetrahedron to be sure.
        // We do this by removing the tetrahedron, and then plugging
        // both holes on either side of the disc with new copies of the
        // tetrahedron.
        int upper = ball->boundaryFace(0);

        Tetrahedron<3>* adj = t->adjacentTetrahedron(upper);
        if (! adj) {
            // The disc is trivial.
            continue;
        }

        Triangulation<3> cut(use, false, false);
        cut.tetrahedron(t->markedIndex())->unjoin(upper);
        Tetrahedron<3>* tet = cut.newTetrahedron();
        tet->join(Edge<3>::edgeVertex[equator][0], tet, Perm<4>(
            Edge<3>::edgeVertex[equator][0], Edge<3>::edgeVertex[equator][1]));
        tet->join(upper, cut.tetrahedron(adj->markedIndex()),
            t->adjacentGluing(upper));

        // If we don't see a new boundary component, the disc boundary is
        // non-separating in the manifold boundary and is therefore a
        // non-trivial curve.
        if (cut.countBoundaryComponents() == use.countBoundaryComponents())
            return *(prop_.compressingDisc_ = true);

        newSphereCount = 0;
        for (auto bc : cut.boundaryComponents())
            if (bc->eulerChar() == 2)
                ++newSphereCount;

        // Was the boundary of the disc non-trivial?
        if (newSphereCount == origSphereCount)
            return *(prop_.compressingDisc_ = true);
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
    if (prop_.haken_.has_value())
        return *prop_.haken_;

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
    Triangulation<3> t(*this, false, false);
    t.intelligentSimplify();

    // First check for an easy answer via homology:
    if (t.homology().rank() > 0) {
        prop_.threeSphere_ = false; // Implied by Hakenness.
        return *(prop_.haken_ = true);
    }

    // Enumerate vertex normal surfaces in quad coordinates.
    // std::cout << "Enumerating surfaces..." << std::endl;
    NormalSurfaces list(t, NormalCoords::Quad);

    // Run through each surface, one at a time.
    // Sort them first however, so we process the (easier) smaller genus
    // surfaces first.
    auto* id = new SurfaceID[list.size()];
    unsigned i;
    for (i = 0; i < list.size(); ++i) {
        id[i].index = i;
        id[i].euler = list[i].eulerChar().longValue();
    }
    std::sort(id, id + list.size());

    for (i = 0; i < list.size(); ++i) {
        // std::cout << "Testing surface " << i << "..." << std::endl;
        if (list[id[i].index].isIncompressible()) {
            delete[] id;
            prop_.threeSphere_ = false; // Implied by Hakenness.
            return *(prop_.haken_ = true);
        }
    }

    delete[] id;
    return *(prop_.haken_ = false);
}

bool Triangulation<3>::knowsHaken() const {
    return prop_.haken_.has_value();
}

} // namespace regina

