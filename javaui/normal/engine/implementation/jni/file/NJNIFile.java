
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

package normal.engine.implementation.jni.file;

import normal.engine.implementation.jni.*;
import normal.engine.file.*;
import normal.engine.packet.*;

public class NJNIFile extends JNIShareableObject implements NFile {
    public NJNIFile(Sentry s) {
        super(s);
    }
    public NJNIFile() {
        super(Sentry.instance);
        newNFile();
    }
    private final native void newNFile();
    
    public native boolean open(String fileName, int openMode);
    public native void close();

    public native int getOpenMode();
    public native int getMajorVersion();
    public native int getMinorVersion();
    public native boolean versionEarlierThan(int major, int minor);
    
    public native void writePacketTree(NPacket packet);
    public native NPacket readPacketTree();
    public native NPacket readPacketTree(NPacket parent);
}
