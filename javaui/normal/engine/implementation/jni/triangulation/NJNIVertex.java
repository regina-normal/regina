
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

package normal.engine.implementation.jni.triangulation;

import normal.engine.implementation.jni.*;
import normal.engine.triangulation.*;

public class NJNIVertex extends JNIShareableObject implements NVertex {
    public NJNIVertex(Sentry s) {
        super(s);
    }
    
    public native NComponent getComponent();
    public native NBoundaryComponent getBoundaryComponent();

    public native int getLink();
    public native boolean isLinkClosed();
    public native boolean isIdeal();
    public native boolean isBoundary();
    public native boolean isStandard();
    public native boolean isLinkOrientable();
    public native long getLinkEulerCharacteristic();

    public native long getNumberOfEmbeddings();
    public native NVertexEmbedding getEmbedding(long index);
}
