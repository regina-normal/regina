
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
 * Reads all files *.rga in the current directory.
 * For each file we computes the number of normal surfaces for each
 * triangulation and write the results to a CSV file (using space separators)
 * whose filename is based on the original regina data filename.
 *
 * By default we use standard tri-quad coordinates; passing -q will change
 * this to quad coordinates instead.  The output directory must be passed
 * as an additional command-line argument, and this directory must already
 * exist.
 *
 * An MPI-enabled version of this tool also exists (mpi/normal-mpi.cpp).
 */

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

using namespace regina;

/**
 * Global variables.
 */

int quad = 0;
std::string outputDir;

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
 * Parse command-line arguments.
 */
bool parseCmdLineOptions(int argc, const char* argv[]) {
    // Set up the command-line arguments.
    poptOption opts[] = {
        { "quad", 'q', POPT_ARG_NONE, &quad, 0,
            "Compute surfaces in quad coordinates, not standard coordinates.",
            0 },
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
 * Helps scandir() identify regina data files.
 */
int isRga(const struct dirent* entry) {
   int len = strlen(entry->d_name);
   return (len > 4 && strcmp(".rga", entry->d_name + len - 4) == 0);
}

/**
 * Main routine for dealing with a single data file.
 */
bool process(const std::string& filename) {
    Packet* tree = open(filename);
    if (! tree)
        return false;

    std::ofstream out((outputDir + "/" + filename + ".dat").c_str());
    if (! out) {
        delete tree;
        return false;
    }

    Triangulation<3>* t;
    NormalSurfaces* s;
    for (Packet* p = tree; p; p = p->nextTreePacket())
        if (p->type() == PACKET_TRIANGULATION3) {
            t = static_cast<Triangulation<3>*>(p);
            s = NormalSurfaces::enumerate(t,
                (quad ? NS_QUAD : NS_STANDARD));
            out << t->size() << ' ' << s->size() << " \""
                << t->label() << '"' << std::endl;

            delete s;
        }

    delete tree;
    return true;
}

/**
 * Main routine that loops through all data files.
 */
int main(int argc, char* argv[]) {
    // Extract command-line options.
    if (! parseCmdLineOptions(argc, (const char**)argv))
        return 1;

    // Find the list of data files to process.
    struct dirent** entries = 0;

    int nEntries = scandir(".", &entries, &isRga, &alphasort);
    if (nEntries < 0) {
        std::cerr << "Could not read directory listing." << std::endl;
        return 1;
    }

    // Sort the entries in descending order by size.
    DataFile* files = new DataFile[nEntries + 1 /* in case nEntries == 0 */];
    int i;
    for (i = 0; i < nEntries; ++i)
        files[i].init(entries[i]->d_name);
    std::sort(files, files + nEntries);

    // Process the files.
    int retVal = 0;
    for (int currEntry = 0; currEntry < nEntries; ++currEntry)
        if (! process(files[currEntry].filename)) {
            std::cerr << "ERROR: Could not process "
                << files[currEntry].filename << "." << std::endl;
            retVal = 1;
        }

    // We should delete the entries array, but we're exiting anyway...
    delete[] files;
    return retVal;
}

