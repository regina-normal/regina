
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

// Affects the number of random 4-4 moves attempted during simplification.
#define COEFF_4_4 5

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

bool Triangulation<3>::simplify() {
    bool changed;

    { // Begin scope for change event block.
        PacketChangeGroup span(*this);

        // Reduce to a local minimum.
        changed = simplifyToLocalMinimum(true);

        // If we still haven't minimised vertices, try to do this now.
        // We will throw this away if it increases the number of tetrahedra,
        // but even if the size stays the same we will keep it since
        // fewer vertices is generally better.
        if (isValid() && ! hasMinimalVertices()) {
            Triangulation<3> tmp(*this, false, true);
            try {
                tmp.minimiseVertices();
            } catch (LockViolation&) {
                // Calling minimiseVertices() could cause a lock violation if
                // there are locked boundary triangles.  In this case it could
                // still have performed some moves, and it guarantees that the
                // resulting triangulation is sensible.  Keep whatever we got.
            }
            tmp.simplifyToLocalMinimum(true);
            if (tmp.size() <= size()) {
                swap(tmp);
                changed = true;
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
            // If we've already simplified then there's no need to use a
            // separate clone since we won't need to undo further changes.
            //
            // If we are cloning the triangulation, ensure we clone the locks
            // also.
            if (changed)
                use = this;
            else {
                use = new Triangulation<3>(*this, false, true);
            }

            // Make random 4-4 moves.
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
                if (fourFourCap < COEFF_4_4 * fourFourAvailable.size())
                    fourFourCap = COEFF_4_4 * fourFourAvailable.size();

                // Have we tried enough 4-4 moves?
                if (fourFourAttempts >= fourFourCap)
                    break;

                // Perform a random 4-4 move on the clone.
                fourFourChoice = fourFourAvailable[
                    RandomEngine::rand(fourFourAvailable.size())];
                use->move44(fourFourChoice.first, fourFourChoice.second);

                // See if we can simplify now.
                if (use->simplifyToLocalMinimum(true)) {
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
                    if (use->simplifyToLocalMinimum(true)) {
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

                        // We don't actually care whether we reduce the
                        // number of tetrahedra or not.  Ignore the
                        // return value from simplifyToLocalMinimum().
                        simplifyToLocalMinimum(true);

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

bool Triangulation<3>::simplifyToLocalMinimum(bool perform) {
    if (! perform) {
        ensureSkeleton();

        // Try to reduce the number of vertices.
        if (countVertices() > components().size() &&
                countVertices() > countBoundaryComponents()) {
            for (Edge<3>* edge : edges()) {
#ifdef PINCH_NOT_COLLAPSE
                if (edge->vertex(0) != edge->vertex(1) &&
                        (edge->vertex(0)->linkType() == Vertex<3>::SPHERE ||
                         edge->vertex(1)->linkType() == Vertex<3>::SPHERE)) {
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

        // Look for internal simplifications.
        for (Edge<3>* edge : edges()) {
            if (hasPachner(edge))
                return true;
            if (has20(edge))
                return true;
            if (has21(edge, 0))
                return true;
            if (has21(edge, 1))
                return true;
        }
        for (Vertex<3>* vertex : vertices())
            if (has20(vertex))
                return true;

        // Look for boundary simplifications.
        if (hasBoundaryTriangles()) {
            for (BoundaryComponent<3>* bc : boundaryComponents()) {
                // Run through triangles of this boundary component looking
                // for shell boundary moves.
                for (Triangle<3>* f : bc->facets())
                    if (hasShellBoundary(f->front().tetrahedron()))
                        return true;
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
            if (countVertices() > components().size() &&
                    countVertices() > countBoundaryComponents()) {
                for (Edge<3>* edge : edges()) {
#ifdef PINCH_NOT_COLLAPSE
                    if (edge->vertex(0) != edge->vertex(1) &&
                            (edge->vertex(0)->linkType() == Vertex<3>::SPHERE ||
                             edge->vertex(1)->linkType() == Vertex<3>::SPHERE)) {
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
                if (changedNow) {
                    if (perform)
                        continue;
                    else
                        return true;
                }
            }

            // Look for internal simplifications.
            for (Edge<3>* edge : edges()) {
                if (pachner(edge)) {
                    changedNow = changed = true;
                    break;
                }
                if (move20(edge)) {
                    changedNow = changed = true;
                    break;
                }
                if (move21(edge, 0)) {
                    changedNow = changed = true;
                    break;
                }
                if (move21(edge, 1)) {
                    changedNow = changed = true;
                    break;
                }
            }
            if (changedNow) {
                if (perform)
                    continue;
                else
                    return true;
            }
            for (Vertex<3>* vertex : vertices())
                if (move20(vertex)) {
                    changedNow = changed = true;
                    break;
                }
            if (changedNow) {
                if (perform)
                    continue;
                else
                    return true;
            }

            // Look for boundary simplifications.
            if (hasBoundaryTriangles()) {
                for (BoundaryComponent<3>* bc : boundaryComponents()) {
                    // Run through triangles of this boundary component looking
                    // for shell boundary moves.
                    for (Triangle<3>* f : bc->facets())
                        if (shellBoundary(f->front().tetrahedron())) {
                            changedNow = changed = true;
                            break;
                        }
                    if (changedNow)
                        break;
                }
                if (changedNow) {
                    if (perform)
                        continue;
                    else
                        return true;
                }
            }
        }
    } // End scope for change event span.

    return changed;
}

} // namespace regina

