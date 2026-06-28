
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <cstdlib>
#include "triangulation/dim3.h"
#include "utilities/randutils.h"

// If you define PINCH_NOT_COLLAPSE, then simplify() will use
// pinchEdge() instead of collapseEdge() to reduce the number of vertices.
// This may *increase* the number of tetrahedra, and so it should be used with
// great care -- it may break the requirements of simplify(), and
// so may adversely affect other code that expects monotonic behaviour.
//
// #define PINCH_NOT_COLLAPSE

namespace regina {

bool Triangulation<3>::minimiseBoundary() {
    // Regina doesn't usually check preconditions, but this one is trivial.
    if (! isValid())
        throw FailedPrecondition("minimiseBoundary() requires a "
            "valid triangulation");

    // Regarding locks on boundary triangles: we leave join() (used to perform
    // the layerings) and closeBook() (used directly for close book moves)
    // to throw a LockViolation where relevant.  We do not go out of our way
    // to find locations for these moves that would _not_ violate locks, since
    // this is a lot of work, and in typical scenarios (e.g., the entire
    // boundary is locked), this would be fruitless anyway.

    // We do not need a ChangeAndClearSpan here, since this bookkeeping is
    // already managed by closeBook() and join().  However, we do add a
    // PacketChangeGroup for optimisation, and we also add our own TopologyLock
    // because join() does not know that it will be preserving topology.
    TopologyLock lock(*this);
    PacketChangeGroup span(*this);

    bool changed = false;

    // Apologies for the use of goto, but this seems cleaner than
    // juggling breaks and continues with nested loops.
startAgain:

    // Find a boundary component to operate on.
    for (auto bc : boundaryComponents()) {
        if (bc->countTriangles() <= 2 || bc->countVertices() <= 1)
            continue;

        // This boundary component needs to be reduced in size.
        changed = true;

        // First try to use a close book move, which does not
        // increase the number of tetrahedra.
        for (auto e : bc->edges()) {
            if (closeBook(e)) {
                // We have changed the triangulation, which means
                // all edges and boundary components have been destroyed.
                // Start over.
                goto startAgain;
            }
        }

        // We could not find ourselves a close book move.
        // Instead locate a boundary edge e that joins two distinct
        // vertices and operate on this.
        for (auto e : bc->edges())
            if (e->vertex(0) != e->vertex(1)) {
                // Our plan is to layer over e, and then do a close book
                // move on the opposite edge of the layering tetrahedron.
                //
                // This would be illegal if both triangles adjacent to e
                // on the boundary were the same, but in that scenario
                // there would be a close book move on the third edge of
                // this common triangle, and so we would not have reached
                // this point in the code.
                //
                // The layer-and-close-book combination is identical to
                // attaching a snapped 3-ball to the triangles on either
                // side of e.  Here the boundary of our snapped ball will be
                // faces 012 and 013, with vertices 01 attaching to edge e.

                Tetrahedron<3>* tet1 = e->front().tetrahedron();
                Tetrahedron<3>* tet2 = e->back().tetrahedron();
                Perm<4> roles1 = e->front().vertices();
                Perm<4> roles2 = e->back().vertices();

                // At this stage, roles1 maps (0,1,2) to the tet1 tetrahedron
                // vertices for the first boundary triangle, and roles2 maps
                // (0,1,3) to the tet2 tetrahedron vertices for the second
                // boundary triangle.  In each case, (0,1) maps to the
                // endpoints of edge e.

                Tetrahedron<3>* snap = newTetrahedron();

                // At this point, all edges and boundary components have
                // been destroyed (so we cannot access edge->...).
                snap->join(0, snap, Perm<4>(0, 1));
                if (roles1.sign() < 0) {
                    snap->join(3, tet1, roles1);
                    snap->join(2, tet2, roles2);
                } else {
                    // In case the triangulation was oriented,
                    // we would like to keep it that way.
                    snap->join(3, tet1, roles1 * Perm<4>(0, 1));
                    snap->join(2, tet2, roles2 * Perm<4>(0, 1));
                }

                goto startAgain;
            }

        // We should never reach this point.
        throw ImpossibleScenario("minimiseBoundary() could not continue");
    }

    // If we fell out of the boundary component loop then all boundary
    // components are minimal, which means we are done.
    return changed;
}

bool Triangulation<3>::minimiseVertices() {
    // Start by minimising the boundary.
    // This also checks the validity precondition, and this is where we would
    // throw LockViolation exceptions if we run into locked boundary triangles.
    bool result = minimiseBoundary();

    // All that remains now is to remove internal vertices.
    // For this, we use collapseEdge() if we can, and pinchEdge() if we must.
    // No lock violations should occur from here onwards.

    // For now, we do a lot of looping through components, since each time we
    // do a move the skeleton will be recomputed entirely.  Ideally we would
    // try to remember what we have already looked at by using the more
    // persistent tetrahedron pointers instead of edge pointers.

    while (true) {
startLoop:
        for (auto* e : edges()) {
            Vertex<3>* u = e->vertex(0);
            Vertex<3>* v = e->vertex(1);
            if (u != v && ! (u->isBoundary() && v->isBoundary())) {
                // This edge needs to be pinched or collapsed.
                if (! collapseEdge(e))
                    pinchEdge(e);
                result = true;
                goto startLoop;
            }
        }

        // No edges needed to be pinched or collapsed.
        return result;
    }
}

// Set the policies for which moves/techniques to use in each simplification
// context.
//
// Regarding random 4-4 moves:
//  --- At present, the implementation of simplifyInternal() uses these in all
//      contexts, and we do not bother with providing a toggle for these. But
//      this may change in future.
//  --- The number of attempted 4-4 moves is partly determined by a
//      coefficient which may depend on the context.
//      The current implementation of simplifyInternal() allows the following
//      options:
//      --> A constant coefficient.
//      --> A coefficient which, above some minimum value, scales with the
//          size of the triangulation.
template<>
struct Triangulation<3>::SimplifyPolicy<
Triangulation<3>::SimplifyContext::Best> {
    // Policies for random 4-4 moves.
    //  --- Only modify the triangulation if we successfully reduced the size.
    //  --- Determine the number of 4-4 moves to attempt using a constant
    //      coefficient.
    static constexpr bool random44AlwaysModify = false;
    static constexpr bool scaleCoeff44 = false;
    static constexpr size_t constantCoeff44 = 5; // Used if (! scaleCoeff44)
    static constexpr size_t minimumCoeff44 = 5; // Used if (scaleCoeff44)
    static constexpr size_t scale44Divisor = 20; // Used if (scaleCoeff44)
    // Use all available techniques to simplify the triangulation as much as
    // possible.
    static constexpr bool allowMinimiseVertices = true;
    static constexpr bool allowOpenAndCloseBook = true;
    static constexpr bool allowShellBoundary = true;
    static constexpr bool allowCollapseEdge = true;
    static constexpr bool allow32 = true;
    // At present, the moves below are allowed in all contexts, but we include
    // these toggles to make future modifications easier.
    static constexpr bool allowEdge20 = true;
    static constexpr bool allowEdge21 = true;
    static constexpr bool allowVertex20 = true;
};
template<>
struct Triangulation<3>::SimplifyPolicy<
Triangulation<3>::SimplifyContext::UpDownDescent> {
    // Policies for random 4-4 moves.
    //  --- Since these moves might be helpful for escaping wells, always
    //      modify the triangulation even if we did not reduce the size.
    //  --- Determine the number of 4-4 moves to attempt using a coefficient
    //      that scales with the size. The 1/20 scaling factor is chosen based
    //      on experimentation with some hard 3-sphere triangulations, and
    //      tries to find a balance between effectiveness and running time.
    static constexpr bool random44AlwaysModify = true;
    static constexpr bool scaleCoeff44 = true;
    static constexpr size_t constantCoeff44 = 5; // Used if (! scaleCoeff44)
    static constexpr size_t minimumCoeff44 = 5; // Used if (scaleCoeff44)
    static constexpr size_t scale44Divisor = 20; // Used if (scaleCoeff44)
    // At present, "down" sequences of simplifyUpDown() only use 2-0 edge
    // moves, 2-1 edge moves, 2-0 vertex moves, and 4-4 moves.
    static constexpr bool allowMinimiseVertices = false;
    static constexpr bool allowOpenAndCloseBook = false;
    static constexpr bool allowShellBoundary = false;
    static constexpr bool allowCollapseEdge = false;
    static constexpr bool allow32 = false;
    // At present, the moves below are allowed in all contexts, but we include
    // these toggles to make future modifications easier.
    static constexpr bool allowEdge20 = true;
    static constexpr bool allowEdge21 = true;
    static constexpr bool allowVertex20 = true;
};

template <Triangulation<3>::SimplifyContext context>
bool Triangulation<3>::simplifyInternal() {
    bool changed;

    { // Begin scope for change event block.
        PacketChangeGroup span(*this);

        // Reduce to a local minimum.
        changed = simplifyToLocalMinimumInternal<context>(true);

        // If we still haven't minimised vertices, try to do this now.
        // We will throw this away if it increases the number of tetrahedra,
        // but even if the size stays the same we will keep it since
        // fewer vertices is generally better.
        if constexpr (SimplifyPolicy<context>::allowMinimiseVertices) {
            if (isValid() && ! hasMinimalVertices()) {
                Triangulation<3> tmp(*this, false, true);
                try {
                    tmp.minimiseVertices();
                } catch (const LockViolation&) {
                    // Calling minimiseVertices() could cause a lock violation if
                    // there are locked boundary triangles.  In this case it could
                    // still have performed some moves, and it guarantees that the
                    // resulting triangulation is sensible.  Keep whatever we got.
                }
                tmp.simplifyToLocalMinimumInternal<context>(true);
                if (tmp.size() <= size()) {
                    swap(tmp);
                    changed = true;
                }
            }
        }

        // Clone to work with when we might want to roll back changes.
        Triangulation<3>* use;

        // Variables used for selecting random 4-4 moves.
        std::vector<std::pair<Edge<3>*, int>> fourFourAvailable;
        std::pair<Edge<3>*, int> fourFourChoice;

        size_t fourFourAttempts, fourFourCap;
        int axis;

        while (true) {
            // --- Random 4-4 moves ---

            // Clone the triangulation and start making changes that might or
            // might not lead to a simplification.
            // If we've already simplified, or if we're happy to always modify
            // the triangulation regardless of whether we simplified, then
            // there's no need to use a separate clone since we won't need to
            // undo further changes.
            //
            // If we are cloning the triangulation, ensure we clone the locks
            // also.
            if constexpr (SimplifyPolicy<context>::random44AlwaysModify) {
                use = this;
            } else if (changed) {
                use = this;
            } else {
                use = new Triangulation<3>(*this, false, true);
            }

            // Make random 4-4 moves.
            size_t coeff44;
            if constexpr (SimplifyPolicy<context>::scaleCoeff44) {
                // For a large triangulation which might otherwise be very
                // difficult to simplify, it can help to try a bit harder with
                // random 4-4 moves.
                coeff44 = use->size() / SimplifyPolicy<context>::scale44Divisor;
                if ( coeff44 < SimplifyPolicy<context>::minimumCoeff44 ) {
                    coeff44 = SimplifyPolicy<context>::minimumCoeff44;
                }
            } else {
                coeff44 = SimplifyPolicy<context>::constantCoeff44;
            }
            fourFourAttempts = fourFourCap = 0;
            while (true) {
                // Calculate the list of available 4-4 moves.
                fourFourAvailable.clear();
                // Use edges() to ensure the skeleton has been calculated.
                for (Edge<3>* edge : use->edges())
                    for (axis = 0; axis < 2; axis++)
                        if (use->has44(edge, axis))
                            fourFourAvailable.emplace_back(edge, axis);

                // Increment fourFourCap if needed.
                if (fourFourCap < coeff44 * fourFourAvailable.size())
                    fourFourCap = coeff44 * fourFourAvailable.size();

                // Have we tried enough 4-4 moves?
                if (fourFourAttempts >= fourFourCap)
                    break;

                // Perform a random 4-4 move on the clone.
                fourFourChoice = fourFourAvailable[
                    RandomEngine::rand(fourFourAvailable.size())];
                use->move44(fourFourChoice.first, fourFourChoice.second);

                // See if we can simplify now.
                if (use->simplifyToLocalMinimumInternal<context>(true)) {
                    // We have successfully simplified!
                    // Start all over again.
                    fourFourAttempts = fourFourCap = 0;
                } else
                    ++fourFourAttempts;
            }

            // Sync the real triangulation with the clone if appropriate.
            if (use != this) {
                // At this point, changed == false.
                if (use->size() < size()) {
                    // The 4-4 moves were successful; accept them.
                    swap(*use);
                    changed = true;
                }
                delete use;
            }

            // At this point we have decided that 4-4 moves will help us
            // no more.
            if constexpr (! SimplifyPolicy<context>::allowOpenAndCloseBook) {
                break;
            }

            // --- Open book and close book moves ---

            if (hasBoundaryTriangles()) {
                // Clone again, always -- we don't want to create gratuitous
                // boundary triangles if they won't be of any help.
                //
                // Again, don't clone properties, but do clone locks.
                use = new Triangulation<3>(*this, false, true);

                // Perform every book opening move we can find.
                bool opened = false;
                bool openedNow = true;
                while (openedNow) {
                    openedNow = false;

                    for (Triangle<3>* t : use->triangles())
                        if (use->openBook(t)) {
                            opened = openedNow = true;
                            break;
                        }
                }

                // If we're lucky, we now have an edge that we can collapse.
                if (opened) {
                    if (use->simplifyToLocalMinimumInternal<context>(true)) {
                        // Yay!
                        swap(*use);
                        changed = true;
                    } else {
                        // No good.
                        // Ditch use and don't open anything.
                        opened = false;
                    }
                }

                delete use;

                // If we did any book opening stuff, start all over again.
                if (opened)
                    continue;

                // If we've made it this far then there seems to be
                // nothing left to do.
                //
                // Perform book *closing* moves to simplify the boundary
                // of the triangulation, even if this does not actually
                // reduce the number of tetrahedra.
                //
                // Since we always want to simplify the boundary, make
                // the changes directly to this triangulation.
                bool closed = false;

                for (Edge<3>* edge : edges())
                    if (closeBook(edge)) {
                        closed = true;
                        changed = true;

                        // We don't actually care whether we reduce the number
                        // of tetrahedra or not. Ignore the return value from
                        // simplifyToLocalMinimumInternal().
                        simplifyToLocalMinimumInternal<context>(true);

                        break;
                    }

                // If we *did* manage to close a book, there might be
                // further internal simplifications that we can now do.
                // Back to the top.
                if (closed)
                    continue;
            }

            // Nothing more we can do here.
            break;
        }
    } // End scope for change event span.

    return changed;
}

// Instantiate all variants of simplifyInternal().
template bool Triangulation<3>::simplifyInternal<
    Triangulation<3>::SimplifyContext::Best>();
template bool Triangulation<3>::simplifyInternal<
    Triangulation<3>::SimplifyContext::UpDownDescent>();

template <Triangulation<3>::SimplifyContext context>
bool Triangulation<3>::simplifyToLocalMinimumInternal(bool perform) {
    if (! perform) {
        ensureSkeleton();

        // Try to reduce the number of vertices.
        if constexpr (SimplifyPolicy<context>::allowCollapseEdge) {
            if (countVertices() > components().size() &&
                    countVertices() > countBoundaryComponents()) {
                for (Edge<3>* edge : edges()) {
#ifdef PINCH_NOT_COLLAPSE
                    if (edge->vertex(0) != edge->vertex(1) &&
                            (edge->vertex(0)->isInternal() ||
                             edge->vertex(1)->isInternal())) {
                        // There must be a pinch-edge move here.
                        // Note: this *increases* the number of tetrahedra.
                        // We return true anyway, since this matches the behaviour
                        // when perform == true.
                        return true;
                    }
#else
                    if (hasCollapseEdge(edge))
                        return true;
#endif
                }
            }
        }


        // Look for internal simplifications.
        for (Edge<3>* edge : edges()) {
            if constexpr (SimplifyPolicy<context>::allow32) {
                if (hasPachner(edge))
                    return true;
            }
            if constexpr (SimplifyPolicy<context>::allowEdge20) {
                if (has20(edge))
                    return true;
            }
            if constexpr (SimplifyPolicy<context>::allowEdge21) {
                if (has21(edge, 0))
                    return true;
                if (has21(edge, 1))
                    return true;
            }
        }
        if constexpr (SimplifyPolicy<context>::allowVertex20) {
            for (Vertex<3>* vertex : vertices())
                if (has20(vertex))
                    return true;
        }

        // Look for boundary simplifications.
        if constexpr (SimplifyPolicy<context>::allowShellBoundary) {
            if (hasBoundaryTriangles()) {
                for (BoundaryComponent<3>* bc : boundaryComponents()) {
                    // Run through triangles of this boundary component looking
                    // for shell boundary moves.
                    for (Triangle<3>* f : bc->facets())
                        if (hasShellBoundary(f->front().tetrahedron()))
                            return true;
                }
            }
        }

        return false;
    }

    bool changed = false;   // Has anything changed ever (for return value)?
    bool changedNow = true; // Did we just change something (for loop control)?

    { // Begin scope for change event span.
        PacketChangeGroup span(*this);

        while (changedNow) {
            changedNow = false;
            ensureSkeleton();

            // Try to reduce the number of vertices.
            if constexpr (SimplifyPolicy<context>::allowCollapseEdge) {
                if (countVertices() > components().size() &&
                        countVertices() > countBoundaryComponents()) {
                    for (Edge<3>* edge : edges()) {
#ifdef PINCH_NOT_COLLAPSE
                        if (edge->vertex(0) != edge->vertex(1) &&
                                (edge->vertex(0)->isInternal() ||
                                 edge->vertex(1)->isInternal())) {
                            // Note: this *increases* the number of tetrahedra.
                            pinchEdge(edge);
                            changedNow = changed = true;
                            break;
                        }
#else
                        if (collapseEdge(edge)) {
                            changedNow = changed = true;
                            break;
                        }
#endif
                    }
                    //TODO perform should always be true at this point.
                    if (changedNow) {
                        if (perform)
                            continue;
                        else
                            return true;
                    }
                }
            }

            // Look for internal simplifications.
            for (Edge<3>* edge : edges()) {
                if constexpr (SimplifyPolicy<context>::allow32) {
                    // Intermediate "down" sequences in simplifyUpDown() should
                    // not use 3-2 moves.
                    if (pachner(edge)) {
                        changedNow = changed = true;
                        break;
                    }
                }
                if constexpr (SimplifyPolicy<context>::allowEdge20) {
                    if (move20(edge)) {
                        changedNow = changed = true;
                        break;
                    }
                }
                if constexpr (SimplifyPolicy<context>::allowEdge21) {
                    if (move21(edge, 0)) {
                        changedNow = changed = true;
                        break;
                    }
                    if (move21(edge, 1)) {
                        changedNow = changed = true;
                        break;
                    }
                }
            }
            if (changedNow) {
                //TODO perform should always be true at this point.
                if (perform)
                    continue;
                else
                    return true;
            }
            if constexpr (SimplifyPolicy<context>::allowVertex20) {
                for (Vertex<3>* vertex : vertices())
                    if (move20(vertex)) {
                        changedNow = changed = true;
                        break;
                    }
                if (changedNow) {
                    //TODO perform should always be true at this point.
                    if (perform)
                        continue;
                    else
                        return true;
                }
            }

            // Look for boundary simplifications.
            if constexpr (SimplifyPolicy<context>::allowShellBoundary) {
                if (hasBoundaryTriangles()) {
                    for (BoundaryComponent<3>* bc : boundaryComponents()) {
                        // Run through triangles of this boundary component
                        // looking for shell boundary moves.
                        for (Triangle<3>* f : bc->facets())
                            if (shellBoundary(f->front().tetrahedron())) {
                                changedNow = changed = true;
                                break;
                            }
                        if (changedNow)
                            break;
                    }
                    if (changedNow) {
                        //TODO perform should always be true at this point.
                        if (perform)
                            continue;
                        else
                            return true;
                    }
                }
            }
        }
    } // End scope for change event span.

    return changed;
}

// Instantiate all variants of simplifyToLocalMinimumInternal().
template bool Triangulation<3>::simplifyToLocalMinimumInternal<
    Triangulation<3>::SimplifyContext::Best>(bool);
template bool Triangulation<3>::simplifyToLocalMinimumInternal<
    Triangulation<3>::SimplifyContext::UpDownDescent>(bool);

bool Triangulation<3>::simplifyUpDown( ssize_t max23, bool alwaysModify,
        ProgressTrackerObjective* tracker ) {
    //TODO Review use of tracker.
    if ( (not alwaysModify) and size() <= 1 ) {
        if (tracker) {
            tracker->setFinished();
        }
        return false;
    }

    // We want our default max23 to be big enough that this routine is often
    // effective at simplifying triangulations that are otherwise difficult
    // to simplify (otherwise there's not much point in using this routine).
    // On the other hand, we don't want the default value to result in
    // prohibitively long running times.
    if ( max23 < 0 ) {
        max23 = 128;
    }
    size_t origSize = size();

    // Set up a temporary working triangulation, just in case we end up making
    // things worse, not better.
    Triangulation<3> working( *this, false, true );

    // Random 2-3 moves.
    for ( ssize_t consec23 = 1; consec23 < 2 * max23; consec23 *= 2 ) {
        ssize_t perform23 = ( consec23 < max23 ) ? consec23 : max23;

        int coeffReps = 6;
        for ( int rep = 1; rep <= coeffReps; ++rep ) {
            if (tracker) {
                tracker->newStage( "Trying run of "
                        + std::to_string(perform23)
                        + " random 2-3 moves, round "
                        + std::to_string(rep) + "/"
                        + std::to_string(coeffReps) );
            }

            // Attempt perform23 consecutive random 2-3 moves.
            for ( ssize_t i = 0; i < perform23; ++i ) {
                // Pick a random triangle through which to do a 2-3 move.
                Triangle<3>* triangle = working.triangle(
                        RandomEngine::rand( working.countTriangles() ) );
                working.pachner(triangle);
            }

            // Start by simplifying using only 2-0, 2-1 and 4-4 moves (in
            // particular, no 3-2 moves, since we don't want to immediately undo
            // all the random 2-3 moves we just did). This hopefully pushes
            // subsequent moves to go somewhere new.
            working.simplifyInternal<SimplifyContext::UpDownDescent>();
            working.simplifyInternal<SimplifyContext::Best>();
            if (tracker) {
                tracker->setObjective( working.size() );
            }
            if ( working.size() < origSize ) {
                // We already simplified, so we might as well stop now.
                swap(working);
                if (tracker) {
                    tracker->setFinished();
                }
                return true;
            } else if ( tracker && tracker->isCancelled() ) {
                if (alwaysModify) {
                    swap(working);
                }
                tracker->setFinished();
                return false;
            }
        }
    }

    // Finish up by trying really hard to simplify.
    bool simplified = working.simplify(); // Did working.simplify() succeed?
    while (simplified) {
        simplified = working.simplify();
    }
    simplified = ( working.size() < origSize ); // Did we reduce size overall?
    if ( simplified or alwaysModify ) {
        swap(working);
    }
    if (tracker) {
        tracker->setFinished();
    }
    return simplified;
}

} // namespace regina

