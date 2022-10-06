
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
        .def(pybind11::init<size_t, const Integer&>())
        .def("swap", &MarkedAbelianGroup::swap)
        .def("rank", &MarkedAbelianGroup::rank)
        .def("torsionRank", overload_cast<const regina::Integer&>(
            &MarkedAbelianGroup::torsionRank, pybind11::const_))
        .def("torsionRank", overload_cast<unsigned long>(
            &MarkedAbelianGroup::torsionRank, pybind11::const_))
        .def("snfRank", &MarkedAbelianGroup::snfRank)
        .def("countInvariantFactors",
            &MarkedAbelianGroup::countInvariantFactors)
        .def("invariantFactor", &MarkedAbelianGroup::invariantFactor)
        .def("unmarked", &MarkedAbelianGroup::unmarked)
        .def("isTrivial", &MarkedAbelianGroup::isTrivial)
        .def("isZ", &MarkedAbelianGroup::isZ)
        .def("isIsomorphicTo", &MarkedAbelianGroup::isIsomorphicTo)
        .def("freeRep", &MarkedAbelianGroup::freeRep)
        .def("torsionRep", &MarkedAbelianGroup::torsionRep)
        // Below, the overloads that take a std::vector must come *last*,
        // since otherwise it treats func(x) as func([x]) never sees
        // the non-vector version.
        .def("ccRep", overload_cast<size_t>(
            &MarkedAbelianGroup::ccRep, pybind11::const_))
        .def("ccRep", overload_cast<const regina::Vector<Integer>&>(
            &MarkedAbelianGroup::ccRep, pybind11::const_))
        .def("cycleProjection", overload_cast<size_t>(
            &MarkedAbelianGroup::cycleProjection, pybind11::const_))
        .def("cycleProjection", overload_cast<const regina::Vector<Integer>&>(
            &MarkedAbelianGroup::cycleProjection, pybind11::const_))
        .def("isCycle", &MarkedAbelianGroup::isCycle)
        .def("boundaryOf", &MarkedAbelianGroup::boundaryOf)
        .def("isBoundary", &MarkedAbelianGroup::isBoundary)
        .def("asBoundary", &MarkedAbelianGroup::asBoundary)
        .def("snfRep", &MarkedAbelianGroup::snfRep)
        .def("ccRank", &MarkedAbelianGroup::ccRank)
        .def("cycleRank", &MarkedAbelianGroup::cycleRank)
        .def("cycleGen", &MarkedAbelianGroup::cycleGen)
        .def("m", &MarkedAbelianGroup::m,
            pybind11::return_value_policy::reference_internal)
        .def("n", &MarkedAbelianGroup::n,
            pybind11::return_value_policy::reference_internal)
        .def("coefficients", &MarkedAbelianGroup::coefficients)
        .def("torsionSubgroup", &MarkedAbelianGroup::torsionSubgroup)
        .def("torsionInclusion", &MarkedAbelianGroup::torsionInclusion)
        .def("utf8", &MarkedAbelianGroup::utf8)
    ;
    regina::python::add_output(c1);
    regina::python::add_eq_operators(c1);

    regina::python::add_global_swap<MarkedAbelianGroup>(m);

    auto c2 = pybind11::class_<HomMarkedAbelianGroup>(m,
            "HomMarkedAbelianGroup")
        .def(pybind11::init<const MarkedAbelianGroup&,
                const MarkedAbelianGroup&, const MatrixInt&>())
        .def(pybind11::init<const HomMarkedAbelianGroup&>())
        .def("swap", &HomMarkedAbelianGroup::swap)
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
        .def("codomain", &HomMarkedAbelianGroup::codomain,
            pybind11::return_value_policy::reference_internal)
        .def("definingMatrix", &HomMarkedAbelianGroup::definingMatrix,
            pybind11::return_value_policy::reference_internal)
        .def("reducedMatrix", &HomMarkedAbelianGroup::reducedMatrix,
            pybind11::return_value_policy::reference_internal)
        .def("summary", pybind11::overload_cast<>(
            &HomMarkedAbelianGroup::summary, pybind11::const_))
        .def("torsionSubgroup", &HomMarkedAbelianGroup::torsionSubgroup)
        .def("evalCC", &HomMarkedAbelianGroup::evalCC)
        .def("evalSNF", &HomMarkedAbelianGroup::evalSNF)
        .def("inverseHom", &HomMarkedAbelianGroup::inverseHom)
        .def(pybind11::self * pybind11::self)
    ;
    regina::python::add_output(c2);
    // Deciding what we want comparisons to *mean* requires some thought.
    // Let's not make a decision now that we might regret later.
    regina::python::disable_eq_operators(c2);

    regina::python::add_global_swap<HomMarkedAbelianGroup>(m);
}

