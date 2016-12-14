
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Manage a distributed census of triangulations using MPI               *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/**
 * Combines the functions of tricensus-mpi and normal-mpi, but only
 * outputs frequency tables.
 *
 * Command-line options are the same as for tricensus-mpi; however,
 * instead of saving the triangulations, this utility merely:
 *
 * - enumerates normal surfaces (in both standard and quad coordinates);
 * - writes a corresponding frequency table to *.sfreq and *.qfreq.
 *
 * Each frequency table will contain several lines of the form:
 *
 * <size> <count>
 *
 * where precisely <count> triangulations in the corresponding data
 * file have precisely <size> vertex normal surfaces.  Lines will be
 * sorted in ascending order by <size>, and only lines with non-zero
 * <count> will be written.
 *
 * TODO:  Only triangulations with < MAX_FREQ vertex surfaces are
 * supported.  Before this code is used in any real computations, add
 * error checking to handle the case where >= MAX_FREQ vertex surfaces
 * are found.
 *
 * Output filenames follow the conventions of tricensus-mpi (but with
 * different filename extensions); output formats follow the conventions
 * of normal-mpi.
 */

#include <algorithm>
#include <cctype>
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <popt.h>
#include "census/gluingpermsearcher3.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "mpi.h"

// MPI constants.
#define TAG_REQUEST_TASK 10
#define TAG_REQUEST_PAIRING 11
#define TAG_REQUEST_SUBSEARCH 12
#define TAG_RESULT 13

// Time constants:
#define MIN_SEC 60
#define HOUR_SEC (60 * MIN_SEC)
#define DAY_SEC (24 * HOUR_SEC)

// Census parameters.
regina::BoolSet
    finiteness(true, true),
    orientability(true, true);
int minimal = 0;
int minimalPrime = 0;
int minimalPrimeP2 = 0;
int whichPurge = 0;
long depth = 0;
int dryRun = 0;

// Filenames read from the command line.
std::string outputStub;
std::string pairsFile;

// Task identification globals.
// For processing a face pairing: ( pairing ID, -1, text data length )
// For processing a partial search: ( pairing ID, search ID, text data length )
// For end of processing: ( -1, -1, -1 )
long taskID[3] = { 0, 0, 0 };

// Controller-specific globals.
std::ofstream logger;
bool controllerError = false;

struct Task {
    long pairing, subtask;
    time_t start;
};
Task* slaveTask;
int nSlaves, nRunningSlaves;
long totTri;

// Slave-specific globals.
long nSolns;
bool serr = false, qerr = false;

#define MAX_FREQ 100000
long long sfreq[MAX_FREQ];
long long qfreq[MAX_FREQ];

// Forward declarations (so that we can keep routines in the same order
// as they appear in tricensus-mpi.cpp):
void slaveMakeTaskFilename(std::string& result, const char* suffix);

/**
 * Generic helper routine.
 *
 * Parse command-line arguments for census and filename options.
 */
int parseCmdLine(int argc, const char* argv[], bool isController) {
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
            "Ignore obviously non-minimal, non-prime and/or disc-reducible "
            "triangulations.", 0 },
        { "minprimep2", 'N', POPT_ARG_NONE, &minimalPrimeP2, 0,
            "Ignore obviously non-minimal, non-prime, disc-reducible and/or "
            "P2-reducible triangulations.", 0 },
        { "depth", 'D', POPT_ARG_LONG, &depth, 0,
            "Split each face pairing into subsearches at the given depth.",
            "<depth>" },
        { "dryrun", 'x', POPT_ARG_NONE, &dryRun, 0,
            "Have slaves ignore each task and simply report zero "
            "triangulations instead.  This allows a quick overview of the "
            "search space.", 0 },
        POPT_AUTOHELP
        { 0, 0, 0, 0, 0, 0, 0 }
    };

    poptContext optCon = poptGetContext(0, argc, argv, opts, 0);
    poptSetOtherOptionHelp(optCon, "<pairs-file> <output-stub>");

    // Parse the command-line arguments.
    int rc = poptGetNextOpt(optCon);
    if (rc != -1) {
        if (isController) {
            std::cerr << poptBadOption(optCon, POPT_BADOPTION_NOALIAS)
                << ": " << poptStrerror(rc) << "\n\n";
            poptPrintHelp(optCon, stderr, 0);
        }
        poptFreeContext(optCon);
        return 1;
    }

    const char** otherOpts = poptGetArgs(optCon);
    if (otherOpts && otherOpts[0] && otherOpts[1]) {
        pairsFile = otherOpts[0];
        outputStub = otherOpts[1];
        if (otherOpts[2]) {
            if (isController) {
                std::cerr << "Too many arguments.\n\n";
                poptPrintHelp(optCon, stderr, 0);
            }
            poptFreeContext(optCon);
            return 1;
        }
    } else {
        if (isController) {
            std::cerr << "Not enough arguments.\n\n";
            poptPrintHelp(optCon, stderr, 0);
        }
        poptFreeContext(optCon);
        return 1;
    }

    // Run a sanity check on the command-line arguments.
    bool broken = false;
    if (pairsFile.empty()) {
        if (isController)
            std::cerr << "A pairs file must be specified.\n";
        broken = true;
    } else if (outputStub.empty()) {
        if (isController)
            std::cerr << "An output filename stub must be specified.\n";
        broken = true;
    } else if (argOr && argNor) {
        if (isController)
            std::cerr << "Options -o/--orientable and -n/--nonorientable "
                << "cannot be used together.\n";
        broken = true;
    } else if (depth < 0) {
        if (isController)
            std::cerr << "The branching depth must be non-negative.\n";
        broken = true;
    }

    if (broken) {
        if (isController) {
            std::cerr << '\n';
            poptPrintHelp(optCon, stderr, 0);
        }
        poptFreeContext(optCon);
        return 1;
    }

    // Done parsing the command line.
    poptFreeContext(optCon);

    // Finalise the census parameters.
    finiteness = regina::BoolSet(! argIdeal, ! argFinite);
    orientability = regina::BoolSet(! argNor, ! argOr);

    if (minimalPrimeP2)
        whichPurge = regina::GluingPermSearcher<3>::PURGE_NON_MINIMAL_PRIME |
            regina::GluingPermSearcher<3>::PURGE_P2_REDUCIBLE;
    else if (minimalPrime)
        whichPurge = regina::GluingPermSearcher<3>::PURGE_NON_MINIMAL_PRIME;
    else if (minimal)
        whichPurge = regina::GluingPermSearcher<3>::PURGE_NON_MINIMAL;
    else
        whichPurge = 0;

    return 0;
}

/**
 * Generic helper routine.
 *
 * Write the given time in human-readable form to the given output stream.
 */
void writeTime(std::ostream& out, long seconds) {
    bool started = false;
    if (seconds >= DAY_SEC) {
        out << (seconds / DAY_SEC) << " days ";
        seconds = seconds % DAY_SEC;
        started = true;
    }
    if (started || seconds >= HOUR_SEC) {
        out << (seconds / HOUR_SEC) << " hrs ";
        seconds = seconds % HOUR_SEC;
        started = true;
    }
    if (started || seconds >= MIN_SEC) {
        out << (seconds / MIN_SEC) << " min ";
        seconds = seconds % MIN_SEC;
        started = true;
    }
    out << seconds << " sec";
}

/**
 * Controller helper routine.
 *
 * Write the current date and time plus whitespace to the log stream and
 * return a reference to the log stream.
 */
std::ostream& ctrlLogStamp() {
    time_t t = time(0);
    std::string time = asctime(localtime(&t));
    // Remove the trailing newline.
    if (time[time.length() - 1] == '\n')
        time.resize(time.length() - 1);
    return logger << time << "  ";
}

/**
 * Controller helper routine.
 *
 * Return the next face pairing representation from the given input stream,
 * or an empty string if end-of-stream is reached.
 */
std::string ctrlNextPairing(std::istream& input) {
    std::string pairing;
    while (true) {
        std::getline(input, pairing);

        if (pairing.length() > 0) {
            // We have a real pairing.
            return pairing;
        }

        // Is the input stream exhausted?
        if (input.eof())
            return std::string();
    }
}

/**
 * Controller helper routine.
 *
 * Return the next slave available for processing work.
 */
int ctrlWaitForSlave(bool runningSlavesOnly = false) {
    if ((! runningSlavesOnly) && (nRunningSlaves < nSlaves)) {
        // We can use a slave that hasn't been started yet.
        // It's probably number (nRunningSlaves + 1).
        if (slaveTask[nRunningSlaves + 1].pairing < 0)
            return nRunningSlaves + 1;

        // Sigh.  Better go look.
        for (int slave = 1; slave <= nSlaves; slave++)
            if (slaveTask[slave].pairing < 0)
                return slave;

        // Strange.
        // Shouldn't get here ever.
        std::cerr << "ERROR: Something is seriously wrong." << std::endl;
        std::cerr << "A free slave could not be found." << std::endl;
        controllerError = true;
        return 0;
    }

    // All slaves are currently working.  Wait for the next one to finish.
    long results[3];
    MPI_Status status;
    MPI_Recv(results, 3, MPI_LONG, MPI_ANY_SOURCE, TAG_RESULT, MPI_COMM_WORLD,
        &status);

    int slave = status.MPI_SOURCE;
    time_t totSec = time(0) - slaveTask[slave].start;

    if (results[0] != slaveTask[slave].pairing ||
            results[1] != slaveTask[slave].subtask) {
        ctrlLogStamp() << "ERROR: Mismatched task data for slave "
            << slave << "." << std::endl;
        controllerError = true;
    }

    if (results[2] < 0) {
        ctrlLogStamp() << "ERROR: Unable to run task (see next line)."
            << std::endl;
        controllerError = true;
    }

    ctrlLogStamp() << "Task [" << results[0];
    if (results[1] >= 0)
        logger << '-' << results[1];
    logger << " @ slave " << slave << "]:  ";

    if (results[2] >= 0)
        logger << results[2] << " found";
    else
        logger << "UNABLE TO RUN TASK";

    logger << ", time " << totSec
        << "s (";
    writeTime(logger, totSec);
    logger << ")" << std::endl;

    if (results[2] >= 0)
        totTri += results[2];

    // Tidy up and return our answer.
    slaveTask[slave].pairing = slaveTask[slave].subtask = -1;
    nRunningSlaves--;
    return slave;
}

/**
 * Controller helper routine.
 *
 * Send the given face pairing to the next available slave for processing.
 */
void ctrlFarmPairing(const std::string& pairingRep) {
    taskID[2] = pairingRep.length();

    int slave = ctrlWaitForSlave();

    nRunningSlaves++;
    ctrlLogStamp() << "Farmed pairing " << taskID[0]
        << " --> slave " << slave << " ..." << std::endl;

    slaveTask[slave].pairing = taskID[0];
    slaveTask[slave].subtask = -1;
    slaveTask[slave].start = time(0);

    MPI_Send(taskID, 3, MPI_LONG, slave, TAG_REQUEST_TASK, MPI_COMM_WORLD);
    MPI_Send(const_cast<char*>(pairingRep.c_str()), taskID[2] + 1, MPI_CHAR,
        slave, TAG_REQUEST_PAIRING, MPI_COMM_WORLD);
}

/**
 * Controller helper routine.
 *
 * Send the given partial search to the next available slave for processing.
 */
void ctrlFarmPartialSearch(const regina::GluingPermSearcher<3>* search, void*) {
    if (! search) {
        // That's it for this face pairing.
        ctrlLogStamp() << "Pairing " << taskID[0] << ": Farmed "
            << taskID[1] << " subsearch(es) in total." << std::endl;
        return;
    }

    // We have a real subsearch.
    taskID[1]++;

    std::ostringstream searchRep;
    search->dumpTaggedData(searchRep);

    taskID[2] = searchRep.str().length();

    int slave = ctrlWaitForSlave();

    nRunningSlaves++;
    ctrlLogStamp() << "Farmed subsearch " << taskID[0] << '-' << taskID[1]
        << " --> slave " << slave << " ..." << std::endl;

    slaveTask[slave].pairing = taskID[0];
    slaveTask[slave].subtask = taskID[1];
    slaveTask[slave].start = time(0);

    MPI_Send(taskID, 3, MPI_LONG, slave, TAG_REQUEST_TASK, MPI_COMM_WORLD);
    MPI_Send(const_cast<char*>(searchRep.str().c_str()), taskID[2] + 1,
        MPI_CHAR, slave, TAG_REQUEST_SUBSEARCH, MPI_COMM_WORLD);
}

/**
 * Controller helper routine.
 *
 * Inform the given slave that it has no more tasks to process, and
 * that it may therefore exit.
 */
void ctrlStopSlave(int slave) {
    long msg[3] = { -1, -1, -1 };
    MPI_Send(&msg, 3, MPI_LONG, slave, TAG_REQUEST_TASK, MPI_COMM_WORLD);

    ctrlLogStamp() << "Slave " << slave << " stopped." << std::endl;
}

/**
 * Main routine for the controller.
 */
int mainController() {
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

    // Go do it.
    slaveTask = new Task[nSlaves + 1];
    int slave;
    for (slave = 1; slave <= nSlaves; slave++)
        slaveTask[slave].pairing = slaveTask[slave].subtask = -1;
    nRunningSlaves = 0;
    totTri = 0;

    std::string pairingRep;
    if (depth > 0) {
        // Generate the face pairings and prepare subsearches.
        regina::FacetPairing<3>* pairing;
        regina::GluingPermSearcher<3>* searcher;
        while (! (pairingRep = ctrlNextPairing(input)).empty()) {
            taskID[0]++;
            taskID[1] = 0;

            pairing = regina::FacetPairing<3>::fromTextRep(pairingRep);
            if (! pairing) {
                ctrlLogStamp() << "ERROR: Pairing " << taskID[0]
                    << " is invalid: " << pairingRep << std::endl;
                controllerError = true;
                continue;
            }
            if (! pairing->isCanonical()) {
                ctrlLogStamp() << "ERROR: Pairing " << taskID[0]
                    << " is not canonical: " << pairingRep << std::endl;
                controllerError = true;
                continue;
            }

            searcher = regina::GluingPermSearcher<3>::bestSearcher(
                pairing, 0 /* autos */,
                ! orientability.hasFalse(), ! finiteness.hasFalse(),
                whichPurge, ctrlFarmPartialSearch);
            searcher->runSearch(depth);
            delete searcher;
        }
    } else {
        // Just farm out the face pairing strings.
        while (! (pairingRep = ctrlNextPairing(input)).empty()) {
            taskID[0]++;
            taskID[1] = -1;

            ctrlFarmPairing(pairingRep);
        }
    }

    // Kill off any slaves that aren't working, since there are no more
    // tasks.
    for (slave = 1; slave <= nSlaves; slave++)
        if (slaveTask[slave].pairing < 0)
            ctrlStopSlave(slave);

    // Wait for everyone else to finish their tasks, and stop them also.
    while (nRunningSlaves > 0)
        ctrlStopSlave(ctrlWaitForSlave(true));

    delete[] slaveTask;

    // All done!
    input.close();
    ctrlLogStamp() << "Done: " << taskID[0] << " pairing(s) read" << std::endl;
    ctrlLogStamp() << "Done: " << totTri
        << " triangulation(s) found" << std::endl;
    if (controllerError)
        ctrlLogStamp() << "Done: ONE OR MORE ERRORS OCCURRED!" << std::endl;
    logger.close();
    return 0;
}

/**
 * Slave helper routine.
 *
 * Called each time the slave finds a complete triangulation.
 */
void slaveFoundGluingPerms(const regina::GluingPermSearcher<3>* perms, void*) {
    if (perms) {
        regina::Triangulation<3>* tri = perms->triangulate();

        bool ok = true;
        if (! tri->isValid())
            ok = false;
        else if ((! finiteness.hasFalse()) && tri->isIdeal())
            ok = false;
        else if ((! finiteness.hasTrue()) && (! tri->isIdeal()))
            ok = false;
        else if ((! orientability.hasTrue()) && tri->isOrientable())
            ok = false;
        else if ((minimal || minimalPrime || minimalPrimeP2) &&
                tri->simplifyToLocalMinimum(false))
            ok = false;

        if (ok) {
            nSolns++;

            // Enumerate normal surfaces!
            regina::NormalSurfaces* s;

            s = regina::NormalSurfaces::enumerate(tri, regina::NS_STANDARD);
            ++sfreq[s->size()];

            s = regina::NormalSurfaces::enumerate(tri, regina::NS_QUAD);
            ++qfreq[s->size()];
        }

        // Toss away the triangulation (and any surface lists beneath it).
        delete tri;
    }
}

/**
 * Slave helper routine.
 *
 * Creates a filename whose name is unique to the current task being
 * processed.  This name will include the face pairing ID, as well as the
 * subsearch ID if appropriate.
 *
 * The given suffix will be appended to the filename; generally this
 * will include an extension (such as ".rga").
 */
void slaveMakeTaskFilename(std::string& result, const char* suffix) {
    std::ostringstream s;
    s << outputStub << '_' << taskID[0];
    if (taskID[1] >= 0)
        s << '-' << taskID[1];
    s << suffix;

    result = s.str();
}

/**
 * Slave helper routine.
 *
 * Inform the controller that the current task was successfully completed.
 */
void slaveSendResult(long nTriangulations) {
    long data[3] = { taskID[0], taskID[1], nTriangulations };
    MPI_Send(data, 3, MPI_LONG, 0, TAG_RESULT, MPI_COMM_WORLD);
}

/**
 * Slave helper routine.
 *
 * Signal that a fatal error occurred whilst working on the current task.
 *
 * A message is written to a newly created error file (specific to this
 * task), and the controller is informed also.
 */
void slaveBail(const std::string& error) {
    // Write the error to file.
    std::string errFile;
    slaveMakeTaskFilename(errFile, ".err");

    std::ofstream out(errFile.c_str());
    if (out) {
        if (taskID[1] < 0)
            out << "ERROR (pairing " << taskID[0] << "):" << std::endl;
        else
            out << "ERROR (pairing " << taskID[0] << ", subsearch "
                << taskID[1] << "):" << std::endl;
        out << error << std::endl;
    }
    out.close();

    // Inform the controller (by returning a negative number of
    // triangulations).
    slaveSendResult(-1);
}

/**
 * Slave helper routine.
 *
 * Request full data for a face pairing subsearch and process that subsearch.
 *
 * A data file is written if and only if at least one triangulation is
 * found (to reduce clutter for large censuses).
 *
 * The controller is informed of the final number of triangulations.
 */
void slaveProcessPartialSearch() {
    char* searchRep = new char[taskID[2] + 1];

    // Fetch the subsearch data.
    MPI_Status status;
    MPI_Recv(searchRep, taskID[2] + 1, MPI_CHAR, 0,
        TAG_REQUEST_SUBSEARCH, MPI_COMM_WORLD, &status);

    // Construct the subsearch.
    regina::GluingPermSearcher<3>* search;
    {
        std::istringstream s(searchRep);
        search = regina::GluingPermSearcher<3>::readTaggedData(s,
            slaveFoundGluingPerms);
    }

    if (search == 0 || search->inputError()) {
        slaveBail(std::string("Invalid search data:\n") + searchRep);
        return;
    }

    // Run the partial census.
    nSolns = 0;
    serr = qerr = false;
    std::fill(sfreq, sfreq + MAX_FREQ, 0);
    std::fill(qfreq, qfreq + MAX_FREQ, 0);
    if (! dryRun)
        search->runSearch();

    if (nSolns > 0) {
        std::string outFile;

        // Write the frequency tables to file and return our results.
        slaveMakeTaskFilename(outFile, ".sfreq");
        {
            std::ofstream out(outFile.c_str());
            if (out) {
                for (int i = 0; i < MAX_FREQ; ++i)
                    if (sfreq[i])
                        out << i << ' ' << sfreq[i] << std::endl;
            } else
                serr = true;
        }

        slaveMakeTaskFilename(outFile, ".qfreq");
        {
            std::ofstream out(outFile.c_str());
            if (out) {
                for (int i = 0; i < MAX_FREQ; ++i)
                    if (qfreq[i])
                        out << i << ' ' << qfreq[i] << std::endl;
            } else
                qerr = true;
        }

        if (serr)
            slaveBail("Could not open output file for standard frequencies.");
        else if (qerr)
            slaveBail("Could not open output file for quad frequencies.");
        else
            slaveSendResult(nSolns);
    } else {
        // No triangulations.  Just inform the controller.
        slaveSendResult(0);
    }

    delete search;
    delete[] searchRep;
}

/**
 * Slave helper routine.
 *
 * Request full data for a single face pairing and process that face pairing.
 *
 * A data file is written if and only if at least one triangulation is
 * found (to reduce clutter for large censuses).
 *
 * The controller is informed of the final number of triangulations.
 */
void slaveProcessPairing() {
    char* pairingRep = new char[taskID[2] + 1];

    // Fetch the pairing itself.
    MPI_Status status;
    MPI_Recv(pairingRep, taskID[2] + 1, MPI_CHAR, 0,
        TAG_REQUEST_PAIRING, MPI_COMM_WORLD, &status);

    // Parse the face pairing.
    regina::FacetPairing<3>* pairing =
        regina::FacetPairing<3>::fromTextRep(pairingRep);
    if (! pairing) {
        slaveBail(std::string("Invalid face pairing: ") + pairingRep);
        return;
    }
    if (! pairing->isCanonical()) {
        slaveBail(std::string("Non-canonical face pairing: ") + pairingRep);
        return;
    }

    // Run the partial census.
    nSolns = 0;
    serr = qerr = false;
    std::fill(sfreq, sfreq + MAX_FREQ, 0);
    std::fill(qfreq, qfreq + MAX_FREQ, 0);
    if (! dryRun)
        regina::GluingPermSearcher<3>::findAllPerms(pairing, 0,
            ! orientability.hasFalse(), ! finiteness.hasFalse(), whichPurge,
            slaveFoundGluingPerms);

    if (nSolns > 0) {
        std::string outFile;

        // Write the frequency tables to file and return our results.
        slaveMakeTaskFilename(outFile, ".sfreq");
        {
            std::ofstream out(outFile.c_str());
            if (out) {
                for (int i = 0; i < MAX_FREQ; ++i)
                    if (sfreq[i])
                        out << i << ' ' << sfreq[i] << std::endl;
            } else
                serr = true;
        }

        slaveMakeTaskFilename(outFile, ".qfreq");
        {
            std::ofstream out(outFile.c_str());
            if (out) {
                for (int i = 0; i < MAX_FREQ; ++i)
                    if (qfreq[i])
                        out << i << ' ' << qfreq[i] << std::endl;
            } else
                qerr = true;
        }

        if (serr)
            slaveBail("Could not open output file for standard frequencies.");
        else if (qerr)
            slaveBail("Could not open output file for quad frequencies.");
        else
            slaveSendResult(nSolns);
    } else {
        // No triangulations.  Just inform the controller.
        slaveSendResult(0);
    }

    delete[] pairingRep;
}

/**
 * Main routine for a slave (ranks 1..size).
 */
int mainSlave() {
    // Keep fetching and processing tasks until there are no more.
    MPI_Status status;
    while (true) {
        // Get identification data for the new processing task.
        // See the taskID declaration at the top of this file for what
        // each array entry means.
        MPI_Recv(taskID, 3, MPI_LONG, 0, TAG_REQUEST_TASK,
            MPI_COMM_WORLD, &status);

        if (taskID[0] < 0)
            break;

        if (taskID[1] < 0)
            slaveProcessPairing();
        else
            slaveProcessPartialSearch();
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

    std::cerr <<
R"(Warning: The MPI utilities in Regina are deprecated, and will be removed from
Regina in a future release.
If you wish to parallelise the generation of a census, we recommend splitting up
the input pairing files into chunks, and using typical queue systems (such as
PBS) to parallelise.)" << std::endl;

    // Which processor are we?
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Extract census options.
    int retVal = parseCmdLine(argc, (const char**)argv, (rank == 0));

    if (retVal == 0) {
        // No breakage yet.  Controller or slave?
        if (rank == 0) {
            // We're the controller.
            // How many processors in total?
            int size;
            MPI_Comm_size(MPI_COMM_WORLD, &size);

            if (size <= 1) {
                std::cerr << "ERROR: At least two processors are required "
                    "(one controller and one slave).\n";
                retVal = 1;
            } else {
                nSlaves = size - 1;
                retVal = mainController();
            }
        } else {
            // We're one of many slaves.
            retVal = mainSlave();
        }
    }

    MPI_Finalize();
    return retVal;
}

