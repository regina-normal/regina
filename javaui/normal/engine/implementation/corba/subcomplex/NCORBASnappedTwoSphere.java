
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

import normal.engine.implementation.corba.Regina.Subcomplex.*;
import normal.engine.implementation.corba.triangulation.*;
import normal.engine.implementation.corba.*;

public class NCORBASnappedTwoSphere extends CORBAShareableObject
        implements normal.engine.subcomplex.NSnappedTwoSphere {
    public NSnappedTwoSphere data;
    public static final Class CORBAClass = NSnappedTwoSphere.class;
    public static final Class helperClass = NSnappedTwoSphereHelper.class;

    public NCORBASnappedTwoSphere(NSnappedTwoSphere data) {
        super(data);
        this.data = data;
    }

    public static NCORBASnappedTwoSphere newWrapper(NSnappedTwoSphere source) {
        return (source == null ? null : new NCORBASnappedTwoSphere(source));
    }

    public normal.engine.subcomplex.NSnappedTwoSphere cloneMe() {
        return NCORBASnappedTwoSphere.newWrapper(data.cloneMe());
    }
    public normal.engine.subcomplex.NSnappedBall getSnappedBall(int index) {
        return NCORBASnappedBall.newWrapper(data.getSnappedBall(index));
    }
    public void reduceTriangulation() {
        data.reduceTriangulation();
    }
    public normal.engine.triangulation.NTriangulation getReducedTriangulation(
            normal.engine.triangulation.NTriangulation original) {
        return NCORBATriangulation.newWrapper(data.getReducedTriangulation(
            ((NCORBATriangulation)original).data));
    }
}
