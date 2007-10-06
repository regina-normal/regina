
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Identify triangulations that are plugged thin/thick I-bundles         *
 *                                                                        *
 *  Copyright (c) 2005-2007, Ben Burton                                   *
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
 * Each triangulation is examined to see it is recognised as a plugged
 * thin I-bundle or a plugged thick I-bundle, as described in
 * "Structures of small closed non-orientable 3-manifold triangulations",
 * Benjamin A. Burton, J. Knot Theory Ramifications 16 (2007), 545--574.
 *
 * Every triangulation is written on its own output line.  If a
 * triangulation is found to be a plugged thin/thick I-bundle, the
 * corresponding parameters are output also.
 *
 * If the option -c is passed, the labels of all container packets are
 * also output as they appear in the packet tree.  This may be useful for
 * dividing the output into sections.
 *
 * All output is written to standard output.
 */

#include <file/nxmlfile.h>
#include <packet/ncontainer.h>
#include <subcomplex/nblockedsfs.h>
#include <subcomplex/nstandardtri.h>
#include <triangulation/ntriangulation.h>

#include <cstdlib>
#include <iostream>

using namespace regina;

unsigned totTris = 0;
unsigned plugged = 0;

bool outputContainers = false;
NPacket* tree;

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " [ -c ] <file.rga>\n";
    std::cerr << std::endl;
    std::cerr << "    -c : Output container packet labels also\n";
    std::cerr << std::endl;
    std::cerr << "Resulting data is written to standard output.\n";
    std::cerr << "Statistics and diagnostic messages are written to standard error.\n";
    exit(1);
}

void process(NTriangulation* t) {
    std::cout << t->getPacketLabel();
    totTris++;

    NStandardTriangulation* s =
        NStandardTriangulation::isStandardTriangulation(t);
    if (s) {
        NBlockedSFS* sfs = dynamic_cast<NBlockedSFS*>(s);
        if (sfs) {
            std::string pluggedName;
            if (sfs->isPluggedIBundle(pluggedName)) {
                plugged++;
                std::cout << "  ==  " << pluggedName;
            }
        }

        delete s;
    }

    std::cout << std::endl;
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
        } else if (optChar == 'c')
            outputContainers = true;
        else
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
        if (p->getPacketType() == NTriangulation::packetType)
            process(static_cast<NTriangulation*>(p));
        else if (outputContainers &&
                p->getPacketType() == NContainer::packetType)
            std::cout << "----- " << p->getPacketLabel() << " -----"
                << std::endl;

    // Write statistics and clean up.
    delete tree;

    std::cerr << std::endl;
    std::cerr << "Final statistics:" << std::endl;
    std::cerr << "    Triangulations read:     " << totTris << std::endl;
    std::cerr << "    Plugged I-bundles found: " << plugged << std::endl;

    return 0;
}

