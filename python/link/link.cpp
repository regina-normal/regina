
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

#include "link/link.h"
#include "maths/nlaurent.h"
#include "maths/nlaurent2.h"
#include "triangulation/ntriangulation.h"
#include "../helpers.h"
#include "../safeheldtype.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Crossing;
using regina::StrandRef;
using regina::Link;

namespace {
    regina::Crossing* (Link::*crossing_nonconst)(size_t) = &Link::crossing;
    Link* (*fromOrientedGauss_str)(const std::string&) =
        &Link::fromOrientedGauss;
    Link* (*fromJenkins_str)(const std::string&) = &Link::fromJenkins;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_complement, Link::complement,
        0, 1);

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

    scope s = class_<Link, bases<regina::NPacket>, SafeHeldType<Link>,
            boost::noncopyable>("Link", init<>())
        .def(init<size_t>())
        .def(init<const Link&>())
        .def("size", &Link::size)
        .def("countComponents", &Link::countComponents)
        .def("crossing", crossing_nonconst,
            return_internal_reference<>())
        .def("component", &Link::component)
        .def("fromOrientedGauss", fromOrientedGauss_list,
            return_value_policy<to_held_type<>>())
        .def("fromOrientedGauss", fromOrientedGauss_str,
            return_value_policy<to_held_type<>>())
        .def("fromJenkins", fromJenkins_str,
            return_value_policy<to_held_type<>>())
        .def("reflect", &Link::reflect)
        .def("rotate", &Link::rotate)
        .def("writhe", &Link::writhe)
        .def("complement", &Link::complement,
            OL_complement()[return_value_policy<to_held_type<>>()])
        .def("bracket", &Link::bracket,
            return_value_policy<manage_new_object>())
        .def("jones", &Link::jones,
            return_value_policy<manage_new_object>())
        .def("homfly", &Link::homfly,
            return_value_policy<manage_new_object>())
        .staticmethod("fromOrientedGauss")
        .staticmethod("fromJenkins")
    ;

    s.attr("typeID") = regina::PACKET_LINK;
    s.attr("packetType") = regina::PACKET_LINK;
    s.attr("jonesVar") = Link::jonesVar;
    s.attr("homflyVarX") = Link::homflyVarX;
    s.attr("homflyVarY") = Link::homflyVarY;

    implicitly_convertible<SafeHeldType<Link>, SafeHeldType<regina::NPacket>>();
}
