
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

package normal.engine.implementation.jni;

import normal.Shell;
import normal.engine.*;
import normal.engine.algebra.*;
import normal.engine.angle.*;
import normal.engine.file.*;
import normal.engine.maths.*;
import normal.engine.packet.*;
import normal.engine.progress.*;
import normal.engine.subcomplex.*;
import normal.engine.surfaces.*;
import normal.engine.triangulation.*;
import normal.engine.utilities.*;
import normal.engine.implementation.jni.algebra.*;
import normal.engine.implementation.jni.angle.*;
import normal.engine.implementation.jni.file.*;
import normal.engine.implementation.jni.maths.*;
import normal.engine.implementation.jni.packet.*;
import normal.engine.implementation.jni.progress.*;
import normal.engine.implementation.jni.subcomplex.*;
import normal.engine.implementation.jni.surfaces.*;
import normal.engine.implementation.jni.triangulation.*;

/**
 * Used when the engine is in an external library to be accessed through JNI.
 * See <tt>Engine</tt> for more details.
 *
 * @see normal.engine.Engine
 */
public class JNIEngine implements Engine {
    /**
     * The name of the external library containing the calculation
     * engine.  See the constructor documentation for further details.
     */
    private String library;

    /**
     * Create a link to the calculation engine using JNI.
     * The engine should be stored in an external library.
     *
     * @param library the name of the external library containing the
     * calculation engine.  Note that the real name of the library may differ
     * between platforms.  For instance, a library called <i>name</i> will be
     * found as lib<i>name</i>.so on unix-style platforms, or <i>name</i>.dll
     * on Win32 platforms.
     * @throws JNILibraryException thrown when the external library failed
     * to load.
     */
    public JNIEngine(String library) throws JNILibraryException {
        this.library = library;
        try {
            System.loadLibrary(library);
        } catch (Throwable th) {
            System.out.println("Could not load library " + library + ": "
                + th.toString());
            throw new JNILibraryException(library);
        }
    }

    /**
     * Provides a polite string representation for this object.
     * 
     * @return a string representation of this engine.
     */
    public String toString() {
        return normal.Application.program +
            " calculation engine accessed through JNI (" + library + ")";
    }

    public int style() {
        return Shell.engineJNI;
    }
    public String styleDescription() {
        return "accessed through JNI (" + library + ")";
    }

    public NAbelianGroup newNAbelianGroup() {
        return new NJNIAbelianGroup();
    }
    public NAbelianGroup newNAbelianGroup(NAbelianGroup cloneMe) {
        return new NJNIAbelianGroup(cloneMe);
    }
    public NAngleStructureList newNAngleStructureList(NTriangulation owner) {
        return new NJNIAngleStructureList(owner);
    }
    public NContainer newNContainer() {
        return new NJNIContainer();
    }
    public NGroupExpression newNGroupExpression() {
        return new NJNIGroupExpression();
    }
    public NGroupExpression newNGroupExpression(NGroupExpression cloneMe) {
        return new NJNIGroupExpression(cloneMe);
    }
    public NGroupPresentation newNGroupPresentation() {
        return new NJNIGroupPresentation();
    }
    public NGroupPresentation newNGroupPresentation(
            NGroupPresentation cloneMe) {
        return new NJNIGroupPresentation(cloneMe);
    }
    public NFile newNFile() {
        return new NJNIFile();
    }
    public NLayeredChain newNLayeredChain(NTetrahedron tet, NPerm vertexRoles) {
        return new NJNILayeredChain(tet, vertexRoles);
    }
    public NLayeredChain newNLayeredChain(NLayeredChain cloneMe) {
        return new NJNILayeredChain(cloneMe);
    }
    public NLensSpace newNLensSpace(long p, long q) {
        return new NJNILensSpace(p, q);
    }
    public NLensSpace newNLensSpace(NLensSpace cloneMe) {
        return new NJNILensSpace(cloneMe);
    }
    public NMatrixInt newNMatrixInt(int rows, int columns) {
        return new NJNIMatrixInt(rows, columns);
    }
    public NMatrixInt newNMatrixInt(NMatrixInt cloneMe) {
        return new NJNIMatrixInt(cloneMe);
    }
    public NNormalSurfaceList newNNormalSurfaceList(NTriangulation owner,
            int flavour, boolean isEmbeddedOnly) {
        return new NJNINormalSurfaceList(owner, flavour, isEmbeddedOnly);
    }
    public NNormalSurfaceList newNNormalSurfaceList(NTriangulation owner,
            int flavour) {
        return new NJNINormalSurfaceList(owner, flavour);
    }
    public NProgressManager newNProgressManager() {
        return new NJNIProgressManager();
    }
    public NSFS newNSFS() {
        return new NJNISFS();
    }
    public NSFS newNSFS(long orbitGenus, boolean orbitOrientable) {
        return new NJNISFS(orbitGenus, orbitOrientable, 0);
    }
    public NSFS newNSFS(long orbitGenus, boolean orbitOrientable,
            long orbitPunctures) {
        return new NJNISFS(orbitGenus, orbitOrientable, orbitPunctures);
    }
    public NSFS newNSFS(NSFS cloneMe) {
        return new NJNISFS(cloneMe);
    }
    public NScript newNScript() {
        return new NJNIScript();
    }
    public NSurfaceFilter newNSurfaceFilter() {
        return new NJNISurfaceFilter();
    }
    public NSurfaceFilter newNSurfaceFilter(NSurfaceFilter cloneMe) {
        return new NJNISurfaceFilter(cloneMe);
    }
    public NSurfaceFilterCombination newNSurfaceFilterCombination() {
        return new NJNISurfaceFilterCombination();
    }
    public NSurfaceFilterCombination newNSurfaceFilterCombination(
            NSurfaceFilterCombination cloneMe) {
        return new NJNISurfaceFilterCombination(cloneMe);
    }
    public NSurfaceFilterProperties newNSurfaceFilterProperties() {
        return new NJNISurfaceFilterProperties();
    }
    public NSurfaceFilterProperties newNSurfaceFilterProperties(
            NSurfaceFilterProperties cloneMe) {
        return new NJNISurfaceFilterProperties(cloneMe);
    }
    public NSurfaceSubset newNSurfaceSubset(NSurfaceSet set,
            NSurfaceFilter filter) {
        return new NJNISurfaceSubset(set, filter);
    }
    public NTetrahedron newNTetrahedron() {
        return new NJNITetrahedron();
    }
    public NTetrahedron newNTetrahedron(String desc) {
        return new NJNITetrahedron(desc);
    }
    public NText newNText() {
        return new NJNIText();
    }
    public NText newNText(String text) {
        return new NJNIText(text);
    }
    public NTriangulation newNTriangulation() {
        return new NJNITriangulation();
    }
    public NTriangulation newNTriangulation(NTriangulation cloneMe) {
        return new NJNITriangulation(cloneMe);
    }

    public native NTriangulation enterTextTriangulation();
    public long formCensus(NPacket parent, int nTetrahedra,
            NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary) {
        return _formCensus(parent, nTetrahedra, finiteness.getByteCode(),
            orientability.getByteCode(), boundary.getByteCode(), -1, null);
    }
    public long formCensus(NPacket parent, int nTetrahedra,
            NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
            int nBdryFaces) {
        return _formCensus(parent, nTetrahedra, finiteness.getByteCode(),
            orientability.getByteCode(), boundary.getByteCode(), nBdryFaces,
            null);
    }
    public long formCensus(NPacket parent, int nTetrahedra, 
            NBoolSet finiteness, NBoolSet orientability, NBoolSet boundary,
            int nBdryFaces, NProgressManager manager) {
        return _formCensus(parent, nTetrahedra, finiteness.getByteCode(),
            orientability.getByteCode(), boundary.getByteCode(),
            nBdryFaces, manager);
    }
    public native long _formCensus(NPacket parent, int nTetrahedra, 
            char finiteness, char orientability, char boundary,
            int nBdryFaces, NProgressManager manager);
    public native NPillowTwoSphere formsPillowTwoSphere(
        NFace face1, NFace face2);
    public native NSnappedTwoSphere formsSnappedTwoSphere(
        NSnappedBall ball1, NSnappedBall ball2);
    public native NSnappedTwoSphere formsSnappedTwoSphere(
        NTetrahedron tet1, NTetrahedron tet2);
    public native int getVersionMajor();
    public native int getVersionMinor();
    public native String getVersionString();
    public native NAugTriSolidTorus isAugTriSolidTorus(NComponent comp);
    public native NLayeredLensSpace isLayeredLensSpace(NComponent comp);
    public native NLayeredLoop isLayeredLoop(NComponent comp);
    public native NLayeredSolidTorus isLayeredSolidTorusBase(NTetrahedron tet);
    public native NSnappedBall isSnappedBall(NTetrahedron tet);
    public NSpiralSolidTorus isSpiralSolidTorus(NTetrahedron tet,
            NPerm vertexRoles) {
        return _isSpiralSolidTorus(tet, vertexRoles.getPermCode());
    }
    public native NSpiralSolidTorus _isSpiralSolidTorus(NTetrahedron tet,
            char vertexRoles);
    public NTriSolidTorus isTriSolidTorus(NTetrahedron tet,
            NPerm vertexRoles) {
        return _isTriSolidTorus(tet, vertexRoles.getPermCode());
    }
    public native NTriSolidTorus _isTriSolidTorus(NTetrahedron tet,
            char vertexRoles);
    public native NMatrixInt makeMatchingEquations(NTriangulation
            triangulation, int flavour);
    public native NPacket readFromFile(String fileName);
    public native NTriangulation readSnapPea(String file);
    public native void smithNormalForm(NMatrixInt matrix);
    public native int testEngine(int value);
    public native boolean writeSnapPea(String fileName, NTriangulation tri);
    public native boolean writeToFile(String fileName, NPacket packet);
}
