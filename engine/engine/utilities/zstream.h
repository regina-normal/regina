
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file zstream.h
 *  \brief Provides compressed I/O streams.
 */

#ifndef __ZSTREAM_H
#ifndef __DOXYGEN
#define __ZSTREAM_H
#endif

#include <iostream>
#include <zlib.h>

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A common base class for compression/decompression stream buffers.
 * This class should not be instantiated directly; see classes
 * CompressionBuffer and DecompressionBuffer instead.
 *
 * The standard \e zlib compression library is used for
 * compression and decompression.
 *
 * \ifacespython Not present.
 */
class ZBuffer : public std::streambuf {
    public:
        static const int zEOF;
            /**< The end-of-file marker used with this stream buffer. */

    private:
        gzFile file;
            /**< The compressed file being read from or written to. */
        int next;
            /**< The next character that will be read, or -1 if unknown. */

    protected:
        /**
         * Creates a new stream buffer.
         */
        ZBuffer();

    public:
        /**
         * Destroys this stream buffer.
         * Any underlying file that is open will be closed.
         */
        virtual ~ZBuffer();

        /**
         * Writes the given character to the underlying file,
         * compressing en route.
         *
         * @param c the uncompressed character to write.
         * @return the given character on success, or zEOF on error.
         */
        virtual int overflow(int c);
        /**
         * Reads the next character from the underlying file,
         * decompressing en route.  The character is not consumed.
         *
         * @return the next uncompressed character, or zEOF if there is
         * an error or there are no more characters.
         */
        virtual int underflow();
        /**
         * Reads the next character from the underlying file,
         * decompressing en route.  The character is consumed.
         *
         * @return the next uncompressed character, or zEOF if there is
         * an error or there are no more characters.
         */
        virtual int uflow();
        /**
         * Writes the given set of characters to the underlying file,
         * compressing en route.
         *
         * @param s the uncompressed array of characters to write.
         * @param n the number of characters to write.
         * @return the number of uncompressed characters that were
         * actually written, or 0 if an error occurred.
         */
        virtual std::streamsize xsputn(const char* s, std::streamsize n);
        /**
         * Reads a set of characters from the underlying file,
         * decompressing en route.  The characters are all consumed.
         * Reading will stop if end-of-file is reached or an error
         * occurs.
         *
         * @param s the array into which the uncompressed characters
         * should be placed.
         * @param n the number of uncompressed characters to read.
         * @return the number of uncompressed characters that were
         * actually read, or zEOF if an error occurred.
         */
        virtual std::streamsize xsgetn(char* s, std::streamsize n);
        /**
         * Pushes the given character back into the underlying input stream.
         *
         * \warning This routine will only succeed if the next character
         * in the underlying input stream has not already been read.
         *
         * @param c the character to push back.
         * @return the character that was pushed back, or zEOF if an
         * error occurred.
         */
        virtual int pbackfail(int c);
        /**
         * Flushes all input/output buffers.
         *
         * @return 0 on success, or zEOF on error.
         */
        virtual int sync();

        /**
         * Closes the underlying file.  If no file is open, this routine
         * does nothing.
         *
         * @return 0 on success, or zEOF on error.
         */
        int close();

        /**
         * Writes a description of the last (de)compression error that
         * occurred.  For the message to be meaningful, the underlying
         * file must still be open.
         *
         * @param out the output stream to which the error description
         * should be written.
         */
        void showError(std::ostream& out);

    protected:
        /**
         * Opens the given file for (de)compressed reading or writing.
         * If a file is already open, it will be closed before the new
         * file is opened.
         *
         * @param path the pathname of the new file to open.
         * @param mode the mode in which to open the file; this must be
         * <tt>"rb"</tt> for reading or <tt>"wb"</tt> for writing.
         * @return 0 on success, or zEOF on error.
         */
        int open(const char *path, const char *mode);
};

/**
 * An output stream buffer that compresses data as it is written.
 * The standard \e zlib compression library is used.
 *
 * This buffer is designed for use with standard C++ I/O streams.
 *
 * This stream buffer should \e not be used for input.
 *
 * \ifacespython Not present.
 */
class CompressionBuffer : public ZBuffer {
    public:
        /**
         * Creates a new compression stream buffer.
         */
        CompressionBuffer();
        /**
         * Creates a new compression stream buffer that writes to the
         * given file.  The underlying file will be opened automatically.
         *
         * @param path the pathname of the new file to open.
         */
        CompressionBuffer(const char* path);

        /**
         * Opens the given file for compressed writing.
         * If a file is already open, it will be closed before the new
         * file is opened.
         *
         * @param path the pathname of the new file to open.
         * @return 0 on success, or zEOF on error.
         */
        int open(const char* path);
};

/**
 * An input stream buffer that decompresses data as it is read.
 * The standard \e zlib compression library is used.
 *
 * This buffer is designed for use with standard C++ I/O streams.
 *
 * This stream buffer should \e not be used for output.
 *
 * \ifacespython Not present.
 */
class DecompressionBuffer : public ZBuffer {
    public:
        /**
         * Creates a new decompression stream buffer.
         */
        DecompressionBuffer();
        /**
         * Creates a new decompression stream buffer that reads from the
         * given file.  The underlying file will be opened automatically.
         *
         * @param path the pathname of the new file to open.
         */
        DecompressionBuffer(const char* path);

        /**
         * Opens the given file for decompressed reading.
         * If a file is already open, it will be closed before the new
         * file is opened.
         *
         * @param path the pathname of the new file to open.
         * @return 0 on success, or zEOF on error.
         */
        int open(const char* path);
};

/**
 * An output stream that compresses data as it is written.
 * The standard \e zlib compression library is used.
 *
 * This stream does its work through a CompressionBuffer.
 *
 * \ifacespython Not present.
 */
class CompressionStream : public std::ostream {
    private:
        CompressionBuffer buf;
            /**< The underlying compression buffer. */

    public:
        /**
         * Creates a new compression stream.
         */
        CompressionStream();
        /**
         * Creates a new compression stream that writes to the given
         * file.  The underlying file will be opened automatically.
         *
         * @param path the pathname of the new file to open.
         */
        CompressionStream(const char* path);

        /**
         * Opens the given file for compressed writing.
         * If a file is already open, it will be closed before the new
         * file is opened.
         *
         * @param path the pathname of the new file to open.
         * @return 0 on success, or ZBuffer::zEOF on error.
         */
        int open(const char* path);
        /**
         * Closes the underlying file.  If no file is open, this routine
         * does nothing.
         *
         * @return 0 on success, or ZBuffer::zEOF on error.
         */
        int close();
};

/**
 * An input stream that decompresses data as it is read.
 * The standard \e zlib compression library is used.
 *
 * This stream does its work through a DecompressionBuffer.
 *
 * \ifacespython Not present.
 */
class DecompressionStream : public std::istream {
    private:
        DecompressionBuffer buf;
            /**< The underlying decompression buffer. */

    public:
        /**
         * Creates a new decompression stream.
         */
        DecompressionStream();
        /**
         * Creates a new decompression stream that reads from the given
         * file.  The underlying file will be opened automatically.
         *
         * @param path the pathname of the new file to open.
         */
        DecompressionStream(const char* path);

        /**
         * Opens the given file for decompressed reading.
         * If a file is already open, it will be closed before the new
         * file is opened.
         *
         * @param path the pathname of the new file to open.
         * @return 0 on success, or ZBuffer::zEOF on error.
         */
        int open(const char* path);
        /**
         * Closes the underlying file.  If no file is open, this routine
         * does nothing.
         *
         * @return 0 on success, or ZBuffer::zEOF on error.
         */
        int close();
};

/*@}*/

// Inline functions for ZBuffer

inline ZBuffer::ZBuffer() : file(0), next(-1) {
    setbuf(0, 0);
}

inline ZBuffer::~ZBuffer() {
    close();
}

inline int ZBuffer::overflow(int c) {
    int ans = gzputc(file, c);
    return (ans == -1 ? zEOF : c);
}

inline int ZBuffer::underflow() {
    if (next == -1)
        next = gzgetc(file);
    return (next == -1 ? zEOF : next);
}

inline int ZBuffer::uflow() {
    int ans = underflow();
    next = -1;
    return ans;
}

inline std::streamsize ZBuffer::xsputn(const char* s, std::streamsize n) {
    return gzwrite(file, const_cast<void*>(static_cast<const void*>(s)), n);
}

inline int ZBuffer::pbackfail(int c) {
    if (c == zEOF)
        return zEOF;
    return (next == -1 ? next = c : zEOF);
}

inline int ZBuffer::sync() {
    return (gzflush(file, Z_SYNC_FLUSH) == Z_OK ? 0 : zEOF);
}

// Inline functions for CompressionBuffer

inline CompressionBuffer::CompressionBuffer() {
}

inline CompressionBuffer::CompressionBuffer(const char* path) {
    open(path);
}

inline int CompressionBuffer::open(const char* path) {
    return ZBuffer::open(path, "wb");
}

// Inline functions for DecompressionBuffer

inline DecompressionBuffer::DecompressionBuffer() {
}

inline DecompressionBuffer::DecompressionBuffer(const char* path) {
    open(path);
}

inline int DecompressionBuffer::open(const char* path) {
    return ZBuffer::open(path, "rb");
}

// Inline functions for CompressionStream

inline CompressionStream::CompressionStream() : std::ostream(0) {
    init(&buf);
}

inline CompressionStream::CompressionStream(const char* path) :
        std::ostream(0) {
    init(&buf);
    open(path);
}

inline int CompressionStream::open(const char* path) {
    int ans = buf.open(path);
    if (ans)
        setstate(std::ios::failbit);
    return ans;
}

inline int CompressionStream::close() {
    int ans = buf.close();
    if (ans)
        setstate(std::ios::failbit);
    return ans;
}

// Inline functions for DecompressionStream

inline DecompressionStream::DecompressionStream() : std::istream(0) {
    init(&buf);
}

inline DecompressionStream::DecompressionStream(const char* path) :
        std::istream(0) {
    init(&buf);
    open(path);
}

inline int DecompressionStream::open(const char* path) {
    int ans = buf.open(path);
    if (ans)
        setstate(std::ios::failbit);
    return ans;
}

inline int DecompressionStream::close() {
    int ans = buf.close();
    if (ans)
        setstate(std::ios::failbit);
    return ans;
}

} // namespace regina

#endif

