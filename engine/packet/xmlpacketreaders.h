
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

/*! \file packet/xmlpacketreaders.h
 *  \brief Deals with parsing XML data for various basic packet types.
 */

#ifndef __XMLPACKETREADERS_H
#ifndef __DOXYGEN
#define __XMLPACKETREADERS_H
#endif

#include "regina-core.h"
#include "packet/xmlpacketreader.h"
#include "packet/container.h"
#include "packet/pdf.h"
#include "packet/script.h"
#include "packet/text.h"

namespace regina {

/**
 * \weakgroup packet
 * @{
 */

/**
 * An XML packet reader that reads a single container.
 *
 * \ifacespython Not present.
 */
class REGINA_API XMLContainerReader : public XMLPacketReader {
    private:
        Container* container;
            /**< The container currently being read. */

    public:
        /**
         * Creates a new container reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLContainerReader(XMLTreeResolver& resolver);

        virtual Packet* packet() override;
};

/**
 * An XML packet reader that reads a single PDF packet.
 *
 * \ifacespython Not present.
 */
class REGINA_API XMLPDFReader : public XMLPacketReader {
    private:
        PDF* pdf;
            /**< The PDF packet currently being read. */

    public:
        /**
         * Creates a new PDF reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLPDFReader(XMLTreeResolver& resolver);

        virtual Packet* packet() override;
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
class REGINA_API XMLScriptReader : public XMLPacketReader {
    private:
        Script* script;
            /**< The script currently being read. */

    public:
        /**
         * Creates a new script reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLScriptReader(XMLTreeResolver& resolver);

        virtual Packet* packet() override;
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
class REGINA_API XMLTextReader : public XMLPacketReader {
    private:
        Text* text;
            /**< The text packet currently being read. */

    public:
        /**
         * Creates a new text packet reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLTextReader(XMLTreeResolver& resolver);

        virtual Packet* packet() override;
        virtual XMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        virtual void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/*@}*/

// Inline functions for XMLContainerReader

inline XMLContainerReader::XMLContainerReader(XMLTreeResolver& resolver) :
        XMLPacketReader(resolver), container(new Container()) {
}

inline Packet* XMLContainerReader::packet() {
    return container;
}

// Inline functions for XMLPDFReader

inline XMLPDFReader::XMLPDFReader(XMLTreeResolver& resolver) :
        XMLPacketReader(resolver), pdf(new PDF()) {
}

inline Packet* XMLPDFReader::packet() {
    return pdf;
}

// Inline functions for XMLScriptReader

inline XMLScriptReader::XMLScriptReader(XMLTreeResolver& resolver) :
        XMLPacketReader(resolver), script(new Script()) {
}

inline Packet* XMLScriptReader::packet() {
    return script;
}

// Inline functions for XMLTextReader

inline XMLTextReader::XMLTextReader(XMLTreeResolver& resolver) :
        XMLPacketReader(resolver), text(new Text()) {
}

inline Packet* XMLTextReader::packet() {
    return text;
}

inline XMLElementReader* XMLTextReader::startContentSubElement(
        const std::string& subTagName, const regina::xml::XMLPropertyDict&) {
    if (subTagName == "text")
        return new XMLCharsReader();
    else
        return new XMLElementReader();
}

inline void XMLTextReader::endContentSubElement(const std::string& subTagName,
        XMLElementReader* subReader) {
    if (subTagName == "text")
        text->setText(dynamic_cast<XMLCharsReader*>(subReader)->chars());
}

} // namespace regina

#endif

