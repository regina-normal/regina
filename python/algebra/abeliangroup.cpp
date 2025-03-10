
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>
#include "algebra/abeliangroup.h"
#include "maths/matrix.h"
#include "../helpers.h"
#include "../docstrings/algebra/abeliangroup.h"

using pybind11::overload_cast;
using regina::AbelianGroup;
using regina::Integer;
using regina::MatrixInt;

void addAbelianGroup(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(AbelianGroup)

    auto c = pybind11::class_<AbelianGroup>(m, "AbelianGroup", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const AbelianGroup&>(), rdoc::__copy)
        .def(pybind11::init<size_t>(), rdoc::__init)
        .def(pybind11::init<size_t, const std::vector<int>&>(), rdoc::__init_2)
        .def(pybind11::init<size_t, const std::vector<Integer>&>(),
            rdoc::__init_2)
        .def(pybind11::init<MatrixInt>(), rdoc::__init_3)
        .def(pybind11::init<MatrixInt, MatrixInt>(), rdoc::__init_4)
        .def(pybind11::init<MatrixInt, MatrixInt, const Integer&>(),
            rdoc::__init_5)
        .def("swap", &AbelianGroup::swap, rdoc::swap)
        .def("addRank", &AbelianGroup::addRank,
            pybind11::arg("extraRank") = 1, rdoc::addRank)
        .def("addTorsion", &AbelianGroup::addTorsion, rdoc::addTorsion)
        .def("addGroup", overload_cast<MatrixInt>(
            &AbelianGroup::addGroup), rdoc::addGroup)
        .def("addGroup", overload_cast<const AbelianGroup&>(
            &AbelianGroup::addGroup), rdoc::addGroup_2)
        .def("rank", &AbelianGroup::rank, rdoc::rank)
        .def("torsionRank", overload_cast<const regina::Integer&>(
            &AbelianGroup::torsionRank, pybind11::const_), rdoc::torsionRank)
        .def("torsionRank", overload_cast<unsigned long>(
            &AbelianGroup::torsionRank, pybind11::const_), rdoc::torsionRank_2)
        .def("countInvariantFactors", &AbelianGroup::countInvariantFactors,
            rdoc::countInvariantFactors)
        .def("invariantFactor", &AbelianGroup::invariantFactor,
            rdoc::invariantFactor)
        .def("isTrivial", &AbelianGroup::isTrivial, rdoc::isTrivial)
        .def("isZ", &AbelianGroup::isZ, rdoc::isZ)
        .def("isFree", &AbelianGroup::isFree, rdoc::isFree)
        .def("isZn", &AbelianGroup::isZn, rdoc::isZn)
        .def("writeXMLData", [](const AbelianGroup& g, pybind11::object file) {
            pybind11::scoped_ostream_redirect stream(std::cout, file);
            g.writeXMLData(std::cout);
        }, rdoc::writeXMLData)
    ;
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    regina::python::add_global_swap<AbelianGroup>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

