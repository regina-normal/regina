
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

package normal.engine.surfaces;

import normal.engine.*;
import normal.engine.triangulation.*;
import java.math.BigInteger;

public interface NNormalSurface extends ShareableObject {
    public static final int vertexSplit[][] = {
        { -1, 0, 1, 2 },
        {  0,-1, 2, 1 },
        {  1, 2,-1, 0 },
        {  2, 1, 0,-1 }
    };
    public static final int vertexSplitMeeting[][][] = {
        { {-1,-1}, { 1, 2}, { 0, 2}, { 0, 1} },
        { { 1, 2}, {-1,-1}, { 0, 1}, { 0, 2} },
        { { 0, 2}, { 0, 1}, {-1,-1}, { 1, 2} },
        { { 0, 1}, { 0, 2}, { 1, 2}, {-1,-1} }
    };
    public static final int vertexSplitDefn[][] = {
        { 0, 1, 2, 3 },
        { 0, 2, 1, 3 },
        { 0, 3, 1, 2 }
    };
    public static final int vertexSplitPartner[][] = {
        { 1, 0, 3, 2 },
        { 2, 3, 0, 1 },
        { 3, 2, 1, 0 }
    };
    public static final String vertexSplitString[] = {
        "01/23", "02/13", "03/12"
    };
    public static final NPerm triDiscArcs[][] = {
        { new NPerm(0,1,2,3), new NPerm(0,2,3,1), new NPerm(0,3,1,2) },
        { new NPerm(1,0,3,2), new NPerm(1,3,2,0), new NPerm(1,2,0,3) },
        { new NPerm(2,3,0,1), new NPerm(2,0,1,3), new NPerm(2,1,3,0) },
        { new NPerm(3,2,1,0), new NPerm(3,1,0,2), new NPerm(3,0,2,1) }
    };
    public static final NPerm quadDiscArcs[][] = {
        { new NPerm(0,2,3,1), new NPerm(3,0,1,2), new NPerm(1,3,2,0), new NPerm(2,1,0,3) },
        { new NPerm(0,3,1,2), new NPerm(1,0,2,3), new NPerm(2,1,3,0), new NPerm(3,2,0,1) },
        { new NPerm(0,1,2,3), new NPerm(2,0,3,1), new NPerm(3,2,1,0), new NPerm(1,3,0,2) }
    };
    public static final NPerm octDiscArcs[][] = {
        { new NPerm(0,3,1,2), new NPerm(0,1,2,3), new NPerm(2,0,3,1), new NPerm(2,3,1,0),
          new NPerm(1,2,0,3), new NPerm(1,0,3,2), new NPerm(3,1,2,0), new NPerm(3,2,0,1) },
        { new NPerm(0,1,2,3), new NPerm(0,2,3,1), new NPerm(3,0,1,2), new NPerm(3,1,2,0),
          new NPerm(2,3,0,1), new NPerm(2,0,1,3), new NPerm(1,2,3,0), new NPerm(1,3,0,2) },
        { new NPerm(0,2,3,1), new NPerm(0,3,1,2), new NPerm(1,0,2,3), new NPerm(1,2,3,0),
          new NPerm(3,1,0,2), new NPerm(3,0,2,1), new NPerm(2,3,1,0), new NPerm(2,1,0,3) }
    };

    public BigInteger getTriangleCoord(long tetIndex, int vertex);
    public BigInteger getQuadCoord(long tetIndex, int quadType);
    public BigInteger getOctCoord(long tetIndex, int octType);
    public BigInteger getEdgeWeight(long edgeIndex);
    public BigInteger getFaceArcs(long faceIndex, int faceVertex);

    public int getNumberOfCoords();
    public NTriangulation getTriangulation();
    public String getName();
    public void setName(String newName);

    public void writeRawVector();

    public boolean isCompact();
    public BigInteger getEulerCharacteristic();
    public int isOrientable();
    public int isTwoSided();
    public int isConnected();
    public boolean hasRealBoundary();
    public boolean isVertexLinking();
    public boolean isSplitting();

    public NTriangulation cutAlong();
    public NTriangulation crush();
}

