
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

package normal.engine.algebra;

import java.math.BigInteger;
import java.util.Enumeration;
import normal.engine.maths.NMatrixInt;

public interface NAbelianGroup {
    public void addRank();
    public void addRank(int extraRank);
    public void addTorsionElement(BigInteger degree);
    public void addTorsionElement(BigInteger degree, int mult);
    public void addTorsionElement(long degree);
    public void addTorsionElement(long degree, int mult);
    public void addTorsionElements(Enumeration torsion);
    public void addGroup(NMatrixInt presentation);
    public void addGroup(NAbelianGroup group);

    public int getRank();
    public int getTorsionRank(BigInteger degree);
    public int getTorsionRank(long degree);
    public long getNumberOfInvariantFactors();
    public BigInteger getInvariantFactor(long index);
}
