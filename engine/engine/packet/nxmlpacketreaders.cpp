
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

#include "packet/nxmlpacketreader.h"
#include "packet/ncontainer.h"
#include "packet/nscript.h"
#include "packet/ntext.h"

// TODO: remove once individual routines are all implemented.
#include "surfaces/nnormalsurfacelist.h"
#include "surfaces/nsurfacefilter.h"
#include "angle/nanglestructurelist.h"

namespace regina {

/**
 * A unique namespace containing a variety of packet readers.
 */
namespace {
    class NContainerReader : public NXMLPacketReader {
        private:
            NContainer* container;

        public:
            NContainerReader() : container(new NContainer()) {
            }

            virtual NPacket* getPacket() {
                return container;
            }
    };

    class NTextReader : public NXMLPacketReader {
        private:
            NText* text;

        public:
            NTextReader() : text(new NText()) {
            }

            virtual NXMLElementReader* startContentSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "text")
                    return new NXMLTextReader();
                else
                    return new NXMLElementReader();
            }

            virtual void endContentSubElement(const std::string& subTagName,
                    NXMLElementReader* subReader) {
                if (subTagName == "text")
                    text->setText(((NXMLTextReader*)subReader)->getText());
            }

            virtual NPacket* getPacket() {
                return text;
            }
    };

    class NScriptVarReader : public NXMLElementReader {
        private:
            std::string name, value;

        public:
            virtual void startElement(const std::string& tagName,
                    const regina::xml::XMLPropertyDict& props,
                    NXMLElementReader*) {
                name = props.lookup("name", "");
                value = props.lookup("value", "");
            }

            const std::string& getName() {
                return name;
            }

            const std::string& getValue() {
                return value;
            }
    };

    class NScriptReader : public NXMLPacketReader {
        private:
            NScript* script;

        public:
            NScriptReader() : script(new NScript()) {
            }

            virtual NXMLElementReader* startContentSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "line")
                    return new NXMLTextReader();
                else if (subTagName == "var")
                    return new NScriptVarReader();
                else
                    return new NXMLElementReader();
            }

            virtual void endContentSubElement(const std::string& subTagName,
                    NXMLElementReader* subReader) {
                if (subTagName == "line")
                    script->addLast(((NXMLTextReader*)subReader)->getText());
                else if (subTagName == "var") {
                    NScriptVarReader* var = (NScriptVarReader*)subReader;
                    if (! var->getName().empty())
                        script->addVariable(var->getName(), var->getValue());
                }
            }
            
            virtual NPacket* getPacket() {
                return script;
            }
    };
}

NXMLPacketReader* NContainer::getXMLReader(NPacket*) {
    return new NContainerReader();
}

NXMLPacketReader* NText::getXMLReader(NPacket*) {
    return new NTextReader();
}

NXMLPacketReader* NScript::getXMLReader(NPacket*) {
    return new NScriptReader();
}

NXMLPacketReader* NNormalSurfaceList::getXMLReader(NPacket*) {
    // TODO: implement real packet reader
    return new NXMLPacketReader();
}

NXMLPacketReader* NSurfaceFilter::getXMLReader(NPacket*) {
    // TODO: implement real packet reader
    return new NXMLPacketReader();
}

NXMLPacketReader* NAngleStructureList::getXMLReader(NPacket*) {
    // TODO: implement real packet reader
    return new NXMLPacketReader();
}

} // namespace regina

