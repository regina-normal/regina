
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

/*! \file packet/xmlpacketreader.h
 *  \brief Deals with parsing XML data for individual packets.
 */

#ifndef __REGINA_XMLPACKETREADER_H
#ifndef __DOXYGEN
#define __REGINA_XMLPACKETREADER_H
#endif

#include "regina-core.h"
#include "file/xml/xmlelementreader.h"
#include <memory>

namespace regina {

class Packet;
class XMLTreeResolver;

/**
 * An XML element reader that reads the data for an individual packet.
 *
 * Generally a subclass of XMLPacketReader will be used to receive and
 * store packets that you care about.  However, if you simply wish to
 * ignore a particular packet (and all of its descendants), you can use
 * class XMLPacketReader itself for the packet(s) you wish to ignore.
 *
 * Routine packet() is used to return the packet that was read; see
 * its documentation for further notes on how the packet should be
 * constructed.
 *
 * Routines startSubElement() and endSubElement() should \e not be
 * overridden by derived classes.  They determine whether the subelement
 * is another packet element or a packet tag; if so then they deal with
 * the subelement themselves (packet elements will be read using a new
 * XMLPacketReader of the correct type), and if not then they call
 * startContentSubElement() and endContentSubElement() which \e should
 * be overridden for processing of non-packet XML subelements.
 *
 * If routine abort() is overridden, it \e must at some point call
 * XMLPacketReader::abort().
 *
 * The XML packet reader should read everything that
 * Packet::writeXMLPacketData() writes, excluding the contents of
 * Packet::writeXMLHeader(), Packet::writeXMLTreeData() and
 * Packet::writeXMLFooter() (i.e., excluding the packet open/close XML tags,
 * any of Regina's packet tags, and/or any child packets).
 *
 * The packet reader may assume that its parent packet (if one exists) has
 * already been read from the file.  The packet reader itself should not
 * insert the new packet into the packet tree (the parent reader will do this).
 *
 * If the new packet needs to store pointers to other packets that might not
 * have been read yet (such as a script packet that needs pointers to its
 * variables), then it should queue a new XMLTreeResolutionTask to the
 * XMLTreeResolver that was passed to its constructor.  After the complete data
 * file has been read, XMLTreeResolver::resolve() will run all of its queued
 * tasks, at which point the new packet can resolve any dangling references.
 *
 * \ifacespython Not present.
 */
class XMLPacketReader : public XMLElementReader {
    private:
        std::shared_ptr<Packet> packet_;
            /**< The packet that has been read and constructed.  This is
                 \c null until commit() is called, at which point it is
                 non-null unless an error occurred during reading or
                 reconstruction. */
        bool readingContent_ { false };
            /**< Identifies whether we are currently reading an XML sub-element
                 that should contain data required to reconstruct this packet
                 (as opposed to generic packet content such as packet
                 tags or child packets).  This is set to \c true before
                 calling startContentSubElement(), and is set to \c false
                 again after calling endContentSubElement(). */

    protected:
        XMLTreeResolver& resolver_;
            /**< The master resolver that will be used to fix dangling packet
                 references after the entire XML file has been read. */

        std::shared_ptr<Packet> parent_;
            /**< The location in the packet tree beneath which this packet
                 should be inserted.  This may only be \c null if (i) \a anon
                 is \c true, or (ii) this packet reader represents the root
                 \<regina\> or \<reginadata\> element. */
        bool anon_;
            /**< Identifies whether this packet appears within an
                 anonymous block. */
        std::string label_;
            /**< The label to assign to the packet being read. */
        std::string id_;
            /**< The string ID that uniquely identifies this packet in the
                 XML data file, or the empty string if this packet has no ID. */

    public:
        /**
         * Creates a new packet element reader.
         *
         * @param resolver the master resolver that will be used to fix
         * dangling packet references after the entire XML file has been read.
         * @param parent the location in the packet tree beneath which this
         * packet will be inserted, once it has been constructed.  This \e must
         * be non-null unless (i) \a anon is \c true, or (ii) this packet
         * reader represents the root \<regina\> or \<reginadata\> element.
         * @param anon \c true if this packet appears within an \<anon\> block.
         * @param label the label that will be assigned to this packet,
         * once it has been constructed.  If this is the empty string,
         * the packet label will not be set (which typically means the packet
         * will have the default empty label).
         * @param id the string ID that identifies this packet in the packet
         * tree, as used for cross-referencing between packets in the XML
         * data file, or the empty string if this packet has no ID.
         */
        XMLPacketReader(XMLTreeResolver& resolver,
            std::shared_ptr<Packet> parent, bool anon, std::string label,
            std::string id);

        /**
         * Returns the packet that has been read and constructed by this
         * element reader.
         *
         * This routine will be called at least once for every packet reader.
         * The first time it is called will be after all packet-specific
         * content should have been read (either immediately before processing
         * packet tags and/or child packets, or during endElement() or abort()).
         * See commit() for details on the precise timeline.
         *
         * This routine should therefore assume that it has received all
         * the packet-specific data it will get, and should make its
         * best attempt to construct a packet accordingly.  It may
         * return \c null if this is not possible, in which case the
         * packet being read (and all its descendants) may be dropped from
         * the packet tree.
         *
         * Once this routine gives a non-null return value, this function must
         * continue to give the same return value from this point onwards
         * (however, typically this function would not be called again).
         *
         * The new packet should not be given a packet label, and should not
         * be inserted into the packet tree.  These tasks will be managed by
         * commit().  Likewise, the new packet should not be given any packet
         * tags or child packets.
         *
         * The default implementation returns \c null.
         *
         * @return the packet that has been constructed, or \c null if
         * this is not possible given the data that has been read.
         */
        virtual std::shared_ptr<Packet> packetToCommit();

        /**
         * Used instead of startSubElement() for XML subelements that
         * are not child packets or packet tags.
         *
         * The default implementation returns a new XMLElementReader
         * which can be used to ignore the subelement completely.
         *
         * @param subTagName the name of the subelement opening tag.
         * @param subTagProps the properties associated with the
         * subelement opening tag.
         * @return a newly created element reader that will be used to
         * parse the subelement.  This class should \e not take care of
         * the new reader's destruction; that will be done by the parser.
         */
        virtual XMLElementReader* startContentSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        /**
         * Used instead of endSubElement() for XML subelements that are
         * not child packets or packet tags.
         *
         * The default implementation does nothing.
         *
         * @param subTagName the name of the subelement closing tag.
         * @param subReader the child reader that was used to parse the
         * subelement (this is the reader that was returned by the
         * corresponding startContentSubElement() call).  It is guaranteed
         * that endElement() has already been called upon this child reader
         * and that the child reader has not yet been destroyed.
         */
        virtual void endContentSubElement(const std::string& subTagName,
            XMLElementReader* subReader);

        void endElement() override;
        XMLElementReader* startSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        void endSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
        void abort(XMLElementReader *subReader) override;

    protected:
        /**
         * Finishes off the packet under construction and inserts it
         * into the packet tree.
         *
         * This routine will \e always be called for each packet reader:
         * either when the first tag/packet child is seen, or (if there are
         * no tags or child packets) from endElement(), or (if necessary)
         * from abort().
         *
         * At this point at which this routine is called, the full contents
         * of the current packet should have been read from XML, and should
         * be accessible via the routine packetToCommit().  This routine will:
         *
         * - fetch the packet from packetToCommit(), set its label,
         *   and register its ID with the resolver;
         *
         * - if \a anon_ is \c false, insert the packet in the tree beneath
         *   \a parent_;
         *
         * - if \a anon_ is \c true, add the packet to the resolver's
         *   anonymous pool.
         *
         * Therefore a side-effect of commit() is to ensure that the
         * packet under construction has its ownership managed by some entity.
         *
         * It is safe to call this routine multiple times; once it
         * receives and processes a non-null packet, subsequent calls
         * will do nothing.
         */
        void commit();
};

// Inline functions for XMLPacketReader

inline XMLPacketReader::XMLPacketReader(XMLTreeResolver& resolver,
        std::shared_ptr<Packet> parent, bool anon, std::string label,
        std::string id) :
        resolver_(resolver), parent_(std::move(parent)), anon_(anon),
        label_(std::move(label)), id_(std::move(id)) {
}

inline std::shared_ptr<Packet> XMLPacketReader::packetToCommit() {
    return nullptr;
}

inline XMLElementReader* XMLPacketReader::startContentSubElement(
        const std::string&, const regina::xml::XMLPropertyDict&) {
    return new XMLElementReader();
}

inline void XMLPacketReader::endContentSubElement(const std::string&,
        XMLElementReader*) {
}

} // namespace regina

#endif

