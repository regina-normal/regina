
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file triangulation/detail/skeleton-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  Triangulation class template.
 *
 *  This file is _not_ included from triangulation.h, and it is not
 *  shipped with Regina's development headers.  The routines it contains are
 *  explicitly instantiated in Regina's calculation engine for all dimensions.
 *
 *  The reason for "quarantining" this file is simply to avoid putting
 *  excessive implementation details in the headers where this is not needed.
 */

#ifndef __REGINA_SKELETON_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_SKELETON_IMPL_H_DETAIL
#endif

#include "triangulation/generic/triangulation.h"

namespace regina::detail {

template <int dim>
void TriangulationBase<dim>::calculateSkeleton() {
    // Set this now so that any simplex query routines do not try to
    // recursively recompute the skeleton again.
    calculatedSkeleton_ = true;

    // Triangulations are valid until proven otherwise.
    // Validity may fail here in the generic skeleton computations,
    // and/or in the specialised work that happens in standard dimensions.
    valid_ = true;

    // -----------------------------------------------------------------
    // Components, including orientability and the dual forest
    // -----------------------------------------------------------------

    // Triangulations are orientable until proven otherwise.
    orientable_ = true;

    for (auto it = simplices_.begin(); it != simplices_.end(); ++it) {
        (*it)->component_ = nullptr;
        (*it)->dualForest_ = 0;
    }

    // Our breadth-first search through simplices is non-recursive.
    // It uses a queue that contains simplices from which we need to propagate
    // component labelling.  We use a plain C array for this queue: since each
    // simplex is processed only once, an array of size simplices_.size()
    // is large enough.
    auto* queue = new Simplex<dim>*[simplices_.size()];
    size_t queueStart = 0, queueEnd = 0;

    // Note: we must work through simplices by increasing index, since
    // Simplex::orientation() promises that the smallest-index simplex
    // in each component will have orientation +1.
    for (auto it = simplices_.begin(); it != simplices_.end(); ++it) {
        Simplex<dim>* s = *it;
        if (! s->component_) {
            auto* c = new Component<dim>();
            components_.push_back(c);

            s->component_ = c;
            c->simplices_.push_back(s);
            s->orientation_ = 1;

            queue[queueEnd++] = s;
            while (queueStart < queueEnd) {
                s = queue[queueStart++];

                for (int facet = 0; facet <= dim; ++facet) {
                    Simplex<dim>* adj = s->adjacentSimplex(facet);
                    if (adj) {
                        int yourOrientation =
                            (s->adjacentGluing(facet).sign() == 1 ?
                            -s->orientation_ : s->orientation_);
                        if (adj->component_) {
                            if (yourOrientation != adj->orientation_) {
                                orientable_ = c->orientable_ = false;
                            }
                        } else {
                            adj->component_ = c;
                            c->simplices_.push_back(adj);
                            adj->orientation_ = yourOrientation;

                            s->dualForest_ |=
                                (typename Simplex<dim>::FacetMask(1) << facet);
                            adj->dualForest_ |=
                                (typename Simplex<dim>::FacetMask(1) <<
                                    s->adjacentFacet(facet));

                            queue[queueEnd++] = adj;
                        }
                    } else
                        ++c->boundaryFacets_;
                }
            }
        }
    }

    delete[] queue;

    // -----------------------------------------------------------------
    // Faces of all dimensions 0, ..., dim-1
    // -----------------------------------------------------------------

    std::fill(nBoundaryFaces_.begin(), nBoundaryFaces_.end(), 0);

    std::apply([this](auto&&... kFaces) {
        (calculateFaces<subdimOf<decltype(kFaces)>()>(this), ...);
    }, faces_);

    // -----------------------------------------------------------------
    // Real boundary components
    // -----------------------------------------------------------------

    calculateRealBoundary();
}

template <int dim>
template <int subdim>
void TriangulationBase<dim>::calculateFaces(TriangulationBase<dim>* tri) {
    // Clear out all subdim-faces of all simplices.
    // These simplex-based arrays will be our markers for what faces
    // have or have not been seen yet.
    //
    for (auto s : tri->simplices_)
        std::get<subdim>(s->faces_).fill(nullptr);

    if constexpr (subdim == dim - 1) {
        // Faces of codimension 1
        // ----------------------

        Simplex<dim> *adj;
        Face<dim, dim-1>* f;
        int facet, adjFacet;

        // We process the facets of each simplex in lexicographical order,
        // according to the truncated permutation labels that are displayed to
        // the user.  This means working through the faces of each simplex
        // in *reverse*.
        for (auto s : tri->simplices_) {
            for (facet = dim; facet >= 0; --facet) {
                // Have we already checked out this facet from the other side?
                if (std::get<dim-1>(s->faces_)[facet])
                    continue;

                // A new face!
                f = new Face<dim, dim-1>(s->component_);
                std::get<dim - 1>(tri->faces_).push_back(f);
                auto map = Face<dim, dim-1>::ordering(facet);

                adj = s->adjacentSimplex(facet);
                if (adj) {
                    // We have an adjacent simplex.
                    adjFacet = s->adjacentFacet(facet);
                    auto adjMap = s->adjacentGluing(facet) * map;

                    std::get<dim-1>(s->faces_)[facet] = f;
                    std::get<dim-1>(s->mappings_)[facet] = map;

                    std::get<dim-1>(adj->faces_)[adjFacet] = f;
                    std::get<dim-1>(adj->mappings_)[adjFacet] = adjMap;

                    // We have an orientation match with exactly one of
                    // {s, map} and {adj, adjMap}.  Ensure the one with
                    // the orientation map becomes the first embedding.
                    if (map.sign() == s->orientation_) {
                        f->embeddings_.push_back({s, map});
                        f->embeddings_.push_back({adj, adjMap});
                    } else {
                        f->embeddings_.push_back({adj, adjMap});
                        f->embeddings_.push_back({s, map});
                    }
                } else {
                    // This is a boundary facet, so we only get one embedding.
                    // If the orientation does not match then we will need to
                    // change the ordering of the vertices of the face.
                    if (map.sign() != s->orientation_)
                        map = map * Perm<dim + 1>(dim - 1, dim - 2);

                    std::get<dim-1>(s->faces_)[facet] = f;
                    std::get<dim-1>(s->mappings_)[facet] = map;

                    f->embeddings_.push_back({s, map});
                }
            }
        }
    } else if constexpr (subdim == dim - 2) {
        // Faces of codimension 2
        // ----------------------

        int start;
        Face<dim, dim-2>* f;
        Simplex<dim> *simp, *adj;
        int adjFace;
        Perm<dim+1> adjMap;
        int dir, exitFacet;
        for (auto s : tri->simplices_) {
            for (start = 0; start < FaceNumbering<dim, dim-2>::nFaces;
                    ++start) {
                if (std::get<dim-2>(s->faces_)[start])
                    continue;

                f = new Face<dim, dim-2>(s->component_);
                std::get<dim - 2>(tri->faces_).push_back(f);
                auto map = Face<dim, dim-2>::ordering(start);
                if (map.sign() != s->orientation_)
                    map = map * Perm<dim + 1>(dim - 1, dim);

                // Since the link of a codimension-2-face is a path or loop, the
                // depth-first search is really just a straight line in either
                // direction.  We therefore do away with the usual stack and
                // just keep track of the next simplex to process in the current
                // direction.
                std::get<dim-2>(s->faces_)[start] = f;
                std::get<dim-2>(s->mappings_)[start] = map;
                f->embeddings_.push_back({s, map});

                for (dir = 0; dir < 2; ++dir) {
                    // Start at the start and walk in one particular direction.
                    simp = s;
                    map = std::get<dim-2>(simp->mappings_)[start];

                    while (true) {
                        // Move through to the next simplex.
                        exitFacet = map[dir == 0 ? dim - 1 : dim];
                        adj = simp->adjacentSimplex(exitFacet);
                        if (! adj)
                            break;

                        adjMap = simp->adjacentGluing(exitFacet) * map *
                            Perm<dim+1>(dim - 1, dim);
                        adjFace = Face<dim, dim-2>::faceNumber(adjMap);

                        if (std::get<dim-2>(adj->faces_)[adjFace]) {
                            // We looped right around.
                            if constexpr (dim > 2) {
                                // Check that we are not mapping the face to
                                // itself with a non-identity permutation.
                                if (std::get<dim-2>(adj->mappings_)[
                                        adjFace] != adjMap) {
                                    // You have chosen unwisely, my son.
                                    if constexpr (standardDim(dim))
                                        f->whyInvalid_.value |=
                                            Face<dim, dim-2>::
                                            INVALID_IDENTIFICATION;
                                    else
                                        f->valid_.value = false;
                                    tri->valid_ = s->component_->valid_ = false;
                                }
                            }
                            break;
                        }

                        // We have not yet seen this face of this simplex.
                        std::get<dim-2>(adj->faces_)[adjFace] = f;
                        std::get<dim-2>(adj->mappings_)[adjFace] = adjMap;

                        if (dir == 0)
                            f->embeddings_.push_back({adj, adjMap});
                        else
                            f->embeddings_.push_front({adj, adjMap});

                        simp = adj;
                        map = adjMap;
                    }
                }
            }
        }
    } else {
        // Faces of codimension > 2
        // ------------------------

        int start;
        Face<dim, subdim>* f;
        Simplex<dim> *simp, *adj;
        int face, adjFace;
        Perm<dim + 1> adjMap;
        int exitFacet;

        // The queue for our breadth-first search.
        // We can do this using simple arrays - since each subdim-face of each
        // simplex is pushed on at most once, the array size does not need to
        // be very large.
        // Each element in our queue is a pair (simplex, face).
        auto* queue = new std::pair<Simplex<dim>*, int>
            [tri->size() * FaceNumbering<dim, subdim>::nFaces];
        unsigned queueStart, queueEnd;
        unsigned pos;

        for (auto s : tri->simplices_) {
            for (start = 0; start < FaceNumbering<dim, subdim>::nFaces;
                    ++start) {
                if (std::get<subdim>(s->faces_)[start])
                    continue;

                f = new Face<dim, subdim>(s->component_);
                std::get<subdim>(tri->faces_).push_back(f);
                auto map = Face<dim, subdim>::ordering(start);
                if (map.sign() != s->orientation_)
                    map = map * Perm<dim + 1>(dim - 1, dim);

                std::get<subdim>(s->faces_)[start] = f;
                std::get<subdim>(s->mappings_)[start] = map;
                f->embeddings_.push_back({s, map});

                // Run a breadth-first search from this vertex to completely
                // enumerate all identifications.
                queueStart = 0;
                queueEnd = 1;
                queue[0].first = s;
                queue[0].second = start;

                while (queueStart < queueEnd) {
                    simp = queue[queueStart].first;
                    face = queue[queueStart].second;
                    ++queueStart;

                    for (exitFacet = 0; exitFacet <= dim; ++exitFacet) {
                        if (Face<dim, subdim>::containsVertex(face, exitFacet))
                            continue;

                        adj = simp->adjacentSimplex(exitFacet);
                        if (adj) {
                            // When we choose an adjacent gluing map, throw in a
                            // swap to preserve the "orientation" of the images
                            // of (subdim+1),...,dim.  Note that this is only
                            // possible if the link of the face is orientable.
                            adjMap = simp->adjacentGluing(exitFacet) *
                                std::get<subdim>(simp->mappings_)[face] *
                                Perm<dim + 1>(dim - 1, dim);
                            adjFace = Face<dim, subdim>::faceNumber(adjMap);

                            if (std::get<subdim>(adj->faces_)[adjFace]) {
                                // We have looped back around to where we've
                                // been before.

                                if constexpr (subdim > 0) {
                                    // Have we mapped the face to itself with a
                                    // non-identity permutation?
                                    // Note that we only need to check
                                    // p[0,...,(subdim-1)] in the permutations
                                    // below, since p[subdim] then comes for
                                    // free.
                                    for (pos = 0; pos < subdim; ++pos)
                                        if (std::get<subdim>(adj->mappings_)
                                                [adjFace][pos] != adjMap[pos]) {
                                            if constexpr (standardDim(dim))
                                                f->whyInvalid_.value |=
                                                    Face<dim, subdim>::
                                                    INVALID_IDENTIFICATION;
                                            else
                                                f->valid_.value = false;
                                            tri->valid_ =
                                                s->component_->valid_ = false;
                                            break;
                                        }
                                }

                                if constexpr (subdim <= dim - 3) {
                                    // Is the link non-orientable?
                                    if (adjMap.sign() !=
                                            std::get<subdim>(adj->mappings_)
                                            [adjFace].sign())
                                        f->linkOrientable_.value = false;
                                }
                            } else {
                                std::get<subdim>(adj->faces_)[adjFace] = f;
                                std::get<subdim>(adj->mappings_)[adjFace] =
                                    adjMap;
                                f->embeddings_.push_back({adj, adjMap});

                                queue[queueEnd].first = adj;
                                queue[queueEnd].second = adjFace;
                                ++queueEnd;
                            }
                        }
                    }
                }
            }
        }

        delete[] queue;
    }
}

template <int dim>
void TriangulationBase<dim>::calculateRealBoundary() {
    // Are there any boundary facets at all?
    long nBdry = 2 * countFaces<dim-1>() - (dim+1) * simplices_.size();
    if (nBdry == 0)
        return;

    // This array stores an orientation for each triangle.
    int* orient = new int[countFaces<dim-1>()];

    // Although we are just doing a BFS, we use a deque instead of a queue
    // since we are already dragging in the deque header, and since queue
    // is implemented using deque anyway.

    BoundaryComponent<dim>* label;
    std::deque<Face<dim, dim-1>*> queue;
    Simplex<dim> *simp;
    int facetNum, ridgeNum, i;
    Face<dim, dim-1> *facet, *adjFacet;
    Face<dim, dim-2>* ridge;
    FaceEmbedding<dim, dim-2> ridgeEmbFront, ridgeEmbBack;
    Perm<dim + 1> switchPerm(dim - 1, dim);
    Perm<dim + 1> facetGluing;
    int adjOrient;
    for (Face<dim, dim-1>* loopFacet : faces<dim-1>()) {
        // We only care about boundary facets that we haven't yet seen.
        if (loopFacet->degree() == 2 || loopFacet->boundaryComponent_)
            continue;

        label = new BoundaryComponent<dim>();
        label->orientable_ = true;
        boundaryComponents_.push_back(label);
        loopFacet->component()->boundaryComponents_.push_back(label);

        // Run a breadth-first search from this boundary facet to
        // completely enumerate all (dim-1)-faces in this boundary component.

        loopFacet->boundaryComponent_ = label;
        ++nBoundaryFaces_[dim - 1];
        label->push_back(loopFacet);
        orient[loopFacet->index()] = 1;

        queue.push_back(loopFacet);

        while (! queue.empty()) {
            facet = queue.front();
            queue.pop_front();
            simp = facet->front().simplex();
            facetNum = facet->front().face();

            // Run through all faces of dimensions 0,...,(dim-3) within facet,
            // and include them in this boundary component.
            std::apply([this, label, facet](auto&&... kFaces) {
                (calculateBoundaryFaces<subdimOf<decltype(kFaces)>()>(
                    label, facet), ...);
            }, faces_);

            // Finally we process the (dim-2)-faces, and also use these to
            // locate adjacent boundary facets.
            for (i = 0; i <= dim; ++i) {
                if (i == facetNum)
                    continue;

                // Examine the (dim-2)-face opposite vertices (i, facetNum)
                // of simp.
                ridgeNum = faceOppositeEdge<dim>(i, facetNum);
                ridge = simp->template face<dim-2>(ridgeNum);
                if (! ridge->boundaryComponent_) {
                    ridge->boundaryComponent_ = label;
                    ++nBoundaryFaces_[dim - 2];
                    label->push_back(ridge);
                }

                // Okay, we can be clever about this.  The current
                // boundary facet is one end of the link of ridge; the
                // *adjacent* boundary facet must be at the other.
                ridgeEmbFront = ridge->front();
                ridgeEmbBack = ridge->back();
                if (ridgeEmbFront.simplex() == simp &&
                        ridgeEmbFront.vertices()[dim-1] == i &&
                        ridgeEmbFront.vertices()[dim] == facetNum) {
                    // We are currently looking at the embedding at the
                    // front of the list.  Take the one at the back.
                    adjFacet = ridgeEmbBack.simplex()->template face<dim-1>(
                        ridgeEmbBack.vertices()[dim-1]);
                    facetGluing =
                        adjFacet->front().vertices().inverse() *
                        ridgeEmbBack.vertices() *
                        switchPerm *
                        ridgeEmbFront.vertices().inverse() *
                        facet->front().vertices();
                } else {
                    // We must be looking at the embedding at the back
                    // of the list.  Take the one at the front.
                    adjFacet = ridgeEmbFront.simplex()->template face<dim-1>(
                        ridgeEmbFront.vertices()[dim]);
                    facetGluing =
                        adjFacet->front().vertices().inverse() *
                        ridgeEmbFront.vertices() *
                        switchPerm *
                        ridgeEmbBack.vertices().inverse() *
                        facet->front().vertices();
                }

                adjOrient = (facetGluing.sign() > 0 ?
                    -orient[facet->index()] : orient[facet->index()]);

                // Push the adjacent facet onto the queue for processing.
                if (adjFacet->boundaryComponent_) {
                    if (adjOrient != orient[adjFacet->index()])
                        label->orientable_ = false;
                } else {
                    adjFacet->boundaryComponent_ = label;
                    ++nBoundaryFaces_[dim - 1];
                    label->push_back(adjFacet);
                    orient[adjFacet->index()] = adjOrient;
                    queue.push_back(adjFacet);
                }
            }
        }
    }

    delete[] orient;
}

template <int dim>
template <int subdim>
void TriangulationBase<dim>::calculateBoundaryFaces(BoundaryComponent<dim>* bc,
        Face<dim, dim-1>* facet) {
    // We do not process ridges (dim-2) or facets (dim-1).
    if constexpr (subdim <= dim - 3) {
        if constexpr (subdim == 0) {
            // Treat vertices separately, since we can optimise the
            // vertex number calculations in this case.
            Simplex<dim>* simp = facet->front().simplex();
            int facetNum = facet->front().face();
            for (int i = 0; i <= dim; ++i)
                if (i != facetNum) {
                    Vertex<dim>* v = simp->vertex(i);
                    // Note: in the case of (invalid) pinched faces,
                    // v might already belong to some other boundary component.
                    if (v->boundaryComponent_ != bc) {
                        if (! v->boundaryComponent_)
                            ++nBoundaryFaces_[0];
                        v->boundaryComponent_ = bc;
                        // If allFaces is false, then the boundary component
                        // only wants to know about ridges and facets.
                        if constexpr (BoundaryComponent<dim>::allFaces)
                            bc->push_back(v);
                    }
                }
        } else {
            for (unsigned i = 0; i < binomSmall(dim, subdim + 1); ++i) {
                Face<dim, subdim>* f = facet->template face<subdim>(i);
                // Note: in the case of (invalid) pinched faces,
                // f might already belong to some other boundary component.
                if (f->boundaryComponent_ != bc) {
                    if (! f->boundaryComponent_)
                        ++nBoundaryFaces_[subdim];
                    f->boundaryComponent_ = bc;
                    // If allFaces is false, then the boundary component only
                    // wants to know about ridges and facets.
                    if constexpr (BoundaryComponent<dim>::allFaces)
                        bc->push_back(f);
                }
            }
        }
    }
}

template <int dim>
void TriangulationBase<dim>::clearBaseProperties() {
    // Delete the skeleton.
    if (calculatedSkeleton_) {
        for (auto c : components_)
            delete c;
        for (auto b : boundaryComponents_)
            delete b;
        components_.clear();
        boundaryComponents_.clear();

        std::apply([](auto&&... kFaces) {
            (kFaces.clear_destructive(), ...);
        }, faces_);

        calculatedSkeleton_ = false;
    }

    // Clear properties.
    if (! topologyLocked()) {
        fundGroup_.reset();
        H1_.reset();
    }
}

template <int dim>
void TriangulationBase<dim>::swapBaseData(TriangulationBase<dim>& other) {
    Snapshottable<Triangulation<dim>>::swap(other);

    // Simplices:
    simplices_.swap(other.simplices_);

    for (auto s : simplices_)
        s->tri_ = static_cast<Triangulation<dim>*>(this);
    for (auto s : other.simplices_)
        s->tri_ = static_cast<Triangulation<dim>*>(&other);

    // Properties stored directly:
    std::swap(valid_, other.valid_);
    std::swap(calculatedSkeleton_, other.calculatedSkeleton_);
    std::swap(orientable_, other.orientable_);

    // Properties stored using std::... containers or MarkedVector:
    components_.swap(other.components_);
    boundaryComponents_.swap(other.boundaryComponents_);
    faces_.swap(other.faces_);
    nBoundaryFaces_.swap(other.nBoundaryFaces_);
    fundGroup_.swap(other.fundGroup_);
    H1_.swap(other.H1_);
}

template <int dim>
template <typename FaceList>
void TriangulationBase<dim>::cloneFaces(const FaceList& srcFaces) {
    static constexpr int subdim =
        std::remove_reference_t<decltype(*srcFaces.front())>::subdimension;

    for (auto you : srcFaces) {
        auto me = new Face<dim, subdim>(components_[you->component_->index()]);
        std::get<subdim>(faces_).push_back(me);

        if (you->boundaryComponent_)
            me->boundaryComponent_ =
                boundaryComponents_[you->boundaryComponent_->index()];

        for (auto emb : you->embeddings_)
            me->embeddings_.push_back(FaceEmbedding<dim, subdim>(
                simplices_[emb.simplex()->index()], emb.vertices()));

        if constexpr (Face<dim, subdim>::allowsNonOrientableLinks)
            me->linkOrientable_ = you->linkOrientable_;
        if constexpr (Face<dim, subdim>::allowsInvalidFaces) {
            if constexpr (standardDim(dim))
                me->whyInvalid_ = you->whyInvalid_;
            else
                me->valid_ = you->valid_;
        }
    }
}

template <int dim>
void TriangulationBase<dim>::cloneSkeleton(const TriangulationBase<dim>& src) {
    // Boundary components:
    for (auto you : src.boundaryComponents_) {
        auto me = new BoundaryComponent<dim>();
        boundaryComponents_.push_back(me);

        // We will clone the face lists later, once we have cloned the faces.

        if constexpr (! BoundaryComponent<dim>::allFaces)
            me->nRidges_ = you->nRidges_;

        me->orientable_ = you->orientable_;

        // Leave boundary_ as build-on-demand for now.
    }

    // Components (uses boundary components):
    for (auto you : src.components_) {
        auto me = new Component<dim>();
        components_.push_back(me);

        for (auto s : you->simplices_)
            me->simplices_.push_back(simplices_[s->index()]);
        for (auto b : you->boundaryComponents_)
            me->boundaryComponents_.push_back(boundaryComponents_[b->index()]);
        me->valid_ = you->valid_;
        me->boundaryFacets_ = you->boundaryFacets_;
        me->orientable_ = you->orientable_;
    }

    // Faces (uses components, boundary components):
    std::apply([this](auto&&... kFaces) {
        (cloneFaces(kFaces), ...);
    }, src.faces_);

    // Face lists in boundary components:
    {
        auto me = boundaryComponents_.begin();
        auto you = src.boundaryComponents_.begin();
        for ( ; me != boundaryComponents_.end(); ++me, ++you) {
            std::apply([this, me](auto&&... kFaces) {
                (cloneBoundaryFaces(*me, kFaces), ...);
            }, (*you)->faces_);
        }
    }

    // Simplices (uses faces, components):
    {
        auto me = simplices_.begin();
        auto you = src.simplices_.begin();
        for ( ; me != simplices_.end(); ++me, ++you) {
            for_constexpr<0, dim>([this, me, you](auto subdim) {
                auto dest = std::get<subdim.value>((*me)->faces_).begin();
                for (auto f : std::get<subdim.value>((*you)->faces_))
                    *dest++ = clonedFace(f);
            });
            (*me)->mappings_ = (*you)->mappings_;
            (*me)->orientation_ = (*you)->orientation_;
            (*me)->component_ = components_[(*you)->component_->index()];
            (*me)->dualForest_ = (*you)->dualForest_;
        }
    }

    // Other properties of the triangulation:
    nBoundaryFaces_ = src.nBoundaryFaces_;
    valid_ = src.valid_;
    orientable_ = src.orientable_;

    calculatedSkeleton_ = true;
}

} // namespace regina::detail

#endif
