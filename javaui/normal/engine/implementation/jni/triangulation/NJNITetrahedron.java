
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

package normal.engine.implementation.jni.triangulation;

import normal.engine.implementation.jni.*;
import normal.engine.triangulation.*;

public class NJNITetrahedron extends JNIShareableObject
        implements NTetrahedron {
    public NJNITetrahedron(Sentry s) {
        super(s);
    }
    public NJNITetrahedron() {
        super(Sentry.instance);
        newNTetrahedron();
    }
    public NJNITetrahedron(String desc) {
        super(Sentry.instance);
        newNTetrahedron(desc);
    }
    private final native void newNTetrahedron();
    private final native void newNTetrahedron(String desc);
    
    public native NTetrahedron getAdjacentTetrahedron(int face);
    public NPerm getAdjacentTetrahedronGluing(int face) {
        return new NPerm(_getAdjacentTetrahedronGluing(face));
    }
    public native char _getAdjacentTetrahedronGluing(int face);
    public native int getAdjacentFace(int face);
    public native boolean hasBoundary();

    public native NComponent getComponent();

    public native NVertex getVertex(int vertex);
    public native NEdge getEdge(int edge);
    public native NFace getFace(int face);

    public NPerm getEdgeMapping(int edge) {
        return new NPerm(_getEdgeMapping(edge));
    }
    public native char _getEdgeMapping(int edge);
    public NPerm getFaceMapping(int face) {
        return new NPerm(_getFaceMapping(face));
    }
    public native char _getFaceMapping(int face);

    public void joinTo(int myFace, NTetrahedron you, NPerm gluing) {
        _joinTo(myFace, you, gluing.getPermCode());
    }
    public native void _joinTo(int myFace, NTetrahedron you, char gluing);
    public native NTetrahedron unjoin(int myFace);
    public native void isolate();

    public native String getDescription();
    public native void setDescription(String description);
}
