
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

package normal.engine;

/**
 * Represents an object in the calculation engine.
 * The specific implementation will depend on how the engine is being
 * accessed.  See <tt>normal.engine</tt> for further details.
 * <p>
 * See the engine documentation regarding usage of
 * <tt>ShareableObject</tt>s.
 *
 * @see normal.engine
 */
public interface ShareableObject {
    public boolean sameObject(ShareableObject object);
    public void destroy();
    public ShareableObject castAs(Class cls);

    public void writeTextShort();
    public void writeTextLong();
    public String toString();
    public String toStringLong();
}
