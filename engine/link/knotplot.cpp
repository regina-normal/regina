
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cctype>
#include "link/spatiallink.h"
#include "utilities/exception.h"

// From wikipedia: IEEE-754 binary32 should represent
// 00111110 00100000 00000000 00000000 = 0.15625.

static bool isFloatBigEndian() {
    uint8_t data[4] = { 62, 32, 0, 0 };
    float f = *reinterpret_cast<const float*>(data);
    return (std::abs(f - 0.15625) < 0.00001);
}

static bool isFloatLittleEndian() {
    uint8_t data[4] = { 0, 0, 32, 62 };
    float f = *reinterpret_cast<const float*>(data);
    return (std::abs(f - 0.15625) < 0.00001);
}

// Converts two successive bytes into a single unsigned 16-bit integer, where
// the input is treated as big-endian (regardless of the endianness of the
// current platform).
//
// PRE: the array c has length at least 2.
static constexpr uint16_t KPInt16(const char c[]) {
    // The double-casts are to ensure that, if char is signed, then
    // -1 casts up to 255 (and not 65535).
    return (uint16_t(uint8_t(c[0])) << 8) |
        (uint16_t(uint8_t(c[1])));
}

// Converts four successive bytes into a single unsigned 32-bit integer, where
// the input is treated as big-endian (regardless of the endianness of the
// current platform).
//
// PRE: the array c has length at least 4.
static constexpr uint32_t KPInt32(const char c[]) {
    // The double-casts are to ensure that, if char is signed, then
    // -1 casts up to 255 (and not 65535).
    return (uint32_t(uint8_t(c[0])) << 24) |
        (uint32_t(uint8_t(c[1])) << 16) |
        (uint32_t(uint8_t(c[2])) << 8) |
        (uint32_t(uint8_t(c[3])));
}

// Converts four successive bytes into a single 32-bit float, where the input
// is treated as big-endian (regardless of the endianness of the current
// platform).
//
// THIS NEEDS TESTING ON DIFFERENT PLATFORMS.  I have seen a promise (on
// cppreferences.com) that float and double should use IEEE-754 binary32 and
// IEEE-754 binary64 respectively.
//
// However, I believe endianness still matters.  My own main platforms are
// all little-endian.
//
// Note: the current endianness test is a bit messy, but once we have C++20
// we should have access to std::endian.
//
// PRE: the array c has length at least 4.
float KPFloat32(const char c[], bool bigEndian) {
    if (bigEndian) {
        return *(reinterpret_cast<const float*>(c));
    } else {
        const char x[4] = { c[3], c[2], c[1], c[0] };
        return *(reinterpret_cast<const float*>(x));
    }
}

// Converts eight successive bytes into a single 64-bit double, where the input
// is treated as big-endian (regardless of the endianness of the current
// platform).
//
// THIS NEEDS TESTING ON DIFFERENT PLATFORMS.  See the comments above for
// KPFloat32().
//
// PRE: the array c has length at least 8.
double KPFloat64(const char c[], bool bigEndian) {
    if (bigEndian) {
        return *(reinterpret_cast<const double*>(c));
    } else {
        const char x[8] = { c[7], c[6], c[5], c[4], c[3], c[2], c[1], c[0] };
        return *(reinterpret_cast<const double*>(x));
    }
}

namespace regina {

SpatialLink SpatialLink::fromKnotPlot(const char* filename) {
    // When we move to C++23, I think we get access to fixed-size floating
    // point types.  I should check this.
    if (sizeof(float) != 4 || sizeof(double) != 8)
        throw NotImplemented("fromKnotPlot(): binary file format requires "
            "a platform with 32-bit floats and 64-bit doubles");

    bool floatBigEndian;
    if (isFloatLittleEndian())
        floatBigEndian = false;
    else if (isFloatBigEndian())
        floatBigEndian = true;
    else
        throw NotImplemented("fromKnotPlot(): could not determine the "
            "endianness for floating point numbers on the current platform");

    std::ifstream in(filename, std::ios::binary);
    if (! in)
        throw FileError("fromKnotPlot(): could not open the given file");

    // The file _must_ begin with "KnotPlot 1.0".
    char banner[12];
    in.read(banner, 12);
    if (! in)
        throw InvalidInput("fromKnotPlot(): unexpected end of file");
    if (std::memcmp(banner, "KnotPlot 1.0", 12) != 0)
        throw InvalidInput("fromKnotPlot(): file has no KnotPlot header");

    // The header ends with '\f' followed by another arbitrary character.
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\f');
    if (! in)
        throw InvalidInput("fromKnotPlot(): unexpected end of file");
    in.get();
    if (! in)
        throw InvalidInput("fromKnotPlot(): unexpected end of file");

    SpatialLink ans;

    bool finished = false;
    while (! finished) {
        // Extract the next field.
        char field[4];
        in.read(field, 4);
        if (! in)
            throw InvalidInput("fromKnotPlot(): unexpected end of file");

        if (std::islower(field[0])) {
            if (std::islower(field[1])) {
                // This field contains no data at all.
                switch (KPInt32(field)) {
                    case KPInt32("endf"):
                        // End of data file.
                        finished = true;
                        break;
                    case KPInt32("comp"):
                        ans.components_.emplace_back();
                        break;
                }
            } else
                throw InvalidInput("fromKnotPlot(): invalid field name");
        } else if (std::isupper(field[0])) {
            if (std::islower(field[1])) {
                // This field contains exactly 4 bytes of data.
                char data[4];
                in.read(data, 4);
                if (! in)
                    throw InvalidInput(
                        "fromKnotPlot(): unexpected end of file");

                switch (KPInt32(field)) {
                    case KPInt32("Attr"):
                        // Attributes are stored as a 4-byte integer:
                        // the lowest order bit is 1 for closed, or 0 for open.
                        // If the attributes are missing entirely then the
                        // component is assumed to be closed.
                        uint32_t attr = KPInt32(data);
                        if (! (attr & 1))
                            throw InvalidInput("fromKnotPlot(): file contains "
                                "an open link component, with free ends");
                        break;
                }
            } else if (std::isupper(field[1])) {
                // This field contains a 4-byte integer indicating how many
                // _subsequent_ bytes the field contains.
                char lenData[4];
                in.read(lenData, 4);
                if (! in)
                    throw InvalidInput("fromKnotPlot(): "
                        "unexpected end of file");

                uint32_t len = KPInt32(lenData);

                switch (KPInt32(field)) {
#if 0
                    case KPInt32("NAME"):
                        // Name of the knot/link.
                        // For now we do not actually use this.
                        {
                            char* data = new char[len];
                            in.read(data, len);
                            if (! in) {
                                delete [] data;
                                throw InvalidInput("fromKnotPlot(): "
                                    "unexpected end of file");
                            }
                            std::string name(data, len);
                            delete[] data;
                            // ... send name to wherever it needs to go.
                        }
                        break;
#endif
                    case KPInt32("LOCS"):
                        // 2-byte unsigned integers for coordinates.
                        // Preceeded by scale and offset data; I am assuming
                        // based on inspecting some KnotPlot sample files that
                        // these are stored as four 4-byte floats (scale,
                        // offset_x, offset_y, offset_z).
                        if (ans.components_.empty())
                            throw InvalidInput("fromKnotPlot(): found "
                                "coordinates before the first component");
                        if (len % 6 != 4)
                            throw InvalidInput("fromKnotPlot(): invalid "
                                "LOCS field length");

                        {
                            char data[16];

                            in.read(data, 16);
                            if (! in)
                                throw InvalidInput(
                                    "fromKnotPlot(): unexpected end of file");

                            float scale = KPFloat32(data, floatBigEndian);
                            float offset[3] = {
                                KPFloat32(data + 4, floatBigEndian),
                                KPFloat32(data + 8, floatBigEndian),
                                KPFloat32(data + 12, floatBigEndian) };

                            for (size_t i = 16; i < len; i += 6) {
                                in.read(data, 6);
                                if (! in)
                                    throw InvalidInput("fromKnotPlot(): "
                                        "unexpected end of file");

                                // Note: we already tested above that
                                // components_ is non-empty.
                                ans.components_.back().emplace_back(
                                    float(KPInt16(data)) * scale + offset[0],
                                    float(KPInt16(data + 2)) * scale +
                                        offset[1],
                                    float(KPInt16(data + 4)) * scale +
                                        offset[2]);
                            }
                        }
                        break;
                    case KPInt32("LOCF"):
                        // 4-byte floats for coordinates.
                        if (ans.components_.empty())
                            throw InvalidInput("fromKnotPlot(): found "
                                "coordinates before the first component");
                        if (len % 12 != 0)
                            throw InvalidInput("fromKnotPlot(): invalid "
                                "LOCF field length");

                        {
                            char data[12];
                            for (size_t i = 0; i < len; i += 12) {
                                in.read(data, 12);
                                if (! in)
                                    throw InvalidInput("fromKnotPlot(): "
                                        "unexpected end of file");

                                // Note: we already tested above that
                                // components_ is non-empty.
                                ans.components_.back().emplace_back(
                                    KPFloat32(data, floatBigEndian),
                                    KPFloat32(data + 4, floatBigEndian),
                                    KPFloat32(data + 8, floatBigEndian));
                            }
                        }
                        break;
                    case KPInt32("LOCD"):
                        // 8-byte doubles for coordinates.
                        if (ans.components_.empty())
                            throw InvalidInput("fromKnotPlot(): found "
                                "coordinates before the first component");
                        if (len % 24 != 0)
                            throw InvalidInput("fromKnotPlot(): invalid "
                                "LOCD field length");

                        {
                            char data[24];
                            for (size_t i = 0; i < len; i += 24) {
                                in.read(data, 24);
                                if (! in)
                                    throw InvalidInput("fromKnotPlot(): "
                                        "unexpected end of file");

                                // Note: we already tested above that
                                // components_ is non-empty.
                                ans.components_.back().emplace_back(
                                    KPFloat64(data, floatBigEndian),
                                    KPFloat64(data + 8, floatBigEndian),
                                    KPFloat64(data + 16, floatBigEndian));
                            }
                        }
                        break;
                    case KPInt32("LOCC"):
                        // This format is not documented alongside the others,
                        // and I'm not convinced I'm interpreting it correctly.
                        // For now we explicitly disable it until such a time
                        // as we can find out exactly what it is meant to store.
                        //
                        // Below is the code for what I _thought_ it was meant
                        // to hold, but some tests on KnotPlot files from the
                        // wild suggest this is not actually what's going on.
                        //
                        // The good news: AFAICT this is used more often with
                        // open paths (not closed loops), which we do not
                        // support anyway.
                        throw InvalidInput("fromKnotPlot(): found a block "
                            "of translations (not coordinates), which "
                            "are not currently supported");
#if 0
                        // This appears to hold a series of translations,
                        // not absolute coordinates.
                        // The translations use signed 1-byte integers for
                        // coordinates.
                        // Before the translations we have two triples of
                        // 4-byte floats:
                        // - scaling factors for x, y, z translations;
                        // - the starting coordinates.
                        if (ans.components_.empty())
                            throw InvalidInput("fromKnotPlot(): found "
                                "coordinates before the first component");
                        if (len % 3 != 0)
                            throw InvalidInput("fromKnotPlot(): invalid "
                                "LOCC field length");

                        {
                            char data[12];

                            in.read(data, 12);
                            if (! in)
                                throw InvalidInput(
                                    "fromKnotPlot(): unexpected end of file");
                            float scale[3] = {
                                KPFloat32(data + 4, floatBigEndian),
                                KPFloat32(data + 8, floatBigEndian),
                                KPFloat32(data + 12, floatBigEndian) };

                            in.read(data, 12);
                            if (! in)
                                throw InvalidInput(
                                    "fromKnotPlot(): unexpected end of file");
                            Node pos(
                                KPFloat32(data + 4, floatBigEndian),
                                KPFloat32(data + 8, floatBigEndian),
                                KPFloat32(data + 12, floatBigEndian));
                            // Note: we already tested above that
                            // components_ is non-empty.
                            ans.components_.back().push_back(pos);

                            for (size_t i = 24; i < len; i += 3) {
                                in.read(data, 3);
                                if (! in)
                                    throw InvalidInput("fromKnotPlot(): "
                                        "unexpected end of file");

                                // Cast from char to int8_t, because we have
                                // no guarantee that char is signed.
                                pos.x += (float(int8_t(data[0])) * scale[0]);
                                pos.y += (float(int8_t(data[1])) * scale[1]);
                                pos.z += (float(int8_t(data[2])) * scale[2]);
                                ans.components_.back().push_back(pos);
                            }
                        }
                        break;
#endif
                    /*
                    case KPInt32("COLR"):
                        // This holds an RGB triple specifying the colour of
                        // the current component.  We might wish to support
                        // this at some later date.
                    */
                    default:
                        // Skip over the remainder of this field.
                        in.ignore(len);
                        if (! in)
                            throw InvalidInput(
                                "fromKnotPlot(): unexpected end of file");
                        break;
                }
            } else
                throw InvalidInput("fromKnotPlot(): invalid field name");
        } else
            throw InvalidInput("fromKnotPlot(): invalid field name");
    }

    if (ans.components_.empty()) {
        // Assume the file used some other method of storing coordinates, and
        // that we were not able to read it.
        throw InvalidInput("fromKnotPlot(): no coordinates could be read");
    }

    return ans;
}

} // namespace regina

