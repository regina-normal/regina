
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
import normal.engine.implementation.corba.*;

public class NCORBASFS extends CORBAShareableObject
        implements normal.engine.subcomplex.NSFS {
    public NSFS data;
    public static final Class CORBAClass = NSFS.class;
    public static final Class helperClass = NSFSHelper.class;

    public NCORBASFS(NSFS data) {
        super(data);
        this.data = data;
    }

    public static NCORBASFS newWrapper(NSFS source) {
        return (source == null ? null : new NCORBASFS(source));
    }

    public long getOrbitGenus() {
        return data.getOrbitGenus();
    }
    public boolean isOrbitOrientable() {
        return data.isOrbitOrientable();
    }
    public long getOrbitPunctures() {
        return data.getOrbitPunctures();
    }
    public long getFibreCount() {
        return data.getFibreCount();
    }
    public long getAlpha(long which) {
        return data.getAlpha((int)which);
    }
    public long getBeta(long which) {
        return data.getBeta((int)which);
    }
    public void insertFibre(long alpha, long beta) {
        data.insertFibre((int)alpha, (int)beta);
    }
    public void reduce() {
        data.reduce();
    }
    public normal.engine.subcomplex.NLensSpace isLensSpace() {
        return NCORBALensSpace.newWrapper(data.isLensSpace());
    }
}
