
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

package normal.engine.triangulation;

import normal.engine.*;

public class NPerm {
    public static final NPerm[] allPermsS4 = {
        new NPerm(0,1,2,3), new NPerm(0,1,3,2),
        new NPerm(0,2,3,1), new NPerm(0,2,1,3),
        new NPerm(0,3,1,2), new NPerm(0,3,2,1),
        new NPerm(1,0,3,2), new NPerm(1,0,2,3),
        new NPerm(1,2,0,3), new NPerm(1,2,3,0),
        new NPerm(1,3,2,0), new NPerm(1,3,0,2),
        new NPerm(2,0,1,3), new NPerm(2,0,3,1),
        new NPerm(2,1,3,0), new NPerm(2,1,0,3),
        new NPerm(2,3,0,1), new NPerm(2,3,1,0),
        new NPerm(3,0,2,1), new NPerm(3,0,1,2),
        new NPerm(3,1,0,2), new NPerm(3,1,2,0),
        new NPerm(3,2,1,0), new NPerm(3,2,0,1)
    };

    public static final int[] allPermsS4Inv = {
        0, 1, 4, 3,
        2, 5, 6, 7,
        12, 19, 18, 13,
        8, 11, 20, 15,
        16, 23, 10, 9,
        14, 21, 22, 17
    };

    public static final NPerm[] orderedPermsS4 = {
        new NPerm(0,1,2,3), new NPerm(0,1,3,2),
        new NPerm(0,2,1,3), new NPerm(0,2,3,1),
        new NPerm(0,3,1,2), new NPerm(0,3,2,1),
        new NPerm(1,0,2,3), new NPerm(1,0,3,2),
        new NPerm(1,2,0,3), new NPerm(1,2,3,0),
        new NPerm(1,3,0,2), new NPerm(1,3,2,0),
        new NPerm(2,0,1,3), new NPerm(2,0,3,1),
        new NPerm(2,1,0,3), new NPerm(2,1,3,0),
        new NPerm(2,3,0,1), new NPerm(2,3,1,0),
        new NPerm(3,0,1,2), new NPerm(3,0,2,1),
        new NPerm(3,1,0,2), new NPerm(3,1,2,0),
        new NPerm(3,2,0,1), new NPerm(3,2,1,0)
    };

    public static final NPerm[] allPermsS3 = {
        new NPerm(0,1,2,3), new NPerm(0,2,1,3),
        new NPerm(1,2,0,3), new NPerm(1,0,2,3),
        new NPerm(2,0,1,3), new NPerm(2,1,0,3)
    };

    public static final int[] allPermsS3Inv = {
        0, 1,
        4, 3,
        2, 5
    };

    public static final NPerm[] orderedPermsS3 = {
        new NPerm(0,1,2,3), new NPerm(0,2,1,3),
        new NPerm(1,0,2,3), new NPerm(1,2,0,3),
        new NPerm(2,0,1,3), new NPerm(2,1,0,3)
    };

    public static final NPerm[] allPermsS2 = {
        new NPerm(0,1,2,3), new NPerm(1,0,2,3)
    };

    public static final int[] allPermsS2Inv = {
        0, 1
    };

    /**
     * Object representing <tt>true</tt> for use with Jython.
     */
    public static final Object TRUE = new Integer(1);
    /**
     * Object representing <tt>false</tt> for use with Jython.
     */
    public static final Object FALSE = new Integer(0);

    private int[] mapping = new int[4];
    
    public NPerm() {
        for (int i=0; i<4; i++)
            mapping[i] = i;
    }
    public NPerm(char newCode) {
        setPermCode(newCode);
    }
    public NPerm(int i, int j) {
        this();
        mapping[i] = j;
        mapping[j] = i;
    }
    public NPerm(int a, int b, int c, int d) {
        mapping[0] = a;
        mapping[1] = b;
        mapping[2] = c;
        mapping[3] = d;
    }
    public NPerm(int a0, int a1, int b0, int b1,
            int c0, int c1, int d0, int d1) {
        mapping[a0] = a1;
        mapping[b0] = b1;
        mapping[c0] = c1;
        mapping[d0] = d1;
    }
    public NPerm(NPerm cloneMe) {
        for (int i=0; i<4; i++)
            mapping[i] = cloneMe.mapping[i];
    }
        
    public NPerm composeWith(NPerm q) {
        return new NPerm(
            mapping[q.mapping[0]],
            mapping[q.mapping[1]],
            mapping[q.mapping[2]],
            mapping[q.mapping[3]]);
    }
    public NPerm inverse() {
        return new NPerm(
            mapping[0], 0,
            mapping[1], 1,
            mapping[2], 2,
            mapping[3], 3);
    }

    public char getPermCode() {
        return (char)(mapping[0] +
            (mapping[1] << 2) +
            (mapping[2] << 4) +
            (mapping[3] << 6));
    }
    public void setPermCode(char newCode) {
        mapping[0] = newCode & 3;
        mapping[1] = (newCode >> 2) & 3;
        mapping[2] = (newCode >> 4) & 3;
        mapping[3] = (newCode >> 6) & 3;
    }
    
    public int imageOf(int a) {
        return mapping[a];
    }
    public int preImageOf(int a) {
        return inverse().mapping[a];
    }

    public int compareWith(NPerm other) {
        for (int i = 0; i < 4; i++) {
            if (imageOf(i) < other.imageOf(i))
                return -1;
            if (imageOf(i) > other.imageOf(i))
                return 1;
        }
        return 0;
    }

    public boolean isIdentity() {
        if (mapping[0] != 0)
            return false;
        if (mapping[1] != 1)
            return false;
        if (mapping[2] != 2)
            return false;
        return true;
    }
    public int sign() {
        int sign = 1;
        int i,j;
        for (i=0; i<4; i++)
            for (j=i+1; j<4; j++)
                if (imageOf(i) > imageOf(j))
                    sign = -sign;
        return sign;
    }
    public String toString() {
        return String.valueOf(mapping[0]) + String.valueOf(mapping[1]) +
            String.valueOf(mapping[2]) + String.valueOf(mapping[3]);
    }

    static public NPerm faceOrdering(int face) {
        switch(face) {
            case 0:
                return new NPerm(1,2,3,0);
            case 1:
                return new NPerm(0,2,3,1);
            case 2:
                return new NPerm(0,1,3,2);
            case 3:
                return new NPerm(0,1,2,3);
        }
        return new NPerm();
    }
    static public NPerm edgeOrdering(int edge) {
        switch(edge) {
            case 0:
                return new NPerm(0,1,2,3);
            case 1:
                return new NPerm(0,2,3,1);
            case 2:
                return new NPerm(0,3,1,2);
            case 3:
                return new NPerm(1,2,0,3);
            case 4:
                return new NPerm(1,3,2,0);
            case 5:
                return new NPerm(2,3,0,1);
        }
        return new NPerm();
    }

    static public String faceDescription(NPerm face) {
        return String.valueOf(face.imageOf(0)) +
            String.valueOf(face.imageOf(1)) +
            String.valueOf(face.imageOf(2));
    }
    static public String faceDescription(int face) {
        return faceDescription(faceOrdering(face));
    }
    static public String edgeDescription(NPerm edge) {
        return String.valueOf(edge.imageOf(0)) +
            String.valueOf(edge.imageOf(1));
    }
    static public String edgeDescription(int edge) {
        return edgeDescription(edgeOrdering(edge));
    }

    /**
     * Jython operator overload.
     */
    public Object __len__() {
        return new Integer(4);
    }
    /**
     * Jython operator overload.
     */
    public Object __getitem__(Object obj) {
        if (! (obj instanceof Number))
            return null;
        int index = ((Number)obj).intValue();
        if (index < 0 || index >= 4)
            throw new IndexOutOfBoundsException();
        return new Integer(mapping[index]);
    }
    /**
     * Jython operator overload.
     */
    public Object __mul__(Object obj) {
        if (obj instanceof NPerm)
            return composeWith((NPerm)obj);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __eq__(Object obj) {
        if (obj instanceof NPerm)
            return (compareWith((NPerm)obj) == 0 ? TRUE : FALSE);
        return null;
    }
    /**
     * Jython operator overload.
     */
    public Object __ne__(Object obj) {
        if (obj instanceof NPerm)
            return (compareWith((NPerm)obj) != 0 ? TRUE : FALSE);
        return null;
    }
}

