
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

/*! \file packet/xmlpacketreaders.h
 *  \brief Deals with parsing XML data for various basic packet types.
 */

#ifndef __REGINA_XMLPACKETREADERS_H
#ifndef __DOXYGEN
#define __REGINA_XMLPACKETREADERS_H
#endif

#include "regina-core.h"
#include "file/xml/xmlpacketreader.h"
#include "packet/container.h"
#include "packet/pdf.h"
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
        Packet* packet;
            /**< The container currently being read. */

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
        XMLAnonRefReader(XMLTreeResolver& resolver, Packet* parent,
            bool anon, std::string label, std::string id);

        virtual Packet* packetToCommit() override;
};

/**
 * An XML packet reader that reads a single container.
 *
 * \ifacespython Not present.
 */
class XMLContainerReader : public XMLPacketReader {
    private:
        Container* container;
            /**< The container currently being read. */

    public:
        /**
         * Creates a new container reader.
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLContainerReader(XMLTreeResolver& resolver, Packet* parent,
            bool anon, std::string label, std::string id);

        virtual Packet* packetToCommit() override;
};

/**
 * An XML packet reader that reads a single PDF packet.
 *
 * \ifacespython Not present.
 */
class XMLPDFReader : public XMLPacketReader {
    private:
        PDF* pdf;
            /**< The PDF packet currently being read. */

    public:
        /**
         * Creates a new PDF reader.
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLPDFReader(XMLTreeResolver& resolver, Packet* parent,
            bool anon, std::string label, std::string id);

        virtual Packet* packetToCommit() override;
        virtual void initialChars(const std::string& chars) override;
};

/**
 * An XML packet reader that reads a single PDF packet using the
 * older second-generation file format.
 *
 * \ifacespython Not present.
 */
class XMLLegacyPDFReader : public XMLPacketReader {
    private:
        PDF* pdf;
            /**< The PDF packet currently being read. */

    public:
        /**
         * Creates a new PDF reader.
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLLegacyPDFReader(XMLTreeResolver& resolver, Packet* parent,
            bool anon, std::string label, std::string id);

        virtual Packet* packetToCommit() override;
        virtual XMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        virtual void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/**
 * An XML packet reader that reads a single script.
 *
 * \ifacespython Not present.
 */
class XMLScriptReader : public XMLPacketReader {
    private:
        Script* script;
            /**< The script currently being read. */

    public:
        /**
         * Creates a new script reader.
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLScriptReader(XMLTreeResolver& resolver, Packet* parent,
            bool anon, std::string label, std::string id);

        virtual Packet* packetToCommit() override;
        virtual XMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        virtual void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/**
 * An XML packet reader that reads a single text packet.
 *
 * \ifacespython Not present.
 */
class XMLTextReader : public XMLPacketReader {
    private:
        Text* text;
            /**< The text packet currently being read. */

    public:
        /**
         * Creates a new text packet reader.
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLTextReader(XMLTreeResolver& resolver, Packet* parent,
            bool anon, std::string label, std::string id);

        virtual Packet* packetToCommit() override;
        virtual void initialChars(const std::string& chars) override;
};

/**
 * An XML packet reader that reads a single text packet using the
 * older second-generation file format.
 *
 * \ifacespython Not present.
 */
class XMLLegacyTextReader : public XMLPacketReader {
    private:
        Text* text;
            /**< The text packet currently being read. */

    public:
        /**
         * Creates a new text packet reader.
         *
         * All parameters are the same as for the parent class XMLPacketReader.
         */
        XMLLegacyTextReader(XMLTreeResolver& resolver, Packet* parent,
            bool anon, std::string label, std::string id);

        virtual Packet* packetToCommit() override;
        virtual XMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        virtual void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

// Inline functions for XMLAnonRefReader

inline Packet* XMLAnonRefReader::packetToCommit() {
    return packet;
}

// Inline functions for XMLContainerReader

inline XMLContainerReader::XMLContainerReader(
        XMLTreeResolver& res, Packet* parent, bool anon,
        std::string label, std::string id) :
        XMLPacketReader(res, parent, anon, std::move(label), std::move(id)),
        container(new Container()) {
}

inline Packet* XMLContainerReader::packetToCommit() {
    return container;
}

// Inline functions for XMLPDFReader

inline XMLPDFReader::XMLPDFReader(
        XMLTreeResolver& res, Packet* parent, bool anon,
        std::string label, std::string id) :
        XMLPacketReader(res, parent, anon, std::move(label), std::move(id)),
        pdf(new PDF()) {
}

inline Packet* XMLPDFReader::packetToCommit() {
    return pdf;
}

// Inline functions for XMLLegacyPDFReader

inline XMLLegacyPDFReader::XMLLegacyPDFReader(
        XMLTreeResolver& res, Packet* parent, bool anon,
        std::string label, std::string id) :
        XMLPacketReader(res, parent, anon, std::move(label), std::move(id)),
        pdf(new PDF()) {
}

inline Packet* XMLLegacyPDFReader::packetToCommit() {
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
        XMLTreeResolver& res, Packet* parent, bool anon,
        std::string label, std::string id) :
        XMLPacketReader(res, parent, anon, std::move(label), std::move(id)),
        script(new Script()) {
}

inline Packet* XMLScriptReader::packetToCommit() {
    return script;
}

// Inline functions for XMLTextReader

inline XMLTextReader::XMLTextReader(
        XMLTreeResolver& res, Packet* parent, bool anon,
        std::string label, std::string id) :
        XMLPacketReader(res, parent, anon, std::move(label), std::move(id)),
        text(new Text()) {
}

inline Packet* XMLTextReader::packetToCommit() {
    return text;
}

inline void XMLTextReader::initialChars(const std::string& chars) {
    text->setText(chars);
}

// Inline functions for XMLLegacyTextReader

inline XMLLegacyTextReader::XMLLegacyTextReader(
        XMLTreeResolver& res, Packet* parent, bool anon,
        std::string label, std::string id) :
        XMLPacketReader(res, parent, anon, std::move(label), std::move(id)),
        text(new Text()) {
}

inline Packet* XMLLegacyTextReader::packetToCommit() {
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
        text->setText(dynamic_cast<XMLCharsReader*>(subReader)->chars());
}

} // namespace regina

#endif

