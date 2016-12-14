
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Attempt to recognise the combinatorial structures of triangulations   *
 *                                                                        *
 *  Copyright (c) 2005-2016, Ben Burton                                   *
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

#include <algebra/abeliangroup.h>
#include <manifold/manifold.h>
#include <subcomplex/standardtri.h>
#include <triangulation/dim3.h>

#include <cstdlib>
#include <iostream>

using namespace regina;

unsigned totTris = 0;
unsigned trisOk = 0;
unsigned mfdsOk = 0;
unsigned homBad = 0;
unsigned homChecked = 0;

bool outputContainers = false;
Packet* tree;

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

void process(Triangulation<3>* t) {
    std::cout << t->label() << "  -->  ";
    totTris++;

    StandardTriangulation* s =
        StandardTriangulation::isStandardTriangulation(t);
    if (s) {
        std::cout << s->name();
        trisOk++;

        Manifold* m = s->manifold();
        if (m) {
            std::string manifold = m->name();
            std::cout << "  ==  " << manifold;
            mfdsOk++;

            std::string structure = m->structure();
            if ((! structure.empty()) && (structure != manifold))
                std::cout << "  ==  " << structure;

            AbelianGroup* h1 = m->homology();
            if (h1) {
                homChecked++;
                if (! (*h1 == t->homology())) {
                    std::cout << "  ...  HOMOLOGY ERROR: "
                        << h1->str() << " != " << t->homology().str();
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
    if (! (tree = open(argv[i]))) {
        std::cerr << "ERROR: Could not read data from " << argv[i] << '.'
            << std::endl;
        return 1;
    }

    // Process the packets.
    for (Packet* p = tree; p; p = p->nextTreePacket())
        if (p->type() == PACKET_TRIANGULATION3)
            process(static_cast<Triangulation<3>*>(p));
        else if (outputContainers && p->type() == PACKET_CONTAINER)
            std::cout << "----- " << p->label() << " -----"
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

