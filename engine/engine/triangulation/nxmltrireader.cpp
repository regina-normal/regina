
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

                    if (tetIndex < 0 || tetIndex >=
                            static_cast<int>(tri->getNumberOfTetrahedra()))
                        continue;
                    if (! NPerm::isPermCode(
                            static_cast<unsigned char>(permCode)))
                        continue;

                    perm.setPermCode(static_cast<unsigned char>(permCode));
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

            virtual void startElement(const std::string& /* tagName */,
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
        public:
            typedef NProperty<NAbelianGroup, StoreManagedPtr> PropType;

        private:
            PropType& prop;

        public:
            NAbelianGroupPropertyReader(PropType& newProp) : prop(newProp) {
            }

            virtual NXMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "abeliangroup")
                    if (! prop.known())
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
                        prop = ans;
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
            PropType& prop;

        public:
            NGroupPresentationPropertyReader(PropType& newProp) :
                    prop(newProp) {
            }

            virtual NXMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "group")
                    if (! prop.known())
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
                        prop = ans;
                }
            }
    };
}

NXMLElementReader* NXMLTriangulationReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict& props) {
    // We don't read boundary component properties since they're stored
    // across multiple property tags and they're easy to calculate
    // anyway.
    if (subTagName == "tetrahedra")
        return new NTetrahedraReader(tri);
    else if (subTagName == "zeroeff") {
        bool b;
        if (valueOf(props.lookup("value"), b))
            tri->zeroEfficient = b;
    } else if (subTagName == "splitsfce") {
        bool b;
        if (valueOf(props.lookup("value"), b))
            tri->splittingSurface = b;
    } else if (subTagName == "threesphere") {
        bool b;
        if (valueOf(props.lookup("value"), b))
            tri->threeSphere = b;
    } else if (subTagName == "H1")
        return new NAbelianGroupPropertyReader(tri->H1);
    else if (subTagName == "H1Rel")
        return new NAbelianGroupPropertyReader(tri->H1Rel);
    else if (subTagName == "H1Bdry")
        return new NAbelianGroupPropertyReader(tri->H1Bdry);
    else if (subTagName == "H2")
        return new NAbelianGroupPropertyReader(tri->H2);
    else if (subTagName == "fundgroup")
        return new NGroupPresentationPropertyReader(tri->fundamentalGroup);
    else if (subTagName == "turaevviro") {
        unsigned long r, root;
        double value;
        if (valueOf(props.lookup("r"), r) &&
                valueOf(props.lookup("root"), root) &&
                valueOf(props.lookup("value"), value))
            tri->turaevViroCache[std::make_pair(r, root)] = value;
    }
    return new NXMLElementReader();
}

void NXMLTriangulationReader::endContentSubElement(const std::string&,
        NXMLElementReader*) {
}

NXMLPacketReader* NTriangulation::getXMLReader(NPacket*) {
    return new NXMLTriangulationReader();
}

} // namespace regina

