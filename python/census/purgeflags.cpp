
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include "census/purgeflags.h"
#include "../helpers.h"
#include "../helpers/flags.h"
#include "../docstrings/census/purgeflags.h"

void addPurgeFlags(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(CensusPurge)

    regina::python::add_flags<regina::CensusPurge, 2 /* hex digits */>(
        m, "CensusPurge", {
            // Note: we cannot use "None", since this is reserved in Python.
            { "Nil", regina::CensusPurge::None, rdoc::None },
            { "NonMinimal", regina::CensusPurge::NonMinimal, rdoc::NonMinimal },
            { "NonPrime", regina::CensusPurge::NonPrime, rdoc::NonPrime },
            { "NonMinimalPrime", regina::CensusPurge::NonMinimalPrime,
                rdoc::NonMinimalPrime },
            { "NonMinimalHyp", regina::CensusPurge::NonMinimalHyp,
                rdoc::NonMinimalHyp },
            { "P2Reducible", regina::CensusPurge::P2Reducible,
                rdoc::P2Reducible }
        }, rdoc_scope, rdoc_global::__bor);

    RDOC_SCOPE_SWITCH_MAIN

    // Deprecated type alias and constants:
    m.attr("CensusPurgeFlags") = m.attr("CensusPurge");
    m.attr("PURGE_NONE") = regina::CensusPurge::None;
    m.attr("PURGE_NON_MINIMAL") = regina::CensusPurge::NonMinimal;
    m.attr("PURGE_NON_PRIME") = regina::CensusPurge::NonPrime;
    m.attr("PURGE_NON_MINIMAL_PRIME") = regina::CensusPurge::NonMinimalPrime;
    m.attr("PURGE_NON_MINIMAL_HYP") = regina::CensusPurge::NonMinimalHyp;
    m.attr("PURGE_P2_REDUCIBLE") = regina::CensusPurge::P2Reducible;

    RDOC_SCOPE_END
}
