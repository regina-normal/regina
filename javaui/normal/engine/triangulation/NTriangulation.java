
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

package normal.engine.triangulation;

import normal.engine.packet.*;
import normal.engine.algebra.*;

public interface NTriangulation extends NPacket {
    public static final int packetType = 3;

    public long getNumberOfTetrahedra();
    public NTetrahedron getTetrahedron(long index);
    public long getTetrahedronIndex(NTetrahedron tetrahedron);
    
    public void addTetrahedron(NTetrahedron newTet);
    public void removeTetrahedronAt(long index);
    public void removeTetrahedron(NTetrahedron tet);
    public void removeAllTetrahedra();

    public void gluingsHaveChanged();

    public long getNumberOfComponents();
    public NComponent getComponent(long index);
    public long getComponentIndex(NComponent component);
    public long getNumberOfBoundaryComponents();
    public NBoundaryComponent getBoundaryComponent(long index);
    public long getBoundaryComponentIndex(NBoundaryComponent bc);
    public long getNumberOfVertices();
    public NVertex getVertex(long index);
    public long getVertexIndex(NVertex vertex);
    public long getNumberOfEdges();
    public NEdge getEdge(long index);
    public long getEdgeIndex(NEdge edge);
    public long getNumberOfFaces();
    public NFace getFace(long index);
    public long getFaceIndex(NFace face);

    public long getEulerCharacteristic();
    public NGroupPresentation getFundamentalGroup();
    public NAbelianGroup getHomologyH1();
    public NAbelianGroup getHomologyH1Rel();
    public NAbelianGroup getHomologyH1Bdry();
    public NAbelianGroup getHomologyH2();
    public long getHomologyH2Z2();

    public boolean isIdeal();
    public boolean isValid();
    public boolean isStandard();
    public boolean isClosed();
    public boolean isOrientable();
    public boolean isConnected();
    public boolean hasBoundaryFaces();

    public boolean isZeroEfficient();
    public boolean hasCentralSurface();
     
    public boolean crushMaximalForest();
    public boolean intelligentSimplify();
    public boolean simplifyToLocalMinimum();
    public boolean simplifyToLocalMinimum(boolean perform);

    public boolean threeTwoMove(NEdge e);
    public boolean threeTwoMove(NEdge e, boolean check);
    public boolean threeTwoMove(NEdge e, boolean check, boolean perform);
    public boolean twoThreeMove(NFace f);
    public boolean twoThreeMove(NFace f, boolean check);
    public boolean twoThreeMove(NFace f, boolean check, boolean perform);
    public boolean fourFourMove(NEdge e, int newAxis);
    public boolean fourFourMove(NEdge e, int newAxis, boolean check);
    public boolean fourFourMove(NEdge e, int newAxis, boolean check,
        boolean perform);
    public boolean twoZeroMove(NEdge e);
    public boolean twoZeroMove(NEdge e, boolean check);
    public boolean twoZeroMove(NEdge e, boolean check, boolean perform);
    public boolean twoZeroMove(NVertex v);
    public boolean twoZeroMove(NVertex v, boolean check);
    public boolean twoZeroMove(NVertex v, boolean check, boolean perform);
    public boolean twoOneMove(NEdge e, int edgeEnd);
    public boolean twoOneMove(NEdge e, int edgeEnd, boolean check);
    public boolean twoOneMove(NEdge e, int edgeEnd, boolean check,
        boolean perform);
    public boolean openBook(NFace f);
    public boolean openBook(NFace f, boolean check);
    public boolean openBook(NFace f, boolean check, boolean perform);
    public boolean shellBoundary(NTetrahedron t);
    public boolean shellBoundary(NTetrahedron t, boolean check);
    public boolean shellBoundary(NTetrahedron t, boolean check,
        boolean perform);

    public boolean idealToFinite();
    public boolean idealToFinite(boolean forceDivision);
    public void barycentricSubdivision();

    public NTetrahedron insertLayeredSolidTorus(long cuts0, long cuts1);
    public void insertLensSpace(long p, long q);
    public void insertTriangulation(NTriangulation source);
    
    public boolean isIsomorphicTo(NTriangulation other);
}
