
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

package normal.engine.implementation.corba.progress;

import normal.engine.implementation.corba.Regina.Progress.*;
import normal.engine.implementation.corba.*;

public class NCORBAProgress extends CORBAShareableObject
        implements normal.engine.progress.NProgress {
    public NProgress data;
    public static final Class CORBAClass = NProgress.class;
    public static final Class helperClass = NProgressHelper.class;

    protected NCORBAProgress(NProgress data) {
        super(data);
        this.data = data;
    }

    public static NCORBAProgress newWrapper(NProgress source) {
        return (source == null ? null : new NCORBAProgress(source));
    }

    public boolean hasChanged() {
        return data.hasChanged();
    }
    public boolean isFinished() {
        return data.isFinished();
    }

    public boolean isCancellable() {
        return data.isCancellable();
    }
    public void cancel() {
        data.cancel();
    }
    public boolean isCancelled() {
        return data.isCancelled();
    }

    public String getDescription() {
        return data.getDescription();
    }
    public boolean isPercent() {
        return data.isPercent();
    }
    public double getPercent() {
        return data.getPercent();
    }
}
