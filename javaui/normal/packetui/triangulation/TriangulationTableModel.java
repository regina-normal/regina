
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

package normal.packetui.triangulation;

import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import normal.engine.*;
import normal.engine.triangulation.*;

/**
 * An internal data model for a table containing triangulation data.
 * This mirrors a triangulation in the engine, allowing
 * the user to play with the model triangulation first
 * before committing changes to the engine.
 * <p>
 * This class is used by <tt>NTriangulationEditor</tt>.
 *
 * @see normal.packetui.triangulation.NTriangulationEditor
 */
public class TriangulationTableModel extends AbstractTableModel {
    /**
     * Contains models of all tetrahedra in the triangulation.
     * @serial
     */        
    private Vector tetrahedra = new Vector();

    /**
     * Is the user allowed to modify this model triangulation?
     * @serial
     */
    private boolean isEditable;
    
    /**
     * Create a new model based on the given underlying triangulation in
     * the engine.  All details of the given triangulation will be
     * reflected in the model.
     *
     * @param packet the underlying packet in the engine upon which to base
     * the new model.
     * @param isEditable whether or not the user may modify the model
     * triangulation.
     */            
    public TriangulationTableModel(NTriangulation packet, boolean isEditable) {
        super();
        this.isEditable = isEditable;
        readFromEngine(packet);
    }

    /**
     * Returns the string name of the requested column.
     * @param col the requested column.
     * @return the string name of the requested column.
     */        
    public String getColumnName(int col) {
        if (col == 0)
            return "Tetrahedron";
        else
            return "Face " + NPerm.faceDescription(4-col);
    }
        
    /**
     * Returns the number of columns in the table.
     * @return the number of columns.
     */        
    public int getColumnCount() {
        return 5;
    }
        
    /**
     * Returns the number of rows in the table.
     * @return the number of rows.
     */        
    public int getRowCount() {
        return tetrahedra.size();
    }
        
    /**
     * Determines whether the given cell may be edited.
     * @param row the row of the given cell.
     * @param col the column of the given cell.
     * @return whether or not the given cell may be edited.
     */        
    public boolean isCellEditable(int row, int col) {
        return isEditable;
    }

    /**
     * Returns the requested value in the table.
     * This will always return the tetrahedron described by the
     * particular row; the individual cell renderers will know how to
     * display that same tetrahedron differently for each column.
     * @param row the row of the requested value.
     * @param col the column of the requested value.
     * @return the value in the given row and column.
     */        
    public Object getValueAt(int row, int col) {
        return tetrahedra.elementAt(row);
    }

    /**
     * Sets the value of the given cell in the table.
     * The value passed will be ignored, since the actual altering of
     * the model tetrahedra is done by the individual cell editors.
     * Instead this routine will merely fire appropriate change
     * notifications.
     * @param value the new value for the given cell.
     * @param row the row of the given cell.
     * @param col the column of the given cell.
     */
    public void setValueAt(Object value, int row, int col) {
        if (col == 0)
            // Merely the tetrahedron description has changed.
            fireTableCellUpdated(row, col);
        else
            // The gluings have changed; this may affect other tetrahedra!
            fireTableDataChanged();
    }
     
    /**
     * Ensure each model tetrahedron has stored its correct index
     * in the triangulation's list of tetrahedra.
     */
    private void reindexTetrahedra() {
        int total = tetrahedra.size();
        for (int i=0; i<total; i++)
            ((TetrahedronModel)tetrahedra.elementAt(i)).index = i;
    }

    /**
     * Return the entire list of tetrahedra.
     *
     * @return the list of tetrahedra in this model.
     */
    public Vector getTetrahedra() {
        return tetrahedra;
    }
    
    /**
     * Add a new tetrahedron to the model triangulation.
     * The tetrahedron will be added to the end of the list.
     */
    public void addTetrahedron() {
        int total = tetrahedra.size();
        TetrahedronModel newTet = new TetrahedronModel();
        newTet.index = total;
        tetrahedra.addElement(newTet);
        fireTableRowsInserted(total, total);
    }

    /**
     * Delete the specified tetrahedra from the model triangulation.
     * Any adjacent tetrahedra will be unglued from these tetrahedra
     * before they are deleted.
     *
     * @param rowsToDelete a list of tetrahedron numbers (the same as row
     * numbers) to be deleted.  These may be sent in any order, but will
     * be sorted during this routine.
     */
    public void deleteTetrahedra(int[] rowsToDelete) {
        int tet, face;
        TetrahedronModel model;
        
        org.gjt.btools.utilities.Sorter.sort(rowsToDelete);
        for (int i = rowsToDelete.length - 1; i >= 0; i--) {
            tet = rowsToDelete[i];
            // Isolate the tetrahedron before removing it.
            model = (TetrahedronModel)tetrahedra.elementAt(tet);
            for (face=0; face<4; face++)
                if (model.adjacent[face] != null)
                    model.adjacent[face].adjacent[model.
                        gluings[face].imageOf(face)] = null;
            // Remove the tetrahedron.
            tetrahedra.removeElementAt(tet);
        }
        reindexTetrahedra();
        fireTableRowsDeleted(rowsToDelete[0],
            rowsToDelete[rowsToDelete.length-1]);
    }

    /**
     * Update the model to reflect the contents of the underlying
     * triangulation in the engine.
     *
     * @param packet the underlying triangulation packet in the engine
     * upon which to base this model.
     */
    public void readFromEngine(NTriangulation packet) {
        tetrahedra.removeAllElements();

        // Insert new tetrahedra into the model and correctly set the
        // indices for these new model tetrahedra.
        int total = (int)packet.getNumberOfTetrahedra();
        int i;
        int face;
        TetrahedronModel model;
        NTetrahedron[] realTetrahedra = new NTetrahedron[total];
        for (i=0; i<total; i++) {
            realTetrahedra[i] = packet.getTetrahedron(i);
            model = new TetrahedronModel(realTetrahedra[i]);
            model.index = i;
            tetrahedra.addElement(model);
        }

        // Glue the model tetrahedra together.
        NTetrahedron adj;
        for (i=0; i<total; i++) {
            for (face=0; face<4; face++) {
                adj = realTetrahedra[i].getAdjacentTetrahedron(face);
                if (adj != null)
                    ((TetrahedronModel)tetrahedra.elementAt(i)).
                        adjacent[face] = (TetrahedronModel)tetrahedra.
                        elementAt((int)packet.getTetrahedronIndex(adj));
            }
        }
        fireTableDataChanged();
    }

    /**
     * Update the underlying triangulation in the engine to reflect
     * the contents of this model.
     *
     * @param packet the underlying triangulation packet in the engine
     * which is to be updated.
     * @param engine the engine through which any new engine objects
     * must be created.
     */
    public void writeToEngine(NTriangulation packet, Engine engine) {
        packet.removeAllTetrahedra();
        int total = tetrahedra.size();
        NTetrahedron[] realTetrahedra = new NTetrahedron[total];
        
        // First add the tetrahedra.
        int i, face;
        for (i=0; i<total; i++) {
            realTetrahedra[i] = engine.newNTetrahedron();
            packet.addTetrahedron(realTetrahedra[i]);
        }
        
        // Then glue up the faces and set descriptions.
        TetrahedronModel model;
        int you, yourFace;
        for (i=0; i<total; i++) {
            model = (TetrahedronModel)tetrahedra.elementAt(i);
            realTetrahedra[i].setDescription(model.description);
            for (face=0; face<4; face++)
                if (model.adjacent[face] != null) {
                    you = model.adjacent[face].index;
                    yourFace = model.gluings[face].imageOf(face);
                    if ((you > i) || (you == i && yourFace > face))
                        realTetrahedra[i].joinTo(face,
                            realTetrahedra[you], model.gluings[face]);
                }
        }

        // Tidy up.
        packet.gluingsHaveChanged();
    }
}
