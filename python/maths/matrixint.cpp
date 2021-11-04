
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
#include "maths/matrix.h"
#include "maths/vector.h"
#include "utilities/exception.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::MatrixInt;

void addMatrixInt(pybind11::module_& m) {
    auto c = pybind11::class_<MatrixInt>(m, "MatrixInt")
        .def(pybind11::init<unsigned long>())
        .def(pybind11::init<unsigned long, unsigned long>())
        .def(pybind11::init<const MatrixInt&>())
        .def(pybind11::init([](pybind11::list l) {
            size_t rows = l.size();
            if (rows == 0)
                throw regina::InvalidArgument(
                    "The number of rows must be strictly positive");

            MatrixInt* m = nullptr;
            size_t cols;

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
        .def("initialise", [](MatrixInt& matrix, pybind11::list values) {
            // Note: this routine is deprecated.
            if (values.size() != matrix.rows() * matrix.columns())
                throw regina::InvalidArgument(
                    "Initialisation list has the wrong length");
            unsigned long r, c;
            unsigned i = 0;
            for (r = 0; r < matrix.rows(); ++r)
                for (c = 0; c < matrix.columns(); ++c) {
                    // Accept any type that we know how to convert to a large
                    // integer.  This includes (at least) regina::Integer,
                    // python integers (both int and long), and strings.
                    try {
                        matrix.entry(r, c) = values[i].cast<regina::Integer>();
                        ++i;
                        continue;
                    } catch (pybind11::cast_error const &) {
                        throw regina::InvalidArgument(
                            "List element not convertible to Integer");
                    }
                }
        })
        .def("rows", &MatrixInt::rows)
        .def("columns", &MatrixInt::columns)
        .def("entry",
            overload_cast<unsigned long, unsigned long>(&MatrixInt::entry),
            pybind11::return_value_policy::reference_internal)
        .def("set", [](MatrixInt& m, unsigned long row, unsigned long col,
                const regina::Integer& value){
            m.entry(row, col) = value;
        })
        .def("transpose", &MatrixInt::transpose)
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
        .def("swapRows", &MatrixInt::swapRows)
        .def("swapCols", &MatrixInt::swapCols,
            pybind11::arg(), pybind11::arg(), pybind11::arg("fromRow") = 0)
        .def("swapColumns", &MatrixInt::swapCols) // deprecated
        .def_static("identity",
            (MatrixInt (*)(unsigned long))
            &MatrixInt::identity<>)
        .def("makeIdentity",
            (void (MatrixInt::*)())
            &MatrixInt::makeIdentity<>)
        .def("addRow",
            (void (MatrixInt::*)(unsigned long, unsigned long))
            &MatrixInt::addRow<>)
        .def("addRowFrom",
            (void (MatrixInt::*)(unsigned long, unsigned long, unsigned long))
            &MatrixInt::addRowFrom<>)
        .def("addRow",
            (void (MatrixInt::*)(unsigned long, unsigned long,
                regina::Integer, unsigned long))
            &MatrixInt::addRow<>,
            pybind11::arg(), pybind11::arg(), pybind11::arg(),
                pybind11::arg("fromCol") = 0)
        .def("addCol",
            (void (MatrixInt::*)(unsigned long, unsigned long))
            &MatrixInt::addCol<>)
        .def("addColFrom",
            (void (MatrixInt::*)(unsigned long, unsigned long, unsigned long))
            &MatrixInt::addColFrom<>)
        .def("addCol",
            (void (MatrixInt::*)(unsigned long, unsigned long,
                regina::Integer, unsigned long))
            &MatrixInt::addCol<>,
            pybind11::arg(), pybind11::arg(), pybind11::arg(),
                pybind11::arg("fromRow") = 0)
        .def("multRow",
            (void (MatrixInt::*)(unsigned long, regina::Integer, unsigned long))
            &MatrixInt::multRow<>,
            pybind11::arg(), pybind11::arg(), pybind11::arg("fromCol") = 0)
        .def("multCol",
            (void (MatrixInt::*)(unsigned long, regina::Integer, unsigned long))
            &MatrixInt::multCol<>,
            pybind11::arg(), pybind11::arg(), pybind11::arg("fromRow") = 0)
        .def("combRows",
            (void (MatrixInt::*)(unsigned long, unsigned long,
                regina::Integer, regina::Integer,
                regina::Integer, regina::Integer, unsigned long))
            &MatrixInt::combRows<>,
            pybind11::arg(), pybind11::arg(), pybind11::arg(), pybind11::arg(),
                pybind11::arg(), pybind11::arg(), pybind11::arg("fromCol") = 0)
        .def("combCols",
            (void (MatrixInt::*)(unsigned long, unsigned long,
                regina::Integer, regina::Integer,
                regina::Integer, regina::Integer, unsigned long))
            &MatrixInt::combCols<>,
            pybind11::arg(), pybind11::arg(), pybind11::arg(), pybind11::arg(),
                pybind11::arg(), pybind11::arg(), pybind11::arg("fromRow") = 0)
        .def("det",
            (regina::Integer (MatrixInt::*)() const)
            &MatrixInt::det<>)
        .def("divRowExact",
            (void (MatrixInt::*)(unsigned long, const regina::Integer&))
            &MatrixInt::divRowExact<>)
        .def("divColExact",
            (void (MatrixInt::*)(unsigned long, const regina::Integer&))
            &MatrixInt::divColExact<>)
        .def("gcdRow",
            (regina::Integer (MatrixInt::*)(unsigned long))
            &MatrixInt::gcdRow<>)
        .def("gcdCol",
            (regina::Integer (MatrixInt::*)(unsigned long))
            &MatrixInt::gcdCol<>)
        .def("reduceRow",
            (void (MatrixInt::*)(unsigned long))
            &MatrixInt::reduceRow<>)
        .def("reduceCol",
            (void (MatrixInt::*)(unsigned long))
            &MatrixInt::reduceCol<>)
        .def("rowEchelonForm",
            (unsigned long (MatrixInt::*)())
            &MatrixInt::rowEchelonForm)
        .def("columnEchelonForm",
            (unsigned long (MatrixInt::*)())
            &MatrixInt::columnEchelonForm)
        .def("__mul__", [](const MatrixInt& m1, const MatrixInt& m2){
            return m1 * m2;
        })
        .def("__mul__", [](const MatrixInt& m, const regina::VectorInt& v){
            return m * v;
        })
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.def("swap", (void(*)(MatrixInt&, MatrixInt&))(regina::swap));
}

