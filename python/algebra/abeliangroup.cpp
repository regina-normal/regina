
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
#include "../pybind11/iostream.h"
#include "../pybind11/stl.h"
#include "algebra/abeliangroup.h"
#include "maths/matrix.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::AbelianGroup;
using regina::Integer;
using regina::MatrixInt;

void addAbelianGroup(pybind11::module_& m) {
    auto c = pybind11::class_<AbelianGroup>(m, "AbelianGroup")
        .def(pybind11::init<>())
        .def(pybind11::init<const AbelianGroup&>())
        .def(pybind11::init<unsigned>())
        .def(pybind11::init<unsigned, const std::vector<int>&>())
        .def(pybind11::init<unsigned, const std::vector<Integer>&>())
        .def(pybind11::init<MatrixInt>())
        .def(pybind11::init<MatrixInt, MatrixInt>())
        .def(pybind11::init<MatrixInt, MatrixInt, const Integer&>())
        .def("swap", &AbelianGroup::swap)
        .def("addRank", &AbelianGroup::addRank,
            pybind11::arg("extraRank") = 1)
        .def("addTorsion", &AbelianGroup::addTorsion)
        .def("addTorsionElement", // deprecated
                [](AbelianGroup& g, const regina::Integer& d, unsigned m) {
            for ( ; m > 0; --m)
                g.addTorsion(d);
        }, pybind11::arg(), pybind11::arg("mult") = 1)
        .def("addTorsionElement", // deprecated
                [](AbelianGroup& g, unsigned long d, unsigned m) {
            for ( ; m > 0; --m)
                g.addTorsion(d);
        }, pybind11::arg(), pybind11::arg("mult") = 1)
        .def("addTorsionElements", [](AbelianGroup& g, pybind11::list l) {
            // This routine is now deprecated.
            // Reimplement it using addTorsionElement() instead.
            for (auto item : l) {
                // Accept any type that we know how to convert to Integer.
                // This includes (at least) regina::Integer, python integers
                // (both int and long), and strings.
                try {
                    g.addTorsion(item.cast<regina::Integer>());
                } catch (pybind11::cast_error const &) {
                    throw regina::InvalidArgument(
                        "List element not convertible to Integer");
                }
            }
        })
        .def("addGroup", overload_cast<MatrixInt>(
            &AbelianGroup::addGroup))
        .def("addGroup", overload_cast<const AbelianGroup&>(
            &AbelianGroup::addGroup))
        .def("rank", &AbelianGroup::rank)
        .def("torsionRank", overload_cast<const regina::Integer&>(
            &AbelianGroup::torsionRank, pybind11::const_))
        .def("torsionRank", overload_cast<unsigned long>(
            &AbelianGroup::torsionRank, pybind11::const_))
        .def("countInvariantFactors", &AbelianGroup::countInvariantFactors)
        .def("invariantFactor", &AbelianGroup::invariantFactor)
        .def("isTrivial", &AbelianGroup::isTrivial)
        .def("isZ", &AbelianGroup::isZ)
        .def("isFree", &AbelianGroup::isFree)
        .def("isZn", &AbelianGroup::isZn)
        .def("utf8", &AbelianGroup::utf8)
        .def("tightEncoding", &AbelianGroup::tightEncoding)
        .def("writeXMLData", [](const AbelianGroup& g, pybind11::object file) {
            pybind11::scoped_ostream_redirect stream(std::cout, file);
            g.writeXMLData(std::cout);
        })
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.def("swap", (void(*)(AbelianGroup&, AbelianGroup&))(regina::swap));
}

