
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

namespace pybind11 { class module_; }

void addBoundaryComponent4(pybind11::module_& m);
void addComponent4(pybind11::module_& m);
void addEdge4(pybind11::module_& m);
void addExample4(pybind11::module_& m);
void addIsomorphism4(pybind11::module_& m);
void addPentachoron4(pybind11::module_& m);
void addTetrahedron4(pybind11::module_& m);
void addTriangle4(pybind11::module_& m);
void addTriangulation4(pybind11::module_& m);
void addVertex4(pybind11::module_& m);

void addDim4Classes(pybind11::module_& m) {
    addBoundaryComponent4(m);
    addComponent4(m);
    addEdge4(m);
    addExample4(m);
    addIsomorphism4(m);
    addPentachoron4(m);
    addTetrahedron4(m);
    addTriangle4(m);
    addTriangulation4(m);
    addVertex4(m);
}

