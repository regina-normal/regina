
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Compare two data files in search of isomorphic triangulations         *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include <cstdlib>
#include "dim4/dim4triangulation.h"
#include "file/nxmlfile.h"
#include "triangulation/ntriangulation.h"
#include "utilities/i18nutils.h"

using regina::Dim4Triangulation;
using regina::NPacket;
using regina::NTriangulation;

bool subcomplexTesting = false;

bool compare(NTriangulation* t1, NTriangulation* t2) {
    if (subcomplexTesting)
        return t1->isContainedIn(*t2).get();
    else
        return t1->isIsomorphicTo(*t2).get();
}

bool compare(Dim4Triangulation* t1, Dim4Triangulation* t2) {
    if (subcomplexTesting)
        return t1->isContainedIn(*t2).get();
    else
        return t1->isIsomorphicTo(*t2).get();
}

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " [ -m | -n ] [ -s ] <file1.rga> <file2.rga>\n";
    std::cerr << std::endl;
    std::cerr << "    -m : List matches, i.e., triangulations contained in both files (default)\n";
    std::cerr << "    -n : List non-matches, i.e., triangulations in one file but not the other\n";
    std::cerr << "    -s : Allow triangulations from file1.rga to be subcomplexes of\n"
                 "         triangulations from file2.rga\n";
    exit(1);
}

void runMatches(NPacket* tree1, NPacket* tree2, std::ostream& out) {
    NPacket *p1, *p2;

    if (subcomplexTesting)
        out << "Matching (isomorphic subcomplex) triangulations:\n"
            << std::endl;
    else
        out << "Matching (isomorphic) triangulations:\n" << std::endl;

    long nMatches = 0;

    for (p1 = tree1; p1; p1 = p1->nextTreePacket())
        if (p1->getPacketType() == NTriangulation::packetType) {
            for (p2 = tree2; p2; p2 = p2->nextTreePacket())
                if (p2->getPacketType() == NTriangulation::packetType)
                    if (compare(static_cast<NTriangulation*>(p1),
                            static_cast<NTriangulation*>(p2))) {
                        out << "    " << p1->getPacketLabel()
                            << (subcomplexTesting ? "  <=  " : "  ==  ")
                            << p2->getPacketLabel() << std::endl;
                        nMatches++;
                    }
        } else if (p1->getPacketType() == Dim4Triangulation::packetType) {
            for (p2 = tree2; p2; p2 = p2->nextTreePacket())
                if (p2->getPacketType() == Dim4Triangulation::packetType)
                    if (compare(static_cast<Dim4Triangulation*>(p1),
                            static_cast<Dim4Triangulation*>(p2))) {
                        out << "    " << p1->getPacketLabel()
                            << (subcomplexTesting ? "  <=  " : "  ==  ")
                            << p2->getPacketLabel() << std::endl;
                        nMatches++;
                    }
        }

    if (nMatches == 0)
        out << "No matches found." << std::endl;
    else if (nMatches == 1)
        out << std::endl << "1 match." << std::endl;
    else
        out << std::endl << nMatches << " matches." << std::endl;
}

void runNonMatches(const std::string& file1, NPacket* tree1,
        const std::string& file2, NPacket* tree2, std::ostream& out) {
    NPacket *p1, *p2;

    out << "Triangulations in " << file1 << " but not " << file2
        << ":\n" << std::endl;
    long nMissing = 0;

    bool matched;
    for (p1 = tree1; p1; p1 = p1->nextTreePacket())
        if (p1->getPacketType() == NTriangulation::packetType) {
            matched = false;
            for (p2 = tree2; p2 && ! matched; p2 = p2->nextTreePacket())
                if (p2->getPacketType() == NTriangulation::packetType)
                    if (compare(static_cast<NTriangulation*>(p1),
                            static_cast<NTriangulation*>(p2)))
                        matched = true;
            if (! matched) {
                out << "    " << p1->getPacketLabel() << std::endl;
                nMissing++;
            }
        } else if (p1->getPacketType() == Dim4Triangulation::packetType) {
            matched = false;
            for (p2 = tree2; p2 && ! matched; p2 = p2->nextTreePacket())
                if (p2->getPacketType() == Dim4Triangulation::packetType)
                    if (compare(static_cast<Dim4Triangulation*>(p1),
                            static_cast<Dim4Triangulation*>(p2)))
                        matched = true;
            if (! matched) {
                out << "    " << p1->getPacketLabel() << std::endl;
                nMissing++;
            }
        }

    if (nMissing == 0)
        out << "All triangulations matched." << std::endl;
    else if (nMissing == 1)
        out << std::endl << "1 non-match." << std::endl;
    else
        out << std::endl << nMissing << " non-matches." << std::endl;
}

int main(int argc, char* argv[]) {
    std::string file1, file2;
    bool listMatches = false;
    bool listNonMatches = false;

    // Parse command line.
    bool noMoreOpts = false;
    char optChar;
    for (int i = 1; i < argc; i++) {
        if ((! noMoreOpts) && *argv[i] == '-') {
            // Option.
            if (! argv[i][1])
                usage(argv[0], std::string("Invalid option: ") + argv[i]);
            else if (argv[i][2])
                usage(argv[0], std::string("Invalid option: ") + argv[i]);

            // Argument has length 2.
            optChar = argv[i][1];
            if (optChar == 'm')
                listMatches = true;
            else if (optChar == 'n')
                listNonMatches = true;
            else if (optChar == 's')
                subcomplexTesting = true;
            else if (optChar == '-')
                noMoreOpts = true;
            else
                usage(argv[0], std::string("Invalid option: ") + argv[i]);
        } else if (*argv[i]) {
            noMoreOpts = true;

            if (file1.empty())
                file1 = argv[i];
            else if (file2.empty())
                file2 = argv[i];
            else
                usage(argv[0], "You may not pass more than two data files.");
        } else
            usage(argv[0], "Empty arguments are not allowed.");
    }

    // Sanity checking.
    if (listMatches && listNonMatches) {
        usage(argv[0], "Options -n and -m may not be used together.");
    } else if (! (listMatches || listNonMatches)) {
        // List matches by default.
        listMatches = true;
    }

    if (file1.empty() || file2.empty())
        usage(argv[0], "Two data files must be specified.");

    // Open the two data files.
    NPacket* tree1 = regina::readFileMagic(file1);
    if (! tree1) {
        std::cerr << "File " << file1 << " could not be read.\n";
        std::cerr << "Please check that it exists and that it is a Regina data file.\n";

        return 1;
    }

    NPacket* tree2 = regina::readFileMagic(file2);
    if (! tree2) {
        std::cerr << "File " << file2 << " could not be read.\n";
        std::cerr << "Please check that it exists and that it is a Regina data file.\n";

        delete tree1;
        return 1;
    }

    // Since we will be writing packet labels on stdout, make sure we use a
    // character encoding that the user can read.
    regina::i18n::IConvStream out(std::cout,
        "UTF-8", regina::i18n::Locale::codeset());

    // Run our tests.
    if (listMatches)
        runMatches(tree1, tree2, out);
    else {
        runNonMatches(file1, tree1, file2, tree2, out);
        if (! subcomplexTesting) {
            out << std::endl << "--------------------\n" << std::endl;
            runNonMatches(file2, tree2, file1, tree1, out);
        }
    }

    // All done.
    delete tree1;
    delete tree2;
    return 0;
}

