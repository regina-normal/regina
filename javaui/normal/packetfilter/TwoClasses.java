
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
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

package normal.packetfilter;

import normal.engine.packet.NPacket;

/**
 * A <tt>PacketFilter</tt> that accepts either of two class of packets
 * or any subclass thereof.
 *
 * @see normal.packetfilter.PacketFilter
 */
public class TwoClasses implements PacketFilter {
    /**
     * The packet classes this filter will accept.
     */
    private Class type1, type2;

    /**
     * Creates a new filter accepting only packets of the given classes
     * or any subclass thereof.
     *
     * @param type1 the first packet class that this filter will accept.
     * @param type2 the second packet class that this filter will accept.
     */
    public TwoClasses(Class type1, Class type2) {
        this.type1 = type1;
        this.type2 = type2;
    }
    
    /**
     * Returns the first packet class that this filter will accept.
     *
     * @return the first packet class associated with this filter.
     */
    public Class getPacketClass1() {
        return type1;
    }
    /**
     * Returns the second packet class that this filter will accept.
     *
     * @return the second packet class associated with this filter.
     */
    public Class getPacketClass2() {
        return type2;
    }

    public boolean acceptPacket(NPacket packet) {
        if (type1.isInstance(packet))
            return true;
        return type2.isInstance(packet);
    }
}
