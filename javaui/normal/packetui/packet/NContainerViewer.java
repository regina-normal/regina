
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

package normal.packetui.packet;

import java.awt.*;
import javax.swing.*;
import normal.Images;
import normal.engine.packet.*;
import normal.packetui.*;
import btools.gui.component.*;

/**
 * An interface through which a user can view a container.
 * See <tt>PacketUI</tt> for more details.
 *
 * @see normal.packetui.PacketUI
 */
public class NContainerViewer extends DefaultPacketViewer {
    /**
     * The packet associated with this interface.
     */
    private NContainer packet;

	/**
	 * The child count.
	 */
	private JLabel nChildren;
	/**
	 * The descendant count.
	 */
	private JLabel nDescendants;
    
    /**
     * Create a new interface to work with a given packet.
     *
     * @param packet the packet to be associated with this interface.
     */
    public NContainerViewer(NPacket packet) {
        super();
        this.packet = (NContainer)packet;
        setLayout(new BorderLayout());

		JPanel infoPane = new JPanel();
		nChildren = new JLabel();
		nDescendants = new JLabel();
		refreshTreeInfo();
		infoPane.setLayout(new GridBagLayout());
		GridBagConstraints cLeft = new GridBagConstraints();
		GridBagConstraints cRight = new GridBagConstraints();
		cLeft.anchor = cLeft.EAST;
		cLeft.fill = cLeft.NONE;
		cLeft.gridx = 0;
		cRight.anchor = cRight.EAST;
		cRight.fill = cRight.NONE;
		cRight.gridx = 1;
		cRight.insets = new Insets(0, 5, 0, 5);
		infoPane.add(new JLabel("Immediate children:"), cLeft);
		infoPane.add(nChildren, cRight);
		infoPane.add(new JLabel("Total descendants:"), cLeft);
		infoPane.add(nDescendants, cRight);
        add(new PaddedPane(infoPane, 5), BorderLayout.NORTH);

        JLabel icon = new JLabel(Images.mainLargeIcon.image());
        icon.setBorder(BorderFactory.createLoweredBevelBorder());
        icon.setHorizontalAlignment(icon.CENTER);
        add(new PaddedPane(icon, 0, false), BorderLayout.CENTER);
    }

    public NPacket getPacket() {
        return packet;
    }

    public void reflectPacket() {
    }

	/**
	 * Refresh the tree statistics displayed in this viewer.
	 */
	private void refreshTreeInfo() {
		nChildren.setText(String.valueOf(packet.getNumberOfChildren()));
		nDescendants.setText(String.valueOf(packet.getNumberOfDescendants()));
	}

	public void subtreeWasInserted(NPacket subtree, PacketUI ui, Frame owner) {
		super.subtreeWasInserted(subtree, ui, owner);
		if (packet.isGrandparentOf(subtree))
			refreshTreeInfo();
	}

	public void subtreeWasDeleted(NPacket parent, PacketUI ui, Frame owner) {
		super.subtreeWasDeleted(parent, ui, owner);
		if (packet.isGrandparentOf(parent))
			refreshTreeInfo();
	}

	public void subtreeHasChanged(NPacket subtree, PacketUI ui, Frame owner) {
		super.subtreeHasChanged(subtree, ui, owner);
		if (packet.isGrandparentOf(subtree) || subtree.isGrandparentOf(packet))
			refreshTreeInfo();
	}
}
