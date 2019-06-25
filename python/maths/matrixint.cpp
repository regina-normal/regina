
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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
#include "../helpers.h"

using regina::MatrixInt;

void addMatrixInt(pybind11::module& m) {
    auto c = pybind11::class_<MatrixInt>(m, "MatrixInt")
        .def(pybind11::init<unsigned long, unsigned long>())
        .def(pybind11::init<const MatrixInt&>())
        .def("initialise", &MatrixInt::initialise)
        .def("initialise", [](MatrixInt& matrix, pybind11::list values) {
            if (values.size() != matrix.rows() * matrix.columns())
                throw pybind11::index_error(
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
                        throw std::invalid_argument(
                            "List element not convertible to Integer");
                    }
                }
        })
        .def("rows", &MatrixInt::rows)
        .def("columns", &MatrixInt::columns)
        .def("entry",
            (regina::Integer& (MatrixInt::*)(unsigned long, unsigned long))
            &MatrixInt::entry,
            pybind11::return_value_policy::reference_internal)
        .def("set", [](MatrixInt& m, unsigned long row, unsigned long col,
                const regina::Integer& value){
            m.entry(row, col) = value;
        })
        .def("isIdentity", &MatrixInt::isIdentity)
        .def("isZero", &MatrixInt::isZero)
        .def("swapRows", &MatrixInt::swapRows)
        .def("swapColumns", &MatrixInt::swapColumns)
        .def("makeIdentity", &MatrixInt::makeIdentity)
        .def("addRow",
            (void (MatrixInt::*)(unsigned long, unsigned long))
            &MatrixInt::addRow)
        .def("addRow",
            (void (MatrixInt::*)(unsigned long, unsigned long, regina::Integer))
            &MatrixInt::addRow)
        .def("addCol",
            (void (MatrixInt::*)(unsigned long, unsigned long))
            &MatrixInt::addCol)
        .def("addCol",
            (void (MatrixInt::*)(unsigned long, unsigned long, regina::Integer))
            &MatrixInt::addCol)
        .def("multRow", &MatrixInt::multRow)
        .def("multCol", &MatrixInt::multCol)
        .def("det", &MatrixInt::det)
        .def("divRowExact", &MatrixInt::divRowExact)
        .def("divColExact", &MatrixInt::divColExact)
        .def("gcdRow", &MatrixInt::gcdRow)
        .def("gcdCol", &MatrixInt::gcdCol)
        .def("reduceRow", &MatrixInt::reduceRow)
        .def("reduceCol", &MatrixInt::reduceCol)
        .def("__mul__", [](const MatrixInt& m1, const MatrixInt& m2){
            return m1.multiplyAs<MatrixInt>(m2);
        })
        .def_readonly_static("zero", &MatrixInt::zero)
        .def_readonly_static("one", &MatrixInt::one)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("NMatrixInt") = m.attr("MatrixInt");
}

