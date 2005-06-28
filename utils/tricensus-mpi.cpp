
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
#include <iostream>
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

// Filenames read from the command line.
std::string outputStub;
std::string pairsFile;

// Generic globals.
long whichPairing = 0;

// Controller-specific globals.
std::ofstream logger;
bool controllerError = false;

// Slave-specific globals.
std::string pairStub;

/**
 * Generic helper routine.
 *
 * Parse command-line arguments for census and filename options.
 */
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

/**
 * Controller helper routine.
 *
 * Read the next face pairing from the given input file.
 * If it exists, send it to the given slave for processing and return true.
 * Otherwise do nothing and return false.
 */
bool ctrlFarmToSlave(std::istream& input, int slave) {
    std::string pairingRep;

    while (true) {
        std::getline(input, pairingRep);

        if (pairingRep.length() > MAX_PAIRING_REP_LEN) {
            // This will cause a buffer overflow in the slave if we try
            // to send it.  Ignore it and note the error.
            whichPairing++;
            logger << "ERROR: Ignoring pairing #" << whichPairing
                << " (line too long)" << std::endl;
            controllerError = true;
        } else if (pairingRep.length() > 0) {
            // We have a real pairing.
            whichPairing++;
            logger << "#" << whichPairing << ": --> slave #" << slave
                << " ..." << std::endl;
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

/**
 * Controller helper routine.
 *
 * Inform the given slave that it has no more pairings to process, and
 * that it may therefore exit.
 */
void ctrlStopSlave(int slave) {
    long msg = -1;
    MPI_Send(&msg, 1, MPI_LONG, slave, TAG_REQUEST_DATA, MPI_COMM_WORLD);
}

/**
 * Main routine for the controller.
 */
int mainController(int nSlaves) {
    // Prepare to read in the face pairings.
    std::ifstream input(pairsFile.c_str());
    if (! input) {
        std::cerr << "Could not open pairs file " << pairsFile
            << " for reading.\n";
        return 1;
    }

    // Start logging.
    std::string logFile = outputStub + ".log";
    logger.open(logFile.c_str());
    if (! logger) {
        std::cerr << "Could not open log file " << logFile << " for writing.\n";
        return 1;
    }

    // Farm out an initial set of tasks to slaves.
    int nRunningSlaves = 0;
    int slave;
    bool noMorePairings = false;

    for (slave = 1; slave <= nSlaves; slave++) {
        if (noMorePairings)
            ctrlStopSlave(slave);
        else if (ctrlFarmToSlave(input, slave))
            nRunningSlaves++;
        else {
            noMorePairings = true;
            ctrlStopSlave(slave);
        }
    }

    // Wait for each slave to finish and keep farming out as required.
    long results[2];
    long totTri = 0;
    MPI_Status status;
    while (nRunningSlaves > 0) {
        // Wait for a slave to return its results.
        MPI_Recv(results, 2, MPI_LONG, MPI_ANY_SOURCE, TAG_RESULT,
            MPI_COMM_WORLD, &status);
        slave = status.MPI_SOURCE;

        logger << "#" << results[0] << ": ";
        if (results[1] < 0) {
            logger << "ERROR" << std::endl;
            controllerError = true;
        } else {
            logger << results[1] << " triangulation(s)" << std::endl;
            totTri += results[1];
        }

        if (! noMorePairings)
            if (! ctrlFarmToSlave(input, slave))
                noMorePairings = true;

        if (noMorePairings) {
            ctrlStopSlave(slave);
            nRunningSlaves--;
        }
    }

    // All done!
    input.close();
    logger << "Done:" << std::endl;
    if (controllerError)
        logger << "  - ONE OR MORE ERRORS OCCURRED!" << std::endl;
    logger << "  - " << whichPairing << " pairing(s) read" << std::endl;
    logger << "  - " << totTri << " triangulation(s) found" << std::endl;
    logger.close();
    return 0;
}

/**
 * Slave helper routine.
 *
 * Construct a skeleton packet tree for the census data file.
 * The census container will be the last child of the parent.
 */
regina::NPacket* slaveSkeletonTree(const regina::NFacePairing* pairing) {
    // Create the overall parent packet.
    regina::NContainer* parent = new regina::NContainer();
    parent->setPacketLabel("Partial MPI census");

    // Create a child packet that describes the census parameters.
    regina::NText* desc = new regina::NText();
    desc->setPacketLabel("Parameters");
    std::ostringstream descStream;

    descStream << "Only used a single face pairing:\n"
        << pairing->toString() << "\n\n";

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
    parent->insertChildLast(desc);

    // Create a child packet that will hold the census triangulations.
    regina::NContainer* census = new regina::NContainer();
    census->setPacketLabel("Triangulations");
    parent->insertChildLast(census);

    // All done!
    return parent;
}

/**
 * Slave helper routine.
 *
 * Inform the controller that the partial census for this face pairing
 * was successfully completed.
 */
void slaveSendResult(long nTriangulations) {
    long data[2] = { whichPairing, nTriangulations };
    MPI_Send(data, 2, MPI_LONG, 0, TAG_RESULT, MPI_COMM_WORLD);
}

/**
 * Slave helper routine.
 *
 * Signal that a fatal error occurred whilst trying to process this
 * particular face pairing.
 *
 * A message is written to a newly created error file (specific to that
 * face pairing), and the controller is informed also.
 */
void slaveBail(const std::string& error) {
    // Write the error to file.
    std::string errFile = pairStub + ".err";
    std::ofstream out(errFile.c_str());
    if (out)
        out << "ERROR (pairing #" << whichPairing << "):\n"
            << error << std::endl;

    // Inform the controller (by returning a negative number of
    // triangulations).
    slaveSendResult(-1);
}

/**
 * Main routine for a slave (ranks 1..size).
 */
int mainSlave() {
    // MPI message passing:
    MPI_Status status;
    char pairingRep[MAX_PAIRING_REP_LEN + 1];

    // Helper variables for the census:
    int whichPurge;
    long ans;

    // Files that we will want to write to:
    std::string outFile, timeFile;

    // Keep fetching and processing pairings until there are no more.
    while (true) {
        // Get the index number of the new pairing.  A negative index
        // number signifies that there are no more.
        MPI_Recv(&whichPairing, 1, MPI_LONG, 0, TAG_REQUEST_DATA,
            MPI_COMM_WORLD, &status);
        if (whichPairing < 0)
            break;

        // Fetch the pairing itself.
        MPI_Recv(pairingRep, MAX_PAIRING_REP_LEN + 1, MPI_CHAR, 0,
            TAG_REQUEST_PAIRING, MPI_COMM_WORLD, &status);

        // All filenames will include the pairing index number, so that
        // people don't clobber each other's files.
        {
            std::ostringstream s;
            s << outputStub << '_' << whichPairing;
            pairStub = s.str();
        }

        outFile = pairStub + ".rga";
        timeFile = pairStub + ".time";

        // Parse the face pairing.
        regina::NFacePairing* pairing =
            regina::NFacePairing::fromTextRep(pairingRep);
        if (! pairing) {
            slaveBail(std::string("Invalid face pairing: ") + pairingRep);
            continue;
        }
        if (! pairing->isCanonical()) {
            slaveBail(std::string("Non-canonical face pairing: ") + pairingRep);
            continue;
        }

        // Run the partial census.
        // TODO: Time file.
        regina::NPacket* parent = slaveSkeletonTree(pairing);

        if (minimalPrimeP2)
            whichPurge = regina::NCensus::PURGE_NON_MINIMAL_PRIME |
                regina::NCensus::PURGE_P2_REDUCIBLE;
        else if (minimalPrime)
            whichPurge = regina::NCensus::PURGE_NON_MINIMAL_PRIME;
        else if (minimal)
            whichPurge = regina::NCensus::PURGE_NON_MINIMAL;
        else
            whichPurge = 0;

        ans = regina::NCensus::formPartialCensus(pairing,
            parent->getLastTreeChild(),
            finiteness, orientability, whichPurge,
            ((minimal || minimalPrime || minimalPrimeP2) ?
            regina::NCensus::mightBeMinimal : 0), 0);

        // Write the completed census to file.
        if (regina::writeXMLFile(outFile.c_str(), parent))
            slaveSendResult(ans);
        else
            slaveBail("Output file could not be written.");

        delete parent;
    }

    return 0;
}

/**
 * Main routine for all processors.
 *
 * Parse the command-line arguments for census options, then determine
 * whether we are controller or slave and run a specialised main routine
 * accordingly.
 */
int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int retVal = parseCmdLine(argc, (const char**)argv);
    if (retVal == 0) {
        // Which processor are we?
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if (rank == 0) {
            // We're the controller.
            // How many processors in total?
            int size;
            MPI_Comm_size(MPI_COMM_WORLD, &size);

            if (size <= 1) {
                std::cerr << "ERROR: At least two processors are required "
                    "(one controller and one slave).\n";
                retVal = 1;
            } else
                retVal = mainController(size - 1);
        } else {
            // We're one of many slaves.
            retVal = mainSlave();
        }
    }

    MPI_Finalize();
    return retVal;
}

