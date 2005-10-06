
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

void addNAugTriSolidTorus();
void addNL31Pillow();
void addNLayeredChain();
void addNLayeredChainPair();
void addNLayeredLensSpace();
void addNLayeredLoop();
void addNLayeredSolidTorus();
void addNLayeredSurfaceBundle();
void addNPillowTwoSphere();
void addNPlugTriSolidTorus();
void addNSnapPeaCensusTri();
void addNSnappedBall();
void addNSnappedTwoSphere();
void addNSpiralSolidTorus();
void addNStandardTriangulation();
void addNTriSolidTorus();
void addNTrivialTri();
void addNTxICore();

void addSubcomplex() {
    addNStandardTriangulation();
    addNAugTriSolidTorus();
    addNL31Pillow();
    addNLayeredChain();
    addNLayeredChainPair();
    addNLayeredLensSpace();
    addNLayeredLoop();
    addNLayeredSolidTorus();
    addNLayeredSurfaceBundle();
    addNPillowTwoSphere();
    addNPlugTriSolidTorus();
    addNSnapPeaCensusTri();
    addNSnappedBall();
    addNSnappedTwoSphere();
    addNSpiralSolidTorus();
    addNTriSolidTorus();
    addNTrivialTri();
    addNTxICore();
}

