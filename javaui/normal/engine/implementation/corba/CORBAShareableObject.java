
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

package normal.engine.implementation.corba;

import java.math.BigInteger;
import normal.engine.utilities.NBoolSet;
import normal.engine.implementation.corba.Regina.*;

/**
 * Java object associated with an object in an external C++ CORBA server.
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
public class CORBAShareableObject implements normal.engine.ShareableObject {
    /**
     * The CORBA client object for the underlying engine object that we are
     * wrapping.
     */
    public ShareableObject data;

    /**
     * The CORBA client class corresponding to this wrapper class.
     */
    public static final Class CORBAClass = ShareableObject.class;

    /**
     * The CORBA helper class corresponding to this wrapper class.
     */
    public static final Class helperClass = ShareableObjectHelper.class;

    /**
     * Converts the given string to a <tt>BigInteger</tt>.
     * If the given string is <tt>inf</tt>, <tt>null</tt> will be
     * returned.  Otherwise the string will be interpreted as the
     * decimal representation of a large integer and the corresponding
     * <tt>BigInteger</tt> will be returned.
     *
     * @param value the string to convert.
     * @return the corresponding <tt>BigInteger</tt>.
     */
    public static BigInteger stringToLarge(String value) {
        if (value.equals("inf"))
            return null;
        return new BigInteger(value);
    }

    /**
     * Creates a new wrapper for the given object.
     *
     * @param data the CORBA client object whose corresponding engine
     * object we are wrapping.
     */
    protected CORBAShareableObject(ShareableObject data) {
        this.data = data;
    }

    public boolean sameObject(normal.engine.ShareableObject object) {
        return data.sameObject(((CORBAShareableObject)object).data);
    }
    public boolean equals(Object obj) {
        if (obj instanceof CORBAShareableObject)
            return data.sameObject(((CORBAShareableObject)obj).data);
        return false;
    }
    public normal.engine.ShareableObject castAs(Class cls) {
        try {
            // Get the helper and CORBA classes.
            Class realHelperClass =
                (Class)cls.getField("helperClass").get(null);
            Class realCORBAClass =
                (Class)cls.getField("CORBAClass").get(null);

            // Use the helper class to narrow [data].
            Class[] narrowArgs = { org.omg.CORBA.Object.class };
            Object[] narrowInvokeArgs = { data };
            Object narrowed = realHelperClass.getMethod("narrow", narrowArgs).
                invoke(null, narrowInvokeArgs);

            // Create and return a new wrapper.
            Class[] constrArgs = { realCORBAClass };
            Object[] constrInvokeArgs = { narrowed };
            return (normal.engine.ShareableObject)
                cls.getConstructor(constrArgs).newInstance(constrInvokeArgs);
        } catch (Throwable th) {
            th.printStackTrace();
            return null;
        }
    }

    public void destroy() {
        data.destroy();
    }

    public void writeTextShort() {
        System.out.println(toString());
    }
    public void writeTextLong() {
        System.out.println(toStringLong());
    }
    public String toString() {
        return data._toString();
    }
    public String toStringLong() {
        return data.toStringLong();
    }
}
