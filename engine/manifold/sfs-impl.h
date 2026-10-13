
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file manifold/sfs-impl.h
 *  \brief Contains implementation details for SFSpace.
 *
 *  This file is automatically included from sfs.h; there is no need for
 *  end users to include it explicitly.
 */

#ifndef __REGINA_SFS_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_SFS_IMPL_H
#endif

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

template <ForwardIteratorFor<SFSPartialReflector> ParRefIterator>
void SFSpace::addOrbifoldBoundary(
        bool twisted, size_t nPunctures, size_t nReflectors,
        ParRefIterator beginParRefs, ParRefIterator endParRefs ) {
    // Enforce the condition which isn't checked by the internal routines.
    size_t twistedCount = (twisted ? (nPunctures + nReflectors) : 0);
    for (auto it = beginParRefs; it != endParRefs; ++it) {
        if (it->twisted) {
            ++twistedCount;
        }
    }
    if (twistedCount % 2 != 0) {
        throw InvalidArgument(
                "SFSpace requires that the total number of twisted "
                "boundaries in the base orbifold always remains even");
    }
    addOrbifoldBoundaryInternal(twisted, nPunctures, nReflectors);
    insertPartialReflectors(beginParRefs, endParRefs);
}

template <typename Iterator>
requires ForwardIteratorFor<Iterator, std::iter_value_t<Iterator>> &&
         StandardCppInteger<std::iter_value_t<Iterator>>
void SFSpace::addOrbifoldBoundary(
        bool twisted, size_t nPunctures, size_t nReflectors,
        Iterator beginReflectorArcCounts, Iterator endReflectorArcCounts ) {
    // Enforce the condition which isn't checked by the internal routines.
    if (twisted) {
        size_t twistedCount(nPunctures + nReflectors);
        for (auto it = beginReflectorArcCounts;
                it != endReflectorArcCounts; ++it) {
            if constexpr (std::is_signed_v<std::iter_value_t<Iterator>>) {
                if (*it < 0) {
                    throw InvalidArgument(
                            "SFSpace::addOrbifoldBoundary() requires each "
                            "reflector arc count to be non-negative");
                }
            }
            ++twistedCount;
        }
        if (twistedCount % 2 != 0) {
            throw InvalidArgument(
                    "SFSpace requires that the total number of twisted "
                    "boundaries in the base orbifold always remains even");
        }
    }
    addOrbifoldBoundaryInternal(twisted, nPunctures, nReflectors);
    for (auto it = beginReflectorArcCounts;
            it != endReflectorArcCounts; ++it) {
        insertPartialReflector( twisted, *it );
    }
}

template <StandardCppInteger T>
void SFSpace::insertPartialReflector(
        bool twisted, T reflectorArcs ) {
    // First handle the case where this is just a puncture with no reflector
    // arcs.
    if (reflectorArcs == 0) {
        if (twisted) {
            ++puncturesTwisted_;
        } else {
            ++punctures_;
        }
        return;
    }

    // OK, now we know that we have a proper partial reflector.
    SFSPartialReflector partialReflector(twisted, reflectorArcs);
    if (twisted) {
        ++nPartialReflectorsTwisted_;
        partialReflectorsTwisted_.insert(
                lower_bound(
                    partialReflectorsTwisted_.begin(),
                    partialReflectorsTwisted_.end(),
                    partialReflector ),
                partialReflector );
    } else {
        ++nPartialReflectors_;
        partialReflectors_.insert(
                lower_bound(
                    partialReflectors_.begin(),
                    partialReflectors_.end(),
                    partialReflector ),
                partialReflector );
    }
}

} // namespace regina

#endif

