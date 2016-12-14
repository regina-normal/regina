
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Attempt to find triangulations related by few elementary moves        *
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
 * This utility is a parallelised version of equiv.cpp, found in the
 * parent directory.  See the notes at the beginning of equiv.cpp for
 * general information on what this utility does.
 *
 * Processes communicate through MPI; the process with rank 0 is the
 * controller and the remainder are slaves.  Each triangulation is
 * farmed out to a slave for processing, where the slave attempts to
 * manipulate it as described in equiv.cpp, producing either a smaller
 * triangulation or equivalent triangulations of the same size.
 *
 * Some things are handled a little differently from equiv.cpp:
 *
 * - Progress reporting and errors are not written to standard error;
 *   instead they are written to the log file equiv.log in the current
 *   directory.  As an exception, any errors that occur during initialisation
 *   (before the log file is opened) are written to standard error.
 *
 * - Known non-minimal triangulations are not listed by name in the
 *   final output.  Instead they are simply removed from the usual
 *   output, and a note for each non-minimal triangulation is written to
 *   the log file.
 *
 * - If a new potentially minimal triangulation is discovered (i.e., one
 *   of the same size as an input triangulation that is not in the input
 *   file), this new triangulation is *not* written to any output files.
 *   This is because slaves do not transmit full triangulation constructions
 *   back to the controller.  Instead a warning is written to the log file,
 *   and a note is made in the final statistics on standard output.
 *
 * As in the non-parallel equiv.cpp, equivalence classes and final statistics
 * are written to standard output, and the results can be optionally saved
 * in a Regina data file by passing -o.
 */

#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <popt.h>

#include "packet/container.h"
#include "triangulation/dim3.h"

#include "mpi.h"

// MPI constants:
#define TAG_REQUEST_TASK 10
#define TAG_RESULT 20
#define TAG_RESULT_DATA 21

// Task result codes:
#define RESULT_OK 1
#define RESULT_NON_MINIMAL 2
#define RESULT_HAS_NEW 3
#define RESULT_ERR 10

// Time constants:
#define MIN_SEC 60
#define HOUR_SEC (60 * MIN_SEC)
#define DAY_SEC (24 * HOUR_SEC)

// MPI constraints:
#define MAX_TRI_LABEL_LEN 250
#define MAX_ERR_MSG_LEN (MAX_TRI_LABEL_LEN + 100)

using namespace regina;



/**
 * General globals for both controller and slaves.
 * Includes all command-line options (which are read by all processes).
 */

// Maximum number of moves of each type to make.
int argUp = 1;
int argAcross = 1;
int argDown = 1;

// The input file.
std::string inFile;

// The output file (if output is required).
const char* outFile = 0;

// The input packet tree.
Packet* tree = 0;

// A set of triangulations that have been found to be homeomorphic to
// each other.  A set of this type forms the solution to a single slave task.
typedef std::set<Triangulation<3>*> TriSet;
TriSet equivs;



/**
 * Controller-specific globals.
 */

// MPI and housekeeping.
int nSlaves;
int nRunningSlaves = 0;
std::ofstream logger;
const char* logFile = "equiv.log";
bool hasError = false;

// The output packet tree.
Packet* newTree = 0;

// A mapping from triangulations to equivalence classes.
typedef std::map<Triangulation<3>*, int> ClassMap;
ClassMap eClass;
int nextClass = 0;

// Overall statistics.
unsigned long nTris = 0;
unsigned long nClasses = 0;
unsigned long nNonMin = 0;
unsigned long nHasNew = 0;



/**
 * Slave-specific globals.
 */

// The original triangulation currently being processed.
Triangulation<3>* orig;

// Facts we have discovered about the original triangulation.
bool nonMin, hasNew;



/**
 * Generic helper routine.
 *
 * Parse command-line arguments for move and filename options.
 */
int parseCmdLine(int argc, const char* argv[]) {
    // Set up the command-line arguments.
    poptOption opts[] = {
        { "up", 'u', POPT_ARG_INT, &argUp, 0,
            "Number of initial 2-3 moves (default is 1).", "<moves>" },
        { "across", 'a', POPT_ARG_INT, &argAcross, 0,
            "Number of subsequent 4-4 moves (default is 1).", "<moves>" },
        { "down", 'd', POPT_ARG_INT, &argDown, 0,
            "Number of final reduction moves before the greedy simplification (default is 1).", "<moves>" },
        { "output", 'o', POPT_ARG_STRING, &outFile, 0,
            "Output equivalence classes (plus new triangulations) to the given file.", "<output-file>" },
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
        return 1;
    }

    const char** otherOpts = poptGetArgs(optCon);
    if (otherOpts && otherOpts[0]) {
        inFile = otherOpts[0];
        if (otherOpts[1]) {
            fprintf(stderr, "Only one input filename may be supplied.\n\n");
            poptPrintHelp(optCon, stderr, 0);
            poptFreeContext(optCon);
            return 1;
        }
    } else {
        fprintf(stderr, "No input filename was supplied.\n\n");
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return 1;
    }

    // Run a sanity check on the command-line arguments.
    bool broken = false;
    if (argUp < 0) {
        fprintf(stderr, "The number of 2-3 moves up may not be negative.\n");
        broken = true;
    }
    if (argAcross < 0) {
        fprintf(stderr, "The number of 4-4 moves across may not be "
            "negative.\n");
        broken = true;
    }
    if (argDown < 0) {
        fprintf(stderr, "The number of reduction moves down may not be "
            "negative.\n");
        broken = true;
    }

    if (broken) {
        fprintf(stderr, "\n");
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return 1;
    }

    // Done parsing the command line.
    poptFreeContext(optCon);
    return 0;
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
 * Stop the given slave.  It is assumed that the given slave is not
 * currently working on any task.
 */
void ctrlStopSlave(int slave) {
    ctrlLogStamp() << "Stopping slave " << slave << "." << std::endl;

    char null = 0;
    MPI_Send(&null, 1, MPI_CHAR, slave, TAG_REQUEST_TASK, MPI_COMM_WORLD);
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
    MPI_Recv(&result, 1, MPI_LONG, MPI_ANY_SOURCE, TAG_RESULT, MPI_COMM_WORLD,
        &status);
    nRunningSlaves--;

    int slave = status.MPI_SOURCE;
    ctrlLogStamp() << "Task completed by slave " << slave << "." << std::endl;

    char triLabel[MAX_TRI_LABEL_LEN + 1];

    if (result == RESULT_OK || result == RESULT_HAS_NEW) {
        if (result == RESULT_HAS_NEW) {
            // The original packet label comes through first so we can
            // write it to the log.  It will come through again shortly
            // as part of the set of equivalent triangulations.
            MPI_Recv(triLabel, MAX_TRI_LABEL_LEN + 1, MPI_CHAR, slave,
                TAG_RESULT_DATA, MPI_COMM_WORLD, &status);
            ctrlLogStamp() << "WARNING: Has unseen equivalent: " << triLabel
                << std::endl;
            nHasNew++;
        }

        equivs.clear();

        Packet* p;
        Triangulation<3>* tri;
        while (1) {
            MPI_Recv(triLabel, MAX_TRI_LABEL_LEN + 1, MPI_CHAR, slave,
                TAG_RESULT_DATA, MPI_COMM_WORLD, &status);
            if (*triLabel == 0)
                break;

            p = tree->findPacketLabel(triLabel);
            if (! p) {
                ctrlLogStamp() << "ERROR: Returned equivalent [" << triLabel
                    << "] not found." << std::endl;
                hasError = true;
            } else {
                tri = dynamic_cast<Triangulation<3>*>(p);
                if (! tri) {
                    ctrlLogStamp() << "ERROR: Returned equivalent [" << triLabel
                        << "] is not a triangulation!" << std::endl;
                    hasError = true;
                } else
                    equivs.insert(tri);
            }
        }
        ctrlLogStamp() << "Resulting set contains "
            << equivs.size() << " equivalent(s)." << std::endl;

        // In equivs we now have a list of all triangulations
        // equivalent to orig.

        // Is this an equivalence class we're already seen?
        TriSet::iterator tit;
        ClassMap::iterator cit, cit2;
        for (tit = equivs.begin(); tit != equivs.end(); tit++) {
            cit = eClass.find(*tit);
            if (cit != eClass.end())
                break;
        }
        if (tit != equivs.end()) {
            // We found an equivalence class.  Insert everything we
            // haven't seen yet, and merge the classes of everything
            // we have.
            int c, cOld;
            c = cit->second;
            for (tit = equivs.begin(); tit != equivs.end(); tit++) {
                cit = eClass.find(*tit);
                if (cit == eClass.end())
                    eClass.insert(std::make_pair(*tit, c));
                else if (cit->second != c) {
                    // Merge the two equivalence classes.
                    cOld = cit->second;
                    for (cit = eClass.begin(); cit != eClass.end(); cit++)
                        if (cit->second == cOld)
                            cit->second = c;
                    nClasses--;
                }
            }
        } else {
            // No such equivalence class.  Insert everything.
            int c = nextClass++;
            for (tit = equivs.begin(); tit != equivs.end(); tit++)
                eClass.insert(std::make_pair(*tit, c));
            nClasses++;
        }
    } else if (result == RESULT_NON_MINIMAL) {
        MPI_Recv(triLabel, MAX_TRI_LABEL_LEN + 1, MPI_CHAR, slave,
            TAG_RESULT_DATA, MPI_COMM_WORLD, &status);
        ctrlLogStamp() << "Non-minimal triangulation: " << triLabel
            << std::endl;

        nNonMin++;
    } else if (result == RESULT_ERR) {
        char errMsg[MAX_ERR_MSG_LEN + 1];
        MPI_Recv(errMsg, MAX_ERR_MSG_LEN + 1, MPI_CHAR, slave,
            TAG_RESULT_DATA, MPI_COMM_WORLD, &status);

        ctrlLogStamp() << "ERROR: " << errMsg << std::endl;
        hasError = true;
    } else {
        ctrlLogStamp() << "ERROR: Unknown result code " << result
            << " received from slave." << std::endl;
        hasError = true;
    }

    return slave;
}

/**
 * Controller helper routine.
 *
 * Farm the given triangulation out to the next available slave.
 * If all slaves are working then this routine will wait until some
 * slave finishes its current task.
 *
 * The given triangulation may be 0, in which case this routine will wait
 * for the next slave to finish its current task, and will then close that
 * slave down.  Note that if the given triangulation is 0 but no slaves
 * are actively working on tasks then this routine will wait forever.
 */
void ctrlFarmTri(Triangulation<3>* tri) {
    int slave;
    if (tri == 0 || nRunningSlaves == nSlaves) {
        // We need to wait for somebody to stop first.
        slave = ctrlWaitForSlave();
    } else {
        // It looks like we're in startup mode, since we have a real
        // triangulation and not all of the slaves are up and working yet.
        //
        // We assume that slaves 1..nRunningSlaves are on their first tasks.
        // Farm this new task out to slave number (nRunningSlaves + 1).
        slave = nRunningSlaves + 1;
    }

    if (tri) {
        ctrlLogStamp() << "Farmed [" << tri->label()
            << "] to slave " << slave << "." << std::endl;

        MPI_Send(const_cast<char*>(tri->label().c_str()),
            tri->label().length() + 1, MPI_CHAR, slave,
            TAG_REQUEST_TASK, MPI_COMM_WORLD);
        nRunningSlaves++;
    } else
        ctrlStopSlave(slave);
}

/**
 * Main routine for the controller.
 */
int mainController() {
    Triangulation<3>* t;

    // Start logging.
    logger.open(logFile);
    if (! logger) {
        fprintf(stderr, "Could not open %s for writing.\n", logFile);
        return 1;
    }

    // Do it.
    for (Packet* p = tree; p; p = p->nextTreePacket())
        if (p->type() == PACKET_TRIANGULATION3) {
            nTris++;
            ctrlFarmTri(static_cast<Triangulation<3>*>(p));
        }

    // Kill off any slaves that never started working, since there are no
    // tasks left to give them.
    if (nRunningSlaves < nSlaves)
        for (int i = nRunningSlaves; i < nSlaves; i++)
            ctrlStopSlave(i + 1);

    // Wait for remaining slaves to finish.
    while (nRunningSlaves > 0)
        ctrlFarmTri(0);

    // Done!
    ctrlLogStamp() << "All slaves finished." << std::endl;

    // Write the summary of results.
    if (nClasses) {
        printf("EQUIVALENCE CLASSES:\n\n");

        if (outFile) {
            newTree = new Container();
            newTree->setLabel("Equivalence Classes");
        }

        int classNum = 1;
        std::string className;
        Container* classCnt = 0;

        ClassMap::iterator cit, cit2;
        int c;
        for (cit = eClass.begin(); cit != eClass.end(); cit++)
            if (cit->second >= 0) {
                // The first triangulation of a new equivalence class.
                c = cit->second;

                std::ostringstream s;
                s << "Class " << classNum << " : " <<
                    cit->first->homology().str();
                className = s.str();
                classNum++;

                printf("%s\n\n", className.c_str());
                if (outFile) {
                    classCnt = new Container();
                    classCnt->setLabel(className);
                    newTree->insertChildLast(classCnt);
                }

                // Find the triangulations in this class, and erase the
                // class as we go.
                for (cit2 = cit; cit2 != eClass.end(); cit2++)
                    if (cit2->second == c) {
                        printf("    %s\n",
                            cit2->first->label().c_str());
                        if (outFile) {
                            t = new Triangulation<3>(*(cit2->first));
                            t->setLabel(cit2->first->label());
                            classCnt->insertChildLast(t);
                        }

                        cit2->second = -1;
                    }

                printf("\n");
            }
    }

    printf("Final statistics:\n");
    printf("    Triangulations read:            %ld\n", nTris);
    printf("    Equivalence classes:            %ld\n", nClasses);
    printf("    Non-minimal triangulations:     %ld\n", nNonMin);
    printf("    Triangulations with new equivs: %ld\n", nHasNew);

    // Are we saving results?
    if (outFile && newTree) {
        ctrlLogStamp() << "Saving results to " << outFile << "." << std::endl;
        newTree->save(outFile);
    } else
        ctrlLogStamp() << "Not saving results." << std::endl;

    // Clean up and exit.
    if (newTree)
        delete newTree;

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
 * Signal that a fatal error occurred whilst working on the current task.
 */
void slaveBail(const std::string& error) {
    long result = RESULT_ERR;
    MPI_Send(&result, 1, MPI_LONG, 0, TAG_RESULT, MPI_COMM_WORLD);

    if (error.length() <= MAX_ERR_MSG_LEN)
        MPI_Send(const_cast<char*>(error.c_str()), error.length() + 1,
            MPI_CHAR, 0, TAG_RESULT_DATA, MPI_COMM_WORLD);
    else
        MPI_Send(const_cast<char*>(error.substr(0, MAX_ERR_MSG_LEN).c_str()),
            MAX_ERR_MSG_LEN + 1, MPI_CHAR, 0, TAG_RESULT_DATA, MPI_COMM_WORLD);
}

/**
 * Slave helper routine.
 *
 * Signal that the triangulation currently being processed is
 * non-minimal, and can be henceforth ignored.
 */
void slaveSendNonMin() {
    long result = RESULT_NON_MINIMAL;
    MPI_Send(&result, 1, MPI_LONG, 0, TAG_RESULT, MPI_COMM_WORLD);

    MPI_Send(const_cast<char*>(orig->label().c_str()),
        orig->label().length() + 1, MPI_CHAR, 0,
        TAG_RESULT_DATA, MPI_COMM_WORLD);
}

/**
 * Slave helper routine.
 *
 * Signal that the triangulation currently being processed has a
 * homeomorphic triangulation of the same size that we haven't seen.
 *
 * The original packet label is returned for logging purposes,
 * followed by a full set of homeomorphic triangulations.
 */
void slaveSendNew() {
    long result = RESULT_HAS_NEW;
    MPI_Send(&result, 1, MPI_LONG, 0, TAG_RESULT, MPI_COMM_WORLD);

    // Send the original packet label for logging purposes, then send
    // the entire set of equivalent triangulations as per normal.
    MPI_Send(const_cast<char*>(orig->label().c_str()),
        orig->label().length() + 1, MPI_CHAR, 0,
        TAG_RESULT_DATA, MPI_COMM_WORLD);

    for (TriSet::iterator tit = equivs.begin(); tit != equivs.end(); tit++)
        MPI_Send(const_cast<char*>((*tit)->label().c_str()),
            (*tit)->label().length() + 1, MPI_CHAR, 0,
            TAG_RESULT_DATA, MPI_COMM_WORLD);

    char null = 0;
    MPI_Send(&null, 1, MPI_CHAR, 0, TAG_RESULT_DATA, MPI_COMM_WORLD);
}

/**
 * Slave helper routine.
 *
 * Signal that processing for this task is complete.  A set of
 * homeomorphic triangulations is returned.
 */
void slaveSendEquivs() {
    long result = RESULT_OK;
    MPI_Send(&result, 1, MPI_LONG, 0, TAG_RESULT, MPI_COMM_WORLD);

    for (TriSet::iterator tit = equivs.begin(); tit != equivs.end(); tit++)
        MPI_Send(const_cast<char*>((*tit)->label().c_str()),
            (*tit)->label().length() + 1, MPI_CHAR, 0,
            TAG_RESULT_DATA, MPI_COMM_WORLD);

    char null = 0;
    MPI_Send(&null, 1, MPI_CHAR, 0, TAG_RESULT_DATA, MPI_COMM_WORLD);
}

/**
 * Slave helper routine.
 *
 * We have a homeomorphic triangulation of the same size as the original.
 */
void slaveSameSize(Triangulation<3>* t) {
    // Hunt for it in the packet tree.
    Triangulation<3>* found = 0;
    for (Packet* p = tree; p; p = p->nextTreePacket())
        if (p->type() == PACKET_TRIANGULATION3)
            if (static_cast<Triangulation<3>*>(p)->isIsomorphicTo(*t).get()) {
                found = static_cast<Triangulation<3>*>(p);
                break;
            }

    if (found == orig)
        return;

    if (! found) {
        hasNew = true;
        return;
    }

    // Yes, it's a set and so the insert should just fail if it's
    // already present.  But this is leading to a small memory leak on
    // my machine which, when multiplied by the _very_ large number of
    // times this is called, has rather unpleasant effects (such as
    // exhausting all swap space).  So let's keep the test here for now.
    if (! equivs.count(found))
        equivs.insert(found);
}

/**
 * Slave helper routine.
 *
 * Do the final greedy simplify and process.
 */
void slaveProcessAlt(Triangulation<3>* t) {
    t->intelligentSimplify();

    if (t->size() < orig->size())
        nonMin = true;
    else if (t->size() == orig->size())
        slaveSameSize(t);
}

/**
 * Slave helper routine.
 *
 * Perform reduction moves.  The given triangulation may be changed.
 */
void slaveTryMovesDown(Triangulation<3>* t, int maxLevels) {
    if (maxLevels == 0) {
        slaveProcessAlt(t);
        return;
    }

    Triangulation<3>* alt;
    unsigned i, j;
    bool found = false;

    for (i = 0; i < t->countEdges(); i++)
        if (t->twoZeroMove(t->edge(i), true, false)) {
            alt = new Triangulation<3>(*t);
            alt->twoZeroMove(alt->edge(i));
            slaveTryMovesDown(alt, maxLevels - 1);
            found = true;
            delete alt;

            if (nonMin)
                return;
        }

    for (i = 0; i < t->countEdges(); i++)
        for (j = 0; j < 2; j++)
            if (t->twoOneMove(t->edge(i), j, true, false)) {
                alt = new Triangulation<3>(*t);
                alt->twoOneMove(alt->edge(i), j);
                slaveTryMovesDown(alt, maxLevels - 1);
                found = true;
                delete alt;

                if (nonMin)
                    return;
            }

    // Only try 3-2 moves if nothing better has worked so far.
    if (! found)
        for (i = 0; i < t->countEdges(); i++)
            if (t->threeTwoMove(t->edge(i), true, false)) {
                alt = new Triangulation<3>(*t);
                alt->threeTwoMove(alt->edge(i));
                slaveTryMovesDown(alt, maxLevels - 1);
                found = true;
                delete alt;

                if (nonMin)
                    return;
            }

    // Only try 4-4 moves if nothing else has worked.
    if (! found)
        for (i = 0; i < t->countEdges(); i++)
            for (j = 0; j < 2; j++)
                if (t->fourFourMove(t->edge(i), j, true, false)) {
                    alt = new Triangulation<3>(*t);
                    alt->fourFourMove(alt->edge(i), j);
                    slaveTryMovesDown(alt, maxLevels - 1);
                    found = true;
                    delete alt;

                    if (nonMin)
                        return;
                }

    // No more moves to make.  Run straight to intelligentSimplify().
    if (! found)
        slaveProcessAlt(t);
}

/**
 * Slave helper routine.
 *
 * Perform 4-4 moves.  The given triangulation may be changed.
 * Moves that revert to prev, prev2 or prev3 will not be considered.
 */
void slaveTryMovesAcross(Triangulation<3>* t, int maxLevels,
        Triangulation<3>* prev = 0, Triangulation<3>* prev2 = 0,
        Triangulation<3>* prev3 = 0) {
    unsigned i, j;
    Triangulation<3>* alt;

    if (maxLevels > 0)
        for (i = 0; i < t->countEdges(); i++)
            for (j = 0; j < 2; j++)
                if (t->fourFourMove(t->edge(i), j, true, false)) {
                    alt = new Triangulation<3>(*t);
                    alt->fourFourMove(alt->edge(i), j);
                    if (prev && alt->isIsomorphicTo(*prev).get()) {
                        // Ignore, reversion.
                    } else if (prev2 && alt->isIsomorphicTo(*prev2).get()) {
                        // Ignore, reversion.
                    } else if (prev3 && alt->isIsomorphicTo(*prev3).get()) {
                        // Ignore, reversion.
                    } else
                        slaveTryMovesAcross(alt, maxLevels - 1, t, prev, prev2);
                    delete alt;

                    if (nonMin)
                        return;
                }

    // Try just going for the simplify.
    slaveTryMovesDown(t, argDown);
}

/**
 * Slave helper routine.
 *
 * Perform 2-3 moves.  The given triangulation will not be changed.
 */
void slaveTryMovesUp(Triangulation<3>* t, int levelsRemaining) {
    Triangulation<3>* alt;

    if (levelsRemaining == 0) {
        // We're not allowed to change the original, so clone it.
        alt = new Triangulation<3>(*t);
        slaveTryMovesAcross(alt, argAcross);
        delete alt;
    } else {
        for (unsigned i = 0; i < t->countTriangles(); i++) {
            alt = new Triangulation<3>(*t);
            if (alt->twoThreeMove(alt->triangle(i))) {
                if (levelsRemaining > 1)
                    slaveTryMovesUp(alt, levelsRemaining - 1);
                else
                    slaveTryMovesAcross(alt, argAcross);
            }
            delete alt;

            if (nonMin)
                return;
        }
    }
}

/**
 * Slave helper routine.
 *
 * Process a single triangulation.
 */
void slaveProcessTri(Triangulation<3>* tri) {
    nonMin = hasNew = false;
    orig = tri;
    equivs.clear();
    equivs.insert(orig);

    slaveTryMovesUp(orig, argUp);

    if (nonMin)
        slaveSendNonMin();
    else if (hasNew)
        slaveSendNew();
    else
        slaveSendEquivs();
}

/**
 * Main routine for a slave (ranks 1..size).
 */
int mainSlave() {
    char triLabel[MAX_TRI_LABEL_LEN + 1];
    Packet* p;
    Triangulation<3>* tri;

    // Keep fetching and processing tasks until there are no more.
    MPI_Status status;
    while (true) {
        // Get the next processing task.
        MPI_Recv(triLabel, MAX_TRI_LABEL_LEN + 1, MPI_CHAR, 0,
            TAG_REQUEST_TASK, MPI_COMM_WORLD, &status);

        if (*triLabel == 0)
            break;

        p = tree->findPacketLabel(triLabel);
        if (! p)
            slaveBail(std::string("Packet ") + triLabel + " not found.");
        else {
            tri = dynamic_cast<Triangulation<3>*>(p);
            if (! tri)
                slaveBail(std::string("Packet ") + triLabel +
                    " is not a triangulation.");
            else
                slaveProcessTri(tri);
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
    if (parseCmdLine(argc, (const char**)argv)) {
        MPI_Finalize();
        return 1;
    }

    // Read the input file.
    if (! (tree = open(inFile.c_str()))) {
        fprintf(stderr, "ERROR: Could not read data from %s.\n",
            inFile.c_str());
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
        } else {
            nSlaves = size - 1;
            retVal = mainController();
        }
    } else {
        // We're one of many slaves.
        retVal = mainSlave();
    }

    // Clean up
    delete tree;

    MPI_Finalize();
    return retVal;
}

