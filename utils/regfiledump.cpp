
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Dump packets from the given file to standard output                   *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
#include <list>
#include "file/nxmlfile.h"
#include "packet/npacket.h"

using regina::NPacket;

typedef std::list<std::string> StringList;

void usage(const char* progName, const std::string& error = std::string()) {
    if (! error.empty())
        std::cerr << error << "\n\n";

    std::cerr << "Usage:\n";
    std::cerr << "    " << progName << " [ -f | -l | -n ] [ -c ]"
        << " <file> [ <packet-label> ... ]\n";
    std::cerr << std::endl;
    std::cerr << "    -f : Display full packet details (default)\n";
    std::cerr << "    -l : Only display packet labels and types\n";
    std::cerr << "    -n : Don't display packets at all (implies -c)\n";
    std::cerr << std::endl;
    std::cerr << "    -c : Finish with a count of all packets in the file\n";
    std::cerr << std::endl;
    std::cerr << "    <packet-label> ... : Only display the listed packets"
        << " (otherwise all\n";
    std::cerr << "                         packets are displayed)\n";
    exit(1);
}

static const char* separator =
    "************************************************************";

void dumpNoPacket(std::ostream& out, const std::string& packetLabel,
        char dumpOpt) {
    if (dumpOpt == 'l')
        out << "ERROR: " << packetLabel << " -- No such packet.\n";
    else if (dumpOpt == 'f') {
        out << separator << '\n';
        out << "*\n";
        out << "* ERROR: " << packetLabel << '\n';
        out << "*        No such packet.\n";
        out << "*\n";
        out << separator << '\n';
        out << '\n';
    }
}

void dumpPacketHeader(std::ostream& out, NPacket* p) {
    out << separator << '\n';
    out << "*\n";
    out << "* Label: " << p->getPacketLabel() << '\n';
    out << "* Type: " << p->getPacketTypeName() << '\n';
    out << "* Parent: " << (p->getTreeParent() ?
        p->getTreeParent()->getPacketLabel() : "(none)") << '\n';
    if (p->hasTags()) {
        out << "* Tags: ";
        const std::set<std::string>& tags = p->getTags();
        for (std::set<std::string>::const_iterator it = tags.begin();
                it != tags.end(); it++) {
            if (it != tags.begin())
                out << ", ";
            out << *it;
        }
        out << '\n';
    }
    out << "*\n";
    out << separator << '\n';
}

void dumpPacket(std::ostream& out, NPacket* p, char dumpOpt) {
    if (dumpOpt == 'l')
        out << p->getFullName() << '\n';
    else if (dumpOpt == 'f') {
        dumpPacketHeader(out, p);
        out << '\n';
        out << p->toStringLong() << '\n';
        out << '\n';
    }
}

int main(int argc, char* argv[]) {
    std::string file;
    StringList packets;
    char dumpOpt = 0;
    bool count = false;

    // Parse command line.
    char optChar;
    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '-') {
            // Option.
            if (! argv[i][1])
                usage(argv[0], std::string("Invalid option: ") + argv[i]);
            else if (argv[i][2])
                usage(argv[0], std::string("Invalid option: ") + argv[i]);

            // Argument has length 2.
            optChar = argv[i][1];
            if (optChar == 'c')
                count = true;
            else if (optChar == 'f' || optChar == 'l' || optChar == 'n') {
                if (dumpOpt)
                    usage(argv[0],
                        "More than one detail level has been specified.");
                else
                    dumpOpt = optChar;
            } else
                usage(argv[0], std::string("Invalid option: ") + argv[i]);
        } else if (*argv[i]) {
            // Filename or packet label.
            if (file.empty())
                file = argv[i];
            else
                packets.push_back(argv[i]);
        } else
            usage(argv[0], "Empty arguments are not allowed.");
    }

    if (file.empty())
        usage(argv[0], "No file has been specified.");

    // Add default options.
    if (! dumpOpt)
        dumpOpt = 'f';
    if (dumpOpt == 'n')
        count = true;

    if (dumpOpt == 'n' && ! packets.empty())
        usage(argv[0],
            "You cannot specify individual packets if packets are not to be displayed.");

    // Do the actual work.
    NPacket* tree = regina::readFileMagic(file);
    if (! tree) {
        std::cerr << "File " << file << " could not be read.\n";
        return 1;
    }

    if (dumpOpt != 'n') {
        NPacket* p;
        if (packets.empty())
            for (p = tree; p; p = p->nextTreePacket())
                dumpPacket(std::cout, p, dumpOpt);
        else
            for (StringList::const_iterator it = packets.begin();
                    it != packets.end(); it++) {
                p = tree->findPacketLabel(*it);
                if (p)
                    dumpPacket(std::cout, p, dumpOpt);
                else
                    dumpNoPacket(std::cout, *it, dumpOpt);
            }
    }

    if (count) {
        if (dumpOpt != 'n')
            std::cout << '\n';
        std::cout << tree->getTotalTreeSize() << " total packets in file.\n";
    }

    delete tree;
    return 0;
}

