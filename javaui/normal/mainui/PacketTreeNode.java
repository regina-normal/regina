
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
 * Nodes will display the packet label in the visual tree, followed by a
 * bracketed plus sign if the packet has any associated tags; this is all
 * arranged through routine <tt>getNodeLabel()</tt>.
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
        super(getNodeLabel(packet));
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
     * Determines if one of this node's children is a <tt>PacketTreeNode</tt>
     * wrapper for the given packet in the internal engine.
     *
     * @param child the packet we are searching for.
     * @return the index in this node's child list of the wrapper for the
     * given packet, or -1 if no wrapper was found.
     */
    public int findChildNodeIndex(NPacket child) {
        Enumeration e = children();
        int index = 0;
        while (e.hasMoreElements()) {
            if (((PacketTreeNode)e.nextElement()).getPacket().
                    sameObject(child))
                return index;
            index++;
        }
        return -1;
    }

    /**
     * Examines all descendants of this packet in the internal engine,
     * inserts new <tt>PacketTreeNode</tt> wrappers into
     * the given tree for all descendant packets that are not already
     * wrapped and removes nodes for packets that have been moved to
     * a different parent.
     * Note that the packet nodes are also reordered to match the
     * ordering in the underlying engine packet tree.
     * <p>
     * <b>Precondition:</b> This tree node already belongs to the given
     * tree.
     *
     * @param model the tree model in which the nodes will be modified.
     * @param newNodes a vector into which each new
     * <tt>PacketTreeNode</tt> wrapper that was inserted into the tree
     * will be placed.  If you do not wish for such an archive to be
     * made, you can simply pass <tt>null</tt> for this parameter.
     */
    public void verifyDescendants(DefaultTreeModel model, Vector newNodes) {
        NPacket child = packet.getFirstTreeChild();
        PacketTreeNode node = null;

        int index = 0;
        int count = getChildCount();
        while (child != null) {
            // child is the first packet that might not be properly
            // mirrored in the tree.
            // index is the index at which the node for child should be
            // placed.
            // count is the number of child nodes currently in the tree.
            if (index == count) {
                // There is no mirror node.
                node = new PacketTreeNode(child);
                if (newNodes != null)
                    newNodes.addElement(node);
                count++;
                model.insertNodeInto(node, this, index);
            } else if (child.sameObject(
                    (node = (PacketTreeNode)getChildAt(index)).getPacket())) {
                // The mirror node is already present.
            } else {
                // Hunt for a mirror node.
                node = findChildNode(child);
                if (node == null) {
                    // There is no mirror node.
                    node = new PacketTreeNode(child);
                    if (newNodes != null)
                        newNodes.addElement(node);
                    count++;
                } else {
                    // The mirror node is in the wrong place.
                    model.removeNodeFromParent(node);
                }
                model.insertNodeInto(node, this, index);
            }

            // At this point, node mirrors child.
            node.verifyDescendants(model, newNodes);

            // Advance to the next packet.
            child = child.getNextTreeSibling();
            index++;
        }

        // Remove extraneous nodes.
        while (count > index) {
            model.removeNodeFromParent((PacketTreeNode)getChildAt(count - 1));
            count--;
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
     * Returns the text label to give the node associated with the given
     * packet.
     *
     * See the <tt>PacketTreeNode</tt> class notes for details on how
     * this label is formed.
     *
     * @param packet the packet whose corresponding node label is requested.
     * @return the corresponding node label.
     */
    public static String getNodeLabel(NPacket packet) {
        if (packet.hasTags())
            return packet.getPacketLabel() + " (+)";
        else
            return packet.getPacketLabel();
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
