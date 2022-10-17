
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "../docstrings/maths/matrix2.h"

using pybind11::overload_cast;

namespace regina {
    /**
     * A utility class that provides access to a single row of a 2-by-2
     * matrix.  This allows us to write m[i][j] in Python for a 2-by-2
     * matrix m.
     */
    class Matrix2Row {
        private:
            std::array<long, 2>& row;

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

        friend std::ostream& operator << (std::ostream&, const Matrix2Row&);
    };

    inline std::ostream& operator << (std::ostream& out, const Matrix2Row& r) {
        return out << "[ " << r.row[0] << ' ' << r.row[1] << " ]";
    }
}

using regina::Matrix2;
using regina::Matrix2Row;

void addMatrix2(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Matrix2)

    auto c = pybind11::class_<Matrix2>(m, "Matrix2", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__init)
        .def(pybind11::init<const Matrix2&>(), rdoc::__copy)
        .def(pybind11::init<long, long, long, long>(), rdoc::__init_2)
        .def("swap", &Matrix2::swap, rdoc::swap)
        .def("__getitem__", [](Matrix2& m, int row) {
            if (row < 0 || row > 1)
                throw pybind11::index_error(
                    "Matrix2 row index out of range");
            return new Matrix2Row(m, row);
        }, pybind11::keep_alive<0, 1>(), rdoc::__array_2)
        .def("__len__", [](const Matrix2&) {
            return 2;
        }, "Returns the number of rows in this matrix. This will always be 2.")
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def(pybind11::self * long(), rdoc::__mul_2)
        .def(pybind11::self + pybind11::self, rdoc::__add)
        .def(pybind11::self - pybind11::self, rdoc::__sub)
        .def(- pybind11::self, rdoc::__sub_2)
        .def("transpose", &Matrix2::transpose, rdoc::transpose)
        .def("inverse", &Matrix2::inverse, rdoc::inverse)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self *= pybind11::self, rdoc::__imul)
        .def(pybind11::self *= long(), rdoc::__imul_2)
        .def("negate", &Matrix2::negate, rdoc::negate)
        .def("invert", &Matrix2::invert, rdoc::invert)
        .def("determinant", &Matrix2::determinant, rdoc::determinant)
        .def("isIdentity", &Matrix2::isIdentity, rdoc::isIdentity)
        .def("isZero", &Matrix2::isZero, rdoc::isZero)
    ;
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);
    regina::python::add_output_ostream(c);

    regina::python::add_global_swap<Matrix2>(m, rdoc::global_swap);

    auto row = pybind11::class_<Matrix2Row>(c, "_Row",
R"doc(Gives access to a single row of a 2-by-2 integer matrix.

See the main class Matrix2 for further details.)doc")
        .def("__getitem__", &Matrix2Row::getItem,
R"doc(Returns the integer at the given index in this row.

The given index must be either 0 or 1.

You should not need to call this directly.  To access the (*i*, *j*)
entry of a 2-by-2 matrix *M*, you can call ``M[i][j]``.

See the main class Matrix2 for further details.)doc")
        .def("__setitem__", &Matrix2Row::setItem,
R"doc(Sets the integer at the given index in this row to the given value.

The given index must be either 0 or 1.

You should not need to call this directly.  To set the (*i*, *j*)
entry of a 2-by-2 matrix *M*, you can use ``M[i][j] = value``.

See the main class Matrix2 for further details.)doc")
        .def("__len__", [](const Matrix2Row&) {
            return 2;
        },
"Returns the number of entries in this row. This will always be 2.")
        ;
    regina::python::add_eq_operators(row,
"Tests whether this and the given row contain the same integer entries.",
"Tests whether this and the given row contain different integer entries.");
    regina::python::add_output_ostream(row);

    RDOC_SCOPE_SWITCH_MAIN

    m.def("simpler", overload_cast<const Matrix2&, const Matrix2&>(
        &regina::simpler), rdoc::simpler);
    m.def("simpler", overload_cast<const Matrix2&, const Matrix2&,
            const Matrix2&, const Matrix2&>(
        &regina::simpler), rdoc::simpler_2);

    RDOC_SCOPE_END
}

