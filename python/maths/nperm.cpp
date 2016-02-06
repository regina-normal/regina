
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

// We need to see Python.h first to avoid a "portability fix" in pyport.h
// that breaks boost.python on MacOSX.
#include "Python.h"
#include <boost/python.hpp>
#include "maths/nperm.h"
#include "maths/nperm5.h"
#include "maths/nperm4.h"
#include "maths/nperm3.h"
#include "maths/nperm2.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NPerm;

namespace {
    template <int n>
    boost::shared_ptr<NPerm<n>> fromList(boost::python::list l) {
        long len = boost::python::len(l);
        if ( len != n ) {
            char err[80];
            snprintf(err, 80, "Initialisation list for NPerm%d must contain "
                              "exactly %d integers.", n, n);
            PyErr_SetString(PyExc_ValueError, err);
            boost::python::throw_error_already_set();
        }
        int image[n];
        for ( long i = 0; i < n; i++) {
            extract<int> val(l[i]);
            if (!val.check()) {
                // Throws an exception
                val();
            }
            image[i] = val();
        }
        return boost::shared_ptr<NPerm<n>>(new NPerm<n>(image));
    }

    template <int n, int k>
    struct NPerm_extend : boost::python::def_visitor<NPerm_extend<n, k>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("extend", &NPerm<n>::template extend<k>);
            c.def(NPerm_extend<n, k-1>());
        }
    };

    template <int n>
    struct NPerm_extend<n, 2> : boost::python::def_visitor<NPerm_extend<n, 2>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("extend", &NPerm<n>::template extend<2>);
            c.staticmethod("extend");
        }
    };

    template <int n, int k>
    struct NPerm_contract : boost::python::def_visitor<NPerm_contract<n, k>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &NPerm<n>::template contract<k>);
            c.def(NPerm_contract<n, k+1>());
        }
    };

    template <int n>
    struct NPerm_contract<n, 16> :
            boost::python::def_visitor<NPerm_contract<n, 16>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            c.def("contract", &NPerm<n>::template contract<16>);
            c.staticmethod("contract");
        }
    };

    template <int n>
    struct NPerm_contract<n, 17> :
            boost::python::def_visitor<NPerm_contract<n, 17>> {
        friend class boost::python::def_visitor_access;

        template <typename Class>
        void visit(Class& c) const {
            // Only called for NPerm16, which has no contract() methods at all.
        }
    };
}

template <int n>
void addNPerm(const char* name) {
    scope s = class_<NPerm<n> >(name)
        .def(init<int, int>())
        .def(init<const NPerm<n>&>())
        .def("__init__", make_constructor(fromList<n>))
        .def("permCode", &NPerm<n>::permCode)
        .def("getPermCode", &NPerm<n>::permCode)
        .def("setPermCode", &NPerm<n>::setPermCode)
        .def("fromPermCode", &NPerm<n>::fromPermCode)
        .def("isPermCode", &NPerm<n>::isPermCode)
        .def(self * self)
        .def("inverse", &NPerm<n>::inverse)
        .def("reverse", &NPerm<n>::reverse)
        .def("sign", &NPerm<n>::sign)
        .def("__getitem__", &NPerm<n>::operator[])
        .def("preImageOf", &NPerm<n>::preImageOf)
        .def("compareWith", &NPerm<n>::compareWith)
        .def("isIdentity", &NPerm<n>::isIdentity)
        .def("atIndex", &NPerm<n>::atIndex)
        .def("index", &NPerm<n>::index)
        .def("rand", &NPerm<n>::rand)
        .def("str", &NPerm<n>::str)
        .def("trunc", &NPerm<n>::trunc)
        .def("__str__", &NPerm<n>::str)
        .def("__repr__", &NPerm<n>::str)
        .def(NPerm_extend<n, n-1>())
        .def(NPerm_contract<n, n+1>())
        .def(regina::python::add_eq_operators())
        .staticmethod("fromPermCode")
        .staticmethod("isPermCode")
        .staticmethod("atIndex")
        .staticmethod("rand")
    ;

    s.attr("nPerms") = NPerm<n>::nPerms;
    s.attr("nPerms_1") = NPerm<n>::nPerms_1;
    s.attr("imageBits") = NPerm<n>::imageBits;
}

void addNPerm() {
    boost::python::def("digit", regina::digit);
    boost::python::def("factorial", regina::factorial);

    addNPerm<6>("NPerm6");
    addNPerm<7>("NPerm7");
    addNPerm<8>("NPerm8");
    addNPerm<9>("NPerm9");
    addNPerm<10>("NPerm10");
    addNPerm<11>("NPerm11");
    addNPerm<12>("NPerm12");
    addNPerm<13>("NPerm13");
    addNPerm<14>("NPerm14");
    addNPerm<15>("NPerm15");
    addNPerm<16>("NPerm16");
}

