
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

package normal.engine.triangulation;

import normal.engine.packet.*;
import normal.engine.algebra.*;

public interface NTriangulation extends NPacket {
    int packetType = 3;

    long getNumberOfTetrahedra();
    NTetrahedron getTetrahedron(long index);
    long getTetrahedronIndex(NTetrahedron tetrahedron);
    
    void addTetrahedron(NTetrahedron newTet);
    void removeTetrahedronAt(long index);
    void removeTetrahedron(NTetrahedron tet);
    void removeAllTetrahedra();

    void gluingsHaveChanged();

    long getNumberOfComponents();
    NComponent getComponent(long index);
    long getComponentIndex(NComponent component);
    long getNumberOfBoundaryComponents();
    NBoundaryComponent getBoundaryComponent(long index);
    long getBoundaryComponentIndex(NBoundaryComponent bc);
    long getNumberOfVertices();
    NVertex getVertex(long index);
    long getVertexIndex(NVertex vertex);
    long getNumberOfEdges();
    NEdge getEdge(long index);
    long getEdgeIndex(NEdge edge);
    long getNumberOfFaces();
    NFace getFace(long index);
    long getFaceIndex(NFace face);

    long getEulerCharacteristic();
    NGroupPresentation getFundamentalGroup();
    NAbelianGroup getHomologyH1();
    NAbelianGroup getHomologyH1Rel();
    NAbelianGroup getHomologyH1Bdry();
    NAbelianGroup getHomologyH2();
    long getHomologyH2Z2();

    boolean isIdeal();
    boolean isValid();
    boolean isStandard();
    boolean isClosed();
    boolean isOrientable();
    boolean isConnected();
    boolean hasBoundaryFaces();

    boolean isZeroEfficient();
    boolean knowsZeroEfficient();
    boolean hasSplittingSurface();
    boolean knowsSplittingSurface();
     
    boolean crushMaximalForest();
    boolean intelligentSimplify();
    boolean simplifyToLocalMinimum();
    boolean simplifyToLocalMinimum(boolean perform);

    boolean threeTwoMove(NEdge e);
    boolean threeTwoMove(NEdge e, boolean check);
    boolean threeTwoMove(NEdge e, boolean check, boolean perform);
    boolean twoThreeMove(NFace f);
    boolean twoThreeMove(NFace f, boolean check);
    boolean twoThreeMove(NFace f, boolean check, boolean perform);
    boolean fourFourMove(NEdge e, int newAxis);
    boolean fourFourMove(NEdge e, int newAxis, boolean check);
    boolean fourFourMove(NEdge e, int newAxis, boolean check, boolean perform);
    boolean twoZeroMove(NEdge e);
    boolean twoZeroMove(NEdge e, boolean check);
    boolean twoZeroMove(NEdge e, boolean check, boolean perform);
    boolean twoZeroMove(NVertex v);
    boolean twoZeroMove(NVertex v, boolean check);
    boolean twoZeroMove(NVertex v, boolean check, boolean perform);
    boolean twoOneMove(NEdge e, int edgeEnd);
    boolean twoOneMove(NEdge e, int edgeEnd, boolean check);
    boolean twoOneMove(NEdge e, int edgeEnd, boolean check, boolean perform);
    boolean openBook(NFace f);
    boolean openBook(NFace f, boolean check);
    boolean openBook(NFace f, boolean check, boolean perform);
    boolean shellBoundary(NTetrahedron t);
    boolean shellBoundary(NTetrahedron t, boolean check);
    boolean shellBoundary(NTetrahedron t, boolean check, boolean perform);
    boolean collapseEdge(NEdge e);
    boolean collapseEdge(NEdge e, boolean check);
    boolean collapseEdge(NEdge e, boolean check, boolean perform);

    void makeDoubleCover();
    boolean idealToFinite();
    boolean idealToFinite(boolean forceDivision);
    void barycentricSubdivision();

    NTetrahedron insertLayeredSolidTorus(long cuts0, long cuts1);
    void insertLensSpace(long p, long q);
    void insertTriangulation(NTriangulation source);
    boolean insertRehydration(String dehydration);
    
    boolean isIsomorphicTo(NTriangulation other);
}
