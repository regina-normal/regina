
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Count vertex normal surfaces for a set of data files                  *
 *                                                                        *
 *  Copyright (c) 2005-2016, Ben Burton                                   *
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
 * Reads the given data file (passed on the command line), and measures
 * the performance of normal surface enumeration for each triangulation
 * therein.
 *
 * For each triangulation, a line is written to the file normal-compare.csv
 * (note that these lines might not be in the same *order* as the
 * corresponding triangulations in the data file).  Each line will contain
 * five integer fields, separated by single spaces.  These fields are:
 *
 * - the number of vertex normal surfaces in standard (tri-quad) coordinates;
 *
 * - the number of vertex normal surfaces in quadrilateral coordinates;
 *
 * - the time taken to enumerate standard vertex surfaces directly, without
 *   going via quad space;
 *
 * - the time taken to enumerate quadrilateral vertex surfaces directly;
 *
 * - the time taken to convert the quadrilateral space solution set to a
 *   standard space solution set.
 *
 * All times are measured in microseconds, as returned by clock().  Any
 * discrepancies between the two standard solution sets will be noted in
 * the log file (this checking only involves counting surfaces, not
 * comparing them coordinate by coordinate).
 *
 * A single-processor, single-triangulation version of this tool also
 * exists (../normal-compare.cpp).
 */

#include <surfaces/normalsurfaces.h>
#include <triangulation/dim3.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include "mpi.h"
#include "popt.h"

// MPI constants:
#define TAG_REQUEST_TASK 10
#define TAG_RESULT 20

#define RESULT_ERR_BADTRI -1
#define RESULT_ERR_BADANS -2

// Time constants:
#define MIN_SEC 60
#define HOUR_SEC (60 * MIN_SEC)
#define DAY_SEC (24 * HOUR_SEC)

using namespace regina;



/**
 * General globals for both controller and slaves.
 * Includes all command-line options (which are read by all processes).
 */

// Command-line options.

std::string dataFile;



/**
 * Controller-specific globals.
 */

// MPI and housekeeping.
int nSlaves;
int nRunningSlaves = 0;
std::ofstream logger;
std::ofstream stats;
const char* logFile = "normal-compare.log";
const char* statsFile = "normal-compare.csv";
bool hasError = false;



/**
 * Generic helper routine.
 *
 * Parse command-line arguments.
 */
bool parseCmdLineOptions(int argc, const char* argv[]) {
    // Set up the command-line arguments.
    poptOption opts[] = {
        POPT_AUTOHELP
        { 0, 0, 0, 0, 0, 0, 0 }
    };

    poptContext optCon = poptGetContext(0, argc, argv, opts, 0);
    poptSetOtherOptionHelp(optCon, "<data_file>");

    // Parse the command-line arguments.
    int rc = poptGetNextOpt(optCon);
    if (rc != -1) {
        fprintf(stderr, "%s: %s\n\n",
            poptBadOption(optCon, POPT_BADOPTION_NOALIAS), poptStrerror(rc));
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return false;
    }

    const char** otherOpts = poptGetArgs(optCon);
    if (otherOpts && otherOpts[0]) {
        dataFile = otherOpts[0];
        if (otherOpts[1]) {
            fprintf(stderr, "Only one output directory may be supplied.\n\n");
            poptPrintHelp(optCon, stderr, 0);
            poptFreeContext(optCon);
            return false;
        }
    } else {
        fprintf(stderr, "No data file was supplied.\n\n");
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return false;
    }

    // All done!
    poptFreeContext(optCon);
    return true;
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
 * Close down the given slave.
 */
void ctrlStopSlave(int slave) {
    ctrlLogStamp() << "Stopping slave " << slave << "." << std::endl;

    long signalStop = -1;
    MPI_Send(&signalStop, 1, MPI_LONG, slave,
        TAG_REQUEST_TASK, MPI_COMM_WORLD);
}

/**
 * Controller helper routine.
 *
 * Wait for the next running slave to finish a task.  Note that if no
 * slaves are currently processing tasks, this routine will block forever!
 */
int ctrlWaitForSlave() {
    long result[5];
    MPI_Status status;
    MPI_Recv(result, 5, MPI_LONG, MPI_ANY_SOURCE, TAG_RESULT,
        MPI_COMM_WORLD, &status);
    nRunningSlaves--;

    int slave = status.MPI_SOURCE;
    ctrlLogStamp() << "Task completed by slave " << slave << "." << std::endl;

    if (result[0] == RESULT_ERR_BADTRI) {
        ctrlLogStamp() << "ERROR: Slave reported bad triangulation number "
            << result[1] << " (last seen was " << result[2] << ")."
            << std::endl;
        hasError = true;
    } else if (result[0] == RESULT_ERR_BADANS) {
        ctrlLogStamp() << "ERROR: Slave reported mismatched surface counts ("
            << result[1] << " != " << result[2] << ")." << std::endl;
        hasError = true;
    } else {
        stats << result[0] << ' ' << result[1] << ' '
            << result[2] << ' ' << result[3] << ' ' << result[4]
            << std::endl;
    }

    return slave;
}

/**
 * Controller helper routine.
 *
 * Farm the given triangulation out to the next available slave.
 * If all slaves are working then this routine will wait until some
 * slave finishes its current task.
 */
void ctrlFarmTask(long whichTri) {
    int slave;
    if (nRunningSlaves == nSlaves) {
        // We need to wait for somebody to stop first.
        slave = ctrlWaitForSlave();
    } else {
        // It looks like we're in startup mode.
        slave = nRunningSlaves + 1;
    }

    ctrlLogStamp() << "Farmed triangulation " << whichTri << " to slave "
        << slave << "." << std::endl;

    MPI_Send(&whichTri, 1, MPI_LONG, slave, TAG_REQUEST_TASK, MPI_COMM_WORLD);

    nRunningSlaves++;
}

/**
 * Main routine for the controller.
 */
int mainController() {
    // Start logging.
    logger.open(logFile);
    if (! logger) {
        fprintf(stderr, "Could not open %s for writing.\n", logFile);
        return 1;
    }

    stats.open(statsFile);
    if (! stats) {
        fprintf(stderr, "Could not open %s for writing.\n", statsFile);
        return 1;
    }

    // Run through the triangulations in the data file and farm them out
    // to slaves in turn.
    Packet* tree = open(dataFile);
    if (! tree) {
        fprintf(stderr, "Could not open data file %s.\n", dataFile.c_str());
        return 1;
    }

    Packet* p = tree;
    while (p && p->type() != PACKET_TRIANGULATION3)
        p = p->nextTreePacket();
    long currTri = 0;

    while (p) {
        ctrlFarmTask(currTri);

        p = p->nextTreePacket();
        while (p && p->type() != PACKET_TRIANGULATION3)
            p = p->nextTreePacket();

        ++currTri;
    }

    // Kill off any slaves that never started working.
    if (nRunningSlaves < nSlaves)
        for (int i = nRunningSlaves; i < nSlaves; i++)
            ctrlStopSlave(i + 1);

    // Wait for remaining slaves to finish.
    while (nRunningSlaves > 0)
        ctrlStopSlave(ctrlWaitForSlave());

    ctrlLogStamp() << "Processed " << currTri << " triangulation(s)."
        << std::endl;

    delete tree;
    return 0;
}

/**
 * Slave helper routine.
 *
 * Return the result of the current task.
 */
void slaveSendResult(long numStd, long numQuad,
        long timeStd, long timeQuad, long timeConv) {
    long ans[5];
    ans[0] = numStd;
    ans[1] = numQuad;
    ans[2] = timeStd;
    ans[3] = timeQuad;
    ans[4] = timeConv;

    MPI_Send(ans, 5, MPI_LONG, 0, TAG_RESULT, MPI_COMM_WORLD);
}

/**
 * Slave helper routine.
 *
 * Return an error.
 */
void slaveSendError(long errorCode, long reason1, long reason2) {
    long ans[5];
    ans[0] = errorCode;
    ans[1] = reason1;
    ans[2] = reason2;
    ans[3] = 0;
    ans[4] = 0;

    MPI_Send(ans, 5, MPI_LONG, 0, TAG_RESULT, MPI_COMM_WORLD);
}

/**
 * Main routine for a slave (ranks 1..size).
 */
int mainSlave() {
    Packet* tree = open(dataFile);
    if (! tree) {
        fprintf(stderr, "Could not open data file %s.\n", dataFile.c_str());
        return 1;
    }

    Packet* p = tree;
    while (p && p->type() != PACKET_TRIANGULATION3)
        p = p->nextTreePacket();
    long currTri = 0;

    Triangulation<3>* t;
    NormalSurfaces* q;
    NormalSurfaces* s;

    long numStd, numQuad;
    long timeStd, timeQuad, timeConv;
    clock_t clockStart;

    // Keep fetching and processing tasks until there are no more.
    MPI_Status status;
    long useTri;
    while (true) {
        // Get the next processing task.
        MPI_Recv(&useTri, 1, MPI_LONG, 0,
            TAG_REQUEST_TASK, MPI_COMM_WORLD, &status);
        if (useTri < 0) {
            // This slave is closing down.
            break;
        }

        if (useTri < currTri) {
            // We shouldn't need to walk backwards through the tree.
            slaveSendError(RESULT_ERR_BADTRI, useTri, currTri);
            continue;
        }

        while (p && currTri < useTri) {
            p = p->nextTreePacket();
            while (p && p->type() != PACKET_TRIANGULATION3)
                p = p->nextTreePacket();

            ++currTri;
        }
        if (! p) {
            slaveSendError(RESULT_ERR_BADTRI, useTri, currTri);
            continue;
        }

        t = static_cast<Triangulation<3>*>(p);
        if ((! t->isValid()) || t->isIdeal()) {
            // We only care about valid triangulations with no ideal vertices.
            slaveSendResult(0, 0, 0, 0, 0);
            continue;
        }

        clockStart = ::clock();
        q = NormalSurfaces::enumerate(t, NS_QUAD, true);
        timeQuad = ::clock() - clockStart;
        numQuad = q->size();

        clockStart = ::clock();
        s = q->quadToStandard();
        timeConv = ::clock() - clockStart;
        numStd = s->size();

        q->makeOrphan(); delete q;
        s->makeOrphan(); delete s;

        clockStart = ::clock();
        s = NormalSurfaces::enumerate(t, NS_STANDARD,
            NS_VERTEX | NS_EMBEDDED_ONLY, NS_VERTEX_STD_DIRECT);
        timeStd = ::clock() - clockStart;
        if (s->size() != numStd) {
            s->makeOrphan(); delete s;
            slaveSendError(RESULT_ERR_BADANS, numStd,
                s->size());
            continue;
        }

        s->makeOrphan(); delete s;
        slaveSendResult(numStd, numQuad, timeStd, timeQuad, timeConv);
    }

    delete tree;
    return 0;
}

/**
 * Main routine for all processors.
 *
 * Parse the command-line arguments for options, then determine whether
 * we are controller or slave and run a specialised main routine accordingly.
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

    // Extract command-line options.
    if (! parseCmdLineOptions(argc, (const char**)argv)) {
        MPI_Finalize();
        return 1;
    }

    // Are things going to break horribly?
    if (sizeof(clock_t) < 8) {
        fprintf(stderr, "ERROR: This program requires a 64-bit "
            "clock_t type.\n");
        MPI_Finalize();
        return 1;
    }

    // Controller or slave?
    int retVal;
    if (rank == 0) {
        // We're the controller.
        // How many processors in total?
        int size;
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        if (size <= 1) {
            fprintf(stderr, "ERROR: At least two processors are required "
                "(one controller and one slave).\n");
            retVal = 1;
        } else {
            nSlaves = size - 1;
            retVal = mainController();
        }
    } else {
        // We're one of many slaves.
        retVal = mainSlave();
    }

    MPI_Finalize();
    return retVal;
}

