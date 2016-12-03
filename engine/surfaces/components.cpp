
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

#include <queue>
#include "surfaces/disc.h"
#include "surfaces/nsvectorstandard.h"
#include "surfaces/nsvectoranstandard.h"

namespace regina {

namespace {
    /**
     * Stores a connected component ID for a normal disc.
     */
    struct ComponentData {
        long id;
            /**< Stores the ID of the connected component that this disc
                 belongs to.  Components are numbered from 0 upwards;
                 -1 means that the component ID is unknown. */

        /**
         * Create a new structure with the component ID initialised to -1.
         */
        ComponentData() : id(-1) {
        }
    };

    /**
     * Splits the given normal surface into connected components.
     *
     * The surface itself will not be changed.  Instead, each
     * connected component will be appended to the end of the
     * given list \a dest.  Note that the list \a dest will \e not
     * be emptied at the beginning of this routine (i.e., any
     * surfaces that were in the list beforehand will be left there).
     *
     * The components inserted into \a dest will always be in standard
     * (tri-quad or tri-quad-oct) coordinates, regardless of the
     * native coordinate system that is used by the given surface.
     * Any transverse orientations will be lost.
     *
     * This routine is slow, since it performs a depth-first search
     * over the entire set of normal discs.  If the surface contains
     * a very large number of discs (large enough to cause integer
     * overflows or exhaust memory) then this routine will give up and
     * return 0.
     *
     * The components inserted into \a dest will be newly created.  It is
     * the responsibility of the caller of this routine to deallocate them.
     *
     * \pre The given normal surface is compact (has finitely many discs)
     * and is also embedded.
     *
     * \todo \prob Check for absurdly large numbers of discs and bail
     * accordingly.
     *
     * @param s the surface to split into components.
     * @param dest the vector into which individual components will
     * be inserted.
     * @return the number of connected components.
     */
    unsigned splitIntoComponents(const NormalSurface& s,
            std::vector<NormalSurface*>& dest) {
        // Shamelessly copied from my orientation/two-sidedness code from
        // years earlier.  Some day I will need to make a generic structure
        // for a depth-first search over normal discs.  Not today.

        // If the precondition (compactness) does not hold, things will get
        // nasty (read: infinite).
        if (! s.isCompact())
            return 0;

        // TODO: First check that there aren't too many discs!

        // All right.  Off we go.
        DiscSetSurfaceData<ComponentData> components(s);
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

        DiscSpec* adjDisc;  // The disc to which the current disc is glued.
        Perm<4> arc[8];       // Holds each gluing arc for the current disc.
        Perm<4> adjArc;       // Represents the corresponding gluing arc on the
                             //     adjacent disc.

        long compID = 0;     // The current working component ID.
        long i;

        while (true) {
            // If there's no discs to propagate from, choose the next
            // one without a component label.
            while (discQueue.empty() && (! it.done())) {
                if (components.data(*it).id == -1) {
                    components.data(*it).id = compID++;
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
                    arc[i] = regina::triDiscArcs(use.type, i);
            } else if (use.type < 7) {
                // Current disc is a quad.
                nGluingArcs = 4;
                for (i = 0; i < 4; i++)
                    arc[i] = regina::quadDiscArcs(use.type - 4, i);
            } else {
                // Current disc is an octagon.
                nGluingArcs = 8;
                for (i = 0; i < 8; i++)
                    arc[i] = regina::octDiscArcs(use.type - 7, i);
            }

            // Process any discs that might be adjacent to each of these
            // gluing arcs.
            for (i = 0; i < nGluingArcs; ++i) {
                // Establish which is the adjacent disc.
                adjDisc = components.adjacentDisc(use, arc[i], adjArc);
                if (adjDisc == 0)
                    continue;

                // There is actually a disc glued along this arc.
                // Propagate the component ID.

                if (components.data(*adjDisc).id == -1) {
                    components.data(*adjDisc).id = components.data(use).id;
                    discQueue.push(*adjDisc);
                }

                // Tidy up.
                delete adjDisc;
            }
        }

        // Were there any discs at all?
        if (compID == 0)
            return 0;

        // Create the set of normal surfaces!
        // Note that all vectors are automagically initialised to zero.
        const Triangulation<3>* tri = s.triangulation();
        NormalSurfaceVector** ans = new NormalSurfaceVector*[compID];

        NormalSurfaceVector* vec;
        long coord;
        if (s.systemAllowsAlmostNormal()) {
            for (i = 0; i < compID; ++i)
                ans[i] = new NSVectorANStandard(
                    10 * tri->size());

            for (it.init(components); ! it.done(); ++it) {
                vec = ans[components.data(*it).id];
                coord = 10 * (*it).tetIndex + (*it).type;
                vec->setElement(coord, (*vec)[coord] + 1);
            }
        } else {
            for (i = 0; i < compID; ++i)
                ans[i] = new NSVectorStandard(
                    7 * tri->size());

            for (it.init(components); ! it.done(); ++it) {
                vec = ans[components.data(*it).id];
                coord = 7 * (*it).tetIndex + (*it).type;
                vec->setElement(coord, (*vec)[coord] + 1);
            }
        }

        for (i = 0; i < compID; ++i)
            dest.push_back(new NormalSurface(tri, ans[i]));
        delete[] ans;

        // All done!
        return compID;
    }
} // anonymous namespace

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
    NormalSurfaceVector* v =
        static_cast<NormalSurfaceVector*>(vector->clone());
    (*v) += *(other.vector);
    NormalSurface* sum = new NormalSurface(triangulation_, v);

    typedef std::vector<NormalSurface*> CompVector;
    CompVector bits;
    splitIntoComponents(*sum, bits);

    bool ans = false;
    if (bits.size() == 2)
        for (int c = 0; c < 2; ++c)
            if (sameSurface(*bits[c])) {
                ans = true;
                break;
            }

    for (CompVector::iterator it = bits.begin(); it != bits.end(); ++it)
        delete *it;
    delete sum;

    return ans;
}

} // namespace regina

