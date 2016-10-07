
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
#include "maths/matrix2.h"
#include "../helpers.h"

namespace regina {
    /**
     * A utility class that provides access to a single row of a 2-by-2
     * matrix.  This allows us to write m[i][j] in Python for a 2-by-2
     * matrix m.
     */
    class Matrix2Row {
        private:
            long* row;

        public:
            Matrix2Row(Matrix2& matrix, int whichRow) :
                    row(matrix[whichRow]) {
            }

            long getItem(int whichCol) {
                if (whichCol < 0 || whichCol > 1) {
                    PyErr_SetString(PyExc_IndexError,
                        "Matrix2 column index out of range");
                    ::boost::python::throw_error_already_set();
                }
                return row[whichCol];
            }

            void setItem(int whichCol, long value) {
                if (whichCol < 0 || whichCol > 1) {
                    PyErr_SetString(PyExc_IndexError,
                        "Matrix2 column index out of range");
                    ::boost::python::throw_error_already_set();
                }
                row[whichCol] = value;
            }

            bool operator == (const Matrix2Row& other) const {
                return (row[0] == other.row[0] && row[1] == other.row[1]);
            }

            bool operator != (const Matrix2Row& other) const {
                return (row[0] != other.row[0] || row[1] != other.row[1]);
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
    regina::Matrix2Row* Matrix2_getRow(regina::Matrix2& m, int whichRow) {
        if (whichRow < 0 || whichRow > 1) {
            PyErr_SetString(PyExc_IndexError,
                "Matrix2 row index out of range");
            ::boost::python::throw_error_already_set();
        }
        return new regina::Matrix2Row(m, whichRow);
    }
}

using namespace boost::python;
using regina::Matrix2;
using regina::Matrix2Row;

namespace {
    bool (*simpler1)(const Matrix2&, const Matrix2&) = &regina::simpler;
    bool (*simpler2)(const Matrix2&, const Matrix2&,
        const Matrix2&, const Matrix2&) = &regina::simpler;
}

void addMatrix2() {
    class_<Matrix2Row>("Matrix2Row", ::boost::python::no_init)
        .def("__getitem__", &Matrix2Row::getItem)
        .def("__setitem__", &Matrix2Row::setItem)
        .def("__len__", size2<Matrix2Row>)
        .def(regina::python::add_eq_operators())
        ;

    class_<Matrix2>("Matrix2")
        .def(init<const Matrix2&>())
        .def(init<long, long, long, long>())
        .def("__getitem__", Matrix2_getRow,
            return_value_policy<manage_new_object,
            with_custodian_and_ward_postcall<0, 1> >())
        .def("__len__", &size2<regina::Matrix2>)
        .def(self * self)
        .def(self * long())
        .def(self + self)
        .def(self - self)
        .def(- self)
        .def("transpose", &Matrix2::transpose)
        .def("inverse", &Matrix2::inverse)
        .def(self += self)
        .def(self -= self)
        .def(self *= self)
        .def(self *= long())
        .def("negate", &Matrix2::negate)
        .def("invert", &Matrix2::invert)
        .def("determinant", &Matrix2::determinant)
        .def("isIdentity", &Matrix2::isIdentity)
        .def("isZero", &Matrix2::isZero)
        .def(self_ns::str(self))
        .def(regina::python::add_eq_operators())
    ;

    def("simpler", simpler1);
    def("simpler", simpler2);

    scope().attr("NMatrix2Row") = scope().attr("Matrix2Row");
    scope().attr("NMatrix2") = scope().attr("Matrix2");
}

