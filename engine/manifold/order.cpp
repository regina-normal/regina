
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

#if !defined(STRING_SPACESHIP_FOUND)
#include <cstring>
#endif

#include <manifold/graphloop.h>
#include <manifold/graphpair.h>
#include <manifold/graphtriple.h>
#include <manifold/lensspace.h>
#include <manifold/sfs.h>
#include <manifold/torusbundle.h>

namespace regina {

std::weak_ordering Manifold::operator <=> (const Manifold& rhs) const {
    // Lens spaces go first.
    const auto* lens1 = dynamic_cast<const LensSpace*>(this);
    const auto* lens2 = dynamic_cast<const LensSpace*>(&rhs);

    if (lens1 && ! lens2)
        return std::weak_ordering::less;
    if (lens2 && ! lens1)
        return std::weak_ordering::greater;
    if (lens1 && lens2)
        return (*lens1 <=> *lens2);

    // Next go through Seifert fibred spaces.
    const auto* sfs1 = dynamic_cast<const SFSpace*>(this);
    const auto* sfs2 = dynamic_cast<const SFSpace*>(&rhs);

    if (sfs1 && ! sfs2)
        return std::weak_ordering::less;
    if (sfs2 && ! sfs1)
        return std::weak_ordering::greater;
    if (sfs1 && sfs2)
        return (*sfs1 <=> *sfs2);

    // Now for torus bundles.
    const auto* bundle1 = dynamic_cast<const TorusBundle*>(this);
    const auto* bundle2 = dynamic_cast<const TorusBundle*>(&rhs);

    if (bundle1 && ! bundle2)
        return std::weak_ordering::less;
    if (bundle2 && ! bundle1)
        return std::weak_ordering::greater;
    if (bundle1 && bundle2) {
        // TODO: Just sort by name here, since bundle parameters will
        // probably need to be made canonical anyway.
#if defined(STRING_SPACESHIP_FOUND)
        return name() <=> rhs.name();
#else
        const auto name1 = name();
        const auto name2 = rhs.name();
        return strcmp(name1.c_str(), name2.c_str());
#endif
    }

    // Finally graph manifolds (SFS pairs, triples and loops).
    const auto* pair1 = dynamic_cast<const GraphPair*>(this);
    const auto* pair2 = dynamic_cast<const GraphPair*>(&rhs);
    if (pair1 && ! pair2)
        return std::weak_ordering::less;
    if (pair2 && ! pair1)
        return std::weak_ordering::greater;
    if (pair1 && pair2)
        return (*pair1 <=> *pair2);

    const auto* triple1 = dynamic_cast<const GraphTriple*>(this);
    const auto* triple2 = dynamic_cast<const GraphTriple*>(&rhs);
    if (triple1 && ! triple2)
        return std::weak_ordering::less;
    if (triple2 && ! triple1)
        return std::weak_ordering::greater;
    if (triple1 && triple2)
        return (*triple1 <=> *triple2);

    const auto* loop1 = dynamic_cast<const GraphLoop*>(this);
    const auto* loop2 = dynamic_cast<const GraphLoop*>(&rhs);
    if (loop1 && ! loop2)
        return std::weak_ordering::less;
    if (loop2 && ! loop1)
        return std::weak_ordering::greater;
    if (loop1 && loop2)
        return (*loop1 <=> *loop2);

    // No idea.  Use the dictionary.
#if defined(STRING_SPACESHIP_FOUND)
    return name() <=> rhs.name();
#else
    const auto name1 = name();
    const auto name2 = rhs.name();
    return strcmp(name1.c_str(), name2.c_str());
#endif
}

} // namespace regina

