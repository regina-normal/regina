
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

package normal.engine.implementation.jni.surfaces;

import normal.engine.implementation.jni.*;
import normal.engine.surfaces.*;
import normal.engine.utilities.NBoolSet;
import java.math.BigInteger;

public class NJNISurfaceFilterProperties extends NJNISurfaceFilter
        implements NSurfaceFilterProperties {
    public NJNISurfaceFilterProperties(Sentry s) {
        super(s);
    }
    public NJNISurfaceFilterProperties() {
        super(Sentry.instance);
        newNSurfaceFilterProperties();
    }
    public NJNISurfaceFilterProperties(NSurfaceFilterProperties cloneMe) {
        super(Sentry.instance);
        newNSurfaceFilterProperties(cloneMe);
    }
    private final native void newNSurfaceFilterProperties();
    private final native void newNSurfaceFilterProperties(
            NSurfaceFilterProperties cloneMe);

    public native long getNumberOfECs();
    public native BigInteger getEC(long index);
    public NBoolSet getOrientability() {
        return NBoolSet.fromByteCode(_getOrientability());
    }
    public native char _getOrientability();
    public NBoolSet getCompactness() {
        return NBoolSet.fromByteCode(_getCompactness());
    }
    public native char _getCompactness();
    public NBoolSet getRealBoundary() {
        return NBoolSet.fromByteCode(_getRealBoundary());
    }
    public native char _getRealBoundary();

    public native void addEC(BigInteger ec);
    public native void removeEC(BigInteger ec);
    public native void removeAllECs();
    public void setOrientability(NBoolSet value) {
        _setOrientability(value.getByteCode());
    }
    public native void _setOrientability(char value);
    public void setCompactness(NBoolSet value) {
        _setCompactness(value.getByteCode());
    }
    public native void _setCompactness(char value);
    public void setRealBoundary(NBoolSet value) {
        _setRealBoundary(value.getByteCode());
    }
    public native void _setRealBoundary(char value);
}
