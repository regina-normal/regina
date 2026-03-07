
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file foreign/isosig.h
 *  \brief Allows reading lists of isomorphism signatures and knot signatures.
 */

#ifndef __REGINA_ISOSIG_H
#ifndef __DOXYGEN
#define __REGINA_ISOSIG_H
#endif

#include "concepts/maths.h"
#include "packet/packet.h"
#include <memory>

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

class Container;

/**
 * Reads a list of signatures (e.g., isomorphism signatures for triangulations,
 * or knot/link signatures) from the given text file, and reconstructs the
 * corresponding mathematical objects in a new packet tree.  The file should
 * contain one signature per line.
 *
 * All signatures should be for the same type of object, which is passed as the
 * template parameter \a ObjectType.  For example, they could all be knot/link
 * signatures (if ObjectType is Link), or they could all be isomorphism
 * signatures for triangulations of some fixed dimension \a dim (if ObjectType
 * is `Triangulation<dim>`).
 *
 * A new container will be returned, and the reconstructed objects (e.g.,
 * triangulations or links) will be inserted as children of this container.
 * The container will not be assigned a label.  The individual reconstructed
 * objects will be assigned labels according to the parameter \a colLabels.
 *
 * Regarding errors:
 *
 * - Empty lines in the file are harmless: they will simply be ignored.
 *
 * - If any signatures are invalid, these will be recorded in an additional
 *   text packet that will be the last child of the container.
 *
 * - If an I/O error occurs while trying to read the given file, this routine
 *   will return `null`.
 *
 * In its simplest form, the text file can simply contain one signature per
 * line and nothing else.  However, more complex formats are allowed.
 * In particular, by passing appropriate values for the arguments \a colSigs
 * and \a colLabels, the signatures and packet labels can be taken from
 * arbitrary columns of the text file.  Columns are considered to be separated
 * by whitespace and are numbered beginning at 0.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file _name_, and
 * simply passes it through unchanged to low-level C/C++ file I/O routines.
 * It assumes however that the _contents_ of the file are in UTF-8.
 *
 * \tparam ObjectType indicates what type of object is reconstructed from each
 * signature in the file, as described above.
 *
 * \python Since Python does not support templates, the object type is
 * specified by passing an integer \a dimension as the first parameter (before
 * the filename).  This should be \a dim to specify the object type
 * `Triangulation<dim>`, or 0 to specify the object type Link.  Currenty no
 * other object type is supported in Python.
 *
 * \param filename the name of the text file from which to read.
 * \param colSigs the column of the text file containing the signatures.
 * \param colLabels the column of the text file that provides packet labels
 * for the reconstructed objects.  If this is negative then the signatures
 * themselves will be used as packet labels.
 * \param ignoreLines the number of lines at the beginning of the text
 * file that should be ignored completely.
 * \return a new container as described above, or `null` if an I/O error
 * occurred whilst reading the given file.
 *
 * \ingroup foreign
 */
template <SigReconstructible ObjectType>
requires PacketHeldType<ObjectType>
std::shared_ptr<Container> readSigList(const char *filename,
    int colSigs = 0, int colLabels = -1, size_t ignoreLines = 0);

} // namespace regina

#include "foreign/fromsiglist-impl.h"

#endif

