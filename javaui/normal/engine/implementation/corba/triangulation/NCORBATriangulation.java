
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

package normal.engine.implementation.corba.triangulation;

import normal.engine.implementation.corba.Regina.Triangulation.*;
import normal.engine.implementation.corba.algebra.*;
import normal.engine.implementation.corba.packet.*;

public class NCORBATriangulation extends NCORBAPacket
        implements normal.engine.triangulation.NTriangulation {
    public NTriangulation data;
    public static final Class CORBAClass = NTriangulation.class;
    public static final Class helperClass = NTriangulationHelper.class;

    public NCORBATriangulation(NTriangulation data) {
        super(data);
        this.data = data;
    }

    public static NCORBATriangulation newWrapper(NTriangulation source) {
        return (source == null ? null : new NCORBATriangulation(source));
    }

    public long getNumberOfTetrahedra() {
        return data.getNumberOfTetrahedra();
    }
    public normal.engine.triangulation.NTetrahedron
            getTetrahedron(long index) {
        return NCORBATetrahedron.newWrapper(data.getTetrahedron((int)index));
    }
    public long getTetrahedronIndex(
            normal.engine.triangulation.NTetrahedron tetrahedron) {
        return data.getTetrahedronIndex(((NCORBATetrahedron)tetrahedron).data);
    }

    public void addTetrahedron(
            normal.engine.triangulation.NTetrahedron newTet) {
        data.addTetrahedron(((NCORBATetrahedron)newTet).data);
    }
    public void removeTetrahedronAt(long index) {
        data.removeTetrahedronAt((int)index);
    }
    public void removeTetrahedron(
            normal.engine.triangulation.NTetrahedron tet) {
        data.removeTetrahedron(((NCORBATetrahedron)tet).data);
    }
    public void removeAllTetrahedra() {
        data.removeAllTetrahedra();
    }

    public void gluingsHaveChanged() {
        data.gluingsHaveChanged();
    }

    public long getNumberOfComponents() {
        return data.getNumberOfComponents();
    }
    public normal.engine.triangulation.NComponent getComponent(long index) {
        return NCORBAComponent.newWrapper(data.getComponent((int)index));
    }
    public long getComponentIndex(normal.engine.triangulation.NComponent obj) {
        return data.getComponentIndex(((NCORBAComponent)obj).data);
    }
    public long getNumberOfBoundaryComponents() {
        return data.getNumberOfBoundaryComponents();
    }
    public normal.engine.triangulation.NBoundaryComponent
            getBoundaryComponent(long index) {
        return NCORBABoundaryComponent.newWrapper(
            data.getBoundaryComponent((int)index));
    }
    public long getBoundaryComponentIndex(
            normal.engine.triangulation.NBoundaryComponent obj) {
        return data.getBoundaryComponentIndex(
            ((NCORBABoundaryComponent)obj).data);
    }
    public long getNumberOfVertices() {
        return data.getNumberOfVertices();
    }
    public normal.engine.triangulation.NVertex getVertex(long index) {
        return NCORBAVertex.newWrapper(data.getVertex((int)index));
    }
    public long getVertexIndex(normal.engine.triangulation.NVertex obj) {
        return data.getVertexIndex(((NCORBAVertex)obj).data);
    }
    public long getNumberOfEdges() {
        return data.getNumberOfEdges();
    }
    public normal.engine.triangulation.NEdge getEdge(long index) {
        return NCORBAEdge.newWrapper(data.getEdge((int)index));
    }
    public long getEdgeIndex(normal.engine.triangulation.NEdge obj) {
        return data.getEdgeIndex(((NCORBAEdge)obj).data);
    }
    public long getNumberOfFaces() {
        return data.getNumberOfFaces();
    }
    public normal.engine.triangulation.NFace getFace(long index) {
        return NCORBAFace.newWrapper(data.getFace((int)index));
    }
    public long getFaceIndex(normal.engine.triangulation.NFace obj) {
        return data.getFaceIndex(((NCORBAFace)obj).data);
    }

    public long getEulerCharacteristic() {
        return data.getEulerCharacteristic();
    }
    public normal.engine.algebra.NAbelianGroup getHomologyH1() {
        return NCORBAAbelianGroup.newWrapper(data.getHomologyH1());
    }
    public normal.engine.algebra.NAbelianGroup getHomologyH1Rel() {
        return NCORBAAbelianGroup.newWrapper(data.getHomologyH1Rel());
    }
    public normal.engine.algebra.NAbelianGroup getHomologyH1Bdry() {
        return NCORBAAbelianGroup.newWrapper(data.getHomologyH1Bdry());
    }
    public normal.engine.algebra.NAbelianGroup getHomologyH2() {
        return NCORBAAbelianGroup.newWrapper(data.getHomologyH2());
    }
    public long getHomologyH2Z2() {
        return data.getHomologyH2Z2();
    }

    public boolean isIdeal() {
        return data.isIdeal();
    }
    public boolean isValid() {
        return data.isValid();
    }
    public boolean isStandard() {
        return data.isStandard();
    }
    public boolean isClosed() {
        return data.isClosed();
    }
    public boolean isOrientable() {
        return data.isOrientable();
    }
    public boolean isConnected() {
        return data.isConnected();
    }
    public boolean hasBoundaryFaces() {
        return data.hasBoundaryFaces();
    }

    public boolean crushMaximalForest() {
        return data.crushMaximalForest();
    }
    public boolean intelligentSimplify() {
        return data.intelligentSimplify();
    }
	public boolean simplifyToLocalMinimum() {
		return simplifyToLocalMinimum(true);
	}
	public boolean simplifyToLocalMinimum(boolean perform) {
		return data.simplifyToLocalMinimum(perform);
	}

    public boolean threeTwoMove(normal.engine.triangulation.NEdge e) {
        return threeTwoMove(e, true, true);
    }
    public boolean threeTwoMove(normal.engine.triangulation.NEdge e,
            boolean check) {
        return threeTwoMove(e, check, true);
    }
    public boolean threeTwoMove(normal.engine.triangulation.NEdge e,
            boolean check, boolean perform) {
        return data.threeTwoMove(((NCORBAEdge)e).data, check, perform);
    }
    public boolean twoThreeMove(normal.engine.triangulation.NFace f) {
        return twoThreeMove(f, true, true);
    }
    public boolean twoThreeMove(normal.engine.triangulation.NFace f,
            boolean check) {
        return twoThreeMove(f, check, true);
    }
    public boolean twoThreeMove(normal.engine.triangulation.NFace f,
            boolean check, boolean perform) {
        return data.twoThreeMove(((NCORBAFace)f).data, check, perform);
    }
    public boolean twoZeroMove(normal.engine.triangulation.NEdge e) {
        return twoZeroMove(e, true, true);
    }
    public boolean twoZeroMove(normal.engine.triangulation.NEdge e,
            boolean check) {
        return twoZeroMove(e, check, true);
    }
    public boolean twoZeroMove(normal.engine.triangulation.NEdge e,
            boolean check, boolean perform) {
        return data.twoZeroMove(((NCORBAEdge)e).data, check, perform);
    }
    public boolean twoOneMove(normal.engine.triangulation.NEdge e, int end) {
        return twoOneMove(e, end, true, true);
    }
    public boolean twoOneMove(normal.engine.triangulation.NEdge e, int end,
            boolean check) {
        return twoOneMove(e, end, check, true);
    }
    public boolean twoOneMove(normal.engine.triangulation.NEdge e, int end,
            boolean check, boolean perform) {
        return data.twoOneMove(((NCORBAEdge)e).data, end, check, perform);
    }
    public boolean openBook(normal.engine.triangulation.NFace f) {
        return openBook(f, true, true);
    }
    public boolean openBook(normal.engine.triangulation.NFace f,
            boolean check) {
        return openBook(f, check, true);
    }
    public boolean openBook(normal.engine.triangulation.NFace f,
            boolean check, boolean perform) {
        return data.openBook(((NCORBAFace)f).data, check, perform);
    }
    public boolean shellBoundary(normal.engine.triangulation.NTetrahedron t) {
        return shellBoundary(t, true, true);
    }
    public boolean shellBoundary(normal.engine.triangulation.NTetrahedron t,
            boolean check) {
        return shellBoundary(t, check, true);
    }
    public boolean shellBoundary(normal.engine.triangulation.NTetrahedron t,
            boolean check, boolean perform) {
        return data.shellBoundary(((NCORBATetrahedron)t).data, check, perform);
    }

    public boolean idealToFinite() {
        return idealToFinite(false);
    }
    public boolean idealToFinite(boolean forceDivision) {
        return data.idealToFinite(forceDivision);
    }
    public void barycentricSubdivision() {
        data.barycentricSubdivision();
    }

    public normal.engine.triangulation.NTetrahedron
            insertLayeredSolidTorus(long cuts0, long cuts1) {
        return NCORBATetrahedron.newWrapper(
            data.insertLayeredSolidTorus((int)cuts0, (int)cuts1));
    }
    public void insertLensSpace(long p, long q) {
        data.insertLensSpace((int)p, (int)q);
    }
    public void insertTriangulation(
            normal.engine.triangulation.NTriangulation source) {
        data.insertTriangulation(((NCORBATriangulation)source).data);
    }

    public boolean isIsomorphicTo(
            normal.engine.triangulation.NTriangulation other) {
        return data.isIsomorphicTo(((NCORBATriangulation)other).data);
    }
}
