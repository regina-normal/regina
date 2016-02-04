
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

/*! \file foreign/csvsurfacelist.h
 *  \brief Deprecated header that exports normal surface lists to plain text
 *  CSV files.
 *
 *  \deprecated The enumeration type SurfaceExportFields (which was once
 *  declared in this file) is now declared in surfaces/nnormalsurfacelist.h.
 *  Moreover, all global functions that were once declared in this file have
 *  now been renamed to member functions of NNormalSurfaceList.
 *  Please include surfaces/nnormalsurfacelist.h instead.
 */

#ifndef __CSVSURFACELIST_H
#ifndef __DOXYGEN
#define __CSVSURFACELIST_H
#endif

#include <iostream>
#include "regina-core.h"
#include "surfaces/nnormalsurfacelist.h"

namespace regina {

class NNormalSurfaceList;

/**
 * \weakgroup foreign
 * @{
 */

/**
 * Deprecated function that exports the given list of normal surfaces as a
 * plain text CSV (comma-separated value) file, using standard
 * triangle-quadrilateral coordinates.
 *
 * \deprecated This routine has been renamed as
 * NNormalSurfaceList::saveCSVStandard().  See the documentation for
 * NNormalSurfaceList::saveCSVStandard() for further details.
 */
REGINA_API bool writeCSVStandard(const char* filename,
    NNormalSurfaceList& surfaces, int additionalFields = surfaceExportAll);

/**
 * Deprecated function that exports the given list of normal surfaces as a
 * plain text CSV (comma-separated value) file, using edge weight coordinates.
 *
 * \deprecated This routine has been renamed as
 * NNormalSurfaceList::saveCSVEdgeWeight().  See the documentation for
 * NNormalSurfaceList::saveCSVEdgeWeight() for further details.
 */
REGINA_API bool writeCSVEdgeWeight(const char* filename,
    NNormalSurfaceList& surfaces, int additionalFields = surfaceExportAll);

/*@}*/

inline bool writeCSVStandard(const char* filename,
        NNormalSurfaceList& surfaces, int additionalFields) {
    return surfaces.saveCSVStandard(filename, additionalFields);
}

inline bool writeCSVEdgeWeight(const char* filename,
        NNormalSurfaceList& surfaces, int additionalFields) {
    return surfaces.saveCSVEdgeWeight(filename, additionalFields);
}

} // namespace regina

#endif

