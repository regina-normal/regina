
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

#include "packet/nxmlpacketreaders.h"

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
            virtual void startElement(const std::string& tagName,
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
        script->addLast(((NXMLCharsReader*)subReader)->getChars());
    else if (subTagName == "var") {
        NScriptVarReader* var = (NScriptVarReader*)subReader;
        if (! var->getName().empty())
            script->addVariable(var->getName(), var->getValue());
    }
}

NXMLPacketReader* NContainer::getXMLReader(NPacket*) {
    return new NXMLContainerReader();
}

NXMLPacketReader* NScript::getXMLReader(NPacket*) {
    return new NXMLScriptReader();
}

NXMLPacketReader* NText::getXMLReader(NPacket*) {
    return new NXMLTextReader();
}

} // namespace regina

