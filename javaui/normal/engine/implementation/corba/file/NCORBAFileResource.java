
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

package normal.engine.implementation.corba.file;

import java.io.*;
import normal.engine.implementation.corba.Regina.File.*;

public class NCORBAFileResource extends _NRandomAccessResourceImplBase {
    /**
     * The file containing the data that forms this resource.
     */
    private RandomAccessFile file;

    /**
     * The name of the file whose data forms this resource.
     */
    private String fileName;

    /**
     * The mode in which the file is currently opened.  This must be one
     * of the predefined mode constants in class
     * <tt>normal.engine.file.NFile</tt>.
     */
    private int openMode;

    /**
     * Create a new resource referring to the data stored in the
     * given file.
     *
     * @param fileName the name of the file whose data forms this
     * resource.
     */
    public NCORBAFileResource(String fileName) {
        this.fileName = fileName;
        this.file = null;
        this.openMode = normal.engine.file.NFile.CLOSED;
    }

    public boolean openRead() {
        try {
            if (file != null)
                file.close();
            file = new RandomAccessFile(fileName, "r");
            openMode = normal.engine.file.NFile.READ;
            return true;
        } catch (IOException e) {
            file = null;
            openMode = normal.engine.file.NFile.CLOSED;
            return false;
        }
    }

    public boolean openWrite() {
        try {
            if (file != null)
                file.close();
            file = new RandomAccessFile(fileName, "rw");
            openMode = normal.engine.file.NFile.WRITE;
            return true;
        } catch (IOException e) {
            file = null;
            openMode = normal.engine.file.NFile.CLOSED;
            return false;
        }
    }

    public void close() {
        try {
            if (file != null)
                file.close();
        } catch (Throwable th) {}

        file = null;
        openMode = normal.engine.file.NFile.CLOSED;
    }

    public int getOpenMode() {
        return openMode;
    }

    public char getChar() {
        try {
            return (char)file.read();
        } catch (Throwable th) {
            return 0;
        }
    }

    public void putChar(char c) {
        try {
            file.write(c);
        } catch (Throwable th) {
        }
    }

    public int getPosition() {
        try {
            return (int)file.getFilePointer();
        } catch (Throwable th) {
            return 0;
        }
    }

    public void setPosition(int pos) {
        try {
            file.seek(pos);
        } catch (Throwable th) {
        }
    }
}
