
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nfileinfo.h
 *  \brief Deals with determining information about Regina data files.
 */

#ifndef __NFILEINFO_H
#ifndef __DOXYGEN
#define __NFILEINFO_H
#endif

#include "shareableobject.h"

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
 */
class NFileInfo : public ShareableObject {
    public:
        static const int TYPE_BINARY;
            /**< Represents an old-style binary data file. */
        static const int TYPE_XML;
            /**< Represents a new-style XML data file. */

    private:
        std::string pathname;
            /**< The pathname of the data file being described. */
        int type;
            /**< The type of data file; this will be one of the file type
                 constants defined in this class. */
        std::string typeDescription;
            /**< A human-readable description of the type of data file. */
        std::string engine;
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
         * @return the pathname.
         */
        const std::string& getPathname() const;
        /**
         * Returns the type of data file.  The type will be given as one
         * of the file type constants defined in this class.
         *
         * @return the type of data file.
         */
        int getType() const;
        /**
         * Returns a human-readable description of the type of data file.
         *
         * @return a description of the type of data file.
         */
        const std::string& getTypeDescription() const;
        /**
         * Returns the version of the calculation engine that wrote this file.
         *
         * @return the engine version for this file.
         */
        const std::string& getEngine() const;
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
         * \ifaces This routine is a member of class Engine and is
         * called Engine::identifyFileInfo().
         *
         * @param idPathname the pathname of the data file to be examined.
         * @return a newly created NFileInfo structure containing
         * information about the given file, or 0 if the file type could not
         * be identified.
         */
        static NFileInfo* identify(const std::string& idPathname);

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Create a new uninitialised structure.
         */
        NFileInfo();
};

/*@}*/

// Inline functions for NFileInfo

inline NFileInfo::NFileInfo() {
}

inline const std::string& NFileInfo::getPathname() const {
    return pathname;
}

inline int NFileInfo::getType() const {
    return type;
}

inline const std::string& NFileInfo::getTypeDescription() const {
    return typeDescription;
}

inline const std::string& NFileInfo::getEngine() const {
    return engine;
}

inline bool NFileInfo::isCompressed() const {
    return compressed;
}

inline bool NFileInfo::isInvalid() const {
    return invalid;
}

} // namespace regina

#endif

