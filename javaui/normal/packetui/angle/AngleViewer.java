
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

package normal.packetui.angle;

import java.awt.*;
import java.math.BigInteger;
import javax.swing.*;
import javax.swing.table.*;
import normal.engine.angle.*;
import normal.engine.packet.*;
import normal.engine.surfaces.*;
import normal.packetui.*;
import org.gjt.btools.gui.component.*;

/**
 * An interface through which the user can view the individual
 * angles in an angle structure list.
 */
public class AngleViewer extends DefaultPacketViewer {
    /**
     * The angle structure list we are examining.
     */
    private NAngleStructureList angles;

    /**
     * The table containing the individual angles.
     */
    private JTable table;

    /**
     * The model for the table containing the individual angles.
     */
    private AngleTableModel model;

    /**
     * Create a new interface to display the individual angles for the
     * given angle structure list.
     *
     * @param angles the angle structure list we are examining.
     */
    public AngleViewer(NAngleStructureList angles) {
        super();
        this.angles = angles;

        init();
    }

    /**
     * Create the interface elements.
     */
    private void init() {
        model = new AngleTableModel();
        table = new JTable();
        table.setAutoResizeMode(table.AUTO_RESIZE_OFF);
            // This ensures we get a horizontal scrollbar!

        setLayout(new BorderLayout());
        add(new JScrollPane(table), BorderLayout.CENTER);
    }

    public NPacket getPacket() {
        return angles;
    }

    public void reflectPacket() {
        table.setModel(model);
        model.fireTableStructureChanged();

        TableColumn col;
        TableCellRenderer renderer = new FancyColumnHeaderRenderer(table);
        for (int i=0; i<model.getColumnCount(); i++) {
            col = table.getColumnModel().getColumn(i);
            col.setPreferredWidth(70);
            col.setHeaderRenderer(renderer);
            col.setHeaderValue(new FancyData(model.getColumnName(i),
                model.getColumnToolTip(i)));
        }
    }

    /**
     * Provides a table model for viewing the individual angles.
     */
    private class AngleTableModel extends AbstractTableModel {
        /**
         * Creates a new table model.
         */
        public AngleTableModel() {
        }

        /**
         * Returns the number of rows in the table.
         * @return the number of rows.
         */
        public int getRowCount() {
            return (int)angles.getNumberOfStructures();
        }

        /**
         * Returns the number of columns in the table.
         * @return the number of columns.
         */
        public int getColumnCount() {
            return (int)(angles.getTriangulation().getNumberOfTetrahedra()
                * 3 + 1);
        }

        /**
         * Returns the requested value in the table.
         * @param row the row of the requested value.
         * @param col the column of the requested value.
         * @return the value in the given row and column.
         */
        public Object getValueAt(int row, int column) {
            NAngleStructure structure = angles.getStructure(row);
            if (column == 0) {
                if (structure.isStrict())
                    return "Strict";
                else if (structure.isTaut())
                    return "Taut";
                else
                    return "";
            } else {
                column--;
                BigInteger num = structure.getAngleNum(column / 3, column % 3);
                BigInteger den = structure.getAngleDen(column / 3, column % 3);
                if (num.equals(BigInteger.ZERO))
                    return "";
                else if (den.equals(BigInteger.ONE)) {
                    if (num.equals(BigInteger.ONE))
                        return "Pi";
                    else
                        return num.toString() + " Pi";
                } else if (num.equals(BigInteger.ONE))
                    return "Pi / " + den.toString();
                else
                    return num.toString() + " Pi / " + den.toString();
            }
        }

        /**
         * Returns the string name of the requested column.
         * @param column the requested column.
         * @return the string name of the requested column.
         */
        public String getColumnName(int column) {
            if (column == 0)
                return "Type";
            else {
                column--;
                return String.valueOf(column / 3) + ": " +
                    NNormalSurface.vertexSplitString[column % 3];
            }
        }

        /**
         * Returns the tooltip for the requested column.
         * @param column the requested column.
         * @return the tooltip for the requested column.
         */
        public String getColumnToolTip(int column) {
            if (column == 0)
                return "Taut or strict?";
            else {
                column--;
                return "Tetrahedron " + String.valueOf(column / 3) +
                    ", edges " + NNormalSurface.vertexSplitString[column % 3];
            }
        }
    }
}
