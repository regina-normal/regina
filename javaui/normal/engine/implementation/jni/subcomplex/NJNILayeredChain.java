
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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
import normal.engine.triangulation.*;
import normal.engine.subcomplex.*;

public class NJNILayeredChain extends JNIShareableObject
        implements NLayeredChain {
    public NJNILayeredChain(Sentry s) {
        super(s);
    }
    public NJNILayeredChain(NTetrahedron tet, NPerm vertexRoles) {
        super(Sentry.instance);
        newNJNILayeredChain(tet, vertexRoles.getPermCode());
    }
    public NJNILayeredChain(NLayeredChain cloneMe) {
        super(Sentry.instance);
        newNJNILayeredChain(cloneMe);
    }
    public final native void newNJNILayeredChain(NTetrahedron tet,
        char vertexRoles);
    public final native void newNJNILayeredChain(NLayeredChain cloneMe);
    
    public native NTetrahedron getBottom();
    public native NTetrahedron getTop();
    public native long getIndex();
    public NPerm getBottomVertexRoles() {
        return new NPerm(_getBottomVertexRoles());
    }
    public native char _getBottomVertexRoles();
    public NPerm getTopVertexRoles() {
        return new NPerm(_getTopVertexRoles());
    }
    public native char _getTopVertexRoles();
    public native boolean extendAbove();
    public native boolean extendBelow();
    public native boolean extendMaximal();
    public native void reverse();
    public native void invert();
}
