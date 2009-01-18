
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include "surfaces/nnormalsurface.h"
#include "surfaces/nnormalsurfacelist.h"
#include "surfaces/nsquad.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NNormalSurfaceList* NNormalSurfaceList::standardToQuad() const {
    NTriangulation* owner = getTriangulation();

    // Basic sanity checks:
    if (flavour != NNormalSurfaceList::STANDARD || ! embedded)
        return 0;
    if (owner->isIdeal() || ! owner->isValid())
        return 0;

    // Prepare a final surface list.
    NNormalSurfaceList* ans = new NNormalSurfaceList(
        NNormalSurfaceList::QUAD, true);

    // Get the empty triangulation out of the way now.
    unsigned n = owner->getNumberOfTetrahedra();
    if (n == 0) {
        owner->insertChildLast(ans);
        return ans;
    }

    // We need to get rid of vertex links entirely before we start.
    typedef const NNormalSurface* SurfacePtr;
    SurfacePtr* use = new SurfacePtr[surfaces.size()];
    unsigned long nUse = 0;

    std::vector<NNormalSurface*>::const_iterator it;
    for (it = surfaces.begin(); it != surfaces.end(); ++it)
        if (! (*it)->isVertexLinking())
            use[nUse++] = *it;

    // We want to take all surfaces with maximal zero sets in quad space.
    // That is, we want surface S if and only if there is no other surface T
    // where, for every quadrilateral coordinate where S is zero, T is
    // zero also.
    bool dominates, strict;
    unsigned tet, quad, pos;
    NNormalSurfaceVectorQuad* v;
    unsigned long i, j;
    for (i = 0; i < nUse; ++i) {
        if (use[i] == 0)
            continue;

        dominates = strict = false;

        for (j = 0; j < nUse; ++j) {
            if (j == i || use[j] == 0)
                continue;

            dominates = true;
            strict = false;
            for (tet = 0; tet < n && dominates; ++tet)
                for (quad = 0; quad < 3; ++quad)
                    if (use[i]->getQuadCoord(tet, quad) ==
                                NLargeInteger::zero &&
                            use[j]->getQuadCoord(tet, quad) !=
                                NLargeInteger::zero) {
                        dominates = false;
                        break;
                    } else if (use[i]->getQuadCoord(tet, quad) !=
                                NLargeInteger::zero &&
                            use[j]->getQuadCoord(tet, quad) ==
                                NLargeInteger::zero) {
                        // If this *does* turn out to be a domination of
                        // zero sets, we know it's strict.
                        strict = true;
                    }

            if (dominates)
                break;
        }

        if (! dominates) {
            // We want this surface.
            v = new NNormalSurfaceVectorQuad(3 * n);
            pos = 0;
            for (tet = 0; tet < n; ++tet)
                for (quad = 0; quad < 3; ++quad)
                    v->setElement(pos++, use[i]->getQuadCoord(tet, quad));
            ans->surfaces.push_back(new NNormalSurface(owner, v));
        } else if (strict) {
            // We can drop this surface entirely from our list.
            // We don't want it for our final solution set, and if
            // use[i] is going to rule out some *other* surface then
            // use[j] will rule out that same other surface also.
            //
            // The domination need to be strict because otherwise we
            // might want use[i] to rule out use[j] (i.e., they both
            // rule out each other).
            use[i] = 0;
        }
    }

    delete[] use;

    // All done!
    owner->insertChildLast(ans);
    return ans;
}

} // namespace regina

