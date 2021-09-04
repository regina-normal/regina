
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

/*! \file surfaces/xmlfilterreader.h
 *  \brief Deals with parsing XML data for normal surface filters.
 */

#ifndef __REGINA_XMLFILTERREADER_H
#ifndef __DOXYGEN
#define __REGINA_XMLFILTERREADER_H
#endif

#include "regina-core.h"
#include "file/xml/xmlpacketreader.h"
#include "surfaces/surfacefilter.h"

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * An XML packet reader that reads a single surface filter using the
 * Regina 6.x file format.
 * The filter type will be determined by this class and an appropriate
 * subclassed filter reader will be used to process the type-specific details.
 *
 * \ifacespython Not present.
 */
class XMLLegacyFilterReader : public XMLPacketReader {
    private:
        XMLPacketReader* dataReader_;
            /**< The subclassed filter reader responsible for reading
                 the "real" content. */
        Packet* filter_;
            /**< The filter that was read by \a dataReader_. */

    public:
        /**
         * Creates a new surface filter packet reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLLegacyFilterReader(XMLTreeResolver& resolver);

        virtual Packet* packet() override;
        virtual XMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        virtual void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/**
 * An XML packet reader that reads a plain (non-subclassed) SurfaceFilter
 * using the Regina 7.0 file format.
 *
 * \ifacespython Not present.
 */
class XMLPlainFilterReader : public XMLPacketReader {
    private:
        SurfaceFilter* filter_;
            /**< The filter currently being read. */

    public:
        /**
         * Creates a new surface filter packet reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLPlainFilterReader(XMLTreeResolver& resolver);

        virtual Packet* packet() override;
};

/**
 * An XML packet reader that reads a single SurfaceFilterCombination filter
 * using the Regina 7.0 file format.
 *
 * \ifacespython Not present.
 */
class XMLCombinationFilterReader : public XMLPacketReader {
    private:
        SurfaceFilterCombination* filter_;
            /**< The filter currently being read. */

    public:
        /**
         * Creates a new surface filter packet reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLCombinationFilterReader(XMLTreeResolver& resolver);

        virtual Packet* packet() override;
        XMLElementReader* startContentSubElement(const std::string& subTagName,
                const regina::xml::XMLPropertyDict& props) override;
};

/**
 * An XML packet reader that reads a single SurfaceFilterProperties filter
 * using the Regina 7.0 file format.
 *
 * \ifacespython Not present.
 */
class XMLPropertiesFilterReader : public XMLPacketReader {
    private:
        SurfaceFilterProperties* filter_;
            /**< The filter currently being read. */

    public:
        /**
         * Creates a new surface filter packet reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLPropertiesFilterReader(XMLTreeResolver& resolver);

        virtual Packet* packet() override;
        XMLElementReader* startContentSubElement(const std::string& subTagName,
                const regina::xml::XMLPropertyDict& props) override;
        void endContentSubElement(const std::string& subTagName,
                XMLElementReader* subReader) override;
};

/*@}*/

// Inline functions for XMLLegacyFilterReader

inline XMLLegacyFilterReader::XMLLegacyFilterReader(XMLTreeResolver& resolver) :
        XMLPacketReader(resolver), filter_(nullptr) {
}

inline Packet* XMLLegacyFilterReader::packet() {
    return filter_;
}

// Inline functions for XMLPlainFilterReader:

inline XMLPlainFilterReader::XMLPlainFilterReader(XMLTreeResolver& resolver) :
        XMLPacketReader(resolver), filter_(new SurfaceFilter()) {
}

inline Packet* XMLPlainFilterReader::packet() {
    return filter_;
}

// Inline functions for XMLCombinationFilterReader:

inline XMLCombinationFilterReader::XMLCombinationFilterReader(
        XMLTreeResolver& resolver) :
        XMLPacketReader(resolver), filter_(nullptr) {
}

inline Packet* XMLCombinationFilterReader::packet() {
    return filter_;
}

// Inline functions for XMLPropertiesFilterReader:

inline XMLPropertiesFilterReader::XMLPropertiesFilterReader(
        XMLTreeResolver& resolver) :
        XMLPacketReader(resolver), filter_(new SurfaceFilterProperties()) {
}

inline Packet* XMLPropertiesFilterReader::packet() {
    return filter_;
}

} // namespace regina

#endif

