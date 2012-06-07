
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
#include "dim2/nxmldim2trireader.h"
#include "utilities/stringutils.h"

namespace regina {

/**
 * A unique namespace containing various specific-task packet readers.
 */
namespace {
    /**
     * Reads a single triangle with its name and gluings.
     */
    class Dim2TriangleReader : public NXMLElementReader {
        private:
            Dim2Triangulation* tri_;
            Dim2Triangle* triangle_;

        public:
            Dim2TriangleReader(Dim2Triangulation* tri, unsigned whichTri) :
                    tri_(tri), triangle_(tri->getTriangles()[whichTri]) {
            }

            virtual void startElement(const std::string&,
                    const regina::xml::XMLPropertyDict& props,
                    NXMLElementReader*) {
                triangle_->setDescription(props.lookup("desc"));
            }

            virtual void initialChars(const std::string& chars) {
                std::vector<std::string> tokens;
                if (basicTokenise(back_inserter(tokens), chars) != 6)
                    return;

                long triIndex, permCode;
                NPerm3 perm;
                Dim2Triangle* adjTri;
                int adjEdge;
                for (int k = 0; k < 3; ++k) {
                    if (! valueOf(tokens[2 * k], triIndex))
                        continue;
                    if (! valueOf(tokens[2 * k + 1], permCode))
                        continue;

                    if (triIndex < 0 || triIndex >=
                            static_cast<int>(tri_->getNumberOfTriangles()))
                        continue;
                    if (! NPerm3::isPermCode(permCode))
                        continue;

                    perm.setPermCode(permCode);
                    adjTri = tri_->getTriangles()[triIndex];
                    adjEdge = perm[k];
                    if (adjTri == triangle_ && adjEdge == k)
                        continue;
                    if (triangle_->adjacentTriangle(k))
                        continue;
                    if (adjTri->adjacentTriangle(adjEdge))
                        continue;

                    triangle_->joinTo(k, adjTri, perm);
                }
            }
    };

    /**
     * Reads an entire set of triangles with their names and gluings.
     */
    class Dim2TrianglesReader : public NXMLElementReader {
        private:
            Dim2Triangulation* tri_;
            unsigned readTriangles_;

        public:
            Dim2TrianglesReader(Dim2Triangulation* tri) :
                    tri_(tri), readTriangles_(0) {
            }

            virtual void startElement(const std::string& /* tagName */,
                    const regina::xml::XMLPropertyDict& props,
                    NXMLElementReader*) {
                long nTriangles;
                if (valueOf(props.lookup("ntriangles"), nTriangles))
                    for ( ; nTriangles > 0; --nTriangles)
                        tri_->newTriangle();
            }

            virtual NXMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "triangle") {
                    if (readTriangles_ < tri_->getNumberOfTriangles())
                        return new Dim2TriangleReader(tri_, readTriangles_++);
                    else
                        return new NXMLElementReader();
                } else
                    return new NXMLElementReader();
            }
    };
}

NXMLElementReader* NXMLDim2TriangulationReader::startContentSubElement(
        const std::string& subTagName,
        const regina::xml::XMLPropertyDict&) {
    if (subTagName == "triangles")
        return new Dim2TrianglesReader(tri_);
    return new NXMLElementReader();
}

void NXMLDim2TriangulationReader::endContentSubElement(const std::string&,
        NXMLElementReader*) {
}

NXMLPacketReader* Dim2Triangulation::getXMLReader(NPacket*) {
    return new NXMLDim2TriangulationReader();
}

} // namespace regina

