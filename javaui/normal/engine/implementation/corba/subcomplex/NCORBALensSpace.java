
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

public class NCORBALensSpace extends CORBAShareableObject
        implements normal.engine.subcomplex.NLensSpace {
    public NLensSpace data;
    public static final Class CORBAClass = NLensSpace.class;
    public static final Class helperClass = NLensSpaceHelper.class;

    public NCORBALensSpace(NLensSpace data) {
        super(data);
        this.data = data;
    }

    public static NCORBALensSpace newWrapper(NLensSpace source) {
        return (source == null ? null : new NCORBALensSpace(source));
    }

    public long getP() {
        return data.getP();
    }
    public long getQ() {
        return data.getQ();
    }
    public boolean isEquivalentTo(normal.engine.subcomplex.NLensSpace comp) {
        return data.isEquivalentTo(((NCORBALensSpace)comp).data);
    }
}
