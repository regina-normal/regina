
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
import normal.engine.implementation.corba.*;

public class NCORBABoundaryComponent extends CORBAShareableObject
        implements normal.engine.triangulation.NBoundaryComponent {
    public NBoundaryComponent data;
    public static final Class CORBAClass = NBoundaryComponent.class;
    public static final Class helperClass = NBoundaryComponentHelper.class;

    public NCORBABoundaryComponent(NBoundaryComponent data) {
        super(data);
        this.data = data;
    }

    public static NCORBABoundaryComponent newWrapper(
            NBoundaryComponent source) {
        return (source == null ? null : new NCORBABoundaryComponent(source));
    }

    public long getEulerCharacteristic() {
        return data.getEulerCharacteristic();
    }
    public boolean isIdeal() {
        return data.isIdeal();
    }
    public boolean isOrientable() {
        return data.isOrientable();
    }

    public long getNumberOfFaces() {
        return data.getNumberOfFaces();
    }
    public long getNumberOfEdges() {
        return data.getNumberOfEdges();
    }
    public long getNumberOfVertices() {
        return data.getNumberOfVertices();
    }

    public normal.engine.triangulation.NFace getFace(long index) {
        return NCORBAFace.newWrapper(data.getFace((int)index));
    }
    public normal.engine.triangulation.NEdge getEdge(long index) {
        return NCORBAEdge.newWrapper(data.getEdge((int)index));
    }
    public normal.engine.triangulation.NVertex getVertex(long index) {
        return NCORBAVertex.newWrapper(data.getVertex((int)index));
    }
}
