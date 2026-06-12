
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#ifndef __TESTEXHAUSTIVE_H
#define __TESTEXHAUSTIVE_H

#include "link/link.h"
#include "link/modellinkgraph.h"

/**
 * Run the given test over all link diagrams from a census that is generated
 * on the fly.
 *
 * \param action the test to run.  This should be a callable object that takes
 * two arguments: a link diagram, and a human-readable link diagram name.
 * \param small indicates whether a smaller census should be used.  This can
 * help with the overall running time when the test \a action is expensive.
 */
template <std::invocable<regina::Link&&, const char*> Action>
inline void runCensusAllVirtual(Action&& action, bool small_ = false) {
    for (int n = 1; n <= (small_ ? 3 : 4) /* size */; ++n) {
        regina::FacetPairing<3>::findAllPairings(n, false, -1,
                [&action](const regina::FacetPairing<3>& p) {
            regina::ModelLinkGraph::generateAllEmbeddings(p, false, {},
                    [&action](const regina::ModelLinkGraph& g) {
                g.generateAllLinks([&action](regina::Link&& link) {
                    auto brief = link.brief();
                    std::invoke(std::forward<Action>(action),
                        std::move(link), brief.c_str());
                });
            });
        });
    }
}

#endif
