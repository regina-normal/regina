
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

/*! \file nxmlfile.h
 *  \brief Deals with storing program data (including packet trees)
 *  in XML data files.
 */

#ifndef __NXMLFILE_H
#ifndef __DOXYGEN
#define __NXMLFILE_H
#endif

namespace regina {

class NPacket;

/**
 * Writes the subtree with the given packet as matriarch to disk as a
 * complete XML file.  The XML file may be optionally compressed.
 *
 * This is the preferred way of writing a packet tree to file.
 *
 * \pre The given packet does not depend on its parent.
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

} // namespace regina

#endif

