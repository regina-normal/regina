
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

void addAugTriSolidTorus(pybind11::module_& m);
void addBlockedSFS(pybind11::module_& m);
void addBlockedSFSLoop(pybind11::module_& m);
void addBlockedSFSPair(pybind11::module_& m);
void addBlockedSFSTriple(pybind11::module_& m);
void addL31Pillow(pybind11::module_& m);
void addLayeredChain(pybind11::module_& m);
void addLayeredChainPair(pybind11::module_& m);
void addLayeredLensSpace(pybind11::module_& m);
void addLayeredLoop(pybind11::module_& m);
void addLayeredSolidTorus(pybind11::module_& m);
void addLayeredTorusBundle(pybind11::module_& m);
void addLayering(pybind11::module_& m);
void addPillowTwoSphere(pybind11::module_& m);
void addPluggedTorusBundle(pybind11::module_& m);
void addPlugTriSolidTorus(pybind11::module_& m);
void addSatAnnulus(pybind11::module_& m);
void addSatBlock(pybind11::module_& m);
void addSatBlockTypes(pybind11::module_& m);
void addSatRegion(pybind11::module_& m);
void addSnapPeaCensusTri(pybind11::module_& m);
void addSnappedBall(pybind11::module_& m);
void addSnappedTwoSphere(pybind11::module_& m);
void addSpiralSolidTorus(pybind11::module_& m);
void addStandardTriangulation(pybind11::module_& m);
void addTriSolidTorus(pybind11::module_& m);
void addTrivialTri(pybind11::module_& m);
void addTxICore(pybind11::module_& m);

void addSubcomplexClasses(pybind11::module_& m) {
    addStandardTriangulation(m);
    addAugTriSolidTorus(m);
    addL31Pillow(m);
    addLayeredChain(m);
    addLayeredChainPair(m);
    addLayeredLensSpace(m);
    addLayeredLoop(m);
    addLayeredSolidTorus(m);
    addLayeredTorusBundle(m);
    addLayering(m);
    addPillowTwoSphere(m);
    addPlugTriSolidTorus(m);
    addSnapPeaCensusTri(m);
    addSnappedBall(m);
    addSnappedTwoSphere(m);
    addSpiralSolidTorus(m);
    addTriSolidTorus(m);
    addTrivialTri(m);
    addTxICore(m);
    addSatAnnulus(m);
    addSatBlock(m);
    addSatBlockTypes(m);
    addSatRegion(m);
    addBlockedSFS(m);
    addBlockedSFSLoop(m);
    addBlockedSFSPair(m);
    addBlockedSFSTriple(m);
    addPluggedTorusBundle(m);
}

