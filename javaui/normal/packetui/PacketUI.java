
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

package normal.packetui;

import java.awt.*;
import javax.swing.text.JTextComponent;
import normal.engine.Engine;
import normal.engine.packet.NPacket;

/**
 * An interface through which a user can interact with a specific packet.
 * This interface can be any component, although usually it will be a
 * container.
 * <p>
 * The <tt>PacketUI</tt> does not itself need to be the interface
 * component, but it must be able to provide the component upon request.
 * This is done through the <tt>getInterface()</tt> method.  Often the
 * <tt>PacketUI</tt> will be the component itself, with the
 * <tt>getInterface()</tt> method simply returning <tt>this</tt>.
 * <p>
 * Each <tt>PacketUI</tt> object works with a single packet.  Thus it is
 * okay to have many different <tt>PacketUI</tt> objects in existence,
 * some of the same class, but each referring to a different packet.
 * <p>
 * Different packet types will generally use different <tt>PacketUI</tt>
 * classes for interaction.
 * See the package overview for details on how to integrate a new type of
 * <tt>PacketUI</tt> into the system.
 * <p>
 * Some interfaces will be capable of editing packets.  In such cases,
 * changes are made first in the interface and then applied to the
 * underlying packet in the engine when asked.  No buttons are required
 * for applying changes; we can expect these to be supplied from
 * elsewhere.
 * <p>
 * If an interface changes status between having and not having changes
 * that are as yet unapplied to the engine, it is expected to fire a
 * <tt>UIHasChangesEvent</tt> to all listeners added through
 * <tt>addUIHasChangesListener()</tt>.
 *
 * @see #addUIHasChangesListener
 * @see normal.packetui
 * @see normal.engine.packet.NPacket
 */
public interface PacketUI {
    /**
     * Adds the given listener to the list of listeners that will
     * receive <tt>UIHasChangesEvent</tt> events fired by this
     * interface.
     * <p>
     * <b>Precondition:</b> The given listener is not already in this
     * list.
     *
     * @param l the listener to add.
     */
    public void addUIHasChangesListener(UIHasChangesListener l);

    /**
     * Removes the given listener from the list of listeners that will
     * receive <tt>UIHasChangesEvent</tt> events fired by this
     * interface.
     * <p>
     * <b>Precondition:</b> The given listener is currently in this list.
     *
     * @param l the listener to add.
     */
    public void removeUIHasChangesListener(UIHasChangesListener l);

    /**
     * Returns the individual packet for which we are providing an interface.
     *
     * @return the packet associated with this interface.
     */
    public NPacket getPacket();
    
    /**
     * Returns the component that forms the interface through which the user
     * can interact with the packet.
     *
     * @return the component providing the actual interface.
     */
    public Component getInterface();

    /**
     * Returns the primary text component in this interface.
     * This component will be hooked up to edit menus and the like.
     *
     * @return the primary text component, or <tt>null</tt> if there is
     * no such component.
     */
    public JTextComponent getPrimaryTextComponent();
    
    /**
     * Update the component to reflect the underlying packet in the engine
     * in its current state.  This can be used for instance when the
     * underlying packet has been changed elsewhere.
     */
    public void reflectPacket();

    /**
     * Is this interface capable of editing the packet?
     *
     * @return <tt>true</tt> if this interface is capable of editing the
     * packet, or <tt>false</tt> if it can only view the packet.
     */
    public boolean isEditor();

    /**
     * Have changes been made in the interface that have yet to be
     * applied to the underlying packet in the engine?
     *
     * @return <tt>true</tt> if and only if there are unapplied changes.
     */
    public boolean hasChanges();

    /**
     * Take any changes made in the interface and apply them to the
     * underlying packet.
     *
     * @param engine the calculation engine through which any new engine
     * objects should be created.
     */
    public void applyChanges(Engine engine);

    /**
     * This will be called whenever any packet's contents have changed.
     * Often the UI will not be interested unless the packet changed
     * was the specific packet that this UI is reflecting.
     *
     * @param packet the packet whose contents have changed.
     * @param ui the interface that caused the change; this may be
     * <tt>null</tt>.
     * @param owner the frame that owns this interface; this can be used for
     * message boxes and the like.
     */
    public void packetWasChanged(NPacket packet, PacketUI ui, Frame owner);

    /**
     * This will be called whenever any packet has been renamed.
     * <p>
     * Note that the renaming of packets might
     * cause <tt>subtreeHasChanged()</tt> to be called instead of this
     * routine if specific details are not available.
     *
     * @param packet the packet that has been renamed.
     * @param ui the interface that caused the renaming; this may be
     * <tt>null</tt>.
     * @param owner the frame that owns this interface; this can be used for
     * message boxes and the like.
     */
    public void packetWasRenamed(NPacket packet, PacketUI ui, Frame owner);

    /**
     * This will be called whenever a packet subtree is about to
     * be deleted.  Note that this routine
     * will only be called using the root packet of that subtree and
     * will not be called again using its descendants.
     * <p>
     * The only exception is when the packet in this interface belongs
     * to the subtree to be deleted, in which case this routine will not
     * be called at all and this interface will instead be forcibly
     * closed through other means.
     * <p>
     * This routine is called <i>before</i> the subtree is deleted;
     * note that <tt>subtreeWasDeleted()</tt> will also be called
     * <i>after</i> the subtree has been deleted.
     * <p>
     * <b>Precondition:</b> The packet in this interface does not belong
     * to the subtree about to be deleted.
     *
     * @param subtree the root of the packet subtree that is to be
     * deleted.
     * @param ui the interface that caused the deletion; this may be
     * <tt>null</tt>.
     * @param owner the frame that owns this interface; this can be used for
     * message boxes and the like.
     *
     * @see #subtreeWasDeleted
     */
    public void subtreeToBeDeleted(NPacket subtree, PacketUI ui, Frame owner);

    /**
     * This will be called after a packet subtree has been deleted.
     * Note that this routine
     * will only be called for the root packet of that subtree and
     * will not be called again for its descendants.
     * <p>
     * Since the subtree has now been destroyed, it cannot be
     * accessed.  To access the members of the deleted subtree before
     * they are deleted, use <tt>subtreeToBeDeleted()</tt> instead.
     * <p>
     * This routine is called <i>after</i> the subtree has been deleted;
     * note that <tt>subtreeToBeDeleted()</tt> will also be called
     * <i>before</i> the subtree is deleted.
     * <p>
     * <b>Precondition:</b> The packet in this interface does not belong
     * to the subtree that was deleted.
     *
     * @param parent the parent packet immediately above the subtree that
     * was deleted; note that this parent packet still exists.
     * @param ui the interface that caused the deletion; this may be
     * <tt>null</tt>.
     * @param owner the frame that owns this interface; this can be used for
     * message boxes and the like.
     *
     * @see #subtreeToBeDeleted
     */
    public void subtreeWasDeleted(NPacket parent, PacketUI ui, Frame owner);

    /**
     * This will be called whenever a packet subtree has been inserted
     * into the overall tree.  Note that this routine will only be
     * called using the root packet of that subtree and will not be
     * called again using its descendants.
     * <p>
     * Note that the insertion of packets might
     * cause <tt>subtreeHasChanged()</tt> to be called instead of this
     * routine if specific details are not available.
     *
     * @param subtree the root of the packet subtree that has been
     * inserted.
     * @param ui the interface that caused the insertion; this may be
     * <tt>null</tt>.
     * @param owner the frame that owns this interface; this can be used for
     * message boxes and the like.
     */
    public void subtreeWasInserted(NPacket subtree, PacketUI ui, Frame owner);

    /**
     * This will be called whenever packets have been inserted and/or
     * renamed and/or moved in the given subtree but more specific details
     * about the alterations are not available.  Multiple
     * packets may have been operated upon.
     * <p>
     * Note that this routine does not cater for packet deletions.
     * <p>
     * <b>Precondition:</b> The root of the given subtree was not one of
     * the newly inserted packets.
     *
     * @param subtree the root of the packet subtree in which the
     * operations have taken place.
     * @param ui the interface that caused the operations; this may be
     * <tt>null</tt>.
     * @param owner the frame that owns this interface; this can be used for
     * message boxes and the like.
     */
    public void subtreeHasChanged(NPacket subtree, PacketUI ui, Frame owner);

    /**
     * This will be called when the interface is about to be destroyed.
     */
    public void cleanUp();
}
