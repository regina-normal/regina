
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file packet/nxmlpacketreaders.h
 *  \brief Deals with parsing XML data for various basic packet types.
 */

#ifndef __NXMLPACKETREADERS_H
#ifndef __DOXYGEN
#define __NXMLPACKETREADERS_H
#endif

#include "regina-core.h"
#include "packet/nxmlpacketreader.h"
#include "packet/ncontainer.h"
#include "packet/npdf.h"
#include "packet/nscript.h"
#include "packet/ntext.h"

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
class REGINA_API NXMLContainerReader : public NXMLPacketReader {
    private:
        NContainer* container;
            /**< The container currently being read. */

    public:
        /**
         * Creates a new container reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        NXMLContainerReader(NXMLTreeResolver& resolver);

        virtual NPacket* packet();
};

/**
 * An XML packet reader that reads a single PDF packet.
 *
 * \ifacespython Not present.
 */
class REGINA_API NXMLPDFReader : public NXMLPacketReader {
    private:
        NPDF* pdf;
            /**< The PDF packet currently being read. */

    public:
        /**
         * Creates a new PDF reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        NXMLPDFReader(NXMLTreeResolver& resolver);

        virtual NPacket* packet();
        virtual NXMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        virtual void endContentSubElement(const std::string& subTagName,
            NXMLElementReader* subReader);
};

/**
 * An XML packet reader that reads a single script.
 *
 * \ifacespython Not present.
 */
class REGINA_API NXMLScriptReader : public NXMLPacketReader {
    private:
        NScript* script;
            /**< The script currently being read. */

    public:
        /**
         * Creates a new script reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        NXMLScriptReader(NXMLTreeResolver& resolver);

        virtual NPacket* packet();
        virtual NXMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        virtual void endContentSubElement(const std::string& subTagName,
            NXMLElementReader* subReader);
};

/**
 * An XML packet reader that reads a single text packet.
 *
 * \ifacespython Not present.
 */
class REGINA_API NXMLTextReader : public NXMLPacketReader {
    private:
        NText* text;
            /**< The text packet currently being read. */

    public:
        /**
         * Creates a new text packet reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        NXMLTextReader(NXMLTreeResolver& resolver);

        virtual NPacket* packet();
        virtual NXMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        virtual void endContentSubElement(const std::string& subTagName,
            NXMLElementReader* subReader);
};

/*@}*/

// Inline functions for NXMLContainerReader

inline NXMLContainerReader::NXMLContainerReader(NXMLTreeResolver& resolver) :
        NXMLPacketReader(resolver), container(new NContainer()) {
}

inline NPacket* NXMLContainerReader::packet() {
    return container;
}

// Inline functions for NXMLPDFReader

inline NXMLPDFReader::NXMLPDFReader(NXMLTreeResolver& resolver) :
        NXMLPacketReader(resolver), pdf(new NPDF()) {
}

inline NPacket* NXMLPDFReader::packet() {
    return pdf;
}

// Inline functions for NXMLScriptReader

inline NXMLScriptReader::NXMLScriptReader(NXMLTreeResolver& resolver) :
        NXMLPacketReader(resolver), script(new NScript()) {
}

inline NPacket* NXMLScriptReader::packet() {
    return script;
}

// Inline functions for NXMLTextReader

inline NXMLTextReader::NXMLTextReader(NXMLTreeResolver& resolver) :
        NXMLPacketReader(resolver), text(new NText()) {
}

inline NPacket* NXMLTextReader::packet() {
    return text;
}

inline NXMLElementReader* NXMLTextReader::startContentSubElement(
        const std::string& subTagName, const regina::xml::XMLPropertyDict&) {
    if (subTagName == "text")
        return new NXMLCharsReader();
    else
        return new NXMLElementReader();
}

inline void NXMLTextReader::endContentSubElement(const std::string& subTagName,
        NXMLElementReader* subReader) {
    if (subTagName == "text")
        text->setText(dynamic_cast<NXMLCharsReader*>(subReader)->getChars());
}

} // namespace regina

#endif

