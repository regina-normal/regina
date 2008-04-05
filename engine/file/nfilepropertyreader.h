
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

/*! \file nfilepropertyreader.h
 *  \brief Deals with reading individual properties of objects from
 *  old-style binary files.
 *  \deprecated The preferred way of storing data is using XML
 *  which is accessed via text I/O streams.  New-style XML data files
 *  should simply bundle object properties in with the standard XML
 *  read/write routines.
 */

#ifndef __NFILEPROPERTYREADER_H
#ifndef __DOXYGEN
#define __NFILEPROPERTYREADER_H
#endif

namespace regina {

class NFile;

/**
 * \weakgroup file
 * @{
 */

/**
 * Assists with reading individual object properties from old-style
 * binary data files.
 *
 * The mechanics of reading and writing properties of objects are
 * described in the notes for NFilePropertyReader::readIndividualProperty()
 * and NFile::writePropertyHeader() respectively.
 *
 * Each property that can be stored on file for a particular type of
 * object has an integer identifier called the <i>property type</i>.
 * Property types must be strictly positive, and must be unique for each
 * type of object that can store properties.
 *
 * \deprecated This class is only for use with old-style binary data
 * files.  New-style XML data files should simply bundle object
 * properties in with the standard XML read/write routines.
 *
 * \ifacespython Not present.
 */
class NFilePropertyReader {
    public:
        /**
         * Default destructor that does nothing.
         */
        virtual ~NFilePropertyReader();

        /**
         * Reads an individual property from an old-style binary file.
         * The property type and bookmarking details should not
         * read; merely the contents of the property that are written
         * to file between NFile::writePropertyHeader() and
         * NFile::writePropertyFooter().  See the
         * NFile::writePropertyHeader() notes for details.
         *
         * The property type of the property to be read will be passed
         * in \a propType.  If the property type is unrecognised, this
         * routine should simply do nothing and return.  If the property
         * type is recognised, this routine should read the property and
         * process it accordingly (e.g., store it in whatever data object
         * is currently being read).
         *
         * @param infile the file from which to read the property.  This
         * should be open for reading and at the position immediately after
         * writePropertyHeader() would have been called during the
         * corresponding write operation.
         * @param propType the property type of the property about to be
         * read.
         */
        virtual void readIndividualProperty(NFile& infile,
            unsigned propType) = 0;
};

/*@}*/

// Inline functions for NFilePropertyReader

inline NFilePropertyReader::~NFilePropertyReader() {
}

} // namespace regina

#endif

