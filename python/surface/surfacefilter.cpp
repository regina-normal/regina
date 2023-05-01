
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
#include "../pybind11/stl.h"
#include "surface/normalsurface.h"
#include "surface/surfacefilter.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/surface/surfacefilter.h"

using regina::SurfaceFilter;
using regina::SurfaceFilterCombination;
using regina::SurfaceFilterProperties;

void addSurfaceFilter(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(SurfaceFilter)

    pybind11::class_<SurfaceFilter, regina::Packet,
            std::shared_ptr<SurfaceFilter>>(m, "SurfaceFilter", rdoc_scope)
        .def("accept", &SurfaceFilter::accept, rdoc::accept)
        .def("filterType", &SurfaceFilter::filterType, rdoc::filterType)
        .def("filterTypeName", &SurfaceFilter::filterTypeName,
            rdoc::filterTypeName)
        .def_readonly_static("typeID", &SurfaceFilter::typeID)
    ;

    RDOC_SCOPE_SWITCH(SurfaceFilterCombination)

    auto c = pybind11::class_<SurfaceFilterCombination, regina::SurfaceFilter,
            std::shared_ptr<SurfaceFilterCombination>>
            (m, "SurfaceFilterCombination", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const SurfaceFilterCombination&>(), rdoc::__copy)
        .def("swap", &SurfaceFilterCombination::swap, rdoc::swap)
        .def("usesAnd", &SurfaceFilterCombination::usesAnd, rdoc::usesAnd)
        .def("setUsesAnd", &SurfaceFilterCombination::setUsesAnd,
            rdoc::setUsesAnd)
        .def_readonly_static("filterTypeID",
            &SurfaceFilterCombination::filterTypeID)
    ;
    regina::python::add_output(c);
    regina::python::packet_eq_operators(c, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<SurfaceFilterCombination>(m,
        rdoc::global_swap);

    RDOC_SCOPE_SWITCH(SurfaceFilterProperties)

    auto p = pybind11::class_<SurfaceFilterProperties, regina::SurfaceFilter,
            std::shared_ptr<SurfaceFilterProperties>>
            (m, "SurfaceFilterProperties", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const SurfaceFilterProperties&>(), rdoc::__copy)
        .def("swap", &SurfaceFilterProperties::swap, rdoc::swap)
        .def("eulerChars", &SurfaceFilterProperties::eulerChars,
            rdoc::eulerChars)
        .def("countEulerChars", &SurfaceFilterProperties::countEulerChars,
            rdoc::countEulerChars)
        .def("eulerChar", &SurfaceFilterProperties::eulerChar, rdoc::eulerChar)
        .def("orientability", &SurfaceFilterProperties::orientability,
            rdoc::orientability)
        .def("compactness", &SurfaceFilterProperties::compactness,
            rdoc::compactness)
        .def("realBoundary", &SurfaceFilterProperties::realBoundary,
            rdoc::realBoundary)
        .def("addEulerChar", &SurfaceFilterProperties::addEulerChar,
            rdoc::addEulerChar)
        .def("removeEulerChar", &SurfaceFilterProperties::removeEulerChar,
            rdoc::removeEulerChar)
        .def("removeAllEulerChars",
            &SurfaceFilterProperties::removeAllEulerChars,
            rdoc::removeAllEulerChars)
        .def("setEulerChars", [](SurfaceFilterProperties& f,
                const std::vector<regina::LargeInteger>& v) {
            f.setEulerChars(v.begin(), v.end());
        }, pybind11::arg("euler"), rdoc::setEulerChars)
        .def("setOrientability", &SurfaceFilterProperties::setOrientability,
            rdoc::setOrientability)
        .def("setCompactness", &SurfaceFilterProperties::setCompactness,
            rdoc::setCompactness)
        .def("setRealBoundary", &SurfaceFilterProperties::setRealBoundary,
            rdoc::setRealBoundary)
        .def_readonly_static("filterTypeID",
            &SurfaceFilterProperties::filterTypeID)
    ;
    regina::python::add_output(p);
    regina::python::packet_eq_operators(p, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<SurfaceFilterProperties>(m,
        rdoc::global_swap);

    RDOC_SCOPE_END
}

