
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Form a census of splitting surface signatures of given order          *
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

#include <cstdlib>
#include "split/sigcensus.h"
#include "utilities/stringutils.h"

#define MAXORDER 20

void usage(const char* progName) {
    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " <order>\n";
    exit(1);
}

void printSignature(const regina::Signature& sig,
        const regina::SigIsoList&, void*) {
    sig.writeCycles(std::cout, "", "", ".");
    std::cout << '\n';
}

int main(int argc, char* argv[]) {
    if (argc != 2)
        usage(argv[0]);

    int order;
    bool valid = regina::valueOf(argv[1], order);
    if ((! valid) || order < 1 || order > MAXORDER) {
        std::cerr << "Only numerical orders between 1 and " << MAXORDER
            << " inclusive are accepted.\n";
        usage(argv[0]);
    }

    unsigned long result = formSigCensus(order, printSignature, 0);
    std::cout << "Total signatures: " << result << '\n';

    return 0;
}

