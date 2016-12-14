
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
 * Reads all files *.sig in the current directory.
 * Each file is passed to a slave, which computes the number of normal
 * surfaces in both quad and standard coordinates for each triangulation
 * and writes the results to a CSV file (using space separators) whose
 * filename is based on the original regina data filename.
 *
 * The output directory must be passed as an additional command-line argument,
 * and this directory must already exist.
 */

#include "mpi.h"

#include <surfaces/normalsurfaces.h>
#include <triangulation/dim3.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include "popt.h"

// MPI constants:
#define TAG_REQUEST_TASK 10
#define TAG_RESULT 20

#define RESULT_ERR_INPUT -1
#define RESULT_ERR_OUTPUT -2

// Time constants:
#define MIN_SEC 60
#define HOUR_SEC (60 * MIN_SEC)
#define DAY_SEC (24 * HOUR_SEC)

// MPI constraints:
#define MAX_FILENAME 250

// Signature constants:
#define MAXSIGLEN 1000

using namespace regina;



/**
 * General globals for both controller and slaves.
 * Includes all command-line options (which are read by all processes).
 */

// Command-line options.

std::string outputDir;



/**
 * Controller-specific globals.
 */

// MPI and housekeeping.
int nSlaves;
int nRunningSlaves = 0;
std::ofstream logger;
const char* logFile = "sig-normal.log";
bool hasError = false;



/**
 * Helper struct that allows the controller to sort files by size.
 */
struct DataFile {
    std::string filename;
    long size;

    DataFile() : size(0) {
    }

    void init(const char* fileToUse) {
        filename = fileToUse;

        // If we can't stat, just pretend the size is zero.
        struct stat info;
        if (stat(fileToUse, &info) == 0)
            size = info.st_size;
        else
            size = 0;
    }

    const bool operator < (const DataFile& rhs) const {
        return (size > rhs.size);
    }
};

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
    poptSetOtherOptionHelp(optCon, "<output_dir>");

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
        outputDir = otherOpts[0];
        if (otherOpts[1]) {
            fprintf(stderr, "Only one output directory may be supplied.\n\n");
            poptPrintHelp(optCon, stderr, 0);
            poptFreeContext(optCon);
            return false;
        }
    } else {
        fprintf(stderr, "No output directory was supplied.\n\n");
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
    char signalStop = 0;
    MPI_Send(&signalStop, 1, MPI_CHAR, slave,
        TAG_REQUEST_TASK, MPI_COMM_WORLD);
}

/**
 * Controller helper routine.
 *
 * Wait for the next running slave to finish a task.  Note that if no
 * slaves are currently processing tasks, this routine will block forever!
 */
int ctrlWaitForSlave() {
    long result;
    MPI_Status status;
    MPI_Recv(&result, 1, MPI_LONG, MPI_ANY_SOURCE, TAG_RESULT,
        MPI_COMM_WORLD, &status);
    nRunningSlaves--;

    int slave = status.MPI_SOURCE;
    ctrlLogStamp() << "Task completed by slave " << slave << "." << std::endl;

    if (result == RESULT_ERR_INPUT) {
        ctrlLogStamp() << "ERROR: Slave input error." << std::endl;
        hasError = true;
    } else if (result == RESULT_ERR_OUTPUT) {
        ctrlLogStamp() << "ERROR: Slave output error." << std::endl;
        hasError = true;
    } else {
        ctrlLogStamp() << "Processed " << result << " triangulation(s)."
            << std::endl;
    }

    return slave;
}

/**
 * Controller helper routine.
 *
 * Farm the given filename out to the next available slave.
 * If all slaves are working then this routine will wait until some
 * slave finishes its current task.
 */
void ctrlFarmTask(const char* filename) {
    int slave;
    if (nRunningSlaves == nSlaves) {
        // We need to wait for somebody to stop first.
        slave = ctrlWaitForSlave();
    } else {
        // It looks like we're in startup mode.
        slave = nRunningSlaves + 1;
    }

    ctrlLogStamp() << "Farmed " << filename << " to slave "
        << slave << "." << std::endl;

    MPI_Send(const_cast<char*>(filename), strlen(filename) + 1,
        MPI_CHAR, slave, TAG_REQUEST_TASK, MPI_COMM_WORLD);

    nRunningSlaves++;
}

/**
 * Controller helper routine.
 *
 * Helps scandir() identify signature lists.
 */
int isSig(const struct dirent* entry) {
   int len = strlen(entry->d_name);
   return (len > 4 && strcmp(".sig", entry->d_name + len - 4) == 0);
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

    // Find the list of data files to process.
    struct dirent** entries = 0;

    int nEntries = scandir(".", &entries, &isSig, &alphasort);
    if (nEntries < 0) {
        fprintf(stderr, "Could not read directory listing.\n", logFile);
        return 1;
    }

    // Sort the entries in descending order by size.
    DataFile* files = new DataFile[nEntries + 1 /* in case nEntries == 0 */];
    int i;
    for (i = 0; i < nEntries; ++i)
        files[i].init(entries[i]->d_name);
    std::sort(files, files + nEntries);

    // Process the files.
    for (int currEntry = 0; currEntry < nEntries; ++currEntry)
        ctrlFarmTask(files[currEntry].filename.c_str());

    // Kill off any slaves that never started working.
    if (nRunningSlaves < nSlaves)
        for (i = nRunningSlaves; i < nSlaves; i++)
            ctrlStopSlave(i + 1);

    // Wait for remaining slaves to finish.
    while (nRunningSlaves > 0)
        ctrlStopSlave(ctrlWaitForSlave());

    ctrlLogStamp() << "Processed " << nEntries << " file(s)." << std::endl;

    // We should delete the entries array, but we're exiting anyway...
    delete[] files;
    return 0;
}

/**
 * Slave helper routine.
 *
 * Return the result of the current task.
 */
void slaveSendResult(long ans) {
    MPI_Send(&ans, 1, MPI_LONG, 0, TAG_RESULT, MPI_COMM_WORLD);
}

/**
 * Main routine for a slave (ranks 1..size).
 */
int mainSlave() {
    char filename[MAX_FILENAME + 1];
    Packet* tree = 0;
    Packet* p;
    Triangulation<3>* t;
    NormalSurfaces* s;
    long done;
    unsigned long nStd, nQuad;

    char signature[MAXSIGLEN];

    // Keep fetching and processing tasks until there are no more.
    MPI_Status status;
    while (true) {
        // Get the next processing task.
        MPI_Recv(filename, MAX_FILENAME + 1, MPI_CHAR, 0,
            TAG_REQUEST_TASK, MPI_COMM_WORLD, &status);
        if (filename[0] == 0) {
            // This slave is closing down.
            break;
        }

        FILE* dat = fopen(filename, "r");
        if (! dat) {
            slaveSendResult(RESULT_ERR_INPUT);
            continue;
        }

        std::ofstream out((outputDir + "/" + filename + ".dat").c_str());
        if (! out) {
            delete tree;
            slaveSendResult(RESULT_ERR_OUTPUT);
            fclose(dat);
            continue;
        }

        done = 0;
        while (true) {
            fscanf(dat, "%s", signature);
            if (feof(dat))
                break;

            t = Triangulation<3>::fromIsoSig(signature);

            s = NormalSurfaces::enumerate(t, NS_QUAD);
            nQuad = s->size();
            delete s;

            s = NormalSurfaces::enumerate(t, NS_STANDARD);
            nStd = s->size();
            delete s;

            delete t;

            out << nQuad << ' ' << nStd << std::endl;
            ++done;
        }
        fclose(dat);

        slaveSendResult(done);
        delete tree;
    }

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

