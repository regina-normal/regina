
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

#include "utilities/nmatrix2.h"
#include <boost/python.hpp>

namespace regina {
    /**
     * A utility class that provides access to a single row of a 2-by-2
     * matrix.  This allows us to write m[i][j] in Python for a 2-by-2
     * matrix m.
     */
    class NMatrix2Row {
        private:
            long* row;

        public:
            NMatrix2Row(NMatrix2& matrix, int whichRow) :
                    row(matrix[whichRow]) {
            }

            long getItem(int whichCol) {
                if (whichCol < 0 || whichCol > 1) {
                    PyErr_SetString(PyExc_IndexError,
                        "NMatrix2 column index out of range");
                    ::boost::python::throw_error_already_set();
                }
                return row[whichCol];
            }

            void setItem(int whichCol, long value) {
                if (whichCol < 0 || whichCol > 1) {
                    PyErr_SetString(PyExc_IndexError,
                        "NMatrix2 column index out of range");
                    ::boost::python::throw_error_already_set();
                }
                row[whichCol] = value;
            }
    };
}

namespace {
    /**
     * A trivial routine for returning the size of a 2-by-2 matrix.
     */
    template <class T>
    long size2(const T&) {
        return 2;
    }

    /**
     * Return a new object for accessing the given row of the given matrix.
     */
    regina::NMatrix2Row* NMatrix2_getRow(regina::NMatrix2& m, int whichRow) {
        if (whichRow < 0 || whichRow > 1) {
            PyErr_SetString(PyExc_IndexError,
                "NMatrix2 row index out of range");
            ::boost::python::throw_error_already_set();
        }
        return new regina::NMatrix2Row(m, whichRow);
    }
}

using namespace boost::python;
using regina::NMatrix2;
using regina::NMatrix2Row;

namespace {
    bool (*simpler1)(const NMatrix2&, const NMatrix2&) = &regina::simpler;
    bool (*simpler2)(const NMatrix2&, const NMatrix2&,
        const NMatrix2&, const NMatrix2&) = &regina::simpler;
}

void addNMatrix2() {
    class_<NMatrix2Row>("NMatrix2Row", ::boost::python::no_init)
        .def("__getitem__", &NMatrix2Row::getItem)
        .def("__setitem__", &NMatrix2Row::setItem)
        .def("__len__", size2<NMatrix2Row>)
        ;

    class_<NMatrix2>("NMatrix2")
        .def(init<const NMatrix2&>())
        .def(init<long, long, long, long>())
        .def("__getitem__", NMatrix2_getRow,
            return_value_policy<manage_new_object,
            with_custodian_and_ward_postcall<0, 1> >())
        .def("__len__", &size2<regina::NMatrix2>)
        .def(self * self)
        .def(self * long())
        .def(self + self)
        .def(self - self)
        .def(- self)
        .def("transpose", &NMatrix2::transpose)
        .def("inverse", &NMatrix2::inverse)
        .def(self += self)
        .def(self -= self)
        .def(self *= self)
        .def(self *= long())
        .def("negate", &NMatrix2::negate)
        .def("invert", &NMatrix2::invert)
        .def(self == self)
        .def(self != self)
        .def("determinant", &NMatrix2::determinant)
        .def("isIdentity", &NMatrix2::isIdentity)
        .def("isZero", &NMatrix2::isZero)
        .def(self_ns::str(self))
    ;

    def("simpler", simpler1);
    def("simpler", simpler2);
}

