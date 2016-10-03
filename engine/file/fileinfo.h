
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

/*! \file file/fileinfo.h
 *  \brief Deals with determining information about Regina data files.
 */

#ifndef __FILEINFO_H
#ifndef __DOXYGEN
#define __FILEINFO_H
#endif

#include "regina-core.h"
#include "output.h"
#include <boost/noncopyable.hpp>

namespace regina {

/**
 * \weakgroup file
 * @{
 */

/**
 * Stores information about a Regina data file, including file type and
 * version.
 *
 * Routine identify() can be used to determine this information for a
 * given file.
 *
 * As of Regina 4.94, the old-style binary files are no longer supported.
 * These have not been in use for over a decade.  The only file type
 * that this class now recognises is TYPE_XML (compressed or uncompressed
 * XML data files).
 */
class REGINA_API FileInfo :
        public Output<FileInfo>,
        public boost::noncopyable {
    public:
        static const int TYPE_XML;
            /**< Represents a new-style XML data file. */

    private:
        std::string pathname_;
            /**< The pathname of the data file being described. */
        int type_;
            /**< The type of data file; this will be one of the file type
                 constants defined in this class. */
        std::string typeDescription_;
            /**< A human-readable description of the type of data file. */
        std::string engine_;
            /**< The version of the calculation engine that wrote this file. */
        bool compressed;
            /**< \c true if this file is stored in compressed format,
                 \c false otherwise.  Currently this option only applies
                 to XML data files. */
        bool invalid;
            /**< \c true if the file metadata could not be read,
                 \c false otherwise. */

    public:
        /**
         * Returns the pathname of the data file being described.
         *
         * \i18n The \ref i18n "character encoding" used in the pathname will
         * be whatever was originally passed to identify().  This might or
         * might not be UTF-8, since it needs to be understood by the
         * low-level C/C++ file I/O routines.
         *
         * @return the pathname.
         */
        const std::string& pathname() const;
        /**
         * Returns the type of data file.  The type will be given as one
         * of the file type constants defined in this class.
         *
         * @return the type of data file.
         */
        int type() const;
        /**
         * Returns a human-readable description of the type of data file.
         *
         * @return a description of the type of data file.
         */
        const std::string& typeDescription() const;
        /**
         * Returns the version of the calculation engine that wrote this file.
         *
         * @return the engine version for this file.
         */
        const std::string& engine() const;
        /**
         * Returns whether this file is stored in compressed format.
         * Currently this option only applies to XML data files.
         *
         * @return \c true if this file is compressed or \c false otherwise.
         */
        bool isCompressed() const;
        /**
         * Returns whether the file metadata could not be read.
         *
         * @return \c true if the metadata could not be read, \c false
         * otherwise.
         */
        bool isInvalid() const;

        /**
         * Return information about the given Regina data file.
         *
         * \i18n This routine makes no assumptions about the
         * \ref i18n "character encoding" used in the given path \e name,
         * and simply passes it through unchanged to low-level C/C++ file I/O
         * routines.  If a FileInfo structure is returned, its pathname()
         * routine will use the same encoding that is passed here.
         *
         * @param idPathname the pathname of the data file to be examined.
         * @return a newly created FileInfo structure containing
         * information about the given file, or 0 if the file type could not
         * be identified.
         */
        static FileInfo* identify(const std::string& idPathname);

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Create a new uninitialised structure.
         */
        FileInfo();
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NFileInfo has now been renamed to FileInfo.
 */
REGINA_DEPRECATED typedef FileInfo NFileInfo;

/*@}*/

// Inline functions for FileInfo

inline FileInfo::FileInfo() {
}

inline const std::string& FileInfo::pathname() const {
    return pathname_;
}

inline int FileInfo::type() const {
    return type_;
}

inline const std::string& FileInfo::typeDescription() const {
    return typeDescription_;
}

inline const std::string& FileInfo::engine() const {
    return engine_;
}

inline bool FileInfo::isCompressed() const {
    return compressed;
}

inline bool FileInfo::isInvalid() const {
    return invalid;
}

} // namespace regina

#endif

