
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Form a census of triangulations that satisfy given properties         *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include <cctype>
#include <fstream>
#include <memory>
#include <sstream>
#include <popt.h>
#include <unistd.h>
#include "census/ncensus.h"
#include "file/nxmlfile.h"
#include "packet/ncontainer.h"
#include "packet/ntext.h"
#include "progress/nprogressmanager.h"

// Constants.
const long MAXTET = 15;
const unsigned SLEEP_SECONDS = 1;

// Census parameters.
long nTet = 0, nBdryFaces = -1;
regina::NBoolSet
    finiteness(true, true),
    orientability(true, true),
    boundary(true, true);
int minimal = 0;
int minimalPrime = 0;
int minimalPrimeP2 = 0;
int usePairs = 0;

// Variables used for a dump of face pairings.
std::auto_ptr<std::ostream> dumpStream;
unsigned long totPairings = 0;

/**
 * Dump the given face pairing to dumpStream.
 */
void dumpPairing(const regina::NFacePairing* pair,
        const regina::NFacePairingIsoList*, void*) {
    if (pair) {
        if (dumpStream.get())
            (*dumpStream) << (*pair).toTextRep() << std::endl;
        else
            std::cout << (*pair).toTextRep() << std::endl;
        totPairings++;
    }
}

/**
 * Return a new text packet storing the census parameters.
 */
regina::NText* parameterPacket() {
    regina::NText* desc = new regina::NText();
    desc->setPacketLabel("Parameters");
    std::ostringstream descStream;

    if (usePairs)
        descStream << "Only used a subset of all available face pairings.\n";
    else {
        descStream << nTet << (nTet == 1 ? " tetrahedron\n" : " tetrahedra\n");

        if (boundary == regina::NBoolSet::sTrue)
            descStream << "Boundary faces only\n";
        else if (boundary == regina::NBoolSet::sFalse)
            descStream << "No boundary faces only\n";
        else
            descStream << "With and without boundary faces\n";

        if (nBdryFaces >= 0)
            descStream << "Requires precisely " << nBdryFaces <<
                " boundary faces\n";
    }

    if (finiteness == regina::NBoolSet::sTrue)
        descStream << "Finite only\n";
    else if (finiteness == regina::NBoolSet::sFalse)
        descStream << "Ideal only\n";
    else
        descStream << "Finite and ideal\n";

    if (orientability == regina::NBoolSet::sTrue)
        descStream << "Orientable only\n";
    else if (orientability == regina::NBoolSet::sFalse)
        descStream << "Non-orientable only\n";
    else
        descStream << "Orientable and non-orientable\n";

    if (minimalPrimeP2)
        descStream << "Ignored obviously non-minimal, non-prime, "
            << "disc-reducible and/or P2-reducible triangulations\n";
    else if (minimalPrime)
        descStream << "Ignored obviously non-minimal, non-prime and/or "
            << "disc-reducible triangulations\n";
    else if (minimal)
        descStream << "Ignored obviously non-minimal triangulations\n";

    desc->setText(descStream.str());
    return desc;
}

int main(int argc, const char* argv[]) {
    // Set up the command-line arguments.
    int argBdry = 0;
    int argNoBdry = 0;
    int argOr = 0;
    int argNor = 0;
    int argFinite = 0;
    int argIdeal = 0;
    int genPairs = 0;
    poptOption opts[] = {
        { "tetrahedra", 't', POPT_ARG_LONG, &nTet, 0,
            "Number of tetrahedra.", "<tetrahedra>" },
        { "boundary", 'b', POPT_ARG_NONE, &argBdry, 0,
            "Must have at least one boundary face.", 0 },
        { "internal", 'i', POPT_ARG_NONE, &argNoBdry, 0,
            "Must have all faces internal (no boundary faces).", 0 },
        { "bdryfaces", 'B', POPT_ARG_LONG, &nBdryFaces, 0,
            "Must have fixed number (>= 1) of boundary faces.", "<faces>" },
        { "orientable", 'o', POPT_ARG_NONE, &argOr, 0,
            "Must be orientable.", 0 },
        { "nonorientable", 'n', POPT_ARG_NONE, &argNor, 0,
            "Must be non-orientable.", 0 },
        { "finite", 'f', POPT_ARG_NONE, &argFinite, 0,
            "Must be finite (no ideal vertices).", 0 },
        { "ideal", 'd', POPT_ARG_NONE, &argIdeal, 0,
            "Must have at least one ideal vertex.", 0 },
        { "minimal", 'm', POPT_ARG_NONE, &minimal, 0,
            "Ignore obviously non-minimal triangulations.", 0 },
        { "minprime", 'M', POPT_ARG_NONE, &minimalPrime, 0,
            "Ignore obviously non-minimal, non-prime and/or disc-reducible triangulations.", 0 },
        { "minprimep2", 'N', POPT_ARG_NONE, &minimalPrimeP2, 0,
            "Ignore obviously non-minimal, non-prime, disc-reducible and/or P2-reducible triangulations.", 0 },
        { "genpairs", 'p', POPT_ARG_NONE, &genPairs, 0,
            "Only generate face pairings, not triangulations.", 0 },
        { "usepairs", 'P', POPT_ARG_NONE, &usePairs, 0,
            "Only use face pairings read from standard input.", 0 },
        POPT_AUTOHELP
        { 0, 0, 0, 0, 0, 0, 0 }
    };

    poptContext optCon = poptGetContext(0, argc, argv, opts, 0);
    poptSetOtherOptionHelp(optCon, "<output-file>");

    // Parse the command-line arguments.
    int rc = poptGetNextOpt(optCon);
    if (rc != -1) {
        std::cerr << poptBadOption(optCon, POPT_BADOPTION_NOALIAS)
            << ": " << poptStrerror(rc) << "\n\n";
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return 1;
    }

    std::string outFile;
    const char** otherOpts = poptGetArgs(optCon);
    if (otherOpts && otherOpts[0]) {
        outFile = otherOpts[0];
        if (otherOpts[1]) {
            std::cerr << "Too many arguments.\n\n";
            poptPrintHelp(optCon, stderr, 0);
            poptFreeContext(optCon);
            return 1;
        }
    }

    // Run a sanity check on the command-line arguments.
    bool broken = false;
    if ((! genPairs) && outFile.empty()) {
        std::cerr << "An output file must be specified.\n";
        broken = true;
    } else if (genPairs && (argOr || argNor)) {
        std::cerr << "Orientability options cannot be used with "
            << "-p/--genpairs.\n";
        broken = true;
    } else if (genPairs && (argFinite || argIdeal)) {
        std::cerr << "Finiteness options cannot be used with -p/--genpairs.\n";
        broken = true;
    } else if (genPairs && (minimal || minimalPrime || minimalPrimeP2)) {
        std::cerr << "Minimality options cannot be used with -p/--genpairs.\n";
        broken = true;
    } else if (usePairs && nTet) {
        std::cerr << "Tetrahedron options cannot be used with -P/--usepairs.\n";
        broken = true;
    } else if (usePairs && (argBdry || argNoBdry || (nBdryFaces != -1))) {
        std::cerr << "Boundary options cannot be used with -P/--usepairs.\n";
        broken = true;
    } else if ((! usePairs) && nTet == 0) {
        std::cerr << "The number of tetrahedra must be specified using "
            << "option -t/--tetrahedra.\n";
        broken = true;
    } else if ((! usePairs) && (nTet < 1 || nTet > MAXTET)) {
        std::cerr << "The number of tetrahedra must be between 1 and "
            << MAXTET << " inclusive.\n";
        broken = true;
    } else if (argBdry && argNoBdry) {
        std::cerr << "Options -b/--boundary and -i/--internal "
            << "cannot be used together.\n";
        broken = true;
    } else if (argOr && argNor) {
        std::cerr << "Options -o/--orientable and -n/--nonorientable "
            << "cannot be used together.\n";
        broken = true;
    } else if (argFinite && argIdeal) {
        std::cerr << "Options -f/--finite and -d/--ideal "
            << "cannot be used together.\n";
        broken = true;
    } else if (genPairs && usePairs) {
        std::cerr << "Options -p/--genpairs and -P/--usepairs "
            << "cannot be used together.\n";
        broken = true;
    }

    if ((! broken) && (nBdryFaces != -1)) {
        if (nBdryFaces < 0) {
            std::cerr << "Number of boundary faces cannot be negative.\n";
            broken = true;
        } else if (nBdryFaces == 0) {
            // Asking for no boundary faces.
            if (argBdry) {
                std::cerr << "Option -b/--boundary cannot be used with "
                    << "-B/--bdryfaces=0.\n";
                broken = true;
            } else
                argNoBdry = 1;
        } else if (nBdryFaces % 2 != 0) {
            std::cerr << "Number of boundary faces must be even.\n";
            broken = true;
        } else if (nBdryFaces > 2 * nTet + 2) {
            std::cerr << "Number of boundary faces for " << nTet
                << (nTet == 1 ? " tetrahedron" : " tetrahedra")
                << " can be at most " << (2 * nTet + 2) << ".\n";
            broken = true;
        } else {
            // Asking for a valid positive number of boundary faces.
            if (argNoBdry) {
                std::cerr << "Option -i/--internal cannot be used with "
                    << "-B/--bdryfaces=<non-zero>.\n";
                broken = true;
            } else
                argBdry = 1;
        }
    }

    if (broken) {
        std::cerr << '\n';
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return 1;
    }

    // Done parsing the command line.
    poptFreeContext(optCon);

    // Finalise the census parameters.
    finiteness = regina::NBoolSet(! argIdeal, ! argFinite);
    orientability = regina::NBoolSet(! argNor, ! argOr);
    boundary = regina::NBoolSet(! argNoBdry, ! argBdry);

    // Are we only dumping face pairings?
    if (genPairs) {
        if (! outFile.empty()) {
            dumpStream.reset(new std::ofstream(outFile.c_str()));
            if ((! dumpStream.get()) || (! *dumpStream)) {
                std::cerr << "Could not write to file " << outFile << ".\n";
                return 1;
            }
        }

        regina::NFacePairing::findAllPairings(nTet, boundary, nBdryFaces,
            dumpPairing, 0, false);
        std::cerr << "Total face pairings: " << totPairings << std::endl;
        return 0;
    }

    // We're actually generating triangulations.

    // Build the packet tree.
    regina::NContainer parent;
    if (usePairs)
        parent.setPacketLabel("Partial command-line census");
    else
        parent.setPacketLabel("Command-line census");

    regina::NText* desc = parameterPacket();

    regina::NContainer* census = new regina::NContainer();
    census->setPacketLabel("Triangulations");

    parent.insertChildLast(desc);
    parent.insertChildLast(census);

    // Start the census running.
    int whichPurge;
    if (minimalPrimeP2)
        whichPurge = regina::NCensus::PURGE_NON_MINIMAL_PRIME |
            regina::NCensus::PURGE_P2_REDUCIBLE;
    else if (minimalPrime)
        whichPurge = regina::NCensus::PURGE_NON_MINIMAL_PRIME;
    else if (minimal)
        whichPurge = regina::NCensus::PURGE_NON_MINIMAL;
    else
        whichPurge = 0;

    if (usePairs) {
        // Only use the face pairings read from standard input.
        std::cout << "Trying face pairings..." << std::endl;
        std::string pairingList("Face pairings:\n\n");

        std::string pairingRep;
        regina::NFacePairing* pairing;
        while (true) {
            std::getline(std::cin, pairingRep);

            if (pairingRep.length() > 0) {
                pairing = regina::NFacePairing::fromTextRep(pairingRep);
                if (! pairing) {
                    std::cerr << "Invalid face pairing: " << pairingRep
                        << std::endl;
                    pairingList += "INVALID: ";
                    pairingList += pairingRep;
                    pairingList += '\n';
                } else if (! pairing->isCanonical()) {
                    std::cerr << "Non-canonical face pairing: " << pairingRep
                        << std::endl;
                    pairingList += "NON-CANONICAL: ";
                    pairingList += pairingRep;
                    pairingList += '\n';
                } else {
                    std::cout << pairing->toString() << std::endl;
                    regina::NCensus::formPartialCensus(pairing, census,
                        finiteness, orientability, whichPurge,
                        ((minimal || minimalPrime || minimalPrimeP2) ?
                        regina::NCensus::mightBeMinimal : 0), 0);

                    pairingList += pairing->toString();
                    pairingList += '\n';
                }
            }

            if (std::cin.eof())
                break;
        }

        // Store the face pairings used with the census.
        regina::NText* pairingPacket = new regina::NText(pairingList);
        pairingPacket->setPacketLabel("Face Pairings");
        parent.insertChildAfter(pairingPacket, desc);
    } else {
        // An ordinary all-face-pairings census.
        std::cout << "Progress reports are periodic." << std::endl;
        std::cout << "Not all face pairings used will be reported."
            << std::endl;

        regina::NProgressManager manager;
        regina::NCensus::formCensus(census, nTet, finiteness,
            orientability, boundary, nBdryFaces, whichPurge,
            ((minimal || minimalPrime || minimalPrimeP2) ?
            regina::NCensus::mightBeMinimal : 0), 0, &manager);

        // Output progress and wait for the census to finish.
        while (! manager.isStarted())
            sleep(SLEEP_SECONDS);

        const regina::NProgress* progress = manager.getProgress();
        while (! manager.isFinished()) {
            if (progress->hasChanged())
                std::cout << progress->getDescription() << std::endl;
            sleep(SLEEP_SECONDS);
        }
        std::cout << progress->getDescription() << std::endl;
    }

    // Write the completed census to file.
    if (! regina::writeXMLFile(outFile.c_str(), &parent)) {
        std::cerr << "Output file " << outFile << " could not be written.\n";
        return 1;
    }

    std::cout << "Total triangulations: " << census->getNumberOfChildren()
        << std::endl;
    return 0;
}

