
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "algebra/homgrouppresentation.h"
#include "algebra/markedabeliangroup.h"
#include "../helpers.h"

using namespace boost::python;
using regina::GroupExpressionTerm;
using regina::GroupExpression;
using regina::GroupPresentation;

namespace {
    void (GroupExpression::*addTermFirst_term)(const GroupExpressionTerm&) =
        &GroupExpression::addTermFirst;
    void (GroupExpression::*addTermFirst_pair)(unsigned long, long) =
        &GroupExpression::addTermFirst;
    void (GroupExpression::*addTermLast_term)(const GroupExpressionTerm&) =
        &GroupExpression::addTermLast;
    void (GroupExpression::*addTermLast_pair)(unsigned long, long) =
        &GroupExpression::addTermLast;
    GroupExpressionTerm& (GroupExpression::*term_non_const)(
        size_t) = &GroupExpression::term;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_simplify,
        GroupExpression::simplify, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_substitute,
        GroupExpression::substitute, 2, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addGenerator,
        GroupPresentation::addGenerator, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_nielsenCombine,
        GroupPresentation::nielsenCombine, 3, 4);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_recogniseGroup,
        GroupPresentation::recogniseGroup, 0, 1);

    std::auto_ptr<GroupExpression> newExpression_str(
            const std::string& str) {
        return std::auto_ptr<GroupExpression>(new GroupExpression(str, 0));
    }

    object terms_list(const GroupExpression& e) {
        boost::python::list ans;
        for (std::list<GroupExpressionTerm>::const_iterator it =
                e.terms().begin(); it != e.terms().end(); it++)
            ans.append(*it);
        return ans;
    }

    void expressionWriteText(const GroupExpression& e,
            bool sw = false, bool utf8 = false) {
        e.writeText(std::cout, sw, utf8);
    }

    void expressionWriteTeX(const GroupExpression& e) {
        e.writeTeX(std::cout);
    }

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_expressionWriteText,
        expressionWriteText, 1, 3);

    void addRelation_clone(GroupPresentation& p, const GroupExpression& e) {
        p.addRelation(new regina::GroupExpression(e));
    }

    void presentationWriteTeX(const GroupPresentation& p) {
        p.writeTeX(std::cout);
    }

    void presentationWriteTextCompact(const GroupPresentation& p) {
        p.writeTextCompact(std::cout);
    }

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_proliferateRelators,
        GroupPresentation::proliferateRelators, 0, 1);
}

void addGroupPresentation() {
    class_<GroupExpressionTerm>("GroupExpressionTerm")
        .def_readwrite("generator", &GroupExpressionTerm::generator)
        .def_readwrite("exponent", &GroupExpressionTerm::exponent)
        .def(init<unsigned long, long>())
        .def(init<const GroupExpressionTerm&>())
        .def(self < self)
        .def("inverse", &GroupExpressionTerm::inverse)
        .def(self += self)
        .def(self_ns::str(self))
        .def(regina::python::add_eq_operators())
    ;
    scope().attr("NGroupExpressionTerm") = scope().attr("GroupExpressionTerm");

    class_<GroupExpression,
            std::auto_ptr<GroupExpression>, boost::noncopyable>
            ("GroupExpression")
        .def(init<const GroupExpression&>())
        .def("__init__", boost::python::make_constructor(newExpression_str))
        .def("terms", terms_list)
        .def("countTerms", &GroupExpression::countTerms)
        .def("wordLength", &GroupExpression::wordLength)
        .def("isTrivial", &GroupExpression::isTrivial)
        .def("erase", &GroupExpression::erase)
        .def("term", term_non_const, return_internal_reference<>())
        .def("generator", &GroupExpression::generator)
        .def("exponent", &GroupExpression::exponent)
        .def("addTermFirst", addTermFirst_term)
        .def("addTermFirst", addTermFirst_pair)
        .def("addTermLast", addTermLast_term)
        .def("addTermLast", addTermLast_pair)
        .def("addTermsFirst", &GroupExpression::addTermsFirst)
        .def("addTermsLast", &GroupExpression::addTermsLast)
        .def("addStringFirst", &GroupExpression::addStringFirst)
        .def("addStringLast", &GroupExpression::addStringLast)
        .def("cycleLeft", &GroupExpression::cycleLeft)
        .def("cycleRight", &GroupExpression::cycleRight)
        .def("inverse", &GroupExpression::inverse,
            return_value_policy<manage_new_object>())
        .def("invert", &GroupExpression::invert)
        .def("power", &GroupExpression::power,
            return_value_policy<manage_new_object>())
        .def("simplify", &GroupExpression::simplify, OL_simplify())
        .def("substitute", &GroupExpression::substitute, OL_substitute())
        .def("toTeX", &GroupExpression::toTeX)
        .def("writeText", expressionWriteText, OL_expressionWriteText())
        .def("writeTeX", expressionWriteTeX)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
    scope().attr("NGroupExpression") = scope().attr("GroupExpression");

    class_<GroupPresentation,
            std::auto_ptr<GroupPresentation>, boost::noncopyable>
            ("GroupPresentation")
        .def(init<const GroupPresentation&>())
        .def("addGenerator", &GroupPresentation::addGenerator,
            OL_addGenerator())
        .def("addRelation", addRelation_clone)
        .def("countGenerators", &GroupPresentation::countGenerators)
        .def("countRelations", &GroupPresentation::countRelations)
        .def("relation", &GroupPresentation::relation,
            return_internal_reference<>())
        .def("isValid", &GroupPresentation::isValid)
        .def("intelligentSimplify", &GroupPresentation::intelligentSimplify)
        .def("intelligentSimplifyDetail",
            &GroupPresentation::intelligentSimplifyDetail)
        .def("smallCancellation", &GroupPresentation::smallCancellation)
        .def("smallCancellationDetail",
            &GroupPresentation::smallCancellationDetail)
        .def("simplifyWord", &GroupPresentation::simplifyWord)
        .def("proliferateRelators", &GroupPresentation::proliferateRelators,
            OL_proliferateRelators())
        .def("identifyAbelian", &GroupPresentation::identifyAbelian)
        .def("nielsenTransposition", &GroupPresentation::nielsenTransposition)
        .def("nielsenInvert", &GroupPresentation::nielsenInvert)
        .def("nielsenCombine", &GroupPresentation::nielsenCombine,
            OL_nielsenCombine())
        .def("intelligentNielsen", &GroupPresentation::intelligentNielsen)
        .def("intelligentNielsenDetail",
            &GroupPresentation::intelligentNielsenDetail)
        .def("homologicalAlignment", &GroupPresentation::homologicalAlignment)
        .def("homologicalAlignmentDetail",
            &GroupPresentation::homologicalAlignmentDetail)
        .def("prettyRewriting", &GroupPresentation::prettyRewriting)
        .def("prettyRewritingDetail",
            &GroupPresentation::prettyRewritingDetail)
        .def("identifySimplyIsomorphicTo",
            &GroupPresentation::identifySimplyIsomorphicTo)
        .def("recogniseGroup", &GroupPresentation::recogniseGroup,
            OL_recogniseGroup())
        .def("relatorLength", &GroupPresentation::relatorLength)
        .def("abelianisation", &GroupPresentation::abelianisation)
        .def("markedAbelianisation", &GroupPresentation::markedAbelianisation)
        .def("toTeX", &GroupPresentation::toTeX)
        .def("compact", &GroupPresentation::compact)
        .def("writeTeX", presentationWriteTeX)
        .def("writeTextCompact", presentationWriteTextCompact)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
    scope().attr("NGroupPresentation") = scope().attr("GroupPresentation");
}

