
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "property/npropertyholder.h"
#include "file/nfile.h"

void NPropertyHolder::readProperties(NFile& infile) {
    unsigned propType = infile.readUInt();
    while (propType) {
        streampos bookmark = infile.readPos();
        readIndividualProperty(infile, propType);
        infile.setPosition(bookmark);
        propType = infile.readUInt();
    }
}

streampos NPropertyHolder::writePropertyHeader(NFile& outfile,
        unsigned propType) const {
    outfile.writeUInt(propType);
    streampos bookmark = outfile.getPosition();
    outfile.writePos(0);
    return bookmark;
}

void NPropertyHolder::writePropertyFooter(NFile& outfile, streampos bookmark)
        const {
    streampos finalPos = outfile.getPosition();
    outfile.setPosition(bookmark);
    outfile.writePos(finalPos);
    outfile.setPosition(finalPos);
}

void NPropertyHolder::writeAllPropertiesFooter(NFile& outfile) const {
    outfile.writeUInt(0);
}

