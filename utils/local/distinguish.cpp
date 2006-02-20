
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
#include <packet/ncontainer.h>
#include <triangulation/ntriangulation.h>

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace regina;

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
    double tv3_1;
    double tv4_1;
    double tv4_3;
    double tv5_2;
    double tv5_3;
    double tv5_4;
    double tv7_2;
    double tv7_3;
    double tv7_4;
    double tv7_5;
    // double tv8_3;

    InvData(NTriangulation* tri) : manifold(0) {
        h1 = tri->getHomologyH1().toString();
        h2z2 = tri->getHomologyH2Z2();
        tv3_1 = tri->turaevViro(3, 1);
        tv4_1 = tri->turaevViro(4, 1);
        tv4_3 = tri->turaevViro(4, 3);
        tv5_2 = tri->turaevViro(5, 2);
        tv5_3 = tri->turaevViro(5, 3);
        tv5_4 = tri->turaevViro(5, 4);
        tv7_2 = tri->turaevViro(7, 2);
        tv7_3 = tri->turaevViro(7, 3);
        tv7_4 = tri->turaevViro(7, 4);
        tv7_5 = tri->turaevViro(7, 5);
        // tv8_3 = tri->turaevViro(8, 3);
    }

    bool mayBeEqual(const InvData& other) const {
        if (h1 != other.h1) return false;
        if (h2z2 != other.h2z2) return false;
        if (! close(tv3_1, other.tv3_1)) return false;
        if (! close(tv4_1, other.tv4_1)) return false;
        if (! close(tv4_3, other.tv4_3)) return false;
        if (! close(tv5_2, other.tv5_2)) return false;
        if (! close(tv5_3, other.tv5_3)) return false;
        if (! close(tv5_4, other.tv5_4)) return false;
        if (! close(tv7_2, other.tv7_2)) return false;
        if (! close(tv7_3, other.tv7_3)) return false;
        if (! close(tv7_4, other.tv7_4)) return false;
        if (! close(tv7_5, other.tv7_5)) return false;
        // if (! close(tv8_3, other.tv8_3)) return false;

        return true;
    }

    bool operator < (const InvData& other) const {
        if (h1 < other.h1) return false;
        if (h1 > other.h1) return true;
        if (h2z2 < other.h2z2) return false;
        if (h2z2 > other.h2z2) return true;
        if (tv3_1 < other.tv3_1) return false;
        if (tv3_1 > other.tv3_1) return true;
        if (tv4_1 < other.tv4_1) return false;
        if (tv4_1 > other.tv4_1) return true;
        if (tv4_3 < other.tv4_3) return false;
        if (tv4_3 > other.tv4_3) return true;
        if (tv5_2 < other.tv5_2) return false;
        if (tv5_2 > other.tv5_2) return true;
        if (tv5_3 < other.tv5_3) return false;
        if (tv5_3 > other.tv5_3) return true;
        if (tv5_4 < other.tv5_4) return false;
        if (tv5_4 > other.tv5_4) return true;
        if (tv7_2 < other.tv7_2) return false;
        if (tv7_2 > other.tv7_2) return true;
        if (tv7_3 < other.tv7_3) return false;
        if (tv7_3 > other.tv7_3) return true;
        if (tv7_4 < other.tv7_4) return false;
        if (tv7_4 > other.tv7_4) return true;
        if (tv7_5 < other.tv7_5) return false;
        if (tv7_5 > other.tv7_5) return true;
        // if (tv8_3 < other.tv8_3) return false;
        // if (tv8_3 > other.tv8_3) return true;

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

