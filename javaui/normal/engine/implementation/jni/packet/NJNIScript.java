
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

package normal.engine.implementation.jni.packet;

import normal.engine.implementation.jni.*;
import normal.engine.packet.*;

public class NJNIScript extends NJNIPacket implements NScript {
    public NJNIScript(Sentry s) {
        super(s);
    }
    public NJNIScript() {
        super(Sentry.instance);
        newNScript();
    }
    private final native void newNScript();

    public native long getNumberOfLines();
    public native String getLine(long index);

    public native void addFirst(String line);
    public native void addLast(String line);
    public native void insertAtPosition(String line, long index);
    public native void replaceAtPosition(String line, long index);
    public native void removeLineAt(long index);
    public native void removeAllLines();

    public native long getNumberOfVariables();
    public native String getVariableName(long index);
    public native String getVariableValue(long index);
    public native String getVariableValue(String name);

    public native boolean addVariable(String name, String value);
    public native void removeVariable(String name);
    public native void removeAllVariables();
}
