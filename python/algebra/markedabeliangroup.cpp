
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "algebra/markedabeliangroup.h"
#include "maths/matrix.h"
#include "../helpers.h"
#include "../docstrings/algebra/markedabeliangroup.h"

using pybind11::overload_cast;
using regina::HomMarkedAbelianGroup;
using regina::MarkedAbelianGroup;
using regina::MatrixInt;
using regina::Integer;

void addMarkedAbelianGroup(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(MarkedAbelianGroup)

    auto c1 = pybind11::class_<MarkedAbelianGroup>(m, "MarkedAbelianGroup",
            rdoc_scope)
        .def(pybind11::init<const MatrixInt&, const MatrixInt&>(), rdoc::__init)
        .def(pybind11::init<const MatrixInt&, const MatrixInt&,
            const Integer&>(), rdoc::__init_2)
        .def(pybind11::init<size_t, const Integer&>(), rdoc::__init_3)
        .def(pybind11::init<const MarkedAbelianGroup&>(), rdoc::__copy)
        .def("swap", &MarkedAbelianGroup::swap, rdoc::swap)
        .def("rank", &MarkedAbelianGroup::rank, rdoc::rank)
        .def("torsionRank", overload_cast<const regina::Integer&>(
            &MarkedAbelianGroup::torsionRank, pybind11::const_),
            rdoc::torsionRank)
        .def("torsionRank", overload_cast<unsigned long>(
            &MarkedAbelianGroup::torsionRank, pybind11::const_),
            rdoc::torsionRank)
        .def("snfRank", &MarkedAbelianGroup::snfRank, rdoc::snfRank)
        .def("countInvariantFactors",
            &MarkedAbelianGroup::countInvariantFactors,
            rdoc::countInvariantFactors)
        .def("invariantFactor", &MarkedAbelianGroup::invariantFactor,
            rdoc::invariantFactor)
        .def("unmarked", &MarkedAbelianGroup::unmarked, rdoc::unmarked)
        .def("isTrivial", &MarkedAbelianGroup::isTrivial, rdoc::isTrivial)
        .def("isZ", &MarkedAbelianGroup::isZ, rdoc::isZ)
        .def("isIsomorphicTo", &MarkedAbelianGroup::isIsomorphicTo,
            rdoc::isIsomorphicTo)
        .def("freeRep", &MarkedAbelianGroup::freeRep, rdoc::freeRep)
        .def("torsionRep", &MarkedAbelianGroup::torsionRep, rdoc::torsionRep)
        // Below, the overloads that take a std::vector must come *last*,
        // since otherwise it treats func(x) as func([x]) never sees
        // the non-vector version.
        .def("ccRep", overload_cast<size_t>(
            &MarkedAbelianGroup::ccRep, pybind11::const_), rdoc::ccRep_2)
        .def("ccRep", overload_cast<const regina::Vector<Integer>&>(
            &MarkedAbelianGroup::ccRep, pybind11::const_), rdoc::ccRep)
        .def("cycleProjection", overload_cast<size_t>(
            &MarkedAbelianGroup::cycleProjection, pybind11::const_),
            rdoc::cycleProjection_2)
        .def("cycleProjection", overload_cast<const regina::Vector<Integer>&>(
            &MarkedAbelianGroup::cycleProjection, pybind11::const_),
            rdoc::cycleProjection)
        .def("isCycle", &MarkedAbelianGroup::isCycle, rdoc::isCycle)
        .def("boundaryOf", &MarkedAbelianGroup::boundaryOf, rdoc::boundaryOf)
        .def("isBoundary", &MarkedAbelianGroup::isBoundary, rdoc::isBoundary)
        .def("asBoundary", &MarkedAbelianGroup::asBoundary, rdoc::asBoundary)
        .def("snfRep", &MarkedAbelianGroup::snfRep, rdoc::snfRep)
        .def("ccRank", &MarkedAbelianGroup::ccRank, rdoc::ccRank)
        .def("cycleRank", &MarkedAbelianGroup::cycleRank, rdoc::cycleRank)
        .def("cycleGen", &MarkedAbelianGroup::cycleGen, rdoc::cycleGen)
        .def("m", &MarkedAbelianGroup::m,
            pybind11::return_value_policy::reference_internal, rdoc::m)
        .def("n", &MarkedAbelianGroup::n,
            pybind11::return_value_policy::reference_internal, rdoc::n)
        .def("coefficients", &MarkedAbelianGroup::coefficients,
            rdoc::coefficients)
        .def("torsionSubgroup", &MarkedAbelianGroup::torsionSubgroup,
            rdoc::torsionSubgroup)
        .def("torsionInclusion", &MarkedAbelianGroup::torsionInclusion,
            rdoc::torsionInclusion)
    ;
    regina::python::add_output(c1);
    regina::python::add_eq_operators(c1, rdoc::__eq);

    regina::python::add_global_swap<MarkedAbelianGroup>(m, rdoc::global_swap);

    RDOC_SCOPE_SWITCH(HomMarkedAbelianGroup)

    auto c2 = pybind11::class_<HomMarkedAbelianGroup>(m,
            "HomMarkedAbelianGroup", rdoc_scope)
        .def(pybind11::init<const MarkedAbelianGroup&,
                const MarkedAbelianGroup&, const MatrixInt&>(), rdoc::__init)
        .def(pybind11::init<const HomMarkedAbelianGroup&>(), rdoc::__copy)
        .def("swap", &HomMarkedAbelianGroup::swap, rdoc::swap)
        .def("isChainMap", &HomMarkedAbelianGroup::isChainMap, rdoc::isChainMap)
        .def("isCycleMap", &HomMarkedAbelianGroup::isCycleMap, rdoc::isCycleMap)
        .def("isEpic", &HomMarkedAbelianGroup::isEpic, rdoc::isEpic)
        .def("isMonic", &HomMarkedAbelianGroup::isMonic, rdoc::isMonic)
        .def("isIsomorphism", &HomMarkedAbelianGroup::isIsomorphism,
            rdoc::isIsomorphism)
        .def("isIdentity", &HomMarkedAbelianGroup::isIdentity, rdoc::isIdentity)
        .def("isZero", &HomMarkedAbelianGroup::isZero, rdoc::isZero)
        .def("kernel", &HomMarkedAbelianGroup::kernel,
            pybind11::return_value_policy::reference_internal, rdoc::kernel)
        .def("cokernel", &HomMarkedAbelianGroup::cokernel,
            pybind11::return_value_policy::reference_internal, rdoc::cokernel)
        .def("image", &HomMarkedAbelianGroup::image,
            pybind11::return_value_policy::reference_internal, rdoc::image)
        .def("domain", &HomMarkedAbelianGroup::domain,
            pybind11::return_value_policy::reference_internal, rdoc::domain)
        .def("codomain", &HomMarkedAbelianGroup::codomain,
            pybind11::return_value_policy::reference_internal, rdoc::codomain)
        .def("definingMatrix", &HomMarkedAbelianGroup::definingMatrix,
            pybind11::return_value_policy::reference_internal,
            rdoc::definingMatrix)
        .def("reducedMatrix", &HomMarkedAbelianGroup::reducedMatrix,
            pybind11::return_value_policy::reference_internal,
            rdoc::reducedMatrix)
        .def("summary", pybind11::overload_cast<>(
            &HomMarkedAbelianGroup::summary, pybind11::const_), rdoc::summary)
        .def("torsionSubgroup", &HomMarkedAbelianGroup::torsionSubgroup,
            rdoc::torsionSubgroup)
        .def("evalCC", &HomMarkedAbelianGroup::evalCC, rdoc::evalCC)
        .def("evalSNF", &HomMarkedAbelianGroup::evalSNF, rdoc::evalSNF)
        .def("inverseHom", &HomMarkedAbelianGroup::inverseHom, rdoc::inverseHom)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
    ;
    regina::python::add_output(c2);
    // Deciding what we want comparisons to *mean* requires some thought.
    // Let's not make a decision now that we might regret later.
    regina::python::disable_eq_operators(c2);

    regina::python::add_global_swap<HomMarkedAbelianGroup>(m,
        rdoc::global_swap);

    RDOC_SCOPE_END
}

