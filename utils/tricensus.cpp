
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Form a census of splitting surface signatures of given order          *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "census/ncensus.h"
#include "file/nxmlfile.h"
#include "packet/ncontainer.h"
#include "packet/ntext.h"
#include "progress/nprogressmanager.h"
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <unistd.h>

#define MAXTET 10
#define SLEEP_SECONDS 1

using regina::NBoolSet;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

void usage(const char* progName) {
    cerr << "Usage:\n";
    cerr << "    " << progName << " <output-file>\n\n";
    cerr <<
        "You will be required to give census parameters on standard input.\n";
    exit(1);
}

int getInt(const char* prompt, int min, int max) {
    std::string token;
    int ans;
    while (true) {
        cout << prompt;
        cin >> token;

        if (token.empty()) {
            cerr << "Unexpected end of standard input.\n";
            exit(1);
        }

        ans = atoi(token.c_str());
        if (ans < min || ans > max)
            cout << "Only numerical values between " << min
                << " and " << max << " are accepted.\n";
        else
            return ans;
    }
}

NBoolSet getBoolSet(const char* prompt, const char* keyDescs,
        const char keys[3]) {
    std::string token;
    while (true) {
        cout << prompt << '\n' << keyDescs << ": ";
        cin >> token;

        if (token.empty()) {
            cerr << "Unexpected end of standard input.\n";
            exit(1);
        }

        if (tolower(token[0]) == tolower(keys[0]))
            return NBoolSet::sTrue;
        if (tolower(token[0]) == tolower(keys[1]))
            return NBoolSet::sFalse;
        if (tolower(token[0]) == tolower(keys[2]))
            return NBoolSet::sBoth;

        cout << token << " is not an accepted response.\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2)
        usage(argv[0]);

    // Get the census parameters.
    int nTetrahedra = getInt("Number of tetrahedra: ", 1, MAXTET);
    NBoolSet finiteness = getBoolSet(
        "Finite and/or ideal triangulations?",
        "(f)inite, (i)deal, (a)ll", "fia");
    NBoolSet orientability = getBoolSet(
        "Orientable and/or non-orientable triangulations?",
        "(o)rientable, (n)on-orientable, (a)ll", "ona");
    NBoolSet boundary = getBoolSet(
        "With and/or without boundary faces?",
        "(b)oundary, (n)o-boundary, (a)ll", "bna");
    int nBdryFaces = -1;
    if (boundary.hasTrue())
        nBdryFaces = getInt("Number of boundary faces (-1 for any): ",
            -1, 2 * nTetrahedra + 2);

    // Build the packet tree.
    regina::NContainer parent;
    parent.setPacketLabel("Command-line census");

    regina::NText* desc = new regina::NText();
    desc->setPacketLabel("Parameters");
    std::ostringstream descStream;
    descStream << nTetrahedra <<
        (nTetrahedra == 1 ? " tetrahedron\n" : " tetrahedra\n");
    if (finiteness == NBoolSet::sTrue)
        descStream << "Finite only\n";
    else if (finiteness == NBoolSet::sFalse)
        descStream << "Ideal only\n";
    else
        descStream << "Finite and ideal\n";
    if (orientability == NBoolSet::sTrue)
        descStream << "Orientable only\n";
    else if (orientability == NBoolSet::sFalse)
        descStream << "Non-orientable only\n";
    else
        descStream << "Orientable and non-orientable.\n";
    if (boundary == NBoolSet::sTrue)
        descStream << "Boundary faces only\n";
    else if (boundary == NBoolSet::sFalse)
        descStream << "No boundary faces only\n";
    else
        descStream << "With and without boundary faces.\n";
    if (nBdryFaces >= 0)
        descStream << "Requires precisely " << nBdryFaces <<
            " boundary faces.\n";
    desc->setText(descStream.str());

    regina::NContainer* census = new regina::NContainer();
    census->setPacketLabel("Triangulations");

    parent.insertChildLast(desc);
    parent.insertChildLast(census);

    // Start the census running.
    regina::NProgressManager manager;
    regina::formCensus(census, nTetrahedra, finiteness, orientability,
        boundary, nBdryFaces, &manager);

    // Output progress and wait for the census to finish.
    while (! manager.isStarted())
        sleep(SLEEP_SECONDS);

    const regina::NProgress* progress = manager.getProgress();
    cout << endl;
    while (! manager.isFinished()) {
        if (progress->hasChanged())
            cout << progress->getDescription() << endl;
        sleep(SLEEP_SECONDS);
    }
    cout << progress->getDescription() << endl;

    // Write the completed census to file.
    if (! regina::writeXMLFile(argv[1], &parent)) {
        cerr << "Output file " << argv[1] << " could not be written.\n";
        return 1;
    }

    cout << "Total triangulations: " << census->getNumberOfChildren() << endl;
    return 0;
}

