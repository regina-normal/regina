
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

package normal.engine.implementation.jni.surfaces;

import normal.engine.implementation.jni.*;
import normal.engine.ShareableObject;
import normal.engine.surfaces.*;
import normal.engine.triangulation.NTriangulation;

public class NJNISurfaceSubset extends JNIShareableObject
        implements NSurfaceSubset {
    public NJNISurfaceSubset(Sentry s) {
        super(s);
    }
    public NJNISurfaceSubset(NSurfaceSet set, NSurfaceFilter filter) {
        super(Sentry.instance);
        newNSurfaceSubset(set.getShareableObject(), filter);
    }
    private final native void newNSurfaceSubset(ShareableObject set,
            NSurfaceFilter filter);

    public native int getFlavour();
    public native boolean allowsAlmostNormal();
    public native boolean isEmbeddedOnly();
    public native NTriangulation getTriangulation();
    public native long getNumberOfSurfaces();
    public native NNormalSurface getSurface(long index);

    public ShareableObject getShareableObject() {
        return this;
    }
}
