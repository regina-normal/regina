
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Dump packets from the given file to standard output                   *
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
#include "packet/npacket.h"
#include <cstdlib>

void usage(const char* progName) {
    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " <file> [ <packet-label> ... ]\n";
    exit(1);
}

static const char* separator =
    "************************************************************";

void dumpNoPacket(std::ostream& out, const char* packetLabel) {
    out << separator << '\n';
    out << "*\n";
    out << "* ERROR: " << packetLabel << '\n';
    out << "*        No such packet.\n";
    out << "*\n";
    out << separator << '\n';
    out << '\n';
}

void dumpPacketHeader(std::ostream& out, NPacket* p) {
    out << separator << '\n';
    out << "*\n";
    out << "* Label: " << p->getPacketLabel() << '\n';
    out << "* Type: " << p->getPacketName() << '\n';
    out << "* Parent: " << (p->getTreeParent() ?
        p->getTreeParent()->getPacketLabel() : "(none)") << '\n';
    out << "*\n";
    out << separator << '\n';
}

void dumpPacket(std::ostream& out, NPacket* p) {
    dumpPacketHeader(out, p);
    out << '\n';
    out << p->toStringLong() << '\n';
    out << '\n';
}

int main(int argc, char* argv[]) {
    if (argc == 1)
        usage(argv[0]);

    NPacket* tree = readFromFile(argv[1]);
    if (! tree) {
        std::cerr << "File " << argv[1] << " could not be read.\n";
        return 1;
    }

    NPacket* p;
    if (argc == 2)
        for (p = tree; p; p = p->nextTreePacket())
            dumpPacket(std::cout, p);
    else
        for (int i = 2; i < argc; i++) {
            p = tree->findPacketLabel(argv[i]);
            if (p)
                dumpPacket(std::cout, p);
            else
                dumpNoPacket(std::cerr, argv[i]);
        }

    delete tree;
    return 0;
}

