
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

/*! \file python/helpers/tightencoding.h
 *  \brief Assists with wrapping Regina's tight encoding and decoding routines.
 */

#ifndef __HELPERS_TIGHTENCODING_H
#ifndef __DOXYGEN
#define __HELPERS_TIGHTENCODING_H
#endif

#include "../docstrings/utilities/tightencoding.h"

namespace regina {

template <typename> struct TightEncodable;

namespace python {

/**
 * Adds tight encoding and decoding functions, plus a hash function, to the
 * Python bindings for a C++ class that provides its own tightEncoding(),
 * tightDecoding() and hash() member functions.
 *
 * This will add corresponding tightEncoding(), tightDecoding() and __hash__()
 * functions to the Python class, with the Python docstrings \a docEnc,
 * \a docDec and \a docHash respectively.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>regina::python::add_tight_encoding(c, ...)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.
 */
template <regina::InherentlyTightEncodable C, typename... options>
requires (! std::derived_from<C, regina::TightEncodable<C>>)
void add_tight_encoding(pybind11::class_<C, options...>& c,
        const char* docEnc, const char* docDec, const char* docHash) {
    c.def("tightEncoding", &C::tightEncoding, docEnc);
    c.def("tightDecoding", &C::tightDecoding, docDec);
    c.def("__hash__", &C::hash, docHash);
}

/**
 * Adds tight encoding and decoding functions, plus a hash function, to the
 * Python bindings for a C++ class that derives from regina::TightEncodable.
 *
 * This will add corresponding tightEncoding(), tightDecoding() and __hash__()
 * functions to the Python class, as provided by the regina::TightEncodable
 * (templated) C++ base class.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>regina::python::add_tight_encoding(c)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.
 */
template <regina::InherentlyTightEncodable C, typename... options>
requires (std::derived_from<C, regina::TightEncodable<C>>)
void add_tight_encoding(pybind11::class_<C, options...>& c) {
    c.def("tightEncoding", &C::tightEncoding,
        regina::python::doc::TightEncodable::tightEncoding);
    c.def("tightDecoding", &C::tightDecoding,
        regina::python::doc::TightEncodable::tightDecoding);
    c.def("__hash__", &C::hash,
        regina::python::doc::TightEncodable::hash);
}

} } // namespace regina::python

#endif
