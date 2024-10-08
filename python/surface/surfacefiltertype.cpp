
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include <pybind11/pybind11.h>
#include "surface/surfacefiltertype.h"
#include "../helpers.h"
#include "../docstrings/surface/surfacefiltertype.h"

using regina::SurfaceFilterType;

void addSurfaceFilterType(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(SurfaceFilterType)

    pybind11::enum_<regina::SurfaceFilterType>(m, "SurfaceFilterType",
            rdoc_scope)
        .value("LegacyDefault", SurfaceFilterType::LegacyDefault,
            rdoc::LegacyDefault)
        .value("Properties", SurfaceFilterType::Properties, rdoc::Properties)
        .value("Combination", SurfaceFilterType::Combination, rdoc::Combination)
        ;

    // Deprecated constants:
    m.attr("NS_FILTER_LEGACY_DEFAULT") = SurfaceFilterType::LegacyDefault;
    m.attr("NS_FILTER_PROPERTIES") = SurfaceFilterType::Properties;
    m.attr("NS_FILTER_COMBINATION") = SurfaceFilterType::Combination;

    RDOC_SCOPE_END
}

