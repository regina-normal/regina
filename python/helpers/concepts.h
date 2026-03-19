
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  \brief C++ concepts for use with Regina's Python bindings.
 */

#include "packet/packet.h"

namespace regina::python {

/**
 * A Python container type whose elements can be accessed via integer indexing.
 * Examples are `pybind11::list`, `pybind11::tuple`, and `pybind11::args`.
 */
template <typename T>
concept PythonSequence =
    std::derived_from<T, pybind11::object> &&
    requires (T x, size_t i) {
        { x[i] } -> std::convertible_to<pybind11::object>;
    };

/**
 * A Python class wrapper type; that is, a type of the form
 * `pybind11::class_<...>`.
 */
template <typename T>
concept PythonClassWrapper =
    requires (T x) { { pybind11::class_(x) } -> std::same_as<T>; };

/**
 * A Python class wrapper type for one of Regina's wrapped packet types.
 * That is, \a T is a type of the form
 * `pybind11::class_<regina::PacketOf<...>, ...>`.
 */
template <typename T>
concept PythonWrappedPacketWrapper =
    PythonClassWrapper<T> &&
    requires {
        typename T::type;
        requires regina::WrappedPacket<typename T::type>;
    };

/**
 * A Python class wrapper type whose corresponding C++ type can be held within
 * one of Regina's wrapped packets.  That is, \a T is a type of the form
 * `pybind11::class_<H, ...>` where \a H adheres to the concept PacketHeldType.
 */
template <typename T>
concept PythonPacketHeldWrapper =
    PythonClassWrapper<T> &&
    requires {
        typename T::type;
        requires regina::PacketHeldType<typename T::type>;
    };

} // namespace regina::python
