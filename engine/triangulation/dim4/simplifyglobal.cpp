
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
#include "triangulation/dim4.h"
#include "utilities/randutils.h"

namespace regina {

template <Triangulation<4>::SimplifyContext context>
bool Triangulation<4>::simplifyToLocalMinimumInternal(bool perform,
        ProgressTrackerObjective* tracker) {
    if (! perform) {
        // In this scenario there should be no progress tracker.
        ensureSkeleton();

        if constexpr (context != SimplifyContext::UpDownDescent) {
            // Crush edges if we can.
            if (countVertices() > countComponents() &&
                    countVertices() > countBoundaryComponents())
                for (Edge<4>* e : edges())
                    if (hasCollapseEdge(e))
                        return true;
        }

        // Look for internal simplifications.
        // Our order of tests follows the case where perform is true.
        for (Edge<4>* e : edges())
            if (has20(e))
                return true;

        for (Triangle<4>* t : triangles())
            if (has20(t))
                return true;

        if constexpr (context == SimplifyContext::UpDownDescent) {
            // In this context, we are not allowed to try any other moves.
            return false;
        }

        for (Vertex<4>* v : vertices())
            if (has20(v))
                return true;

        for (Edge<4>* e : edges())
            if (hasPachner(e))
                return true;

        // Look for boundary simplifications.
        if (hasBoundaryTetrahedra()) {
            for (BoundaryComponent<4>* bc : boundaryComponents())
                for (Tetrahedron<4>* f : bc->facets())
                    if (hasShellBoundary(f->front().pentachoron()))
                        return true;
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

            if (tracker && tracker->isCancelled())
                return changed;

            if constexpr (context != SimplifyContext::UpDownDescent) {
                // Crush edges if we can.
                if (countVertices() > countComponents() &&
                        countVertices() > countBoundaryComponents()) {
                    for (Edge<4>* e : edges()) {
                        if (collapseEdge(e)) {
                            changedNow = changed = true;
                            if (tracker)
                                tracker->setObjective(size());
                            break;
                        }
                    }
                    if (changedNow) {
                        if (perform)
                            continue;
                        else
                            return true;
                    }
                }
            }

            // Look for internal simplifications.

            // Experience suggests that 2-0 moves are more important to
            // "unblock" other moves, and we should leave the simpler
            // 4-2 moves until last.
            //
            // We prioritise edge moves, since in general we are trying to
            // reduce the number of edges.
            for (Edge<4>* e : edges()) {
                if (move20(e)) {
                    changedNow = changed = true;
                    if (tracker)
                        tracker->setObjective(size());
                    break;
                }
            }
            if (changedNow) {
                if (perform)
                    continue;
                else
                    return true;
            }

            for (Triangle<4>* t : triangles()) {
                if (move20(t)) {
                    changedNow = changed = true;
                    if (tracker)
                        tracker->setObjective(size());
                    break;
                }
            }
            if (changedNow) {
                if (perform)
                    continue;
                else
                    return true;
            }

            if constexpr (context == SimplifyContext::UpDownDescent) {
                // In this context, we are not allowed to try any other moves.
                break;
            }

            for (Vertex<4>* v : vertices()) {
                if (move20(v)) {
                    changedNow = changed = true;
                    if (tracker)
                        tracker->setObjective(size());
                    break;
                }
            }
            if (changedNow) {
                if (perform)
                    continue;
                else
                    return true;
            }

            for (Edge<4>* e : edges()) {
                if (pachner(e)) {
                    changedNow = changed = true;
                    if (tracker)
                        tracker->setObjective(size());
                    break;
                }
            }
            if (changedNow) {
                if (perform)
                    continue;
                else
                    return true;
            }

            // Look for boundary simplifications.
            if (hasBoundaryTetrahedra()) {
                for (BoundaryComponent<4>* bc : boundaryComponents()) {
                    // Run through facets of this boundary component looking
                    // for shell boundary moves.
                    for (Tetrahedron<4>* f : bc->facets())
                        if (shellBoundary(f->front().pentachoron())) {
                            changedNow = changed = true;
                            if (tracker)
                                tracker->setObjective(size());
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

// Instantiate all variants of simplifyToLocalMinimumInternal().
template bool Triangulation<4>::simplifyToLocalMinimumInternal<
    Triangulation<4>::SimplifyContext::Best>(
    bool, ProgressTrackerObjective*);
template bool Triangulation<4>::simplifyToLocalMinimumInternal<
    Triangulation<4>::SimplifyContext::UpDownDescent>(
    bool, ProgressTrackerObjective*);

template <Triangulation<4>::SimplifyContext context>
bool Triangulation<4>::simplifyGreedyInternal(
        ProgressTrackerObjective* tracker) {
    bool changed = false;

    PacketChangeGroup span(*this);

    // ---------- Reduce to local minimum ----------

    if (tracker)
        tracker->newStage("Reducing to local minimum");
    if (simplifyToLocalMinimumInternal<context>(true, tracker)) {
        if (tracker)
            if (! tracker->setObjective(size()))
                return true; // cancelled, but triangulation was improved
        changed = true;
    } else if (tracker && tracker->isCancelled())
        return false; // cancelled, and triangulation was not improved

    // ---------- Try random "sideways" moves ----------

    // The pool of available 3-3 moves that we will randomly select from.
    std::vector<Triangle<4>*> threeThreeAvailable;

    bool allowOpenBook;
    if constexpr (context == SimplifyContext::UpDownDescent) {
        // In this context, we are not allowed to use book-opening moves.
        allowOpenBook = false;
    } else {
        // We need boundary tetrahedra for book-opening moves to make sense.
        // Note: whether boundary tetrahedra exist should remain the same as
        // the simplification algorithm runs, so precomputing this now is fine.
        allowOpenBook = hasBoundaryTetrahedra();
    }

    if (tracker) {
        if (allowOpenBook)
            tracker->newStage("Trying 3-3 and open-book moves");
        else
            tracker->newStage("Trying 3-3 moves");
    }

    while (true) {
        // We will try (1) 3-3 moves always, and (2) open-book moves if allowed.

        // --- Random 3-3 moves ---

        // Clone the triangulation and start making changes that might or
        // might not lead to a simplification.
        // If we've already simplified then there's no need to use a
        // separate clone since we won't need to undo further changes.
        //
        // If we _are_ cloning the triangulation, ensure we clone the locks
        // also.
        Triangulation<4>* use = (changed ? this :
            new Triangulation<4>(*this, false, true));

        // Make random 3-3 moves.
        static constexpr int COEFF_3_3 =
            (context == SimplifyContext::UpDownDescent ? 200 : 10);

        size_t threeThreeAttempts = 0;
        size_t threeThreeCap = 0;
        while (true) {
            // Calculate the list of available 3-3 moves.
            threeThreeAvailable.clear();
            // Use triangles() to ensure the skeleton has been calculated.
            for (Triangle<4>* triangle : use->triangles())
                if (use->hasPachner(triangle))
                    threeThreeAvailable.push_back(triangle);

            // Increment threeThreeCap if needed.
            if (threeThreeCap < COEFF_3_3 * threeThreeAvailable.size())
                threeThreeCap = COEFF_3_3 * threeThreeAvailable.size();

            // Have we tried enough 3-3 moves?
            if (threeThreeAttempts >= threeThreeCap)
                break;

            // Perform a random 3-3 move on the clone.
            Triangle<4>* threeThreeChoice = threeThreeAvailable[
                RandomEngine::rand(threeThreeAvailable.size())];
            use->pachner(threeThreeChoice, regina::unprotected);

            // See if we can simplify now.
            if (use->simplifyToLocalMinimumInternal<context>(true, tracker)) {
                // We have successfully simplified!
                if (tracker)
                    if (! tracker->setObjective(use->size())) {
                        // The operation was cancelled: stop making 3-3 moves,
                        // but don't return until we collect results from *use.
                        break;
                    }
                // Start all over again.
                threeThreeAttempts = threeThreeCap = 0;
            } else {
                if (tracker && tracker->isCancelled())
                    break; // as above: don't return until we collect results
                threeThreeAttempts++;
            }
        }

        // Sync the real triangulation with the clone if appropriate.
        if (use != this) {
            // At this point, changed == false.
            if (use->size() < size()) {
                // The 3-3 moves were successful; accept them.
                swap(*use);
                changed = true;
            }
            delete use;
        }

        // Now that we have collected results from *use, we can finally return
        // if the operation was cancelled.
        if (tracker && tracker->isCancelled())
            return changed;

        // --- Open book moves ---

        if (allowOpenBook) {
            // Clone again, always -- we don't want to create gratuitous
            // boundary facets if they won't be of any help.
            //
            // Again, don't clone properties, but do clone locks.
            Triangulation<4> working(*this, false, true);

            // Perform every book opening move we can find.
            bool opened = false;
            bool openedNow = true;
            while (openedNow) {
                openedNow = false;

                for (Tetrahedron<4>* tet : working.tetrahedra())
                    if (working.openBook(tet)) {
                        opened = openedNow = true;
                        break;
                    }
            }

            // If we're lucky, we can now simplify further.
            if (opened) {
                if (working.simplifyToLocalMinimumInternal<context>(
                        true, tracker)) {
                    // Yay!
                    swap(working);
                    if (tracker)
                        if (! tracker->setObjective(size()))
                            return true; // cancelled, and improved
                    changed = true;
                } else {
                    // No good.
                    // Throw away working, and ignore our open books.
                    if (tracker && tracker->isCancelled())
                        return changed;
                    opened = false;
                }
            } else if (tracker && tracker->isCancelled())
                return changed;

            // If we did any book opening stuff, start all over again.
            if (opened)
                continue;
        }

        // Nothing more we can do here.
        break;
    }

    return changed;
}

// Instantiate all variants of simplifyGreedyInternal().
template bool Triangulation<4>::simplifyGreedyInternal<
    Triangulation<4>::SimplifyContext::Best>(ProgressTrackerObjective*);
template bool Triangulation<4>::simplifyGreedyInternal<
    Triangulation<4>::SimplifyContext::UpDownDescent>(ProgressTrackerObjective*);

bool Triangulation<4>::simplifyUpDownInternal(ssize_t max24, ssize_t max33,
        bool alwaysModify, ProgressTrackerObjective* tracker) {
    if ((! alwaysModify) && size() <= 2)
        return false;

    // Set up some sensible default arguments.
    if (max24 < 0)
        max24 = 10;
    if (max33 < 0)
        max33 = max24 * 3;

    size_t initSize = size();

    // Set up a temporary working triangulation, just in case we end up making
    // things worse, not better.
    Triangulation<4> working(*this, false, true);

    for (ssize_t attempts = 1; attempts <= max24; ++attempts) {
        if (tracker)
            tracker->newStage("Trying run of " + std::to_string(attempts)
                + "× 2-4 moves");

        // Do attempts successive 2-4 moves.
        for (ssize_t i=0; i<attempts; i++)
            for (auto tet : working.tetrahedra())
                if (working.pachner(tet))
                    break;

        // Simplify using only 2-0 edge/triangle moves and 3-3 moves.
        // Do not bother with the tracker for this.
        working.simplifyGreedyInternal<SimplifyContext::UpDownDescent>(nullptr);

        if (working.size() < initSize) {
            // We simplified!
            swap(working);
            if (tracker)
                tracker->setObjective(size());
            return true;
        } else if (tracker && tracker->isCancelled())
            return false;

        // Make the requested number of 3-3 moves.
        for (int i=0; i<max33; i++)
            for (auto tri : working.triangles())
                if (working.pachner(tri))
                    break;

        if (tracker && tracker->isCancelled())
            return false;
    }

    // We never reduced the number of pentachora.
    if (alwaysModify)
        swap(working);
    return false;
}

bool Triangulation<4>::simplify(ProgressTrackerObjective* tracker) {
    // For now, we try our greedy heuristics followed by one round of up-down
    // simplify (regardless of whether the greedy heuritics worked).

    // Regarding progress trackers: our internal functions will manage the
    // stages and objective values.  We just need to check for cancellation.

    bool changed;
    {
        PacketChangeGroup span(*this);

        // Stage 1: greedy heuristics
        changed = simplifyGreedyInternal<SimplifyContext::Best>(tracker);

        // Stage 2: up-down simplify
        if (! (tracker && tracker->isCancelled()))
            changed |= simplifyUpDownInternal(-1, -1, false, tracker);
    }

    // Now that the packet change span is closed, no more actions or updates
    // will be triggered from our end.  We can safely mark the progress
    // tracker as finished.
    if (tracker)
        tracker->setFinished();
    return changed;
}

} // namespace regina

