
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

import normal.engine.implementation.corba.Regina.Subcomplex.*;
import normal.engine.implementation.corba.*;
import normal.engine.triangulation.NPerm;

public class NCORBAAugTriSolidTorus extends CORBAShareableObject
        implements normal.engine.subcomplex.NAugTriSolidTorus {
    public NAugTriSolidTorus data;
    public static final Class CORBAClass = NAugTriSolidTorus.class;
    public static final Class helperClass = NAugTriSolidTorusHelper.class;

    public NCORBAAugTriSolidTorus(NAugTriSolidTorus data) {
        super(data);
        this.data = data;
    }

    public static NCORBAAugTriSolidTorus newWrapper(NAugTriSolidTorus source) {
        return (source == null ? null : new NCORBAAugTriSolidTorus(source));
    }

    public normal.engine.subcomplex.NAugTriSolidTorus cloneMe() {
        return NCORBAAugTriSolidTorus.newWrapper(data.cloneMe());
    }
    public long getAlpha(int index) {
        return data.getAlpha(index);
    }
    public long getBeta(int index) {
        return data.getBeta(index);
    }
    public long getExceptionalFibres() {
        return data.getExceptionalFibres();
    }
    public normal.engine.subcomplex.NTriSolidTorus getCore() {
        return NCORBATriSolidTorus.newWrapper(data.getCore());
    }
    public normal.engine.subcomplex.NLayeredSolidTorus getAugTorus(
            int annulus) {
        return NCORBALayeredSolidTorus.newWrapper(data.getAugTorus(annulus));
    }
    public NPerm getEdgeGroupRoles(int annulus) {
        return new NPerm(data.getEdgeGroupRoles(annulus));
    }
}
