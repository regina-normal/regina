
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

package normal.engine.implementation.corba.angle;

import java.math.BigInteger;
import normal.engine.implementation.corba.Regina.Angle.*;
import normal.engine.implementation.corba.triangulation.*;
import normal.engine.implementation.corba.*;

public class NCORBAAngleStructure extends CORBAShareableObject
        implements normal.engine.angle.NAngleStructure {
    public NAngleStructure data;
    public static final Class CORBAClass = NAngleStructure.class;
    public static final Class helperClass = NAngleStructureHelper.class;

    protected NCORBAAngleStructure(NAngleStructure data) {
        super(data);
        this.data = data;
    }

    public static NCORBAAngleStructure newWrapper(NAngleStructure source) {
        return (source == null ? null : new NCORBAAngleStructure(source));
    }

    public BigInteger getAngleNum(long tetIndex, int edgePair) {
        return stringToLarge(data.getAngleNum((int)tetIndex, edgePair));
    }
    public BigInteger getAngleDen(long tetIndex, int edgePair) {
        return stringToLarge(data.getAngleDen((int)tetIndex, edgePair));
    }
    public normal.engine.triangulation.NTriangulation getTriangulation() {
        return NCORBATriangulation.newWrapper(data.getTriangulation());
    }
}
