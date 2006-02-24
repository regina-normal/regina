
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Attempt to recognise the underlying 3-manifolds in a census           *
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
 */

#include <file/nxmlfile.h>
#include <manifold/nlensspace.h>
#include <manifold/nngsfsloop.h>
#include <manifold/nngsfspair.h>
#include <manifold/nsfs.h>
#include <manifold/ntorusbundle.h>
#include <packet/ncontainer.h>
#include <subcomplex/nstandardtri.h>
#include <triangulation/ntriangulation.h>

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
NPacket* tree;

struct ManifoldSpec {
    NPacket* container;
    bool hasTriangulation;
    NManifold* manifold; // 0 if unknown

    ManifoldSpec() : container(0), hasTriangulation(false), manifold(0) {
    }
    ManifoldSpec(NPacket* c, bool t, NManifold* m) :
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
        " [ -d ] [ -o ] [ -r ] [ -s ] <file.rga>\n";
    std::cerr << std::endl;
    std::cerr << "    -d : Use more detailed 3-manifold names\n";
    std::cerr << "    -o : Save changes to the original file\n";
    std::cerr << "    -r : Rename container packets according to 3-manifold\n";
    std::cerr << "    -s : Sort container packets by 3-manifold\n";
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

    // Lens spaces go first.
    NLensSpace* lens1 = dynamic_cast<NLensSpace*>(manifold);
    NLensSpace* lens2 = dynamic_cast<NLensSpace*>(other.manifold);

    if (lens1 && ! lens2)
        return true;
    if (lens2 && ! lens1)
        return false;
    if (lens1 && lens2) {
        return (lens1->getP() < lens2->getP() ||
            (lens1->getP() == lens2->getP() && lens1->getQ() < lens2->getQ()));
    }

    // Next go through Seifert fibred spaces.
    NSFSpace* sfs1 = dynamic_cast<NSFSpace*>(manifold);
    NSFSpace* sfs2 = dynamic_cast<NSFSpace*>(other.manifold);

    if (sfs1 && ! sfs2)
        return true;
    if (sfs2 && ! sfs1)
        return false;
    if (sfs1 && sfs2)
        return (*sfs1 < *sfs2);

    // Now for torus bundles.
    NTorusBundle* bundle1 = dynamic_cast<NTorusBundle*>(manifold);
    NTorusBundle* bundle2 = dynamic_cast<NTorusBundle*>(other.manifold);

    if (bundle1 && ! bundle2)
        return true;
    if (bundle2 && ! bundle1)
        return false;
    if (bundle1 && bundle2) {
        // Just sort by name here, since bundle parameters will probably
        // need to be made canonical anyway.
        return manifold->getName() < other.manifold->getName();
    }

    // Finally non-geometric stuff (SFS pairs and loops).
    // In all of these cases sort by name for want of something more
    // sophisticated (TODO).
    NNGSFSPair* pair1 = dynamic_cast<NNGSFSPair*>(manifold);
    NNGSFSPair* pair2 = dynamic_cast<NNGSFSPair*>(other.manifold);
    if (pair1 && ! pair2)
        return true;
    if (pair2 && ! pair1)
        return false;
    if (pair1 && pair2)
        return manifold->getName() < other.manifold->getName();

    NNGSFSLoop* loop1 = dynamic_cast<NNGSFSLoop*>(manifold);
    NNGSFSLoop* loop2 = dynamic_cast<NNGSFSLoop*>(other.manifold);
    if (loop1 && ! loop2)
        return true;
    if (loop2 && ! loop1)
        return false;
    if (loop1 && loop2)
        return manifold->getName() < other.manifold->getName();

    // No idea.  Use the dictionary.
    return manifold->getName() < other.manifold->getName();
}

/**
 * Returns true if and only if the given container contains any triangulations.
 */
bool process(NContainer* c) {
    bool foundTri = false;

    NStandardTriangulation* std;
    NManifold* mfd;
    std::string name, structure;
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

        name = mfd->getName();
        if (detailedNames) {
            structure = mfd->getStructure();
            if ((! structure.empty()) && (structure != name))
                name = structure;
        }

        // Hold the newline until we know whether we've been renamed.
        if (! sortMfds)
            std::cout << c->getPacketLabel() << "  ->>  " << name;
        totMfds++;
        totMfdsOk++;

        if (renameMfds)
            if (c->getPacketLabel() != name) {
                c->setPacketLabel(name);
                totMfdsRenamed++;

                if (sortMfds)
                    std::cout << "RENAMED: " << c->getPacketLabel() <<
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
        if (! sortMfds)
            std::cout << c->getPacketLabel() << "  ->>  UNKNOWN\n";
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
    if (! sortMfds) {
        // Just run through them in prefix order.
        for (NPacket* p = tree; p; p = p->nextTreePacket())
            if (p->getPacketType() == NContainer::packetType)
                process(static_cast<NContainer*>(p));
    } else {
        // We need to be careful about how we run through the tree.
        // We need to do all child containers of each parent container at
        // once, so we can sort within parents.

        // Process the root first, since it doesn't have a parent to be
        // sorted within.
        if (tree->getPacketType() == NContainer::packetType)
            process(static_cast<NContainer*>(tree));

        NPacket *parent, *p;
        bool foundManifolds;
        for (parent = tree; parent; parent = parent->nextTreePacket()) {
            // Process all children of parent, and then sort.
            foundManifolds = false;
            children.clear();

            for (p = parent->getFirstTreeChild(); p;
                    p = p->getNextTreeSibling())
                if (p->getPacketType() == NContainer::packetType) {
                    if (process(static_cast<NContainer*>(p)))
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
            if (p->getPacketType() == NContainer::packetType)
                std::cout << p->getPacketLabel() << '\n';
    }

    // Save the data file if required.
    if (saveChanges) {
        tree->makeUniqueLabels(0);

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
            } else if (writeXMLFile(argv[i], tree))
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

