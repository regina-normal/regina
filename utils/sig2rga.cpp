
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Combine a list of isomorphism signatures into a Regina data file      *
 *                                                                        *
 *  Copyright (c) 1999-2015, Ben Burton                                   *
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

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include "file/nxmlfile.h"
#include "packet/ncontainer.h"
#include "triangulation/ntriangulation.h"

void usage(const char* progName) {
    std::cerr << "Usage:" << std::endl;
    std::cerr << "    " << progName << " [data-file [output-file]]" << std::endl;
    std::cerr << "    If an option is \"-\" or omitted, standard input and "
        "output are used instead." << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    std::istream *input;
    const std::string minus("-");
    const std::string help("-h");
    if ((argc == 2) && (help == argv[1]))
        usage(argv[0]);
    if ((argc < 2) || (minus == argv[1]))
        input = &std::cin;
    else
        input = new std::ifstream(argv[1]);

    regina::NContainer root;
    for(std::string line; std::getline(*input, line);) {
        regina::NTriangulation *t = regina::NTriangulation::fromIsoSig(line);
        if (t == NULL) {
            std::cerr << "Failed to construct " << line << std::endl;
            continue;
        }
        t->setPacketLabel(line);
        root.insertChildLast(t);
    }

    if ((argc < 3) || (minus == argv[2]))
        root.writeXMLFile(std::cout);
    else
        root.save(argv[2]);

    return 0;
}

