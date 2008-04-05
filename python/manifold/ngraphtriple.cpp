
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "regina-config.h"
#include "manifold/ngraphtriple.h"
#include "manifold/nsfs.h"
#include <boost/python.hpp>

#ifdef BOOST_MAKE_CONSTRUCTOR_HACK
#include <boost/python/make_constructor.hpp>
#endif

using namespace boost::python;
using regina::NGraphTriple;
using regina::NMatrix2;
using regina::NSFSpace;

namespace {
    NGraphTriple* createNGraphTriple(std::auto_ptr<NSFSpace> s1,
            std::auto_ptr<NSFSpace> s2, std::auto_ptr<NSFSpace> s3,
            const NMatrix2& m1, const NMatrix2& m2) {
        NGraphTriple* ans = new NGraphTriple(s1.get(), s2.get(), s3.get(),
            m1, m2);
        s1.release();
        s2.release();
        s3.release();
        return ans;
    }
}

void addNGraphTriple() {
    class_<NGraphTriple, bases<regina::NManifold>,
            std::auto_ptr<NGraphTriple>, boost::noncopyable>
            ("NGraphTriple", no_init)
        .def("__init__", make_constructor(createNGraphTriple))
        .def("end", &NGraphTriple::end,
            return_internal_reference<>())
        .def("centre", &NGraphTriple::centre,
            return_internal_reference<>())
        .def("matchingReln", &NGraphTriple::matchingReln,
            return_internal_reference<>())
        .def(self < self)
    ;

    implicitly_convertible<std::auto_ptr<NGraphTriple>,
        std::auto_ptr<regina::NManifold> >();
}

