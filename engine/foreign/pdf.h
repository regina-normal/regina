
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file foreign/pdf.h
 *  \brief Allows reading and writing PDF documents.
 */

#ifndef __PDF_H
#ifndef __DOXYGEN
#define __PDF_H
#endif

#include "regina-core.h"

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
REGINA_API NPDF* readPDF(const char *filename);

/**
 * Deprecated function that writes the given PDF document to the given file.
 *
 * \deprecated This routine has been renamed as NPDF::savePDF().  This old
 * routine behaves exactly as in the past, but the new NPDF::savePDF() has
 * a slight change in behaviour: if the PDF packet contains no data then
 * it no longer creates an empty file, but instead does nothing and returns
 * \c false.  See NPDF::savePDF() for further details.
 */
REGINA_API bool writePDF(const char* filename, const NPDF& pdf);

/*@}*/

} // namespace regina

#endif

