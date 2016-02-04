
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file file/nxmlfile.h
 *  \brief Deals with storing program data (including packet trees)
 *  in XML data files.
 *
 *  \deprecated The global functions that were once declared in this file have
 *  been renamed, and are now declared in npacket.h.
 */

#ifndef __NXMLFILE_H
#ifndef __DOXYGEN
#define __NXMLFILE_H
#endif

#include <string>
#include "regina-core.h"
#include "packet/npacket.h"

namespace regina {

class NPacket;

/**
 * \addtogroup file File I/O
 * File reading and writing.
 * @{
 */

/**
 * Deprecated routine that writes a packet tree (or subtree) to a Regina
 * data file using Regina's native XML file format.
 *
 * \deprecated This routine has been renamed as NPacket::save().
 * See the documentation of NPacket::save() for further details.
 */
REGINA_DEPRECATED REGINA_API bool writeXMLFile(const char* filename,
    NPacket* subtree, bool compressed = true);

/**
 * Deprecated routine that reads a packet tree from the given Regina
 * data file using Regina's native XML file format.
 *
 * \deprecated This routine has been renamed as regina::open(), and is
 * now declared in npacket.h.  See the documentation for regina::open()
 * for further details.
 */
REGINA_DEPRECATED REGINA_API NPacket* readXMLFile(const char* filename);

/**
 * Deprecated routine that reads a packet tree from the given Regina
 * data file using Regina's native XML file format.
 *
 * \deprecated This routine has been renamed as regina::open(), and is
 * now declared in npacket.h.  See the documentation for regina::open()
 * for further details.
 */
REGINA_DEPRECATED REGINA_API NPacket* readFileMagic(
    const std::string& filename);

/*@}*/

// Inline functions:

inline bool writeXMLFile(const char* filename, NPacket* subtree,
        bool compressed) {
    return subtree->save(filename, compressed);
}

inline NPacket* readXMLFile(const char* filename) {
    return regina::open(filename);
}

inline NPacket* readFileMagic(const std::string& filename) {
    return regina::open(filename.c_str());
}

} // namespace regina

#endif

