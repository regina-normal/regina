
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

package normal.engine.implementation.jni.surfaces;

import java.math.BigInteger;
import normal.engine.implementation.jni.*;
import normal.engine.surfaces.*;
import normal.engine.triangulation.*;

public class NJNINormalSurface extends JNIShareableObject
        implements NNormalSurface {
    public NJNINormalSurface(Sentry s) {
        super(s);
    }

    public native BigInteger getTriangleCoord(long tetIndex, int vertex);
    public native BigInteger getQuadCoord(long tetIndex, int quadType);
    public native BigInteger getOctCoord(long tetIndex, int octType);
    public native BigInteger getEdgeWeight(long edgeIndex);
    public native BigInteger getFaceArcs(long faceIndex, int faceVertex);

    public native int getNumberOfCoords();
    public native NTriangulation getTriangulation();
    public native String getName();
    public native void setName(String newName);

    public native void writeRawVector();

    public native boolean isCompact();
    public native BigInteger getEulerCharacteristic();
    public native int isOrientable();
    public native int isTwoSided();
    public native int isConnected();
    public native boolean hasRealBoundary();
    public native boolean isVertexLinking();
    public native NVertex isVertexLink();
    public native NEdge[] isThinEdgeLink();
    public native boolean isSplitting();

    public native NTriangulation cutAlong();
    public native NTriangulation crush();
    public native boolean knownCanCrush();
}
