
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

package normal.engine.implementation.corba.surfaces;

import normal.engine.implementation.corba.Regina.Surfaces.*;
import normal.engine.implementation.corba.packet.*;

public class NCORBASurfaceFilter extends NCORBAPacket
        implements normal.engine.surfaces.NSurfaceFilter {
    public NSurfaceFilter data;
    public static final Class CORBAClass = NSurfaceFilter.class;
    public static final Class helperClass = NSurfaceFilterHelper.class;

    protected NCORBASurfaceFilter(NSurfaceFilter data) {
        super(data);
        this.data = data;
    }

    public static NCORBASurfaceFilter newWrapper(NSurfaceFilter source) {
        if (source == null)
            return null;
        switch (source.getFilterID()) {
            case normal.engine.surfaces.NSurfaceFilterCombination.filterID:
                return NCORBASurfaceFilterCombination.newWrapper(
                    NSurfaceFilterCombinationHelper.narrow(source));
            case normal.engine.surfaces.NSurfaceFilterProperties.filterID:
                return NCORBASurfaceFilterProperties.newWrapper(
                    NSurfaceFilterPropertiesHelper.narrow(source));
        }
        return new NCORBASurfaceFilter(source);
    }

    public boolean accept(normal.engine.surfaces.NNormalSurface surface) {
        return data.accept(((NCORBANormalSurface)surface).data);
    }

    public int getFilterID() {
        return data.getFilterID();
    }
    public String getFilterName() {
        return data.getFilterName();
    }
}
