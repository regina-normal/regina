
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

void addBoundaryComponent3(pybind11::module_& m);
void addComponent3(pybind11::module_& m);
void addEdge3(pybind11::module_& m);
void addExample3(pybind11::module_& m);
void addFacePair(pybind11::module_& m);
void addFacetPairing3(pybind11::module_& m);
void addHomologicalData(pybind11::module_& m);
void addIsomorphism3(pybind11::module_& m);
void addTetrahedron3(pybind11::module_& m);
void addTriangle3(pybind11::module_& m);
void addTriangulation3(pybind11::module_& m);
void addVertex3(pybind11::module_& m);

void addTriangulationClasses(pybind11::module_& m) {
    addBoundaryComponent3(m);
    addComponent3(m);
    addEdge3(m);
    addExample3(m);
    addFacePair(m);
    addFacetPairing3(m);
    addHomologicalData(m);
    addIsomorphism3(m);
    addTetrahedron3(m);
    addTriangle3(m);
    addTriangulation3(m);
    addVertex3(m);
}

