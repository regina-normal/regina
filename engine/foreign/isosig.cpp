
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include <fstream>
#include <sstream>

#include "foreign/isosig.h"
#include "packet/ncontainer.h"
#include "packet/ntext.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NContainer* readIsoSigList(const char *filename, unsigned dimension,
        unsigned colSigs, int colLabels, unsigned long ignoreLines) {
    // Open the file.
    std::ifstream in(filename);
    if (! in)
        return 0;

    // Ignore the specified number of lines.
    std::string line;

    unsigned long i;
    for (i = 0; i < ignoreLines; i++) {
        std::getline(in, line);
        if (in.eof())
            return new NContainer();
    }

    // Read in and process the remaining lines.
    NContainer* ans = new NContainer();
    std::string errStrings;

    int col;
    std::string token;

    std::string isoSig;
    std::string label;
    NTriangulation* tri3;

    while(! in.eof()) {
        // Read in the next line.
        line.clear();
        std::getline(in, line);

        if (line.empty())
            continue;

        // Find the appropriate tokens.
        std::istringstream tokens(line);

        isoSig.clear();
        label.clear();
        for (col = 0; col <= static_cast<int>(colSigs) ||
                col <= colLabels; col++) {
            tokens >> token;
            if (token.empty())
                break;
            if (col == static_cast<int>(colSigs))
                isoSig = token;
            if (col == colLabels)
                label = token;
        }

        if (! isoSig.empty()) {
            // Process this isomorphism signature.
            if (dimension == 3) {
                if ((tri3 = NTriangulation::fromIsoSig(isoSig))) {
                    tri3->setPacketLabel(label.empty() ? isoSig : label);
                    ans->insertChildLast(tri3);
                } else
                    errStrings = errStrings + '\n' + isoSig;
            } else
                errStrings = errStrings + '\n' + isoSig;
        }
    }

    // Finish off.
    if (! errStrings.empty()) {
        std::ostringstream msg;
        msg << "The following isomorphism string(s) could not be interpreted "
            "as " << dimension << "-manifold triangulations:\n" << errStrings;
        NText* errPkt = new NText(msg.str());
        errPkt->setPacketLabel("Errors");
        ans->insertChildLast(errPkt);
    }

    ans->makeUniqueLabels(0);
    return ans;
}

} // namespace regina
