
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

#include <fstream>
#include "file/nfile.h"
#include "file/nfileinfo.h"
#include "file/nxmlfile.h"
#include "packet/npacket.h"
#include "utilities/zstream.h"

// TODO: remove ntext.h
#include "packet/ntext.h"

namespace regina {

bool writeXMLFile(const char* fileName, NPacket* subtree, bool compressed) {
    if (compressed) {
        CompressionStream out(fileName);
        if (! out)
            return false;
        subtree->writeXMLFile(out);
    } else {
        std::ofstream out(fileName);
        if (! out)
            return false;
        subtree->writeXMLFile(out);
    }
    return true;
}

NPacket* readXMLFile(const char*) {
    // TODO: implement XML file read!
    NText* oops = new NText("XML reading is not yet implemented.");
    oops->setPacketLabel("XML Data");
    return oops;
}

NPacket* readFileMagic(const std::string& fileName) {
    NFileInfo* info = NFileInfo::identify(fileName);
    if (! info)
        return 0;

    NPacket* ans;
    if (info->getType() == NFileInfo::TYPE_XML)
        ans = readXMLFile(fileName.c_str());
    else if (info->getType() == NFileInfo::TYPE_BINARY)
        ans = readFromFile(fileName.c_str());
    else
        ans = 0;

    delete info;
    return ans;
}

} // namespace regina

