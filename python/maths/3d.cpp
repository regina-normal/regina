
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "maths/3d.h"
#include "../helpers.h"
#include "../docstrings/maths/3d.h"

using regina::Matrix3D;
using regina::Rotation3D;
using regina::Segment3D;
using regina::Vector3D;

namespace regina {
    /**
     * A utility class that provides access to a single row of a 3-D matrix.
     * This allows us to write m[i][j] in Python for a 3-D matrix m.
     */
    class Matrix3DRow {
        private:
            std::array<double, 3>& row_;

        public:
            Matrix3DRow(Matrix3D<double>& m, int row) : row_(m[row]) {
            }

            double getItem(int col) {
                if (col < 0 || col > 2)
                    throw pybind11::index_error(
                        "Matrix3D column index out of range");
                return row_[col];
            }

            void setItem(int col, double value) {
                if (col < 0 || col > 2)
                    throw pybind11::index_error(
                        "Matrix3D column index out of range");
                row_[col] = value;
            }

            bool operator == (const Matrix3DRow& other) const {
                return row_ == other.row_;
            }

        friend std::ostream& operator << (std::ostream&, const Matrix3DRow&);
    };

    inline std::ostream& operator << (std::ostream& out, const Matrix3DRow& r) {
        return out << "[ " << r.row_[0] << ' ' << r.row_[1] << ' '
            << r.row_[2] << " ]";
    }
}

void add3D(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Vector3D)

    auto v = pybind11::class_<Vector3D<double>>(m, "Vector3D", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Vector3D<double>&>(), rdoc::__copy)
        .def(pybind11::init<double, double, double>(), rdoc::__init)
        .def(pybind11::init<std::array<double, 3>>(), rdoc::__init_2)
        .def(pybind11::self + pybind11::self, rdoc::__add)
        .def(pybind11::self - pybind11::self, rdoc::__sub)
        .def(pybind11::self * double(), rdoc::__mul)
        .def(pybind11::self / double(), rdoc::__div)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self *= double(), rdoc::__imul)
        .def(pybind11::self /= double(), rdoc::__idiv)
        .def(pybind11::self * pybind11::self, rdoc::__mul_2)
        .def("length", &Vector3D<double>::length, rdoc::length)
        .def("distance", &Vector3D<double>::distance, rdoc::distance)
        .def("midpoint", &Vector3D<double>::midpoint, rdoc::midpoint)
        .def_readwrite("x", &Vector3D<double>::x, rdoc::x)
        .def_readwrite("y", &Vector3D<double>::y, rdoc::y)
        .def_readwrite("z", &Vector3D<double>::z, rdoc::z)
    ;
    regina::python::add_output_ostream(v);
    regina::python::add_eq_operators(v, rdoc::__eq);

    RDOC_SCOPE_SWITCH(Segment3D)

    auto seg = pybind11::class_<Segment3D<double>>(m, "Segment3D", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Segment3D<double>&>(), rdoc::__copy)
        .def(pybind11::init<const Vector3D<double>&, const Vector3D<double>&>(),
            rdoc::__init)
        .def("length", &Segment3D<double>::length, rdoc::length)
        .def("point", &Segment3D<double>::point, rdoc::point)
        .def(pybind11::self + Vector3D(), rdoc::__add)
        .def(pybind11::self - Vector3D(), rdoc::__sub)
        .def(pybind11::self += Vector3D(), rdoc::__iadd)
        .def(pybind11::self -= Vector3D(), rdoc::__isub)
        .def("midpoint", &Segment3D<double>::midpoint, rdoc::midpoint)
        .def("closest", &Segment3D<double>::closest, rdoc::closest)
        .def_readwrite("u", &Segment3D<double>::u, rdoc::u)
        .def_readwrite("v", &Segment3D<double>::v, rdoc::v)
    ;
    regina::python::add_output_ostream(seg);
    regina::python::add_eq_operators(seg, rdoc::__eq);

    RDOC_SCOPE_SWITCH(Matrix3D)

    auto mat = pybind11::class_<Matrix3D<double>>(m, "Matrix3D", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Matrix3D<double>&>(), rdoc::__copy)
        .def(pybind11::init<double, double, double, double, double, double,
            double, double, double>(), rdoc::__init)
        .def("__getitem__", [](Matrix3D<double>& m, int row) {
            if (row < 0 || row > 2)
                throw pybind11::index_error(
                    "Matrix3D row index out of range");
            return new regina::Matrix3DRow(m, row);
        }, pybind11::keep_alive<0, 1>(), rdoc::__array)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def(pybind11::self *= pybind11::self, rdoc::__imul)
        .def(pybind11::self * Vector3D(), rdoc::__mul_2)
        .def("inverse", &Matrix3D<double>::inverse, rdoc::inverse)
        .def("__len__", [](const Matrix3D<double>&) {
            return 3;
        }, "Returns the number of rows in this matrix. This will always be 3.")
    ;
    regina::python::add_output_ostream(mat);
    regina::python::add_eq_operators(mat, rdoc::__eq);

    auto row = pybind11::class_<regina::Matrix3DRow>(mat, "_Row",
R"doc(Gives access to a single row of a 3-D matrix.

See the main class Matrix3D for further details.)doc")
        .def("__getitem__", &regina::Matrix3DRow::getItem,
R"doc(Returns the entry at the given index in this row.

The given index must be either 0, 1 or 2.

You should not need to call this directly.  To access the (*i*, *j*)
entry of a 3-D matrix *M*, you can call ``M[i][j]``.

See the main class Matrix3D for further details.)doc")
        .def("__setitem__", &regina::Matrix3DRow::setItem,
R"doc(Sets the entry at the given index in this row to the given value.

The given index must be either 0, 1 or 2.

You should not need to call this directly.  To set the (*i*, *j*)
entry of a 3-D matrix *M*, you can use ``M[i][j] = value``.

See the main class Matrix3D for further details.)doc")
        .def("__len__", [](const regina::Matrix3DRow&) {
            return 3;
        },
"Returns the number of entries in this row. This will always be 3.")
        ;
    regina::python::add_eq_operators(row,
R"doc(Tests whether this and the given row contain the same entries.

Be aware of the inherent risks of floating-point comparisons.)doc");
    regina::python::add_output_ostream(row);

    RDOC_SCOPE_SWITCH(Rotation3D)

    auto r = pybind11::class_<Rotation3D<double>>(m, "Rotation3D", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Rotation3D<double>&>(), rdoc::__copy)
        .def(pybind11::init<double, double, double, double>(), rdoc::__init)
        .def("__getitem__", [](const Rotation3D<double>& r, int i) -> double {
            return r[i];
        }, rdoc::__array)
        .def("normalise", &Rotation3D<double>::normalise, rdoc::normalise)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def(pybind11::self *= pybind11::self, rdoc::__imul)
        .def(pybind11::self * Vector3D(), rdoc::__mul_2)
        .def("inverse", &Rotation3D<double>::inverse, rdoc::inverse)
        .def("matrix", &Rotation3D<double>::matrix, rdoc::matrix)
    ;
    regina::python::add_output_ostream(r);
    regina::python::add_eq_operators(r, rdoc::__eq);

    RDOC_SCOPE_END
}
