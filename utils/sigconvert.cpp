
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Convert between different generations of signature                    *
 *                                                                        *
 *  Copyright (c) 2017-2026, Ben Burton                                   *
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

#include <getopt.h>
#include "link/link.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/generic.h"

static constexpr int maxDim = regina::maxDim();

// Command-line arguments:
int dimension = -1; // 0 indicates knot/link signatures,
                    // -1 indicates not yet specified
int sigGen = 2;

void help() {
    std::cerr <<
R"(Usage: sigconvert [ <isosig> ... | - ]
  -k, --knot                  Input is a knot/link signature
  -2, --dim2                  Input is a 2-D isomorphism signature
  -3, --dim3                  Input is a 3-D isomorphism signature (default)
  -4, --dim4                  Input is a 4-D isomorphism signature
  -d, --dim=<dimension>       Input is an isomorphism signature for the given
                              dimension of triangulation (2 ≤ dimension ≤ )"
        << maxDim << R"()
  -g, --gen=<generation>      Output first-generation signatures (--gen=1) or
                              second-generation signatures (--gen=2, default)
  -v, --version               Show which version of Regina is being used
      --help                  Show this help message
)";
}

bool processLink(const std::string& sig) {
    try {
        auto link = regina::Link::fromSig(sig);
        switch (sigGen) {
            case 1: std::cout << link.knotSig() << std::endl; break;
            case 2: std::cout << link.neoSig() << std::endl; break;
        }
        return true;
    } catch (const regina::InvalidArgument&) {
        std::cerr << "ERROR: Invalid knot/link signature: " << sig
            << std::endl;
        return false;
    }
}

template <int dim> requires (regina::supportedDim(dim))
bool processTri(const std::string& sig) {
    try {
        auto tri = regina::Triangulation<dim>::fromSig(sig);
        switch (sigGen) {
            case 1: std::cout << tri.isoSig() << std::endl; break;
            case 2: std::cout << tri.neoSig() << std::endl; break;
        }
        return true;
    } catch (const regina::InvalidArgument&) {
        std::cerr << "ERROR: Invalid " << dim << "-dimensional "
            "isomorphism signature: " << sig << std::endl;
        return false;
    }
}

int main(int argc, char* argv[]) {
    // Parse the command-line arguments.
    const char* shortOpt = ":d:g:k234v";
    struct option longOpt[] = {
        { "knot", no_argument, nullptr, 'k' },
        { "dim2", no_argument, nullptr, '2' },
        { "dim3", no_argument, nullptr, '3' },
        { "dim4", no_argument, nullptr, '4' },
        { "dim", required_argument, nullptr, 'd' },
        { "gen", required_argument, nullptr, 'g' },
        { "version", no_argument, nullptr, 'v' },
        { "help", no_argument, nullptr, '_' },
        { nullptr, 0, nullptr, 0 }
    };

    int opt;
    char* endptr;
    while ((opt = getopt_long(argc, argv, shortOpt, longOpt, nullptr)) != -1) {
        switch (opt) {
            case 'k':
                if (dimension >= 0 && dimension != 0) {
                    std::cerr << "You cannot pass more than one of "
                        "--knot, --dim2, --dim3, --dim4, or --dim.\n\n";
                    help();
                    return 1;
                }
                dimension = 0;
                break;
            case '2':
                if (dimension >= 0 && dimension != 2) {
                    std::cerr << "You cannot pass more than one of "
                        "--knot, --dim2, --dim3, --dim4, or --dim.\n\n";
                    help();
                    return 1;
                }
                dimension = 2;
                break;
            case '3':
                if (dimension >= 0 && dimension != 3) {
                    std::cerr << "You cannot pass more than one of "
                        "--knot, --dim2, --dim3, --dim4, or --dim.\n\n";
                    help();
                    return 1;
                }
                dimension = 3;
                break;
            case '4':
                if (dimension >= 0 && dimension != 4) {
                    std::cerr << "You cannot pass more than one of "
                        "--knot, --dim2, --dim3, --dim4, or --dim.\n\n";
                    help();
                    return 1;
                }
                dimension = 4;
                break;
            case 'd':
                {
                    int arg = strtol(optarg, &endptr, 10);
                    if (dimension >= 0 && dimension != arg) {
                        std::cerr << "You cannot pass more than one of "
                            "--knot, --dim2, --dim3, --dim4, or --dim.\n\n";
                        help();
                        return 1;
                    }
                    if (*endptr != 0 || arg < 2 || arg > maxDim) {
                        std::cerr << "The dimension must be an integer between "
                            "2 and " << maxDim << " inclusive.\n\n";
                        help();
                        return 1;
                    }
                    dimension = arg;
                }
                break;
            case 'g':
                sigGen = strtol(optarg, &endptr, 10);
                if (*endptr != 0 || ! (sigGen == 1 || sigGen == 2)) {
                    std::cerr << "The signature generation must be 1 or 2.\n\n";
                    help();
                    return 1;
                }
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
    if (dimension < 0)
        dimension = 3;

    if (optind == argc) {
        if (dimension == 0)
            std::cerr << "Please give a knot/link signature.\n\n";
        else
            std::cerr << "Please give an isomorphism signature.\n\n";
        help();
        return 1;
    }

    bool success = true;
    if (optind == argc - 1 && strcmp(argv[optind], "-") == 0) {
        // Read signatures from standard input.
        std::string sig;
        if (dimension == 0) {
            std::getline(std::cin, sig);
            while (! std::cin.eof()) {
                success &= processLink(sig);
                std::getline(std::cin, sig);
            }
        } else {
            std::getline(std::cin, sig);
            while (! std::cin.eof()) {
                regina::select_constexpr<2, maxDim + 1, void>(dimension,
                        [=, &sig, &success](auto dim) {
                    success &= processTri<dim>(sig);
                });
                std::getline(std::cin, sig);
            }
        }
    } else {
        // Read signatures from the command line.
        for (int i = optind; i < argc; ++i)
            if (strcmp(argv[i], "-") == 0) {
                std::cerr <<
R"(A dash (-) indicates that you will supply signatures via standard input.
However, you also appear to be supplying signatures on the command line.
You may use either standard input or the command line, but not both.

)";
                help();
                return 1;
            }

        if (dimension == 0) {
            for (int i = optind; i < argc; ++i)
                success &= processLink(argv[i]);
        } else {
            regina::select_constexpr<2, maxDim + 1, void>(dimension,
                    [=, &success](auto dim) {
                for (int i = optind; i < argc; ++i)
                    success &= processTri<dim>(argv[i]);
            });
        }
    }

    return (success ? 0 : 2);
}

