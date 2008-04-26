
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
#include <cstring>
#include <fstream>
#include "engine.h"
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

        public:
            /**
             * Create a new top-level reader.
             */
            ReginaDataReader() : isReginaData(false) {
            }

            virtual NPacket* getPacket() {
                if (isReginaData)
                    return &container;
                else
                    return 0;
            }

            virtual void startElement(const std::string& n,
                    const regina::xml::XMLPropertyDict&, NXMLElementReader*) {
                if (n == "reginadata")
                    isReginaData = true;
            }

            virtual void abort(NXMLElementReader*) {
                // Delete all children of the top-level container.
                while (NPacket* child = container.getFirstTreeChild()) {
                    child->makeOrphan();
                    delete child;
                }
            }
    };

    /**
     * The number of characters by which we expect to have finished the
     * <reginadata ...> opening tag in an old pre-utf8 XML data stream.
     */
    const int regDataOpenBy = 200;

    /**
     * The size of the XML chunks that are sent through to libxml2 for parsing.
     *
     * Since we plan to perform surgery on pre-utf8 data streams, the chunk
     * size must be large enough to catch the regina engine version in the
     * first bite.  In particular, we require regChunkSize > regDataOpenBy.
     */
    const int regChunkSize = 1024;
}

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

NPacket* readXMLFile(const char* fileName) {
    DecompressionStream in(fileName);
    if (! in)
        return 0;

    ReginaDataReader reader;
    regina::NXMLCallback callback(reader, std::cerr);

    // Instead of using the ready-made regina::xml::XMLParser::parse_stream(),
    // we split the stream into parseable chunks manually.  This allows us to
    // perform surgery on pre-utf8 streams and tell the parser that old files
    // actually store their data in latin1 (which the old file format
    // neglected to specify in the XML prologue).
    {
        regina::xml::XMLParser parser(callback);

        char* buf = new char[regChunkSize];
        int chunkRead;
        bool seenFirstChunk = false;
        while (true) {
            // Read in the next chunk.
            for (chunkRead = 0; chunkRead < regChunkSize; chunkRead++) {
                buf[chunkRead] = in.get();
                if (in.eof())
                    break;
            }
            if (chunkRead == 0)
                break;

            // Parse the chunk that has just been read.
            // Interesting things (i.e., surgery) can only happen in the
            // first chunk.
            if (seenFirstChunk) {
                parser.parse_chunk(std::string(buf, chunkRead));
                continue;
            }

            seenFirstChunk = true;

            // This is the first chunk in the data stream.  See if we
            // need to perform surgery upon it.
            //
            // We will be fairly rigid in looking for substrings, since
            // we are only trying to match old pre-utf8 data files.  If
            // future versions of the calculation engine change the
            // opening "signature" then this does not matter, since they
            // will not need surgery anyway.
            //
            // If anything looks out of place, just abort the surgery
            // and send everything straight through to the XML parser,
            // in the hope that either the file is new enough to not
            // need surgery (i.e., it uses utf8 already) or it's old but
            // only uses plain ASCII.

            char tmp = buf[regDataOpenBy];
            buf[regDataOpenBy] = 0;

            char* start = ::strstr(buf, "<reginadata engine=\"");
            if (! start) {
                buf[regDataOpenBy] = tmp;
                parser.parse_chunk(std::string(buf, chunkRead));
                continue;
            }
            start += 20; /* Length of "<reginadata engine=\"" */

            char* finish = ::strchr(start, '"');
            if (finish == 0 || finish == start) {
                // Never found a closing quote, or else the engine version
                // string is empty.
                buf[regDataOpenBy] = tmp;
                parser.parse_chunk(std::string(buf, chunkRead));
                continue;
            }

            // We've found the engine version.
            buf[regDataOpenBy] = tmp;

            tmp = *finish;
            *finish = 0;

            if (versionUsesUTF8(start)) {
                // No need for surgery.
                *finish = tmp;
                parser.parse_chunk(std::string(buf, chunkRead));
                continue;
            }

            // Surgery is required.
            // Make sure there is somewhere to squeeze in an encoding.
            start = ::strstr(buf, "encoding=");
            if (start) {
                // Someone else seems to have done it already.. *shrug*
                *finish = tmp;
                parser.parse_chunk(std::string(buf, chunkRead));
                continue;
            }

            start = ::strstr(buf, "?>");
            if (! start) {
                // Can't find the end of the <?xml ...?> opening sequence!
                *finish = tmp;
                parser.parse_chunk(std::string(buf, chunkRead));
                continue;
            }

            // Good to go.  Parse the chunk with a new encoding squeezed in
            // just before the "?>".
            *finish = tmp;

            parser.parse_chunk(std::string(buf, start - buf));
            parser.parse_chunk(" encoding=\"ISO-8859-1\"");
            parser.parse_chunk(std::string(start, chunkRead - (start - buf)));
        }

        parser.finish();
        delete[] buf;
    }

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

