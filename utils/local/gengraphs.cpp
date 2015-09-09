
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Form a census of triangulations that satisfy given properties         *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include "census/nfacepairing.h"
#include <cstdlib>
#include <iostream>

using regina::NBoolSet;
using regina::NFacePairing;

void dump(const NFacePairing* p, const NFacePairing::IsoList*, void*) {
    if (p)
        std::cout << p->toTextRep() << std::endl;
}

int main(int argc, char* argv[]) {
    int nTet = std::atoi(argv[1]);
    if (argc == 2)
        NFacePairing::findAllPairings(
            nTet, NBoolSet::sFalse, 0, &dump, 0, false);
    else if (argc == 3) {
        NFacePairing* f = NFacePairing::fromTextRep(argv[2]);
        while (f->nextPairing(NBoolSet::sFalse, 0))
            std::cout << f->toTextRep() << std::endl;
        delete f;
    } else
        return 1;
    return 0;
}

