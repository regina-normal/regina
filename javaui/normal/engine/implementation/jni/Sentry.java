
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

package normal.engine.implementation.jni;

/**
 * Do-nothing sentry class.
 * The sole purpose of this class is to not be mistaken for anything else.
 * It is used to specify the do-nothing constructor for a class derived
 * from <tt>JNIShareableObject</tt>.
 * <p>
 * When looking for a <tt>Sentry</tt> to pass to a <tt>JNIShareableObject</tt>
 * constructor, you can use the pre-existing object <tt>Sentry.instance</tt>.
 *
 * @see normal.engine.implementation.jni.JNIShareableObject
 */
public final class Sentry {
    /**
     * Pre-existing <tt>Sentry</tt> object to call upon whenever required.
     */
    public static final Sentry instance = new Sentry();
    
    /**
     * Constructor that does nothing because there is nothing to do.
     */
    public Sentry() {
    }
} 
