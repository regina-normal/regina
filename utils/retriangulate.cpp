
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Exhaustively retriangulate a given manifold                           *
 *                                                                        *
 *  Copyright (c) 2017-2025, Ben Burton                                   *
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

#include <mutex>
#include <thread>
#include <getopt.h>
#include "link/link.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

std::mutex mutex;

constexpr int NO_HEIGHT = -1000;

// Command-line arguments:
int height = NO_HEIGHT;
long threads = 1;
enum {
    FLAVOUR_NONE = 0,
    FLAVOUR_DIM3 = 3,
    FLAVOUR_DIM4 = 4,
    FLAVOUR_KNOT = 100
} flavour = FLAVOUR_NONE;
bool showAll = false;
bool internalSig = false;
bool virtualMoves = false;
bool classicalMoves = false;

template <int dim>
void process(const regina::Triangulation<dim>& tri) {
    size_t nSolns = 0;
    bool nonMinimal = false;
    std::string simpler;

    tri.retriangulate(height, threads, nullptr /* tracker */,
        [&nSolns, &nonMinimal, &simpler, &tri](
                const std::string& sig, const regina::Triangulation<dim>& t) {
            if (t.size() > tri.size()) {
                if (showAll) {
                    if (internalSig) {
                        std::lock_guard<std::mutex> lock(mutex);
                        std::cout << sig << std::endl;
                        ++nSolns;
                    } else {
                        // Recompute the signature using IsoSigClassic.
                        std::string classic = t.isoSig();

                        std::lock_guard<std::mutex> lock(mutex);
                        std::cout << classic << std::endl;
                        ++nSolns;
                    }
                }
                return false;
            }

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
                // Recompute the signature using IsoSigClassic.
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

void process(const regina::Link& link) {
    size_t nSolns = 0;
    bool nonMinimal = false;
    std::string simpler;

    std::function<bool(const std::string&, const regina::Link&)> action =
            [&nSolns, &nonMinimal, &simpler, &link](
            const std::string& sig, const regina::Link& k) {
        if (k.size() > link.size()) {
            if (showAll) {
                std::lock_guard<std::mutex> lock(mutex);
                std::cout << sig << std::endl;
                ++nSolns;
            }
            return false;
        }

        std::lock_guard<std::mutex> lock(mutex);
        std::cout << sig << std::endl;

        if (k.size() < link.size()) {
            nonMinimal = true;
            simpler = sig;
            return true;
        }

        ++nSolns;
        return false;
    };

    if (virtualMoves || ! (classicalMoves || link.isClassical()))
        link.rewriteVirtual(height, threads, nullptr /* tracker */, action);
    else
        link.rewrite(height, threads, nullptr /* tracker */, action);

    if (nonMinimal) {
        std::cerr << "Link is non-minimal!" << std::endl;
        std::cerr << "Smaller link: " << simpler << std::endl;
    } else
        std::cerr << "Found " << nSolns << " link(s)." << std::endl;
}

void help() {
    std::cerr <<
R"help(Usage: retriangulate <isosig>
  -h, --height=<height>       Number of extra simplices/crossings (default = 1)
  -t, --threads=<threads>     Number of parallel threads (default = 1)
  -3, --dim3                  Input is a 3-manifold signature (default)
  -4, --dim4                  Input is a 4-manifold signature
  -k, --knot                  Input is a knot/link signature
  -l, --all                   Output larger triangulations/links also
  -a, --anysig                Output does not need to use classic signature(s)
  -c, --classical             Never allow virtual type II moves (default for
                              classical link diagrams)
  -V, --virtual               Always allow virtual type II moves (default for
                              virtual link diagrams)
  -v, --version               Show which version of Regina is being used
      --help                  Show this help message
)help";
}

int main(int argc, char* argv[]) {
    // Parse the command-line arguments.
    const char* shortOpt = ":h:t:34klacVv";
    struct option longOpt[] = {
        { "height", required_argument, nullptr, 'h' },
        { "threads", required_argument, nullptr, 't' },
        { "dim3", no_argument, nullptr, '3' },
        { "dim4", no_argument, nullptr, '4' },
        { "knot", no_argument, nullptr, 'k' },
        { "all", no_argument, nullptr, 'l' },
        { "anysig", no_argument, nullptr, 'a' },
        { "classical", no_argument, nullptr, 'c' },
        { "virtual", no_argument, nullptr, 'V' },
        { "version", no_argument, nullptr, 'v' },
        { "help", no_argument, nullptr, '_' },
        { nullptr, 0, nullptr, 0 }
    };

    int opt;
    char* endptr;
    while ((opt = getopt_long(argc, argv, shortOpt, longOpt, nullptr)) != -1) {
        switch (opt) {
            case 'h':
                height = strtol(optarg, &endptr, 10);
                if (*endptr != 0) {
                    std::cerr << "The height must be a non-negative "
                        "integer.\n\n";
                    help();
                    return 1;
                }
                break;
            case 't':
                threads = strtol(optarg, &endptr, 10);
                if (*endptr != 0) {
                    std::cerr << "The number of threads must be a positive "
                        "integer.\n\n";
                    help();
                    return 1;
                }
                break;
            case '3':
                if (flavour && flavour != FLAVOUR_DIM3) {
                    std::cerr << "You cannot pass more than one of "
                        "--dim3, --dim4 or --knot.\n\n";
                    help();
                    return 1;
                }
                flavour = FLAVOUR_DIM3;
                break;
            case '4':
                if (flavour && flavour != FLAVOUR_DIM4) {
                    std::cerr << "You cannot pass more than one of "
                        "--dim3, --dim4 or --knot.\n\n";
                    help();
                    return 1;
                }
                flavour = FLAVOUR_DIM4;
                break;
            case 'k':
                if (flavour && flavour != FLAVOUR_KNOT) {
                    std::cerr << "You cannot pass more than one of "
                        "--dim3, --dim4 or --knot.\n\n";
                    help();
                    return 1;
                }
                flavour = FLAVOUR_KNOT;
                break;
            case 'l':
                showAll = true;
                break;
            case 'a':
                internalSig = true;
                break;
            case 'c':
                classicalMoves = true;
                break;
            case 'V':
                virtualMoves = true;
                break;
            case 'v':
                // If other arguments were passed, just silently ignore them
                // for now.  Ideally we would give an error in this scenario.
                std::cout << PACKAGE_BUILD_STRING << std::endl;
                return 0;
            case '_':
                help();
                return 0;
            case '?':
                if (optopt == 0) {
                    // Unknown long option.
                    std::cerr << "Unknown option: " << argv[optind - 1];
                } else {
                    // Unknown short option.
                    std::cerr << "Unknown option: -" <<
                        static_cast<char>(optopt);
                }
                std::cerr << "\n\n";
                help();
                return 1;
            case ':':
                // If an argument is missing, getopt_long() behaves as though
                // the short option were passed, and there is no indication
                // as to whether the user specified a long option instead.
                // For now we will just use the short option in our error
                // message, since the subsequent help text explains both the
                // short and long options anyway.
                std::cerr << "Missing argument: -" << static_cast<char>(optopt);
                std::cerr << "\n\n";
                help();
                return 1;
        }
    }

    // Set any undefined options to their defaults.
    if (! flavour)
        flavour = FLAVOUR_DIM3;

    std::string sig;
    if (optind < argc) {
        sig = argv[optind];
        if (optind + 1 < argc) {
            std::cerr << "Too many arguments.\n\n";
            help();
            return 1;
        }
    } else {
        if (flavour == FLAVOUR_KNOT)
            std::cerr << "Please give a knot/link signature.\n\n";
        else
            std::cerr << "Please give an isomorphism signature.\n\n";
        help();
        return 1;
    }

    // Set default arguments.
    if (height == NO_HEIGHT) {
        switch (flavour) {
            case FLAVOUR_DIM3: height = 1; break;
            case FLAVOUR_DIM4: height = 2; break;
            case FLAVOUR_KNOT: height = 1; break;
            default: /* should never occur */ break;
        }
    }

    // Run a sanity check on the command-line arguments.
    bool broken = false;
    if (height < 0) {
        std::cerr << "The height cannot be negative.\n";
        broken = true;
    } else if (flavour == FLAVOUR_DIM3 && height == 0) {
        std::cerr << "In three dimensions the height must be positive.\n";
        broken = true;
    } else if (flavour == FLAVOUR_DIM4 && (height % 2)) {
        std::cerr << "In four dimensions the height must be even.\n";
        broken = true;
    }
    if (threads <= 0) {
        std::cerr << "The number of threads must be positive.\n";
        broken = true;
    }
    if ((virtualMoves || classicalMoves) && flavour != FLAVOUR_KNOT) {
        std::cerr << "The options --classical and --virtual can only be used "
            "with --knot.\n";
        broken = true;
    }
    if (virtualMoves && classicalMoves) {
        std::cerr << "The options --classical and --virtual cannot be used "
            "together.\n";
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
                    process(regina::Link::fromSig(sig));
                } catch (const regina::InvalidArgument&) {
                    std::cerr << "I could not interpret the given "
                        "knot/link signature.\n";
                    broken = true;
                }
                break;
            }
            default: /* should never occur */
                break;
        }
    }

    if (broken) {
        std::cerr << '\n';
        help();
        return 1;
    } else {
        return 0;
    }
}

