
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
            else
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
void TriangulationBase<dim>::makeDoubleCover() {
    size_t sheetSize = simplices_.size();
    if (sheetSize == 0)
        return;

    // Manage change events and snapshots manually, since we will be managing
    // simplices and gluings manually also.  The reason for this manual work
    // is so that we can do the right thing with simplex/facet locks.
    Snapshottable<Triangulation<dim>>::takeSnapshot();
    ChangeEventSpan span(static_cast<Triangulation<dim>&>(*this));

    // Clear all properties now, so that the skeleton is deleted.
    static_cast<Triangulation<dim>*>(this)->clearAllProperties();

    // Create a second sheet of simplices.
    auto* upper = new Simplex<dim>*[sheetSize];
    for (size_t i = 0; i < sheetSize; i++) {
        upper[i] = new Simplex<dim>(*simplices_[i],
            static_cast<Triangulation<dim>*>(this));
        simplices_.push_back(upper[i]);
    }

    // Reset each simplex orientation.
    auto sit = simplices_.begin();
    for (size_t i = 0; i < sheetSize; i++) {
        (*sit++)->orientation_ = 0;
        upper[i]->orientation_ = 0;
    }

    // Run through the upper sheet and recreate the gluings as we
    // propagate simplex orientations through components.
    //
    // We use a breadth-first search to propagate orientations.
    // The underlying queue is implemented using a plain C array - since each
    // simplex is processed only once, an array of size sheetSize is enough.
    //
    // We will ignore the requirement that the lowest-index simplex in each
    // component must have orientation +1: this is because our new orientations
    // are temporary only.  (We called clearAllProperties() above, which will
    // force a full recomputation of the skeleton when next required.)
    auto* queue = new size_t[sheetSize];
    size_t queueStart = 0, queueEnd = 0;

    for (size_t i = 0; i < sheetSize; i++)
        if (upper[i]->orientation_ == 0) {
            // We've found a new component.
            // Completely recreate the gluings for this component.
            upper[i]->orientation_ = 1;
            simplices_[i]->orientation_ = -1;
            queue[queueEnd++] = i;

            while (queueStart < queueEnd) {
                size_t upperSimp = queue[queueStart++];
                Simplex<dim>* lowerSimp = simplices_[upperSimp];

                for (int facet = 0; facet <= dim; ++facet) {
                    Simplex<dim>* lowerAdj = lowerSimp->adjacentSimplex(facet);

                    // See if this simplex is glued to something in the
                    // lower sheet.
                    if (! lowerAdj)
                        continue;

                    // Determine the expected orientation of the
                    // adjacent simplex in the lower sheet.
                    Perm<dim + 1> gluing = lowerSimp->adjacentGluing(facet);
                    int lowerAdjOrientation = (gluing.sign() == 1 ?
                        -lowerSimp->orientation_ : lowerSimp->orientation_);

                    size_t upperAdj = lowerAdj->index();
                    if (lowerAdj->orientation_ == 0) {
                        // We haven't seen the adjacent simplex yet.
                        lowerAdj->orientation_ = lowerAdjOrientation;
                        upper[upperAdj]->orientation_ = -lowerAdjOrientation;
                        upper[upperSimp]->adj_[facet] = upper[upperAdj];
                        upper[upperSimp]->gluing_[facet] = gluing;
                        queue[queueEnd++] = upperAdj;
                    } else if (lowerAdj->orientation_ == lowerAdjOrientation) {
                        // The adjacent simplex already has the
                        // correct orientation.
                        upper[upperSimp]->adj_[facet] = upper[upperAdj];
                        upper[upperSimp]->gluing_[facet] = gluing;
                    } else {
                        // The adjacent simplex already has the
                        // incorrect orientation.  Make a cross between
                        // the two sheets.
                        lowerSimp->adj_[facet] = upper[upperAdj];
                        lowerSimp->gluing_[facet] = gluing;
                        upper[upperSimp]->adj_[facet] = lowerAdj;
                        upper[upperSimp]->gluing_[facet] = gluing;
                    }
                }
            }
        }

    // Tidy up.
    delete[] upper;
    delete[] queue;
}

template <int dim>
void TriangulationBase<dim>::subdivide() {
    size_t nOld = simplices_.size();
    if (nOld == 0)
        return;

    Triangulation<dim> staging;
    // Ensure only one event pair is fired in this sequence of changes.
    ChangeEventSpan span(staging);

    static_assert(standardDim(dim),
        "subdivide() may only be used in standard dimensions.");

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
        newSimp[simp] = staging.newSimplex();

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
                    newSimp[Perm<dim+1>::nPerms * simp + permIdx]->join(perm[i],
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
            newSimp[Perm<dim+1>::nPerms * simp + permIdx]->join(perm[dim],
                newSimp[Perm<dim+1>::nPerms * oldSimp->adjacentSimplex(
                    perm[dim])->index() + (glue * perm).orderedSnIndex()],
                glue);
        }

    // Delete the existing simplices and put in the new ones.
    // TODO: If the skeleton has been calculated and we know the
    // triangulation to be valid, then preserve vertex link properties.
    static_cast<Triangulation<dim>*>(this)->swap(staging);
    delete[] newSimp;
}

template <int dim>
bool TriangulationBase<dim>::finiteToIdeal() {
    if (! hasBoundaryFacets())
        return false;

    // Make a list of all boundary facets, indexed by (dim-1)-face number,
    // and create the corresponding new simplices.
    // We put these new simplices in a new "staging" triangulation for
    // the time being, since we will still need to iterate through
    // (dim-2)-faces of the original triangulation.

    size_t nFaces = countFaces<dim - 1>();

    auto* bdry = new Simplex<dim>*[nFaces];
    auto* bdryPerm = new Perm<dim + 1>[nFaces];
    auto* cone = new Simplex<dim>*[nFaces];

    Triangulation<dim> staging;
    // Ensure only one event pair is fired in this sequence of changes.
    ChangeEventSpan span1(staging);

    for (Face<dim, dim - 1>* f : faces<dim - 1>()) {
        if (f->degree() > 1) {
            // Not a boundary facet.
            bdry[f->index()] = cone[f->index()] = nullptr;
            continue;
        }

        bdry[f->index()] = f->front().simplex();
        bdryPerm[f->index()] = f->front().vertices();
        cone[f->index()] = staging.newSimplex();
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

        cone[facet1->index()]->join(f1Perm[dim - 1],
            cone[facet2->index()], f2Perm * f1Perm.inverse());
    }

    // Now join the new simplices to the boundary facets of the original
    // triangulation.
    // Again, ensure only one event pair is fired in this sequence of changes.
    ChangeEventSpan span2(static_cast<Triangulation<dim>&>(*this));

    staging.moveContentsTo(static_cast<Triangulation<dim>&>(*this));

    for (size_t i = 0; i < nFaces; ++i)
        if (cone[i])
            cone[i]->join(dim, bdry[i], bdryPerm[i]);

    // Clean up and return.
    delete[] cone;
    delete[] bdryPerm;
    delete[] bdry;
    return true;
}

} // namespace regina::detail

#endif
