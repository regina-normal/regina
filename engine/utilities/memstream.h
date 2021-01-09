
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2019-2021, Ben Burton                                   *
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

/*! \file utilities/memstream.h
 *  \brief Supports input streams that read directly from memory.
 */

#ifndef __MEMSTREAM_H
#ifndef __DOXYGEN
#define __MEMSTREAM_H
#endif

#include <iostream>
#include "regina-core.h"

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * An input stream that reads an array of characters in memory.
 *
 * This input stream does not take any responsibility for managing the
 * memory that is read, and the caller should ensure that this memory
 * remains allocated for at least as long as this input stream.
 *
 * End users should not use this class, but should use mem_istream instead.
 * The API for this class is subject to change in future versions of Regina.
 *
 * \ifacespython Not present.
 */
class REGINA_API mem_istream : public std::istream {
    public:
        /**
         * Initialies an input stream that reads from the given array of
         * characters.
         *
         * @param begin a pointer to the beginning of the array to read.
         * @param end a pointer past-the-end of the array to read.
         */
        mem_istream(const char* begin, const char* end);
        /**
         * Destructor.  Note that this does \e not deallocate the array of
         * characters being read.
         */
        ~mem_istream();

        // Make this class non-copyable.
        mem_istream(const mem_istream&) = delete;
        mem_istream& operator = (const mem_istream&) = delete;
};

/**
 * A stream buffer that provides the implementation details for mem_istream.
 *
 * End users should not use this class, but should use mem_istream instead.
 * The API for this class is subject to change in future versions of Regina.
 *
 * \ifacespython Not present.
 */
class REGINA_API mem_streambuf : public std::streambuf {
    public:
        /**
         * Initialies a read-only stream buffer that reads from the
         * given array of characters.
         *
         * @param begin a pointer to the beginning of the array to read.
         * @param end a pointer past-the-end of the array to read.
         */
        mem_streambuf(const char *begin, const char *end);

        // Make this class non-copyable.
        mem_streambuf(const mem_streambuf&) = delete;
        mem_streambuf& operator = (const mem_streambuf&) = delete;

    protected:
        int_type underflow() override;
        int_type uflow() override;
        int_type pbackfail(int_type ch) override;
        std::streamsize xsgetn(char* s, std::streamsize n) override;
        std::streamsize showmanyc() override;

        pos_type seekpos(pos_type pos, std::ios_base::openmode which) override;
        pos_type seekoff(off_type off, std::ios_base::seekdir dir,
            std::ios_base::openmode which) override;

    private:
        const char* const begin_;
            /**< The beginning of the array being read. */
        const char* const end_;
            /**< Past the end of the array being read. */
        const char* current_;
            /**< The current position within the array being read. */
};

/*@}*/

// Inline functions for mem_streambuf

inline mem_streambuf::mem_streambuf(const char *begin, const char *end) :
        begin_(begin), end_(end), current_(begin_) {
}

inline std::streambuf::int_type mem_streambuf::underflow() {
    if (current_ == end_)
        return EOF;
    else
        return traits_type::to_int_type(*current_);
}

inline std::streambuf::int_type mem_streambuf::uflow() {
    if (current_ == end_)
        return EOF;
    else
        return traits_type::to_int_type(*current_++);
}

inline std::streambuf::int_type mem_streambuf::pbackfail(int_type ch) {
    // Fail if there is no space to put back, or if the character we are
    // putting back does not match what is already there.
    if (current_ == begin_ || (ch != EOF && ch != *(current_ - 1)))
        return EOF;
    else
        return traits_type::to_int_type(*--current_);
}

inline std::streamsize mem_streambuf::showmanyc() {
    // A return value of 0 means "no information available".
    // A return value of -1 means "no more characters left".
    return (current_ < end_ ? end_ - current_ : -1);
}

// Inline functions for mem_istream

inline mem_istream::mem_istream(const char* begin, const char* end) :
        std::istream(new mem_streambuf(begin, end)) {
}

inline mem_istream::~mem_istream() {
    delete rdbuf();
}

} // namespace regina

#endif

