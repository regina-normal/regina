
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

#include <cstring>

#include "file/nresources.h"

namespace regina {

bool NLocalFileResource::openRead() {
    infile.open(fileName, sysModeRead());
    if (infile.is_open()) {
        openMode = READ;
        return true;
    } else
        return false;
}

bool NLocalFileResource::openWrite() {
    outfile.open(fileName, sysModeWrite());
    if (outfile.is_open()) {
        openMode = WRITE;
        return true;
    } else
        return false;
}

void NLocalFileResource::close() {
    if (openMode == READ)
        infile.close();
    else if (openMode == WRITE)
        outfile.close();
    openMode = CLOSED;
}

long NLocalFileResource::getPosition() {
    if (openMode == READ)
        return infile.tellg();
    else
        return outfile.tellp();
}

void NLocalFileResource::setPosition(long pos) {
    if (openMode == READ)
        infile.seekg(pos);
    else
        outfile.seekp(pos);
}

} // namespace regina

