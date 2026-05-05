
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

using regina::Base64Encoder;
using regina::Base64SigEncoding;
using regina::Bitmask;
using regina::BitEncoder;
using regina::PackedSigEncoder;

namespace regina::python {
    // For Python (but not C++), we need Base64Decoder and BitDecoder
    // to keep a deep copy of the base64 string / byte sequence.  We define
    // a wrapper class Decoder_Copy<T> here that manages this, where T should
    // be one of the class templates Base64Decoder, BitDecoder, etc.
    //
    // Awkwardly, we need to copy the string _before_ passing its iterators to
    // the relevant decoder constructor, which means we can't just keep the
    // string as member variable (since base classes are initialised first).
    // Instead we use _another_ base class holding the string, which is
    // initialised before Base64Decoder / BitDecoder. Bleh.

    struct StringStorage {
        std::string str_;
        StringStorage(std::string&& str) : str_(std::move(str)) {}
    };

    template <template <typename> class MainDecoder>
    class Decoder_Copy : private StringStorage,
            public MainDecoder<std::string::const_iterator> {
        private:
            using Base = MainDecoder<std::string::const_iterator>;

        public:
            Decoder_Copy(std::string str, bool stripWhitespace)
            requires std::same_as<typename Base::Encoding, std::string> :
                    StringStorage(std::move(str)),
                    Base(str_.begin(), str_.end(), stripWhitespace) {
            }
            Decoder_Copy(pybind11::bytes bytes)
            requires std::same_as<typename Base::Encoding, ByteSequence> :
                    StringStorage(bytes),
                    Base(str_.begin(), str_.end()) {
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
    class SafeBitEncoder {
        private:
            regina::BitEncoder encoder_;
            bool valid { true };

        public:
            SafeBitEncoder() = default;

            regina::BitEncoder& encoder() {
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

    RDOC_SCOPE_SWITCH(Base64Encoder)

    auto be = pybind11::class_<Base64Encoder>(m, "Base64Encoder", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def("str",
            static_cast<const std::string&(Base64Encoder::*)() const &>(
                &Base64Encoder::str),
            rdoc::str)
        .def_static("integerWidth", &Base64Encoder::integerWidth,
            rdoc::integerWidth)
        .def("encodeSingle", &Base64Encoder::encodeSingle<long>,
            rdoc::encodeSingle)
        .def("encodeSize", &Base64Encoder::encodeSize, rdoc::encodeSize)
        .def("encodeInt", &Base64Encoder::encodeInt<long>, rdoc::encodeInt)
        .def("encodeInts",
            pybind11::overload_cast<const std::vector<long>&, int>(
                &Base64Encoder::encodeInts<const std::vector<long>&>),
            rdoc::encodeInts)
        .def("encodeBits", &Base64Encoder::encodeBits<Bitmask>,
            rdoc::encodeBits)
        .def("encodeTrits",
            pybind11::overload_cast<const std::vector<uint8_t>&>(
                &Base64Encoder::encodeTrits<const std::vector<uint8_t>&>),
            rdoc::encodeTrits)
        .def("append", &Base64Encoder::append, rdoc::append)
        .def("reserve", &Base64Encoder::reserve, rdoc::reserve)
        .def_readonly_static("spare", &Base64Encoder::spare)
    ;
    regina::python::add_eq_operators(be);

    RDOC_SCOPE_SWITCH(Base64Decoder)

    using Decoder = regina::Base64Decoder<std::string::const_iterator>;
    using Wrapper = regina::python::Decoder_Copy<regina::Base64Decoder>;
    auto bd = pybind11::class_<Wrapper>(m, "Base64Decoder", rdoc_scope)
        .def(pybind11::init<const std::string&, bool>(),
            pybind11::arg(), pybind11::arg("skipInitialWhitespace") = true,
            rdoc::__init)
        .def("skipWhitespace", &Decoder::skipWhitespace, rdoc::skipWhitespace)
        .def("done",
            pybind11::overload_cast<>(&Decoder::done, pybind11::const_),
            rdoc::done)
        .def("peek", &Decoder::peek, rdoc::peek)
        .def("remaining", &Decoder::remaining, rdoc::remaining)
        .def("skip", &Decoder::skip, rdoc::skip)
        .def("decodeSingle", &Decoder::decodeSingle<long>, rdoc::decodeSingle)
        .def("decodeSize", &Decoder::decodeSize, rdoc::decodeSize)
        .def("decodeInt", &Decoder::decodeInt<long>, rdoc::decodeInt)
        .def("decodeInts", [](Wrapper& dec, size_t count, int nChars) {
            // Reimplement this using decodeInt(), since the iterators for
            // pybind11::list have the wrong value type.
            pybind11::list ans;
            for (size_t i = 0; i < count; ++i)
                ans.append(dec.decodeInt<long>(nChars));
            return ans;
        })
        .def("decodeBits", &Decoder::decodeBits<Bitmask>, rdoc::decodeBits)
        // overload_cast cannot handle template vs non-template overloads.
        .def("decodeTrits",
            static_cast<std::array<uint8_t, 3>(Decoder::*)()>(
                &Decoder::decodeTrits),
            rdoc::decodeTrits)
        .def_static("isValid", &Decoder::isValid, rdoc::isValid)
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
        pybind11::overload_cast<bool>(&Decoder::done, pybind11::const_),
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

    using Decoder = regina::PackedSigDecoder<std::string::const_iterator>;
    using Wrapper = regina::python::Decoder_Copy<regina::PackedSigDecoder>;
    auto pd = pybind11::class_<Wrapper>(m, "PackedSigDecoder", rdoc_scope)
        .def(pybind11::init<pybind11::bytes>(), rdoc::__init)
        .def("done", &Decoder::done, rdoc::done)
        .def("remaining", &Decoder::remaining, rdoc::remaining)
        .def("next", &Decoder::next<long>, rdoc::next)
        .def("decodeSize", &Decoder::decodeSize, rdoc::decodeSize)
        .def("decodeInts", [](Wrapper& dec, size_t count, int nChars) {
            // Again, the iterators for pybind11::list have the wrong value
            // type.  Use an intermediate data structure, since we don't have
            // decodeInt() to fall back on this time.
            auto result = dec.decodeInts<long>(count, nChars);
            pybind11::list ans;
            for (auto i : result)
                ans.append(i);
            return ans;
        })
        .def("decodeBits", &Decoder::decodeBits<Bitmask>, rdoc::decodeBits)
        // overload_cast cannot handle template vs non-template overloads.
        .def("decodeTrits",
            static_cast<std::array<uint8_t, 4>(Decoder::*)()>(
                &Decoder::decodeTrits),
            rdoc::decodeTrits)
    ;
    regina::python::add_eq_operators(pd);

    RDOC_SCOPE_SWITCH(BitEncoder)

    using regina::python::SafeBitEncoder;
    auto te = pybind11::class_<SafeBitEncoder>(m, "BitEncoder", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def("bytes", &SafeBitEncoder::bytes, rdoc::bytes)
        .def("encodeBit", [](SafeBitEncoder& enc, bool bit) {
            enc.encoder().encodeBit(bit);
        }, rdoc::encodeBit)
        .def("encodeInt", [](SafeBitEncoder& enc, int n, unsigned long b) {
            enc.encoder().encodeInt(n, b);
        }, rdoc::encodeInt)
        .def("encodeBitmask", [](SafeBitEncoder& enc, int n, const Bitmask& b) {
            enc.encoder().encodeBitmask(n, b);
        }, rdoc::encodeBitmask)
        .def("reserveBits", [](SafeBitEncoder& enc, size_t count) {
            enc.encoder().reserveBits(count);
        }, rdoc::reserveBits)
        .def("reserveBytes", [](SafeBitEncoder& enc, size_t count) {
            enc.encoder().reserveBytes(count);
        }, rdoc::reserveBytes)
    ;
    regina::python::add_eq_operators(te);

    RDOC_SCOPE_SWITCH(BitDecoder)

    using Decoder = regina::BitDecoder<std::string::const_iterator>;
    using Wrapper = regina::python::Decoder_Copy<regina::BitDecoder>;
    auto td = pybind11::class_<Wrapper>(m, "BitDecoder", rdoc_scope)
        .def(pybind11::init<pybind11::bytes>(), rdoc::__init)
        .def("maybeDone", &Decoder::maybeDone, rdoc::maybeDone)
        .def("noMoreBits", &Decoder::noMoreBits, rdoc::noMoreBits)
        .def("remainingBits", &Decoder::remainingBits, rdoc::remainingBits)
        .def("decodeBit", &Decoder::decodeBit, rdoc::decodeBit)
        .def("decodeInt", &Decoder::decodeInt<unsigned long>, rdoc::decodeInt)
        .def("decodeBitmask", &Decoder::decodeBitmask<Bitmask>,
            rdoc::decodeBitmask)
        .def("flushByte", &Decoder::flushByte, rdoc::flushByte)
    ;
    regina::python::add_eq_operators(td);

    RDOC_SCOPE_END
}

