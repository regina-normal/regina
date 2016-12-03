
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Attempt to calculate the hyperbolic volumes of triangulations         *
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
 * Each triangulation is run through the SnapPea kernel to see whether a
 * volume can be calculated.  The volume and solution type are output.
 * Closed triangulations are supported.
 *
 * If the option -c is passed, the labels of all container packets are
 * also output as they appear in the packet tree.  This may be useful for
 * dividing the output into sections.
 *
 * All output is written to standard output.
 */

#include <snappea/snappeatriangulation.h>
#include <triangulation/dim3.h>

#include <cstdlib>
#include <iostream>

using namespace regina;

unsigned totTris = 0;
unsigned totGeometric = 0;
unsigned totUnusable = 0;

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

    SnapPeaTriangulation s(*t, true);
    if (s.isNull()) {
        std::cout << "UNUSABLE";
        totUnusable++;
    } else {
        SnapPeaTriangulation::SolutionType type = s.solutionType();
        if (type == SnapPeaTriangulation::geometric_solution)
            totGeometric++;

        int precision;
        double vol = s.volume(precision);

        std::cout << vol << " (" << precision << " place(s)) ... ";

        switch(type) {
            case SnapPeaTriangulation::not_attempted:
                std::cout << "not attempted"; break;
            case SnapPeaTriangulation::geometric_solution:
                std::cout << "geometric"; break;
            case SnapPeaTriangulation::nongeometric_solution:
                std::cout << "nongeometric"; break;
            case SnapPeaTriangulation::flat_solution:
                std::cout << "flat"; break;
            case SnapPeaTriangulation::degenerate_solution:
                std::cout << "degenerate"; break;
            case SnapPeaTriangulation::other_solution:
                std::cout << "other solution"; break;
            case SnapPeaTriangulation::no_solution:
                std::cout << "no solution"; break;
        }
    }

    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    SnapPeaTriangulation::disableKernelMessages();

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
    std::cerr << "    Triangulations read: " << totTris << std::endl;
    std::cerr << "    Geometric solutions: " << totGeometric << std::endl;
    std::cerr << "    Unusable:            " << totUnusable << std::endl;

    return 0;
}

