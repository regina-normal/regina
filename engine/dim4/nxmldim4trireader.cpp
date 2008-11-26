
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <vector>
#include "algebra/nxmlalgebrareader.h"
#include "dim4/nxmldim4trireader.h"
#include "utilities/stringutils.h"

namespace regina {

/**
 * A unique namespace containing various specific-task packet readers.
 */
namespace {
    /**
     * Reads a single pentachoron and its name and gluings.
     */
    class Dim4PentachoronReader : public NXMLElementReader {
        private:
            Dim4Triangulation* tri_;
            Dim4Pentachoron* pent_;

        public:
            Dim4PentachoronReader(Dim4Triangulation* tri, unsigned whichPent) :
                    tri_(tri), pent_(tri->getPentachora()[whichPent]) {
            }

            virtual void startElement(const std::string&,
                    const regina::xml::XMLPropertyDict& props,
                    NXMLElementReader*) {
                pent_->setDescription(props.lookup("desc"));
            }

            virtual void initialChars(const std::string& chars) {
                std::vector<std::string> tokens;
                if (basicTokenise(back_inserter(tokens), chars) != 10)
                    return;

                long pentIndex, permCode;
                NPerm5 perm;
                Dim4Pentachoron* adjPent;
                int adjFacet;
                for (int k = 0; k < 5; ++k) {
                    if (! valueOf(tokens[2 * k], pentIndex))
                        continue;
                    if (! valueOf(tokens[2 * k + 1], permCode))
                        continue;

                    if (pentIndex < 0 || pentIndex >=
                            static_cast<int>(tri_->getNumberOfPentachora()))
                        continue;
                    if (! NPerm5::isPermCode(permCode))
                        continue;

                    perm.setPermCode(permCode);
                    adjPent = tri_->getPentachora()[pentIndex];
                    adjFacet = perm[k];
                    if (adjPent == pent_ && adjFacet == k)
                        continue;
                    if (pent_->adjacentPentachoron(k))
                        continue;
                    if (adjPent->adjacentPentachoron(adjFacet))
                        continue;

                    pent_->joinTo(k, adjPent, perm);
                }
            }
    };

    /**
     * Reads an entire set of pentachora with their names and gluings.
     */
    class Dim4PentachoraReader : public NXMLElementReader {
        private:
            Dim4Triangulation* tri_;
            unsigned readPents_;

        public:
            Dim4PentachoraReader(Dim4Triangulation* tri) : tri_(tri),
                    readPents_(0) {
            }

            virtual void startElement(const std::string& /* tagName */,
                    const regina::xml::XMLPropertyDict& props,
                    NXMLElementReader*) {
                long nPents;
                if (valueOf(props.lookup("npent"), nPents))
                    for ( ; nPents > 0; --nPents)
                        tri_->addPentachoron(new Dim4Pentachoron());
            }

            virtual NXMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "pent") {
                    if (readPents_ < tri_->getNumberOfPentachora())
                        return new Dim4PentachoronReader(tri_, readPents_++);
                    else
                        return new NXMLElementReader();
                } else
                    return new NXMLElementReader();
            }
    };

    /**
     * Reads an abelian group property.
     */
    class NAbelianGroupPropertyReader : public NXMLElementReader {
        public:
            typedef NProperty<NAbelianGroup, StoreManagedPtr> PropType;

        private:
            PropType& prop_;

        public:
            NAbelianGroupPropertyReader(PropType& prop) : prop_(prop) {
            }

            virtual NXMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "abeliangroup")
                    if (! prop_.known())
                        return new NXMLAbelianGroupReader();
                return new NXMLElementReader();
            }

            virtual void endSubElement(const std::string& subTagName,
                    NXMLElementReader* subReader) {
                if (subTagName == "abeliangroup") {
                    NAbelianGroup* ans =
                        dynamic_cast<NXMLAbelianGroupReader*>(subReader)->
                        getGroup();
                    if (ans)
                        prop_ = ans;
                }
            }
    };

    /**
     * Reads a group presentation property.
     */
    class NGroupPresentationPropertyReader : public NXMLElementReader {
        public:
            typedef NProperty<NGroupPresentation, StoreManagedPtr> PropType;

        private:
            PropType& prop_;

        public:
            NGroupPresentationPropertyReader(PropType& prop) : prop_(prop) {
            }

            virtual NXMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "group")
                    if (! prop_.known())
                        return new NXMLGroupPresentationReader();
                return new NXMLElementReader();
            }

            virtual void endSubElement(const std::string& subTagName,
                    NXMLElementReader* subReader) {
                if (subTagName == "group") {
                    NGroupPresentation* ans =
                        dynamic_cast<NXMLGroupPresentationReader*>(subReader)->
                        getGroup();
                    if (ans)
                        prop_ = ans;
                }
            }
    };
}

NXMLElementReader* NXMLDim4TriangulationReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == "pentachora")
        return new Dim4PentachoraReader(tri_);
    else if (subTagName == "H1")
        return new NAbelianGroupPropertyReader(tri_->H1_);
    else if (subTagName == "H2")
        return new NAbelianGroupPropertyReader(tri_->H2_);
    else if (subTagName == "fundgroup")
        return new NGroupPresentationPropertyReader(tri_->fundGroup_);
    return new NXMLElementReader();
}

void NXMLDim4TriangulationReader::endContentSubElement(const std::string&,
        NXMLElementReader*) {
}

NXMLPacketReader* Dim4Triangulation::getXMLReader(NPacket*) {
    return new NXMLDim4TriangulationReader();
}

} // namespace regina

