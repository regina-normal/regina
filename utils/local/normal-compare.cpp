
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
 * Reads the given data file (passed on the command line), and measures the
 * performance of normal surface enumeration for the ith triangulation in
 * the data file (where the index i is also passed on the command line).
 * Triangulation numbering begins at 0.
 *
 * Output will be written to the given output file (again passed on the
 * command line).  The last line of output will contain five integer
 * fields, separated by single spaces.  These fields are:
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
 * discrepancies between the two standard solution sets will be noted on
 * standard error (this checking only involves counting surfaces, not
 * comparing them coordinate by coordinate).
 *
 * An MPI-enabled version of this tool also exists (mpi/normal-compare-mpi.cpp).
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
#include "popt.h"

using namespace regina;



/**
 * General globals for both controller and slaves.
 * Includes all command-line options (which are read by all processes).
 */

// Command-line options.

std::string dataFile;
std::string statsFile;
long useTri;



/**
 * Controller-specific globals.
 */

// MPI and housekeeping.
std::ofstream stats;
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
    poptSetOtherOptionHelp(optCon, "<data_file> <output_file> <tri_num>");

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
    if (! (otherOpts && otherOpts[0])) {
        fprintf(stderr, "No data file was supplied.\n\n");
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return false;
    }
    dataFile = otherOpts[0];

    if (! otherOpts[1]) {
        fprintf(stderr, "No output file file was supplied.\n\n");
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return false;
    }
    statsFile = otherOpts[1];

    if (! otherOpts[2]) {
        fprintf(stderr, "No triangulation number was supplied.\n\n");
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return false;
    }
    // TODO: Check for non-number.
    useTri = atol(otherOpts[2]);

    if (otherOpts[3]) {
        fprintf(stderr, "Too many arguments were supplied.\n\n");
        poptPrintHelp(optCon, stderr, 0);
        poptFreeContext(optCon);
        return false;
    }

    // All done!
    poptFreeContext(optCon);
    return true;
}



/**
 * Main routine.
 */
int main(int argc, char* argv[]) {
    // Extract command-line options.
    if (! parseCmdLineOptions(argc, (const char**)argv))
        return 1;

    // Are things going to break horribly?
    if (sizeof(clock_t) < 8) {
        fprintf(stderr, "ERROR: This program requires a 64-bit "
            "clock_t type.\n");
        return 1;
    }

    freopen(statsFile.c_str(), "w", stdout);

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

    // Find the requested triangulation.
    while (p && currTri < useTri) {
        p = p->nextTreePacket();
        while (p && p->type() != PACKET_TRIANGULATION3)
            p = p->nextTreePacket();

        ++currTri;
    }
    if (! p) {
        fprintf(stderr, "Could not find triangulation %ld.\n", useTri);
        return 1;
    }

    printf("Using triangulation %ld\n", useTri);
    t = static_cast<Triangulation<3>*>(p);
    if ((! t->isValid()) || t->isIdeal()) {
        // We only care about valid triangulations with no ideal vertices.
        printf("0 0 0 0 0");
    } else {
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
        if (s->size() == numStd)
            printf("%ld %ld %ld %ld %ld\n",
                numStd, numQuad, timeStd, timeQuad, timeConv);
        else
            fprintf(stderr, "Mismatched surface counts: %ld vs %ld\n",
                numStd, s->size());

        s->makeOrphan(); delete s;
    }

    delete tree;
    fclose(stdout);
    return 0;
}

