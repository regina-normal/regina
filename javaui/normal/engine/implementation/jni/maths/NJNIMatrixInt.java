
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

package normal.engine.implementation.jni.maths;

import normal.engine.implementation.jni.*;
import normal.engine.maths.NMatrixInt;
import java.math.BigInteger;

public class NJNIMatrixInt extends JNIShareableObject implements NMatrixInt {
    public NJNIMatrixInt(Sentry s) {
        super(s);
    }
    public NJNIMatrixInt(int rows, int columns) {
        super(Sentry.instance);
        newNMatrixInt(rows, columns);
    }
    public NJNIMatrixInt(NMatrixInt cloneMe) {
        super(Sentry.instance);
        newNMatrixInt(cloneMe);
    }
    private final native void newNMatrixInt(int rows, int columns);
    private final native void newNMatrixInt(NMatrixInt cloneMe);

    public native void initialise(BigInteger value);
    public native void makeIdentity();
    
    public native int rows();
    public native int columns();

    public native BigInteger getEntry(int row, int column);
    public native void setEntry(int row, int column, BigInteger value);

    public native void swapRows(int first, int second);
    public native void swapColumns(int first, int second);

    public native void addRow(int source, int dest);
    public native void addRow(int source, int dest, BigInteger copies);
    public native void addCol(int source, int dest);
    public native void addCol(int source, int dest, BigInteger copies);
    public native void multRow(int row, BigInteger factor);
    public native void multCol(int column, BigInteger factor);
}
