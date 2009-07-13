
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include <cstring>

#include "engine.h"
#include "file/nfile.h"
#include "file/nfilepropertyreader.h"
#include "packet/packetregistry.h"

#define TREE_CHILD 'c'
#define TREE_NO_MORE_CHILDREN 'n'

namespace regina {

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
            int len = strlen(NFILE_PROGRAM_NAME);
            char* sentry = new char[len+1];
            for (int i=0; i<len+1; i++)
                sentry[i] = resource->getChar();
            if (sentry[len]) {
                close();
                return false;
            }
            if (strcmp(sentry, NFILE_PROGRAM_NAME)) {
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
            majorVersion = regina::getVersionMajor();
            minorVersion = regina::getVersionMinor();
            const char* sentry = NFILE_PROGRAM_NAME;

            for (const char* c = sentry; *c != 0; c++)
                resource->putChar(*c);
            resource->putChar(0);
            writeInt(majorVersion);
            writeInt(minorVersion);
            return true;
        }
        else {
            close();
            return false;
        }
    }
    return false;
}

std::streampos NFile::writePropertyHeader(unsigned propType) {
    writeUInt(propType);
    std::streampos bookmark = getPosition();
    writePos(0);
    return bookmark;
}

void NFile::writePropertyFooter(std::streampos bookmark) {
    std::streampos finalPos = getPosition();
    setPosition(bookmark);
    writePos(finalPos);
    setPosition(finalPos);
}

void NFile::writeAllPropertiesFooter() {
    writeUInt(0);
}

void NFile::readProperties(NFilePropertyReader* reader) {
    unsigned propType = readUInt();
    while (propType) {
        std::streampos bookmark = readPos();
        if (reader)
            reader->readIndividualProperty(*this, propType);
        setPosition(bookmark);
        propType = readUInt();
    }
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
    for (int i=0; i<NFILE_SIZE_INT; i++) {
        resource->putChar(static_cast<unsigned char>(val));
        val >>= 8;
    }
}

void NFile::writeULong(unsigned long val) {
    int size = NFILE_SIZE_LONG;
    for (int i=0; i<size; i++) {
        resource->putChar(static_cast<unsigned char>(val));
        val >>= 8;
    }
}

int NFile::readInt() {
    if (resource->getChar() == 0)
        return static_cast<int>(readUInt());
    else
        return -static_cast<int>(readUInt());
}

long NFile::readLong() {
    if (resource->getChar() == 0)
        return static_cast<long>(readULong());
    else
        return -static_cast<long>(readULong());
}

unsigned int NFile::readUInt() {
    int i;
    unsigned char b[NFILE_SIZE_INT];
    for (i=0; i<NFILE_SIZE_INT; i++)
        b[i] = resource->getChar();
    unsigned long ans = 0;
    for (i=NFILE_SIZE_INT-1; i>=0; i--) {
        ans <<= 8;
        ans += b[i];
    }
    return static_cast<unsigned>(ans);
}

unsigned long NFile::readULong() {
    int size = NFILE_SIZE_LONG;
    int i;
    unsigned char* b = new unsigned char[size];
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

void NFile::writeString(const std::string& s) {
    unsigned len = static_cast<unsigned>(s.length());
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
    unsigned char b[NFILE_SIZE_FILEPOS];
    for (i=0; i<NFILE_SIZE_FILEPOS; i++)
        b[i] = resource->getChar();
    std::streamoff ans(0);
    for (i=NFILE_SIZE_FILEPOS-1; i>=0; i--) {
        ans <<= 8;
        ans += b[i];
    }
    return std::streampos(ans);
}

void NFile::writePos(std::streampos realVal) {
    std::streamoff val(realVal);
    for (int i=0; i<NFILE_SIZE_FILEPOS; i++) {
        resource->putChar(static_cast<unsigned char>(val));
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

// Tidy up.
#undef REGISTER_PACKET
#undef __PACKET_REGISTRY_BODY

} // namespace regina

