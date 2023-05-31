
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Form a census of triangulations that satisfy given properties         *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include <condition_variable>
#include <cctype>
#include <fstream>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <getopt.h>
#include <unistd.h>
#include "census/gluingpermsearcher2.h"
#include "census/gluingpermsearcher4.h"
#include "census/gluingpermsearcher3.h"
#include "packet/container.h"
#include "packet/text.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

#define WORD_face (dimension == 4 ? "facet" : dimension == 2 ? "edge" : "face")
#define WORD_Face (dimension == 4 ? "Facet" : dimension == 2 ? "Edge" : "Face")
#define WORD_faces (dimension == 4 ? "facets" : dimension == 2 ? "edges" : "faces")
#define WORD_tetrahedra (dimension == 4 ? "pentachora" : dimension == 2 ? "triangles" : "tetrahedra")
#define WORD_tetrahedron (dimension == 4 ? "pentachoron" : dimension == 2 ? "triangle" : "tetrahedron")
#define WORD_Tetrahedron (dimension == 4 ? "Pentachoron" : dimension == 2 ? "Triangle" : "Tetrahedron")

// Constants.
constexpr int MAXTET = 20;

// Census parameters.
int nTet = 0, nBdryFaces = -1;
regina::BoolSet
    finiteness(true, true),
    orientability(true, true),
    boundary(true, true);
int minimal = 0;
int minimalPrime = 0;
int minimalPrimeP2 = 0;
int minimalHyp = 0;
int allowInvalid = 0;
int dimension = 0;
int usePairs = 0;
int sigs = 0;
int canonical = 0;
int tight = 0;
regina::CensusPurge whichPurge;
int genPairs = 0;
int subContainers = 0;
int threads = 1;
std::string outFile;

// Variables used for a dump of face pairings.
std::unique_ptr<std::ostream> dumpStream;
unsigned long totPairings = 0;

// Variables used for output.
long long nSolns;
std::ofstream sigStream;

// Variables used for multithreaded operation.
std::mutex casesMutex;
std::mutex outputMutex;
std::condition_variable cv;
using Case = std::pair<std::string, std::shared_ptr<regina::Packet>>;
std::queue<Case> cases;
bool noMoreCases = false;

// Forward declarations:
template <int dim>
void foundGluingPerms(const regina::GluingPerms<dim>&,
    const std::shared_ptr<regina::Packet>&);

template <int dim>
void foundPartial(const regina::GluingPerms<dim>&,
    const regina::GluingPermSearcher<dim>&,
    const std::shared_ptr<regina::Packet>&);

template <int dim>
void findAllPerms(const regina::FacetPairing<dim>&,
    typename regina::FacetPairing<dim>::IsoList,
    bool, bool, regina::CensusPurge, const std::shared_ptr<regina::Packet>&);

template <int dim>
bool mightBeMinimal(regina::Triangulation<dim>&);

template <int dim>
int runCensus();

// Differences between censuses of 2, 3 and 4-manifolds:
template <>
inline void findAllPerms<2>(const regina::FacetPairing<2>& p,
        regina::FacetPairing<2>::IsoList autos, bool orientableOnly,
        bool /* finiteOnly */, regina::CensusPurge,
        const std::shared_ptr<regina::Packet>& dest) {
    if (threads > 1) {
        constexpr int depth = 6; // want (dim!)^depth >> threads
        auto searcher = regina::GluingPermSearcher<2>::bestSearcher(
            p, std::move(autos), orientableOnly);
        searcher->partialSearch(depth, foundPartial<2>, *searcher, dest);
    } else {
        regina::GluingPermSearcher<2>::findAllPerms(p, std::move(autos),
            orientableOnly, foundGluingPerms<2>, dest);
    }
}

template <>
inline bool mightBeMinimal<2>(regina::Triangulation<2>& tri) {
    return tri.isMinimal();
}

template <>
inline void findAllPerms<3>(const regina::FacetPairing<3>& p,
        regina::FacetPairing<3>::IsoList autos, bool orientableOnly,
        bool finiteOnly, regina::CensusPurge usePurge,
        const std::shared_ptr<regina::Packet>& dest) {
    if (threads > 1) {
        constexpr int depth = 3; // want (dim!)^depth >> threads
        auto searcher = regina::GluingPermSearcher<3>::bestSearcher(
            p, std::move(autos), orientableOnly, finiteOnly, usePurge);
        searcher->partialSearch(depth, foundPartial<3>, *searcher, dest);
    } else {
        regina::GluingPermSearcher<3>::findAllPerms(p, std::move(autos),
            orientableOnly, finiteOnly, usePurge, foundGluingPerms<3>, dest);
    }
}

template <>
inline bool mightBeMinimal<3>(regina::Triangulation<3>& tri) {
    return ! tri.simplifyToLocalMinimum(false);
}

template <>
inline void findAllPerms<4>(const regina::FacetPairing<4>& p,
        regina::FacetPairing<4>::IsoList autos, bool orientableOnly,
        bool finiteOnly, regina::CensusPurge,
        const std::shared_ptr<regina::Packet>& dest) {
    if (threads > 1) {
        constexpr int depth = 2; // want (dim!)^depth >> threads
        auto searcher = regina::GluingPermSearcher<4>::bestSearcher(
            p, std::move(autos), orientableOnly, finiteOnly);
        searcher->partialSearch(depth, foundPartial<4>, *searcher, dest);
    } else {
        regina::GluingPermSearcher<4>::findAllPerms(p, std::move(autos),
            orientableOnly, finiteOnly, foundGluingPerms<4>, dest);
    }
}

template <>
inline bool mightBeMinimal<4>(regina::Triangulation<4>&) {
    return true;
}

/**
 * What to do with each complete triangulation that is generated.
 */
template <int dim>
void foundGluingPerms(const regina::GluingPerms<dim>& perms,
        const std::shared_ptr<regina::Packet>& container) {
    regina::Triangulation<dim> tri = perms.triangulate();

    // For minimalHyp, we don't run mightBeMinimal<dim>().
    // This is because mightBeMinimal() only tests for immediate
    // reductions (i.e., it doesn't use 4-4 moves or well-climbing
    // techniques), and HyperbolicMinSearcher already ensures that
    // no such moves are possible (since it ensures no internal vertices
    // and no low-degree edges).

    if (! (allowInvalid || tri.isValid()))
        return;
    if ((! finiteness.hasFalse()) && tri.isIdeal())
        return;
    if ((! finiteness.hasTrue()) && (! tri.isIdeal()))
        return;
    if ((! orientability.hasTrue()) && tri.isOrientable())
        return;
    if ((minimal || minimalPrime || minimalPrimeP2) &&
            ! mightBeMinimal<dim>(tri))
        return;

    // Put it in the census!
    if (sigs) {
        std::string sig = tri.isoSig();
        if (threads > 1) {
            std::unique_lock<std::mutex> lock(outputMutex);
            sigStream << sig << std::endl;
            ++nSolns;
        } else {
            sigStream << sig << std::endl;
            ++nSolns;
        }
    } else if (canonical) {
        auto [sig, iso] = tri.isoSigDetail();
        std::string isoEnc = iso.inverse().tightEncoding();
        if (threads > 1) {
            std::unique_lock<std::mutex> lock(outputMutex);
            sigStream << sig << ' ' << isoEnc << std::endl;
            ++nSolns;
        } else {
            sigStream << sig << ' ' << isoEnc << std::endl;
            ++nSolns;
        }
    } else if (tight) {
        std::string enc = tri.tightEncoding();
        if (threads > 1) {
            std::unique_lock<std::mutex> lock(outputMutex);
            sigStream << enc << std::endl;
            ++nSolns;
        } else {
            sigStream << enc << std::endl;
            ++nSolns;
        }
    } else {
        std::ostringstream out;
        auto packet = regina::make_packet(std::move(tri));
        if (threads > 1) {
            std::unique_lock<std::mutex> lock(outputMutex);
            out << "Item " << (nSolns + 1);
            packet->setLabel(out.str());
            container->append(packet);
            ++nSolns;
        } else {
            out << "Item " << (nSolns + 1);
            packet->setLabel(out.str());
            container->append(packet);
            ++nSolns;
        }
    }
}

/**
 * What to do with each partially-constructed triangulation in a
 * multi-threaded search.
 */
template <int dim>
void foundPartial(const regina::GluingPerms<dim>& perms,
        const regina::GluingPermSearcher<dim>& searcher,
        const std::shared_ptr<regina::Packet>& container) {
    if (searcher.isComplete()) {
        // The search tree was extremely shallow, which means everything
        // has essentially been enumerated by the main thread.
        foundGluingPerms(perms, container);
        return;
    }

    std::string data = searcher.taggedData();
    {
        std::unique_lock<std::mutex> lock(casesMutex);
        cases.emplace(std::move(data), container);
    }
    cv.notify_one();
}

/**
 * What to do with each face/facet pairing that is generated.
 */
template <int dim>
void foundFacePairing(const regina::FacetPairing<dim>& pairing,
        typename regina::FacetPairing<dim>::IsoList autos,
        const std::shared_ptr<regina::Packet>& container) {
    std::cout << pairing.str() << std::endl;
    // If creating a full .rga file, store triangulations for each face
    // pairing in a different container.
    if (subContainers) {
        auto subContainer = std::make_shared<regina::Container>();
        subContainer->setLabel(pairing.str());
        container->append(subContainer);

        findAllPerms<dim>(pairing, std::move(autos),
            ! orientability.hasFalse(), ! finiteness.hasFalse(),
            whichPurge, subContainer);
    } else {
        findAllPerms<dim>(pairing, std::move(autos),
            ! orientability.hasFalse(), ! finiteness.hasFalse(),
            whichPurge, container);
    }
}

/**
 * The work performed by each thread in a multithreaded census.
 */
template <int dim>
void processCases() {
    Case c;
    while (true) {
        {
            std::unique_lock<std::mutex> lock(casesMutex);
            if (! cases.empty()) {
                c = std::move(cases.front());
                cases.pop();
            } else if (noMoreCases)
                return;
            else {
                // There are no cases available right now, but some
                // might appear later.
                cv.wait(lock);
                continue;
            }
        }

        // Process the subsearch encoded by data.
        regina::GluingPermSearcher<dim>::fromTaggedData(c.first)->runSearch(
            foundGluingPerms<dim>, c.second);
    }
}

/**
 * Return a new text packet storing the census parameters.
 */
std::shared_ptr<regina::Text> parameterPacket() {
    std::shared_ptr<regina::Text> desc = std::make_shared<regina::Text>();
    desc->setLabel("Parameters");
    std::ostringstream descStream;

    descStream << "Searching for " << dimension << "-manifold triangulations\n";

    if (usePairs)
        descStream << "Only used a subset of all available "
            << WORD_face << " pairings\n";
    else {
        descStream << nTet << ' ' <<
            (nTet == 1 ? WORD_tetrahedron : WORD_tetrahedra) << "\n";

        if (boundary == true)
            descStream << "Boundary " << WORD_faces << " only\n";
        else if (boundary == false)
            descStream << "No boundary " << WORD_faces << " only\n";
        else
            descStream << "With and without boundary " << WORD_faces << "\n";

        if (nBdryFaces >= 0)
            descStream << "Requires precisely " << nBdryFaces <<
                " boundary " << WORD_faces << "\n";
    }

    if (finiteness == true)
        descStream << "Finite only\n";
    else if (finiteness == false)
        descStream << "Ideal only\n";
    else
        descStream << "Finite and ideal\n";

    if (orientability == true)
        descStream << "Orientable only\n";
    else if (orientability == false)
        descStream << "Non-orientable only\n";
    else
        descStream << "Orientable and non-orientable\n";

    if (minimalHyp)
        descStream << "Ignored triangulations that are obviously not "
            "minimal ideal triangulations of cusped finite-volume "
            "hyperbolic 3-manifolds\n";
    else if (minimalPrimeP2)
        descStream << "Ignored obviously non-minimal, non-prime, "
            "disc-reducible and/or P2-reducible triangulations\n";
    else if (minimalPrime)
        descStream << "Ignored obviously non-minimal, non-prime and/or "
            "disc-reducible triangulations\n";
    else if (minimal)
        descStream << "Ignored obviously non-minimal triangulations\n";

    desc->setText(descStream.str());
    return desc;
}

void help() {
    // TODO: Compactify this
    std::cerr <<
R"help(Usage: tricensus <output-file>
  -t, --tetrahedra, --size=<num>    Number of tetrahedra.
  -b, --boundary                    Must have at least one boundary face.
  -i, --internal                    Must have all faces internal (no boundary
                                    faces).
  -B, --bdryfaces=<faces>           Must have fixed number (>= 1) of boundary
                                    facets.
  -o, --orientable                  Must be orientable.
  -n, --nonorientable               Must be non-orientable.
  -f, --finite                      Must be finite (no ideal vertices).
  -d, --ideal                       Must have at least one ideal vertex.
  -m, --minimal                     Ignore obviously non-minimal
                                    triangulations.
  -M, --minprime                    Ignore obviously non-minimal, non-prime
                                    and/or disc-reducible triangulations.
  -N, --minprimep2                  Ignore obviously non-minimal, non-prime,
                                    disc-reducible and/or P2-reducible
                                    triangulations.
  -h, --minhyp                      Ignore triangulations that are obviously
                                    not minimal ideal triangulations of cusped
                                    finite-volume hyperbolic 3-manifolds.
                                    Implies --internal and --ideal.
      --allowinvalid                Do not test triangulations for validity
                                    before output.
  -2, --dim2                        Run a census of 2-manifold triangulations,
                                    not 3-manifold triangulations.  Here
                                    --tetrahedra counts triangles, and
                                    --bdryfaces counts boundary edges.
  -4, --dim4                        Run a census of 4-manifold triangulations,
                                    not 3-manifold triangulations.  Here
                                    --tetrahedra counts pentachora, and
                                    --bdryfaces counts boundary facets.
  -s, --sigs                        Write isomorphism signatures only, not
                                    full Regina data files.
  -S, --canonical                   Write isomorphism signatures with matching
                                    isomorphisms that yield canonical facet
                                    pairings.
  -e, --encodings                   Write tight encodings only, not full
                                    Regina data files.
  -c, --subcontainers               For each face pairing, place resulting
                                    triangulations into different subcontainers
  -p, --genpairs                    Only generate face pairings, not
                                    triangulations.
  -P, --usepairs                    Only use face pairings read from standard
                                    input.
      --threads=<threads>           Number of parallel threads (default = 1).
  -v, --version                     Show which version of Regina is being used.
      --help                        Show this help message
)help";
}

/**
 * Parse the command-line arguments and then farm out to runCensus(),
 * which does the real work.
 */
int main(int argc, char* argv[]) {
    // Parse the command-line arguments.
    // TODO: Fix types of vars
    const char* shortOpt = ":t:biB:onfdmMNh24sSecpPv";
    struct option longOpt[] = {
        { "tetrahedra", required_argument, nullptr, 't' },
        { "size", required_argument, nullptr, 't' }, // alias for --tetrahedra
        { "boundary", no_argument, nullptr, 'b' },
        { "internal", no_argument, nullptr, 'i' },
        { "bdryfaces", required_argument, nullptr, 'B' },
        { "orientable", no_argument, nullptr, 'o' },
        { "nonorientable", no_argument, nullptr, 'n' },
        { "finite", no_argument, nullptr, 'f' },
        { "ideal", no_argument, nullptr, 'd' },
        { "minimal", no_argument, nullptr, 'm' },
        { "minprime", no_argument, nullptr, 'M' },
        { "minprimep2", no_argument, nullptr, 'N' },
        { "minhyp", no_argument, nullptr, 'h' },
        { "allowinvalid", no_argument, nullptr, 'I' }, // no short opt
        { "dim2", no_argument, nullptr, '2' },
        { "dim4", no_argument, nullptr, '4' },
        { "sigs", no_argument, nullptr, 's' },
        { "canonical", no_argument, nullptr, 'S' },
        { "encodings", no_argument, nullptr, 'e' },
        { "subcontainers", no_argument, nullptr, 'c' },
        { "genpairs", no_argument, nullptr, 'p' },
        { "usepairs", no_argument, nullptr, 'P' },
        { "threads", required_argument, nullptr, 'T' }, // no short opt
        { "version", no_argument, nullptr, 'v' },
        { "help", no_argument, nullptr, '_' },
        { nullptr, 0, nullptr, 0 }
    };

    int opt;
    char* endptr;
    while ((opt = getopt_long(argc, argv, shortOpt, longOpt, nullptr)) != -1) {
        switch (opt) {
            case 't':
                nTet = strtol(optarg, &endptr, 10);
                if (*endptr != 0) {
                    std::cerr << "The triangulation/knot size must be a "
                        "positive integer.\n\n";
                    help();
                    return 1;
                }
                break;
            case 'b':
                if (! boundary.hasTrue()) {
                    std::cerr << "You cannot pass more than one of "
                        "--boundary or --internal.\n\n";
                    help();
                    return 1;
                }
                boundary = true;
                break;
            case 'i':
                if (! boundary.hasFalse()) {
                    std::cerr << "You cannot pass more than one of "
                        "--boundary or --internal.\n\n";
                    help();
                    return 1;
                }
                boundary = false;
                break;
            case 'B':
                nBdryFaces = strtol(optarg, &endptr, 10);
                if (*endptr != 0) {
                    std::cerr << "The number of boundary facets must be a "
                        "non-negative integer.\n\n";
                    help();
                    return 1;
                }
                break;
            case 'o':
                if (! orientability.hasTrue()) {
                    std::cerr << "You cannot pass more than one of "
                        "--orientable or --nonorientable.\n\n";
                    help();
                    return 1;
                }
                orientability = true;
                break;
            case 'n':
                if (! orientability.hasFalse()) {
                    std::cerr << "You cannot pass more than one of "
                        "--orientable or --nonorientable.\n\n";
                    help();
                    return 1;
                }
                orientability = false;
                break;
            case 'f':
                if (! finiteness.hasTrue()) {
                    std::cerr << "You cannot pass more than one of "
                        "--finite or --ideal.\n\n";
                    help();
                    return 1;
                }
                finiteness = true;
                break;
            case 'd':
                if (! finiteness.hasFalse()) {
                    std::cerr << "You cannot pass more than one of "
                        "--finite or --ideal.\n\n";
                    help();
                    return 1;
                }
                finiteness = false;
                break;
            case 'm':
                minimal = 1;
                break;
            case 'M':
                minimalPrime = 1;
                break;
            case 'N':
                minimalPrimeP2 = 1;
                break;
            case 'h':
                minimalHyp = 1;
                break;
            case 'I':
                allowInvalid = 1;
                break;
            case '2':
                if (dimension && dimension != 2) {
                    std::cerr << "You cannot pass more than one dimension "
                        "option.\n\n";
                    help();
                    return 1;
                }
                dimension = 2;
                break;
            case '4':
                if (dimension && dimension != 4) {
                    std::cerr << "You cannot pass more than one dimension "
                        "option.\n\n";
                    help();
                    return 1;
                }
                dimension = 4;
                break;
            case 's':
                sigs = 1;
                break;
            case 'S':
                canonical = 1;
                break;
            case 'e':
                tight = 1;
                break;
            case 'c':
                subContainers = 1;
                break;
            case 'p':
                genPairs = 1;
                break;
            case 'P':
                usePairs = 1;
                break;
            case 'T':
                threads = strtol(optarg, &endptr, 10);
                if (*endptr != 0) {
                    std::cerr << "The number of threads must be a "
                        "positive integer.\n\n";
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

    if (optind < argc) {
        outFile = argv[optind];
        if (optind + 1 < argc) {
            std::cerr << "Too many arguments.\n\n";
            help();
            return 1;
        }
    }

    // Set any undefined options to their defaults.
    if (! dimension)
        dimension = 3;

    // Some options imply others.
    if (minimalHyp) {
        if (! finiteness.hasFalse()) {
            std::cerr << "Options -f/--finite and -h/--minhyp "
                "cannot be used together.\n\n";
            help();
            return 1;
        }
        finiteness = false;

        if (! usePairs) {
            if (! boundary.hasFalse()) {
                std::cerr << "Options -b/--boundary and -h/--minhyp "
                    "cannot be used together.\n\n";
                help();
                return 1;
            }
            if (nBdryFaces > 0) {
                std::cerr << "Option -h/--minhyp cannot be used with "
                    "-B/--bdryfaces=<non-zero>.\n\n";
                help();
                return 1;
            }
            boundary = false;
        }
    }

    // Run a sanity check on the command-line arguments.
    bool broken = false;
    if ((! genPairs) && outFile.empty()) {
        std::cerr << "An output file must be specified.\n";
        broken = true;
    } else if (genPairs &&
            (minimal || minimalPrime || minimalPrimeP2 || minimalHyp)) {
        std::cerr << "Minimality options cannot be used with -p/--genpairs.\n";
        broken = true;
    } else if (genPairs && ! orientability.full()) {
        std::cerr << "Orientability options cannot be used with "
            "-p/--genpairs.\n";
        broken = true;
    } else if (genPairs && ! finiteness.full()) {
        std::cerr << "Finiteness options cannot be used with -p/--genpairs.\n";
        broken = true;
    } else if (genPairs && (sigs || canonical)) {
        std::cerr << "Signature output cannot be used with -p/--genpairs.\n";
        broken = true;
    } else if (genPairs && threads != 1) {
        std::cerr << "Multithreading options cannot be used with "
            "-p/--genpairs.\n";
        broken = true;
    } else if (dimension == 2 && minimalHyp) {
        std::cerr << "Hyperbolicity options cannot be used with -2/--dim2.\n";
        broken = true;
    } else if (dimension == 2 && (minimalPrime || minimalPrimeP2)) {
        std::cerr << "Primeness options cannot be used with -2/--dim2 "
            "(the weaker -m/--minimal can).\n";
        broken = true;
    } else if (dimension == 2 && ! finiteness.full()) {
        std::cerr << "Finiteness options cannot be used with -2/--dim2.\n";
        broken = true;
    } else if (dimension == 4 &&
            (minimal || minimalPrime || minimalPrimeP2 || minimalHyp)) {
        std::cerr << "Minimality options cannot be used with -4/--dim4.\n";
        broken = true;
    } else if (usePairs && nTet) {
        std::cerr << WORD_Tetrahedron
            << " options cannot be used with -P/--usepairs.\n";
        broken = true;
    } else if (usePairs && ((! boundary.full()) || (nBdryFaces != -1))) {
        std::cerr << "Boundary options cannot be used with -P/--usepairs.\n";
        broken = true;
    } else if ((! usePairs) && nTet == 0) {
        std::cerr << "The number of " << WORD_tetrahedra
            << " must be specified using option -t/--tetrahedra.\n";
        broken = true;
    } else if ((! usePairs) && (nTet < 1 || nTet > MAXTET)) {
        std::cerr << "The number of " << WORD_tetrahedra
            << " must be between 1 and " << MAXTET << " inclusive.\n";
        broken = true;
    } else if (allowInvalid && ! finiteness.full()) {
        std::cerr << "Option --allowinvalid cannot be used with finite/ideal "
            "options.\n";
        broken = true;
    } else if (allowInvalid &&
            (minimal || minimalPrime || minimalPrimeP2 || minimalHyp)) {
        std::cerr << "Option --allowinvalid cannot be used with minimality "
            "options.\n";
        broken = true;
    } else if (genPairs && usePairs) {
        std::cerr << "Options -p/--genpairs and -P/--usepairs "
            "cannot be used together.\n";
        broken = true;
    } else if (subContainers && (sigs || canonical || tight)) {
        std::cerr << "Signatures (-s/--sigs or -S/--canonical) or "
            "tight encodings (-e/--encodings) cannot be used with "
            "sub-containers (-c/--subcontainers).\n";
        broken = true;
    } else if ((sigs && canonical) || (sigs && tight) || (canonical && tight)) {
        std::cerr << "At most one of the options -s/--sigs, -S/--canonical "
            "or -e/--encodings can be used.\n";
        broken = true;
    } else if (threads < 1) {
        std::cerr << "The number of threads must be strictly positive.\n";
        broken = true;
    }

    if ((! broken) && (nBdryFaces != -1)) {
        if (nBdryFaces < 0) {
            std::cerr << "Number of boundary "
                << WORD_faces << " cannot be negative.\n";
            broken = true;
        } else if (nBdryFaces == 0) {
            // Asking for no boundary faces.
            if (! boundary.hasFalse()) {
                std::cerr << "Option -b/--boundary cannot be used with "
                    "-B/--bdryfaces=0.\n";
                broken = true;
            } else
                boundary = false;
        } else if (! boundary.hasTrue()) {
            std::cerr << "Option -i/--internal cannot be used with "
                "-B/--bdryfaces=<non-zero>.\n";
            broken = true;
        } else if (dimension == 3 && (nBdryFaces % 2 != 0)) {
            std::cerr << "Number of boundary faces must be even.\n";
            broken = true;
        } else if (dimension == 2 && ((nTet + nBdryFaces) % 2 != 0)) {
            std::cerr << "Number of boundary edges must have the "
                "same parity as the number of triangles.\n";
            broken = true;
        } else if (dimension == 4 && ((nTet + nBdryFaces) % 2 != 0)) {
            std::cerr << "Number of boundary facets must have the "
                "same parity as the number of pentachora.\n";
            broken = true;
        } else if (dimension == 3 && (nBdryFaces > 2 * nTet + 2)) {
            std::cerr << "Number of boundary faces for " << nTet
                << (nTet == 1 ? " tetrahedron" : " tetrahedra")
                << " can be at most " << (2 * nTet + 2) << ".\n";
            broken = true;
        } else if (dimension == 2 && (3 * nTet - nBdryFaces < 2 * (nTet - 1))) {
            std::cerr << "Number of boundary edges for " << nTet
                << (nTet == 1 ? " triangle" : " triangles")
                << " can be at most " << (nTet + 2) << ".\n";
            broken = true;
        } else if (dimension == 4 && (5 * nTet - nBdryFaces < 2 * (nTet - 1))) {
            std::cerr << "Number of boundary facets for " << nTet
                << (nTet == 1 ? " pentachoron" : " pentachora")
                << " can be at most " << (3 * nTet + 2) << ".\n";
            broken = true;
        } else {
            // Asking for a valid positive number of boundary faces.
            boundary = true;
        }
    }

    if (broken) {
        std::cerr << '\n';
        help();
        return 1;
    }

    // And off we go!
    switch (dimension) {
        case 2: return runCensus<2>();
        case 3: return runCensus<3>();
        case 4: return runCensus<4>();
    }
}

/**
 * The main census generation routine.
 */
template <int dim>
int runCensus() {
    // Are we only dumping face pairings?
    if (genPairs) {
        if (! outFile.empty()) {
            dumpStream = std::make_unique<std::ofstream>(outFile.c_str());
            if ((! dumpStream.get()) || (! *dumpStream)) {
                std::cerr << "Could not write to file " << outFile << ".\n";
                return 1;
            }
        }

        regina::FacetPairing<dim>::findAllPairings(nTet, boundary, nBdryFaces,
                [](const regina::FacetPairing<dim>& pair) {
            if (dumpStream.get()) {
                if (tight)
                    pair.tightEncode(*dumpStream);
                else
                    (*dumpStream) << pair.textRep();
                (*dumpStream) << std::endl;
            } else {
                if (tight)
                    pair.tightEncode(std::cout);
                else
                    std::cout << pair.textRep();
                std::cout << std::endl;
            }
            totPairings++;
        });
        std::cerr << "Total " << WORD_face << " pairings: "
            << totPairings << std::endl;
        return 0;
    }

    // We're actually generating triangulations.
    nSolns = 0;

    // Prepare the packet tree (or signature file) for output.
    std::shared_ptr<regina::Packet> parent;
    std::shared_ptr<regina::Packet> census;
    std::shared_ptr<regina::Packet> desc;
    if (sigs || canonical || tight) {
        sigStream.open(outFile.c_str());
        if (! sigStream) {
            std::cerr << "Signature file " << outFile
                << " could not be written.\n";
            return 1;
        }
    } else {
        parent = std::make_shared<regina::Container>();
        if (usePairs)
            parent->setLabel("Partial command-line census");
        else
            parent->setLabel("Command-line census");

        desc = parameterPacket();

        census = std::make_shared<regina::Container>();
        census->setLabel("Triangulations");

        parent->append(desc);
        parent->append(census);
    }

    // Start the census running.
    if (minimalPrimeP2)
        whichPurge = regina::PURGE_NON_MINIMAL_PRIME |
            regina::PURGE_P2_REDUCIBLE;
    else if (minimalPrime)
        whichPurge = regina::PURGE_NON_MINIMAL_PRIME;
    else if (minimalHyp)
        whichPurge = regina::PURGE_NON_MINIMAL_HYP;
    else if (minimal)
        whichPurge = regina::PURGE_NON_MINIMAL;

    std::thread** t = nullptr;
    if (threads > 1) {
        t = new std::thread*[threads - 1];
        for (int i = 0; i < threads - 1; ++i)
            t[i] = new std::thread(processCases<dim>);
    }

    if (usePairs) {
        // Only use the face pairings read from standard input.
        std::cout << "Trying " << WORD_face << " pairings..." << std::endl;
        std::string pairingList = WORD_Face;
        pairingList += " pairings:\n\n";

        std::string pairingRep;
        while (! std::cin.eof()) {
            std::getline(std::cin, pairingRep);

            if (pairingRep.length() > 0) {
                // We use a std::optional here because FacetPairing has
                // no default constructor.
                std::optional<regina::FacetPairing<dim>> p;
                try {
                    p = regina::FacetPairing<dim>::fromTextRep(pairingRep);
                } catch (const regina::InvalidArgument&) {
                    std::cerr << "Invalid " << WORD_face << " pairing: "
                        << pairingRep << std::endl;
                    pairingList += "INVALID: ";
                    pairingList += pairingRep;
                    pairingList += '\n';
                    continue;
                }

                if (! p->isCanonical()) {
                    std::cerr << "Non-canonical " << WORD_face
                        << " pairing: " << pairingRep << std::endl;
                    pairingList += "NON-CANONICAL: ";
                    pairingList += pairingRep;
                    pairingList += '\n';
                } else {
                    foundFacePairing<dim>(*p, p->findAutomorphisms(), census);
                    pairingList += p->str();
                    pairingList += '\n';
                }
            }
        }

        // Store the face pairings used with the census.
        if (! (sigs || canonical || tight)) {
            auto pairingPacket = std::make_shared<regina::Text>(pairingList);
            pairingPacket->setLabel(
                dimension == 4 ? "Facet Pairings" :
                dimension == 2 ? "Edge Pairings" :
                "Face Pairings");
            parent->insert(pairingPacket, desc);
        }
    } else {
        // An ordinary all-face-pairings census.
        std::cout << "Starting census generation..." << std::endl;

        regina::FacetPairing<dim>::findAllPairings(nTet, boundary, nBdryFaces,
            foundFacePairing<dim>, census /* dest */);
    }

    if (threads > 1) {
        {
            std::unique_lock<std::mutex> lock(casesMutex);
            noMoreCases = true;
        }
        cv.notify_all();

        std::cout << "Waiting for subsearches to finish..." << std::endl;

        // In most scenarios this main thread will finish long before
        // the workers.  The main thread should offer to help out since
        // there is nothing else left for it to do.
        processCases<dim>();

        for (int i = 0; i < threads - 1; ++i) {
            t[i]->join();
            delete t[i];
        }
        delete[] t;
    }

    std::cout << "Finished." << std::endl;

    // Write the completed census to file.
    if (sigs || canonical || tight) {
        sigStream.close();
    } else {
        if (! parent->save(outFile.c_str())) {
            std::cerr << "Output file " << outFile
                << " could not be written.\n";
            return 1;
        }
    }

    std::cout << "Total triangulations: " << nSolns << std::endl;
    return 0;
}

