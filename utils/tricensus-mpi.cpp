
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Manage a distributed census of triangulations using MPI               *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <cctype>
#include <fstream>
#include <memory>
#include <sstream>
#include <popt.h>
#include "census/ncensus.h"
#include "file/nxmlfile.h"
#include "packet/ncontainer.h"
#include "packet/ntext.h"
#include "mpi.h"

// MPI constants.
#define TAG_REQUEST_DATA 10
#define TAG_REQUEST_PAIRING 11
#define TAG_RESULT 12

#define MAX_PAIRING_REP_LEN 500

// Census parameters.
regina::NBoolSet
    finiteness(true, true),
    orientability(true, true);
int minimal = 0;
int minimalPrime = 0;
int minimalPrimeP2 = 0;

std::string outputStub;
std::string pairsFile;

bool controllerError = false;

int parseCmdLine(int argc, const char* argv[]) {
    // Set up the command-line arguments.
    int argOr = 0;
    int argNor = 0;
    int argFinite = 0;
    int argIdeal = 0;
    poptOption opts[] = {
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
        POPT_AUTOHELP
        { 0, 0, 0, 0, 0, 0, 0 }
    };

    poptContext optCon = poptGetContext(0, argc, argv, opts, 0);
    poptSetOtherOptionHelp(optCon, "<pairs-file> <output-stub>");

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
    if (otherOpts && otherOpts[0] && otherOpts[1]) {
        pairsFile = otherOpts[0];
        outputStub = otherOpts[1];
        if (otherOpts[2]) {
            std::cerr << "Too many arguments.\n\n";
            poptPrintHelp(optCon, stderr, 0);
            poptFreeContext(optCon);
            return 1;
        }
    } else {
        std::cerr << "Not enough arguments.\n\n";
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return 1;
    }

    // Run a sanity check on the command-line arguments.
    bool broken = false;
    if (pairsFile.empty()) {
        std::cerr << "A pairs file must be specified.\n";
        broken = true;
    } else if (outputStub.empty()) {
        std::cerr << "An output filename stub must be specified.\n";
        broken = true;
    } else if (argOr && argNor) {
        std::cerr << "Options -o/--orientable and -n/--nonorientable "
            << "cannot be used together.\n";
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

    // Finalise the census parameters.
    finiteness = regina::NBoolSet(! argIdeal, ! argFinite);
    orientability = regina::NBoolSet(! argNor, ! argOr);

    return 0;
}

bool nextPairingToSlave(std::istream& input, int slave, long whichPairing) {
    std::string pairingRep;
    regina::NFacePairing* pairing;

    while (true) {
        std::getline(input, pairingRep);

        if (pairingRep.length() > MAX_PAIRING_REP_LEN) {
            std::cout << "ERROR: Ignoring pairing #" << whichPairing
                << " (line too long)" << std::endl;
            controllerError = true;
        } else if (pairingRep.length() > 0) {
            std::cout << "Farming pairing #" << whichPairing
                << " to slave #" << slave << std::endl;
            MPI_Send(&whichPairing, 1, MPI_LONG, slave,
                TAG_REQUEST_DATA, MPI_COMM_WORLD);
            MPI_Send(const_cast<char*>(pairingRep.c_str()),
                pairingRep.length() + 1, MPI_CHAR, slave,
                TAG_REQUEST_PAIRING, MPI_COMM_WORLD);
            return true;
        }

        // Are we all out of pairs?
        if (input.eof())
            return false;
    }

    // Should never reach this point.
    return false;
}

void sendStopToSlave(int slave) {
    long whichPairing = -1;
    MPI_Send(&whichPairing, 1, MPI_LONG, slave, TAG_REQUEST_DATA,
        MPI_COMM_WORLD);
}

int mainController(int nSlaves) {
    // Prepare to read in the face pairings.
    std::ifstream input(pairsFile.c_str());
    if (! input) {
        std::cerr << "Could not open pairs file " << pairsFile
            << " for reading.\n";
        return 1;
    }

    // Start farming out tasks to slaves.
    int nRunningSlaves = 0;
    int i;
    long whichPairing = 0;
    bool noMorePairings = false;
    for (i = 1; i <= nSlaves; i++) {
        if (noMorePairings)
            sendStopToSlave(i);
        else if (nextPairingToSlave(input, i, ++whichPairing))
            nRunningSlaves++;
        else {
            noMorePairings = true;
            sendStopToSlave(i);
        }
    }

    // Wait for each slave to finish and keep farming out as required.
    long results[2];
    long totTri = 0;
    MPI_Status status;
    while (nRunningSlaves > 0) {
        MPI_Recv(results, 2, MPI_LONG, MPI_ANY_SOURCE, TAG_RESULT,
            MPI_COMM_WORLD, &status);
        i = status.MPI_SOURCE;
        std::cout << "#" << results[0] << ": ";
        if (results[1] < 0) {
            std::cout << "ERROR";
            controllerError = true;
        } else {
            std::cout << results[1] << " triangulation(s)";
            totTri += results[1];
        }
        std::cout << " (node #" << i << ")" << std::endl;

        if (! noMorePairings)
            if (! nextPairingToSlave(input, i, ++whichPairing))
                noMorePairings = true;

        if (noMorePairings) {
            sendStopToSlave(i);
            nRunningSlaves--;
        }
    }
    whichPairing--;

    // All done.
    input.close();
    std::cout << "Done:" << std::endl;
    if (controllerError)
        std::cout << "  - ONE OR MORE ERRORS OCCURRED!" << std::endl;
    std::cout << "  - " << whichPairing << " pairing(s) read" << std::endl;
    std::cout << "  - " << totTri << " triangulation(s) found" << std::endl;
    return 0;
}

/**
 * Return a new text packet storing the census parameters.
 */
regina::NText* parameterPacket(const regina::NFacePairing* pairing) {
    regina::NText* desc = new regina::NText();
    desc->setPacketLabel("Parameters");
    std::ostringstream descStream;

    descStream << "Only used a single face pairing:\n"
        << pairing->toString() << '\n';

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

void sendResult(long whichPairing, long result) {
    long data[2] = { whichPairing, result };
    MPI_Send(data, 2, MPI_LONG, 0, TAG_RESULT, MPI_COMM_WORLD);
}

void bail(long whichPairing, const std::string& errFile,
        const std::string& error) {
    std::ofstream out(errFile.c_str());
    if (out)
        out << "ERROR (pairing #" << whichPairing << "):\n"
            << error << std::endl;
    sendResult(whichPairing, -1);
}

int mainSlave() {
    long whichPairing;
    char pairingRep[MAX_PAIRING_REP_LEN + 1];
    MPI_Status status;
    int whichPurge;
    long ans;

    // TODO: time file
    std::string pairStub, outFile, timeFile, errFile;

    while (true) {
        MPI_Recv(&whichPairing, 1, MPI_LONG, 0, TAG_REQUEST_DATA,
            MPI_COMM_WORLD, &status);
        if (whichPairing < 0)
            break;

        MPI_Recv(pairingRep, MAX_PAIRING_REP_LEN + 1, MPI_CHAR, 0,
            TAG_REQUEST_PAIRING, MPI_COMM_WORLD, &status);

        {
            std::ostringstream s;
            s << outputStub << '_' << whichPairing;
            pairStub = s.str();
        }

        outFile = pairStub + ".rga";
        timeFile = pairStub + ".time";
        errFile = pairStub + ".err";

        // Parse the face pairing.
        regina::NFacePairing* pairing =
            regina::NFacePairing::fromTextRep(pairingRep);
        if (! pairing) {
            bail(whichPairing, errFile,
                std::string("Invalid face pairing: ") + pairingRep);
            continue;
        }
        if (! pairing->isCanonical()) {
            bail(whichPairing, errFile,
                std::string("Non-canonical face pairing: ") + pairingRep);
            continue;
        }

        // Prepare the packet tree.
        regina::NContainer parent;
        parent.setPacketLabel("Partial MPI census");

        regina::NText* desc = parameterPacket(pairing);

        regina::NContainer* census = new regina::NContainer();
        census->setPacketLabel("Triangulations");

        parent.insertChildLast(desc);
        parent.insertChildLast(census);

        // Run the partial census.
        if (minimalPrimeP2)
            whichPurge = regina::NCensus::PURGE_NON_MINIMAL_PRIME |
                regina::NCensus::PURGE_P2_REDUCIBLE;
        else if (minimalPrime)
            whichPurge = regina::NCensus::PURGE_NON_MINIMAL_PRIME;
        else if (minimal)
            whichPurge = regina::NCensus::PURGE_NON_MINIMAL;
        else
            whichPurge = 0;

        ans = regina::NCensus::formPartialCensus(pairing, census,
            finiteness, orientability, whichPurge,
            ((minimal || minimalPrime || minimalPrimeP2) ?
            regina::NCensus::mightBeMinimal : 0), 0);

        // Write the completed census to file.
        if (regina::writeXMLFile(outFile.c_str(), &parent))
            sendResult(whichPairing, ans);
        else
            bail(whichPairing, errFile, "Output file cound not be written.");
    }

    return 0;
}

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);

    int retVal = parseCmdLine(argc, (const char**)argv);
    if (retVal == 0) {
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (rank == 0) {
            MPI_Comm_size(MPI_COMM_WORLD, &size);
            if (size <= 1) {
                std::cerr << "ERROR: At least two processors are required.\n";
                retVal = 1;
            } else
                retVal = mainController(size - 1);
        } else {
            retVal = mainSlave();
        }
    }

    MPI_Finalize();
    return retVal;
}

