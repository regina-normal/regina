
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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "utilities/bitmask.h"
#include "utilities/sigutils.h"
#include "../helpers.h"
#include "../helpers/bytesequence.h"
#include "../docstrings/utilities/sigutils.h"

using regina::Base64SigEncoder;
using regina::Base64SigEncoding;
using regina::Bitmask;
using regina::BitSigEncoder;
using regina::PackedSigEncoder;

using Base64Decoder = regina::Base64SigDecoder<std::string::const_iterator>;
using PackedDecoder = regina::PackedSigDecoder<std::string::const_iterator>;
using BitDecoder = regina::BitSigDecoder<std::string::const_iterator>;

namespace regina::python {
    // For Python (but not C++), we need Base64SigDecoder and PackedSigDecoder
    // to keep a deep copy of the base64 string / byte sequence.
    //
    // Awkwardly, we need to copy the string _before_ passing its iterators to
    // the relevant decoder constructor, which means we can't just keep the
    // string as member variable (since base classes are initialised first).
    // Instead we use _another_ base class holding the string, which is
    // initialised before Base64SigDecoder / PackedSigDecoder. Bleh.

    struct StringStorage {
        std::string str_;
        StringStorage(std::string&& str) : str_(std::move(str)) {}
    };

    class Base64SigDecoder_Copy : private StringStorage, public Base64Decoder {
        public:
            Base64SigDecoder_Copy(std::string str, bool stripWhitespace) :
                    StringStorage(std::move(str)),
                    Base64Decoder(str_.begin(), str_.end(), stripWhitespace) {
            }
    };

    class PackedSigDecoder_Copy : private StringStorage, public PackedDecoder {
        public:
            PackedSigDecoder_Copy(pybind11::bytes bytes) :
                    StringStorage(bytes),
                    PackedDecoder(str_.begin(), str_.end()) {
            }
    };

    class BitSigDecoder_Copy : private StringStorage, public BitDecoder {
        public:
            BitSigDecoder_Copy(pybind11::bytes bytes) :
                    StringStorage(bytes),
                    BitDecoder(str_.begin(), str_.end()) {
            }
    };

    /**
     * Keeps track of whether the encoder has been invalidated by calling
     * bytes().  We use this to account for the fact that bytes() is an rvalue
     * member function - in C++ the compiler will force you to use std::move()
     * on the encoder, but there is AFAICT no way to enforce such things in
     * the Python bindings.  We work around this in Python by keeping a
     * separate validity flag alongside the encoder.
     */
    class SafeBitSigEncoder {
        private:
            regina::BitSigEncoder encoder_;
            bool valid { true };

        public:
            SafeBitSigEncoder() = default;

            regina::BitSigEncoder& encoder() {
                if (valid)
                    return encoder_;
                else
                    throw std::runtime_error(
                        "This encoder has been invalidated - you cannot "
                        "use it after calling bytes()");
            }

            auto bytes() {
                auto ans = std::move(encoder()).bytes();
                valid = false;
                return ans;
            }
    };
}

void addSigUtils(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Base64SigEncoding)

    #if defined(__GNUC__)
    // The class Base64SigEncoding is deprecated, but we still need to bind it.
    // Silence the inevitable deprecation warning that will occur.
    #pragma GCC diagnostic push
    #if defined(__clang__)
    #pragma GCC diagnostic ignored "-Wdeprecated"
    #else
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    #endif
    #endif
    auto c = pybind11::class_<Base64SigEncoding>(m, "Base64SigEncoding",
            rdoc_scope)
        .def_static("decodeSingle", &Base64SigEncoding::decodeSingle,
            rdoc::decodeSingle)
        .def_static("encodeSingle", &Base64SigEncoding::encodeSingle,
            rdoc::encodeSingle)
        .def_static("isValid", &Base64SigEncoding::isValid, rdoc::isValid)
        .def_static("encodeInt", &Base64SigEncoding::encodeInt<long>,
            rdoc::encodeInt)
        .def_static("decodeInt", &Base64SigEncoding::decodeInt<long>,
            rdoc::decodeInt)
        .def_static("encodeTrits",
            pybind11::overload_cast<const std::array<uint8_t, 0>&>(
                &Base64SigEncoding::encodeTrits<0>),
            rdoc::encodeTrits)
        .def_static("encodeTrits",
            pybind11::overload_cast<const std::array<uint8_t, 1>&>(
                &Base64SigEncoding::encodeTrits<1>),
            rdoc::encodeTrits)
        .def_static("encodeTrits",
            pybind11::overload_cast<const std::array<uint8_t, 2>&>(
                &Base64SigEncoding::encodeTrits<2>),
            rdoc::encodeTrits)
        .def_static("encodeTrits",
            pybind11::overload_cast<const std::array<uint8_t, 3>&>(
                &Base64SigEncoding::encodeTrits<3>),
            rdoc::encodeTrits)
        // overload_cast cannot handle template vs non-template overloads.
        .def_static("decodeTrits", static_cast<std::array<uint8_t, 3>(&)(char)>(
                Base64SigEncoding::decodeTrits),
            rdoc::decodeTrits)
    ;
    regina::python::no_eq_static(c);
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif

    RDOC_SCOPE_SWITCH(Base64SigEncoder)

    auto be = pybind11::class_<Base64SigEncoder>(m, "Base64SigEncoder",
            rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def("str",
            static_cast<const std::string&(Base64SigEncoder::*)() const &>(
                &Base64SigEncoder::str),
            rdoc::str)
        .def_static("integerWidth", &Base64SigEncoder::integerWidth,
            rdoc::integerWidth)
        .def("encodeSingle", &Base64SigEncoder::encodeSingle<long>,
            rdoc::encodeSingle)
        .def("encodeSize", &Base64SigEncoder::encodeSize, rdoc::encodeSize)
        .def("encodeInt", &Base64SigEncoder::encodeInt<long>, rdoc::encodeInt)
        .def("encodeInts",
            pybind11::overload_cast<const std::vector<long>&, int>(
                &Base64SigEncoder::encodeInts<const std::vector<long>&>),
            rdoc::encodeInts)
        .def("encodeBits", &Base64SigEncoder::encodeBits<Bitmask>,
            rdoc::encodeBits)
        .def("encodeTrits",
            pybind11::overload_cast<const std::vector<uint8_t>&>(
                &Base64SigEncoder::encodeTrits<const std::vector<uint8_t>&>),
            rdoc::encodeTrits)
        .def("append", &Base64SigEncoder::append, rdoc::append)
        .def("reserve", &Base64SigEncoder::reserve, rdoc::reserve)
        .def_readonly_static("spare", &Base64SigEncoder::spare)
    ;
    regina::python::add_eq_operators(be);

    RDOC_SCOPE_SWITCH(Base64SigDecoder)

    auto bd = pybind11::class_<regina::python::Base64SigDecoder_Copy>(m,
            "Base64SigDecoder", rdoc_scope)
        .def(pybind11::init<const std::string&, bool>(),
            pybind11::arg(), pybind11::arg("skipInitialWhitespace") = true,
            rdoc::__init)
        .def("skipWhitespace", &Base64Decoder::skipWhitespace,
            rdoc::skipWhitespace)
        .def("done",
            pybind11::overload_cast<>(&Base64Decoder::done, pybind11::const_),
            rdoc::done)
        .def("peek", &Base64Decoder::peek, rdoc::peek)
        .def("remaining", &Base64Decoder::remaining, rdoc::remaining)
        .def("skip", &Base64Decoder::skip, rdoc::skip)
        .def("decodeSingle", &Base64Decoder::decodeSingle<long>,
            rdoc::decodeSingle)
        .def("decodeSize", &Base64Decoder::decodeSize, rdoc::decodeSize)
        .def("decodeInt", &Base64Decoder::decodeInt<long>, rdoc::decodeInt)
        .def("decodeInts", [](regina::python::Base64SigDecoder_Copy& dec,
                size_t count, int nChars) {
            // Reimplement this using decodeInt(), since the iterators for
            // pybind11::list have the wrong value type.
            pybind11::list ans;
            for (size_t i = 0; i < count; ++i)
                ans.append(dec.decodeInt<long>(nChars));
            return ans;
        })
        .def("decodeBits", &Base64Decoder::decodeBits<Bitmask>,
            rdoc::decodeBits)
        // overload_cast cannot handle template vs non-template overloads.
        .def("decodeTrits",
            static_cast<std::array<uint8_t, 3>(Base64Decoder::*)()>(
                &Base64Decoder::decodeTrits),
            rdoc::decodeTrits)
        .def_static("isValid", &Base64Decoder::isValid, rdoc::isValid)
    ;
    #if defined(__GNUC__)
    // The routine done(bool) is deprecated, but we still need to bind it.
    // Silence the inevitable deprecation warning that will occur.
    #pragma GCC diagnostic push
    #if defined(__clang__)
    #pragma GCC diagnostic ignored "-Wdeprecated"
    #else
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    #endif
    #endif
    bd.def("done",
        pybind11::overload_cast<bool>(&Base64Decoder::done, pybind11::const_),
        rdoc::done_2); // deprecated
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif
    regina::python::add_eq_operators(bd);

    RDOC_SCOPE_SWITCH(PackedSigEncoder)

    auto pe = pybind11::class_<PackedSigEncoder>(m, "PackedSigEncoder",
            rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def("bytes",
            static_cast<const regina::ByteSequence&(PackedSigEncoder::*)()
                const &>(&PackedSigEncoder::bytes),
            rdoc::bytes)
        .def_static("integerWidth", &PackedSigEncoder::integerWidth,
            rdoc::integerWidth)
        .def("encodeSize", &PackedSigEncoder::encodeSize, rdoc::encodeSize)
        .def("encodeInts",
            pybind11::overload_cast<const std::vector<long>&, int>(
                &PackedSigEncoder::encodeInts<const std::vector<long>&>),
            rdoc::encodeInts)
        .def("encodeBits", &PackedSigEncoder::encodeBits<Bitmask>,
            rdoc::encodeBits)
        .def("encodeTrits",
            pybind11::overload_cast<const std::vector<uint8_t>&>(
                &PackedSigEncoder::encodeTrits<const std::vector<uint8_t>&>),
            rdoc::encodeTrits)
        .def("reserve", &PackedSigEncoder::reserve, rdoc::reserve)
    ;
    regina::python::add_eq_operators(pe);

    RDOC_SCOPE_SWITCH(PackedSigDecoder)

    auto pd = pybind11::class_<regina::python::PackedSigDecoder_Copy>(m,
            "PackedSigDecoder", rdoc_scope)
        .def(pybind11::init<pybind11::bytes>(), rdoc::__init)
        .def("done", &PackedDecoder::done, rdoc::done)
        .def("remaining", &PackedDecoder::remaining, rdoc::remaining)
        .def("next", &PackedDecoder::next<long>, rdoc::next)
        .def("decodeSize", &PackedDecoder::decodeSize, rdoc::decodeSize)
        .def("decodeInts", [](regina::python::PackedSigDecoder_Copy& dec,
                size_t count, int nChars) {
            // Again, the iterators for pybind11::list have the wrong value
            // type.  Use an intermediate data structure, since we don't have
            // decodeInt() to fall back on this time.
            auto result = dec.decodeInts<long>(count, nChars);
            pybind11::list ans;
            for (auto i : result)
                ans.append(i);
            return ans;
        })
        .def("decodeBits", &PackedDecoder::decodeBits<Bitmask>,
            rdoc::decodeBits)
        // overload_cast cannot handle template vs non-template overloads.
        .def("decodeTrits",
            static_cast<std::array<uint8_t, 4>(PackedDecoder::*)()>(
                &PackedDecoder::decodeTrits),
            rdoc::decodeTrits)
    ;
    regina::python::add_eq_operators(pd);

    RDOC_SCOPE_SWITCH(BitSigEncoder)

    using regina::python::SafeBitSigEncoder;
    auto te = pybind11::class_<SafeBitSigEncoder>(m, "BitSigEncoder",
            rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def("bytes", &SafeBitSigEncoder::bytes, rdoc::bytes)
        .def("encodeBit", [](SafeBitSigEncoder& enc, bool bit) {
            enc.encoder().encodeBit(bit);
        }, rdoc::encodeBit)
        .def("encodeInt", [](SafeBitSigEncoder& enc, int n, unsigned long b) {
            enc.encoder().encodeInt(n, b);
        }, rdoc::encodeInt)
        .def("encodeBitmask", [](SafeBitSigEncoder& enc, int n,
                const Bitmask& b) {
            enc.encoder().encodeBitmask(n, b);
        }, rdoc::encodeBitmask)
        .def("reserveBits", [](SafeBitSigEncoder& enc, size_t count) {
            enc.encoder().reserveBits(count);
        }, rdoc::reserveBits)
        .def("reserveBytes", [](SafeBitSigEncoder& enc, size_t count) {
            enc.encoder().reserveBytes(count);
        }, rdoc::reserveBytes)
    ;
    regina::python::add_eq_operators(te);

    RDOC_SCOPE_SWITCH(BitSigDecoder)

    auto td = pybind11::class_<regina::python::BitSigDecoder_Copy>(m,
            "BitSigDecoder", rdoc_scope)
        .def(pybind11::init<pybind11::bytes>(), rdoc::__init)
        .def("maybeDone", &BitDecoder::maybeDone, rdoc::maybeDone)
        .def("noMoreBits", &BitDecoder::noMoreBits, rdoc::noMoreBits)
        .def("remainingBits", &BitDecoder::remainingBits, rdoc::remainingBits)
        .def("decodeBit", &BitDecoder::decodeBit, rdoc::decodeBit)
        .def("decodeInt", &BitDecoder::decodeInt<unsigned long>,
            rdoc::decodeInt)
        .def("decodeBitmask", &BitDecoder::decodeBitmask<Bitmask>,
            rdoc::decodeBitmask)
    ;
    regina::python::add_eq_operators(td);

    RDOC_SCOPE_END
}

