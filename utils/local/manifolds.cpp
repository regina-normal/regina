
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Attempt to recognise the combinatorial structures of triangulations   *
 *                                                                        *
 *  Copyright (c) 2005-2006, Ben Burton                                   *
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

/**
 * The given topology data file is read.
 *
 * It is assumed that triangulations in the data file are grouped into
 * containers, with each container representing different triangulations
 * of the same 3-manifold.
 *
 * For each container, the triangulations are examined one by one until
 * a 3-manifold can be recognised.  If so, the name of the 3-manifold is
 * output alongside the name of the container, and no further
 * triangulations for that container are examined.  If none of its
 * triangulations are recognised, the word UNKNOWN is output alongside
 * the container instead.
 *
 * All output is written to standard output.
 */

#include <file/nxmlfile.h>
#include <manifold/nmanifold.h>
#include <packet/ncontainer.h>
#include <subcomplex/nstandardtri.h>
#include <triangulation/ntriangulation.h>

#include <cstdlib>
#include <iostream>

using namespace regina;

unsigned totMfds = 0;
unsigned totMfdsOk = 0;

NPacket* tree;

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " <file.rga>\n";
    std::cerr << std::endl;
    std::cerr << "Resulting data is written to standard output.\n";
    std::cerr << "Statistics and diagnostic messages are written to standard error.\n";
    exit(1);
}

void process(NContainer* c) {
    bool foundTri = false;

    NStandardTriangulation* std;
    NManifold* mfd;
    for (NPacket* child = c->getFirstTreeChild(); child;
            child = child->getNextTreeSibling()) {
        if (child->getPacketType() != NTriangulation::packetType)
            continue;

        foundTri = true;

        std = NStandardTriangulation::isStandardTriangulation(
            static_cast<NTriangulation*>(child));
        if (! std)
            continue;

        mfd = std->getManifold();
        if (! mfd) {
            delete std;
            continue;
        }

        std::cout << c->getPacketLabel() << "  ->>  " << mfd->getName() << '\n';
        totMfds++;
        totMfdsOk++;

        delete std;
        delete mfd;
        return;
    }

    if (foundTri) {
        std::cout << c->getPacketLabel() << "  ->>  UNKNOWN\n";
        totMfds++;
    }
}

int main(int argc, char* argv[]) {
    // Command-line parsing.
    char optChar;
    int i;
    for (i = 1; i < argc && *argv[i] == '-'; i++) {
        if (! argv[i][1])
            usage(argv[0], std::string("Invalid option: ") + argv[i]);
        if (argv[i][2])
            usage(argv[0], std::string("Invalid option: ") + argv[i]);

        // The argument has length precisely 2.
        optChar = argv[i][1];

        if (optChar == '-') {
            i++;
            break;
        } else
            usage(argv[0], std::string("Invalid option: ") + argv[i]);
    }

    // argv[i] is the first filename.
    if (i != argc - 1)
        usage(argv[0], "Precisely one data file must be given.");

    // Read the data file.
    if (! (tree = readXMLFile(argv[i]))) {
        std::cerr << "ERROR: Could not read data from " << argv[i] << '.'
            << std::endl;
        return 1;
    }

    // Process the packets.
    for (NPacket* p = tree; p; p = p->nextTreePacket())
        if (p->getPacketType() == NContainer::packetType && p != tree)
            process(static_cast<NContainer*>(p));

    // Write statistics and clean up.
    delete tree;

    std::cerr << std::endl;
    std::cerr << "Final statistics:" << std::endl;
    std::cerr << "    3-manifolds examined:   " << totMfds << std::endl;
    std::cerr << "    3-manifolds recognised: " << totMfdsOk << std::endl;

    return 0;
}

