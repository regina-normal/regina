
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

package normal.engine;

import normal.engine.algebra.*;
import normal.engine.angle.*;
import normal.engine.file.*;
import normal.engine.maths.*;
import normal.engine.packet.*;
import normal.engine.progress.*;
import normal.engine.split.*;
import normal.engine.subcomplex.*;
import normal.engine.surfaces.*;
import normal.engine.triangulation.*;
import normal.engine.utilities.*;

/**
 * Represents the link between Java and the calculation engine,
 * which may be external.
 * See the Java notes page in the engine documentation for further
 * details.
 * <p>
 * Specific engine implementations can be found in the package
 * <tt>normal.engine.implementation</tt>.
 */
public interface Engine {
    /**
     * Returns the style of engine represented by this engine object.
     * This will be one of the predefined engine style constants in
     * class <tt>normal.Shell</tt>.
     *
     * @return the style of engine.
     */
    int style();
    /**
     * Returns a short description of the style of engine represented by
     * this engine object.
     *
     * @return a description of the style of engine.
     */
    String styleDescription();

    NAbelianGroup newNAbelianGroup();
    NAbelianGroup newNAbelianGroup(NAbelianGroup cloneMe);
    NAngleStructureList newNAngleStructureList(NTriangulation owner);
    NContainer newNContainer();
    NFile newNFile();
    NGroupExpression newNGroupExpression();
    NGroupExpression newNGroupExpression(NGroupExpression cloneMe);
    NGroupPresentation newNGroupPresentation();
    NGroupPresentation newNGroupPresentation(NGroupPresentation cloneMe);
    NLayeredChain newNLayeredChain(NTetrahedron tet, NPerm vertexRoles);
    NLayeredChain newNLayeredChain(NLayeredChain cloneMe);
    NLensSpace newNLensSpace(long p, long q);
    NLensSpace newNLensSpace(NLensSpace cloneMe);
    NMatrixInt newNMatrixInt(int rows, int columns);
    NMatrixInt newNMatrixInt(NMatrixInt cloneMe);
    NNormalSurfaceList newNNormalSurfaceList(NTriangulation owner, int flavour,
        boolean isEmbeddedOnly);
    NNormalSurfaceList newNNormalSurfaceList(NTriangulation owner, int flavour);
    NProgressManager newNProgressManager();
    NSFS newNSFS();
    NSFS newNSFS(long orbitGenus, boolean orbitOrientable);
    NSFS newNSFS(long orbitGenus, boolean orbitOrientable, long orbitPunctures);
    NSFS newNSFS(NSFS cloneMe);
    NScript newNScript();
    NSignature newNSignature(NSignature cloneMe);
    NSurfaceFilter newNSurfaceFilter();
    NSurfaceFilter newNSurfaceFilter(NSurfaceFilter cloneMe);
    NSurfaceFilterCombination newNSurfaceFilterCombination();
    NSurfaceFilterCombination newNSurfaceFilterCombination(
        NSurfaceFilterCombination cloneMe);
    NSurfaceFilterProperties newNSurfaceFilterProperties();
    NSurfaceFilterProperties newNSurfaceFilterProperties(
        NSurfaceFilterProperties cloneMe);
    NSurfaceSubset newNSurfaceSubset(NSurfaceSet set, NSurfaceFilter filter);
    NTetrahedron newNTetrahedron();
    NTetrahedron newNTetrahedron(String desc);
    NText newNText();
    NText newNText(String text);
    NTriangulation newNTriangulation();
    NTriangulation newNTriangulation(NTriangulation cloneMe);

    NTriangulation enterTextTriangulation();
    long formCensus(NPacket parent, int nTetrahedra,
        NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary);
    long formCensus(NPacket parent, int nTetrahedra,
        NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
        int nBdryFaces);
    long formCensus(NPacket parent, int nTetrahedra,
        NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
        int nBdryFaces, NProgressManager manager);
    NPillowTwoSphere formsPillowTwoSphere(NFace face1, NFace face2);
    NSnappedTwoSphere formsSnappedTwoSphere(NSnappedBall ball1,
        NSnappedBall ball2);
    NSnappedTwoSphere formsSnappedTwoSphere(NTetrahedron tet1,
        NTetrahedron tet2);
    int getVersionMajor();
    int getVersionMinor();
    String getVersionString();
    NFileInfo identifyFileInfo(String idPathname);
    NAugTriSolidTorus isAugTriSolidTorus(NComponent comp);
    NLayeredLensSpace isLayeredLensSpace(NComponent comp);
    NLayeredLoop isLayeredLoop(NComponent comp);
    NLayeredSolidTorus isLayeredSolidTorusBase(NTetrahedron tet);
    NSnappedBall isSnappedBall(NTetrahedron tet);
    NSpiralSolidTorus isSpiralSolidTorus(NTetrahedron tet, NPerm roles);
    NTriSolidTorus isTriSolidTorus(NTetrahedron tet, NPerm vertexRoles);
    NMatrixInt makeMatchingEquations(NTriangulation triangulation, int flavour);
    NSignature parseSignature(String sig);
    NPacket readFileMagic(String fileName);
    NPacket readFromFile(String fileName);
    NPacket readXMLFile(String fileName);
    NTriangulation readSnapPea(String file);
    void smithNormalForm(NMatrixInt matrix);
    int testEngine(int value);
    boolean writeSnapPea(String fileName, NTriangulation tri);
    boolean writeToFile(String fileName, NPacket packet);
    boolean writeXMLFile(String fileName, NPacket subtree);
    boolean writeXMLFile(String fileName, NPacket subtree, boolean compressed);
}

