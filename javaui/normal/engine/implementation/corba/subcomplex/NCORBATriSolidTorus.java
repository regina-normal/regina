
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

package normal.engine.implementation.corba.subcomplex;

import normal.engine.triangulation.NPerm;
import normal.engine.implementation.corba.Regina.Subcomplex.*;
import normal.engine.implementation.corba.triangulation.*;
import normal.engine.implementation.corba.*;

public class NCORBATriSolidTorus extends CORBAShareableObject
        implements normal.engine.subcomplex.NTriSolidTorus {
    public NTriSolidTorus data;
    public static final Class CORBAClass = NTriSolidTorus.class;
    public static final Class helperClass = NTriSolidTorusHelper.class;

    public NCORBATriSolidTorus(NTriSolidTorus data) {
        super(data);
        this.data = data;
    }

    public static NCORBATriSolidTorus newWrapper(NTriSolidTorus source) {
        return (source == null ? null : new NCORBATriSolidTorus(source));
    }

    public normal.engine.subcomplex.NTriSolidTorus cloneMe() {
        return NCORBATriSolidTorus.newWrapper(data.cloneMe());
    }
    public normal.engine.triangulation.NTetrahedron getTetrahedron(int index) {
        return NCORBATetrahedron.newWrapper(data.getTetrahedron(index));
    }
    public NPerm getVertexRoles(int index) {
        return new NPerm(data.getVertexRoles(index));
    }
}
