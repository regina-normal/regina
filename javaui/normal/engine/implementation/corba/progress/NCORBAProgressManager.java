
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

package normal.engine.implementation.corba.progress;

import normal.engine.implementation.corba.Regina.Progress.*;
import normal.engine.implementation.corba.*;

public class NCORBAProgressManager extends CORBAShareableObject
        implements normal.engine.progress.NProgressManager {
    public NProgressManager data;
    public static final Class CORBAClass = NProgressManager.class;
    public static final Class helperClass = NProgressManagerHelper.class;

    protected NCORBAProgressManager(NProgressManager data) {
        super(data);
        this.data = data;
    }

    public static NCORBAProgressManager newWrapper(NProgressManager source) {
        return (source == null ? null : new NCORBAProgressManager(source));
    }

    public boolean isStarted() {
        return data.isStarted();
    }
    public boolean isFinished() {
        return data.isFinished();
    }

    public normal.engine.progress.NProgress getProgress() {
        return NCORBAProgress.newWrapper(data.getProgress());
    }
}
