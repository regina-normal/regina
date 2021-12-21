
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
#include "surface/disc.h"
#include "surface/normalsurface.h"
#include "../helpers.h"

using regina::DiscSpec;
using regina::DiscSetTet;
using regina::DiscSetSurface;
using regina::DiscSpecIterator;

namespace {
    // Support for iterables and iterators:
    template <typename TetData>
    DiscSpec nextDiscSpec(DiscSpecIterator<TetData>& it) {
        if (! it.done())
            return *it++;
        else
            throw pybind11::stop_iteration();
    }
}
void addDisc(pybind11::module_& m) {
    auto d = pybind11::class_<DiscSpec>(m, "DiscSpec")
        .def(pybind11::init<>())
        .def(pybind11::init<unsigned long, int, unsigned long>())
        .def(pybind11::init<const DiscSpec&>())
        .def_readwrite("tetIndex", &DiscSpec::tetIndex)
        .def_readwrite("type", &DiscSpec::type)
        .def_readwrite("number", &DiscSpec::number)
    ;
    regina::python::add_output_ostream(d);
    regina::python::add_eq_operators(d);

    m.def("numberDiscsAwayFromVertex", regina::numberDiscsAwayFromVertex);
    m.def("discOrientationFollowsEdge", regina::discOrientationFollowsEdge);

    auto t = pybind11::class_<DiscSetTet>(m, "DiscSetTet")
        .def(pybind11::init<const regina::NormalSurface&, unsigned long>())
        .def(pybind11::init<const DiscSetTet&>())
        .def("nDiscs", &DiscSetTet::nDiscs)
        .def("arcFromDisc", &DiscSetTet::arcFromDisc)
        .def("discFromArc", &DiscSetTet::discFromArc)
    ;
    regina::python::add_output_custom(t, [](const DiscSetTet& d,
            std::ostream& s) {
        s << '(';
        for (int i = 0; i < 10; ++i)
            s << ' ' << d.nDiscs(i);
        s << " )";
    });
    regina::python::add_eq_operators(t);

    auto s = pybind11::class_<DiscSetSurface>(m, "DiscSetSurface")
        .def(pybind11::init<const regina::NormalSurface&>())
        .def(pybind11::init<const DiscSetSurface&>())
        .def("swap", &DiscSetSurface::swap)
        .def("nTets", &DiscSetSurface::nTets)
        .def("nDiscs", &DiscSetSurface::nDiscs)
        .def("tetDiscs", &DiscSetSurface::tetDiscs,
            pybind11::return_value_policy::reference_internal)
        .def("adjacentDisc", &DiscSetSurface::adjacentDisc)
        .def("__iter__", [](const DiscSetSurface& d) {
            return DiscSpecIterator(d);
        }, pybind11::keep_alive<0, 1>()) // iterator keeps disc set alive
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
    regina::python::add_eq_operators(s);

    m.def("swap", (void(*)(DiscSetSurface&, DiscSetSurface&))(regina::swap));

    auto it = pybind11::class_<DiscSpecIterator<DiscSetTet>>(m, "DiscSpecIterator")
        .def("next", nextDiscSpec<DiscSetTet>) // for python 2
        .def("__next__", nextDiscSpec<DiscSetTet>) // for python 3
    ;
    regina::python::add_eq_operators(it);
}

