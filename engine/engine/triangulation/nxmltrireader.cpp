
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

#include <cstdlib>
#include <vector>
#include "packet/nxmlpacketreader.h"
#include "triangulation/ntriangulation.h"
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
                const std::string& name = props.lookup("desc", "");
                if (! name.empty())
                    tet->setDescription(name);
            }

            virtual void initialChars(const std::string& chars) {
                std::vector<std::string> tokens;
                if (basicTokenise(back_inserter(tokens), chars) != 8)
                    return;

                int tetIndex;
                char permCode;
                NPerm perm;
                NTetrahedron* adjTet;
                int adjFace;
                for (int k = 0; k < 4; k ++) {
                    tetIndex = atoi(tokens[2 * k].c_str());
                    permCode = (char)atoi(tokens[2 * k + 1].c_str());

                    if (tetIndex < 0 ||
                            tetIndex >= (int)tri->getNumberOfTetrahedra())
                        continue;
                    if (! NPerm::isPermCode(permCode))
                        continue;

                    perm.setPermCode(permCode);
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
                for (int nTets = atoi(props.lookup("ntet", "").c_str());
                        nTets > 0; nTets--)
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
     * Reads an entire triangulation.
     */
    class NTriangulationReader : public NXMLPacketReader {
        private:
            NTriangulation* tri;

        public:
            NTriangulationReader() : tri(new NTriangulation()) {
            }

            virtual NXMLElementReader* startContentSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "tetrahedra")
                    return new NTetrahedraReader(tri);
                else
                    return new NXMLElementReader();
                // TODO: read triangulation properties.
            }

            virtual NPacket* getPacket() {
                return tri;
            }
    };
}

NXMLPacketReader* NTriangulation::getXMLReader(NPacket*) {
    return new NTriangulationReader();
}

} // namespace regina

