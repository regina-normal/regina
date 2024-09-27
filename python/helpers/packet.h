
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

/*! \file python/helpers/packet.h
 *  \brief Assists with packets that wrap standalone C++ types.
 */

namespace regina {

class Packet;
template <class> class PacketData;
template <class> class PacketOf;

template <typename Held> std::shared_ptr<PacketOf<Held>> make_packet(Held&&);
template <typename Held> std::shared_ptr<PacketOf<Held>> make_packet(Held&&,
    const std::string&);

namespace python {

/**
 * Adds Python bindings for the class PacketOf<Held>, as well as corresponding
 * make_packet functions.
 *
 * The new packet class will be given a deep copy constructor, which takes a
 * single argument of type const Held&, and also acts as a copy constructor
 * for PacketOf<Held>.
 *
 * For all other Held constructors (except for the copy constructor), you will
 * need to add a corresponding "forwarding constructor" to this class by
 * calling add_packet_constructor() with the pybind11::class_ object that
 * this function returns.
 *
 * Aside from the constructors, the new packet class will (as a subclass
 * of Held) inherit the full interface from Held.
 *
 * Since all packet types are held by std::shared_ptr in their Python
 * bindings, you _must_ ensure that the base class Held is likewise held by
 * std::shared_ptr (not the default std::unique_ptr that pybind11 uses
 * unless instructed otherwise).  If you do not do this, then Python
 * will raise an ImportError when loading Regina's module.
 *
 * Note that, when creating the bindings for the \a Held type, you should
 * use packet_eq_operators() and not add_eq_operators().  See
 * python/helpers/equality.h for further details.
 */
template <class Held>
auto add_packet_wrapper(pybind11::module_& m, const char* className) {
    auto c = pybind11::class_<regina::PacketOf<Held>, Held, regina::Packet,
            std::shared_ptr<regina::PacketOf<Held>>>(m, className,
            doc::common::PacketOf)
        .def(pybind11::init<const Held&>(), // also takes PacketOf<Held>
            doc::common::PacketOf_copy)
        .def_readonly_static("typeID", &regina::PacketOf<Held>::typeID)
    ;
    regina::python::add_output(c);

    m.def("make_packet", [](const Held& h) {
        // The C++ make_packet expects an rvalue reference.
        return regina::make_packet(Held(h));
    }, doc::common::make_packet);
    m.def("make_packet", [](const Held& h, const std::string& label) {
        // The C++ make_packet expects an rvalue reference.
        return regina::make_packet(Held(h), label);
    }, doc::common::make_packet_2);

    // Be kind to users who expect regina-style capitalisation.
    m.def("makePacket", [](const Held& h) {
        return regina::make_packet(Held(h)); // rvalue ref, as above
    }, doc::common::make_packet);
    m.def("makePacket", [](const Held& h, const std::string& label) {
        return regina::make_packet(Held(h), label); // rvalue ref, as above
    }, doc::common::make_packet_2);

    return c;
}

/**
 * Adds a Python constructor for PacketOf<Held> that forwards its arguments
 * to a corresponding Held constructor.
 *
 * Specifically, if the type Held has a constructor that takes arguments
 * x, y, ..., z of types Tx, Ty, ..., Tz respectively, then this function will
 * add a corresponding constructor to the Python wrapper for PacketOf<Held>.
 *
 * At the Python level, this constructor looks like PacketOfHeld(x, y, .., z).
 * At the C++ level, it will call PacketOf<Held>(std::in_place, x, y, ..., z).
 *
 * To add the wrapper, call add_packet_constructor<Tx, Ty, ...  Tz>(c, ...),
 * where c is the pybind11::class_ object returned from add_packet_wrapper()
 * (that is, the pybind11 wrapper for the C++ class PacketOf<Held>).  Any
 * additional arguments (e.g., a docstring) will be passed through to
 * class_.def().
 *
 * The additional \a options arguments are the usual pybind11 options
 * (for example, pybind11::arg objects to specify default arguments).
 * The types PythonClass and Options... are deduced automatically (as
 * opposed to the constructor argument types Args..., which must be
 * explicitly specified as part of the template function call).
 */
template <typename... Args, typename PythonClass, typename... Options>
void add_packet_constructor(PythonClass& classWrapper, Options&&... options) {
    classWrapper.def(pybind11::init([](Args... args) {
        using WrappedType = typename PythonClass::type;
        return new WrappedType(std::in_place, std::forward<Args>(args)...);
    }), std::forward<Options>(options)...);
}

/**
 * Adds wrappers for the member functions for a C++ type Held that are
 * inherited from PacketData<Held>.
 *
 * The argument \a classWrapper should be the pybind11::class_ object
 * that wraps the C++ class Held.
 */
template <typename PythonClass>
void add_packet_data(PythonClass& classWrapper) {
    using DataType = regina::PacketData<typename PythonClass::type>;
    classWrapper
        .def("packet", pybind11::overload_cast<>(&DataType::packet),
            doc::common::PacketData_packet)
        .def("anonID", &DataType::anonID, doc::common::PacketData_anonID)
        ;
}

} // namespace python
} // namespace regina

