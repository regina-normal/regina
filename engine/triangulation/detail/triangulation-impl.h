
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

/*! \file triangulation/detail/triangulation-impl.h
 *  \brief Contains some of the implementation details for the generic
 *  Triangulation class template.
 *
 *  This file is _not_ included from triangulate.h, and it is not
 *  shipped with Regina's development headers.  The routines it contains are
 *  explicitly instantiated in Regina's calculation engine for all dimensions.
 *
 *  The reason for "quarantining" this file is simply to avoid putting
 *  excessive implementation details in the headers where this is not needed.
 */

#ifndef __REGINA_TRIANGULATION_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_TRIANGULATION_IMPL_H_DETAIL
#endif

#include "triangulation/generic/triangulation.h"
#include "utilities/fixedarray.h"

// The symbol used in writeTextLong() to indicate a locked simplex or facet:
#define LOCKED_MARKER '*'
// #define LOCKED_MARKER "⚿"; // U+26BF: squared key

namespace regina::detail {

template <int dim>
void TriangulationBase<dim>::writeTextShort(std::ostream& out) const {
    if (isEmpty()) {
        out << "Empty " << dim << "-D triangulation";
        return;
    }

    if (! isValid())
        out << "Invalid ";
    else if constexpr (dim == 2) {
        if (hasBoundaryFacets())
            out << "Bounded ";
        else
            out << "Closed ";
    } else if constexpr (standardDim(dim)) {
        if (static_cast<const Triangulation<dim>*>(this)->isClosed())
            out << "Closed ";
        else if (static_cast<const Triangulation<dim>*>(this)->isIdeal()) {
            if (hasBoundaryFacets())
                out << "Ideal/bounded ";
            else
                out << "Ideal ";
        } else
            out << "Bounded ";
    } else {
        if (hasBoundaryFacets())
            out << "Bounded ";
        else
            out << "Possibly closed ";
    }

    if (isOrientable())
        out << "orientable ";
    else
        out << "non-orientable ";

    out << dim << "-D triangulation, f = (";
    for (auto f : fVector())
        out << ' ' << f;
    out << " )";
}

template <int dim>
void TriangulationBase<dim>::writeTextLong(std::ostream& out) const {
    ensureSkeleton();

    if constexpr (dim > 4) {
        writeTextShort(out);
        out << "\n\n";
    } else {
        out << "Size of the skeleton:\n";
        if constexpr (dim >= 4)
            out << "  Pentachora: " << countFaces<4>() << '\n';
        if constexpr (dim >= 3)
            out << "  Tetrahedra: " << countFaces<3>() << '\n';
        out << "  Triangles: " << countFaces<2>() << '\n';
        out << "  Edges: " << countFaces<1>() << '\n';
        out << "  Vertices: " << countFaces<0>() << '\n';
        out << '\n';
    }

    Simplex<dim>* simp;
    Simplex<dim>* adj;
    size_t pos;
    int j;
    Perm<dim+1> gluing;

    if constexpr (dim == 2)
        out << "Triangle gluing:\n  Triangle  |  gluing:";
    else if constexpr (dim == 3)
        out << "Tetrahedron gluing:\n  Tet  |  gluing:";
    else if constexpr (dim == 4)
        out << "Pentachoron gluing:\n  Pent  |  gluing:";
    else
        out << "  Simplex  |  gluing:";
    for (int i = dim; i >= 0; --i) {
        if constexpr (dim == 3)
            out << "       (";
        else
            out << "      (";
        for (j = 0; j <= dim; ++j)
            if (j != i)
                out << regina::digit(j);
        out << ')';
    }
    out << '\n';
    if constexpr (dim == 2)
        out << "  ----------+---------";
    else if constexpr (dim == 3)
        out << "  -----+---------";
    else if constexpr (dim == 4)
        out << "  ------+---------";
    else
        out << "  ---------+---------";
    for (int i = dim; i >= 0; --i)
        for (j = 0; j < (dim == 3 ? 12 : 8 + dim); ++j)
            out << '-';
    out << '\n';
    for (pos=0; pos < simplices_.size(); pos++) {
        simp = simplices_[pos];
        if constexpr (dim == 2)
            out << "      ";
        else if constexpr (dim == 3)
            out << ' ';
        else if constexpr (dim == 4)
            out << "  ";
        else
            out << "     ";
        out << std::setw(4) << pos;
        if (simp->isLocked())
            out << LOCKED_MARKER;
        else
            out << ' ';
        out << " |          ";
        for (int i = dim; i >= 0; --i) {
            adj = simp->adjacentSimplex(i);
            if (! adj) {
                for (j = 0; j < (dim == 3 ? 3 : dim - 1); ++j)
                    out << ' ';
                out << "boundary";
            } else {
                gluing = simp->adjacentGluing(i);
                out << std::setw(dim == 3 ? 5 : 4) << adj->index() << " (";
                for (j = 0; j <= dim; ++j) {
                    if (j != i)
                        out << regina::digit(gluing[j]);
                }
                out << ")";
            }
            if (simp->isFacetLocked(i))
                out << LOCKED_MARKER;
            else if (i > 0)
                out << ' ';
        }
        out << '\n';
    }
    out << '\n';

    // For Regina's standard dimensions, write skeletal details also.

    if constexpr (dim <= 4) {
        out << "Vertices:\n";
        if constexpr (dim == 2)
            out << "  Triangle  |  vertex: ";
        else if constexpr (dim == 3)
            out << "  Tet  |  vertex: ";
        else if constexpr (dim == 4)
            out << "  Pent  |  vertex: ";
        for (int i = 0; i <= dim; ++i)
            out << "   " << i;
        out << '\n';
        if constexpr (dim == 2)
            out << "  ----------+----------";
        else if constexpr (dim == 3)
            out << "  -----+----------";
        else if constexpr (dim == 4)
            out << "  ------+----------";
        for (int i = 0; i <= dim; ++i)
            out << "----";
        out << '\n';
        for (size_t triPos = 0; triPos < simplices_.size(); ++triPos) {
            const Simplex<dim>* tri = simplices_[triPos];
            if constexpr (dim == 2)
                out << "      " << std::setw(4) << triPos << "  |          ";
            else if constexpr (dim == 3)
                out << " " << std::setw(4) << triPos << "  |          ";
            else if constexpr (dim == 4)
                out << "  " << std::setw(4) << triPos << "  |          ";
            for (int i = 0; i <= dim; ++i)
                out << ' ' << std::setw(3) << tri->vertex(i)->index();
            out << '\n';
        }
        out << '\n';

        out << "Edges:\n";
        if constexpr (dim == 2)
            out << "  Triangle  |  edge: ";
        else if constexpr (dim == 3)
            out << "  Tet  |  edge: ";
        else if constexpr (dim == 4)
            out << "  Pent  |  edge: ";
        for (int i1 = 0; i1 < dim; ++i1)
            for (int i2 = i1 + 1; i2 <= dim; ++i2)
                out << "  " << i1 << i2;
        out << '\n';
        if constexpr (dim == 2)
            out << "  ----------+--------";
        else if constexpr (dim == 3)
            out << "  -----+--------";
        else if constexpr (dim == 4)
            out << "  ------+--------";
        for (int i = 0; i < ((dim * (dim + 1)) / 2); ++i)
            out << "----";
        out << '\n';
        for (size_t triPos = 0; triPos < simplices_.size(); ++triPos) {
            const Simplex<dim>* tri = simplices_[triPos];
            if constexpr (dim == 2)
                out << "      " << std::setw(4) << triPos << "  |        ";
            else if constexpr (dim == 3)
                out << " " << std::setw(4) << triPos << "  |        ";
            else if constexpr (dim == 4)
                out << "  " << std::setw(4) << triPos << "  |        ";
            // Forward lexicographic numbering kicks in at dimension 3.
            if constexpr (dim == 2) {
                for (int i = 2; i >= 0; --i)
                    out << ' ' << std::setw(3) << tri->edge(i)->index();
            } else {
                for (int i = 0; i < ((dim * (dim + 1)) / 2); ++i)
                    out << ' ' << std::setw(3) << tri->edge(i)->index();
            }
            out << '\n';
        }
        out << '\n';

        if constexpr (dim >= 3) {
            out << "Triangles:\n";
            if constexpr (dim == 3)
                out << "  Tet  |  face: ";
            else
                out << "  Pent  |  triangle: ";
            for (int i1 = 0; i1 < dim - 1; ++i1)
                for (int i2 = i1 + 1; i2 < dim; ++i2)
                    for (int i3 = i2 + 1; i3 <= dim; ++i3)
                        out << ' ' << i1 << i2 << i3;
            out << '\n';
            if constexpr (dim == 3)
                out << "  -----+--------";
            else
                out << "  ------+------------";
            for (int i = 0; i < (dim == 3 ? 4 : 10); ++i)
                out << "----";
            out << '\n';
            for (size_t tetPos = 0; tetPos < simplices_.size(); ++tetPos) {
                const Simplex<dim>* tet = simplices_[tetPos];
                if constexpr (dim == 3)
                    out << "  " << std::setw(3) << tetPos << "  |        ";
                else
                    out << "  " << std::setw(4) << tetPos << "  |            ";
                // Forward lexicographic numbering kicks in at dimension 5.
                // Here we are only working with dimensions 3 and 4.
                for (int face = (dim == 3 ? 3 : 9); face >= 0; face--)
                    out << ' ' << std::setw(3) << tet->triangle(face)->index();
                out << '\n';
            }
            out << '\n';
        }

        if constexpr (dim == 4) {
            // Here the dimension is fixed, and so we can just hard-code
            // everything for dimension 4 specifically.
            out << "Tetrahedra:\n";
            out << "  Pent  |  facet:  0123 0124 0134 0234 1234\n";
            out << "  ------+----------------------------------\n";
            for (size_t pentPos = 0; pentPos < simplices_.size(); ++pentPos) {
                const Simplex<dim>* pent = simplices_[pentPos];
                out << "  " << std::setw(4) << pentPos << "  |         ";
                for (int i = 4; i >= 0; --i)
                    out << ' ' << std::setw(4) << pent->tetrahedron(i)->index();
                out << '\n';
            }
            out << '\n';
        }
    }
}

template <int dim>
void TriangulationBase<dim>::reorderBFS(bool reverse) {
    size_t n = size();
    if (n == 0)
        return;

    ChangeAndClearSpan<ChangeType::Cosmetic> span(*this);

    // Run a breadth-first search over all top-dimensional simplices.
    auto* ordered = new Simplex<dim>*[n];
    bool* used = new bool[n];

    std::fill(used, used + n, false);
    size_t filled = 0;    /* Placed in ordered[]. */
    size_t processed = 0; /* All neighbours placed in ordered[]. */
    size_t nextSimp = 0;  /* Used to search for connected components. */

    Simplex<dim> *simp, *adj;
    while (processed < n) {
        if (filled == processed) {
            // Look for the next connected component.
            while (used[nextSimp])
                ++nextSimp;

            ordered[filled++] = simplices_[nextSimp];
            used[nextSimp] = true;
            ++nextSimp;
        }

        simp = ordered[processed];

        // Add all neighbours of simp to the queue.
        for (int i = 0; i <= dim; ++i)
            if ((adj = simp->adjacentSimplex(i)))
                if (! used[adj->markedIndex()]) {
                    ordered[filled++] = adj;
                    used[adj->markedIndex()] = true;
                }

        ++processed;
    }

    // Flush the simplices from the triangulation, and reinsert them in
    // the order in which they were found during the breadth-first search.
    simplices_.clear();

    if (reverse) {
        for (size_t j = n; j > 0; )
            simplices_.push_back(ordered[--j]);
    } else {
        for (size_t j = 0; j < n; )
            simplices_.push_back(ordered[j++]);
    }

    delete[] used;
    delete[] ordered;
}

template <int dim>
Triangulation<dim> TriangulationBase<dim>::doubleCover() const {
    size_t sheetSize = simplices_.size();
    if (sheetSize == 0)
        return {};

    Triangulation<dim> ans;

    // Create two sheets of simplices, cloning simplex descriptions and locks.
    for (size_t i = 0; i < sheetSize; i++)
        ans.simplices_.push_back(new Simplex<dim>(*simplices_[i],
            std::addressof(ans)));
    for (size_t i = 0; i < sheetSize; i++)
        ans.simplices_.push_back(new Simplex<dim>(*simplices_[i],
            std::addressof(ans)));

    // We will temporarily hijack the Simplex::orientation_ fields for our own
    // purposes.  These will be reset/overwritten if/when we compute the
    // skeleton at some later time.
    //
    // We manage all simplex gluings manually at a low level, so we can do
    // the right thing with simplex/facet locks.

    // Mark all simplex orientations as unknown in both sheets.
    for (auto s : ans.simplices_)
        s->orientation_ = 0;

    // Run through the original triangulation and recreate the gluings as we
    // propagate simplex orientations through components.
    //
    // We use a breadth-first search to propagate orientations.
    // The underlying queue is implemented using a plain C array - since each
    // simplex is processed only once, an array of size sheetSize is enough.
    FixedArray<size_t> queue(sheetSize);
    size_t queueStart = 0, queueEnd = 0;

    for (size_t i = 0; i < sheetSize; i++)
        if (ans.simplices_[i]->orientation_ == 0) {
            // We've found a new component.
            // Completely recreate the gluings for this component.
            ans.simplices_[i]->orientation_ = 1;
            ans.simplices_[i + sheetSize]->orientation_ = -1;
            queue[queueEnd++] = i;

            while (queueStart < queueEnd) {
                size_t pos = queue[queueStart++];
                Simplex<dim>* orig = simplices_[pos];
                Simplex<dim>* lowerSimp = ans.simplices_[pos];
                Simplex<dim>* upperSimp = ans.simplices_[pos + sheetSize];

                for (int facet = 0; facet <= dim; ++facet) {
                    // See if this simplex is glued to something.
                    Simplex<dim>* adj = orig->adjacentSimplex(facet);
                    if (! adj)
                        continue;

                    Simplex<dim>* lowerAdj = ans.simplices_[adj->index()];
                    Simplex<dim>* upperAdj = ans.simplices_[
                        adj->index() + sheetSize];

                    // Determine the expected orientation of the
                    // adjacent simplex in the lower sheet.
                    Perm<dim + 1> gluing = orig->adjacentGluing(facet);
                    lowerSimp->gluing_[facet] = gluing;
                    upperSimp->gluing_[facet] = gluing;

                    int lowerAdjOrientation = (gluing.sign() == 1 ?
                        -lowerSimp->orientation_ : lowerSimp->orientation_);

                    if (lowerAdj->orientation_ == 0) {
                        // We haven't seen the adjacent simplex yet.
                        // Stay within the same sheet.
                        lowerAdj->orientation_ = lowerAdjOrientation;
                        upperAdj->orientation_ = -lowerAdjOrientation;
                        lowerSimp->adj_[facet] = lowerAdj;
                        upperSimp->adj_[facet] = upperAdj;
                        queue[queueEnd++] = lowerAdj->index();
                    } else if (lowerAdj->orientation_ == lowerAdjOrientation) {
                        // The adjacent simplex already has the correct
                        // orientation.  Stay within the same sheet.
                        lowerSimp->adj_[facet] = lowerAdj;
                        upperSimp->adj_[facet] = upperAdj;
                    } else {
                        // The adjacent simplex already has the incorrect
                        // orientation.  Make a cross between the two sheets.
                        lowerSimp->adj_[facet] = upperAdj;
                        upperSimp->adj_[facet] = lowerAdj;
                    }
                }
            }
        }

    return ans;
}

template <int dim>
Triangulation<dim> TriangulationBase<dim>::doubleOverBoundary() const {
    size_t origSize = simplices_.size();
    if (origSize == 0)
        return {};

    Triangulation<dim> ans;

    // Create two blocks of simplices, cloning simplex descriptions and locks.
    for (size_t i = 0; i < origSize; i++)
        ans.simplices_.push_back(new Simplex<dim>(*simplices_[i],
            std::addressof(ans)));
    for (size_t i = 0; i < origSize; i++)
        ans.simplices_.push_back(new Simplex<dim>(*simplices_[i],
            std::addressof(ans)));

    // Recreate the gluings.
    auto src = simplices_.begin();
    auto dest0 = ans.simplices_.begin();
    auto dest1 = dest0 + origSize;
    size_t idx = 0;
    for ( ; src != simplices_.end(); ++src, ++dest0, ++dest1, ++idx) {
        for (int f = 0; f <= dim; ++f) {
            if (auto adj = (*src)->adjacentSimplex(f)) {
                // We have an internal facet.  Mirror the gluing in each block.
                Perm<dim+1> gluing = (*src)->adjacentGluing(f);
                (*dest0)->adj_[f] = ans.simplices_[adj->index()];
                (*dest0)->gluing_[f] = gluing;
                (*dest1)->adj_[f] = ans.simplices_[adj->index() + origSize];
                (*dest1)->gluing_[f] = gluing;
            } else {
                // We have a boundary facet.  Connect the two blocks.
                (*dest0)->adj_[f] = *dest1;
                (*dest1)->adj_[f] = *dest0;
                (*dest0)->gluing_[f] = (*dest1)->gluing_[f] = Perm<dim+1>();
            }
        }
    }

    return ans;
}

template <int dim>
void TriangulationBase<dim>::subdivide() {
    size_t nOld = simplices_.size();
    if (nOld == 0)
        return;

    static_assert(standardDim(dim),
        "subdivide() may only be used in standard dimensions.");

    // Any simplex or facet locks at all will be a problem here.
    if (hasLocks())
        throw LockViolation("An attempt was made to subdivide a "
            "triangulation with one or more locked simplices or facets");

    // Since staging is new here, we can use the "raw" simplex routines
    // that do not generate change events / snapshots, check locks, etc.
    Triangulation<dim> staging;

    auto* newSimp = new Simplex<dim>*[nOld * Perm<dim+1>::nPerms];

    // A top-dimensional simplex in the subdivision is uniquely defined
    // by a permutation p on (dim+1) elements.
    //
    // As described in the documentation for subdivide(),
    // this is the simplex that:
    // - meets the boundary in the facet opposite vertex p[dim];
    // - meets that facet in the (dim-2)-face opposite vertex p[dim-1];
    // - meets that (dim-2)-face in the (dim-3)-face opposite vertex p[dim-2];
    // - ...
    // - meets that edge in the vertex opposite vertex p[1];
    // - directly touches vertex p[0].

    size_t simp;
    for (simp = 0; simp < Perm<dim+1>::nPerms * nOld; ++simp)
        newSimp[simp] = staging.newSimplexRaw();

    // Do all of the internal gluings
    typename Perm<dim+1>::Index permIdx, adjIdx;
    Perm<dim+1> perm, glue;
    int i;
    for (simp=0; simp < nOld; ++simp)
        for (permIdx = 0; permIdx < Perm<dim+1>::nPerms; ++permIdx) {
            perm = Perm<dim+1>::orderedSn[permIdx];

            // Internal gluings within the old simplex:
            for (i = 0; i < dim; ++i) {
                adjIdx = (perm * Perm<dim+1>(i, i+1)).orderedSnIndex();
                if (permIdx < adjIdx)
                    newSimp[Perm<dim+1>::nPerms * simp + permIdx]->joinRaw(
                        perm[i],
                        newSimp[Perm<dim+1>::nPerms * simp + adjIdx],
                        Perm<dim+1>(perm[i], perm[i+1]));
            }

            // Adjacent gluings to the adjacent simplex:
            Simplex<dim>* oldSimp = simplex(simp);
            if (! oldSimp->adjacentSimplex(perm[dim]))
                continue; // This hits a boundary facet.
            if (newSimp[Perm<dim+1>::nPerms * simp + permIdx]->adjacentSimplex(
                    perm[dim]))
                continue; // We've already done this gluing from the other side.

            glue = oldSimp->adjacentGluing(perm[dim]);
            newSimp[Perm<dim+1>::nPerms * simp + permIdx]->joinRaw(perm[dim],
                newSimp[Perm<dim+1>::nPerms * oldSimp->adjacentSimplex(
                    perm[dim])->index() + (glue * perm).orderedSnIndex()],
                glue);
        }

    // Delete the existing simplices and put in the new ones.
    // The change event and snapshot will be fired here, and computed
    // properties will be cleared, all during swap().

    // TODO: If the skeleton has been calculated and we know the
    // triangulation to be valid, then preserve vertex link properties.
    static_cast<Triangulation<dim>*>(this)->swap(staging);
    delete[] newSimp;
}

template <int dim>
bool TriangulationBase<dim>::makeIdeal() {
    if (! hasBoundaryFacets())
        return false;

    // Make a list of all boundary facets, indexed by (dim-1)-face number,
    // and create the corresponding new simplices.
    // We put these new simplices in a new "staging" triangulation for
    // the time being, since we will still need to iterate through
    // (dim-2)-faces of the original triangulation.

    size_t nFaces = countFaces<dim - 1>();

    FixedArray<Simplex<dim>*> bdry(nFaces);
    FixedArray<Perm<dim + 1>> bdryPerm(nFaces);
    FixedArray<Simplex<dim>*> cone(nFaces);

    // Since staging is new here, we can use the "raw" simplex routines
    // that do not generate change events / snapshots, check locks, etc.
    Triangulation<dim> staging;

    for (Face<dim, dim - 1>* f : faces<dim - 1>()) {
        if (f->degree() > 1) {
            // Not a boundary facet.
            bdry[f->index()] = cone[f->index()] = nullptr;
            continue;
        }

        if (f->isLocked())
            throw LockViolation("An attempt was made to change the boundary "
                "of a triangulation with one or more locked boundary facets");

        bdry[f->index()] = f->front().simplex();
        bdryPerm[f->index()] = f->front().vertices();
        cone[f->index()] = staging.newSimplexRaw();
    }

    // Glue the new simplices to each other.
    Face<dim, dim - 1> *facet1, *facet2;
    Perm<dim + 1> f1Perm, f2Perm;
    for (auto ridge : faces<dim - 2>()) {
        // Is this (dim-2)-face on a real boundary component?
        // Look for the boundary facets at either end.
        const FaceEmbedding<dim, dim - 2>& e1 = ridge->front();
        facet1 = e1.simplex()->template face<dim - 1>(e1.vertices()[dim]);
        if (facet1->degree() > 1)
            continue;

        // Yes!  We're on a real boundary component.
        const FaceEmbedding<dim, dim - 2>& e2 = ridge->back();
        facet2 = e2.simplex()->template face<dim - 1>(e2.vertices()[dim - 1]);

        f1Perm = bdryPerm[facet1->index()].inverse() * e1.vertices();
        f2Perm = bdryPerm[facet2->index()].inverse() * e2.vertices() *
            Perm<dim + 1>(dim - 1, dim);

        cone[facet1->index()]->joinRaw(f1Perm[dim - 1],
            cone[facet2->index()], f2Perm * f1Perm.inverse());
    }

    // Now join the new simplices to the boundary facets of the original
    // triangulation.  This will be where change events, snapshots, etc. are
    // fired and properties are cleared.  From here on we need to stop
    // using joinRaw(), and let join() do all of its extra management.

    PacketChangeGroup span(static_cast<Triangulation<dim>&>(*this));

    insertTriangulation(std::move(staging));

    for (size_t i = 0; i < nFaces; ++i)
        if (cone[i])
            cone[i]->join(dim, bdry[i], bdryPerm[i]);

    return true;
}

template <int dim>
std::string TriangulationBase<dim>::source(Language language) const {
    std::ostringstream ans;

    // Does Perm<dim+1> have a constructor that takes (dim+1) integers?
    static constexpr bool hasSimplePermConstructor = (dim <= 6);

    switch (language) {
        case Language::Cxx:
            ans << "Triangulation<" << dim << "> tri = Triangulation<" << dim
                << ">::fromGluings(" << size() << ", {\n";
            break;
        case Language::Python:
            ans << "tri = Triangulation" << dim << ".fromGluings("
                << size() << ", [\n";
            break;
    }

    size_t wrote = 0;
    for (size_t i = 0; i < size(); ++i) {
        Simplex<dim>* s = simplices_[i];
        for (int j = 0; j <= dim; ++j) {
            Simplex<dim>* adj = s->adjacentSimplex(j);
            if (adj) {
                Perm<dim + 1> g = s->adjacentGluing(j);
                if (adj->index() > i || (adj->index() == i && g[j] > j)) {
                    if (wrote == 0)
                        ans << "    ";
                    else if (wrote % 2 == 0)
                        ans << ",\n    ";
                    else
                        ans << ", ";

                    switch (language) {
                        case Language::Cxx:
                            ans << "{ " << i << ", " << j << ", "
                                << adj->index() << ", {";
                            break;
                        case Language::Python:
                            ans << "[ " << i << ", " << j << ", "
                                << adj->index() << ", Perm" << (dim+1) << '(';
                            if constexpr (! hasSimplePermConstructor)
                                ans << '[';
                            break;
                    }
                    for (int k = 0; k <= dim; ++k) {
                        if (k > 0)
                            ans << ',';
                        ans << g[k];
                    }
                    switch (language) {
                        case Language::Cxx:
                            ans << "} }";
                            break;
                        case Language::Python:
                            if constexpr (! hasSimplePermConstructor)
                                ans << ']';
                            ans << ") ]";
                            break;
                    }

                    ++wrote;
                }
            }
        }
    }
    switch (language) {
        case Language::Cxx:    ans << "});\n"; break;
        case Language::Python: ans << "])\n";  break;
    }
    return ans.str();
}

template <int dim>
void TriangulationBase<dim>::writeDot(std::ostream& out, bool labels) const {
    // For a full visual list of named colours, see:
    // https://graphviz.org/doc/info/colors.html

    out << "graph tri {" << std::endl;
    out << "edge [color=gray25];" << std::endl;
    out << R"(node [shape=circle,style=filled,height=0.15,fixedsize=true,label="",fontsize=9,fontcolor="#751010"];)" << std::endl;

    // Ancient versions of graphviz seem to ignore the default label="".
    // Make this explicit for each node.
    for (size_t p = 0; p < size(); ++p) {
        out << "s_" << p << " [";
        if (simplices_[p]->isLocked())
            out << "color=darkgoldenrod4,fontcolor=tan4,fillcolor=lightgoldenrod,";
        out << "label=\"";
        if (labels)
            out << p;
        out << "\"]" << std::endl;
    }

    for (size_t p = 0; p < size(); ++p) {
        Simplex<dim>* s = simplices_[p];
        for (int f = 0; f <= dim; ++f) {
            Simplex<dim>* adj = s->adjacentSimplex(f);
            if (! adj)
                continue;
            if (adj->index() < p ||
                    (adj->index() == p && s->adjacentFacet(f) < f))
                continue;
            out << "s_" << p << " -- s_" << adj->index();
            if (s->isFacetLocked(f))
                out << " [color=darkgoldenrod]";
            out << ';' << std::endl;
        }
    }

    out << '}' << std::endl;
}

} // namespace regina::detail

#endif
