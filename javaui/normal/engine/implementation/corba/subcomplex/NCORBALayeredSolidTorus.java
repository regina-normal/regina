
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

package normal.engine.implementation.corba.subcomplex;

import normal.engine.implementation.corba.Regina.Subcomplex.*;
import normal.engine.implementation.corba.triangulation.*;
import normal.engine.implementation.corba.*;

public class NCORBALayeredSolidTorus extends CORBAShareableObject
        implements normal.engine.subcomplex.NLayeredSolidTorus {
    public NLayeredSolidTorus data;
    public static final Class CORBAClass = NLayeredSolidTorus.class;
    public static final Class helperClass = NLayeredSolidTorusHelper.class;

    public NCORBALayeredSolidTorus(NLayeredSolidTorus data) {
        super(data);
        this.data = data;
    }

    public static NCORBALayeredSolidTorus newWrapper(NLayeredSolidTorus source) {
        return (source == null ? null : new NCORBALayeredSolidTorus(source));
    }

    public normal.engine.subcomplex.NLayeredSolidTorus cloneMe() {
        return NCORBALayeredSolidTorus.newWrapper(data.cloneMe());
    }
    public long getNumberOfTetrahedra() {
        return data.getNumberOfTetrahedra();
    }
    public normal.engine.triangulation.NTetrahedron getBase() {
        return NCORBATetrahedron.newWrapper(data.getBase());
    }
    public int getBaseEdge(int group, int index) {
        return data.getBaseEdge(group, index);
    }
    public int getBaseEdgeGroup(int edge) {
        return data.getBaseEdgeGroup(edge);
    }
    public int getBaseFace(int index) {
        return data.getBaseFace(index);
    }
    public normal.engine.triangulation.NTetrahedron getTopLevel() {
        return NCORBATetrahedron.newWrapper(data.getTopLevel());
    }
    public long getMeridinalCuts(int group) {
        return data.getMeridinalCuts(group);
    }
    public int getTopEdge(int group, int index) {
        return data.getTopEdge(group, index);
    }
    public int getTopEdgeGroup(int edge) {
        return data.getTopEdgeGroup(edge);
    }
    public int getTopFace(int index) {
        return data.getTopFace(index);
    }
}
