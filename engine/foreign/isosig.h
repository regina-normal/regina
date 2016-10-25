
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

/*! \file foreign/isosig.h
 *  \brief Allows reading lists of isomorphism signatures.
 */

#ifndef __ISOSIG_H
#ifndef __DOXYGEN
#define __ISOSIG_H
#endif

#include "regina-core.h"

namespace regina {

class Container;

/**
 * \weakgroup foreign
 * @{
 */

/**
 * Reads a list of isomorphism signatures from the given text file.
 * The file should contain one isomorphism signature per line.
 * Signatures for both 3-manifold triangulations and 4-manifold triangulations
 * are accepted (though they cannot be mixed together within the same file).
 * These isomorphism signatures will be converted into triangulations using
 * Triangulation<3>::fromIsoSig() and Triangulation<4>::fromIsoSig()
 * respectively.
 *
 * A newly allocated container will be returned; the imported
 * triangulations will be inserted as children of this container.
 * The container will not be assigned a label.  The individual
 * triangulations will be assigned labels according to the parameter
 * \a colLabels.
 *
 * If any isomorphism signatures are invalid, these will be recorded in an
 * additional text packet that will be the last child of the returned
 * container.
 *
 * If an I/O error occurred while trying to read the given file, 0 will be
 * returned.
 *
 * In its simplest form, the text file can simply contain one
 * isomorphism signature per line and nothing else.  However, more complex
 * formats are allowed.  In particular, by passing appropriate values
 * for the arguments \a colSigs and \a colLabels, the isomorphism signatures
 * and triangulation packet labels can be taken from arbitrary
 * columns of the text file.  Columns are considered to be separated by
 * whitespace and are numbered beginning at 0.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and
 * simply passes it through unchanged to low-level C/C++ file I/O routines.
 * It assumes however that the \e contents of the file are in UTF-8.
 *
 * @param filename the name of the text file from which to read.
 * @param dimension either 3 or 4, indicating whether isomorphism
 * signatures should be expanded into 3-manifold triangulations or
 * 4-manifold triangulations respectively.
 * @param colSigs the column of the text file containing the
 * isomorphism signatures.
 * @param colLabels the column of the text file containing the
 * triangulation packet labels.  If this is negative then the
 * isomorphism signatures themselves will be used as packet labels.
 * @param ignoreLines the number of lines at the beginning of the text
 * file that should be ignored completely.
 * @return a new container as described above, or 0 if an I/O error occurred
 * whilst reading the given file.
 */
REGINA_API Container* readIsoSigList(const char *filename,
    unsigned dimension = 3, unsigned colSigs = 0, int colLabels = -1,
    unsigned long ignoreLines = 0);

/*@}*/

} // namespace regina

#endif

