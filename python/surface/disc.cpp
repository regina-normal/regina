
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
#include "surface/disc.h"
#include "surface/normalsurface.h"
#include "../helpers.h"
#include "../docstrings/surface/disc.h"

using regina::DiscSpec;
using regina::DiscSetTet;
using regina::DiscSetSurface;
using regina::DiscSpecIterator;

void addDisc(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(DiscSpec)

    auto d = pybind11::class_<DiscSpec>(m, "DiscSpec", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<size_t, int, unsigned long>(), rdoc::__init)
        .def(pybind11::init<const DiscSpec&>(), rdoc::__copy)
        .def_readwrite("tetIndex", &DiscSpec::tetIndex)
        .def_readwrite("type", &DiscSpec::type)
        .def_readwrite("number", &DiscSpec::number)
    ;
    regina::python::add_output_ostream(d);
    regina::python::add_eq_operators(d, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_SWITCH_MAIN

    m.def("numberDiscsAwayFromVertex", regina::numberDiscsAwayFromVertex,
        rdoc::numberDiscsAwayFromVertex);
    m.def("discOrientationFollowsEdge", regina::discOrientationFollowsEdge,
        rdoc::discOrientationFollowsEdge);

    RDOC_SCOPE_SWITCH(DiscSetTet)

    auto t = pybind11::class_<DiscSetTet>(m, "DiscSetTet", rdoc_scope)
        .def(pybind11::init<const regina::NormalSurface&, size_t>(),
            rdoc::__init)
        .def(pybind11::init<unsigned long, unsigned long, unsigned long,
            unsigned long, unsigned long, unsigned long, unsigned long,
            unsigned long, unsigned long, unsigned long>(),
            pybind11::arg("tri0"), pybind11::arg("tri1"), pybind11::arg("tri2"),
            pybind11::arg("tri3"), pybind11::arg("quad0"),
            pybind11::arg("quad1"), pybind11::arg("quad2"),
            pybind11::arg("oct0") = 0, pybind11::arg("oct1") = 0,
            pybind11::arg("oct2") = 0,
            rdoc::__init_2)
        .def(pybind11::init<const DiscSetTet&>(), rdoc::__copy)
        .def("nDiscs", &DiscSetTet::nDiscs, rdoc::nDiscs)
        .def("arcFromDisc", &DiscSetTet::arcFromDisc, rdoc::arcFromDisc)
        .def("discFromArc", &DiscSetTet::discFromArc, rdoc::discFromArc)
    ;
    regina::python::add_output_custom(t, [](const DiscSetTet& d,
            std::ostream& s) {
        s << '(';
        for (int i = 0; i < 10; ++i)
            s << ' ' << d.nDiscs(i);
        s << " )";
    });
    regina::python::add_eq_operators(t, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_SWITCH(DiscSetSurfaceDataImpl)

    auto s = pybind11::class_<DiscSetSurface>(m, "DiscSetSurface", rdoc_scope)
        .def(pybind11::init<const regina::NormalSurface&>(), rdoc::__init)
        .def(pybind11::init<const DiscSetSurface&>(), rdoc::__copy)
        .def("swap", &DiscSetSurface::swap, rdoc::swap)
        .def("nTets", &DiscSetSurface::nTets, rdoc::nTets)
        .def("nDiscs", &DiscSetSurface::nDiscs, rdoc::nDiscs)
        .def("tetDiscs", &DiscSetSurface::tetDiscs,
            pybind11::return_value_policy::reference_internal, rdoc::tetDiscs)
        .def("adjacentDisc", &DiscSetSurface::adjacentDisc, rdoc::adjacentDisc)
        .def("__iter__", [](const DiscSetSurface& d) {
            return DiscSpecIterator(d);
        }, pybind11::keep_alive<0, 1>(), // iterator keeps disc set alive
            rdoc::__iter__)
    ;
    regina::python::add_output_custom(s, [](const DiscSetSurface& d,
            std::ostream& s) {
        s << '(';
        for (size_t i = 0; i < d.nTets(); ++i) {
            if (i > 0)
                s << " |";
            for (int j = 0; j < 10; ++j)
                s << ' ' << d.nDiscs(i, j);
        }
        s << " )";
    });
    regina::python::add_eq_operators(s, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<DiscSetSurface>(m, rdoc::global_swap);

    RDOC_SCOPE_SWITCH(DiscSpecIterator)

    auto it = pybind11::class_<DiscSpecIterator<DiscSetTet>>(
            m, "DiscSpecIterator", rdoc_scope)
        .def("__next__", [](DiscSpecIterator<DiscSetTet>& it) {
            if (! it.done())
                return *it++;
            else
                throw pybind11::stop_iteration();
        }, rdoc::__next__);
    regina::python::add_eq_operators(it, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_END
}

