
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Analyse invariants of 3-manifolds in a census                         *
 *                                                                        *
 *  Copyright (c) 2005-2006, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

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
 * If the option -v is passed, the program will write the label of each
 * container as it is processed.
 */

#include <file/nxmlfile.h>
#include <maths/numbertheory.h>
#include <packet/ncontainer.h>
#include <triangulation/ntriangulation.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#define MAX_TV_MAX_R 20
#define MAX_TV_PARAM_COUNT (MAX_TV_MAX_R * (MAX_TV_MAX_R - 1) / 2)

using namespace regina;

const unsigned tvMaxR = 8;
unsigned tvParams[MAX_TV_PARAM_COUNT][2];
unsigned tvParamCount;

unsigned totMfds = 0;
unsigned totMfdsInconsistent = 0;
unsigned totMfdsDuplicate = 0;

bool verbose = false;
NPacket* tree;

struct InvData;

std::vector<InvData*> manifolds;

struct InvData {
    NContainer* manifold;

    std::string h1;
    unsigned long h2z2;
    double* turaevViro;

    InvData(NTriangulation* tri) : manifold(0) {
        h1 = tri->getHomologyH1().toString();
        h2z2 = tri->getHomologyH2Z2();

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
            if (turaevViro[i] < other.turaevViro[i]) return false;
            if (turaevViro[i] > other.turaevViro[i]) return true;
        }

        return false;
    }

    static bool close(double x, double y) {
        return (x < y + 0.001 && x > y - 0.001);
    }
};

inline bool cmpInvData(const InvData* x, const InvData* y) {
    return (*x < *y);
}

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " [ -v ] <file.rga>\n";
    std::cerr << std::endl;
    std::cerr << "    -v : Verbose output (show progress)\n";
    std::cerr << std::endl;
    std::cerr << "Results are written to standard output.\n";
    std::cerr << "Statistics and diagnostic messages are written to standard error.\n";
    exit(1);
}

void initTVParams() {
    tvParamCount = 0;

    unsigned r, root;
    for (r = 3; r <= tvMaxR; r++)
        for (root = 1; root < r; root++)
            if (gcd(r, root) == 1) {
                tvParams[tvParamCount][0] = r;
                tvParams[tvParamCount][1] = root;
                tvParamCount++;
            }
}

void process(NContainer* c) {
    InvData* mfdData = 0;
    std::string mfdDataName;
    bool mfdInconsistent = false;

    InvData* triData;

    for (NPacket* child = c->getFirstTreeChild(); child;
            child = child->getNextTreeSibling()) {
        if (child->getPacketType() != NTriangulation::packetType)
            continue;

        triData = new InvData(static_cast<NTriangulation*>(child));

        if (! mfdData) {
            mfdData = triData;
            mfdDataName = child->getPacketLabel();
            mfdData->manifold = c;
            manifolds.push_back(mfdData);
        } else {
            if (! triData->mayBeEqual(*mfdData)) {
                std::cout << "INCONSISTENCY: " << c->getPacketLabel() << '\n';
                std::cout << "    a) " << mfdDataName << '\n';
                std::cout << "    b) " << child->getPacketLabel() << '\n';

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
                std::cout << "    - " << prev->manifold->getPacketLabel()
                    << '\n';
                totMfdsDuplicate++;
            }

            std::cout << "    - " << (*it)->manifold->getPacketLabel() << '\n';
            totMfdsDuplicate++;
        } else
            first = 0;

        prev = *it;
    }
}

int main(int argc, char* argv[]) {
    // Command-line parsing.
    char optChar;
    int i;
    for (i = 1; i < argc && *argv[i] == '-'; i++) {
        if (! argv[i][1])
            usage(argv[0], std::string("Invalid option: ") + argv[i]);
        if (argv[i][2])
            usage(argv[0], std::string("Invalid option: ") + argv[i]);

        // The argument has length precisely 2.
        optChar = argv[i][1];

        if (optChar == '-') {
            i++;
            break;
        } else if (optChar == 'v')
            verbose = true;
        else
            usage(argv[0], std::string("Invalid option: ") + argv[i]);
    }

    // argv[i] is the first filename.
    if (i != argc - 1)
        usage(argv[0], "Precisely one data file must be given.");

    // Set up the list of Turaev-Viro parameters to try.
    initTVParams();

    // Read the data file.
    if (! (tree = readXMLFile(argv[i]))) {
        std::cerr << "ERROR: Could not read data from " << argv[i] << '.'
            << std::endl;
        return 1;
    }

    // Process the packets.
    for (NPacket* p = tree; p; p = p->nextTreePacket())
        if (p->getPacketType() == NContainer::packetType) {
            if (verbose)
                std::cout << "... " << p->getPacketLabel() << " ...\n";
            process(static_cast<NContainer*>(p));
        }

    findDuplicates();

    // Write statistics and clean up.
    delete tree;

    std::cerr << std::endl;
    std::cerr << "Final statistics:" << std::endl;
    std::cerr << "    3-manifolds examined: " << totMfds << std::endl;
    std::cerr << "    Inconsistencies:      " << totMfdsInconsistent
        << std::endl;
    std::cerr << "    Possible duplicates:  " << totMfdsDuplicate << std::endl;

    return 0;
}

