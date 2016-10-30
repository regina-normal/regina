
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

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "utilities/xmlutils.h"

namespace regina {

Triangulation<4>::Triangulation(const std::string& description) :
        knownSimpleLinks_(false) {
    Triangulation<4>* attempt;

    if ((attempt = fromIsoSig(description))) {
        cloneFrom(*attempt);
        setLabel(description);
    }

    delete attempt;
}

void Triangulation<4>::writeTextLong(std::ostream& out) const {
    ensureSkeleton();

    out << "Size of the skeleton:\n";
    out << "  Pentachora: " << size() << '\n';
    out << "  Tetrahedra: " << countTetrahedra() << '\n';
    out << "  Triangles: " << countTriangles() << '\n';
    out << "  Edges: " << countEdges() << '\n';
    out << "  Vertices: " << countVertices() << '\n';
    out << '\n';

    Pentachoron<4>* pent;
    Pentachoron<4>* adjPent;
    unsigned pentPos;
    int i, j, k;
    Perm<5> adjPerm;

    out << "Pentachoron gluing:\n";
    out << "  Pent  |  glued to:     (0123)     (0124)     (0134)     (0234)     (1234)\n";
    out << "  ------+------------------------------------------------------------------\n";
    for (pentPos=0; pentPos < simplices_.size(); pentPos++) {
        pent = simplices_[pentPos];
        out << "  " << std::setw(4) << pentPos << "  |           ";
        for (i = 4; i >= 0; --i) {
            out << " ";
            adjPent = pent->adjacentPentachoron(i);
            if (! adjPent)
                out << "  boundary";
            else {
                adjPerm = pent->adjacentGluing(i);
                out << std::setw(3) << adjPent->index() << " (";
                for (j = 0; j < 5; ++j) {
                    if (j == i) continue;
                    out << adjPerm[j];
                }
                out << ")";
            }
        }
        out << '\n';
    }
    out << '\n';

    out << "Vertices:\n";
    out << "  Pent  |  vertex:    0   1   2   3   4\n";
    out << "  ------+------------------------------\n";
    for (pentPos = 0; pentPos < simplices_.size(); ++pentPos) {
        pent = simplices_[pentPos];
        out << "  " << std::setw(4) << pentPos << "  |          ";
        for (i = 0; i < 5; ++i)
            out << ' ' << std::setw(3) << pent->vertex(i)->index();
        out << '\n';
    }
    out << '\n';

    out << "Edges:\n";
    out << "  Pent  |  edge:   01  02  03  04  12  13  14  23  24  34\n";
    out << "  ------+------------------------------------------------\n";
    for (pentPos = 0; pentPos < simplices_.size(); ++pentPos) {
        pent = simplices_[pentPos];
        out << "  " << std::setw(4) << pentPos << "  |        ";
        for (i = 0; i < 5; ++i)
            for (j = i + 1; j < 5; ++j)
                out << ' ' << std::setw(3)
                    << pent->edge(Edge<4>::edgeNumber[i][j])->index();
        out << '\n';
    }
    out << '\n';

    out << "Triangles:\n";
    out << "  Pent  |  triangle:  012 013 014 023 024 034 123 124 134 234\n";
    out << "  ------+----------------------------------------------------\n";
    for (pentPos = 0; pentPos < simplices_.size(); ++pentPos) {
        pent = simplices_[pentPos];
        out << "  " << std::setw(4) << pentPos << "  |            ";
        for (i = 0; i < 5; ++i)
            for (j = i + 1; j < 5; ++j)
                for (k = j + 1; k < 5; ++k)
                    out << ' ' << std::setw(3)
                        << pent->triangle(
                            Triangle<4>::triangleNumber[i][j][k])->index();
        out << '\n';
    }
    out << '\n';

    out << "Tetrahedra:\n";
    out << "  Pent  |  facet:  0123 0124 0134 0234 1234\n";
    out << "  ------+----------------------------------\n";
    for (pentPos = 0; pentPos < simplices_.size(); ++pentPos) {
        pent = simplices_[pentPos];
        out << "  " << std::setw(4) << pentPos << "  |         ";
        for (i = 4; i >= 0; --i)
            out << ' ' << std::setw(4) << pent->tetrahedron(i)->index();
        out << '\n';
    }
    out << '\n';
}

long Triangulation<4>::eulerCharManifold() const {
    // Begin with V - E + F - T + P.
    // This call to eulerCharTri() also ensures that the skeleton has
    // been calculated.
    long ans = eulerCharTri();

    // Truncate any ideal vertices.
    if (ideal_) {
        for (auto bc : boundaryComponents_)
            if (bc->isIdeal()) {
                // Because our 4-manifold triangulation is valid, all
                // vertex links in the 3-manifold boundary must be
                // spheres or discs.  We can therefore use V - E + F - T
                // on this boundary component.
                ans += bc->vertices_.front()->link_->eulerCharTri() - 1;
            }
    }

    return ans;
}

void Triangulation<4>::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the pentachoron gluings.
    PentachoronIterator it;
    Pentachoron<4>* adjPent;
    int facet;

    out << "  <pentachora npent=\"" << simplices_.size() << "\">\n";
    for (it = simplices_.begin(); it != simplices_.end(); ++it) {
        out << "    <pent desc=\"" <<
            xmlEncodeSpecialChars((*it)->description()) << "\"> ";
        for (facet = 0; facet < 5; ++facet) {
            adjPent = (*it)->adjacentPentachoron(facet);
            if (adjPent) {
                out << adjPent->index() << ' '
                    << (*it)->adjacentGluing(facet).permCode() << ' ';
            } else
                out << "-1 -1 ";
        }
        out << "</pent>\n";
    }
    out << "  </pentachora>\n";

    if (fundGroup_.known()) {
        out << "  <fundgroup>\n";
        fundGroup_.value()->writeXMLData(out);
        out << "  </fundgroup>\n";
    }
    if (H1_.known()) {
        out << "  <H1>";
        H1_.value()->writeXMLData(out);
        out << "</H1>\n";
    }
    if (H2_.known()) {
        out << "  <H2>";
        H2_.value()->writeXMLData(out);
        out << "</H2>\n";
    }
}

void Triangulation<4>::cloneFrom(const Triangulation<4>& X) {
    ChangeEventSpan span(this);

    removeAllPentachora();

    PentachoronIterator it;
    for (it = X.simplices_.begin(); it != X.simplices_.end(); ++it)
        newPentachoron((*it)->description());

    // Make the gluings.
    long pentPos, adjPos;
    Pentachoron<4>* pent;
    Pentachoron<4>* adjPent;
    Perm<5> adjPerm;
    int facet;
    pentPos = 0;
    for (it = X.simplices_.begin(); it != X.simplices_.end(); ++it) {
        pent = *it;
        for (facet = 0; facet < 5; ++facet) {
            adjPent = pent->adjacentPentachoron(facet);
            if (adjPent) {
                adjPos = adjPent->index();
                adjPerm = pent->adjacentGluing(facet);
                if (adjPos > pentPos ||
                        (adjPos == pentPos && adjPerm[facet] > facet)) {
                    simplices_[pentPos]->join(facet,
                        simplices_[adjPos], adjPerm);
                }
            }
        }
        ++pentPos;
    }

    // Properties:
    if (X.knownSimpleLinks_)
        knownSimpleLinks_ = true;
    if (X.fundGroup_.known())
        fundGroup_ = new NGroupPresentation(*(X.fundGroup_.value()));
    if (X.H1_.known())
        H1_ = new AbelianGroup(*(X.H1_.value()));
    if (X.H2_.known())
        H2_ = new AbelianGroup(*(X.H2_.value()));
}

void Triangulation<4>::deleteSkeleton() {
    for (auto b : boundaryComponents_)
        delete b;
    boundaryComponents_.clear();

    TriangulationBase<4>::deleteSkeleton();
}

void Triangulation<4>::clearAllProperties() {
    if (calculatedSkeleton())
        deleteSkeleton();

    knownSimpleLinks_ = false;
    fundGroup_.clear();
    H1_.clear();
    H2_.clear();
}

} // namespace regina
