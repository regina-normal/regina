
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
     * Reads a single face with its name and gluings.
     */
    class Dim2FaceReader : public NXMLElementReader {
        private:
            Dim2Triangulation* tri_;
            Dim2Face* face_;

        public:
            Dim2FaceReader(Dim2Triangulation* tri, unsigned whichFace) :
                    tri_(tri), face_(tri->getFaces()[whichFace]) {
            }

            virtual void startElement(const std::string&,
                    const regina::xml::XMLPropertyDict& props,
                    NXMLElementReader*) {
                face_->setDescription(props.lookup("desc"));
            }

            virtual void initialChars(const std::string& chars) {
                std::vector<std::string> tokens;
                if (basicTokenise(back_inserter(tokens), chars) != 6)
                    return;

                long faceIndex, permCode;
                NPerm3 perm;
                Dim2Face* adjFace;
                int adjEdge;
                for (int k = 0; k < 3; ++k) {
                    if (! valueOf(tokens[2 * k], faceIndex))
                        continue;
                    if (! valueOf(tokens[2 * k + 1], permCode))
                        continue;

                    if (faceIndex < 0 || faceIndex >=
                            static_cast<int>(tri_->getNumberOfFaces()))
                        continue;
                    if (! NPerm3::isPermCode(permCode))
                        continue;

                    perm.setPermCode(permCode);
                    adjFace = tri_->getFaces()[faceIndex];
                    adjEdge = perm[k];
                    if (adjFace == face_ && adjEdge == k)
                        continue;
                    if (face_->adjacentFace(k))
                        continue;
                    if (adjFace->adjacentFace(adjEdge))
                        continue;

                    face_->joinTo(k, adjFace, perm);
                }
            }
    };

    /**
     * Reads an entire set of faces with their names and gluings.
     */
    class Dim2FacesReader : public NXMLElementReader {
        private:
            Dim2Triangulation* tri_;
            unsigned readFaces_;

        public:
            Dim2FacesReader(Dim2Triangulation* tri) : tri_(tri), readFaces_(0) {
            }

            virtual void startElement(const std::string& /* tagName */,
                    const regina::xml::XMLPropertyDict& props,
                    NXMLElementReader*) {
                long nFaces;
                if (valueOf(props.lookup("nfaces"), nFaces))
                    for ( ; nFaces > 0; --nFaces)
                        tri_->newFace();
            }

            virtual NXMLElementReader* startSubElement(
                    const std::string& subTagName,
                    const regina::xml::XMLPropertyDict&) {
                if (subTagName == "face") {
                    if (readFaces_ < tri_->getNumberOfFaces())
                        return new Dim2FaceReader(tri_, readFaces_++);
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
    if (subTagName == "faces")
        return new Dim2FacesReader(tri_);
    return new NXMLElementReader();
}

void NXMLDim2TriangulationReader::endContentSubElement(const std::string&,
        NXMLElementReader*) {
}

NXMLPacketReader* Dim2Triangulation::getXMLReader(NPacket*) {
    return new NXMLDim2TriangulationReader();
}

} // namespace regina

