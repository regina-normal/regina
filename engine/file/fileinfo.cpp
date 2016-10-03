
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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
#include <iostream>
#include <sstream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "file/fileinfo.h"

namespace regina {

#define STARTS_FALSE 0
#define STARTS_TRUE 1
#define STARTS_COULD_NOT_OPEN 2

// const int FileInfo::TYPE_BINARY = 1; // OBSOLETE as of Regina 4.94.
const int FileInfo::TYPE_XML = 2;

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

FileInfo* FileInfo::identify(const std::string& idPathname) {
    // Check for an XML file.
    int starts = fileStartsWith(idPathname.c_str(), "<?xml");
    if (starts == STARTS_COULD_NOT_OPEN)
        return 0;

    FileInfo* ans = 0;
    if (starts == STARTS_TRUE) {
        ans = new FileInfo();
        ans->compressed = false;
    } else {
        std::ifstream file(idPathname.c_str(),
            std::ios_base::in | std::ios_base::binary);
        if (file && file.peek() == 0x1f) {
            ::boost::iostreams::filtering_istream in;
            in.push(::boost::iostreams::gzip_decompressor());
            in.push(file);

            std::string s;
            in >> s;
            if ((! in.eof()) && (s == "<?xml")) {
                ans = new FileInfo();
                ans->compressed = true;
            }
        }
    }

    if (ans) {
        ans->pathname_ = idPathname;
        ans->type_ = FileInfo::TYPE_XML;
        ans->typeDescription_ = "XML Regina data file";

        // Make it an invalid file until we know otherwise.
        ans->invalid = true;

        std::ifstream file(idPathname.c_str(),
            std::ios_base::in | std::ios_base::binary);
        if (! file)
            return ans;

        ::boost::iostreams::filtering_istream in;
        if (file.peek() == 0x1f)
            in.push(::boost::iostreams::gzip_decompressor());
        in.push(file);

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
        if (s != "<reginadata")
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
        ans->engine_ = s.substr(8, pos - 8);

        // That's as far as we need to go; we've extracted everything we want.
        ans->invalid = false;
        return ans;
    }

    // Unknown format.
    return 0;
}

void FileInfo::writeTextShort(std::ostream& out) const {
    out << "File information: " << typeDescription_;
    if (compressed)
        out << " (compressed)";
}

void FileInfo::writeTextLong(std::ostream& out) const {
    out << "Regina data\n" << typeDescription_;
    if (compressed)
        out << " (compressed)";
    out << '\n';

    if (invalid)
        out << "File contains invalid metadata.\n";
    else
        out << "Engine " << engine_ << '\n';
}

} // namespace regina

