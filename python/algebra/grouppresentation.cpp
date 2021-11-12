
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
#include "../pybind11/functional.h"
#include "../pybind11/operators.h"
#include "../pybind11/stl.h"
#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "algebra/homgrouppresentation.h"
#include "algebra/markedabeliangroup.h"
#include "utilities/exception.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::GroupExpressionTerm;
using regina::GroupExpression;
using regina::GroupPresentation;

void addGroupPresentation(pybind11::module_& m) {
    auto c1 = pybind11::class_<GroupExpressionTerm>(m, "GroupExpressionTerm")
        .def_readwrite("generator", &GroupExpressionTerm::generator)
        .def_readwrite("exponent", &GroupExpressionTerm::exponent)
        .def(pybind11::init<>())
        .def(pybind11::init<unsigned long, long>())
        .def(pybind11::init<const GroupExpressionTerm&>())
        .def(pybind11::self < pybind11::self)
        .def("inverse", &GroupExpressionTerm::inverse)
        .def(pybind11::self += pybind11::self)
    ;
    regina::python::add_output_ostream(c1);
    regina::python::add_eq_operators(c1);

    auto c2 = pybind11::class_<GroupExpression>(m, "GroupExpression")
        .def(pybind11::init<>())
        .def(pybind11::init<const GroupExpressionTerm&>())
        .def(pybind11::init<unsigned long, long>())
        .def(pybind11::init<const GroupExpression&>())
        .def(pybind11::init<const std::string&>())
        .def("swap", &GroupExpression::swap)
        .def("terms", overload_cast<>(
            &GroupExpression::terms, pybind11::const_))
        .def("countTerms", &GroupExpression::countTerms)
        .def("wordLength", &GroupExpression::wordLength)
        .def("isTrivial", &GroupExpression::isTrivial)
        .def("erase", &GroupExpression::erase)
        .def("term", overload_cast<size_t>(&GroupExpression::term),
            pybind11::return_value_policy::reference_internal)
        .def("generator", &GroupExpression::generator)
        .def("exponent", &GroupExpression::exponent)
        .def("addTermFirst", overload_cast<const GroupExpressionTerm&>(
            &GroupExpression::addTermFirst))
        .def("addTermFirst", overload_cast<unsigned long, long>(
            &GroupExpression::addTermFirst))
        .def("addTermLast", overload_cast<const GroupExpressionTerm&>(
            &GroupExpression::addTermLast))
        .def("addTermLast", overload_cast<unsigned long, long>(
            &GroupExpression::addTermLast))
        .def("addTermsFirst", &GroupExpression::addTermsFirst)
        .def("addTermsLast", &GroupExpression::addTermsLast)
        .def("addStringFirst", [](GroupExpression& g, const std::string& s) {
            // This is deprecated, so we reimplement it here.
            try {
                g.addTermsFirst(s);
            } catch (const regina::InvalidArgument&) {
                return false;
            }
            return true;
        })
        .def("addStringLast", [](GroupExpression& g, const std::string& s) {
            // This is deprecated, so we reimplement it here.
            try {
                g.addTermsLast(s);
            } catch (const regina::InvalidArgument&) {
                return false;
            }
            return true;
        })
        .def("cycleLeft", &GroupExpression::cycleLeft)
        .def("cycleRight", &GroupExpression::cycleRight)
        .def("inverse", &GroupExpression::inverse)
        .def("invert", &GroupExpression::invert)
        .def("power", &GroupExpression::power)
        .def("simplify", &GroupExpression::simplify,
            pybind11::arg("cyclic") = false)
        .def("substitute",
            overload_cast<unsigned long, const GroupExpression&, bool>(
                &GroupExpression::substitute),
            pybind11::arg(), pybind11::arg(), pybind11::arg("cyclic") = false)
        .def("substitute",
            overload_cast<const std::vector<GroupExpression>&, bool>(
                &GroupExpression::substitute),
            pybind11::arg(), pybind11::arg("cyclic") = false)
        .def("tex", &GroupExpression::tex)
        .def("toTeX", &GroupExpression::tex) // deprecated
        .def("writeText", [](const GroupExpression& e, bool sw, bool utf8) {
            e.writeText(std::cout, sw, utf8);
        }, pybind11::arg("shortword") = false, pybind11::arg("utf8") = false)
    ;
    regina::python::add_output(c2);
    regina::python::add_eq_operators(c2);

    m.def("swap", (void(*)(GroupExpression&, GroupExpression&))(regina::swap));

    auto c3 = pybind11::class_<GroupPresentation>(m, "GroupPresentation")
        .def(pybind11::init<>())
        .def(pybind11::init<unsigned long>())
        .def(pybind11::init<unsigned long, const std::vector<std::string>&>())
        .def(pybind11::init<const GroupPresentation&>())
        .def("swap", &GroupPresentation::swap)
        .def("addGenerator", &GroupPresentation::addGenerator,
            pybind11::arg("numToAdd") = 1)
        .def("addRelation", &GroupPresentation::addRelation)
        .def("countGenerators", &GroupPresentation::countGenerators)
        .def("countRelations", &GroupPresentation::countRelations)
        .def("relation", &GroupPresentation::relation,
            pybind11::return_value_policy::reference_internal)
        .def("relations", &GroupPresentation::relations)
        .def("isValid", &GroupPresentation::isValid)
        .def("intelligentSimplify", &GroupPresentation::intelligentSimplify)
        .def("intelligentSimplifyDetail",
            &GroupPresentation::intelligentSimplify) // deprecated
        .def("smallCancellation", &GroupPresentation::smallCancellation)
        .def("smallCancellationDetail",
            &GroupPresentation::smallCancellation) // deprecated
        .def("simplifyWord", &GroupPresentation::simplifyWord)
        .def("proliferateRelators", &GroupPresentation::proliferateRelators,
            pybind11::arg("depth") = 1)
        .def("identifyAbelian", &GroupPresentation::identifyAbelian)
        .def("nielsenTransposition", &GroupPresentation::nielsenTransposition)
        .def("nielsenInvert", &GroupPresentation::nielsenInvert)
        .def("nielsenCombine", &GroupPresentation::nielsenCombine,
            pybind11::arg(), pybind11::arg(), pybind11::arg(),
            pybind11::arg("rightMult") = true)
        .def("intelligentNielsen", &GroupPresentation::intelligentNielsen)
        .def("intelligentNielsenDetail",
            &GroupPresentation::intelligentNielsen) // deprecated
        .def("homologicalAlignment", &GroupPresentation::homologicalAlignment)
        .def("homologicalAlignmentDetail",
            &GroupPresentation::homologicalAlignment) // deprecated
        .def("prettyRewriting", &GroupPresentation::prettyRewriting)
        .def("prettyRewritingDetail",
            &GroupPresentation::prettyRewriting) // deprecated
        .def("identifySimplyIsomorphicTo",
            &GroupPresentation::identifySimplyIsomorphicTo)
        .def("recogniseGroup", &GroupPresentation::recogniseGroup,
            pybind11::arg("moreUtf8") = false)
        .def("relatorLength", &GroupPresentation::relatorLength)
        .def("abelianisation", &GroupPresentation::abelianisation)
        .def("abelianRank", &GroupPresentation::abelianRank)
        .def("markedAbelianisation", &GroupPresentation::markedAbelianisation)
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
                default:
                    PyErr_SetString(PyExc_ValueError,
                        "The index passed to enumerateCovers() must be between "
                        "2 and 7 inclusive.");
            }
            return ans;
        })
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
            }
            PyErr_SetString(PyExc_ValueError,
                "The index passed to enumerateCovers() must be between "
                "2 and 7 inclusive.");
            return 0;
        })
        .def("incidence", &GroupPresentation::incidence)
        .def("tex", &GroupPresentation::tex)
        .def("toTeX", &GroupPresentation::tex) // deprecated
        .def("compact", &GroupPresentation::compact)
        .def("gap", &GroupPresentation::gap,
            pybind11::arg("groupVariable") = "g")
    ;
    regina::python::add_output(c3);
    regina::python::add_eq_operators(c3);

    m.def("swap",
        (void(*)(GroupPresentation&, GroupPresentation&))(regina::swap));
}

