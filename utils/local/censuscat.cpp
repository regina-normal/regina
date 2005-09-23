
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Combine a set of data files as a first draft for a census             *
 *                                                                        *
 *  Copyright (c) 2005, Ben Burton                                        *
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
 * The given topology data files are read in one after another.
 *
 * All triangulations are extracted and processed.  Triangulations that
 * obviously non-minimal are ignored.  All remaining triangulations are
 * sorted into groups according to their first homology groups.
 *
 * A final data file is written as XML to stdout, providing a separate
 * container of triangulations for each homology group.
 *
 * Orientable triangulations that are not 0-efficient can be dropped by
 * passing -0.
 *
 * A standard tri-quad normal surface list can be optionally generated
 * for each triangulation by passing -s.
 */

#include "file/nxmlfile.h"
#include "packet/ncontainer.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"

#include <cstdlib>
#include <iostream>
#include <map>

using namespace regina;

unsigned totGoodFiles = 0;
unsigned totErrFiles = 0;
unsigned totTris = 0;
unsigned totKept = 0;

bool makeSurfaces = false;
bool checkZeroEff = false;
NContainer* all;

typedef std::map<std::string, NPacket*> HomologyMap;
HomologyMap H1Map;

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " [ -s ] <file.rga> [ <file.rga> ... ]\n";
    std::cerr << std::endl;
    std::cerr << "    -s : Enumerate standard tri-quad normal surfaces for each triangulation\n";
    std::cerr << std::endl;
    std::cerr << "Resulting XML file is written to standard output.\n";
    std::cerr << "Statistics and diagnostic messages are written to standard error.\n";
    exit(1);
}

void insertTri(const NTriangulation& source) {
    NTriangulation* t = new NTriangulation(source);
    t->setPacketLabel(source.getPacketLabel());

    std::string H1 = t->getHomologyH1().toString();

    HomologyMap::iterator it = H1Map.find(H1);
    if (it == H1Map.end()) {
        NPacket* p = new NContainer();
        p->setPacketLabel(H1);
        p->insertChildLast(t);

        H1Map.insert(std::make_pair(H1, p));
    } else {
        it->second->insertChildLast(t);
    }

    if (makeSurfaces)
        NNormalSurfaceList::enumerate(t, NNormalSurfaceList::STANDARD)->
            setPacketLabel(t->getPacketLabel() + ": Surfaces");
}

void process(const char* filename) {
    NPacket* tree = readXMLFile(filename);
    if (tree == 0) {
        std::cerr << "ERROR: Could not read data from " << filename << '.'
            << std::endl;
        totErrFiles++;
        return;
    }

    unsigned nTris = 0;
    unsigned nGood = 0;

    NTriangulation* t;

    for (NPacket* p = tree; p; p = p->nextTreePacket())
        if (p->getPacketType() == NTriangulation::packetType) {
            nTris++;

            t = static_cast<NTriangulation*>(p);

            if (t->intelligentSimplify())
                continue;

            if (checkZeroEff && t->isOrientable() && ! t->isZeroEfficient())
                continue;

            // Looks okay.  Use it.
            nGood++;
            insertTri(*t);
        }

    delete tree;

    std::cerr << filename << ": " << nTris << " read, " << nGood << " kept."
        << std::endl;

    totGoodFiles++;
    totTris += nTris;
    totKept += nGood;
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
        } else if (optChar == '0')
            checkZeroEff = true;
        else if (optChar == 's')
            makeSurfaces = true;
        else
            usage(argv[0], std::string("Invalid option: ") + argv[i]);
    }

    // argv[i] is the first filename.
    if (i >= argc)
        usage(argv[0], "At least one data file must be given.");

    // Find the triangulations and sort into homology classes.
    for ( ; i < argc; i++)
        process(argv[i]);

    // Insert each homology container into the overall container.
    all = new NContainer();
    all->setPacketLabel("All");

    long size;
    std::cerr << std::endl;
    std::cerr << "Final homology groups:" << std::endl;
    if (H1Map.empty())
        std::cerr << "No triangulations kept." << std::endl;
    else
        for (HomologyMap::iterator it = H1Map.begin();
                it != H1Map.end(); it++) {
            size = it->second->getNumberOfChildren();
            std::cerr << "    " << it->first << " (" << size
                << (size == 1 ? " triangulation)" : " triangulations)")
                << std::endl;
            all->insertChildLast(it->second);
        }

    all->makeUniqueLabels(0);

    // Final statistics and output.
    std::cerr << std::endl;
    std::cerr << "Final statistics:" << std::endl;
    std::cerr << "    Files processed:     " << totGoodFiles << std::endl;
    std::cerr << "    Files with errors:   " << totErrFiles << std::endl;
    std::cerr << "    Triangulations read: " << totTris << std::endl;
    std::cerr << "    Triangulations kept: " << totKept << std::endl;

    all->writeXMLFile(std::cout);

    return 0;
}

