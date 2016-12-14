
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Attempt to recognise the underlying 3-manifolds in a census           *
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
 * For each container, the triangulations are examined one by one until
 * a 3-manifold can be recognised.  If so, the name of the 3-manifold is
 * output alongside the name of the container, and no further
 * triangulations for that container are examined.  If none of its
 * triangulations are recognised, the word UNKNOWN is output alongside
 * the container instead.
 *
 * If the option -d is passed, more detailed names will be given to
 * 3-manifolds (e.g., simpler quotient spaces will be named according to
 * their full Seifert structures).
 *
 * If the option -o is passed, the original file will be overwritten
 * with any changes that were made.  Otherwise output will be written to
 * the console only.
 *
 * If the option -r is passed, container packets will be renamed
 * according to the newly discovered 3-manifold names.
 *
 * If the option -s is passed, container packets will be sorted (within
 * their respective parent containers) according to the underlying
 * 3-manifold.  The method of ordering 3-manifolds is subject to change;
 * currently it bears some resemblance to the ordering used by Martelli
 * and Petronio in their tables of closed orientable 3-manifolds.
 *
 * If the option -w is passed, a raw list of manifolds and homology
 * groups (where they can be calculated) will be output.  Each manifold
 * that cannot be recognised will be written on its own line as "UNKNOWN".
 * This option may not be used with any of the other options listed above.
 */

#include <algebra/abeliangroup.h>
#include <manifold/manifold.h>
#include <packet/container.h>
#include <subcomplex/standardtri.h>
#include <triangulation/dim3.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace regina;

unsigned totMfds = 0;
unsigned totMfdsOk = 0;
unsigned totMfdsRenamed = 0;

bool detailedNames = false;
bool renameMfds = false;
bool sortMfds = false;
bool saveChanges = false;
bool rawList = false;
Packet* tree;

struct ManifoldSpec {
    Packet* container;
    bool hasTriangulation;
    Manifold* manifold; // 0 if unknown

    ManifoldSpec() : container(0), hasTriangulation(false), manifold(0) {
    }
    ManifoldSpec(Packet* c, bool t, Manifold* m) :
            container(c), hasTriangulation(t), manifold(m) {
    }

    bool operator < (const ManifoldSpec& other) const;
};

std::vector<ManifoldSpec> children;

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName <<
        " [ -d ] [ -o ] [ -r ] [ -s ] [ -w ] <file.rga>\n";
    std::cerr << std::endl;
    std::cerr << "    -d : Use more detailed 3-manifold names\n";
    std::cerr << "    -o : Save changes to the original file\n";
    std::cerr << "    -r : Rename container packets according to 3-manifold\n";
    std::cerr << "    -s : Sort container packets by 3-manifold\n";
    std::cerr << "    -w : Dump a raw list of manifolds and homology groups\n";
    std::cerr << std::endl;
    std::cerr << "Resulting data is written to standard output.\n";
    std::cerr << "Statistics and diagnostic messages are written to standard error.\n";
    exit(1);
}

bool ManifoldSpec::operator < (const ManifoldSpec& other) const {
    // Here comes the part where we sort manifolds.

    // Before we continue, anything that doesn't come with a
    // triangulation goes first, and anything with a triangulation but
    // no manifold goes last.

    if (! other.hasTriangulation)
        return false;
    if (! hasTriangulation)
        return true;

    if (! manifold)
        return false;
    if (! other.manifold)
        return true;

    // Both structures have a triangulation and a manifold.
    return (*manifold < *other.manifold);
}

/**
 * Returns true if and only if the given container contains any triangulations.
 */
bool process(Container* c) {
    bool foundTri = false;

    StandardTriangulation* std;
    Manifold* mfd;
    std::string name, structure;
    for (Packet* child = c->firstChild(); child;
            child = child->nextSibling()) {
        if (child->type() != PACKET_TRIANGULATION3)
            continue;

        foundTri = true;

        std = StandardTriangulation::isStandardTriangulation(
            static_cast<Triangulation<3>*>(child));
        if (! std)
            continue;

        mfd = std->manifold();
        if (! mfd) {
            delete std;
            continue;
        }

        name = mfd->name();
        if (detailedNames) {
            structure = mfd->structure();
            if ((! structure.empty()) && (structure != name))
                name = structure;
        }

        // Hold the newline until we know whether we've been renamed.
        if (rawList) {
            std::cout << name;

            AbelianGroup* h1 = mfd->homology();
            if (h1) {
                std::cout << ", H1 = " << h1->str();
                delete h1;
            }
        } else if (! sortMfds)
            std::cout << c->label() << "  ->>  " << name;
        totMfds++;
        totMfdsOk++;

        if (renameMfds)
            if (c->label() != name) {
                c->setLabel(name);
                totMfdsRenamed++;

                if (sortMfds)
                    std::cout << "RENAMED: " << c->label() <<
                        "  ->>  " << name << '\n';
                else
                    std::cout << "  (RENAMED)";
            }

        if (! sortMfds)
            std::cout << '\n';

        if (sortMfds)
            children.push_back(ManifoldSpec(c, true, mfd));
        else
            delete mfd;

        delete std;
        return true;
    }

    if (foundTri) {
        if (rawList)
            std::cout << "UNKNOWN\n";
        else if (! sortMfds)
            std::cout << c->label() << "  ->>  UNKNOWN\n";
        totMfds++;
    }

    if (sortMfds)
        children.push_back(ManifoldSpec(c, foundTri, 0));

    return foundTri;
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
        } else if (optChar == 'd')
            detailedNames = true;
        else if (optChar == 'o')
            saveChanges = true;
        else if (optChar == 'r')
            renameMfds = true;
        else if (optChar == 's')
            sortMfds = true;
        else if (optChar == 'w')
            rawList = true;
        else
            usage(argv[0], std::string("Invalid option: ") + argv[i]);
    }

    if (rawList && (detailedNames || saveChanges || renameMfds || sortMfds))
        usage(argv[0], "Option -w may not be used with any other options.");

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
    if (! sortMfds) {
        // Just run through them in prefix order.
        for (Packet* p = tree; p; p = p->nextTreePacket())
            if (p->type() == PACKET_CONTAINER)
                process(static_cast<Container*>(p));
    } else {
        // We need to be careful about how we run through the tree.
        // We need to do all child containers of each parent container at
        // once, so we can sort within parents.

        // Process the root first, since it doesn't have a parent to be
        // sorted within.
        if (tree->type() == PACKET_CONTAINER)
            process(static_cast<Container*>(tree));

        Packet *parent, *p;
        bool foundManifolds;
        for (parent = tree; parent; parent = parent->nextTreePacket()) {
            // Process all children of parent, and then sort.
            foundManifolds = false;
            children.clear();

            for (p = parent->firstChild(); p; p = p->nextSibling())
                if (p->type() == PACKET_CONTAINER) {
                    if (process(static_cast<Container*>(p)))
                        foundManifolds = true;
                } else
                    children.push_back(ManifoldSpec(p, false, 0));

            if (foundManifolds) {
                std::stable_sort(children.begin(), children.end());

                for (std::vector<ManifoldSpec>::iterator it = children.begin();
                        it != children.end(); it++)
                    it->container->moveToLast();
            }
        }

        // Finally tell us what we did.
        std::cout << "Final container listing:\n\n";
        for (p = tree; p; p = p->nextTreePacket())
            if (p->type() == PACKET_CONTAINER)
                std::cout << p->label() << '\n';
    }

    // Save the data file if required.
    if (saveChanges) {
        std::cerr << std::endl;
        if (totMfdsRenamed == 0 && ! sortMfds)
            std::cerr << "No 3-manifolds were renamed; not saving data.\n";
        else {
            if (sortMfds)
                std::cerr << "Manifolds were sorted and ";
            std::cerr << totMfdsRenamed
                << (totMfdsRenamed == 1 ? " manifold" : " manifolds")
                << " will be renamed.\n";
            std::cerr << "WARNING: The data file " << argv[i]
                << " will be overwritten.\n";
            std::cerr << "Proceed? (y/n) ";

            std::string line;
            getline(std::cin, line);
            while (line != "y" && line != "n") {
                std::cerr << "Please answer y or n.  Proceed? (y/n) ";
                getline(std::cin, line);
            }

            if (line != "y") {
                std::cerr << "Not saving data file.\n";
                renameMfds = false;
            } else if (tree->save(argv[i]))
                std::cerr << "Data saved to " << argv[i] << ".\n";
            else
                std::cerr << "ERROR: The data file could not be saved.\n";
        }
    }

    // Write statistics and clean up.
    delete tree;

    std::cerr << std::endl;
    std::cerr << "Final statistics:" << std::endl;
    std::cerr << "    3-manifolds examined:   " << totMfds << std::endl;
    std::cerr << "    3-manifolds recognised: " << totMfdsOk << std::endl;
    if (renameMfds)
        std::cerr << "    3-manifolds renamed:    " << totMfdsRenamed
            << std::endl;

    return 0;
}

