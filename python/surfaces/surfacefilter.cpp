
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include "../pybind11/stl.h"
#include "surfaces/normalsurface.h"
#include "surfaces/surfacefilter.h"
#include "utilities/safeptr.h"
#include "../helpers.h"

using regina::SurfaceFilter;
using regina::SurfaceFilterCombination;
using regina::SurfaceFilterProperties;

void addSurfaceFilter(pybind11::module_& m) {
    pybind11::class_<SurfaceFilter, regina::Packet,
            regina::SafePtr<SurfaceFilter>>(m, "SurfaceFilter")
        .def(pybind11::init<>())
        .def(pybind11::init<const SurfaceFilter&>())
        .def("accept", &SurfaceFilter::accept)
        .def("filterType", &SurfaceFilter::filterType)
        .def("filterTypeName", &SurfaceFilter::filterTypeName)
        .def_property_readonly_static("typeID", [](pybind11::object) {
            // We cannot take the address of typeID, so use a getter function.
            return SurfaceFilter::typeID;
        })
        .def_property_readonly_static("filterTypeID", [](pybind11::object) {
            // Likewise for filterTypeID.
            return SurfaceFilter::filterTypeID;
        })
    ;

    pybind11::class_<SurfaceFilterCombination, regina::SurfaceFilter,
            regina::SafePtr<SurfaceFilterCombination>>
            (m, "SurfaceFilterCombination")
        .def(pybind11::init<>())
        .def(pybind11::init<const SurfaceFilterCombination&>())
        .def("usesAnd", &SurfaceFilterCombination::usesAnd)
        .def("setUsesAnd", &SurfaceFilterCombination::setUsesAnd)
        .def_property_readonly_static("filterTypeID", [](pybind11::object) {
            return SurfaceFilterCombination::filterTypeID;
        })
    ;

    pybind11::class_<SurfaceFilterProperties, regina::SurfaceFilter,
            regina::SafePtr<SurfaceFilterProperties>>
            (m, "SurfaceFilterProperties")
        .def(pybind11::init<>())
        .def(pybind11::init<const SurfaceFilterProperties&>())
        .def("eulerChars", &SurfaceFilterProperties::eulerChars)
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
        .def_property_readonly_static("filterTypeID", [](pybind11::object) {
            return SurfaceFilterProperties::filterTypeID;
        })
    ;

    m.attr("NSurfaceFilter") = m.attr("SurfaceFilter");
    m.attr("NSurfaceFilterCombination") = m.attr("SurfaceFilterCombination");
    m.attr("NSurfaceFilterProperties") = m.attr("SurfaceFilterProperties");
}

