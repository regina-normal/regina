
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

#include "pybind11/pybind11.h"
#include "link/link.h"
#include "../helpers.h"
#include "../docstrings/link/linksig.h"

using regina::BoolSet;
using regina::Link;
using regina::LinkSigCompact;
using regina::LinkSigData;
using regina::LinkSigPacked;
using regina::LinkSigPrintable;

void addLinkSig(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(LinkSigData)

    auto d = pybind11::class_<LinkSigData>(m, "LinkSigData", rdoc_scope)
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

    regina::python::add_global_swap<LinkSigData>(m, rdoc::global_swap);

    RDOC_SCOPE_INNER_BEGIN(Term)

    auto t = pybind11::class_<LinkSigData::Term>(d, "Term", rdoc_inner_scope)
        .def_readonly("crossing", &LinkSigData::Term::crossing,
            rdoc_inner::crossing)
        .def_readonly("strand", &LinkSigData::Term::strand, rdoc_inner::strand)
        .def_readonly("sign", &LinkSigData::Term::sign, rdoc_inner::sign)
        .def("makeSentinel", &LinkSigData::Term::makeSentinel,
            rdoc_inner::makeSentinel)
        ;
    regina::python::add_eq_operators(t, rdoc_inner::__eq);
    regina::python::add_cmp_operators(t, rdoc_inner::__cmp);

    RDOC_SCOPE_INNER_END
    RDOC_SCOPE_SWITCH(LinkSigPrintable)

    auto c = pybind11::class_<LinkSigPrintable>(m, "LinkSigPrintable",
            rdoc_scope)
        .def_static("encodeEmpty", &LinkSigPrintable::encodeEmpty,
            rdoc::encodeEmpty)
        .def_static("encodeUnknot", &LinkSigPrintable::encodeUnknot,
            rdoc::encodeUnknot)
        .def_static("encode", &LinkSigPrintable::encode, rdoc::encode)
        .def_static("length", &LinkSigPrintable::length, rdoc::length)
        ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_SWITCH(LinkSigCompact)

    auto c = pybind11::class_<LinkSigCompact>(m, "LinkSigCompact", rdoc_scope)
        .def_static("encodeEmpty", &LinkSigCompact::encodeEmpty,
            rdoc::encodeEmpty)
        .def_static("encodeUnknot", &LinkSigCompact::encodeUnknot,
            rdoc::encodeUnknot)
        .def_static("encode", &LinkSigCompact::encode, rdoc::encode)
        .def_static("length", &LinkSigCompact::length, rdoc::length)
        ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_SWITCH(LinkSigPacked)

    auto c = pybind11::class_<LinkSigPacked>(m, "LinkSigPacked", rdoc_scope)
        // We have to manage conversions between regina::ByteSequence and
        // pybind11::bytes in the bindings below.
        .def_static("encodeEmpty", []() {
            return pybind11::bytes(LinkSigPacked::encodeEmpty().asString());
        }, rdoc::encodeEmpty)
        .def_static("encodeUnknot", []() {
            return pybind11::bytes(LinkSigPacked::encodeUnknot().asString());
        }, rdoc::encodeUnknot)
        .def_static("encode", [](const LinkSigData& data) {
            return pybind11::bytes(LinkSigPacked::encode(data).asString());
        }, rdoc::encode)
        .def_static("length", &LinkSigPacked::length, rdoc::length)
        ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_END
}

