
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Java user interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
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
import normal.engine.file.*;
import normal.engine.maths.*;
import normal.engine.packet.*;
import normal.engine.progress.*;
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
    public int style();
    /**
     * Returns a short description of the style of engine represented by
     * this engine object.
     *
     * @return a description of the style of engine.
     */
    public String styleDescription();

    public NAbelianGroup newNAbelianGroup();
    public NAbelianGroup newNAbelianGroup(NAbelianGroup cloneMe);
    public NContainer newNContainer();
    public NFile newNFile();
	public NGroupExpression newNGroupExpression();
	public NGroupExpression newNGroupExpression(NGroupExpression cloneMe);
	public NGroupPresentation newNGroupPresentation();
	public NGroupPresentation newNGroupPresentation(
		NGroupPresentation cloneMe);
    public NMatrixInt newNMatrixInt(int rows, int columns);
    public NMatrixInt newNMatrixInt(NMatrixInt cloneMe);
    public NNormalSurfaceList newNNormalSurfaceList(NTriangulation owner,
        int flavour, boolean isEmbeddedOnly);
    public NNormalSurfaceList newNNormalSurfaceList(NTriangulation owner,
        int flavour);
    public NProgressManager newNProgressManager();
    public NScript newNScript();
    public NSurfaceFilter newNSurfaceFilter();
    public NSurfaceFilter newNSurfaceFilter(NSurfaceFilter cloneMe);
    public NSurfaceFilterCombination newNSurfaceFilterCombination();
    public NSurfaceFilterCombination newNSurfaceFilterCombination(
        NSurfaceFilterCombination cloneMe);
    public NSurfaceFilterProperties newNSurfaceFilterProperties();
    public NSurfaceFilterProperties newNSurfaceFilterProperties(
        NSurfaceFilterProperties cloneMe);
    public NSurfaceSubset newNSurfaceSubset(NSurfaceSet set,
        NSurfaceFilter filter);
    public NTetrahedron newNTetrahedron();
    public NTetrahedron newNTetrahedron(String desc);
    public NText newNText();
    public NText newNText(String text);
    public NTriangulation newNTriangulation();
    public NTriangulation newNTriangulation(NTriangulation cloneMe);

    public NTriangulation enterTextTriangulation();
    public long formCensus(NPacket parent, int nTetrahedra,
        NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary);
    public long formCensus(NPacket parent, int nTetrahedra,
        NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
        int nBdryFaces);
    public long formCensus(NPacket parent, int nTetrahedra,
        NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
        int nBdryFaces, NProgressManager manager);
	public NPillowTwoSphere formsPillowTwoSphere(NFace face1, NFace face2);
	public NSnappedTwoSphere formsSnappedTwoSphere(NSnappedBall ball1,
		NSnappedBall ball2);
	public NSnappedTwoSphere formsSnappedTwoSphere(NTetrahedron tet1,
		NTetrahedron tet2);
    public int getVersionMajor();
    public int getVersionMinor();
    public String getVersionString();
	public NLayeredLensSpace isLayeredLensSpace(NComponent comp);
	public NLayeredSolidTorus isLayeredSolidTorusBase(NTetrahedron tet);
	public NSnappedBall isSnappedBall(NTetrahedron tet);
    public NMatrixInt makeMatchingEquations(NTriangulation
        triangulation, int flavour);
    public NPacket readFromFile(String fileName);
    public NTriangulation readSnapPea(String file);
    public void smithNormalForm(NMatrixInt matrix);
    public int testEngine(int value);
    public boolean writeToFile(String fileName, NPacket packet);
}

