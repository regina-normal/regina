
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

import normal.engine.implementation.corba.Regina.Angle.*;
import normal.engine.implementation.corba.packet.*;
import normal.engine.implementation.corba.triangulation.*;

public class NCORBAAngleStructureList extends NCORBAPacket implements
        normal.engine.angle.NAngleStructureList {
    public NAngleStructureList data;
    public static final Class CORBAClass = NAngleStructureList.class;
    public static final Class helperClass = NAngleStructureListHelper.class;

    protected NCORBAAngleStructureList(NAngleStructureList data) {
        super(data);
        this.data = data;
    }

    public static NCORBAAngleStructureList newWrapper(
            NAngleStructureList source) {
        return (source == null ? null : new NCORBAAngleStructureList(source));
    }

    public normal.engine.triangulation.NTriangulation getTriangulation() {
        return NCORBATriangulation.newWrapper(data.getTriangulation());
    }
    public long getNumberOfStructures() {
        return data.getNumberOfStructures();
    }
    public normal.engine.angle.NAngleStructure getStructure(long index) {
        return NCORBAAngleStructure.newWrapper(data.getStructure((int)index));
    }

    public boolean allowsStrict() {
        return data.allowsStrict();
    }
    public boolean allowsTaut() {
        return data.allowsTaut();
    }
}
