
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

#include <fstream>
#include <sstream>

#include "foreign/dehydration.h"
#include "packet/container.h"
#include "packet/text.h"
#include "triangulation/dim3.h"

namespace regina {

std::shared_ptr<Container> readDehydrationList(const char *filename,
        unsigned colDehydrations, int colLabels, unsigned long ignoreLines) {
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
    std::shared_ptr<Container> ans = std::make_shared<Container>();
    std::string errStrings;

    int col;
    std::string token;

    std::string dehydration;
    std::string label;

    while(! in.eof()) {
        // Read in the next line.
        line.clear();
        std::getline(in, line);

        if (line.empty())
            continue;

        // Find the appropriate tokens.
        std::istringstream tokens(line);

        dehydration.clear();
        label.clear();
        for (col = 0; col <= static_cast<int>(colDehydrations) ||
                col <= colLabels; col++) {
            tokens >> token;
            if (token.empty())
                break;
            if (col == static_cast<int>(colDehydrations))
                dehydration = token;
            if (col == colLabels)
                label = token;
        }

        if (! dehydration.empty()) {
            // Process this dehydration string.
            try {
                ans->insertChildLast(make_packet(
                    Triangulation<3>::rehydrate(dehydration),
                    (label.empty() ? dehydration : label)));
            } catch (const InvalidArgument&) {
                errStrings += '\n';
                errStrings += dehydration;
            }
        }
    }

    // Finish off.
    if (! errStrings.empty()) {
        auto errPkt = std::make_shared<Text>(std::string(
            "The following dehydration string(s) could not be rehydrated:\n") +
            errStrings);
        errPkt->setLabel("Errors");
        ans->insertChildLast(errPkt);
    }

    return ans;
}

} // namespace regina
