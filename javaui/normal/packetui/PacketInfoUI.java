
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

import java.awt.*;

/**
 * An interface through which a user can interact with a specific set of
 * details for a specific packet.
 * Several <tt>PacketInfoUI</tt> interfaces may be open for the same
 * packet simultaneously within a single <tt>PacketUI</tt>.
 * An example is when a larger <tt>PacketUI</tt>
 * uses a tabbed pane, each tab containing a single <tt>PacketInfoUI</tt>.
 * <p>
 * Only one <tt>PacketInfoUI</tt> within a single <tt>PacketUI</tt>
 * should be editable at any given time.  Preferably, when one such
 * sub-interface is being edited, the others will blank out their
 * details and disable editing accordingly until the initial editing has
 * been cancelled or committed to the engine.
 * <p>
 * The parent <tt>PacketUI</tt> should listen to each sub-interface for
 * <tt>UIHasChangesEvent</tt> events to know when editing has begun on
 * an interface.  It should notify all other sub-interfaces that editing
 * has begun by calling <tt>editingElsewhere()</tt>, and should notify
 * them that editing has finished by calling <tt>reflectPacket()</tt>.
 * <p>
 * Note however that a sub-interface should only be adjusted or
 * refreshed when it becomes visible, to save unnecessary calculation.
 * Thus such calls to <tt>editingElsewhere()</tt> or
 * <tt>reflectPacket()</tt> should be delayed until the appropriate
 * sub-interface becomes visible.
 */
public interface PacketInfoUI extends PacketUI {
    /**
     * Update this sub-interface to reflect the fact that the corresponding
     * packet is being edited elsewhere within the same overall
     * <tt>PacketUI</tt> interface.
     */
    void editingElsewhere();
}

