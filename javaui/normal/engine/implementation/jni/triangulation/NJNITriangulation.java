
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
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

package normal.engine.implementation.jni.triangulation;

import normal.engine.implementation.jni.packet.*;
import normal.engine.implementation.jni.*;
import normal.engine.triangulation.*;
import normal.engine.algebra.*;

public class NJNITriangulation extends NJNIPacket
        implements NTriangulation {
    public NJNITriangulation(Sentry s) {
        super(s);
    }
    public NJNITriangulation() {
        super(Sentry.instance);
        newNTriangulation();
    }
    public NJNITriangulation(NTriangulation cloneMe) {
        super(Sentry.instance);
        newNTriangulation(cloneMe);
    }
    private final native void newNTriangulation();
    private final native void newNTriangulation(NTriangulation cloneMe);

    public native long getNumberOfTetrahedra();
    public native NTetrahedron getTetrahedron(long index);
    public native long getTetrahedronIndex(NTetrahedron tetrahedron);

    public native void addTetrahedron(NTetrahedron newTet);
    public native void removeTetrahedronAt(long index);
    public native void removeTetrahedron(NTetrahedron tet);
    public native void removeAllTetrahedra();

    public native void gluingsHaveChanged();

    public native long getNumberOfComponents();
    public native NComponent getComponent(long index);
    public native long getComponentIndex(NComponent component);
    public native long getNumberOfBoundaryComponents();
    public native NBoundaryComponent getBoundaryComponent(long index);
    public native long getBoundaryComponentIndex(NBoundaryComponent bc);
    public native long getNumberOfVertices();
    public native NVertex getVertex(long index);
    public native long getVertexIndex(NVertex vertex);
    public native long getNumberOfEdges();
    public native NEdge getEdge(long index);
    public native long getEdgeIndex(NEdge edge);
    public native long getNumberOfFaces();
    public native NFace getFace(long index);
    public native long getFaceIndex(NFace face);

    public native long getEulerCharacteristic();
    public native NGroupPresentation getFundamentalGroup();
    public native NAbelianGroup getHomologyH1();
    public native NAbelianGroup getHomologyH1Rel();
    public native NAbelianGroup getHomologyH1Bdry();
    public native NAbelianGroup getHomologyH2();
    public native long getHomologyH2Z2();

    public native boolean isIdeal();
    public native boolean isValid();
    public native boolean isStandard();
    public native boolean isClosed();
    public native boolean isOrientable();
    public native boolean isConnected();
    public native boolean hasBoundaryFaces();

    public native boolean crushMaximalForest();
    public native boolean intelligentSimplify();
    public boolean simplifyToLocalMinimum() {
        return simplifyToLocalMinimum(true);
    }
    public native boolean simplifyToLocalMinimum(boolean perform);

    public boolean threeTwoMove(NEdge e) {
        return threeTwoMove(e, true, true);
    }
    public boolean threeTwoMove(NEdge e, boolean check) {
        return threeTwoMove(e, check, true);
    }
    public native boolean threeTwoMove(NEdge e, boolean check, boolean perform);
    public boolean twoThreeMove(NFace f) {
        return twoThreeMove(f, true, true);
    }
    public boolean twoThreeMove(NFace f, boolean check) {
        return twoThreeMove(f, check, true);
    }
    public native boolean twoThreeMove(NFace f, boolean check, boolean perform);
    public boolean fourFourMove(NEdge e, int newAxis) {
        return fourFourMove(e, newAxis, true, true);
    }
    public boolean fourFourMove(NEdge e, int newAxis, boolean check) {
        return fourFourMove(e, newAxis, check, true);
    }
    public native boolean fourFourMove(NEdge e, int newAxis, boolean check,
            boolean perform);
    public boolean twoZeroMove(NEdge e) {
        return twoZeroMove(e, true, true);
    }
    public boolean twoZeroMove(NEdge e, boolean check) {
        return twoZeroMove(e, check, true);
    }
    public native boolean twoZeroMove(NEdge e, boolean check, boolean perform);
    public boolean twoZeroMove(NVertex v) {
        return twoZeroMove(v, true, true);
    }
    public boolean twoZeroMove(NVertex v, boolean check) {
        return twoZeroMove(v, check, true);
    }
    public native boolean twoZeroMove(NVertex v, boolean check, boolean perform);
    public boolean twoOneMove(NEdge e, int edgeEnd) {
        return twoOneMove(e, edgeEnd, true, true);
    }
    public boolean twoOneMove(NEdge e, int edgeEnd, boolean check) {
        return twoOneMove(e, edgeEnd, check, true);
    }
    public native boolean twoOneMove(NEdge e, int edgeEnd, boolean check, 
        boolean perform);
    public boolean openBook(NFace f) {
        return openBook(f, true, true);
    }
    public boolean openBook(NFace f, boolean check) {
        return openBook(f, check, true);
    }
    public native boolean openBook(NFace f, boolean check, boolean perform);
    public boolean shellBoundary(NTetrahedron t) {
        return shellBoundary(t, true, true);
    }
    public boolean shellBoundary(NTetrahedron t, boolean check) {
        return shellBoundary(t, check, true);
    }
    public native boolean shellBoundary(NTetrahedron t, boolean check, 
        boolean perform);

    public boolean idealToFinite() {
        return idealToFinite(false);
    }
    public native boolean idealToFinite(boolean forceDivision);
    public native void barycentricSubdivision();

    public native NTetrahedron insertLayeredSolidTorus(long cuts0, long cuts1);
    public native void insertLensSpace(long p, long q);
    public native void insertTriangulation(NTriangulation source);

    public native boolean isIsomorphicTo(NTriangulation other);
}
