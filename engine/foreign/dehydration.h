
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

/*! \file foreign/dehydration.h
 *  \brief Allows reading lists of dehydrated triangulations.
 */

#ifndef __DEHYDRATION_H
#ifndef __DOXYGEN
#define __DEHYDRATION_H
#endif

#include "regina-core.h"

namespace regina {

class Container;

/**
 * \weakgroup foreign
 * @{
 */

/**
 * Reads a list of dehydrated triangulations from the given text file.
 * The file should contain one dehydration string per line.  These
 * strings will be rehydrated as described in
 * Triangulation<3>::insertRehydration().
 *
 * A newly allocated container will be returned; the imported
 * triangulations will be inserted as children of this container.
 * The container will not be assigned a label.  The individual
 * triangulations will be assigned labels according to the parameter
 * \a colLabels.
 *
 * If any dehydrations strings are invalid, these will be recorded in an
 * additional text packet that will be the last child of the returned
 * container.
 *
 * If an I/O error occurred while trying to read the given file, 0 will be
 * returned.
 *
 * In its simplest form, the text file can simply contain one
 * dehydration string per line and nothing else.  However, more complex
 * formats are allowed.  In particular, by passing appropriate values
 * for the arguments \a colDehydrations and \a colLabels, the dehydration
 * strings and triangulation packet labels can be taken from arbitrary
 * columns of the text file.  Columns are considered to be separated by
 * whitespace and are numbered beginning at 0.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and
 * simply passes it through unchanged to low-level C/C++ file I/O routines.
 * It assumes however that the \e contents of the file are in UTF-8.
 *
 * @param filename the name of the text file from which to read.
 * @param colDehydrations the column of the text file containing the
 * dehydration strings.
 * @param colLabels the column of the text file containing the
 * triangulation packet labels.  If this is negative then the dehydration
 * strings themselves will be used as packet labels.
 * @param ignoreLines the number of lines at the beginning of the text
 * file that should be ignored completely.
 * @return a new container as described above, or 0 if an I/O error occurred
 * whilst reading the given file.
 */
REGINA_API Container* readDehydrationList(const char *filename,
    unsigned colDehydrations = 0, int colLabels = -1,
    unsigned long ignoreLines = 0);

/*@}*/

} // namespace regina

#endif

