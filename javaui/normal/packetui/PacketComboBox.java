
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

package normal.packetui;

import java.util.Enumeration;
import javax.swing.*;
import normal.engine.*;
import normal.engine.packet.NPacket;
import normal.mainui.PacketTreeNode;
import normal.packetfilter.*;
import org.gjt.btools.utilities.NullObject;

/**
 * A combo box through which a user can select a packet.
 * The packets presented in the combo box may be an entire packet tree or
 * subtree, or may be filtered using a packet filter.  The actual objects
 * stored in the combo box will be either <tt>PacketTreeNode</tt>
 * or <tt>NPacket</tt> objects.
 * <p>
 * This combo box may contain a <tt>NullObject</tt> (from package
 * <tt>btools.utilities</tt>); it will be displayed appropriately,
 * and if such an object is selected then
 * both routines <tt>getSelectedPacket()</tt> and
 * <tt>getSelectedNode()</tt> will return <tt>null</tt>.
 *
 * @see normal.packetfilter.PacketFilter
 * @see normal.mainui.PacketTreeNode
 * @see normal.engine.packet.NPacket
 */
public class PacketComboBox extends JComboBox {
    /**
     * The matriarch of the packet subtree represented by this combo box,
     * assuming nodes are stored.
     */
    private PacketTreeNode subtreeNode;

    /**
     * The matriarch of the packet subtree represented by this combo box,
     * assuming raw packets are stored.
     */
    private NPacket subtreePacket;

    /**
     * The filter describing which packets in the subtree to place in
     * this combo box.
     */
    private PacketFilter filter;

    /**
     * Should a null option be included in the combo box?
     */
    private boolean includeNull;

    /**
     * Creates a new packet combo box that stores raw packets.
     * All packets in the given subtree will be placed in the combo box.
     * The combo box will not contain a null option.
     * No initial selection will be made.
     *
     * @param subtree the matriarch of the subtree whose packets
     * will be placed in this combo box.
     */
    public PacketComboBox(NPacket subtree) {
        this(subtree, null, false, null);
    }

    /**
     * Creates a new packet combo box that stores packet tree nodes.
     * All packets in the given subtree will be placed in the combo box.
     * The combo box will not contain a null option.
     * No initial selection will be made.
     *
     * @param subtree the matriarch of the subtree whose packets
     * will be placed in this combo box.
     */
    public PacketComboBox(PacketTreeNode subtree) {
        this(subtree, null, false, null);
    }

    /**
     * Creates a new packet combo box that stores raw packets.
     * The combo box will not contain a null option.
     * No initial selection will be made.
     *
     * @param subtree the matriarch of the subtree whose packets
     * will be placed in this combo box.
     * @param filter a filter describing which packets in the given
     * subtree should be placed in this combo box, or <tt>null</tt> if no
     * filtering is required.
     */
    public PacketComboBox(NPacket subtree, PacketFilter filter) {
        this(subtree, filter, false, null);
    }

    /**
     * Creates a new packet combo box that stores packet tree nodes.
     * The combo box will not contain a null option.
     * No initial selection will be made.
     *
     * @param subtree the matriarch of the subtree whose packets
     * will be placed in this combo box.
     * @param filter a filter describing which packets in the given
     * subtree should be placed in this combo box, or <tt>null</tt> if no
     * filtering is required.
     */
    public PacketComboBox(PacketTreeNode subtree, PacketFilter filter) {
        this(subtree, filter, false, null);
    }

    /**
     * Creates a new packet combo box that stores raw packets.
     * No initial selection will be made.
     *
     * @param subtree the matriarch of the subtree whose packets
     * will be placed in this combo box.
     * @param filter a filter describing which packets in the given
     * subtree should be placed in this combo box, or <tt>null</tt> if no
     * filtering is required.
     * @param includeNull <tt>true</tt> if and only if a null option
     * should be included in this combo box.
     */
    public PacketComboBox(NPacket subtree, PacketFilter filter,
            boolean includeNull) {
        this(subtree, filter, includeNull, null);
    }

    /**
     * Creates a new packet combo box that stores packet tree nodes.
     * No initial selection will be made.
     *
     * @param subtree the matriarch of the subtree whose packets
     * will be placed in this combo box.
     * @param filter a filter describing which packets in the given
     * subtree should be placed in this combo box, or <tt>null</tt> if no
     * filtering is required.
     * @param includeNull <tt>true</tt> if and only if a null option
     * should be included in this combo box.
     */
    public PacketComboBox(PacketTreeNode subtree, PacketFilter filter,
            boolean includeNull) {
        this(subtree, filter, includeNull, null);
    }

    /**
     * Creates a new packet combo box that stores raw packets.
     *
     * @param subtree the matriarch of the subtree whose packets
     * will be placed in this combo box.
     * @param filter a filter describing which packets in the given
     * subtree should be placed in this combo box, or <tt>null</tt> if no
     * filtering is required.
     * @param includeNull <tt>true</tt> if and only if a null option
     * should be included in this combo box.
     * @param defaultSelection the packet in the given subtree that should
     * be initially selected in the combo box, or <tt>null</tt> if no initial
     * selection is to be made.
     */
    public PacketComboBox(NPacket subtree, PacketFilter filter,
            boolean includeNull, NPacket defaultSelection) {
        super();
        this.subtreePacket = subtree;
        this.subtreeNode = null;
        this.filter = filter;
        this.includeNull = includeNull;
        updateContents();

        setRenderer(new PacketListCellRenderer());
        if (defaultSelection != null)
            setSelectedItem(defaultSelection);
    }
    
    /**
     * Creates a new packet combo box that stores packet tree nodes.
     *
     * @param subtree the matriarch of the subtree whose packets
     * will be placed in this combo box.
     * @param filter a filter describing which packets in the given
     * subtree should be placed in this combo box, or <tt>null</tt> if no
     * filtering is required.
     * @param includeNull <tt>true</tt> if and only if a null option
     * should be included in this combo box.
     * @param defaultSelection the node in the given subtree that should
     * be initially selected in the combo box, or <tt>null</tt> if no initial
     * selection is to be made.
     */
    public PacketComboBox(PacketTreeNode subtree, PacketFilter filter,
            boolean includeNull, PacketTreeNode defaultSelection) {
        super(subtree.findDescendants(filter == null ?
            AllowAll.instance : filter));
        this.subtreeNode = subtree;
        this.subtreePacket = null;
        this.filter = filter;
        this.includeNull = includeNull;

        if (includeNull) {
            insertItemAt(NullObject.instance, 0);
            setSelectedIndex(0);
        }

        setRenderer(new PacketTreeNode.ListCellRenderer());
        if (defaultSelection != null)
            setSelectedItem(defaultSelection);
    }

    /**
     * Returns the node currently selected, or <tt>null</tt> if nothing
     * is currently selected.
     * <p>
     * If this combo box stores raw packets instead of packet tree nodes,
     * <tt>null</tt> will be returned.
     *
     * @return the node currently selected.
     */
    public PacketTreeNode getSelectedNode() {
        if (subtreeNode == null)
            return null;

        Object sel = getSelectedItem();
        if (sel instanceof NullObject)
            return null;
        else
            return (PacketTreeNode)sel;
    }

    /**
     * Returns the packet that is currently
     * selected, or <tt>null</tt> if no packet is selected.
     * This routine will work whether raw packets or packet tree nodes
     * are stored.
     *
     * @return the packet currently selected.
     */
    public NPacket getSelectedPacket() {
        Object sel = getSelectedItem();
        if (sel instanceof NullObject)
            return null;

        if (subtreeNode == null)
            return (NPacket)sel;

        PacketTreeNode node = (PacketTreeNode)sel;
        if (node == null)
            return null;
        else
            return node.getPacket();
    }

    /**
     * Updates the contents of the combo box.  This should be called when
     * the packet subtree being displayed is known to have changed
     * elsewhere.  The current selection in the combo box will be
     * selected again if possible.
     */
    public void updateContents() {
        Object selectedItem = getSelectedItem();
        removeAllItems();

        if (includeNull)
            addItem(NullObject.instance);

        if (subtreeNode != null) {
            Enumeration e = subtreeNode.findDescendants(filter == null ?
                AllowAll.instance : filter).elements();
            while (e.hasMoreElements())
                addItem(e.nextElement());
        } else {
            NPacket p = subtreePacket;
            while (p != null && subtreePacket.isGrandparentOf(p)) {
                if (filter == null || filter.acceptPacket(p))
                    addItem(p);
                p = p.nextTreePacket();
            }
        }

        if (selectedItem != null)
            setSelectedItem(selectedItem);
    }
}

