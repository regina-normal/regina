
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

package normal.engine.implementation.jni.algebra;

import normal.engine.implementation.jni.*;
import normal.engine.algebra.NGroupExpression;

public class NJNIGroupExpression extends JNIShareableObject
        implements NGroupExpression {
    public NJNIGroupExpression(Sentry s) {
        super(s);
    }
    public NJNIGroupExpression() {
        super(Sentry.instance);
        newNGroupExpression();
    }
    public NJNIGroupExpression(NGroupExpression cloneMe) {
        super(Sentry.instance);
        newNGroupExpression(cloneMe);
    }
    public final native void newNGroupExpression();
    public final native void newNGroupExpression(NGroupExpression cloneMe);

    public native long getNumberOfTerms();
    public native long getGenerator(long index);
    public native long getExponent(long index);
    public native void addTermFirst(long generator, long exponent);
    public native void addTermLast(long generator, long exponent);
    public native NGroupExpression inverse();
    public native NGroupExpression power(long exponent);
    public boolean simplify() {
        return simplify(false);
    }
    public native boolean simplify(boolean cyclic);
    public boolean substitute(long generator, NGroupExpression expansion) {
        return substitute(generator, expansion, false);
    }
    public native boolean substitute(long generator,
            NGroupExpression expansion, boolean cyclic);
}
