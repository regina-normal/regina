
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Compare two data files in search of isomorphic triangulations         *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <cstdlib>
#include <cstring>
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/generic.h"
#include "utilities/i18nutils.h"

using regina::Packet;
using regina::Triangulation;

template <int dim> requires (regina::supportedDim(dim))
using TriPacket = regina::PacketOf<Triangulation<dim>>;

bool subcomplexTesting = false;

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " [ -m | -n ] [ -s ] "
        "<file1.rga> <file2.rga>\n"
        "    " << progName << " [ -v, --version | -?, --help ]\n";
    std::cerr << std::endl;
    std::cerr << "    -m : List matches, i.e., triangulations "
        "contained in both files (default)\n";
    std::cerr << "    -n : List non-matches, i.e., triangulations "
        "in one file but not the other\n";
    std::cerr << "    -s : Allow triangulations from file1.rga to be "
        "subcomplexes of\n"
        "         triangulations from file2.rga\n";
    std::cerr << std::endl;
    std::cerr << "    -v, --version : Show which version of Regina "
        "is being used\n";
    std::cerr << "    -?, --help    : Display this help\n";
    exit(1);
}

template <int dim> requires (regina::supportedDim(dim))
bool hasMatchFrom(const TriPacket<dim>& src, const Packet& tree) {
    for (const Packet& p : tree) {
        if (p.type() != src.type())
            continue;

        if (subcomplexTesting) {
            if (src.isContainedIn(static_cast<const TriPacket<dim>&>(p)))
                return true;
        } else {
            if (src.isIsomorphicTo(static_cast<const TriPacket<dim>&>(p)))
                return true;
        }
    }
    return false;
}

template <int dim> requires (regina::supportedDim(dim))
size_t writeMatchesFrom(const TriPacket<dim>& src, const Packet& tree,
        std::ostream& out) {
    size_t matches = 0;
    for (const Packet& p : tree) {
        if (p.type() != src.type())
            continue;

        if (subcomplexTesting) {
            if (src.isContainedIn(static_cast<const TriPacket<dim>&>(p))) {
                out << "    " << src.humanLabel() << "  <=  "
                    << p.humanLabel() << std::endl;
                ++matches;
            }
        } else {
            if (src.isIsomorphicTo(static_cast<const TriPacket<dim>&>(p))) {
                out << "    " << src.humanLabel() << "  ==  "
                    << p.humanLabel() << std::endl;
                ++matches;
            }
        }
    }
    return matches;
}

void runMatches(const Packet& tree1, const Packet& tree2, std::ostream& out) {
    if (subcomplexTesting)
        out << "Matching (isomorphic subcomplex) triangulations:\n"
            << std::endl;
    else
        out << "Matching (isomorphic) triangulations:\n" << std::endl;

    long matches = 0;

    for (const Packet& src : tree1)
        if (int srcDim = regina::isTriangulation(src.type()))
            regina::select_constexpr<2, regina::maxDim() + 1, void>(srcDim,
                [&](auto dim) {
                    matches += writeMatchesFrom(
                        static_cast<const TriPacket<dim>&>(src), tree2, out);
                });

    if (matches == 0)
        out << "No matches found." << std::endl;
    else if (matches == 1)
        out << std::endl << "1 match." << std::endl;
    else
        out << std::endl << matches << " matches." << std::endl;
}

void runNonMatches(const std::string& file1, const Packet& tree1,
        const std::string& file2, const Packet& tree2,
        std::ostream& out) {
    out << "Triangulations in " << file1 << " but not " << file2
        << ":\n" << std::endl;
    long missing = 0;

    bool matched;
    for (const Packet& src : tree1)
        if (int srcDim = regina::isTriangulation(src.type()))
            regina::select_constexpr<2, regina::maxDim() + 1, void>(srcDim,
                [&](auto dim) {
                    if (! hasMatchFrom(static_cast<const TriPacket<dim>&>(src),
                            tree2)) {
                        out << "    " << src.humanLabel() << std::endl;
                        ++missing;
                    }
                });

    if (missing == 0)
        out << "All triangulations matched." << std::endl;
    else if (missing == 1)
        out << std::endl << "1 non-match." << std::endl;
    else
        out << std::endl << missing << " non-matches." << std::endl;
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
            // Standard arguments:
            if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0)
                usage(argv[0]);
            if (strcmp(argv[i], "-v") == 0 ||
                    strcmp(argv[i], "--version") == 0) {
                if (argc != 2)
                    usage(argv[0], "Option --version cannot be "
                        "used with any other arguments.");
                std::cout << PACKAGE_BUILD_STRING << std::endl;
                exit(0);
            }

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
    std::shared_ptr<Packet> tree1 = regina::open(file1.c_str());
    if (! tree1) {
        std::cerr << "File " << file1 << " could not be read.\n";
        std::cerr << "Please check that it exists and that it is a "
            "Regina data file.\n";
        return 1;
    }

    std::shared_ptr<Packet> tree2 = regina::open(file2.c_str());
    if (! tree2) {
        std::cerr << "File " << file2 << " could not be read.\n";
        std::cerr << "Please check that it exists and that it is a "
            "Regina data file.\n";
        return 1;
    }

    // Since we will be writing packet labels on stdout, make sure we use a
    // character encoding that the user can read.
    regina::i18n::IConvStream out(std::cout,
        "UTF-8", regina::i18n::Locale::codeset());

    // Run our tests.
    if (listMatches)
        runMatches(*tree1, *tree2, out);
    else {
        runNonMatches(file1, *tree1, file2, *tree2, out);
        if (! subcomplexTesting) {
            out << std::endl << "--------------------\n" << std::endl;
            runNonMatches(file2, *tree2, file1, *tree1, out);
        }
    }

    // All done.
    return 0;
}

