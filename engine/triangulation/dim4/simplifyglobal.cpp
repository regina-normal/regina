
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

#include <cstdlib>
#include "triangulation/dim4.h"
#include "utilities/randutils.h"

namespace regina {

template <Triangulation<4>::SimplifyContext context>
bool Triangulation<4>::simplifyInternal() {
    bool changed;

    { // Begin scope for change event span.
        PacketChangeGroup span(*this);

        // Reduce to a local minimum.
        changed = simplifyToLocalMinimumInternal<context>(true);

        // Clone to work with when we might want to roll back changes.
        Triangulation<4>* use;

        // Variables for selecting random 3-3 moves.
        std::vector<Triangle<4>*> threeThreeAvailable;
        Triangle<4>* threeThreeChoice;

        unsigned long threeThreeAttempts;
        unsigned long threeThreeCap;

        while (true) {
            // --- Random 3-3 moves ---

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
                use = new Triangulation<4>(*this, false, true);
            }

            // Make random 3-3 moves.
            static constexpr int COEFF_3_3 =
                (context == SimplifyContext::UpDownDescent ? 200 : 10);

            threeThreeAttempts = threeThreeCap = 0;
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
                threeThreeChoice = threeThreeAvailable[
                    RandomEngine::rand(threeThreeAvailable.size())];
                use->pachner(threeThreeChoice, regina::unprotected);

                // See if we can simplify now.
                if (use->simplifyToLocalMinimumInternal<context>(true)) {
                    // We have successfully simplified!
                    // Start all over again.
                    threeThreeAttempts = threeThreeCap = 0;
                } else
                    threeThreeAttempts++;
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

            // At this point we have decided that 3-3 moves will help us
            // no more.

            if constexpr (context == SimplifyContext::UpDownDescent) {
                // In this context, we are not allowed to try any other moves.
                break;
            }

            // --- Open book moves ---

            if (hasBoundaryTetrahedra()) {
                // Clone again, always -- we don't want to create gratuitous
                // boundary facets if they won't be of any help.
                //
                // Again, don't clone properties, but do clone locks.
                use = new Triangulation<4>(*this, false, true);

                // Perform every book opening move we can find.
                bool opened = false;
                bool openedNow = true;
                while (openedNow) {
                    openedNow = false;

                    for (Tetrahedron<4>* tet : use->tetrahedra())
                        if (use->openBook(tet, true, true)) {
                            opened = openedNow = true;
                            break;
                        }
                }

                // If we're lucky, we can now simplify further.
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
            }

            // Nothing more we can do here.
            break;
        }
    } // End scope for change event span.

    return changed;
}

// Instantiate all variants of simplifyInternal().
template bool Triangulation<4>::simplifyInternal<
    Triangulation<4>::SimplifyContext::Best>();
template bool Triangulation<4>::simplifyInternal<
    Triangulation<4>::SimplifyContext::UpDownDescent>();

template <Triangulation<4>::SimplifyContext context>
bool Triangulation<4>::simplifyToLocalMinimumInternal(bool perform) {
    if (! perform) {
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

            if constexpr (context != SimplifyContext::UpDownDescent) {
                // Crush edges if we can.
                if (countVertices() > countComponents() &&
                        countVertices() > countBoundaryComponents()) {
                    for (Edge<4>* e : edges()) {
                        if (collapseEdge(e)) {
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
    Triangulation<4>::SimplifyContext::Best>(bool);
template bool Triangulation<4>::simplifyToLocalMinimumInternal<
    Triangulation<4>::SimplifyContext::UpDownDescent>(bool);

bool Triangulation<4>::simplifyUpDown(ssize_t max24, ssize_t max33,
        bool alwaysModify) {
    if ((! alwaysModify) && size() <= 2)
        return false;

    // Set up some sensible default arguments.
    if (max24 < 0)
        max24 = 10;
    if (max33 < 0)
        max33 = max24 * 3;

    size_t initEdges = countEdges();

    // Set up a temporary working triangulation, just in case we end up making
    // things worse, not better.
    Triangulation<4> working(*this, false, true);

    for (ssize_t attempts = 1; attempts <= max24; ++attempts) {
        // Do attempts successive 2-4 moves.
        for (ssize_t i=0; i<attempts; i++) {
            for (auto tet : working.tetrahedra()) {
                if (working.pachner(tet)) {
                    break;
                }
            }
        }

        // Simplify using only 2-0 edge/triangle moves and 3-3 moves.
        working.simplifyInternal<SimplifyContext::UpDownDescent>();

        if (working.countEdges() < initEdges) {
            // We simplified!
            swap(working);
            return true;
        }

        // Make the requested number of 3-3 moves.
        for (int i=0; i<max33; i++) {
            for (auto tri : working.triangles()) {
                if (working.pachner(tri)) {
                    break;
                }
            }
        }
    }

    // We never reduced the number of edges.
    if (alwaysModify)
        swap(working);
    return false;
}

} // namespace regina

