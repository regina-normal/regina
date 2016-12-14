
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

#include <manifold/graphloop.h>
#include <manifold/graphpair.h>
#include <manifold/graphtriple.h>
#include <manifold/lensspace.h>
#include <manifold/sfs.h>
#include <manifold/torusbundle.h>

namespace regina {

bool Manifold::operator < (const Manifold& compare) const {
    // Lens spaces go first.
    const LensSpace* lens1 = dynamic_cast<const LensSpace*>(this);
    const LensSpace* lens2 = dynamic_cast<const LensSpace*>(&compare);

    if (lens1 && ! lens2)
        return true;
    if (lens2 && ! lens1)
        return false;
    if (lens1 && lens2) {
        return (lens1->p() < lens2->p() ||
            (lens1->p() == lens2->p() && lens1->q() < lens2->q()));
    }

    // Next go through Seifert fibred spaces.
    const SFSpace* sfs1 = dynamic_cast<const SFSpace*>(this);
    const SFSpace* sfs2 = dynamic_cast<const SFSpace*>(&compare);

    if (sfs1 && ! sfs2)
        return true;
    if (sfs2 && ! sfs1)
        return false;
    if (sfs1 && sfs2)
        return (*sfs1 < *sfs2);

    // Now for torus bundles.
    const TorusBundle* bundle1 = dynamic_cast<const TorusBundle*>(this);
    const TorusBundle* bundle2 = dynamic_cast<const TorusBundle*>(&compare);

    if (bundle1 && ! bundle2)
        return true;
    if (bundle2 && ! bundle1)
        return false;
    if (bundle1 && bundle2) {
        // TODO: Just sort by name here, since bundle parameters will
        // probably need to be made canonical anyway.
        return name() < compare.name();
    }

    // Finally graph manifolds (SFS pairs, triples and loops).
    const GraphPair* pair1 = dynamic_cast<const GraphPair*>(this);
    const GraphPair* pair2 = dynamic_cast<const GraphPair*>(&compare);
    if (pair1 && ! pair2)
        return true;
    if (pair2 && ! pair1)
        return false;
    if (pair1 && pair2)
        return (*pair1 < *pair2);

    const GraphTriple* triple1 = dynamic_cast<const GraphTriple*>(this);
    const GraphTriple* triple2 = dynamic_cast<const GraphTriple*>(&compare);
    if (triple1 && ! triple2)
        return true;
    if (triple2 && ! triple1)
        return false;
    if (triple1 && triple2)
        return (*triple1 < *triple2);

    const GraphLoop* loop1 = dynamic_cast<const GraphLoop*>(this);
    const GraphLoop* loop2 = dynamic_cast<const GraphLoop*>(&compare);
    if (loop1 && ! loop2)
        return true;
    if (loop2 && ! loop1)
        return false;
    if (loop1 && loop2)
        return (*loop1 < *loop2);

    // No idea.  Use the dictionary.
    return name() < compare.name();
}

} // namespace regina

