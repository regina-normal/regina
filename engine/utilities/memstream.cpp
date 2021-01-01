
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

#include <cstring>
#include "utilities/memstream.h"

namespace regina {

std::streamsize mem_streambuf::xsgetn(char* s, std::streamsize n) {
    if (current_ + n > end_)
        n = end_ - current_;

    std::memcpy(s, current_, n);
    current_ += n;
    return n;
}

mem_streambuf::pos_type mem_streambuf::seekpos(
        pos_type pos, std::ios_base::openmode which) {
    if ((which & std::ios_base::in) && begin_ + pos <= end_) {
        current_ = begin_ + pos;
        return pos;
    } else
        return pos_type(off_type(-1));
}

mem_streambuf::pos_type mem_streambuf::seekoff(off_type off,
        std::ios_base::seekdir dir, std::ios_base::openmode which) {
    if (! (which & std::ios_base::in))
        return pos_type(off_type(-1));

    if (off == std::ios_base::beg) {
        if (off >= 0 && begin_ + off <= end_) {
            current_ = begin_ + off;
            return off;
        }
    } else if (off == std::ios_base::cur) {
        if (current_ + off >= begin_ && current_ + off <= end_) {
            current_ += off;
            return (current_ - begin_);
        }
    } else if (off == std::ios_base::end) {
        if (off <= 0 && end_ + off >= begin_) {
            current_ = end_ + off;
            return (current_ - begin_);
        }
    }

    return pos_type(off_type(-1));
}

} // namespace regina

