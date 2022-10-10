
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
#include "maths/matrix.h"
#include "maths/vector.h"
#include "../helpers.h"

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

    auto c = pybind11::class_<Matrix>(m, className)
        .def(pybind11::init<size_t>())
        .def(pybind11::init<size_t, size_t>())
        .def(pybind11::init<const Matrix&>())
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
        }))
        .def("initialise", &Matrix::initialise)
        .def("swap", &Matrix::swap)
        .def("rows", &Matrix::rows)
        .def("columns", &Matrix::columns)
        .def("entry", overload_cast<size_t, size_t>(&Matrix::entry),
            pybind11::return_value_policy::reference_internal)
        .def("set", [](Matrix& m, size_t row, size_t col,
                Ref value) {
            m.entry(row, col) = value;
        })
        .def("transpose", &Matrix::transpose)
        .def("swapRows", &Matrix::swapRows)
        .def("swapCols", &Matrix::swapCols,
            pybind11::arg(), pybind11::arg(), pybind11::arg("fromRow") = 0)
    ;
    if constexpr (Info::ring) {
        // The C-style casts below are to avoid a compile error under gcc7
        // (but not gcc8), where the compiler cannot determine the type of a
        // template member function.
        c
            .def("isIdentity",
                (bool (Matrix::*)() const)
                &Matrix::template isIdentity<>)
            .def("isZero",
                (bool (Matrix::*)() const)
                &Matrix::template isZero<>)
            .def("swap", &Matrix::swap)
            .def_static("identity",
                (Matrix (*)(size_t))
                &Matrix::template identity<>)
            .def("makeIdentity",
                (void (Matrix::*)())
                &Matrix::template makeIdentity<>)
            .def("addRow",
                (void (Matrix::*)(size_t, size_t))
                &Matrix::template addRow<>)
            .def("addRowFrom",
                (void (Matrix::*)(size_t, size_t, size_t))
                &Matrix::template addRowFrom<>)
            .def("addRow",
                (void (Matrix::*)(size_t, size_t, Value, size_t))
                &Matrix::template addRow<>,
                pybind11::arg(), pybind11::arg(), pybind11::arg(),
                    pybind11::arg("fromCol") = 0)
            .def("addCol",
                (void (Matrix::*)(size_t, size_t))
                &Matrix::template addCol<>)
            .def("addColFrom",
                (void (Matrix::*)(size_t, size_t, size_t))
                &Matrix::template addColFrom<>)
            .def("addCol",
                (void (Matrix::*)(size_t, size_t, Value, size_t))
                &Matrix::template addCol<>,
                pybind11::arg(), pybind11::arg(), pybind11::arg(),
                    pybind11::arg("fromRow") = 0)
            .def("multRow",
                (void (Matrix::*)(size_t, Value, size_t))
                &Matrix::template multRow<>,
                pybind11::arg(), pybind11::arg(), pybind11::arg("fromCol") = 0)
            .def("multCol",
                (void (Matrix::*)(size_t, Value, size_t))
                &Matrix::template multCol<>,
                pybind11::arg(), pybind11::arg(), pybind11::arg("fromRow") = 0)
            .def("combRows",
                (void (Matrix::*)(size_t, size_t,
                    Value, Value,
                    Value, Value, size_t))
                &Matrix::template combRows<>,
                pybind11::arg(), pybind11::arg(), pybind11::arg(),
                    pybind11::arg(), pybind11::arg(), pybind11::arg(),
                    pybind11::arg("fromCol") = 0)
            .def("combCols",
                (void (Matrix::*)(size_t, size_t,
                    Value, Value,
                    Value, Value, size_t))
                &Matrix::template combCols<>,
                pybind11::arg(), pybind11::arg(), pybind11::arg(),
                    pybind11::arg(), pybind11::arg(), pybind11::arg(),
                    pybind11::arg("fromRow") = 0)
            .def("det",
                (Value (Matrix::*)() const)
                &Matrix::template det<>)
            .def("divRowExact",
                (void (Matrix::*)(size_t, Ref&))
                &Matrix::template divRowExact<>)
            .def("divColExact",
                (void (Matrix::*)(size_t, Ref&))
                &Matrix::template divColExact<>)
            .def("gcdRow",
                (Value (Matrix::*)(size_t))
                &Matrix::template gcdRow<>)
            .def("gcdCol",
                (Value (Matrix::*)(size_t))
                &Matrix::template gcdCol<>)
            .def("reduceRow",
                (void (Matrix::*)(size_t))
                &Matrix::template reduceRow<>)
            .def("reduceCol",
                (void (Matrix::*)(size_t))
                &Matrix::template reduceCol<>)
            .def("rowEchelonForm",
                (size_t (Matrix::*)())
                &Matrix::rowEchelonForm)
            .def("columnEchelonForm",
                (size_t (Matrix::*)())
                &Matrix::columnEchelonForm)
            .def("__mul__", [](const Matrix& m1, const Matrix& m2){
                return m1 * m2;
            })
            .def("__mul__", [](const Matrix& m,
                    const regina::VectorInt& v){
                return m * v;
            })
            .def("__mul__", [](const Matrix& m,
                    const regina::VectorLarge& v){
                return m * v;
            })
        ;
    }
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    regina::python::add_global_swap<Matrix>(m);
}

void addMatrix(pybind11::module_& m) {
    addMatrixInfo<MatrixBoolInfo>(m, "MatrixBool");
    addMatrixInfo<MatrixIntInfo>(m, "MatrixInt");
}

