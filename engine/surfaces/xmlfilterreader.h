
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

/*! \file surfaces/xmlfilterreader.h
 *  \brief Deals with parsing XML data for normal surface filters.
 */

#ifndef __XMLFILTERREADER_H
#ifndef __DOXYGEN
#define __XMLFILTERREADER_H
#endif

#include "regina-core.h"
#include "packet/xmlpacketreader.h"
#include "surfaces/surfacefilter.h"

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * An XML element reader that reads the specific details of a normal
 * surface filter.  These details are generally contained within a
 * <tt>\<filter\></tt> ... <tt>\</filter\></tt> pair.
 *
 * Generally a subclass of XMLFilterReader will be used to receive and
 * store filters that you care about.  However, if you simply wish to
 * ignore a particular filter (and all of its descendants), you can use
 * class XMLFilterReader itself for the filter(s) you wish to ignore.
 *
 * Routine filter() is used to return the filter that was read; see
 * its documentation for further notes on how the filter should be
 * constructed.
 *
 * \ifacespython Not present.
 */
class REGINA_API XMLFilterReader : public XMLElementReader {
    public:
        /**
         * Creates a new filter element reader.
         */
        XMLFilterReader();

        /**
         * Returns the newly allocated filter that has been read by
         * this element reader.
         *
         * Deallocation of this new filter is not the responsibility of
         * this class.  Once this routine gives a non-zero return value,
         * it should continue to give the same non-zero return value
         * from this point onwards.
         *
         * The default implementation returns 0.
         *
         * @return the filter that has been read, or 0 if filter reading
         * is incomplete, the filter should be ignored or an error
         * occurred.
         */
        virtual SurfaceFilter* filter();
};

/**
 * An XML packet reader that reads a single surface filter.
 * The filter type will be determined by this class and an appropriate
 * XMLFilterReader will be used to process the type-specific details.
 *
 * \pre The parent XML element reader is in fact an XMLPacketReader.
 *
 * \ifacespython Not present.
 */
class REGINA_API XMLFilterPacketReader : public XMLPacketReader {
    private:
        SurfaceFilter* filter_;
            /**< The surface filter currently being read. */
        Packet* parent_;
            /**< The parent packet of the filter currently being read. */

    public:
        /**
         * Creates a new surface filter packet reader.
         *
         * @param newParent the parent packet of the filter to be read,
         * or 0 if this filter is to be tree matriarch.
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         */
        XMLFilterPacketReader(Packet* newParent, XMLTreeResolver& resolver);

        virtual Packet* packet() override;
        virtual XMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        virtual void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

/*@}*/

// Inline functions for XMLFilterReader

inline XMLFilterReader::XMLFilterReader() {
}

inline SurfaceFilter* XMLFilterReader::filter() {
    return 0;
}

// Inline functions for XMLFilterPacketReader

inline XMLFilterPacketReader::XMLFilterPacketReader(Packet* newParent,
        XMLTreeResolver& resolver) :
        XMLPacketReader(resolver), filter_(0), parent_(newParent) {
}

inline Packet* XMLFilterPacketReader::packet() {
    return filter_;
}

} // namespace regina

#endif

