
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

/*! \file packet/attachment.h
 *  \brief A packet that contains an arbitrary file attachment.
 */

#ifndef __REGINA_ATTACHMENT_H
#ifndef __DOXYGEN
#define __REGINA_ATTACHMENT_H
#endif

#include <cstdlib>
#include <cstring>
#include "regina-core.h"
#include "packet/packet.h"

namespace regina {

/**
 * A packet that can hold an arbitrary file attachment.
 *
 * The attachment is stored as a block of data in memory.  If the
 * attachment was originally read from the filesystem, this link is not
 * maintained (i.e., if the source file subsequently changes, the attachment
 * data stored in this packet will \e not change to reflect this).
 *
 * This packet may or may not contain an attachment at any given time.
 * This can be tested by calling isNull(), and can be changed by calling
 * reset().
 *
 * This class does not distinguish between an empty file attachment versus
 * no attachment at all.  In both cases, isNull() will return \c true.
 *
 * Like all packet types, this class does not support C++ move semantics
 * since this would interfere with the structure of the packet tree.
 * It does support copy construction, copy assignment and swaps; however,
 * these operations only copy/swap the mathematical content, not the packet
 * infrastructure (e.g., they do not touch packet labels, or the packet
 * tree, or event listeners).
 *
 * Until Regina 6.0.1, this class was called PDF.  It was renamed and expanded
 * in Regina 7.0 to allow for any kind of file to be attached.
 *
 * \ingroup packet
 */
class Attachment : public Packet {
    REGINA_PACKET(PACKET_ATTACHMENT, "Attachment")

    public:
        /**
         * Describes how an attachment packet should claim ownership of a block
         * of binary data.
         *
         * \nopython For Python users, blocks are always copied (i.e., the
         * ownership policy is always DEEP_COPY).
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
            /**< The binary data of the file attachment that is stored in this
                 packet, or \c null if no attachment is currently stored. */
        size_t size_;
            /**< The size of the binary data in bytes, or 0 if no attachment
                 is currently stored. */
        OwnershipPolicy alloc_;
            /**< Describes how the binary data (if any) was allocated; this
                 must be either \a OWN_MALLOC or \a OWN_NEW. */

        std::string filename_;
            /**< The original filename of the attachment.  Regina's user
                 interface will treat this as a hint only.  Typically this
                 filename would not contain any path information. */

    public:
        /**
         * Creates a packet with no attachment stored.
         *
         * Until some content is assigned (e.g., by the assignment operator,
         * or by a non-trivial call to reset()), isNull() will return \c true.
         */
        Attachment();

        /**
         * Creates a new attachment containing the contents of the given file.
         *
         * The file will be read immediately, but no ongoing link will be
         * maintained.  That is, if the given file should later change its
         * contents on the filesystem, the change will \e not be reflected in
         * this attachment packet.
         *
         * If the file could not be read or is empty, then no attachment will
         * be stored.  You can test this by calling isNull().
         *
         * The filename that is stored with this attachment (i.e., the string
         * that will be returned by filename()) will be the argument
         * \a pathname with any directory prefixes removed (i.e., just the
         * final filename component).  If an error occurs when attempting
         * to remove directory prefixes, the filename will be the empty string.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file \e name, and
         * simply passes it through unchanged to low-level C/C++ file I/O
         * routines.
         *
         * @param pathname the full pathname of the attachment to read.
         */
        Attachment(const char* pathname);

        /**
         * Creates a new attachment containing the given binary data.
         *
         * The \a data array must contain a block of binary data, of
         * \a size bytes.  There is no notion of null termination:
         * the data block may contain null bytes within it, and does
         * not need to be terminated by a null byte at the end.
         *
         * The \a alloc argument shows if/how this packet claims ownership of
         * the data.  In particular, unless \a alloc is \a DEEP_COPY, this
         * packet will claim ownership of the given data block and will
         * deallocate it when the packet is destroyed.  If \a alloc is
         * \a DEEP_COPY then the given block of data will not be modified in
         * any way.
         *
         * It is possible to pass a null pointer as the data array, in
         * which case the new packet will have no attachment stored
         * (so isNull() will return \c true).
         *
         * \ifacespython The argument \a data should be a Python \c bytes
         * object.  The \a size and \a alloc arguments are not present,
         * since a \c bytes object knows its size, and in Python the
         * only available ownership policy is to make a deep copy.
         *
         * @param data the block of binary data that forms the attachment,
         * or \c null if no attachment is to be stored.
         * @param size the number of bytes in this block of binary data;
         * if actual data is passed (i.e., \a data is not \c null) then
         * this must be strictly positive.
         * @param alloc describes if/how this packet should claim ownership
         * of the given block of data; see the notes above for details.
         * @param filename the filename to associated with this attachment;
         * typically this would be a filename only, with no directory prefixes.
         * See filename() for details on how this string will be used.
         */
        Attachment(char* data, size_t size, OwnershipPolicy alloc,
            std::string filename);

        /**
         * Creates a new deep copy of the given attachment.
         *
         * Like all packet types, this only copies the content of the
         * attachment, not the packet infrastructure (e.g., it will not copy
         * the packet label, it will not clone the given packet's children,
         * and it will not insert the new packet into any packet tree).
         *
         * This is safe to call even if \a src does not contain a non-empty
         * attachment (i.e., if <tt>src.isNull()</tt> returns \c true).
         *
         * @param src the attachment packet whose contents should be copied.
         */
        Attachment(const Attachment& src);

        /**
         * Destroys this attachment packet and deallocates data if required.
         */
        ~Attachment() override;

        /**
         * Sets this to be a deep copy of the given attachment.
         *
         * Like all packet types, this only copies the content of the
         * attachment, not the packet infrastructure (e.g., it will not copy
         * the packet label, or change this packet's location in any packet
         * tree).
         *
         * This is safe to call even if \a src does not contain a non-empty
         * attachment (i.e., if <tt>src.isNull()</tt> returns \c true).
         *
         * @param src the attachment packet whose contents should be copied.
         * @return a reference to this packet.
         */
        Attachment& operator = (const Attachment& src);

        /**
         * Swaps the contents of this and the given attachment.
         *
         * Like all packet types, this only swaps the content of the
         * attachments, not the packet infrastructure (e.g., it will not swap
         * packet labels, or change either packet's location in any packet
         * tree).
         *
         * This is safe to call even if this packet and/or \a other does not
         * contain a non-empty attachment (i.e., if isNull() returns \c true).
         *
         * @param other the attachment packet whose contents should be swapped
         * with this.
         */
        void swap(Attachment& other);

        /**
         * Determines whether this packet is currently holding a non-empty
         * attachment.
         *
         * @return \c true if and only if this packet is holding a
         * non-empty attachment.
         */
        bool isNull() const;

        /**
         * Returns a pointer to the block of raw data that forms this
         * attachment.  The number of bytes in this block can be found
         * by calling size().
         *
         * If this packet does not currently hold a non-empty attachment
         * (i.e., if isNull() returns \c true), then this routine will return
         * \c null.
         *
         * \ifacespython This routine returns a Python \c bytes object.
         * Unlike the C++ version of this routine, the Python version returns
         * a deep copy of the raw data (so this routine becomes slower, but
         * the resulting byte sequence can be freely modified).
         *
         * @return the raw attachment data.
         */
        const char* data() const;

        /**
         * Returns the size of this attachment in bytes.
         *
         * If this packet does not currently hold a non-empty attachment
         * (i.e., if isNull() returns \c true), then this routine will return
         * zero.
         *
         * @return the number of bytes.
         */
        size_t size() const;

        /**
         * Returns the filename associated with this attachment.  Typically
         * this would be a filename only, with no path information.
         *
         * The user interface may use the result of filename() and/or
         * extension() to determine which external viewer to launch when the
         * user asks to view the attachment.
         *
         * There are no guarantees on the contents of this string, and
         * user interfaces should treat it as a hint only (i.e., they
         * may modify or even completely override it when saving the
         * attachment as a local file).  It is possible in some scenarios
         * that this could be the empty string, or could be some other string
         * that is not a valid filename on the current platform.
         *
         * @return the filename associated with this attachment.
         */
        const std::string& filename() const;

        /**
         * Returns the extension of the filename associated with this
         * attachment.
         *
         * The user interface may use the result of filename() and/or
         * extension() to determine which external viewer to launch when the
         * user asks to view the attachment.
         *
         * The extension will include the leading period.  So, for
         * example, if filename() returns "file.pdf", then extension()
         * will return ".pdf".
         *
         * If the associated filename has no extension, or if an error
         * occurred when attempting to deduce it, then this routine will
         * return the empty string.
         *
         * @return the extension of the filename for this attachment.
         */
        std::string extension() const;

        /**
         * Empties this packet so that no attachment is stored.
         * After calling this routine, isNull() will return \c true.
         *
         * The old data will be deallocated if required.
         */
        void reset();

        /**
         * Refills this attachment packet with the given binary data.
         * The old data will be deallocated if required.
         *
         * This routine behaves like the class constructor; see the
         * constructor documentation for details.
         *
         * \warning This cannot be used for self-assignment.  If the argument
         * \a data is in fact data() then this routine will most likely crash.
         *
         * \ifacespython The argument \a data should be a Python \c bytes
         * object.  The \a size and \a alloc arguments are not present,
         * since a \c bytes object knows its size, and in Python the
         * only available ownership policy is to make a deep copy.
         *
         * @param data the block of binary data that forms the new attachment,
         * or \c null if no attachment is to be stored.
         * @param size the number of bytes in this new block of binary data;
         * if actual data is passed (i.e., \a data is not \c null) then
         * this must be strictly positive.
         * @param alloc describes if/how this packet should claim ownership
         * of the given block of data; see the notes above for details.
         * @param filename the new filename to associated with this attachment;
         * this will override the previously stored filename.  Typically
         * this would be a filename only, with no directory prefixes.
         * See filename() for details on how this string will be used.
         */
        void reset(char* data, size_t size, OwnershipPolicy alloc,
            std::string filename);

        /**
         * Saves the contents of this attachment to the given file.
         *
         * If this packet does not currently hold a non-empty attachment
         * (i.e., if isNull() returns \c true), then this routine will do
         * nothing and simply return \c false.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given file \e name, and
         * simply passes it unchanged to low-level C/C++ file I/O routines.
         *
         * @param pathname the full pathname of the file to write.
         * @return \c true if the file was successfully written, or
         * \c false otherwise.
         */
        bool save(const char* pathname) const;

        /**
         * Determines if this and the given attachment hold identical data.
         *
         * The filenames will not be compared.
         *
         * It is safe to call this operator if one or both attachments is
         * empty (i.e., isNull() returns \c true), in which case an empty
         * attachment will compare as equal to any other empty attachment.
         *
         * @param other the attachment to compare with this.
         * @return \c true if and only if this and the given attachment
         * contain identical data.
         */
        bool operator == (const Attachment& other) const;

        /**
         * Determines if this and the given attachment hold different data.
         *
         * The filenames will not be compared.
         *
         * It is safe to call this operator if one or both attachments is
         * empty (i.e., isNull() returns \c true), in which case an empty
         * attachment will compare as equal to any other empty attachment.
         *
         * @param other the attachment to compare with this.
         * @return \c true if and only if this and the given attachment
         * contain different data.
         */
        bool operator != (const Attachment& other) const;

        void writeTextShort(std::ostream& out) const override;

    protected:
        std::shared_ptr<Packet> internalClonePacket() const override;
        void writeXMLPacketData(std::ostream& out, FileFormat format,
            bool anon, PacketRefs& refs) const override;
};

/**
 * Swaps the contents of the given attachment packets.
 *
 * This global routine simply calls Attachment::swap(); it is provided so that
 * Attachment meets the C++ Swappable requirements.
 *
 * @param a the first attachment packet whose contents should be swapped.
 * @param b the second attachment packet whose contents should be swapped.
 *
 * \ingroup packet
 */
void swap(Attachment& a, Attachment& b);

// Inline functions for Attachment

inline Attachment::Attachment() : data_(nullptr), size_(0), alloc_(OWN_NEW) {
}

inline Attachment::Attachment(char* data, size_t size, OwnershipPolicy alloc,
        std::string filename) :
        data_(data), size_(size), alloc_(alloc),
        filename_(std::move(filename)) {
    if (alloc_ == DEEP_COPY) {
        if (data_) {
            data_ = static_cast<char*>(::malloc(size_));
            ::memcpy(data_, static_cast<const char*>(data), size_);
        }
        alloc_ = OWN_MALLOC;
    } else if (! data_)
        size_ = 0;
}

inline Attachment::Attachment(const Attachment& src) :
        Attachment(src.data_, src.size_, DEEP_COPY, src.filename_) {
}

inline Attachment::~Attachment() {
    if (data_) {
        if (alloc_ == OWN_MALLOC)
            ::free(data_);
        else
            delete[] data_;
    }
}

inline Attachment& Attachment::operator = (const Attachment& src) {
    // reset() cannot handle self-assignment.
    if (std::addressof(src) == this)
        return *this;

    reset(src.data_, src.size_, DEEP_COPY, src.filename_);
    return *this;
}

inline void Attachment::swap(Attachment& other) {
    ChangeEventSpan span1(*this);
    ChangeEventSpan span2(other);

    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(alloc_, other.alloc_);
    filename_.swap(other.filename_);
}

inline const char* Attachment::data() const {
    return data_;
}

inline bool Attachment::isNull() const {
    return ! data_;
}

inline size_t Attachment::size() const {
    return size_;
}

inline const std::string& Attachment::filename() const {
    return filename_;
}

inline bool Attachment::operator == (const Attachment& other) const {
    if (size_ != other.size_)
        return false;
    return size_ == 0 || ::memcmp(data_, other.data_, size_) == 0;
}

inline bool Attachment::operator != (const Attachment& other) const {
    if (size_ != other.size_)
        return true;
    return size_ != 0 && ::memcmp(data_, other.data_, size_) != 0;
}

inline void Attachment::writeTextShort(std::ostream& o) const {
    if (isNull())
        o << "Empty attachment";
    else {
        o << "Attachment (" << size_ << (size_ == 1 ? " byte)" : " bytes)");
        if (! filename_.empty())
            o << ": " << filename_;
    }
}

inline std::shared_ptr<Packet> Attachment::internalClonePacket() const {
    return std::make_shared<Attachment>(data_, size_, DEEP_COPY, filename_);
}

inline void swap(Attachment& a, Attachment& b) {
    a.swap(b);
}

} // namespace regina

#endif

