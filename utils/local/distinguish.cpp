
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

#include <cstdlib>
#include <iostream>
#include <vector>
#include "popt.h"

#define MAX_TV_MAX_R 20
#define MAX_TV_PARAM_COUNT (MAX_TV_MAX_R * (MAX_TV_MAX_R - 1) / 2)

#define DEFAULT_TV_MAX_R 7
#define DEFAULT_TV_MAX_R_STR "7"

using namespace regina;

int tvMaxR = DEFAULT_TV_MAX_R;
unsigned tvParams[MAX_TV_PARAM_COUNT][2];
unsigned tvParamCount;

unsigned totMfds = 0;
unsigned totMfdsInconsistent = 0;
unsigned totMfdsDuplicate = 0;

int verbose = 0;
std::string filename;
Packet* tree;

struct InvData;

std::vector<InvData*> manifolds;

struct InvData {
    Container* manifold;

    std::string h1;
    unsigned long h2z2;
    double* turaevViro;

    InvData(Triangulation<3>* tri) : manifold(0) {
        h1 = tri->homology().str();
        h2z2 = tri->homologyH2Z2();

        turaevViro = new double[tvParamCount];
        for (unsigned i = 0; i < tvParamCount; i++)
            turaevViro[i] = tri->turaevViro(tvParams[i][0], tvParams[i][1]);
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

inline bool cmpInvData(const InvData* x, const InvData* y) {
    return (*x < *y);
}

void initTVParams() {
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

void process(Container* c) {
    InvData* mfdData = 0;
    std::string mfdDataName;
    bool mfdInconsistent = false;

    InvData* triData;

    for (Packet* child = c->firstChild(); child;
            child = child->nextSibling()) {
        if (child->type() != PACKET_TRIANGULATION3)
            continue;

        triData = new InvData(static_cast<Triangulation<3>*>(child));

        if (! mfdData) {
            mfdData = triData;
            mfdDataName = child->label();
            mfdData->manifold = c;
            manifolds.push_back(mfdData);
        } else {
            if (! triData->mayBeEqual(*mfdData)) {
                std::cout << "INCONSISTENCY: " << c->label() << '\n';
                std::cout << "    a) " << mfdDataName << '\n';
                std::cout << "    b) " << child->label() << '\n';

                if (! mfdInconsistent) {
                    totMfdsInconsistent++;
                    mfdInconsistent = true;
                }
            }
            delete triData;
        }
    }

    if (mfdData)
        totMfds++;
}

void findDuplicates() {
    std::sort(manifolds.begin(), manifolds.end(), cmpInvData);

    std::vector<InvData*>::iterator it;
    InvData* prev = 0;
    InvData* first = 0;
    for (it = manifolds.begin(); it != manifolds.end(); it++) {
        if (prev && prev->mayBeEqual(**it)) {
            // Duplicate!
            if (! first) {
                first = prev;
                std::cout << "POSSIBLE DUPLICATES:\n";
                std::cout << "    - " << prev->manifold->label()
                    << '\n';
                totMfdsDuplicate++;
            }

            std::cout << "    - " << (*it)->manifold->label() << '\n';
            totMfdsDuplicate++;
        } else
            first = 0;

        prev = *it;
    }
}

bool parseCmdLineOptions(int argc, const char* argv[]) {
    // Set up the command-line arguments.
    poptOption opts[] = {
        { "tvmax", 't', POPT_ARG_INT, &tvMaxR, 0,
            "Maximum r for Turaev-Viro invariants (default is "
            DEFAULT_TV_MAX_R_STR ").", "<max_r>" },
        { "verbose", 'v', POPT_ARG_NONE, &verbose, 0,
            "Show progress.", 0 },
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

    if (broken) {
        fprintf(stderr, "\n");
        poptPrintHelp(optCon, stderr, 0);
    }

    // All done!
    poptFreeContext(optCon);
    return (! broken);
}

int main(int argc, const char* argv[]) {
    // Parse the command-line options.
    if (! parseCmdLineOptions(argc, argv))
        return 1;

    // Set up the list of Turaev-Viro parameters to try.
    initTVParams();

    // Read the data file.
    if (! (tree = open(filename.c_str()))) {
        fprintf(stderr, "ERROR: Could not read data from %s.\n",
            filename.c_str());
        return 1;
    }

    // Process the packets.
    for (Packet* p = tree; p; p = p->nextTreePacket())
        if (p->type() == PACKET_CONTAINER) {
            if (verbose)
                std::cout << "... " << p->label() << " ...\n";
            process(static_cast<Container*>(p));
        }

    findDuplicates();

    // Write statistics and clean up.
    delete tree;

    std::cout << std::endl;
    std::cout << "Final statistics:" << std::endl;
    std::cout << "    3-manifolds examined: " << totMfds << std::endl;
    std::cout << "    Inconsistencies:      " << totMfdsInconsistent
        << std::endl;
    std::cout << "    Possible duplicates:  " << totMfdsDuplicate << std::endl;

    return 0;
}

