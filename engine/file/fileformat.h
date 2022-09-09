
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file file/fileformat.h
 *  \brief Represents the different generations of Regina data file format.
 */

#ifndef __REGINA_FILEFORMAT_H
#ifndef __DOXYGEN
#define __REGINA_FILEFORMAT_H
#endif

namespace regina {

/**
 * Represents each generation of Regina's file formats.
 *
 * These constants are intended to capture "generational changes" to the
 * file format, where backward compatibility is broken (i.e., where older
 * versions of Regina that do not know about the new generation format cannot
 * even attempt to read its data files at all).
 *
 * In constrast, they do not capture incremental changes (e.g., new elements
 * in data files that correspond to new features of Regina, which are simply
 * ignored when an older version of Regina sees them).
 *
 * Put differently: within each generation, the file formats should be
 * considered "interoperable", in that they maintain both forward and backward
 * compatibility as far as possible.  In contrast, between generations
 * the file formats are not compatible at all, and require some form of
 * explicit conversion.
 *
 * Of course, Regina can still read and write some older generation formats;
 * these abilities are indicated alongside the constants below.
 *
 * \ingroup file
 */
enum FileFormat {
    /**
     * Indicates the old first-generation binary format used by Regina 2.4
     * and earlier.  This format was discontinued in mid-2002, when Regina
     * adopted an XML-based format instead.
     *
     * Regina no longer supports this (ancient) binary format at all: since
     * version 4.94 in late 2013, Regina cannot read or write this format.
     */
    REGINA_BINARY_GEN_1 = 1,
    /**
     * Indicates the second-generation file format used from Regina 3.0
     * through to Regina 6.0.1 inclusive.  This was the first file format
     * based on XML.  The root XML element is \c reginadata.
     *
     * Regina can still transparently read second-generation data files,
     * and can write them when explicitly asked.
     */
    REGINA_XML_GEN_2 = 2,
    /**
     * Indicates the third-generation file format used from Regina 7.0
     * onwards.  This format is both more streamlined and more human-readable.
     * The root XML element is \c regina.
     *
     * This is the current file format, and is used by default when
     * saving data files.
     */
    REGINA_XML_GEN_3 = 3,
    /**
     * An alias for whichever file format is current.  The numerical
     * value of this constant may change in future releases of Regina,
     * if/when new generational changes to the file format occur.
     */
    REGINA_CURRENT_FILE_FORMAT = REGINA_XML_GEN_3
};

} // namespace regina

#endif

