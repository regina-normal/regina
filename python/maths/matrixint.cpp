
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
using regina::MatrixInt;

void addMatrixInt(pybind11::module_& m) {
    auto c = pybind11::class_<MatrixInt>(m, "MatrixInt")
        .def(pybind11::init<size_t>())
        .def(pybind11::init<size_t, size_t>())
        .def(pybind11::init<const MatrixInt&>())
        .def(pybind11::init([](pybind11::list l) {
            size_t rows = l.size();
            if (rows == 0)
                throw regina::InvalidArgument(
                    "The number of rows must be strictly positive");

            MatrixInt* m = nullptr;
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
                    m = new MatrixInt(rows, cols);
                } else if (row.size() != cols) {
                    delete m;
                    throw regina::InvalidArgument(
                        "All rows must be given as lists of the same size");
                }
                for (size_t j = 0; j < cols; ++j) {
                    try {
                        m->entry(i, j) = row[j].cast<regina::Integer>();
                    } catch (const pybind11::cast_error&) {
                        delete m;
                        throw regina::InvalidArgument(
                            "Matrix element not convertible to Integer");
                    }
                }
            }

            return m;
        }))
        .def("initialise", &MatrixInt::initialise)
        .def("rows", &MatrixInt::rows)
        .def("columns", &MatrixInt::columns)
        .def("entry", overload_cast<size_t, size_t>(&MatrixInt::entry),
            pybind11::return_value_policy::reference_internal)
        .def("set", [](MatrixInt& m, size_t row, size_t col,
                const regina::Integer& value){
            m.entry(row, col) = value;
        })
        .def("transpose", &MatrixInt::transpose)
        .def("swapRows", &MatrixInt::swapRows)
        .def("swapCols", &MatrixInt::swapCols,
            pybind11::arg(), pybind11::arg(), pybind11::arg("fromRow") = 0)
        // The C-style casts below are to avoid a compile error under gcc7
        // (but not gcc8), where the compiler cannot determine the type of a
        // template member function.
        .def("isIdentity",
            (bool (MatrixInt::*)() const)
            &MatrixInt::isIdentity<>)
        .def("isZero",
            (bool (MatrixInt::*)() const)
            &MatrixInt::isZero<>)
        .def("swap", &MatrixInt::swap)
        .def_static("identity",
            (MatrixInt (*)(size_t))
            &MatrixInt::identity<>)
        .def("makeIdentity",
            (void (MatrixInt::*)())
            &MatrixInt::makeIdentity<>)
        .def("addRow",
            (void (MatrixInt::*)(size_t, size_t))
            &MatrixInt::addRow<>)
        .def("addRowFrom",
            (void (MatrixInt::*)(size_t, size_t, size_t))
            &MatrixInt::addRowFrom<>)
        .def("addRow",
            (void (MatrixInt::*)(size_t, size_t, regina::Integer, size_t))
            &MatrixInt::addRow<>,
            pybind11::arg(), pybind11::arg(), pybind11::arg(),
                pybind11::arg("fromCol") = 0)
        .def("addCol",
            (void (MatrixInt::*)(size_t, size_t))
            &MatrixInt::addCol<>)
        .def("addColFrom",
            (void (MatrixInt::*)(size_t, size_t, size_t))
            &MatrixInt::addColFrom<>)
        .def("addCol",
            (void (MatrixInt::*)(size_t, size_t, regina::Integer, size_t))
            &MatrixInt::addCol<>,
            pybind11::arg(), pybind11::arg(), pybind11::arg(),
                pybind11::arg("fromRow") = 0)
        .def("multRow",
            (void (MatrixInt::*)(size_t, regina::Integer, size_t))
            &MatrixInt::multRow<>,
            pybind11::arg(), pybind11::arg(), pybind11::arg("fromCol") = 0)
        .def("multCol",
            (void (MatrixInt::*)(size_t, regina::Integer, size_t))
            &MatrixInt::multCol<>,
            pybind11::arg(), pybind11::arg(), pybind11::arg("fromRow") = 0)
        .def("combRows",
            (void (MatrixInt::*)(size_t, size_t,
                regina::Integer, regina::Integer,
                regina::Integer, regina::Integer, size_t))
            &MatrixInt::combRows<>,
            pybind11::arg(), pybind11::arg(), pybind11::arg(), pybind11::arg(),
                pybind11::arg(), pybind11::arg(), pybind11::arg("fromCol") = 0)
        .def("combCols",
            (void (MatrixInt::*)(size_t, size_t,
                regina::Integer, regina::Integer,
                regina::Integer, regina::Integer, size_t))
            &MatrixInt::combCols<>,
            pybind11::arg(), pybind11::arg(), pybind11::arg(), pybind11::arg(),
                pybind11::arg(), pybind11::arg(), pybind11::arg("fromRow") = 0)
        .def("det",
            (regina::Integer (MatrixInt::*)() const)
            &MatrixInt::det<>)
        .def("divRowExact",
            (void (MatrixInt::*)(size_t, const regina::Integer&))
            &MatrixInt::divRowExact<>)
        .def("divColExact",
            (void (MatrixInt::*)(size_t, const regina::Integer&))
            &MatrixInt::divColExact<>)
        .def("gcdRow",
            (regina::Integer (MatrixInt::*)(size_t))
            &MatrixInt::gcdRow<>)
        .def("gcdCol",
            (regina::Integer (MatrixInt::*)(size_t))
            &MatrixInt::gcdCol<>)
        .def("reduceRow",
            (void (MatrixInt::*)(size_t))
            &MatrixInt::reduceRow<>)
        .def("reduceCol",
            (void (MatrixInt::*)(size_t))
            &MatrixInt::reduceCol<>)
        .def("rowEchelonForm",
            (size_t (MatrixInt::*)())
            &MatrixInt::rowEchelonForm)
        .def("columnEchelonForm",
            (size_t (MatrixInt::*)())
            &MatrixInt::columnEchelonForm)
        .def("__mul__", [](const MatrixInt& m1, const MatrixInt& m2){
            return m1 * m2;
        })
        .def("__mul__", [](const MatrixInt& m, const regina::VectorInt& v){
            return m * v;
        })
        .def("__mul__", [](const MatrixInt& m, const regina::VectorLarge& v){
            return m * v;
        })
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    regina::python::add_global_swap<MatrixInt>(m);
}

