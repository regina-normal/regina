
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "subcomplex/pluggedtorusbundle.h"
#include "subcomplex/satregion.h"
#include "subcomplex/txicore.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using namespace boost::python;
using regina::PluggedTorusBundle;

void addPluggedTorusBundle() {
    class_<PluggedTorusBundle, bases<regina::StandardTriangulation>,
            std::auto_ptr<PluggedTorusBundle>, boost::noncopyable>
            ("PluggedTorusBundle", no_init)
        .def("bundle", &PluggedTorusBundle::bundle,
            return_value_policy<reference_existing_object>())
        .def("bundleIso", &PluggedTorusBundle::bundleIso,
            return_internal_reference<>())
        .def("region", &PluggedTorusBundle::region,
            return_internal_reference<>())
        .def("matchingReln", &PluggedTorusBundle::matchingReln,
            return_internal_reference<>())
        .def("isPluggedTorusBundle", &PluggedTorusBundle::isPluggedTorusBundle,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_eq_operators())
        .staticmethod("isPluggedTorusBundle")
    ;

    implicitly_convertible<std::auto_ptr<PluggedTorusBundle>,
        std::auto_ptr<regina::StandardTriangulation> >();

    scope().attr("NPluggedTorusBundle") = scope().attr("PluggedTorusBundle");
}

