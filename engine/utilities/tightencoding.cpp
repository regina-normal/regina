
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2021, Ben Burton                                   *
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

#include "maths/integer.h"
#include "utilities/tightencoding-impl.h"

using it1 = std::string::const_iterator;
using it2 = std::istreambuf_iterator<char>;

namespace regina::detail {

template void tightEncodeInteger<int>(std::ostream&, int);
template void tightEncodeInteger<long>(std::ostream&, long);
template void tightEncodeInteger<long long>(std::ostream&, long long);
template void tightEncodeInteger<unsigned>(std::ostream&, unsigned);
template void tightEncodeInteger<unsigned long>(std::ostream&, unsigned long);
template void tightEncodeInteger<unsigned long long>(std::ostream&,
    unsigned long long);
template void tightEncodeInteger<Integer>(std::ostream&, Integer);
template void tightEncodeInteger<LargeInteger>(std::ostream&, LargeInteger);

template int tightDecodingInteger<int, it1>(it1, it1, bool);
template int tightDecodingInteger<int, it2>(it2, it2, bool);
template long tightDecodingInteger<long, it1>(it1, it1, bool);
template long tightDecodingInteger<long, it2>(it2, it2, bool);
template long long tightDecodingInteger<long long, it1>(it1, it1, bool);
template long long tightDecodingInteger<long long, it2>(it2, it2, bool);
template unsigned tightDecodingInteger<unsigned, it1>(it1, it1, bool);
template unsigned tightDecodingInteger<unsigned, it2>(it2, it2, bool);
template unsigned long tightDecodingInteger<unsigned long, it1>(it1, it1, bool);
template unsigned long tightDecodingInteger<unsigned long, it2>(it2, it2, bool);
template unsigned long long tightDecodingInteger<unsigned long long, it1>(
    it1, it1, bool);
template unsigned long long tightDecodingInteger<unsigned long long, it2>(
    it2, it2, bool);
template Integer tightDecodingInteger<Integer, it1>(it1, it1, bool);
template Integer tightDecodingInteger<Integer, it2>(it2, it2, bool);
template LargeInteger tightDecodingInteger<LargeInteger, it1>(it1, it1, bool);
template LargeInteger tightDecodingInteger<LargeInteger, it2>(it2, it2, bool);

template void tightEncodeIndex<size_t>(std::ostream&, size_t);
template void tightEncodeIndex<ssize_t>(std::ostream&, ssize_t);

template size_t tightDecodingIndex<size_t>(std::istream&);
template ssize_t tightDecodingIndex<ssize_t>(std::istream&);

} // namespace regina::detail

