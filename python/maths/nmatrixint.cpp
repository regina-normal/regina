
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "maths/nmatrixint.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NMatrixInt;

namespace {
    regina::NLargeInteger& (NMatrixInt::*entry_non_const)(unsigned long,
        unsigned long) = &NMatrixInt::entry;
    void (NMatrixInt::*addRow_triple)(unsigned long, unsigned long,
        regina::NLargeInteger) = &NMatrixInt::addRow;
    void (NMatrixInt::*addCol_triple)(unsigned long, unsigned long,
        regina::NLargeInteger) = &NMatrixInt::addCol;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addRow,
        NMatrixInt::addRow, 2, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addCol,
        NMatrixInt::addCol, 2, 3);
}

void addNMatrixInt() {
    scope s = class_<NMatrixInt, bases<regina::ShareableObject>,
            std::auto_ptr<NMatrixInt>, boost::noncopyable>("NMatrixInt",
            init<unsigned long, unsigned long>())
        .def(init<const NMatrixInt&>())
        .def("initialise", &NMatrixInt::initialise)
        .def("rows", &NMatrixInt::rows)
        .def("columns", &NMatrixInt::columns)
        .def("entry", entry_non_const, return_internal_reference<>())
        .def("swapRows", &NMatrixInt::swapRows)
        .def("swapColumns", &NMatrixInt::swapColumns)
        .def("makeIdentity", &NMatrixInt::makeIdentity)
        .def("addRow", addRow_triple, OL_addRow())
        .def("addCol", addCol_triple, OL_addCol())
        .def("multRow", &NMatrixInt::multRow)
        .def("multCol", &NMatrixInt::multCol)
    ;

    s.attr("zero") = NMatrixInt::zero;
    s.attr("one") = NMatrixInt::one;
}

