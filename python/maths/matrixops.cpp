
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
#include "maths/matrixops.h"
#include <boost/python/detail/api_placeholder.hpp> // For len().

using namespace boost::python;
using regina::MatrixInt;

namespace {
    void (*SNF_nobasis)(MatrixInt&) = regina::smithNormalForm;
    void (*SNF_basis)(MatrixInt&, MatrixInt&, MatrixInt&,
        MatrixInt&, MatrixInt&) = regina::smithNormalForm;

    void columnEchelonForm_list(MatrixInt& m, MatrixInt& r,
            MatrixInt& rInv, boost::python::list rowList) {
        std::vector<unsigned> rowListVector;

        long len = boost::python::len(rowList);
        for (long i = 0; i < len; i++) {
            extract<long> x_long(rowList[i]);

            // Better make sure we can actually convert to an unsigned int.
            if (x_long() < 0) {
                PyErr_SetString(PyExc_IndexError,
                    "Row indices may not be negative.");
                boost::python::throw_error_already_set();
            }

            rowListVector.push_back(x_long());
        }

        regina::columnEchelonForm(m, r, rInv, rowListVector);
    }

    std::unique_ptr<MatrixInt> preImageOfLattice_list(const MatrixInt& m,
            boost::python::list l) {
        if (boost::python::len(l) != m.rows()) {
            PyErr_SetString(PyExc_IndexError,
                "Sublattice vector does not contain the expected number "
                "of elements.");
            boost::python::throw_error_already_set();
        }

        std::vector<regina::Integer> lVector;

        for (unsigned long i = 0; i < m.rows(); ++i) {
            // Accept any type that we know how to convert to a large integer.
            extract<regina::Integer&> x_large(l[i]);
            if (x_large.check()) {
                lVector.push_back(x_large());
                continue;
            }

            extract<long> x_long(l[i]);
            if (x_long.check()) {
                lVector.push_back(x_long());
                continue;
            }

            extract<const char*> x_str(l[i]);
            if (x_str.check()) {
                lVector.push_back(x_str());
                continue;
            }

            // Throw an exception.
            x_large();
        }

        return regina::preImageOfLattice(m, lVector);
    }

    std::unique_ptr<MatrixInt> torsionAutInverse_list(const MatrixInt& m,
            boost::python::list l) {
        if (boost::python::len(l) != m.rows()) {
            PyErr_SetString(PyExc_IndexError,
                "The vector invF does not contain the expected number "
                "of elements.");
            boost::python::throw_error_already_set();
        }

        std::vector<regina::Integer> lVector;

        for (unsigned long i = 0; i < m.rows(); ++i) {
            // Accept any type that we know how to convert to a large integer.
            extract<regina::Integer&> x_large(l[i]);
            if (x_large.check()) {
                lVector.push_back(x_large());
                continue;
            }

            extract<long> x_long(l[i]);
            if (x_long.check()) {
                lVector.push_back(x_long());
                continue;
            }

            extract<const char*> x_str(l[i]);
            if (x_str.check()) {
                lVector.push_back(x_str());
                continue;
            }

            // Throw an exception.
            x_large();
        }

        return regina::torsionAutInverse(m, lVector);
    }
}

void addMatrixOps() {
    def("smithNormalForm", SNF_nobasis);
    def("smithNormalForm", SNF_basis);
    def("metricalSmithNormalForm", regina::metricalSmithNormalForm);
    def("rowBasis", regina::rowBasis);
    def("rowBasisAndOrthComp", regina::rowBasisAndOrthComp);
    def("columnEchelonForm", columnEchelonForm_list);
    def("preImageOfLattice", preImageOfLattice_list);
    def("torsionAutInverse", torsionAutInverse_list);
}

