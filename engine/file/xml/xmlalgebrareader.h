
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file algebra/xmlalgebrareader.h
 *  \brief Deals with parsing XML data for various algebraic structures.
 */

#ifndef __REGINA_XMLALGEBRAREADER_H
#ifndef __DOXYGEN
#define __REGINA_XMLALGEBRAREADER_H
#endif

#include <optional>
#include "regina-core.h"
#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "file/xml/xmlelementreader.h"

namespace regina {

/**
 * An XML element reader that reads a single abelian group.
 * An abelian group is generally contained within an
 * <tt>\<abeliangroup\></tt> ... <tt>\</abeliangroup\></tt> pair.
 *
 * \nopython
 */
class XMLAbelianGroupReader : public XMLElementReader {
    private:
        std::optional<AbelianGroup> group_;
            /**< The abelian group currently being read. */

    public:
        /**
         * Creates a new abelian group reader.
         */
        XMLAbelianGroupReader() = default;

        /**
         * Returns a reference to the abelian group that has been read by
         * this element reader.
         *
         * @return the group that has been read, or no value if an error
         * occurred.
         */
        std::optional<AbelianGroup>& group();

        void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            XMLElementReader* parentReader) override;
        void initialChars(const std::string& chars) override;
};

/**
 * An XML element reader that reads a single group presentation.
 * A group presentation is generally contained within a
 * <tt>\<group\></tt> ... <tt>\</group\></tt> pair.
 *
 * \nopython
 */
class XMLGroupPresentationReader : public XMLElementReader {
    private:
        std::optional<GroupPresentation> group_;
            /**< The group presentation currently being read. */

    public:
        /**
         * Creates a new group presentation reader.
         */
        XMLGroupPresentationReader() = default;

        /**
         * Returns a reference to the group presentation that has been read by
         * this element reader.
         *
         * @return the group that has been read, or no value if an error
         * occurred.
         */
        std::optional<GroupPresentation>& group();

        void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            XMLElementReader* parentReader) override;
        XMLElementReader* startSubElement(const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps) override;
        void endSubElement(const std::string& subTagName,
            XMLElementReader* subReader) override;
};

// Inline functions for XMLAbelianGroupReader

inline std::optional<AbelianGroup>& XMLAbelianGroupReader::group() {
    return group_;
}

// Inline functions for XMLGroupPresentationReader

inline std::optional<GroupPresentation>& XMLGroupPresentationReader::group() {
    return group_;
}

} // namespace regina

#endif

