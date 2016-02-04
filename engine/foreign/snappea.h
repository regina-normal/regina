
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

/*! \file foreign/snappea.h
 *  \brief Deprecated header that allows reading and writing SnapPea files.
 *
 *  \deprecated All global functions that were once declared in this
 *  file have now been renamed to member functions of NSnapPeaTriangulation.
 */

#ifndef __SNAPPEA_H
#ifndef __DOXYGEN
#define __SNAPPEA_H
#endif

#include <string>
#include "regina-core.h"

namespace regina {

template <int> class Triangulation;
typedef Triangulation<3> NTriangulation;

/**
 * \addtogroup foreign Foreign File Formats
 * Reading and writing foreign file formats.
 * @{
 */

/**
 * Deprecated function that reads the given SnapPea data file and converts
 * the result to Regina's NTriangulation class (thereby losing some
 * SnapPea-specific information in the process).
 *
 * If the file could not be read or if the data was not in the correct
 * format, 0 will be returned.  Otherwise a newly allocated triangulation
 * will be returned, and it is the user's responsibility to deallocate this
 * when it is finished with.
 *
 * \deprecated You should use the NSnapPeaTriangulation constructor
 * NSnapPeaTriangulation::NSnapPeaTriangulation(const std::string&)
 * instead.  In situations where this routine would have returned a
 * null pointer, the NSnapPeaTriangulation constructor will instead
 * create a null triangulation (i.e., one for which isNull() returns \c true).
 * See the NSnapPeaTriangulation constructor for further details.
 *
 * \warning Since this routine returns an NTriangulation (Regina's native data
 * type), it will lose any SnapPea-specific information (such as peripheral
 * curves).  It is strongly recommended that you use the NSnapPeaTriangulation
 * constructor instead (as described above), since this will return an
 * NSnapPeaTriangulation object that preserves SnapPea's additional data.
 */
REGINA_API NTriangulation* readSnapPea(const char *filename);

/**
 * Deprecated function that reads the contents of a SnapPea data file from
 * the given input stream, and converts the result to Regina's NTriangulation
 * class (thereby losing some SnapPea-specific information in the process).
 *
 * If the input stream could not be read or if the data was not in the correct
 * format, 0 will be returned.  Otherwise a newly allocated triangulation will
 * be returned, and it is the user's responsibility to deallocate this when
 * it is finished with.
 *
 * \deprecated You should use the NSnapPeaTriangulation constructor
 * NSnapPeaTriangulation::NSnapPeaTriangulation(const std::string&)
 * instead (you will need to pass the contents of the input stream, not
 * the input stream itself).  In situations where this routine would have
 * returned a null pointer, the NSnapPeaTriangulation constructor will instead
 * create a null triangulation (i.e., one for which isNull() returns \c true).
 * See the NSnapPeaTriangulation constructor for further details.
 *
 * \warning Since this routine returns an NTriangulation (Regina's native data
 * type), it will lose any SnapPea-specific information (such as peripheral
 * curves).  It is strongly recommended that you use the NSnapPeaTriangulation
 * constructor instead (as described above), since this will return an
 * NSnapPeaTriangulation object that preserves SnapPea's additional data.
 *
 * \ifacespython Not present.
 */
REGINA_API NTriangulation* readSnapPea(std::istream& in);

/**
 * Deprecated function that writes the given triangulation to the given file
 * using SnapPea's native file format.
 *
 * \deprecated This routine has been renamed as NTriangulation::saveSnapPea().
 * See the documentation of NTriangulation::saveSnapPea() for further details.
 */
REGINA_API bool writeSnapPea(const char* filename, const NTriangulation& tri);

/**
 * Deprecated function that writes the given triangulation to the
 * given output stream using SnapPea's native file format.
 *
 * \deprecated This routine has been renamed as
 * NTriangulation::snapPea(std::ostream&).  See the documentation for
 * NTriangulation::snapPea(std::ostream&) for further information.
 *
 * \ifacespython Not present.
 */
REGINA_API void writeSnapPea(std::ostream& out, const NTriangulation& tri);

/*@}*/

} // namespace regina

#endif

