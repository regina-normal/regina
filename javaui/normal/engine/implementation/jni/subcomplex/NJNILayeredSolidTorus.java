
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

package normal.engine.implementation.jni.subcomplex;

import normal.engine.implementation.jni.*;
import normal.engine.triangulation.*;
import normal.engine.subcomplex.*;

public class NJNILayeredSolidTorus extends JNIShareableObject
        implements NLayeredSolidTorus {
    public NJNILayeredSolidTorus(Sentry s) {
        super(s);
    }
    
    public native NLayeredSolidTorus cloneMe();
    public native long getNumberOfTetrahedra();
    public native NTetrahedron getBase();
    public native int getBaseEdge(int group, int index);
    public native int getBaseEdgeGroup(int edge);
    public native int getBaseFace(int index);
    public native NTetrahedron getTopLevel();
    public native long getMeridinalCuts(int group);
    public native int getTopEdge(int group, int index);
    public native int getTopEdgeGroup(int edge);
    public native int getTopFace(int index);
}
