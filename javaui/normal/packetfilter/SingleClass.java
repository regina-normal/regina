
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

package normal.packetfilter;

import normal.engine.packet.NPacket;

/**
 * A <tt>PacketFilter</tt> that only accepts packets of a given class or
 * a subclass thereof.
 *
 * @see normal.packetfilter.PacketFilter
 */
public class SingleClass implements PacketFilter {
    /**
     * The packet class this filter will accept.
     */
    private Class type;

    /**
     * Creates a new filter accepting only packets of the given class or
     * a subclass thereof.
     *
     * @param type the packet class that this filter will accept.
     */
    public SingleClass(Class type) {
        this.type = type;
    }
    
    /**
     * Returns the packet class that this filter will accept.
     *    
     * @return the packet class associated with this filter.
     */
    public Class getPacketClass() {
        return type;
    }

    public boolean acceptPacket(NPacket packet) {
        return type.isInstance(packet);
    }
}
