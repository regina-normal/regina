
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
#include "link/link.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/census/census.h"

using namespace pybind11::literals;

using pybind11::overload_cast;
using regina::Census;
using regina::CensusDB;
using regina::CensusHit;
using regina::Link;
using regina::Triangulation;

template <regina::SignatureReconstructible ObjectType>
void addCensusCollection(pybind11::module_& m, const char* name) {
    using Collection = regina::CensusCollection<ObjectType>;

    RDOC_SCOPE_BEGIN(CensusCollection)

    auto c = pybind11::class_<Collection>(m, name, rdoc::__class)
        .def_static("lookup",
            overload_cast<const ObjectType&>(&Collection::lookup),
            rdoc::lookup)
        .def_static("lookup",
            overload_cast<const std::string&>(&Collection::lookup),
            rdoc::lookup_2)
    ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_END
}

void addCensus(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(CensusDB)

    auto db = pybind11::class_<CensusDB>(m, "CensusDB", rdoc::__class)
        .def(pybind11::init<const std::string&, const std::string&,
                const std::string&, size_t>(),
            "filename"_a, "desc"_a, "tag"_a = std::string(), "maxSize"_a = 0,
            rdoc::__init)
        .def(pybind11::init<const CensusDB&>(), rdoc::__copy)
        .def_static("global",
            overload_cast<const char*, const char*, size_t>(&CensusDB::global),
            "basename"_a, "desc"_a, "maxSize"_a = 0, rdoc::global)
        .def_static("global",
            overload_cast<const char*, const char*, const char*, size_t>(
                &CensusDB::global),
            "basename"_a, "desc"_a, "tag"_a, "maxSize"_a = 0, rdoc::global_2)
        .def("filename", &CensusDB::filename, rdoc::filename)
        .def("maxSize", &CensusDB::maxSize, rdoc::maxSize)
        .def("desc", &CensusDB::desc, rdoc::desc)
        .def("tag", &CensusDB::tag, rdoc::tag)
        .def("swap", &CensusDB::swap, rdoc::swap)
        .def("lookupKey", [](const CensusDB& db, int generation,
                const std::string& isoSig,
                const std::function<void(CensusHit&&)>& action) {
            if (generation == 2)
                db.lookupKey<2>(isoSig, action);
            else
                throw regina::InvalidArgument("This generation of "
                    "signature is not supported by CensusDB");
        }, "generation"_a, "isoSig"_a, "action"_a, rdoc::lookupKey)
    ;
    regina::python::add_output_custom(db,
            [](const CensusDB& db, std::ostream& s) {
        s << db.filename() << " [" << db.desc() << ']';
    });
    regina::python::add_eq_operators(db, rdoc::__eq);
    regina::python::add_global_swap<CensusDB, rdoc>(m);

    RDOC_SCOPE_SWITCH(CensusHit)

    auto h = pybind11::class_<CensusHit>(m, "CensusHit", rdoc::__class)
        .def(pybind11::init<const CensusHit&>(), rdoc::__copy)
        .def("swap", &CensusHit::swap, rdoc::swap)
        .def("name", &CensusHit::name, rdoc::name)
        .def("db", &CensusHit::db,
            pybind11::return_value_policy::reference, rdoc::db)
    ;
    regina::python::add_output_custom(h,
            [](const CensusHit& hit, std::ostream& s) {
        s << hit.name() << " -- " << hit.db().desc();
    });
    regina::python::add_eq_operators(h, rdoc::__eq);
    regina::python::add_global_swap<CensusHit, rdoc>(m);

    RDOC_SCOPE_SWITCH(Census)

    auto c = pybind11::class_<Census>(m, "Census", rdoc::__class)
        .def_static("lookup", &Census::lookup<Triangulation<3>>, rdoc::lookup)
        .def_static("lookup", &Census::lookup<Link>, rdoc::lookup)
        .def_static("lookupAs",
                [](pybind11::type objectType, const std::string& sig) {
            if (objectType.is(pybind11::type::of<regina::Triangulation<3>>())) {
                return Census::lookupAs<Triangulation<3>>(sig);
            } else if (objectType.is(pybind11::type::of<regina::Link>())) {
                return Census::lookupAs<Link>(sig);
            } else {
                throw regina::InvalidArgument("Not a supported object type");
            }
        }, "objectType"_a, "sig"_a, rdoc::lookupAs)
        .def_static("lookup",
            static_cast<std::list<CensusHit> (*)(const std::string&)>(
                &Census::lookup), rdoc::lookup_2)
    ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_END

    addCensusCollection<Triangulation<3>>(m, "CensusCollection3");
    addCensusCollection<Link>(m, "CensusCollectionLink");
}

