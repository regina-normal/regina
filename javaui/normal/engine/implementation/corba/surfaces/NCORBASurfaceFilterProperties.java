
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

package normal.engine.implementation.corba.surfaces;

import java.math.BigInteger;
import normal.engine.utilities.NBoolSet;
import normal.engine.implementation.corba.Regina.Surfaces.*;

public class NCORBASurfaceFilterProperties extends NCORBASurfaceFilter
        implements normal.engine.surfaces.NSurfaceFilterProperties {
    public NSurfaceFilterProperties data;
    public static final Class CORBAClass = NSurfaceFilterProperties.class;
    public static final Class helperClass =
        NSurfaceFilterPropertiesHelper.class;

    protected NCORBASurfaceFilterProperties(NSurfaceFilterProperties data) {
        super(data);
        this.data = data;
    }

    public static NCORBASurfaceFilterProperties newWrapper(
            NSurfaceFilterProperties source) {
        return (source == null ? null :
            new NCORBASurfaceFilterProperties(source));
    }

    public long getNumberOfECs() {
        return data.getNumberOfECs();
    }
    public BigInteger getEC(long index) {
        return stringToLarge(data.getEC((int)index));
    }
    public NBoolSet getOrientability() {
        return NBoolSet.fromByteCode(data.getOrientability());
    }
    public NBoolSet getCompactness() {
        return NBoolSet.fromByteCode(data.getCompactness());
    }
    public NBoolSet getRealBoundary() {
        return NBoolSet.fromByteCode(data.getRealBoundary());
    }

    public void addEC(BigInteger ec) {
        data.addEC(ec.toString());
    }
    public void removeEC(BigInteger ec) {
        data.removeEC(ec.toString());
    }
    public void removeAllECs() {
        data.removeAllECs();
    }
    public void setOrientability(NBoolSet value) {
        data.setOrientability(value.getByteCode());
    }
    public void setCompactness(NBoolSet value) {
        data.setCompactness(value.getByteCode());
    }
    public void setRealBoundary(NBoolSet value) {
        data.setRealBoundary(value.getByteCode());
    }
}
