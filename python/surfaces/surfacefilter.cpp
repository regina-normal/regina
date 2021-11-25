
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
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::SurfaceFilter;
using regina::SurfaceFilterCombination;
using regina::SurfaceFilterProperties;

void addSurfaceFilter(pybind11::module_& m) {
    pybind11::class_<SurfaceFilter, regina::Packet,
            std::shared_ptr<SurfaceFilter>>(m, "SurfaceFilter")
        .def("accept", &SurfaceFilter::accept)
        .def("filterType", &SurfaceFilter::filterType)
        .def("filterTypeName", &SurfaceFilter::filterTypeName)
        .def_readonly_static("typeID", &SurfaceFilter::typeID)
    ;

    pybind11::class_<SurfaceFilterCombination, regina::SurfaceFilter,
            std::shared_ptr<SurfaceFilterCombination>>
            (m, "SurfaceFilterCombination")
        .def(pybind11::init<>())
        .def(pybind11::init<const SurfaceFilterCombination&>())
        .def("swap", &SurfaceFilterCombination::swap)
        .def("usesAnd", &SurfaceFilterCombination::usesAnd)
        .def("setUsesAnd", &SurfaceFilterCombination::setUsesAnd)
        .def_readonly_static("filterTypeID",
            &SurfaceFilterCombination::filterTypeID)
    ;

    m.def("swap", (void(*)(SurfaceFilterCombination&,
        SurfaceFilterCombination&))(regina::swap));

    pybind11::class_<SurfaceFilterProperties, regina::SurfaceFilter,
            std::shared_ptr<SurfaceFilterProperties>>
            (m, "SurfaceFilterProperties")
        .def(pybind11::init<>())
        .def(pybind11::init<const SurfaceFilterProperties&>())
        .def("swap", &SurfaceFilterProperties::swap)
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
        .def_readonly_static("filterTypeID",
            &SurfaceFilterProperties::filterTypeID)
    ;

    m.def("swap", (void(*)(SurfaceFilterProperties&,
        SurfaceFilterProperties&))(regina::swap));
}

