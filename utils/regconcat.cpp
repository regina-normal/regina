
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Combine several data files into a single larger data file             *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include <cstdlib>
#include <cstring>
#include <list>
#include "file/nxmlfile.h"
#include "packet/ncontainer.h"

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName <<
        " [ -o <output-file> ] <data-file> ...\n";
    std::cerr << std::endl;
    std::cerr << "    -o <output-file> : Write to the given data file "
        "(otherwise standard\n";
    std::cerr << "                       output is used)\n";
    exit(1);
}

int main(int argc, char* argv[]) {
    // Parse the command line.
    std::list<std::string> files;
    std::string outputFile;
    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '-') {
            // Option.

            // Is it an argument we don't understand?
            if (strcmp(argv[i], "-o"))
                usage(argv[0], std::string("Invalid option: ") + argv[i]);

            // Are we missing the actual output filename?
            if (i == argc - 1)
                usage(argv[0], "Option -o is missing an output filename.");

            // Have we already specified an output filename?
            if (! outputFile.empty())
                usage(argv[0],
                    "More than one output filename has been supplied.");

            // Suck in the output filename.
            outputFile = argv[++i];
        } else if (*argv[i]) {
            // Just an ordinary filename.
            files.push_back(argv[i]);
        } else
            usage(argv[0], "Empty arguments are not allowed.");
    }

    if (files.empty())
        usage(argv[0], "No data files have been supplied.");

    // Read the input files one by one.
    regina::NPacket* ans = new regina::NContainer();
    ans->setPacketLabel("Combined Data");

    bool error = false;
    regina::NPacket* data;
    for(std::list<std::string>::const_iterator it = files.begin();
            it != files.end(); it++) {
        data = regina::readFileMagic(*it);
        if (!data) {
            std::cerr << "File " << *it << " could not be read.\n";
            error = true;
            continue;
        }

        ans->insertChildLast(data);
    }

    // Tidy up the final data file and write it.
    ans->makeUniqueLabels(0);
    if (outputFile.empty())
        ans->writeXMLFile(std::cout);
    else if (! regina::writeXMLFile(outputFile.c_str(), ans))
        std::cerr << "File " << outputFile << " could not be written.\n";

    delete ans;
    return (error ? 1 : 0);
}

