
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

package normal.engine.implementation.corba.triangulation;

import normal.engine.implementation.corba.Regina.Triangulation.*;
import normal.engine.implementation.corba.*;
import normal.engine.triangulation.NPerm;

public class NCORBAFace extends CORBAShareableObject
        implements normal.engine.triangulation.NFace {
    public NFace data;
    public static final Class CORBAClass = NFace.class;
    public static final Class helperClass = NFaceHelper.class;

    public NCORBAFace(NFace data) {
        super(data);
        this.data = data;
    }

    public static NCORBAFace newWrapper(NFace source) {
        return (source == null ? null : new NCORBAFace(source));
    }

    public normal.engine.triangulation.NComponent getComponent() {
        return NCORBAComponent.newWrapper(data.getComponent());
    }
    public normal.engine.triangulation.NBoundaryComponent
            getBoundaryComponent() {
        return NCORBABoundaryComponent.newWrapper(data.getBoundaryComponent());
    }
    public normal.engine.triangulation.NVertex getVertex(int vertex) {
        return NCORBAVertex.newWrapper(data.getVertex(vertex));
    }
    public normal.engine.triangulation.NEdge getEdge(int edge) {
        return NCORBAEdge.newWrapper(data.getEdge(edge));
    }
    public NPerm getEdgeMapping(int edge) {
        return new NPerm(data.getEdgeMapping(edge));
    }

    public boolean isBoundary() {
        return data.isBoundary();
    }

    public int getNumberOfEmbeddings() {
        return data.getNumberOfEmbeddings();
    }
    public normal.engine.triangulation.NFaceEmbedding
            getEmbedding(int index) {
        NTetrahedronHolder tet = new NTetrahedronHolder();
        org.omg.CORBA.IntHolder face = new org.omg.CORBA.IntHolder();
        data.getEmbedding(tet, face, index);
        return new normal.engine.triangulation.NFaceEmbedding(
            NCORBATetrahedron.newWrapper(tet.value), face.value);
    }
}
