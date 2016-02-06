
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file foreign/pdf.h
 *  \brief Deprecated header that allows reading and writing PDF documents.
 *
 *  \deprecated All global functions that were once declared in this file have
 *  now been renamed to member functions of NPDF.
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
 * Deprecated function that reads a PDF document from the given file.
 *
 * If the file could not be read, or if the file is empty, then 0 will be
 * returned.  Otherwise a newly allocated PDF packet will be returned, and it
 * is the user's responsibility to deallocate this when it is finished with.
 *
 * \deprecated You should use the NPDF constructor NPDF::NPDF(const char*),
 * and you should pass the filename as the single string argument.
 * In situations where this routine would have returned a null pointer, the
 * NPDF constructor will instead create a null document (i.e., one for which
 * NPDF::isNull() returns \c true).  See the NPDF constructor for further
 * details.
 */
REGINA_DEPRECATED REGINA_API NPDF* readPDF(const char *filename);

/**
 * Deprecated function that writes the given PDF document to the given file.
 *
 * \deprecated This routine has been renamed as NPDF::savePDF().  This old
 * routine behaves exactly as in the past, but the new NPDF::savePDF() has
 * a slight change in behaviour: if the PDF packet contains no data then
 * it no longer creates an empty file, but instead does nothing and returns
 * \c false.  See NPDF::savePDF() for further details.
 */
REGINA_DEPRECATED REGINA_API bool writePDF(const char* filename,
    const NPDF& pdf);

/*@}*/

} // namespace regina

#endif

