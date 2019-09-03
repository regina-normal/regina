
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

namespace pybind11 { class module; }

void addBinom(pybind11::module& m);
void addMatrixOps(pybind11::module& m);
void addCyclotomic(pybind11::module& m);
void addInteger(pybind11::module& m);
void addLargeInteger(pybind11::module& m);
void addLaurent(pybind11::module& m);
void addLaurent2(pybind11::module& m);
void addMatrix2(pybind11::module& m);
void addMatrixInt(pybind11::module& m);
void addPerm2(pybind11::module& m);
void addPerm3(pybind11::module& m);
void addPerm4(pybind11::module& m);
void addPerm5(pybind11::module& m);
void addPerm(pybind11::module& m);
void addPolynomial(pybind11::module& m);
void addPrimes(pybind11::module& m);
void addRational(pybind11::module& m);
void addNumberTheory(pybind11::module& m);

void addMathsClasses(pybind11::module& m) {
    addBinom(m);
    addMatrixOps(m);
    addCyclotomic(m);
    addInteger(m);
    addLargeInteger(m);
    addLaurent(m);
    addLaurent2(m);
    addMatrix2(m);
    addMatrixInt(m);
    addPerm2(m);
    addPerm3(m);
    addPerm4(m);
    addPerm5(m);
    addPerm(m);
    addPolynomial(m);
    addPrimes(m);
    addRational(m);
    addNumberTheory(m);
}

