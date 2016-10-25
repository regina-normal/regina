
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Compare two data files in search of isomorphic triangulations         *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <cstdlib>
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "utilities/i18nutils.h"

using regina::Packet;
using regina::Triangulation;

bool subcomplexTesting = false;

bool compare(Triangulation<3>* t1, Triangulation<3>* t2) {
    if (subcomplexTesting)
        return t1->isContainedIn(*t2).get();
    else
        return t1->isIsomorphicTo(*t2).get();
}

bool compare(Triangulation<4>* t1, Triangulation<4>* t2) {
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

void runMatches(Packet* tree1, Packet* tree2, std::ostream& out) {
    Packet *p1, *p2;

    if (subcomplexTesting)
        out << "Matching (isomorphic subcomplex) triangulations:\n"
            << std::endl;
    else
        out << "Matching (isomorphic) triangulations:\n" << std::endl;

    long nMatches = 0;

    for (p1 = tree1; p1; p1 = p1->nextTreePacket())
        if (p1->type() == regina::PACKET_TRIANGULATION3) {
            for (p2 = tree2; p2; p2 = p2->nextTreePacket())
                if (p2->type() == regina::PACKET_TRIANGULATION3)
                    if (compare(static_cast<Triangulation<3>*>(p1),
                            static_cast<Triangulation<3>*>(p2))) {
                        out << "    " << p1->humanLabel()
                            << (subcomplexTesting ? "  <=  " : "  ==  ")
                            << p2->humanLabel() << std::endl;
                        nMatches++;
                    }
        } else if (p1->type() == regina::PACKET_TRIANGULATION4) {
            for (p2 = tree2; p2; p2 = p2->nextTreePacket())
                if (p2->type() == regina::PACKET_TRIANGULATION4)
                    if (compare(static_cast<Triangulation<4>*>(p1),
                            static_cast<Triangulation<4>*>(p2))) {
                        out << "    " << p1->label()
                            << (subcomplexTesting ? "  <=  " : "  ==  ")
                            << p2->label() << std::endl;
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

void runNonMatches(const std::string& file1, Packet* tree1,
        const std::string& file2, Packet* tree2, std::ostream& out) {
    Packet *p1, *p2;

    out << "Triangulations in " << file1 << " but not " << file2
        << ":\n" << std::endl;
    long nMissing = 0;

    bool matched;
    for (p1 = tree1; p1; p1 = p1->nextTreePacket())
        if (p1->type() == regina::PACKET_TRIANGULATION3) {
            matched = false;
            for (p2 = tree2; p2 && ! matched; p2 = p2->nextTreePacket())
                if (p2->type() == regina::PACKET_TRIANGULATION3)
                    if (compare(static_cast<Triangulation<3>*>(p1),
                            static_cast<Triangulation<3>*>(p2)))
                        matched = true;
            if (! matched) {
                out << "    " << p1->humanLabel() << std::endl;
                nMissing++;
            }
        } else if (p1->type() == regina::PACKET_TRIANGULATION4) {
            matched = false;
            for (p2 = tree2; p2 && ! matched; p2 = p2->nextTreePacket())
                if (p2->type() == regina::PACKET_TRIANGULATION4)
                    if (compare(static_cast<Triangulation<4>*>(p1),
                            static_cast<Triangulation<4>*>(p2)))
                        matched = true;
            if (! matched) {
                out << "    " << p1->label() << std::endl;
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
    Packet* tree1 = regina::open(file1.c_str());
    if (! tree1) {
        std::cerr << "File " << file1 << " could not be read.\n";
        std::cerr << "Please check that it exists and that it is a Regina data file.\n";

        return 1;
    }

    Packet* tree2 = regina::open(file2.c_str());
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

