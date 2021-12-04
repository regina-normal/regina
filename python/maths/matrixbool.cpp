
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
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Matrix;

void addMatrixBool(pybind11::module_& m) {
    auto c = pybind11::class_<Matrix<bool>>(m, "MatrixBool")
        .def(pybind11::init<unsigned long>())
        .def(pybind11::init<unsigned long, unsigned long>())
        .def(pybind11::init<const Matrix<bool>&>())
        .def(pybind11::init([](pybind11::list l) {
            size_t rows = l.size();
            if (rows == 0)
                throw regina::InvalidArgument(
                    "The number of rows must be strictly positive");

            Matrix<bool>* m = nullptr;
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
                    m = new Matrix<bool>(rows, cols);
                } else if (row.size() != cols) {
                    delete m;
                    throw regina::InvalidArgument(
                        "All rows must be given as lists of the same size");
                }
                for (size_t j = 0; j < cols; ++j) {
                    try {
                        m->entry(i, j) = row[j].cast<bool>();
                    } catch (const pybind11::cast_error&) {
                        delete m;
                        throw regina::InvalidArgument(
                            "Matrix element not convertible to a boolean");
                    }
                }
            }

            return m;
        }))
        .def("initialise", &Matrix<bool>::initialise)
        .def("initialise", [](Matrix<bool>& matrix, pybind11::list values) {
            // Note: this routine is deprecated.
            if (values.size() != matrix.rows() * matrix.columns())
                throw regina::InvalidArgument(
                    "Initialisation list has the wrong length");
            unsigned long r, c;
            unsigned i = 0;
            for (r = 0; r < matrix.rows(); ++r)
                for (c = 0; c < matrix.columns(); ++c) {
                    // Accept any type that we know how to convert to a bool.
                    try {
                        matrix.entry(r, c) = values[i].cast<bool>();
                        ++i;
                        continue;
                    } catch (pybind11::cast_error const &) {
                        throw regina::InvalidArgument(
                            "List element not convertible to boolean");
                    }
                }
        })
        .def("swap", &Matrix<bool>::swap)
        .def("rows", &Matrix<bool>::rows)
        .def("columns", &Matrix<bool>::columns)
        .def("entry",
            overload_cast<unsigned long, unsigned long>(&Matrix<bool>::entry),
            pybind11::return_value_policy::reference_internal)
        .def("set", [](Matrix<bool>& m, unsigned long row, unsigned long col,
                bool value){
            m.entry(row, col) = value;
        })
        .def("transpose", &Matrix<bool>::transpose)
        .def("swapRows", &Matrix<bool>::swapRows)
        .def("swapCols", &Matrix<bool>::swapCols)
        .def("swapColumns", &Matrix<bool>::swapCols) // deprecated
    ;
    regina::python::add_output(c, regina::python::PYTHON_REPR_NONE);
    regina::python::add_eq_operators(c);

    m.def("swap", (void(*)(Matrix<bool>&, Matrix<bool>&))(regina::swap));
}

