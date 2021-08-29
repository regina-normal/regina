
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

#include "surfaces/normalsurface.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"

namespace regina {

// Although internalStandardToReduced() is a template routine, we implement
// it here in this C++ file to avoid dragging it into the headers.
//
// The following definitions should ensure that the template is fully
// instantiated where it needs to be.

NormalSurfaces* NormalSurfaces::standardToQuad() const {
    return internalStandardToReduced<NormalSpec>();
}

NormalSurfaces* NormalSurfaces::standardANToQuadOct() const {
    return internalStandardToReduced<AlmostNormalSpec>();
}

template <class Variant>
NormalSurfaces* NormalSurfaces::internalStandardToReduced() const {
    // And off we go!
    const Triangulation<3>& owner = triangulation();

    // Basic sanity checks:
    if (coords_ != Variant::standardCoords)
        return nullptr;
    if (which_ != (NS_EMBEDDED_ONLY | NS_VERTEX))
        return nullptr;
    if (owner.isIdeal() || ! owner.isValid())
        return nullptr;

    // Prepare a final surface list.
    NormalSurfaces* ans = new NormalSurfaces(
        Variant::reducedCoords, NS_EMBEDDED_ONLY | NS_VERTEX, NS_ALG_CUSTOM);

    // Get the empty triangulation out of the way now.
    unsigned long n = owner.size();
    if (n == 0) {
        parent()->insertChildLast(ans);
        return ans;
    }

    // We need to get rid of vertex links entirely before we start.
    // Build a new list of pointers to the (non-vertex-linking) surfaces
    // that we are interested in.
    typedef const Vector<LargeInteger>* VectorPtr;
    VectorPtr* use = new VectorPtr[surfaces_.size()];
    size_t nUse = 0;

    for (const NormalSurface& s : surfaces_)
        if (! s.isVertexLinking())
            use[nUse++] = &s.vector();

    // We want to take all surfaces with maximal zero sets in quad space.
    // That is, we want surface S if and only if there is no other surface T
    // where, for every quad coordinate where S is zero, T is zero also.
    // For almost normal surfaces, simply replace "quad coordinate" with
    // "quad or oct coordinate".
    unsigned tet, quad, pos;
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
            for (tet = 0; tet < n && dominates; ++tet)
                for (quad = 0; quad < Variant::reducedPerTet; ++quad)
                    if ((*use[i])[Variant::stdPos(tet, 4 + quad)] ==
                                LargeInteger::zero &&
                            (*use[j])[Variant::stdPos(tet, 4 + quad)] !=
                                LargeInteger::zero) {
                        dominates = false;
                        break;
                    } else if ((*use[i])[Variant::stdPos(tet, 4 + quad)] !=
                                LargeInteger::zero &&
                            (*use[j])[Variant::stdPos(tet, 4 + quad)] ==
                                LargeInteger::zero) {
                        // If this *does* turn out to be a domination of
                        // zero sets, we know it's strict.
                        strict = true;
                    }

            if (dominates)
                break;
        }

        if (! dominates) {
            // We want this surface.
            Vector<LargeInteger> v(Variant::redLen(n));
            pos = 0;
            for (tet = 0; tet < n; ++tet)
                for (quad = 0; quad < Variant::reducedPerTet; ++quad)
                    v[pos++] = (*use[i])[Variant::stdPos(tet, 4 + quad)];
            ans->surfaces_.push_back(NormalSurface(owner,
                Variant::reducedCoords, std::move(v)));
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

    // All done!
    parent()->insertChildLast(ans);
    return ans;
}

} // namespace regina

