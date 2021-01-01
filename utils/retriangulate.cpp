
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Exhaustively retriangulate a given manifold                           *
 *                                                                        *
 *  Copyright (c) 2017-2021, Ben Burton                                   *
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

#include <mutex>
#include <thread>
#include <popt.h>
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

std::mutex mutex;

// Command-line arguments:
int argHeight = 0;
int argThreads = 1;
bool dim4 = 0;

template <int dim>
void process(regina::Triangulation<dim>* tri) {
    unsigned long nSolns = 0;
    bool nonMinimal = false;
    std::string simpler;

    tri->retriangulate(argHeight, argThreads, nullptr /* tracker */,
        [&nSolns, &nonMinimal, &simpler, tri](regina::Triangulation<dim>& t) {
            if (t.size() > tri->size())
                return false;

            std::lock_guard<std::mutex> lock(mutex);
            std::cout << t.isoSig() << std::endl;

            if (t.size() < tri->size()) {
                nonMinimal = true;
                simpler = t.isoSig();
                return true;
            }

            ++nSolns;
            return false;
        });

    delete tri;

    if (nonMinimal) {
        std::cerr << "Triangulation is non-minimal!" << std::endl;
        std::cerr << "Smaller triangulation: " << simpler << std::endl;
    } else
        std::cerr << "Found " << nSolns << " triangulation(s)." << std::endl;
}

int main(int argc, const char* argv[]) {
    // Set up the command-line arguments.
    std::string sig;

    poptOption opts[] = {
        { "height", 'h', POPT_ARG_INT, &argHeight, 0,
            "Number of extra tetrahedra (default = 1)", "<height>" },
        { "threads", 't', POPT_ARG_INT, &argThreads, 0,
            "Number of parallel threads (default = 1)", "<threads>" },
        { "dim4", '4', POPT_ARG_NONE, &dim4, 0,
            "Input is a 4-manifold signature (default is 3-manifold)", 0 },
        POPT_AUTOHELP
        { 0, 0, 0, 0, 0, 0, 0 }
    };

    poptContext optCon = poptGetContext(0, argc, argv, opts, 0);
    poptSetOtherOptionHelp(optCon, "<isosig>");

    // Parse the command-line arguments.
    int rc = poptGetNextOpt(optCon);
    if (rc != -1) {
        std::cerr << poptBadOption(optCon, POPT_BADOPTION_NOALIAS)
            << ": " << poptStrerror(rc) << "\n\n";
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return 1;
    }

    const char** otherOpts = poptGetArgs(optCon);
    if (otherOpts && otherOpts[0]) {
        sig = otherOpts[0];
        if (otherOpts[1]) {
            std::cerr << "Too many arguments.\n\n";
            poptPrintHelp(optCon, stderr, 0);
            poptFreeContext(optCon);
            return 1;
        }
    } else {
        std::cerr << "Please give an isomorphism signature.\n\n";
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return 1;
    }

    // Set default arguments.
    if (argHeight == 0)
        argHeight = (dim4 ? 2 : 1);

    // Run a sanity check on the command-line arguments.
    bool broken = false;
    if (argHeight < 0) {
        std::cerr << "The height must be positive.\n";
        broken = true;
    } else if (dim4 && (argHeight % 2)) {
        std::cerr << "In four dimensions the height must be even.\n";
        broken = true;
    }
    if (argThreads <= 0) {
        std::cerr << "The number of threads must be positive.\n";
        broken = true;
    }

    regina::Triangulation<3>* tri3 = nullptr;
    regina::Triangulation<4>* tri4 = nullptr;

    if (dim4)
        tri4 = regina::Triangulation<4>::fromIsoSig(sig);
    else
        tri3 = regina::Triangulation<3>::fromIsoSig(sig);

    if (! (tri3 || tri4)) {
        std::cerr << "I could not interpret the given isomorphism signature.\n";
        broken = true;
    }

    if (broken) {
        std::cerr << '\n';
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return 1;
    }

    // Done parsing the command line.
    poptFreeContext(optCon);

    // Off we go!
    if (tri3)
        process(tri3);
    else
        process(tri4);

    return 0;
}

