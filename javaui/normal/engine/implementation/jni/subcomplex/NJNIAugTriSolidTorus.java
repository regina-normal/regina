
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

package normal.engine.implementation.jni.subcomplex;

import normal.engine.implementation.jni.*;
import normal.engine.subcomplex.*;
import normal.engine.triangulation.NPerm;

public class NJNIAugTriSolidTorus extends JNIShareableObject
        implements NAugTriSolidTorus {
    public NJNIAugTriSolidTorus(Sentry s) {
        super(s);
    }

    public native NAugTriSolidTorus cloneMe();
    public native long getAlpha(int index);
    public native long getBeta(int index);
    public native long getExceptionalFibres();
    public native NTriSolidTorus getCore();
    public native NLayeredSolidTorus getAugTorus(int annulus);
    public NPerm getEdgeGroupRoles(int annulus) {
        return new NPerm(_getEdgeGroupRoles(annulus));
    }
    public native char _getEdgeGroupRoles(int annulus);
}
