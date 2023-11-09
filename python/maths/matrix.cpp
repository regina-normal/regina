
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "maths/matrix.h"
#include "maths/vector.h"
#include "../helpers.h"
#include "../docstrings/maths/matrix.h"

using pybind11::overload_cast;

namespace {
    struct MatrixBoolInfo {
        using Matrix = regina::Matrix<bool>;
        static constexpr bool ring = false;
        using Value = bool;
        using Ref = bool; // this class never passes bool args by reference
        static constexpr const char* castError =
            "Matrix element not convertible to a boolean";
    };

    struct MatrixIntInfo {
        using Matrix = regina::MatrixInt;
        static constexpr bool ring = true;
        using Value = regina::Integer;
        using Ref = const regina::Integer&;
        static constexpr const char* castError =
            "Matrix element not convertible to Integer";
    };
}

template <typename Info>
void addMatrixInfo(pybind11::module_& m, const char* className) {
    using Matrix = typename Info::Matrix;
    using Value = typename Info::Value;
    using Ref = typename Info::Ref;

    RDOC_SCOPE_BEGIN(Matrix)

    auto c = pybind11::class_<Matrix>(m, className, rdoc_scope)
        .def(pybind11::init<size_t>(), rdoc::__init)
        .def(pybind11::init<size_t, size_t>(), rdoc::__init_2)
        .def(pybind11::init<const Matrix&>(), rdoc::__copy)
        .def(pybind11::init([](pybind11::list l) {
            size_t rows = l.size();
            if (rows == 0)
                throw regina::InvalidArgument(
                    "The number of rows must be strictly positive");

            Matrix* m = nullptr;
            size_t cols = 0; // zero is unnecessary but silences warnings

            pybind11::list row;
            for (size_t i = 0; i < rows; ++i) {
                try {
                    row = l[i].cast<pybind11::list>();
                } catch (const pybind11::cast_error&) {
                    delete m;
                    throw regina::InvalidArgument(
                        "Each row must be given as a separate Python list");
                }
                if (i == 0) {
                    cols = row.size();
                    if (cols == 0)
                        throw regina::InvalidArgument(
                            "The number of columns must be strictly positive");
                    m = new Matrix(rows, cols);
                } else if (row.size() != cols) {
                    delete m;
                    throw regina::InvalidArgument(
                        "All rows must be given as lists of the same size");
                }
                for (size_t j = 0; j < cols; ++j) {
                    try {
                        m->entry(i, j) = row[j].cast<Value>();
                    } catch (const pybind11::cast_error&) {
                        delete m;
                        throw regina::InvalidArgument(Info::castError);
                    }
                }
            }

            return m;
        }), rdoc::__init_3)
        .def("fill", &Matrix::fill, rdoc::fill)
        .def("initialise", &Matrix::fill, rdoc::initialise) // deprecated
        .def("swap", &Matrix::swap, rdoc::swap)
        .def("rows", &Matrix::rows, rdoc::rows)
        .def("columns", &Matrix::columns, rdoc::columns)
        .def("initialised", &Matrix::initialised, rdoc::initialised)
        .def("set", [](Matrix& m, size_t row, size_t col, Ref value) {
            m.entry(row, col) = value;
        }, rdoc::set)
        // Give the read-only entry() the same docstring as the read-write
        // set(), so users know they are allowed to call set().
        .def("entry", overload_cast<size_t, size_t>(&Matrix::entry),
            pybind11::return_value_policy::reference_internal, rdoc::entry)
        .def("transpose", &Matrix::transpose, rdoc::transpose)
        .def("swapRows", &Matrix::swapRows, rdoc::swapRows)
        .def("swapCols", &Matrix::swapCols,
            pybind11::arg(), pybind11::arg(), pybind11::arg("fromRow") = 0,
            rdoc::swapCols)
    ;
    if constexpr (Info::ring) {
        c
            .def("isIdentity", &Matrix::isIdentity, rdoc::isIdentity)
            .def("isZero", &Matrix::isZero, rdoc::isZero)
            .def_static("identity", &Matrix::identity, rdoc::identity)
            .def("makeIdentity", &Matrix::makeIdentity, rdoc::makeIdentity)
            .def("addRow", overload_cast<size_t, size_t>(&Matrix::addRow),
                rdoc::addRow)
            .def("addRowFrom", &Matrix::addRowFrom, rdoc::addRowFrom)
            .def("addRow",
                overload_cast<size_t, size_t, Value, size_t>(&Matrix::addRow),
                pybind11::arg(), pybind11::arg(), pybind11::arg(),
                    pybind11::arg("fromCol") = 0, rdoc::addRow_2)
            .def("addCol", overload_cast<size_t, size_t>(&Matrix::addCol),
                rdoc::addCol)
            .def("addColFrom", &Matrix::addColFrom, rdoc::addColFrom)
            .def("addCol",
                overload_cast<size_t, size_t, Value, size_t>(&Matrix::addCol),
                pybind11::arg(), pybind11::arg(), pybind11::arg(),
                    pybind11::arg("fromRow") = 0, rdoc::addCol_2)
            .def("multRow", &Matrix::multRow,
                pybind11::arg(), pybind11::arg(), pybind11::arg("fromCol") = 0,
                rdoc::multRow)
            .def("multCol", &Matrix::multCol,
                pybind11::arg(), pybind11::arg(), pybind11::arg("fromRow") = 0,
                rdoc::multCol)
            .def("combRows", &Matrix::combRows,
                pybind11::arg(), pybind11::arg(), pybind11::arg(),
                    pybind11::arg(), pybind11::arg(), pybind11::arg(),
                    pybind11::arg("fromCol") = 0,
                rdoc::combRows)
            .def("combCols", &Matrix::combCols,
                pybind11::arg(), pybind11::arg(), pybind11::arg(),
                    pybind11::arg(), pybind11::arg(), pybind11::arg(),
                    pybind11::arg("fromRow") = 0,
                rdoc::combCols)
            .def("det", &Matrix::det, rdoc::det)
            .def("negateRow", &Matrix::negateRow, rdoc::negateRow)
            .def("negateCol", &Matrix::negateCol, rdoc::negateCol)
            .def("divRowExact", &Matrix::divRowExact, rdoc::divRowExact)
            .def("divColExact", &Matrix::divColExact, rdoc::divColExact)
            .def("gcdRow", &Matrix::gcdRow, rdoc::gcdRow)
            .def("gcdCol", &Matrix::gcdCol, rdoc::gcdCol)
            .def("reduceRow", &Matrix::reduceRow, rdoc::reduceRow)
            .def("reduceCol", &Matrix::reduceCol, rdoc::reduceCol)
            .def("rowEchelonForm", &Matrix::rowEchelonForm,
                rdoc::rowEchelonForm)
            .def("columnEchelonForm", &Matrix::columnEchelonForm,
                rdoc::columnEchelonForm)
            .def("rank", static_cast<size_t (Matrix::*)() const&>(
                &Matrix::rank), rdoc::rank)
            .def("__mul__", [](const Matrix& m1, const Matrix& m2){
                return m1 * m2;
            }, rdoc::__mul)
            .def("__mul__", [](const Matrix& m, const regina::VectorInt& v){
                return m * v;
            }, rdoc::__mul_2)
            .def("__mul__", [](const Matrix& m, const regina::VectorLarge& v){
                return m * v;
            }, rdoc::__mul_2)
        ;
    }
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<Matrix>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

void addMatrix(pybind11::module_& m) {
    addMatrixInfo<MatrixBoolInfo>(m, "MatrixBool");
    addMatrixInfo<MatrixIntInfo>(m, "MatrixInt");
}

