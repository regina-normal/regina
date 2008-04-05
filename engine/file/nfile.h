
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file nfile.h
 *  \brief Deals with reading and writing packet trees to and from
 *  old-style binary files.
 *  \deprecated The preferred way of representing data is using XML
 *  which is accessed using text I/O streams.  See file nxmlfile.h for
 *  XML routines corresponding to the old-style binary file routines
 *  declared in this header.
 */

#ifndef __NFILE_H
#ifndef __DOXYGEN
#define __NFILE_H
#endif

#include "shareableobject.h"
#include "file/nresources.h"
#include "utilities/nmpi.h"
#include "utilities/nbooleans.h"

namespace regina {

class NFilePropertyReader;
class NPacket;

/**
 * \weakgroup file
 * @{
 */

#define NFILE_PROGRAM_NAME "Regina"
    /**< String name of the program, to be placed at the beginning of
     *   data files. */
#define NFILE_SIZE_INT 4
    /**< The number of bytes written to files to represent an integer;
     *   any higher order bytes will be ignored.  This ensures
     *   constancy of file format across platforms. */
#define NFILE_SIZE_LONG 8
    /**< The number of bytes written to files to represent a long integer;
     *   any higher order bytes will be ignored.  This ensures
     *   constancy of file format across platforms. */
#define NFILE_SIZE_FILEPOS 8
    /**< The number of bytes written to files to represent a file position;
     *   any higher order bytes will be ignored.  This ensures
     *   constancy of file format across platforms. */

/**
 * Reads a packet tree from the given old-style binary file doing everything
 * in a single step.
 *
 * This routine reads the complete packet tree from the given file.
 * If the highest level packet could not be read, this
 * routine will return null.  The behaviour regarding problematic
 * subpackets is identical to that of NFile::readPacketTree().
 *
 * \deprecated The preferred way of representing data is using XML which
 * is accessed using text I/O streams.  See routines readXMLFile() and
 * readFileMagic() for corresponding XML all-at-once read routines.
 *
 * \ifacespython Not present.
 *
 * @param fileName the pathname of the file to read from.
 * @return the packet tree read from file, or \c null if problems
 * were encountered or the file could not be opened.
 */
NPacket* readFromFile(const char* fileName);

/**
 * Writes the given packet tree to the given old-style binary file doing
 * everything in a single step.
 *
 * \deprecated The preferred way of representing data is using XML which
 * is accessed using text I/O streams.  See routine writeXMLFile() for a
 * corresponding XML all-at-once write routine.
 *
 * \ifacespython Not present.
 *
 * @param fileName the pathname of the file to write to.
 * @param packet the packet tree to write to file.
 * @return \c true if and only if the packet tree was successfully
 * written.
 */
bool writeToFile(const char* fileName, NPacket* packet);

/**
 * Represents an old-style binary file containing a packet tree.
 * Provides routines for opening, closing, reading and writing.
 *
 * \deprecated The preferred way of representing data is using XML which
 * is accessed using text I/O streams.
 *
 * \ifacespython Not present.
 */
class NFile : public ShareableObject {
    protected:
        int majorVersion;
            /**< Major version number of the engine that wrote this file. */
        int minorVersion;
            /**< Minor version number of the engine that wrote this file. */

    private:
        NRandomAccessResource* resource;
            /**< The underlying resource containing this file's contents. */

    public:
        /**
         * Creates a new closed file.
         */
        NFile();

        /**
         * Default destructor.
         * Ensures that the file is closed.
         */
        virtual ~NFile();

        /**
         * Opens the requested resource in the requested mode.
         * If the resource is to be opened for reading, it will fail if the
         * resource does not exist.
         * If the resource is to be opened for writing, it will delete any
         * existing resource contents.
         *
         * This \a NFile will be responsible for destroying the given
         * resource \a newResource, whether or not the open succeeds.
         * It should not be destroyed elsewhere.
         *
         * \pre This file is currently closed.
         *
         * @param newResource the resource to open.
         * @param newOpenMode specifies in which mode the resource is to be
         * opened.  This should be either \c READ or \c WRITE.
         * @return \c true on success, \c false on failure.
         */
        bool open(NRandomAccessResource* newResource,
            NRandomAccessResource::mode newOpenMode);

        /**
         * Opens the requested file in the requested mode.
         * If the file is to be opened for reading, it will fail if the
         * file does not exist.
         * If the file is to be opened for writing, it will delete any
         * existing file contents.
         *
         * \pre This file is currently closed.
         *
         * @param fileName the pathname of the file to open.
         * @param newOpenMode specifies in which mode the file is to be
         * opened.  This should be either \c READ or \c WRITE.
         * @return \c true on success, \c false on failure.
         */
        bool open(const char* fileName,
            NRandomAccessResource::mode newOpenMode);

        /**
         * Closes the file.
         * If the file is already closed, this routine will do nothing.
         */
        void close();

        /**
         * Returns the current state of the file.
         * If the file is open, the mode in which it was opened will be
         * returned (either \c READ or \c WRITE).  If the file is
         * closed, 0 (alternatively, \c CLOSED) will be returned.
         *
         * @return the current state of the file.
         */
        NRandomAccessResource::mode getOpenMode() const;

        /**
         * Returns the major version number of the engine responsible
         * for this file.
         * If the file is open for reading, this will be the number of
         * the engine that wrote the file.
         * If the file is open for writing, this will be the number of
         * this engine.
         *
         * \pre The file is currently open.
         *
         * @return the major version number.
         */
        int getMajorVersion();

        /**
         * Returns the minor version number of the engine responsible
         * for this file.
         * If the file is open for reading, this will be the number of
         * the engine that wrote the file.
         * If the file is open for writing, this will be the number of
         * this engine.
         *
         * \pre The file is currently open.
         *
         * @return the minor version number.
         */
        int getMinorVersion();

        /**
         * Determines if this file has a version earlier than the given
         * version.
         * This routine will return \c false if the versions are
         * identical.
         *
         * @param major the major version to compare this file with.
         * @param minor the minor version to compare this file with.
         * @return \c true if and only if the version of this file is
         * earlier than the given major-minor combination.
         */
        bool versionEarlierThan(int major, int minor);

        /**
         * Writes the given packet tree to file.
         *
         * \pre The file is currently opened for writing.
         *
         * @param packet the packet tree to be written to file.
         */
        void writePacketTree(NPacket* packet);

        /**
         * Reads a packet tree from file.  This routine can also be used
         * to read a packet subtree.
         * 
         * Any packets encountered of unknown
         * type will be skipped, along with their descendants.
         * If an individual packet read routine runs into problems or
         * reads in the wrong amount of data, this will not affect other
         * packets in the overall tree aside from descendants of the
         * problematic packet.
         *
         * \pre The file is currently opened for reading.
         *
         * @param parent the packet that will become the parent of the
         * new subtree being read, or 0 if an entire packet tree is
         * being read.  Note that the subtree being read might or might not
         * be inserted as a child of \a parent; this information is
         * provided mainly for reference.
         * @return the packet tree read from file, or 0 if problems were
         * encountered with the highest level packet in the tree.
         */
        NPacket* readPacketTree(NPacket* parent = 0);

        /**
         * Writes a header for a property associated with some object.
         * This header will include the given property type as well as
         * some bookmarking details.  The bookmark returned should later
         * be passed to writePropertyFooter() for housekeeping.
         *
         * If some set of properties is to be stored for some object,
         * the procedure for writing properties to a file is as follows.
         *
         * - For each property:
         *   - Call writePropertyHeader(), passing a unique integer
         *     (the property type) that indicates which particular property
         *     is being written;
         *   - Write the contents of the property to file (there is no need
         *     to write the property type);
         *   - Call writePropertyFooter(), passing the bookmarking
         *     information that was returned from the earlier call to
         *     writePropertyHeader().
         * - After all properties have been written, call
         *   writeAllPropertiesFooter().
         *
         * This allows an NFilePropertyReader to read the properties from
         * file in a manner independent of which version of Regina was
         * originally used to create the file.  In particular,
         * properties of unknown type can simply be ignored, and errors
         * in reading individual properties can be gracefully overcome.
         *
         * To read a set of properties that has been stored for some
         * object, call readProperties() with an NFilePropertyReader that
         * understands the different potential property types.
         *
         * \pre The file is currently opened for writing.
         *
         * @param propType the property type to write in the property
         * header; this must be strictly positive.
         * @return bookmarking information that should later be passed
         * to writePropertyFooter().
         */
        std::streampos writePropertyHeader(unsigned propType);

        /**
         * Writes a footer for a property associated with some object.
         * See the writePropertyHeader() notes for details about writing
         * object properties to file.
         *
         * \pre The file is currently opened for writing.
         *
         * @param bookmark the bookmark returned from the corresponding
         * call to writePropertyHeader().
         */
        void writePropertyFooter(std::streampos bookmark);

        /**
         * Writes a footer indicating that all properties associated with
         * the current object have now been written.
         * See the writePropertyHeader() notes for details about writing
         * object properties to file.
         *
         * \pre The file is currently opened for writing.
         */
        void writeAllPropertiesFooter();

        /**
         * Reads in an entire set of properties associated with some
         * object.  These properties must have been written using the
         * mechanism described in the writePropertyHeader() notes.
         *
         * Note that different types of properties may have been written to
         * file, some of which can be understood and some of which cannot
         * (since a different version of Regina may have been used to
         * originally write the file).  Properties that cannot be
         * understood will simply be ignored.  If errors occur whilst
         * reading properties that can be understood, these errors will
         * be gracefully overcome.
         *
         * The property reader that is passed is responsible for
         * declaring which property types are recognised as well as
         * reading and processing individual properties of recognised
         * types.
         *
         * Once this routine has finished the current position in the
         * file will be after the final property footer as written by
         * writeAllPropertiesFooter().
         *
         * \pre The file is currently opened for reading.
         *
         * @param reader the property reader responsible for reading
         * individual properties.  This may be 0, in which case all
         * properties will be ignored (though the current file position
         * will still be adjusted as described above).
         */
        void readProperties(NFilePropertyReader* reader);

        /**
         * Reads a signed integer from file.
         *
         * \pre The file is currently opened for reading.
         *
         * @return the integer read.
         */
        int readInt();

        /**
         * Reads an unsigned integer from file.
         *
         * \pre The file is currently opened for reading.
         *
         * @return the integer read.
         */
        unsigned readUInt();

        /**
         * Reads a signed long integer from file.
         *
         * \pre The file is currently opened for reading.
         *
         * @return the long integer read.
         */
        long readLong();

        /**
         * Reads an unsigned long integer from file.
         *
         * \pre The file is currently opened for reading.
         *
         * @return the long integer read.
         */
        unsigned long readULong();

        /**
         * Reads an arbitrary precision integer from file.
         *
         * \pre The file is currently opened for reading.
         *
         * @return the arbitrary precision integer read.
         */
        NLargeInteger readLarge();

        /**
         * Reads a character from file.
         *
         * \pre The file is currently opened for reading.
         *
         * @return the character read.
         */
        char readChar();

        /**
         * Reads a string from file.
         *
         * \pre The file is currently opened for reading.
         *
         * @return the string read.
         */
        std::string readString();

        /**
         * Reads a bool from file.
         *
         * \pre The file is currently opend for reading.
         *
         * @return the bool read.
         */
        bool readBool();

        /**
         * Reads a boolean set from file.
         *
         * \pre The file is currently opend for reading.
         *
         * @return the boolean set read.
         */
        NBoolSet readBoolSet();

        /**
         * Writes a signed integer to file.
         *
         * \pre The file is currently opened for writing.
         *
         * @param i the integer to write.
         */
        void writeInt(int i);

        /**
         * Writes an unsigned integer to file.
         *
         * \pre The file is currently opened for writing.
         *
         * @param i the integer to write.
         */
        void writeUInt(unsigned i);

        /**
         * Writes a signed long integer to file.
         *
         * \pre The file is currently opened for writing.
         *
         * @param i the long integer to write.
         */
        void writeLong(long i);

        /**
         * Writes an unsigned long integer to file.
         *
         * \pre The file is currently opened for writing.
         *
         * @param i the long integer to write.
         */
        void writeULong(unsigned long i);

        /**
         * Writes an arbitrary precision integer to file.
         *
         * \pre The file is currently opened for writing.
         *
         * @param i the arbitrary precision integer to write.
         */
        void writeLarge(const NLargeInteger& i);

        /**
         * Writes a character to file.
         *
         * \pre The file is currently opened for writing.
         *
         * @param c the character to write.
         */
        void writeChar(char c);

        /**
         * Writes a string to file.
         *
         * \pre The file is currently opened for writing.
         *
         * @param s the string to write.
         */
        void writeString(const std::string& s);

        /**
         * Writes a bool to file.
         *
         * \pre The file is currently opened for writing.
         *
         * @param b the bool to write.
         */
        void writeBool(bool b);

        /**
         * Writes a boolean set to file.
         *
         * \pre The file is currently opened for writing.
         *
         * @param b the boolean set to write.
         */
        void writeBoolSet(const NBoolSet& b);

        /**
         * Returns the current position in the file.
         *
         * \pre The file is currently open.
         *
         * @return the current file position.
         */
        std::streampos getPosition();

        /**
         * Moves to the requested position in the file.
         *
         * \pre The file is currently open.
         *
         * @param pos the position to which to move.
         */
        void setPosition(std::streampos pos);

        /**
         * Reads a file position from file.
         *
         * \pre The file is currently opened for reading.
         *
         * @return the file position read.
         */
        std::streampos readPos();

        /**
         * Writes a file position to file.
         *
         * \pre The file is currently opened for writing.
         *
         * @param pos the file position to write.
         */
        void writePos(std::streampos pos);

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Reads a single packet from file.
         * The packet information (including type and label)
         * and actual packet data will both be
         * read, and a newly created \c NPacket will be returned.
         * Only a single packet will be read.
         *
         * @param parent the packet that will sit above the newly read
         * packet in the overall tree structure.  This information is
         * provided for reference only; the new packet might or might
         * not be inserted as a child of \a parent.
         * @param a variable in which to store bookmarking information.
         * The file position representing the end of this packet and its
         * entire subtree, which is stored with the
         * packet specification, will be read and returned in this
         * variable.
         * @return a newly created \c NPacket representing the data that
         * was read from file, or 0 if problems arose (such as an
         * unrecognised packet type).
         */
        NPacket* readIndividualPacket(NPacket* parent,
            std::streampos& bookmark);
};

/*@}*/

// Inline functions for NFile

inline NFile::NFile() : resource(0) {
}

inline NFile::~NFile() {
    close();
}

inline void NFile::close() {
    if (resource) {
        resource->close();
        delete resource;
    }
    resource = 0;
}

inline NRandomAccessResource::mode NFile::getOpenMode() const {
    return (resource == 0 ?
        NRandomAccessResource::CLOSED : resource->getOpenMode());
}

inline int NFile::getMajorVersion() {
    return majorVersion;
}

inline int NFile::getMinorVersion() {
    return minorVersion;
}

inline bool NFile::versionEarlierThan(int major, int minor) {
    if (majorVersion < major) return true;
    else if (majorVersion > major) return false;
    else return (minorVersion < minor);
}

inline void NFile::writeLarge(const NLargeInteger& i) {
    writeString(i.stringValue());
}

inline NLargeInteger NFile::readLarge() {
    return readString().c_str();
}

inline void NFile::writeChar(char c) {
    resource->putChar(c);
}

inline char NFile::readChar() {
    return resource->getChar();
}

inline void NFile::writeBool(bool b) {
    if (b) writeUInt(1);
    else writeUInt(0);
}

inline bool NFile::readBool() {
    if (readUInt() == 1) return true;
    return false;
}

inline void NFile::writeBoolSet(const NBoolSet& b) {
    writeBool(b.hasTrue());
    writeBool(b.hasFalse());
}

inline NBoolSet NFile::readBoolSet() {
    return NBoolSet(readBool(), readBool());
}

inline std::streampos NFile::getPosition() {
    return static_cast<std::streampos>(resource->getPosition());
}

inline void NFile::setPosition(std::streampos pos) {
    resource->setPosition(static_cast<long>(pos));
}

} // namespace regina

#endif

