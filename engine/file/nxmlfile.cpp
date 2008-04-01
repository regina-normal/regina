
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <cctype>
#include <fstream>
#include "file/nfile.h"
#include "file/nfileinfo.h"
#include "file/nxmlcallback.h"
#include "file/nxmlfile.h"
#include "packet/ncontainer.h"
#include "packet/nxmlpacketreader.h"
#include "utilities/zstream.h"

namespace regina {

namespace {
    /**
     * Reads the outermost \<reginadata ...\> XML element.
     */
    class ReginaDataReader : public regina::NXMLPacketReader {
        private:
            NContainer container;
                /**< Sits above the entire packet tree read from file. */
            bool isReginaData;
                /**< Are we actually reading a \<reginadata ...\> element? */

            regina::xml::XMLParser* parser;
                /**< The current XML parser, or 0 if we have not yet been
                     given this information. */

        public:
            /**
             * Create a new top-level reader.
             */
            ReginaDataReader() : isReginaData(false), parser(0) {
            }

            virtual NPacket* getPacket() {
                if (isReginaData)
                    return &container;
                else
                    return 0;
            }

            virtual void usingParser(regina::xml::XMLParser* p) {
                parser = p;
            }

            virtual void startElement(const std::string& n,
                    const regina::xml::XMLPropertyDict& tagProps,
                    NXMLElementReader*) {
                if (n == "reginadata") {
                    isReginaData = true;

                    // Old files (pre-4.4.1) are stored in latin1, not utf8.
                    regina::xml::XMLPropertyDict::const_iterator it =
                        tagProps.find("engine");
                    if (it != tagProps.end())
                        if (parser && reginaUsedLatin1(it->second.c_str()))
                            parser->switchEncoding(XML_CHAR_ENCODING_8859_1);
                }
            }

            virtual void abort(NXMLElementReader*) {
                // Delete all children of the top-level container.
                while (NPacket* child = container.getFirstTreeChild()) {
                    child->makeOrphan();
                    delete child;
                }
            }

        private:
            /**
             * Did the given version of Regina store data files in latin1
             * instead of utf8, without specifying an encoding?  This
             * covers all versions up to and including 4.4.
             *
             * @param version a Regina version string.
             * @return \c true if and only if the given version stored
             * XML files in latin1.
             */
            bool reginaUsedLatin1(const char* version) {
                // No version is fine.
                // Also any version that does not begin with 0-4 is fine.
                if (version[0] < '0' || version[0] > '4')
                    return false;

                if (version[0] < '4') {
                    // Deal with 0.x .. 3.x.
                    // Unless this is the beginning of a larger number,
                    // this is a bad version.
                    return (version[0] == '0' ||
                        version[1] < '0' || version[1] > '9');
                } else {
                    // The 4.x case is a bit messier to deal with.

                    // If it's the beginning of a larger number, we're fine.
                    if (version[1] >= '0' && version[1] <= '9')
                        return false;

                    // Definitely begins with 4 and 4 alone.
                    // The only way to save ourselves is to have 4.x for
                    // the right x.
                    if (version[1] != '.')
                        return true;

                    // We definitely begin with "4.".
                    // The only good possibilities now are to begin with:
                    // - 4.[number larger than 4]
                    // - 4.4.[digit larger than 1]
                    if (version[2] >= '0' && version[2] <= '3')
                        return (version[2] == '0' ||
                            version[3] < '0' || version[3] > '9');
                    else if (version[2] == '4') {
                        if (version[3] >= '0' && version[3] <= '9')
                            return false;
                        else if (version[3] == '.' &&
                                version[4] >= '1' && version[4] <= '9')
                            return false;
                        else
                            return true;
                    } else if (version[2] >= '5' && version[2] <= '9')
                        return false;
                    else
                        return true;
                }
            }
    };
}

bool writeXMLFile(const char* fileName, NPacket* subtree, bool compressed,
        const char* encoding) {
    if (compressed) {
        CompressionStream out(fileName);
        if (! out)
            return false;
        subtree->writeXMLFile(out, encoding);
    } else {
        std::ofstream out(fileName);
        if (! out)
            return false;
        subtree->writeXMLFile(out, encoding);
    }
    return true;
}

NPacket* readXMLFile(const char* fileName) {
    DecompressionStream in(fileName);
    if (! in)
        return 0;

    ReginaDataReader reader;
    regina::NXMLCallback callback(reader, std::cerr);
    regina::xml::XMLParser::parse_stream(callback, in);

    // See if we read anything.
    // If so, break it away from the top-level container and return it.
    NPacket* p = reader.getPacket();
    if (p) {
        p = p->getFirstTreeChild();
        if (p)
            p->makeOrphan();
        return p;
    } else
        return 0;
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

