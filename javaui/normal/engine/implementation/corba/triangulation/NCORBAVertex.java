
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

public class NCORBAVertex extends CORBAShareableObject
        implements normal.engine.triangulation.NVertex {
    public NVertex data;
    public static final Class CORBAClass = NVertex.class;
    public static final Class helperClass = NVertexHelper.class;

    public NCORBAVertex(NVertex data) {
        super(data);
        this.data = data;
    }

    public static NCORBAVertex newWrapper(NVertex source) {
        return (source == null ? null : new NCORBAVertex(source));
    }

    public normal.engine.triangulation.NComponent getComponent() {
        return NCORBAComponent.newWrapper(data.getComponent());
    }
    public normal.engine.triangulation.NBoundaryComponent
            getBoundaryComponent() {
        return NCORBABoundaryComponent.newWrapper(data.getBoundaryComponent());
    }

    public int getLink() {
        return data.getLink();
    }
    public boolean isLinkClosed() {
        return data.isLinkClosed();
    }
    public boolean isIdeal() {
        return data.isIdeal();
    }
    public boolean isBoundary() {
        return data.isBoundary();
    }
    public boolean isStandard() {
        return data.isStandard();
    }
    public boolean isLinkOrientable() {
        return data.isLinkOrientable();
    }
    public long getLinkEulerCharacteristic() {
        return data.getLinkEulerCharacteristic();
    }

    public long getNumberOfEmbeddings() {
        return data.getNumberOfEmbeddings();
    }
    public normal.engine.triangulation.NVertexEmbedding
            getEmbedding(long index) {
        NTetrahedronHolder tet = new NTetrahedronHolder();
        org.omg.CORBA.IntHolder vertex = new org.omg.CORBA.IntHolder();
        data.getEmbedding(tet, vertex, (int)index);
        return new normal.engine.triangulation.NVertexEmbedding(
            NCORBATetrahedron.newWrapper(tet.value), vertex.value);
    }
}
