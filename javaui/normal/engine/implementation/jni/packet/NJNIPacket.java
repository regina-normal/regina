
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

package normal.engine.implementation.jni.packet;

import normal.engine.implementation.jni.*;
import normal.engine.packet.*;
import java.util.*;

public class NJNIPacket extends JNIShareableObject implements NPacket {
    public NJNIPacket(Sentry s) {
        super(s);
    }

    public native int getPacketType();
    public native String getPacketName();
    public native String getPacketLabel();
    public native void setPacketLabel(String newLabel);
    public native String getFullName();

    public native NPacket getFirstTreeChild();
    public native NPacket getLastTreeChild();
    public native NPacket getPrevTreeSibling();
    public native NPacket getNextTreeSibling();
    public native NPacket getTreeParent();
    public native NPacket getTreeMatriarch();
    public native void insertChildFirst(NPacket child);
    public native void insertChildLast(NPacket child);
    public native void insertChildAfter(NPacket newChild, NPacket prevChild);
    public native void makeOrphan();
    public native void swapWithNextSibling();
    
    public native NPacket nextTreePacket();
    public native NPacket firstTreePacket(String type);
    public native NPacket nextTreePacket(String type);
    public native NPacket findPacketLabel(String label);
    public native String makeUniqueLabel(String base);

    public native int levelsDownTo(NPacket descendant);
    public native int levelsUpTo(NPacket ancestor);
    public native boolean isGrandparentOf(NPacket descendant);
    public native long totalTreeSize();

    public Map makeTreeDict() {
        HashMap ans = new HashMap();
        NPacket packet = this;
        while (packet != null) {
            ans.put(packet.getPacketLabel(), packet);
            packet = packet.nextTreePacket();
        }
        return ans;
    }
    public List makeChildList() {
        Vector ans = new Vector();
        NPacket child = getFirstTreeChild();
        while (child != null) {
            ans.add(child);
            child = child.getNextTreeSibling();
        }
        return ans;
    }

    public native boolean dependsOnParent();
    public native boolean isPacketEditable();
    public NPacket clonePacket() {
        return clonePacket(false, true);
    }
    public NPacket clonePacket(boolean cloneDescendants) {
        return clonePacket(cloneDescendants, true);
    }
    public native NPacket clonePacket(boolean cloneDescendants, boolean end);
}
