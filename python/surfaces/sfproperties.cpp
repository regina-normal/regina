
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include "surfaces/sfproperties.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NSurfaceFilterProperties;

namespace {
    boost::python::list getECs_list(const NSurfaceFilterProperties& f) {
        const std::set<regina::NLargeInteger>& ECs = f.getECs();
        std::set<regina::NLargeInteger>::const_iterator it;

        boost::python::list ans;
        for (it = ECs.begin(); it != ECs.end(); it++)
            ans.append(*it);
        return ans;
    }
}

void addNSurfaceFilterProperties() {
    scope s = class_<NSurfaceFilterProperties, bases<regina::NSurfaceFilter>,
            std::auto_ptr<NSurfaceFilterProperties>, boost::noncopyable>
            ("NSurfaceFilterProperties")
        .def(init<const NSurfaceFilterProperties&>())
        .def("getECs", getECs_list)
        .def("getNumberOfECs", &NSurfaceFilterProperties::getNumberOfECs)
        .def("getEC", &NSurfaceFilterProperties::getEC)
        .def("getOrientability", &NSurfaceFilterProperties::getOrientability)
        .def("getCompactness", &NSurfaceFilterProperties::getCompactness)
        .def("getRealBoundary", &NSurfaceFilterProperties::getRealBoundary)
        .def("addEC", &NSurfaceFilterProperties::addEC)
        .def("removeEC", &NSurfaceFilterProperties::removeEC)
        .def("removeAllECs", &NSurfaceFilterProperties::removeAllECs)
        .def("setOrientability", &NSurfaceFilterProperties::setOrientability)
        .def("setCompactness", &NSurfaceFilterProperties::setCompactness)
        .def("setRealBoundary", &NSurfaceFilterProperties::setRealBoundary)
    ;

    s.attr("filterID") = NSurfaceFilterProperties::filterID;

    implicitly_convertible<std::auto_ptr<NSurfaceFilterProperties>,
        std::auto_ptr<regina::NSurfaceFilter> >();
}

