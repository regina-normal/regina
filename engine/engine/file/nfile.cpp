
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
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

#include <string.h>
#include "config.h"
#include "regina.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nfile.h"
    #include "packetregistry.h"
#else
    #include "engine/file/nfile.h"
    #include "engine/packet/packetregistry.h"
#endif

#define TREE_CHILD 'c'
#define TREE_NO_MORE_CHILDREN 'n'

NPacket* readFromFile(const char* fileName) {
    NFile f;
    if (f.open(fileName, f.READ)) {
        NPacket* ans = f.readPacketTree();
        f.close();
        return ans;
    }
    return 0;
}

bool writeToFile(const char* fileName, NPacket* tree) {
    NFile f;
    if (f.open(fileName, f.WRITE)) {
        f.writePacketTree(tree);
        f.close();
        return true;
    }
    return false;
}

bool NFile::open(const char* fileName, mode newOpenMode) {
    if (openMode)
        close();
    // openMode is now set to 0 (CLOSED) and the file is closed.
    if (newOpenMode == READ) {
        #ifdef __BINARY_IO
            infile.open(fileName, MODE_READ | ios::binary);
        #else
            infile.open(fileName, MODE_READ);
        #endif
        if (infile.is_open()) {
            int len = strlen(PROGRAM_NAME);
            char* sentry = new char[len+1];
            for (int i=0; i<len+1; i++)
                sentry[i] = char(infile.get());
            if (sentry[len]) {
                infile.close();
                return false;
            }
            if (strcmp(sentry, PROGRAM_NAME)) {
                // We might have a pre-2.1 file format.
                if (strcmp(sentry, "Normal")) {
                    infile.close();
                    return false;
                }
            }
            delete[] sentry;
            majorVersion = readInt();
            minorVersion = readInt();
            openMode = READ;
            return true;
        } else
            return false;
    }
    if (newOpenMode == WRITE) {
        #ifdef __BINARY_IO
            outfile.open(fileName, MODE_WRITE | ios::binary);
        #else
            outfile.open(fileName, MODE_WRITE);
        #endif
        if (outfile.is_open()) {
            openMode = WRITE;
            majorVersion = ENGINE_VERSION_MAJOR;
            minorVersion = ENGINE_VERSION_MINOR;
            outfile << PROGRAM_NAME;
            outfile.put(0);
            writeInt(ENGINE_VERSION_MAJOR);
            writeInt(ENGINE_VERSION_MINOR);
            return true;
        }
        else
            return false;
    }
    return false;
}

void NFile::writeInt(int val) {
    if (val >= 0) {
        outfile.put(0);
        writeUInt(val);
    } else {
        outfile.put(-1);
        writeUInt(-val);
    }
}

void NFile::writeLong(long val) {
    if (val >= 0) {
        outfile.put(0);
        writeULong(val);
    } else {
        outfile.put(-1);
        writeULong(-val);
    }
}

void NFile::writeUInt(unsigned val) {
    for (int i=0; i<SIZE_INT; i++) {
        outfile.put((unsigned char)val);
        val >>= 8;
    }
}

void NFile::writeULong(unsigned long val) {
    int size = SIZE_LONG;
    for (int i=0; i<size; i++) {
        outfile.put((unsigned char)val);
        val >>= 8;
    }
}

int NFile::readInt() {
    if (infile.get() == 0)
        return (int)readUInt();
    else
        return -((int)readUInt());
}

long NFile::readLong() {
    if (infile.get() == 0)
        return (long)readULong();
    else
        return -((long)readULong());
}

unsigned int NFile::readUInt() {
    int i;
    unsigned char b[SIZE_INT];
    for (i=0; i<SIZE_INT; i++)
        b[i] = char(infile.get());
    unsigned long ans = 0;
    for (i=SIZE_INT-1; i>=0; i--) {
        ans <<= 8;
        ans += b[i];
    }
    return ans;
}

unsigned long NFile::readULong() {
    int size = SIZE_LONG;
    int i;
    unsigned char* b = new (unsigned char)[size];
    for (i=0; i<size; i++)
        b[i] = char(infile.get());
    unsigned long ans = 0;
    for (i=size-1; i>=0; i--) {
        ans <<= 8;
        ans += b[i];
    }
    delete[] b;
    return ans;
}

void NFile::writeLarge(const NLargeInteger& i) {
    char* str = i.stringValue();
    writeString(str);
    delete[] str;
}

NLargeInteger NFile::readLarge() {
    char* str = readString().dupe();
    NLargeInteger ans = NLargeInteger(str);
    delete[] str;
    return ans;
}

void NFile::writeString(const NString& s) {
    unsigned len = s.length();
    writeUInt(len);
    for (unsigned i=0; i<len; i++)
        outfile.put(s[i]);
}

NString NFile::readString() {
    unsigned len = readUInt();
    char* buf = new char[len+1];
    for (unsigned i=0; i<len; i++)
        buf[i] = char(infile.get());
    buf[len] = 0;
    return NString(buf);
}

void NFile::writePacketTree(NPacket* packet) {
    // Write the packet.
    writeInt(packet->getPacketType());
    writeString(packet->getPacketLabel());
    streampos bookmarkPos(getPosition());
    writePos(0);
    packet->writePacket(*this);
    
    // Write the children.
    NPacket* child = packet->getFirstTreeChild();
    if (child) {
        writeChar(TREE_CHILD);
        while (child) {
            writePacketTree(child);
            child = child->getNextTreeSibling();
            if (child)
                writeChar(TREE_CHILD);
        }
    }
    
    // Signify that there are no more children.
    writeChar(TREE_NO_MORE_CHILDREN);

    // Write bookmarking information.
    streampos finalPos(getPosition());
    setPosition(bookmarkPos);
    writePos(finalPos);
    setPosition(finalPos);
}

NPacket* NFile::readPacketTree(NPacket* parent) {
    streampos bookmark(0);
    NPacket* ans = readIndividualPacket(parent, bookmark);
    if (! ans) {
        setPosition(bookmark);
        return 0;
    }

    NPacket* child;
    char dir = readChar();
    while (dir == TREE_CHILD) {
        child = readPacketTree(ans);
        if (child) {
            if (! (child->getTreeParent()))
                ans->insertChildLast(child);
        }
        dir = readChar();
    }
    // Move to the start of the next packet anyway, just in case
    // something got out of kilter.
    setPosition(bookmark);

    // If this is the tree matriarch, call tidyReadPacket() for each
    // packet in the tree.
    if (parent == 0)
        for (child = ans; child; child = child->nextTreePacket())
            child->tidyReadPacket();

    return ans;
}

NPacket* NFile::readIndividualPacket(NPacket* parent, streampos& bookmark) {
    int packetType = readInt();
    NString packetLabel = readString();
    bookmark = readPos();
    
    // Check through each possible packet type.
    NPacket* ans;

    // Define a task for each REGISTER_PACKET to perform.
    #define REGISTER_PACKET(pClass, pType, pName) \
    if (packetType == pType) {                    \
        ans = pClass::readPacket(*this, parent);  \
        if (ans)                                  \
            ans->setPacketLabel(packetLabel);     \
        return ans;                               \
    }

    // Import all the REGISTER_PACKET lines.
    #define __PACKET_REGISTRY_BODY
    #ifdef __NO_INCLUDE_PATHS
        #include "packetregistry.h"
    #else
        #include "engine/packet/packetregistry.h"
    #endif

    // The packet is of an unknown type.
    return 0;
}

streampos NFile::getPosition() {
    if (openMode == READ)
        return infile.tellg();
    else
        return outfile.tellp();
}

void NFile::setPosition(streampos pos) {
    if (openMode == READ)
        infile.seekg(pos);
    else
        outfile.seekp(pos);
}

streampos NFile::readPos()
{
    int i;
    unsigned char b[SIZE_FILEPOS];
    for (i=0; i<SIZE_FILEPOS; i++)
        b[i] = char(infile.get());
    streamoff ans(0);
    for (i=SIZE_FILEPOS-1; i>=0; i--) {
        ans <<= 8;
        ans += b[i];
    }
    return streampos(ans);
}

void NFile::writePos(streampos realVal) {
    streamoff val(realVal);
    for (int i=0; i<SIZE_FILEPOS; i++) {
        outfile.put((unsigned char)val);
        val >>= 8;
    }
}

void NFile::writeTextShort(ostream& out) const {
    if (! openMode)
        out << "Closed file";
    else {
        out << "File version " << majorVersion << '.'
            << minorVersion << " open for ";
        if (openMode == READ)
            out << "reading";
        else
            out << "writing";
    }
}

