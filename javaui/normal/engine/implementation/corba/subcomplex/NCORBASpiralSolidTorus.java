
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

package normal.engine.implementation.corba.subcomplex;

import normal.engine.triangulation.NPerm;
import normal.engine.implementation.corba.Regina.Subcomplex.*;
import normal.engine.implementation.corba.triangulation.*;
import normal.engine.implementation.corba.*;

public class NCORBASpiralSolidTorus extends CORBAShareableObject
        implements normal.engine.subcomplex.NSpiralSolidTorus {
    public NSpiralSolidTorus data;
    public static final Class CORBAClass = NSpiralSolidTorus.class;
    public static final Class helperClass = NSpiralSolidTorusHelper.class;

    public NCORBASpiralSolidTorus(NSpiralSolidTorus data) {
        super(data);
        this.data = data;
    }

    public static NCORBASpiralSolidTorus newWrapper(NSpiralSolidTorus source) {
        return (source == null ? null : new NCORBASpiralSolidTorus(source));
    }

    public normal.engine.subcomplex.NSpiralSolidTorus cloneMe() {
        return NCORBASpiralSolidTorus.newWrapper(data.cloneMe());
    }
    public long getNumberOfTetrahedra() {
        return data.getNumberOfTetrahedra();
    }
    public normal.engine.triangulation.NTetrahedron getTetrahedron(long index) {
        return NCORBATetrahedron.newWrapper(data.getTetrahedron((int)index));
    }
    public NPerm getVertexRoles(long index) {
        return new NPerm(data.getVertexRoles((int)index));
    }
    public void reverse() {
        data.reverse();
    }
    public void cycle(long k) {
        data.cycle((int)k);
    }
    public boolean makeCanonical(normal.engine.triangulation.NTriangulation t) {
        return data.makeCanonical(((NCORBATriangulation)t).data);
    }
    public boolean isCanonical(normal.engine.triangulation.NTriangulation t) {
        return data.isCanonical(((NCORBATriangulation)t).data);
    }
}
