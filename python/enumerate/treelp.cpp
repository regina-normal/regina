
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "../docstrings/enumerate/treelp.h"

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
    RDOC_SCOPE_BEGIN(LPInitialTableaux)

    using Tableaux = LPInitialTableaux<LPConstraint>;

    auto c = pybind11::class_<Tableaux>(m, name, rdoc_scope)
        .def(pybind11::init<const Triangulation<3>&, NormalEncoding, bool>(),
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("enumeration") = true,
            rdoc::__init)
        .def(pybind11::init<const Tableaux&>(), rdoc::__init_2)
        .def("swap", &Tableaux::swap, rdoc::swap)
        .def("tri", &Tableaux::tri,
            pybind11::return_value_policy::reference_internal, rdoc::tri)
        .def("system", &Tableaux::system, rdoc::system)
        .def("rank", &Tableaux::rank, rdoc::rank)
        .def("columns", &Tableaux::columns, rdoc::columns)
        .def("coordinateColumns", &Tableaux::coordinateColumns,
            rdoc::coordinateColumns)
        .def("columnPerm", [](const Tableaux& t) {
            const size_t* perm = t.columnPerm();

            pybind11::list ans;
            for (size_t i = 0; i < t.columns(); ++i)
                ans.append(perm[i]);
            return ans;
        }, rdoc::columnPerm)
        .def("multColByRow", &Tableaux::template multColByRow<Integer>,
            rdoc::multColByRow)
        .def("multColByRowOct", &Tableaux::template multColByRowOct<Integer>,
            rdoc::multColByRowOct)
        .def("fillInitialTableaux",
            &Tableaux::template fillInitialTableaux<Integer>,
            rdoc::fillInitialTableaux)
        ;
    regina::python::add_output(c);
    // We need to think more about what a comparison between tableaux should
    // test.  In the meantime, don't make a decision we might regret later.
    regina::python::disable_eq_operators(c);

    regina::python::add_global_swap<Tableaux>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

template <class LPConstraint>
void addLPData(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(LPData)

    using Data = LPData<LPConstraint, Integer>;

    auto c = pybind11::class_<Data>(m, name, rdoc_scope)
        .def(pybind11::init<>(), rdoc::__init)
        .def("swap", &Data::swap, rdoc::swap)
        .def("reserve", &Data::reserve, rdoc::reserve)
        .def("initStart", &Data::initStart, rdoc::initStart)
        .def("initClone", &Data::initClone, rdoc::initClone)
        .def("columns", &Data::columns, rdoc::columns)
        .def("coordinateColumns", &Data::coordinateColumns,
            rdoc::coordinateColumns)
        .def("isFeasible", &Data::isFeasible, rdoc::isFeasible)
        .def("isActive", &Data::isActive, rdoc::isActive)
        .def("sign", &Data::sign, rdoc::sign)
        .def("constrainZero", &Data::constrainZero, rdoc::constrainZero)
        .def("constrainPositive", &Data::constrainPositive,
            rdoc::constrainPositive)
        .def("constrainOct", &Data::constrainOct, rdoc::constrainOct)
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
        }, rdoc::extractSolution)
        ;
    regina::python::add_output(c);
    // We need to think more about what a comparison between tableaux should
    // test.  Do we just compare basis indices?  Do we do a deep comparison of
    // all the internal data?  Let's not force a decision right now.
    regina::python::disable_eq_operators(c);

    regina::python::add_global_swap<Data>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

void addTreeLP(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(LPMatrix)

    auto c = pybind11::class_<LPMatrix<Integer>>(m, "LPMatrix", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__init)
        .def(pybind11::init<size_t, size_t>(), rdoc::__init_2)
        .def("swap", &LPMatrix<Integer>::swap, rdoc::swap)
        .def("reserve", &LPMatrix<Integer>::reserve, rdoc::reserve)
        .def("initClone", &LPMatrix<Integer>::initClone, rdoc::initClone)
        .def("initIdentity", &LPMatrix<Integer>::initIdentity,
            rdoc::initIdentity)
        .def("entry", overload_cast<size_t, size_t>(&LPMatrix<Integer>::entry),
            pybind11::return_value_policy::reference_internal, rdoc::entry)
        .def("set", [](LPMatrix<Integer>& m, size_t row, size_t col,
                const regina::Integer& value){
            m.entry(row, col) = value;
        }, rdoc::entry)
        .def("rows", &LPMatrix<Integer>::rows, rdoc::rows)
        .def("columns", &LPMatrix<Integer>::columns, rdoc::columns)
        .def("swapRows", &LPMatrix<Integer>::swapRows, rdoc::swapRows)
        .def("combRow", &LPMatrix<Integer>::combRow, rdoc::combRow)
        .def("combRowAndNorm", &LPMatrix<Integer>::combRowAndNorm,
            rdoc::combRowAndNorm)
        .def("negateRow", &LPMatrix<Integer>::negateRow, rdoc::negateRow)
        ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<LPMatrix<Integer>>(m, rdoc::global_swap);

    RDOC_SCOPE_SWITCH(LPSystem)

    auto s = pybind11::class_<LPSystem>(m, "LPSystem", rdoc_scope)
        .def(pybind11::init<regina::NormalEncoding>(), rdoc::__init)
        .def(pybind11::init<const LPSystem&>(), rdoc::__init_2)
        .def("normal", &LPSystem::normal, rdoc::normal)
        .def("angle", &LPSystem::angle, rdoc::angle)
        .def("standard", &LPSystem::standard, rdoc::standard)
        .def("quad", &LPSystem::quad, rdoc::quad)
        .def("coords", &LPSystem::coords, rdoc::coords)
        ;
    regina::python::add_output(s);
    regina::python::add_eq_operators(s, rdoc::__eq, rdoc::__ne);

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

    RDOC_SCOPE_END
}

