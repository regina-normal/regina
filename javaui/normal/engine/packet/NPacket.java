
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

package normal.engine.packet;

import normal.engine.*;
import java.util.*;

public interface NPacket extends ShareableObject {
    int getPacketType();
    String getPacketTypeName();
    String getPacketLabel();
    void setPacketLabel(String newLabel);
    String getFullName();

    boolean hasTag(String tag);
    boolean hasTags();
    boolean addTag(String tag);
    boolean removeTag(String tag);
    void removeAllTags();
    Enumeration getTags();

    NPacket getFirstTreeChild();
    NPacket getLastTreeChild();
    NPacket getPrevTreeSibling();
    NPacket getNextTreeSibling();
    NPacket getTreeParent();
    NPacket getTreeMatriarch();
    void insertChildFirst(NPacket child);
    void insertChildLast(NPacket child);
    void insertChildAfter(NPacket newChild, NPacket prevChild);
    void makeOrphan();
    void swapWithNextSibling();

    NPacket nextTreePacket();
    NPacket firstTreePacket(String type);
    NPacket nextTreePacket(String type);
    NPacket findPacketLabel(String label);
    String makeUniqueLabel(String base);
    boolean makeUniqueLabels(NPacket reference);

    int levelsDownTo(NPacket descendant);
    int levelsUpTo(NPacket ancestor);
    boolean isGrandparentOf(NPacket descendant);
    long getNumberOfChildren();
    long getNumberOfDescendants();
    long getTotalTreeSize();

    Map makeTreeDict();
    List makeChildList();

    boolean dependsOnParent();
    boolean isPacketEditable();
    NPacket clonePacket();
    NPacket clonePacket(boolean cloneDescendants);
    NPacket clonePacket(boolean cloneDescendants, boolean end);
}
