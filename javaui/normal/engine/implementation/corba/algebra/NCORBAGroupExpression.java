
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

package normal.engine.implementation.corba.algebra;

import normal.engine.implementation.corba.Regina.Algebra.*;
import normal.engine.implementation.corba.*;

public class NCORBAGroupExpression extends CORBAShareableObject
        implements normal.engine.algebra.NGroupExpression {
    public NGroupExpression data;
    public static final Class CORBAClass = NGroupExpression.class;
    public static final Class helperClass = NGroupExpressionHelper.class;

    protected NCORBAGroupExpression(NGroupExpression data) {
        super(data);
        this.data = data;
    }

    public static NCORBAGroupExpression newWrapper(NGroupExpression source) {
        return (source == null ? null : new NCORBAGroupExpression(source));
    }

    public long getNumberOfTerms() {
        return data.getNumberOfTerms();
    }
    public long getGenerator(long index) {
        return data.getGenerator((int)index);
    }
    public long getExponent(long index) {
        return data.getExponent((int)index);
    }
    public void addTermFirst(long generator, long exponent) {
        data.addTermFirst((int)generator, (int)exponent);
    }
    public void addTermLast(long generator, long exponent) {
        data.addTermLast((int)generator, (int)exponent);
    }
    public normal.engine.algebra.NGroupExpression inverse() {
        return NCORBAGroupExpression.newWrapper(data.inverse());
    }
    public normal.engine.algebra.NGroupExpression power(long exponent) {
        return NCORBAGroupExpression.newWrapper(data.power((int)exponent));
    }
    public boolean simplify() {
        return simplify(false);
    }
    public boolean simplify(boolean cyclic) {
        return data.simplify(cyclic);
    }
    public boolean substitute(long generator,
            normal.engine.algebra.NGroupExpression expansion) {
        return substitute(generator, expansion, false);
    }
    public boolean substitute(long generator,
            normal.engine.algebra.NGroupExpression expansion,
            boolean cyclic) {
        return data.substitute((int)generator,
            ((NCORBAGroupExpression)expansion).data, cyclic);
    }
}
