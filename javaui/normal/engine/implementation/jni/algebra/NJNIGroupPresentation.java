
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

package normal.engine.implementation.jni.algebra;

import normal.engine.implementation.jni.*;
import normal.engine.algebra.*;

public class NJNIGroupPresentation extends JNIShareableObject
        implements NGroupPresentation {
    public NJNIGroupPresentation(Sentry s) {
        super(s);
    }
    public NJNIGroupPresentation() {
        super(Sentry.instance);
        newNGroupPresentation();
    }
    public NJNIGroupPresentation(NGroupPresentation cloneMe) {
        super(Sentry.instance);
        newNGroupPresentation(cloneMe);
    }
    public final native void newNGroupPresentation();
    public final native void newNGroupPresentation(NGroupPresentation cloneMe);

	public long addGenerator() {
		return addGenerator(1);
	}
	public native long addGenerator(long numToAdd);
	public native void addRelation(NGroupExpression rel);
	public native long getNumberOfGenerators();
	public native long getNumberOfRelations();
	public native NGroupExpression getRelation(long index);
	public native boolean intelligentSimplify();
	public native String recogniseGroup();
}
