
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

#include "../pybind11/pybind11.h"
#include "surfaces/normalflags.h"
#include "../helpers.h"
#include "../flags.h"

using pybind11::overload_cast;
using regina::NormalAlg;
using regina::NormalAlgFlags;
using regina::NormalList;
using regina::NormalListFlags;

void addNormalFlags(pybind11::module_& m) {
    regina::python::add_flags<regina::NormalListFlags>(
        m, "NormalListFlags", "NormalList", {
            { "NS_LIST_DEFAULT", regina::NS_LIST_DEFAULT },
            { "NS_EMBEDDED_ONLY", regina::NS_EMBEDDED_ONLY },
            { "NS_IMMERSED_SINGULAR", regina::NS_IMMERSED_SINGULAR },
            { "NS_VERTEX", regina::NS_VERTEX },
            { "NS_FUNDAMENTAL", regina::NS_FUNDAMENTAL },
            { "NS_LEGACY", regina::NS_LEGACY },
            { "NS_CUSTOM", regina::NS_CUSTOM }
        });

    regina::python::add_flags<regina::NormalAlgFlags>(
        m, "NormalAlgFlags", "NormalAlg", {
            { "NS_ALG_DEFAULT", regina::NS_ALG_DEFAULT },
            { "NS_VERTEX_VIA_REDUCED", regina::NS_VERTEX_VIA_REDUCED },
            { "NS_VERTEX_STD_DIRECT", regina::NS_VERTEX_STD_DIRECT },
            { "NS_VERTEX_TREE", regina::NS_VERTEX_TREE },
            { "NS_VERTEX_DD", regina::NS_VERTEX_DD },
            { "NS_HILBERT_PRIMAL", regina::NS_HILBERT_PRIMAL },
            { "NS_HILBERT_DUAL", regina::NS_HILBERT_DUAL },
            { "NS_HILBERT_CD", regina::NS_HILBERT_CD },
            { "NS_HILBERT_FULLCONE", regina::NS_HILBERT_FULLCONE },
            { "NS_ALG_LEGACY", regina::NS_ALG_LEGACY },
            { "NS_ALG_CUSTOM", regina::NS_ALG_CUSTOM }
        });

    pybind11::enum_<regina::NormalTransform>(m, "NormalTransform")
        .value("NS_CONV_REDUCED_TO_STD", regina::NS_CONV_REDUCED_TO_STD)
        .value("NS_CONV_STD_TO_REDUCED", regina::NS_CONV_STD_TO_REDUCED)
        .value("NS_FILTER_COMPATIBLE", regina::NS_FILTER_COMPATIBLE)
        .value("NS_FILTER_DISJOINT", regina::NS_FILTER_DISJOINT)
        .value("NS_FILTER_INCOMPRESSIBLE", regina::NS_FILTER_INCOMPRESSIBLE)
        .export_values();
}
