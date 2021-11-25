
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

void addGraphLoop(pybind11::module_& m);
void addGraphPair(pybind11::module_& m);
void addGraphTriple(pybind11::module_& m);
void addHandlebody(pybind11::module_& m);
void addManifold(pybind11::module_& m);
void addLensSpace(pybind11::module_& m);
void addSFSpace(pybind11::module_& m);
void addSFSAlt(pybind11::module_& m);
void addSimpleSurfaceBundle(pybind11::module_& m);
void addSnapPeaCensusManifold(pybind11::module_& m);
void addTorusBundle(pybind11::module_& m);

void addManifoldClasses(pybind11::module_& m) {
    addManifold(m);
    addHandlebody(m);
    addLensSpace(m);
    addSFSpace(m);
    addSFSAlt(m);
    addSimpleSurfaceBundle(m);
    addSnapPeaCensusManifold(m);
    addTorusBundle(m);
    addGraphLoop(m);
    addGraphPair(m);
    addGraphTriple(m);
}

