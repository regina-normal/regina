
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

/*! \file link/pd-impl.h
 *  \brief Contains implementation details for parsing planar diagram codes
 *  of links.
 *
 *  This file is automatically included from link.h; there is no need
 *  for end users to include it explicitly.
 */

#ifndef __PD_IMPL_H
#ifndef __DOXYGEN
#define __PD_IMPL_H
#endif

namespace regina {

template <typename Iterator>
Link* Link::fromPD(Iterator begin, Iterator end) {
    // Extract the number of crossings.
    size_t n = end - begin;
    if (n == 0)
        return new Link(1);

    // For now just print the elements of the tuple.
    for (Iterator it = begin; it != end; ++it) {
        std::cerr << "(";
        for (int i = 0; i < 4; ++i)
            std::cerr << " " << (*it)[i];
        std::cerr << " ) ";
    }
    std::cerr << std::endl;
    return nullptr;
}

} // namespace regina

#endif

