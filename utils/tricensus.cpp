
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
#define WORD_simplices (dimension == 4 ? "pentachora" : dimension == 2 ? "triangles" : "tetrahedra")
#define WORD_simplex (dimension == 4 ? "pentachoron" : dimension == 2 ? "triangle" : "tetrahedron")
#define WORD_Simplex (dimension == 4 ? "Pentachoron" : dimension == 2 ? "Triangle" : "Tetrahedron")

// Constants.
constexpr int MAXTOP = 20;

// Census parameters.
int nTop = 0, nBdryFaces = -1;
regina::BoolSet
    finiteness(true, true),
    orientability(true, true),
    boundary(true, true);
int allowInvalid = 0;
int dimension = 0; // default of 3 will be set later
enum {
    MODE_NONE = 0,
    MODE_GENPAIRS,
    MODE_USEPAIRS,
    MODE_FULLCENSUS
} mode = MODE_NONE; // default of MODE_FULLCENSUS will be set later
enum {
    OUTPUT_NONE = 0,
    OUTPUT_PAIRINGS,
    OUTPUT_DATA,
    OUTPUT_DATA_SUBCONTAINERS,
    OUTPUT_SIGS,
    OUTPUT_SIGS_CANONICAL,
    OUTPUT_TIGHT
} outputType = OUTPUT_NONE; // default will be set later
regina::Flags<regina::CensusPurge> purge {};
int threads = 1;
std::string outFile;

// Variables used for a dump of face pairings.
std::unique_ptr<std::ostream> dumpStream;
unsigned long totPairings = 0;

// Variables used for output.
long long nSolns;
bool textOutput;
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
    bool, bool, regina::Flags<regina::CensusPurge>,
    const std::shared_ptr<regina::Packet>&);

template <int dim>
bool mightBeMinimal(regina::Triangulation<dim>&);

template <int dim>
int runCensus();

// Differences between censuses of 2, 3 and 4-manifolds:
template <>
inline void findAllPerms<2>(const regina::FacetPairing<2>& p,
        regina::FacetPairing<2>::IsoList autos, bool orientableOnly,
        bool /* finiteOnly */, regina::Flags<regina::CensusPurge>,
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
        bool finiteOnly, regina::Flags<regina::CensusPurge> usePurge,
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
        bool finiteOnly, regina::Flags<regina::CensusPurge>,
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

    // For CensusPurge::NonMinimalHyp, we don't run mightBeMinimal<dim>().
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
    if (purge && purge != regina::CensusPurge::NonMinimalHyp)
        if (! mightBeMinimal<dim>(tri))
            return;

    // Put it in the census!
    switch (outputType) {
        case OUTPUT_SIGS:
            {
                std::string sig = tri.isoSig();
                if (threads > 1) {
                    std::unique_lock<std::mutex> lock(outputMutex);
                    sigStream << sig << std::endl;
                    ++nSolns;
                } else {
                    sigStream << sig << std::endl;
                    ++nSolns;
                }
            }
            break;
        case OUTPUT_SIGS_CANONICAL:
            {
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
            }
            break;
        case OUTPUT_TIGHT:
            {
                std::string enc = tri.tightEncoding();
                if (threads > 1) {
                    std::unique_lock<std::mutex> lock(outputMutex);
                    sigStream << enc << std::endl;
                    ++nSolns;
                } else {
                    sigStream << enc << std::endl;
                    ++nSolns;
                }
            }
            break;
        default:
            {
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
            break;
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
    if (outputType == OUTPUT_DATA_SUBCONTAINERS) {
        auto subContainer = std::make_shared<regina::Container>();
        subContainer->setLabel(pairing.str());
        container->append(subContainer);

        findAllPerms<dim>(pairing, std::move(autos),
            ! orientability.hasFalse(), ! finiteness.hasFalse(),
            purge, subContainer);
    } else {
        findAllPerms<dim>(pairing, std::move(autos),
            ! orientability.hasFalse(), ! finiteness.hasFalse(),
            purge, container);
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

    if (mode == MODE_USEPAIRS)
        descStream << "Only used a subset of all available "
            << WORD_face << " pairings\n";
    else {
        descStream << nTop << ' ' <<
            (nTop == 1 ? WORD_simplex : WORD_simplices) << "\n";

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

    if (purge == regina::CensusPurge::NonMinimal)
        descStream << "Ignored obviously non-minimal triangulations\n";
    else if (purge == regina::CensusPurge::NonMinimalPrime)
        descStream << "Ignored obviously non-minimal, non-prime and/or "
            "disc-reducible triangulations\n";
    else if (purge == (regina::CensusPurge::NonMinimalPrime |
            regina::CensusPurge::P2Reducible))
        descStream << "Ignored obviously non-minimal, non-prime, "
            "disc-reducible and/or P2-reducible triangulations\n";
    else if (purge == regina::CensusPurge::NonMinimalHyp)
        descStream << "Ignored triangulations that are obviously not "
            "minimal ideal triangulations of cusped finite-volume "
            "hyperbolic 3-manifolds\n";

    desc->setText(descStream.str());
    return desc;
}

void help() {
    std::cerr <<
R"help(Usage: tricensus <output-file>
  -t, --top=<num>           Number of top-dimensional simplices.
      --triangles=<num>     Alias for --top (relevant for dimension 2).
      --tetrahedra=<num>    Alias for --top (relevant for dimension 3).
      --pentachora=<num>    Alias for --top (relevant for dimension 4).
  -b, --boundary            Must have at least one boundary facet.
  -i, --internal            Must have all facets internal (no boundary facets).
  -B, --bdryfaces=<faces>   Must have fixed number (>= 1) of boundary facets.
  -o, --orientable          Must be orientable.
  -n, --nonorientable       Must be non-orientable.
  -f, --finite              Must be finite (no ideal vertices).
  -d, --ideal               Must have at least one ideal vertex.
  -m, --minimal             Ignore obviously non-minimal triangulations.
  -M, --minprime            Ignore obviously non-minimal, non-prime and/or
                            disc-reducible triangulations.
  -N, --minprimep2          Ignore obviously non-minimal, non-prime,
                            disc-reducible and/or P2-reducible triangulations.
  -h, --minhyp              Ignore triangulations that are obviously not
                            minimal ideal triangulations of cusped
                            finite-volume hyperbolic 3-manifolds.
                            Implies --internal and --ideal.
      --allowinvalid        Do not test triangulations for validity.
  -2, --dim2                Run a census of 2-manifold triangulations.
  -3, --dim3                Run a census of 3-manifold triangulations (default).
  -4, --dim4                Run a census of 4-manifold triangulations.
  -r, --regina              Output is a Regina data file (default).
  -c, --subcontainers       Output is a Regina data file, with each facet
                            pairing using a different subcontainer.
  -s, --sigs                Output is text-based, using isomorphism signatures.
  -S, --canonical           Output is text-based, using isomorphism signatures
                            and matching isomorphisms that yield canonical
                            facet pairings.
  -e, --encodings           Output is text-based, using tight encodings.
  -p, --genpairs            Only generate facet pairings, not triangulations.
  -P, --usepairs            Only use facet pairings read from standard input.
      --threads=<threads>   Number of parallel threads (default = 1).
  -v, --version             Show which version of Regina is being used.
      --help                Show this help message
)help";
}

/**
 * Parse the command-line arguments and then farm out to runCensus(),
 * which does the real work.
 */
int main(int argc, char* argv[]) {
    // Parse the command-line arguments.
    const char* shortOpt = ":t:biB:onfdmMNh234rcsSepPv";
    struct option longOpt[] = {
        { "top", required_argument, nullptr, 't' },
        { "triangles", required_argument, nullptr, 't' }, // alias for --top
        { "tetrahedra", required_argument, nullptr, 't' }, // alias for --top
        { "pentachora", required_argument, nullptr, 't' }, // alias for --top
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
        { "dim3", no_argument, nullptr, '3' },
        { "dim4", no_argument, nullptr, '4' },
        { "regina", no_argument, nullptr, 'r' },
        { "subcontainers", no_argument, nullptr, 'c' },
        { "sigs", no_argument, nullptr, 's' },
        { "canonical", no_argument, nullptr, 'S' },
        { "encodings", no_argument, nullptr, 'e' },
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
                nTop = strtol(optarg, &endptr, 10);
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
                purge = regina::CensusPurge::NonMinimal;
                break;
            case 'M':
                purge = regina::CensusPurge::NonMinimalPrime;
                break;
            case 'N':
                purge = regina::CensusPurge::NonMinimalPrime |
                    regina::CensusPurge::P2Reducible;
                break;
            case 'h':
                purge = regina::CensusPurge::NonMinimalHyp;
                break;
            case 'I':
                allowInvalid = 1;
                break;
            case '2':
                if (dimension && dimension != 2) {
                    std::cerr << "You cannot pass multiple dimension "
                        "options (-2/--dim2, -3/--dim3, -4/--dim4).\n\n";
                    help();
                    return 1;
                }
                dimension = 2;
                break;
            case '3':
                if (dimension && dimension != 3) {
                    std::cerr << "You cannot pass multiple dimension "
                        "options (-2/--dim2, -3/--dim3, -4/--dim4).\n\n";
                    help();
                    return 1;
                }
                dimension = 3;
                break;
            case '4':
                if (dimension && dimension != 4) {
                    std::cerr << "You cannot pass multiple dimension "
                        "options (-2/--dim2, -3/--dim3, -4/--dim4).\n\n";
                    help();
                    return 1;
                }
                dimension = 4;
                break;
            case 's':
                if (outputType && outputType != OUTPUT_SIGS) {
                    std::cerr << "You cannot use multiple output "
                        "formats (-r/--regina, -c/--subcontainers,\n"
                        "-s/--sigs, -S/--canonical, -e/--encodings).\n\n";
                    help();
                    return 1;
                }
                outputType = OUTPUT_SIGS;
                break;
            case 'S':
                if (outputType && outputType != OUTPUT_SIGS_CANONICAL) {
                    std::cerr << "You cannot use multiple output "
                        "formats (-r/--regina, -c/--subcontainers,\n"
                        "-s/--sigs, -S/--canonical, -e/--encodings).\n\n";
                    help();
                    return 1;
                }
                outputType = OUTPUT_SIGS_CANONICAL;
                break;
            case 'e':
                if (outputType && outputType != OUTPUT_TIGHT) {
                    std::cerr << "You cannot use multiple output "
                        "formats (-r/--regina, -c/--subcontainers,\n"
                        "-s/--sigs, -S/--canonical, -e/--encodings).\n\n";
                    help();
                    return 1;
                }
                outputType = OUTPUT_TIGHT;
                break;
            case 'r':
                if (outputType && outputType != OUTPUT_DATA) {
                    std::cerr << "You cannot use multiple output "
                        "formats (-r/--regina, -c/--subcontainers,\n"
                        "-s/--sigs, -S/--canonical, -e/--encodings).\n\n";
                    help();
                    return 1;
                }
                outputType = OUTPUT_DATA;
                break;
            case 'c':
                if (outputType && outputType != OUTPUT_DATA_SUBCONTAINERS) {
                    std::cerr << "You cannot use multiple output "
                        "formats (-r/--regina, -c/--subcontainers,\n"
                        "-s/--sigs, -S/--canonical, -e/--encodings).\n\n";
                    help();
                    return 1;
                }
                outputType = OUTPUT_DATA_SUBCONTAINERS;
                break;
            case 'p':
                if (mode && mode != MODE_GENPAIRS) {
                    std::cerr << "You cannot use multiple modes of operation "
                        "(-p/--genpairs, -P/--usepairs).\n\n";
                    help();
                    return 1;
                }
                mode = MODE_GENPAIRS;
                break;
            case 'P':
                if (mode && mode != MODE_USEPAIRS) {
                    std::cerr << "You cannot use multiple modes of operation "
                        "(-p/--genpairs, -P/--usepairs).\n\n";
                    help();
                    return 1;
                }
                mode = MODE_USEPAIRS;
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
    if (! mode)
        mode = MODE_FULLCENSUS;
    if (! outputType)
        outputType = (mode == MODE_GENPAIRS ? OUTPUT_PAIRINGS : OUTPUT_DATA);

    // Some options imply others.
    if (purge == regina::CensusPurge::NonMinimalHyp) {
        if (! finiteness.hasFalse()) {
            std::cerr << "Options -f/--finite and -h/--minhyp "
                "cannot be used together.\n\n";
            help();
            return 1;
        }
        finiteness = false;

        if (mode != MODE_USEPAIRS) {
            // If we _are_ in MODE_USEPAIRS, then boundary facet options are
            // illegal anyway.
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

    // Check for arguments that are invalid for this mode of operation.
    bool broken = false;
    switch (mode) {
        case MODE_GENPAIRS:
            if (purge) {
                std::cerr << "Minimality options cannot be used with "
                    "-p/--genpairs.\n";
                broken = true;
            }
            if (! orientability.full()) {
                std::cerr << "Orientability options cannot be used with "
                    "-p/--genpairs.\n";
                broken = true;
            }
            if (! finiteness.full()) {
                std::cerr << "Finiteness options cannot be used with "
                    "-p/--genpairs.\n";
                broken = true;
            }
            if (outputType != OUTPUT_PAIRINGS && outputType != OUTPUT_TIGHT) {
                std::cerr << "Output format options (other than "
                    "-e/--encodings) cannot be used with\n-p/--genpairs.\n";
                broken = true;
            }
            if (threads != 1) {
                std::cerr << "Multithreading options cannot be used with "
                    "-p/--genpairs.\n";
                broken = true;
            }
            break;
        case MODE_USEPAIRS:
            if (nTop) {
                std::cerr << WORD_Simplex
                    << " options cannot be used with -P/--usepairs.\n";
                broken = true;
            }
            if ((! boundary.full()) || (nBdryFaces != -1)) {
                std::cerr << "Boundary options cannot be used with "
                    "-P/--usepairs.\n";
                broken = true;
            }
            break;
        default:
            break;
    }
    if (broken) {
        std::cerr << '\n';
        help();
        return 1;
    }

    // Run a broader sanity check on the command-line arguments.
    if (mode != MODE_GENPAIRS && outFile.empty()) {
        std::cerr << "An output file must be specified.\n";
        broken = true;
    } else if (dimension == 2 && purge == regina::CensusPurge::NonMinimalHyp) {
        std::cerr << "Hyperbolicity options cannot be used with -2/--dim2.\n";
        broken = true;
    } else if (dimension == 2 && (purge & regina::CensusPurge::NonPrime)) {
        std::cerr << "Primeness options cannot be used with -2/--dim2 "
            "(the weaker -m/--minimal can).\n";
        broken = true;
    } else if (dimension == 2 && ! finiteness.full()) {
        std::cerr << "Finiteness options cannot be used with -2/--dim2.\n";
        broken = true;
    } else if (dimension == 4 && purge) {
        std::cerr << "Minimality options cannot be used with -4/--dim4.\n";
        broken = true;
    } else if (mode != MODE_USEPAIRS && nTop == 0) {
        std::cerr << "The number of " << WORD_simplices
            << " must be specified using option -t/--top.\n";
        broken = true;
    } else if (mode != MODE_USEPAIRS && (nTop < 1 || nTop > MAXTOP)) {
        std::cerr << "The number of " << WORD_simplices
            << " must be between 1 and " << MAXTOP << " inclusive.\n";
        broken = true;
    } else if (allowInvalid && ! finiteness.full()) {
        std::cerr << "Option --allowinvalid cannot be used with finite/ideal "
            "options.\n";
        broken = true;
    } else if (allowInvalid && purge) {
        std::cerr << "Option --allowinvalid cannot be used with minimality "
            "options.\n";
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
        } else if (dimension == 2 && ((nTop + nBdryFaces) % 2 != 0)) {
            std::cerr << "Number of boundary edges must have the "
                "same parity as the number of triangles.\n";
            broken = true;
        } else if (dimension == 4 && ((nTop + nBdryFaces) % 2 != 0)) {
            std::cerr << "Number of boundary facets must have the "
                "same parity as the number of pentachora.\n";
            broken = true;
        } else if (dimension == 3 && (nBdryFaces > 2 * nTop + 2)) {
            std::cerr << "Number of boundary faces for " << nTop
                << (nTop == 1 ? " tetrahedron" : " tetrahedra")
                << " can be at most " << (2 * nTop + 2) << ".\n";
            broken = true;
        } else if (dimension == 2 && (3 * nTop - nBdryFaces < 2 * (nTop - 1))) {
            std::cerr << "Number of boundary edges for " << nTop
                << (nTop == 1 ? " triangle" : " triangles")
                << " can be at most " << (nTop + 2) << ".\n";
            broken = true;
        } else if (dimension == 4 && (5 * nTop - nBdryFaces < 2 * (nTop - 1))) {
            std::cerr << "Number of boundary facets for " << nTop
                << (nTop == 1 ? " pentachoron" : " pentachora")
                << " can be at most " << (3 * nTop + 2) << ".\n";
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
    if (mode == MODE_GENPAIRS) {
        if (! outFile.empty()) {
            dumpStream = std::make_unique<std::ofstream>(outFile.c_str());
            if ((! dumpStream.get()) || (! *dumpStream)) {
                std::cerr << "Could not write to file " << outFile << ".\n";
                return 1;
            }
        }

        regina::FacetPairing<dim>::findAllPairings(nTop, boundary, nBdryFaces,
                [](const regina::FacetPairing<dim>& pair) {
            if (dumpStream.get()) {
                if (outputType == OUTPUT_TIGHT)
                    pair.tightEncode(*dumpStream);
                else
                    (*dumpStream) << pair.textRep();
                (*dumpStream) << std::endl;
            } else {
                if (outputType == OUTPUT_TIGHT)
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
    textOutput = (
        outputType == OUTPUT_SIGS ||
        outputType == OUTPUT_SIGS_CANONICAL ||
        outputType == OUTPUT_TIGHT);

    // Prepare the packet tree (or signature file) for output.
    std::shared_ptr<regina::Packet> parent;
    std::shared_ptr<regina::Packet> census;
    std::shared_ptr<regina::Packet> desc;
    if (textOutput) {
        sigStream.open(outFile.c_str());
        if (! sigStream) {
            std::cerr << "Signature file " << outFile
                << " could not be written.\n";
            return 1;
        }
    } else {
        parent = std::make_shared<regina::Container>();
        if (mode == MODE_USEPAIRS)
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
    std::thread** t = nullptr;
    if (threads > 1) {
        t = new std::thread*[threads - 1];
        for (int i = 0; i < threads - 1; ++i)
            t[i] = new std::thread(processCases<dim>);
    }

    if (mode == MODE_USEPAIRS) {
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
        if (! textOutput) {
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

        regina::FacetPairing<dim>::findAllPairings(nTop, boundary, nBdryFaces,
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
    if (textOutput) {
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

