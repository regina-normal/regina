
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include "file/fileinfo.h"
#include "utilities/exception.h"
#include "utilities/zstr.h"

namespace regina {

namespace {
    /**
     * Does the given file begin with the given set of characters?
     *
     * This throws a FileError if the given file cannot be read.
     */
    bool fileStartsWith(const char* file, const char* prefix) {
        FILE* f = fopen(file, "rb");
        if (! f)
            throw FileError("Could not open the given file");

        bool ans = false;
        size_t len = strlen(prefix);
        char* buf = new char[len];

        if (fread(buf, sizeof(char), len, f) == len)
            if (strncmp(buf, prefix, len) == 0)
                ans = true;

        delete[] buf;
        fclose(f);
        return ans;
    }
}

FileInfo FileInfo::identify(std::string idPathname) {
    // Check for an XML file.
    bool compressed = false;
    if (! fileStartsWith(idPathname.c_str(), "<?xml")) {
        // Try for compressed XML.
        std::ifstream file(idPathname.c_str(),
            std::ios_base::in | std::ios_base::binary);
        if (! file)
            throw FileError("Could not open the given file");
        try {
            zstr::istream in(file);

            std::string s;
            in >> s;
            if ((! in.eof()) && (s == "<?xml"))
                compressed = true;
            else
                throw InvalidInput("The given file does not appear to be XML");
        } catch (const zstr::Exception& e) {
            throw InvalidInput("An error occurred during file decompression");
        }
    }

    FileInfo ans;
    ans.compressed_ = compressed;
    ans.pathname_ = std::move(idPathname);
    ans.format_ = FileFormat::Current;

    // Note: we cannot use the idPathname argument from here on, since we moved
    // its data out.  We must use ans.pathname_ instead.

    std::ifstream file(ans.pathname_.c_str(),
        std::ios_base::in | std::ios_base::binary);
    if (! file)
        throw FileError("Could not open the given file");

    try {
        zstr::istream in(file); // Can handle compressed or uncompressed.

        std::string s;

        // Start by slurping in the opening "<?xml".
        if (in.eof())
            throw InvalidInput("Missing XML file prefix");
        in >> s;
        if (s != "<?xml")
            throw InvalidInput("Invalid XML file prefix");

        // Hunt for the matching "...?>".
        // Try skipping through several strings in case there are extra
        // arguments in the XML prologue (such as encoding or standalone
        // declarations).
        int i;
        for (i = 0; ; i++) {
            if (in.eof())
                throw InvalidInput("Incomplete XML file prefix");
            in >> s;
            if (s.length() >= 2 &&
                    s[s.length() - 2] == '?' &&
                    s[s.length() - 1] == '>')
                break;

            // If we can't find it after enough tries, just give up.
            // Ten tries should be more than sufficient, since the current XML
            // spec supports only version, encoding and standalone arguments
            // at present.
            if (i >= 10)
                throw InvalidInput("Overlong XML file prefix");
        }

        // The next thing we see should be the <reginadata ...> element.
        if (in.eof())
            throw InvalidInput("Missing root Regina XML element");
        in >> s;
        if (s == "<regina")
            ans.format_ = FileFormat::XmlGen3;
        else if (s == "<reginadata")
            ans.format_ = FileFormat::XmlGen2;
        else
            throw InvalidInput("Invalid root Regina XML element");

        // Next should be the engine version.
        if (in.eof())
            throw InvalidInput("Missing engine version");
        in >> s;
        if (s.length() < 8 || s.substr(0, 8).compare("engine=\"") != 0)
            throw InvalidInput("Invalid engine version");

        // We've found the engine attribute; extract its value.
        std::string::size_type pos = s.find('"', 8);
        if (pos == std::string::npos)
            throw InvalidInput("Malformed engine version");
        ans.engine_ = s.substr(8, pos - 8);
    } catch (const zstr::Exception& e) {
        throw InvalidInput("An error occurred during file decompression");
    }

    // That's as far as we need to go; we've extracted everything we want.
    return ans;
}

void FileInfo::writeTextShort(std::ostream& out) const {
    out << "Regina data: " << formatDescription();
    if (compressed_)
        out << ", compressed";
}

void FileInfo::writeTextLong(std::ostream& out) const {
    // All supported file types are XML.
    out << "Regina data: " << formatDescription();
    if (compressed_)
        out << ", compressed";
    out << "\nEngine " << engine_ << '\n';
}

} // namespace regina

