
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

package normal.engine.packet;

import normal.engine.*;
import java.util.*;

public interface NPacket extends ShareableObject {
    public int getPacketType();
    public String getPacketName();
    public String getPacketLabel();
    public void setPacketLabel(String newLabel);
    public String getFullName();

    public NPacket getFirstTreeChild();
    public NPacket getLastTreeChild();
    public NPacket getPrevTreeSibling();
    public NPacket getNextTreeSibling();
    public NPacket getTreeParent();
    public NPacket getTreeMatriarch();
    public void insertChildFirst(NPacket child);
    public void insertChildLast(NPacket child);
    public void insertChildAfter(NPacket newChild, NPacket prevChild);
    public void makeOrphan();
    public void swapWithNextSibling();

    public NPacket nextTreePacket();
    public NPacket firstTreePacket(String type);
    public NPacket nextTreePacket(String type);
    public NPacket findPacketLabel(String label);
    public String makeUniqueLabel(String base);
    public boolean makeUniqueLabels(NPacket reference);

    public int levelsDownTo(NPacket descendant);
    public int levelsUpTo(NPacket ancestor);
    public boolean isGrandparentOf(NPacket descendant);
    public long getNumberOfChildren();
    public long getNumberOfDescendants();
    public long getTotalTreeSize();

    Map makeTreeDict();
    List makeChildList();

    public boolean dependsOnParent();
    public boolean isPacketEditable();
    public NPacket clonePacket();
    public NPacket clonePacket(boolean cloneDescendants);
    public NPacket clonePacket(boolean cloneDescendants, boolean end);
}
