
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

/*! \file python/helpers/packet.h
 *  \brief Assists with packets that wrap standalone C++ types.
 */

namespace regina {

class Packet;
template <class> class PacketOf;
template <class> class SafePtr;

namespace python {

/**
 * Adds Python bindings for the class PacketOf<Held>.
 *
 * The new packet class will be given an "inherited copy constructor" which
 * takes a single (const Held&).  All other constructors will need to be
 * explicitly added using the pybind11::class_ object that is returned.
 */
template <class Held>
auto& add_packet_wrapper(pybind11::module_& m,
        const char* className) {
    return pybind11::class_<regina::PacketOf<Held>, regina::Packet,
            regina::SafePtr<regina::PacketOf<Held>>>(m, className)
        .def(pybind11::init<const Held&>())
        .def("data", [](regina::PacketOf<Held>* p) {
            return static_cast<Held*>(p);
        }, pybind11::return_value_policy::reference_internal)
        .def_property_readonly_static("typeID", [](pybind11::object) {
            // We cannot take the address of typeID, so use a getter function.
            return regina::PacketOf<Held>::typeID;
        })
    ;
}

// NOTE: Args must be explicitly given; PythonClass and Options are deduced.
template <typename... Args, typename PythonClass, typename... Options>
void add_packet_constructor(PythonClass& classWrapper, Options&&... options) {
    classWrapper.def(pybind11::init([](Args... args) {
        using WrappedType = typename PythonClass::type;
        return new WrappedType(std::in_place, std::forward<Args>(args)...);
    }), std::forward<Options>(options)...);
}

} // namespace python
} // namespace regina

