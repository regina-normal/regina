
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

/*! \file foreign/pdf.h
 *  \brief Allows reading and writing PDF documents.
 */

#ifndef __PDF_H
#ifndef __DOXYGEN
#define __PDF_H
#endif

namespace regina {

class NPDF;

/**
 * \weakgroup foreign
 * @{
 */

/**
 * Reads a PDF document from the given file.  A newly allocated PDF packet
 * will be returned; it is the user's responsibility to deallocate this
 * when it is finished with.
 *
 * This routine does not check whether the given file \e looks like a
 * PDF document; it simply loads the file contents blindly.
 *
 * The packet label of the new PDF packet will be left empty.
 *
 * If the file could not be read, 0 will be returned.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and
 * simply passes it through unchanged to low-level C/C++ file I/O routines.
 *
 * @param filename the filename of the PDF document to read.
 * @return a new PDF packet containing the PDF document, or 0 on error.
 */
NPDF* readPDF(const char *filename);

/**
 * Writes the given PDF document to the given file.
 *
 * This routine does not check whether the contents of the given packet
 * \e look like a PDF document; it simply writes them blindly to the
 * given file.
 *
 * If the given PDF packet is empty (i.e., does not contain a real block
 * of data) then the resulting file will be created but left empty.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and
 * simply passes it through unchanged to low-level C/C++ file I/O routines.
 *
 * @param filename the filename of the PDF document to write.
 * @param pdf the PDF packet to write to the given file.
 * @return \c true if the export was successful, or \c false otherwise.
 */
bool writePDF(const char* filename, const NPDF& pdf);

/*@}*/

} // namespace regina

#endif

