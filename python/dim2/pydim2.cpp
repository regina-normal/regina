
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

namespace pybind11 { class module_; }

void addBoundaryComponent2(pybind11::module_& m, pybind11::module_& internal);
void addComponent2(pybind11::module_& m, pybind11::module_& internal);
void addEdge2(pybind11::module_& m, pybind11::module_& internal);
void addExample2(pybind11::module_& m);
void addIsomorphism2(pybind11::module_& m);
void addTriangle2(pybind11::module_& m);
void addTriangulation2(pybind11::module_& m, pybind11::module_& internal);
void addVertex2(pybind11::module_& m, pybind11::module_& internal);

void addDim2Classes(pybind11::module_& m, pybind11::module_& internal) {
    addBoundaryComponent2(m, internal);
    addComponent2(m, internal);
    addVertex2(m, internal);
    addEdge2(m, internal);
    addTriangle2(m);
    addIsomorphism2(m);
    addTriangulation2(m, internal);
    addExample2(m);
}

