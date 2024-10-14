
/**************************************************************************
 *                                                                        *
 *  Regina - A Hyper Surface Theory Calculator                           *
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
#include "hypersurface/hyperflags.h"
#include "../helpers.h"
#include "../helpers/flags.h"
#include "../docstrings/hypersurface/hyperflags.h"

void addHyperFlags(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(HyperList)

    regina::python::add_flags<regina::HyperList>(m, "HyperList", {
            { "Default", regina::HyperList::Default, rdoc::Default },
            { "EmbeddedOnly", regina::HyperList::EmbeddedOnly,
                rdoc::EmbeddedOnly },
            { "ImmersedSingular", regina::HyperList::ImmersedSingular,
                rdoc::ImmersedSingular },
            { "Vertex", regina::HyperList::Vertex, rdoc::Vertex },
            { "Fundamental", regina::HyperList::Fundamental,
                rdoc::Fundamental },
            { "Legacy", regina::HyperList::Legacy, rdoc::Legacy },
            { "Custom", regina::HyperList::Custom, rdoc::Custom }
        }, rdoc_scope, rdoc_global::__bor);

    RDOC_SCOPE_SWITCH_MAIN

    // Deprecated type alias and constants:
    m.attr("HyperListFlags") = m.attr("HyperList");
    m.attr("HS_LIST_DEFAULT") = regina::HyperList::Default;
    m.attr("HS_EMBEDDED_ONLY") = regina::HyperList::EmbeddedOnly;
    m.attr("HS_IMMERSED_SINGULAR") = regina::HyperList::ImmersedSingular;
    m.attr("HS_VERTEX") = regina::HyperList::Vertex;
    m.attr("HS_FUNDAMENTAL") = regina::HyperList::Fundamental;
    m.attr("HS_LEGACY") = regina::HyperList::Legacy;
    m.attr("HS_CUSTOM") = regina::HyperList::Custom;

    RDOC_SCOPE_SWITCH(HyperAlg)

    regina::python::add_flags<regina::HyperAlg>(m, "HyperAlg", {
            { "Default", regina::HyperAlg::Default, rdoc::Default },
            { "VertexDD", regina::HyperAlg::VertexDD, rdoc::VertexDD },
            { "HilbertPrimal", regina::HyperAlg::HilbertPrimal,
                rdoc::HilbertPrimal },
            { "HilbertDual", regina::HyperAlg::HilbertDual, rdoc::HilbertDual },
            { "Legacy", regina::HyperAlg::Legacy, rdoc::Legacy },
            { "Custom", regina::HyperAlg::Custom, rdoc::Custom }
        }, rdoc_scope, rdoc_global::__bor_2);

    RDOC_SCOPE_SWITCH_MAIN

    // Deprecated type alias and constants:
    m.attr("HyperAlgFlags") = m.attr("HyperAlg");
    m.attr("HS_ALG_DEFAULT") = regina::HyperAlg::Default;
    m.attr("HS_VERTEX_DD") = regina::HyperAlg::VertexDD;
    m.attr("HS_HILBERT_PRIMAL") = regina::HyperAlg::HilbertPrimal;
    m.attr("HS_HILBERT_DUAL") = regina::HyperAlg::HilbertDual;
    m.attr("HS_ALG_LEGACY") = regina::HyperAlg::Legacy;
    m.attr("HS_ALG_CUSTOM") = regina::HyperAlg::Custom;

    RDOC_SCOPE_END
}
