
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

import javax.swing.*;
import normal.Shell;
import normal.engine.packet.NPacket;
import btools.image.StandardImage;

/**
 * Creates new interfaces in which the type of interface is determined
 * on the fly, based on some input data.
 */
public class PacketUIManager {
    /**
     * Icon to be used for unknown packet types.
     */
    public static ImageIcon unknownIcon =
		StandardImage.makeImageIcon("normal/images/packet/unknown.gif",
			PacketUIManager.class);
        
    /**
     * Icon to be used for container.
     */
    public static ImageIcon containerIcon =
		StandardImage.makeImageIcon("normal/images/packet/container.gif",
			PacketUIManager.class);
        
    /**
     * Icon to be used for text packets.
     */
    public static ImageIcon textIcon =
		StandardImage.makeImageIcon("normal/images/packet/text.gif",
			PacketUIManager.class);

    /**
     * Icon to be used for triangulations.
     */
    public static ImageIcon triangulationIcon =
		StandardImage.makeImageIcon("normal/images/packet/triangulation.gif",
			PacketUIManager.class);

    /**
     * Icon to be used for surface lists.
     */
    public static ImageIcon surfaceListIcon =
		StandardImage.makeImageIcon("normal/images/packet/surface.gif",
			PacketUIManager.class);

    /**
     * Icon to be used for scripts.
     */
    public static ImageIcon scriptIcon =
		StandardImage.makeImageIcon("normal/images/packet/script.gif",
			PacketUIManager.class);

    /**
     * Icon to be used for filters.
     */
    public static ImageIcon filterIcon =
		StandardImage.makeImageIcon("normal/images/packet/filter.gif",
			PacketUIManager.class);

    /**
     * Icon to be used for filters by properties.
     */
    public static ImageIcon filterPropertiesIcon =
		StandardImage.makeImageIcon("normal/images/packet/filterprop.gif",
			PacketUIManager.class);

    /**
     * Icon to be used for combination filters.
     */
    public static ImageIcon filterCombinationIcon =
		StandardImage.makeImageIcon("normal/images/packet/filtercomb.gif",
			PacketUIManager.class);

    /**
     * Return an icon with which to represent a specific packet.
     * The icon returned will be the most appropriate
     * for the given packet.
     *
     * @param packet the packet for which an icon will be found.
     * @return an icon most appropriate to the given packet.
     * If no suitable icon can be found, <tt>unknownIcon</tt> will
     * be returned.
     */
    public static ImageIcon packetIcon(NPacket packet) {
        // Available icons:
        if (packet instanceof normal.engine.packet.NContainer)
            return containerIcon;
        if (packet instanceof normal.engine.packet.NText)
            return textIcon;
        if (packet instanceof normal.engine.triangulation.NTriangulation)
            return triangulationIcon;
        if (packet instanceof normal.engine.surfaces.NNormalSurfaceList)
            return surfaceListIcon;
        if (packet instanceof normal.engine.packet.NScript)
            return scriptIcon;
        if (packet instanceof normal.engine.surfaces.NSurfaceFilterProperties)
            return filterPropertiesIcon;
        if (packet instanceof normal.engine.surfaces.NSurfaceFilterCombination)
            return filterCombinationIcon;

		// It's a surface filter but we don't know what kind.
        if (packet instanceof normal.engine.surfaces.NSurfaceFilter)
            return filterIcon;
        
        // No suitable icon was found.
        return unknownIcon;
    }

    /**
     * Create a new interface with which to interact with a specific packet.
     * The type of interface created will be the most appropriate
     * for the given packet.
     *
     * @param packet the packet for which a new interface will
     * be created.
     * @param shell the shell representing the entire program,
     * in case the new interface needs information from it.
     * @param mayEdit <tt>true</tt> if the interface returned is allowed
     * to edit the packet, or <tt>false</tt> if the interface should
     * only allow the packet to be viewed.
     * @return a new viewer that provides the most appropriate
     * interface to the given packet.  If no appropriate interface can
     * be created, an <tt>UnknownPacketViewer</tt> will be returned.
     * @see normal.packetui.PacketUI
     * @see normal.packetui.UnknownPacketViewer
     */
    public static PacketUI newPacketUI(NPacket packet, Shell shell,
            boolean mayEdit) {
        // Available interfaces:
        if (packet instanceof normal.engine.packet.NContainer)
            return new normal.packetui.packet.NContainerViewer(packet);
        if (packet instanceof normal.engine.packet.NText)
            return new normal.packetui.packet.NTextEditor(packet, mayEdit);
        if (packet instanceof normal.engine.packet.NScript)
            return new normal.packetui.packet.NScriptEditor(packet, shell,
                mayEdit);
        if (packet instanceof normal.engine.triangulation.NTriangulation)
            return new normal.packetui.triangulation.NTriangulationEditor(
                packet, shell, mayEdit);
        if (packet instanceof normal.engine.surfaces.NNormalSurfaceList)
            return new normal.packetui.surfaces.NNormalSurfaceListViewer(
                packet, shell);
        if (packet instanceof normal.engine.surfaces.NSurfaceFilter)
            return new normal.packetui.surfaces.NSurfaceFilterEditor(
                packet, shell, mayEdit);
        
        // No suitable viewer was found.
        return new UnknownPacketViewer(packet);
    }

    /**
     * Create a new <tt>PacketCreator</tt> corresponding to a specific
     * new packet command.  The command is as might appear on a
     * <i>New...</i> menu (for instance, <i>Packet</i> might be one
     * such command).
     *
     * @param command the command dictating the type of
     * <tt>PacketCreator</tt> to create.  Note that commands are case sensitive.
     * @return a new <tt>PacketCreator</tt> corresponding to the given command,
     * or null if the command was not recognised.
     * @see normal.packetui.PacketCreator
     */
    public static PacketCreator newPacketCreator(String command) {
        // Available PacketCreators:
        if (command.equals("Container"))
            return new normal.packetui.packet.NContainerCreator();
        if (command.equals("Text"))
            return new normal.packetui.packet.NTextCreator();
        if (command.equals("Triangulation"))
            return new normal.packetui.triangulation.NTriangulationCreator();
        if (command.equals("Census"))
            return new normal.packetui.census.NCensusCreator();
        if (command.equals("Normal Surface List"))
            return new normal.packetui.surfaces.NNormalSurfaceListCreator();
        if (command.equals("Surface Filter"))
            return new normal.packetui.surfaces.NSurfaceFilterCreator();
        if (command.equals("Script"))
            return new normal.packetui.packet.NScriptCreator();
            
        // No suitable PacketCreator could be found.
        return null;
    }
}
