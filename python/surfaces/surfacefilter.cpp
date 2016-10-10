
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

#include "surfaces/normalsurface.h"
#include "surfaces/surfacefilter.h"
#include "../safeheldtype.h"
#include "../helpers.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::SurfaceFilter;
using regina::SurfaceFilterCombination;
using regina::SurfaceFilterProperties;

namespace {
    boost::python::list eulerChars_list(const SurfaceFilterProperties& f) {
        boost::python::list ans;
        for (auto& e : f.eulerChars())
            ans.append(e);
        return ans;
    }
}

void addSurfaceFilter() {
    {
        scope s = class_<SurfaceFilter, bases<regina::Packet>,
                SafeHeldType<SurfaceFilter>, boost::noncopyable>
                ("SurfaceFilter")
            .def(init<const SurfaceFilter&>())
            .def("accept", &SurfaceFilter::accept)
            .def("filterType", &SurfaceFilter::filterType)
            .def("filterTypeName", &SurfaceFilter::filterTypeName)
        ;

        s.attr("typeID") = regina::PACKET_SURFACEFILTER;
        s.attr("filterTypeID") = regina::NS_FILTER_DEFAULT;
    }

    class_<SurfaceFilterCombination, bases<regina::SurfaceFilter>,
            SafeHeldType<SurfaceFilterCombination>, boost::noncopyable>
            ("SurfaceFilterCombination")
        .def(init<const SurfaceFilterCombination&>())
        .def("usesAnd", &SurfaceFilterCombination::usesAnd)
        .def("setUsesAnd", &SurfaceFilterCombination::setUsesAnd)
        .attr("filterTypeID") = regina::NS_FILTER_COMBINATION;
    ;

    class_<SurfaceFilterProperties, bases<regina::SurfaceFilter>,
            SafeHeldType<SurfaceFilterProperties>, boost::noncopyable>
            ("SurfaceFilterProperties")
        .def(init<const SurfaceFilterProperties&>())
        .def("eulerChars", eulerChars_list)
        .def("countEulerChars", &SurfaceFilterProperties::countEulerChars)
        .def("eulerChar", &SurfaceFilterProperties::eulerChar)
        .def("orientability", &SurfaceFilterProperties::orientability)
        .def("compactness", &SurfaceFilterProperties::compactness)
        .def("realBoundary", &SurfaceFilterProperties::realBoundary)
        .def("addEulerChar", &SurfaceFilterProperties::addEulerChar)
        .def("removeEulerChar", &SurfaceFilterProperties::removeEulerChar)
        .def("removeAllEulerChars",
            &SurfaceFilterProperties::removeAllEulerChars)
        .def("setOrientability", &SurfaceFilterProperties::setOrientability)
        .def("setCompactness", &SurfaceFilterProperties::setCompactness)
        .def("setRealBoundary", &SurfaceFilterProperties::setRealBoundary)
        .attr("filterTypeID") = regina::NS_FILTER_PROPERTIES;
    ;

    implicitly_convertible<SafeHeldType<SurfaceFilter>,
        SafeHeldType<regina::Packet> >();

    implicitly_convertible<SafeHeldType<SurfaceFilterCombination>,
        SafeHeldType<regina::SurfaceFilter> >();

    implicitly_convertible<SafeHeldType<SurfaceFilterProperties>,
        SafeHeldType<regina::SurfaceFilter> >();

    FIX_REGINA_BOOST_CONVERTERS(SurfaceFilter);
    FIX_REGINA_BOOST_CONVERTERS(SurfaceFilterCombination);
    FIX_REGINA_BOOST_CONVERTERS(SurfaceFilterProperties);

    scope().attr("NSurfaceFilter") = scope().attr("SurfaceFilter");
    scope().attr("NSurfaceFilterCombination") =
        scope().attr("SurfaceFilterCombination");
    scope().attr("NSurfaceFilterProperties") =
        scope().attr("SurfaceFilterProperties");
}

