
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

#include "algebra/nxmlalgebrareader.h"
#include "dim4/dim4triangulation.h"
#include "dim4/nxmldim4trireader.h"
#include "utilities/nproperty.h"

namespace regina {

/**
 * A unique namespace containing various specific-task packet readers.
 */
namespace {
    /**
     * Reads an abelian group property.
     */
    class NAbelianGroupPropertyReader : public XMLElementReader {
        public:
            typedef NProperty<NAbelianGroup, StoreManagedPtr> PropType;

        private:
            PropType& prop_;

        public:
            NAbelianGroupPropertyReader(PropType& prop) : prop_(prop) {
            }

            virtual XMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "abeliangroup")
                    if (! prop_.known())
                        return new NXMLAbelianGroupReader();
                return new XMLElementReader();
            }

            virtual void endSubElement(const std::string& subTagName,
                    XMLElementReader* subReader) {
                if (subTagName == "abeliangroup") {
                    NAbelianGroup* ans =
                        dynamic_cast<NXMLAbelianGroupReader*>(subReader)->
                        group();
                    if (ans)
                        prop_ = ans;
                }
            }
    };

    /**
     * Reads a group presentation property.
     */
    class NGroupPresentationPropertyReader : public XMLElementReader {
        public:
            typedef NProperty<NGroupPresentation, StoreManagedPtr> PropType;

        private:
            PropType& prop_;

        public:
            NGroupPresentationPropertyReader(PropType& prop) : prop_(prop) {
            }

            virtual XMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "group")
                    if (! prop_.known())
                        return new NXMLGroupPresentationReader();
                return new XMLElementReader();
            }

            virtual void endSubElement(const std::string& subTagName,
                    XMLElementReader* subReader) {
                if (subTagName == "group") {
                    NGroupPresentation* ans =
                        dynamic_cast<NXMLGroupPresentationReader*>(subReader)->
                        group();
                    if (ans)
                        prop_ = ans;
                }
            }
    };
}

XMLElementReader* XMLTriangulationReader<4>::startPropertySubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == "H1")
        return new NAbelianGroupPropertyReader(tri_->H1_);
    else if (subTagName == "H2")
        return new NAbelianGroupPropertyReader(tri_->H2_);
    else if (subTagName == "fundgroup")
        return new NGroupPresentationPropertyReader(tri_->fundGroup_);
    return new XMLElementReader();
}

XMLPacketReader* Triangulation<4>::xmlReader(Packet*,
        XMLTreeResolver& resolver) {
    return new NXMLDim4TriangulationReader(resolver);
}

} // namespace regina

