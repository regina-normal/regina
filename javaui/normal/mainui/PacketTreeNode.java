
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

package normal.mainui;

import java.awt.Component;
import java.util.*;
import javax.swing.*;
import javax.swing.tree.*;
import normal.engine.packet.NPacket;
import normal.packetfilter.*;
import normal.packetui.PacketUIManager;

/**
 * Represents a node in a visual tree that corresponds to an internal
 * packet tree.  Each such node represents an individual packet.
 * <p>
 * Nodes will display the packet label in the visual tree, as
 * returned by <tt>NPacket.getPacketLabel()</tt>.
 * <p>
 * Any tree that uses <tt>PacketTreeNode</tt>s should <b>only</b> use
 * <tt>PacketTreeNode</tt>s.
 *
 * @see normal.engine.packet.NPacket
 */
public class PacketTreeNode extends DefaultMutableTreeNode {
    /**
     * The packet represented by this tree node.
     * @serial
     */
    private NPacket packet;

    /**
     * Create a new node to represent a given packet.
     *
     * @param packet the packet represented by this node.
     */
    public PacketTreeNode(NPacket packet) {
        super(packet.getPacketLabel());
        this.packet = packet;
    }
    
    /**
     * Returns the packet represented by this tree node.
     *
     * @return the packet representec by this node.
     */
    public NPacket getPacket() {
        return packet;
    }

    /**
     * Returns a string representation for this node.
     * The representation will be the full text name of this node,
     * which in turn is the full name of the corresponding packet.
     *
     * @return a string representation for this node.
     */        
    public String toString() {
        return (String)(getUserObject());
    }
    
    /**
     * Returns the full text name of this node.
     * This will be the full name of the corresponding packet.
     *
     * @return the full text name of this node.
     */
    public String getFullName() {
        return (String)(getUserObject());
    }
    
    /**
     * Returns a vector containing all descendants of this node whose
     * underlying packets satisfy the given filter.
     * This node itself may be included in the list.
     * The returned vector will be a vector of <tt>PacketTreeNode</tt>s.
     * @param filter a filter to determine which nodes will be included
     * in the returned list.
     * @return a list of nodes whose underlying packets satisfy the
     * given filter.
     */
    public Vector findDescendants(PacketFilter filter) {
        Enumeration e = preorderEnumeration();
        PacketTreeNode n;
        Vector ans = new Vector();
        while (e.hasMoreElements()) {
            n = (PacketTreeNode)(e.nextElement());
            if (filter.acceptPacket(n.getPacket()))
                ans.addElement(n);
        }
        return ans;
    }

    /**
     * Determines if one of this node's children is a <tt>PacketTreeNode</tt>
     * wrapper for the given packet in the internal engine.
     *
     * @param child the packet we are searching for.
     * @return the wrapper for the given packet, or <tt>null</tt>
     * if no wrapper was found.
     */
    public PacketTreeNode findChildNode(NPacket child) {
        Enumeration e = children();
        PacketTreeNode node;
        while (e.hasMoreElements()) {
            node = (PacketTreeNode)e.nextElement();
            if (node.getPacket().sameObject(child))
                return node;
        }
        return null;
    }

    /**
     * Examines all descendants of this packet in the internal engine
     * and inserts new <tt>PacketTreeNode</tt> wrappers into
     * the given tree for all descendant packets that are not already
     * wrapped as such.
	 * <p>
	 * As of Regina 2.1.1, this routine has been improved from quadratic
	 * time to linear time.
	 * <p>
     * <b>Precondition:</b> This tree node already belongs to the given
     * tree.
     *
     * @param model the tree model into which the new nodes will be inserted.
     * @param newNodes a vector into which each new
     * <tt>PacketTreeNode</tt> wrapper that was inserted into the tree
     * will be placed.  If you do not wish for such an archive to be
     * made, you can simply pass <tt>null</tt> for this parameter.
     */
    public void insertUnwrappedDescendants(DefaultTreeModel model,
            Vector newNodes) {
		// Make a list of all packets needing to be wrapped.
		Vector mustWrap = new Vector();
		NPacket child = packet.getFirstTreeChild();
		while (child != null) {
			if (findChildNode(child) == null)
				mustWrap.addElement(child);
			child = child.getNextTreeSibling();
		}
		
		// Wrap these packets.
		PacketTreeNode childNode;
		Enumeration e = mustWrap.elements();
		while (e.hasMoreElements()) {
			childNode = new PacketTreeNode((NPacket)e.nextElement());
			model.insertNodeInto(childNode, this, getChildCount());
			if (newNodes != null)
				newNodes.addElement(childNode);
			childNode.insertUnwrappedDescendants(model, newNodes);
		}
    }

    /**
     * Deletes any existing descendant nodes of this tree node and inserts a
     * fresh set of
     * <tt>PacketTreeNode</tt> wrappers for all descendant packets of
     * this packet in the internal engine.
     * A new tree node will be created for every descendant of this
     * packet in the internal engine, whether or not it had a tree node
     * originally.  This tree node itself will remain untouched.
     *
     * <b>Precondition:</b> This tree node already belongs to the given
     * tree.
     *
     * @param model the tree model into which the new nodes will be inserted.
     */
    public void rewrapAllDescendants(DefaultTreeModel model) {
        while (getChildCount() > 0)
            model.removeNodeFromParent((PacketTreeNode)getFirstChild());
        insertAllDescendants(model);
    }

    /**
     * Inserts a new <tt>PacketTreeNode</tt> wrapper into the given tree
     * for every descendant packet of this packet in the internal engine.
     * It is assumed that the node for this packet is already in the
     * tree.
     *
     * <b>Precondition:</b> This tree node already belongs to the given
     * tree and currently has no child nodes.
     *
     * @param model the tree model into which the new nodes will be inserted.
     */
    public void insertAllDescendants(DefaultTreeModel model) {
        NPacket child = packet.getFirstTreeChild();
        PacketTreeNode childNode;
        int index = 0;
        while (child != null) {
            childNode = new PacketTreeNode(child);
            model.insertNodeInto(childNode, this, index);
            childNode.insertAllDescendants(model);
            child = child.getNextTreeSibling();
            index++;
        }
    }

    /**
     * A renderer for packet tree nodes that can be used in lists.
     * Each packet will have a cute icon placed next to it according to
     * the packet type.
     *
     * <b>Precondition:</b> Only objects derived from
     * <tt>PacketTreeNode</tt> are rendered by this renderer.
     */
    public static class ListCellRenderer extends DefaultListCellRenderer {
        /**
         * Creates a new renderer.
         */
        public ListCellRenderer() {
        }

        /**
         * Returns the component used for rendering.
         *
         * @param list the list containing the cell to be rendered.
         * @param value the value of the cell to be rendered.
         * @param index the index of the cell to be rendered.
         * @param isSelected whether or not the cell in question is
         * selected.
         * @param cellHasFocus whether or not the cell in question has
         * the focus.
         * @return the component used for rendering.
         */
        public Component getListCellRendererComponent(JList list,
                 Object value, int index, boolean isSelected,
                 boolean cellHasFocus) {
            Component ans = super.getListCellRendererComponent(list,
                value, index, isSelected, cellHasFocus);
            if (value != null)
                ((JLabel)ans).setIcon(PacketUIManager.packetIcon(
                    ((PacketTreeNode)value).getPacket()));
            return ans;
        }
    }
}
