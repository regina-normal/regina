
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
import normal.engine.implementation.corba.*;

public class NCORBALayeredLensSpace extends CORBAShareableObject
        implements normal.engine.subcomplex.NLayeredLensSpace {
    public NLayeredLensSpace data;
    public static final Class CORBAClass = NLayeredLensSpace.class;
    public static final Class helperClass = NLayeredLensSpaceHelper.class;

    public NCORBALayeredLensSpace(NLayeredLensSpace data) {
        super(data);
        this.data = data;
    }

    public static NCORBALayeredLensSpace newWrapper(NLayeredLensSpace source) {
        return (source == null ? null : new NCORBALayeredLensSpace(source));
    }

    public normal.engine.subcomplex.NLayeredLensSpace cloneMe() {
        return NCORBALayeredLensSpace.newWrapper(data.cloneMe());
    }
    public long getP() {
        return data.getP();
    }
    public long getQ() {
        return data.getQ();
    }
    public normal.engine.subcomplex.NLayeredSolidTorus getTorus() {
        return NCORBALayeredSolidTorus.newWrapper(data.getTorus());
    }
    public int getMobiusBoundaryGroup() {
        return data.getMobiusBoundaryGroup();
    }
    public boolean isSnapped() {
        return data.isSnapped();
    }
    public boolean isTwisted() {
        return data.isTwisted();
    }
}
