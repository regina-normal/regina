
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include <cstring>
#include "regina.h"

#include "file/nfile.h"
#include "packet/packetregistry.h"

#define TREE_CHILD 'c'
#define TREE_NO_MORE_CHILDREN 'n'

NPacket* readFromFile(const char* fileName) {
    NFile f;
    if (f.open(fileName, NRandomAccessResource::READ)) {
        NPacket* ans = f.readPacketTree();
        f.close();
        return ans;
    }
    return 0;
}

bool writeToFile(const char* fileName, NPacket* tree) {
    NFile f;
    if (f.open(fileName, NRandomAccessResource::WRITE)) {
        f.writePacketTree(tree);
        f.close();
        return true;
    }
    return false;
}

bool NFile::open(const char* fileName,
        NRandomAccessResource::mode newOpenMode) {
    return open(new NLocalFileResource(fileName), newOpenMode);
}

bool NFile::open(NRandomAccessResource* newResource,
        NRandomAccessResource::mode newOpenMode) {
    if (resource)
        close();
    // Resource is set to 0 and the file is closed.
    if (newOpenMode == NRandomAccessResource::READ) {
        resource = newResource;
        if (resource->openRead()) {
            int len = strlen(PROGRAM_NAME);
            char* sentry = new char[len+1];
            for (int i=0; i<len+1; i++)
                sentry[i] = resource->getChar();
            if (sentry[len]) {
                close();
                return false;
            }
            if (strcmp(sentry, PROGRAM_NAME)) {
                // We might have a pre-2.1 file format.
                if (strcmp(sentry, "Normal")) {
                    close();
                    return false;
                }
            }
            delete[] sentry;
            majorVersion = readInt();
            minorVersion = readInt();
            return true;
        } else {
            close();
            return false;
        }
    }
    if (newOpenMode == NRandomAccessResource::WRITE) {
        resource = newResource;
        if (resource->openWrite()) {
            majorVersion = ENGINE_VERSION_MAJOR;
            minorVersion = ENGINE_VERSION_MINOR;
            char* sentry = PROGRAM_NAME;

            for (char* c = sentry; *c != 0; c++)
                resource->putChar(*c);
            resource->putChar(0);
            writeInt(ENGINE_VERSION_MAJOR);
            writeInt(ENGINE_VERSION_MINOR);
            return true;
        }
        else {
            close();
            return false;
        }
    }
    return false;
}

void NFile::writeInt(int val) {
    if (val >= 0) {
        resource->putChar(0);
        writeUInt(val);
    } else {
        resource->putChar(-1);
        writeUInt(-val);
    }
}

void NFile::writeLong(long val) {
    if (val >= 0) {
        resource->putChar(0);
        writeULong(val);
    } else {
        resource->putChar(-1);
        writeULong(-val);
    }
}

void NFile::writeUInt(unsigned val) {
    for (int i=0; i<SIZE_INT; i++) {
        resource->putChar((unsigned char)val);
        val >>= 8;
    }
}

void NFile::writeULong(unsigned long val) {
    int size = SIZE_LONG;
    for (int i=0; i<size; i++) {
        resource->putChar((unsigned char)val);
        val >>= 8;
    }
}

int NFile::readInt() {
    if (resource->getChar() == 0)
        return (int)readUInt();
    else
        return -((int)readUInt());
}

long NFile::readLong() {
    if (resource->getChar() == 0)
        return (long)readULong();
    else
        return -((long)readULong());
}

unsigned int NFile::readUInt() {
    int i;
    unsigned char b[SIZE_INT];
    for (i=0; i<SIZE_INT; i++)
        b[i] = resource->getChar();
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
        b[i] = resource->getChar();
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

void NFile::writeString(const std::string& s) {
    unsigned len = s.length();
    writeUInt(len);
    for (unsigned i=0; i<len; i++)
        resource->putChar(s[i]);
}

std::string NFile::readString() {
    unsigned len = readUInt();
    char* buf = new char[len+1];
    for (unsigned i=0; i<len; i++)
        buf[i] = resource->getChar();
    buf[len] = 0;
    return buf;
}

void NFile::writePacketTree(NPacket* packet) {
    // Write the packet.
    writeInt(packet->getPacketType());
    writeString(packet->getPacketLabel());
    std::streampos bookmarkPos(getPosition());
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
    std::streampos finalPos(getPosition());
    setPosition(bookmarkPos);
    writePos(finalPos);
    setPosition(finalPos);
}

NPacket* NFile::readPacketTree(NPacket* parent) {
    std::streampos bookmark(0);
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

NPacket* NFile::readIndividualPacket(NPacket* parent,
        std::streampos& bookmark) {
    int packetType = readInt();
    std::string packetLabel = readString();
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
    #include "packet/packetregistry.h"

    // The packet is of an unknown type.
    return 0;
}

std::streampos NFile::readPos()
{
    int i;
    unsigned char b[SIZE_FILEPOS];
    for (i=0; i<SIZE_FILEPOS; i++)
        b[i] = resource->getChar();
    std::streamoff ans(0);
    for (i=SIZE_FILEPOS-1; i>=0; i--) {
        ans <<= 8;
        ans += b[i];
    }
    return std::streampos(ans);
}

void NFile::writePos(std::streampos realVal) {
    std::streamoff val(realVal);
    for (int i=0; i<SIZE_FILEPOS; i++) {
        resource->putChar((unsigned char)val);
        val >>= 8;
    }
}

void NFile::writeTextShort(std::ostream& out) const {
    NRandomAccessResource::mode openMode = getOpenMode();
    if (! openMode)
        out << "Closed file";
    else {
        out << "File version " << majorVersion << '.'
            << minorVersion << " open for ";
        if (openMode == NRandomAccessResource::READ)
            out << "reading";
        else
            out << "writing";
    }
}

