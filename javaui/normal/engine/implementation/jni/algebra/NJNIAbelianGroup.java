
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

package normal.engine.implementation.jni.algebra;

import java.math.BigInteger;
import java.util.Enumeration;
import normal.engine.implementation.jni.*;
import normal.engine.algebra.NAbelianGroup;
import normal.engine.maths.NMatrixInt;

public class NJNIAbelianGroup extends JNIShareableObject
        implements NAbelianGroup {
    public NJNIAbelianGroup(Sentry s) {
        super(s);
    }
    public NJNIAbelianGroup() {
        super(Sentry.instance);
        newNAbelianGroup();
    }
    public NJNIAbelianGroup(NAbelianGroup cloneMe) {
        super(Sentry.instance);
        newNAbelianGroup(cloneMe);
    }
    public final native void newNAbelianGroup();
    public final native void newNAbelianGroup(NAbelianGroup cloneMe);

    public void addRank() {
        addRank(1);
    }
    public native void addRank(int extraRank);
    public void addTorsionElement(BigInteger degree) {
        addTorsionElement(degree, 1);
    }
    public native void addTorsionElement(BigInteger degree, int mult);
    public void addTorsionElement(long degree) {
        addTorsionElement(degree, 1);
    }
    public native void addTorsionElement(long degree, int mult);
    public native void addTorsionElements(Enumeration torsion);
    public native void addGroup(NMatrixInt presentation);
    public native void addGroup(NAbelianGroup group);
    
    public native int getRank();
    public native int getTorsionRank(BigInteger degree);
    public native int getTorsionRank(long degree);
    public native long getNumberOfInvariantFactors();
    public native BigInteger getInvariantFactor(long index);
}
