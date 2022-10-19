
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Exhaustively retriangulate a given manifold                           *
 *                                                                        *
 *  Copyright (c) 2017-2022, Ben Burton                                   *
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
#include "link/link.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

std::mutex mutex;

enum Flavours {
    FLAVOUR_DIM3 = 0,
    FLAVOUR_DIM4 = 1,
    FLAVOUR_KNOT = 100
};

constexpr int NO_HEIGHT = -1000;

// Command-line arguments:
int argHeight = NO_HEIGHT;
int argThreads = 1;
int flavour = FLAVOUR_DIM3;
int internalSig = 0;

template <int dim>
void process(const regina::Triangulation<dim>& tri) {
    unsigned long nSolns = 0;
    bool nonMinimal = false;
    std::string simpler;

    tri.retriangulate(argHeight, argThreads, nullptr /* tracker */,
        [&nSolns, &nonMinimal, &simpler, &tri](
                const std::string& sig, const regina::Triangulation<dim>& t) {
            if (t.size() > tri.size())
                return false;

            if (internalSig) {
                std::lock_guard<std::mutex> lock(mutex);
                std::cout << sig << std::endl;

                if (t.size() < tri.size()) {
                    nonMinimal = true;
                    simpler = sig;
                    return true;
                }

                ++nSolns;
                return false;
            } else {
                // Recompute the signature using the default type IsoSigClassic.
                std::string classic = t.isoSig();

                std::lock_guard<std::mutex> lock(mutex);
                std::cout << classic << std::endl;

                if (t.size() < tri.size()) {
                    nonMinimal = true;
                    simpler = std::move(classic);
                    return true;
                }

                ++nSolns;
                return false;
            }
        });

    if (nonMinimal) {
        std::cerr << "Triangulation is non-minimal!" << std::endl;
        std::cerr << "Smaller triangulation: " << simpler << std::endl;
    } else
        std::cerr << "Found " << nSolns << " triangulation(s)." << std::endl;
}

void process(const regina::Link& knot) {
    unsigned long nSolns = 0;
    bool nonMinimal = false;
    std::string simpler;

    knot.rewrite(argHeight, argThreads, nullptr /* tracker */,
        [&nSolns, &nonMinimal, &simpler, &knot](
                const std::string& sig, const regina::Link& k) {
            if (k.size() > knot.size())
                return false;

            std::lock_guard<std::mutex> lock(mutex);
            std::cout << sig << std::endl;

            if (k.size() < knot.size()) {
                nonMinimal = true;
                simpler = sig;
                return true;
            }

            ++nSolns;
            return false;
        });

    if (nonMinimal) {
        std::cerr << "Knot is non-minimal!" << std::endl;
        std::cerr << "Smaller knot: " << simpler << std::endl;
    } else
        std::cerr << "Found " << nSolns << " knot(s)." << std::endl;
}

int main(int argc, const char* argv[]) {
    // Set up the command-line arguments.
    int showVersion = 0;
    std::string sig;

    poptOption opts[] = {
        { "height", 'h', POPT_ARG_INT, &argHeight, 0,
            "Number of extra simplices/crossings (default = 1)", "<height>" },
        { "threads", 't', POPT_ARG_INT, &argThreads, 0,
            "Number of parallel threads (default = 1)", "<threads>" },
        { "dim3", '3', POPT_ARG_VAL, &flavour, FLAVOUR_DIM3,
            "Input is a 3-manifold signature (default)", nullptr },
        { "dim4", '4', POPT_ARG_VAL, &flavour, FLAVOUR_DIM4,
            "Input is a 4-manifold signature", nullptr },
        { "knot", 'k', POPT_ARG_VAL, &flavour, FLAVOUR_KNOT,
            "Input is a knot signature", nullptr },
        { "anysig", 'a', POPT_ARG_NONE, &internalSig, 0,
            "Output does not need to use classic signature(s)", nullptr },
        { "version", 'v', POPT_ARG_NONE, &showVersion, 0,
            "Show which version of Regina is being used.", nullptr },
        POPT_AUTOHELP
        { nullptr, 0, 0, nullptr, 0, nullptr, nullptr }
    };

    poptContext optCon = poptGetContext(nullptr, argc, argv, opts, 0);
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

    if (showVersion) {
        // If other arguments were passed, just silently ignore them for now.
        // Other (non-popt) command-line tools give an error in this scenario.
        std::cout << PACKAGE_BUILD_STRING << std::endl;
        poptFreeContext(optCon);
        return 0;
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
        std::cerr << "Please give an isomorphism/knot signature.\n\n";
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return 1;
    }

    // Set default arguments.
    if (argHeight == NO_HEIGHT) {
        switch (flavour) {
            case FLAVOUR_DIM3: argHeight = 1; break;
            case FLAVOUR_DIM4: argHeight = 2; break;
            case FLAVOUR_KNOT: argHeight = 1; break;
        }
    }

    // Run a sanity check on the command-line arguments.
    bool broken = false;
    if (argHeight < 0) {
        std::cerr << "The height cannot be negative.\n";
        broken = true;
    } else if (flavour == FLAVOUR_DIM3 && argHeight == 0) {
        std::cerr << "In three dimensions the height must be positive.\n";
        broken = true;
    } else if (flavour == FLAVOUR_DIM4 && (argHeight % 2)) {
        std::cerr << "In four dimensions the height must be even.\n";
        broken = true;
    }
    if (argThreads <= 0) {
        std::cerr << "The number of threads must be positive.\n";
        broken = true;
    }

    if (! broken) {
        switch (flavour) {
            case FLAVOUR_DIM3: {
                try {
                    process(regina::Triangulation<3>::fromIsoSig(sig));
                } catch (const regina::InvalidArgument&) {
                    std::cerr << "I could not interpret the given "
                        "3-manifold isomorphism signature.\n";
                    broken = true;
                }
                break;
            }
            case FLAVOUR_DIM4: {
                try {
                    process(regina::Triangulation<4>::fromIsoSig(sig));
                } catch (const regina::InvalidArgument&) {
                    std::cerr << "I could not interpret the given "
                        "4-manifold isomorphism signature.\n";
                    broken = true;
                }
                break;
            }
            case FLAVOUR_KNOT: {
                try {
                    process(regina::Link::fromKnotSig(sig));
                } catch (const regina::InvalidArgument&) {
                    std::cerr << "I could not interpret the given "
                        "knot signature.\n";
                    broken = true;
                }
                break;
            }
        }
    }

    if (broken) {
        std::cerr << '\n';
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return 1;
    } else {
        poptFreeContext(optCon);
        return 0;
    }
}

