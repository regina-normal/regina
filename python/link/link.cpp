
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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

#include "link/link.h"
#include "maths/laurent.h"
#include "maths/laurent2.h"
#include "triangulation/dim3.h"
#include "../safeheldtype.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>
#include "../helpers.h"

using namespace boost::python;
using namespace regina::python;
using regina::Crossing;
using regina::StrandRef;
using regina::Link;

namespace {
    std::string (Link::*gauss_str)() const = &Link::gauss;
    std::string (Link::*orientedGauss_str)() const = &Link::orientedGauss;
    std::string (Link::*jenkins_str)() const = &Link::jenkins;
    std::string (Link::*dt_str)(bool) const = &Link::dt;
    Link* (*fromGauss_str)(const std::string&) = &Link::fromGauss;
    Link* (*fromOrientedGauss_str)(const std::string&) =
        &Link::fromOrientedGauss;
    Link* (*fromJenkins_str)(const std::string&) = &Link::fromJenkins;
    Link* (*fromDT_str)(const std::string&) = &Link::fromDT;

    bool (Link::*r1a)(Crossing*, bool, bool) = &Link::r1;
    bool (Link::*r1b)(StrandRef, int, int, bool, bool) = &Link::r1;
    bool (Link::*r2a)(Crossing*, bool, bool) = &Link::r2;
    bool (Link::*r2b)(StrandRef, bool, bool) = &Link::r2;
    bool (Link::*r2c)(StrandRef, int, StrandRef, int, bool, bool) = &Link::r2;
    bool (Link::*r3a)(Crossing*, int, bool, bool) = &Link::r3;
    bool (Link::*r3b)(StrandRef, int, bool, bool) = &Link::r3;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_complement, Link::complement,
        0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_r1a, Link::r1, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_r1b, Link::r1, 3, 5);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_r2a, Link::r2, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_r2b, Link::r2, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_r2c, Link::r2, 4, 6);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_r3a, Link::r3, 2, 4);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_r3b, Link::r3, 2, 4);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_simplifyToLocalMinimum,
        Link::simplifyToLocalMinimum, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_jones, Link::jones, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_homfly, Link::homfly, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_homflyAZ, Link::homflyAZ, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_homflyLM, Link::homflyLM, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_bracket, Link::bracket, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_knotSig, Link::knotSig, 0, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_simplifyExhaustive,
        Link::simplifyExhaustive, 0, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_dt, Link::dt, 0, 1);

    Link* fromGauss_list(boost::python::list terms) {
        long len = boost::python::len(terms);

        int* s = new int[len];
        for (long i = 0; i < len; ++i) {
            extract<int> val(terms[i]);
            if (! val.check()) {
                // Throw an exception.
                delete[] s;
                val();
            }
            s[i] = val();
        }

        Link* ans = Link::fromGauss(s, s + len);
        delete[] s;
        return ans;
    }

    Link* fromOrientedGauss_list(boost::python::list terms) {
        long len = boost::python::len(terms);

        std::string* s = new std::string[len];
        for (long i = 0; i < len; ++i) {
            extract<std::string> val(terms[i]);
            if (! val.check()) {
                // Throw an exception.
                delete[] s;
                val();
            }
            s[i] = val();
        }

        Link* ans = Link::fromOrientedGauss(s, s + len);
        delete[] s;
        return ans;
    }

    Link* fromDT_list(boost::python::list terms) {
        long len = boost::python::len(terms);

        int* s = new int[len];
        for (long i = 0; i < len; ++i) {
            extract<int> val(terms[i]);
            if (! val.check()) {
                // Throw an exception.
                delete[] s;
                val();
            }
            s[i] = val();
        }

        Link* ans = Link::fromDT(s, s + len);
        delete[] s;
        return ans;
    }

    void strand_inc_operator(StrandRef& s) {
       ++s;
    }

    void strand_dec_operator(StrandRef& s) {
       --s;
    }
}

void addLink() {
    class_<StrandRef>("StrandRef", init<>())
        .def(init<Crossing*, int>())
        .def(init<const StrandRef&>())
        .def("crossing", &StrandRef::crossing,
            return_value_policy<reference_existing_object>())
        .def("strand", &StrandRef::strand)
        .def("inc", strand_inc_operator)
        .def("dec", strand_dec_operator)
        .def("next", &StrandRef::next)
        .def("prev", &StrandRef::prev)
        .def("jump", &StrandRef::jump)
        .def(self_ns::str(self))
        .def(self_ns::repr(self))
        .def(regina::python::add_eq_operators())
    ;

    class_<Crossing, std::auto_ptr<Crossing>, boost::noncopyable>
            ("Crossing", no_init)
        .def("index", &Crossing::index)
        .def("sign", &Crossing::sign)
        .def("upper", &Crossing::upper)
        .def("lower", &Crossing::lower)
        .def("over", &Crossing::over)
        .def("under", &Crossing::under)
        .def("strand", &Crossing::strand)
        .def("next", &Crossing::next)
        .def("prev", &Crossing::prev)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope s = class_<Link, bases<regina::Packet>, SafeHeldType<Link>,
            boost::noncopyable>("Link", init<>())
        .def(init<size_t>())
        .def(init<const Link&>())
        .def(init<const Link&, bool>())
        .def(init<const std::string&>())
        .def("isEmpty", &Link::isEmpty)
        .def("size", &Link::size)
        .def("countComponents", &Link::countComponents)
        .def("crossing", &Link::crossing, return_internal_reference<>())
        .def("component", &Link::component)
        .def("fromGauss", fromGauss_list,
            return_value_policy<to_held_type<>>())
        .def("fromGauss", fromGauss_str,
            return_value_policy<to_held_type<>>())
        .def("fromOrientedGauss", fromOrientedGauss_list,
            return_value_policy<to_held_type<>>())
        .def("fromOrientedGauss", fromOrientedGauss_str,
            return_value_policy<to_held_type<>>())
        .def("fromJenkins", fromJenkins_str,
            return_value_policy<to_held_type<>>())
        .def("fromDT", fromDT_list,
            return_value_policy<to_held_type<>>())
        .def("fromDT", fromDT_str,
            return_value_policy<to_held_type<>>())
        .def("fromKnotSig", &Link::fromKnotSig,
            return_value_policy<to_held_type<>>())
        .def("swapContents", &Link::swapContents)
        .def("reflect", &Link::reflect)
        .def("rotate", &Link::rotate)
        .def("reverse", &Link::reverse)
        .def("change", &Link::change)
        .def("resolve", &Link::resolve)
        .def("isAlternating", &Link::isAlternating)
        .def("writhe", &Link::writhe)
        .def("complement", &Link::complement,
            OL_complement()[return_value_policy<to_held_type<>>()])
        .def("connected", &Link::connected)
        .def("bracket", &Link::bracket,
            OL_bracket()[return_internal_reference<>()])
        .def("jones", &Link::jones,
            OL_jones()[return_internal_reference<>()])
        .def("homfly", &Link::homfly,
            OL_homfly()[return_internal_reference<>()])
        .def("homflyAZ", &Link::homflyAZ,
            OL_homflyAZ()[return_internal_reference<>()])
        .def("homflyLM", &Link::homflyLM,
            OL_homflyLM()[return_internal_reference<>()])
        .def("knowsBracket", &Link::knowsBracket)
        .def("knowsJones", &Link::knowsJones)
        .def("knowsHomfly", &Link::knowsHomfly)
        .def("niceTreeDecomposition", &Link::niceTreeDecomposition,
            return_internal_reference<>())
        .def("brief", &Link::brief)
        .def("gauss", gauss_str)
        .def("orientedGauss", orientedGauss_str)
        .def("jenkins", jenkins_str)
        .def("dt", dt_str, OL_dt())
        .def("knotSig", &Link::knotSig, OL_knotSig())
        .def("dumpConstruction", &Link::dumpConstruction)
        .def("r1", r1a, OL_r1a())
        .def("r1", r1b, OL_r1b())
        .def("r2", r2a, OL_r2a())
        .def("r2", r2b, OL_r2b())
        .def("r2", r2c, OL_r2c())
        .def("r3", r3a, OL_r3a())
        .def("r3", r3b, OL_r3b())
        .def("intelligentSimplify", &Link::intelligentSimplify)
        .def("simplifyToLocalMinimum", &Link::simplifyToLocalMinimum,
             OL_simplifyToLocalMinimum())
        .def("simplifyExhaustive", &Link::simplifyExhaustive,
             OL_simplifyExhaustive())
        .staticmethod("fromGauss")
        .staticmethod("fromOrientedGauss")
        .staticmethod("fromJenkins")
        .staticmethod("fromDT")
        .staticmethod("fromKnotSig")
    ;

    s.attr("typeID") = regina::PACKET_LINK;
    s.attr("jonesVar") = Link::jonesVar;
    s.attr("homflyVarX") = Link::homflyVarX;
    s.attr("homflyVarY") = Link::homflyVarY;
    s.attr("homflyAZVarX") = Link::homflyAZVarX;
    s.attr("homflyAZVarY") = Link::homflyAZVarY;
    s.attr("homflyLMVarX") = Link::homflyLMVarX;
    s.attr("homflyLMVarY") = Link::homflyLMVarY;

    implicitly_convertible<SafeHeldType<Link>, SafeHeldType<regina::Packet>>();
}
