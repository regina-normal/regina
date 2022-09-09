
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file packet/xmlpacketreaders.h
 *  \brief Deals with parsing XML data for various basic packet types.
 */

#ifndef __REGINA_XMLPACKETREADERS_H
#ifndef __DOXYGEN
#define __REGINA_XMLPACKETREADERS_H
#endif

#include "regina-core.h"
#include "file/xml/xmlpacketreader.h"
#include "packet/attachment.h"
#include "packet/container.h"
#include "packet/script.h"
#include "packet/text.h"

namespace regina {

/**
 * An XML packet reader that resolves an anonref element.
 *
 * Immediately upon construction, this reader will attempt to resolve
 * the given ID.  If successful, the corresopnding packet will be removed
 * from its current location (either the packet tree or the anonymous pool),
 * with the assumption that it will be re-inserted when this anonref element
 * commits its packet.
 *
 * \ifacespython Not present.
 */
class XMLAnonRefReader : public XMLPacketReader {
    private:
        std::shared_ptr<Packet> packet;
            /**< The packet currently being read. */

    public:
        /**
         * Creates a new anonref reader.
         *
         * The \a id argument is the ID that should be resolved (and,
         * after resolution, the packet will retain this ID).
         *
         * If the \a label argument is non-empty, then this will overwrite any
         * existing packet label.  If the \a label argument is empty, then
         * the packet will retain the label that it had before resolution.
         *
         * If \a anon is \c true, then when it is finally committed, the
         * packet will be put back into the anonymous pool (so the use
         * of anonref is redundant but harmless in this case).
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLAnonRefReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id);

        std::shared_ptr<Packet> packetToCommit() override;
};

/**
 * An XML packet reader that reads a single container.
 *
 * \ifacespython Not present.
 */
class XMLContainerReader : public XMLPacketReader {
    private:
        std::shared_ptr<Container> container;
            /**< The container currently being read. */

    public:
        /**
         * Creates a new container reader.
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLContainerReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id);

        std::shared_ptr<Packet> packetToCommit() override;
};

/**
 * An XML packet reader that reads a single file attachment.
 *
 * \ifacespython Not present.
 */
class XMLAttachmentReader : public XMLPacketReader {
    private:
        std::string filename;
            /**< The name of the file attachment. */
        std::shared_ptr<Attachment> attachment;
            /**< The file attachment currently being read. */

    public:
        /**
         * Creates a new attachment reader.
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLAttachmentReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id, const regina::xml::XMLPropertyDict& props);

        std::shared_ptr<Packet> packetToCommit() override;
        void initialChars(const std::string& chars) override;
};

/**
 * An XML packet reader that reads a single PDF attachment using the
 * older second-generation file format.
 *
 * \ifacespython Not present.
 */
class XMLLegacyPDFReader : public XMLPacketReader {
    private:
        std::shared_ptr<Attachment> pdf;
            /**< The PDF attachment currently being read. */

    public:
        /**
         * Creates a new PDF attachment reader.
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLLegacyPDFReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id);

        std::shared_ptr<Packet> packetToCommit() override;
        XMLElementReader* startContentSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/**
 * An XML packet reader that reads a single script.
 *
 * \ifacespython Not present.
 */
class XMLScriptReader : public XMLPacketReader {
    private:
        std::shared_ptr<Script> script;
            /**< The script currently being read. */

    public:
        /**
         * Creates a new script reader.
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLScriptReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id);

        std::shared_ptr<Packet> packetToCommit() override;
        XMLElementReader* startContentSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/**
 * An XML packet reader that reads a single text packet.
 *
 * \ifacespython Not present.
 */
class XMLTextReader : public XMLPacketReader {
    private:
        std::shared_ptr<Text> text;
            /**< The text packet currently being read. */

    public:
        /**
         * Creates a new text packet reader.
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLTextReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id);

        std::shared_ptr<Packet> packetToCommit() override;
        void initialChars(const std::string& chars) override;
};

/**
 * An XML packet reader that reads a single text packet using the
 * older second-generation file format.
 *
 * \ifacespython Not present.
 */
class XMLLegacyTextReader : public XMLPacketReader {
    private:
        std::shared_ptr<Text> text;
            /**< The text packet currently being read. */

    public:
        /**
         * Creates a new text packet reader.
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLLegacyTextReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id);

        std::shared_ptr<Packet> packetToCommit() override;
        XMLElementReader* startContentSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

// Inline functions for XMLAnonRefReader

inline std::shared_ptr<Packet> XMLAnonRefReader::packetToCommit() {
    return packet;
}

// Inline functions for XMLContainerReader

inline XMLContainerReader::XMLContainerReader(
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        std::string label, std::string id) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label),
            std::move(id)),
        container(new Container()) {
}

inline std::shared_ptr<Packet> XMLContainerReader::packetToCommit() {
    return container;
}

// Inline functions for XMLAttachmentReader

inline XMLAttachmentReader::XMLAttachmentReader(
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        std::string label, std::string id,
        const regina::xml::XMLPropertyDict& props) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label),
            std::move(id)),
        filename(props.lookup("filename")), attachment(new Attachment()) {
}

inline std::shared_ptr<Packet> XMLAttachmentReader::packetToCommit() {
    return attachment;
}

// Inline functions for XMLLegacyPDFReader

inline XMLLegacyPDFReader::XMLLegacyPDFReader(
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        std::string label, std::string id) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label),
            std::move(id)),
        pdf(new Attachment()) {
}

inline std::shared_ptr<Packet> XMLLegacyPDFReader::packetToCommit() {
    return pdf;
}

inline XMLElementReader* XMLLegacyPDFReader::startContentSubElement(
        const std::string& subTagName, const regina::xml::XMLPropertyDict&) {
    if (subTagName == "pdf")
        return new XMLCharsReader();
    else
        return new XMLElementReader();
}

// Inline functions for XMLScriptReader

inline XMLScriptReader::XMLScriptReader(
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        std::string label, std::string id) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label),
            std::move(id)),
        script(new Script()) {
}

inline std::shared_ptr<Packet> XMLScriptReader::packetToCommit() {
    return script;
}

// Inline functions for XMLTextReader

inline XMLTextReader::XMLTextReader(
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        std::string label, std::string id) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label),
            std::move(id)),
        text(new Text()) {
}

inline std::shared_ptr<Packet> XMLTextReader::packetToCommit() {
    return text;
}

inline void XMLTextReader::initialChars(const std::string& chars) {
    text->setText(chars);
}

// Inline functions for XMLLegacyTextReader

inline XMLLegacyTextReader::XMLLegacyTextReader(
        XMLTreeResolver& res, std::shared_ptr<Packet> parent, bool anon,
        std::string label, std::string id) :
        XMLPacketReader(res, std::move(parent), anon, std::move(label),
            std::move(id)),
        text(new Text()) {
}

inline std::shared_ptr<Packet> XMLLegacyTextReader::packetToCommit() {
    return text;
}

inline XMLElementReader* XMLLegacyTextReader::startContentSubElement(
        const std::string& subTagName, const regina::xml::XMLPropertyDict&) {
    if (subTagName == "text")
        return new XMLCharsReader();
    else
        return new XMLElementReader();
}

inline void XMLLegacyTextReader::endContentSubElement(
        const std::string& subTagName, XMLElementReader* subReader) {
    if (subTagName == "text")
        text->setText(static_cast<XMLCharsReader*>(subReader)->chars());
}

} // namespace regina

#endif

