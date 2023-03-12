
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

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include "file/fileinfo.h"
#include "utilities/zstr.h"

namespace regina {

#define STARTS_FALSE 0
#define STARTS_TRUE 1
#define STARTS_COULD_NOT_OPEN 2

namespace {
    /**
     * Does the given file begin with the given set of characters?
     *
     * Returns STARTS_FALSE, STARTS_TRUE or STARTS_COULD_NOT_OPEN.
     */
    int fileStartsWith(const char* file, const char* prefix) {
        FILE* f = fopen(file, "rb");
        if (! f)
            return STARTS_COULD_NOT_OPEN;

        int ans = STARTS_FALSE;
        size_t len = strlen(prefix);
        char* buf = new char[len];

        if (fread(buf, sizeof(char), len, f) == len)
            if (strncmp(buf, prefix, len) == 0)
                ans = STARTS_TRUE;

        delete[] buf;
        fclose(f);
        return ans;
    }
}

std::optional<FileInfo> FileInfo::identify(std::string idPathname) {
    // Check for an XML file.
    int starts = fileStartsWith(idPathname.c_str(), "<?xml");
    if (starts == STARTS_COULD_NOT_OPEN)
        return std::nullopt;

    bool compressed = false;
    if (starts != STARTS_TRUE) {
        // Try for compressed XML.
        std::ifstream file(idPathname.c_str(),
            std::ios_base::in | std::ios_base::binary);
        if (! file)
            return std::nullopt;
        try {
            zstr::istream in(file);

            std::string s;
            in >> s;
            if ((! in.eof()) && (s == "<?xml"))
                compressed = true;
            else
                return std::nullopt;
        } catch (const zstr::Exception& e) {
            return std::nullopt;
        }
    }

    FileInfo ans;
    ans.compressed_ = compressed;
    ans.pathname_ = std::move(idPathname);
    ans.format_ = REGINA_CURRENT_FILE_FORMAT;

    // Note: we cannot use the idPathname argument from here on, since we moved its data out.
    // We must use ans.pathname_ instead.

    // Make it an invalid file until we know otherwise.
    ans.invalid_ = true;

    std::ifstream file(ans.pathname_.c_str(),
        std::ios_base::in | std::ios_base::binary);
    if (! file)
        return ans;

    try {
        zstr::istream in(file); // Can handle compressed or uncompressed.

        std::string s;

        // Start by slurping in the opening "<?xml".
        if (in.eof())
            return ans;
        in >> s;
        if (s != "<?xml")
            return ans;

        // Hunt for the matching "...?>".
        // Try skipping through several strings in case there are extra
        // arguments in the XML prologue (such as encoding or standalone
        // declarations).
        int i;
        for (i = 0; ; i++) {
            if (in.eof())
                return ans;
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
                return ans;
        }

        // The next thing we see should be the <reginadata ...> element.
        if (in.eof())
            return ans;
        in >> s;
        if (s == "<regina")
            ans.format_ = REGINA_XML_GEN_3;
        else if (s == "<reginadata")
            ans.format_ = REGINA_XML_GEN_2;
        else
            return ans;

        // Next should be the engine version.
        if (in.eof())
            return ans;
        in >> s;
        if (s.length() < 8)
            return ans;
        if (s.substr(0, 8).compare("engine=\"") != 0)
            return ans;

        // We've found the engine attribute; extract its value.
        std::string::size_type pos = s.find('"', 8);
        if (pos == std::string::npos)
            return ans;
        ans.engine_ = s.substr(8, pos - 8);
    } catch (const zstr::Exception& e) {
        return ans;
    }

    // That's as far as we need to go; we've extracted everything we want.
    ans.invalid_ = false;
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
    out << '\n';

    if (invalid_)
        out << "File contains invalid metadata.\n";
    else
        out << "Engine " << engine_ << '\n';
}

} // namespace regina

