
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

package normal.packetui.surfaces;

import java.awt.*;
import java.math.BigInteger;
import javax.swing.*;
import javax.swing.table.*;
import normal.engine.packet.*;
import normal.engine.surfaces.*;
import normal.engine.triangulation.NTriangulation;
import normal.engine.maths.*;
import normal.packetui.*;
import org.gjt.btools.gui.component.*;

/**
 * An interface through which the user can view the matching equations
 * used to create a normal surface list.
 */
public class MatchingEquationsViewer extends DefaultPacketViewer
        implements PacketInfoUI {
    /**
     * The normal surface list we are examining.
     */
    private NNormalSurfaceList list;

    /**
     * The matching equations we are examining.
     */
    private NMatrixInt matchingEquations;

    /**
     * The table containing the matching equation matrix.
     */
    private JTable table;

    /**
     * The model for the table containing the matching equation matrix.
     */
    private METableModel model;

    /**
     * A label describing the flavour of coordinate system used.
     */
    private JLabel flavour;

    /**
     * Create a new interface to display the matching equations for the
     * given normal surface list.
     */
    public MatchingEquationsViewer(NPacket list) {
        super();
        this.list = (NNormalSurfaceList)list;
        this.matchingEquations = null;
        init();
    }

    /**
     * Create the interface elements.
     */
    private void init() {
        flavour = new JLabel(" ", JLabel.CENTER);
        model = new METableModel();
        table = new JTable();
        table.setAutoResizeMode(table.AUTO_RESIZE_OFF);
            // This ensures we get a horizontal scrollbar!

        setLayout(new BorderLayout());
        add(new PaddedPane(flavour, 4), BorderLayout.NORTH);
        add(new JScrollPane(table), BorderLayout.CENTER);
    }

    public NPacket getPacket() {
        return list;
    }

    public void reflectPacket() {
        flavour.setText(Coordinates.coordinateSystemName(list.getFlavour()) +
            " coordinates");
        if (matchingEquations != null)
            matchingEquations.destroy();

        matchingEquations = list.recreateMatchingEquations();
        table.setModel(model);
        model.fireTableStructureChanged();

        TableColumn col;
        TableCellRenderer renderer = new FancyColumnHeaderRenderer(table);
        NTriangulation triang = list.getTriangulation();
        for (int i=0; i<matchingEquations.columns(); i++) {
            col = table.getColumnModel().getColumn(i);
            col.setPreferredWidth(70);
            col.setHeaderRenderer(renderer);
            col.setHeaderValue(new FancyData(model.getColumnName(i),
                Coordinates.getCoordinateDesc(list.getFlavour(), triang, i)));
        }
    }

    public void editingElsewhere() {
        flavour.setText("Editing...");
        if (matchingEquations != null)
            matchingEquations.destroy();
            
        matchingEquations = null;
        table.setModel(new DefaultTableModel());
        ((DefaultTableModel)table.getModel()).fireTableStructureChanged();
    }

    public void cleanUp() {
        if (matchingEquations != null)
            matchingEquations.destroy();
    }

    /**
     * Provides a table model for viewing the matching equation matrix.
     */
    private class METableModel extends AbstractTableModel {
        /**
         * Returns the number of rows in the table.
         * @return the number of rows.
         */
        public int getRowCount() {
            return matchingEquations.rows();
        }

        /**
         * Returns the number of columns in the table.
         * @return the number of columns.
         */
        public int getColumnCount() {
            return matchingEquations.columns();
        }

        /**
         * Returns the requested value in the table.
         * @param row the row of the requested value.
         * @param col the column of the requested value.
         * @return the value in the given row and column.
         */
        public Object getValueAt(int row, int column) {
            BigInteger ans = matchingEquations.getEntry(row, column);
            if (ans.signum() == 0)
                return "";
            else
                return ans;
        }

        /**
         * Returns the string name of the requested column.
         * @param column the requested column.
         * @return the string name of the requested column.
         */
        public String getColumnName(int column) {
            return Coordinates.getCoordinateAbbr(list.getFlavour(),
                list.getTriangulation(), column);
        }
    }
}

