
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

#include "pybind11/pybind11.h"
#include "link/link.h"
#include "../helpers.h"
#include "../helpers/bytesequence.h"
#include "../docstrings/link/linksig.h"

using regina::BoolSet;
using regina::Link;
using regina::LinkSigBinary;
using regina::LinkSigData;
using regina::LinkSigPrintable;

void addLinkSig(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(LinkSigData)

    auto d = pybind11::class_<LinkSigData>(m, "LinkSigData", rdoc::__class)
        .def(pybind11::init<const Link&, BoolSet, bool, BoolSet>(),
            rdoc::__init)
        .def(pybind11::init<const LinkSigData&>(), rdoc::__copy)
        .def("size", &LinkSigData::size, rdoc::size)
        .def("sequence", [](const LinkSigData& data) {
            pybind11::list ans;
            for (const auto& term : data.sequence())
                ans.append(term);
            return ans;
        }, rdoc::sequence)
        .def("swap", &LinkSigData::swap, rdoc::swap)
        ;
    regina::python::add_eq_operators(d, rdoc::__eq);
    regina::python::add_cmp_operators(d, rdoc::__cmp);
    regina::python::add_global_swap<LinkSigData, rdoc>(m);

    auto t = pybind11::class_<LinkSigData::Term>(d, "Term", rdoc::Term::__class)
        .def_readonly("crossing", &LinkSigData::Term::crossing,
            rdoc::Term::crossing)
        .def_readonly("strand", &LinkSigData::Term::strand, rdoc::Term::strand)
        .def_readonly("sign", &LinkSigData::Term::sign, rdoc::Term::sign)
        .def("makeSentinel", &LinkSigData::Term::makeSentinel,
            rdoc::Term::makeSentinel)
        ;
    regina::python::add_eq_operators(t, rdoc::Term::__eq);
    regina::python::add_cmp_operators(t, rdoc::Term::__cmp);

    RDOC_SCOPE_SWITCH(LinkSigPrintable)

    auto c = pybind11::class_<LinkSigPrintable>(m, "LinkSigPrintable",
            rdoc::__class)
        .def_static("encodeEmpty", &LinkSigPrintable::encodeEmpty,
            rdoc::encodeEmpty)
        .def_static("encodeUnknot", &LinkSigPrintable::encodeUnknot,
            rdoc::encodeUnknot)
        .def_static("encode", [](int generation, const LinkSigData& data) {
            switch (generation) {
                case 1: return LinkSigPrintable::encode<1>(data);
                case 2: return LinkSigPrintable::encode<2>(data);
                default: throw regina::InvalidArgument(
                    "Not a supported generation of knot/link signature");
            }
        }, rdoc::encode)
        .def_static("length", [](int generation, const LinkSigData& data) {
            switch (generation) {
                case 1: return LinkSigPrintable::length<1>(data);
                case 2: return LinkSigPrintable::length<2>(data);
                default: throw regina::InvalidArgument(
                    "Not a supported generation of knot/link signature");
            }
        }, rdoc::length)
        .def_static("generation", &LinkSigPrintable::generation,
            rdoc::generation)
        ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_SWITCH(LinkSigBinary)

    auto c = pybind11::class_<LinkSigBinary>(m, "LinkSigBinary", rdoc::__class)
        .def_static("encodeEmpty", &LinkSigBinary::encodeEmpty,
            rdoc::encodeEmpty)
        .def_static("encodeUnknot", &LinkSigBinary::encodeUnknot,
            rdoc::encodeUnknot)
        .def_static("encode", [](int generation, const LinkSigData& data) {
            switch (generation) {
                case 2: return LinkSigBinary::encode<2>(data);
                default: throw regina::InvalidArgument(
                    "Not a supported generation of knot/link signature");
            }
        }, rdoc::encode)
        .def_static("length", [](int generation, const LinkSigData& data) {
            switch (generation) {
                case 2: return LinkSigBinary::length<2>(data);
                default: throw regina::InvalidArgument(
                    "Not a supported generation of knot/link signature");
            }
        }, rdoc::length)
        .def_static("asString", &LinkSigBinary::asString, rdoc::asString)
        ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_SWITCH_MAIN

    regina::python::add_concept<rdoc::LinkSigEncoding>(m, "LinkSigEncoding");

    RDOC_SCOPE_END
}

