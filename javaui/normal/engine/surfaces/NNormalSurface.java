
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
    int vertexSplit[][] = {
        { -1, 0, 1, 2 },
        {  0,-1, 2, 1 },
        {  1, 2,-1, 0 },
        {  2, 1, 0,-1 }
    };
    int vertexSplitMeeting[][][] = {
        { {-1,-1}, { 1, 2}, { 0, 2}, { 0, 1} },
        { { 1, 2}, {-1,-1}, { 0, 1}, { 0, 2} },
        { { 0, 2}, { 0, 1}, {-1,-1}, { 1, 2} },
        { { 0, 1}, { 0, 2}, { 1, 2}, {-1,-1} }
    };
    int vertexSplitDefn[][] = {
        { 0, 1, 2, 3 },
        { 0, 2, 1, 3 },
        { 0, 3, 1, 2 }
    };
    int vertexSplitPartner[][] = {
        { 1, 0, 3, 2 },
        { 2, 3, 0, 1 },
        { 3, 2, 1, 0 }
    };
    String vertexSplitString[] = {
        "01/23", "02/13", "03/12"
    };
    NPerm triDiscArcs[][] = {
        { new NPerm(0,1,2,3), new NPerm(0,2,3,1), new NPerm(0,3,1,2) },
        { new NPerm(1,0,3,2), new NPerm(1,3,2,0), new NPerm(1,2,0,3) },
        { new NPerm(2,3,0,1), new NPerm(2,0,1,3), new NPerm(2,1,3,0) },
        { new NPerm(3,2,1,0), new NPerm(3,1,0,2), new NPerm(3,0,2,1) }
    };
    NPerm quadDiscArcs[][] = {
        { new NPerm(0,2,3,1), new NPerm(3,0,1,2), new NPerm(1,3,2,0), new NPerm(2,1,0,3) },
        { new NPerm(0,3,1,2), new NPerm(1,0,2,3), new NPerm(2,1,3,0), new NPerm(3,2,0,1) },
        { new NPerm(0,1,2,3), new NPerm(2,0,3,1), new NPerm(3,2,1,0), new NPerm(1,3,0,2) }
    };
    NPerm octDiscArcs[][] = {
        { new NPerm(0,3,1,2), new NPerm(0,1,2,3), new NPerm(2,0,3,1), new NPerm(2,3,1,0),
          new NPerm(1,2,0,3), new NPerm(1,0,3,2), new NPerm(3,1,2,0), new NPerm(3,2,0,1) },
        { new NPerm(0,1,2,3), new NPerm(0,2,3,1), new NPerm(3,0,1,2), new NPerm(3,1,2,0),
          new NPerm(2,3,0,1), new NPerm(2,0,1,3), new NPerm(1,2,3,0), new NPerm(1,3,0,2) },
        { new NPerm(0,2,3,1), new NPerm(0,3,1,2), new NPerm(1,0,2,3), new NPerm(1,2,3,0),
          new NPerm(3,1,0,2), new NPerm(3,0,2,1), new NPerm(2,3,1,0), new NPerm(2,1,0,3) }
    };

    BigInteger getTriangleCoord(long tetIndex, int vertex);
    BigInteger getQuadCoord(long tetIndex, int quadType);
    BigInteger getOctCoord(long tetIndex, int octType);
    BigInteger getEdgeWeight(long edgeIndex);
    BigInteger getFaceArcs(long faceIndex, int faceVertex);

    int getNumberOfCoords();
    NTriangulation getTriangulation();
    String getName();
    void setName(String newName);

    void writeRawVector();

    boolean isCompact();
    BigInteger getEulerCharacteristic();
    int isOrientable();
    int isTwoSided();
    int isConnected();
    boolean hasRealBoundary();
    boolean isVertexLinking();
    boolean isSplitting();

    NTriangulation cutAlong();
    NTriangulation crush();
}

