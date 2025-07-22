
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <queue>
#include "maths/rational.h"
#include "triangulation/dim3.h"

#include <iostream>

namespace regina {

void Triangulation<3>::calculateSkeleton() {
    TriangulationBase<3>::calculateSkeleton();

    ideal_ = false;
    standard_ = true;

#if 0
    checkPermutations();
        // Sets valid to false if gluings are mismatched (which should
        // never happen if the Tetrahedron<3> gluing routines have been
        // used correctly)
#endif

    calculateVertexLinks();
        // Sets valid, ideal, Vertex<3>.link,
        //     Vertex<3>.linkEulerChar, Component<3>.ideal,
        //     boundaryComponents, Vertex<3>.boundaryComponent

    // Flesh out the details of each component.
    for (auto v : vertices())
        v->component()->vertices_.push_back(v);
    for (auto e : edges())
        e->component()->edges_.push_back(e);
    for (auto t : triangles())
        t->component()->triangles_.push_back(t);
}

void Triangulation<3>::checkPermutations() {
    for (Tetrahedron<3>* tet : simplices_)
        for (int face = 0; face < 4; face++) {
            Tetrahedron<3> * adjacent = tet->adjacentTetrahedron(face);

            if (adjacent) {
                Perm<4> perm = tet->adjacentGluing(face);

                Perm<4> adj_perm = adjacent->adjacentGluing(perm[face]);

                if (!(perm*adj_perm).isIdentity()) {
                    valid_ = false;

                    // This printing statement is temporary code 
                    // to be removed once enough people have tested it
                    std::cerr << "ERROR: Permutations of adjacent faces "
                                 "do not match in skeleton.cpp" << std::endl;
                }

                if (tet != adjacent->adjacentTetrahedron(perm[face])) {
                    valid_ = false;

                    // This printing statement is temporary code 
                    // to be removed once enough people have tested it
                    std::cerr << "ERROR: Adjacency relations do not match"
                                 " in skeleton.cpp" << std::endl;
                }
            }
        }
}

void Triangulation<3>::calculateVertexLinks() {
    // Begin by calculating Euler characteristics.
    // Here we use the formula:  chi = (2 v_int + v_bdry - f) / 2, which
    // is easily proven with a little arithmetic.

    // Note that Vertex<3>::linkEulerChar is initialised to 0 in
    // the Vertex<3> constructor.

    // Begin by calculating (2 v_int + v_bdry) for each vertex link.
    Vertex<3>* end0;
    Vertex<3>* end1;
    Tetrahedron<3>* tet;
    for (Edge<3>* e : edges()) {
        // Try to compute e->vertex(0) and e->vertex(1), but
        // without calling e->vertex() which will recursively try to
        // recompute the skeleton.
        const EdgeEmbedding<3>& emb = e->front();
        tet = emb.tetrahedron();

        end0 = std::get<0>(tet->faces_)[
            std::get<1>(tet->mappings_)[emb.edge()][0]];
        end1 = std::get<0>(tet->faces_)[
            std::get<1>(tet->mappings_)[emb.edge()][1]];

        if (e->isBoundary()) {
            // Contribute to v_bdry.
            end0->linkEulerChar_++;
            if (e->isValid())
                end1->linkEulerChar_++;
        } else {
            // Contribute to 2 v_int.
            end0->linkEulerChar_ += 2;
            if (e->isValid())
                end1->linkEulerChar_ += 2;
        }
    }

    // Run through each vertex and finalise Euler characteristic, link
    // and more.

    for (Vertex<3>* vertex : vertices()) {
        // Fix the Euler characteristic (subtract f, divide by two).
        vertex->linkEulerChar_ = (vertex->linkEulerChar_
            - static_cast<long>(vertex->degree())) / 2;

        if (vertex->isBoundary()) {
            // We haven't added ideal vertices to the boundary list yet,
            // so this must be real boundary.
            if (vertex->linkEulerChar_ == 1)
                vertex->link_ = Vertex<3>::Link::Disc;
            else {
                vertex->link_ = Vertex<3>::Link::Invalid;
                vertex->whyInvalid_.value |= Vertex<3>::INVALID_LINK;
                valid_ = vertex->component_->valid_ = false;
                standard_ = false;
            }
        } else {
            if (vertex->linkEulerChar_ == 2)
                vertex->link_ = Vertex<3>::Link::Sphere;
            else {
                if (vertex->linkEulerChar_ == 0)
                    vertex->link_ = (vertex->isLinkOrientable() ?
                        Vertex<3>::Link::Torus : Vertex<3>::Link::KleinBottle);
                else {
                    vertex->link_ = Vertex<3>::Link::NonStandardCusp;
                    standard_ = false;
                }

                ideal_ = true;
                vertex->component()->ideal_ = true;

                auto* bc = new BoundaryComponent<3>();
                bc->push_back(vertex);
                bc->orientable_ = vertex->isLinkOrientable();
                vertex->boundaryComponent_ = bc;
                ++nBoundaryFaces_[0];
                boundaryComponents_.push_back(bc);
                vertex->component()->boundaryComponents_.push_back(bc);
            }
        }
    }
}

void Triangulation<3>::calculateBoundaryProperties() const {
    // Make sure the skeleton has been calculated!
    ensureSkeleton();

    bool localTwoSphereBoundaryComponents = false;
    bool localNegativeIdealBoundaryComponents = false;

    for (BoundaryComponent<3>* bc : boundaryComponents_) {
        if (bc->eulerChar() == 2)
            localTwoSphereBoundaryComponents = true;
        else if (bc->isIdeal() && bc->eulerChar() < 0)
            localNegativeIdealBoundaryComponents = true;

        // Stop the search if we've found everything we're looking for.
        if (localTwoSphereBoundaryComponents &&
                localNegativeIdealBoundaryComponents)
            break;
    }

    prop_.twoSphereBoundaryComponents_ = localTwoSphereBoundaryComponents;
    prop_.negativeIdealBoundaryComponents_ =
        localNegativeIdealBoundaryComponents;
}

void Triangulation<3>::cloneSkeleton(const Triangulation& src) {
    TriangulationBase<3>::cloneSkeleton(src);

    ideal_ = src.ideal_;
    standard_ = src.standard_;

    {
        auto me = vertices().begin();
        auto you = src.vertices().begin();
        for ( ; me != vertices().end(); ++me, ++you) {
            (*me)->link_ = (*you)->link_;
            (*me)->linkEulerChar_ = (*you)->linkEulerChar_;
            // Leave linkTri_ as built-on-demand for now.
        }
    }

    {
        auto me = triangles().begin();
        auto you = src.triangles().begin();
        for ( ; me != triangles().end(); ++me, ++you) {
            (*me)->type_ = (*you)->type_;
            (*me)->subtype_ = (*you)->subtype_;
        }
    }

    {
        auto me = components_.begin();
        auto you = src.components_.begin();
        for ( ; me != components_.end(); ++me, ++you) {
            (*me)->ideal_ = (*you)->ideal_;

            for (auto f : (*you)->vertices_)
                (*me)->vertices_.push_back(vertex(f->index()));
            for (auto f : (*you)->edges_)
                (*me)->edges_.push_back(edge(f->index()));
            for (auto f : (*you)->triangles_)
                (*me)->triangles_.push_back(triangle(f->index()));
        }
    }
}

} // namespace regina
