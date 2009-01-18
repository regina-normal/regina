
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Attempt to recognise the combinatorial structures of triangulations   *
 *                                                                        *
 *  Copyright (c) 2005-2009, Ben Burton                                   *
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
 * Each triangulation is examined to see it is recognised, i.e., if it
 * has a combinatorial structure that Regina is familiar with.  If so,
 * the name of the combinatorial triangulation is output (plus the name of
 * the underlying 3-manifold if possible).  If not, the word UNKNOWN is
 * output instead.
 *
 * Whenever the underlying 3-manifold is recognised and the expected
 * homology group can be computed, this is compared with the observed
 * homology group to ensure that they are isomorphic.
 *
 * If the option -c is passed, the labels of all container packets are
 * also output as they appear in the packet tree.  This may be useful for
 * dividing the output into sections.
 *
 * All output is written to standard output.
 */

#include <algebra/nabeliangroup.h>
#include <file/nxmlfile.h>
#include <manifold/nmanifold.h>
#include <packet/ncontainer.h>
#include <subcomplex/nstandardtri.h>
#include <triangulation/ntriangulation.h>

#include <cstdlib>
#include <iostream>

using namespace regina;

unsigned totTris = 0;
unsigned trisOk = 0;
unsigned mfdsOk = 0;
unsigned homBad = 0;
unsigned homChecked = 0;

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
    std::cout << t->getPacketLabel() << "  -->  ";
    totTris++;

    NStandardTriangulation* s =
        NStandardTriangulation::isStandardTriangulation(t);
    if (s) {
        std::cout << s->getName();
        trisOk++;

        NManifold* m = s->getManifold();
        if (m) {
            std::string manifold = m->getName();
            std::cout << "  ==  " << manifold;
            mfdsOk++;

            std::string structure = m->getStructure();
            if ((! structure.empty()) && (structure != manifold))
                std::cout << "  ==  " << structure;

            NAbelianGroup* h1 = m->getHomologyH1();
            if (h1) {
                homChecked++;
                if (! (*h1 == t->getHomologyH1())) {
                    std::cout << "  ...  HOMOLOGY ERROR: "
                        << h1->toString() << " != "
                        << t->getHomologyH1().toString();
                    homBad++;
                }

                delete h1;
            }

            delete m;
        }

        delete s;
    } else
        std::cout << "UNKNOWN";

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
    std::cerr << "    Triangulations read:       " << totTris << std::endl;
    std::cerr << "    Triangulations recognised: " << trisOk << std::endl;
    std::cerr << "    3-manifolds recognised:    " << mfdsOk << std::endl;
    std::cerr << "    Homology groups checked:   " << homChecked << std::endl;
    std::cerr << "    Homology errors:           " << homBad << std::endl;

    return 0;
}

