
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

import java.util.*;

/**
 * An event that occurs when a packet interface changes status between
 * having and not having unapplied changes.
 *
 * @see normal.packetui.PacketUI#hasChanges
 */
public class UIHasChangesEvent extends EventObject {
    /**
     * Did the packet interface in question have unapplied changes when
     * this event was created?
     */
    private boolean internalHasChanges;

    /**
     * Creates a new event.
     *
     * @param source the packet interface whose status has changed.
     */
    public UIHasChangesEvent(PacketUI source) {
        super(source);
        this.internalHasChanges = source.hasChanges();
    }

    /**
     * Returns whether or not the packet interface in question had
     * unapplied changes when this event was created.
     *
     * @return <tt>true</tt> if and only if the interface had unapplied
     * changes when this interface was created.
     */
    public boolean hasChanges() {
        return internalHasChanges;
    }
}

