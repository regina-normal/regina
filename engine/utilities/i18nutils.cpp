
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include <cerrno>
#include <cstddef>
#include <cstring>
#include <clocale>
#include "regina-config.h"
#include "utilities/i18nutils.h"

#ifdef LANGINFO_FOUND
    #include <langinfo.h>
#endif

namespace regina::i18n {

std::string::const_iterator utf8ValidTo(const std::string& s) {
    const char* ans = utf8ValidTo(s.c_str());
    return s.begin() + (ans - s.c_str());
}

const char* utf8ValidTo(const char* s) {
    const char* p = s;
    while (*p) {
        if (! (*p & 0x80)) {
            ++p;
        } else if (! (*p & 0x40)) {
            // Invalid!
            return p;
        } else if (! (*p & 0x20)) {
            // 2-byte code
            if ((*(p+1) & 0xC0) == 0x80)
                p += 2;
            else
                return p;
        } else if (! (*p & 0x10)) {
            // 3-byte code
            if ((*(p+1) & 0xC0) == 0x80 && (*(p+2) & 0xC0) == 0x80)
                p += 3;
            else
                return p;
        } else if (! (*p & 0x08)) {
            // 4-byte code
            if ((*(p+1) & 0xC0) == 0x80 && (*(p+2) & 0xC0) == 0x80 &&
                    (*(p+3) & 0xC0) == 0x80)
                p += 4;
            else
                return p;
        } else {
            // Invalid!
            return p;
        }
    }
    return p;
}

bool Locale::initialised = false;

// NOLINTNEXTLINE(misc-misplaced-const)
const iconv_t IConvStreamBuffer::cdNone((iconv_t)(-1));

#ifdef LANGINFO_FOUND
    const char* Locale::codeset() {
        if (! initialised) {
            ::setlocale(LC_ALL, "");
            initialised = true;
        }

        return nl_langinfo(CODESET);
    }
#else
    namespace {
        const char* noLanginfoCodeset = "UTF-8";
    }
    const char* Locale::codeset() {
        return noLanginfoCodeset;
    }
#endif

IConvStreamBuffer* IConvStreamBuffer::open(std::ostream& dest,
        const char* srcCode, const char* destCode) {
    if (sink)
        if (! close())
            return nullptr;

    sink = &dest;

    cd = iconv_open(destCode, srcCode);
    if (cd == cdNone) {
        if (errno != EINVAL)
            return nullptr;

        // The given encodings are not supported.
        // This is fine; we'll just pass data through to sink untranslated.
    }

    // When we give the buffer to std::streambuf, leave space for an
    // extra overflow character; this will make the implementation of
    // overflow() simpler.
    setp(preBuffer, preBuffer + (sizeof(preBuffer) - 1));
    return this;
}

IConvStreamBuffer* IConvStreamBuffer::close() noexcept {
    sync();

    if (cd == cdNone) {
        // We're passing data through untranslated; nothing more to do.
        return this;
    }

    // Close down the internal iconv workings.
    if (iconv_close(cd) == 0) {
        cd = cdNone;
        return this;
    } else
        return nullptr;
}

IConvStreamBuffer::int_type IConvStreamBuffer::overflow(
        IConvStreamBuffer::int_type c) {
    // Are we even open?
    if (! sink)
        return traits_type::eof();

    // Add the extra character to the end of the buffer before processing.
    if (c != traits_type::eof()) {
        *pptr() = static_cast<char_type>(c);
        pbump(1);
    }

    // Do we know how to translate between encodings?  If not, just
    // send the data straight through to the destination stream.
    if (cd == cdNone) {
        // Note: since preBuffer is very small, we can safely cast
        // n to an int.
        ptrdiff_t n = pptr() - preBuffer;
        sink->write(preBuffer, n);
        pbump(- static_cast<int>(n));

        if (sink->fail())
            return traits_type::eof();
        else
            return 0;
    }

    // Convert the data through iconv().
    // We might need more than one run through this.
    while (pptr() > preBuffer) {
        size_t inBytes = pptr() - preBuffer;
        size_t outBytes = sizeof(postBuffer);

        ICONV_CONST char* inPtr = preBuffer;
        char* outPtr = postBuffer;

        ::iconv(cd, &inPtr, &inBytes, &outPtr, &outBytes);
        int iconvErr = errno;
        errno = 0;

        // If we got any output, write it to the destination stream.
        if (outPtr > postBuffer) {
            sink->write(postBuffer, outPtr - postBuffer);
            if (sink->fail())
                return traits_type::eof();
        }

        // Note: all the int casts below are safe, since preBuffer has a
        // very small size.

        // Are we completely finished?
        if (inBytes == 0) {
            // Yes!
            pbump(- static_cast<int>(inPtr - preBuffer));
            return 0;
        }

        // Something went wrong.
        if (iconvErr == E2BIG) {
            // The output buffer filled up.  This shouldn't happen, but
            // anyway; move the leftover input to the front of the input
            // buffer and try again.
            ::memmove(preBuffer, inPtr, inBytes);
            pbump(- static_cast<int>(inPtr - preBuffer));
            continue;
        }
        if (iconvErr == EINVAL) {
            // We hit an incomplete multibyte sequence.  Move the
            // leftover input to the front of the buffer and stop, since
            // we need more input before we can continue translating.
            ::memmove(preBuffer, inPtr, inBytes);
            pbump(- static_cast<int>(inPtr - preBuffer));
            return 0;
        }
        if (iconvErr == EILSEQ) {
            // We hit an invalid multibyte sequence.
            // Try to recover gracefully by just skipping over it.
            ::memmove(preBuffer, inPtr + 1, inBytes - 1);
            pbump(- static_cast<int>(inPtr + 1 - preBuffer));

            sink->write("?", 1);
            if (sink->fail())
                return traits_type::eof();

            continue;
        }
        // We should never reach this point, since it indicates an error
        // state that iconv() should not set.
        std::cerr << "ERROR: Unexpected state after call to iconv().\n";
        std::cerr << "Please report this as a bug to the Regina author(s).\n";
        std::cerr.flush();
        return traits_type::eof();
    }

    // We can never reach this point, but keep the compiler happy.
    return 0;
}

int IConvStreamBuffer::sync() {
    if (sink) {
        IConvStreamBuffer::int_type ret = overflow(traits_type::eof());
        sink->flush();
        return (ret == traits_type::eof() || sink->fail()) ? -1 : 0;
    } else
        return -1;
}

} // namespace regina::i18n
