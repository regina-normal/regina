
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file file/fileinfo.h
 *  \brief Deals with determining information about Regina data files.
 */

#ifndef __REGINA_FILEINFO_H
#ifndef __DOXYGEN
#define __REGINA_FILEINFO_H
#endif

#include <optional>
#include "core/output.h"
#include "file/fileformat.h"

namespace regina {

/**
 * \defgroup file File I/O
 * File formats and the filesystem
 */

/**
 * Stores information about a Regina data file, including file format and
 * version.
 *
 * Routine identify() can be used to determine this information for a
 * given file.
 *
 * As of Regina 4.94, the ancient first-generation binary files
 * (FileFormat::BinaryGen1) are no longer supported, and this class cannot
 * recognise them at all.  These have not been in use since mid-2002.
 * The only file formats that this class now recognises are Regina's newer
 * XML-based (compressed or uncompressed) data files.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement, though it does not implement (or need) its own custom swap()
 * function.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.
 *
 * \ingroup file
 */
class FileInfo : public Output<FileInfo> {
    private:
        std::string pathname_;
            /**< The pathname of the data file being described. */
        FileFormat format_;
            /**< Indicates which of Regina's XML file formats the file uses. */
        std::string engine_;
            /**< The version of the calculation engine that wrote this file. */
        bool compressed_;
            /**< \c true if this file is stored in compressed format,
                 \c false otherwise.  Currently this option only applies
                 to XML data files. */
        bool invalid_;
            /**< \c true if the file metadata could not be read,
                 \c false otherwise. */

    public:
        /**
         * Creates a new copy of the given file information.
         */
        FileInfo(const FileInfo&) = default;
        /**
         * Moves the contents of the given file information to this new object.
         *
         * The object that was passed will no longer be usable.
         */
        FileInfo(FileInfo&&) noexcept = default;
        /**
         * Returns the pathname of the data file being described.
         *
         * \i18n The \ref i18n "character encoding" used in the pathname will
         * be whatever was originally passed to identify().  This might or
         * might not be UTF-8, since it needs to be understood by the
         * low-level C/C++ file I/O routines.
         *
         * \return the pathname.
         */
        const std::string& pathname() const;
        /**
         * Returns which of Regina's file formats the data file uses.
         *
         * In particular, this encodes which generation of XML the file
         * uses, but does not encode whether the XML is compressed.
         *
         * \return the file format.
         */
        FileFormat format() const;
        /**
         * Returns a human-readable description of the file format used
         * by the data file.
         *
         * Like format(), this indicates which generation of XML the file
         * uses, but not whether the XML is compressed.
         *
         * \return a description of the file format.
         */
        std::string formatDescription() const;
        /**
         * Returns the version of the calculation engine that wrote this file.
         *
         * \return the engine version for this file.
         */
        const std::string& engine() const;
        /**
         * Returns whether this file is stored in compressed format.
         * Currently this option only applies to XML data files.
         *
         * \return \c true if this file is compressed or \c false otherwise.
         */
        bool isCompressed() const;
        /**
         * Returns whether the file metadata could not be read.
         *
         * \return \c true if the metadata could not be read, \c false
         * otherwise.
         */
        bool isInvalid() const;

        /**
         * Sets this to be a copy of the given file information.
         */
        FileInfo& operator = (const FileInfo&) = default;
        /**
         * Moves the contents of the given file information to this new object.
         *
         * The object that was passed will no longer be usable.
         *
         * \return a reference to this object.
         */
        FileInfo& operator = (FileInfo&&) noexcept = default;
        /**
         * Swaps the contents of this and the given file information.
         *
         * \param other the object whose contents should be swapped with this.
         */
        void swap(FileInfo& other) noexcept;

        /**
         * Determines whether this and the given file information
         * describe the same format and version.
         *
         * For two FileInfo objects to compare as equal, they must have
         * the same file formats, use the same version of the calculation
         * engine, and use the same compression type.  The pathnames of
         * the files being described are ignored.
         *
         * It is safe to compare FileInfo objects even if one or both is
         * invalid.  Two invalid FileInfo objects will compare as equal.
         *
         * \param other the file information to compare with this.
         * \return \c true if and only if this and the given file information
         * describe the same format and version, as described above.
         */
        bool operator == (const FileInfo& other) const;

        /**
         * Return information about the given Regina data file.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given path _name_,
         * and simply passes it through unchanged to low-level C/C++ file I/O
         * routines.  If a FileInfo structure is returned, its pathname()
         * routine will use the same encoding that is passed here.
         *
         * \param idPathname the pathname of the data file to be examined.
         * \return a FileInfo structure containing information about the
         * given file, or \nullopt if the file type could not be identified.
         */
        static std::optional<FileInfo> identify(std::string idPathname);

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \nopython Use detail() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Create a new uninitialised structure.
         */
        FileInfo() = default;
};

/**
 * Swaps the contents of the two given file information objects.
 *
 * This global routine simply calls FileInfo::swap(); it is provided
 * so that FileInfo meets the C++ Swappable requirements.
 *
 * \param a the object whose contents should be swapped with \a b.
 * \param b the object whose contents should be swapped with \a a.
 *
 * \ingroup file
 */
void swap(FileInfo& a, FileInfo& b) noexcept;

// Inline functions for FileInfo

inline const std::string& FileInfo::pathname() const {
    return pathname_;
}

inline FileFormat FileInfo::format() const {
    return format_;
}

inline std::string FileInfo::formatDescription() const {
    switch (format_) {
        case FileFormat::BinaryGen1:
            return "First-generation binary format (Regina 2.4 and earlier)";
        case FileFormat::XmlGen2:
            return "Second-generation XML format (Regina 3.0-6.0.1)";
        case FileFormat::XmlGen3:
            return "Third-generation XML format (Regina 7.0+)";
        default:
            return "Unknown file format";
    }
}

inline const std::string& FileInfo::engine() const {
    return engine_;
}

inline bool FileInfo::isCompressed() const {
    return compressed_;
}

inline bool FileInfo::isInvalid() const {
    return invalid_;
}

inline void FileInfo::swap(FileInfo& other) noexcept {
    pathname_.swap(other.pathname_);
    std::swap(format_, other.format_);
    engine_.swap(other.engine_);
    std::swap(compressed_, other.compressed_);
    std::swap(invalid_, other.invalid_);
}

inline bool FileInfo::operator == (const FileInfo& other) const {
    if (invalid_)
        return other.invalid_;
    else
        return (! other.invalid_) && format_ == other.format_ &&
            compressed_ == other.compressed_ && engine_ == other.engine_;
}

inline void swap(FileInfo& a, FileInfo& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

