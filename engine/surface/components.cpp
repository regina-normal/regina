
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#include <queue>
#include "surface/disc.h"
#include "surface/normalsurface.h"

namespace regina {

std::vector<NormalSurface> NormalSurface::components() const {
    if (connected_.has_value()) {
        // We already know that either the surface is empty or it is a
        // single connected component.
        if (isEmpty())
            return {};
        else
            return { *this };
    }

    // Shamelessly copied from my orientation/two-sidedness code from
    // years earlier.  Some day I will need to make a generic structure
    // for a depth-first search over normal discs.  Not today.

    // If the precondition (compactness) does not hold, things will get
    // nasty (read: infinite).
    if (! isCompact())
        return {};

    // TODO: First check that there aren't too many discs!

    // The components structure stores an integer alongside each disc;
    // that integer will be the ID of its connected component, or -1 if
    // this is unknown.  Components are numbered from 0 upwards.
    DiscSetSurfaceData<long> components(*this, -1);
        // Stores the component ID for each disc.
    std::queue<DiscSpec> discQueue;
        // A queue of discs whose component IDs must be propagated.
    DiscSpecIterator it(components);
        // Runs through the discs whose component IDs might not have yet
        // been determined.
    DiscSpec use;
        // The disc that currently holds our interest.

    int nGluingArcs;     // The number of arcs on the current disc to
                         //     which an adjacent disc might may be glued.
    Perm<4> arc[8];       // Holds each gluing arc for the current disc.

    long compID = 0;     // The current working component ID.
    long i;

    while (true) {
        // If there's no discs to propagate from, choose the next
        // one without a component label.
        while (discQueue.empty() && (! it.done())) {
            if (components.data(*it) == -1) {
                components.data(*it) = compID++;
                discQueue.push(*it);
            }
            ++it;
        }
        if (discQueue.empty())
            break;

        // At the head of the queue is the next already-labelled disc
        // whose component ID must be propagated.
        use = discQueue.front();
        discQueue.pop();

        // Determine along which arcs we may glue other discs.
        if (use.type < 4) {
            // Current disc is a triangle.
            nGluingArcs = 3;
            for (i = 0; i < 3; i++)
                arc[i] = regina::triDiscArcs[use.type][i];
        } else if (use.type < 7) {
            // Current disc is a quad.
            nGluingArcs = 4;
            for (i = 0; i < 4; i++)
                arc[i] = regina::quadDiscArcs[use.type - 4][i];
        } else {
            // Current disc is an octagon.
            nGluingArcs = 8;
            for (i = 0; i < 8; i++)
                arc[i] = regina::octDiscArcs[use.type - 7][i];
        }

        // Process any discs that might be adjacent to each of these
        // gluing arcs.
        for (i = 0; i < nGluingArcs; ++i) {
            // Establish which is the adjacent disc.
            auto adjDisc = components.adjacentDisc(use, arc[i]);
            if (! adjDisc)
                continue;

            // There is actually a disc glued along this arc.
            // Propagate the component ID.

            if (components.data(adjDisc->first) == -1) {
                components.data(adjDisc->first) = components.data(use);
                discQueue.push(adjDisc->first);
            }
        }
    }

    // Were there any discs at all?
    if (compID == 0) {
        connected_ = true;
        return {};
    }

    // Create the set of normal surfaces!
    // Note that all vectors are automagically initialised to zero.
    std::vector<NormalSurface> dest;
    std::vector<Vector<LargeInteger>> ans;
    ans.reserve(compID);

    if (couldBeAlmostNormal()) {
        size_t size = 10 * triangulation_->size();

        for (i = 0; i < compID; ++i)
            ans.emplace_back(size);

        for (const auto& disc : components)
            ++ans[components.data(disc)][10 * disc.tetIndex + disc.type];

        for (i = 0; i < compID; ++i)
            dest.emplace_back(triangulation_, NS_AN_STANDARD, ans[i]);
    } else {
        size_t size = 7 * triangulation_->size();

        for (i = 0; i < compID; ++i)
            ans.emplace_back(size);

        for (const auto& disc : components)
            ++ans[components.data(disc)][7 * disc.tetIndex + disc.type];

        for (i = 0; i < compID; ++i)
            dest.emplace_back(triangulation_, NS_STANDARD, ans[i]);
    }

    connected_ = (compID == 1);
    return dest;
}

bool NormalSurface::disjoint(const NormalSurface& other) const {
    // Some sanity tests before we begin.
    // These should all pass if the user has adhered to the preconditions.
    if (! (isCompact() && other.isCompact()))
        return false;
    if (! (isConnected() && other.isConnected()))
        return false;

    // Begin with a local compatibility test.
    if (! locallyCompatible(other))
        return false;

    // Now we know that the sum of both surfaces is an embedded surface.
    // Form the sum, pull it apart into connected components, and see
    // whether we get our original two surfaces back.
    //
    // Note: splitIntoComponents() may return surfaces that use
    // different vector encodings, but equality testing can handle this.
    std::vector<NormalSurface> bits = ((*this) + other).components();
    return (bits.size() == 2 && ((*this) == bits[0] || (*this) == bits[1]));
}

} // namespace regina

