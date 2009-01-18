
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

/*! \file nxmlfile.h
 *  \brief Deals with storing program data (including packet trees)
 *  in XML data files.
 */

#ifndef __NXMLFILE_H
#ifndef __DOXYGEN
#define __NXMLFILE_H
#endif

#include <string>

namespace regina {

class NPacket;

/**
 * \addtogroup file File I/O
 * File reading and writing.
 * @{
 */

/**
 * Writes the subtree with the given packet as matriarch to disk as a
 * complete XML file.  The XML file may be optionally compressed.
 *
 * This is the preferred way of writing a packet tree to file.
 *
 * \pre The given packet does not depend on its parent.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and simply
 * passes it through unchanged to low-level C/C++ file I/O routines.
 *
 * @param fileName the pathname of the file to write to.
 * @param subtree the matriarch of the packet tree that should be written.
 * @param compressed \c true if the XML file should be compressed or
 * \c false if it should be stored as plain text.
 * @return \c true if and only if the packet subtree was successfully
 * written.
 */
bool writeXMLFile(const char* fileName, NPacket* subtree,
    bool compressed = true);

/**
 * Reads the packet tree stored in the given XML file.  It does not
 * matter whether the XML file is compressed.
 *
 * If the matriarch of the packet tree could not be read, this routine
 * will return 0.  If a lower-level packet could not be read, it (and
 * its descendants) will simply be ignored.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and simply
 * passes it through unchanged to low-level C/C++ file I/O routines.
 *
 * @param fileName the pathname of the file to read from.
 * @return the packet tree read from file, or 0 if problems were
 * encountered or the file could not be opened.
 */
NPacket* readXMLFile(const char* fileName);

/**
 * Reads a packet tree from a file whose format is unknown.  The file
 * may be in either XML (optionally compressed) or old-style binary format.
 *
 * If the matriarch of the packet tree could not be read, this routine
 * will return 0.  If a lower-level packet could not be read, it (and
 * its descendants) will simply be ignored.
 *
 * The given file will almost certainly be opened and closed multiple
 * times during this routine.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and simply
 * passes it through unchanged to low-level C/C++ file I/O routines.
 *
 * @param fileName the pathname of the file to read from.
 * @return the packet tree read from file, or 0 if problems were
 * encountered or the file could not be opened.
 */
NPacket* readFileMagic(const std::string& fileName);

/*@}*/

} // namespace regina

#endif

