
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Convert old-style binary data files to new-style XML                  *
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

#include "file/nfile.h"
#include "file/nxmlfile.h"
#include "packet/npacket.h"
#include <cstdlib>

void usage(const char* progName) {
    std::cerr << "Usage:\n";
    std::cerr << "    " << progName
        << " <old-binary-file> [ <new-XML-file> ]\n";
    std::cerr << "\nThe same filename may be given for both arguments.\n";
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3)
        usage(argv[0]);

    regina::NPacket* tree = regina::readFromFile(argv[1]);
    if (! tree) {
        std::cerr << "Binary file " << argv[1] << " could not be read.\n";
        return 1;
    }

    if (argc == 2)
        tree->writeXMLFile(std::cout);
    else if (! regina::writeXMLFile(argv[2], tree, true)) {
        std::cerr << "XML file " << argv[2] << " could not be written.\n";
        delete tree;
        return 1;
    }

    delete tree;
    return 0;
}

