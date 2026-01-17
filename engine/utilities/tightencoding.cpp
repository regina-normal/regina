
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
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
#ifdef INT128_AVAILABLE
template void tightEncodeInteger<regina::Int128>(std::ostream&, regina::Int128);
template void tightEncodeInteger<regina::UInt128>(std::ostream&,
    regina::UInt128);
#endif
template void tightEncodeInteger<Integer>(std::ostream&, Integer);
template void tightEncodeInteger<LargeInteger>(std::ostream&, LargeInteger);

template int tightDecodeInteger<int, it1>(it1, it1, bool);
template int tightDecodeInteger<int, it2>(it2, it2, bool);
template long tightDecodeInteger<long, it1>(it1, it1, bool);
template long tightDecodeInteger<long, it2>(it2, it2, bool);
template long long tightDecodeInteger<long long, it1>(it1, it1, bool);
template long long tightDecodeInteger<long long, it2>(it2, it2, bool);
template unsigned tightDecodeInteger<unsigned, it1>(it1, it1, bool);
template unsigned tightDecodeInteger<unsigned, it2>(it2, it2, bool);
template unsigned long tightDecodeInteger<unsigned long, it1>(it1, it1, bool);
template unsigned long tightDecodeInteger<unsigned long, it2>(it2, it2, bool);
template unsigned long long tightDecodeInteger<unsigned long long, it1>(
    it1, it1, bool);
template unsigned long long tightDecodeInteger<unsigned long long, it2>(
    it2, it2, bool);
#ifdef INT128_AVAILABLE
template regina::Int128 tightDecodeInteger<regina::Int128, it1>(it1, it1, bool);
template regina::Int128 tightDecodeInteger<regina::Int128, it2>(it2, it2, bool);
template regina::UInt128 tightDecodeInteger<regina::UInt128, it1>(
    it1, it1, bool);
template regina::UInt128 tightDecodeInteger<regina::UInt128, it2>(
    it2, it2, bool);
#endif
template Integer tightDecodeInteger<Integer, it1>(it1, it1, bool);
template Integer tightDecodeInteger<Integer, it2>(it2, it2, bool);
template LargeInteger tightDecodeInteger<LargeInteger, it1>(it1, it1, bool);
template LargeInteger tightDecodeInteger<LargeInteger, it2>(it2, it2, bool);

// End users don't get to use the index routines, so what we instantiate
// here simply needs to be whatever Regina uses internally.

template void tightEncodeIndex<unsigned>(std::ostream&, unsigned);
template void tightEncodeIndex<unsigned long>(std::ostream&, unsigned long);
template void tightEncodeIndex<unsigned long long>(std::ostream&,
    unsigned long long);
template void tightEncodeIndex<ssize_t>(std::ostream&, ssize_t);

template unsigned tightDecodeIndex<unsigned>(std::istream&);
template unsigned long tightDecodeIndex<unsigned long>(std::istream&);
template unsigned long long tightDecodeIndex<unsigned long long>(std::istream&);
template ssize_t tightDecodeIndex<ssize_t>(std::istream&);

} // namespace regina::detail

