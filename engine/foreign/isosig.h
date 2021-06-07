
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
 *  \brief Allows reading lists of isomorphism signatures and knot signatures.
 */

#ifndef __REGINA_ISOSIG_H
#ifndef __DOXYGEN
#define __REGINA_ISOSIG_H
#endif

#include "regina-core.h"

namespace regina {

class Container;

/**
 * \weakgroup foreign
 * @{
 */

/**
 * Reads a list of isomorphism signatures or knot signatures from the given
 * text file.  The file should contain one signature per line.
 * Signatures for knots or triangulations of any dimension are all accepted,
 * though the type of object must be known in advance and fixed for the
 * entire function call using the template parameter \a PacketType.
 * These signatures will be converted into knots and/or triangulations using
 * Link::fromKnotSig() and Triangulation<dim>::fromIsoSig() respectively.
 *
 * A newly allocated container will be returned; the imported
 * knots or triangulations will be inserted as children of this container.
 * The container will not be assigned a label.  The individual
 * knots or triangulations will be assigned labels according to the parameter
 * \a colLabels.
 *
 * If any signatures are invalid, these will be recorded in an additional
 * text packet that will be the last child of the returned container.
 *
 * If an I/O error occurred while trying to read the given file, \c null will be
 * returned.
 *
 * In its simplest form, the text file can simply contain one
 * signature per line and nothing else.  However, more complex
 * formats are allowed.  In particular, by passing appropriate values
 * for the arguments \a colSigs and \a colLabels, the signatures
 * and packet labels can be taken from arbitrary
 * columns of the text file.  Columns are considered to be separated by
 * whitespace and are numbered beginning at 0.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and
 * simply passes it through unchanged to low-level C/C++ file I/O routines.
 * It assumes however that the \e contents of the file are in UTF-8.
 *
 * \tparam PacketType Indicates which types of signatures the file contains.
 * This must be either Link (indicating that the file contains knot signatures),
 * or one of the Triangulation<dim> classes (indicating that the file contains
 * isomorphism signatures for <i>dim</i>-dimensional triangulations).
 *
 * \ifacespython Since Python does not support templates, the Python
 * version of this function takes an extra first parameter \a dimension.
 * This should be the dimension of the underlying triangulation type, or 0 to
 * indicate that we are working with knot signatures.  Moreover, the Python
 * version currently only supports Regina's standard dimensions (i.e., you
 * cannot use the Python version of this function with triangulations of
 * dimension 5 or higher).
 *
 * @param filename the name of the text file from which to read.
 * @param colSigs the column of the text file containing the signatures.
 * @param colLabels the column of the text file containing the packet labels
 * for the resulting knots or triangulations.  If this is negative then the
 * signatures themselves will be used as packet labels.
 * @param ignoreLines the number of lines at the beginning of the text
 * file that should be ignored completely.
 * @return a new container as described above, or \c null if an I/O error
 * occurred whilst reading the given file.
 */
template <class PacketType>
Container* readSigList(const char *filename,
    unsigned colSigs = 0, int colLabels = -1, unsigned long ignoreLines = 0);

/**
 * A deprecated weaker version of readSigList(), which reads a list of
 * isomorphism signatures for 3-manifold or 4-manifold triangulations from
 * the given text file.
 *
 * Unlike readSigList() (which is much more general), this function can only
 * work with 2-manifold, 3-manifold and 4-manifold triangulations.
 *
 * Also, unlike readSigList(), this is not a template function.  Instead of
 * the template argument \a PacketType used by readSigList() to indicate
 * what kind of object to construct, this function has an additional
 * \a dimension parameter.
 *
 * See readSigList() for further details on what this routine does.
 *
 * \deprecated Use readSigList<Triangulation<3>> or
 * readSigList<Triangulation<4>> instead.
 *
 * @param filename the name of the text file from which to read.
 * @param dimension either 2, 3 or 4, indicating whether isomorphism
 * signatures should be expanded into 2-manifold, 3-manifold or 4-manifold
 * triangulations respectively.
 * @param colSigs the column of the text file containing the signatures.
 * @param colLabels the column of the text file containing the packet labels
 * for the resulting triangulations.  If this is negative then the
 * signatures themselves will be used as packet labels.
 * @param ignoreLines the number of lines at the beginning of the text
 * file that should be ignored completely.
 * @return a new container, or \c null if an I/O error
 * occurred whilst reading the given file.
 */
[[deprecated]] Container* readIsoSigList(const char *filename,
    unsigned dimension = 3, unsigned colSigs = 0, int colLabels = -1,
    unsigned long ignoreLines = 0);

/*@}*/

} // namespace regina

#include "foreign/fromsiglist-impl.h"

#endif

