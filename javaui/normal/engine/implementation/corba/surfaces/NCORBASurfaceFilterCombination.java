
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

package normal.engine.implementation.corba.surfaces;

import normal.engine.implementation.corba.Regina.Surfaces.*;

public class NCORBASurfaceFilterCombination extends NCORBASurfaceFilter
		implements normal.engine.surfaces.NSurfaceFilterCombination {
	public NSurfaceFilterCombination data;
	public static final Class CORBAClass = NSurfaceFilterCombination.class;
	public static final Class helperClass =
		NSurfaceFilterCombinationHelper.class;

    protected NCORBASurfaceFilterCombination(NSurfaceFilterCombination data) {
		super(data);
		this.data = data;
    }

	public static NCORBASurfaceFilterCombination newWrapper(
			NSurfaceFilterCombination source) {
		return (source == null ? null :
			new NCORBASurfaceFilterCombination(source));
	}

	public boolean getUsesAnd() {
		return data.getUsesAnd();
	}
	public void setUsesAnd(boolean value) {
		data.setUsesAnd(value);
	}
}
