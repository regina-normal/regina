
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "../pybind11/pybind11.h"
#include "../pybind11/operators.h"
#include "maths/matrix2.h"
#include "../helpers.h"

using pybind11::overload_cast;

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
                if (whichCol < 0 || whichCol > 1)
                    throw pybind11::index_error(
                        "Matrix2 column index out of range");
                return row[whichCol];
            }

            void setItem(int whichCol, long value) {
                if (whichCol < 0 || whichCol > 1)
                    throw pybind11::index_error(
                        "Matrix2 column index out of range");
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

using regina::Matrix2;
using regina::Matrix2Row;

void addMatrix2(pybind11::module_& m) {
    auto c1 = pybind11::class_<Matrix2Row>(m, "Matrix2Row")
        .def("__getitem__", &Matrix2Row::getItem)
        .def("__setitem__", &Matrix2Row::setItem)
        .def("__len__", [](const Matrix2Row&) {
            return 2;
        })
        ;
    regina::python::add_eq_operators(c1);

    auto c2 = pybind11::class_<Matrix2>(m, "Matrix2")
        .def(pybind11::init<>())
        .def(pybind11::init<const Matrix2&>())
        .def(pybind11::init<long, long, long, long>())
        .def("__getitem__", [](Matrix2& m, int row) {
            if (row < 0 || row > 1)
                throw pybind11::index_error(
                    "Matrix2 row index out of range");
            return new Matrix2Row(m, row);
        }, pybind11::keep_alive<0, 1>())
        .def("__len__", [](const Matrix2&) {
            return 2;
        })
        .def(pybind11::self * pybind11::self)
        .def(pybind11::self * long())
        .def(pybind11::self + pybind11::self)
        .def(pybind11::self - pybind11::self)
        .def(- pybind11::self)
        .def("transpose", &Matrix2::transpose)
        .def("inverse", &Matrix2::inverse)
        .def(pybind11::self += pybind11::self)
        .def(pybind11::self -= pybind11::self)
        .def(pybind11::self *= pybind11::self)
        .def(pybind11::self *= long())
        .def("negate", &Matrix2::negate)
        .def("invert", &Matrix2::invert)
        .def("determinant", &Matrix2::determinant)
        .def("isIdentity", &Matrix2::isIdentity)
        .def("isZero", &Matrix2::isZero)
    ;
    regina::python::add_eq_operators(c2);
    regina::python::add_output_ostream(c2);

    m.def("simpler", overload_cast<const Matrix2&, const Matrix2&>(
        &regina::simpler));
    m.def("simpler", overload_cast<const Matrix2&, const Matrix2&,
            const Matrix2&, const Matrix2&>(
        &regina::simpler));

    m.attr("NMatrix2Row") = m.attr("Matrix2Row");
    m.attr("NMatrix2") = m.attr("Matrix2");
}

