
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Form a census of splitting surface signatures of given order          *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "split/nsigcensus.h"
#include <cstdlib>

#define MAXORDER 20

using regina::NSignature;
using regina::NSigIsoList;

void usage(const char* progName) {
    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " <order>\n";
    exit(1);
}

void printSignature(const NSignature& sig, const NSigIsoList&, void*) {
    sig.writeCycles(std::cout, "", "", ".");
    std::cout << '\n';
}

int main(int argc, char* argv[]) {
    if (argc != 2)
        usage(argv[0]);

    int order = atoi(argv[1]);
    if (order < 1 || order > MAXORDER) {
        std::cerr << "Only numerical orders between 1 and " << MAXORDER
            << " inclusive are accepted.\n";
        usage(argv[0]);
    }

    unsigned long result = formSigCensus(order, printSignature, 0);
    std::cout << "Total signatures: " << result << '\n';

    return 0;
}

