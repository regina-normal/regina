
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
using regina::NormalTransform;

void addNormalFlags(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(NormalList)

    regina::python::add_flags<NormalList>(m, "NormalList", {
            { "Default", NormalList::Default, rdoc::Default },
            { "EmbeddedOnly", NormalList::EmbeddedOnly, rdoc::EmbeddedOnly },
            { "ImmersedSingular", NormalList::ImmersedSingular,
                rdoc::ImmersedSingular },
            { "Vertex", NormalList::Vertex, rdoc::Vertex },
            { "Fundamental", NormalList::Fundamental, rdoc::Fundamental },
            { "Legacy", NormalList::Legacy, rdoc::Legacy },
            { "Custom", NormalList::Custom, rdoc::Custom }
        }, rdoc_scope, rdoc_global::__bor);

    RDOC_SCOPE_SWITCH_MAIN

    // Deprecated type alias and constants:
    m.attr("NormalListFlags") = m.attr("NormalList");
    m.attr("NS_LIST_DEFAULT") = NormalList::Default;
    m.attr("NS_EMBEDDED_ONLY") = NormalList::EmbeddedOnly;
    m.attr("NS_IMMERSED_SINGULAR") = NormalList::ImmersedSingular;
    m.attr("NS_VERTEX") = NormalList::Vertex;
    m.attr("NS_FUNDAMENTAL") = NormalList::Fundamental;
    m.attr("NS_LEGACY") = NormalList::Legacy;
    m.attr("NS_CUSTOM") = NormalList::Custom;

    RDOC_SCOPE_SWITCH(NormalAlg)

    regina::python::add_flags<NormalAlg>(m, "NormalAlg", {
            { "Default", NormalAlg::Default, rdoc::Default },
            { "VertexViaReduced", NormalAlg::VertexViaReduced,
                rdoc::VertexViaReduced },
            { "VertexStdDirect", NormalAlg::VertexStdDirect,
                rdoc::VertexStdDirect },
            { "VertexTree", NormalAlg::VertexTree, rdoc::VertexTree },
            { "VertexDD", NormalAlg::VertexDD, rdoc::VertexDD },
            { "HilbertPrimal", NormalAlg::HilbertPrimal, rdoc::HilbertPrimal },
            { "HilbertDual", NormalAlg::HilbertDual, rdoc::HilbertDual },
            { "HilbertCD", NormalAlg::HilbertCD, rdoc::HilbertCD },
            { "HilbertFullCone", NormalAlg::HilbertFullCone,
                rdoc::HilbertFullCone },
            { "Legacy", NormalAlg::Legacy, rdoc::Legacy },
            { "Custom", NormalAlg::Custom, rdoc::Custom }
        }, rdoc_scope, rdoc_global::__bor);

    RDOC_SCOPE_SWITCH_MAIN

    // Deprecated type alias and constants:
    m.attr("NormalAlgFlags") = m.attr("NormalAlg");
    m.attr("NS_ALG_DEFAULT") = NormalAlg::Default;
    m.attr("NS_VERTEX_VIA_REDUCED") = NormalAlg::VertexViaReduced;
    m.attr("NS_VERTEX_STD_DIRECT") = NormalAlg::VertexStdDirect;
    m.attr("NS_VERTEX_TREE") = NormalAlg::VertexTree;
    m.attr("NS_VERTEX_DD") = NormalAlg::VertexDD;
    m.attr("NS_HILBERT_PRIMAL") = NormalAlg::HilbertPrimal;
    m.attr("NS_HILBERT_DUAL") = NormalAlg::HilbertDual;
    m.attr("NS_HILBERT_CD") = NormalAlg::HilbertCD;
    m.attr("NS_HILBERT_FULLCONE") = NormalAlg::HilbertFullCone;
    m.attr("NS_ALG_LEGACY") = NormalAlg::Legacy;
    m.attr("NS_ALG_CUSTOM") = NormalAlg::Custom;

    RDOC_SCOPE_SWITCH(NormalTransform)

    pybind11::enum_<NormalTransform>(m, "NormalTransform", rdoc_scope)
        .value("ConvertReducedToStandard",
            NormalTransform::ConvertReducedToStandard,
            rdoc::ConvertReducedToStandard)
        .value("ConvertStandardToReduced",
            NormalTransform::ConvertStandardToReduced,
            rdoc::ConvertStandardToReduced)
        .value("FilterCompatible", NormalTransform::FilterCompatible,
            rdoc::FilterCompatible)
        .value("FilterDisjoint", NormalTransform::FilterDisjoint,
            rdoc::FilterDisjoint)
        .value("FilterIncompressible", NormalTransform::FilterIncompressible,
            rdoc::FilterIncompressible)
        ;

    // Deprecated constants:
    m.attr("NS_CONV_REDUCED_TO_STD") =
        NormalTransform::ConvertReducedToStandard;
    m.attr("NS_CONV_STD_TO_REDUCED") =
        NormalTransform::ConvertStandardToReduced;
    m.attr("NS_FILTER_COMPATIBLE") = NormalTransform::FilterCompatible;
    m.attr("NS_FILTER_DISJOINT") = NormalTransform::FilterDisjoint;
    m.attr("NS_FILTER_INCOMPRESSIBLE") = NormalTransform::FilterIncompressible;

    RDOC_SCOPE_END
}
