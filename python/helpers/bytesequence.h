
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

/*! \file python/helpers/concepts.h
 *  \brief Conversions between `regina::ByteSequence` and Python `bytes`.
 *
 *  This header is _not_ included automatically by python/helpers.h.
 *  If you are binding any routines that take or return ByteSequence objects,
 *  you will need to explicitly include this header yourself.
 */

#ifndef __HELPERS_BYTESEQUENCE_H
#ifndef __DOXYGEN
#define __HELPERS_BYTESEQUENCE_H
#endif

#include <pybind11/pybind11.h>
#include "utilities/bytesequence.h"

namespace pybind11::detail {

template <>
struct type_caster<regina::ByteSequence> {
    PYBIND11_TYPE_CASTER(regina::ByteSequence, const_name("bytes"));

    static pybind11::handle cast(const regina::ByteSequence& seq,
            pybind11::return_value_policy, pybind11::handle /* parent */) {
        return pybind11::bytes(seq.asString()).release();
    }

    bool load(pybind11::handle src, bool /* allow implicit conversions? */) {
        // Check if handle is a Sequence
        if (! pybind11::isinstance<pybind11::bytes>(src))
            return false;

        // Load the contents from pybind11::bytes via std::string.
        value.load(pybind11::reinterpret_borrow<pybind11::bytes>(src));
        return true;
    }
};

} // namespace pybind11::detail

#endif
