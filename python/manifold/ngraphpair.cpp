
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include "manifold/ngraphpair.h"
#include "manifold/nsfs.h"
#include <boost/python.hpp>

#ifdef BOOST_MAKE_CONSTRUCTOR_HACK
#include <boost/python/make_constructor.hpp>
#endif

using namespace boost::python;
using regina::NGraphPair;
using regina::NMatrix2;
using regina::NSFSpace;

namespace {
    NGraphPair* createNGraphPair_longs(
            std::auto_ptr<NSFSpace> s1, std::auto_ptr<NSFSpace> s2,
            long a, long b, long c, long d) {
        NGraphPair* ans = new NGraphPair(s1.get(), s2.get(), a, b, c, d);
        s1.release();
        s2.release();
        return ans;
    }

    NGraphPair* createNGraphPair_matrix(
            std::auto_ptr<NSFSpace> s1, std::auto_ptr<NSFSpace> s2,
            const NMatrix2& m) {
        NGraphPair* ans = new NGraphPair(s1.get(), s2.get(), m);
        s1.release();
        s2.release();
        return ans;
    }
}

void addNGraphPair() {
    class_<NGraphPair, bases<regina::NManifold>,
            std::auto_ptr<NGraphPair>, boost::noncopyable>
            ("NGraphPair", no_init)
        .def("__init__", make_constructor(createNGraphPair_longs))
        .def("__init__", make_constructor(createNGraphPair_matrix))
        .def("sfs", &NGraphPair::sfs,
            return_internal_reference<>())
        .def("matchingReln", &NGraphPair::matchingReln,
            return_internal_reference<>())
        .def(self < self)
    ;

    implicitly_convertible<std::auto_ptr<NGraphPair>,
        std::auto_ptr<regina::NManifold> >();
}

