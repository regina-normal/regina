
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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

/*! \file nxmlfilterreader.h
 *  \brief Deals with parsing XML data for normal surface filters.
 */

#ifndef __NXMLFILTERREADER_H
#ifndef __DOXYGEN
#define __NXMLFILTERREADER_H
#endif

#include "packet/nxmlpacketreader.h"
#include "surfaces/nsurfacefilter.h"

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
 * Generally a subclass of NXMLFilterReader will be used to receive and
 * store filters that you care about.  However, if you simply wish to
 * ignore a particular filter (and all of its descendants), you can use
 * class NXMLFilterReader itself for the filter(s) you wish to ignore.
 *
 * Routine getFilter() is used to return the filter that was read; see
 * its documentation for further notes on how the filter should be
 * constructed.
 *
 * \ifaces Not present.
 */
class NXMLFilterReader : public NXMLElementReader {
    public:
        /**
         * Creates a new filter element reader.
         */
        NXMLFilterReader();

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
        virtual NSurfaceFilter* getFilter();
};

/**
 * An XML packet reader that reads a single surface filter.
 * The filter type will be determined by this class and an appropriate
 * NXMLFilterReader will be used to process the type-specific details.
 *
 * \pre The parent XML element reader is in fact an NXMLPacketReader.
 *
 * \ifaces Not present.
 */
class NXMLFilterPacketReader : public NXMLPacketReader {
    private:
        NSurfaceFilter* filter;
            /**< The surface filter currently being read. */
        NPacket* parent;
            /**< The parent packet of the filter currently being read. */

    public:
        /**
         * Creates a new surface filter packet reader.
         *
         * @param newParent the parent packet of the filter to be read,
         * or 0 if this filter is to be tree matriarch.
         */
        NXMLFilterPacketReader(NPacket* newParent);

        virtual NPacket* getPacket();
        virtual NXMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        virtual void endContentSubElement(const std::string& subTagName,
            NXMLElementReader* subReader);
};

/*@}*/

// Inline functions for NXMLFilterReader

inline NXMLFilterReader::NXMLFilterReader() {
}

inline NSurfaceFilter* NXMLFilterReader::getFilter() {
    return 0;
}

// Inline functions for NXMLFilterPacketReader

inline NXMLFilterPacketReader::NXMLFilterPacketReader(NPacket* newParent) :
        filter(0), parent(newParent) {
}

inline NPacket* NXMLFilterPacketReader::getPacket() {
    return filter;
}

} // namespace regina

#endif

