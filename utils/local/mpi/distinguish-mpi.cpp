
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Analyse invariants of 3-manifolds in a census                         *
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
 * TODO.
 *
 * The given topology data file is read.
 *
 * It is assumed that triangulations in the data file are grouped into
 * containers, with each container representing different triangulations
 * of the same 3-manifold.
 *
 * This program first examines invariants of the various triangulations
 * within each container, and reports an inconsistencies that are found
 * (meaning that triangulations that were thought to represent the same
 * 3-manifold in fact do not).
 *
 * Following this, the program reports different containers that appear
 * to share the same sets of invariants (meaning that the two different
 * containers might in fact represent the same 3-manifold).
 *
 * Since Turaev-Viro invariants may be slow to calculate, the option -t
 * can be used to alter the number of Turaev-Viro invariants that are used.
 *
 * If the option -v is passed, the program will write the label of each
 * container as it is processed.
 */

#include <maths/numbertheory.h>
#include <packet/container.h>
#include <triangulation/dim3.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include "mpi.h"
#include "popt.h"

// Turaev-Viro constraints:
#define MAX_TV_MAX_R 30
#define MAX_TV_PARAM_COUNT (MAX_TV_MAX_R * (MAX_TV_MAX_R - 1) / 2)

#define DEFAULT_TV_MAX_R 10
#define DEFAULT_TV_MAX_R_STR "10"

#define DEFAULT_TV_MAX_R_SELF 7
#define DEFAULT_TV_MAX_R_SELF_STR "7"

#define TV_UNKNOWN -10.0

// MPI constants:
#define TAG_REQUEST_TASK 10
#define TAG_CHANGE_TRI 11
#define TAG_RESULT 20

#define TV_SIGNAL_STOP -1
#define TV_SIGNAL_CHANGE_TRI -2

const long signalStop[2] = { TV_SIGNAL_STOP, 0 };
const long signalChangeTri[2] = { TV_SIGNAL_CHANGE_TRI, 0 };

#define RESULT_ERR_BAD_R -1.0
#define RESULT_ERR_BAD_ROOT -2.0
#define RESULT_ERR_NO_TRI -3.0

// Time constants:
#define MIN_SEC 60
#define HOUR_SEC (60 * MIN_SEC)
#define DAY_SEC (24 * HOUR_SEC)

// MPI constraints:
#define MAX_SLAVES 100
#define MAX_TRI_LABEL_LEN 250

using namespace regina;



/**
 * General globals for both controller and slaves.
 * Includes all command-line options (which are read by all processes).
 */

// Command-line options.
int tvMaxR = DEFAULT_TV_MAX_R;
int tvMaxRSelf = DEFAULT_TV_MAX_R_SELF;
int firstOnly = 0;
std::string filename;

// The input packet tree.
Packet* tree;



/**
 * Controller-specific globals.
 */

// MPI and housekeeping.
int nSlaves;
int nRunningSlaves = 0;
std::ofstream logger;
const char* logFile = "duplicates.log";
bool hasError = false;

// The set of Turaev-Viro invariants to calculate for each triangulation.
long tvParams[MAX_TV_PARAM_COUNT][2];
unsigned tvParamCount;

// Final statistics.
unsigned totMfds = 0;
unsigned totMfdsInconsistent = 0;
unsigned totMfdsDuplicate = 0;

// Calculated invariants for each manifold.
struct InvData;
std::vector<InvData*> manifolds;

// The current working data for each slave.
Triangulation<3>* slaveWorkingTri[MAX_SLAVES + 1];
InvData* slaveWorkingData[MAX_SLAVES + 1];
int slaveWorkingTV[MAX_SLAVES + 1];



/**
 * Generic helper routine.
 *
 * Parse command-line arguments.
 */
bool parseCmdLineOptions(int argc, const char* argv[]) {
    // Set up the command-line arguments.
    poptOption opts[] = {
        { "tvmax", 't', POPT_ARG_INT, &tvMaxR, 0,
            "Maximum r for Turaev-Viro invariants (default is "
            DEFAULT_TV_MAX_R_STR ").", "<max_r>" },
        { "tvmaxself", 's', POPT_ARG_INT, &tvMaxRSelf, 0,
            "Maximum r for Turaev-Viro invariants that are calculated "
            "directly by the controller (default is "
            DEFAULT_TV_MAX_R_SELF_STR ").", "<max_r_self>" },
        { "first", 'f', POPT_ARG_NONE, &firstOnly, 0,
            "Only examine the first triangulation for each manifold.", 0 },
        POPT_AUTOHELP
        { 0, 0, 0, 0, 0, 0, 0 }
    };

    poptContext optCon = poptGetContext(0, argc, argv, opts, 0);
    poptSetOtherOptionHelp(optCon, "<file.rga>");

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
        filename = otherOpts[0];
        if (otherOpts[1]) {
            fprintf(stderr, "Only one filename may be supplied.\n\n");
            poptPrintHelp(optCon, stderr, 0);
            poptFreeContext(optCon);
            return false;
        }
    } else {
        fprintf(stderr, "No filename was supplied.\n\n");
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return false;
    }

    // Sanity checking.
    bool broken = false;
    if (tvMaxR < 3) {
        fprintf(stderr,
            "The maximum r for Turaev-Viro invariants must be at least 3.\n");
        broken = true;
    } else if (tvMaxR > MAX_TV_MAX_R) {
        fprintf(stderr,
            "The maximum r for Turaev-Viro invariants may be at most %d.\n",
            MAX_TV_MAX_R);
        broken = true;
    }

    if (tvMaxR <= tvMaxRSelf) {
        fprintf(stderr,
            "The maximum r for Turaev-Viro invariants (%d) should be "
            "greater than the maximum r calculated directly by the "
            "controller (%d); otherwise slave processes will never be used.\n",
            tvMaxR, tvMaxRSelf);
        broken = true;
    }

    if (broken) {
        fprintf(stderr, "\n");
        poptPrintHelp(optCon, stderr, 0);
    }

    // All done!
    poptFreeContext(optCon);
    return (! broken);
}

/**
 * Generic helper routine.
 *
 * Verify that the input packet tree is suitable for processing.
 */
bool checkInputTree() {
    std::set<std::string> allLabels;
    unsigned labelLen;

    for (Packet* p = tree; p; p = p->nextTreePacket()) {
        labelLen = p->label().length();
        if (labelLen == 0) {
            fprintf(stderr, "ERROR: Empty packet label found in input file.\n");
            return false;
        } else if (labelLen > MAX_TRI_LABEL_LEN) {
            fprintf(stderr, "ERROR: Overlong packet label [%s] found in "
                "input file.\n",
                p->label().c_str());
            return false;
        } else if (! allLabels.insert(p->label()).second) {
            fprintf(stderr, "ERROR: Duplicate packet label [%s] found in "
                "input file.\n",
                p->label().c_str());
            return false;
        }
    }

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
 * Initialise the set of Turaev-Viro parameters to be calculated for
 * each triangulation.
 */
void ctrlInitTVParams() {
    tvParamCount = 0;

    int r, root;
    for (r = 3; r <= tvMaxR; r++)
        for (root = 1; root < r; root++)
            if (gcd(r, root) == 1) {
                tvParams[tvParamCount][0] = r;
                tvParams[tvParamCount][1] = root;
                tvParamCount++;
            }
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
 * Controller helper structure.
 *
 * Contains a set of invariants for a 3-manifold triangulation.
 */
struct InvData {
    Container* manifold;

    std::string h1;
    unsigned long h2z2;
    double* turaevViro;

    bool inconsistent;

    InvData(Container* useManifold) : manifold(useManifold),
            turaevViro(new double[tvParamCount]), inconsistent(false) {
    }

    ~InvData() {
        delete[] turaevViro;
    }

    bool mayBeEqual(const InvData& other) const {
        if (h1 != other.h1) return false;
        if (h2z2 != other.h2z2) return false;

        for (unsigned i = 0; i < tvParamCount; i++)
            if (! close(turaevViro[i], other.turaevViro[i])) return false;

        return true;
    }

    bool operator < (const InvData& other) const {
        if (h1 < other.h1) return false;
        if (h1 > other.h1) return true;
        if (h2z2 < other.h2z2) return false;
        if (h2z2 > other.h2z2) return true;

        for (unsigned i = 0; i < tvParamCount; i++) {
            if (close(turaevViro[i], other.turaevViro[i])) continue;
            if (turaevViro[i] < other.turaevViro[i]) return false;
            if (turaevViro[i] > other.turaevViro[i]) return true;
        }

        return false;
    }

    static inline bool close(double x, double y) {
        return (x < y + 0.001 && x > y - 0.001);
    }
};

/**
 * Controller helper routine.
 *
 * Compare two sets of invariants according to their natural ordering.
 */
inline bool cmpInvData(const InvData* x, const InvData* y) {
    return (*x < *y);
}

/**
 * Controller helper routine.
 *
 * Called when different triangulations of the same manifold are
 * discovered to have different invariants.
 */
void ctrlInconsistent(InvData* data, Triangulation<3>* tri, const char* inv) {
    printf("INCONSISTENCY: %s\n", data->manifold->label().c_str());
    printf("    Invariant: %s\n", inv);
    printf("    Triangulation: %s\n", tri->label().c_str());

    if (! data->inconsistent) {
        totMfdsInconsistent++;
        data->inconsistent = true;
    }
}

/**
 * Controller helper routine.
 *
 * Called when different triangulations of the same manifold are
 * discovered to have different Turaev-Viro invariants.
 */
void ctrlInconsistentTV(InvData* data, Triangulation<3>* tri, int whichTV) {
    printf("INCONSISTENCY: %s\n", data->manifold->label().c_str());
    printf("    Invariant: Turaev-Viro(%ld, %ld)\n",
        tvParams[whichTV][0], tvParams[whichTV][1]);
    printf("    Triangulation: %s\n", tri->label().c_str());

    if (! data->inconsistent) {
        totMfdsInconsistent++;
        data->inconsistent = true;
    }
}

/**
 * Controller helper routine.
 *
 * Close down the given slave.
 */
void ctrlStopSlave(int slave) {
    MPI_Send(const_cast<long*>(signalStop), 2, MPI_LONG, slave,
        TAG_REQUEST_TASK, MPI_COMM_WORLD);
}

/**
 * Controller helper routine.
 *
 * Wait for the next running slave to finish a task.  Note that if no
 * slaves are currently processing tasks, this routine will block forever!
 */
int ctrlWaitForSlave() {
    double result;
    MPI_Status status;
    MPI_Recv(&result, 1, MPI_DOUBLE, MPI_ANY_SOURCE, TAG_RESULT,
        MPI_COMM_WORLD, &status);
    nRunningSlaves--;

    int slave = status.MPI_SOURCE;
    ctrlLogStamp() << "Task completed by slave " << slave << "." << std::endl;

    if (InvData::close(result, RESULT_ERR_BAD_R)) {
        ctrlLogStamp() << "ERROR: Turaev-Viro parameter r = "
            << tvParams[slaveWorkingTV[slave]] << " out of range." << std::endl;
        hasError = true;
    } else if (InvData::close(result, RESULT_ERR_BAD_ROOT)) {
        ctrlLogStamp() << "ERROR: Turaev-Viro parameter root = "
            << tvParams[slaveWorkingTV[slave]] << " out of range." << std::endl;
        hasError = true;
    } else if (InvData::close(result, RESULT_ERR_NO_TRI)) {
        ctrlLogStamp() << "ERROR: Slave could not locate the requested "
            "triangulation." << std::endl;
        hasError = true;
    } else {
        double& ans = slaveWorkingData[slave]->
            turaevViro[slaveWorkingTV[slave]];
        if (InvData::close(ans, TV_UNKNOWN))
            ans = result;
        else if (! InvData::close(ans, result))
            ctrlInconsistentTV(slaveWorkingData[slave],
                slaveWorkingTri[slave], slaveWorkingTV[slave]);
    }

    return slave;
}

/**
 * Controller helper routine.
 *
 * Farm the given Turaev-Viro calculation out to the next available slave.
 * If all slaves are working then this routine will wait until some
 * slave finishes its current task.
 */
void ctrlFarmTask(Triangulation<3>* tri, InvData* data, int whichTV) {
    int slave;
    if (nRunningSlaves == nSlaves) {
        // We need to wait for somebody to stop first.
        slave = ctrlWaitForSlave();
    } else {
        // It looks like we're in startup mode.
        slave = nRunningSlaves + 1;
    }

    if (slaveWorkingTri[slave] != tri) {
        MPI_Send(const_cast<long*>(signalChangeTri), 2, MPI_LONG, slave,
            TAG_REQUEST_TASK, MPI_COMM_WORLD);
        MPI_Send(const_cast<char*>(tri->label().c_str()),
            tri->label().length() + 1, MPI_CHAR, slave,
            TAG_CHANGE_TRI, MPI_COMM_WORLD);

        slaveWorkingTri[slave] = tri;
        slaveWorkingData[slave] = data;
    }

    ctrlLogStamp() << "Farmed TV(" << tvParams[whichTV][0] << ", "
        << tvParams[whichTV][1] << ") to slave " << slave << "." << std::endl;

    MPI_Send(tvParams[whichTV], 2, MPI_LONG, slave, TAG_REQUEST_TASK,
        MPI_COMM_WORLD);

    slaveWorkingTV[slave] = whichTV;
    nRunningSlaves++;
}

/**
 * Controller helper routine.
 *
 * Process a single manifold container (and specifically, all of its
 * triangulation children).
 */
void ctrlProcess(Container* c) {
    InvData* mfdData = 0;
    Triangulation<3>* tri;
    int i;

    for (Packet* child = c->firstChild(); child;
            child = child->nextSibling()) {
        if (child->type() != PACKET_TRIANGULATION3)
            continue;
        tri = static_cast<Triangulation<3>*>(child);

        ctrlLogStamp() << "Processing triangulation: "
            << tri->label() << std::endl;

        if (! mfdData) {
            mfdData = new InvData(c);
            manifolds.push_back(mfdData);

            for (i = tvParamCount - 1; i >= 0; i--)
                if (tvParams[i][0] <= tvMaxRSelf)
                    mfdData->turaevViro[i] = tri->turaevViro(
                        tvParams[i][0], tvParams[i][1]);
                else {
                    mfdData->turaevViro[i] = TV_UNKNOWN;
                    ctrlFarmTask(tri, mfdData, i);
                }

            mfdData->h1 = tri->homology().str();
            mfdData->h2z2 = tri->homologyH2Z2();
        } else {
            for (i = tvParamCount - 1; i >= 0; i--)
                if (tvParams[i][0] <= tvMaxRSelf) {
                    if (! InvData::close(mfdData->turaevViro[i],
                            tri->turaevViro(tvParams[i][0], tvParams[i][1])))
                        ctrlInconsistentTV(mfdData, tri, i);
                } else
                    ctrlFarmTask(tri, mfdData, i);

            if (mfdData->h1 != tri->homology().str())
                ctrlInconsistent(mfdData, tri, "H1(M)");
            if (mfdData->h2z2 != tri->homologyH2Z2())
                ctrlInconsistent(mfdData, tri, "H2(M ; Z_2)");
        }

        if (firstOnly)
            break;
    }

    if (mfdData)
        totMfds++;
}

/**
 * Controller helper routine.
 *
 * Sort the manifolds according to their invariant sets, and locate
 * manifolds with the same invariants.
 */
void ctrlFindDuplicates() {
    std::sort(manifolds.begin(), manifolds.end(), cmpInvData);

    std::vector<InvData*>::iterator it;
    InvData* prev = 0;
    InvData* first = 0;
    for (it = manifolds.begin(); it != manifolds.end(); it++) {
        if (prev && prev->mayBeEqual(**it)) {
            // Duplicate!
            if (! first) {
                first = prev;
                printf("POSSIBLE DUPLICATES:\n");
                printf("    - %s\n", prev->manifold->label().c_str());
                totMfdsDuplicate++;
            }

            printf("    - %s\n", (*it)->manifold->label().c_str());
            totMfdsDuplicate++;
        } else
            first = 0;

        prev = *it;
    }
}

/**
 * Main routine for the controller.
 */
int mainController() {
    // Set up the list of Turaev-Viro parameters to try.
    ctrlInitTVParams();

    // Start logging.
    logger.open(logFile);
    if (! logger) {
        fprintf(stderr, "Could not open %s for writing.\n", logFile);
        return 1;
    }

    // Initialise slave tasks.
    for (int i = 1; i <= nSlaves; i++) {
        slaveWorkingTri[i] = 0;
        slaveWorkingData[i] = 0;
        slaveWorkingTV[i] = 0;
    }

    // Process the packets.
    for (Packet* p = tree; p; p = p->nextTreePacket())
        if (p->type() == PACKET_CONTAINER) {
            ctrlLogStamp() << "Processing container: " << p->label()
                << std::endl;
            ctrlProcess(static_cast<Container*>(p));
        }

    // Kill off any slaves that never started working.
    if (nRunningSlaves < nSlaves)
        for (int i = nRunningSlaves; i < nSlaves; i++)
            ctrlStopSlave(i + 1);

    // Wait for remaining slaves to finish.
    while (nRunningSlaves > 0)
        ctrlStopSlave(ctrlWaitForSlave());

    // Post-processing.
    ctrlLogStamp() << "Identifying duplicates ..." << std::endl;
    ctrlFindDuplicates();

    // Final statistics.
    printf("Final statistics:\n");
    printf("    3-manifolds examined: %d\n", totMfds);
    printf("    Inconsistencies:      %d\n", totMfdsInconsistent);
    printf("    Possible duplicates:  %d\n", totMfdsDuplicate);

    // Clean up and exit.
    for (std::vector<InvData*>::iterator it = manifolds.begin();
            it != manifolds.end(); it++)
        delete *it;

    if (hasError) {
        ctrlLogStamp() << "ERROR: One or more errors occurred; "
            "read back through the log for details." << std::endl;
        printf("\nERROR: One or more errors occurred.\n");
        printf(  "       Please read through the log file %s for details.\n",
            logFile);
    } else
        ctrlLogStamp() << "All done." << std::endl;

    return 0;
}

/**
 * Slave helper routine.
 *
 * Return the result of the current task.
 */
void slaveSendResult(double ans) {
    MPI_Send(&ans, 1, MPI_DOUBLE, 0, TAG_RESULT, MPI_COMM_WORLD);
}

/**
 * Main routine for a slave (ranks 1..size).
 */
int mainSlave() {
    char triLabel[MAX_TRI_LABEL_LEN + 1];
    long taskParams[2];
    Triangulation<3>* workingTri = 0;

    // Keep fetching and processing tasks until there are no more.
    MPI_Status status;
    while (true) {
        // Get the next processing task.
        MPI_Recv(taskParams, 2, MPI_LONG, 0, TAG_REQUEST_TASK, MPI_COMM_WORLD,
            &status);
        if (taskParams[0] == TV_SIGNAL_STOP) {
            // This slave is closing down.
            break;
        } else if (taskParams[0] == TV_SIGNAL_CHANGE_TRI) {
            // Fetch a new working triangulation.
            MPI_Recv(triLabel, MAX_TRI_LABEL_LEN + 1, MPI_CHAR, 0,
                TAG_CHANGE_TRI, MPI_COMM_WORLD, &status);

            workingTri = dynamic_cast<Triangulation<3>*>(
                tree->findPacketLabel(triLabel));
        } else if (taskParams[0] < 3 || taskParams[0] > tvMaxR) {
            // Value of r is out of range.
            slaveSendResult(RESULT_ERR_BAD_R);
        } else if (taskParams[1] <= 0 || taskParams[1] >= taskParams[0] * 2) {
            // Value of root is out of range.
            slaveSendResult(RESULT_ERR_BAD_ROOT);
        } else if (! workingTri) {
            // The current working triangulation was never located.
            slaveSendResult(RESULT_ERR_NO_TRI);
        } else {
            // We have a Turaev-Viro invariant to calculate.
            // This is where most of the real work lies.
            slaveSendResult(workingTri->turaevViro
                (taskParams[0], taskParams[1]));
        }
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

    // Read the data file.
    if (! (tree = open(filename.c_str()))) {
        fprintf(stderr, "ERROR: Could not read data from %s.\n",
            filename.c_str());
        MPI_Finalize();
        return 1;
    }
    if (! checkInputTree()) {
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
        } else if (size > MAX_SLAVES + 1) {
            fprintf(stderr, "ERROR: At most %d slaves are supported.\n",
                MAX_SLAVES);
            retVal = 1;
        } else {
            nSlaves = size - 1;
            retVal = mainController();
        }
    } else {
        // We're one of many slaves.
        retVal = mainSlave();
    }

    // Clean up.
    delete tree;

    MPI_Finalize();
    return retVal;
}

