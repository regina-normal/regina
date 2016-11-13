
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

/*! \file algebra/xmlalgebrareader.h
 *  \brief Deals with parsing XML data for various algebraic structures.
 */

#ifndef __XMLALGEBRAREADER_H
#ifndef __DOXYGEN
#define __XMLALGEBRAREADER_H
#endif

#include "regina-core.h"
#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "utilities/xmlelementreader.h"

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
class REGINA_API XMLAbelianGroupReader : public XMLElementReader {
    private:
        AbelianGroup* group_;
            /**< The abelian group currently being read. */

    public:
        /**
         * Creates a new abelian group reader.
         */
        XMLAbelianGroupReader();

        /**
         * Returns the newly allocated abelian group that has been read by
         * this element reader.
         *
         * @return the group that has been read, or 0 if an error occurred.
         */
        virtual AbelianGroup* group();

        virtual void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            XMLElementReader* parentReader);
        virtual void initialChars(const std::string& chars);
};

/**
 * An XML element reader that reads a single group presentation.
 * A group presentation is generally contained within a
 * <tt>\<group\></tt> ... <tt>\</group\></tt> pair.
 *
 * \ifacespython Not present.
 */
class REGINA_API XMLGroupPresentationReader : public XMLElementReader {
    private:
        GroupPresentation* group_;
            /**< The group presentation currently being read. */

    public:
        /**
         * Creates a new group presentation reader.
         */
        XMLGroupPresentationReader();

        /**
         * Returns the newly allocated group presentation that has been read by
         * this element reader.
         *
         * @return the group that has been read, or 0 if an error occurred.
         */
        virtual GroupPresentation* group();

        virtual void startElement(const std::string& tagName,
            const regina::xml::XMLPropertyDict& tagProps,
            XMLElementReader* parentReader);
        virtual XMLElementReader* startSubElement(
            const std::string& subTagName,
            const regina::xml::XMLPropertyDict& subTagProps);
        virtual void endSubElement(const std::string& subTagName,
            XMLElementReader* subReader);
};

/*@}*/

// Inline functions for XMLAbelianGroupReader

inline XMLAbelianGroupReader::XMLAbelianGroupReader() : group_(0) {
}

inline AbelianGroup* XMLAbelianGroupReader::group() {
    return group_;
}

// Inline functions for XMLGroupPresentationReader

inline XMLGroupPresentationReader::XMLGroupPresentationReader() : group_(0) {
}

inline GroupPresentation* XMLGroupPresentationReader::group() {
    return group_;
}

} // namespace regina

#endif

