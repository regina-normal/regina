
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

package normal.engine.implementation.jni.subcomplex;

import normal.engine.implementation.jni.*;
import normal.engine.algebra.*;
import normal.engine.subcomplex.*;

public class NJNISFS extends JNIShareableObject
        implements NSFS {
    public NJNISFS(Sentry s) {
        super(s);
    }
    public NJNISFS() {
        super(Sentry.instance);
        newNSFS();
    }
    public NJNISFS(long orbitGenus, boolean orbitOrientable,
            long orbitPunctures) {
        super(Sentry.instance);
        newNSFS(orbitGenus, orbitOrientable, orbitPunctures);
    }
    public NJNISFS(NSFS cloneMe) {
        super(Sentry.instance);
        newNSFS(cloneMe);
    }
    private final native void newNSFS();
    private final native void newNSFS(long orbitGenus, boolean orbitOrientable,
            long orbitPunctures);
    private final native void newNSFS(NSFS cloneMe);
    
    public native long getOrbitGenus();
    public native boolean isOrbitOrientable();
    public native long getOrbitPunctures();
    public native long getFibreCount();
    public native long getAlpha(long which);
    public native long getBeta(long which);
    public native void insertFibre(long alpha, long beta);
    public native void reduce();
    public native NLensSpace isLensSpace();
    public native NAbelianGroup getHomologyH1();
}
