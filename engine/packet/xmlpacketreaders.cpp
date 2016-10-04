
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

#include "packet/xmlpacketreaders.h"
#include "packet/xmltreeresolver.h"
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
    class ScriptVarReader : public XMLElementReader {
        private:
            std::string name, valueID, valueLabel;

        public:
            inline void startElement(const std::string& /* tagName */,
                    const regina::xml::XMLPropertyDict& props,
                    XMLElementReader*) {
                name = props.lookup("name");
                valueID = props.lookup("valueid");
                valueLabel = props.lookup("value");
            }

            inline const std::string& getName() {
                return name;
            }

            inline const std::string& getValueID() {
                return valueID;
            }

            inline const std::string& getValueLabel() {
                return valueLabel;
            }
    };

    /**
     * A resolution task that, after the entire XML file has been read,
     * will bind a script variable to its corresponding packet reference.
     */
    class VariableResolutionTask : public XMLTreeResolutionTask {
        private:
            Script* script_;
            std::string name_;
            std::string valueID_;
                /**< An internal packet ID.  Used by Regina >= 4.95. */
            std::string valueLabel_;
                /**< A packet label.  Used by Regina <= 4.94. */

        public:
            inline VariableResolutionTask(Script* script,
                    const std::string& name,
                    const std::string& valueID,
                    const std::string& valueLabel) :
                    script_(script), name_(name), valueID_(valueID),
                    valueLabel_(valueLabel) {
            }

            inline void resolve(const XMLTreeResolver& resolver) {
                Packet* resolution = 0;
                if (! valueID_.empty()) {
                    XMLTreeResolver::IDMap::const_iterator it =
                        resolver.ids().find(valueID_);
                    resolution = (it == resolver.ids().end() ? 0 : it->second);
                }
                if ((! resolution) && (! valueLabel_.empty()))
                    resolution = script_->root()->findPacketLabel(valueLabel_);

                script_->addVariable(name_, resolution);
            }
    };
}

XMLElementReader* XMLPDFReader::startContentSubElement(
        const std::string& subTagName, const regina::xml::XMLPropertyDict&) {
    if (subTagName == "pdf")
        return new XMLCharsReader();
    else
        return new XMLElementReader();
}

void XMLPDFReader::endContentSubElement(const std::string& subTagName,
        XMLElementReader* subReader) {
    if (subTagName == "pdf") {
        std::string base64 = dynamic_cast<XMLCharsReader*>(subReader)->
            chars();

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
            pdf->reset(data, dataLen, PDF::OWN_NEW);
        else
            pdf->reset();
    }
}

XMLElementReader* XMLScriptReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == "text")
        return new XMLCharsReader();
    else if (subTagName == "line") // Old-style
        return new XMLCharsReader();
    else if (subTagName == "var")
        return new ScriptVarReader();
    else
        return new XMLElementReader();
}

void XMLScriptReader::endContentSubElement(const std::string& subTagName,
        XMLElementReader* subReader) {
    if (subTagName == "text")
        script->setText(dynamic_cast<XMLCharsReader*>(subReader)->chars());
    else if (subTagName == "line") { // Old-style
        script->append(dynamic_cast<XMLCharsReader*>(subReader)->chars());
        script->append("\n");
    } else if (subTagName == "var") {
        ScriptVarReader* var = dynamic_cast<ScriptVarReader*>(subReader);
        if (! var->getName().empty())
            resolver_.queueTask(new VariableResolutionTask(
                script, var->getName(),
                var->getValueID(), var->getValueLabel()));
    }
}

XMLPacketReader* Container::xmlReader(Packet*,
        XMLTreeResolver& resolver) {
    return new XMLContainerReader(resolver);
}

XMLPacketReader* PDF::xmlReader(Packet*, XMLTreeResolver& resolver) {
    return new XMLPDFReader(resolver);
}

XMLPacketReader* Script::xmlReader(Packet*, XMLTreeResolver& resolver) {
    return new XMLScriptReader(resolver);
}

XMLPacketReader* Text::xmlReader(Packet*, XMLTreeResolver& resolver) {
    return new XMLTextReader(resolver);
}

} // namespace regina

