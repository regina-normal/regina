
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
#include <pybind11/iostream.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "algebra/homgrouppresentation.h"
#include "algebra/markedabeliangroup.h"
#include "utilities/exception.h"
#include "../helpers.h"
#include "../docstrings/algebra/grouppresentation.h"

using namespace pybind11::literals;

using pybind11::overload_cast;
using regina::GroupExpressionTerm;
using regina::GroupExpression;
using regina::GroupPresentation;
using regina::SplayedExpression;

void addGroupPresentation(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(GroupExpressionTerm)

    auto c = pybind11::class_<GroupExpressionTerm>(m, "GroupExpressionTerm",
            rdoc::__class)
        .def_readwrite("generator", &GroupExpressionTerm::generator)
        .def_readwrite("exponent", &GroupExpressionTerm::exponent)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<size_t, long>(), rdoc::__init)
        .def(pybind11::init<const GroupExpressionTerm&>(), rdoc::__copy)
        .def("inverse", &GroupExpressionTerm::inverse, rdoc::inverse)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
    ;
    regina::python::add_output_ostream(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    RDOC_SCOPE_SWITCH(GroupExpression)

    auto c = pybind11::class_<GroupExpression>(m, "GroupExpression",
            rdoc::__class)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const GroupExpressionTerm&>(), rdoc::__init)
        .def(pybind11::init<size_t, long>(), rdoc::__init_2)
        .def(pybind11::init<const GroupExpression&>(), rdoc::__copy)
        .def(pybind11::init<const std::string&, size_t>(),
            "input"_a, "nGens"_a = 0, rdoc::__init_3)
        .def("swap", &GroupExpression::swap, rdoc::swap)
        .def("terms", overload_cast<>(&GroupExpression::terms),
            pybind11::return_value_policy::reference_internal, rdoc::terms)
        .def("countTerms", &GroupExpression::countTerms, rdoc::countTerms)
        .def("wordLength", &GroupExpression::wordLength, rdoc::wordLength)
        .def("empty", &GroupExpression::empty, rdoc::empty)
        .def("isTrivial", &GroupExpression::empty, rdoc::isTrivial) //deprecated
        .def("clear", &GroupExpression::clear, rdoc::clear)
        .def("erase", &GroupExpression::clear, rdoc::erase) // deprecated
        .def("term", overload_cast<size_t>(&GroupExpression::term),
            pybind11::return_value_policy::reference_internal, rdoc::term)
        .def("generator", &GroupExpression::generator, rdoc::generator)
        .def("exponent", &GroupExpression::exponent, rdoc::exponent)
        .def("addTermFirst", overload_cast<const GroupExpressionTerm&>(
            &GroupExpression::addTermFirst), rdoc::addTermFirst)
        .def("addTermFirst", overload_cast<size_t, long>(
            &GroupExpression::addTermFirst), rdoc::addTermFirst_2)
        .def("addTermLast", overload_cast<const GroupExpressionTerm&>(
            &GroupExpression::addTermLast), rdoc::addTermLast)
        .def("addTermLast", overload_cast<size_t, long>(
            &GroupExpression::addTermLast), rdoc::addTermLast_2)
        .def("addTermsFirst", &GroupExpression::addTermsFirst,
            rdoc::addTermsFirst)
        .def("addTermsLast", &GroupExpression::addTermsLast, rdoc::addTermsLast)
        .def("cycleLeft", &GroupExpression::cycleLeft, rdoc::cycleLeft)
        .def("cycleRight", &GroupExpression::cycleRight, rdoc::cycleRight)
        .def("inverse", &GroupExpression::inverse, rdoc::inverse)
        .def("invert", &GroupExpression::invert, rdoc::invert)
        .def("power", &GroupExpression::power, rdoc::power)
        .def("simplify", &GroupExpression::simplify,
            "cyclic"_a = false, rdoc::simplify)
        .def("substitute",
            overload_cast<size_t, const GroupExpression&, bool>(
                &GroupExpression::substitute),
            "generator"_a, "replacement"_a, "cyclic"_a = false,
            rdoc::substitute)
        .def("substitute",
            overload_cast<const std::vector<GroupExpression>&, bool>(
                &GroupExpression::substitute),
            "replacements"_a, "cyclic"_a = false, rdoc::substitute_2)
        .def("writeXMLData", [](const GroupExpression& e,
                pybind11::object file) {
            pybind11::scoped_ostream_redirect stream(std::cout, file);
            e.writeXMLData(std::cout);
        }, rdoc::writeXMLData)
        .def("tex", &GroupExpression::tex, rdoc::tex)
        .def("str",
            overload_cast<bool>(&GroupExpression::str, pybind11::const_),
            "alpha"_a, rdoc::str)
        .def("utf8",
            overload_cast<bool>(&GroupExpression::utf8, pybind11::const_),
            "alpha"_a, rdoc::utf8)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_global_swap<GroupExpression, rdoc>(m);

    RDOC_SCOPE_SWITCH(SplayedExpression)

    auto c = pybind11::class_<SplayedExpression>(m, "SplayedExpression",
            rdoc::__class)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const SplayedExpression&>(), rdoc::__copy)
        .def(pybind11::init<const GroupExpression&>(), rdoc::__init)
        .def(pybind11::init<const GroupExpression&, size_t>(), rdoc::__init_2)
        .def("desplay", &SplayedExpression::desplay, rdoc::desplay)
        .def("swap", &SplayedExpression::swap, rdoc::swap)
        .def("size", &SplayedExpression::size, rdoc::size)
        .def("empty", &SplayedExpression::empty, rdoc::empty)
        .def("__iter__", [](const SplayedExpression& exp) {
            return pybind11::make_iterator(exp);
        }, pybind11::keep_alive<0, 1>(), // iterator keeps expression alive
            rdoc::__iter__)
        .def("__getitem__", overload_cast<size_t>(
                &SplayedExpression::operator[], pybind11::const_),
            rdoc::__array)
        .def("__setitem__", [](SplayedExpression& exp, size_t index,
                    SplayedExpression::Term value) {
                exp[index] = value;
            }, rdoc::__array)
        .def("inverse", &SplayedExpression::inverse, rdoc::inverse)
        .def("invert", &SplayedExpression::invert, rdoc::invert)
        .def("simplify", &SplayedExpression::simplify,
            "cyclic"_a = false, rdoc::simplify)
        .def("substitute", &SplayedExpression::substitute,
            "generator"_a, "replacement"_a, "cyclic"_a = false,
            rdoc::substitute)
        .def("str",
            overload_cast<bool>(&SplayedExpression::str, pybind11::const_),
            "alpha"_a, rdoc::str)
        .def("utf8",
            overload_cast<bool>(&SplayedExpression::utf8, pybind11::const_),
            "alpha"_a, rdoc::utf8)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);
    regina::python::add_global_swap<SplayedExpression, rdoc>(m);

    RDOC_SCOPE_SWITCH(GroupPresentation)

    auto c = pybind11::class_<GroupPresentation>(m, "GroupPresentation",
            rdoc::__class)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const GroupPresentation&>(), rdoc::__copy)
        .def(pybind11::init<size_t>(), rdoc::__init)
        .def(pybind11::init<size_t, const std::vector<std::string>&>(),
            rdoc::__init_2)
        .def("swap", &GroupPresentation::swap, rdoc::swap)
        .def("addGenerator", &GroupPresentation::addGenerator,
            "numToAdd"_a = 1, rdoc::addGenerator)
        .def("addRelation", &GroupPresentation::addRelation, rdoc::addRelation)
        .def("countGenerators", &GroupPresentation::countGenerators,
            rdoc::countGenerators)
        .def("countRelations", &GroupPresentation::countRelations,
            rdoc::countRelations)
        .def("relation", &GroupPresentation::relation,
            pybind11::return_value_policy::reference_internal, rdoc::relation)
        .def("relations", &GroupPresentation::relations,
            pybind11::return_value_policy::reference_internal, rdoc::relations)
        .def("isValid", &GroupPresentation::isValid, rdoc::isValid)
        .def("simplify", &GroupPresentation::simplify,
            rdoc::simplify)
        .def("intelligentSimplify", &GroupPresentation::simplify, // deprecated
            rdoc::intelligentSimplify)
        .def("smallCancellation", &GroupPresentation::smallCancellation,
            rdoc::smallCancellation)
        .def("simplifyAndConjugate", overload_cast<GroupExpression&>(
                &GroupPresentation::simplifyAndConjugate, pybind11::const_),
            rdoc::simplifyAndConjugate)
        .def("simplifyAndConjugate", overload_cast<SplayedExpression&>(
                &GroupPresentation::simplifyAndConjugate, pybind11::const_),
            rdoc::simplifyAndConjugate_2)
        .def("proliferateRelators", &GroupPresentation::proliferateRelators,
            "depth"_a = 1, rdoc::proliferateRelators)
        .def("identifyAbelian", &GroupPresentation::identifyAbelian,
            rdoc::identifyAbelian)
        .def("nielsenTransposition", &GroupPresentation::nielsenTransposition,
            rdoc::nielsenTransposition)
        .def("nielsenInvert", &GroupPresentation::nielsenInvert,
            rdoc::nielsenInvert)
        .def("nielsenCombine", &GroupPresentation::nielsenCombine,
            "i"_a, "j"_a, "k"_a, "rightMult"_a = true, rdoc::nielsenCombine)
        .def("nielsen", &GroupPresentation::nielsen, rdoc::nielsen)
        .def("intelligentNielsen", &GroupPresentation::nielsen, // deprecated
            rdoc::intelligentNielsen)
        .def("homologicalAlignment", &GroupPresentation::homologicalAlignment,
            rdoc::homologicalAlignment)
        .def("prettyRewriting", &GroupPresentation::prettyRewriting,
            rdoc::prettyRewriting)
        .def("identifySimplyIsomorphicTo",
            &GroupPresentation::identifySimplyIsomorphicTo,
            rdoc::identifySimplyIsomorphicTo)
        .def("recogniseGroup", &GroupPresentation::recogniseGroup,
            "moreUtf8"_a = false, rdoc::recogniseGroup)
        .def("writeXMLData", [](const GroupPresentation& p,
                pybind11::object file) {
            pybind11::scoped_ostream_redirect stream(std::cout, file);
            p.writeXMLData(std::cout);
        }, rdoc::writeXMLData)
        .def("relatorLength", &GroupPresentation::relatorLength,
            rdoc::relatorLength)
        .def("abelianisation", &GroupPresentation::abelianisation,
            rdoc::abelianisation)
        .def("abelianRank", &GroupPresentation::abelianRank, rdoc::abelianRank)
        .def("markedAbelianisation", &GroupPresentation::markedAbelianisation,
            rdoc::markedAbelianisation)
        .def("enumerateCovers", [](const GroupPresentation& p, int index) {
            std::vector<GroupPresentation> ans;
            auto push = [&](GroupPresentation&& result) {
                ans.push_back(std::move(result));
            };
            switch (index) {
                case 2: p.enumerateCovers<2>(push); break;
                case 3: p.enumerateCovers<3>(push); break;
                case 4: p.enumerateCovers<4>(push); break;
                case 5: p.enumerateCovers<5>(push); break;
                case 6: p.enumerateCovers<6>(push); break;
                case 7: p.enumerateCovers<7>(push); break;
                case 8: p.enumerateCovers<8>(push); break;
                case 9: p.enumerateCovers<9>(push); break;
                case 10: p.enumerateCovers<10>(push); break;
                case 11: p.enumerateCovers<11>(push); break;
                default:
                    throw regina::InvalidArgument("The index passed to "
                        "enumerateCovers() must be between 2 and 11 "
                        "inclusive.");
            }
            return ans;
        }, "index"_a, rdoc::enumerateCovers)
        .def("enumerateCovers", [](const GroupPresentation& p, int index,
                const std::function<void(GroupPresentation&&)>& action)
                -> size_t {
            switch (index) {
                case 2: return p.enumerateCovers<2>(action);
                case 3: return p.enumerateCovers<3>(action);
                case 4: return p.enumerateCovers<4>(action);
                case 5: return p.enumerateCovers<5>(action);
                case 6: return p.enumerateCovers<6>(action);
                case 7: return p.enumerateCovers<7>(action);
                case 8: return p.enumerateCovers<8>(action);
                case 9: return p.enumerateCovers<9>(action);
                case 10: return p.enumerateCovers<10>(action);
                case 11: return p.enumerateCovers<11>(action);
            }
            throw regina::InvalidArgument("The index passed to "
                "enumerateCovers() must be between 2 and 11 inclusive.");
        }, "index"_a, "action"_a, rdoc::enumerateCovers)
        .def("incidence", &GroupPresentation::incidence, rdoc::incidence)
        .def("tex", &GroupPresentation::tex, rdoc::tex)
        .def("compact", &GroupPresentation::compact, rdoc::compact)
        .def("gap", &GroupPresentation::gap,
            "groupVariable"_a = "g", rdoc::gap)
    ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_global_swap<GroupPresentation, rdoc>(m);

    RDOC_SCOPE_END
}

