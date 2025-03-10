
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include <cstdio>
#include <filesystem>
#include <sys/stat.h>
#include "packet/attachment.h"
#include "utilities/base64.h"
#include "utilities/xmlutils.h"

#define BASE64_LINE_LEN 76

namespace regina {

Attachment::Attachment(const char* pathname) :
        data_(nullptr), size_(0), alloc_(OwnershipPolicy::OwnNew) {
    // Use FILE* so we can call fstat().

    // Open the file.
    FILE* in = fopen(pathname, "rb");
    if (! in)
        return;

    // Get the file size.
    struct stat s;
    if (fstat(fileno(in), &s)) {
        fclose(in);
        return;
    }
    size_t size = s.st_size;

    if (size == 0) {
        fclose(in);
        return;
    }

    // Read the file contents.
    char* data = new char[size];
    if (fread(data, 1, size, in) != size) {
        fclose(in);
        delete[] data;
        return;
    }

    // Is there more to the file that we weren't expecting?
    char c;
    if (fread(&c, 1, 1, in) > 0) {
        fclose(in);
        delete[] data;
        return;
    }

    // All good!
    fclose(in);

    data_ = data;
    size_ = size;
    try {
        // The choice bewteen string() vs generic_string() is irrelevant here.
        filename_ = std::filesystem::path(pathname).filename().string();
    } catch (const std::exception&) {
        filename_.clear();
    }
}

std::string Attachment::extension() const {
    if (! data_)
        return {};

    try {
        // The choice bewteen string() vs generic_string() is irrelevant here.
        return std::filesystem::path(filename_).extension().string();
    } catch (const std::exception&) {
        return {};
    }
}

void Attachment::reset() {
    PacketChangeSpan span(*this);

    if (data_) {
        if (alloc_ == OwnershipPolicy::OwnMalloc)
            ::free(data_);
        else
            delete[] data_;
    }

    data_ = nullptr;
    size_ = 0;
    alloc_ = OwnershipPolicy::OwnNew;
    filename_.clear();
}

void Attachment::reset(char* data, size_t size, OwnershipPolicy alloc,
        std::string filename) {
    PacketChangeSpan span(*this);

    // Out with the old data.
    if (data_) {
        if (alloc_ == OwnershipPolicy::OwnMalloc)
            ::free(data_);
        else
            delete[] data_;
    }

    // In with the new.
    if (data) {
        data_ = data;
        size_ = size;

        if (alloc == OwnershipPolicy::DeepCopy) {
            data_ = static_cast<char*>(::malloc(size_));
            ::memcpy(data_, static_cast<const char*>(data), size_);
            alloc_ = OwnershipPolicy::OwnMalloc;
        } else
            alloc_ = alloc;
    } else {
        data_ = nullptr;
        size_ = 0;
        alloc_ = OwnershipPolicy::OwnNew;
    }

    filename_ = std::move(filename);
}

bool Attachment::save(const char* pathname) const {
    if (! data_)
        return false;

    // Use FILE* for symmetry with the file load routine.

    // Open the file.
    FILE* out = fopen(pathname, "wb");
    if (!out)
        return false;

    // Is there anything to write?
    if (fwrite(data_, 1, size_, out) != size_) {
        fclose(out);
        return false;
    }

    // All done.
    fclose(out);
    return true;
}

void Attachment::writeXMLPacketData(std::ostream& out, FileFormat format,
        bool anon, PacketRefs& refs) const {
    char* base64 = nullptr;
    size_t len64 = 0;

    if (data_)
        len64 = base64Encode(data_, size_, &base64);

    if (! base64) {
        // Either we have an empty attachment, or the base64 conversion failed.
        writeXMLHeader(out, "attachment", format, anon, refs, true,
            std::pair("encoding", "null"), std::pair("filename", ""));
        if (format == FileFormat::XmlGen2)
            out << "  <pdf encoding=\"null\"></pdf>\n";
        if (! anon)
            writeXMLTreeData(out, format, refs);
        writeXMLFooter(out, "attachment", format);
        return;
    }

    writeXMLHeader(out, "attachment", format, anon, refs, true,
        std::pair("encoding", "base64"),
        std::pair("filename", regina::xml::xmlEncodeSpecialChars(filename_)));
    if (format == FileFormat::XmlGen2)
        out << "  <pdf encoding=\"base64\">\n";

    const char* pos = base64;
    while (len64 > BASE64_LINE_LEN) {
        out.write(pos, BASE64_LINE_LEN);
        out << std::endl;

        pos += BASE64_LINE_LEN;
        len64 -= BASE64_LINE_LEN;
    }
    if (len64 > 0) {
        out.write(pos, len64);
        out << std::endl;
    }

    if (format == FileFormat::XmlGen2)
        out << "  </pdf>\n";
    if (! anon)
        writeXMLTreeData(out, format, refs);
    writeXMLFooter(out, "attachment", format);

    delete[] base64;
}

} // namespace regina

