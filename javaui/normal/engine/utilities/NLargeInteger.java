
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

package normal.engine.utilities;

import java.math.BigInteger;
import java.util.Random;

/**
 * An arbitrary precision integer class for use with Jython.
 * Many routines <tt>java.math.BigInteger</tt> have been
 * overridden to return
 * an <tt>NLargeInteger</tt> instead.  Addition routines have been
 * inserted to overload standard operators in Jython.
 * <p>
 * The net effect is that <tt>NLargeInteger</tt> objects <i>should</i>
 * be easy to use in Jython in an intuitive sense.
 * <p>
 * Routines that have been overridden or duplicated from
 * <tt>BigInteger</tt> and routines that provide standard Jython
 * operator overloads will not be commented in this class.
 */
public class NLargeInteger extends BigInteger {
    /**
     * Rewritten <tt>BigInteger</tt> member.
     */
    public static final NLargeInteger ONE = new NLargeInteger("1");
    /**
     * Rewritten <tt>BigInteger</tt> member.
     */
    public static final NLargeInteger ZERO = new NLargeInteger("0");

    /**
     * Object representing <tt>true</tt> for use with Jython.
     */
    public static final Object TRUE = new Integer(1);
    /**
     * Object representing <tt>false</tt> for use with Jython.
     */
    public static final Object FALSE = new Integer(0);

    /**
     * Creates a new large integer containing the same value as the
     * given large integer.
     *
     * @param val the value to assign to this large integer.
     */
    public NLargeInteger(BigInteger val) {
        super(val.toByteArray());
    }
    /**
     * Rewritten <tt>BigInteger</tt> routine.
     */
    public NLargeInteger(byte[] val) {
        super(val);
    }
    /**
     * Rewritten <tt>BigInteger</tt> routine.
     */
    public NLargeInteger(int signum, byte[] magnitude) {
        super(signum, magnitude);
    }
    /**
     * Rewritten <tt>BigInteger</tt> routine.
     */
    public NLargeInteger(int bitLength, int certainty, Random rnd) {
        super(bitLength, certainty, rnd);
    }
    /**
     * Rewritten <tt>BigInteger</tt> routine.
     */
    public NLargeInteger(int numBits, Random rnd) {
        super(numBits, rnd);
    }
    /**
     * Rewritten <tt>BigInteger</tt> routine.
     */
    public NLargeInteger(String val) {
        super(val);
    }
    /**
     * Rewritten <tt>BigInteger</tt> routine.
     */
    public NLargeInteger(String val, int radix) {
        super(val, radix);
    }

    /**
     * Jython operator overload.
     */
    public int __cmp__(Object obj) {
        if (obj instanceof BigInteger)
            return compareTo(obj);
        if (obj instanceof Number)
            return compareTo(new BigInteger(obj.toString()));
        return -2;
    }
    /**
     * Jython operator overload.
     */
    public Object __eq__(Object obj) {
        if (obj instanceof BigInteger)
            return (equals(obj) ? TRUE : FALSE);
        if (obj instanceof Number)
            return (equals(new BigInteger(obj.toString())) ? TRUE : FALSE);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __ne__(Object obj) {
        if (obj instanceof BigInteger)
            return (equals(obj) ? FALSE : TRUE);
        if (obj instanceof Number)
            return (equals(new BigInteger(obj.toString())) ? FALSE : TRUE);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __le__(Object obj) {
        if (obj instanceof BigInteger)
            return (compareTo(obj) <= 0 ? TRUE : FALSE);
        if (obj instanceof Number)
            return (compareTo(new BigInteger(
                obj.toString())) <= 0 ? TRUE : FALSE);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __lt__(Object obj) {
        if (obj instanceof BigInteger)
            return (compareTo(obj) < 0 ? TRUE : FALSE);
        if (obj instanceof Number)
            return (compareTo(new BigInteger(
                obj.toString())) < 0 ? TRUE : FALSE);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __ge__(Object obj) {
        if (obj instanceof BigInteger)
            return (compareTo(obj) >= 0 ? TRUE : FALSE);
        if (obj instanceof Number)
            return (compareTo(new BigInteger(
                obj.toString())) >= 0 ? TRUE : FALSE);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __gt__(Object obj) {
        if (obj instanceof BigInteger)
            return (compareTo(obj) > 0 ? TRUE : FALSE);
        if (obj instanceof Number)
            return (compareTo(new BigInteger(
                obj.toString())) > 0 ? TRUE : FALSE);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __nonzero__() {
        return (equals(ZERO) ? FALSE : TRUE);
    }

    /**
     * Jython operator overload.
     */
    public Object __add__(Object obj) {
        if (obj instanceof BigInteger)
            return add((BigInteger)obj);
        if (obj instanceof Number)
            return add(new BigInteger(obj.toString()));
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __sub__(Object obj) {
        if (obj instanceof BigInteger)
            return subtract((BigInteger)obj);
        if (obj instanceof Number)
            return subtract(new BigInteger(obj.toString()));
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __mul__(Object obj) {
        if (obj instanceof BigInteger)
            return multiply((BigInteger)obj);
        if (obj instanceof Number)
            return multiply(new BigInteger(obj.toString()));
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __div__(Object obj) {
        if (obj instanceof BigInteger)
            return divide((BigInteger)obj);
        if (obj instanceof Number)
            return divide(new BigInteger(obj.toString()));
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __mod__(Object obj) {
        if (obj instanceof BigInteger)
            return mod((BigInteger)obj);
        if (obj instanceof Number)
            return mod(new BigInteger(obj.toString()));
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __divmod__(Object obj) {
        if (obj instanceof BigInteger)
            return divideAndRemainder((BigInteger)obj);
        if (obj instanceof Number)
            return divideAndRemainder(new BigInteger(obj.toString()));
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __pow__(Object obj) {
        if (obj instanceof Number)
            return pow(((Number)obj).intValue());
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __pow__(Object obj, Object modulo) {
        if (! (obj instanceof Number))
            return null;
        if (! (modulo instanceof Number))
            return null;
        if (! (obj instanceof BigInteger))
            obj = new BigInteger(obj.toString());
        if (! (modulo instanceof BigInteger))
            modulo = new BigInteger(modulo.toString());
        return modPow((BigInteger)obj, (BigInteger)modulo);
    }
    /**
     * Jython operator overload.
     */
    public Object __lshift__(Object obj) {
        if (obj instanceof Number)
            return shiftLeft(((Number)obj).intValue());
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __rshift__(Object obj) {
        if (obj instanceof Number)
            return shiftRight(((Number)obj).intValue());
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __and__(Object obj) {
        if (obj instanceof BigInteger)
            return and((BigInteger)obj);
        if (obj instanceof Number)
            return and(new BigInteger(obj.toString()));
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __or__(Object obj) {
        if (obj instanceof BigInteger)
            return or((BigInteger)obj);
        if (obj instanceof Number)
            return or(new BigInteger(obj.toString()));
        return null;
    }

    /**
     * Jython operator overload.
     */
    public Object __radd__(Object obj) {
        if (obj instanceof BigInteger)
            return ((BigInteger)obj).add(this);
        if (obj instanceof Number)
            return (new BigInteger(obj.toString())).add(this);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __rsub__(Object obj) {
        if (obj instanceof BigInteger)
            return ((BigInteger)obj).subtract(this);
        if (obj instanceof Number)
            return (new BigInteger(obj.toString())).subtract(this);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __rmul__(Object obj) {
        if (obj instanceof BigInteger)
            return ((BigInteger)obj).multiply(this);
        if (obj instanceof Number)
            return (new BigInteger(obj.toString())).multiply(this);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __rdiv__(Object obj) {
        if (obj instanceof BigInteger)
            return ((BigInteger)obj).divide(this);
        if (obj instanceof Number)
            return (new BigInteger(obj.toString())).divide(this);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __rmod__(Object obj) {
        if (obj instanceof BigInteger)
            return ((BigInteger)obj).mod(this);
        if (obj instanceof Number)
            return (new BigInteger(obj.toString())).mod(this);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __rdivmod__(Object obj) {
        if (obj instanceof BigInteger)
            return ((BigInteger)obj).divideAndRemainder(this);
        if (obj instanceof Number)
            return (new BigInteger(obj.toString())).divideAndRemainder(this);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __rpow__(Object obj) {
        if (obj instanceof BigInteger)
            return ((BigInteger)obj).pow(intValue());
        if (obj instanceof Number)
            return (new BigInteger(obj.toString())).pow(intValue());
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __rlshift__(Object obj) {
        if (obj instanceof BigInteger)
            return ((BigInteger)obj).shiftLeft(intValue());
        if (obj instanceof Number)
            return (new BigInteger(obj.toString())).shiftLeft(intValue());
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __rrshift__(Object obj) {
        if (obj instanceof BigInteger)
            return ((BigInteger)obj).shiftRight(intValue());
        if (obj instanceof Number)
            return (new BigInteger(obj.toString())).shiftRight(intValue());
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __rand__(Object obj) {
        if (obj instanceof BigInteger)
            return ((BigInteger)obj).and(this);
        if (obj instanceof Number)
            return (new BigInteger(obj.toString())).and(this);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __ror__(Object obj) {
        if (obj instanceof BigInteger)
            return ((BigInteger)obj).or(this);
        if (obj instanceof Number)
            return (new BigInteger(obj.toString())).or(this);
        return null;
    }

    /**
     * Jython operator overload.
     */
    public Object __neg__() {
        return negate();
    }
    /**
     * Jython operator overload.
     */
    public Object __pos__() {
        return this;
    }
    /**
     * Jython operator overload.
     */
    public Object __abs__() {
        return abs();
    }
    /**
     * Jython operator overload.
     */
    public Object __invert__() {
        return not();
    }

    /**
     * Jython operator overload.
     */
    public Object __int__() {
        return new Integer(intValue());
    }
    /**
     * Jython operator overload.
     */
    public Object __long__() {
        return new Long(longValue());
    }

    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger abs() {
        return new NLargeInteger(super.abs());
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger add(BigInteger val) {
        return new NLargeInteger(super.add(val));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger and(BigInteger val) {
        return new NLargeInteger(super.and(val));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger andNot(BigInteger val) {
        return new NLargeInteger(super.andNot(val));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger clearBit(int n) {
        return new NLargeInteger(super.clearBit(n));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger divide(BigInteger val) {
        return new NLargeInteger(super.divide(val));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger[] divideAndRemainder(BigInteger val) {
        BigInteger[] orig = super.divideAndRemainder(val);
        BigInteger[] ans = new BigInteger[2];
        ans[0] = new NLargeInteger(orig[0]);
        ans[1] = new NLargeInteger(orig[1]);
        return ans;
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger flipBit(int n) {
        return new NLargeInteger(super.flipBit(n));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger gcd(BigInteger val) {
        return new NLargeInteger(super.gcd(val));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger max(BigInteger val) {
        return new NLargeInteger(super.max(val));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger min(BigInteger val) {
        return new NLargeInteger(super.min(val));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger mod(BigInteger m) {
        return new NLargeInteger(super.mod(m));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger modInverse(BigInteger m) {
        return new NLargeInteger(super.modInverse(m));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger modPow(BigInteger exponent, BigInteger m) {
        return new NLargeInteger(super.modPow(exponent, m));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger multiply(BigInteger val) {
        return new NLargeInteger(super.multiply(val));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger negate() {
        return new NLargeInteger(super.negate());
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger not() {
        return new NLargeInteger(super.not());
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger or(BigInteger val) {
        return new NLargeInteger(super.or(val));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger pow(int exponent) {
        return new NLargeInteger(super.pow(exponent));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger remainder(BigInteger val) {
        return new NLargeInteger(super.remainder(val));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger setBit(int n) {
        return new NLargeInteger(super.setBit(n));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger shiftLeft(int n) {
        return new NLargeInteger(super.shiftLeft(n));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger shiftRight(int n) {
        return new NLargeInteger(super.shiftRight(n));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger subtract(BigInteger val) {
        return new NLargeInteger(super.subtract(val));
    }
    /**
     * Overridden <tt>BigInteger</tt> routine.
     */
    public BigInteger xor(BigInteger val) {
        return new NLargeInteger(super.xor(val));
    }

    /**
     * Rewritten <tt>BigInteger</tt> routine.
     */
    public static BigInteger valueOf(long val) {
        return new NLargeInteger(BigInteger.valueOf(val));
    }
}

