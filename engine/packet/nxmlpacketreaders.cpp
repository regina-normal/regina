
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "packet/nxmlpacketreaders.h"
#include "utilities/base64.h"
#include <cctype>
#include <string>

namespace regina {

/**
 * A unique namespace containing various task-specific packet readers.
 */
namespace {
    /**
     * Reads a single script variable and its value.
     */
    class NScriptVarReader : public NXMLElementReader {
        private:
            std::string name, value;

        public:
            virtual void startElement(const std::string& /* tagName */,
                    const regina::xml::XMLPropertyDict& props,
                    NXMLElementReader*) {
                name = props.lookup("name");
                value = props.lookup("value");
            }

            const std::string& getName() {
                return name;
            }

            const std::string& getValue() {
                return value;
            }
    };
}

NXMLElementReader* NXMLPDFReader::startContentSubElement(
        const std::string& subTagName, const regina::xml::XMLPropertyDict&) {
    if (subTagName == "pdf")
        return new NXMLCharsReader();
    else
        return new NXMLElementReader();
}

void NXMLPDFReader::endContentSubElement(const std::string& subTagName,
        NXMLElementReader* subReader) {
    if (subTagName == "pdf") {
        std::string base64 = dynamic_cast<NXMLCharsReader*>(subReader)->
            getChars();

        // Strip out whitespace.
        std::string::iterator in = base64.begin();
        std::string::iterator out = base64.begin();
        while (in != base64.end()) {
            if (::isspace(*in))
                ++in;
            else {
                if (in == out) {
                    ++in;
                    ++out;
                } else
                    *out++ = *in++;
            }
        }

        // Is there any data at all?
        if (out == base64.begin()) {
            pdf->reset();
            return;
        }

        // Convert from base64.
        char* data;
        size_t dataLen;
        if (base64Decode(base64.c_str(), out - base64.begin(), &data, &dataLen))
            pdf->reset(data, dataLen, NPDF::OWN_NEW);
        else
            pdf->reset();
    }
}

NXMLElementReader* NXMLScriptReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == "line")
        return new NXMLCharsReader();
    else if (subTagName == "var")
        return new NScriptVarReader();
    else
        return new NXMLElementReader();
}

void NXMLScriptReader::endContentSubElement(const std::string& subTagName,
        NXMLElementReader* subReader) {
    if (subTagName == "line")
        script->addLast(dynamic_cast<NXMLCharsReader*>(subReader)->getChars());
    else if (subTagName == "var") {
        NScriptVarReader* var = dynamic_cast<NScriptVarReader*>(subReader);
        if (! var->getName().empty())
            script->addVariable(var->getName(), var->getValue());
    }
}

NXMLPacketReader* NContainer::getXMLReader(NPacket*) {
    return new NXMLContainerReader();
}

NXMLPacketReader* NPDF::getXMLReader(NPacket*) {
    return new NXMLPDFReader();
}

NXMLPacketReader* NScript::getXMLReader(NPacket*) {
    return new NXMLScriptReader();
}

NXMLPacketReader* NText::getXMLReader(NPacket*) {
    return new NXMLTextReader();
}

} // namespace regina

