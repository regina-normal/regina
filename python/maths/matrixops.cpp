
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include "maths/matrixops.h"
#include <boost/python.hpp>
#include <boost/python/detail/api_placeholder.hpp> // For len().

using namespace boost::python;
using regina::NMatrixInt;

namespace {
    void (*SNF_nobasis)(NMatrixInt&) = regina::smithNormalForm;
    void (*SNF_basis)(NMatrixInt&, NMatrixInt&, NMatrixInt&,
        NMatrixInt&, NMatrixInt&) = regina::smithNormalForm;

    void columnEchelonForm_list(NMatrixInt& m, NMatrixInt& r,
            NMatrixInt& rInv, boost::python::list rowList) {
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

    std::auto_ptr<NMatrixInt> preImageOfLattice_list(const NMatrixInt& m,
            boost::python::list l) {
        if (boost::python::len(l) != m.rows()) {
            PyErr_SetString(PyExc_IndexError,
                "Sublattice vector does not contain the expected number "
                "of elements.");
            boost::python::throw_error_already_set();
        }

        std::vector<regina::NLargeInteger> lVector;

        for (unsigned long i = 0; i < m.rows(); ++i) {
            // Accept any type that we know how to convert to a large integer.
            extract<regina::NLargeInteger&> x_large(l[i]);
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
}

void addMatrixOps() {
    def("smithNormalForm", SNF_nobasis);
    def("smithNormalForm", SNF_basis);
    def("metricalSmithNormalForm", regina::metricalSmithNormalForm);
    def("rowBasis", regina::rowBasis);
    def("rowBasisAndOrthComp", regina::rowBasisAndOrthComp);
    def("columnEchelonForm", columnEchelonForm_list);
    def("preImageOfLattice", preImageOfLattice_list);
    // TODO: torsionAutInverse_list
}

