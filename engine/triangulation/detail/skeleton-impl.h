
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

/*! \file triangulation/detail/skeleton-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  Triangulation class template.
 *
 *  This file is \e not included from triangulation.h, but the routines
 *  it contains are explicitly instantiated in Regina's calculation engine.
 *  Therefore end users should never need to include this header.
 */

#ifndef __SKELETON_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __SKELETON_IMPL_H_DETAIL
#endif

#include "triangulation/generic/triangulation.h"

namespace regina {
namespace detail {

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

    SimplexIterator it;
    for (it = simplices_.begin(); it != simplices_.end(); ++it) {
        (*it)->component_ = 0;
        (*it)->dualForest_ = 0;
    }

    // Our breadth-first search through simplices is non-recursive.
    // It uses a queue that contains simplices from which we need to propagate
    // component labelling.  We use a plain C array for this queue: since each
    // simplex is processed only once, an array of size simplices_.size()
    // is large enough.
    Simplex<dim>** queue = new Simplex<dim>*[simplices_.size()];
    size_t queueStart = 0, queueEnd = 0;

    Component<dim>* c;
    Simplex<dim> *s, *adj;
    int facet;
    int yourOrientation;
    for (it = simplices_.begin(); it != simplices_.end(); ++it) {
        s = *it;
        if (s->component_ == 0) {
            c = new Component<dim>();
            components_.push_back(c);

            s->component_ = c;
            c->simplices_.push_back(s);
            s->orientation_ = 1;

            queue[queueEnd++] = s;
            while (queueStart < queueEnd) {
                s = queue[queueStart++];

                for (facet = 0; facet <= dim; ++facet) {
                    adj = s->adjacentSimplex(facet);
                    if (adj) {
                        yourOrientation =
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
    // Faces of all dimensions
    // -----------------------------------------------------------------

    FaceCalculator<dim, dim - 1, 1>::calculate(*this);

    // -----------------------------------------------------------------
    // Real boundary components
    // -----------------------------------------------------------------

    calculateRealBoundary();
}

template <int dim>
void TriangulationBase<dim>::calculateSkeletonCodim1() {
    for (auto s : simplices_)
        s->SimplexFaces<dim, dim-1>::clear();

    Simplex<dim> *adj;
    Face<dim, dim-1>* f;
    int facet, adjFacet;

    // We process the facets of each simplex in lexicographical order,
    // according to the truncated permutation labels that are displayed to
    // the user.  This means working through the faces of each simplex
    // in *reverse*.
    for (auto s : simplices_) {
        for (facet = dim; facet >= 0; --facet) {
            // Have we already checked out this facet from the other side?
            if (s->SimplexFaces<dim, dim-1>::face_[facet])
                continue;

            // A new face!
            f = new Face<dim, dim-1>(s->component_);
            FaceList<dim, dim-1>::push_back(f);

            s->SimplexFaces<dim, dim-1>::face_[facet] = f;
            s->SimplexFaces<dim, dim-1>::mapping_[facet] =
                Face<dim, dim-1>::ordering(facet);

            adj = s->adjacentSimplex(facet);
            if (adj) {
                // We have an adjacent simplex.
                adjFacet = s->adjacentFacet(facet);

                adj->SimplexFaces<dim, dim-1>::face_[adjFacet] = f;
                adj->SimplexFaces<dim, dim-1>::mapping_[adjFacet] =
                    s->adjacentGluing(facet) *
                    s->SimplexFaces<dim, dim-1>::mapping_[facet];

                f->push_back(FaceEmbedding<dim, dim-1>(s, facet));
                f->push_back(FaceEmbedding<dim, dim-1>(adj, adjFacet));
            } else {
                // This is a boundary facet.
                f->push_back(FaceEmbedding<dim, dim-1>(s, facet));
            }
        }
    }
}

template <int dim>
void TriangulationBase<dim>::calculateSkeletonCodim2() {
    for (auto s : simplices_)
        s->SimplexFaces<dim, dim-2>::clear();

    int start;
    Face<dim, dim-2>* f;
    Simplex<dim> *simp, *adj;
    int adjFace;
    Perm<dim+1> map, adjMap;
    int dir, exitFacet;
    for (auto s : simplices_) {
        for (start = 0; start < FaceNumbering<dim, dim-2>::nFaces; ++start) {
            if (s->SimplexFaces<dim, dim-2>::face_[start])
                continue;

            f = new Face<dim, dim-2>(s->component_);
            FaceList<dim, dim-2>::push_back(f);

            // Since the link of a codimension-2-face is a path or loop, the
            // depth-first search is really just a straight line in either
            // direction.  We therefore do away with the usual stack and
            // just keep track of the next simplex to process in the current
            // direction.
            s->SimplexFaces<dim, dim-2>::face_[start] = f;
            s->SimplexFaces<dim, dim-2>::mapping_[start] =
                Face<dim, dim-2>::ordering(start);
            f->push_back(FaceEmbedding<dim, dim-2>(s, start));

            for (dir = 0; dir < 2; ++dir) {
                // Start at the start and walk in one particular direction.
                simp = s;
                map = simp->SimplexFaces<dim, dim-2>::mapping_[start];

                while (true) {
                    // Move through to the next simplex.
                    exitFacet = map[dir == 0 ? dim - 1 : dim];
                    adj = simp->adjacentSimplex(exitFacet);
                    if (! adj)
                        break;

                    adjMap = simp->adjacentGluing(exitFacet) * map *
                        Perm<dim+1>(dim - 1, dim);
                    adjFace = Face<dim, dim-2>::faceNumber(adjMap);

                    if (adj->SimplexFaces<dim, dim-2>::face_[adjFace]) {
                        // We looped right around.
                        if (dim > 2) {
                            // Check that we are not mapping the face to
                            // itself with a non-identity permutation.
                            if (adj->SimplexFaces<dim, dim-2>::mapping_[
                                    adjFace] != adjMap) {
                                // You have chosen unwisely, my son.
                                f->markBadIdentification();
                                valid_ = s->component_->valid_ = false;
                            }
                        }
                        break;
                    }

                    // We have not yet seen this face of this simplex.
                    adj->SimplexFaces<dim, dim-2>::face_[adjFace] = f;
                    adj->SimplexFaces<dim, dim-2>::mapping_[adjFace] =
                        adjMap;

                    if (dir == 0)
                        f->push_back(FaceEmbedding<dim, dim-2>(
                            adj, adjFace));
                    else
                        f->push_front(FaceEmbedding<dim, dim-2>(
                            adj, adjFace));

                    simp = adj;
                    map = adjMap;
                }
            }
        }
    }
}

template <int dim>
template <int subdim>
void TriangulationBase<dim>::calculateSkeletonSubdim() {
    static_assert(subdim < dim - 2,
        "The generic implementation of "
        "TriangulationBase::calculateSkeletonSubdim() should only be "
        "used for faces of codimension > 2.");

    for (auto s : simplices_)
        s->SimplexFaces<dim, subdim>::clear();

    int start;
    Face<dim, subdim>* f;

    // The queue for our breadth-first search.
    // We can do this using simple arrays - since each subdim-face of each
    // simplex is pushed on at most once, the array size does not need to
    // be very large.
    typedef std::pair<Simplex<dim>*, int> Spec; /* (simplex, face) */
    Spec* queue = new Spec[size() * FaceNumbering<dim, subdim>::nFaces];
    unsigned queueStart, queueEnd;
    unsigned pos;

    for (auto s : simplices_) {
        for (start = 0; start < FaceNumbering<dim, subdim>::nFaces; ++start) {
            if (s->SimplexFaces<dim, subdim>::face_[start])
                continue;

            f = new Face<dim, subdim>(s->component_);
            FaceList<dim, subdim>::push_back(f);

            s->SimplexFaces<dim, subdim>::face_[start] = f;
            s->SimplexFaces<dim, subdim>::mapping_[start] =
                Face<dim, subdim>::ordering(start);
            f->push_back(FaceEmbedding<dim, subdim>(s, start));

            // Run a breadth-first search from this vertex to completely
            // enumerate all identifications.
            queueStart = 0;
            queueEnd = 1;
            queue[0].first = s;
            queue[0].second = start;

            Simplex<dim> *simp, *adj;
            int face, adjFace;
            Perm<dim + 1> adjMap;
            int facet;

            while (queueStart < queueEnd) {
                simp = queue[queueStart].first;
                face = queue[queueStart].second;
                ++queueStart;

                for (facet = 0; facet <= dim; ++facet) {
                    if (Face<dim, subdim>::containsVertex(face, facet))
                        continue;

                    adj = simp->adjacentSimplex(facet);
                    if (adj) {
                        // When we choose an adjacent gluing map, throw in a
                        // swap to preserve the "orientation" of the images
                        // of (subdim+1),...,dim.  Note that this is only
                        // possible if the link of the face is orientable.
                        adjMap = simp->adjacentGluing(facet) *
                            simp->SimplexFaces<dim, subdim>::mapping_[face] *
                            Perm<dim + 1>(dim - 1, dim);
                        adjFace = Face<dim, subdim>::faceNumber(adjMap);

                        if (adj->SimplexFaces<dim, subdim>::face_[adjFace]) {
                            // We have looped back around to where we've
                            // been before.

                            if (subdim > 0) {
                                // Have we mapped the face to itself with a
                                // non-identity permutation?
                                // Note that we only need to check the images
                                // p[0,...,(subdim-1)] in the permutations
                                // below, since p[subdim] will then come for
                                // free.
                                for (pos = 0; pos < subdim; ++pos)
                                    if (adj->SimplexFaces<dim, subdim>::
                                            mapping_[adjFace][pos] !=
                                            adjMap[pos]) {
                                        f->markBadIdentification();
                                        valid_ = s->component_->valid_ = false;
                                        break;
                                    }
                            }

                            if (subdim <= dim - 3) {
                                // Is the link non-orientable?
                                if (adjMap.sign() !=
                                        adj->SimplexFaces<dim, subdim>::
                                        mapping_[adjFace].sign())
                                    f->markLinkNonorientable();
                            }
                        } else {
                            adj->SimplexFaces<dim, subdim>::face_[adjFace] = f;
                            adj->SimplexFaces<dim, subdim>::mapping_[adjFace] =
                                adjMap;
                            f->push_back(FaceEmbedding<dim, subdim>(
                                adj, adjFace));

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
        label->push_back(loopFacet);
        orient[loopFacet->index()] = 1;

        queue.push_back(loopFacet);

        while (! queue.empty()) {
            facet = queue.front();
            queue.pop_front();
            simp = facet->front().simplex();
            facetNum = facet->front().face();

            // Run through all faces of dimensions 0,...,(dim-2) within facet,
            // and include them in this boundary component.

            // Treat the vertices separately, since we can optimise the
            // vertex number calculations in this case.
            if (dim >= 3)
                for (i = 0; i <= dim; ++i)
                    if (i != facetNum) {
                        Face<dim, 0>* vertex = simp->vertex(i);
                        if (vertex->boundaryComponent_ != label) {
                            vertex->boundaryComponent_ = label;
                            label->push_back(vertex);
                        }
                    }

            // Now for faces of dimension 1..(dim-3):
            BoundaryComponentCalculator<dim, dim - 3>::identify(*this,
                label, facet);

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
    for (unsigned i = 0; i < binomSmall(dim, subdim + 1); ++i) {
        Face<dim, subdim>* f = facet->template face<subdim>(i);
        if (f->boundaryComponent_ != bc) {
            f->boundaryComponent_ = bc;
            bc->push_back(f);
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

        FaceListSuite<dim, dim - 1>::deleteFaces();

        calculatedSkeleton_ = false;
    }

    // Clear properties.
    fundGroup_.clear();
    H1_.clear();
}

template <int dim>
void TriangulationBase<dim>::swapBaseProperties(TriangulationBase<dim>& other) {
    // Properties stored directly:
    std::swap(valid_, other.valid_);
    std::swap(calculatedSkeleton_, other.calculatedSkeleton_);
    std::swap(orientable_, other.orientable_);

    // Properties stored using std::... containers or MarkedVector:
    components_.swap(other.components_);
    boundaryComponents_.swap(other.boundaryComponents_);

    FaceListSuite<dim, dim - 1>::swapFaces(other);

    // Properties stored using the Property<...> class template:
    fundGroup_.swap(other.fundGroup_);
    H1_.swap(other.H1_);
}

} } // namespace regina::detail

#endif
