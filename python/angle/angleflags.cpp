
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

#include "../pybind11/pybind11.h"
#include "angle/angleflags.h"
#include "../helpers.h"
#include "../helpers/flags.h"
#include "../docstrings/angle/angleflags.h"

void addAngleFlags(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(AngleAlgFlags)

    regina::python::add_flags<regina::AngleAlgFlags>(
        m, "AngleAlgFlags", "AngleAlg", {
            { "AS_ALG_DEFAULT", regina::AS_ALG_DEFAULT, rdoc::AS_ALG_DEFAULT },
            { "AS_ALG_TREE", regina::AS_ALG_TREE, rdoc::AS_ALG_TREE },
            { "AS_ALG_DD", regina::AS_ALG_DD, rdoc::AS_ALG_DD },
            { "AS_ALG_LEGACY", regina::AS_ALG_LEGACY, rdoc::AS_ALG_LEGACY },
            { "AS_ALG_CUSTOM", regina::AS_ALG_CUSTOM, rdoc::AS_ALG_CUSTOM }
        }, rdoc_scope, rdoc_global::__bor);

    RDOC_SCOPE_END
}
