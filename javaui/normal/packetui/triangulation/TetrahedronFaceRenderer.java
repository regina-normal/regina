
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

package normal.packetui.triangulation;

import java.awt.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.table.*;
import normal.engine.triangulation.*;

/**
 * Used to render cells bearing face gluings for specific tetrahedra.
 */
public class TetrahedronFaceRenderer extends JLabel
        implements TableCellRenderer {
    /**
     * Border to be used for cells with no focus.
     */
    private static final Border noFocusBorder =
        new EmptyBorder(1,2,1,2);

    /**
     * Create a new renderer to display the given face of a tetrahedron.
     */
    public TetrahedronFaceRenderer() {
        setOpaque(true); // So background colours will show.
        setToolTipText("Face gluing");
    }
    
    /**
     * Returns the component that actually does the rendering.
     *
     * @param table the table containing the cell to be rendered.
     * @param value the value of the cell to be rendered (which in this
     * case is the tetrahedron whose row is being edited).
     * @param isSelected whether the table cell is currently selected.
     * @param hasFocus whether the table cell currently has focus.
     * @param row the row of the cell to be rendered.
     * @param column the column of the cell to be rendered.
     * @return the component that does the rendering.
     */
    public Component getTableCellRendererComponent(JTable table, Object value,
            boolean isSelected, boolean hasFocus, int row, int column) {
        // Set the attributes of the component.
        setForeground(isSelected ? table.getSelectionForeground() :
            table.getForeground());
        setBackground(isSelected ? table.getSelectionBackground() :
            table.getBackground());
        setFont(table.getFont());

        // Set the border of the component.
        if (hasFocus) {
            setBorder(UIManager.getBorder("Table.focusCellHighlightBorder"));
            if (table.isCellEditable(row, column)) {
                setForeground(UIManager.getColor("Table.focusCellForeground"));
                setBackground(UIManager.getColor("Table.focusCellBackground"));
            }
        }
        else
            setBorder(noFocusBorder);
        
        // Set the component's contents.
        int face = 4 - column;
        TetrahedronModel adj = ((TetrahedronModel)value).adjacent[face];
        if (adj == null)
            setText("Boundary");
        else {
            NPerm perm = ((TetrahedronModel)value).gluings[face].
                composeWith(NPerm.faceOrdering(face));
            setText("> " + adj.index + " (" + 
                NPerm.faceDescription(perm) + ')');
        }

        return this;
    }
}
