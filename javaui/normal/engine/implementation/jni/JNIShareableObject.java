
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

import java.lang.reflect.*;
import normal.engine.*;

/**
 * Java object associated with an object in an external C++ library
 * through JNI.
 * <p>
 * The protected member <tt>cppPtr</tt> is a memory pointer to the external
 * C++ object. Through this member variable, a C++ implementation of a
 * native method that is passed only the Java object can thus access the
 * corresponding native C++ object.
 * <p>
 * When a new derivative of <tt>JNIShareableObject</tt> is created, many
 * constructors
 * may be called.  The corresponding native C++ object should <i>not</i> be
 * created in more than one of these constructors.  Thus every derivative of
 * <tt>JNIShareableObject</tt> should provide a constructor taking a single
 * <tt>Sentry</tt> argument that does absolutely nothing.  All initialisation
 * and creation of native C++ objects should take place in a constructor for
 * the most derived class, and every constructor should call the do-nothing
 * constructor for its parent class.
 *
 * @see #cppPtr
 * @see normal.engine.implementation.jni.Sentry
 */
public abstract class JNIShareableObject implements ShareableObject {
    /**
     * Memory pointer to the associated native C++ object.
     * A C++ implementation of a native method should cast
     * <tt>cppPtr</tt> to a pointer to gain access to the
     * native C++ object.
     */
    protected long cppPtr;

    /**
     * The do-nothing constructor.
     *
     * @param s any <tt>Sentry</tt> object; <tt>Sentry.instance</tt> will do.
     * @see normal.engine.implementation.jni.JNIShareableObject
     * @see normal.engine.implementation.jni.Sentry
     */
    protected JNIShareableObject(Sentry s) {
    }

    /**
     * Determines whether the two given longs represent pointers to the
     * same underlying object in the C++ calculate engine.
     *
     * <b>Warning:</b> Note that under some C++ compilers this
     * question <b>cannot</b> be answered by directly comparing the
     * two given longs.  This is why this routine exists and why it
     * is implemented in the C++ calculation engine.
     *
     * <b>Precondition:</b> The two given longs, when cast as C++
     * pointers, are pointers to <tt>ShareableObject</tt> instances in
     * the calculation engine.
     *
     * @param ptr1 the first C++ pointer to examine.
     * @param ptr2 the second C++ pointer to examine.
     * @return <tt>true</tt> if and only if the two given longs, when
     * cast as C++ pointers, point to the same underlying object.
     */
    protected native static boolean sameCppPtr(long ptr1, long ptr2);

    public boolean sameObject(ShareableObject object) {
        return (object != null && sameCppPtr(cppPtr,
            ((JNIShareableObject)object).cppPtr));
    }
    public boolean equals(Object obj) {
        if (obj instanceof JNIShareableObject)
            return sameObject((JNIShareableObject)obj);
        return false;
    }
    public ShareableObject castAs(Class cls) {
        Class[] parameters = { Sentry.class };
        Object[] args = { Sentry.instance };
        try {
            Constructor c = cls.getConstructor(parameters);
            JNIShareableObject ans = (JNIShareableObject)c.newInstance(args);
            ans.cppPtr = this.cppPtr;
            return ans;
        } catch (Throwable th) {
            th.printStackTrace();
            return null;
        }
    }

    public native void destroy();

    public native void writeTextShort();
    public native void writeTextLong();
    public native String toString();
    public native String toStringLong();
}
