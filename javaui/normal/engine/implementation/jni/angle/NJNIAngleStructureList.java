
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

package normal.engine.implementation.jni.angle;

import normal.engine.implementation.jni.*;
import normal.engine.implementation.jni.packet.NJNIPacket;
import normal.engine.angle.*;
import normal.engine.triangulation.NTriangulation;

public class NJNIAngleStructureList extends NJNIPacket
        implements NAngleStructureList {
    public NJNIAngleStructureList(Sentry s) {
        super(s);
    }
    public NJNIAngleStructureList(NTriangulation owner) {
        super(Sentry.instance);
        newNAngleStructureList(owner);
    }
    private final native void newNAngleStructureList(NTriangulation owner);

    public native NTriangulation getTriangulation();
    public native long getNumberOfStructures();
    public native NAngleStructure getStructure(long index);

    public native boolean allowsStrict();
    public native boolean allowsTaut();
}
