
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

#include "surface/normalsurface.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"

namespace regina {

void NormalSurfaces::buildReducedFromStandard(
        const std::vector<NormalSurface>& stdList) {
    // Get the empty triangulation out of the way now.
    unsigned long n = triangulation_->size();
    if (n == 0)
        return;

    bool almostNormal = allowsAlmostNormal();

    // We need to get rid of vertex links entirely before we start.
    // Build a new list of pointers to the (non-vertex-linking) surfaces
    // that we are interested in.
    auto* use = new const NormalSurface*[stdList.size()];
    size_t nUse = 0;

    for (const NormalSurface& s : stdList)
        if (! s.isVertexLinking())
            use[nUse++] = &s;

    // We want to take all surfaces with maximal zero sets in quad space.
    // That is, we want surface S if and only if there is no other surface T
    // where, for every quad coordinate where S is zero, T is zero also.
    // For almost normal surfaces, simply replace "quad coordinate" with
    // "quad or oct coordinate".
    unsigned tet;
    for (size_t i = 0; i < nUse; ++i) {
        if (! use[i])
            continue;

        bool dominates = false;
        bool strict = false;

        for (size_t j = 0; j < nUse; ++j) {
            if (j == i || ! use[j])
                continue;

            dominates = true;
            strict = false;
            for (tet = 0; tet < n && dominates; ++tet) {
                // Check the zero/non-zero status of the quads.
                for (int type = 0; type < 3; ++type) {
                    if (use[i]->quads(tet, type) == 0 &&
                            use[j]->quads(tet, type) != 0) {
                        dominates = false;
                        break;
                    } else if (use[i]->quads(tet, type) != 0 &&
                            use[j]->quads(tet, type) == 0) {
                        // If this *does* turn out to be a domination of
                        // zero sets, we know it's strict.
                        strict = true;
                    }
                }
                // Likewise for the octagons.
                if (almostNormal && dominates) {
                    for (int type = 0; type < 3; ++type) {
                        if (use[i]->octs(tet, type) == 0 &&
                                use[j]->octs(tet, type) != 0) {
                            dominates = false;
                            break;
                        } else if (use[i]->octs(tet, type) != 0 &&
                                use[j]->octs(tet, type) == 0) {
                            strict = true;
                        }
                    }
                }
            }

            if (dominates)
                break;
        }

        if (! dominates) {
            // We want this surface.
            // Although we now have vertices in a different coordinate
            // system, the encoding should not have changed.

            // TODO: However.. we *should* give the encoding the extra
            // flag that guarantees it is not vertex linking.
            // Not doing so is harmless but a bit wasteful.
            // However, the entire standard-to-quad conversion is also
            // somewhat unnecessary and wasteful, so leave this alone for now.
            surfaces_.push_back(*use[i]);
        } else if (strict) {
            // We can drop this surface entirely from our list.
            // We don't want it for our final solution set, and if
            // use[i] is going to rule out some *other* surface then
            // use[j] will rule out that same other surface also.
            //
            // The domination need to be strict because otherwise we
            // might want use[i] to rule out use[j] (i.e., they both
            // rule out each other).
            use[i] = nullptr;
        }
    }

    delete[] use;
}

} // namespace regina

