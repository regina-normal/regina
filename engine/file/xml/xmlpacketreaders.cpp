
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "file/xml/xmlpacketreaders.h"
#include "file/xml/xmltreeresolver.h"
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
                    XMLElementReader*) override {
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
            std::shared_ptr<Script> script_;
            std::string name_;
            std::string valueID_;
                /**< An internal packet ID.  Used by Regina >= 4.95. */
            std::string valueLabel_;
                /**< A packet label.  Used by Regina <= 4.94. */

        public:
            inline VariableResolutionTask(std::shared_ptr<Script> script,
                    std::string name,
                    std::string valueID,
                    std::string valueLabel) :
                    script_(std::move(script)),
                    name_(std::move(name)),
                    valueID_(std::move(valueID)),
                    valueLabel_(std::move(valueLabel)) {
            }

            inline void resolve(const XMLTreeResolver& resolver) override {
                std::shared_ptr<Packet> resolution;
                if (! valueID_.empty())
                    resolution = resolver.resolve(valueID_);
                if ((! resolution) && (! valueLabel_.empty()))
                    resolution = script_->root()->findPacketLabel(valueLabel_);

                script_->addVariable(name_, resolution);
            }
    };
}

namespace {
    void extractAttachmentFromBase64(Attachment& att, std::string base64,
            std::string filename) {
        // Strip out the whitespace.
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
            att.reset();
            return;
        }

        // Convert from base64.
        char* data;
        size_t dataLen;
        if (base64Decode(base64.c_str(), out - base64.begin(), &data, &dataLen))
            att.reset(data, dataLen, Attachment::OWN_NEW, filename);
        else
            att.reset();
    }
}

void XMLAttachmentReader::initialChars(const std::string& chars) {
    extractAttachmentFromBase64(*attachment, chars, filename);
}

void XMLLegacyPDFReader::endContentSubElement(const std::string& subTagName,
        XMLElementReader* subReader) {
    if (subTagName == "pdf") {
        extractAttachmentFromBase64(*pdf,
            static_cast<XMLCharsReader*>(subReader)->chars(),
            "attachment.pdf");
    }
}

XMLElementReader* XMLScriptReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == "code" || subTagName == "text")
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
    if (subTagName == "code" || subTagName == "text")
        script->setText(static_cast<XMLCharsReader*>(subReader)->chars());
    else if (subTagName == "line") { // Old-style
        script->append(static_cast<XMLCharsReader*>(subReader)->chars());
        script->append("\n");
    } else if (subTagName == "var") {
        auto* var = static_cast<ScriptVarReader*>(subReader);
        if (! var->getName().empty())
            resolver_.queueTask(new VariableResolutionTask(
                script, var->getName(),
                var->getValueID(), var->getValueLabel()));
    }
}

XMLAnonRefReader::XMLAnonRefReader(
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        std::string label, std::string id) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label),
            std::move(id)) {
    packet = res.resolve(id_);
}

} // namespace regina

