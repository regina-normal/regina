
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
#include "../pybind11/functional.h"
#include "../pybind11/stl.h"
#include "census/census.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Census;
using regina::CensusDB;
using regina::CensusHit;
using regina::Triangulation;

void addCensus(pybind11::module_& m) {
    auto db = pybind11::class_<CensusDB>(m, "CensusDB")
        .def(pybind11::init<const std::string&, const std::string&>())
        .def(pybind11::init<const CensusDB&>())
        .def("filename", &CensusDB::filename)
        .def("desc", &CensusDB::desc)
        .def("swap", &CensusDB::swap)
        .def("lookup", &CensusDB::lookup<const std::function<
            void(CensusHit&&)>&>)
    ;
    regina::python::add_output_custom(db,
            [](const CensusDB& db, std::ostream& s) {
        s << db.filename() << " [" << db.desc() << ']';
    });
    regina::python::add_eq_operators(db);

    m.def("swap", (void(*)(CensusDB&, CensusDB&))(regina::swap));

    auto h = pybind11::class_<CensusHit>(m, "CensusHit")
        .def(pybind11::init<const CensusHit&>())
        .def("swap", &CensusDB::swap)
        .def("name", &CensusHit::name)
        .def("db", &CensusHit::db,
            pybind11::return_value_policy::reference)
    ;
    regina::python::add_output_custom(h,
            [](const CensusHit& hit, std::ostream& s) {
        s << hit.name() << " -- " << hit.db().desc();
    });
    regina::python::add_eq_operators(h);

    m.def("swap", (void(*)(CensusHit&, CensusHit&))(regina::swap));

    auto c = pybind11::class_<Census>(m, "Census")
        .def_static("lookup",
            overload_cast<const Triangulation<3>&>(&Census::lookup))
        .def_static("lookup",
            overload_cast<const std::string&>(&Census::lookup))
    ;
    regina::python::no_eq_operators(c);
}

