
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
import normal.engine.implementation.corba.triangulation.*;
import normal.engine.implementation.corba.*;

public class NCORBASurfaceSubset extends CORBAShareableObject
        implements normal.engine.surfaces.NSurfaceSubset, NCORBASurfaceSet {
    public NSurfaceSubset data;
    public static final Class CORBAClass = NSurfaceSubset.class;
    public static final Class helperClass = NSurfaceSubsetHelper.class;

    protected NCORBASurfaceSubset(NSurfaceSubset data) {
        super(data);
        this.data = data;
    }

    public static NCORBASurfaceSubset newWrapper(
            NSurfaceSubset source) {
        return (source == null ? null : new NCORBASurfaceSubset(source));
    }

    public NSurfaceSet getNSurfaceSet() {
        return data;
    }

    public int getFlavour() {
        return data.getFlavour();
    }
    public boolean allowsAlmostNormal() {
        return data.allowsAlmostNormal();
    }
    public boolean isEmbeddedOnly() {
        return data.isEmbeddedOnly();
    }
    public normal.engine.triangulation.NTriangulation getTriangulation() {
        return NCORBATriangulation.newWrapper(data.getTriangulation());
    }
    public long getNumberOfSurfaces() {
        return data.getNumberOfSurfaces();
    }
    public normal.engine.surfaces.NNormalSurface getSurface(long index) {
        return NCORBANormalSurface.newWrapper(data.getSurface((int)index));
    }

    public normal.engine.ShareableObject getShareableObject() {
        return this;
    }
}
