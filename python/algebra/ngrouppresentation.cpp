
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
#include "algebra/nabeliangroup.h"
#include "algebra/ngrouppresentation.h"
#include "algebra/nhomgrouppresentation.h"
#include "algebra/nmarkedabeliangroup.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NGroupExpressionTerm;
using regina::NGroupExpression;
using regina::NGroupPresentation;

namespace {
    void (NGroupExpression::*addTermFirst_term)(const NGroupExpressionTerm&) =
        &NGroupExpression::addTermFirst;
    void (NGroupExpression::*addTermFirst_pair)(unsigned long, long) =
        &NGroupExpression::addTermFirst;
    void (NGroupExpression::*addTermLast_term)(const NGroupExpressionTerm&) =
        &NGroupExpression::addTermLast;
    void (NGroupExpression::*addTermLast_pair)(unsigned long, long) =
        &NGroupExpression::addTermLast;
    NGroupExpressionTerm& (NGroupExpression::*term_non_const)(
        size_t) = &NGroupExpression::term;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_simplify,
        NGroupExpression::simplify, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_substitute,
        NGroupExpression::substitute, 2, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addGenerator,
        NGroupPresentation::addGenerator, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_nielsenCombine,
        NGroupPresentation::nielsenCombine, 3, 4);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_recogniseGroup,
        NGroupPresentation::recogniseGroup, 0, 1);

    std::auto_ptr<NGroupExpression> newExpression_str(
            const std::string& str) {
        return std::auto_ptr<NGroupExpression>(new NGroupExpression(str, 0));
    }

    object terms_list(const NGroupExpression& e) {
        boost::python::list ans;
        for (std::list<NGroupExpressionTerm>::const_iterator it =
                e.terms().begin(); it != e.terms().end(); it++)
            ans.append(*it);
        return ans;
    }

    void expressionWriteText(const NGroupExpression& e,
            bool sw = false, bool utf8 = false) {
        e.writeText(std::cout, sw, utf8);
    }

    void expressionWriteTeX(const NGroupExpression& e) {
        e.writeTeX(std::cout);
    }

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_expressionWriteText,
        expressionWriteText, 1, 3);

    void addRelation_clone(NGroupPresentation& p, const NGroupExpression& e) {
        p.addRelation(new regina::NGroupExpression(e));
    }

    void presentationWriteTeX(const NGroupPresentation& p) {
        p.writeTeX(std::cout);
    }

    void presentationWriteTextCompact(const NGroupPresentation& p) {
        p.writeTextCompact(std::cout);
    }

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_proliferateRelators,
        NGroupPresentation::proliferateRelators, 0, 1);
}

void addNGroupPresentation() {
    class_<NGroupExpressionTerm>("NGroupExpressionTerm")
        .def_readwrite("generator", &NGroupExpressionTerm::generator)
        .def_readwrite("exponent", &NGroupExpressionTerm::exponent)
        .def(init<unsigned long, long>())
        .def(init<const NGroupExpressionTerm&>())
        .def(self < self)
        .def("inverse", &NGroupExpressionTerm::inverse)
        .def(self += self)
        .def(self_ns::str(self))
        .def(regina::python::add_eq_operators())
    ;

    class_<NGroupExpression,
            std::auto_ptr<NGroupExpression>, boost::noncopyable>
            ("NGroupExpression")
        .def(init<const NGroupExpression&>())
        .def("__init__", boost::python::make_constructor(newExpression_str))
        .def("terms", terms_list)
        .def("getTerms", terms_list)
        .def("countTerms", &NGroupExpression::countTerms)
        .def("getNumberOfTerms", &NGroupExpression::countTerms)
        .def("wordLength", &NGroupExpression::wordLength)
        .def("isTrivial", &NGroupExpression::isTrivial)
        .def("erase", &NGroupExpression::erase)
        .def("getTerm", term_non_const, return_internal_reference<>())
        .def("term", term_non_const, return_internal_reference<>())
        .def("generator", &NGroupExpression::generator)
        .def("getGenerator", &NGroupExpression::generator)
        .def("exponent", &NGroupExpression::exponent)
        .def("getExponent", &NGroupExpression::exponent)
        .def("addTermFirst", addTermFirst_term)
        .def("addTermFirst", addTermFirst_pair)
        .def("addTermLast", addTermLast_term)
        .def("addTermLast", addTermLast_pair)
        .def("addTermsFirst", &NGroupExpression::addTermsFirst)
        .def("addTermsLast", &NGroupExpression::addTermsLast)
        .def("addStringFirst", &NGroupExpression::addStringFirst)
        .def("addStringLast", &NGroupExpression::addStringLast)
        .def("cycleLeft", &NGroupExpression::cycleLeft)
        .def("cycleRight", &NGroupExpression::cycleRight)
        .def("inverse", &NGroupExpression::inverse,
            return_value_policy<manage_new_object>())
        .def("invert", &NGroupExpression::invert)
        .def("power", &NGroupExpression::power,
            return_value_policy<manage_new_object>())
        .def("simplify", &NGroupExpression::simplify, OL_simplify())
        .def("substitute", &NGroupExpression::substitute, OL_substitute())
        .def("toTeX", &NGroupExpression::toTeX)
        .def("writeText", expressionWriteText, OL_expressionWriteText())
        .def("writeTeX", expressionWriteTeX)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    class_<NGroupPresentation,
            std::auto_ptr<NGroupPresentation>, boost::noncopyable>
            ("NGroupPresentation")
        .def(init<const NGroupPresentation&>())
        .def("addGenerator", &NGroupPresentation::addGenerator,
            OL_addGenerator())
        .def("addRelation", addRelation_clone)
        .def("countGenerators", &NGroupPresentation::countGenerators)
        .def("getNumberOfGenerators", &NGroupPresentation::countGenerators)
        .def("countRelations", &NGroupPresentation::countRelations)
        .def("getNumberOfRelations", &NGroupPresentation::countRelations)
        .def("relation", &NGroupPresentation::relation,
            return_internal_reference<>())
        .def("getRelation", &NGroupPresentation::relation,
            return_internal_reference<>())
        .def("isValid", &NGroupPresentation::isValid)
        .def("intelligentSimplify", &NGroupPresentation::intelligentSimplify)
        .def("intelligentSimplifyDetail",
            &NGroupPresentation::intelligentSimplifyDetail)
        .def("smallCancellation", &NGroupPresentation::smallCancellation)
        .def("smallCancellationDetail",
            &NGroupPresentation::smallCancellationDetail)
        .def("simplifyWord", &NGroupPresentation::simplifyWord)
        .def("proliferateRelators", &NGroupPresentation::proliferateRelators,
            OL_proliferateRelators())
        .def("identifyAbelian", &NGroupPresentation::identifyAbelian)
        .def("nielsenTransposition", &NGroupPresentation::nielsenTransposition)
        .def("nielsenInvert", &NGroupPresentation::nielsenInvert)
        .def("nielsenCombine", &NGroupPresentation::nielsenCombine,
            OL_nielsenCombine())
        .def("intelligentNielsen", &NGroupPresentation::intelligentNielsen)
        .def("intelligentNielsenDetail",
            &NGroupPresentation::intelligentNielsenDetail)
        .def("homologicalAlignment", &NGroupPresentation::homologicalAlignment)
        .def("homologicalAlignmentDetail",
            &NGroupPresentation::homologicalAlignmentDetail)
        .def("prettyRewriting", &NGroupPresentation::prettyRewriting)
        .def("prettyRewritingDetail",
            &NGroupPresentation::prettyRewritingDetail)
        .def("identifySimplyIsomorphicTo",
            &NGroupPresentation::identifySimplyIsomorphicTo)
        .def("recogniseGroup", &NGroupPresentation::recogniseGroup,
            OL_recogniseGroup())
        .def("relatorLength", &NGroupPresentation::relatorLength)
        .def("abelianisation", &NGroupPresentation::abelianisation)
        .def("markedAbelianisation", &NGroupPresentation::markedAbelianisation)
        .def("toTeX", &NGroupPresentation::toTeX)
        .def("toStringCompact", &NGroupPresentation::compact)
        .def("compact", &NGroupPresentation::compact)
        .def("writeTeX", presentationWriteTeX)
        .def("writeTextCompact", presentationWriteTextCompact)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
}

