
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

/*! \file utilities/i18nutils.h
 *  \brief Various classes and routines for working with internationalisation
 *  and character encodings.
 */

#ifndef __REGINA_I18NUTILS_H
#ifndef __DOXYGEN
#define __REGINA_I18NUTILS_H
#endif

#include "regina-core.h"
#include "regina-config.h"
#include <iostream>
#include <string>
#include <iconv.h>

/**
 * Various classes and routines for working with internationalisation and
 * character encodings.
 */
namespace regina::i18n {

/**
 * Identifies the longest prefix of the given string that is valid UTF-8.
 *
 * The substring from <tt>s.begin()</tt> to the iterator that is
 * returned is guaranteed to be valid UTF-8.  If the entire string is
 * valid UTF-8, then this routine will return <tt>s.end()</tt>.
 *
 * \ifacespython This routine returns the \e length of the longest valid
 * UTF-8 prefix.  The length is measured in raw bytes (not unicode characters).
 *
 * @return an iterator marking the end of the longest valid UTF-8 prefix.
 *
 * \ingroup utilities
 */
std::string::const_iterator utf8ValidTo(const std::string& s);

/**
 * Identifies the longest prefix of the given string that is valid UTF-8.
 *
 * The substring from \a s until just before the pointer that is returned
 * is guaranteed to be valid UTF-8.  If the entire string is valid UTF-8,
 * then this routine will return a pointer to the null terminator of \a s.
 *
 * \ifacespython This routine returns the \e length of the longest valid
 * UTF-8 prefix.  The length is measured in raw bytes (not unicode characters).
 *
 * @return a pointer marking the end of the longest valid UTF-8 prefix.
 *
 * \ingroup utilities
 */
const char* utf8ValidTo(const char* s);

/**
 * A simple class with static routines for querying information about the
 * current locale.
 *
 * These routines use ::setlocale() to determine the current locale,
 * which means they respect environment variables such as LANG and LC_ALL.
 *
 * \ingroup utilities
 */
class Locale {
    private:
        static bool initialised;
            /**< Have we determined the current locale yet?  This is
                 done the first time a query routine is called. */

    public:
        /**
         * Returns the character encoding used in the current locale.
         * This is a plain string, such as "UTF-8" or "ISO-8859-1".
         *
         * @return the character encoding for the current locale.
         */
        static const char* codeset();

        // Make this class non-constructible.
        Locale() = delete;
};

/**
 * An output stream buffer that translates between character encodings.
 * The \e iconv library is used to do the real work.
 *
 * Users should not normally instantiate this class directly; instead
 * see IConvStream for a higher-level interface to character conversion.
 *
 * This class will still work if \e iconv is not supported on the build
 * machine, though in this case it will simply pass data through without
 * performing any translations.
 *
 * \ifacespython Not included.
 *
 * @author Parts of this code are modified from the cxxtools library
 * (<tt>http://www.tntnet.org/cxxutils.html</tt>), which is
 * copyright (c) 2003 by Tommi Maekitalo, and covered by the GNU Lesser
 * General Public License.
 *
 * \ingroup utilities
 */
class IConvStreamBuffer : public std::streambuf {
    private:
        std::ostream* sink;
            /**< The destination output stream, or \c null if it has not yet
                 been initialised. */
        char preBuffer[16];
            /**< The internal pre-conversion character buffer. */
        char postBuffer[64];
            /**< The internal post-conversion character buffer.
                 Allow characters to blow out to four times their size. */
        iconv_t cd;
            /**< The iconv conversion descriptor, or \a cdNone if no
                 conversion descriptor is currently active. */

        // NOLINTNEXTLINE(misc-misplaced-const)
        static const iconv_t cdNone;
            /**< Signifies that no iconv conversion descriptor is
                 currently active. */

    public:
        /**
         * Creates a new stream buffer.
         */
        IConvStreamBuffer();
        /**
         * Destroys this stream buffer.  This stream buffer will be
         * closed, but the destination output stream will not be.
         */
        ~IConvStreamBuffer() override;

        /**
         * Opens a new stream buffer that wraps around the given output
         * stream.  If this stream buffer is already open, it will be
         * closed and then reopened with the given parameters.
         *
         * Any data that is sent to this stream buffer will be
         * translated from \e srcCode to \e destCode and passed on to the
         * given output stream.
         *
         * If the given encodings are invalid, this stream will still
         * forward data to the given output stream but no conversion
         * will take place.
         *
         * See the \e iconv documentation for information on what
         * encodings are supported.  For the GNU C library implementation,
         * valid encodings can be found by running <tt>iconv --list</tt>.
         *
         * \pre The destination output stream is already open.
         *
         * @param dest the destination output stream.
         * @param srcCode the character encoding for data that is to be
         * written into this stream buffer.
         * @param destCode the character encoding for the translated data
         * that will subsequently be written to the destination output stream.
         * @return this stream buffer on success, or \c null on error.
         */
        IConvStreamBuffer* open(std::ostream& dest,
            const char* srcCode, const char* destCode);
        /**
         * Closes this stream buffer.
         *
         * @return this stream buffer on success, or \c null on error.
         */
        IConvStreamBuffer* close() noexcept;

        /**
         * Sends buffered data to the destination output stream,
         * converting between character sets en route.
         *
         * The buffer will be flushed as far as possible, and any
         * invalid characters will be replaced with one or more question
         * marks.  If the buffer ends in an incomplete multibyte character,
         * this incomplete character will be held back (since it presumably
         * needs to be combined with later input).
         *
         * @param c an extra character to send that did not fit in the
         * internal buffer, or EOF if we simply wish to flush the buffer.
         * @return 0 on success, or EOF on error.
         */
        int_type overflow(int_type c) override;
        /**
         * Simply returns EOF (since this is not an input stream).
         *
         * @return EOF.
         */
        int_type underflow() override;
        /**
         * Flushes all output buffers.  The buffers for both this stream
         * and the destination output stream will be flushed.
         *
         * @return 0 on success, or -1 on error.
         */
        int sync() override;

        // Make this class non-copyable.
        IConvStreamBuffer(const IConvStreamBuffer&) = delete;
        IConvStreamBuffer& operator = (const IConvStreamBuffer&) = delete;
};

/**
 * An output stream that converts between character encodings.
 * The \e iconv library does all the work behind the scenes.
 *
 * An IConvStream acts as a wrapper around some other destination
 * output stream (for instance, std::cout).  To use an IConvStream:
 *
 * - Construct it, passing the destination output stream and the to/from
 *   character encodings to the class constructor;
 *
 * - Write data to this IConvStream, which will be converted and
 *   forwarded on to the destination output stream;
 *
 * - Destroy this IConvStream.  The destination output stream will
 *   remain open.
 *
 * This class will still work if \e iconv is not supported on the build
 * machine, though in this case it will simply pass data straight through
 * to the destination output stream without any conversion.
 *
 * \ifacespython Not present.
 *
 * @author Parts of this code are modified from the cxxtools library
 * (<tt>http://www.tntnet.org/cxxutils.html</tt>), which is
 * copyright (c) 2003 by Tommi Maekitalo, and covered by the GNU Lesser
 * General Public License.
 *
 * \ingroup utilities
 */
class IConvStream : public std::ostream {
    private:
        IConvStreamBuffer buf;
            /**< The IConvStreamBuffer that does all the actual work. */

    public:
        /**
         * Creates a new IConvStream; see the class notes for details.
         *
         * If the given encodings are invalid, this stream will still
         * forward data to the given output stream but no conversion
         * will take place.
         *
         * See the \e iconv documentation for information on what
         * encodings are supported.  For the GNU C library implementation,
         * valid encodings can be found by running <tt>iconv --list</tt>.
         *
         * \pre The destination output stream is already open.
         *
         * @param dest the destination output stream.
         * @param srcCode the character encoding for data that is to be
         * written into this IConvStream.
         * @param destCode the character encoding for the translated data
         * that will subsequently be written to the destination output stream.
         */
        IConvStream(std::ostream& dest,
            const char* srcCode, const char* destCode);

        // Make this class non-copyable.
        IConvStream(const IConvStream&) = delete;
        IConvStream& operator = (const IConvStream&) = delete;
};

// Inline functions for IConvStreamBuffer

inline IConvStreamBuffer::IConvStreamBuffer() :
        sink(nullptr), cd(cdNone) {
}

inline IConvStreamBuffer::~IConvStreamBuffer() {
    close();
}

inline IConvStreamBuffer::int_type IConvStreamBuffer::underflow() {
    return traits_type::eof();
}

// Inline functions for IConvStream

inline IConvStream::IConvStream(std::ostream& dest,
        const char* srcCode, const char* destCode) : std::ostream(&buf) {
    if (buf.open(dest, srcCode, destCode) == nullptr)
        setstate(std::ios::failbit);
}

} // namespace regina::i18n

#endif
