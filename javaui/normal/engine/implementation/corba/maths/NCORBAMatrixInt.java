
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

package normal.engine.implementation.corba.maths;

import java.math.BigInteger;
import normal.engine.implementation.corba.Regina.Maths.*;
import normal.engine.implementation.corba.*;

public class NCORBAMatrixInt extends CORBAShareableObject
        implements normal.engine.maths.NMatrixInt {
    public NMatrixInt data;
    public static final Class CORBAClass = NMatrixInt.class;
    public static final Class helperClass = NMatrixIntHelper.class;

    protected NCORBAMatrixInt(NMatrixInt data) {
        super(data);
        this.data = data;
    }

    public static NCORBAMatrixInt newWrapper(NMatrixInt source) {
        return (source == null ? null : new NCORBAMatrixInt(source));
    }

    public void initialise(BigInteger value) {
        data.initialise(value.toString());
    }
    public void makeIdentity() {
        data.makeIdentity();
    }

    public int rows() {
        return data.rows();
    }
    public int columns() {
        return data.columns();
    }

    public BigInteger getEntry(int row, int column) {
        return stringToLarge(data.getEntry(row, column));
    }
    public void setEntry(int row, int column, BigInteger value) {
        data.setEntry(row, column, value.toString());
    }

    public void swapRows(int first, int second) {
        data.swapRows(first, second);
    }
    public void swapColumns(int first, int second) {
        data.swapColumns(first, second);
    }

    public void addRow(int source, int dest) {
        data.addRow_long_long(source, dest);
    }
    public void addRow(int source, int dest, BigInteger copies) {
        data.addRow_long_long_bigInt(source, dest, copies.toString());
    }
    public void addCol(int source, int dest) {
        data.addCol_long_long(source, dest);
    }
    public void addCol(int source, int dest, BigInteger copies) {
        data.addCol_long_long_bigInt(source, dest, copies.toString());
    }
    public void multRow(int row, BigInteger factor) {
        data.multRow(row, factor.toString());
    }
    public void multCol(int column, BigInteger factor) {
        data.multCol(column, factor.toString());
    }
}
