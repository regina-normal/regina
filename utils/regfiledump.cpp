
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Dump packets from the given file to standard output                   *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <cstdlib>
#include <list>
#include "packet/packet.h"
#include "utilities/i18nutils.h"

using regina::Packet;

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

void dumpPacketHeader(std::ostream& out, Packet* p) {
    out << separator << '\n';
    out << "*\n";
    out << "* Label: " << p->humanLabel() << '\n';
    out << "* Type: " << p->typeName() << '\n';
    out << "* Parent: " << (p->parent() ?
        p->parent()->humanLabel() : "(none)") << '\n';
    if (p->hasTags()) {
        out << "* Tags: ";
        const std::set<std::string>& tags = p->tags();
        for (auto it = tags.begin(); it != tags.end(); it++) {
            if (it != tags.begin())
                out << ", ";
            out << *it;
        }
        out << '\n';
    }
    out << "*\n";
    out << separator << '\n';
}

void dumpPacket(std::ostream& out, Packet* p, char dumpOpt) {
    if (dumpOpt == 'l')
        out << p->fullName() << '\n';
    else if (dumpOpt == 'f') {
        dumpPacketHeader(out, p);
        out << '\n';
        out << p->detail() << '\n';
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

    // Make sure that we write any international characters in an encoding
    // that the user can read.
    regina::i18n::IConvStream out(std::cout,
        "UTF-8", regina::i18n::Locale::codeset());

    // Do the actual work.
    Packet* tree = regina::open(file.c_str());
    if (! tree) {
        std::cerr << "File " << file << " could not be read.\n";
        return 1;
    }

    if (dumpOpt != 'n') {
        Packet* p;
        if (packets.empty())
            for (p = tree; p; p = p->nextTreePacket())
                dumpPacket(out, p, dumpOpt);
        else
            for (StringList::const_iterator it = packets.begin();
                    it != packets.end(); it++) {
                p = tree->findPacketLabel(*it);
                if (p)
                    dumpPacket(out, p, dumpOpt);
                else
                    dumpNoPacket(out, *it, dumpOpt);
            }
    }

    if (count) {
        if (dumpOpt != 'n')
            out << '\n';
        out << tree->totalTreeSize() << " total packets in file.\n";
    }

    delete tree;
    return 0;
}

