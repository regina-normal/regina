
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

package normal.packetui;

import java.awt.*;
import normal.Shell;
import normal.engine.packet.NPacket;
import normal.packetfilter.*;

/**
 * An interface through which a user can create a new packet.
 * This interface can be any component, although usually it will be a
 * container.
 * <p>
 * The <tt>PacketCreator</tt> does not itself need to be the interface
 * component, but it must be able to provide the component upon request.
 * This is done through the <tt>getCreator()</tt> method.
 * Often the <tt>PacketCreator</tt> will be the component itself, with the
 * <tt>getCreator()</tt> method simply returning <tt>this</tt>.
 * <p>
 * The interface will be placed within a <tt>NewPacketDialog</tt>, which
 * will provide a mechanism for selecting the parent packet beneath which
 * the new packet will be placed, as well as a label for the new packet.
 * The dialog will also provide <i>OK</i> and <i>Cancel</i> buttons to
 * begin or abort creation of the new packet.  The only interface required
 * from the <tt>PacketCreator</tt> is for entering details specific to the
 * type of packet being created.  In particular, the new packet should not
 * be created until asked to by <tt>makeNewPacket()</tt>.  The dialog
 * will provide empty padding around the edges of the contained
 * <tt>PacketCreator</tt>.
 * <p>
 * Different <tt>PacketCreator</tt> types will generally be used to create
 * different types of packet.
 * See the package overview for details on how to integrate a new type of
 * <tt>PacketCreator</tt> into the system.
 * <p>
 * If a user wishes to create a new packet, all you need do is call
 * <tt>NewPacketDialog.newPacket()</tt>; a dialog will be created, user
 * input will be collected and the new packet will be created.
 * See <tt>NewPacketDialog</tt> for further details.
 *
 * @see normal.packetui.NewPacketDialog
 * @see normal.packetui
 * @see normal.engine.packet.NPacket
 */
public interface PacketCreator {
    /**
     * Returns the component that forms the interface through which the user
     * can enter details for the new packet.  Details on what is required of
     * this interface are given in the class overview.
     *
     * @return the component providing the actual interface.
     * @see normal.packetui.PacketCreator
     */
    Component getCreator();
    
    /**
     * Returns the title to be given to the <tt>NewPacketDialog</tt> that
     * will contain this interface.
     *
     * @return a title for the packet creation dialog.
     * @see normal.packetui.NewPacketDialog
     */
    String getDialogTitle();
    
    /**
     * Returns a <tt>PacketFilter</tt> specifying which packets may be used
     * as a parent in the tree structure for the new packet.  This filter
     * will determine which potential parent packets are available to choose
     * from in the <tt>NewPacketDialog</tt> that contains this interface.
     *
     * @return a <tt>PacketFilter</tt> describing potential parents for the
     * new packet.
     * @see normal.packetfilter.PacketFilter
     */
    PacketFilter getPacketFilter();
    
    /**
     * Returns a suggested label for the new packet in the event that
     * the user is not creative enough to think up one of her/his own.
     *
     * @return a suggested packet label.
     */
    String getSuggestedPacketLabel();

    /**
     * Create the new packet.
     * This should first verify that all information entered is valid,
     * displaying error messages if necessary.
     * Then the new packet should be created and returned.
     * If the packet could not be created, the error(s) should be
     * reported to the user.
     * <p>
     * The packet may be inserted into the packet tree if desired,
     * although this is not necessary.
     * There is no need to label the packet or verify the user's
     * chosen label.  The <tt>NewPacketDialog</tt> containing this
     * interface will take care of all these tasks.
     *
     * @param shell the shell representing the entire program, from
     * which the calculation engine can be accessed.
     * @param parent the packet beneath which the new packet will be
     * placed in the packet tree structure.  This is provided for
     * reference only; whether or not the new packet is inserted is
     * up to the caller.
     * @param parentDialog the dialog to be used as parent for any
     * dialogs that may arise during this procedure.
     * @return the newly created packet, or <tt>null</tt> if an error
     * occurred.
     */
    NPacket makeNewPacket(Shell shell, NPacket parent, Dialog parentDialog);
}
