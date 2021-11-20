
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
#include "enumerate/treelp.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Integer;
using regina::LPData;
using regina::LPInitialTableaux;
using regina::LPMatrix;
using regina::LPSystem;
using regina::NormalEncoding;
using regina::Triangulation;

using regina::LPConstraintNone;
using regina::LPConstraintEulerPositive;
using regina::LPConstraintEulerZero;
using regina::LPConstraintNonSpun;

template <class LPConstraint>
void addLPInitialTableaux(pybind11::module_& m, const char* name) {
    using Tableaux = LPInitialTableaux<LPConstraint>;

    auto c = pybind11::class_<Tableaux>(m, name)
        .def(pybind11::init<const Triangulation<3>&, NormalEncoding, bool>(),
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("enumeration") = true)
        .def(pybind11::init<const Tableaux&>())
        .def("swap", &Tableaux::swap)
        .def("tri", &Tableaux::tri,
            pybind11::return_value_policy::reference_internal)
        .def("system", &Tableaux::system)
        .def("rank", &Tableaux::rank)
        .def("columns", &Tableaux::columns)
        .def("coordinateColumns", &Tableaux::coordinateColumns)
        .def("columnPerm", [](const Tableaux& t) {
            const int* perm = t.columnPerm();

            pybind11::list ans;
            for (size_t i = 0; i < t.columns(); ++i)
                ans.append(perm[i]);
            return ans;
        })
        .def("multColByRow", &Tableaux::template multColByRow<Integer>)
        .def("multColByRowOct", &Tableaux::template multColByRowOct<Integer>)
        .def("fillInitialTableaux",
            &Tableaux::template fillInitialTableaux<Integer>)
        ;
    regina::python::add_eq_operators(c);

    m.def("swap", (void(*)(Tableaux&, Tableaux&))(regina::swap));
}

template <class LPConstraint>
void addLPData(pybind11::module_& m, const char* name) {
    using Data = LPData<LPConstraint, Integer>;

    auto c = pybind11::class_<Data>(m, name)
        .def(pybind11::init<>())
        .def("swap", &Data::swap)
        .def("reserve", &Data::reserve)
        .def("initStart", &Data::initStart)
        .def("initClone", &Data::initClone)
        .def("columns", &Data::columns)
        .def("coordinateColumns", &Data::coordinateColumns)
        .def("isFeasible", &Data::isFeasible)
        .def("isActive", &Data::isActive)
        .def("sign", &Data::sign)
        .def("constrainZero", &Data::constrainZero)
        .def("constrainPositive", &Data::constrainPositive)
        .def("constrainOct", &Data::constrainOct)
        .def("__str__", [](const Data& d) {
            std::ostringstream out;
            d.dump(out);
            return out.str();
        })
        .def("extractSolution", [](const Data& d, const std::vector<int>& t) {
            // Currently LPData does not give us an easy way to extract the
            // expected length of the type vector, and so we cannot sanity-check
            // the size of t right now.  Probably we should add an access
            // function to LPData that lets us view the original tableaux.
            char* types = new char[t.size()];
            std::copy(t.begin(), t.end(), types);
            auto ans = d.template extractSolution<regina::VectorInt>(types);
            delete[] types;
            return ans;
        })
        ;
    regina::python::add_eq_operators(c);

    m.def("swap", (void(*)(Data&, Data&))(regina::swap));
}

void addTreeLP(pybind11::module_& m) {
    auto c = pybind11::class_<LPMatrix<Integer>>(m, "LPMatrix")
        .def(pybind11::init<>())
        .def(pybind11::init<unsigned, unsigned>())
        .def("swap", &LPMatrix<Integer>::swap)
        .def("reserve", &LPMatrix<Integer>::reserve)
        .def("initClone", &LPMatrix<Integer>::initClone)
        .def("initIdentity", &LPMatrix<Integer>::initIdentity)
        .def("entry",
            overload_cast<unsigned, unsigned>(&LPMatrix<Integer>::entry),
            pybind11::return_value_policy::reference_internal)
        .def("set", [](LPMatrix<Integer>& m, unsigned row, unsigned col,
                const regina::Integer& value){
            m.entry(row, col) = value;
        })
        .def("rows", &LPMatrix<Integer>::rows)
        .def("columns", &LPMatrix<Integer>::columns)
        .def("swapRows", &LPMatrix<Integer>::swapRows)
        .def("combRow", &LPMatrix<Integer>::combRow)
        .def("combRowAndNorm", &LPMatrix<Integer>::combRowAndNorm)
        .def("negateRow", &LPMatrix<Integer>::negateRow)
        .def("__str__", [](const LPMatrix<Integer>& m) {
            std::ostringstream out;
            m.dump(out);
            return out.str();
        })
        ;
    regina::python::add_eq_operators(c);

    m.def("swap",
        (void(*)(LPMatrix<Integer>&, LPMatrix<Integer>&))(regina::swap));

    auto s = pybind11::class_<LPSystem>(m, "LPSystem")
        .def(pybind11::init<regina::NormalEncoding>())
        .def(pybind11::init<const LPSystem&>())
        .def("normal", &LPSystem::normal)
        .def("angle", &LPSystem::angle)
        .def("standard", &LPSystem::standard)
        .def("quad", &LPSystem::quad)
        .def("coords", &LPSystem::coords)
        ;
    regina::python::add_eq_operators(s);

    addLPInitialTableaux<LPConstraintNone>(m, "LPInitialTableaux");
    addLPInitialTableaux<LPConstraintEulerPositive>(m,
        "LPInitialTableaux_EulerPositive");
    addLPInitialTableaux<LPConstraintEulerZero>(m,
        "LPInitialTableaux_EulerZero");
    addLPInitialTableaux<LPConstraintNonSpun>(m, "LPInitialTableaux_NonSpun");

    addLPData<LPConstraintNone>(m, "LPData");
    addLPData<LPConstraintEulerPositive>(m, "LPData_EulerPositive");
    addLPData<LPConstraintEulerZero>(m, "LPData_EulerZero");
    addLPData<LPConstraintNonSpun>(m, "LPData_NonSpun");
}

