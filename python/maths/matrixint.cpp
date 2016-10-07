
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
#include "maths/matrix.h"
#include <boost/python/detail/api_placeholder.hpp> // For len().
#include "../helpers.h"

using namespace boost::python;
using regina::MatrixInt;

namespace {
    regina::Integer& (MatrixInt::*entry_non_const)(unsigned long,
        unsigned long) = &MatrixInt::entry;
    void (MatrixInt::*addRow_triple)(unsigned long, unsigned long,
        regina::Integer) = &MatrixInt::addRow;
    void (MatrixInt::*addCol_triple)(unsigned long, unsigned long,
        regina::Integer) = &MatrixInt::addCol;

    std::unique_ptr<MatrixInt> multiply(const MatrixInt& m1,
            const MatrixInt& m2) {
        return m1.multiplyAs<MatrixInt>(m2);
    }

    void setEntry(MatrixInt& matrix, unsigned long row, unsigned long column,
            const regina::Integer& value) {
        matrix.entry(row, column) = value;
    }

    void initialise_list(MatrixInt& matrix, boost::python::list values) {
        if (boost::python::len(values) != matrix.rows() * matrix.columns()) {
            PyErr_SetString(PyExc_IndexError,
                "Initialisation list does not contain the "
                "expected number of elements.");
            boost::python::throw_error_already_set();
        } else {
            unsigned long r, c;
            unsigned i = 0;
            for (r = 0; r < matrix.rows(); ++r)
                for (c = 0; c < matrix.columns(); ++c) {
                    // Accept any type that we know how to convert to a
                    // large integer.
                    extract<regina::Integer&> x_large(values[i]);
                    if (x_large.check()) {
                        matrix.entry(r, c) = x_large();
                        ++i;
                        continue;
                    }

                    extract<long> x_long(values[i]);
                    if (x_long.check()) {
                        matrix.entry(r, c) = x_long();
                        ++i;
                        continue;
                    }

                    extract<const char*> x_str(values[i]);
                    if (x_str.check()) {
                        matrix.entry(r, c) = x_str();
                        ++i;
                        continue;
                    }

                    // Throw an exception.
                    x_large();
                }
        }
    }

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addRow,
        MatrixInt::addRow, 2, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addCol,
        MatrixInt::addCol, 2, 3);
}

void addMatrixInt() {
    {
        scope s = class_<MatrixInt,
                std::auto_ptr<MatrixInt>, boost::noncopyable>("MatrixInt",
                init<unsigned long, unsigned long>())
            .def(init<const MatrixInt&>())
            .def("initialise", &MatrixInt::initialise)
            .def("initialise", initialise_list)
            .def("rows", &MatrixInt::rows)
            .def("columns", &MatrixInt::columns)
            .def("entry", entry_non_const, return_internal_reference<>())
            .def("set", setEntry)
            .def("isIdentity", &MatrixInt::isIdentity)
            .def("isZero", &MatrixInt::isZero)
            .def("swapRows", &MatrixInt::swapRows)
            .def("swapColumns", &MatrixInt::swapColumns)
            .def("makeIdentity", &MatrixInt::makeIdentity)
            .def("addRow", addRow_triple, OL_addRow())
            .def("addCol", addCol_triple, OL_addCol())
            .def("multRow", &MatrixInt::multRow)
            .def("multCol", &MatrixInt::multCol)
            .def("det", &MatrixInt::det)
            .def("divRowExact", &MatrixInt::divRowExact)
            .def("divColExact", &MatrixInt::divColExact)
            .def("gcdRow", &MatrixInt::gcdRow)
            .def("gcdCol", &MatrixInt::gcdCol)
            .def("reduceRow", &MatrixInt::reduceRow)
            .def("reduceCol", &MatrixInt::reduceCol)
            .def("__mul__", multiply)
            .def(regina::python::add_output())
            .def(regina::python::add_eq_operators())
        ;

        s.attr("zero") = MatrixInt::zero;
        s.attr("one") = MatrixInt::one;
    }

    scope().attr("NMatrixInt") = scope().attr("MatrixInt");
}

