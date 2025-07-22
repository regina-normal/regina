
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

/*! \file foreign/fromsiglist-impl.h
 *  \brief Contains implementation details for the template function
 *  fromSigList().
 *
 *  This file is automatically included from foreign/isosig.h, there is
 *  no need for end users to include it explicitly.
 */

#ifndef __REGINA_FROMSIGLIST_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_FROMSIGLIST_IMPL_H_DETAIL
#endif

#include <fstream>
#include <sstream>
#include <type_traits>
#include "packet/container.h"
#include "packet/text.h"
#include "utilities/exception.h"

namespace regina {

class Link;
template <int dim> class Triangulation;

template <class ObjectType>
std::shared_ptr<Container> readSigList(const char *filename, unsigned colSigs,
        int colLabels, unsigned long ignoreLines) {
    // Open the file.
    std::ifstream in(filename);
    if (! in)
        return nullptr;

    // Ignore the specified number of lines.
    std::string line;

    unsigned long i;
    for (i = 0; i < ignoreLines; i++) {
        std::getline(in, line);
        if (in.eof())
            return std::make_shared<Container>();
    }

    // Read in and process the remaining lines.
    auto ans = std::make_shared<Container>();
    std::string errStrings;

    int col;
    std::string token;

    std::string sig;
    std::string label;

    while(! in.eof()) {
        // Read in the next line.
        line.clear();
        std::getline(in, line);

        if (line.empty())
            continue;

        // Find the appropriate tokens.
        std::istringstream tokens(line);

        sig.clear();
        label.clear();
        for (col = 0; col <= static_cast<int>(colSigs) ||
                col <= colLabels; col++) {
            tokens >> token;
            if (token.empty())
                break;
            if (col == static_cast<int>(colSigs))
                sig = token;
            if (col == colLabels)
                label = token;
        }

        if (! sig.empty()) {
            // Process this isomorphism signature.
            try {
                ans->append(make_packet(ObjectType::fromSig(sig),
                    (label.empty() ? sig : label)));
            } catch (const InvalidArgument&) {
                errStrings += '\n';
                errStrings += sig;
            }
        }
    }

    // Finish off.
    if (! errStrings.empty()) {
        std::ostringstream msg;
        msg << "The following signature(s) could not be interpreted as ";
        if constexpr (std::is_same_v<ObjectType, Link>)
            msg << "knots:\n";
        else
            msg << ObjectType::dimension << "-manifold triangulations:\n";
        msg << errStrings;

        auto errPkt = std::make_shared<Text>(msg.str());
        errPkt->setLabel("Errors");
        ans->append(errPkt);
    }

    return ans;
}

} // namespace regina

#endif
