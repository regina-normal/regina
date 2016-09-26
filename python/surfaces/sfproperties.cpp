
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

#include "surfaces/sfproperties.h"
#include "../safeheldtype.h"
#include "../helpers.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::NSurfaceFilterProperties;

namespace {
    boost::python::list eulerChars_list(const NSurfaceFilterProperties& f) {
        boost::python::list ans;
        for (auto& e : f.eulerChars())
            ans.append(e);
        return ans;
    }
}

void addNSurfaceFilterProperties() {
    scope s = class_<NSurfaceFilterProperties, bases<regina::NSurfaceFilter>,
            SafeHeldType<NSurfaceFilterProperties>, boost::noncopyable>
            ("NSurfaceFilterProperties")
        .def(init<const NSurfaceFilterProperties&>())
        .def("eulerChars", eulerChars_list)
        .def("getECs", eulerChars_list)
        .def("countEulerChars", &NSurfaceFilterProperties::countEulerChars)
        .def("eulerChar", &NSurfaceFilterProperties::eulerChar)
        .def("getEC", &NSurfaceFilterProperties::eulerChar)
        .def("orientability", &NSurfaceFilterProperties::orientability)
        .def("getOrientability", &NSurfaceFilterProperties::orientability)
        .def("compactness", &NSurfaceFilterProperties::compactness)
        .def("getCompactness", &NSurfaceFilterProperties::compactness)
        .def("realBoundary", &NSurfaceFilterProperties::realBoundary)
        .def("getRealBoundary", &NSurfaceFilterProperties::realBoundary)
        .def("addEulerChar", &NSurfaceFilterProperties::addEulerChar)
        .def("addEC", &NSurfaceFilterProperties::addEulerChar)
        .def("removeEulerChar", &NSurfaceFilterProperties::removeEulerChar)
        .def("removeEC", &NSurfaceFilterProperties::removeEulerChar)
        .def("removeAllEulerChars",
            &NSurfaceFilterProperties::removeAllEulerChars)
        .def("removeAllECs", &NSurfaceFilterProperties::removeAllEulerChars)
        .def("setOrientability", &NSurfaceFilterProperties::setOrientability)
        .def("setCompactness", &NSurfaceFilterProperties::setCompactness)
        .def("setRealBoundary", &NSurfaceFilterProperties::setRealBoundary)
    ;

    s.attr("filterTypeID") = regina::NS_FILTER_PROPERTIES;

    implicitly_convertible<SafeHeldType<NSurfaceFilterProperties>,
        SafeHeldType<regina::NSurfaceFilter> >();

    FIX_REGINA_BOOST_CONVERTERS(NSurfaceFilterProperties);
}

