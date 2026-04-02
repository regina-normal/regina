
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

/*! \file link/linksig-impl.h
*   \brief Contains implementation details for working with knot/link
*   signatures.
 *
 *  This file is _not_ included from link.h, but the routines it contains are
 *  explicitly instantiated in Regina's calculation engine.
 *  Therefore end users should never need to include this header.
 */

#ifndef __REGINA_LINKSIG_IMPL_H
#ifndef __DOXYGEN
#define __REGINA_LINKSIG_IMPL_H
#endif

#include "link/link.h"

namespace regina {

// Knot/link signatures choose a labelling that minimises
// LinkSigData::sequence() for each connected diagram component.
//
// If a link diagram has multiple connected components, each diagram component
// is treated as a standalone link diagram (i.e., we reuse crossing numbers),
// diagram components are sorted according to the ordering on LinkSigData, and
// then we simply concatenate the signatures for each diagram component.
//
// Regarding reflection and reversal:
//
// - Signature creation without allowing reversal of link components is
//   polynomial time in the number of crossings.  If we do allow reversal then
//   we must multiply this by an exponential in the number of link components.
//
// - If we allow reflection of the entire diagram, we do everything once without
//   reflection and once with reflection, and take the smaller LinkSigData.

template <LinkSigEncoding Encoding>
typename Encoding::Signature Link::sig(bool allowReflection, bool allowReversal,
        bool allowRotation) const {
    if (components_.size() >= 64)
        throw NotImplemented("Signatures are only implemented for "
            "fewer than 64 link components");

    // Get the zero-crossing cases out of the way first.
    if (components_.empty()) {
        return Encoding::encodeEmpty();
    } else if (crossings_.empty()) {
        typename Encoding::Signature sig = Encoding::encodeUnknot();
        for (size_t i = 1; i < components_.size(); ++i)
            sig += Encoding::encodeUnknot();
        return sig;
    }

    // We have at least one crossing, and therefore at least one component.
    if (isConnected()) {
        // This is the easy case.
        return Encoding::encode(LinkSigData(*this,
            allowReflection ? BoolSet(true, true) /* both options */ :
                BoolSet(false) /* false only */,
            allowReversal,
            allowRotation ? BoolSet(true, true) /* both options */ :
                BoolSet(false) /* false only */));
    } else {
        // We need to build a signature for each connected component.
        // For now we will not worry too much about overhead since people
        // should not be doing intense work with disconnected link diagrams
        // in practice (?).
        //
        // Do this first without reflection or rotation.
        auto components = diagramComponents();
        size_t nTrivial = 0;

        std::vector<LinkSigData> bits;
        for (auto c : components) {
            if (c.size() == 0) {
                // This is a zero-crossing unknot component.
                ++nTrivial;
            } else {
                bits.emplace_back(c,
                    BoolSet(false), allowReversal, BoolSet(false));
            }
        }
        std::sort(bits.begin(), bits.end());

        // ... and again with reflection and/or rotation.
        if (allowReflection) {
            std::vector<LinkSigData> alt;
            for (auto c : components) {
                if (c.size() > 0)
                    alt.emplace_back(c,
                        BoolSet(true), allowReversal, BoolSet(false));
            }
            std::sort(alt.begin(), alt.end());
            if (alt < bits)
                alt.swap(bits);
        }
        if (allowRotation) {
            std::vector<LinkSigData> alt;
            for (auto c : components) {
                if (c.size() > 0)
                    alt.emplace_back(c,
                        BoolSet(false), allowReversal, BoolSet(true));
            }
            std::sort(alt.begin(), alt.end());
            if (alt < bits)
                alt.swap(bits);
        }
        if (allowReflection && allowRotation) {
            std::vector<LinkSigData> alt;
            for (auto c : components) {
                if (c.size() > 0)
                    alt.emplace_back(c,
                        BoolSet(true), allowReversal, BoolSet(true));
            }
            std::sort(alt.begin(), alt.end());
            if (alt < bits)
                alt.swap(bits);
        }

        typename Encoding::Signature sig;
        for (const auto& data : bits)
            sig += Encoding::encode(data);
        for (size_t i = 0; i < nTrivial; ++i)
            sig += Encoding::encodeUnknot();
        return sig;
    }
}

} // namespace regina

#endif

