
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

#include "surfaces/nnormalsurface.h"
#include "surfaces/nsurfacefilter.h"
#include "../safeheldtype.h"
#include "../helpers.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::NSurfaceFilter;
using regina::NSurfaceFilterCombination;
using regina::NSurfaceFilterProperties;

namespace {
    boost::python::list eulerChars_list(const NSurfaceFilterProperties& f) {
        boost::python::list ans;
        for (auto& e : f.eulerChars())
            ans.append(e);
        return ans;
    }
}

void addNSurfaceFilter() {
    {
        scope s = class_<NSurfaceFilter, bases<regina::Packet>,
                SafeHeldType<NSurfaceFilter>, boost::noncopyable>
                ("NSurfaceFilter")
            .def(init<const NSurfaceFilter&>())
            .def("accept", &NSurfaceFilter::accept)
            .def("filterType", &NSurfaceFilter::filterType)
            .def("filterTypeName", &NSurfaceFilter::filterTypeName)
        ;

        s.attr("typeID") = regina::PACKET_SURFACEFILTER;
        s.attr("filterTypeID") = regina::NS_FILTER_DEFAULT;
    }

    class_<NSurfaceFilterCombination, bases<regina::NSurfaceFilter>,
            SafeHeldType<NSurfaceFilterCombination>, boost::noncopyable>
            ("NSurfaceFilterCombination")
        .def(init<const NSurfaceFilterCombination&>())
        .def("usesAnd", &NSurfaceFilterCombination::usesAnd)
        .def("setUsesAnd", &NSurfaceFilterCombination::setUsesAnd)
        .attr("filterTypeID") = regina::NS_FILTER_COMBINATION;
    ;

    class_<NSurfaceFilterProperties, bases<regina::NSurfaceFilter>,
            SafeHeldType<NSurfaceFilterProperties>, boost::noncopyable>
            ("NSurfaceFilterProperties")
        .def(init<const NSurfaceFilterProperties&>())
        .def("eulerChars", eulerChars_list)
        .def("countEulerChars", &NSurfaceFilterProperties::countEulerChars)
        .def("eulerChar", &NSurfaceFilterProperties::eulerChar)
        .def("orientability", &NSurfaceFilterProperties::orientability)
        .def("compactness", &NSurfaceFilterProperties::compactness)
        .def("realBoundary", &NSurfaceFilterProperties::realBoundary)
        .def("addEulerChar", &NSurfaceFilterProperties::addEulerChar)
        .def("removeEulerChar", &NSurfaceFilterProperties::removeEulerChar)
        .def("removeAllEulerChars",
            &NSurfaceFilterProperties::removeAllEulerChars)
        .def("setOrientability", &NSurfaceFilterProperties::setOrientability)
        .def("setCompactness", &NSurfaceFilterProperties::setCompactness)
        .def("setRealBoundary", &NSurfaceFilterProperties::setRealBoundary)
        .attr("filterTypeID") = regina::NS_FILTER_PROPERTIES;
    ;

    implicitly_convertible<SafeHeldType<NSurfaceFilter>,
        SafeHeldType<regina::Packet> >();

    implicitly_convertible<SafeHeldType<NSurfaceFilterCombination>,
        SafeHeldType<regina::NSurfaceFilter> >();

    implicitly_convertible<SafeHeldType<NSurfaceFilterProperties>,
        SafeHeldType<regina::NSurfaceFilter> >();

    FIX_REGINA_BOOST_CONVERTERS(NSurfaceFilter);
    FIX_REGINA_BOOST_CONVERTERS(NSurfaceFilterCombination);
    FIX_REGINA_BOOST_CONVERTERS(NSurfaceFilterProperties);
}

