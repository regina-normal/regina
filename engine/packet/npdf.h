
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file npdf.h
 *  \brief A packet that contains a PDF document.
 */

#ifndef __NPDF_H
#ifndef __DOXYGEN
#define __NPDF_H
#endif

#include <cstdlib>
#include <cstring>
#include "packet/npacket.h"

namespace regina {

class NXMLPacketReader;

/**
 * \weakgroup packet
 * @{
 */

/**
 * A packet that can hold a PDF document.
 *
 * This packet may or may not contain a PDF document at any given time.
 * This state can be changed by calling reset().
 */
class NPDF : public NPacket {
    public:
        /**
         * Describes how a PDF packet should claim ownership of a block
         * of binary data.
         */
        enum OwnershipPolicy {
            /**
             * The packet should claim ownership of the block, and
             * should assume that it was allocated using \a malloc().
             */
            OWN_MALLOC,
            /**
             * The packet should claim ownership of the block, and
             * should assume that it was allocated using \c new[].
             */
            OWN_NEW,
            /**
             * The packet should not claim ownership of the block, but
             * should instead make its own deep copy.
             */
            DEEP_COPY
        };

    private:
        char* data_;
            /**< The binary data of the PDF document that is stored in
                 this packet, or 0 if no PDF document is currently stored. */
        size_t size_;
            /**< The size of the binary data in bytes, or 0 if no PDF
                 document is currently stored. */
        OwnershipPolicy alloc_;
            /**< Describes how the binary data (if any) was allocated; this
                 must be either \a OWN_MALLOC or \a OWN_NEW. */

    public:
        static const int packetType;

        /**
         * Creates a PDF packet with no document stored.
         */
        NPDF();

        /**
         * Creates a packet to store the given PDF data.
         *
         * The \a data array must contain a full PDF document as a
         * block of binary data.
         *
         * The \a alloc argument shows if/how this packet claims ownership of
         * the data.  In particular, unless \a alloc is \a DEEP_COPY, this
         * packet will claim ownership of the given data block and will
         * deallocate it when the packet is destroyed.  If \a alloc is
         * \a DEEP_COPY then the given block of data will not be modified in
         * any way.
         *
         * It is possible to pass a null pointer as the data array, in
         * which case the new packet will have no PDF document stored.
         *
         * \ifacespython Not present.
         *
         * @param data the block of binary data that forms the PDF document,
         * or \c null if no document is to be stored.
         * @param size the number of bytes in this block of binary data;
         * if actual data is passed (i.e., \a data is not \c null) then
         * this must be strictly positive.
         * @param alloc describes if/how this packet should claim ownership
         * of the given block of data; see the notes above for details.
         */
        NPDF(char* data, size_t size, OwnershipPolicy alloc);

        /**
         * Destroys this PDF packet and deallocates data if required.
         */
        ~NPDF();

        /**
         * Returns a pointer to the block of raw data that forms this
         * PDF document.  The number of bytes in this block can be found
         * by calling size().
         *
         * If no PDF document is currently stored, this routine will
         * return a null pointer.
         *
         * \ifacespython Not present.
         *
         * @return the raw PDF data.
         */
        const char* data() const;

        /**
         * Returns the size of this PDF document in bytes.
         *
         * If no PDF document is currently stored, this routine will
         * return zero.
         *
         * @return the number of bytes.
         */
        size_t size() const;

        /**
         * Empties this PDF packet so that no document is stored.
         *
         * The old data will be deallocated if required.
         */
        void reset();

        /**
         * Refills this PDF packet with the given PDF data.
         * The old data will be deallocated if required.
         *
         * This routine behaves like the class constructor; see the
         * constructor documentation for details.
         *
         * \ifacespython Not present.
         *
         * @param data the block of binary data that forms the new PDF
         * document, or \c null if no document is to be stored.
         * @param size the number of bytes in this new block of binary data;
         * if actual data is passed (i.e., \a data is not \c null) then
         * this must be strictly positive.
         * @param alloc describes if/how this packet should claim ownership
         * of the given block of data; see the notes above for details.
         */
        void reset(char* data, size_t size, OwnershipPolicy alloc);

        virtual int getPacketType() const;
        virtual std::string getPacketTypeName() const;

        virtual void writeTextShort(std::ostream& out) const;
        static NXMLPacketReader* getXMLReader(NPacket* parent);
        virtual void writePacket(NFile& out) const;
        static NPDF* readPacket(NFile& in, NPacket* parent);
        virtual bool dependsOnParent() const;

    protected:
        virtual NPacket* internalClonePacket(NPacket* parent) const;
        virtual void writeXMLPacketData(std::ostream& out) const;
};

/*@}*/

// Inline functions for NPDF

inline NPDF::NPDF() : data_(0), size_(0), alloc_(OWN_NEW) {
}

inline NPDF::NPDF(char* data, size_t size, OwnershipPolicy alloc) :
        data_(data), size_(size), alloc_(alloc) {
    if (alloc_ == DEEP_COPY) {
        if (data_) {
            data_ = static_cast<char*>(::malloc(size_));
            ::memcpy(data_, static_cast<const char*>(data), size_);
        }
        alloc_ = OWN_MALLOC;
    } else if (! data_)
        size_ = 0;
}

inline NPDF::~NPDF() {
    if (data_) {
        if (alloc_ == OWN_MALLOC)
            ::free(data_);
        else
            delete[] data_;
    }
}

inline const char* NPDF::data() const {
    return data_;
}

inline size_t NPDF::size() const {
    return size_;
}

inline void NPDF::writeTextShort(std::ostream& o) const {
    o << "PDF packet (" << size_ << (size_ == 1 ? " byte)" : " bytes)");
}

inline bool NPDF::dependsOnParent() const {
    return false;
}

inline NPacket* NPDF::internalClonePacket(NPacket*) const {
    return new NPDF(data_, size_, DEEP_COPY);
}

} // namespace regina

#endif

