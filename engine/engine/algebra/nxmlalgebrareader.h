
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

/*! \file nxmlalgebrareader.h
 *  \brief Deals with parsing XML data for various algebraic structures.
 */

#ifndef __NXMLALGEBRAREADER_H
#ifndef __DOXYGEN
#define __NXMLALGEBRAREADER_H
#endif

#include "algebra/nabeliangroup.h"
#include "algebra/ngrouppresentation.h"
#include "file/nxmlelementreader.h"

namespace regina {

/**
 * \weakgroup algebra
 * @{
 */

/**
 * An XML element reader that reads a single abelian group.
 * An abelian group is generally contained within an
 * <tt>\<abeliangroup\></tt> ... <tt>\</abeliangroup\></tt> pair.
 *
 * \ifacespython Not present.
 */
class NXMLAbelianGroupReader : public NXMLElementReader {
    private:
        NAbelianGroup* group;
            /**< The abelian group currently being read. */

    public:
        /**
         * Creates a new abelian group reader.
         */
        NXMLAbelianGroupReader();

        /**
         * Returns the newly allocated abelian group that has been read by
         * this element reader.
         *
         * @return the group that has been read, or 0 if an error occurred.
         */
        virtual NAbelianGroup* getGroup();

        virtual void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            NXMLElementReader* parentReader);
        virtual void initialChars(const std::string& chars);
};

/**
 * An XML element reader that reads a single group presentation.
 * A group presentation is generally contained within a
 * <tt>\<group\></tt> ... <tt>\</group\></tt> pair.
 *
 * \ifacespython Not present.
 */
class NXMLGroupPresentationReader : public NXMLElementReader {
    private:
        NGroupPresentation* group;
            /**< The group presentation currently being read. */

    public:
        /**
         * Creates a new group presentation reader.
         */
        NXMLGroupPresentationReader();

        /**
         * Returns the newly allocated group presentation that has been read by
         * this element reader.
         *
         * @return the group that has been read, or 0 if an error occurred.
         */
        virtual NGroupPresentation* getGroup();

        virtual void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            NXMLElementReader* parentReader);
        virtual NXMLElementReader* startSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        virtual void endSubElement(const std::string& subTagName,
            NXMLElementReader* subReader);
};

/*@}*/

// Inline functions for NXMLAbelianGroupReader

inline NXMLAbelianGroupReader::NXMLAbelianGroupReader() : group(0) {
}

inline NAbelianGroup* NXMLAbelianGroupReader::getGroup() {
    return group;
}

// Inline functions for NXMLGroupPresentationReader

inline NXMLGroupPresentationReader::NXMLGroupPresentationReader() : group(0) {
}

inline NGroupPresentation* NXMLGroupPresentationReader::getGroup() {
    return group;
}

} // namespace regina

#endif

