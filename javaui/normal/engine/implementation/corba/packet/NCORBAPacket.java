
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

package normal.engine.implementation.corba.packet;

import normal.engine.implementation.corba.Regina.Packet.*;
import normal.engine.implementation.corba.Regina.Angle.*;
import normal.engine.implementation.corba.Regina.Surfaces.*;
import normal.engine.implementation.corba.Regina.Triangulation.*;
import normal.engine.implementation.corba.angle.*;
import normal.engine.implementation.corba.surfaces.*;
import normal.engine.implementation.corba.triangulation.*;
import normal.engine.implementation.corba.*;
import java.util.*;

public class NCORBAPacket extends CORBAShareableObject
        implements normal.engine.packet.NPacket {
    public NPacket data;
    public static final Class CORBAClass = NPacket.class;
    public static final Class helperClass = NPacketHelper.class;

    protected NCORBAPacket(NPacket data) {
        super(data);
        this.data = data;
    }

    public static NCORBAPacket newWrapper(NPacket source) {
        if (source == null)
            return null;
        switch (source.getPacketType()) {
            case normal.engine.packet.NContainer.packetType:
                return NCORBAContainer.newWrapper(
                    NContainerHelper.narrow(source));
            case normal.engine.surfaces.NNormalSurfaceList.packetType:
                return NCORBANormalSurfaceList.newWrapper(
                    NNormalSurfaceListHelper.narrow(source));
            case normal.engine.packet.NScript.packetType:
                return NCORBAScript.newWrapper(NScriptHelper.narrow(source));
            case normal.engine.packet.NText.packetType:
                return NCORBAText.newWrapper(NTextHelper.narrow(source));
            case normal.engine.triangulation.NTriangulation.packetType:
                return NCORBATriangulation.newWrapper(
                    NTriangulationHelper.narrow(source));
            case normal.engine.surfaces.NSurfaceFilter.packetType:
                return NCORBASurfaceFilter.newWrapper(
                    NSurfaceFilterHelper.narrow(source));
            case normal.engine.angle.NAngleStructureList.packetType:
                return NCORBAAngleStructureList.newWrapper(
                    NAngleStructureListHelper.narrow(source));
        }
        return new NCORBAPacket(source);
    }

    public int getPacketType() {
        return data.getPacketType();
    }
    public String getPacketName() {
        return data.getPacketName();
    }
    public String getPacketLabel() {
        return data.getPacketLabel();
    }
    public void setPacketLabel(String newLabel) {
        data.setPacketLabel(newLabel);
    }
    public String getFullName() {
        return data.getFullName();
    }

    public normal.engine.packet.NPacket getFirstTreeChild() {
        return NCORBAPacket.newWrapper(data.getFirstTreeChild());
    }
    public normal.engine.packet.NPacket getLastTreeChild() {
        return NCORBAPacket.newWrapper(data.getLastTreeChild());
    }
    public normal.engine.packet.NPacket getPrevTreeSibling() {
        return NCORBAPacket.newWrapper(data.getPrevTreeSibling());
    }
    public normal.engine.packet.NPacket getNextTreeSibling() {
        return NCORBAPacket.newWrapper(data.getNextTreeSibling());
    }
    public normal.engine.packet.NPacket getTreeParent() {
        return NCORBAPacket.newWrapper(data.getTreeParent());
    }
    public normal.engine.packet.NPacket getTreeMatriarch() {
        return NCORBAPacket.newWrapper(data.getTreeMatriarch());
    }
    public void insertChildFirst(normal.engine.packet.NPacket child) {
        data.insertChildFirst(((NCORBAPacket)child).data);
    }
    public void insertChildLast(normal.engine.packet.NPacket child) {
        data.insertChildLast(((NCORBAPacket)child).data);
    }
    public void insertChildAfter(normal.engine.packet.NPacket newChild,
            normal.engine.packet.NPacket prevChild) {
        data.insertChildAfter(((NCORBAPacket)newChild).data,
            ((NCORBAPacket)prevChild).data);
    }
    public void makeOrphan() {
        data.makeOrphan();
    }
    public void swapWithNextSibling() {
        data.swapWithNextSibling();
    }
    
    public normal.engine.packet.NPacket nextTreePacket() {
        return NCORBAPacket.newWrapper(data.nextTreePacket_());
    }
    public normal.engine.packet.NPacket firstTreePacket(String type) {
        return NCORBAPacket.newWrapper(data.firstTreePacket(type));
    }
    public normal.engine.packet.NPacket nextTreePacket(String type) {
        return NCORBAPacket.newWrapper(data.nextTreePacket_string(type));
    }
    public normal.engine.packet.NPacket findPacketLabel(String label) {
        return NCORBAPacket.newWrapper(data.findPacketLabel(label));
    }
    public String makeUniqueLabel(String base) {
        return data.makeUniqueLabel(base);
    }
    public boolean makeUniqueLabels(normal.engine.packet.NPacket ref) {
        return data.makeUniqueLabels(((NCORBAPacket)ref).data);
    }

    public int levelsDownTo(normal.engine.packet.NPacket descendant) {
        return data.levelsDownTo(((NCORBAPacket)descendant).data);
    }
    public int levelsUpTo(normal.engine.packet.NPacket ancestor) {
        return data.levelsUpTo(((NCORBAPacket)ancestor).data);
    }
    public boolean isGrandparentOf(normal.engine.packet.NPacket descendant) {
        return data.isGrandparentOf(((NCORBAPacket)descendant).data);
    }
    public long getNumberOfChildren() {
        return data.getNumberOfChildren();
    }
    public long getNumberOfDescendants() {
        return data.getNumberOfDescendants();
    }
    public long getTotalTreeSize() {
        return data.getTotalTreeSize();
    }

    public Map makeTreeDict() {
        HashMap ans = new HashMap();
        normal.engine.packet.NPacket packet = this;
        while (packet != null) {
            ans.put(packet.getPacketLabel(), packet);
            packet = packet.nextTreePacket();
        }
        return ans;
    }
    public List makeChildList() {
        Vector ans = new Vector();
        normal.engine.packet.NPacket child = getFirstTreeChild();
        while (child != null) {
            ans.add(child);
            child = child.getNextTreeSibling();
        }
        return ans;
    }

    public boolean dependsOnParent() {
        return data.dependsOnParent();
    }
    public boolean isPacketEditable() {
        return data.isPacketEditable();
    }
    public normal.engine.packet.NPacket clonePacket() {
        return clonePacket(false, true);
    }
    public normal.engine.packet.NPacket clonePacket(boolean cloneDescendants) {
        return clonePacket(cloneDescendants, true);
    }
    public normal.engine.packet.NPacket clonePacket(boolean cloneDescendants,
            boolean end) {
        return NCORBAPacket.newWrapper(data.clonePacket(
            cloneDescendants, end));
    }
}
