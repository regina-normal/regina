
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

package normal.engine.utilities;

public class NBoolSet {
    /**
     * Calculates the xor (exclusive or) of the two given booleans.
     * This will be true precisely when one of the given booleans is
     * true and the other is false.
     *
     * @param a the first boolean to xor.
     * @param b the second boolean to xor.
     * @return the xor of the two given booleans.
     */
    private static final boolean xor(boolean a, boolean b) {
        return ((a && ! b) || (b && ! a));
    }

    private boolean containsTrue;
    private boolean containsFalse;

    public static final NBoolSet sNone = new NBoolSet();
    public static final NBoolSet sTrue = new NBoolSet(true);
    public static final NBoolSet sFalse = new NBoolSet(false);
    public static final NBoolSet sBoth = new NBoolSet(true, true);

    public NBoolSet() {
        this.containsTrue = false;
        this.containsFalse = false;
    }
    public NBoolSet(boolean member) {
        this.containsTrue = member;
        this.containsFalse = ! member;
    }
    public NBoolSet(NBoolSet cloneMe) {
        this.containsTrue = cloneMe.containsTrue;
        this.containsFalse = cloneMe.containsFalse;
    }
    public NBoolSet(boolean insertTrue, boolean insertFalse) {
        this.containsTrue = insertTrue;
        this.containsFalse = insertFalse;
    }

    public boolean hasTrue() {
        return this.containsTrue;
    }
    public boolean hasFalse() {
        return this.containsFalse;
    }
    public boolean contains(boolean value) {
        return (value ? containsTrue : containsFalse);
    }

    public void insertTrue() {
        this.containsTrue = true;
    }
    public void insertFalse() {
        this.containsFalse = true;
    }
    public void removeTrue() {
        this.containsTrue = false;
    }
    public void removeFalse() {
        this.containsFalse = false;
    }
    public void empty() {
        this.containsTrue = false;
        this.containsFalse = false;
    }
    public void fill() {
        this.containsTrue = true;
        this.containsFalse = true;
    }

    public boolean sameSet(NBoolSet other) {
        return ! (xor(containsTrue, other.containsTrue) ||
            xor(containsFalse, other.containsFalse));
    }
    public boolean notSameSet(NBoolSet other) {
        return (xor(containsTrue, other.containsTrue) ||
            xor(containsFalse, other.containsFalse));
    }
    public boolean properSubsetOf(NBoolSet other) {
        return (subsetOf(other) && ! equals(other));
    }
    public boolean properSupersetOf(NBoolSet other) {
        return (supersetOf(other) && ! equals(other));
    }
    public boolean subsetOf(NBoolSet other) {
        return ! ((containsTrue && ! other.containsTrue) ||
            (containsFalse && ! other.containsFalse));
    }
    public boolean supersetOf(NBoolSet other) {
        return ! ((other.containsTrue && ! containsTrue) ||
            (other.containsFalse && ! containsFalse));
    }

    public NBoolSet setTo(NBoolSet cloneMe) {
        this.containsTrue = cloneMe.containsTrue;
        this.containsFalse = cloneMe.containsFalse;
        return this;
    }
    public NBoolSet setTo(boolean member) {
        this.containsTrue = member;
        this.containsFalse = ! member;
        return this;
    }

    public NBoolSet formUnion(NBoolSet other) {
        return new NBoolSet(containsTrue || other.containsTrue,
            containsFalse || other.containsFalse);
    }
    public NBoolSet formIntersection(NBoolSet other) {
        return new NBoolSet(containsTrue && other.containsTrue,
            containsFalse && other.containsFalse);
    }
    public NBoolSet formSymmDiff(NBoolSet other) {
        return new NBoolSet(xor(containsTrue, other.containsTrue),
            xor(containsFalse, other.containsFalse));
    }
    public NBoolSet formComplement() {
        return new NBoolSet(! containsTrue, ! containsFalse);
    }

    public char getByteCode() {
        return (char)((containsTrue ? 1 : 0) + (containsFalse ? 2 : 0));
    }
    public void setByteCode(char code) {
        containsTrue = ((code & 1) != 0);
        containsFalse = ((code & 2) != 0);
    }
    public static NBoolSet fromByteCode(char code) {
        return new NBoolSet((code & 1) != 0, (code & 2) != 0);
    }
}

