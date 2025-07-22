
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "census/census.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/census/census.h"

using pybind11::overload_cast;
using regina::Census;
using regina::CensusDB;
using regina::CensusHit;
using regina::Triangulation;

void addCensus(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(CensusDB)

    auto db = pybind11::class_<CensusDB>(m, "CensusDB", rdoc_scope)
        .def(pybind11::init<const std::string&, const std::string&>(),
            rdoc::__init)
        .def(pybind11::init<const CensusDB&>(), rdoc::__copy)
        .def("filename", &CensusDB::filename, rdoc::filename)
        .def("desc", &CensusDB::desc, rdoc::desc)
        .def("swap", &CensusDB::swap, rdoc::swap)
        .def("lookup", &CensusDB::lookup<const std::function<
            void(CensusHit&&)>&>, rdoc::lookup)
    ;
    regina::python::add_output_custom(db,
            [](const CensusDB& db, std::ostream& s) {
        s << db.filename() << " [" << db.desc() << ']';
    });
    regina::python::add_eq_operators(db, rdoc::__eq);

    regina::python::add_global_swap<CensusDB>(m, rdoc::global_swap);

    RDOC_SCOPE_SWITCH(CensusHit)

    auto h = pybind11::class_<CensusHit>(m, "CensusHit", rdoc_scope)
        .def(pybind11::init<const CensusHit&>(), rdoc::__copy)
        .def("swap", &CensusDB::swap, rdoc::swap)
        .def("name", &CensusHit::name, rdoc::name)
        .def("db", &CensusHit::db,
            pybind11::return_value_policy::reference, rdoc::db)
    ;
    regina::python::add_output_custom(h,
            [](const CensusHit& hit, std::ostream& s) {
        s << hit.name() << " -- " << hit.db().desc();
    });
    regina::python::add_eq_operators(h, rdoc::__eq);

    regina::python::add_global_swap<CensusHit>(m, rdoc::global_swap);

    RDOC_SCOPE_SWITCH(Census)

    auto c = pybind11::class_<Census>(m, "Census", rdoc_scope)
        .def_static("lookup",
            overload_cast<const Triangulation<3>&>(&Census::lookup),
            rdoc::lookup)
        .def_static("lookup",
            overload_cast<const std::string&>(&Census::lookup), rdoc::lookup_2)
    ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_END
}

