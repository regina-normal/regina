
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
#include "surface/normalflags.h"
#include "../helpers.h"
#include "../helpers/flags.h"
#include "../docstrings/surface/normalflags.h"

using pybind11::overload_cast;
using regina::NormalAlg;
using regina::NormalList;

void addNormalFlags(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(NormalList)

    regina::python::add_flags<regina::NormalList>(m, "NormalList", {
            { "Default", regina::NormalList::Default, rdoc::Default },
            { "EmbeddedOnly", regina::NormalList::EmbeddedOnly,
                rdoc::EmbeddedOnly },
            { "ImmersedSingular", regina::NormalList::ImmersedSingular,
                rdoc::ImmersedSingular },
            { "Vertex", regina::NormalList::Vertex, rdoc::Vertex },
            { "Fundamental", regina::NormalList::Fundamental,
                rdoc::Fundamental },
            { "Legacy", regina::NormalList::Legacy, rdoc::Legacy },
            { "Custom", regina::NormalList::Custom, rdoc::Custom }
        }, rdoc_scope, rdoc_global::__bor);

    RDOC_SCOPE_SWITCH_MAIN

    // Deprecated type alias and constants:
    m.attr("NormalListFlags") = m.attr("NormalList");
    m.attr("NS_LIST_DEFAULT") = regina::NormalList::Default;
    m.attr("NS_EMBEDDED_ONLY") = regina::NormalList::EmbeddedOnly;
    m.attr("NS_IMMERSED_SINGULAR") = regina::NormalList::ImmersedSingular;
    m.attr("NS_VERTEX") = regina::NormalList::Vertex;
    m.attr("NS_FUNDAMENTAL") = regina::NormalList::Fundamental;
    m.attr("NS_LEGACY") = regina::NormalList::Legacy;
    m.attr("NS_CUSTOM") = regina::NormalList::Custom;

    RDOC_SCOPE_SWITCH(NormalAlg)

    regina::python::add_flags<regina::NormalAlg>(m, "NormalAlg", {
            { "Default", regina::NormalAlg::Default, rdoc::Default },
            { "VertexViaReduced", regina::NormalAlg::VertexViaReduced,
                rdoc::VertexViaReduced },
            { "VertexStdDirect", regina::NormalAlg::VertexStdDirect,
                rdoc::VertexStdDirect },
            { "VertexTree", regina::NormalAlg::VertexTree, rdoc::VertexTree },
            { "VertexDD", regina::NormalAlg::VertexDD, rdoc::VertexDD },
            { "HilbertPrimal", regina::NormalAlg::HilbertPrimal,
                rdoc::HilbertPrimal },
            { "HilbertDual", regina::NormalAlg::HilbertDual,
                rdoc::HilbertDual },
            { "HilbertCD", regina::NormalAlg::HilbertCD, rdoc::HilbertCD },
            { "HilbertFullCone", regina::NormalAlg::HilbertFullCone,
                rdoc::HilbertFullCone },
            { "Legacy", regina::NormalAlg::Legacy, rdoc::Legacy },
            { "Custom", regina::NormalAlg::Custom, rdoc::Custom }
        }, rdoc_scope, rdoc_global::__bor);

    RDOC_SCOPE_SWITCH_MAIN

    // Deprecated type alias and constants:
    m.attr("NormalAlgFlags") = m.attr("NormalAlg");
    m.attr("NS_ALG_DEFAULT") = regina::NormalAlg::Default;
    m.attr("NS_VERTEX_VIA_REDUCED") = regina::NormalAlg::VertexViaReduced;
    m.attr("NS_VERTEX_STD_DIRECT") = regina::NormalAlg::VertexStdDirect;
    m.attr("NS_VERTEX_TREE") = regina::NormalAlg::VertexTree;
    m.attr("NS_VERTEX_DD") = regina::NormalAlg::VertexDD;
    m.attr("NS_HILBERT_PRIMAL") = regina::NormalAlg::HilbertPrimal;
    m.attr("NS_HILBERT_DUAL") = regina::NormalAlg::HilbertDual;
    m.attr("NS_HILBERT_CD") = regina::NormalAlg::HilbertCD;
    m.attr("NS_HILBERT_FULLCONE") = regina::NormalAlg::HilbertFullCone;
    m.attr("NS_ALG_LEGACY") = regina::NormalAlg::Legacy;
    m.attr("NS_ALG_CUSTOM") = regina::NormalAlg::Custom;

    RDOC_SCOPE_SWITCH(NormalTransform)

    pybind11::enum_<regina::NormalTransform>(m, "NormalTransform", rdoc_scope)
        .value("NS_CONV_REDUCED_TO_STD", regina::NS_CONV_REDUCED_TO_STD,
            rdoc::NS_CONV_REDUCED_TO_STD)
        .value("NS_CONV_STD_TO_REDUCED", regina::NS_CONV_STD_TO_REDUCED,
            rdoc::NS_CONV_STD_TO_REDUCED)
        .value("NS_FILTER_COMPATIBLE", regina::NS_FILTER_COMPATIBLE,
            rdoc::NS_FILTER_COMPATIBLE)
        .value("NS_FILTER_DISJOINT", regina::NS_FILTER_DISJOINT,
            rdoc::NS_FILTER_DISJOINT)
        .value("NS_FILTER_INCOMPRESSIBLE", regina::NS_FILTER_INCOMPRESSIBLE,
            rdoc::NS_FILTER_INCOMPRESSIBLE)
        .export_values();

    RDOC_SCOPE_END
}
