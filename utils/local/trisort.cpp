
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Rename and sort triangulations within their 3-manifold containers     *
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
 * It is assumed that triangulations in the data file are grouped into
 * containers, with each container representing different triangulations
 * of the same 3-manifold.
 *
 * For each container, the triangulations are identified where possible.
 * Triangulations will be renamed according to the manifold name
 * (assumed to be the container's packet label), and will be sorted within
 * each container according to the triangulation name.  Triangulations
 * that cannot be identified will always appear last within a container.
 *
 * The original file will be overwritten with any changes that were made.
 */

#include <packet/container.h>
#include <subcomplex/blockedsfs.h>
#include <subcomplex/standardtri.h>
#include <triangulation/dim3.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

using namespace regina;

bool saveChanges = true;
Packet* tree;

struct TriSpec {
    Packet* packet;
    bool isTri;
    bool hasName;
    bool hasSpecialName;
    std::string name;
    std::string specialName;

    TriSpec() : packet(0), isTri(false), hasName(false), hasSpecialName(false) {
    }

    bool operator < (const TriSpec& other) const {
        if (other.isTri && ! isTri)
            return true;
        if (! (isTri && other.isTri))
            return false;

        // Both are triangulations.

        if (hasName && ! other.hasName)
            return true;
        if (! (hasName && other.hasName))
            return false;

        // Both have names.

        if (name < other.name)
            return true;
        if (other.name < name)
            return false;

        // Both have identical names.
        // Use special names as a tiebreaker.

        if (hasSpecialName && ! other.hasSpecialName)
            return true;
        if (other.hasSpecialName && ! hasSpecialName)
            return false;

        return (specialName < other.specialName);
    }
};

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " <file.rga>\n";
    std::cerr << std::endl;
    std::cerr << "Diagnostic messages are written to standard error.\n";
    exit(1);
}

bool hasTriangulation(Container* c) {
    for (Packet* child = c->firstChild(); child; child = child->nextSibling())
        if (child->type() == PACKET_TRIANGULATION3)
            return true;

    return false;
}

void process(Container* c) {
    if (! hasTriangulation(c))
        return;

    StandardTriangulation* std;
    std::vector<TriSpec> children;
    TriSpec spec;
    for (Packet* child = c->firstChild(); child;
            child = child->nextSibling()) {
        spec.packet = child;
        spec.isTri = (child->type() == PACKET_TRIANGULATION3);

        if (spec.isTri) {
            std = StandardTriangulation::isStandardTriangulation(
                static_cast<Triangulation<3>*>(child));
            spec.hasName = (std != 0);

            if (spec.hasName) {
                spec.name = std->name();
                spec.hasSpecialName = false;

                BlockedSFS* sfs = dynamic_cast<BlockedSFS*>(std);
                if (sfs && sfs->isPluggedIBundle(spec.specialName))
                    spec.hasSpecialName = true;

                delete std;
            }
        }

        children.push_back(spec);
    }

    std::stable_sort(children.begin(), children.end());

    unsigned which = 1;
    for (std::vector<TriSpec>::iterator it = children.begin();
            it != children.end(); it++) {
        it->packet->moveToLast();
        if (it->isTri) {
            std::ostringstream s;
            s << c->label() << " : #" << (which++);
            it->packet->setLabel(s.str());
        }
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
        } else if (optChar == 'o')
            saveChanges = true;
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

    // Process each container.
    for (Packet* p = tree; p; p = p->nextTreePacket())
        if (p->type() == PACKET_CONTAINER)
            process(static_cast<Container*>(p));

    // Save the data file if required.
    if (saveChanges) {
        std::cerr << "WARNING: The data file " << argv[i]
            << " will be overwritten.\n";
        std::cerr << "Proceed? (y/n) ";

        std::string line;
        getline(std::cin, line);
        while (line != "y" && line != "n") {
            std::cerr << "Please answer y or n.  Proceed? (y/n) ";
            getline(std::cin, line);
        }

        if (line != "y")
            std::cerr << "Not saving data file.\n";
        else if (tree->save(argv[i]))
            std::cerr << "Data saved to " << argv[i] << ".\n";
        else
            std::cerr << "ERROR: The data file could not be saved.\n";
    }

    // Clean up.
    delete tree;
    return 0;
}

