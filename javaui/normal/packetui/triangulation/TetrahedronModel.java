
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

package normal.packetui.triangulation;

import normal.engine.triangulation.*;

/**
 * A temporary model for a single tetrahedron in the triangulation.
 * The user may play with this model before committing changes to the engine.
 * See <tt>TriangulationTableModel</tt> for further details.
 *
 * @see normal.packetui.triangulation.TriangulationTableModel
 */
public class TetrahedronModel {
    /**
     * An optional English description of the tetrahedron.
     * @serial
     */
    public String description;
    
    /**
     * The tetrahedra adjacent to each face.
     * @serial
     */
    public TetrahedronModel[] adjacent = new TetrahedronModel[4];
    
    /**
     * The specific fashion in which each adjacent tetrahedron
     * is glued.
     * @serial
     */
    public NPerm[] gluings = new NPerm[4];
    
    /**
     * The position of this tetrahedron in the overall list of tetrahedra.
     * @serial
     */
    public int index;
    
    /**
     * Create a new model with no description and no adjacent tetrahedra.
     */
    public TetrahedronModel() {
        description = new String();
        for (int i=0; i<4; i++)
            adjacent[i] = null;
    }
    
    /**
     * Create a new model based on the given real tetrahedron in the engine.
     * The description and gluing descriptions will be copied, but the
     * list of adjacent tetrahedra will <i>not</i> be copied into the
     * <tt>adjacent</tt> array.  The index for this tetrahedron will be
     * set to -1.
     *
     * @param engineObject the tetrahedron in the engine upon which the new
     * model will be based.
     */
    public TetrahedronModel(NTetrahedron engineObject) {
        description = engineObject.getDescription();
        index = -1;
        for (int i=0; i<4; i++) {
            adjacent[i] = null;
            gluings[i] = engineObject.getAdjacentTetrahedronGluing(i);
        }
    }
}
