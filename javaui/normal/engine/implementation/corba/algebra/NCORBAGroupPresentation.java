
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

package normal.engine.implementation.corba.algebra;

import normal.engine.implementation.corba.Regina.Algebra.*;
import normal.engine.implementation.corba.*;

public class NCORBAGroupPresentation extends CORBAShareableObject
        implements normal.engine.algebra.NGroupPresentation {
    public NGroupPresentation data;
    public static final Class CORBAClass = NGroupPresentation.class;
    public static final Class helperClass = NGroupPresentationHelper.class;

    protected NCORBAGroupPresentation(NGroupPresentation data) {
        super(data);
        this.data = data;
    }

    public static NCORBAGroupPresentation newWrapper(
            NGroupPresentation source) {
        return (source == null ? null : new NCORBAGroupPresentation(source));
    }

    public long addGenerator() {
        return addGenerator(1);
    }
    public long addGenerator(long numToAdd) {
        return data.addGenerator((int)numToAdd);
    }
    public void addRelation(normal.engine.algebra.NGroupExpression rel) {
        data.addRelation(((NCORBAGroupExpression)rel).data);
    }
    public long getNumberOfGenerators() {
        return data.getNumberOfGenerators();
    }
    public long getNumberOfRelations() {
        return data.getNumberOfRelations();
    }
    public normal.engine.algebra.NGroupExpression getRelation(long index) {
        return NCORBAGroupExpression.newWrapper(data.getRelation((int)index));
    }
    public boolean intelligentSimplify() {
        return data.intelligentSimplify();
    }
    public String recogniseGroup() {
        return data.recogniseGroup();
    }
}
