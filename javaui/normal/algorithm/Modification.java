
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

package normal.algorithm;

import java.awt.event.KeyEvent;
import java.util.*;
import javax.swing.*;
import normal.Shell;
import normal.engine.packet.*;
import normal.mainui.*;
import normal.packetfilter.*;

/**
 * An algorithm that modifies a packet of a particular class.
 * <p>
 * To fill a menu with modifications, the static method <tt>makeMenu()</tt>
 * should be extended to include all desired modifications and should be
 * called from the appropriate visual component initialisation routine.
 * <p>
 * See the <tt>Modification</tt> constructor for details regarding how to
 * specify a specific modification.
 *
 * @see #Modification
 */
public abstract class Modification extends Algorithm {
    /**
     * Creates a new modification with the given specifications.
     * This modification object will also be registered as an action
     * listener for itself.
     *
     * @param shell the shell representing the entire program.
     * @param menuLabel the text to display in the menu item
     * corresponding to this modification.
     * @param packetPostLabel the text to add to the end of a packet
     * label if the packet is cloned to perform this modification; this
     * text should not begin with any spaces.
     * @param packetClass the subclass of <tt>NPacket</tt> upon which we
     * are allowed to perform this modification.
     * @see normal.engine.packet.NPacket
     */
    public Modification(Shell shell, String menuLabel,
            String packetPostLabel, Class packetClass) {
        super(shell, menuLabel, packetPostLabel,
            new SingleClass(packetClass), true);
    }

    /**
     * Fills the given menu and vector with modifications.
     * Each modification placed in the given menu structure will also be placed
     * in the given vector.
     *
     * @param shell the shell representing the entire program.
     * @param menu the menu to fill.
     * @param vector a vector in which to place each modification that is
     * placed in the menu structure; this vector will <i>not</i> be
     * emptied before we begin.
     */
    public static void makeMenu(Shell shell, JMenu menu, Vector vector) {
        JMenu triangulation = new JMenu("Triangulation");
		triangulation.setMnemonic(KeyEvent.VK_T);
        (new Modification(shell, "Intelligent Simplify", "simplified",
            normal.engine.triangulation.NTriangulation.class) {
                public boolean perform(NPacket packet) {
                    return
                        ((normal.engine.triangulation.NTriangulation)packet).
                        intelligentSimplify();
                }
            }).insert(triangulation, vector);
        (new Modification(shell, "Convert from Ideal to Finite", "made finite",
            normal.engine.triangulation.NTriangulation.class) {
                public boolean perform(NPacket packet) {
                    return ((normal.engine.triangulation.NTriangulation)packet).
                        idealToFinite();
                }
            }).insert(triangulation, vector);
        (new Modification(shell, "Barycentric Subdivision", "subdivided",
            normal.engine.triangulation.NTriangulation.class) {
                public boolean perform(NPacket packet) {
                    ((normal.engine.triangulation.NTriangulation)packet).
                        barycentricSubdivision();
                    return true;
                }
            }).insert(triangulation, vector);

        menu.add(triangulation);
    }
}

