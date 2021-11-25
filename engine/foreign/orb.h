
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

/*! \file foreign/orb.h
 *  \brief Allows reading Orb / Casson triangulation files.
 */

#ifndef __REGINA_ORB_H
#ifndef __DOXYGEN
#define __REGINA_ORB_H
#endif

#include <memory>
#include <string>
#include "regina-core.h"
#include "triangulation/forward.h"

namespace regina {

template <typename> class PacketOf;

/**
 * Reads a triangulation from the given Orb / Casson file.  A new
 * triangulation packet will be returned.
 *
 * The packet label of the new triangulation will be the manifold name
 * read from the second line of the Orb / Casson file.  The first line
 * of the Orb / Casson file must simply be ``<tt>% orb</tt>''.
 *
 * If the file could not be read or if the data was not in the correct
 * format, \c null will be returned.
 *
 * \i18n This routine makes no assumptions about the
 * \ref i18n "character encoding" used in the given file \e name, and
 * simply passes it through unchanged to low-level C/C++ file I/O routines.
 * It assumes however that the \e contents of the file are in UTF-8.
 *
 * @param filename the name of the Orb / Casson file from which to read.
 * @return a new triangulation packet containing the data read from the
 * Orb / Casson file, or \c null on error.
 *
 * @author Ryan Budney, also with code from Damien Heard
 *
 * \ingroup foreign
 */
std::shared_ptr<PacketOf<Triangulation<3>>> readOrb(const char *filename);

} // namespace regina

#endif

