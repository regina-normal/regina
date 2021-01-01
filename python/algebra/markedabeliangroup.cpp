
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
#include "../pybind11/operators.h"
#include "../pybind11/stl.h"
#include "algebra/markedabeliangroup.h"
#include "maths/matrix.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::HomMarkedAbelianGroup;
using regina::MarkedAbelianGroup;
using regina::MatrixInt;
using regina::Integer;

void addMarkedAbelianGroup(pybind11::module_& m) {
    auto c1 = pybind11::class_<MarkedAbelianGroup>(m, "MarkedAbelianGroup")
        .def(pybind11::init<const MatrixInt&, const MatrixInt&>())
        .def(pybind11::init<const MarkedAbelianGroup&>())
        .def(pybind11::init<const MatrixInt&, const MatrixInt&,
            const Integer&>())
        .def(pybind11::init<unsigned long, const Integer&>())
        .def("isChainComplex", &MarkedAbelianGroup::isChainComplex)
        .def("rank", &MarkedAbelianGroup::rank)
        .def("torsionRank", overload_cast<const regina::Integer&>(
            &MarkedAbelianGroup::torsionRank, pybind11::const_))
        .def("torsionRank", overload_cast<unsigned long>(
            &MarkedAbelianGroup::torsionRank, pybind11::const_))
        .def("minNumberOfGenerators",
            &MarkedAbelianGroup::minNumberOfGenerators)
        .def("countInvariantFactors",
            &MarkedAbelianGroup::countInvariantFactors)
        .def("invariantFactor", &MarkedAbelianGroup::invariantFactor)
        .def("isTrivial", &MarkedAbelianGroup::isTrivial)
        .def("isZ", &MarkedAbelianGroup::isZ)
        .def("isIsomorphicTo", &MarkedAbelianGroup::isIsomorphicTo)
        .def("equalTo", &MarkedAbelianGroup::equalTo)
        .def("freeRep", &MarkedAbelianGroup::freeRep)
        .def("torsionRep", &MarkedAbelianGroup::torsionRep)
        // Below, the overloads that take a std::vector must come *last*,
        // since otherwise it treats func(x) as func([x]) never sees
        // the non-vector version.
        .def("ccRep", overload_cast<unsigned long>(
            &MarkedAbelianGroup::ccRep, pybind11::const_))
        .def("ccRep", overload_cast<const std::vector<Integer>&>(
            &MarkedAbelianGroup::ccRep, pybind11::const_))
        .def("cycleProjection", overload_cast<unsigned long>(
            &MarkedAbelianGroup::cycleProjection, pybind11::const_))
        .def("cycleProjection", overload_cast<const std::vector<Integer>&>(
            &MarkedAbelianGroup::cycleProjection, pybind11::const_))
        .def("isCycle", &MarkedAbelianGroup::isCycle)
        .def("boundaryMap", &MarkedAbelianGroup::boundaryMap)
        .def("isBoundary", &MarkedAbelianGroup::isBoundary)
        .def("writeAsBoundary", &MarkedAbelianGroup::writeAsBoundary)
        .def("snfRep", &MarkedAbelianGroup::snfRep)
        .def("rankCC", &MarkedAbelianGroup::rankCC)
        .def("minNumberCycleGens", &MarkedAbelianGroup::minNumberCycleGens)
        .def("cycleGen", &MarkedAbelianGroup::cycleGen)
        .def("M", &MarkedAbelianGroup::M,
            pybind11::return_value_policy::reference_internal)
        .def("N", &MarkedAbelianGroup::N,
            pybind11::return_value_policy::reference_internal)
        .def("coefficients", &MarkedAbelianGroup::coefficients)
        .def("torsionSubgroup", &MarkedAbelianGroup::torsionSubgroup)
        .def("torsionInclusion", &MarkedAbelianGroup::torsionInclusion)
        .def("utf8", &MarkedAbelianGroup::utf8)
    ;
    regina::python::add_output(c1);
    regina::python::add_eq_operators(c1);
    m.attr("NMarkedAbelianGroup") = m.attr("MarkedAbelianGroup");

    auto c2 = pybind11::class_<HomMarkedAbelianGroup>(m,
            "HomMarkedAbelianGroup")
        .def(pybind11::init<const MarkedAbelianGroup&,
                const MarkedAbelianGroup&, const MatrixInt&>())
        .def(pybind11::init<const HomMarkedAbelianGroup&>())
        .def("isChainMap", &HomMarkedAbelianGroup::isChainMap)
        .def("isCycleMap", &HomMarkedAbelianGroup::isCycleMap)
        .def("isEpic", &HomMarkedAbelianGroup::isEpic)
        .def("isMonic", &HomMarkedAbelianGroup::isMonic)
        .def("isIsomorphism", &HomMarkedAbelianGroup::isIsomorphism)
        .def("isIdentity", &HomMarkedAbelianGroup::isIdentity)
        .def("isZero", &HomMarkedAbelianGroup::isZero)
        .def("kernel", &HomMarkedAbelianGroup::kernel,
            pybind11::return_value_policy::reference_internal)
        .def("cokernel", &HomMarkedAbelianGroup::cokernel,
            pybind11::return_value_policy::reference_internal)
        .def("image", &HomMarkedAbelianGroup::image,
            pybind11::return_value_policy::reference_internal)
        .def("domain", &HomMarkedAbelianGroup::domain,
            pybind11::return_value_policy::reference_internal)
        .def("range", &HomMarkedAbelianGroup::range,
            pybind11::return_value_policy::reference_internal)
        .def("definingMatrix", &HomMarkedAbelianGroup::definingMatrix,
            pybind11::return_value_policy::reference_internal)
        .def("reducedMatrix", &HomMarkedAbelianGroup::reducedMatrix,
            pybind11::return_value_policy::reference_internal)
        .def("torsionSubgroup", &HomMarkedAbelianGroup::torsionSubgroup)
        .def("writeReducedMatrix", [](const HomMarkedAbelianGroup& h) {
            h.writeReducedMatrix(std::cout);
        })
        .def("evalCC", &HomMarkedAbelianGroup::evalCC)
        .def("evalSNF", &HomMarkedAbelianGroup::evalSNF)
        .def("inverseHom", &HomMarkedAbelianGroup::inverseHom)
        .def(pybind11::self * pybind11::self) /* returns std::unique_ptr */
    ;
    regina::python::add_output(c2);
    regina::python::add_eq_operators(c2);
    m.attr("NHomMarkedAbelianGroup") = m.attr("HomMarkedAbelianGroup");
}

