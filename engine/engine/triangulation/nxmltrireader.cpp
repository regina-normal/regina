
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

#include <vector>
#include "algebra/nxmlalgebrareader.h"
#include "triangulation/nxmltrireader.h"
#include "utilities/stringutils.h"

namespace regina {

/**
 * A unique namespace containing various specific-task packet readers.
 */
namespace {
    /**
     * Reads a single tetrahedron and its name and gluings.
     */
    class NTetrahedronReader : public NXMLElementReader {
        private:
            NTriangulation* tri;
            NTetrahedron* tet;

        public:
            NTetrahedronReader(NTriangulation* newTri, unsigned whichTet) :
                    tri(newTri), tet(tri->getTetrahedra()[whichTet]) {
            }

            virtual void startElement(const std::string&,
                    const regina::xml::XMLPropertyDict& props,
                    NXMLElementReader*) {
                tet->setDescription(props.lookup("desc"));
            }

            virtual void initialChars(const std::string& chars) {
                std::vector<std::string> tokens;
                if (basicTokenise(back_inserter(tokens), chars) != 8)
                    return;

                long tetIndex, permCode;
                NPerm perm;
                NTetrahedron* adjTet;
                int adjFace;
                for (int k = 0; k < 4; k ++) {
                    if (! valueOf(tokens[2 * k], tetIndex))
                        continue;
                    if (! valueOf(tokens[2 * k + 1], permCode))
                        continue;

                    if (tetIndex < 0 ||
                            tetIndex >= (int)tri->getNumberOfTetrahedra())
                        continue;
                    if (! NPerm::isPermCode((unsigned char)permCode))
                        continue;

                    perm.setPermCode((unsigned char)permCode);
                    adjTet = tri->getTetrahedra()[tetIndex];
                    adjFace = perm[k];
                    if (adjTet == tet && adjFace == k)
                        continue;
                    if (tet->getAdjacentTetrahedron(k))
                        continue;
                    if (adjTet->getAdjacentTetrahedron(adjFace))
                        continue;

                    tet->joinTo(k, adjTet, perm);
                }
            }
    };

    /**
     * Reads an entire set of tetrahedra with their names and gluings.
     */
    class NTetrahedraReader : public NXMLElementReader {
        private:
            NTriangulation* tri;
            unsigned readTets;

        public:
            NTetrahedraReader(NTriangulation* newTri) : tri(newTri),
                    readTets(0) {
            }

            virtual void startElement(const std::string& tagName,
                    const regina::xml::XMLPropertyDict& props,
                    NXMLElementReader*) {
                long nTets;
                if (valueOf(props.lookup("ntet"), nTets))
                    for ( ; nTets > 0; nTets--)
                        tri->addTetrahedron(new NTetrahedron());
            }

            virtual NXMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "tet") {
                    if (readTets < tri->getNumberOfTetrahedra())
                        return new NTetrahedronReader(tri, readTets++);
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
        private:
            NAbelianGroup** groupProp;
            bool* calculatedProp;

        public:
            NAbelianGroupPropertyReader(NAbelianGroup** newProp,
                    bool* newCalc) : groupProp(newProp),
                    calculatedProp(newCalc) {
            }

            virtual NXMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "abeliangroup")
                    if (! (*calculatedProp))
                        return new NXMLAbelianGroupReader();
                return new NXMLElementReader();
            }

            virtual void endSubElement(const std::string& subTagName,
                    NXMLElementReader* subReader) {
                if (subTagName == "abeliangroup") {
                    *groupProp = ((NXMLAbelianGroupReader*)subReader)->
                        getGroup();
                    if (*groupProp)
                        *calculatedProp = true;
                }
            }
    };

    /**
     * Reads a group presentation property.
     */
    class NGroupPresentationPropertyReader : public NXMLElementReader {
        private:
            NGroupPresentation** groupProp;
            bool* calculatedProp;

        public:
            NGroupPresentationPropertyReader(NGroupPresentation** newProp,
                    bool* newCalc) : groupProp(newProp),
                    calculatedProp(newCalc) {
            }

            virtual NXMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "group")
                    if (! (*calculatedProp))
                        return new NXMLGroupPresentationReader();
                return new NXMLElementReader();
            }

            virtual void endSubElement(const std::string& subTagName,
                    NXMLElementReader* subReader) {
                if (subTagName == "group") {
                    *groupProp = ((NXMLGroupPresentationReader*)subReader)->
                        getGroup();
                    if (*groupProp)
                        *calculatedProp = true;
                }
            }
    };
}

NXMLElementReader* NXMLTriangulationReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    if (subTagName == "tetrahedra")
        return new NTetrahedraReader(tri);
    else if (subTagName == "zeroeff") {
        if (valueOf(props.lookup("value"), tri->zeroEfficient))
            tri->calculatedZeroEfficient = true;
    } else if (subTagName == "splitsfce") {
        if (valueOf(props.lookup("value"), tri->splittingSurface))
            tri->calculatedSplittingSurface = true;
    } else if (subTagName == "H1")
        return new NAbelianGroupPropertyReader(&tri->H1, &tri->calculatedH1);
    else if (subTagName == "H1Rel")
        return new NAbelianGroupPropertyReader(&tri->H1Rel,
            &tri->calculatedH1Rel);
    else if (subTagName == "H1Bdry")
        return new NAbelianGroupPropertyReader(&tri->H1Bdry,
            &tri->calculatedH1Bdry);
    else if (subTagName == "H2")
        return new NAbelianGroupPropertyReader(&tri->H2, &tri->calculatedH2);
    else if (subTagName == "fundgroup")
        return new NGroupPresentationPropertyReader(&tri->fundamentalGroup,
            &tri->calculatedFundamentalGroup);
    return new NXMLElementReader();
}

void NXMLTriangulationReader::endContentSubElement(const std::string&,
        NXMLElementReader*) {
}

NXMLPacketReader* NTriangulation::getXMLReader(NPacket*) {
    return new NXMLTriangulationReader();
}

} // namespace regina

