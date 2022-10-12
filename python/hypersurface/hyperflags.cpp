
/**************************************************************************
 *                                                                        *
 *  Regina - A Hyper Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "hypersurface/hyperflags.h"
#include "../helpers.h"
#include "../flags.h"
#include "../docstrings/hypersurface/hyperflags.h"

void addHyperFlags(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(HyperListFlags)

    regina::python::add_flags<regina::HyperListFlags>(
        m, "HyperListFlags", "HyperList", {
            { "HS_LIST_DEFAULT", regina::HS_LIST_DEFAULT,
                rdoc::HS_LIST_DEFAULT },
            { "HS_EMBEDDED_ONLY", regina::HS_EMBEDDED_ONLY,
                rdoc::HS_EMBEDDED_ONLY },
            { "HS_IMMERSED_SINGULAR", regina::HS_IMMERSED_SINGULAR,
                rdoc::HS_IMMERSED_SINGULAR },
            { "HS_VERTEX", regina::HS_VERTEX, rdoc::HS_VERTEX },
            { "HS_FUNDAMENTAL", regina::HS_FUNDAMENTAL, rdoc::HS_FUNDAMENTAL },
            { "HS_LEGACY", regina::HS_LEGACY, rdoc::HS_LEGACY },
            { "HS_CUSTOM", regina::HS_CUSTOM, rdoc::HS_CUSTOM }
        }, rdoc_scope, rdoc_global::__bor);

    RDOC_SCOPE_SWITCH(HyperAlgFlags)

    regina::python::add_flags<regina::HyperAlgFlags>(
        m, "HyperAlgFlags", "HyperAlg", {
            { "HS_ALG_DEFAULT", regina::HS_ALG_DEFAULT, rdoc::HS_ALG_DEFAULT },
            { "HS_VERTEX_DD", regina::HS_VERTEX_DD, rdoc::HS_VERTEX_DD },
            { "HS_HILBERT_PRIMAL", regina::HS_HILBERT_PRIMAL,
                rdoc::HS_HILBERT_PRIMAL },
            { "HS_HILBERT_DUAL", regina::HS_HILBERT_DUAL,
                rdoc::HS_HILBERT_DUAL },
            { "HS_ALG_LEGACY", regina::HS_ALG_LEGACY, rdoc::HS_ALG_LEGACY },
            { "HS_ALG_CUSTOM", regina::HS_ALG_CUSTOM, rdoc::HS_ALG_CUSTOM }
        }, rdoc_scope, rdoc_global::__bor_2);

    RDOC_SCOPE_END
}
