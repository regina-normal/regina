
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

package normal.engine.implementation.corba.surfaces;

import java.math.BigInteger;
import normal.engine.implementation.corba.Regina.Surfaces.*;
import normal.engine.implementation.corba.triangulation.*;
import normal.engine.implementation.corba.*;

public class NCORBANormalSurface extends CORBAShareableObject
        implements normal.engine.surfaces.NNormalSurface {
    public NNormalSurface data;
    public static final Class CORBAClass = NNormalSurface.class;
    public static final Class helperClass = NNormalSurfaceHelper.class;

    protected NCORBANormalSurface(NNormalSurface data) {
        super(data);
        this.data = data;
    }

    public static NCORBANormalSurface newWrapper(NNormalSurface source) {
        return (source == null ? null : new NCORBANormalSurface(source));
    }

    public BigInteger getTriangleCoord(long tetIndex, int vertex) {
        return stringToLarge(data.getTriangleCoord((int)tetIndex, vertex));
    }
    public BigInteger getQuadCoord(long tetIndex, int quadType) {
        return stringToLarge(data.getQuadCoord((int)tetIndex, quadType));
    }
    public BigInteger getOctCoord(long tetIndex, int octType) {
        return stringToLarge(data.getOctCoord((int)tetIndex, octType));
    }
    public BigInteger getEdgeWeight(long edgeIndex) {
        return stringToLarge(data.getEdgeWeight((int)edgeIndex));
    }
    public BigInteger getFaceArcs(long faceIndex, int faceVertex) {
        return stringToLarge(data.getFaceArcs((int)faceIndex, faceVertex));
    }

    public int getNumberOfCoords() {
        return data.getNumberOfCoords();
    }
    public normal.engine.triangulation.NTriangulation getTriangulation() {
        return NCORBATriangulation.newWrapper(data.getTriangulation());
    }

    public void writeRawVector() {
    }

    public boolean isCompact() {
        return data.isCompact();
    }
    public BigInteger getEulerCharacteristic() {
        return stringToLarge(data.getEulerCharacteristic());
    }
    public int isOrientable() {
        return data.isOrientable();
    }
    public boolean hasRealBoundary() {
        return data.hasRealBoundary();
    }
}
