
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

package normal.engine.algebra;

import java.math.BigInteger;
import java.util.Enumeration;
import normal.engine.ShareableObject;
import normal.engine.maths.NMatrixInt;

public interface NAbelianGroup extends ShareableObject {
    void addRank();
    void addRank(int extraRank);
    void addTorsionElement(BigInteger degree);
    void addTorsionElement(BigInteger degree, int mult);
    void addTorsionElement(long degree);
    void addTorsionElement(long degree, int mult);
    void addTorsionElements(Enumeration torsion);
    void addGroup(NMatrixInt presentation);
    void addGroup(NAbelianGroup group);

    int getRank();
    int getTorsionRank(BigInteger degree);
    int getTorsionRank(long degree);
    long getNumberOfInvariantFactors();
    BigInteger getInvariantFactor(long index);
}
