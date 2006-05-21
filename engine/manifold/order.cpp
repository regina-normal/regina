
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include <manifold/ngraphloop.h>
#include <manifold/ngraphpair.h>
#include <manifold/ngraphtriple.h>
#include <manifold/nlensspace.h>
#include <manifold/nsfs.h>
#include <manifold/ntorusbundle.h>

namespace regina {

bool NManifold::operator < (const NManifold& compare) const {
    // Lens spaces go first.
    const NLensSpace* lens1 = dynamic_cast<const NLensSpace*>(this);
    const NLensSpace* lens2 = dynamic_cast<const NLensSpace*>(&compare);

    if (lens1 && ! lens2)
        return true;
    if (lens2 && ! lens1)
        return false;
    if (lens1 && lens2) {
        return (lens1->getP() < lens2->getP() ||
            (lens1->getP() == lens2->getP() && lens1->getQ() < lens2->getQ()));
    }

    // Next go through Seifert fibred spaces.
    const NSFSpace* sfs1 = dynamic_cast<const NSFSpace*>(this);
    const NSFSpace* sfs2 = dynamic_cast<const NSFSpace*>(&compare);

    if (sfs1 && ! sfs2)
        return true;
    if (sfs2 && ! sfs1)
        return false;
    if (sfs1 && sfs2)
        return (*sfs1 < *sfs2);

    // Now for torus bundles.
    const NTorusBundle* bundle1 = dynamic_cast<const NTorusBundle*>(this);
    const NTorusBundle* bundle2 = dynamic_cast<const NTorusBundle*>(&compare);

    if (bundle1 && ! bundle2)
        return true;
    if (bundle2 && ! bundle1)
        return false;
    if (bundle1 && bundle2) {
        // TODO: Just sort by name here, since bundle parameters will
        // probably need to be made canonical anyway.
        return getName() < compare.getName();
    }

    // Finally graph manifolds (SFS pairs, triples and loops).
    const NGraphPair* pair1 = dynamic_cast<const NGraphPair*>(this);
    const NGraphPair* pair2 = dynamic_cast<const NGraphPair*>(&compare);
    if (pair1 && ! pair2)
        return true;
    if (pair2 && ! pair1)
        return false;
    if (pair1 && pair2)
        return (*pair1 < *pair2);

    const NGraphTriple* triple1 = dynamic_cast<const NGraphTriple*>(this);
    const NGraphTriple* triple2 = dynamic_cast<const NGraphTriple*>(&compare);
    if (triple1 && ! triple2)
        return true;
    if (triple2 && ! triple1)
        return false;
    if (triple1 && triple2)
        return (*triple1 < *triple2);

    const NGraphLoop* loop1 = dynamic_cast<const NGraphLoop*>(this);
    const NGraphLoop* loop2 = dynamic_cast<const NGraphLoop*>(&compare);
    if (loop1 && ! loop2)
        return true;
    if (loop2 && ! loop1)
        return false;
    if (loop1 && loop2)
        return (*loop1 < *loop2);

    // No idea.  Use the dictionary.
    return getName() < compare.getName();
}

} // namespace regina

