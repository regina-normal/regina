
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include <vector>
#include "surfaces/normalsurfaces.h"
#include "surfaces/surfacefilter.h"
#include "triangulation/dim3.h"

namespace regina {

NormalSurfaces* NormalSurfaces::filter(const SurfaceFilter* filter) const {
    NormalSurfaces* ans = new NormalSurfaces(
        coords_,
            (which_ & (NS_EMBEDDED_ONLY | NS_IMMERSED_SINGULAR)) | NS_CUSTOM,
            algorithm_ | NS_ALG_CUSTOM);

    for (const NormalSurface& s : surfaces_)
        if (filter->accept(s))
            ans->surfaces_.push_back(s);

    parent()->insertChildLast(ans);
    return ans;
}

NormalSurfaces* NormalSurfaces::filterForLocallyCompatiblePairs()
        const {
    // Sanity check:
    if (! isEmbeddedOnly())
        return nullptr;

    NormalSurfaces* ans = new NormalSurfaces(
        coords_, NS_CUSTOM | NS_EMBEDDED_ONLY, NS_ALG_CUSTOM);

    // Find all surfaces that have a compatible partner.
    std::vector<NormalSurface>::const_iterator first, second;
    for (first = surfaces_.begin(); first != surfaces_.end(); ++first) {
        for (second = surfaces_.begin(); second != surfaces_.end();
                ++second) {
            if (second == first)
                continue;

            if (first->locallyCompatible(*second)) {
                ans->surfaces_.push_back(*first);
                break;
            }
        }
    }

    parent()->insertChildLast(ans);
    return ans;
}

NormalSurfaces* NormalSurfaces::filterForDisjointPairs() const {
    // Sanity check:
    if (! isEmbeddedOnly())
        return nullptr;

    NormalSurfaces* ans = new NormalSurfaces(
        coords_, NS_CUSTOM | NS_EMBEDDED_ONLY, NS_ALG_CUSTOM);

    // Collect all the surfaces that we might care about.
    // This means non-empty, connected and compact.
    std::vector<const NormalSurface*> interesting;
    for (const NormalSurface& s : surfaces_) {
        if (s.isEmpty())
            continue;
        if (! s.isCompact())
            continue;
        if (! s.isConnected())
            continue;

        interesting.push_back(&s);
    }

    // Find all surfaces that have a disjoint partner.
    std::vector<const NormalSurface*>::iterator first, second;
    for (first = interesting.begin(); first != interesting.end(); ++first) {
        for (second = interesting.begin(); second != interesting.end();
                ++second) {
            if (second == first)
                continue;

            if ((*first)->disjoint(**second)) {
                ans->surfaces_.push_back(**first);
                break;
            }
        }
    }

    parent()->insertChildLast(ans);
    return ans;
}

NormalSurfaces* NormalSurfaces::filterForPotentiallyIncompressible()
        const {
    // Sanity check:
    if (! isEmbeddedOnly())
        return nullptr;

    NormalSurfaces* ans = new NormalSurfaces(
        coords_, NS_CUSTOM | NS_EMBEDDED_ONLY, NS_ALG_CUSTOM);

    Triangulation<3>* t;
#ifdef DEBUG
    int which = 0;
#endif
    for (const NormalSurface& s : surfaces_) {
#ifdef DEBUG
        std::cout << "Processing surface " << which++ << "..." << std::endl;
#endif
        if (s.isVertexLinking())
            continue;
        if (s.isThinEdgeLink().first)
            continue;

        // If we have a one-sided surface, don't worry about taking the
        // two-sided double cover.  If the complement of the one-sided
        // surface has a compressing disc, then the complement of the
        // double cover has the same compressing disc, and this surface
        // can happily be tossed away.
        t = s.cutAlong();
        if (! t->hasSimpleCompressingDisc())
            ans->surfaces_.push_back(s);
        delete t;
    }

    parent()->insertChildLast(ans);
    return ans;
}

} // namespace regina

