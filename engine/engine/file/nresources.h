
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

/*! \file nresources.h
 *  \brief Provides methods for accessing file-like resources.
 */

#ifndef __NRESOURCES_H
#ifndef __DOXYGEN
#define __NRESOURCES_H
#endif

#include <fstream>

/**
 * Provides a standard random access interface for file-like resources.
 *
 * \ifaces Not present.
 */
class NRandomAccessResource {
    public:
        /**
         * Specifies the current state of a resource.
         *
         * \ifaces This enumeration is a public member of class NFile.
         */
        enum mode {
            CLOSED = 0,
                /**< The resource is closed. */
            READ = 1,
                /**< The resource is open for reading. */
            WRITE = 2,
                /**< The resource is open for writing. */
        };

    public:
        /**
         * Destroys this resource.
         */
        virtual ~NRandomAccessResource();
        /**
         * Open the resource in read mode.
         * This routine should fail if the resource does not exist.
         *
         * \pre The resource is currently closed.
         *
         * @return \c true on success, \c false on failure.
         */
        virtual bool openRead() = 0;
        /**
         * Open the resource in write mode.
         * If the resource already exists, any existing contents should
         * be deleted.
         *
         * \pre The resource is currently closed.
         *
         * @return \c true on success, \c false on failure.
         */
        virtual bool openWrite() = 0;
        /**
         * Close the resource.
         * If the resource is already closed, this routine should do
         * nothing.
         */
        virtual void close() = 0;
        /**
         * Returns the current state of the resource.
         * If the resource is open, the mode in which it was opened will
         * be returned (either \c READ or \c WRITE).  If the file is
         * closed, 0 (alternatively, \c CLOSED) will be returned.
         *
         * @return the current state of the resource.
         */
        virtual mode getOpenMode() const = 0;
        /**
         * Reads a character from the current position in the resource
         * and moves on to the next position.
         *
         * \pre The resource is currently open in read mode.
         *
         * @return the character read.
         */
        virtual char getChar() = 0;
        /**
         * Writes the given character to the resource at the current
         * position and moves on to the next position.
         *
         * \pre The resource is currently open in write mode.
         *
         * @param c the character to write.
         */
        virtual void putChar(char c) = 0;
        /**
         * Returns the current position in the resource.
         *
         * \pre The resource is currently open.
         *
         * @return the current positon, as counted in bytes.
         */
        virtual long getPosition() = 0;
        /**
         * Moves to the given position in the resource.
         *
         * \pre The resource is currently open.
         *
         * @param pos the position to which to move, as counted in bytes.
         */
        virtual void setPosition(long pos) = 0;
};

/**
 * A random access resource that is simply a local file.
 */
class NLocalFileResource : public NRandomAccessResource {
    public:
        /**
         * Provides mode flags to pass to the system file opening
         * routines.
         */
        enum systemMode {
            #ifdef __NO_IOS_NOCREATE
            MODE_READ = ios::in | ios::binary,
            #else
            MODE_READ = ios::in | ios::binary | ios::nocreate,
            #endif
                /**< Open the file for reading. */
            MODE_WRITE = ios::out | ios::trunc | ios::binary
                /**< Open the file for writing. */
        };
    private:
        ifstream infile;
            /**< The file that is being read from. */
        ofstream outfile;
            /**< The file that is being written to. */
        mode openMode;
            /**< The current state of the file. */
        char* fileName;
            /**< The pathname of the file that is this resource. */
    public:
        /**
         * Creates a new resource referring to the given file.
         * The file will not be accessed until one of the <tt>open...</tt>
         * routines is called.
         *
         * \pre Parameter \a newFileName is not an empty string.
         *
         * @param newFileName the pathname of the file that is this
         * resource.
         */
        NLocalFileResource(const char* newFileName);
        /**
         * Destroys this resource reference, closing the corresponding
         * file if necessary.
         */
        virtual ~NLocalFileResource();

        virtual bool openRead();
        virtual bool openWrite();
        virtual void close();
        virtual mode getOpenMode() const;
        virtual char getChar();
        virtual void putChar(char c);
        virtual long getPosition();
        virtual void setPosition(long pos);
};

// Inline functions for NRandomAccessResource

inline NRandomAccessResource::~NRandomAccessResource() {
}

// Inline functions for NLocalFileResource

inline NLocalFileResource::NLocalFileResource(const char* newFileName) :
        openMode(CLOSED), fileName(new char[strlen(newFileName) + 1]) {
    strcpy(fileName, newFileName);
}

inline NLocalFileResource::~NLocalFileResource() {
    close();
    delete[] fileName;
}

inline NRandomAccessResource::mode NLocalFileResource::getOpenMode() const {
    return openMode;
}

inline char NLocalFileResource::getChar() {
    return infile.get();
}
inline void NLocalFileResource::putChar(char c) {
    outfile.put(c);
}

#endif
